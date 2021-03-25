#ifdef USER_SETUP_LOADED

#include <Arduino.h>

#include "display/st7789_utils.h"
#include "display/display_util.h"

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

const int R_HEIGHT = 56;
const int Y_PADDING = 2;

// temperature X pos
const long R1_X = 117;

// temperature Y pos
const long R1_Y = 0;

// humidity X pos
const long R2_X = 114;

// humidity Y pos
const long R2_Y = R1_Y + R_HEIGHT;

// air pressure X pos
const long R3_X = 127;

// air pressure Y pos
const long R3_Y = R2_Y + R_HEIGHT + Y_PADDING + 4;
const long R4_X = 127;
const long R4_Y = R3_Y + R_HEIGHT + Y_PADDING + 6;

void _clear_screen_big() {
	tft.fillScreen(TFT_BLACK);

	// 縦画面
	if (config.get(ConfigNames::DISPLAY_FLIP) == ConfigValues::DISPLAY_FLIP_OFF) {
		tft.setRotation(2);
	} else {
		tft.setRotation(0);
	}
}

/**
 * ＢＩＧモード：ヘッダ部分のみを表示する（この部分は変更されない）
 */
void _disp_header_big(String ip, String mDNS)
{
	if (config.get(ConfigNames::DISPLAY_FLIP) == ConfigValues::DISPLAY_FLIP_OFF) {
		tft.setRotation(2);
	} else {
		tft.setRotation(0);
	}
}

void write_values(int x, int y, String value, value_alert_t alert, int align) {

	if (alert.warning) {
		tft.setTextColor(TFT_RED, TFT_BLACK);
	} else if (alert.caution) {
		tft.setTextColor(TFT_YELLOW, TFT_BLACK);
	} else {
		tft.setTextColor(TFT_WHITE, TFT_BLACK);		
	}

	tft.setTextDatum(align);
	tft.setTextPadding(x);
	tft.drawString(value, x, y, DIGIT_FONT);

	tft.setTextColor(TFT_WHITE, TFT_BLACK);
}

void _disp_draw_delta(int x, int y, delta_value_t delta) {

	tft.setTextDatum(TR_DATUM);
	if (delta.positive) {
		tft.setTextColor(TFT_GREENYELLOW, TFT_BLACK);
		tft.drawString(delta.formattedValue, x, y, DIGIT_FONT);
	} else if (delta.negative) {
		tft.setTextColor(TFT_ORANGE, TFT_BLACK);
		tft.drawString(delta.formattedValue, x, y, DIGIT_FONT);
	} else {
		tft.setTextColor(TFT_WHITE, TFT_BLACK); // ゼロ
		tft.drawString(delta.formattedValue, x, y, DIGIT_FONT);
	}

	if (delta.drawIcon && delta.positive) {
		draw_arrow(0 + 3, y + 8, ARROW_ICON::PLUS, TFT_GREENYELLOW, 34, 22);
	} else if (delta.drawIcon && delta.negative) {
		draw_arrow(0 + 3, y + 8, ARROW_ICON::MINUS, TFT_ORANGE, 34, 22);
	} else {
		draw_arrow(0 + 3, y + 8, ARROW_ICON::NONE, TFT_BLACK, 34, 22);
	}
}

/**
 * BIGモード（縦）
 * @param val 値
 * @param erase true => 黒文字で書く（要するに消す） false => 普通に書く
 */
void _disp_sensor_value_big(disp_values_t values, value_alerts_t alerts)
{
	static int tick = 0;
	static bool showDelta = false;

	sensor_values_t v = sensorValues;

	tft.setTextColor(TFT_WHITE, TFT_BLACK);

	// LINE 1
	tft.setTextDatum(TL_DATUM);
	tft.drawString(ver, R1_X + 3, R1_Y, XSMALL_FONT);

	tft.setTextDatum(TL_DATUM);
	tft.drawString("c", R1_X + 3, R1_Y + 30, DEFAULT_FONT);
	if (showDelta) {
		delta_value_t delta = get_delta_struct(deltaValues.temperature, true);
		_disp_draw_delta(R1_X, R1_Y, delta);
	} else {
		write_values(R1_X, R1_Y, String(v.temperature, 1), alerts.temperature, TR_DATUM);
	}

	// LINE 2
	tft.setTextDatum(TL_DATUM);
	tft.drawString("%", 114, R2_Y + 24, DEFAULT_FONT);
	// 気圧表示
	if (showDelta) {
		delta_value_t delta = get_delta_struct(deltaValues.humidity, true);
		_disp_draw_delta(R2_X, R2_Y, delta);
	} else {
		write_values(R2_X, R2_Y, String(v.humidity, 0), alerts.humidity, TR_DATUM);
	}

	// LINE 3
	tft.setTextDatum(TL_DATUM);
	tft.drawString("h", R3_X + 1, R3_Y + 3, SMALL_FONT);
	tft.drawString("p", R3_X + 1, R3_Y + 15, SMALL_FONT);
	tft.drawString("a", R3_X + 1, R3_Y + 31, SMALL_FONT);

	// 気圧表示
	if (showDelta) {
		delta_value_t delta = get_delta_struct(deltaValues.pressure, true);
		_disp_draw_delta(R3_X, R3_Y, delta);
	} else {
		write_values(R3_X, R3_Y, String(v.pressure, 0), alerts.pressure, TR_DATUM);
	}

	tft.drawFastHLine(4, R3_Y + 28 + 27, TFT_WIDTH - 6, TFT_DARKGREY);
	tft.drawFastHLine(4, R3_Y + 28 + 28, TFT_WIDTH - 6, TFT_DARKGREY);

	// LINE 4
	tft.setTextDatum(TL_DATUM);

	if (config.get(ConfigNames::MHZ19B) == ConfigValues::MHZ_NOUSE) {
		tft.drawString("L", R4_X + 1, R4_Y + 20, SMALL_FONT);
		tft.drawString("X", R4_X + 1, R4_Y + 36, SMALL_FONT);
		write_values(R4_X, R4_Y, String(v.lux, 0), alerts.lux, TR_DATUM);
	} else {
		tft.drawString("p", R4_X + 1, R4_Y + 2, SMALL_FONT);
		tft.drawString("p", R4_X + 1, R4_Y + 20, SMALL_FONT);
		tft.drawString("m", R4_X + 1, R4_Y + 36, SMALL_FONT);

		if (showDelta) {
			delta_value_t delta = get_delta_struct(deltaValues.co2ppm, true);
			_disp_draw_delta(R4_X, R4_Y, delta);
		} else {
			write_values(R4_X, R4_Y, String(v.co2ppm), alerts.co2, TR_DATUM);
		}
	}

	tick++;
	if (tick > 5) {
		tick = 0;
		showDelta = !showDelta;
	}
}

#endif