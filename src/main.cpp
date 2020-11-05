/**
 * EnvBoy X main
 */
#include <Arduino.h>

#include "global.h"
#include "setupmode.h"
#include "normalmode.h"

#include <ESP8266WiFi.h>
#include <ESP8266mDNS.h>

#include <WiFiClient.h> 

#include <Wire.h>

#include <FS.h>
#include <LittleFS.h>
 
bool isNormal = false;

void setup()
{
  Serial.begin(115200);
  Serial.println(""); // 1行目にゴミが出るので改行しないと読めない
  Serial.println("");
  Serial.println("Starting " + product);

  // Init I2C Serial
  Wire.begin(5, 4);

  bool isNormal = has_valid_config_file();

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