#include <Arduino.h>
#include <ArduinoJson.h>

#include "config/config.h"

#include "global.h"

#include "network/http_utils.h"
#include "network/webserver.h"

extern HTTPWEBSERVER server;

void http_handle_not_found() {
  // HEADER
  server.sendContent("HTTP/1.1 404 FILE NOT FOUND\r\n");
  server.sendContent("Content-Type: text/plain\r\n");
  if (DEBUG_BUILD) {
    server.sendContent("Access-Control-Allow-Origin: *\r\n");
  }
  server.sendContent("\r\n");

  // CONTENT
  String message = "404 File Not Found\n\n";
  server.sendContent(message);
}

void http_not_found_setup() {
  server.onNotFound ( http_handle_not_found );
}