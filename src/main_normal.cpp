#include <Arduino.h>
#include <ArduinoJson.h>
#include <MQTTClient.h>
#include <TimerCall.h>

#include "config.h"
#include "display.h"
#include "global.h"
#include "halt.h"
#include "http_normal.h"
#include "i2c.h"
#include "log.h"
#include "main_normal_mqtt.h"
#include "mdns_client.h"
#include "sensors/adt7410.h"
#include "sensors/am2320.h"
#include "sensors/bh1750.h"
#include "sensors/bme280.h"
#include "sensors/lps22hb.h"
#include "sensors/mhz19.h"
#include "sensors/tsl2561.h"
#include "watchdog.h"
#include "wifi.h"

WiFiClient net;

extern TimerCall timer;
extern String stasticsJSON;

bool merge_sensor_characters(sensor_characters_t c) {
  sensorCharacters.temperature = sensorCharacters.temperature || c.temperature;
  sensorCharacters.humidity = sensorCharacters.humidity || c.humidity;
  sensorCharacters.pressure = sensorCharacters.pressure || c.pressure;
  sensorCharacters.lux = sensorCharacters.lux || c.lux;
  sensorCharacters.luxIr = sensorCharacters.luxIr || c.luxIr;
  sensorCharacters.co2ppm = sensorCharacters.co2ppm || c.co2ppm;
  sensorCharacters.co2ppmAccuracy =
      sensorCharacters.co2ppmAccuracy || c.co2ppmAccuracy;

  return true;
}

void init_sensors() {
  sectionlog(F("Initializing sensors start."));
  if (bme_setup()) {
    merge_sensor_characters(bme_characters());
    timer.add(bme_read_data, "BME280", 1000);
  }

  if (adt_setup()) {
    merge_sensor_characters(adt_characters());
    timer.add(adt_read_data, "ADT7410", 1000);
  }

  if (am_setup()) {
    merge_sensor_characters(am_characters());
    timer.add(am_read_data, "AM2320", 2000);
  }

  if (bh_setup()) {
    merge_sensor_characters(bh_characters());
    timer.add(bh_read_data, "BH1750", 2000);
  }

  if (lps_setup()) {
    merge_sensor_characters(lps_characters());
    timer.add(lps_read_data, "LPS22HB", 1000);
  }

  if (tsl_setup()) {
    merge_sensor_characters(tsl_characters());
    timer.add(read_data_tsl2561, "TSL2561", 1000);
  }

	if (mhz_setup()) {
		merge_sensor_characters(mhz_characters());
		timer.add(mhz_read_data, "MHZ19B", 3000);
	}

  sectionlog(F("Initializing sensors done."));
}

void call_disp_sensor_value() {
  disp_sensor_value(get_wifi_ip_addr(), config.mDNS);
}

// 統計情報を取得
void updateStastics(std::vector<TimerCall::TimerCallTask> &tasks) {
  const String STAT = "stastics";

  DynamicJsonDocument doc(500);
  doc["time"] = millis();

  int idx = 0;
  for (auto it = tasks.begin(), e = tasks.end(); it != e; ++it) {
    statlog(+"name=" + String(it->info.name) +
            " last=" + String(it->info.lastExecMills) +
            " last exec=" + String(it->info.lastElapsedMills) +
            " total=" + String(it->info.totalElapsedMills) +
            " count=" + String(it->info.callCount));

    // 統計
    doc[STAT][idx]["name"] = String(it->info.name);
    doc[STAT][idx]["lastExecMs"] = it->info.lastExecMills;
    doc[STAT][idx]["lastElapsedMs"] = it->info.lastElapsedMills;
    doc[STAT][idx]["totalElapsedMs"] = it->info.totalElapsedMills;
    doc[STAT][idx]["callCount"] = it->info.callCount;
    idx++;
  }

#ifdef ESP32
  doc["cputemp"] = temperatureRead();  // CPU温度
#endif

  String json = "";
  serializeJson(doc, json);
  stasticsJSON = json;
}

void add_timer_tasks() {
  timer.add(wifi_store_rssi, "WIFI", 1000);

  // 画面表示はセンサー読み込みよりあとに実行したいので最後に追加する
  timer.add(call_disp_sensor_value, "DISP", 1000);
  timer.addStasticsFunction(updateStastics, "STAT", 60000);
}

/**
 * 通常起動モードのSETUP
 */
void setup_normal() {
  // Init I2C Serial
  init_i2c(I2C_SDA, I2C_SCL);

  bool config_ok = read_config();

  sectionlog(F("Setup display"));
  setup_display();

  if (!config_ok) {
    mainlog("[MAYBE BUG] Config read failed dropping to setup mode");
    remove_configure_flag_file();
    halt("config err", "setup again", "Reset now");
  }

  if (config.opMode == OPMODE_MQTT) {
    setup_normal_mqtt();
    return;  // MQTTモードの場合はもう戻ってこない（ディープスリープする）
  }

  sectionlog(F("Start watchdog"));
  setup_watchdog();

  // 起動画面
  disp_normal_startup_screen(product_long);

  // setupモードに入りやすくするための処理
  sectionlog(F("Reset to reconfig start."));
  remove_configure_flag_file();

  disp_wait_for_reconfig();

  // 設定済みフラグファイル
  create_configure_flag_file();

  sectionlog(F("Reconfigure timeout. continue."));

  // start WiFi
  sectionlog(F("Connecting WiFi."));
  disp_wifi_starting(1);
  make_sure_wifi_connected();
  disp_wifi_info(get_wifi_ip_addr(), config.mDNS);

  mdns_setup();

  sectionlog(F("Starting HTTP server."));
  http_setup_normal();

  mainlog("TimerCall version: " + String(timer.VERSION, 2));
  init_sensors();

  // TimerCall
  add_timer_tasks();
  timer.forceRunStasticsOnce();

  // 初期化終了時に画面表示をどうにかできるフック
  disp_all_initialize_complete(get_wifi_ip_addr(), config.mDNS);
}

/**
 * LOOP
 */
void loop_normal() {
  // sectionlog("loop start");
  watchdog_feed();

  // WiFiが繋がってなければ意味がないので接続チェック
  make_sure_wifi_connected();

  // mainlog("WiFi connected. IP=" + get_wifi_ip_addr());

  timer.start();
  timer.loop();

  http_loop_normal();

  // mainlog("Wait for Next tick.");

  watchdog_feed();
}
