#include <Arduino.h>
#include <WiFiClient.h>

#include "global_MimeType.h"

#include "config_values.h"
#include "config_names.h"
#include "ConfigClass.h"

#include "structs.h"

// --------------------------------------------------------------------
// プロダクト名定数
// --------------------------------------------------------------------
extern const String product_short;
extern const String product;
extern const String ver;
extern const String minorVer;
extern const String product_long;

extern const String SETTING_ID;

extern const bool DEBUG_BUILD;

// SETUPモード OR NORMALモード
extern const int OPERATING_MODE_NORMAL;
extern const int OPERATING_MODE_SETUP;
extern int OPERATING_MODE;

// --------------------------------------------------------------------
// デバイス周りの定数
// --------------------------------------------------------------------
extern const int I2C_SDA;
extern const int I2C_SCL;

extern const int MHZ_BAUDRATE;
extern const byte SSD1306_I2C_ADDR;

extern const int MHZ_RANGE;

// --------------------------------------------------------------------
// 設定値保存
// --------------------------------------------------------------------
extern Config *config;

// --------------------------------------------------------------------
// センサー値関連
// --------------------------------------------------------------------

// センサー値
extern sensor_values_t sensorValues;

// センサー値のdelta（差分）
extern sensor_delta_values_t deltaValues;

// センサーが存在しているか否かのフラグ
extern sensor_characters_t sensorCharacters;

// --------------------------------------------------------------------
// 統計情報
// --------------------------------------------------------------------
extern String stasticsJSON;