#include <Arduino.h>
#include <ArduinoJson.h>

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

  char buf[10];
  snprintf (buf, sizeof buf,"%02d:%02d:%02d", hr, min, sec);
  return String(buf);
}

String http_normal_data_json() {

  unsigned long ms = millis();
  String timeString = get_time_string(ms);

  char temp[10], hum[10], pres[10];
  char lux[10], luxIr[10],ppm[10];

  DynamicJsonDocument doc(3000);
  doc["product"] = product;
  doc["uptime"] = timeString;
  doc["uptimeMills"] = ms;
  doc["temparature"] = dtostrf(sensorValues.temperature, 0, 2, temp);
  doc["humidity"] = dtostrf(sensorValues.humidity, 0, 2, hum);
  doc["pressure"] = dtostrf(sensorValues.pressure, 0, 2, pres);
  doc["luminous"] = dtostrf(sensorValues.lux, 0, 0, lux);
  doc["luminousIr"] = dtostrf(sensorValues.luxIr, 0, 0, luxIr);
  doc["co2ppm"] = dtostrf(sensorValues.co2ppm, 0, 0, ppm);
  doc["co2ppmAccuracy"] =  sensorValues.co2ppmAccuracy;
  doc["rssi"] = sensorValues.rssi;
  doc["freeHeap"] = sensorValues.freeHeap;

  String json;
  serializeJson(doc, json);
  return json;
}

String http_normal_ping_json() {

  unsigned long ms = millis();
  String timeString = get_time_string(ms);

  DynamicJsonDocument doc(2000);
  doc["product"] = product;
  doc["uptime"] = timeString;
  doc["uptimeMills"] = ms;

  String json;
  serializeJson(doc, json);

  httplog(json);
  return json;
}
