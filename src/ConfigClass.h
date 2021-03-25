#pragma once

#include <Arduino.h>
#include <unordered_map>

#include "log.h"
#include "halt.h"
#include "config_names.h"
#include "config_values.h"

#include "SimpleMap.h"

//  * 項目追加時の実装項目：
// - [ ] 1. structs.h           config struct
// - [ ] 2. Setup HTML          http_setup_web_get.cpp http_setup_get_root_content
// - [ ] 3. Setup POST後の表示   http_setup_web_post.cpp http_setup_post_root_content
// - [ ] 4. Web API             http_api_backup_config
// - [ ] 5. Web API             updateConfig
// - [ ] 6. Write Config        _create_config_json
// - [ ] 7. Read Config         read_config_file
// - [ ] 8. Print config        print_config
// - [ ] 9. Config version      global.cpp

/**
 * Config Class
 */
class Config {
  private:
    SimpleMap configMap;

    bool checkKeyExist(String operation, String key, bool haltOnNoKey) {
      if (configMap.hasKey(key)) {
        if (haltOnNoKey) { 
          halt("Cfg no key", operation, key);
        } else {
          cfglog("[WARN] Config Nokey key=" + key + " operation=" + operation);
        }
        return false;
      }
      return true;
    }

    String getAlertKey(String alertType, String key) {
      return alertType + "." + key;
    }

    bool addConfig(String key, String value) {
      return configMap.put(key, value, true);
    }

    bool addConfig(String alertType, String key, String value) {
      String cfgKey = getAlertKey(alertType, key);
      return configMap.put(cfgKey, value, true);
    }

    /** 
     * 初期値をセットする 
     * ここでセットしていない項目は存在しないものとして扱われる
     */
    void loadDefaultValue();

    Config() {
      loadDefaultValue();
    }

  public:
    // 設定をすべて初期化する
    void loadDefault() {
      this->configMap.clear();
      loadDefaultValue();
    }

    std::vector<String> getKeys() {
      return configMap.getKeys();
    }

    String get(String key) {
      checkKeyExist("get", key, true);
      return configMap.get(key);
    }

    String get(String alertType, String keySuffix) {
      String key = getAlertKey(alertType, keySuffix);
      checkKeyExist("get", key, true);
      return configMap.get(key);
    }

    bool set(String key, String value, bool haltOnNoKey = true) {
      bool ret = checkKeyExist("set", key, haltOnNoKey);
      value.trim();
      ret = ret && configMap.set(key, value);
      return ret;
    }

    bool set(String alertType, String keySuffix, String value, bool haltOnNoKey = true) {
      String key = getAlertKey(alertType, keySuffix);
      bool ret = checkKeyExist("set", key, haltOnNoKey);
      value.trim();
      ret = ret && configMap.set(key, value);
      return ret;
    }

    // // 設定を取得する
    // static String get(String key) {
    //     return Config::getInstance().getConfig(key);
    // }

    // // 設定を取得する
    // static String get(String alertType, String key) {
    //     return Config::getInstance().getConfig(alertType, key);
    // }

    // // 設定を取得する(Intにキャストする)
    // static int getInt(String key) {
    //     return Config::get(key).toInt();
    // }

    // // 設定を取得する(Intにキャストする)
    // static int getInt(String alertType, String key) {
    //     return Config::get(alertType, key).toInt();
    // }

    // // 設定を保存する
    // static bool set(String key, String value, bool haltOnNoKey = true) {
    //     return Config::getInstance().setConfig(key, value, haltOnNoKey);
    // }

    // // 設定を保存する（アラート）
    // static bool set(String alertType, String key, String value, bool haltOnNoKey = true) {
    //     return Config::getInstance().setConfig(alertType, key, value, haltOnNoKey);
    // }

    // std::vector<String> getKeys() {
    //   return Config::getInstance().getAllKeys();
    // }
    ~Config() {
      Serial.println("Config Class is destructed. maybe bug?");
    }

    static Config& getInstance() {
      static Config instance;
      return instance;
    }
};