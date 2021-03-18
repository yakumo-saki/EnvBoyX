#pragma once

#include <Arduino.h>

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

typedef struct {
  String low;
  String high;
} config_alert_range_t;

typedef struct { 
  config_alert_range_t caution1;
  config_alert_range_t caution2;
  config_alert_range_t warning1;
  config_alert_range_t warning2;
} config_alert_t;


/**
 * Config 構造体
 * 項目追加時の実装項目：
 * 1. Struct（ここ）
 * 2. config default値    set_default_config_value
 * 3. Setup HTML          http_setup_web_get.cpp http_setup_get_root_content
 * 4. Setup POST後の表示   http_setup_web_post.cpp http_setup_post_root_content
 * 5. Web API             http_api_backup_config
 * 6. Web API             updateConfig
 * 6. Print config        print_config
 * 7. 実際の実装
 */ 
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
  String mhz19bABC;
  
  String displayFlip;
  String displayBrightness;
  String displaySkipReconfigure;

  String st7789;
  String st7789Mode;
  String mqttBroker;
  String mqttName;
  String oledType;

  // 以下は個別設定なのでここでは初期値を入れない
  config_alert_t temperatureAlerts;
  config_alert_t humidityAlerts;
  config_alert_t pressureAlerts;
  config_alert_t luxAlerts;
  config_alert_t co2Alerts;
} config_t;

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
