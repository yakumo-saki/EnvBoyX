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


String http_normal_ping_json() {

  unsigned long ms = millis();
  String timeString = getTimeString(ms);

  DynamicJsonDocument doc(2000);
  doc["product"] = product;
  doc["uptime"] = timeString;
  doc["uptimeMills"] = ms;
  doc["majorVer"] = ver;
  doc["minorVer"] = minorVer;
  doc["settingId"] = SETTING_ID; 

  String json;
  serializeJson(doc, json);

  httplog(json);
  return json;
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

void http_handle_ping() {
  sendHttpHeader(MimeType::JSON);
  String message = http_normal_ping_json();
  server.sendContent(message);
}

void http_api_basic_setup() {
  server.on ( "/ping", HTTP_GET, http_handle_ping);
  server.on ( "/stastics", HTTP_GET, http_handle_stastics );
  server.on ( "/goto_setup", HTTP_POST, http_handle_goto_setup );

  server.on ( "/ping", HTTP_OPTIONS, http_handle_cors);

  apilog("Basic API initialized.");
}