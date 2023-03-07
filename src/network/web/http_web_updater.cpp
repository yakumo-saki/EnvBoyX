#include <Arduino.h>

#ifdef ESP32
#include <Update.h>
#endif

#include "config/config.h"
#include "global.h"
#include "log.h"
#include "network/http_utils.h"
#include "network/webserver.h"

#include "util/watchdog.h"

// very thanks to 
// https://garretlab.web.fc2.com/arduino/esp32/examples/ArduinoOTA/OTAWebUpdater.html

#ifdef ESP32

#include "embed/update_html.h"
#include "embed/update_done_html.h"
#include "embed/update_fail_html.h"

void handle_post_update() {
  server.sendHeader("Connection", "close");

  if (Update.hasError()) {
    server.send(200, "text/html", UPDATE_FAIL_HTML);
  } else {
    server.send(200, "text/html", UPDATE_DONE_HTML);
  }

  delay(3000);
  ESP.restart();
}

// このメソッドはアップロード中にすごい回数呼ばれる
void handle_post_update_sending() {
  watchdog_feed();

  HTTPUpload& upload = server.upload();

  if (upload.status == UPLOAD_FILE_START) {
    httplog("Update filename:" + upload.filename);
    if (!Update.begin(UPDATE_SIZE_UNKNOWN)) {  // start with max available size
      Update.printError(Serial);
    }
  } else if (upload.status == UPLOAD_FILE_WRITE) {
    /* flashing firmware to ESP*/
    if (Update.write(upload.buf, upload.currentSize) != upload.currentSize) {
      Update.printError(Serial);
    }
  } else if (upload.status == UPLOAD_FILE_END) {
    if (Update.end(true)) {  // true to set the size to the current progress
      httplog("Update Success size: " + String(upload.totalSize) + " bytes");
    } else {
      Update.printError(Serial);
    }
  }
}

void handle_get_update() {
  sendHttpHeader();
  server.sendContent(String(UPDATE_HTML));
}

void http_web_updater_setup() {

  server.on("/update", HTTP_GET, handle_get_update);
  server.on("/update", HTTP_POST, handle_post_update, handle_post_update_sending);
  
  httplog(F("HTTP web updater initialized"));
}
#endif

#ifndef ESP32
#include "embed/update_unsupport_html.h"

void handle_get_update() {
  sendHttpHeader();

  // TODO ESP8266 can supports updater...
  server.sendContent(UPDATE_UNSUPPORT_HTML);
}

void http_web_updater_setup() {
  server.on("/update", HTTP_GET, handle_get_update);
  httplog(F("HTTP web updater is not supported."));
}
#endif
