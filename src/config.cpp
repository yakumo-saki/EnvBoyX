#include <Arduino.h>
#include "FS.h"

#include <ArduinoJson.h>

#include "log.h"
#include "global.h"

#define JSON_BUF 2048

/**
 * とりあえずのデフォルト値をグローバル変数にセットする
 */
void set_default_config_value()
{
  config.ssid = "";
  config.password = "";
  config.mDNS = "ebx";
  config.opMode = OPMODE_DISPLAY;
  config.mhz19b = MHZ_NOUSE;
  config.mhz19bPwmPin = "14";
  #ifdef ESP32
  config.mhz19bRxPin = "32";
  config.mhz19bTxPin = "33";
  #elif defined(ESP8266)
  config.mhz19bRxPin = "14";
  config.mhz19bTxPin = "0";
  #endif

  config.displayFlip = DISPLAY_FLIP_OFF;
  config.displayBrightness = "255";

  config.st7789 = ST7789_NOUSE;
  config.st7789Mode = ST7789_MODE_NORMAL;

  config.mqttBroker = "";
  config.mqttName = "";
}

void print_config() {
  cfglog("SSID: " + config.ssid);
  cfglog("PASS: " + config.password);
  cfglog("mDNS: " + config.mDNS);
  cfglog("opMode: " + config.opMode);
  cfglog("DISPLAY:");
  cfglog("   Flip: " + config.displayFlip);
  cfglog("   Brightness: " + config.displayBrightness);
  cfglog("ST7789: " + config.st7789);
  cfglog("   MODE: " + config.st7789Mode);
  cfglog("use MHZ19B: " + config.mhz19b);
  cfglog("   PWM PIN: " + config.mhz19bPwmPin);
  cfglog("   RX  PIN: " + config.mhz19bRxPin);
  cfglog("   TX  PIN: " + config.mhz19bTxPin);
  cfglog("MQTT Broker: " + config.mqttBroker);
  cfglog("MQTT Name  : " + config.mqttName);
}

void trim_config() {
  config.settingId.trim();
  config.ssid.trim();
  config.password.trim();
  config.mDNS.trim();
  config.opMode.trim();
  config.displayFlip.trim();
  config.displayBrightness.trim();
  config.st7789.trim();
  config.st7789Mode.trim();
  config.mhz19b.trim();
  config.mhz19bPwmPin.trim();
  config.mhz19bRxPin.trim();
  config.mhz19bTxPin.trim();
  config.mqttBroker.trim();
  config.mqttName.trim();
}

void write_config_file(File f) {
  
  trim_config();

  StaticJsonDocument<JSON_BUF> doc;
  doc["settingId"] = SETTING_ID;  // これから書くConfigなので必ず想定しているconfig versionを書く
  doc["ssid"] = config.ssid;
  doc["password"] = config.password;
  doc["mDNS"] = config.mDNS;
  doc["opMode"] = config.opMode;
  doc["displayFlip"] = config.displayFlip;
  doc["displayBrightness"] = config.displayBrightness;
  doc["st7789"] = config.st7789;
  doc["st7789Mode"] = config.st7789Mode;
  doc["mhz19b"] = config.mhz19b;
  doc["mhz19bPwmPin"] = config.mhz19bPwmPin;
  doc["mhz19bRxPin"] = config.mhz19bRxPin;
  doc["mhz19bTxPin"] = config.mhz19bTxPin;
  doc["mqttBroker"] = config.mqttBroker;
  doc["mqttName"] = config.mqttName;

  if (serializeJson(doc, f) == 0) {
    cfglog(F("Failed to write to file"));
  }
}

void set_config_value(String& cfg, StaticJsonDocument<JSON_BUF> &json, String key) {
  // https://arduinojson.org/v6/api/jsonobject/containskey/
  JsonVariant value = json[key];
  if (value.isNull()) {
    cfglog("Config file not contains key:" + key);
    return;
  }
  cfg = value.as<String>();
}

void read_config_file(File f) {

  set_default_config_value(); // とりあえずデフォルト値をロードしておく。

  StaticJsonDocument<JSON_BUF> doc;
  DeserializationError error = deserializeJson(doc, f);
  if (error) {
    config.settingId = "INVALID";
    cfglog(F("Failed to read file or Parse as json failed"));
    return;
  }

  set_config_value(config.settingId ,doc ,"settingId");
  set_config_value(config.ssid ,doc ,"ssid");
  set_config_value(config.password ,doc ,"password");
  set_config_value(config.mDNS ,doc ,"mDNS");
  set_config_value(config.opMode ,doc ,"opMode");
  set_config_value(config.displayFlip,doc  ,"displayFlip");
  set_config_value(config.displayBrightness ,doc ,"displayBrightness");
  set_config_value(config.st7789 ,doc ,"st7789");
  set_config_value(config.st7789Mode ,doc ,"st7789Mode");
  set_config_value(config.mhz19b ,doc ,"mhz19b");
  set_config_value(config.mhz19bPwmPin ,doc ,"mhz19bPwmPin");
  set_config_value(config.mhz19bRxPin ,doc ,"mhz19bRxPin");
  set_config_value(config.mhz19bTxPin ,doc ,"mhz19bTxPin");
  set_config_value(config.mqttBroker ,doc ,"mqttBroker");
  set_config_value(config.mqttName ,doc ,"mqttName");

  print_config();
}