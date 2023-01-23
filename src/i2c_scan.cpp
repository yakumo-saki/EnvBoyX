#include <Arduino.h>
#include <Wire.h>

#include "log.h"

String get_device_name_by_address(byte address) {
  // ("0x39=TSL2561/0x3c=LCD/0x5c=AM2320/0x5d=LPS22HB/0x76=BME280"));
  if (address == 0x23) return "BH1750 Lux sensor";
  if (address == 0x39) return "TSL2561 Lux sensor";
  if (address == 0x3c) return "SSD1306/SH1106 OLED display";
  if (address == 0x5c) return "AM2320 Temperature and Humidity sensor";
  if (address == 0x5d) return "LPS22HB High precision air pressure sensor";
  if (address == 0x76)
    return "BME280 Temperature and Humidity and air pressure sensor";
  return "unknown device";
}

void i2c_scan() {
  unsigned long start = millis();

  byte error, address;
  int nDevices = 0;
  for (address = 1; address < 127; address++) {
    Wire.beginTransmission(address);
    error = Wire.endTransmission();

    if (error == 0) {
      String msg = "Found at 0x";
      if (address < 16) msg += "0";
      msg += String(address, HEX);
      msg += " " + String(address);
      i2clog(msg + " " + get_device_name_by_address(address));
      nDevices++;
    } else if (error == 4) {
      String msg = "Unknown error at address 0x";
      if (address < 16) msg += "0";
      msg += String(address, HEX);
      i2clog(msg);
    }
  }

  if (nDevices == 0) {
    i2clog(F("No I2C devices found"));
  }

  i2clog("I2C scan done. time = " + String(millis() - start) + " ms");
}
