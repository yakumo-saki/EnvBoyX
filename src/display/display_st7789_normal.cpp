#ifdef ESP32

#include <Arduino.h>

#include "global.h"
#include "structs.h"
#include "log.h"

#include <WiFiClient.h>

#include <TFT_eSPI.h>
#include <SPI.h>

extern int disp_switch;

extern const int ST7789_PWM_LED_CHANNEL;
extern const int ST7789_PWM_FREQ;
extern const int ST7789_PWM_RESOLUTION;

extern TFT_eSPI tft;

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

void _clear_screen_normal() {
	tft.fillScreen(TFT_BLACK);

	// 横画面
	if (config.displayFlip == DISPLAY_FLIP_OFF) {
		tft.setRotation(1);
	} else {
		tft.setRotation(3);
	}
}

/**
 * ノーマルモード：ヘッダ部分のみを表示する（この部分は変更されない）
 */
void _disp_header_normal(String ip, String mDNS)
{
	_clear_screen_normal();

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

/**
 * @param val 値
 * @param erase true => 黒文字で書く（要するに消す） false => 普通に書く
 * eraseは一応機能するが
 */
void _disp_sensor_value_normal(disp_values_t values, value_alerts_t alerts)
{
	tft.setTextColor(TFT_WHITE, TFT_BLACK);
	tft.setTextSize(1);

	// Row 1
	tft.setTextPadding(VALUE_WIDTH);
	tft.drawString(values.temperature, LEFT_VAL_X, ROW1_Y, DEFAULT_FONT);

	tft.setTextPadding(VALUE_WIDTH);
	tft.drawString(values.lux, RIGHT_VAL_X, ROW1_Y, DEFAULT_FONT);

	// Row 2
	tft.setTextPadding(VALUE_WIDTH);
	tft.drawString(values.humidity, LEFT_VAL_X, ROW2_Y, DEFAULT_FONT);

	// Row 3
	tft.setTextPadding(VALUE_WIDTH_LONG);
	tft.drawString(values.pressure, LEFT_VAL_X, ROW3_Y, DEFAULT_FONT);

	// Row 4
	tft.setTextPadding(VALUE_WIDTH_LONG);
	tft.drawString(values.co2ppm, LEFT_VAL_X + HEAD_WIDTH, ROW4_Y, DEFAULT_FONT);
}

#endif