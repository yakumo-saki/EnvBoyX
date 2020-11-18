#ifdef ESP32

#include <Arduino.h>

#include "log.h"
#include "global.h"

#include "http_normal.h"
#include "display.h"

#include <WiFi.h>
#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>

extern AsyncWebServer server;

void http_handle_not_found(AsyncWebServerRequest *request) {
  String message = http_normal_not_found_html();
  request->send ( 404, "text/plain", message );
}

void http_handle_data(AsyncWebServerRequest *request) {
  String message = http_normal_data_json();
  request->send ( 200, F("application/json"), message );
}

void http_handle_ping(AsyncWebServerRequest *request) {
  String message = http_normal_ping_json();
  request->send ( 200, F("application/json"), message );
}

void http_handle_brightness(AsyncWebServerRequest *request) {
  if (!request->hasParam("value")) {
    request->send ( 400, F("text/plain"), "no value param");
    return;
  }
  
  int brightness = request->getParam("value")->value().toInt();
  String msg = disp_set_brightness(brightness);

  String message = "OK\n" + msg;
  request->send ( 200, F("text/plain"), message );
}

void http_setup_normal() {
  httplog("HTTP web server initializing");
  server.on ( "/ping", HTTP_GET, http_handle_ping);
  server.on ( "/", HTTP_GET, http_handle_data );
  server.on ( "/brightness", HTTP_GET, http_handle_data );
  server.onNotFound ( http_handle_not_found );
  server.begin(); 
  httplog("HTTP web server initialized");
}

void http_loop_normal() {
}

#endif