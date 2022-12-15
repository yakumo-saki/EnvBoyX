#include <Arduino.h>
#include <ArduinoJson.h>

#include "config.h"

#include "global.h"

#include "network/webserver.h"

extern HTTPWEBSERVER server;

void http_handle_not_found() {
  String message = "404 File Not Found\n\n";
  server.send(404, MimeType::HTML, message);
}

void http_not_found_setup() {
  server.onNotFound ( http_handle_not_found );
}