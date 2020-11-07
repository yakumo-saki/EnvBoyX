/**
 * EnvBoy X main
 */
#include <Arduino.h>

#include "log.h"
#include "global.h"
#include "config.h"
#include "main_setup.h"
#include "main_normal.h"

#include <Wire.h>

bool isNormal = false;

void setup()
{
  Serial.begin(9600);
  Serial.println(""); // 1行目にゴミが出るので改行しないと読めない
  Serial.println("");
  Serial.println("Starting " + product);

  sectionlog("Initializing LittleFS.");
  config_setup();
  isNormal = has_valid_config_file();

  if (!isNormal) {
    sectionlog("Entering setup mode.");
    setup_setupmode(); 
  } else {
    sectionlog("Entering normal mode.");

    // Init I2C Serial
    sectionlog("Initializing I2C SDA=" + String(I2C_SDA) + " SCL=" + String(I2C_SCL));
    Wire.begin(I2C_SDA, I2C_SCL);

    setup_normal();
  }

  sectionlog("setup done.");
}
 
void loop() {
  if (!isNormal) {
    loop_setupmode(); 
  } else {
    loop_normal(); 
  }
}
