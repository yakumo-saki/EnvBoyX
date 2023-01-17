#include <Arduino.h>

#include <ArduinoJson.h>

#include "log.h"
#include "global.h"
#include "config/config.h"
#include "ConfigClass.h"
#include "utils.h"

#include "mdns_client.h"

#include "network/webserver.h"
#include "network/api/api_util.h"
#include "display/display.h"

extern unsigned int CONF_JSON_SIZE;

extern HTTPWEBSERVER server;

const String CONFIG_NONE = "#**#_##NONE##_#*#";

struct ConfigHookFlags {
  bool needDisplayRedraw = false;
  bool needMDnsRestart = false;
  bool needReboot = false;
  bool configFailed = false;
};

struct UpdateConfigParamResult_t {
  RunningConfigChangeFlags result = RunningConfigChangeFlags::BLOCKED;
  String value = "";
};

// Config set API の処理
// すべての有効なconfig keyのループ→POSTパラメタにそれが存在するか？という流れなので
// すべての有効なconfig keyでここが実行される
void updateConfigParamForApi(DynamicJsonDocument& msgArray, ConfigHookFlags &flags, std::vector<String>& validKeys, String key) {
  
  if (!server.hasArg(key)) {
    return;
  } 
  
  String value = server.arg(key);

  ConfigSetResult setResult = config->set(key, value);

  if (setResult != ConfigSetResult::NO_KEY) {
    // 有効な設定名だったので記録しておく
    validKeys.push_back(key);
  }

  if (setResult == ConfigSetResult::OK) {
    apilog("Config SET: OK   : " + key + " => " + value);
  } else {
    flags.configFailed = true;
    apilog("Config SET: ERROR: " + key + " => " + value);
  }

  // 値エラー判定
  if (setResult == ConfigSetResult::INVALID_VALUE) {
    msgArray.add("[ERROR][INVALID_VALUE] " + key + "=" + value);
    return;
  } else if (setResult == ConfigSetResult::OTHER_ERROR) {
    msgArray.add("[ERROR][OTHER_ERROR] " + key + "=" + value);
    return;
  }

  // 設定後の処理を取得
  ConfigMeta meta = config->getMeta(key, true);

  if (meta.flags == RunningConfigChangeFlags::BLOCKED) {
    flags.configFailed = true;
    msgArray.add("[ERROR] " + key + " is blocked from running change. use setup mode.");
  } else if (meta.flags == RunningConfigChangeFlags::REBOOT_REQ) {
    flags.needReboot = true;
    msgArray.add("[OK][REBOOT REQ] " + key);
  } else if (meta.flags == RunningConfigChangeFlags::DISPLAY_REDRAW_REQ) {
    flags.needDisplayRedraw = true;
    // msgArray.add("[OK][REDRAW] " + key);
  } else if (meta.flags == RunningConfigChangeFlags::MDNS_RESTART_REQ) {
    flags.needMDnsRestart = true;
    // msgArray.add("[OK][mDNS RESTART] " + key);
  } else if (meta.flags == RunningConfigChangeFlags::OK) {
    // msgArray.add("[OK] " + key);
  } else {
    apilog("MAYBE BUG");
  } 
}

void _reflectConfig(ConfigHookFlags& flags, bool all = false) {

  if (all || flags.needDisplayRedraw) {
    apilog("Exec display redraw.");
    disp_redraw_sensor_value_screen();
  }

  if (all || flags.needMDnsRestart) {
    apilog("Exec mDNS restart.");
    apilog("New mDNS name restart." + config->get(ConfigNames::MDNS));
    mdns_hostname_change(config->get(ConfigNames::MDNS));
  }
}

// revertしたときに画面書き換え等を全部実行する
void reflectConfigAll() {
  ConfigHookFlags flags;
  _reflectConfig(flags, true);
}

// Config SET API のエントリポイント
String updateConfig() {

  // DynamicJsonDocument.createNestedArray() スべきだがそうするとここですべての
  // JSONの容量を食ってしまい、メモリ不足で落ちる(ESP8266)
  DynamicJsonDocument msgs(5000);

  ConfigHookFlags flags;

  std::vector<String> validKeys;

  for (auto &key : config->getKeys()) {
    updateConfigParamForApi(msgs, flags, validKeys, key);
  } 

  for (int i = 0; i < server.args(); i++) {
    String key = server.argName(i);
  
    if (key == "plain") {
      continue;  // ESP8266実装では、POST BODY が plain として渡されるので無視
    }
    // debuglog(String(validKeys.size()) + " k=" + key);
    if (!vectorStringContains(validKeys, key)) {
      apilog("Config SET: [INVALID KEY] " + key);
      msgs.add("[INVALID KEY] " + key);
      flags.configFailed = true;
    }
  }
  
  _reflectConfig(flags);

  // ここ自体は大した容量を必要としないのでこれで十分。ESP8266ではメモリがカツカツなので無駄に増やさないこと
  DynamicJsonDocument json(1000);
  json["command"] = "CONFIG_SET";
  json["success"] = !flags.configFailed;
  json["needReboot"] = flags.needReboot;
  json["msgs"] = msgs;

  if (flags.configFailed) {
    json["message"] = "Some error detected. Check msgs.";
  } else {
    json["message"] = "Don't forget calling config/commit. API";
  }
  json.shrinkToFit();

  String jsonStr;
  serializeJson(json, jsonStr);

  return jsonStr;
}