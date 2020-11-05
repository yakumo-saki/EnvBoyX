/**
 * EnvBoy X main
 */
#include <Arduino.h>

#include "log.h"
#include "global.h"
#include "config.h"
#include "main_setup.h"
#include "main_normal.h"

#include <ESP8266WiFi.h>
#include <ESP8266mDNS.h>

#include <WiFiClient.h> 

#include <Wire.h>

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
    sectionlog("Entering setup mode.");
    setup_setupmode(); 
  } else {
    sectionlog("Entering normal mode.");
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
