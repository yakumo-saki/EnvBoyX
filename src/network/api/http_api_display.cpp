#include <Arduino.h>
#include <ArduinoJson.h>

#include "log.h"
#include "global.h"
#include "display/display.h"
#include "network/http_api.h"

extern HTTPWEBSERVER server;

void http_handle_power() {
  String power = server.arg("value");

  if (power == "1") {
    disp_set_power(true);
  } else {
    disp_set_power(false);
  }


  DynamicJsonDocument json(100);
  json["command"] = "DISPLAY_POWER";
  json["success"] = true;

  String jsonStr;
  serializeJson(json, jsonStr);
  server.send(200, MimeType::JSON, jsonStr);
}

void http_handle_brightness() {
  String value = server.arg("value");
  int brightness = value.toInt();
  String msg = disp_set_brightness(brightness);

  DynamicJsonDocument json(100);
  json["command"] = "DISPLAY_BRIGHTNESS";
  json["success"] = true;
  json["msg"] = msg;

  String jsonStr;
  serializeJson(json, jsonStr);
  server.send(200, MimeType::JSON, jsonStr);
}

void http_api_display_setup() {
  server.on ( "/brightness", HTTP_GET, http_handle_brightness );
  server.on ( "/display", HTTP_GET, http_handle_power );

  apilog("API Display initialized.");
}