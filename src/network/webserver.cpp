// NORMAL_MODEとSETUPMODEでserverを共有するため
#ifdef ESP32
#include <WiFi.h>
#include <AsyncTCP.h>
#include <WebServer.h>

typedef WebServer HTTPWEBSERVER;

HTTPWEBSERVER server(80);
#endif

#ifdef ESP8266
#include <ESP8266WebServer.h>
typedef ESP8266WebServer HTTPWEBSERVER;

HTTPWEBSERVER server(80);
#endif
