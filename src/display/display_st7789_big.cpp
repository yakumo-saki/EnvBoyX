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

const int R_HEIGHT = 56;
const int Y_PADDING = 2;
const long R1_X = 117;
const long R1_Y = 0;
const long R2_X = 114;
const long R2_Y = R1_Y + R_HEIGHT;
const long R3_X = 127;
const long R3_Y = R2_Y + R_HEIGHT + Y_PADDING + 4;
const long R4_X = 127;
const long R4_Y = R3_Y + R_HEIGHT + Y_PADDING + 6;


void _clear_screen_big() {
	tft.fillScreen(TFT_BLACK);

	// 縦画面
	if (config.displayFlip == DISPLAY_FLIP_OFF) {
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
	if (config.displayFlip == DISPLAY_FLIP_OFF) {
		tft.setRotation(2);
	} else {
		tft.setRotation(0);
	}
}


/**
 * BIGモード（縦）
 * @param val 値
 * @param erase true => 黒文字で書く（要するに消す） false => 普通に書く
 */
void _disp_sensor_value_big(disp_values_t val)
{

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

	if (config.mhz19b == MHZ_NOUSE) {
		tft.drawString("l", R4_X + 1, R4_Y + 20, SMALL_FONT);
		tft.drawString("x", R4_X + 1, R4_Y + 36, SMALL_FONT);
		tft.setTextDatum(TR_DATUM);
		tft.setTextPadding(R2_X);
		tft.drawString(String(v.lux,0), R4_X, R4_Y, DIGIT_FONT);
	} else {
		tft.drawString("p", R4_X + 1, R4_Y + 2, SMALL_FONT);
		tft.drawString("p", R4_X + 1, R4_Y + 20, SMALL_FONT);
		tft.drawString("m", R4_X + 1, R4_Y + 36, SMALL_FONT);
		tft.setTextDatum(TR_DATUM);
		tft.setTextPadding(R2_X);
		tft.drawString(String(v.co2ppm), R4_X, R4_Y, DIGIT_FONT);
	}
}

#endif