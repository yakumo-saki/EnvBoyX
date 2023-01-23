/**
 * EnvBoy X main
 */
#include <Arduino.h>

#include "log.h"
#include "global.h"
#include "config/config.h"
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

  sectionlog(F("Initializing Config."));
  config = &Config::getInstance();

  sectionlog(F("Initializing FS."));
  mainlog(F("Starting config"));
  config_setup();

  mainlog(F("Checking config files"));
  isNormal = has_configured_file();

  CFG_VALIDATE_RESULT validateResult = has_valid_config_file();
  if (validateResult == CFG_VALIDATE_RESULT::VALID) {
    mainlog(F("Config file is valid."));
  } else if (validateResult == CFG_VALIDATE_RESULT::NEED_UPGRADE) {
    sectionlog(F("Config file needs upgrade. upgrading..."));
    read_config();
    save_config();  // upgrade means json + default, so only saving is needed.
    sectionlog(F("Config file upgrade done!"));
  } else {
    mainlog(F("Config file validation error. dropping to setup mode."));
    isNormal = false;
  }

  if (!isNormal) {
    OPERATING_MODE = OPERATING_MODE_SETUP;
    sectionlog(F("Entering setup mode."));

    setup_setupmode(); 
  } else {
    OPERATING_MODE = OPERATING_MODE_NORMAL;
    sectionlog(F("Entering normal mode."));

    setup_normal();
  }

  sectionlog(F("setup done. Starting main loop"));
}
 
void loop() {
  if (!isNormal) {
    loop_setupmode(); 
  } else {
    loop_normal(); 
  }
}
