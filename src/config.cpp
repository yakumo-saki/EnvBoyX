#include <Arduino.h>
#include "FS.h"

#include "log.h"
#include "global.h"

/**
 * とりあえずのデフォルト値をグローバル変数にセットする
 */
void set_default_config_value()
{
  config.ssid = "";
  config.password = "";
  config.mDNS = "ebx";
  config.opMode = OPMODE_DISPLAY;
  config.use_mhz19b = MHZ_NOUSE;
  config.mhz19b_pwmpin = "14";
  #ifdef ESP32
  config.mhz19b_rxpin = "32";
  config.mhz19b_txpin = "33";
  #elif defined(ESP8266)
  config.mhz19b_rxpin = "14";
  config.mhz19b_txpin = "0";
  #endif

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
  cfglog("ST7789: " + config.st7789);
  cfglog("   MODE: " + config.st7789Mode);
  cfglog("use MHZ19B: " + config.use_mhz19b);
  cfglog("   PWM PIN: " + config.mhz19b_pwmpin);
  cfglog("   RX  PIN: " + config.mhz19b_rxpin);
  cfglog("   TX  PIN: " + config.mhz19b_txpin);
  cfglog("MQTT Broker: " + config.mqttBroker);
  cfglog("MQTT Name  : " + config.mqttName);
}

void trim_config() {
  config.settingId.trim();
  config.ssid.trim();
  config.password.trim();
  config.mDNS.trim();
  config.opMode.trim();
  config.st7789.trim();
  config.st7789Mode.trim();
  config.use_mhz19b.trim();
  config.mhz19b_pwmpin.trim();
  config.mhz19b_rxpin.trim();
  config.mhz19b_txpin.trim();
  config.mqttBroker.trim();
  config.mqttName.trim();
}

void write_config_file(File f) {
  f.println(String(SETTING_ID));
  f.println(config.ssid);
  f.println(config.password);
  f.println(config.mDNS);
  f.println(config.opMode);
  f.println(config.st7789);
  f.println(config.st7789Mode);
  f.println(config.use_mhz19b);
  f.println(config.mhz19b_pwmpin);
  f.println(config.mhz19b_rxpin);
  f.println(config.mhz19b_txpin);
  f.println(config.mqttBroker);
  f.println(config.mqttName);
}

void read_config_file(File f) {
  String settingId = f.readStringUntil('\n'); // 使わない
  if (settingId != SETTING_ID) {
    cfglog("Setting id is not equal. file=" + settingId + " code=" + SETTING_ID);
  }
  config.ssid = f.readStringUntil('\n');
  config.password = f.readStringUntil('\n');
  config.mDNS = f.readStringUntil('\n');
  config.opMode = f.readStringUntil('\n');
  config.st7789 = f.readStringUntil('\n');
  config.st7789Mode = f.readStringUntil('\n');
  config.use_mhz19b = f.readStringUntil('\n');
  config.mhz19b_pwmpin = f.readStringUntil('\n');
  config.mhz19b_rxpin = f.readStringUntil('\n');
  config.mhz19b_txpin = f.readStringUntil('\n');
  config.mqttBroker = f.readStringUntil('\n');
  config.mqttName = f.readStringUntil('\n');
}