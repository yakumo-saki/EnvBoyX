#include <Arduino.h>
#include <ArduinoJson.h>
#include "structs.h"
#include "TimerCall.h"

// --------------------------------------------------------------------
// プロダクト名定数
// --------------------------------------------------------------------

// ebx
extern const String product_short = "EBX";

// Enable debug function
extern const bool DEBUG_BUILD = false;

// EnvBoyX
extern const String product = "EnvBoyX";
extern const String ver = "44";      // Major
extern const String minorVer = "0";  // Bugfix
extern const String ver_long = " Ver." + ver + "." + minorVer;

// Setting version
extern const String SETTING_ID = "EBXCFG.v44";

// EnvBoyX Ver.53.0
extern const String product_long = product + " " + ver_long + (DEBUG_BUILD ? "(DEBUG BUILD)" : "");


// --------------------------------------------------------------------
// デバイス周りの定数
// --------------------------------------------------------------------
#ifdef ESP32
// ESP32 SDA
extern const int I2C_SDA = 21;
// ESP32 SCLu8g2Flip
extern const int I2C_SCL = 22;
#elif defined(ESP8266)
// ESP8266 SDA
extern const int I2C_SDA = 5;
// ESP8266 SCL
extern const int I2C_SCL = 4;
#endif

// --------------------------------------------------------------------
// 設定値保存
// --------------------------------------------------------------------
config_t config;

// --------------------------------------------------------------------
// センサー値
// --------------------------------------------------------------------
sensor_values_t sensorValues;
sensor_delta_values_t deltaValues;
sensor_characters_t sensorCharacters;

// --------------------------------------------------------------------
// 統計情報
// --------------------------------------------------------------------
String stasticsJSON;

// --------------------------------------------------------------------
// タイマー
// --------------------------------------------------------------------
TimerCall timer = TimerCall();
