#include <Arduino.h>

#include "log.h"
#include "global.h"

String http_normal_not_found_html() {
  String message = "File Not Found\n\n";
  return message;
}

String http_normal_data_json() {

  char buf[400];
  int sec = millis() / 1000;
  int min = (int) ((sec / 60) % 60);
  int hr = (int) ((min / 60) % 60);

  char temp[16], hum[16], pres[16];
  char lux[16], luxIr[16],ppm[16];
  char product[32];
  product_long.toCharArray(product, sizeof(product), 0);

  snprintf ( buf, sizeof buf,
    "{ \"product\": \"%s\", \"uptime\": \"%02d:%02d:%02d\", \"uptimeMills\": \"%lu\", \"temparature\": \"%s\","
    " \"humidity\": \"%s\", \"pressure\": \"%s\",\"luminous\": \"%s\", \"luminousIr\": \"%s\""
    ", \"co2ppm\": \"%s\", \"co2ppmAccuracy\": \"%s\" }"
    , product, hr, min, sec, millis()
    , dtostrf(sensorValues.temperature, 0, 2, temp)
    , dtostrf(sensorValues.humidity, 0, 2, hum)
    , dtostrf(sensorValues.pressure, 0, 2, pres)
    , dtostrf(sensorValues.lux, 0, 0, lux)
    , dtostrf(sensorValues.luxIr, 0, 0, luxIr)
    , dtostrf(sensorValues.co2ppm, 0, 0, ppm)
    , sensorValues.co2ppmAccuracy.c_str()
  );

  String json = String(buf);
  httplog("return json: " + json);
  return json;
}

String http_normal_ping_json() {

  char temp[100];
  int sec = millis() / 1000;
  int min = sec / 60;
  int hr = min / 60;

  snprintf ( temp, sizeof temp,
    "{ \"uptime\": \"%02d:%02d:%02d\", \"uptimeMills\": \"%lu\" }\n"
    , hr, min % 60, sec % 60, millis()
  );

  String json = "return ping: " + String(temp);
  httplog(json);
  return json;
}

