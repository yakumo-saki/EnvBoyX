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
  request->send ( 404, MIME_TEXT, message );
}

void http_handle_data(AsyncWebServerRequest *request) {
  String message = http_normal_data_json();
  request->send ( 200, MIME_JSON, message );
}

void http_handle_ping(AsyncWebServerRequest *request) {
  String message = http_normal_ping_json();
  request->send ( 200, MIME_JSON, message );
}

void http_handle_stastics(AsyncWebServerRequest *request) {
  request->send ( 200, MIME_JSON, stasticsJSON );
}


void http_handle_brightness(AsyncWebServerRequest *request) {
  if (!request->hasParam("value")) {
    request->send ( 400, MIME_TEXT, "no value param");
    return;
  }
  
  int brightness = request->getParam("value")->value().toInt();
  String msg = disp_set_brightness(brightness);

  String message = "OK\n" + msg;
  request->send ( 200, MIME_TEXT, message );
}

void http_handle_power(AsyncWebServerRequest *request) {
  if (!request->hasParam("value")) {
    request->send ( 400, MIME_TEXT, "no value param");
    return;
  }
  
  int power = request->getParam("value")->value().toInt();

  if (power == 1) {
    disp_set_power(true);
  } else {
    disp_set_power(false);
  }

  String message = "OK";
  request->send ( 200, MIME_TEXT, message );
}

void http_setup_normal() {
  httplog("HTTP web server initializing");
  server.on ( "/ping", HTTP_GET, http_handle_ping);
  server.on ( "/", HTTP_GET, http_handle_data );
  server.on ( "/brightness", HTTP_GET, http_handle_brightness );
  server.on ( "/display", HTTP_GET, http_handle_brightness );
  server.on ( "/stastics", HTTP_GET, http_handle_stastics );
  server.onNotFound ( http_handle_not_found );
  server.begin(); 
  httplog("HTTP web server initialized");
}

void http_loop_normal() {
}

#endif