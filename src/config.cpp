#include <Arduino.h>

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
  config.mqttBroker = "";
  config.mqttName = "";
}

void print_config() {
  cfglog("SSID: " + config.ssid);
  cfglog("PASS: " + config.password);
  cfglog("mDNS: " + config.mDNS);
  cfglog("opMode: " + config.opMode);
  cfglog("ST7789: " + config.st7789);
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
  config.use_mhz19b.trim();
  config.mhz19b_pwmpin.trim();
  config.mhz19b_rxpin.trim();
  config.mhz19b_txpin.trim();
  config.mqttBroker.trim();
  config.mqttName.trim();

}