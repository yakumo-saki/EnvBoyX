#include <Arduino.h>
#include <ArduinoJson.h>

#include "config/config.h"

#include "log.h"
#include "global.h"
#include "display/display.h"
#include "network/webserver.h"
#include "network/api/api_util.h"


extern HTTPWEBSERVER server;

String http_normal_data_json() {

  unsigned long ms = millis();
  String timeString = getTimeString(ms);

  char temp[10], hum[10], pres[10];
  char lux[10], luxIr[10],ppm[10];

  DynamicJsonDocument doc(2000);
  doc["product"] = product;
  doc["uptime"] = timeString;
  doc["uptimeMills"] = ms;
  // TODO SenserValueAdjustment
  doc["temparature"] = dtostrf(sensorValues.temperature, 0, 2, temp);
  doc["humidity"] = dtostrf(sensorValues.humidity, 0, 2, hum);
  doc["pressure"] = dtostrf(sensorValues.pressure, 0, 2, pres);
  doc["luminous"] = dtostrf(sensorValues.lux, 0, 0, lux);
  doc["luminousIr"] = dtostrf(sensorValues.luxIr, 0, 0, luxIr);
  doc["co2ppm"] = dtostrf(sensorValues.co2ppm, 0, 0, ppm);
  doc["co2ppmAccuracy"] =  sensorValues.co2ppmAccuracy;
  doc["rssi"] = sensorValues.rssi;
  doc["freeHeap"] = sensorValues.freeHeap;
  doc["name"] = config->get(ConfigNames::MDNS);

  DynamicJsonDocument rawdoc(1000);
  rawdoc["temparature"] = dtostrf(sensorValues.temperature, 0, 2, temp);
  rawdoc["humidity"] = dtostrf(sensorValues.humidity, 0, 2, hum);
  rawdoc["pressure"] = dtostrf(sensorValues.pressure, 0, 2, pres);
  rawdoc["luminous"] = dtostrf(sensorValues.lux, 0, 0, lux);
  rawdoc["luminousIr"] = dtostrf(sensorValues.luxIr, 0, 0, luxIr);
  rawdoc["co2ppm"] = dtostrf(sensorValues.co2ppm, 0, 0, ppm);
  rawdoc["co2ppmAccuracy"] =  sensorValues.co2ppmAccuracy;

  doc["rawdata"] = rawdoc;


  String json;
  serializeJson(doc, json);
  doc.clear();
  
  return json;
}

void http_handle_data() {
  String message = http_normal_data_json();
  server.send(200, MimeType::JSON, message);
}

void http_api_getdata_setup() {
  server.on ( "/api/v1/getdata", HTTP_GET, http_handle_data );

  apilog("API getdata initialized.");
}