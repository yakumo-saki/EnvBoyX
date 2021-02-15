#include <Arduino.h>

#include "global.h"
#include "network/http_api.h"
#include "network/http_api_util.h"
#include "sensors/mhz19_uart.h"

extern HTTPWEBSERVER server;


void _set_mhz_abc() {
  if (config.mhz19b != MHZ_USE_UART) {
    server.send(500, MIME_TEXT, F("MHZ19B IS DISABLED\n"));
    return;
  }

  String abc = server.arg("value");
  if (!abc || abc == "") {
    server.send(400, MIME_TEXT, F("'value' parameter is missing.\n"));
    return;
  }

  bool onoff = parseBooleanString(abc);

  bool success = mhz_set_abc(onoff);

  if (success) {
    if (onoff) {
      server.send( 200, MIME_TEXT, "OK. ABC is enabled. value=" + abc);
    } else {
      server.send( 200, MIME_TEXT, "OK. ABC is disabled. value=" + abc);
    }
  } else {
    server.send( 500, MIME_TEXT, "Setting failed." );
  }
}

void _get_mhz_abc() {
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

void _mhz_zero_calibration() {
  if (config.mhz19b != MHZ_USE_UART) {
    server.send(500, MIME_TEXT, F("MHZ19B IS DISABLED\n"));
    return;
  }

  bool success = mhz_do_zero_calibration();

  if (success) {
    server.send(200, MIME_TEXT, "OK\n");
  } else {
    server.send(200, MIME_TEXT, "FAILED\n");
  }
}

void http_api_mhz_setup() {
  server.on ( "/mhz19b/abc", HTTP_GET, _get_mhz_abc );
  server.on ( "/mhz19b/abc", HTTP_POST, _set_mhz_abc );
  server.on ( "/mhz19b/zeroCalibration", HTTP_POST, _mhz_zero_calibration );
}