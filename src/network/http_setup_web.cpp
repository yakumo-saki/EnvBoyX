#include <Arduino.h>

#include "log.h"
#include "global.h"
#include "config.h"

#include "http_setup.h"

#include "embed/style_css.h"

#ifdef ESP32
#include <WebServer.h>
extern WebServer server;
#endif

#ifdef ESP8266
#include <ESP8266WebServer.h>
extern ESP8266WebServer server;
#endif

/**
 * GET 設定画面
 */
void handle_get_root() {

  String html = http_setup_get_root_content();

  server.send(200, MimeType::HTML, html);
}

/**
 * Post 設定 ( config の post。 ファイルに設定を保存）
 */
void handle_post_root() {
  
  String a = server.arg("aaaa");
  debuglog("aaaa = " + a);

  for (auto &key : config->getKeys()) {
    config->set(key, server.arg(key));
  } 

  String html = http_setup_post_root_content();

  server.send(200, MimeType::HTML, html);
}

void handle_get_style_css() {
  httplog(F("style.css accessed"));
  server.send(200, MimeType::CSS, STYLE_CSS);
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
