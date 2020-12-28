#include <Arduino.h>
#include "structs.h"

// --------------------------------------------------------------------
// プロダクト名定数
// --------------------------------------------------------------------
// ebx
String product_short = "ebx";
// EnvBoyX
String product = "EnvBoyX";
String ver = "3.1";
// EnvBoyX Ver.3.0
String product_long = product + " Ver." + ver;

// --------------------------------------------------------------------
// デバイス周りの定数
// --------------------------------------------------------------------
#ifdef ESP32
extern const int I2C_SDA = 21;
extern const int I2C_SCL = 22;
#elif defined(ESP8266)
extern const int I2C_SDA = 5;
extern const int I2C_SCL = 4;
#endif

extern const byte SSD1306_I2C_ADDR = 0x3c;
extern const int MHZ_BAUDRATE = 9600;

// --------------------------------------------------------------------
// 設定ファイル用定数
// --------------------------------------------------------------------
String settings = "/wifi_settings.txt";
String configured_file = "/config_ok.txt";

// 次からv3 -> ただの連番にして単純カウントアップにする
String SETTING_ID = "EBXCFG.v3b1";
String OPMODE_MQTT = "mqtt";
String OPMODE_DISPLAY = "always";

String ST7789_USE = "yes";
String ST7789_NOUSE = "no";

String MHZ_USE_PWM = "yes_pwm";
String MHZ_USE_UART = "yes_uart";
String MHZ_NOUSE = "no";

// --------------------------------------------------------------------
// 設定値保存
// --------------------------------------------------------------------
config_t config;

// --------------------------------------------------------------------
// センサー値
// --------------------------------------------------------------------
sensor_values_t sensorValues;
