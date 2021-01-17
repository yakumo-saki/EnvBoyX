//
// LPS22HB barometric sensor
//
#include <Arduino.h>
#include <Wire.h>

#include <Sodaq_LPS22HB.h>

#include "global.h"
#include "log.h"

Sodaq_LPS22HB lps22hb;

bool use_lps22hb = false;

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
    sensorValues.pressure = tempPres;

    lpslog("Pressure=" + String(tempPres, 2) + "hPa");
  }  
}
