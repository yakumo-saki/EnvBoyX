/**
 * EnvBoy X main
 */
#include <Arduino.h>

#include "log.h"
#include "global.h"
#include "config.h"
#include "main_setup.h"
#include "main_normal.h"

bool isNormal = false;

void setup()
{
  Serial.begin(74880);
  while (!Serial); // Serial起動待ち
  
  Serial.println(""); // 1行目にゴミが出るので改行しないと読めない
  Serial.println("");
  Serial.println("Starting " + product_long);
  Serial.println(""); 
  Serial.println("");

  sectionlog("Initializing LittleFS.");
  config_setup();
  isNormal = has_valid_config();

  if (!isNormal) {
    sectionlog("Entering setup mode.");
    setup_setupmode(); 
  } else {
    sectionlog("Entering normal mode.");

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
