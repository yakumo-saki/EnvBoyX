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

String http_setup_post_root_error_content(std::vector<std::pair<String, String>> errors) {
  String html = "";
  html += "<html>";
  html += "<head>";
  html += "<title>" + product + " setting done. please restart.</title>";
  html += "<meta charset='UTF-8'>";
  html += "<link rel='stylesheet' href='/style.css'>";
  html += "<style>";
  html += "  input { width:200px; }";
  html += "</style>";
  html += "</head>";
  html += "<body class='setup_err'>";
  html += "以下の設定値が正しくありません。<br>";
  html += "<ul>";
  for (auto& error: errors) {
    html += "<li>" + error.first + " = " + error.second + "</li>";
  }
  html += "</ul>";
  html += "設定にエラーがあるため、保存できませんでした。<br>";
  html += "以下のリンクから再設定を行ってください。<br>";
  html += "<br>";
  html += "<a class='setup_again' href='/?configNoLoad'>再設定</a>";
  html += "</body>";
  html += "</html>";

  return html;
}

/**
 * GET 設定画面
 */
void handle_get_root() {

  // エラー画面からの戻りであればConfigを読まない（すでにセットされているから上書きしてしまう）
  if (!server.hasArg("configNoLoad")) {
    read_config();
  }

  String html = http_setup_get_root_content();

  server.send(200, MimeType::HTML, html);
}

/**
 * Post 設定 ( config の post。 ファイルに設定を保存）
 */
void handle_post_root() {
  
  std::vector<String> SKIPABLE_KEY {ConfigNames::SETTING_ID};
  std::vector<std::pair<String, String>> errors;

  for (auto &key : config->getKeys()) {
    bool argExist = server.hasArg(key);
    // debuglog("server arg key=" + key + " ret=" + (argExist ? "EXIST" : "NONE"));
    String value = server.arg(key);
    if (argExist) {
      ConfigSetResult result = config->set(key, value);
      if (result == ConfigSetResult::INVALID_VALUE) {
        httplog("[INVALID_VALUE] " + key + " " + value);
        std::pair<String, String> pair(key, value);
        errors.push_back(pair);
      }
    } else {
      if (vectorStringContains(SKIPABLE_KEY, key)) {
        httplog("[OK] Skippable key. " + key);
      } else {
        halt("WebPOST ERR", "NO KEY", key);
      }
    }
  }

  String html;
  if (errors.size() == 0) {
    httplog("[OK] Config save start");
    save_config();
    httplog("[OK] Sending done HTML");
    html = http_setup_post_root_content();
  } else {
    httplog("Send config error page");
    html = http_setup_post_root_error_content(errors);
  }

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
