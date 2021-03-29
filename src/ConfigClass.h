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

enum class ConfigValueType {
  String = 0,  // 任意の文字列
  Integer = 1, // 整数
  Choise = 2   // validValuesに含まれている文字列
};

// APIにより起動中に設定を変更する際の後処理を表す
enum class RunningConfigChangeFlags {
  OK = 0,
  REBOOT_REQ = 1,            // 再起動するまで反映されない
  DISPLAY_REDRAW_REQ = 2,    // 液晶を再描画する必要がある
  MDNS_RESTART_REQ = 3,      // mDNSを再起動する必要がある
  BLOCKED = 4,               // その設定は変更できない
  ERROR = 8,                 // その他のエラー
  NOT_SPECIFIED = 16         // そのキーは存在しない
};

typedef struct config_meta_t {
  String key;
  ConfigValueType type;
  std::vector<String> validValues;
  RunningConfigChangeFlags flags;
} ConfigMeta;

/**
 * Config Class
 */
class Config {
  private:
    SimpleMap configMap;
    std::vector<ConfigMeta> configMetaList;

    bool checkKeyExist(String operation, String key, bool haltOnNoKey) {
      if (!configMap.hasKey(key)) {
        if (haltOnNoKey) { 
          cfglog("[ERROR] Config no key key=" + key + " operation=" + operation);
          halt("CFG NoKey", "OP=" + operation, key);
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

    void addMeta(ConfigMeta meta) {
      configMetaList.push_back(meta);
    }

    /** 
     * 初期値をセットする 
     * ここでセットしていない項目は存在しないものとして扱われる
     */
    void loadDefaultValue();

    // メタデータをセットする。メタデータは初期化しても変更しない
    void loadMetadata();

    // メタデータをセットする。（アラート用）
    void loadMetadataAlert(String alertType);

    // メタデータをセットする。（アラート用のサブルーチン）
    void _loadMetadataAlert(String alertType, String suffix);

    Config() {
      loadMetadata();
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

    // set 普通のキー
    bool set(String key, String value, bool haltOnNoKey = true) {
      // debuglog("[Config] key=" + key + " value=" + value);
      bool keyExist = checkKeyExist("set", key, haltOnNoKey);
      value.trim();

      bool setOK = configMap.set(key, value);
      return (keyExist && setOK);
    }

    // set アラート用
    bool set(String alertType, String keySuffix, String value, bool haltOnNoKey = true) {
      String key = getAlertKey(alertType, keySuffix);
      bool keyExist = checkKeyExist("set", key, haltOnNoKey);
      value.trim();
      bool setOK = configMap.set(key, value);
      return (keyExist && setOK);
    }

    ~Config() {
      Serial.println("Config Class is destructed. maybe bug?");
    }

    static Config& getInstance() {
      static Config instance;
      return instance;
    }
};