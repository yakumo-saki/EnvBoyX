#include <Arduino.h>

// using sodaq_lps22hb ver 1.0.0
#include <Sodaq_LPS22HB.h>

// test purpose only. false for production
bool NO_DEEP_SLEEP = false;

WiFiClient net;
MQTTClient mqttClient;

// deepsleep duration (us)
const int REBOOT_NOW = 1 * 1000 * 1000;         // us
const int NO_DEEP_SLEEP_DURATION = 10 * 1000;   // ms
const int NORMAL_DURATION = 180 * 1000 * 1000;   // us

int sensorPin = A0;    // select the input pin for the potentiometer

int counter;

// 最後に取得した値
float lastTemp;
float lastHumidity;
float lastPressure;
float lastLuxFull;
float lastLuxIr;
int lastPpm;

//
// SETUP
//
void setup_normal() {
  
  read_config();

  disp_normal_startup_screen(product_long);
  
  // setupモードに入りやすくするための処理
  if (opMode == OPMODE_DISPLAY) {
    Serial.println(">>> Reset to reconfig start.");
    LittleFS.remove(configured_file);
    list_dir();

    disp_wait_for_reconfig();
  
    // 設定済みフラグファイル
    File f = LittleFS.open(configured_file, "w");
    f.println("ok");
    f.close();

    list_dir();
    Serial.println("=== reconfigure timeout. continue.");

    http_setup();
  }

  // start WiFi
  make_sure_wifi_connected();
  disp_wifi_info(WiFi.localIP().toString(), mDNS);

  Serial.println(">>> Initializing sensors start.");
  bme_setup();
  adt_setup();
  am_setup();
  lps_setup();
  tsl_setup();

  if (use_mhz19b != MHZ_NOUSE) {
    mhz_setup();
  }
  Serial.println("=== Initializing sensors done.");

}

//
// LOOP
//
void loop_normal() {

  Serial.println("");

  // WiFiが繋がってなければ意味がないので接続チェック
  make_sure_wifi_connected();

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
    disp_sensor_value(WiFi.localIP().toString(), mDNS);
    delay(1000);
    disp_power_off();
  
    if (NO_DEEP_SLEEP) {
      Serial.println("!!! NOT deep sleep because of NO_DEEP_SLEEP is set !!!");
      delay(NO_DEEP_SLEEP_DURATION);
      Serial.println("!!! Going to next loop                             !!!");
    } else {
      delay(500);
      Serial.println("*** Goto deep sleep ***");
      ESP.deepSleep(NORMAL_DURATION, WAKE_RF_DEFAULT);
      delay(10000);
    }
  } else if (opMode == OPMODE_DISPLAY) {  
    disp_sensor_value(WiFi.localIP().toString(), mDNS);
    http_loop();
    Serial.println("display: next tick.");
    delay(1500);
  }
}

//
// 設定読込
void read_config() {
  File f = LittleFS.open(settings, "r");
  String settingId = f.readStringUntil('\n'); // 使わない
  ssid = f.readStringUntil('\n');
  password = f.readStringUntil('\n');
  mDNS = f.readStringUntil('\n');  
  opMode = f.readStringUntil('\n');
  use_mhz19b = f.readStringUntil('\n');
  mhz19b_pwmpin = f.readStringUntil('\n');
  mqttBroker = f.readStringUntil('\n');
  mqttName = f.readStringUntil('\n');  
  f.close();

  settingId.trim();
  ssid.trim();
  password.trim();
  mDNS.trim();
  opMode.trim();
  use_mhz19b.trim();
  mqttBroker.trim();
  mqttName.trim();

  Serial.println("S-ID: " + settingId);
  Serial.println("SSID: " + ssid);
  Serial.println("PASS: " + password);
  Serial.println("mDNS: " + mDNS);
  Serial.println("opMode: " + opMode);
  Serial.println("use MHZ19B: " + use_mhz19b);
  Serial.println("MQTT Broker: " + mqttBroker);
  Serial.println("MQTT Name  : " + mqttName);  
}

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

void make_sure_wifi_connected() {
  WiFi.softAPdisconnect(true);
  WiFi.enableAP(false);

  if (WiFi.status() == WL_CONNECTED) {
    return;
  }

  Serial.println("WiFi is down or not initialized. connecting");
  WiFi.disconnect();

  int retryCount = 0;
  WiFi.begin(ssid.c_str(), password.c_str());
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
    retryCount++;
    if (retryCount % 10 == 0) {
      delay(100);
      WiFi.disconnect();   
      delay(100);
      WiFi.begin(ssid.c_str(), password.c_str());
      Serial.println("");
      Serial.println("Still reconnecting WiFi");
    }

    if (retryCount > 38) {
      Serial.println("");
      Serial.println("WiFi connect failure. restarting");
      ESP.deepSleep(REBOOT_NOW, WAKE_RF_DEFAULT);
      delay(10000);
    }
  }

  Serial.println("");
  Serial.println("WiFi (re) connected.");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
}
