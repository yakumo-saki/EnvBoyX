#ifdef ESP8266

#include <Arduino.h>

#include "http.h"
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