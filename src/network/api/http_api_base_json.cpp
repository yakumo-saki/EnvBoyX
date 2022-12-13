#include <Arduino.h>
#include <ArduinoJson.h>

#include "log.h"
#include "global.h"

#include "network/http_api_util.h"

String http_normal_data_json() {

  unsigned long ms = millis();
  String timeString = getTimeString(ms);

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
