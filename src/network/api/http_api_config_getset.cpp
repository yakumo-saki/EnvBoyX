#include <Arduino.h>

#include <ArduinoJson.h>

#include "log.h"
#include "global.h"
#include "config.h"

#include "mdns_client.h"

#include "network/http_api.h"
#include "network/http_api_util.h"
#include "display/display.h"

extern unsigned int CONF_JSON_SIZE;

extern HTTPWEBSERVER server;

const String CONFIG_NONE = "#**#_##NONE##_#*#";

const std::vector<String> BLOCKED_CONFIG{ConfigNames::SSID, ConfigNames::PASSWORD, ConfigNames::OPMODE};
const std::vector<String> NEED_REBOOT_CONFIG{ConfigNames::OLED_TYPE, ConfigNames::ST7789, ConfigNames::MHZ19B,
                                             ConfigNames::MHZ19B_PWM, ConfigNames::MHZ19B_RX, ConfigNames::MHZ19B_TX};

// 再描画が必要になる設定(Brightnessは再描画しなくても良いのだが面倒なのでこうする)
const std::vector<String> NEED_REDRAW_CONFIG{ConfigNames::ST7789_MODE, ConfigNames::DISPLAY_FLIP, ConfigNames::DISPLAY_BRIGHTNESS};

bool vectorStringContains(std::vector<String> keyArray, String key) {

  for (const auto& k : keyArray) {
    if (k == key) return true;
  }
  return false;
}

struct _config_hook_flags {
  bool needDisplayRedraw = false;
  bool needMDnsRestart = false;
  bool needReboot = false;
  bool configFailed = false;
};

enum class UpdateConfigParamResult {
  OK = 0, REBOOT_REQ = 1, DISPLAY_REDRAW_REQ = 2, MDNS_RESTART_REQ = 3, BLOCKED = 4, ERROR = 8, NOT_SPECIFIED = 16
};

struct UpdateConfigParamResult_t {
  UpdateConfigParamResult result;
  String value;
};

UpdateConfigParamResult_t updateConfigParam(String key, String& config) {

  UpdateConfigParamResult_t ret;

  if (server.hasArg(key)) {
    if (vectorStringContains(BLOCKED_CONFIG, key)) {
      ret.result = UpdateConfigParamResult::BLOCKED;
      ret.value = "";
      return ret;
    }

    String value = server.arg(key);
    config = value; 
    ret.value = value;

    if (vectorStringContains(NEED_REBOOT_CONFIG, key)) {
      ret.result = UpdateConfigParamResult::REBOOT_REQ;
    } else {

      if (vectorStringContains(NEED_REDRAW_CONFIG, key)) {
        ret.result = UpdateConfigParamResult::DISPLAY_REDRAW_REQ;
      } else {
        ret.result = UpdateConfigParamResult::OK;
      }
    }

    if (key == ConfigNames::MDNS) {
      ret.result = UpdateConfigParamResult::MDNS_RESTART_REQ;
    }

    return ret;
  }

  // 指定されたキーはAPIに指定されていない（想定された動作）
  ret.result = UpdateConfigParamResult::NOT_SPECIFIED; 
  ret.value = "";
  return ret;
}

void updateConfigParamForApi(DynamicJsonDocument& jsonArray, _config_hook_flags& flags, String key, String& config) {
  UpdateConfigParamResult_t ret = updateConfigParam(key, config);
  if (ret.result == UpdateConfigParamResult::BLOCKED) {
    flags.configFailed = true;
    jsonArray.add("[ERROR] " + key + " is blocked from running change. use setup mode.");
  } else if (ret.result == UpdateConfigParamResult::OK) {
    jsonArray.add("[OK] " + key + " = " + (ret.value == "" ? "(empty)" : ret.value));
  } else if (ret.result == UpdateConfigParamResult::REBOOT_REQ) {
    flags.needReboot = true;
    jsonArray.add("[OK][REBOOT REQ] " + key + " = " + ret.value);
  } else if (ret.result == UpdateConfigParamResult::DISPLAY_REDRAW_REQ) {
    flags.needDisplayRedraw = true;
    jsonArray.add("[OK][REDRAW] " + key + " = " + ret.value);
  } else if (ret.result == UpdateConfigParamResult::MDNS_RESTART_REQ) {
    flags.needMDnsRestart = true;
    jsonArray.add("[OK][mDNS RESTART] " + key + " = " + ret.value);
  } else if (ret.result == UpdateConfigParamResult::NOT_SPECIFIED) {
    // do nothing
    // debuglog(key + " NOT SPECIFIED");
  } else {
    apilog("MAYBE BUG");
  } 
}

void updateConfigAlerts(DynamicJsonDocument& msgs, _config_hook_flags& flags, String keyPrefix, config_alert_t& alerts) {
  updateConfigParamForApi(msgs, flags, keyPrefix + ".warning1.low", alerts.warning1.low);
  updateConfigParamForApi(msgs, flags, keyPrefix + ".warning1.high", alerts.warning1.high);
  updateConfigParamForApi(msgs, flags, keyPrefix + ".warning2.low", alerts.warning2.low);
  updateConfigParamForApi(msgs, flags, keyPrefix + ".warning2.high", alerts.warning2.high);

  updateConfigParamForApi(msgs, flags, keyPrefix + ".caution1.low", alerts.caution1.low);
  updateConfigParamForApi(msgs, flags, keyPrefix + ".caution1.high", alerts.caution1.high);
  updateConfigParamForApi(msgs, flags, keyPrefix + ".caution2.low", alerts.caution2.low);
  updateConfigParamForApi(msgs, flags, keyPrefix + ".caution2.high", alerts.caution2.high);
}

void _reflectConfig(_config_hook_flags& flags, bool all = false) {
  // debuglog(String(flags.needDisplayRedraw) + String(flags.needMDnsRestart));

  if (all || flags.needDisplayRedraw) {
    apilog("Exec display redraw.");
    disp_redraw_sensor_value_screen();
  }

  if (all || flags.needMDnsRestart) {
    apilog("Exec mDNS restart.");
    mdns_hostname_change(config.mDNS);
  }
}

void reflectConfigAll() {
  _config_hook_flags flags;
  _reflectConfig(flags, true);
}

DynamicJsonDocument updateConfig() {

  _config_hook_flags flags;

  DynamicJsonDocument msgs(4096);

  updateConfigParamForApi(msgs, flags, ConfigNames::SSID, config.ssid);
  updateConfigParamForApi(msgs, flags, ConfigNames::PASSWORD, config.password);
  updateConfigParamForApi(msgs, flags, ConfigNames::MDNS, config.mDNS);
  updateConfigParamForApi(msgs, flags, ConfigNames::OPMODE, config.opMode);
  updateConfigParamForApi(msgs, flags, ConfigNames::OLED_TYPE, config.oledType);
  updateConfigParamForApi(msgs, flags, ConfigNames::DISPLAY_FLIP, config.displayFlip);
  updateConfigParamForApi(msgs, flags, ConfigNames::DISPLAY_BRIGHTNESS, config.displayBrightness);
  updateConfigParamForApi(msgs, flags, ConfigNames::ST7789, config.st7789);
  updateConfigParamForApi(msgs, flags, ConfigNames::ST7789_MODE, config.st7789Mode);
  
  updateConfigParamForApi(msgs, flags, ConfigNames::MHZ19B, config.mhz19b);
  updateConfigParamForApi(msgs, flags, ConfigNames::MHZ19B_PWM, config.mhz19bPwmPin);
  updateConfigParamForApi(msgs, flags, ConfigNames::MHZ19B_RX, config.mhz19bRxPin);
  updateConfigParamForApi(msgs, flags, ConfigNames::MHZ19B_TX, config.mhz19bTxPin);

  updateConfigParamForApi(msgs, flags, ConfigNames::MQTT_BROKER, config.mqttBroker);
  updateConfigParamForApi(msgs, flags, ConfigNames::MQTT_NAME, config.mqttName);

  updateConfigAlerts(msgs, flags, ConfigNames::TEMP_ALERT, config.temperatureAlerts);
  updateConfigAlerts(msgs, flags, ConfigNames::HUMI_ALERT, config.humidityAlerts);
  updateConfigAlerts(msgs, flags, ConfigNames::LUX_ALERT, config.luxAlerts);
  updateConfigAlerts(msgs, flags, ConfigNames::PRES_ALERT, config.pressureAlerts);
  updateConfigAlerts(msgs, flags, ConfigNames::CO2_ALERT, config.co2Alerts);

  _reflectConfig(flags);

  DynamicJsonDocument json(10240);
  json["success"] = !flags.configFailed;
  json["needReboot"] = flags.needReboot;
  json["msgs"] = msgs;

  return json;
}