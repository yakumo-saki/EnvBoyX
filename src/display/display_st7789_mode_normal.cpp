#ifdef ESP32

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
const int SIDE_LINE_WIDTH = 3;

const int LEFT_HEAD_X = SIDE_LINE_WIDTH + 3;
const int LEFT_HEAD_R_ALIGN_X = LEFT_HEAD_X + HEAD_WIDTH - 5; // 右寄せ T: H: P:
const int RIGHT_HEAD_X = 128;

const int LEFT_VAL_X = LEFT_HEAD_X + HEAD_WIDTH;
const int RIGHT_VAL_X = RIGHT_HEAD_X + HEAD_WIDTH;

const int VALUE_WIDTH = RIGHT_HEAD_X - LEFT_HEAD_X - HEAD_WIDTH;
const int VALUE_WIDTH_LONG = VALUE_WIDTH + 12; // Pressure or CO2 ppm

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
	tft.drawString("X", 53, 2, SMALL_FONT);
	tft.setTextColor(TFT_WHITE);
	tft.drawString("X", 50, 0, SMALL_FONT);

	// version
	tft.setTextColor(TFT_WHITE);
	tft.drawString(ver, 64, 0, XSMALL_FONT);

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

	tft.setTextDatum(TL_DATUM);
	tft.drawString("L:", RIGHT_HEAD_X, ROW1_Y, DEFAULT_FONT);

	// Row 2
	tft.setTextColor(TFT_WHITE);
	tft.setTextDatum(TR_DATUM);
	tft.drawString("H:", LEFT_HEAD_R_ALIGN_X, ROW2_Y, DEFAULT_FONT);

	// Row 3
	tft.setTextColor(TFT_WHITE);
	tft.setTextDatum(TR_DATUM);
	tft.drawString("P:", LEFT_HEAD_R_ALIGN_X, ROW3_Y, DEFAULT_FONT);

	// Row 4
	tft.setTextColor(TFT_WHITE);
	tft.setTextDatum(TL_DATUM);
	tft.drawString("CO2:", LEFT_HEAD_X, ROW4_Y, DEFAULT_FONT);

}

void write_value(String value, int x, int y, int width, int font, value_alert_t alert) {
	if (alert.warning) {
		tft.setTextColor(TFT_RED, TFT_BLACK);
	} else if (alert.caution) {
		tft.setTextColor(TFT_YELLOW, TFT_BLACK);
	} else {
		tft.setTextColor(TFT_WHITE, TFT_BLACK);
	}

	tft.setTextPadding(width);
	tft.drawString(value, x, y, font);

	tft.setTextColor(TFT_WHITE, TFT_BLACK);

}


/**
 * @param val 値
 */
void _disp_sensor_value_normal(disp_values_t values, value_alerts_t alerts)
{
	tft.setTextColor(TFT_WHITE, TFT_BLACK);
	tft.setTextSize(1);

	// Row 1
	write_value(values.temperature, LEFT_VAL_X, ROW1_Y, VALUE_WIDTH, DEFAULT_FONT, alerts.temperature);

	write_value(values.lux, RIGHT_VAL_X, ROW1_Y, VALUE_WIDTH, DEFAULT_FONT, alerts.lux);

	// Row 2
	write_value(values.humidity, LEFT_VAL_X, ROW2_Y, VALUE_WIDTH, DEFAULT_FONT, alerts.humidity);

	// Row 3
	write_value(values.pressure, LEFT_VAL_X, ROW3_Y, VALUE_WIDTH_LONG, DEFAULT_FONT, alerts.pressure);

	// Air pressure delta
	tft.setTextPadding(VALUE_WIDTH);

	int presDeltaX = LEFT_VAL_X + VALUE_WIDTH_LONG + 20;
	pressure_delta_t delta = get_pressure_delta_struct(sensorValues.pressureDelta);

	// 気圧の変化
	int presDeltaStrX = presDeltaX + 20;
	tft.setTextPadding(60);

	// 気圧deltaの文字
	if (delta.positive) {
		tft.setTextColor(TFT_GREENYELLOW, TFT_BLACK);
	} else if (delta.negative) {
		tft.setTextColor(TFT_ORANGE, TFT_BLACK);
	} else {
		tft.setTextColor(TFT_WHITE, TFT_BLACK);
	}

	tft.drawString(delta.formattedValue, presDeltaStrX, ROW3_Y, DEFAULT_FONT);

	// 気圧deltaのアイコン
	if (delta.positive && delta.drawIcon) {
		draw_arrow(presDeltaX, ROW3_Y, ARROW_ICON::UP, TFT_GREENYELLOW);
	} else if (delta.negative && delta.drawIcon) {
		draw_arrow(presDeltaX, ROW3_Y, ARROW_ICON::DOWN, TFT_ORANGE);
	} else {
		draw_arrow(presDeltaX, ROW3_Y, ARROW_ICON::NONE, TFT_ORANGE);
	}

	// Row 4
	tft.setTextColor(TFT_WHITE, TFT_BLACK);
	write_value(values.co2ppm, LEFT_VAL_X + HEAD_WIDTH, ROW4_Y, VALUE_WIDTH_LONG, DEFAULT_FONT, alerts.co2);

	// Alert color
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
}

#endif