#pragma once

#include <Arduino.h>
#include "config_values.h"

typedef struct {
  float temperature = 0.0;
  float humidity = 0.0;
  float pressure = 0.0;
  float lux = 0.0;
  float luxIr = 0.0;
  int co2ppm = 0;
  int co2ppmAccuracy = 0;
  int rssi = 0;        // not sensor value. get from wifi
  uint32_t freeHeap;   // not sensor value. get from ESP
} sensor_values_t;

typedef struct {
  float temperature = 0.0;
  float humidity = 0.0;
  float pressure = 0.0;
  float lux = 0.0;
  int co2ppm = 0;
} sensor_delta_values_t;

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
  bool warning = false;
  bool caution = false;
} value_alert_t;

typedef struct {
  value_alert_t temperature;
  value_alert_t humidity;
  value_alert_t pressure;
  value_alert_t lux;
  value_alert_t co2;
} value_alerts_t;

// typedef struct {
//   String low;
//   String high;
// } config_alert_range_t;

// typedef struct { 
//   config_alert_range_t caution1;
//   config_alert_range_t caution2;
//   config_alert_range_t warning1;
//   config_alert_range_t warning2;
// } config_alert_t;

/**
 * センサーごとにどの項目を測定できるかのフラグ
 */
typedef struct {
  bool temperature = false;
  bool humidity = false;
  bool pressure = false;
  bool lux = false;
  bool luxIr = false;
  bool co2ppm = false;
  bool co2ppmAccuracy = false;
  // int rssi; センサーじゃないので入れない
} sensor_characters_t;
