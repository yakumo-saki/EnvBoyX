#include <Arduino.h>
#include <ArduinoJson.h>
#include "structs.h"
#include "TimerCall.h"

// --------------------------------------------------------------------
// プロダクト名定数
// --------------------------------------------------------------------
// ebx
extern const String product_short = "EBX";
// EnvBoyX
extern const String product = "EnvBoyX";
extern const String ver = "44";      // Major
extern const String minorVer = "0";  // Bugfix
extern const String ver_long = " Ver." + ver + "." + minorVer;

extern const bool DEBUG_BUILD = true;

// EnvBoyX Ver.53.0
extern const String product_long = product + " " + ver_long;


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

extern const byte SSD1306_I2C_ADDR = 0x3c;
extern const int MHZ_BAUDRATE = 9600;

// MH-Z19B の測定レンジ。 2000 or 5000. これ以上の値のco2ppmはエラー扱い
extern const int MHZ_RANGE = 2000;

// --------------------------------------------------------------------
// データ埋め込み .rodata から取得するもの
// --------------------------------------------------------------------
extern const char DATA_STYLE_CSS[] asm("_binary_src_asmdata_style_css_start");


// --------------------------------------------------------------------
// 設定ファイル用定数
// --------------------------------------------------------------------
extern const unsigned int CONF_JSON_SIZE = 2000;

// --------------------------------------------------------------------
// ただの定数
// --------------------------------------------------------------------

extern const String MIME_HTML = "text/html";
extern const String MIME_TEXT = "text/plain";
extern const String MIME_JSON = "application/json";

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
