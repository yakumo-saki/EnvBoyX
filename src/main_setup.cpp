#include <Arduino.h>

#ifdef ESP32
#include <WiFi.h>
#elif defined(ESP8266)
#include <ESP8266WiFi.h>
#endif

#include "log.h"
#include "display.h"
#include "global.h"
#include "config.h"
#include "http_setup.h"
#include "wifi.h"
#include "i2c.h"

/**
 * 初期化
 */
void setup_setupmode() {

  // configured ファイルがなければconfigを読まずにここに来るので一度configを読む。
  set_default_config_value();
  bool success = read_config();
  if (success) {
    mainlog("Config load successful.");
  } else {
    mainlog("Config load failed or failed partially.");
  }

  start_wifi_access_point();
  setup_http_setup();

	// Init I2C Serial
	init_i2c(I2C_SDA, I2C_SCL);

  setup_display();
  disp_setup_startup_screen(WiFi.softAPIP().toString());

  mainlog(F("Setup complete."));
}

void loop_setupmode() {
  loop_http_setup();
}