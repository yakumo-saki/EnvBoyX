#include <Arduino.h>
#include <Update.h>

#include "config.h"
#include "global.h"
#include "log.h"
#include "network/http_utils.h"
#include "network/webserver.h"

#include "watchdog.h"
#include "embed/update_html.h"

// very thanks to 
// https://garretlab.web.fc2.com/arduino/esp32/examples/ArduinoOTA/OTAWebUpdater.html

void handle_post_update() {
  server.sendHeader("Connection", "close");

  if (Update.hasError()) {
    server.send(200, "text/html", "FAIL");
  } else {
    server.send(200, "text/html", "OKOKOKOKOKOKOKOKOKOK");
  }

  delay(3000);
  ESP.restart();
}

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
