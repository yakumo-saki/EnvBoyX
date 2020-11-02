/**
 * 
 * EnvBoy X main
 * 
 */
#include <Arduino.h>

#include <MQTTClient.h>
#include <MQTT.h>

#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>

#include <WiFiClient.h> 

#include <Wire.h>
#include <SSD1306.h>

#include <FS.h>
#include <LittleFS.h>
 
ESP8266WebServer server(80);

// Wi-Fi設定保存ファイル
String settings = "/wifi_settings.txt";
String configured_file = "/config_ok.txt";

String product_short = "ebx";
String product = "EnvBoyX";
String ver = "1.5";
String product_long = product + " Ver." + ver;

#define SETTING_ID "EBX.r4"

#define OPMODE_MQTT "mqtt"
#define OPMODE_DISPLAY "always"
#define MHZ_USE_PWM "yes_pwm"
#define MHZ_USE_UART "yes_uart"
#define MHZ_NOUSE "no"

// setup時は、setup用SSID。 normal時は接続先SSID
String ssid = "";
String password = "";
String mDNS = "";
String opMode = "";
String use_mhz19b = ""; // MHZ-19Bはコードから接続されているかわからない
String mhz19b_pwmpin = "";
String mqttBroker = "";
String mqttName = "";

bool isNormal = false;

void setup()
{
  Serial.begin(115200);
  Serial.println(""); // 1行目にゴミが出るので改行しないと読めない
  Serial.println("");
  Serial.println("Starting " + product);

  // Init I2C Serial
  Wire.begin(5, 4);

  LittleFS.begin();
  delay(50);

  // ファイルが存在しないか、バージョン違いであればセットアップモード
  isNormal = false;

  if (!LittleFS.exists(configured_file)) {
    // reconfigure用ファイルがなければセットアップモード
    // wait for reconfigure でリセットされたとき。
    Serial.println("configured_file not found. goto setup mode");
    isNormal = false;
  } else if (LittleFS.exists(settings)) {
    File f = LittleFS.open(settings, "r");
    String settingId = f.readStringUntil('\n');   
    settingId.trim();
    f.close();

    if (String(SETTING_ID).equals(settingId)) {
      Serial.println("SETTING_ID verified.");
      isNormal = true;
    } else {
      Serial.println("SETTING_ID NOT match!");      
      isNormal = false;
    }
  }

  if (!isNormal) {
    Serial.println("Entering setup mode.");
    setup_setupmode(); 
  } else {
    Serial.println("Entering normal mode.");
    setup_normal();
  }
}
 
void loop() {
  if (!isNormal) {
    loop_setupmode(); 
  } else {
    loop_normal(); 
  }
}

void list_dir() {
  char cwdName[2];

  Serial.println(">>> LittleFS directory listing");

  strcpy(cwdName,"/");
  Dir dir = LittleFS.openDir(cwdName);
  
  while( dir.next()) {
    String fn, fs;
    fn = dir.fileName();
    fn.remove(0, 1);
    fs = String(dir.fileSize());
    Serial.println("<" + fn + "> size=" + fs);
  } // end while

  Serial.println("===");

}
