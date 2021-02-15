
#include <Arduino.h>

#ifdef ESP32
#include <ESPmDNS.h>
#endif
#ifdef ESP8266
#include <ESP8266mDNS.h>
#endif

#include "global.h"
#include "config.h"
#include "log.h"

bool start_mdns(String name) {
  char n[name.length() + 1];
  name.toCharArray(n, sizeof n);

  mdnslog(F("mDNS responder starting"));

  if (!MDNS.begin(n)) {
    mdnslog(F("Error setting up MDNS responder!"));
    return false;
  }
  mdnslog("mDNS responder started name=" + name);
  return true;
}

void mdns_setup() {
  sectionlog(F("Starting mDNS server."));  
  start_mdns(config.mDNS);
}

void mdns_loop() {
  // do nothing
}