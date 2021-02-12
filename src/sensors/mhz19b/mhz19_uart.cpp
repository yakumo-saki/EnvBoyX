#include <Arduino.h>
#include <MHZ19.h>

#include "log.h"
#include "global.h"
#include "sensors/mhz19_main.h"
#include "sensors/mhz19_util.h"

const int MHZ_RESULT_OK = 1;

// 400ppmの校正(ABC)を行う。これをするには、20分以上外気に晒し続ける必要がある。
// 終了後は false に戻す。
bool AUTO_BASELINE_CORRECTION = false;

#ifdef ESP32
  HardwareSerial mhzSerial(2); // use UART2
#elif defined(ESP8266)
#include <SoftwareSerial.h>
SoftwareSerial mhzSerial(14, 0);
#endif

unsigned long mhzGetDataTimer = 0;                     

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
  mhzlog("ESP32 serial begin RX=" + String(config.mhz19bRxPin.toInt()) + " TX=" + String(config.mhz19bTxPin.toInt()));
  mhzSerial.begin(MHZ_BAUDRATE, SERIAL_8N1, config.mhz19bRxPin.toInt(), config.mhz19bTxPin.toInt());
#elif defined(ARDUINO_ARCH_ESP8266) 
  mhzSerial.begin(MHZ_BAUDRATE);
#endif

  mhzlog(F("Wait for MHZ UART serial"));
  while(!mhzSerial);

  mhz19.begin(mhzSerial);

  mhz19.setRange(5000);
  printErrorCode();

  // mhzlog("setSpan()");
  // mhz19.setSpan(2000);                  
  // printErrorCode();

  mhzlog("setAutoCalibration() " + String(AUTO_BASELINE_CORRECTION));
  mhz19.autoCalibration(AUTO_BASELINE_CORRECTION);
  printErrorCode();

  if (AUTO_BASELINE_CORRECTION) {
    mhzlog(F("WARNING -------------------------- WARNING"));
    mhzlog(F("     Auto Baseline Correction is ON!"));
    mhzlog(F("WARNING -------------------------- WARNING"));
  }

  mhz_setup_check_device_uart();

  mhzlog(F("initialized."));
  return true;
}

void mhz_read_data_uart() {

  if ( (millis() - mhzGetDataTimer) < 3000) {
    return;
  }

  mhz19.verify();
  if (mhz19.errorCode != MHZ_RESULT_OK) {
    printErrorCode();
    mhzlog(F("MH-Z19B connection failed. abort."));
    return;
  }

  int co2ppm = mhz19.getCO2();
  if (mhz19.errorCode == 1) {
    mhzGetDataTimer = millis();
  } else {
    printErrorCode();
  }

  int temp = mhz19.getTemperature();
  printErrorCode();

  String acc = String(mhz19.getAccuracy());
  printErrorCode();

  mhzlog("CO2 (ppm): " + String(co2ppm) + " Accuracy: " + acc + " Temp: " + String(temp));
  sensorValues.co2ppm = co2ppm;
  sensorValues.co2ppmAccuracy = acc;
}
