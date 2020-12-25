// NORMAL_MODEとSETUPMODEでserverを共有するため
#ifdef ESP32
#include <WiFi.h>
#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>

AsyncWebServer server(80);

#elif defined(ESP8266)
#include <ESP8266WebServer.h>

ESP8266WebServer server(80);
#endif
