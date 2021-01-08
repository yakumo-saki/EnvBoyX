#include <Arduino.h>

#include "log.h"
#include "global.h"

String http_normal_not_found_html() {
  String message = "File Not Found\n\n";
  return message;
}

String get_time_string(unsigned long ms) {
  unsigned long second = ms / 1000;
  unsigned long minute = second / 60;

  int sec = second % 60;
  int min = (second / 60) % 60;
  int hr =  minute / 60;

  char buf[400];
  snprintf (buf, sizeof buf,"%02d:%02d:%02d", hr, min, sec);
  return String(buf);
}

String http_normal_data_json() {

  unsigned long ms = millis();
  String timeString = get_time_string(ms);

  char temp[16], hum[16], pres[16];
  char lux[16], luxIr[16],ppm[16];
  char product[32];
  product_long.toCharArray(product, sizeof(product), 0);

  char buf[400];
  snprintf ( buf, sizeof buf,
    "{ \"product\": \"%s\", \"uptime\": \"%s\", \"uptimeMills\": \"%lu\", \"temparature\": \"%s\","
    " \"humidity\": \"%s\", \"pressure\": \"%s\",\"luminous\": \"%s\", \"luminousIr\": \"%s\""
    ", \"co2ppm\": \"%s\", \"co2ppmAccuracy\": \"%s\" }"
    , product, timeString.c_str(), ms
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
  unsigned long ms = millis();
  String timeString = get_time_string(ms);

  snprintf ( temp, sizeof temp,
    "{ \"uptime\": \"%s\", \"uptimeMills\": \"%lu\" }\n"
    , timeString.c_str(), ms
  );

  String json = "return ping: " + String(temp);
  httplog(json);
  return json;
}

