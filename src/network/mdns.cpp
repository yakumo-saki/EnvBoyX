
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

  mdnslog(F("mDNS responder starting"));

  MDNS.setInstanceName("EnvBoyX");
  MDNS.addService("_http", "_tcp", 80);

  if (!MDNS.begin(name.c_str())) {
    mdnslog(F("Error setting up MDNS responder!"));
    return false;
  }
  mdnslog("mDNS responder started name=" + name);
  return true;
}

void mdns_hostname_change(String hostname) {
  MDNS.end();
  MDNS.begin(hostname.c_str());
}

void mdns_setup() {
  sectionlog(F("Starting mDNS server."));  
  start_mdns(config.mDNS);
}

void mdns_loop() {
  // do nothing
}
