#include <Arduino.h>
#include <ArduinoJson.h>

#include "config.h"

#include "log.h"
#include "global.h"
#include "display/display.h"

#include "network/webserver.h"
#include "network/api/basic_api.h"
#include "network/api/api_util.h"
#include "network/http_utils.h"

extern HTTPWEBSERVER server;

void http_handle_stastics() {
  server.send(200, MimeType::JSON, stasticsJSON);
}

void http_api_stastics_setup() {
  server.on ( "/stastics", HTTP_GET, http_handle_stastics );

  apilog("Stastics API initialized.");
}
