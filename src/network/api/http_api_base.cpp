#include <Arduino.h>
#include <ArduinoJson.h>

#include "config.h"

#include "log.h"
#include "global.h"
#include "display/display.h"
#include "network/http_api.h"
#include "network/http_api_base_json.h"

extern HTTPWEBSERVER server;

void http_handle_data() {
  String message = http_normal_data_json();
  server.send(200, MimeType::JSON, message);
}

void http_handle_stastics() {
  server.send(200, MimeType::JSON, stasticsJSON);
}

void http_handle_goto_setup() {

  remove_configure_flag_file();
  
  DynamicJsonDocument json(200);
  json["command"] = "GOTO_SETUP";
  json["success"] = true;
  json["msg"] = "OK. Entering setup mode at next boot.";

  String jsonStr;
  serializeJson(json, jsonStr);
  server.send(200, MimeType::JSON, jsonStr);
}

void http_api_base_setup() {
  server.on ( "/", HTTP_GET, http_handle_data );
  server.on ( "/stastics", HTTP_GET, http_handle_stastics );
  server.on ( "/goto_setup", HTTP_POST, http_handle_goto_setup );

  apilog("API Base initialized.");
}