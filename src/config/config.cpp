#include <Arduino.h>
#include <LittleFS.h>

#include <ArduinoJson.h>

#include "log.h"
#include "global.h"
#include "structs.h"
#include "utils.h"

#include "ConfigClass.h"
#include "config_names.h"

const unsigned int CONF_JSON_SIZE = 4000;
extern const String CFG_VERSION_INVALID = "INVALID";


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
bool read_config_file(File f, bool readForUpdate) {

  // とりあえずデフォルト値をロードしておく。
  config->loadDefault();

  DynamicJsonDocument doc(CONF_JSON_SIZE);

  cfglog(F("Json deserialize start"));

  if (DEBUG_BUILD) {
    debuglog("*****  CONFIG RAW DATA DUMP (DEBUG BUILD ONLY) *****");
    while(f.available()){
        Serial.write(f.read());
    }
    Serial.println("");
    f.seek(0);
    debuglog("*****  CONFIG RAW DATA DUMP END                *****");
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
      ret = (config->set(key, val) == ConfigSetResult::OK) && ret;
    }
  }

  cfglog(F("Config read completed."));
  return ret;
}

String read_config_setting_id(File f) {
  DynamicJsonDocument doc(CONF_JSON_SIZE);
  DeserializationError error = deserializeJson(doc, f);

  if (error) {
    return CFG_VERSION_INVALID;
  }

  JsonVariant value = doc[ConfigNames::SETTING_ID];
  if (value.isNull()) {
    return CFG_VERSION_INVALID; // JSON not contains SETTING_ID
  }

  return value.as<String>();
}