#include <Arduino.h>
#include <ArduinoJson.h>

#include "log.h"
#include "global.h"
#include "config.h"
#include "network/http_api.h"
#include "network/http_api_util.h"
#include "network/http_api_config_getset.h"
#include "network/http_api_config_backup.h"
#include "sensors/mhz19_uart.h"

extern HTTPWEBSERVER server;

void _get_config() {

  String keys = server.arg("key");

  std::vector<String> keyArray = stringSplit(keys, ",");

  for (const auto& k : keyArray) {
    debuglog(k);
  }

  DynamicJsonDocument json = create_config_json(keyArray);
  json["command"] = "CONFIG_GET";
  json["success"] = true;

  String jsonStr;
  serializeJson(json, jsonStr);
  server.send(200, MIME_JSON, jsonStr);
}

void _set_config() {

  DynamicJsonDocument json = updateConfig();
  json["command"] = "CONFIG_SET";

  String jsonStr;
  serializeJson(json, jsonStr);

  server.send(200, MIME_JSON, jsonStr);
}

void _revert_config() {

  // revertすると何が変更されるかわからないので、全ての反映を実行
  reflectConfigAll();

  DynamicJsonDocument json(100);
  json["command"] = "CONFIG_REVERT";
  json["success"] = true;
  read_config();

  String jsonStr;
  serializeJson(json, jsonStr);
  server.send(200, MIME_JSON, jsonStr);
}

void _commit_config() {
  DynamicJsonDocument json(100);
  
  json["command"] = "CONFIG_COMMIT";
  json["success"] = true;
  save_config();

  String jsonStr;
  serializeJson(json, jsonStr);
  server.send(200, MIME_JSON, jsonStr);
}

void _backup_config() {
  String ret = http_api_backup_config();
  server.send(200, MIME_TEXT, ret);
}

void http_api_config_setup() {
  server.on ( "/config", HTTP_GET, _get_config );
  server.on ( "/config", HTTP_POST, _set_config );
  server.on ( "/config/revert", HTTP_POST, _revert_config );
  server.on ( "/config/commit", HTTP_POST, _commit_config );
  server.on ( "/config/backup", HTTP_GET, _backup_config );
  
  apilog("API Config initialized.");
}