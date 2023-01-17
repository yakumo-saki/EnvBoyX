#include <Arduino.h>

#include "global.h"
#include "log.h"
#include "structs.h"

#include "wifiutil.h"
#include "config/config.h"
#include "ConfigClass.h"

#include "display/scan_alert.h"
#include "display/display_formatter.h"
#include "display/display_ssd1306.h"
#include "display/display_st7789.h"

bool displayInitialized = false;
disp_values_t disp_values;

/** SSD1306を使うか否か。 I2C接続なので自動チェック可能 */
bool use_ssd1306() {
	if (!displayInitialized) return false;
	if (!has_ssd1306()) return false;

	return true;
}

bool has_st7789() {
	return (config->get(ConfigNames::ST7789) == ConfigValues::ST7789_USE);
}

bool use_st7789() {
	if (!displayInitialized) return false;
	return has_st7789();
}

/**
 * セットアップモード時のディスプレイ表示
 */
void disp_setup_startup_screen(String ipAddr, String ssid) {

	static int disp_switch = 0;

	if (use_ssd1306()) {
		disp_ssd1306_setup_startup_screen(ipAddr, ssid, disp_switch);	
	}
	if (use_st7789()) {
		disp_st7789_setup_startup_screen(ipAddr, ssid, disp_switch);
	}

	disp_switch++;
	if (disp_switch > 5) {
		disp_switch = 0;
	}
}

void disp_message(bool isError, String msg1, String msg2, String msg3, String msg4) {
	if (use_ssd1306()) {
		disp_ssd1306_message(isError, msg1, msg2, msg3, msg4);	
	}
	if (use_st7789()) {
		disp_st7789_message(isError, msg1, msg2, msg3, msg4);	
	}
}


/**
 * WiFi接続中表示
 * @param wait_print_row Please wait を何行目に表示するか(0,1,2)
 */
void disp_wifi_starting(int wait_print_row) {
	if (use_ssd1306()) {
		disp_ssd1306_wifi_starting();
	}
	if (use_st7789()) {
		disp_st7789_wifi_starting();
	}
}

void disp_wifi_info(String ip, String mDNS) {
	if (use_ssd1306()) {
		disp_ssd1306_wifi_info(ip, mDNS);
	}
	if (use_st7789()) {
		disp_st7789_wifi_info(ip, mDNS);
	}
  
	delay(300);
}

void disp_wifi_error() {
	if (use_ssd1306()) {
		disp_ssd1306_wifi_error();
	}
	if (use_st7789()) {
		disp_st7789_wifi_error();
	}
}

/**
 * wait for reconfigure 画面を出しながら待つ
 */
void disp_wait_for_reconfig() {

	const int WAIT_PER_BAR = 33; // ms per bar
	const int MAX_BAR = 30;

	if (use_st7789()) {
		disp_st7789_wait_for_reconfig_init();
	}
	if (use_ssd1306()) {
		disp_ssd1306_wait_for_reconfig_init();
	}

	displog(F("Wait for reconfigure start"));
	for (int i = 0; i < MAX_BAR; i++)
	{
		if (use_st7789()) {
			disp_st7789_wait_for_reconfig_bar(i + 1, MAX_BAR);
		}
		if (use_ssd1306()) {
			disp_ssd1306_wait_for_reconfig_bar(i + 1, MAX_BAR);
		}
	
	    delay(WAIT_PER_BAR);
	}
	displog(F("Wait for reconfigure end"));

}

// 初期化完了、センサー値表示画面のヘッダ等変更されない部分を描画する
void disp_all_initialize_complete(String ip, String mdns) {
	if (use_ssd1306()) {
		disp_ssd1306_all_initialize_complete();
	}
	if (use_st7789()) {
		disp_st7789_all_initialize_complete(ip, mdns);
	}
}

/**
 * 通常画面
 */
void disp_sensor_value(String ip, String mdns) {
	
    value_alerts_t alerts = check_for_alerts();
	disp_values_t last_values = disp_values;
	disp_values = create_disp_values();
	disp_values.ip = ip;
	disp_values.mDNS = mdns;
	
	if (use_ssd1306()) {
		disp_ssd1306_sensor_value(disp_values, alerts);
	}
	if (use_st7789()) {
		disp_st7789_sensor_value(disp_values, alerts);
	}

}

void disp_power_off() {
	if (use_ssd1306()) {
		disp_ssd1306_power_off();
	}
	if (use_st7789()) {
		disp_st7789_power_off();
	}
}

/**
 * 液晶の明るさを変更する 
 * @return 応答メッセージ
 */
String disp_set_brightness(int brightness) {
	String ret = "";
	String msg = "";
	if (use_ssd1306()) {
		msg = "SSD1306: " + disp_ssd1306_set_brightness(brightness);
		displog(msg);
		ret += msg + "\n";
	}
	if (use_st7789()) {
		msg = "ST7789: " + disp_st7789_set_brightness(brightness);
		displog(msg);
		ret += msg + "\n";
	}
	return ret;
}

void disp_set_power(bool poweron) {
	if (use_ssd1306()) {
		disp_ssd1306_set_power(poweron);
	}
	if (use_st7789()) {
		disp_st7789_set_power(poweron);
	}
}

void setup_display() {
	if (has_ssd1306()) {
		setup_disp_ssd1306();
	} else {
		ssdlog(F("SSD1306 not found."));
	}
	if (has_st7789()) {
		setup_disp_st7789();
	}

	// initialize configured brightness
	disp_set_brightness(config->get(ConfigNames::DISPLAY_BRIGHTNESS).toInt());

	displayInitialized = true;
}

/**
 * APIでconfigが変更された際に、値表示画面を書き直す処理
 */
void disp_redraw_sensor_value_screen() {
	displog("Redraw start");

	disp_set_brightness(config->get(ConfigNames::DISPLAY_BRIGHTNESS).toInt());
	String ip = get_wifi_ip_addr();
	disp_all_initialize_complete(ip, config->get(ConfigNames::MDNS));
	disp_sensor_value(ip, config->get(ConfigNames::MDNS));

	displog("Redraw complete");
}