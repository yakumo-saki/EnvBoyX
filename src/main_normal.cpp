#include <Arduino.h>

#include <MQTTClient.h>
#include <TimerCall.h>
#include <ArduinoJson.h>

#include "log.h"
#include "global.h"
#include "display.h"
#include "sensors/bme280.h"
#include "sensors/adt7410.h"
#include "sensors/am2320.h"
#include "sensors/bh1750.h"
#include "sensors/lps22hb.h"
#include "sensors/mhz19.h"
#include "sensors/tsl2561.h"
#include "http_normal.h"
#include "config.h"
#include "wifi.h"
#include "mdns_client.h"

#include "i2c.h"
#include "main_normal_mqtt.h"
#include "watchdog.h"

WiFiClient net;

TimerCall timer = TimerCall();
extern String stasticsJSON;

void init_sensors()
{
	sectionlog(F("Initializing sensors start."));
	bme_setup();
	adt_setup();
	am_setup();
	bh_setup();
	lps_setup();
	tsl_setup();

	if (config.mhz19b != MHZ_NOUSE)
	{
		mhz_setup();
	}
	sectionlog(F("Initializing sensors done."));
}

void call_disp_sensor_value() {
	disp_sensor_value(get_wifi_ip_addr(), config.mDNS);
}

void updateStastics(std::vector<TimerCall::TimerCallTask> &tasks) {

	const String STAT = "stastics";

	DynamicJsonDocument doc(4096);
	doc["time"] = millis();

	int idx = 0;
    for (auto it = tasks.begin(), e = tasks.end(); it != e; ++it) {
        statlog(
           + "name=" + String(it->info.name)
           + " last=" + String(it->info.lastExecMills)
           + " last exec=" + String(it->info.lastElapsedMills)
           + " total=" + String(it->info.totalElapsedMills)
           + " count=" + String(it->info.callCount)
        );

		// 統計
		doc[STAT][idx]["name"] = String(it->info.name);
        doc[STAT][idx]["lastExecMs"] = it->info.lastExecMills;
        doc[STAT][idx]["lastElapsedMs"] = it->info.lastElapsedMills;
        doc[STAT][idx]["totalElapsedMs"] = it->info.totalElapsedMills;
        doc[STAT][idx]["callCount"] = it->info.callCount;
		idx++;
    }

	String json = "";
	serializeJson(doc, json);
	stasticsJSON = json;

}

void init_timer() {
	mainlog("TimerCall version: " + String(timer.VERSION, 2));
	timer.add(bme_read_data, "BME280", 1000); 
	timer.add(am_read_data, "AM2320", 2000);
	timer.add(adt_read_data, "ADT7410", 1000);
	timer.add(lps_read_data, "LPS22HB", 1000);
	timer.add(read_data_tsl2561, "TSL2561", 1000);
	timer.add(bh_read_data, "BH1750", 2000);
	timer.add(mhz_read_data, "MHZ19B", 3000);

	// 画面表示はセンサー読み込みよりあとに実行したいので最後に追加する
	timer.add(call_disp_sensor_value, "DISP", 1000);
	timer.addStasticsFunction(updateStastics, "STAT", 60000);
}

/**
 * 通常起動モードのSETUP
 */
void setup_normal()
{

	read_config();

	// Init I2C Serial
	init_i2c(I2C_SDA, I2C_SCL);

	if (config.opMode == OPMODE_MQTT)
	{
		setup_normal_mqtt();
		return; // MQTTモードの場合はもう戻ってこない（ディープスリープする）
	}

	sectionlog(F("Start watchdog"));
	setup_watchdog();

	
	sectionlog(F("Setup display"));
	setup_display();
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

	init_sensors();

	// TimerCall
	init_timer();
	timer.forceRunStasticsOnce();

	// 初期化終了時に画面表示をどうにかできるフック
	disp_all_initialize_complete(get_wifi_ip_addr(), config.mDNS);

	//
}

/**
 * LOOP
 */
void loop_normal()
{
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
