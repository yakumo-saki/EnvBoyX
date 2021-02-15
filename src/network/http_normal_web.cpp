
#include <Arduino.h>

#include "log.h"
#include "global.h"
#include "config.h"

#include "http_normal.h"

#include "network/http_api.h"
#include "network/http_api_base.h"
#include "network/http_api_display.h"
#include "network/http_api_mhz.h"

extern HTTPWEBSERVER server;

void http_setup_normal() {
  httplog("HTTP web server initializing");
  
  http_api_display_setup();
  http_api_base_setup();
  http_api_mhz_setup();

  server.begin(); 
  httplog("HTTP web server initialized");
}

void http_loop_normal() {
  server.handleClient();
}


