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
 * 2. Setup HTML          http_setup_web_get.cpp http_setup_get_root_content
 * 3. Setup POST後の表示   http_setup_web_post.cpp http_setup_post_root_content
 * 4. Web API             http_api_backup_config
 * 5. Web API             updateConfig
 * 6. Write Config        _create_config_json
 * 6. Read Config         read_config_file
 * 7. Print config        print_config
 * 8. 実際の実装
 */ 
typedef struct _config_t {
  String settingId;
  String ssid;
  String password;
  String mDNS = "ebx";
  String opMode = ConfigValues::OPMODE_DISPLAY;
  
  String mhz19b = ConfigValues::MHZ_NOUSE;
  String mhz19bPwmPin = "14";
  #ifdef ESP32
  String mhz19bRxPin = "32";
  String mhz19bTxPin = "33";
  #elif defined(ESP8266)
  String mhz19bRxPin = "14";
  String mhz19bTxPin = "0";
  #endif

  String mhz19bABC = ConfigValues::MHZ_ABC_OFF; // v44;
  
  String displayFlip = ConfigValues::DISPLAY_FLIP_OFF;
  String displayBrightness = "255";
  String displayWaitForReconfigure = ConfigValues::DISPLAY_RECONFIG_ON; // v44;

  String oledType = ConfigValues::OLED_SSD1306;

  String st7789 = ConfigValues::ST7789_NOUSE;
  String st7789Mode = ConfigValues::ST7789_MODE_NORMAL;
  String mqttBroker = "";
  String mqttName = "";

  // 以下は個別設定なのでここでは初期値を入れない
  config_alert_t temperatureAlerts;
  config_alert_t humidityAlerts;
  config_alert_t pressureAlerts;
  config_alert_t luxAlerts;
  config_alert_t co2Alerts;

  _config_t() {
    this->temperatureAlerts.warning1.low = "-99";
    this->temperatureAlerts.warning1.high = "10";
    this->temperatureAlerts.caution1.low = "10";
    this->temperatureAlerts.caution1.high = "15";
    this->temperatureAlerts.caution2.low = "28";
    this->temperatureAlerts.caution2.high = "30";
    this->temperatureAlerts.warning2.low = "30";
    this->temperatureAlerts.warning2.high = "99";

    this->humidityAlerts.warning1.low = "0";
    this->humidityAlerts.warning1.high = "20";
    this->humidityAlerts.caution1.low = "20";
    this->humidityAlerts.caution1.high = "35";
    this->humidityAlerts.caution2.low = "65";
    this->humidityAlerts.caution2.high = "75";
    this->humidityAlerts.warning2.low = "75";
    this->humidityAlerts.warning2.high = "100";

    this->pressureAlerts.warning1.low = "0";
    this->pressureAlerts.warning1.high = "995";
    this->pressureAlerts.caution1.low = "995";
    this->pressureAlerts.caution1.high = "1000";
    this->pressureAlerts.caution2.low = "3000";
    this->pressureAlerts.caution2.high = "3000";
    this->pressureAlerts.warning2.low = "1025";
    this->pressureAlerts.warning2.high = "3000";

    this->luxAlerts.warning1.low = "0";
    this->luxAlerts.warning1.high = "1";
    this->luxAlerts.caution1.low = "-1";
    this->luxAlerts.caution1.high = "-1";
    this->luxAlerts.caution2.low = "-1";
    this->luxAlerts.caution2.high = "-1";
    this->luxAlerts.warning2.low = "2000";
    this->luxAlerts.warning2.high = "99999";

    this->co2Alerts.warning1.low = "0";
    this->co2Alerts.warning1.high = "200";
    this->co2Alerts.caution1.low = "200";
    this->co2Alerts.caution1.high = "300";
    this->co2Alerts.caution2.low = "800";
    this->co2Alerts.caution2.high = "1000";
    this->co2Alerts.warning2.low = "1000";
    this->co2Alerts.warning2.high = "9999";
  }

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
