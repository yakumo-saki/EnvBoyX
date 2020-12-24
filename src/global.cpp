#include <Arduino.h>

// --------------------------------------------------------------------
// プロダクト名定数
// --------------------------------------------------------------------
String product_short = "ebx";
String product = "EnvBoyX";
String ver = "3.0";
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

String SETTING_ID = "EBX.v2.1";
String OPMODE_MQTT = "mqtt";
String OPMODE_DISPLAY = "always";
String MHZ_USE_PWM = "yes_pwm";
String MHZ_USE_UART = "yes_uart";
String MHZ_NOUSE = "no";

// --------------------------------------------------------------------
// 設定値保存
// --------------------------------------------------------------------
// setup時は、setup用SSID。 normal時は接続先SSID
String ssid = "";
String password = "";
String mDNS = "";
String opMode = "";
String use_mhz19b = ""; // MHZ-19Bはコードから接続されているかわからない
String mhz19b_pwmpin = "";
String mqttBroker = "";
String mqttName = "";

// --------------------------------------------------------------------
// センサー値
// --------------------------------------------------------------------
float lastTemp;
float lastHumidity;
float lastPressure;
float lastLuxFull;
float lastLuxIr;
int lastPpm;
String lastPpmAccuracy;