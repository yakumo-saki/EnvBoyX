#include <Arduino.h>
#include "FS.h"

#include <ArduinoJson.h>

#include "log.h"
#include "global.h"

#define CONF_JSON_SIZE 1400

/**
 * とりあえずのデフォルト値をグローバル変数にセットする
 */
void set_default_config_value()
{
  config.ssid = "";
  config.password = "";
  config.mDNS = "ebx";
  config.opMode = OPMODE_DISPLAY;
  config.mhz19b = MHZ_NOUSE;
  config.mhz19bPwmPin = "14";
  #ifdef ESP32
  config.mhz19bRxPin = "32";
  config.mhz19bTxPin = "33";
  #elif defined(ESP8266)
  config.mhz19bRxPin = "14";
  config.mhz19bTxPin = "0";
  #endif

  config.displayFlip = DISPLAY_FLIP_OFF;
  config.displayBrightness = "255";

  config.st7789 = ST7789_NOUSE;
  config.st7789Mode = ST7789_MODE_NORMAL;

  config.mqttBroker = "";
  config.mqttName = "";

  config.temperatureAlerts.warning1.low = "-99";
  config.temperatureAlerts.warning1.high = "10";
  config.temperatureAlerts.caution1.low = "10";
  config.temperatureAlerts.caution1.high = "15";
  config.temperatureAlerts.caution2.low = "250";
  config.temperatureAlerts.caution2.high = "30";
  config.temperatureAlerts.warning2.low = "30";
  config.temperatureAlerts.warning2.high = "99";

  config.humidityAlerts.warning1.low = "0";
  config.humidityAlerts.warning1.high = "20";
  config.humidityAlerts.caution1.low = "20";
  config.humidityAlerts.caution1.high = "35";
  config.humidityAlerts.caution2.low = "65";
  config.humidityAlerts.caution2.high = "75";
  config.humidityAlerts.warning2.low = "75";
  config.humidityAlerts.warning2.high = "100";

  config.pressureAlerts.warning1.low = "0";
  config.pressureAlerts.warning1.high = "995";
  config.pressureAlerts.caution1.low = "995";
  config.pressureAlerts.caution1.high = "1000";
  config.pressureAlerts.caution2.low = "3000";
  config.pressureAlerts.caution2.high = "3000";
  config.pressureAlerts.warning2.low = "1025";
  config.pressureAlerts.warning2.high = "3000";

  config.luxAlerts.warning1.low = "0";
  config.luxAlerts.warning1.high = "1";
  config.luxAlerts.caution1.low = "-1";
  config.luxAlerts.caution1.high = "-1";
  config.luxAlerts.caution2.low = "-1";
  config.luxAlerts.caution2.high = "-1";
  config.luxAlerts.warning2.low = "2000";
  config.luxAlerts.warning2.high = "99999";

  config.co2Alerts.warning1.low = "0";
  config.co2Alerts.warning1.high = "390";
  config.co2Alerts.caution1.low = "390";
  config.co2Alerts.caution1.high = "398";
  config.co2Alerts.caution2.low = "800";
  config.co2Alerts.caution2.high = "1000";
  config.co2Alerts.warning2.low = "1000";
  config.co2Alerts.warning2.high = "9999";
}

String alerts_to_log_string(const config_alert_t& alerts) {
  String log = "";
  cfglog("    WARNING1 '" + alerts.warning1.low + "' ~ '" + alerts.warning1.high + "'");
  cfglog("    CAUTION1 '" + alerts.caution1.low + "' ~ '" + alerts.caution1.high + "'");
  cfglog("    CAUTION2 '" + alerts.caution2.low + "' ~ '" + alerts.caution2.high + "'");
  cfglog("    WARNING2 '" + alerts.warning2.low + "' ~ '" + alerts.warning2.high + "'");

  return log;
}

void print_config() {
  cfglog("SSID: " + config.ssid);
  cfglog("PASS: " + config.password);
  cfglog("mDNS: " + config.mDNS);
  cfglog("opMode: " + config.opMode);
  cfglog(F("DISPLAY:"));
  cfglog("   Flip: " + config.displayFlip);
  cfglog("   Brightness: " + config.displayBrightness);
  cfglog("ST7789: " + config.st7789);
  cfglog("   MODE: " + config.st7789Mode);
  cfglog("use MHZ19B: " + config.mhz19b);
  cfglog("   PWM PIN: " + config.mhz19bPwmPin);
  cfglog("   RX  PIN: " + config.mhz19bRxPin);
  cfglog("   TX  PIN: " + config.mhz19bTxPin);
  cfglog("MQTT Broker: " + config.mqttBroker);
  cfglog("MQTT Name  : " + config.mqttName);
  cfglog(F("Alerts:"));
  
  cfglog(F("  Temperature:"));
  alerts_to_log_string(config.temperatureAlerts);
  
  cfglog(F("  Humidity   :"));
  alerts_to_log_string(config.humidityAlerts);

  cfglog(F("  Pressure   :"));
  alerts_to_log_string(config.pressureAlerts);  

  cfglog(F("  Luminous   :"));
  alerts_to_log_string(config.luxAlerts);

  cfglog(F("  CO2        :"));
  alerts_to_log_string(config.co2Alerts);
}

void trim_alert_range(config_alert_range_t& range) {
  range.low.trim();
  range.high.trim();
}

void trim_alerts(config_alert_t& alerts) {
  trim_alert_range(alerts.warning1);
  trim_alert_range(alerts.warning2);
  trim_alert_range(alerts.caution1);
  trim_alert_range(alerts.caution2);
}

void trim_config() {
  config.settingId.trim();
  config.ssid.trim();
  config.password.trim();
  config.mDNS.trim();
  config.opMode.trim();
  config.displayFlip.trim();
  config.displayBrightness.trim();
  config.st7789.trim();
  config.st7789Mode.trim();
  config.mhz19b.trim();
  config.mhz19bPwmPin.trim();
  config.mhz19bRxPin.trim();
  config.mhz19bTxPin.trim();
  config.mqttBroker.trim();
  config.mqttName.trim();

  trim_alerts(config.temperatureAlerts);
  trim_alerts(config.humidityAlerts);
  trim_alerts(config.pressureAlerts);
  trim_alerts(config.luxAlerts);
  trim_alerts(config.co2Alerts);
}

DynamicJsonDocument alerts_to_json(const config_alert_t& alerts) {
  DynamicJsonDocument json(100);
  json["warn1.L"] = alerts.warning1.low;
  json["warn1.H"] = alerts.warning1.high;
  json["warn2.L"] = alerts.warning2.low;
  json["warn2.H"] = alerts.warning2.high;
  json["caut1.L"] = alerts.caution1.low;
  json["caut1.H"] = alerts.caution1.high;
  json["caut2.L"] = alerts.caution2.low;
  json["caut2.H"] = alerts.caution2.high;
  return json;
}

void write_config_file(File f) {
  
  trim_config();

  DynamicJsonDocument doc(CONF_JSON_SIZE);
  doc["settingId"] = SETTING_ID;  // これから書くConfigなので必ず想定しているconfig versionを書く
  doc["ssid"] = config.ssid;
  doc["password"] = config.password;
  doc["mDNS"] = config.mDNS;
  doc["opMode"] = config.opMode;
  doc["displayFlip"] = config.displayFlip;
  doc["displayBrightness"] = config.displayBrightness;
  doc["st7789"] = config.st7789;
  doc["st7789Mode"] = config.st7789Mode;
  doc["mhz19b"] = config.mhz19b;
  doc["mhz19bPwmPin"] = config.mhz19bPwmPin;
  doc["mhz19bRxPin"] = config.mhz19bRxPin;
  doc["mhz19bTxPin"] = config.mhz19bTxPin;
  doc["mqttBroker"] = config.mqttBroker;
  doc["mqttName"] = config.mqttName;

  doc["temperatureAlerts"] = alerts_to_json(config.temperatureAlerts);
  doc["humidityAlerts"] = alerts_to_json(config.humidityAlerts);
  doc["luxAlerts"] = alerts_to_json(config.luxAlerts);
  doc["pressureAlerts"] = alerts_to_json(config.pressureAlerts);
  doc["co2Alerts"] = alerts_to_json(config.co2Alerts);

  cfglog(F("Writing config"));
  if (serializeJson(doc, f) == 0) {
    cfglog(F("Failed to write to file"));
  }
}

// 指定されたキーが存在していれば値をセットする。存在しなければセットしない
void set_config_value(String& cfg, DynamicJsonDocument &json, String key) {
  // https://arduinojson.org/v6/api/jsonobject/containskey/
  JsonVariant value = json[key];
  if (value.isNull()) {
    cfglog("Config file not contains key:" + key);
    return;
  }
  cfg = value.as<String>();
}

void set_config_value(String& cfg, DynamicJsonDocument &json, String key1, String key2) {

  JsonVariant middleObj = json[key1];

  if (middleObj.isNull()) {
    
    cfglog("Config file not contains first key:" + key1);
    return;
  } else {
    JsonVariant value = middleObj[key2];

    if (value.isNull()) {
      cfglog("Config file not contains second key:" + key2);
      return;
    }

    cfg = value.as<String>();
  }
}

void read_config_alerts(config_alert_t& alerts, DynamicJsonDocument doc, String key1) {
  set_config_value(alerts.warning1.low ,doc, key1, "warn1.L");
  set_config_value(alerts.warning1.high ,doc, key1, "warn1.H");
  set_config_value(alerts.caution1.low ,doc, key1, "caut1.L");
  set_config_value(alerts.caution1.high ,doc, key1, "caut1.H");
  set_config_value(alerts.warning2.low ,doc, key1, "warn2.L");
  set_config_value(alerts.warning2.high ,doc, key1, "warn2.H");
  set_config_value(alerts.caution2.low ,doc, key1, "caut2.L");
  set_config_value(alerts.caution2.high ,doc, key1, "caut2.H");
}

void read_config_file(File f) {

  set_default_config_value(); // とりあえずデフォルト値をロードしておく。

  DynamicJsonDocument doc(CONF_JSON_SIZE);

  cfglog(F("Json deserialize start"));

  DeserializationError error = deserializeJson(doc, f);

  cfglog(F("Json deserialize done :)"));

  if (error) {
    config.settingId = "INVALID";
    cfglog(F("Failed to read file or Parse as json failed"));
    return;
  }

  set_config_value(config.settingId ,doc ,"settingId");
  set_config_value(config.ssid ,doc ,"ssid");
  set_config_value(config.password ,doc ,"password");
  set_config_value(config.mDNS ,doc ,"mDNS");
  set_config_value(config.opMode ,doc ,"opMode");
  set_config_value(config.displayFlip,doc  ,"displayFlip");
  set_config_value(config.displayBrightness ,doc ,"displayBrightness");
  set_config_value(config.st7789 ,doc ,"st7789");
  set_config_value(config.st7789Mode ,doc ,"st7789Mode");
  set_config_value(config.mhz19b ,doc ,"mhz19b");
  set_config_value(config.mhz19bPwmPin ,doc ,"mhz19bPwmPin");
  set_config_value(config.mhz19bRxPin ,doc ,"mhz19bRxPin");
  set_config_value(config.mhz19bTxPin ,doc ,"mhz19bTxPin");
  set_config_value(config.mqttBroker ,doc ,"mqttBroker");
  set_config_value(config.mqttName ,doc ,"mqttName");

  read_config_alerts(config.temperatureAlerts, doc, "temperatureAlerts");
  read_config_alerts(config.humidityAlerts, doc, "humidityAlerts");
  read_config_alerts(config.pressureAlerts, doc, "pressureAlerts");
  read_config_alerts(config.luxAlerts, doc, "luxAlerts");
  read_config_alerts(config.co2Alerts, doc, "co2Alerts");

  print_config();
}