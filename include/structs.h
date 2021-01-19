#ifndef STRUCTS_H
#define STRUCTS_H

#include <Arduino.h>

typedef struct {
  float temperature;
  float humidity;
  float pressure;
  float lux;
  float luxIr;
  int co2ppm;
  String co2ppmAccuracy;
} sensor_values_t;

typedef struct {
    String ip;
    String mDNS;
    String temperature;
    String humidity;
    String pressure;
    String lux;
    String luxIr;
    String co2ppm;
    sensor_values_t rawValues;
} disp_values_t;
typedef struct {
  String settingId;
  String ssid;
  String password;
  String mDNS;
  String opMode;
  String mhz19b;
  String mhz19bPwmPin;
  String mhz19bRxPin;
  String mhz19bTxPin;
  String displayFlip;
  String displayBrightness;
  String st7789;
  String st7789Mode;
  String mqttBroker;
  String mqttName;
} config_t;

#endif