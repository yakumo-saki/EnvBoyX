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
  #ifdef ESP32
  Serial.begin(115200);
  #else
  Serial.begin(74880);
  #endif
  
  while (!Serial); // Serial起動待ち
  
  Serial.println(""); // 1行目にゴミが出るので改行しないと読めない
  Serial.println("");
  Serial.println("Starting " + product_long);
  Serial.println(""); 
  Serial.println("");

//  Serial.println(test);

  sectionlog(F("Initializing FS."));
  mainlog(F("Starting config"));
  config_setup();

  mainlog(F("Checking config files"));
  isNormal = has_valid_config();

  if (!isNormal) {
    sectionlog(F("Entering setup mode."));
    setup_setupmode(); 
  } else {
    sectionlog(F("Entering normal mode."));

    setup_normal();
  }

  sectionlog(F("setup done."));
}
 
void loop() {
  if (!isNormal) {
    loop_setupmode(); 
  } else {
    loop_normal(); 
  }
}
