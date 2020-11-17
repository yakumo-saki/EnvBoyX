#ifdef ESP32

#include <Arduino.h>

#include "log.h"
#include "global.h"

#include "http_normal.h"

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
  // digitalWrite(BUZZER_PIN, LOW );
}

void http_setup_normal() {
  httplog("HTTP web server initializing");
  server.on ( "/ping", HTTP_GET, http_handle_ping);
  server.on ( "/", HTTP_GET, http_handle_data );
  server.onNotFound ( http_handle_not_found );
  server.begin(); 
  httplog("HTTP web server initialized");
}

void http_loop_normal() {
}

#endif