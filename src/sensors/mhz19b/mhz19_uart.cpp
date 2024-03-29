#include <Arduino.h>
#include <MHZ19.h>

#include "log.h"
#include "global.h"
#include "utils.h"

#include "config/config.h"
#include "ConfigClass.h"

#include "sensors/mhz19_main.h"
#include "sensors/mhz19_util.h"

const int MHZ_RESULT_OK = 1;

// 400ppmの校正(ABC)を行う。これをするには、20分以上外気に晒し続ける必要がある。
// 終了後は false に戻す。
bool autoBaselineCorrection = false;

const int MHZ_BAUDRATE = 9600;

// MH-Z19B の測定レンジ。 2000 or 5000. これ以上の値のco2ppmはエラー扱い
const int MHZ_RANGE = 2000;

#ifdef ESP32
  HardwareSerial mhzSerial(2); // use UART2
#elif defined(ESP8266)
#include <SoftwareSerial.h>
SoftwareSerial mhzSerial(14, 0);
#endif

MHZ19 mhz19;

void printErrorCode() {
  if (mhz19.errorCode != 1) {
    mhzlog(mhz19_code_to_msg(mhz19.errorCode));
  }
}

void mhz_setup_check_device_uart() {

  char myVersion[4];          
  mhz19.getVersion(myVersion);

  String version = "";
  for(byte i = 0; i < 4; i++)
  {
    version = version + String(myVersion[i]);
    if(i == 1)
      version = version + ".";    
  }  

  mhzlog(F("Device Info ======================== "));
  mhzlog("Version        : " + version);
  mhzlog("Range          : " + String(mhz19.getRange()));
  mhzlog("Background CO2 : " + String(mhz19.getBackgroundCO2()));
  mhzlog("Temperature Cal: " + String(mhz19.getTempAdjustment()));
  mhzlog("ABC Status     : " + String(mhz19.getABC()) + " " + String(mhz19.getABC() ? "ON" : "OFF"));
}

bool mhz_setup_uart() {

  sensorValues.co2ppm = CO2_PPM_INVALID;

  mhzlog(F("Enabled (UART mode)."));

#ifdef ARDUINO_ARCH_ESP32
  mhzlog("ESP32 serial begin RX=" + String(config->get(ConfigNames::MHZ19B_RX).toInt()) + " TX=" + String(config->get(ConfigNames::MHZ19B_TX).toInt()));
  mhzSerial.begin(MHZ_BAUDRATE, SERIAL_8N1, config->get(ConfigNames::MHZ19B_RX).toInt(), config->get(ConfigNames::MHZ19B_TX).toInt());
#elif defined(ARDUINO_ARCH_ESP8266) 
  mhzSerial.begin(MHZ_BAUDRATE);
#endif

  mhzlog(F("Wait for MHZ UART serial"));
  while(!mhzSerial);

  mhz19.begin(mhzSerial);

  mhz19.setRange(MHZ_RANGE);
  printErrorCode();

  mhzlog("setAutoCalibration = " + String(parseBooleanString(config->get(ConfigNames::MHZ19B_ABC))));
  mhz19.autoCalibration(parseBooleanString(config->get(ConfigNames::MHZ19B_ABC)));
  printErrorCode();

  mhz_setup_check_device_uart();

  mhzlog(F("initialized."));
  return true;
}

// For API
bool mhz_do_zero_calibration() {
  if (config->get(ConfigNames::MHZ19B) != ConfigValues::MHZ_USE_UART) return false;
  mhz19.calibrate();
  if (mhz19.errorCode != 1) {
    mhzlog("API: ZeroCalibration: Error " + mhz19_code_to_msg(mhz19.errorCode));
    return false;
  }

  mhzlog(F("API: ZeroCalibration: done"));
  return true;
}

// For API
bool mhz_get_abc() {
  if (config->get(ConfigNames::MHZ19B) != ConfigValues::MHZ_USE_UART) return false;
  return mhz19.getABC();
}

// For API
bool mhz_set_abc(bool onoff) {
  if (config->get(ConfigNames::MHZ19B) != ConfigValues::MHZ_USE_UART) return false;
  mhz19.autoCalibration(onoff);
  if (mhz19.errorCode != 1) {
    mhzlog("API: " + mhz19_code_to_msg(mhz19.errorCode));
    return false;
  }

  if (onoff) {
    mhzlog(F("API: ABC is now ON"));
  } else {
    mhzlog(F("API: ABC is now OFF"));
  }

  return true;
}

void mhz_read_data_uart() {

  mhz19.verify();
  if (mhz19.errorCode != MHZ_RESULT_OK) {
    printErrorCode();
    mhzlog(F("MH-Z19B connection failed. abort."));
    return;
  }

  int co2ppm = mhz19.getCO2();
  if (mhz19.errorCode != 1) {
    printErrorCode();
  }

  int acc = mhz19.getAccuracy();
  printErrorCode();

#ifdef SENSOR_VALUE_LOG
  int temp = mhz19.getTemperature();
  printErrorCode();
  mhzlog("CO2 (ppm): " + String(co2ppm) + " Accuracy: " + acc + " Temp: " + String(temp));
#endif
  sensorValues.co2ppm = co2ppm;
  sensorValues.co2ppmAccuracy = acc;
}
