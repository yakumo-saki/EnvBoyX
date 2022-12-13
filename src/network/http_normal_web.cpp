
#include <Arduino.h>

#include "log.h"
#include "global.h"
#include "config.h"

#include "http_normal.h"

#include "network/http_not_found.h"
#include "network/http_api.h"
#include "network/http_api_ping.h"
#include "network/http_api_base.h"
#include "network/http_api_display.h"
#include "network/http_api_mhz.h"
#include "network/http_api_config.h"

#include "network/http_web_config.h"


extern HTTPWEBSERVER server;

void http_setup_normal() {
  httplog("HTTP web server initializing");
  
  http_not_found_setup();
  http_api_ping_setup();
  http_api_display_setup();
  http_api_base_setup();
  http_api_mhz_setup();
  http_api_config_setup();
  http_web_config_setup();
  
  server.begin(); 
  httplog("HTTP web server initialized");
}

void http_loop_normal() {
  server.handleClient();
}
