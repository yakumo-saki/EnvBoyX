#include <Arduino.h>

#include "log.h"
#include "global.h"
#include "config/config.h"

#include "network/webserver.h"

#include "network/http_utils.h"

#include "embed/config_html.h"
#include "embed/style_css.h"
#include "embed/config_js.h"

extern HTTPWEBSERVER server;

/**
 * GET 設定画面
 */
void handle_get_webconfig() {

  sendHttpHeader();

  server.sendContent(CONFIG_HTML);
}

void handle_get_style_css() {

  sendHttpHeader(MimeType::CSS);

  server.sendContent(STYLE_CSS);
}

void handle_get_config_js() {

  sendHttpHeader(MimeType::JAVASCRIPT);

  server.sendContent(CONFIG_JS);
}

void http_web_config_setup() {
  server.on("/static/style.css", HTTP_GET, handle_get_style_css);
  server.on("/static/config.js", HTTP_GET, handle_get_config_js);
  server.on("/", HTTP_GET, handle_get_webconfig);
  
  httplog(F("HTTP web config initialized"));
}
