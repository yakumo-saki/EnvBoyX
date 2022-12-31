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

void http_api_gotosetup_setup() {
  server.on ( "/goto_setup", HTTP_POST, http_handle_goto_setup );

  apilog("Goto Setup API initialized.");
}