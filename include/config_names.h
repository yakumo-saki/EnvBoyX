#pragma once

#include <Arduino.h>

/**
 * Configで使用される、名称の定数クラス。
 * 実際の値は ConfigValue クラスにある
 */
class ConfigNames {
  public:
    static const String SETTING_ID;
    static const String SSID;
    static const String PASSWORD;
    static const String MDNS;

    static const String OPMODE;

    static const String DISPLAY_FLIP;
    static const String DISPLAY_BRIGHTNESS;
    static const String DISPLAY_RECONFIG;
    static const String DISPLAY_AUTODIM_LUX; // v45
    static const String DISPLAY_AUTODIM_WAIT_SEC; // v45

    static const String OLED_TYPE;

    static const String ST7789;
    static const String ST7789_MODE;
    
    static const String MHZ19B;
    static const String MHZ19B_PWM;
    static const String MHZ19B_RX;
    static const String MHZ19B_TX;
    static const String MHZ19B_ABC;
    
    static const String MQTT_BROKER;
    static const String MQTT_NAME;

    static const String TEMP_ALERT;
    static const String HUMI_ALERT;
    static const String LUX_ALERT;
    static const String PRES_ALERT;
    static const String CO2_ALERT;

    static const String ALERT_WARN1_LO;
    static const String ALERT_WARN1_HI;
    static const String ALERT_WARN2_LO;
    static const String ALERT_WARN2_HI;
    static const String ALERT_CAUTION1_LO;
    static const String ALERT_CAUTION1_HI;
    static const String ALERT_CAUTION2_LO;
    static const String ALERT_CAUTION2_HI;
};
