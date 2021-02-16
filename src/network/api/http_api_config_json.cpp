#include <Arduino.h>

#include <ArduinoJson.h>

#include "log.h"
#include "global.h"
#include "config.h"
#include "network/http_api.h"
#include "network/http_api_util.h"

extern unsigned int CONF_JSON_SIZE;

extern HTTPWEBSERVER server;

const String CFG_NONE = "#**#_##NONE##_#*#";

const std::vector<String> BLOCKED_CONFIG{CFG_SSID, CFG_PASSWORD, CFG_OPMODE};
const std::vector<String> NEED_REBOOT_CONFIG{CFG_MDNS, CFG_OLED_TYPE, CFG_ST7789, CFG_MHZ19B,
                                             CFG_MHZ19B_PWM, CFG_MHZ19B_RX, CFG_MHZ19B_TX};

bool vectorStringContains(std::vector<String> keyArray, String key) {

  for (const auto& k : keyArray) {
    if (k == key) return true;
  }
  return false;
}

String updateConfigParam(String key, String& config) {

  if (server.hasArg(key)) {
    if (vectorStringContains(BLOCKED_CONFIG, key)) {
      return "[ERROR] " + key + " is blocked from running change. use setup mode.";
    }

    String value = server.arg(key);
    config = value; 

    if (vectorStringContains(NEED_REBOOT_CONFIG, key)) {
      return "[REBOOT REQ] " + key + " = " + value;
    } else {
      return "[OK] " + key +  " = " + value;
    }

  }
  return ""; // 指定されたキーはAPIに指定されていない
}

void updateConfigParamForApi(DynamicJsonDocument& jsonArray, String key, String& config) {
  String msg = updateConfigParam(key, config);
  if (msg != "") {
    jsonArray.add(msg);
  }
}

void updateConfigAlerts(DynamicJsonDocument& msgs, String keyPrefix, config_alert_t& alerts) {
  updateConfigParamForApi(msgs, keyPrefix + ".warning1.low", alerts.warning1.low);
  updateConfigParamForApi(msgs, keyPrefix + ".warning1.high", alerts.warning1.low);
  updateConfigParamForApi(msgs, keyPrefix + ".warning2.low", alerts.warning2.low);
  updateConfigParamForApi(msgs, keyPrefix + ".warning2.high", alerts.warning2.high);

  updateConfigParamForApi(msgs, keyPrefix + ".caution1.low", alerts.caution1.low);
  updateConfigParamForApi(msgs, keyPrefix + ".caution1.high", alerts.caution1.high);
  updateConfigParamForApi(msgs, keyPrefix + ".caution2.low", alerts.caution1.low);
  updateConfigParamForApi(msgs, keyPrefix + ".caution2.high", alerts.caution1.high);
}

DynamicJsonDocument updateConfig() {
  DynamicJsonDocument msgs(1024);

  updateConfigParamForApi(msgs, CFG_SSID, config.ssid);
  updateConfigParamForApi(msgs, CFG_PASSWORD, config.password);
  updateConfigParamForApi(msgs, CFG_MDNS, config.mDNS);
  updateConfigParamForApi(msgs, CFG_OPMODE, config.opMode);
  updateConfigParamForApi(msgs, CFG_OLED_TYPE, config.oledType);
  updateConfigParamForApi(msgs, CFG_DISPLAY_FLIP, config.displayFlip);
  updateConfigParamForApi(msgs, CFG_DISPLAY_BRIGHTNESS, config.displayBrightness);
  updateConfigParamForApi(msgs, CFG_ST7789, config.st7789);
  updateConfigParamForApi(msgs, CFG_ST7789_MODE, config.st7789Mode);
  
  updateConfigParamForApi(msgs, CFG_MHZ19B, config.mhz19b);
  updateConfigParamForApi(msgs, CFG_MHZ19B_PWM, config.mhz19bPwmPin);
  updateConfigParamForApi(msgs, CFG_MHZ19B_RX, config.mhz19bRxPin);
  updateConfigParamForApi(msgs, CFG_MHZ19B_TX, config.mhz19bTxPin);

  updateConfigParamForApi(msgs, CFG_MQTT_BROKER, config.mqttBroker);
  updateConfigParamForApi(msgs, CFG_MQTT_NAME, config.mqttName);

  updateConfigAlerts(msgs, CFG_TEMP_ALERT, config.temperatureAlerts);
  updateConfigAlerts(msgs, CFG_HUMI_ALERT, config.humidityAlerts);
  updateConfigAlerts(msgs, CFG_LUX_ALERT, config.luxAlerts);
  updateConfigAlerts(msgs, CFG_PRES_ALERT, config.pressureAlerts);
  updateConfigAlerts(msgs, CFG_CO2_ALERT, config.co2Alerts);

  return msgs;
}