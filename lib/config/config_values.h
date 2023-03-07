#pragma once

#include <Arduino.h>

/**
 * Configで使用する値の定数クラス。
 * HTML上の name要素だったり、JSONのキー（＝APIのキー）に使われる名前は ConfigNamesにある
 */
class ConfigValues {
  public:
    static const String settings;
    static const String configured_file;

    static const String SETTING_ID;

    static const String OPMODE_NAME;
    static const String OPMODE_MQTT;
    static const String OPMODE_DISPLAY;

    static const String NTP_ON; // v47
    static const String NTP_OFF; // v47

    static const String NTP_AUTO_DIMMER_ON; // v47
    static const String NTP_AUTO_DIMMER_OFF; // v47

    static const String ST7789_USE;
    static const String ST7789_NOUSE;

    static const String ST7789_MODE_BIG;
    static const String ST7789_MODE_NORMAL;

    static const String OLED_SSD1306;
    static const String OLED_SH1106;

    static const String DISPLAY_FLIP_ON;
    static const String DISPLAY_FLIP_OFF;

    static const String DISPLAY_RECONFIG_SKIP;
    static const String DISPLAY_RECONFIG_ON;

    static const String MHZ_USE_PWM;
    static const String MHZ_USE_UART;
    static const String MHZ_NOUSE;

    static const String MHZ_ABC_ON;
    static const String MHZ_ABC_OFF;
};
