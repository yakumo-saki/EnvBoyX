#ifdef ESP32

#include <Arduino.h>

#include "global.h"
#include "structs.h"
#include "log.h"

#include <WiFiClient.h>

#include <TFT_eSPI.h>
#include <SPI.h>

extern int disp_switch;
const int ST7789_PWM_LED_CHANNEL = 0;
const int ST7789_PWM_FREQ = 5000;
const int ST7789_PWM_RESOLUTION = 8;

int st7789_last_brightness = 255;

TFT_eSPI tft = TFT_eSPI(); // Invoke library

// フォント番号は0,2,4,6 は同一フォントのサイズ違い。
// フォント番号 7 は8セグフォント
const int XSMALL_FONT = 1;
const int SMALL_FONT = 2;
const int DEFAULT_FONT = 4;
const int LARGE_FONT = 6;
const int DIGIT_FONT = 7;

// x: という２文字の場合の幅
const int HEAD_WIDTH = 32;

const int LEFT_HEAD_X = 0;
const int RIGHT_HEAD_X = 124;

const int LEFT_VAL_X = LEFT_HEAD_X + HEAD_WIDTH;
const int RIGHT_VAL_X = RIGHT_HEAD_X + HEAD_WIDTH;

const int VALUE_WIDTH = RIGHT_HEAD_X - LEFT_HEAD_X - HEAD_WIDTH;
const int VALUE_WIDTH_LONG = 180; // Pressure or CO2 ppm

const int ROW_HEIGHT = 27;
const int ROW1_Y = 26;
const int ROW2_Y = ROW1_Y + ROW_HEIGHT;
const int ROW3_Y = ROW2_Y + ROW_HEIGHT;
const int ROW4_Y = ROW3_Y + ROW_HEIGHT;

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
		// 縦画面
		if (config.displayFlip == DISPLAY_FLIP_OFF) {
			tft.setRotation(2);
		} else {
			tft.setRotation(0);
		}
	} else {
		// 横画面
		if (config.displayFlip == DISPLAY_FLIP_OFF) {
			tft.setRotation(1);
		} else {
			tft.setRotation(3);
		}
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

/**
 * ＢＩＧモード：ヘッダ部分のみを表示する（この部分は変更されない）
 */
void _disp_header_big(String ip, String mDNS)
{
	if (config.displayFlip == DISPLAY_FLIP_OFF) {
		tft.setRotation(2);
	} else {
		tft.setRotation(0);
	}
}

/**
 * ノーマルモード：ヘッダ部分のみを表示する（この部分は変更されない）
 */
void _disp_header_normal(String ip, String mDNS)
{
	clear_screen();

	tft.setTextColor(TFT_WHITE);

	// Logo
	tft.setTextDatum(TL_DATUM);

	// EnvBoy
	tft.setTextColor(TFT_DARKGREY);
	tft.drawString(product.substring(0, product.length() - 1), 3, 7, SMALL_FONT);
	tft.setTextColor(TFT_WHITE);
	tft.drawString(product.substring(0, product.length() - 1), 2, 6, SMALL_FONT);

	// X 影→本体の順で書かないと重なった部分が上書きされるのに注意
	tft.setTextColor(TFT_DARKGREY);
	tft.drawString("X", 53, 0, DEFAULT_FONT);
	tft.setTextColor(TFT_WHITE);
	tft.drawString("X", 49, 0, DEFAULT_FONT);

	// version
	tft.setTextColor(TFT_WHITE);
	tft.drawString(ver, 70, 0, XSMALL_FONT);

	tft.setTextSize(1);

	// Header
	tft.setTextDatum(TR_DATUM);
	tft.drawString(ip, 240, 0, XSMALL_FONT);
	tft.drawString(mDNS, 240, 12, XSMALL_FONT);
	tft.setTextDatum(TL_DATUM);

	// Row 1
	tft.setTextColor(TFT_WHITE);
	tft.drawString("T:", LEFT_HEAD_X, ROW1_Y, DEFAULT_FONT);
	tft.drawString("L:", RIGHT_HEAD_X, ROW1_Y, DEFAULT_FONT);

	// Row 2
	tft.setTextColor(TFT_WHITE);
	tft.drawString("H:", LEFT_HEAD_X, ROW2_Y, DEFAULT_FONT);

	// Row 3
	tft.setTextColor(TFT_WHITE);
	tft.drawString("P:", LEFT_HEAD_X, ROW3_Y, DEFAULT_FONT);

	// Row 4
	tft.setTextColor(TFT_WHITE);
	tft.drawString("CO2:", LEFT_HEAD_X, ROW4_Y, DEFAULT_FONT);
}

void disp_st7789_power_off()
{
	// display.displayOff();
}

/**
 * BIGモード（縦）
 * @param val 値
 * @param erase true => 黒文字で書く（要するに消す） false => 普通に書く
 */
void _disp_sensor_value_big(disp_values_t val)
{

	const int R_HEIGHT = 56;
	const int Y_PADDING = 2;
	long R1_X = 117;
	long R1_Y = 0;
	long R2_X = 114;
	long R2_Y = R1_Y + R_HEIGHT;
	long R3_X = 127;
	long R3_Y = R2_Y + R_HEIGHT + Y_PADDING + 4;
	long R4_X = 127;
	long R4_Y = R3_Y + R_HEIGHT + Y_PADDING + 6;

	sensor_values_t v = sensorValues;

	tft.setTextColor(TFT_WHITE, TFT_BLACK);

	tft.setTextDatum(TL_DATUM);
	tft.drawString(ver, R1_X + 3, R1_Y, XSMALL_FONT);

	tft.setTextDatum(TR_DATUM);
	tft.setTextPadding(R1_X);
	tft.drawString(String(v.temperature, 1), R1_X, R1_Y, DIGIT_FONT);

	tft.setTextDatum(TL_DATUM);
	tft.drawString("c", R1_X + 3, R1_Y + 30, DEFAULT_FONT);

	tft.setTextDatum(TR_DATUM);
	tft.setTextPadding(R2_X);
	tft.drawString(String(v.humidity, 0), R2_X, R2_Y, DIGIT_FONT);

	tft.setTextDatum(TL_DATUM);
	tft.drawString("%", 114, R2_Y + 24, DEFAULT_FONT);

	// Row 3
	// この行はわざと左にはみ出るようにしているので、右揃えに出来ない
	// 気圧は 1000しかないので、1の左の隙間を節約している
	tft.setTextDatum(TL_DATUM);
	tft.drawString("h", R3_X + 1, R3_Y + 3, SMALL_FONT);
	tft.drawString("p", R3_X + 1, R3_Y + 15, SMALL_FONT);
	tft.drawString("a", R3_X + 1, R3_Y + 31, SMALL_FONT);

	tft.setTextDatum(TR_DATUM);
	tft.setTextPadding(R2_X);
	tft.drawString(String(v.pressure, 0), R3_X, R3_Y, DIGIT_FONT);

	tft.drawFastHLine(4, R3_Y + 28 + 27, TFT_WIDTH - 6, TFT_DARKGREY);
	tft.drawFastHLine(4, R3_Y + 28 + 28, TFT_WIDTH - 6, TFT_DARKGREY);

	// Row 4
	tft.setTextDatum(TL_DATUM);
	tft.drawString("p", R4_X + 1, R4_Y + 2, SMALL_FONT);
	tft.drawString("p", R4_X + 1, R4_Y + 20, SMALL_FONT);
	tft.drawString("m", R4_X + 1, R4_Y + 36, SMALL_FONT);
	tft.setTextDatum(TR_DATUM);
	tft.setTextPadding(R2_X);
	tft.drawString(String(v.co2ppm), R4_X, R4_Y, DIGIT_FONT);
}

/**
 * @param val 値
 * @param erase true => 黒文字で書く（要するに消す） false => 普通に書く
 * eraseは一応機能するが
 */
void _disp_sensor_value_normal(disp_values_t val)
{
	tft.setTextColor(TFT_WHITE, TFT_BLACK);
	tft.setTextSize(1);

	// Row 1
	tft.setTextPadding(VALUE_WIDTH);
	tft.drawString(val.temperature, LEFT_VAL_X, ROW1_Y, DEFAULT_FONT);

	tft.setTextPadding(VALUE_WIDTH);
	tft.drawString(val.lux, RIGHT_VAL_X, ROW1_Y, DEFAULT_FONT);

	// Row 2
	tft.setTextPadding(VALUE_WIDTH);
	tft.drawString(val.humidity, LEFT_VAL_X, ROW2_Y, DEFAULT_FONT);

	// Row 3
	tft.setTextPadding(VALUE_WIDTH_LONG);
	tft.drawString(val.pressure, LEFT_VAL_X, ROW3_Y, DEFAULT_FONT);

	// Row 4
	tft.setTextPadding(VALUE_WIDTH_LONG);
	tft.drawString(val.co2ppm, LEFT_VAL_X + HEAD_WIDTH, ROW4_Y, DEFAULT_FONT);
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

	return "PWM Setting done.";
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
	tft.setRotation(3);
	tft.fillScreen(TFT_BLACK);
	stlog("ST7789 Initialized.");
}

#endif