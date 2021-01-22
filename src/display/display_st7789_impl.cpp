#ifdef ESP32

#include <Arduino.h>

#include "global.h"
#include "structs.h"
#include "log.h"

#include <WiFiClient.h>

#include <TFT_eSPI.h>
#include <SPI.h>

#include "display_st7789_big.h"
#include "display_st7789_normal.h"

extern int disp_switch;
const int ST7789_PWM_LED_CHANNEL = 0;
const int ST7789_PWM_FREQ = 5000;
const int ST7789_PWM_RESOLUTION = 8;

int st7789_last_brightness = 255;

TFT_eSPI tft = TFT_eSPI(); // Invoke library

const int DEFAULT_FONT = 4;

/**
 * ＢＩＧモードか否かを返す
 */
bool isBigMode()
{
	return config.st7789Mode == ST7789_MODE_BIG;
}

void clear_screen() {
	tft.fillScreen(TFT_BLACK);

	if (config.st7789Mode == ST7789_MODE_BIG) {
		_clear_screen_big();
	} else {
		_clear_screen_normal();
	}
}

/**
 * 起動時の画面表示（共通）
 */
void disp_st7789_normal_startup_screen(String product_long)
{

	tft.startWrite();
	tft.fillScreen(TFT_BLACK);
	tft.setCursor(0, 0, DEFAULT_FONT);
	tft.setTextColor(TFT_WHITE, TFT_BLACK);

	tft.println("startup");

	tft.endWrite();
}

/**
 * セットアップモード時のディスプレイ表示
 */
void disp_st7789_setup_startup_screen(String ipAddr)
{
	return; // cant support this.
}

/**
 * WiFi接続中表示
 * @param wait_print_row Please wait を何行目に表示するか(0,1,2)
 */
void disp_st7789_wifi_starting(int wait_print_row)
{

	tft.startWrite();
	clear_screen();
	tft.setCursor(0, 0, DEFAULT_FONT);
	tft.setTextColor(TFT_WHITE, TFT_BLACK);

	tft.println("WiFi starting");

	tft.endWrite();
}

void disp_st7789_wifi_info(String ip, String mDNS)
{

	tft.startWrite();
	clear_screen();
	tft.setCursor(0, 0, DEFAULT_FONT);
	tft.setTextColor(TFT_WHITE, TFT_BLACK);

	tft.println("WiFi info:");
	tft.println(ip);
	tft.println(mDNS);

	tft.endWrite();
}

void disp_st7789_wifi_error()
{

	tft.startWrite();
	clear_screen();
	tft.fillScreen(TFT_RED);
	tft.setCursor(0, 0, DEFAULT_FONT);
	tft.setTextColor(TFT_YELLOW, TFT_BLACK);

	tft.println("WiFi ERROR");

	tft.endWrite();
}

/**
 * wait for reconfigure 画面を出しながら待つ
 */
void disp_st7789_wait_for_reconfig_init()
{

	tft.startWrite();

	clear_screen();
	tft.setCursor(0, 0, DEFAULT_FONT);
	tft.setTextColor(TFT_WHITE, TFT_BLACK);

	tft.println("Wait for reconfig");

	tft.endWrite();
}

void disp_st7789_wait_for_reconfig_bar(int now, const int max)
{
	tft.setCursor(0 + (now * 8), 100, DEFAULT_FONT);
	tft.print("-");
}


void disp_st7789_power_off()
{
	// display.displayOff();
}


/**
 * メイン画面に移る前の初期化
 */
void disp_st7789_all_initialize_complete(String ip, String mdns)
{
	// clear entire screen. because of sensor value screen uses partial update
	// to prevent screen flickering.
	tft.startWrite();
	tft.fillScreen(TFT_BLACK);

	if (isBigMode())
	{
		_disp_header_big(ip, mdns);
	}
	else
	{
		_disp_header_normal(ip, mdns);
	}
	tft.endWrite();
}

/**
 * 通常画面
 */
void disp_st7789_sensor_value(disp_values_t new_values, disp_values_t last_values)
{

	if (isBigMode())
	{
		_disp_sensor_value_big(new_values);
	}
	else
	{
		_disp_sensor_value_normal(new_values);
	}
}

String disp_st7789_set_brightness(int brightness)
{
	ledcSetup(ST7789_PWM_LED_CHANNEL, ST7789_PWM_FREQ, ST7789_PWM_RESOLUTION);
	ledcAttachPin(TFT_BL, ST7789_PWM_LED_CHANNEL);

	ledcWrite(ST7789_PWM_LED_CHANNEL, brightness);

	st7789_last_brightness = brightness;

	return "PWM Setting done. (0-255) value=" + String(brightness);
}

void disp_st7789_set_power(bool poweron)
{
	if (poweron) {
		disp_st7789_set_brightness(st7789_last_brightness);
	} else {
		disp_st7789_set_brightness(0);
	}
}

void setup_disp_st7789()
{
	tft.init();
	clear_screen();
	stlog("Initialized.");
}

#endif