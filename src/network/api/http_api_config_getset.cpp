#include <Arduino.h>

#include <ArduinoJson.h>

#include "log.h"
#include "global.h"
#include "config.h"
#include "ConfigClass.h"
#include "utils.h"

#include "mdns_client.h"

#include "network/http_api.h"
#include "network/http_api_util.h"
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

  if (setResult != ConfigSetResult::OK) {
    cfglog("ERROR " + key);
  }

  // 有効な設定名だったので記録しておく
  validKeys.push_back(key);

  // 設定後の処理を取得
  ConfigMeta meta = config->getMeta(key, true);

  if (meta.flags == RunningConfigChangeFlags::BLOCKED) {
    flags.configFailed = true;
    msgArray.add("[ERROR] " + key + " is blocked from running change. use setup mode.");
  } else if (meta.flags == RunningConfigChangeFlags::REBOOT_REQ) {
    flags.needReboot = true;
    msgArray.add("[OK][REBOOT REQ] " + key + " = " + value);
  } else if (meta.flags == RunningConfigChangeFlags::DISPLAY_REDRAW_REQ) {
    flags.needDisplayRedraw = true;
    msgArray.add("[OK][REDRAW] " + key + " = " + value);
  } else if (meta.flags == RunningConfigChangeFlags::MDNS_RESTART_REQ) {
    flags.needMDnsRestart = true;
    msgArray.add("[OK][mDNS RESTART] " + key + " = " + value);
  } else if (meta.flags == RunningConfigChangeFlags::OK) {
    msgArray.add("[OK] " + key + " = " + (value == "" ? "(empty)" : value));
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
    mdns_hostname_change(config->get(ConfigNames::MDNS));
  }
}

// revertしたときに画面書き換え等を全部実行する
void reflectConfigAll() {
  ConfigHookFlags flags;
  _reflectConfig(flags, true);
}

// Config SET API のエントリポイント
DynamicJsonDocument updateConfig() {

  DynamicJsonDocument msgs(10240);

  ConfigHookFlags flags;

  std::vector<String> validKeys;

  for (auto &key : config->getKeys()) {
    updateConfigParamForApi(msgs, flags, validKeys, key);
  } 

  for (int i = 0; i < server.args(); i++) {
    String key = server.argName(i);

    if (vectorStringContains(validKeys, key) == false) {
      msgs.add("[INVALID KEY] " + key);
    }
  }
  
  _reflectConfig(flags);

  DynamicJsonDocument json(10240);
  json["success"] = !flags.configFailed;
  json["needReboot"] = flags.needReboot;
  json["msgs"] = msgs;
  json["message"] = "Don't forget calling /config/commit before restart.";

  return json;
}