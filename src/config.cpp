#include <Arduino.h>

#include "log.h"
#include "global.h"

/**
 * とりあえずのデフォルト値をグローバル変数にセットする
 */
void set_default_config_value()
{
  ssid = "";
  password = "";
  mDNS = "";
  opMode = OPMODE_DISPLAY;
  use_mhz19b = MHZ_NOUSE;
  mhz19b_pwmpin = "14";
  mqttBroker = "";
  mqttName = "";
}
