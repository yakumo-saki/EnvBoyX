#include <Arduino.h>
#include <ESP8266WebServer.h>

// setup時は、setup用SSID。 normal時は接続先SSID
extern String ssid;
extern String password;
extern String mDNS;
extern String opMode;
extern String use_mhz19b; // MHZ-19Bはコードから接続されているかわからない
extern String mhz19b_pwmpin;
extern String mqttBroker;
extern String mqttName;

extern ESP8266WebServer server;

// 測定値
extern float lastTemp;
extern float lastHumidity;
extern float lastPressure;
extern float lastLuxFull;
extern float lastLuxIr;
extern int lastPpm;
