#include <Arduino.h>
#include <WiFiClient.h>

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
extern const String MHZ_USE_PWM;
extern const String MHZ_USE_UART;
extern const String MHZ_NOUSE;

// --------------------------------------------------------------------
// 設定値保存
// --------------------------------------------------------------------
// setup時は、setup用SSID。 normal時は接続先SSID
extern String ssid;
extern String password;
extern String mDNS;
extern String opMode;
extern String use_mhz19b; // MHZ-19Bはコードから接続されているかわからない
extern String mhz19b_pwmpin;
extern String mqttBroker;
extern String mqttName;

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

