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