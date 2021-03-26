#include <Arduino.h>

#include <MQTTClient.h>
#include <TimerCall.h>

#include "log.h"
#include "global.h"
#include "config.h"
#include "ConfigClass.h"
#include "wifi.h"
#include "main_normal.h"

extern WiFiClient net;
MQTTClient mqttClient;

bool NO_DEEP_SLEEP = false;
const int NO_DEEP_SLEEP_DURATION = 3000;
const int NORMAL_DURATION = 1000;

extern TimerCall timer;

void begin_mqtt_connection() {
  
  Serial.println("MQTT Begin");
  mqttClient.begin(config.get(ConfigNames::MQTT_BROKER).c_str(), net);
  delay(10);

  Serial.println("MQTT Started successfully.");
}

void mqtt_publish(String topic, String value) {

  if (config.get(ConfigNames::OPMODE) != ConfigValues::OPMODE_MQTT) {
    return;
  }
  
  String t = "/" + config.get(ConfigNames::MQTT_NAME) + "/" + topic;
  Serial.println("MQTT Publish topic=>" + t + " value=>" + value);
  mqttClient.publish(t, value);
}

/**
 * LOOP
 */
void send_and_sleep() {

  sectionlog(F("loop start"));

  // WiFiが繋がってなければ意味がないので接続チェック
  make_sure_wifi_connected();

  mainlog(F("WiFi connected."));

  // MQTT
  begin_mqtt_connection();
  
  mqttClient.loop();
  delay(10);  // <- fixes some issues with WiFi stability
  
  mainlog(F("MQTT Connect begin"));
  while (!mqttClient.connect(config.get(ConfigNames::MDNS).c_str(), "", "")) { // username and password not support
    Serial.print(".");
    delay(1000);
  }
  mainlog(F("MQTT Connect OK"));
  
  add_timer_tasks();
  timer.forceOnce();

  mqtt_publish("lux", String(sensorValues.lux, 0));
  mqtt_publish("luxIr", String(sensorValues.luxIr, 0));
  mqtt_publish("co2ppm", String(sensorValues.co2ppm,0));
  mqtt_publish("co2ppmAccuracy", String(sensorValues.co2ppmAccuracy));
  mqtt_publish("temperature",String(sensorValues.temperature, 2));
  mqtt_publish("humidity", String(sensorValues.humidity, 2));
  mqtt_publish("pressure", String(sensorValues.pressure, 1));

  if (NO_DEEP_SLEEP) {
    mainlog(F("!!! NOT deep sleep because of NO_DEEP_SLEEP is set !!!"));
    delay(NO_DEEP_SLEEP_DURATION);
    mainlog(F("!!! Going to next loop                             !!!"));
  } else {
    delay(500);
    mainlog(F("*** Goto deep sleep ***"));
    ESP.deepSleep(NORMAL_DURATION);
    delay(10000);
  }
}

/**
 * MQTT通常起動モードのSETUP
 */
void setup_normal_mqtt() {
  
  // read_config(); configは読み込み済
  // I2C は初期化済

  // start WiFi
  sectionlog(F("Connecting WiFi."));
  make_sure_wifi_connected();

  init_sensors();

  send_and_sleep();
}

