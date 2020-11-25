#include <Arduino.h>
#include <MHZ19.h>

#include "log.h"
#include "global.h"
#include "sensors/mhz19_main.h"
#include "sensors/mhz19_util.h"

// 400ppmの校正(ABC)を行う。これをするには、20分以上外気に晒し続ける必要がある。
// 終了後は false に戻す。
bool AUTO_BASELINE_CORRECTION = false;

unsigned long mhzGetDataTimer = 0;                     

MHZ19 mhz19;

#ifdef ARDUINO_ARCH_ESP32
//include ESP32 specific libs
HardwareSerial mhzSerial(2);                  
#elif defined(ARDUINO_ARCH_ESP8266) 
//include esp8266 specific libs
#include <SoftwareSerial.h>                                // Remove if using HardwareSerial or Arduino package without SoftwareSerial support
#define MHZ_RX_PIN 14     // Rx pin which the MHZ19 Tx pin is attached to
#define MHZ_TX_PIN 0      // Tx pin which the MHZ19 Rx pin is attached to
SoftwareSerial mhzSerial(MHZ_RX_PIN, MHZ_TX_PIN);
#endif

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

  mhzlog("Device Info ======================== ");
  mhzlog("Version        : " + version);
  mhzlog("Range          : " + String(mhz19.getRange()));
  mhzlog("Background CO2 : " + String(mhz19.getBackgroundCO2()));
  mhzlog("Temperature Cal: " + String(mhz19.getTempAdjustment()));
  mhzlog("ABC Status     : " + String(mhz19.getABC()) + " " + String(mhz19.getABC() ? "ON" : "OFF"));
}

void mhz_setup_uart() {

  lastPpm = CO2_PPM_INVALID;

  mhzlog("Enabled (UART mode).");

  mhzlog("begin()");
  mhzSerial.begin(9600);
  mhz19.begin(mhzSerial);

  mhzlog("setRange()");
  mhz19.setRange(5000);
  printErrorCode();

  // mhzlog("setSpan()");
  // mhz19.setSpan(2000);                  
  // printErrorCode();

  mhzlog("setAutoCalibration() " + String(AUTO_BASELINE_CORRECTION));
  mhz19.autoCalibration(AUTO_BASELINE_CORRECTION);
  printErrorCode();

  if (AUTO_BASELINE_CORRECTION) {
    mhzlog("WARNING -------------------------- WARNING");
    mhzlog("     Auto Baseline Correction is ON!");
    mhzlog("WARNING -------------------------- WARNING");
  }

  mhz_setup_check_device_uart();

  mhzlog("initialized.");
 
}

void mhz_read_data_uart() {

  if ( (millis() - mhzGetDataTimer) < 3000) {
    return;
  }

  mhzGetDataTimer = millis();

  String range = String(mhz19.getRange());
  printErrorCode();

  mhz19.verify();
  printErrorCode();

  String acc = String(mhz19.getAccuracy());
  printErrorCode();

  int co2ppm = mhz19.getCO2(false);
  printErrorCode();

  int temp = mhz19.getTemperature();
  printErrorCode();
  
  mhzlog("CO2 (ppm): " + String(co2ppm) + " Accuracy: " + acc + " Temp: " + String(temp) + " CO2 range: "+ range);
  lastPpm = co2ppm;
  lastPpmAccuracy = acc;
}
