#include <Arduino.h>

#include "log.h"
#include "global.h"
#include "network/http_api.h"
#include "network/http_api_util.h"
#include "sensors/mhz19_uart.h"

extern HTTPWEBSERVER server;


void _get_config() {

  String keys = server.arg("key");

  std::vector<String> keyArray;
  keyArray = stringSplit(keys, ",");

  String ret = keys + " => ";
  for (const auto& key : keyArray) {
    ret += "|" + key + "| ";
  }

  if (true) {
    server.send(200, MIME_TEXT, ret + "\n");
  } else {
    server.send(200, MIME_TEXT, "OFF\n");
  }
}

void _set_config() {

  bool success = true;

  if (success) {
    server.send(200, MIME_TEXT, "OK\n");
  } else {
    server.send(200, MIME_TEXT, "FAILED\n");
  }
}

void http_api_config_setup() {
  server.on ( "/config", HTTP_GET, _get_config );
  server.on ( "/config", HTTP_POST, _set_config );
  
  apilog("API Config initialized.");
}