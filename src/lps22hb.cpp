//
// LPS22HB barometric sensor
//
#include <Arduino.h>
#include <Wire.h>

#include <Sodaq_LPS22HB.h>

Sodaq_LPS22HB lps22hb;

extern float lastPressure;

bool use_lps22hb = false;

void lpslog(String msg) {
  Serial.println("LPS22HB: " + msg);
}

void lps_setup(void) {

  // init LPS22HB if found (0x5d)
  if (lps22hb.init()) {
      lpslog("Barometric sensor initialization succeeded!");
      lps22hb.enableSensor(Sodaq_LPS22HB::OdrOneShot);
      use_lps22hb = true;
  } else {
      lpslog("Barometric sensor initialization failed. disable.");
      use_lps22hb = false;
  }
}

void lps_read_data() {
  if (!use_lps22hb) {
    return;
  }
  
  float tempPres(NAN);
  tempPres = lps22hb.readPressureHPA();
  if (tempPres != 0) {
    lastPressure = tempPres;
    
    Serial.print("LPS22HB: ");
    Serial.print("Pressure=");
    Serial.print(tempPres, 2);
    Serial.println("hPa");
  }  
}