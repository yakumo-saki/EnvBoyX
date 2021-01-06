#include <Arduino.h>
#include <Wire.h>
#include "log.h"

bool init_i2c(int sda, int scl) {
    #ifdef ESP32
    i2clog("Initializing ESP32 I2C SDA=" + String(sda) + " SCL=" + String(scl));
    return Wire.begin(sda, scl);
    #endif
    #ifdef ESP8266
    i2clog("Initializing ESP8266 I2C SDA=" + String(sda) + " SCL=" + String(scl));
    Wire.begin(sda, scl);
    return true;
    #endif
}