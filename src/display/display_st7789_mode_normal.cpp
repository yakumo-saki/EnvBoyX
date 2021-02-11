#ifdef ESP32

#include <Arduino.h>

#include "global.h"
#include "structs.h"
#include "log.h"
#include "display/display_util.h"

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

void _draw_pres_delta_up(int x, int y, int width, int height, uint16_t color) {
	int half = width / 2;
	int halfX = x + half;
	int rightX = x + width;
	int halfY = y + (height / 2);
	int bottomY = y + height;

	tft.drawLine(halfX, y, halfX, bottomY, color);
	tft.drawLine(x, halfY, halfX, y, color);
	tft.drawLine(halfX, y, rightX, halfY, color);

}

void _draw_pres_delta_down(int x, int y, int width, int height, uint16_t color) {
	int half = width / 2;
	int halfX = x + half;
	int rightX = x + width;
	int halfY = y + (height / 2);
	int bottomY = y + height;

	tft.drawLine(x, y, rightX, bottomY, color);
	tft.drawLine(x - 1, y, rightX, bottomY, color);
	tft.drawLine(x + 1, y, rightX, bottomY, color);
	tft.drawTriangle(rightX, halfY, rightX, bottomY, halfX, bottomY, color);

	halfX--;
	rightX--;
	halfY--;
	bottomY--;
	tft.drawTriangle(rightX, halfY, rightX, bottomY, halfX, bottomY, color);
}

enum class DELTA_ICON {
	UP = 1, DOWN = 2, NONE = 3
};

void _draw_pres_delta_icon(int x, int y, DELTA_ICON type, uint16_t color) {
	int HEIGHT = 22;
	int WIDTH = 12;

	tft.fillRect(x - 1, y, WIDTH + 3, HEIGHT + 1, TFT_BLACK);

	if (type == DELTA_ICON::UP) {
		_draw_pres_delta_up(x, y, WIDTH, HEIGHT, color);
		_draw_pres_delta_up(x + 1, y, WIDTH, HEIGHT, color);
	} else if (type == DELTA_ICON::DOWN) {
		_draw_pres_delta_down(x, y, WIDTH, HEIGHT, color);
	}
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
	String pressureDelta = format_air_pressure_delta(sensorValues.pressureDelta);


	// 気圧の変化
	int presDeltaStrX = presDeltaX + 20;
	tft.setTextPadding(60);
	if (sensorValues.pressureDelta > 0.2) {
		// UP
		tft.setTextColor(TFT_GREENYELLOW, TFT_BLACK);
		_draw_pres_delta_icon(presDeltaX, ROW3_Y, DELTA_ICON::UP, TFT_GREENYELLOW);
		tft.drawString(pressureDelta, presDeltaStrX, ROW3_Y, DEFAULT_FONT);
	} else if (sensorValues.pressureDelta < -0.2) {
		// DOWN
		tft.setTextColor(TFT_ORANGE, TFT_BLACK);
		_draw_pres_delta_icon(presDeltaX, ROW3_Y, DELTA_ICON::DOWN, TFT_ORANGE);
		tft.drawString(pressureDelta, presDeltaStrX, ROW3_Y, DEFAULT_FONT);
	} else {
		// 変わらない
		_draw_pres_delta_icon(presDeltaX, ROW3_Y, DELTA_ICON::NONE, TFT_ORANGE);

		if (sensorValues.pressureDelta >= 0.10) {
			tft.setTextColor(TFT_GREENYELLOW, TFT_BLACK);
		} else if (sensorValues.pressureDelta <= -0.10) {
			tft.setTextColor(TFT_ORANGE, TFT_BLACK);
		} else {
			tft.setTextColor(TFT_WHITE, TFT_BLACK);
		}

		tft.drawString(pressureDelta, presDeltaStrX, ROW3_Y, DEFAULT_FONT);
	}

	tft.setTextColor(TFT_WHITE, TFT_BLACK);

	// Row 4
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