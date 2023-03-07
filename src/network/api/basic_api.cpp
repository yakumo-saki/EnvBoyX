#include <Arduino.h>
#include <ArduinoJson.h>

#include "config/config.h"

#include "log.h"
#include "global.h"
#include "display/display.h"

#include "TimeUtil.h"

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
  doc["datetime"] = getFormattedTime();
  doc["uptime"] = timeString;
  doc["uptimeMills"] = ms;
  doc["majorVer"] = ver;
  doc["minorVer"] = minorVer;
  doc["settingId"] = SETTING_ID;
  doc["name"] = config->get(ConfigNames::MDNS);
  doc["build"] = DEBUG_BUILD ? "DEBUG" : "Release";
  
  if (OPERATING_MODE == OPERATING_MODE_NORMAL) {
    doc["mode"] = "NORMAL";
  } else if (OPERATING_MODE == OPERATING_MODE_SETUP) {
    doc["mode"] = "SETUP";
  } else {
    doc["mode"] = "INVALID";
  }

  String json;
  serializeJson(doc, json);
  doc.clear();

  httplog(json);
  return json;
}

void http_handle_ping() {
  sendHttpHeader(MimeType::JSON);
  String message = http_normal_ping_json();
  server.sendContent(message);
}

void http_api_basic_setup() {
  server.on ( "/ping", HTTP_GET, http_handle_ping);

  // Config web ではPingが最初に実行されるので、ここにOPTION CORSの問い合わせが飛んでくる
  // 一度答えればキャッシュされるようなので他のAPIには不要
  server.on ( "/ping", HTTP_OPTIONS, http_handle_cors);

  apilog("Basic API initialized.");
}