#include <Arduino.h>
#include <Wire.h>

#include "global.h"
#include "log.h"

#include <BH1750.h>
BH1750 bh1750(0x23);


const byte BH1750_ADDR = 0x23;
bool use_bh1750 = false;

void bh_setup(void) {

  Wire.beginTransmission(BH1750_ADDR);
  byte error = Wire.endTransmission();

  if (!(error == 0)) {
    amlog("BH1750 disabled. ");
    return;
  }

  if (!bh1750.begin(BH1750::CONTINUOUS_HIGH_RES_MODE)) {
    amlog("BH1750 disabled: Device found but initialize error. ");
    return;
  }

  amlog("BH1750 Enabled");
  use_bh1750 = true;
}

void bh_read_data() {

  if (!use_bh1750) {
    return;
  }
  
  if (bh1750.measurementReady()) {
    float lux = bh1750.readLightLevel();

    bhlog("Light: " + String(lux,0) + " lx");
    sensorValues.lux = lux;
  }
}
