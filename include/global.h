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

extern String SETTING_ID;
extern String OPMODE_MQTT;
extern String OPMODE_DISPLAY;

extern String ST7789_USE;
extern String ST7789_NOUSE;

extern String MHZ_USE_PWM;
extern String MHZ_USE_UART;
extern String MHZ_NOUSE;

// --------------------------------------------------------------------
// 設定値保存
// --------------------------------------------------------------------
extern config_t config;

// --------------------------------------------------------------------
// センサー値
// --------------------------------------------------------------------
extern float lastTemp;
extern float lastHumidity;
extern float lastPressure;
extern float lastLuxFull;
extern float lastLuxIr;
extern int lastPpm;
extern String lastPpmAccuracy;

