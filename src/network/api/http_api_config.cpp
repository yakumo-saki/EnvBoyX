#include <Arduino.h>

#include "global.h"
#include "network/http_api.h"
#include "network/http_api_util.h"
#include "sensors/mhz19_uart.h"

extern HTTPWEBSERVER server;


void _get_config() {
  if (config.mhz19b != MHZ_USE_UART) {
    server.send(500, MIME_TEXT, F("MHZ19B IS DISABLED\n"));
    return;
  }

  bool abc = mhz_get_abc();

  if (abc) {
    server.send(200, MIME_TEXT, "ON\n");
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
}