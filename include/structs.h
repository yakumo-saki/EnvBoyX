#ifndef STRUCTS_H
#define STRUCTS_H

#include <Arduino.h>
typedef struct {
    String ip;
    String mDNS;
    String temperature;
    String humidity;
    String pressure;
    String lux;
    String luxIr;
    String co2ppm;
} disp_values_t;
typedef struct {
  String settingId;
  String ssid;
  String password;
  String mDNS;
  String opMode;
  String use_mhz19b;
  String mhz19b_pwmpin;
  String mhz19b_rxpin;
  String mhz19b_txpin;
  String st7789;
  String mqttBroker;
  String mqttName;
} config_t;

#endif