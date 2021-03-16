#pragma once

#include <Arduino.h>

/**
 * Configで使用する定数クラス。
 * 〜〜_NAME はHTML上の name要素だったり、APIのキーに使われる
 */
class ConfigValues {
  public:
    static const String settings;
    static const String configured_file;

    static const String SETTING_ID;

    static const String OPMODE_NAME;
    static const String OPMODE_MQTT;
    static const String OPMODE_DISPLAY;

    static const String ST7789_NAME;
    static const String ST7789_USE;
    static const String ST7789_NOUSE;

    static const String ST7789_MODE_NAME;
    static const String ST7789_MODE_BIG;
    static const String ST7789_MODE_NORMAL;

    static const String OLED_NAME;
    static const String OLED_SSD1306;
    static const String OLED_SH1106;

    static const String DISPLAY_FLIP_NAME;
    static const String DISPLAY_FLIP_ON;
    static const String DISPLAY_FLIP_OFF;

    static const String DISPLAY_RECONFIG_NAME;
    static const String DISPLAY_RECONFIG_SKIP;
    static const String DISPLAY_RECONFIG_ON;

    static const String MHZ_NAME;
    static const String MHZ_USE_PWM;
    static const String MHZ_USE_UART;
    static const String MHZ_NOUSE;

    static const String MHZ_ABC_NAME;
    static const String MHZ_ABC_ON;
    static const String MHZ_ABC_OFF;
};