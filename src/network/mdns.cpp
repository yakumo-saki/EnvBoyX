
#include <Arduino.h>

#ifdef ESP32
#include <ESPmDNS.h>
#endif
#ifdef ESP8266
#include <ESP8266mDNS.h>
#endif

#include "log.h"

#include "global.h"
#include "config.h"
#include "ConfigClass.h"

bool start_mdns(String name) {

  mdnslog(F("mDNS responder starting"));

  if (!MDNS.begin(name.c_str())) {
    mdnslog(F("Error setting up MDNS responder!"));
    return false;
  }

  MDNS.setInstanceName(product + (DEBUG_BUILD ? "_DEBUG_" : "_") + ver + "_" + minorVer + "-" + name);
  MDNS.addService("_http", "_tcp", 80);

  mdnslog("mDNS responder started name=" + name);
  return true;
}

void mdns_hostname_change(String hostname) {
  MDNS.end();
  MDNS.begin(hostname.c_str());
}

void mdns_setup() {
  sectionlog(F("Starting mDNS server."));  
  start_mdns(config->get(ConfigNames::MDNS));
}

void mdns_loop() {
  // do nothing
}
