#include <Arduino.h>

#include "global.h"
#include "structs.h"
#include "log.h"

#include <WiFiClient.h> 

#include <TFT_eSPI.h>
#include <SPI.h>

extern int disp_switch;

TFT_eSPI tft = TFT_eSPI();  // Invoke library

// temp あとで消す
int count = 1;

// フォント番号は0,2,4,6 は同一フォントのサイズ違い。
// フォント番号 7 は8セグフォント
const int XSMALL_FONT = 1;
const int SMALL_FONT = 2;
const int DEFAULT_FONT = 4;

// x: という２文字の場合の幅
const int HEAD_WIDTH = 32;

const int LEFT_HEAD_X = 0;
const int RIGHT_HEAD_X = 124;

const int LEFT_VAL_X = LEFT_HEAD_X + HEAD_WIDTH;
const int RIGHT_VAL_X = RIGHT_HEAD_X + HEAD_WIDTH;

const int VALUE_WIDTH = RIGHT_HEAD_X - LEFT_HEAD_X - HEAD_WIDTH;

const int ROW_HEIGHT = 27;
const int ROW1_Y = 26;
const int ROW2_Y = ROW1_Y + ROW_HEIGHT;
const int ROW3_Y = ROW2_Y + ROW_HEIGHT;
const int ROW4_Y = ROW3_Y + ROW_HEIGHT;


/**
 * 起動時の画面表示（共通）
 */
void disp_st7789_normal_startup_screen(String product_long) {

	tft.startWrite();
	tft.fillScreen(TFT_BLACK);
	tft.setCursor(0, 0, DEFAULT_FONT);
	tft.setTextColor(TFT_WHITE,TFT_BLACK);

	tft.println("startup");

	tft.endWrite();
  // display.init();

  // if (needFlip) {
  //   display.flipScreenVertically();
  // }
  // display.setFont(ArialMT_Plain_16);
  // display.clear();
  // display.setTextAlignment(TEXT_ALIGN_LEFT);
  // display.drawString(0, 0,  "ziomatrix corp.");
  // display.drawString(0, 16, product_long);
  // display.drawString(0, 32, "initialize or flash");
  // display.drawString(0, 48, "Please wait");
  // display.display();

  // delay(1000);
}

/**
 * セットアップモード時のディスプレイ表示
 */
void disp_st7789_setup_startup_screen(String ipAddr) {

	return; // cant support this.

  // if (!has_ssd1306()) return;

  // // display init and show initial screen
  // display.init();
  // if (needFlip) {
  //   display.flipScreenVertically();
  // }

  // display.setFont(ArialMT_Plain_16);
  // display.clear();
  // display.setTextAlignment(TEXT_ALIGN_LEFT);
  // display.drawString(0, 0,  product_long);
  // display.drawString(0, 16, "Setup mode.");
  // display.drawString(0, 33, "http://" + ipAddr + "/" );
  // display.setFont(ArialMT_Plain_10);
  // display.drawString(0, 52, ssid);
  // display.display();
}

/**
 * WiFi接続中表示
 * @param wait_print_row Please wait を何行目に表示するか(0,1,2)
 */
void disp_st7789_wifi_starting(int wait_print_row) {

	tft.startWrite();
	tft.fillScreen(TFT_BLACK);
	tft.setCursor(0, 0, DEFAULT_FONT);
	tft.setTextColor(TFT_WHITE,TFT_BLACK);

	tft.println("WiFi starting");

	tft.endWrite();

  // int row = (wait_print_row % 3) + 1;

  // display.init();
 
  // if (needFlip) {
  //   display.flipScreenVertically();
  // }
  // display.clear();
  // display.setTextAlignment(TEXT_ALIGN_LEFT);
  // display.setFont(ArialMT_Plain_10);
  // display.setFont(ArialMT_Plain_16);
  // display.drawString(0, 0, "WiFi Connecting");
  // display.drawString(0, 16*row, "Please wait...");

  // display.display();
}

void disp_st7789_wifi_info(String ip, String mDNS) {

	tft.startWrite();
	tft.fillScreen(TFT_BLACK);
	tft.setCursor(0, 0, DEFAULT_FONT);
	tft.setTextColor(TFT_WHITE,TFT_BLACK);

	tft.println("WiFi info:");
	tft.println(ip);
	tft.println(mDNS);

	tft.endWrite();

//   display.init();

// //  String ssidStr = "SSID ";
// //  ssidStr.concat(ssid);
  
//   if (needFlip) {
//     display.flipScreenVertically();
//   }
//   display.clear();
//   display.setTextAlignment(TEXT_ALIGN_LEFT);
//   display.setFont(ArialMT_Plain_10);
//   display.setFont(ArialMT_Plain_16);
//   display.drawString(0, 0, ssid);
//   display.drawString(0, 16, ip);
//   display.drawString(0, 32, mDNS);
//   display.drawString(0, 48, "Starting up...");
//   display.display();

//   delay(300);
   
}

void disp_st7789_wifi_error() {

	tft.startWrite();
	tft.fillScreen(TFT_RED);
	tft.setCursor(0, 0, DEFAULT_FONT);
	tft.setTextColor(TFT_YELLOW,TFT_BLACK);

	tft.println("WiFi ERROR");

	tft.endWrite();

  // // display init and show initial screen
  // display.init();
  
  // if (needFlip) {
  //   display.flipScreenVertically();
  // }
  // display.setFont(ArialMT_Plain_16);
  // display.clear();
  // display.setTextAlignment(TEXT_ALIGN_LEFT);
  // display.drawString(0, 0, product + " *HALT*");
  // display.drawString(0, 16, "* Please Restart *");
  // display.drawString(0, 32, "WiFi connect err");
  // display.drawString(0, 48, "Check Settings");
  // display.display();

}

/**
 * wait for reconfigure 画面を出しながら待つ
 */
void disp_st7789_wait_for_reconfig() {

	tft.startWrite();

	tft.fillScreen(TFT_BLACK);
	tft.setCursor(0, 0, DEFAULT_FONT);
	tft.setTextColor(TFT_WHITE,TFT_BLACK);

	tft.println("Wait for reconfig");

	tft.endWrite();

  // display.init();
 
  // if (needFlip) {
  //   display.flipScreenVertically();
  // }
  // display.clear();
  // display.setTextAlignment(TEXT_ALIGN_LEFT);
  // display.setFont(ArialMT_Plain_16);
  // display.drawString(0, 0, "Wait for re-config");
  // display.drawString(0, 33, "Power off now");
  // display.drawString(0, 48, " to re-configure");

  // // バーを生成する
  // // 下の幅の部分を先に書かせてしまう。
  // int MAX_UNDER_BAR = 20;  // _ の数。プロポーショナルフォントなので幅注意
  // String bar = "";
  // for (int n = 0; n < MAX_UNDER_BAR ; n++) {
  //   bar = bar + "_";
  //   display.drawString(0, 16, bar);
  //   display.drawString(0, 0, bar);
  //   display.display();
  // }

  // int MAX_BAR = 31;  // _ の数。プロポーショナルフォントなので幅注意  
  // int now = 1;
  // for (int i = 0; i < MAX_BAR; i++) {
  //   bar = "|";
    
  //   for (int n = 0; n < now ; n++) {
  //     bar = bar + "|";
  //   }

  //   display.drawString(0, 16, bar);
  //   display.display();
  //   delay(WAIT_PER_BAR);
  //   now = now + 1;
  // }

}

/**
 * ヘッダ部分のみを表示する（この部分は変更されない）
 */
void _disp_header(String ip, String mDNS) {
  tft.setTextColor(TFT_WHITE);

	// Logo
	tft.setTextDatum(TL_DATUM);

  // EnvBoy
  tft.setTextColor(TFT_DARKGREY);
	tft.drawString(product.substring(0, product.length() - 1), 1, 6, SMALL_FONT);
  tft.setTextColor(TFT_WHITE);
	tft.drawString(product.substring(0, product.length() - 1), 0, 5, SMALL_FONT);

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

void disp_st7789_power_off() {
  // display.displayOff();
}

/**
 * @param val 値
 * @param erase true => 黒文字で書く（要するに消す） false => 普通に書く
 * eraseは一応機能するが
 */
void _disp_sensor_value(disp_values_t val) {

	tft.setTextColor(TFT_WHITE, TFT_BLACK);
	tft.setTextSize(1);

	// Row 1
  tft.setTextPadding(VALUE_WIDTH);
	tft.drawString(val.temperature, LEFT_VAL_X, ROW1_Y, DEFAULT_FONT);

	tft.drawString(val.lux, RIGHT_VAL_X, ROW1_Y, DEFAULT_FONT);

	// Row 2
	tft.drawString(val.humidity, LEFT_VAL_X, ROW2_Y, DEFAULT_FONT);

	// Row 3
	tft.drawString(val.pressure, LEFT_VAL_X, ROW3_Y, DEFAULT_FONT);

	// Row 4
	tft.drawString(val.co2ppm, LEFT_VAL_X + HEAD_WIDTH, ROW4_Y, DEFAULT_FONT);  
}

/**
 * メイン画面に移る前の初期化
 */
void disp_st7789_all_initialize_complete(String ip, String mdns) {
	// clear entire screen. because of sensor value screen uses partial update
	// to prevent screen flickering.
	tft.startWrite();
	tft.fillScreen(TFT_BLACK);
  _disp_header(ip, mdns);
	tft.endWrite();
}

/**
 * 通常画面
 */
void disp_st7789_sensor_value(disp_values_t new_values, disp_values_t last_values) {

	// setTextDatum( XY_DATUM: X=Top,Middle,Bottom / Y=Left,Right,Center)

	// tft.startWrite();

  _disp_sensor_value(new_values);

	// tft.endWrite();

}

String disp_st7789_set_brightness(int brightness) {
  const String PWR_OFF = "Display Power-Off";
  const String MAX = "Set Display brightness MAX(255)";
  const String VALUE = "Set Display brightness (0-255) = ";

  // if (brightness == 0) {
  //   displog(PWR_OFF);
  //   display.displayOff();
  //   return PWR_OFF;
  // } else if (brightness == 255) {
  //   display.displayOn();
  //   display.setContrast(255, 255, 255);
  //   return MAX;
  // } else {
  //   String msg = VALUE + String(brightness);
  //   displog(msg);
  //   display.displayOn();
  //   display.setBrightness(brightness);
  //   return msg;
  // }
  return "not impl";
}

void disp_st7789_set_power(bool poweron) {
  // if (!poweron) {
  //   display.displayOff();
  // } else { 
  //   display.displayOn();
  // }
}

void setup_disp_st7789() {
	tft.init();
	tft.setRotation(3);
	tft.fillScreen(TFT_BLACK);
	displog("ST7789: Nothing to do.");
}