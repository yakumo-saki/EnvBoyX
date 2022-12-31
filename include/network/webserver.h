#ifdef ESP32
#include <WebServer.h>
extern WebServer server;
typedef WebServer HTTPWEBSERVER;
#endif

#ifdef ESP8266
#include <ESP8266WebServer.h>
extern ESP8266WebServer server;
typedef ESP8266WebServer HTTPWEBSERVER;
#endif
