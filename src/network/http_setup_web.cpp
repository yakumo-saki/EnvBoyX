#include <Arduino.h>

#include "log.h"
#include "global.h"
#include "config.h"

#include "http_setup.h"

#include "halt.h"
#include "utils.h"

#include "embed/style_css.h"

#include "network/webserver.h"
#include "network/http_utils.h"

void http_setup_post_root_error_content(std::vector<std::pair<String, String>> errors) {

  sendHttpHeader();
  sendHtmlHeader();

  String html = "";
  html += "<body class='setup_err'>";
  html += "<h1>" + product + " Settings  (" + SETTING_ID + ")</h1>";
  html += "<h3>以下の設定値が正しくありません。</h3>";
  html += "<ul>";
  for (auto& error: errors) {
    html += "<li>" + error.first + " = '" + error.second + "'</li>";
  }
  html += "</ul>";
  html += "設定にエラーがあるため、保存できませんでした。<br>";
  html += "以下のリンクから再設定を行ってください。<br>";
  html += "<br>";
  html += "<a class='setup_again' href='/?configNoLoad=1'>再設定</a>";
  html += "</body>";
  html += "</html>";

  server.sendContent(html);
}

/**
 * GET 設定画面
 */
void handle_get_root() {

  // エラー画面からの戻りであればConfigを読まない（すでにセットされているから上書きしてしまう）
  if (!server.hasArg("configNoLoad")) {
    mainlog("No configNoLoad parameter. loading config.");
    read_config();
  } else {
    mainlog("configNoLoad parameter specified. Skip loading config.");
  }

  http_send_setup_get_root_html();

  // server.send(200, MimeType::HTML, html);
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
    http_send_setup_post_root_html();
  } else {
    httplog("Send config error page");
    http_setup_post_root_error_content(errors);
  }
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
