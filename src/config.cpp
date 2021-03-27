#include <Arduino.h>
#include "FS.h"

#include <ArduinoJson.h>

#include "log.h"
#include "global.h"
#include "structs.h"
#include "utils.h"

#include "ConfigClass.h"
#include "config_names.h"

const unsigned int CONF_JSON_SIZE = 4000;

void alerts_to_log_string(const String &alertType) {
  cfglog("    WARNING1 '" + config->get(alertType, ConfigNames::ALERT_WARN1_LO) 
         + "' ~ '" + config->get(alertType, ConfigNames::ALERT_WARN1_HI) + "'");
  cfglog("    CAUTION1 '" + config->get(alertType, ConfigNames::ALERT_CAUTION1_LO) 
         + "' ~ '" + config->get(alertType, ConfigNames::ALERT_CAUTION1_HI) + "'");
  cfglog("    CAUTION2 '" + config->get(alertType, ConfigNames::ALERT_CAUTION2_LO) 
         + "' ~ '" + config->get(alertType, ConfigNames::ALERT_CAUTION2_HI) + "'");
  cfglog("    WARNING2 '" + config->get(alertType, ConfigNames::ALERT_WARN2_LO) 
         + "' ~ '" + config->get(alertType, ConfigNames::ALERT_WARN2_HI) + "'");
}

void print_config() {

  cfglog("SSID: " + config->get(ConfigNames::SSID));
  cfglog("PASS: " + config->get(ConfigNames::PASSWORD));
  cfglog("mDNS: " + config->get(ConfigNames::MDNS));
  cfglog("opMode: " + config->get(ConfigNames::OPMODE));
  cfglog(F("DISPLAY:"));
  cfglog("   Flip: " + config->get(ConfigNames::DISPLAY_FLIP));
  cfglog("   Brightness: " + config->get(ConfigNames::DISPLAY_BRIGHTNESS));
  cfglog("   Wait for reconfigure: " + config->get(ConfigNames::DISPLAY_RECONFIG));
  cfglog("I2C OLED TYPE: " + config->get(ConfigNames::OLED_TYPE));
  cfglog("ST7789: " + config->get(ConfigNames::ST7789));
  cfglog("   MODE: " + config->get(ConfigNames::ST7789_MODE));
  cfglog("use MHZ19B: " + config->get(ConfigNames::MHZ19B));
  cfglog("   PWM PIN: " + config->get(ConfigNames::MHZ19B_PWM));
  cfglog("   RX  PIN: " + config->get(ConfigNames::MHZ19B_RX));
  cfglog("   TX  PIN: " + config->get(ConfigNames::MHZ19B_TX));
  cfglog("   ABC    : " + config->get(ConfigNames::MHZ19B_ABC));
  cfglog("MQTT Broker: " + config->get(ConfigNames::MQTT_BROKER));
  cfglog("MQTT Name  : " + config->get(ConfigNames::MQTT_NAME));
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
}

DynamicJsonDocument _create_config_json(bool save, const std::vector<String> &keyArray) {

  DynamicJsonDocument json(CONF_JSON_SIZE);

  std::vector<String> ONLY_SAVE = {ConfigNames::PASSWORD};

  std::vector<String> keys = config->getKeys();
  for(String key : keys) {
    if (!save && vectorStringContains(ONLY_SAVE, key)) {
      continue;
    }

    json[key] = config->get(key);
  }

  return json;
}

DynamicJsonDocument create_config_json(std::vector<String> &keys) {
  return _create_config_json(false, keys);
}

DynamicJsonDocument create_config_json_all() {
  std::vector<String> dummyKey;
  return _create_config_json(true, dummyKey);
}

/**
 * CONFIG の SAVE
 */
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
 */
bool read_config_file(File f) {

  // とりあえずデフォルト値をロードしておく。
  config->loadDefault();

  DynamicJsonDocument doc(CONF_JSON_SIZE);

  cfglog(F("Json deserialize start"));

  if (false) {
    debuglog("*****  CONFIG DUMP     *****");
    Serial.println("");
    while(f.available()){
        Serial.write(f.read());
    }
    Serial.println("");
    f.seek(0);
    debuglog("*****  CONFIG DUMP END *****");
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

  std::vector<String> keys = config->getKeys();
  // debuglog("keys=" + String(keys.size()));
  
  for(String key : keys) {
    
    JsonVariant value = doc[key];

    if (value.isNull()) {
      cfglog("[WARN] Config file not contains key:" + key);
      ret = false;
    } else {
      String val = value.as<String>();
      // debuglog("read config key=" + key + " value=" + val);
      ret = config->set(key, val, false) && ret;
    }
  }

  cfglog(F("Config read completed."));
  return ret;
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