
#include <Arduino.h>

#include "log.h"
#include "global.h"
#include "config/config.h"

#include "http_normal.h"

#include "network/webserver.h"
#include "network/http_not_found.h"
#include "network/api/basic_api.h"
#include "network/api/v1/display.h"
#include "network/api/v1/mhz19b.h"
#include "network/api/v1/config.h"
#include "network/api/v1/getdata.h"

#include "network/web/http_web_config.h"


extern HTTPWEBSERVER server;

void setup_http_setup() {
  httplog("HTTP web server initializing");
  
  http_not_found_setup();
  http_api_basic_setup();
  http_api_config_setup();

  http_web_config_setup();
  
  server.begin(); 
  httplog("HTTP web server initialized");
}

void loop_http_setup() {
  server.handleClient();
}
