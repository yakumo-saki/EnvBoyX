#include <Arduino.h>
#include <ArduinoJson.h>

#include "log.h"
#include "global.h"
#include "config.h"
#include "network/http_api.h"
#include "network/http_api_util.h"
#include "network/http_api_config_json.h"
#include "sensors/mhz19_uart.h"

extern HTTPWEBSERVER server;

void _get_config() {

  String keys = server.arg("key");

  std::vector<String> keyArray = stringSplit(keys, ",");

  for (const auto& k : keyArray) {
    debuglog(k);
  }

  DynamicJsonDocument json = create_config_json(keyArray);

  String jsonStr;
  serializeJson(json, jsonStr);
  server.send(200, MIME_JSON, jsonStr);
}

void _set_config() {

  DynamicJsonDocument json = updateConfig();
  String jsonStr;
  serializeJson(json, "{" + jsonStr + "}");

  server.send(200, MIME_JSON, jsonStr);
}

void http_api_config_setup() {
  server.on ( "/config", HTTP_GET, _get_config );
  server.on ( "/config", HTTP_POST, _set_config );
  
  apilog("API Config initialized.");
}