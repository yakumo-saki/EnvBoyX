#include <Arduino.h>
#include <Wire.h>

#include "log.h"

void i2c_scan()
{

  unsigned long start = millis();

  byte error, address;
  int nDevices = 0;
  for(address = 1; address < 127; address++ )
  {

    Wire.beginTransmission(address);
    error = Wire.endTransmission();
 
    if (error == 0)
    {
      String msg = "I2C device found at address 0x";
      if (address<16)
        msg += "0";
      msg += String(address,HEX);
      msg += " " + String(address);
      i2clog(msg);
      nDevices++;
    }
    else if (error==4)
    {
      String msg = "Unknown error at address 0x";
      if (address<16)
        msg += "0";
      msg += String(address,HEX);
      i2clog(msg);
    }
  }

  if (nDevices == 0) {
    i2clog(F("No I2C devices found"));
  } else {
    i2clog(F("0x39=TSL2561/0x3c=LCD/0x5c=AM2320/0x5d=LPS22HB/0x76=BME280"));
  }

  i2clog("I2C scan done. time = " + String(millis() - start) + " ms");

}
