#include <Arduino.h>

#include <Wire.h>
#include <Tsl2561.h>

#include "global.h"
#include "log.h"

Tsl2561 Tsl(Wire);

bool use_tsl = false;

void tsl_setup() {
  // init TSL2561
  Tsl.begin();
  if( !Tsl.available() ) {
    tsllog("No Tsl2561 found. Tsl2561 disabled.");
  }
}

// 
void read_data_tsl2561() {
  
  uint8_t id;
  uint16_t full, ir;

  if( !Tsl.available() ) {
    return;
  }

  Tsl.on();

  Tsl.setSensitivity(false, Tsl2561::EXP_402);
  //delay(16);
  //delay(110);
  delay(402 + 10);

  Tsl.id(id);
  Tsl.fullLuminosity(full);
  Tsl.irLuminosity(ir);

  char log[80];
  snprintf(log, sizeof log, "Luminosity: %5u lx IR: %5u lx", Tsl.address(), id, full, ir);
  tsllog(String(log));

  sensorValues.lux = full;
  sensorValues.luxIr = ir;

  Tsl.off(); 
}
