#include <Arduino.h>

#include <SoftwareSerial.h>
#include <MHZ19.h>

#include "log.h"
#include "global.h"
#include "mhz19_main.h"
#include "mhz19_util.h"

// wait for co2 sensor warmup (maybe forever).
bool WAIT_FOR_CO2_WARMUP = false;
bool WAIT_FOR_CO2_WARMUP_FOREVER = false;

// 400ppmの校正(ABC)を行う。これをするには、20分以上外気に晒し続ける必要がある。
// 終了後は false に戻す。
bool AUTO_BASELINE_CORRECTION = false;

unsigned long mhzGetDataTimer = 0;                     

MHZ19 mhz19;
SoftwareSerial mhzSerial(mhz19b_rxpin.toInt(), mhz19b_txpin.toInt());

void printErrorCode() {
  mhzlog(mhz19_code_to_msg(mhz19.errorCode));
}

void mhz_setup_check_device_uart() {

  char myVersion[4];          
  mhz19.getVersion(myVersion);

  // mhzlog("\nFirmware Version: " + String(myVersion));
  // for(byte i = 0; i < 4; i++)
  // {
  //   Serial.print(myVersion[i]);
  //   if(i == 1)
  //     Serial.print(".");    
  // }

   mhzlog("Range: " + String(mhz19.getRange()));
   mhzlog("Background CO2: " + String(mhz19.getBackgroundCO2()));
  //  mhzlog("Temperature Cal: " + String(mhz19.getTempAdjustment()));
   mhzlog("ABC Status: " + String(mhz19.getABC() ? Serial.println("ON") :  Serial.println("OFF")));
}

void mhz_setup_uart() {

  lastPpm = CO2_PPM_INVALID;

  mhzlog("Enabled (UART mode).");

  mhzSerial.begin(MHZ_BAUDRATE);
  mhz19.begin(mhzSerial);

  mhz19.autoCalibration(AUTO_BASELINE_CORRECTION);
  if (AUTO_BASELINE_CORRECTION) {
    mhzlog("WARNING -------------------------- WARNING");
    mhzlog("     Auto Baseline Correction is ON!");
    mhzlog("WARNING -------------------------- WARNING");
  }

  if (mhz19.errorCode == RESULT_OK)
        mhz19.calibrateZero();                            // Calibrate
    else
      printErrorCode();

  if (mhz19.errorCode == RESULT_OK)
      mhz19.setSpan(2000);                               // Set Span 2000
  else
      printErrorCode();

  if (mhz19.errorCode == RESULT_OK)
      mhz19.autoCalibration(false);                       // Turn auto calibration OFF
  else
      printErrorCode();

  mhz_setup_check_device_uart();

  mhzlog("initialized.");
 
}

void mhz_read_data_uart() {

  if ( (millis() - mhzGetDataTimer) < 3000) {
    return;
  }

  mhzGetDataTimer = millis();
  int co2ppm = mhz19.getCO2();
  int temp = mhz19.getTemperature();
  
  mhzlog("CO2 (ppm): " + String(co2ppm) + " Temp: " + String(temp) );
  lastPpm = co2ppm;

}
