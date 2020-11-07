#include <Arduino.h>

#include "http.h"
#include "log.h"
#include "global.h"

void http_handle_not_found(AsyncWebServerRequest *request) {
  String message = "File Not Found\n\n";
  // message += "URI: ";
  // message += request->url();
  // message += "\nMethod: ";
  // message += ( request->methodToString() ) ? "GET" : "POST";
  // message += "\nArguments: ";
  // message += request->args();
  // message += "\n";

  // for ( uint8_t i = 0; i < server.args(); i++ ) {
  //   message += " " + server.argName ( i ) + ": " + server.arg ( i ) + "\n";
  // }

  httplog("404 Not found -> " + String(request->url()) );
  request->send ( 404, "text/plain", message );
}

void http_handle_data(AsyncWebServerRequest *request) {

  char buf[400];
  int sec = millis() / 1000;
  int min = (int) ((sec / 60) % 60);
  int hr = (int) ((min / 60) % 60);

  char temp[16], hum[16], pres[16];
  char lux[16], luxIr[16],ppm[16], acc[16];
  char product[32];
  product_long.toCharArray(product, sizeof(product), 0);

  snprintf ( buf, sizeof buf,
    "{ \"product\": \"%s\", \"uptime\": \"%02d:%02d:%02d\", \"uptimeMills\": \"%lu\", \"temparature\": \"%s\","
    " \"humidity\": \"%s\", \"pressure\": \"%s\",\"luminous\": \"%s\", \"luminousIr\": \"%s\""
    ", \"co2ppm\": \"%s\", \"co2ppmAccuracy\": \"%s\" }"
    , product, hr, min, sec, millis()
    , dtostrf(lastTemp, 0, 2, temp), dtostrf(lastHumidity, 0, 2, hum), dtostrf(lastPressure, 0, 2, pres)
    , dtostrf(lastLuxFull, 0, 0, lux), dtostrf(lastLuxIr, 0, 0, luxIr)
    , dtostrf(lastPpm, 0, 0, ppm), lastPpmAccuracy
  );

  httplog("return json: " + String(buf));
  request->send ( 200, F("application/json"), buf );
}

void http_handle_ping(AsyncWebServerRequest *request) {

  char temp[100];
  int sec = millis() / 1000;
  int min = sec / 60;
  int hr = min / 60;

  snprintf ( temp, sizeof temp,
    "{ \"uptime\": \"%02d:%02d:%02d\", \"uptimeMills\": \"%lu\" }\n"
    , hr, min % 60, sec % 60, millis()
  );

  httplog("return ping: " + String(temp));
  request->send ( 200, F("application/json"), temp );
  // digitalWrite(BUZZER_PIN, LOW );
}

void http_setup_normal() {
  httplog("HTTP web server initializing");
  server.on ( "/ping", HTTP_GET, http_handle_ping);
  server.on ( "/", HTTP_GET, http_handle_data );
  server.onNotFound ( http_handle_not_found );
  server.begin(); 
  httplog("HTTP web server initialized");
}

void http_loop_normal() {
}
