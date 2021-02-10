
#include <Arduino.h>

#include "log.h"
#include "global.h"
#include "config.h"

#include "http_normal.h"
#include "display/display.h"
#include "config.h"

#ifdef ESP32
#include <WebServer.h>
extern WebServer server;
#endif

#ifdef ESP8266
#include <ESP8266WebServer.h>
extern ESP8266WebServer server;
#endif

void http_handle_not_found() {
  String message = http_normal_not_found_html();
  server.send(200, MIME_HTML, message);
}

void http_handle_data() {
  String message = http_normal_data_json();
  server.send(200, MIME_JSON, message);
}

void http_handle_ping() {
  String message = http_normal_ping_json();
  server.send(200, MIME_JSON, message);
}

void http_handle_stastics() {
  server.send(200, MIME_JSON, stasticsJSON);
}

void http_handle_brightness() {
  String value = server.arg("value");
  int brightness = value.toInt();
  String msg = disp_set_brightness(brightness);
  server.send(200, MIME_TEXT, "OK\n" + msg);
}

void http_handle_power() {
  String power = server.arg("value");

  if (power == "1") {
    disp_set_power(true);
  } else {
    disp_set_power(false);
  }

  String message = "OK";
  server.send( 200, MIME_TEXT, message );
}

void http_handle_goto_setup() {

  remove_configure_flag_file();
  
  String message = "OK. Entering setup mode at next boot.";
  server.send( 200, MIME_TEXT, message );
}

void http_setup_normal() {
  httplog("HTTP web server initializing");
  server.on ( "/ping", HTTP_GET, http_handle_ping);
  server.on ( "/", HTTP_GET, http_handle_data );
  server.on ( "/brightness", HTTP_GET, http_handle_brightness );
  server.on ( "/display", HTTP_GET, http_handle_power );
  server.on ( "/stastics", HTTP_GET, http_handle_stastics );
  server.on ( "/goto_setup", HTTP_POST, http_handle_goto_setup );
  
  server.onNotFound ( http_handle_not_found );
  server.begin(); 
  httplog("HTTP web server initialized");
}

void http_loop_normal() {
  server.handleClient();
}


