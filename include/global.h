#include <Arduino.h>
#include <WiFiClient.h>

#include "structs.h"

// --------------------------------------------------------------------
// プロダクト名定数
// --------------------------------------------------------------------
extern const String product_short;
extern const String product;
extern const String ver;
extern const String product_long;

// --------------------------------------------------------------------
// デバイス周りの定数
// --------------------------------------------------------------------
extern const int I2C_SDA;
extern const int I2C_SCL;

extern const int MHZ_BAUDRATE;
extern const byte SSD1306_I2C_ADDR;

// --------------------------------------------------------------------
// 設定ファイル用定数
// --------------------------------------------------------------------
extern const String settings;
extern const String configured_file;

extern const String SETTING_ID;
extern const String OPMODE_MQTT;
extern const String OPMODE_DISPLAY;

extern const String ST7789_USE;
extern const String ST7789_NOUSE;
extern const String ST7789_MODE_BIG;
extern const String ST7789_MODE_NORMAL;

extern const String OLED_SSD1306;
extern const String OLED_SH1106;

extern const String DISPLAY_FLIP_ON;
extern const String DISPLAY_FLIP_OFF;

extern const String MHZ_USE_PWM;
extern const String MHZ_USE_UART;
extern const String MHZ_NOUSE;

extern const String MIME_HTML;
extern const String MIME_TEXT;
extern const String MIME_JSON;

// --------------------------------------------------------------------
// 設定値保存
// --------------------------------------------------------------------
extern config_t config;

// --------------------------------------------------------------------
// センサー値
// --------------------------------------------------------------------
extern sensor_values_t sensorValues;
extern sensor_characters_t sensorCharacters;

// --------------------------------------------------------------------
// 統計情報
// --------------------------------------------------------------------
extern String stasticsJSON;