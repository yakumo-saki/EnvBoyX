#include <Arduino.h>

#include <Wire.h>
#include <Tsl2561.h>

Tsl2561 Tsl(Wire);

bool use_tsl = false;

extern float lastLuxFull;
extern float lastLuxIr;

void tsllog(String msg) {
  Serial.println("TSL2561: " + msg);
}

void tsl_setup() {
  // init TSL2561
  Tsl.begin();
  if( !Tsl.available() ) {
    tsllog("No Tsl2561 found. Tsl2561 disabled.");
  }
}

// 
bool read_data_tsl2561() {
  
  uint8_t id;
  uint16_t full, ir;

  if( !Tsl.available() ) {
    return false;
  }

  Tsl.on();

  Tsl.setSensitivity(false, Tsl2561::EXP_402);
  //delay(16);
  //delay(110);
  delay(402 + 10);

  Tsl.id(id);
  Tsl.fullLuminosity(full);
  Tsl.irLuminosity(ir);

  Serial.printf("Tsl2561: at 0x%02x(id=0x%02x) luminosity is %5u (full) and %5u (ir)\n", Tsl.address(), id, full, ir);

  lastLuxFull = full;
  lastLuxIr = ir;

  Tsl.off(); 

  return true;
}
