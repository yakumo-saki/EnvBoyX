#ifdef USER_SETUP_LOADED

#include <Arduino.h>

#include "global.h"
#include "structs.h"
#include "log.h"
#include "display/display_util.h"
#include "display/st7789_utils.h"

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

//
const int MAX_X  = 239;
const int MAX_Y = 134;

// x: という２文字の場合の幅
const int HEAD_WIDTH = 32;

// 
const int SIDE_LINE_WIDTH = 2;

const int LEFT_HEAD_X = SIDE_LINE_WIDTH + 3;
const int LEFT_HEAD_R_ALIGN_X = LEFT_HEAD_X + HEAD_WIDTH - 5; // 右寄せ T: H: P:
const int RIGHT_HEAD_X = 128;

const int LEFT_VAL_X = LEFT_HEAD_X + HEAD_WIDTH;
const int LEFT_VAL_CO2_X = LEFT_HEAD_X + HEAD_WIDTH + 24;

const int VALUE_WIDTH = RIGHT_HEAD_X - LEFT_HEAD_X - HEAD_WIDTH;
const int VALUE_WIDTH_LONG = VALUE_WIDTH + 12; // Pressure or CO2 ppm

const int RIGHT_VAL_X = LEFT_VAL_X + VALUE_WIDTH_LONG + 12;
const int RIGHT_VALUE_WIDTH = MAX_X - RIGHT_VAL_X - SIDE_LINE_WIDTH;

const int ROW_HEIGHT = 28;
const int ROW1_Y = 24;
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
	tft.drawString(product.substring(0, product.length() - 1), 3, 2, SMALL_FONT);
	tft.setTextColor(TFT_WHITE);
	tft.drawString(product.substring(0, product.length() - 1), 2, 1, SMALL_FONT);

	// X 影→本体の順で書かないと重なった部分が上書きされるのに注意
	tft.setTextColor(TFT_SILVER);
	tft.drawString("X", 51, 2, SMALL_FONT);
	tft.setTextColor(TFT_WHITE);
	tft.drawString("X", 50, 0, SMALL_FONT);

	// version
	tft.setTextColor(TFT_WHITE);
	tft.drawString(ver, 62, 0, XSMALL_FONT);

	tft.setTextSize(1);

	// Header
	tft.setTextDatum(TR_DATUM);
	tft.drawString(ip, 240, 0, XSMALL_FONT);
	tft.drawString(mDNS, 240, 12, XSMALL_FONT);
	tft.setTextDatum(TL_DATUM);

	// Row 1
	tft.setTextColor(TFT_WHITE);
	tft.setTextDatum(TR_DATUM);
	tft.drawString("T:", LEFT_HEAD_R_ALIGN_X, ROW1_Y, DEFAULT_FONT);

	if (sensorCharacters.co2ppm) {
		tft.setTextDatum(TL_DATUM);
		tft.drawString("L:", RIGHT_HEAD_X, ROW1_Y, DEFAULT_FONT);
	}

	// Row 2
	tft.setTextColor(TFT_WHITE);
	tft.setTextDatum(TR_DATUM);
	tft.drawString("H:", LEFT_HEAD_R_ALIGN_X, ROW2_Y, DEFAULT_FONT);

	// Row 3
	tft.setTextColor(TFT_WHITE);
	tft.setTextDatum(TR_DATUM);
	tft.drawString("P:", LEFT_HEAD_R_ALIGN_X, ROW3_Y, DEFAULT_FONT);

	// Row 4
	if (sensorCharacters.co2ppm) {
		tft.setTextColor(TFT_WHITE);
		tft.setTextDatum(TL_DATUM);
		tft.drawString("CO", LEFT_HEAD_X, ROW4_Y + 1, DEFAULT_FONT);
		tft.drawString("2", LEFT_HEAD_X + 38, ROW4_Y - 1, SMALL_FONT);
		tft.drawString(":", LEFT_HEAD_X + 44, ROW4_Y, DEFAULT_FONT);
	} else {
		tft.setTextDatum(TR_DATUM);
		tft.drawString("L:", LEFT_HEAD_R_ALIGN_X, ROW4_Y, DEFAULT_FONT);
	}
}

// 値を書き込む（単位あり、フォント違い）
void write_value(String value, int x, int y, int width, int valueFont, value_alert_t alert, String unit, int unitFont) {

	// 値と単位のフォントの高さを調整するための値を決める
	int unitFontDiffY = 0;
	if (valueFont == DEFAULT_FONT) {
		if (unitFont == SMALL_FONT) {
			unitFontDiffY = 6;
		}
	}

	if (alert.warning) {
		tft.setTextColor(TFT_RED, TFT_BLACK);
	} else if (alert.caution) {
		tft.setTextColor(TFT_YELLOW, TFT_BLACK);
	} else {
		tft.setTextColor(TFT_WHITE, TFT_BLACK);
	}

	tft.setTextPadding(width);
	int written = tft.drawString(value, x, y, valueFont);

	if (unit != "") {
		tft.setTextPadding(width - written);
		tft.drawString(unit, x + written + 1, y + unitFontDiffY, unitFont);
	}

	tft.setTextColor(TFT_WHITE, TFT_BLACK);
}

// 値を書き込む（単位無し or 値と同じフォント）
void write_value(String value, int x, int y, int width, int font, value_alert_t alert) {
	write_value(value, x, y, width, font, alert, "", DEFAULT_FONT);
}

// Delta値表示
void _write_value_delta(int x, int y, delta_value_t delta, String unit) {
	// 気圧の変化
	int deltaStrX = x + 20;
	tft.setTextPadding(60);

	// 気圧deltaの文字
	if (delta.positive) {
		tft.setTextColor(TFT_GREENYELLOW, TFT_BLACK);
	} else if (delta.negative) {
		tft.setTextColor(TFT_ORANGE, TFT_BLACK);
	} else {
		tft.setTextColor(TFT_WHITE, TFT_BLACK);
	}

	// tft.drawString(delta.formattedValue, deltaStrX, y, DEFAULT_FONT);
	value_alert_t alert;
	alert.caution = false;
	alert.warning = false;
	if (unit.length() > 1) {
		write_value(delta.formattedValue, deltaStrX, y, 60, DEFAULT_FONT, alert, unit, SMALL_FONT);
	} else {
		write_value(delta.formattedValue, deltaStrX, y, 60, DEFAULT_FONT, alert, unit, DEFAULT_FONT);
	}


	// 気圧deltaのアイコン
	if (delta.positive && delta.drawIcon) {
		draw_arrow(x, y, ARROW_ICON::UP, TFT_GREENYELLOW);
	} else if (delta.negative && delta.drawIcon) {
		draw_arrow(x, y, ARROW_ICON::DOWN, TFT_ORANGE);
	} else {
		draw_arrow(x, y, ARROW_ICON::NONE, TFT_ORANGE);
	}

}

/**
 * @param val 値
 */
void _disp_sensor_value_normal(disp_values_t values, value_alerts_t alerts)
{

	static int disp_switch = 0;

	tft.setTextColor(TFT_WHITE, TFT_BLACK);
	tft.setTextSize(1);
	tft.setTextDatum(TL_DATUM);

	// ========================
	// Row 1
	// ========================
	write_value(values.temperature, LEFT_VAL_X, ROW1_Y, VALUE_WIDTH, DEFAULT_FONT, alerts.temperature);

	if (sensorCharacters.co2ppm || disp_switch < 3) {
		// 通常表示
		if (sensorCharacters.co2ppm) {
			write_value(values.lux, RIGHT_VAL_X, ROW1_Y, RIGHT_VALUE_WIDTH, DEFAULT_FONT, alerts.lux);
		}
	} else {
		// delta
		value_alert_t alert;
		write_value("", RIGHT_VAL_X, ROW1_Y, RIGHT_VALUE_WIDTH, DEFAULT_FONT, alert);
		delta_value_t delta = get_delta_struct(deltaValues.temperature, true);
		_write_value_delta(RIGHT_VAL_X, ROW1_Y, delta, "c");
	}

	// ========================
	// Row 2
	// ========================
	write_value(values.humidity, LEFT_VAL_X, ROW2_Y, VALUE_WIDTH, DEFAULT_FONT, alerts.humidity);

	{
		delta_value_t delta = get_delta_struct(deltaValues.humidity, true);
		_write_value_delta(RIGHT_VAL_X, ROW2_Y, delta, "%");
	}

	// ========================
	// Row 3
	// ========================
	write_value(values.pressure, LEFT_VAL_X, ROW3_Y, VALUE_WIDTH_LONG, DEFAULT_FONT, alerts.pressure, "hPa", SMALL_FONT);

	// Air pressure delta
	tft.setTextPadding(VALUE_WIDTH);

	// 気圧の変化
	{
		delta_value_t delta = get_delta_struct(deltaValues.pressure, true);
		_write_value_delta(RIGHT_VAL_X, ROW3_Y, delta, "hPa");
	}

	// ========================
	// Row 4
	// ========================
	if (sensorCharacters.co2ppm) {
		tft.setTextColor(TFT_WHITE, TFT_BLACK);
		write_value(values.co2ppm, LEFT_VAL_CO2_X, ROW4_Y, VALUE_WIDTH_LONG, DEFAULT_FONT, alerts.co2, "ppm", SMALL_FONT);
	} else {
		write_value(values.lux, LEFT_VAL_X, ROW4_Y, VALUE_WIDTH, DEFAULT_FONT, alerts.lux);
	}

	{
		delta_value_t delta = get_delta_struct(deltaValues.co2ppm, true);
		_write_value_delta(RIGHT_VAL_X, ROW4_Y, delta, "ppm");
	}

	// ========================
	// Alert Sidebar
	// ========================
	if (has_caution(alerts)) {
		tft.fillRect(0, ROW1_Y, SIDE_LINE_WIDTH, MAX_Y, TFT_YELLOW);
		tft.fillRect(MAX_X - SIDE_LINE_WIDTH, ROW1_Y, MAX_X, MAX_Y, TFT_YELLOW);
	} else if (has_warning(alerts)) {
		tft.fillRect(0, ROW1_Y, SIDE_LINE_WIDTH, MAX_Y, TFT_RED);
		tft.fillRect(MAX_X - SIDE_LINE_WIDTH, ROW1_Y, MAX_X, MAX_Y, TFT_RED);
	} else {
		tft.fillRect(0, ROW1_Y, SIDE_LINE_WIDTH, MAX_Y, TFT_BLACK);
		tft.fillRect(MAX_X - SIDE_LINE_WIDTH, ROW1_Y, MAX_X, MAX_Y, TFT_BLACK);
	}

	// disp_switch ++
	disp_switch++;
	if (disp_switch > 5) disp_switch = 0;
}

#endif