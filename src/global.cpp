#include <Arduino.h>
#include <ArduinoJson.h>
#include "structs.h"
#include "TimerCall.h"
#include "ConfigClass.h"


// --------------------------------------------------------------------
// プロダクト名定数
// --------------------------------------------------------------------

// ebx
extern const String product_short = "EBX";

// Enable debug function
extern const bool DEBUG_BUILD = false;

// EnvBoyX
// note: Also referenced by create_release.sh
extern const String product = "EnvBoyX";
extern const String ver = "48";      // Major
extern const String minorVer = "0";  // Bugfix
extern const String ver_long = " Ver." + ver + "." + minorVer;

// Setting version (version number when settings changed)
extern const String SETTING_ID = "EBXCFG.v48";

// EnvBoyX Ver.53.0
extern const String product_long = product + " " + (DEBUG_BUILD ? " Dev." + ver + "." + minorVer : ver_long);

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
// センサー値グローバル
// --------------------------------------------------------------------
// センサー値（グローバル）
sensor_values_t sensorValues;

// センサー値差分（グローバル）
sensor_delta_values_t deltaValues;

// 測定できる項目（グローバル）
sensor_characters_t sensorCharacters;

// --------------------------------------------------------------------
// 統計情報
// --------------------------------------------------------------------
String stasticsJSON;

// --------------------------------------------------------------------
// Config
// --------------------------------------------------------------------
Config *config;

// --------------------------------------------------------------------
// タイマー
// --------------------------------------------------------------------
TimerCall timer = TimerCall();

// --------------------------------------------------------------------
// 動作モード
// --------------------------------------------------------------------
extern const int OPERATING_MODE_NORMAL = 2;
extern const int OPERATING_MODE_SETUP = 1;
int OPERATING_MODE = 0;
