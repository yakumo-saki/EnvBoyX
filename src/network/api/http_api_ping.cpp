#include <Arduino.h>
#include <ArduinoJson.h>

#include "config.h"

#include "log.h"
#include "global.h"
#include "display/display.h"
#include "network/http_api.h"
#include "network/http_api_util.h"
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


void http_handle_ping() {
  sendHttpHeader(MimeType::JSON);
  String message = http_normal_ping_json();
  server.sendContent(message);
}

void http_api_ping_setup() {
  server.on ( "/ping", HTTP_GET, http_handle_ping);
 
  server.on ( "/ping", HTTP_OPTIONS, http_handle_cors);

  apilog("API Ping initialized.");
}