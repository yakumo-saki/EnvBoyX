#pragma once

#include <Arduino.h>
#include <unordered_map>

#include "config_names.h"
#include "config_values.h"

#include "log.h"
#include "utils.h"
#include "SimpleMap.h"


enum class ConfigValueType {
  String = 0,  // 任意の文字列
  Integer = 1, // 整数
  Choise = 2,  // validValuesに含まれている文字列
  Double = 3,  // 小数
  NotFound = 255   // そのキーは存在しない。
};

// APIにより起動中に設定を変更する際の後処理を表す
enum class RunningConfigChangeFlags {
  OK = 0,                    // 後処理は不要
  REBOOT_REQ = 1,            // 再起動するまで反映されない
  DISPLAY_REDRAW_REQ = 2,    // 液晶を再描画する必要がある
  MDNS_RESTART_REQ = 3,      // mDNSを再起動する必要がある
  BLOCKED = 4,               // その設定は変更できない
};

enum class ConfigSetResult {
  OK = 0,
  INVALID_VALUE = 8,         // 値が不正
  OTHER_ERROR = 15,          // その他のエラー
  NO_KEY = 16                // そのキーはConfigに存在しない
};

typedef struct config_meta_t {
  String key;
  ConfigValueType type = ConfigValueType::NotFound;
  std::vector<String> validValues;
  RunningConfigChangeFlags flags;
} ConfigMeta;

/**
 * Config Class
 */
class Config {
  private:
    SimpleMap<String> configMap;
    SimpleMap<ConfigMeta> configMetaMap;

    bool checkKeyExist(String operation, String key) {
      if (!configMap.hasKey(key)) {
        return false;
      }
      return true;
    }

    bool addConfig(String key, String value) {
      return configMap.put(key, value, true);
    }

    bool addConfig(String alertType, String key, String value) {
      String cfgKey = getAlertKey(alertType, key);
      return configMap.put(cfgKey, value, true);
    }

    void addMeta(ConfigMeta meta) {
      configMetaMap.put(meta.key, meta);
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

    // メタデータを取得する
    ConfigMeta getMeta(String key) {
      if (!this->configMetaMap.hasKey(key)) {
        ConfigMeta meta;
        meta.type = ConfigValueType::NotFound;
        return meta;
      }
      return configMetaMap.get(key);
    }

    // 設定値を取得する(String)
    String get(String key) {
      checkKeyExist("get", key);
      return configMap.get(key);
    }

    // 設定値を取得する(bool)
    bool getAsBoolean(String key) {
      String val = configMap.get(key);
      return parseBooleanString(val);
    }

    // 設定値を取得する(int)
    int getAsInteger(String key) {
      String val = configMap.get(key);
      int i = val.toInt();
      return i;
    }

    // 設定値を取得する(float)
    float getAsFloat(String key) {
      String val = configMap.get(key);
      double i = val.toFloat();
      return i;
    }

    /** 
     * 設定値を設定する。set 普通のキー
     * @param key キー
     * @param value 値（文字列表記）
     * @return ConfigSetResult 設定を行った結果。呼び出し元で確認必要（正常な値だったか、反映されたか、再起動が必要か）
     */
    ConfigSetResult set(String key, String value) {
      // debuglog("[Config] key=" + key + " value=" + value);
      bool keyExist = checkKeyExist("set", key);
      if (!keyExist) return ConfigSetResult::NO_KEY;

      value.trim();
      
      if (validate(key, value)) {
        bool setOK = configMap.set(key, value);
        if (!setOK) {
          return ConfigSetResult::OTHER_ERROR;
        }
        return ConfigSetResult::OK;
      } else {
        cfglog("INVALID VALUE KEY=" + key + " VALUE=" + value);
        return ConfigSetResult::INVALID_VALUE;
      }

      return ConfigSetResult::OTHER_ERROR;
    }

    // Validation
    // @return looks good or bad
    bool validate(String key, String value) {
      ConfigMeta meta = configMetaMap.get(key);
      bool ret = false;
      if (meta.type == ConfigValueType::Choise) {
        ret = vectorStringContains(meta.validValues, value);
      } else if (meta.type == ConfigValueType::Integer) {
        int dummy = value.toInt(); // 変な文字列が来ている場合0が返る
        ret = (String(dummy) == value); // +10 とか書かれるとアウトだけれども妥協
      } else if (meta.type == ConfigValueType::String) {
        ret = true;
      } else if (meta.type == ConfigValueType::Double) {
        // double dummy = value.toDouble();
        ret = true;
      } else {
        cfglog("[maybe bug]Config SET validation invalid type. key=" + key);
        ret = false;
      }

      return ret;
    }

    static String getAlertKey(String alertType, String key) {
      return alertType + "." + key;
    }

    ~Config() {
      Serial.println("Config Class is destructed. maybe bug?");
    }

    static Config& getInstance() {
      static Config instance;
      return instance;
    }
};