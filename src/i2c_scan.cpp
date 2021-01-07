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
    i2clog("No I2C devices found");
  }

  i2clog("i2c scan done. time = " + String(millis() - start) + " ms");

}
