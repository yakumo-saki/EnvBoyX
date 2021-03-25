#include <Arduino.h>
#include "FS.h"

#include <ArduinoJson.h>

#include "log.h"
#include "global.h"
#include "structs.h"

#include "config.h"
#include "ConfigClass.h"

const unsigned int CONF_JSON_SIZE = 2000;

void alerts_to_log_string(const String &alertType) {
  cfglog("    WARNING1 '" + config.get(alertType, ConfigNames::ALERT_WARN1_LO) 
         + "' ~ '" + config.get(alertType, ConfigNames::ALERT_WARN1_HI) + "'");
  cfglog("    CAUTION1 '" + config.get(alertType, ConfigNames::ALERT_CAUTION1_LO) 
         + "' ~ '" + config.get(alertType, ConfigNames::ALERT_CAUTION1_HI) + "'");
  cfglog("    CAUTION2 '" + config.get(alertType, ConfigNames::ALERT_CAUTION2_LO) 
         + "' ~ '" + config.get(alertType, ConfigNames::ALERT_CAUTION2_HI) + "'");
  cfglog("    WARNING2 '" + config.get(alertType, ConfigNames::ALERT_WARN2_LO) 
         + "' ~ '" + config.get(alertType, ConfigNames::ALERT_WARN2_HI) + "'");
}

void print_config() {

  cfglog("SSID: " + config.get(ConfigNames::SSID));
  cfglog("PASS: " + config.get(ConfigNames::PASSWORD));
  cfglog("mDNS: " + config.get(ConfigNames::MDNS));
  cfglog("opMode: " + config.get(ConfigNames::OPMODE));
  cfglog(F("DISPLAY:"));
  cfglog("   Flip: " + config.get(ConfigNames::DISPLAY_FLIP));
  cfglog("   Brightness: " + config.get(ConfigNames::DISPLAY_BRIGHTNESS));
  cfglog("   Wait for reconfigure: " + config.get(ConfigNames::DISPLAY_RECONFIG));
  cfglog("I2C OLED TYPE: " + config.get(ConfigNames::OLED_TYPE));
  cfglog("ST7789: " + config.get(ConfigNames::ST7789));
  cfglog("   MODE: " + config.get(ConfigNames::ST7789_MODE));
  cfglog("use MHZ19B: " + config.get(ConfigNames::MHZ19B));
  cfglog("   PWM PIN: " + config.get(ConfigNames::MHZ19B_PWM));
  cfglog("   RX  PIN: " + config.get(ConfigNames::MHZ19B_RX));
  cfglog("   TX  PIN: " + config.get(ConfigNames::MHZ19B_TX));
  cfglog("   ABC    : " + config.get(ConfigNames::MHZ19B_ABC));
  cfglog("MQTT Broker: " + config.get(ConfigNames::MQTT_BROKER));
  cfglog("MQTT Name  : " + config.get(ConfigNames::MQTT_NAME));
  cfglog(F("Alerts:"));
  
  cfglog(F("  Temperature:"));
  alerts_to_log_string(ConfigNames::TEMP_ALERT);
  
  cfglog(F("  Humidity   :"));
  alerts_to_log_string(ConfigNames::HUMI_ALERT);

  cfglog(F("  Pressure   :"));
  alerts_to_log_string(ConfigNames::PRES_ALERT);  

  cfglog(F("  Luminous   :"));
  alerts_to_log_string(ConfigNames::LUX_ALERT);

  cfglog(F("  CO2        :"));
  alerts_to_log_string(ConfigNames::CO2_ALERT);
}

void trim_config() {
  // config.settingId.trim();
  // config.ssid.trim();
  // config.password.trim();
  // config.mDNS.trim();
  // config.opMode.trim();
  // config.displayFlip.trim();
  // config.displayBrightness.trim();
  // config.oledType.trim();
  // config.st7789.trim();
  // config.st7789Mode.trim();
  // config.mhz19b.trim();
  // config.mhz19bPwmPin.trim();
  // config.mhz19bRxPin.trim();
  // config.mhz19bTxPin.trim();
  // config.mqttBroker.trim();
  // config.mqttName.trim();

  // trim_alerts(config.temperatureAlerts);
  // trim_alerts(config.humidityAlerts);
  // trim_alerts(config.pressureAlerts);
  // trim_alerts(config.luxAlerts);
  // trim_alerts(config.co2Alerts);
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

DynamicJsonDocument _create_config_json(bool save, const std::vector<String> &keyArray) {

  DynamicJsonDocument json(CONF_JSON_SIZE);

  // if (save) json[ConfigNames::PASSWORD] = config.password; // all

  // for (const auto& k : keyArray) {
  //   if (save || k == ConfigNames::SSID)         json[ConfigNames::SSID] = config.ssid;
  //   if (save || k == ConfigNames::MDNS)         json[ConfigNames::MDNS] = config.mDNS;
  //   if (save || k == ConfigNames::OPMODE)       json[ConfigNames::OPMODE] = config.opMode;
  //   if (save || k == ConfigNames::DISPLAY_FLIP) json[ConfigNames::DISPLAY_FLIP] = config.displayFlip;
  //   if (save || k == ConfigNames::DISPLAY_BRIGHTNESS) json[ConfigNames::DISPLAY_BRIGHTNESS] = config.displayBrightness;
  //   if (save || k == ConfigNames::DISPLAY_RECONFIG) json[ConfigNames::DISPLAY_RECONFIG] = config.displayWaitForReconfigure;
    
  //   if (save || k == ConfigNames::OLED_TYPE)    json[ConfigNames::OLED_TYPE] = config.oledType;
    
  //   if (save || k == ConfigNames::ST7789)       json[ConfigNames::ST7789] = config.st7789;
  //   if (save || k == ConfigNames::ST7789_MODE)  json[ConfigNames::ST7789_MODE] = config.st7789Mode;

  //   if (save || k == ConfigNames::MHZ19B)       json[ConfigNames::MHZ19B] = config.mhz19b;
  //   if (save || k == ConfigNames::MHZ19B_PWM)   json[ConfigNames::MHZ19B_PWM] = config.mhz19bPwmPin;
  //   if (save || k == ConfigNames::MHZ19B_RX)    json[ConfigNames::MHZ19B_RX] = config.mhz19bRxPin;
  //   if (save || k == ConfigNames::MHZ19B_TX)    json[ConfigNames::MHZ19B_TX] = config.mhz19bTxPin;
  //   if (save || k == ConfigNames::MHZ19B_ABC)   json[ConfigNames::MHZ19B_ABC] = config.mhz19bABC;

  //   if (save || k == ConfigNames::MQTT_BROKER)  json[ConfigNames::MQTT_BROKER] = config.mqttBroker;
  //   if (save || k == ConfigNames::MQTT_NAME)    json[ConfigNames::MQTT_NAME] = config.mqttName;

  //   if (save || k == ConfigNames::TEMP_ALERT) json[ConfigNames::TEMP_ALERT] = alerts_to_json(config.temperatureAlerts, "temperature");
  //   if (save || k == ConfigNames::HUMI_ALERT) json[ConfigNames::HUMI_ALERT] = alerts_to_json(config.humidityAlerts, "humidity");
  //   if (save || k == ConfigNames::LUX_ALERT)  json[ConfigNames::LUX_ALERT] = alerts_to_json(config.luxAlerts, "lux");
  //   if (save || k == ConfigNames::PRES_ALERT) json[ConfigNames::PRES_ALERT] = alerts_to_json(config.pressureAlerts, "pressure");
  //   if (save || k == ConfigNames::CO2_ALERT)  json[ConfigNames::CO2_ALERT] = alerts_to_json(config.co2Alerts, "co2");
  // }

  // これをやると以降の変更が反映されなくなるのでやらない
  // json.shrinkToFit();
  return json;
}

DynamicJsonDocument create_config_json(std::vector<String> &keys) {
  return _create_config_json(false, keys);
}

DynamicJsonDocument create_config_json_all() {
  std::vector<String> dummyKey;
  return _create_config_json(true, dummyKey);
}


void write_config_file(File f) {
  
  trim_config();

  DynamicJsonDocument doc = create_config_json_all();
  
  // これから書くConfigなので必ず想定しているconfig versionを書く
  doc[ConfigNames::SETTING_ID] = SETTING_ID;

  cfglog(F("Writing config"));
  size_t size = serializeJson(doc, f);
  if (size == 0) {
    cfglog(F("Failed to write to file (size = 0)"));
  } else {
    cfglog("Overall JSON is " + String(size) + " bytes");
  }
}

/**
 * CONFIGを読み込む（本体）
 * @param dump_config CONFIGをシリアルにログ出力するか否か
 */
bool read_config_file(File f, bool dump_config) {

  // とりあえずデフォルト値をロードしておく。
  config.loadDefault();

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
    cfglog(F("Failed to read file or Parse as json failed"));
    cfglog("Reason: " + String(error.c_str()));
    return false;
  } else {
    cfglog(F("Json deserialize success. Trying to read :)"));
  }

  bool ret = true;

  std::vector<String> keys = config.getKeys();
  for(String key : keys) {
    
    JsonVariant value = doc[key];

    if (value.isNull()) {
      cfglog("Config file not contains key:" + key);
      ret = false;
    } else {
      String val = value.as<String>();
      ret = ret && config.set(key, val, false);
    }
  }

  return ret;
}

bool read_config_file(File f) {
  return read_config_file(f, false);
}

String read_config_setting_id(File f) {
  DynamicJsonDocument doc(CONF_JSON_SIZE);
  DeserializationError error = deserializeJson(doc, f);

  if (error) {
    return "INVALID";
  }

  JsonVariant value = doc[ConfigNames::SETTING_ID];
  if (value.isNull()) {
    return "INVALID"; // JSON not contains SETTING_ID
  }

  return value.as<String>();
}