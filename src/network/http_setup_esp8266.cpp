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
  config.use_mhz19b = server.arg("use_mhz19b");
  config.mhz19b_pwmpin = server.arg("mhz19b_pwmpin");
  // SoftwareSerialの初期化時にこれを渡す方法がわからないので固定
  config.mhz19b_rxpin = "14";
  config.mhz19b_txpin = "0";
  config.mqttBroker = server.arg("mqttbroker");
  config.mqttName = server.arg("mqttname");

  trim_config();
  String html = http_setup_post_root_content();

  server.send(200, "text/html", html);
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
  server.handleClient();
}

#endif
