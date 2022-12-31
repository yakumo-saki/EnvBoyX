#include <Arduino.h>
#include <ArduinoJson.h>

#include "log.h"
#include "global.h"
#include "config.h"
#include "sensors/mhz19_uart.h"

#include "network/webserver.h"
#include "network/api/api_util.h"
#include "network/api/v1/config_getset.h"
#include "network/api/v1/config_backup.h"
#include "network//http_utils.h"

extern HTTPWEBSERVER server;

void _get_config() {

  sendHttpHeader(MimeType::JSON);

  String keys = server.arg("key");

  std::vector<String> keyArray = stringSplit(keys, ",");

  DynamicJsonDocument json(1200);
  json["command"] = "CONFIG_GET";
  json["success"] = true;
  json["config"] = create_config_json(keyArray);

  String jsonStr;
  serializeJson(json, jsonStr);
  server.sendContent(jsonStr);
}

void _set_config() {

  sendHttpHeader(MimeType::JSON);

  String jsonStr = updateConfig();

  server.sendContent(jsonStr);
}

void _revert_config() {

  sendHttpHeader(MimeType::JSON);

  // revertすると何が変更されるかわからないので、全ての反映を実行
  reflectConfigAll();

  DynamicJsonDocument json(100);
  json["command"] = "CONFIG_REVERT";
  json["success"] = true;
  read_config();

  String jsonStr;
  serializeJson(json, jsonStr);
  server.sendContent(jsonStr);
}

void _commit_config() {

  DynamicJsonDocument json(100);
  
  json["command"] = "CONFIG_COMMIT";
  json["success"] = true;
  save_config();

  sendHttpHeader(MimeType::JSON);
  
  String jsonStr;
  serializeJson(json, jsonStr);
  server.sendContent(jsonStr);
}

void _backup_config() {

  sendHttpHeader(MimeType::TEXT);

  String ret = http_api_backup_config();
  server.sendContent(ret);
}

void _factory_reset() {

  sendHttpHeader(MimeType::JSON);

  DynamicJsonDocument json(8000);

  apilog("FACTORY RESET INITIATED");
  json["command"] = "CONFIG_FACTORY_RESET";
  json["success"] = true;
  json["backup"] = http_api_backup_config();
  json["msg"] = "Config deleted. Reset or power off now.";

  config_factory_reset();

  apilog("FACTORY RESET DONE");

  server.sendContent(jsonToString(json));
}

void http_api_config_setup() {
  server.on ( "/api/v1/config", HTTP_GET, _get_config );
  server.on ( "/api/v1/config", HTTP_POST, _set_config );
  server.on ( "/api/v1/config/revert", HTTP_POST, _revert_config );
  server.on ( "/api/v1/config/commit", HTTP_POST, _commit_config );
  server.on ( "/api/v1/config/backup", HTTP_GET, _backup_config );
  server.on ( "/api/v1/config/factory-reset", HTTP_POST, _factory_reset );
  
  server.on ( "/api/v1/config", HTTP_OPTIONS, http_handle_cors);
  server.on ( "/api/v1/config/revert", HTTP_POST, http_handle_cors );
  server.on ( "/api/v1/config/commit", HTTP_OPTIONS, http_handle_cors );
  server.on ( "/api/v1/config/backup", HTTP_OPTIONS, http_handle_cors );

  apilog("API Config initialized.");
}