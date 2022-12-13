#include <Arduino.h>
#include <ArduinoJson.h>

#include "config.h"

#include "log.h"
#include "global.h"
#include "display/display.h"
#include "network/http_api.h"
#include "network/http_api_base_json.h"

extern HTTPWEBSERVER server;

void http_handle_not_found() {
  String message = "404 File Not Found\n\n";
  server.send(404, MimeType::HTML, message);
}

void http_not_found_setup() {
  server.onNotFound ( http_handle_not_found );
}