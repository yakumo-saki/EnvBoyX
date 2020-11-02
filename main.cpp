/**
 * 
 * EnvBoy X main
 * 
 */
#include <Arduino.h>

#include "define.h"
#include "setupmode.h"
#include "normalmode.h"

#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>

#include <WiFiClient.h> 

#include <Wire.h>

#include <FS.h>
#include <LittleFS.h>
 
ESP8266WebServer server(80);

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
