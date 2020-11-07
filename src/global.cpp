#include <Arduino.h>

// 定数
String SETTING_ID = "EBX.v2.1";
String OPMODE_MQTT = "mqtt";
String OPMODE_DISPLAY = "always";
String MHZ_USE_PWM = "yes_pwm";
String MHZ_USE_UART = "yes_uart";
String MHZ_NOUSE = "no";
extern const int MHZ_BAUDRATE = 9600;

#ifdef ESP32
extern const int I2C_SDA = 21;
extern const int I2C_SCL = 22;
#elif defined(ESP8266)
extern const int I2C_SDA = 5;
extern const int I2C_SCL = 4;
#endif

// Wi-Fi設定保存ファイル
String settings = "/wifi_settings.txt";
String configured_file = "/config_ok.txt";

String product_short = "ebx";
String product = "EnvBoyX";
String ver = "2.1";
String product_long = product + " Ver." + ver;

// setup時は、setup用SSID。 normal時は接続先SSID
String ssid = "";
String password = "";
String mDNS = "";
String opMode = "";
String use_mhz19b = ""; // MHZ-19Bはコードから接続されているかわからない
String mhz19b_pwmpin = "";
String mqttBroker = "";
String mqttName = "";

// 最後に取得した値
float lastTemp;
float lastHumidity;
float lastPressure;
float lastLuxFull;
float lastLuxIr;
int lastPpm;
