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

#include "i2c.h"
#include "main_normal_mqtt.h"

WiFiClient net;

void read_data() {

  mainlog("Reading sensor data start.");

  // 精度の怪しいものから順に値を上書きしていく。
  bme_read_data();  // temp humi pres
  am_read_data();   // temp humi
  adt_read_data();  // temp
  lps_read_data();  // pres

  if (read_data_tsl2561()) { 
    // MQTT
    char buf[24] = "";
    sprintf(buf, "%2f", sensorValues.lux);
      sprintf(buf, "%2f", sensorValues.luxIr);
  } else {
    sensorValues.lux = 0;
    sensorValues.luxIr = 0;
  }

  if (config.use_mhz19b) {
    // 
    if (config.opMode == OPMODE_MQTT) {
        delay(3050);       // MHZデータ取得待ち
        mhz_read_data();   // ここは adhoc 

      // MQTT: if ppm == -1 , MH-Z19 error.
      if (sensorValues.co2ppm > 0) {
        char buf[24] = "";
        sprintf(buf, "%d", sensorValues.co2ppm);
      } else {
      // MH-Z19B read error. do nothing.
      }
    } else {
        mhz_read_data();
    }
    
  } 

  mainlog("Reading sensor data complete."); 
}

void init_sensors() {
  sectionlog("Initializing sensors start.");
  bme_setup();
  adt_setup();
  am_setup();
  lps_setup();
  tsl_setup();

  if (config.use_mhz19b != MHZ_NOUSE) {
    mhz_setup();
  }
  sectionlog("Initializing sensors done.");
}

/**
 * 通常起動モードのSETUP
 */
void setup_normal() {
  
  read_config();

  // Init I2C Serial
  init_i2c(I2C_SDA, I2C_SCL);

  if (config.opMode == OPMODE_MQTT) {
    setup_normal_mqtt();
    return; // MQTTモードの場合はもう戻ってこない（ディープスリープする）
  }

  setup_display();
  disp_normal_startup_screen(product_long);
  
  // setupモードに入りやすくするための処理
  sectionlog("Reset to reconfig start.");
  remove_configure_flag_file();

  disp_wait_for_reconfig();

  // 設定済みフラグファイル
  create_configure_flag_file();

  sectionlog("Reconfigure timeout. continue.");

  // start WiFi
  sectionlog("Connecting WiFi.");
  disp_wifi_starting(1);
  make_sure_wifi_connected();
  disp_wifi_info(get_wifi_ip_addr(), config.mDNS);

  sectionlog("Starting mDNS server.");  
  start_mdns(config.mDNS);

  sectionlog("Starting HTTP server.");  
  http_setup_normal();

  init_sensors();

  // 初期化終了時に画面表示をどうにかできるフック
  disp_all_initialize_complete(get_wifi_ip_addr(), config.mDNS);

}

/**
 * LOOP
 */
void loop_normal() {

  sectionlog("loop start");

  // WiFiが繋がってなければ意味がないので接続チェック
  make_sure_wifi_connected();

  mainlog("WiFi connected. IP=" + get_wifi_ip_addr());
  
  read_data();

  disp_sensor_value(get_wifi_ip_addr(), config.mDNS);

  http_loop_normal();

  mainlog("Wait for Next tick.");
  delay(1000);
}
