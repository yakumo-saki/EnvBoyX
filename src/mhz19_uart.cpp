#include <Arduino.h>

#include <SoftwareSerial.h>
#include <MHZ19.h>

#include "log.h"
#include "global.h"
#include "mhz19_main.h"
#include "mhz19_util.h"

#define MHZ_RX_PIN 14     // Rx pin which the MHZ19 Tx pin is attached to
#define MHZ_TX_PIN 0      // Tx pin which the MHZ19 Rx pin is attached to

// wait for co2 sensor warmup (maybe forever).
bool WAIT_FOR_CO2_WARMUP = false;
bool WAIT_FOR_CO2_WARMUP_FOREVER = false;

// 400ppmの校正(ABC)を行う。これをするには、20分以上外気に晒し続ける必要がある。
// 終了後は false に戻す。
bool AUTO_BASELINE_CORRECTION = false;

unsigned long mhzGetDataTimer = 0;                     

MHZ19 mhz19;

// SoftwareSerial mhzSerial(mhz19b_rxpin.toInt(), mhz19b_txpin.toInt());
SoftwareSerial mhzSerial(MHZ_RX_PIN, MHZ_TX_PIN);

void printErrorCode() {
  mhzlog(mhz19_code_to_msg(mhz19.errorCode));
}

void mhz_setup_check_device_uart() {

  char myVersion[4];          
  mhz19.getVersion(myVersion);

  mhzlog("Device Info ======================== ");
  mhzlog("Version       : " + String(myVersion));
  mhzlog("Range         : " + String(mhz19.getRange()));
  mhzlog("Background CO2: " + String(mhz19.getBackgroundCO2()));
  //  mhzlog("Temperature Cal: " + String(mhz19.getTempAdjustment()));
  mhzlog("ABC Status    : " + String(mhz19.getABC()) + " " + String(mhz19.getABC() ? "ON" : "OFF"));
}

void mhz_setup_uart() {

  lastPpm = CO2_PPM_INVALID;

  mhzlog("Enabled (UART mode). RX=" + mhz19b_rxpin + " TX=" + mhz19b_txpin);
  mhzlog("RX=" + mhz19b_rxpin);
  mhzlog("TX=" + mhz19b_txpin);

  mhzlog("begin()");
  mhzSerial.begin(9600);
  mhz19.begin(mhzSerial);

  mhzlog("setRange()");
  mhz19.setRange(2000);
  printErrorCode();

  mhzlog("setSpan()");
  mhz19.setSpan(2000);                  
  printErrorCode();

  mhzlog("setAutoCalibration()");
  mhz19.autoCalibration(AUTO_BASELINE_CORRECTION);
  printErrorCode();

  if (AUTO_BASELINE_CORRECTION) {
    mhzlog("WARNING -------------------------- WARNING");
    mhzlog("     Auto Baseline Correction is ON!");
    mhzlog("WARNING -------------------------- WARNING");
  }

  // mhz_setup_check_device_uart();

  mhzlog("initialized.");
 
}

void mhz_read_data_uart() {

  if ( (millis() - mhzGetDataTimer) < 3000) {
    return;
  }

  mhzGetDataTimer = millis();

  mhzlog("Range: " + String(mhz19.getRange()));
  printErrorCode();

  mhz19.verify();
  printErrorCode();

  int co2ppm = mhz19.getCO2(false);
  printErrorCode();

  int temp = mhz19.getTemperature();
  printErrorCode();
  
  mhzlog("CO2 (ppm): " + String(co2ppm) + " Temp: " + String(temp) );
  lastPpm = co2ppm;

}
