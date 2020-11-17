#ifdef ESP8266

#include <Arduino.h>

#include "http.h"
#include "log.h"
#include "global.h"
#include "config.h"

#include "http_normal.h"

#include <ESP8266WebServer.h>
extern ESP8266WebServer server;

void http_handle_not_found() {
  String message = http_normal_not_found_html();
  server.send(200, "text/html", message);
}

void http_handle_data() {
  String message = http_normal_data_json();
  server.send(200, "text/html", message);
}

void http_handle_ping() {
  String message = http_normal_ping_json();
  server.send(200, "text/html", message);
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
  server.handleClient();
}


#endif