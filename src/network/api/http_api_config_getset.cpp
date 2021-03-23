#include <Arduino.h>

#include <ArduinoJson.h>

#include "log.h"
#include "global.h"
#include "config.h"
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

enum class UpdateConfigParamResult {
  OK = 0, REBOOT_REQ = 1, DISPLAY_REDRAW_REQ = 2, MDNS_RESTART_REQ = 3, BLOCKED = 4, ERROR = 8, NOT_SPECIFIED = 16
};

struct UpdateConfigParamResult_t {
  UpdateConfigParamResult result = UpdateConfigParamResult::ERROR;
  String value = "";
};

/**
 * 指定された項目の値を server から取り出し、追加で必要な反映処理を判定して返す
 */
UpdateConfigParamResult_t _updateConfigParam(String key, String& config) {

  // これを一番上に持っていくと、ConfigNamesの値がセットされる前に処理されてしまい、空文字列になってしまう。

  std::vector<String> BLOCKED_CONFIG = {ConfigNames::SSID, ConfigNames::PASSWORD, ConfigNames::OPMODE};
  std::vector<String> NEED_REBOOT_CONFIG = {ConfigNames::OLED_TYPE, ConfigNames::ST7789, 
                                            ConfigNames::MHZ19B, ConfigNames::MHZ19B_PWM,
                                            ConfigNames::MHZ19B_RX, ConfigNames::MHZ19B_TX};

  // 再描画が必要になる設定(Brightnessは再描画しなくても良いのだが面倒なのでこうする)
  std::vector<String> NEED_REDRAW_CONFIG = {ConfigNames::ST7789_MODE, ConfigNames::DISPLAY_FLIP,
                                            ConfigNames::DISPLAY_BRIGHTNESS};

  //

  UpdateConfigParamResult_t ret;

  if (!server.hasArg(key)) {
    ret.result = UpdateConfigParamResult::NOT_SPECIFIED; 
    ret.value = "";
    return ret;
  }

  if (vectorStringContains(BLOCKED_CONFIG, key)) {
    ret.result = UpdateConfigParamResult::BLOCKED;
    ret.value = "";
    return ret;
  }

  String value = server.arg(key);
  config = value; 
  ret.value = value;

  // ここから先はOK
  if (vectorStringContains(NEED_REBOOT_CONFIG, key)) {
    ret.result = UpdateConfigParamResult::REBOOT_REQ;
  } else if (vectorStringContains(NEED_REDRAW_CONFIG, key)) {
    ret.result = UpdateConfigParamResult::DISPLAY_REDRAW_REQ;
  } else if (key == ConfigNames::MDNS) {
    ret.result = UpdateConfigParamResult::MDNS_RESTART_REQ;
  } else {
    ret.result = UpdateConfigParamResult::OK;
  }

  return ret;
}

// Config set API の処理
// updateConfigParam　の　API用ラッパー
void updateConfigParamForApi(DynamicJsonDocument& msgArray, ConfigHookFlags &flags, std::vector<String>& validKeys, String key, String& config) {
  
  UpdateConfigParamResult_t ret = _updateConfigParam(key, config);

  // int retnum = static_cast<int>(ret.result);
  // debuglog("RET RESULT = " + String(retnum));

  if (ret.result == UpdateConfigParamResult::NOT_SPECIFIED) {
    return;
  } else if (ret.result == UpdateConfigParamResult::ERROR) {
    debuglog("ERROR " + key);
  }

  // 有効な設定名だったので記録しておく
  validKeys.push_back(key);

  if (ret.result == UpdateConfigParamResult::BLOCKED) {
    flags.configFailed = true;
    msgArray.add("[ERROR] " + key + " is blocked from running change. use setup mode.");
  } else if (ret.result == UpdateConfigParamResult::REBOOT_REQ) {
    flags.needReboot = true;
    msgArray.add("[OK][REBOOT REQ] " + key + " = " + ret.value);
  } else if (ret.result == UpdateConfigParamResult::DISPLAY_REDRAW_REQ) {
    flags.needDisplayRedraw = true;
    msgArray.add("[OK][REDRAW] " + key + " = " + ret.value);
  } else if (ret.result == UpdateConfigParamResult::MDNS_RESTART_REQ) {
    flags.needMDnsRestart = true;
    msgArray.add("[OK][mDNS RESTART] " + key + " = " + ret.value);
  } else if (ret.result == UpdateConfigParamResult::OK) {
    msgArray.add("[OK] " + key + " = " + (ret.value == "" ? "(empty)" : ret.value));
  } else {
    apilog("MAYBE BUG");
  } 
}

void updateConfigAlerts(DynamicJsonDocument& msgs, ConfigHookFlags& flags, std::vector<String>& validKeys, String prefix, config_alert_t& alerts) {
  updateConfigParamForApi(msgs, flags, validKeys, prefix + "." + ConfigNames::ALERT_WARN1_LO, alerts.warning1.low);
  updateConfigParamForApi(msgs, flags, validKeys, prefix + "." + ConfigNames::ALERT_WARN1_HI, alerts.warning1.high);
  updateConfigParamForApi(msgs, flags, validKeys, prefix + "." + ConfigNames::ALERT_WARN2_LO, alerts.warning2.low);
  updateConfigParamForApi(msgs, flags, validKeys, prefix + "." + ConfigNames::ALERT_WARN2_HI, alerts.warning2.high);

  updateConfigParamForApi(msgs, flags, validKeys, prefix + "." + ConfigNames::ALERT_CAUTION1_LO, alerts.caution1.low);
  updateConfigParamForApi(msgs, flags, validKeys, prefix + "." + ConfigNames::ALERT_CAUTION1_HI, alerts.caution1.high);
  updateConfigParamForApi(msgs, flags, validKeys, prefix + "." + ConfigNames::ALERT_CAUTION2_LO, alerts.caution2.low);
  updateConfigParamForApi(msgs, flags, validKeys, prefix + "." + ConfigNames::ALERT_CAUTION2_HI, alerts.caution2.high);
}

void _reflectConfig(ConfigHookFlags& flags, bool all = false) {

  if (all || flags.needDisplayRedraw) {
    apilog("Exec display redraw.");
    disp_redraw_sensor_value_screen();
  }

  if (all || flags.needMDnsRestart) {
    apilog("Exec mDNS restart.");
    mdns_hostname_change(config.mDNS);
  }
}

// revertしたときに画面書き換え等を全部実行する
void reflectConfigAll() {
  ConfigHookFlags flags;
  _reflectConfig(flags, true);
}

// Config SET API のエントリポイント
DynamicJsonDocument updateConfig() {

  DynamicJsonDocument msgs(4096);

  ConfigHookFlags flags;

  std::vector<String> validKeys;

  updateConfigParamForApi(msgs, flags, validKeys, ConfigNames::SSID, config.ssid);
  updateConfigParamForApi(msgs, flags, validKeys, ConfigNames::PASSWORD, config.password);
  
  updateConfigParamForApi(msgs, flags, validKeys, ConfigNames::MDNS, config.mDNS);
  updateConfigParamForApi(msgs, flags, validKeys, ConfigNames::OPMODE, config.opMode);
  updateConfigParamForApi(msgs, flags, validKeys, ConfigNames::OLED_TYPE, config.oledType);
  
  updateConfigParamForApi(msgs, flags, validKeys, ConfigNames::DISPLAY_FLIP, config.displayFlip);
  updateConfigParamForApi(msgs, flags, validKeys, ConfigNames::DISPLAY_BRIGHTNESS, config.displayBrightness);
  updateConfigParamForApi(msgs, flags, validKeys, ConfigNames::DISPLAY_RECONFIG, config.displayWaitForReconfigure);

  updateConfigParamForApi(msgs, flags, validKeys, ConfigNames::ST7789, config.st7789);
  updateConfigParamForApi(msgs, flags, validKeys, ConfigNames::ST7789_MODE, config.st7789Mode);
  
  updateConfigParamForApi(msgs, flags, validKeys, ConfigNames::MHZ19B, config.mhz19b);
  updateConfigParamForApi(msgs, flags, validKeys, ConfigNames::MHZ19B_PWM, config.mhz19bPwmPin);
  updateConfigParamForApi(msgs, flags, validKeys, ConfigNames::MHZ19B_RX, config.mhz19bRxPin);
  updateConfigParamForApi(msgs, flags, validKeys, ConfigNames::MHZ19B_TX, config.mhz19bTxPin);
  updateConfigParamForApi(msgs, flags, validKeys, ConfigNames::MHZ19B_ABC, config.mhz19bABC);
  
  updateConfigParamForApi(msgs, flags, validKeys, ConfigNames::MQTT_BROKER, config.mqttBroker);
  updateConfigParamForApi(msgs, flags, validKeys, ConfigNames::MQTT_NAME, config.mqttName);

  updateConfigAlerts(msgs, flags, validKeys, ConfigNames::TEMP_ALERT, config.temperatureAlerts);
  updateConfigAlerts(msgs, flags, validKeys, ConfigNames::HUMI_ALERT, config.humidityAlerts);
  updateConfigAlerts(msgs, flags, validKeys, ConfigNames::LUX_ALERT, config.luxAlerts);
  updateConfigAlerts(msgs, flags, validKeys, ConfigNames::PRES_ALERT, config.pressureAlerts);
  updateConfigAlerts(msgs, flags, validKeys, ConfigNames::CO2_ALERT, config.co2Alerts);

  for (int i = 0; i < server.args(); i++)
  {
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