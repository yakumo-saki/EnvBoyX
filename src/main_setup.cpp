#include <Arduino.h>

#include <WiFi.h>

#include "log.h"
#include "display.h"
#include "global.h"
#include "config.h"
#include "http_setup.h"

/**
 * WiFi
 */
void setup_wifi() {
  byte mac[6];
  WiFi.macAddress(mac);

  // SSID は macaddress をSUFFIXする
  ssid = product_short + "_";
  for (int i = 0; i < 6; i++) {
    ssid += String(mac[i], HEX);
  }
  
  mainlog("SSID: " + ssid);
  // Serial.println("PASS: " + pass);

  /* You can remove the password parameter if you want the AP to be open. */
  // WiFi.softAP(ssid.c_str(), pass.c_str());
  WiFi.softAP(ssid.c_str());
}

/**
 * 初期化
 */
void setup_setupmode() {
  // ファイルシステム初期化
  set_default_config_value();

  setup_wifi();
  setup_http_setup();
  disp_setup_startup_screen(WiFi.softAPIP().toString());
}

void loop_setupmode() {
  return;
}