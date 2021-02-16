#include <Arduino.h>
#include "FS.h"

#include <ArduinoJson.h>

#include "log.h"
#include "global.h"
#include "structs.h"

#include "config.h"

extern const unsigned int CONF_JSON_SIZE;

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

  config.oledType = OLED_SSD1306;

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
  config.co2Alerts.warning1.high = "200";
  config.co2Alerts.caution1.low = "200";
  config.co2Alerts.caution1.high = "300";
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
  cfglog("I2C OLED TYPE: " + config.oledType);
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
  config.oledType.trim();
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

DynamicJsonDocument alerts_to_json(const config_alert_t& alerts, String logname) {
  DynamicJsonDocument json(300);
  json[CFG_ALERT_WARN1_LO] = alerts.warning1.low;
  json[CFG_ALERT_WARN1_HI] = alerts.warning1.high;
  json[CFG_ALERT_WARN2_LO] = alerts.warning2.low;
  json[CFG_ALERT_WARN2_HI] = alerts.warning2.high;
  json[CFG_ALERT_CAUTION1_LO] = alerts.caution1.low;
  json[CFG_ALERT_CAUTION1_HI] = alerts.caution1.high;
  json[CFG_ALERT_CAUTION2_LO] = alerts.caution2.low;
  json[CFG_ALERT_CAUTION2_HI] = alerts.caution2.high;
  json.shrinkToFit();

  size_t size = measureJson(json);

  cfglog("Json alerts " + logname + " section is " + String(size) + " bytes");

  return json;
}

DynamicJsonDocument _create_config_json(bool save, std::vector<String> keyArray) {

  DynamicJsonDocument json(CONF_JSON_SIZE);

  if (save) json[CFG_PASSWORD] = config.password; // all

  for (const auto& k : keyArray) {
    debuglog(k);
    if (save || k == CFG_SSID)         json[CFG_SSID] = config.ssid;
    if (save || k == CFG_MDNS)         json[CFG_MDNS] = config.mDNS;
    if (save || k == CFG_OPMODE)       json[CFG_OPMODE] = config.opMode;
    if (save || k == CFG_DISPLAY_FLIP) json[CFG_DISPLAY_FLIP] = config.displayFlip;
    if (save || k == CFG_DISPLAY_BRIGHTNESS) json[CFG_DISPLAY_BRIGHTNESS] = config.displayBrightness;
    if (save || k == CFG_OLED_TYPE)    json[CFG_OLED_TYPE] = config.oledType;
    if (save || k == CFG_ST7789)       json[CFG_ST7789] = config.st7789;
    if (save || k == CFG_ST7789_MODE)  json[CFG_ST7789_MODE] = config.st7789Mode;
    if (save || k == CFG_MHZ19B)       json[CFG_MHZ19B] = config.mhz19b;
    if (save || k == CFG_MHZ19B_PWM)   json[CFG_MHZ19B_PWM] = config.mhz19bPwmPin;
    if (save || k == CFG_MHZ19B_RX)    json[CFG_MHZ19B_RX] = config.mhz19bRxPin;
    if (save || k == CFG_MHZ19B_TX)    json[CFG_MHZ19B_TX] = config.mhz19bTxPin;
    if (save || k == CFG_MQTT_BROKER)  json[CFG_MQTT_BROKER] = config.mqttBroker;
    if (save || k == CFG_MQTT_NAME)    json[CFG_MQTT_NAME] = config.mqttName;

    if (save || k == CFG_TEMP_ALERT) json[CFG_TEMP_ALERT] = alerts_to_json(config.temperatureAlerts, "temperature");
    if (save || k == CFG_HUMI_ALERT) json[CFG_HUMI_ALERT] = alerts_to_json(config.humidityAlerts, "humidity");
    if (save || k == CFG_LUX_ALERT)  json[CFG_LUX_ALERT] = alerts_to_json(config.luxAlerts, "lux");
    if (save || k == CFG_PRES_ALERT) json[CFG_LUX_ALERT] = alerts_to_json(config.pressureAlerts, "pressure");
    if (save || k == CFG_CO2_ALERT)  json[CFG_CO2_ALERT] = alerts_to_json(config.co2Alerts, "co2");
  }

  json.shrinkToFit();
  return json;
}

DynamicJsonDocument create_config_json(std::vector<String> keys) {
  return _create_config_json(false, keys);
}

DynamicJsonDocument create_config_json_all() {
  std::vector<String> dummyKey;
  dummyKey.push_back("dummy");
  return _create_config_json(true, dummyKey);
}


void write_config_file(File f) {
  
  trim_config();

  DynamicJsonDocument doc = create_config_json_all();
  
  // これから書くConfigなので必ず想定しているconfig versionを書く
  doc[CFG_SETTING_ID] = SETTING_ID;

  cfglog(F("Writing config"));
  size_t size = serializeJson(doc, f);
  if (size == 0) {
    cfglog(F("Failed to write to file (size = 0)"));
  } else {
    cfglog("Overall JSON is " + String(size) + " bytes");
  }
}

// 指定されたキーが存在していれば値をセットする。存在しなければセットしない
bool set_config_value(String& cfg, DynamicJsonDocument &json, String key) {
  // https://arduinojson.org/v6/api/jsonobject/containskey/
  JsonVariant value = json[key];
  if (value.isNull()) {
    cfglog("Config file not contains key:" + key);
    return false;
  }
  cfg = value.as<String>();
  return true;
}

bool set_config_value(String& cfg, DynamicJsonDocument &json, String key1, String key2) {

  JsonVariant middleObj = json[key1];

  if (middleObj.isNull()) {
    
    cfglog("Config file not contains first key:" + key1);
    return false;
  } else {
    JsonVariant value = middleObj[key2];

    if (value.isNull()) {
      cfglog("Config file not contains second key:" + key1 + "->" + key2);
      return false;
    }

    cfg = value.as<String>();
  }

  return true;
}

bool read_config_alerts(config_alert_t& alerts, DynamicJsonDocument doc, String key1) {
  bool ret = true;

  // 失敗しているものがあってもとりあえず最後まで設定読み込みを行う（セットアップモードの時に必要なので）
  ret = ret && set_config_value(alerts.warning1.low ,doc, key1, CFG_ALERT_WARN1_LO);
  ret = ret && set_config_value(alerts.warning1.high ,doc, key1, CFG_ALERT_WARN1_HI);
  ret = ret && set_config_value(alerts.caution1.low ,doc, key1, CFG_ALERT_CAUTION1_LO);
  ret = ret && set_config_value(alerts.caution1.high ,doc, key1, CFG_ALERT_CAUTION1_HI);
  ret = ret && set_config_value(alerts.warning2.low ,doc, key1, CFG_ALERT_WARN2_LO);
  ret = ret && set_config_value(alerts.warning2.high ,doc, key1, CFG_ALERT_WARN2_HI);
  ret = ret && set_config_value(alerts.caution2.low ,doc, key1, CFG_ALERT_CAUTION2_LO);
  ret = ret && set_config_value(alerts.caution2.high ,doc, key1, CFG_ALERT_CAUTION2_HI);
  
  return ret;
}

bool read_config_file(File f, bool dump_config) {

  set_default_config_value(); // とりあえずデフォルト値をロードしておく。

  DynamicJsonDocument doc(CONF_JSON_SIZE);

  cfglog(F("Json deserialize start"));

  if (dump_config) {
    Serial.println("");
    while(f.available()){
        Serial.write(f.read());
    }
    Serial.println("");
    f.seek(0);
  }

  DeserializationError error = deserializeJson(doc, f);

  if (error) {
    config.settingId = "INVALID";
    cfglog(F("Failed to read file or Parse as json failed"));
    cfglog("Reason: " + String(error.c_str()));
    return false;
  } else {
    cfglog(F("Json deserialize done :)"));
  }

  bool ret = true;

  ret = ret && set_config_value(config.settingId ,doc, CFG_SETTING_ID);
  ret = ret && set_config_value(config.ssid ,doc, CFG_SSID);
  ret = ret && set_config_value(config.password ,doc , CFG_PASSWORD);
  ret = ret && set_config_value(config.mDNS ,doc, CFG_MDNS);
  ret = ret && set_config_value(config.opMode ,doc, CFG_OPMODE);
  ret = ret && set_config_value(config.displayFlip,doc, CFG_DISPLAY_FLIP);
  ret = ret && set_config_value(config.displayBrightness, doc, CFG_DISPLAY_BRIGHTNESS);
  ret = ret && set_config_value(config.oledType, doc, CFG_OLED_TYPE);
  ret = ret && set_config_value(config.st7789 ,doc, CFG_ST7789);
  ret = ret && set_config_value(config.st7789Mode, doc, CFG_ST7789_MODE);
  ret = ret && set_config_value(config.mhz19b, doc, CFG_MHZ19B);
  ret = ret && set_config_value(config.mhz19bPwmPin, doc, CFG_MHZ19B_PWM);
  ret = ret && set_config_value(config.mhz19bRxPin, doc, CFG_MHZ19B_RX);
  ret = ret && set_config_value(config.mhz19bTxPin, doc, CFG_MHZ19B_TX);
  ret = ret && set_config_value(config.mqttBroker, doc, CFG_MQTT_BROKER);
  ret = ret && set_config_value(config.mqttName, doc, CFG_MQTT_NAME);

  ret = ret && read_config_alerts(config.temperatureAlerts, doc, CFG_TEMP_ALERT);
  ret = ret && read_config_alerts(config.humidityAlerts, doc, CFG_HUMI_ALERT);
  ret = ret && read_config_alerts(config.pressureAlerts, doc, CFG_PRES_ALERT);
  ret = ret && read_config_alerts(config.luxAlerts, doc, CFG_LUX_ALERT);
  ret = ret && read_config_alerts(config.co2Alerts, doc, CFG_CO2_ALERT);

  return ret;
}

bool read_config_file(File f) {
  return read_config_file(f, false);
}
