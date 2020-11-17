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

/**
 * Post 設定 ( config の post。 ファイルに設定を保存）
 */
void handle_post_root(AsyncWebServerRequest *request) {

  ssid = request->getParam("ssid", true)->value();
  password = request->getParam("pass", true)->value();
  mDNS = request->getParam("mdnsname", true)->value();
  opMode = request->getParam("opmode", true)->value();
  use_mhz19b = request->getParam("use_mhz19b", true)->value();
  mhz19b_pwmpin = request->getParam("mhz19b_pwmpin", true)->value();
  mqttBroker = request->getParam("mqttbroker", true)->value();
  mqttName = request->getParam("mqttname", true)->value();

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