#include <Arduino.h>
#include <Wire.h>

#include "log.h"
#include "i2c_scan.h"

bool init_i2c(int sda, int scl) {
    bool ret = true;
    #ifdef ESP32
    i2clog("Initializing ESP32 I2C SDA=" + String(sda) + " SCL=" + String(scl));
    ret = Wire.begin(sda, scl);
    if (ret) {
        i2clog(F("I2C initialized."));
    } else {
        i2clog(F("error initializing I2C"));
    }
    #endif
    #ifdef ESP8266
    i2clog("Initializing ESP8266 I2C SDA=" + String(sda) + " SCL=" + String(scl));
    Wire.begin(sda, scl); // 8266のbeginは返り値がない
    i2clog(F("I2C initialized."));
    #endif

    i2c_scan();

    return ret;
}

