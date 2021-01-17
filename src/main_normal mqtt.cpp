#include <Arduino.h>

#include <MQTTClient.h>
#include <TimerCall.h>

#include "log.h"
#include "global.h"
#include "config.h"
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
  mqttClient.begin(config.mqttBroker.c_str(), net);
  delay(10);

  Serial.println("MQTT Started successfully.");
}

void mqtt_publish(String topic, String value) {

  if (config.opMode != OPMODE_MQTT) {
    return;
  }
  
  String t = "/" + config.mqttName + "/" + topic;
  Serial.println("MQTT Publish topic=>" + t + " value=>" + value);
  mqttClient.publish(t, value);
}

/**
 * LOOP
 */
void send_and_sleep() {

  sectionlog("loop start");

  // WiFiが繋がってなければ意味がないので接続チェック
  make_sure_wifi_connected();

  mainlog("WiFi connected.");

  // MQTT
  begin_mqtt_connection();
  
  mqttClient.loop();
  delay(10);  // <- fixes some issues with WiFi stability
  
  mainlog("MQTT Connect begin");
  while (!mqttClient.connect(config.mDNS.c_str(), "", "")) { // username and password not support
    Serial.print(".");
    delay(1000);
  }
  mainlog("MQTT Connect OK");
  
  init_timer();
  timer.forceOnce();

  mqtt_publish("lux", String(sensorValues.lux, 0));
  mqtt_publish("luxIr", String(sensorValues.luxIr, 0));
  mqtt_publish("co2ppm", String(sensorValues.co2ppm,0));
  mqtt_publish("co2ppmAccuracy", sensorValues.co2ppmAccuracy);
  mqtt_publish("temperature",String(sensorValues.temperature, 2));
  mqtt_publish("humidity", String(sensorValues.humidity, 2));
  mqtt_publish("pressure", String(sensorValues.pressure, 1));

  if (NO_DEEP_SLEEP) {
    mainlog("!!! NOT deep sleep because of NO_DEEP_SLEEP is set !!!");
    delay(NO_DEEP_SLEEP_DURATION);
    mainlog("!!! Going to next loop                             !!!");
  } else {
    delay(500);
    mainlog("*** Goto deep sleep ***");
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
  sectionlog("Connecting WiFi.");
  make_sure_wifi_connected();

  init_sensors();

  send_and_sleep();
}

