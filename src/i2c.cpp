#include <Arduino.h>
#include <Wire.h>

bool init_i2c(int sda, int scl) {
    #ifdef ESP32
    return Wire.begin(sda, scl);
    #endif
    #ifdef ESP8266
    Wire.begin(sda, scl);
    return true;
    #endif
}