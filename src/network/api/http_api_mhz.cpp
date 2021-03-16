#include <Arduino.h>
#include <ArduinoJson.h>

#include "log.h"
#include "global.h"
#include "network/http_api.h"
#include "network/http_api_util.h"
#include "sensors/mhz19_uart.h"

extern HTTPWEBSERVER server;


void _set_mhz_abc() {
  if (config.mhz19b != ConfigValues::MHZ_USE_UART) {
    server.send(500, MimeType::TEXT, F("MHZ19B IS DISABLED\n"));
    return;
  }

  String abc = server.arg("value");
  if (!abc || abc == "") {
    server.send(400, MimeType::TEXT, F("'value' parameter is missing.\n"));
    return;
  }

  bool onoff = parseBooleanString(abc);

  bool success = mhz_set_abc(onoff);

  DynamicJsonDocument json(200);
  json["command"] = "MHZ19B_SET_ABC";
  json["success"] = success;
  json["msg"] = (onoff ? "ABC is enabled" : "ABC is disabled");

  String jsonStr;
  serializeJson(json, jsonStr);
  server.send(200, MimeType::JSON, jsonStr);
}

void _get_mhz_abc() {
  if (config.mhz19b != ConfigValues::MHZ_USE_UART) {
    server.send(500, MimeType::TEXT, F("MHZ19B IS DISABLED\n"));
    return;
  }

  bool abc = mhz_get_abc();

  DynamicJsonDocument json(200);
  json["command"] = "MHZ19B_GET_ABC";
  json["success"] = true;
  json["ABC"] = abc ? "ON" : "OFF";

  String jsonStr;
  serializeJson(json, jsonStr);
  server.send(200, MimeType::JSON, jsonStr);
}

void _mhz_zero_calibration() {
  if (config.mhz19b != ConfigValues::MHZ_USE_UART) {
    server.send(500, MimeType::TEXT, F("MHZ19B IS DISABLED\n"));
    return;
  }

  bool success = mhz_do_zero_calibration();

  DynamicJsonDocument json(200);
  json["command"] = "MHZ19B_ZERO_CALIBRATION";
  json["success"] = success;

  String jsonStr;
  serializeJson(json, jsonStr);
  server.send(200, MimeType::JSON, jsonStr);
}

void http_api_mhz_setup() {
  server.on ( "/mhz19b/abc", HTTP_GET, _get_mhz_abc );
  server.on ( "/mhz19b/abc", HTTP_POST, _set_mhz_abc );
  server.on ( "/mhz19b/zeroCalibration", HTTP_POST, _mhz_zero_calibration );

  apilog("API MHZ-19B initialized.");
}