#ifdef ESP8266

#include <Arduino.h>

#include "log.h"
#include "global.h"
#include "config.h"

#include "http_setup.h"

#include <ESP8266WebServer.h>
extern ESP8266WebServer server;

/**
 * GET 設定画面
 */
void handle_get_root() {

  String html = http_setup_get_root_content();

  server.send(200, "text/html", html);
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
  config.displayFlip = server.arg("displayFlip");
  config.displayBrightness = server.arg("displayBrightness");
  config.st7789 = server.arg("st7789");
  config.st7789Mode = server.arg("st7789Mode");
  config.mhz19b = server.arg("mhz19b");
  config.mhz19bPwmPin = server.arg("mhz19bPwmPin");
  // SoftwareSerialの初期化時にこれを渡す方法がわからないので固定
  config.mhz19bRxPin = "14";
  config.mhz19bTxPin = "0";
  config.mqttBroker = server.arg("mqttbroker");
  config.mqttName = server.arg("mqttname");

  alerts_to_config(config.temperatureAlerts, "tempAlerts.");
  alerts_to_config(config.humidityAlerts, "humiAlerts.");
  alerts_to_config(config.luxAlerts, "luxAlerts.");
  alerts_to_config(config.pressureAlerts, "presAlerts.");
  alerts_to_config(config.co2Alerts, "co2Alerts.");

  trim_config();
  String html = http_setup_post_root_content();

  server.send(200, MIME_HTML, html);
}

/**
 * 初期化(設定用Webサーバモード)
 */
void setup_http_setup() {
  httplog(F("HTTP web server initializing"));
  server.on("/", HTTP_GET, handle_get_root);
  server.on("/", HTTP_POST, handle_post_root);
  server.begin();
  httplog(F("HTTP web server initialized"));
}

void loop_http_setup() {
  server.handleClient();
}

#endif
