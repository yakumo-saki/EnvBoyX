
#ifdef ESP32
#include <WebServer.h>
typedef WebServer HTTPWEBSERVER;
#endif

#ifdef ESP8266
#include <ESP8266WebServer.h>
typedef ESP8266WebServer HTTPWEBSERVER;
#endif
