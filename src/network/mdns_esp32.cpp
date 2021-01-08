#ifdef ESP32

#include <Arduino.h>

#include <ESPmDNS.h>

#include "global.h"
#include "config.h"
#include "log.h"

bool start_mdns(String name) {
  char n[name.length() + 1];
  name.toCharArray(n, sizeof n);

  mdnslog("mDNS responder starting");

  if (!MDNS.begin(n)) {
    mdnslog("Error setting up MDNS responder!");
    return false;
  }
  mdnslog("mDNS responder started name=" + name);
  return true;
}

void mdns_setup() {
  sectionlog("Starting mDNS server.");  
  start_mdns(config.mDNS);
}

void mdns_loop() {
}
#endif