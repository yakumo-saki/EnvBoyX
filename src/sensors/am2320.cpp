#include <Arduino.h>
#include <Wire.h>

#include "global.h"
#include "log.h"

#include <AM232X.h>

AM232X AM2320;

const uint8_t AM2320_ADDR = 0x5C;
bool use_am2320 = false;

bool am_setup(void) {

  AM2320.begin(I2C_SDA, I2C_SCL);
  
  if (!AM2320.isConnected()) {
    amlog(F("AM2320 disabled. "));
    return false;
  }

  amlog(F("AM2320 Enabled"));
  use_am2320 = true;
  return true;
}

void am_read_data() {

  if (!use_am2320) {
    return;
  }
  
  int status = AM2320.read();
  switch (status)
  {
  case AM232X_OK:
    sensorValues.temperature = AM2320.getTemperature();
    sensorValues.humidity = AM2320.getHumidity();
    break;
  default:
    amlog("Error: " + String(status));
    break;
  }

}
