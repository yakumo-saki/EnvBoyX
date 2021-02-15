#include <Arduino.h>

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

  String message = "OK";
  server.send( 200, MIME_TEXT, message );
}

void http_handle_brightness() {
  String value = server.arg("value");
  int brightness = value.toInt();
  String msg = disp_set_brightness(brightness);
  server.send(200, MIME_TEXT, "OK\n" + msg);
}

void http_api_display_setup() {
  server.on ( "/brightness", HTTP_GET, http_handle_brightness );
  server.on ( "/display", HTTP_GET, http_handle_power );
}