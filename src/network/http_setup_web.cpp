#include <Arduino.h>

#include "log.h"
#include "global.h"
#include "config.h"

#include "http_setup.h"

#include "halt.h"
#include "utils.h"

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
  
  std::vector<String> SKIPABLE_KEY {ConfigNames::SETTING_ID};

  for (auto &key : config->getKeys()) {
    bool argExist = server.hasArg(key);
    debuglog("server arg key=" + key + " ret=" + (argExist ? "EXIST" : "NONE"));
    if (argExist) {
      config->set(key, server.arg(key));
    } else {
      if (vectorStringContains(SKIPABLE_KEY, key)) {
        httplog("[OK] Skippable key. " + key);
      } else {
        halt("WebPOST ERR", "NO KEY", key);
      }
    }
  }

  save_config();

  String html = http_setup_post_root_content();

  server.send(200, MimeType::HTML, html);
}

void handle_get_style_css() {
  // httplog(F("style.css accessed"));
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
