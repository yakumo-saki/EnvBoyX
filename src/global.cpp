#include <Arduino.h>
#include <ESP8266WebServer.h>

// 定数
String SETTING_ID = "EBX.v20";
String OPMODE_MQTT = "mqtt";
String OPMODE_DISPLAY = "always";
String MHZ_USE_PWM = "yes_pwm";
String MHZ_USE_UART = "yes_uart";
String MHZ_NOUSE = "no";
int MHZ_BAUDRATE = 9600;

// Wi-Fi設定保存ファイル
String settings = "/wifi_settings.txt";
String configured_file = "/config_ok.txt";

String product_short = "ebx";
String product = "EnvBoyX";
String ver = "2.0";
String product_long = product + " Ver." + ver;

ESP8266WebServer server(80);

// setup時は、setup用SSID。 normal時は接続先SSID
String ssid = "";
String password = "";
String mDNS = "";
String opMode = "";
String use_mhz19b = ""; // MHZ-19Bはコードから接続されているかわからない
String mhz19b_pwmpin = "";
String mhz19b_rxpin = "";
String mhz19b_txpin = "";
String mqttBroker = "";
String mqttName = "";
