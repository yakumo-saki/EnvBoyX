#include <Arduino.h>
#include "config_values.h"

// configの項目が選択式の場合の文字列表記を列挙する

const String ConfigValues::settings = "/wifi_settings.txt";
const String ConfigValues::configured_file = "/config_ok.txt";

const String ConfigValues::OPMODE_NAME = "opmode";
const String ConfigValues::OPMODE_MQTT = "mqtt";
const String ConfigValues::OPMODE_DISPLAY = "always";

const String ConfigValues::NTP_ON = "yes"; // v47
const String ConfigValues::NTP_OFF = "no"; // v47

const String ConfigValues::NTP_AUTO_DIMMER_ON = "yes"; // v47
const String ConfigValues::NTP_AUTO_DIMMER_OFF = "no"; // v47

const String ConfigValues::OLED_SSD1306 = "SSD";
const String ConfigValues::OLED_SH1106 = "SH";

const String ConfigValues::ST7789_USE = "yes";
const String ConfigValues::ST7789_NOUSE = "no";

const String ConfigValues::ST7789_MODE_BIG = "st7789_BIG";
const String ConfigValues::ST7789_MODE_NORMAL = "st7789_normal";

const String ConfigValues::DISPLAY_FLIP_ON = "yes";
const String ConfigValues::DISPLAY_FLIP_OFF = "no";

const String ConfigValues::DISPLAY_RECONFIG_SKIP = "skip";
const String ConfigValues::DISPLAY_RECONFIG_ON = "on";

const String ConfigValues::MHZ_USE_PWM = "yes_pwm";
const String ConfigValues::MHZ_USE_UART = "yes_uart";
const String ConfigValues::MHZ_NOUSE = "no";

const String ConfigValues::MHZ_ABC_ON = "yes";
const String ConfigValues::MHZ_ABC_OFF = "no";
