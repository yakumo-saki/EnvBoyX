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
extern const String ver = "40";
// EnvBoyX Ver.3.0
extern const String product_long = product + " Ver." + ver;

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

// --------------------------------------------------------------------
// データ埋め込み .rodata から取得するもの
// --------------------------------------------------------------------
extern const char DATA_STYLE_CSS[] asm("_binary_src_asmdata_style_css_start");


// --------------------------------------------------------------------
// 設定ファイル用定数
// --------------------------------------------------------------------
extern const String settings = "/wifi_settings.txt";
extern const String configured_file = "/config_ok.txt";

extern const String SETTING_ID = "EBXCFG.v12";
extern const String OPMODE_MQTT = "mqtt";
extern const String OPMODE_DISPLAY = "always";

extern const String OLED_SSD1306 = "SSD";
extern const String OLED_SH1106 = "SH";

extern const String ST7789_USE = "yes";
extern const String ST7789_NOUSE = "no";
extern const String ST7789_MODE_BIG = "st7789_BIG";
extern const String ST7789_MODE_NORMAL = "st7789_normal";

// SSD1306 , ST7789 Flip
extern const String DISPLAY_FLIP_ON = "yes";
extern const String DISPLAY_FLIP_OFF = "no";

extern const String MHZ_USE_PWM = "yes_pwm";
extern const String MHZ_USE_UART = "yes_uart";
extern const String MHZ_NOUSE = "no";

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
sensor_characters_t sensorCharacters;

// --------------------------------------------------------------------
// 統計情報
// --------------------------------------------------------------------
String stasticsJSON;

// --------------------------------------------------------------------
// タイマー
// --------------------------------------------------------------------
TimerCall timer = TimerCall();
