#include <Arduino.h>
#include "FS.h"

#include <ArduinoJson.h>

#include "log.h"
#include "global.h"
#include "structs.h"

#include "config.h"

const unsigned int CONF_JSON_SIZE = 2000;

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
  cfglog("   Wait for reconfigure: " + config.displayWaitForReconfigure);
  cfglog("I2C OLED TYPE: " + config.oledType);
  cfglog("ST7789: " + config.st7789);
  cfglog("   MODE: " + config.st7789Mode);
  cfglog("use MHZ19B: " + config.mhz19b);
  cfglog("   PWM PIN: " + config.mhz19bPwmPin);
  cfglog("   RX  PIN: " + config.mhz19bRxPin);
  cfglog("   TX  PIN: " + config.mhz19bTxPin);
  cfglog("   ABC    : " + config.mhz19bABC);
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
  json[ConfigNames::ALERT_WARN1_LO] = alerts.warning1.low;
  json[ConfigNames::ALERT_WARN1_HI] = alerts.warning1.high;
  json[ConfigNames::ALERT_WARN2_LO] = alerts.warning2.low;
  json[ConfigNames::ALERT_WARN2_HI] = alerts.warning2.high;
  json[ConfigNames::ALERT_CAUTION1_LO] = alerts.caution1.low;
  json[ConfigNames::ALERT_CAUTION1_HI] = alerts.caution1.high;
  json[ConfigNames::ALERT_CAUTION2_LO] = alerts.caution2.low;
  json[ConfigNames::ALERT_CAUTION2_HI] = alerts.caution2.high;
  json.shrinkToFit();

  size_t size = measureJson(json);

  cfglog("Json alerts " + logname + " section is " + String(size) + " bytes");

  return json;
}

DynamicJsonDocument _create_config_json(bool save, std::vector<String> keyArray) {

  DynamicJsonDocument json(CONF_JSON_SIZE);

  if (save) json[ConfigNames::PASSWORD] = config.password; // all

  for (const auto& k : keyArray) {
    if (save || k == ConfigNames::SSID)         json[ConfigNames::SSID] = config.ssid;
    if (save || k == ConfigNames::MDNS)         json[ConfigNames::MDNS] = config.mDNS;
    if (save || k == ConfigNames::OPMODE)       json[ConfigNames::OPMODE] = config.opMode;
    if (save || k == ConfigNames::DISPLAY_FLIP) json[ConfigNames::DISPLAY_FLIP] = config.displayFlip;
    if (save || k == ConfigNames::DISPLAY_BRIGHTNESS) json[ConfigNames::DISPLAY_BRIGHTNESS] = config.displayBrightness;
    if (save || k == ConfigNames::DISPLAY_RECONFIG) json[ConfigNames::DISPLAY_RECONFIG] = config.displayWaitForReconfigure;
    
    if (save || k == ConfigNames::OLED_TYPE)    json[ConfigNames::OLED_TYPE] = config.oledType;
    
    if (save || k == ConfigNames::ST7789)       json[ConfigNames::ST7789] = config.st7789;
    if (save || k == ConfigNames::ST7789_MODE)  json[ConfigNames::ST7789_MODE] = config.st7789Mode;

    if (save || k == ConfigNames::MHZ19B)       json[ConfigNames::MHZ19B] = config.mhz19b;
    if (save || k == ConfigNames::MHZ19B_PWM)   json[ConfigNames::MHZ19B_PWM] = config.mhz19bPwmPin;
    if (save || k == ConfigNames::MHZ19B_RX)    json[ConfigNames::MHZ19B_RX] = config.mhz19bRxPin;
    if (save || k == ConfigNames::MHZ19B_TX)    json[ConfigNames::MHZ19B_TX] = config.mhz19bTxPin;
    if (save || k == ConfigNames::MHZ19B_ABC)   json[ConfigNames::MHZ19B_ABC] = config.mhz19bABC;

    if (save || k == ConfigNames::MQTT_BROKER)  json[ConfigNames::MQTT_BROKER] = config.mqttBroker;
    if (save || k == ConfigNames::MQTT_NAME)    json[ConfigNames::MQTT_NAME] = config.mqttName;

    if (save || k == ConfigNames::TEMP_ALERT) json[ConfigNames::TEMP_ALERT] = alerts_to_json(config.temperatureAlerts, "temperature");
    if (save || k == ConfigNames::HUMI_ALERT) json[ConfigNames::HUMI_ALERT] = alerts_to_json(config.humidityAlerts, "humidity");
    if (save || k == ConfigNames::LUX_ALERT)  json[ConfigNames::LUX_ALERT] = alerts_to_json(config.luxAlerts, "lux");
    if (save || k == ConfigNames::PRES_ALERT) json[ConfigNames::PRES_ALERT] = alerts_to_json(config.pressureAlerts, "pressure");
    if (save || k == ConfigNames::CO2_ALERT)  json[ConfigNames::CO2_ALERT] = alerts_to_json(config.co2Alerts, "co2");
  }

  // これをやると以降の変更が反映されなくなるのでやらない
  // json.shrinkToFit();
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
  doc[ConfigNames::SETTING_ID] = ConfigValues::SETTING_ID;

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
  ret = ret && set_config_value(alerts.warning1.low ,doc, key1, ConfigNames::ALERT_WARN1_LO);
  ret = ret && set_config_value(alerts.warning1.high ,doc, key1, ConfigNames::ALERT_WARN1_HI);
  ret = ret && set_config_value(alerts.caution1.low ,doc, key1, ConfigNames::ALERT_CAUTION1_LO);
  ret = ret && set_config_value(alerts.caution1.high ,doc, key1, ConfigNames::ALERT_CAUTION1_HI);
  ret = ret && set_config_value(alerts.warning2.low ,doc, key1, ConfigNames::ALERT_WARN2_LO);
  ret = ret && set_config_value(alerts.warning2.high ,doc, key1, ConfigNames::ALERT_WARN2_HI);
  ret = ret && set_config_value(alerts.caution2.low ,doc, key1, ConfigNames::ALERT_CAUTION2_LO);
  ret = ret && set_config_value(alerts.caution2.high ,doc, key1, ConfigNames::ALERT_CAUTION2_HI);
  
  return ret;
}

bool read_config_file(File f, bool dump_config) {

  // とりあえずデフォルト値をロードしておく。
  config = config_t();

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

  ret = ret && set_config_value(config.settingId ,doc, ConfigNames::SETTING_ID);
  ret = ret && set_config_value(config.ssid ,doc, ConfigNames::SSID);
  ret = ret && set_config_value(config.password ,doc , ConfigNames::PASSWORD);
  ret = ret && set_config_value(config.mDNS ,doc, ConfigNames::MDNS);
  ret = ret && set_config_value(config.opMode ,doc, ConfigNames::OPMODE);
  ret = ret && set_config_value(config.displayFlip,doc, ConfigNames::DISPLAY_FLIP);
  ret = ret && set_config_value(config.displayBrightness, doc, ConfigNames::DISPLAY_BRIGHTNESS);
  ret = ret && set_config_value(config.displayWaitForReconfigure, doc, ConfigNames::DISPLAY_RECONFIG);
  ret = ret && set_config_value(config.oledType, doc, ConfigNames::OLED_TYPE);
  ret = ret && set_config_value(config.st7789 ,doc, ConfigNames::ST7789);
  ret = ret && set_config_value(config.st7789Mode, doc, ConfigNames::ST7789_MODE);
  ret = ret && set_config_value(config.mhz19b, doc, ConfigNames::MHZ19B);
  ret = ret && set_config_value(config.mhz19bPwmPin, doc, ConfigNames::MHZ19B_PWM);
  ret = ret && set_config_value(config.mhz19bRxPin, doc, ConfigNames::MHZ19B_RX);
  ret = ret && set_config_value(config.mhz19bTxPin, doc, ConfigNames::MHZ19B_TX);
  ret = ret && set_config_value(config.mhz19bABC, doc, ConfigNames::MHZ19B_ABC);
  ret = ret && set_config_value(config.mqttBroker, doc, ConfigNames::MQTT_BROKER);
  ret = ret && set_config_value(config.mqttName, doc, ConfigNames::MQTT_NAME);

  ret = ret && read_config_alerts(config.temperatureAlerts, doc, ConfigNames::TEMP_ALERT);
  ret = ret && read_config_alerts(config.humidityAlerts, doc, ConfigNames::HUMI_ALERT);
  ret = ret && read_config_alerts(config.pressureAlerts, doc, ConfigNames::PRES_ALERT);
  ret = ret && read_config_alerts(config.luxAlerts, doc, ConfigNames::LUX_ALERT);
  ret = ret && read_config_alerts(config.co2Alerts, doc, ConfigNames::CO2_ALERT);

  return ret;
}

bool read_config_file(File f) {
  return read_config_file(f, false);
}
