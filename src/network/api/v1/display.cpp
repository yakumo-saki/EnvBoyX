#include <Arduino.h>
#include <ArduinoJson.h>

#include "log.h"
#include "global.h"
#include "display/display.h"
#include "network/webserver.h"

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
  // brightnessManagerに投げたいところだが、一時的に変更する用途だろうと思われるので直接変更
  String msg = disp_set_brightness(brightness);

  DynamicJsonDocument json(100);
  json["command"] = "DISPLAY_BRIGHTNESS";
  json["success"] = true;
  json["msg"] = msg + "\nDont forget to change config if you like new brightness!";

  String jsonStr;
  serializeJson(json, jsonStr);
  server.send(200, MimeType::JSON, jsonStr);
}

void http_api_display_setup() {
  server.on ( "/api/v1/brightness", HTTP_GET, http_handle_brightness );
  server.on ( "/api/v1/display", HTTP_GET, http_handle_power );

  apilog("API Display initialized.");
}