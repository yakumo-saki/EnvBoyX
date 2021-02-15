#include <Arduino.h>

#include "config.h"

#include "log.h"
#include "global.h"
#include "display/display.h"
#include "network/http_api.h"
#include "network/http_api_base_json.h"

extern HTTPWEBSERVER server;

void http_handle_data() {
  String message = http_normal_data_json();
  server.send(200, MIME_JSON, message);
}

void http_handle_ping() {
  String message = http_normal_ping_json();
  server.send(200, MIME_JSON, message);
}

void http_handle_stastics() {
  server.send(200, MIME_JSON, stasticsJSON);
}

void http_handle_goto_setup() {

  remove_configure_flag_file();
  
  String message = "OK. Entering setup mode at next boot.";
  server.send( 200, MIME_TEXT, message );
}

void http_handle_not_found() {
  String message = http_normal_not_found_html();
  server.send(404, MIME_HTML, message);
}

void http_api_base_setup() {
  server.on ( "/ping", HTTP_GET, http_handle_ping);
  server.on ( "/", HTTP_GET, http_handle_data );
  server.on ( "/stastics", HTTP_GET, http_handle_stastics );
  server.on ( "/goto_setup", HTTP_POST, http_handle_goto_setup );

  server.onNotFound ( http_handle_not_found );
  apilog("API Base initialized.");
}