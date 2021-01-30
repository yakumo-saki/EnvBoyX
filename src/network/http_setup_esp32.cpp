#ifdef ESP32

#include <Arduino.h>

#include "log.h"
#include "global.h"
#include "config.h"

#include "http_setup.h"

#include <WiFi.h>
#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>

extern AsyncWebServer server;

/**
 * GET 設定画面
 */
void handle_get_root(AsyncWebServerRequest *request) {

  String html = http_setup_get_root_content();

  request->send(200, "text/html", html);
}

void alerts_to_config(AsyncWebServerRequest *request, config_alert_t& alerts, String prefix) {
  alerts.caution1.low =  request->getParam(prefix + "caution1.low", true)->value();
  alerts.caution1.high = request->getParam(prefix + "caution1.high", true)->value();
  alerts.caution2.low =  request->getParam(prefix + "caution2.low", true)->value();
  alerts.caution2.high = request->getParam(prefix + "caution2.high", true)->value();

  alerts.warning1.low =  request->getParam(prefix + "warning1.low", true)->value();
  alerts.warning1.high = request->getParam(prefix + "warning1.high", true)->value();
  alerts.warning2.low =  request->getParam(prefix + "warning2.low", true)->value();
  alerts.warning2.high = request->getParam(prefix + "warning2.high", true)->value();
}

/**
 * Post 設定 ( config の post。 ファイルに設定を保存）
 */
void handle_post_root(AsyncWebServerRequest *request) {

  config.ssid = request->getParam("ssid", true)->value();
  config.password = request->getParam("pass", true)->value();
  config.mDNS = request->getParam("mdnsname", true)->value();
  config.opMode = request->getParam("opmode", true)->value();
  config.displayFlip = request->getParam("displayFlip", true)->value();
  config.displayBrightness = request->getParam("displayBrightness", true)->value();
  config.st7789 = request->getParam("st7789", true)->value();
  config.st7789Mode = request->getParam("st7789Mode", true)->value();
  config.mhz19b = request->getParam("mhz19b", true)->value();
  config.mhz19bPwmPin = request->getParam("mhz19bPwmPin", true)->value();
  config.mhz19bRxPin = request->getParam("mhz19bRxPin", true)->value();
  config.mhz19bTxPin = request->getParam("mhz19bTxPin", true)->value();
  config.mqttBroker = request->getParam("mqttbroker", true)->value();
  config.mqttName = request->getParam("mqttname", true)->value();

  alerts_to_config(request, config.temperatureAlerts, "tempAlerts.");
  alerts_to_config(request, config.humidityAlerts, "humiAlerts.");
  alerts_to_config(request, config.luxAlerts, "luxAlerts.");
  alerts_to_config(request, config.pressureAlerts, "presAlerts.");
  alerts_to_config(request, config.co2Alerts, "co2Alerts.");

  trim_config();
  String html = http_setup_post_root_content();

  request->send(200, "text/html", html);
}

/**
 * 初期化(設定用Webサーバモード)
 */
void setup_http_setup() {
  httplog("HTTP web server initializing");
  server.on("/", HTTP_GET, handle_get_root);
  server.on("/", HTTP_POST, handle_post_root);
  server.begin();
  httplog("HTTP web server initialized");
}

void loop_http_setup() {

}

#endif