#include <Arduino.h>

#include "log.h"
#include "global.h"
#include "config.h"

#include "http_setup.h"

#ifdef ESP32
#include <WebServer.h>
extern WebServer server;
#endif

#ifdef ESP8266
#include <ESP8266WebServer.h>
extern ESP8266WebServer server;
#endif

extern const uint8_t STYLE_CSS[] asm("_binary_embed_style_css_start");

/**
 * GET 設定画面
 */
void handle_get_root() {

  String html = http_setup_get_root_content();

  server.send(200, MimeType::HTML, html);
}

void alerts_to_config(config_alert_t& alerts, String prefix) {
  alerts.caution1.low = server.arg(prefix + "caution1.low");
  alerts.caution1.high = server.arg(prefix + "caution1.high");
  alerts.caution2.low = server.arg(prefix + "caution2.low");
  alerts.caution2.high = server.arg(prefix + "caution2.high");

  alerts.warning1.low = server.arg(prefix + "warning1.low");
  alerts.warning1.high = server.arg(prefix + "warning1.high");
  alerts.warning2.low = server.arg(prefix + "warning2.low");
  alerts.warning2.high = server.arg(prefix + "warning2.high");
}

/**
 * Post 設定 ( config の post。 ファイルに設定を保存）
 */
void handle_post_root() {
  
  config.ssid = server.arg("ssid");
  config.password = server.arg("pass");
  config.mDNS = server.arg("mdnsname");
  config.opMode = server.arg("opmode");

  config.displayFlip = server.arg(ConfigValues::DISPLAY_FLIP_NAME);
  config.displayBrightness = server.arg("displayBrightness");
  config.displaySkipReconfigure = server.arg(ConfigValues::DISPLAY_RECONFIG_NAME);

  config.oledType = server.arg(ConfigValues::OLED_NAME);

  config.st7789 = server.arg(ConfigValues::ST7789_NAME);
  config.st7789Mode = server.arg(ConfigValues::ST7789_MODE_NAME);

  config.mhz19b = server.arg(ConfigValues::MHZ_NAME);
  config.mhz19bPwmPin = server.arg("mhz19bPwmPin");
  config.mhz19bRxPin = server.arg("mhz19bRxPin");
  config.mhz19bTxPin = server.arg("mhz19bTxPin");;
  config.mhz19bABC = server.arg(ConfigValues::MHZ_ABC_NAME);

  config.mqttBroker = server.arg("mqttbroker");
  config.mqttName = server.arg("mqttname");

  alerts_to_config(config.temperatureAlerts, "tempAlerts.");
  alerts_to_config(config.humidityAlerts, "humiAlerts.");
  alerts_to_config(config.luxAlerts, "luxAlerts.");
  alerts_to_config(config.pressureAlerts, "presAlerts.");
  alerts_to_config(config.co2Alerts, "co2Alerts.");

  trim_config();
  String html = http_setup_post_root_content();

  server.send(200, MimeType::HTML, html);
}

void handle_get_style_css() {
  httplog(F("style.css accessed"));
  server.send(200, MimeType::CSS, (const char *)STYLE_CSS);
}


/**
 * 初期化(設定用Webサーバモード)
 */
void setup_http_setup() {
  httplog(F("HTTP web server initializing"));
  server.on("/", HTTP_GET, handle_get_root);
  server.on("/", HTTP_POST, handle_post_root);
  server.on("/style.css", HTTP_GET, handle_get_style_css);
  server.begin();
  httplog(F("HTTP web server initialized"));
}

void loop_http_setup() {
  server.handleClient();
}
