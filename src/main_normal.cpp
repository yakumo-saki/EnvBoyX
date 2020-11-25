#include <Arduino.h>

#include <MQTTClient.h>

#include "log.h"
#include "global.h"
#include "display.h"
#include "sensors/bme280.h"
#include "sensors/adt7410.h"
#include "sensors/am2320.h"
#include "sensors/lps22hb.h"
#include "sensors/mhz19.h"
#include "sensors/tsl2561.h"
#include "http_normal.h"
#include "config.h"
#include "wifi.h"

WiFiClient net;
MQTTClient mqttClient;

int counter;

void begin_mqtt_connection() {
  
  Serial.println("MQTT Begin");
  mqttClient.begin(mqttBroker.c_str(), net);
  delay(10);

  Serial.println("MQTT Started successfully.");
}

void mqtt_publish(String topic, String value) {

  if (opMode != OPMODE_MQTT) {
    return;
  }
  
  String t = "/" + mqttName + "/" + topic;
  Serial.println("MQTT Publish topic=>" + t + " value=>" + value);
  mqttClient.publish(t, value);
}

void read_data() {

  mainlog("Reading data start.");

  // 電圧
  //  if (true) {
  //    int s0 = analogRead(0); 
  //    mqtt_publish("battery", String(s0));
  //  }

  // 精度の怪しいものから順に値を上書きしていく。
  bme_read_data();  // temp humi pres
  am_read_data();   // temp humi
  adt_read_data();  // temp
  lps_read_data();  // pres

  if (read_data_tsl2561()) { 
    // MQTT
    char buf[24] = "";
    sprintf(buf, "%2f", lastLuxFull);
    mqtt_publish("lux", buf);
  
    sprintf(buf, "%2f", lastLuxIr);
    mqtt_publish("luxIr", buf);
  } else {
    lastLuxFull = 0;
    lastLuxIr = 0;
  }

  if (use_mhz19b) {
    // 
    mhz_read_data();

    if (opMode == OPMODE_MQTT) {
      delay(3050);       // MHZデータ取得待ち
      mhz_read_data();   // ここは adhoc 
    }
    
    // MQTT: if ppm == -1 , MH-Z19 error.
    if (lastPpm > 0) {
      char buf[24] = "";
      sprintf(buf, "%d", lastPpm);
      mqtt_publish("co2ppm", buf);
    } else {
      // MH-Z19B read error. do nothing.
    }
  } 

  // MQTT
  char buf[24] = "";
  if (lastTemp != NAN) {
    sprintf(buf, "%2f", lastTemp);
    mqtt_publish("temp", buf);
  }
  
  if (lastHumidity != NAN) {
    sprintf(buf, "%2f", lastHumidity);
    mqtt_publish("humi", buf);
  }

  if (lastPressure != NAN) {
    sprintf(buf, "%2f", lastPressure);
    mqtt_publish("pres", buf);
  }
  
}

//
// SETUP
//
void setup_normal() {
  
  read_config();

  disp_normal_startup_screen(product_long);
  
  // setupモードに入りやすくするための処理
  if (opMode == OPMODE_DISPLAY) {
    sectionlog("Reset to reconfig start.");
    remove_configure_flag_file();
    // list_dir();

    disp_wait_for_reconfig();
  
    // 設定済みフラグファイル
    create_configure_flag_file();

    // list_dir();
    sectionlog("Reconfigure timeout. continue.");

  }

  // start WiFi
  sectionlog("Connecting WiFi.");
  disp_wifi_starting(1);
  make_sure_wifi_connected();
  disp_wifi_info(get_wifi_ip_addr(), mDNS);

  sectionlog("Starting mDNS server.");  
  start_mdns(mDNS);

  sectionlog("Starting HTTP server.");  
  http_setup_normal();

  sectionlog("Initializing sensors start.");
  bme_setup();
  adt_setup();
  am_setup();
  lps_setup();
  tsl_setup();

  if (use_mhz19b != MHZ_NOUSE) {
    mhz_setup();
  }
  sectionlog("Initializing sensors done.");

}

//
// LOOP
//
void loop_normal() {

  mainlog("loop start");

  // WiFiが繋がってなければ意味がないので接続チェック
  make_sure_wifi_connected();

  mainlog("WiFi connected.");

  // MQTT
  if (opMode == OPMODE_MQTT) {
    begin_mqtt_connection();
    
    mqttClient.loop();
    delay(10);  // <- fixes some issues with WiFi stability
  
    Serial.println("MQTT Connect");
    while (!mqttClient.connect(mDNS.c_str(), "", "")) { // username and password not support
      Serial.print(".");
      delay(1000);
    }
    Serial.println("");
  }
  
  read_data();

  // sleep to next.
  if (opMode == OPMODE_MQTT) {
    disp_sensor_value(get_wifi_ip_addr(), mDNS);
    delay(1000);
    disp_power_off();
  
    // if (NO_DEEP_SLEEP) {
    //   mainlog("!!! NOT deep sleep because of NO_DEEP_SLEEP is set !!!");
    //   delay(NO_DEEP_SLEEP_DURATION);
    //   mainlog("!!! Going to next loop                             !!!");
    // } else {
    //   delay(500);
    //   mainlog("*** Goto deep sleep ***");
    //   ESP.deepSleep(NORMAL_DURATION);
    //   delay(10000);
    // }
  } else if (opMode == OPMODE_DISPLAY) {  
    disp_sensor_value(get_wifi_ip_addr(), mDNS);
    http_loop_normal();
    sectionlog("Wait for Next tick.");
    delay(750);
  }
}
