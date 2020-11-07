#include <Arduino.h>
#include <WiFiClient.h>

extern String SETTING_ID;
extern String OPMODE_MQTT;
extern String OPMODE_DISPLAY;
extern const String MHZ_USE_PWM;
extern const String MHZ_USE_UART;
extern const String MHZ_NOUSE;
extern const int MHZ_BAUDRATE;

extern const int I2C_SDA;
extern const int I2C_SCL;

extern const String settings;
extern const String configured_file;

extern const String product_short;
extern const String product;
extern const String ver;
extern const String product_long;

// setup時は、setup用SSID。 normal時は接続先SSID
extern String ssid;
extern String password;
extern String mDNS;
extern String opMode;
extern String use_mhz19b; // MHZ-19Bはコードから接続されているかわからない
extern String mhz19b_pwmpin;
extern String mqttBroker;
extern String mqttName;

// 測定値
extern float lastTemp;
extern float lastHumidity;
extern float lastPressure;
extern float lastLuxFull;
extern float lastLuxIr;
extern int lastPpm;
extern String lastPpmAccuracy;