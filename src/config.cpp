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
