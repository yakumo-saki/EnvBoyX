#include <Arduino.h>

#include <Wire.h>
#include <U8g2lib.h>

#include "global.h"
#include "log.h"

#include "display/display_util.h"
#include "display/u8g2_utils.h"

// The complete list is available here: https://github.com/olikraus/u8g2/wiki/u8g2setupcpp
U8G2_SH1106_128X64_NONAME_F_HW_I2C u8g2_sh1106(U8G2_R0, U8X8_PIN_NONE, I2C_SCL, I2C_SDA);
U8G2_SSD1306_128X64_NONAME_F_HW_I2C u8g2_ssd1306(U8G2_R0, U8X8_PIN_NONE, I2C_SCL, I2C_SDA); // artifact

U8G2 u8g2;

extern int disp_switch;

#if false
const String FONT_ARROW_UP = "@";
const String FONT_ARROW_DOWN = "C";
const uint8_t *FONT_ARROW = u8g2_font_open_iconic_arrow_1x_t;
#endif

const String FONT_ARROW_UP = "[";
const String FONT_ARROW_DOWN = "]";
const uint8_t *FONT_ARROW = u8g2_font_twelvedings_t_all;

const uint8_t *FONT_SMALL_VERSION = u8g2_font_chikita_tn;
const uint8_t *FONT_SMALL_NARROW = u8g2_font_t0_11_tr;
const uint8_t *FONT_SMALL = u8g2_font_roentgen_nbp_tr;
const uint8_t *FONT_BOOT = u8g2_font_ncenR10_tr;
const uint8_t *FONT_PLAIN_10 = u8g2_font_9x15_tr;
const uint8_t *FONT_SSID = u8g2_font_8x13_tr;

const uint8_t WHITE = 1;
const uint8_t BLACK = 0;

int ssd1306_connected = -1;


bool has_ssd1306_i2c() {

  Wire.beginTransmission(SSD1306_I2C_ADDR);
  byte error = Wire.endTransmission();

  if (error != 0) {
    ssdlog("Error oled connection Addr: " + String(SSD1306_I2C_ADDR) + " ErrorNo: " + String(error));
    return false;
  }
  return true;
   
}

bool has_ssd1306() {
  if (ssd1306_connected == 1) return true;
  if (ssd1306_connected == 0) return false;
  if (ssd1306_connected == -1) {
    if (has_ssd1306_i2c()) {
      ssd1306_connected = 1;
      return true;
    } else {
      ssd1306_connected = 0;
      return false;
    }
  }
  return false;
}

void init_u8g2() {
  u8g2.clearBuffer();
  u8g2.setFontPosTop(); // 文字描画の座標指定を左上にする（default=baseline)

  if (config.displayFlip == DISPLAY_FLIP_ON) {
    u8g2.setDisplayRotation(U8G2_R2);
  } else {
    u8g2.setDisplayRotation(U8G2_R0);
  }
}

/**
 * 起動時の画面表示（共通）
 */
void disp_ssd1306_normal_startup_screen(String product_long) {

  if (!has_ssd1306()) return;

  init_u8g2();
  
  u8g2.setFont(FONT_BOOT);
  // u8g2.setTextAlignment(TextAlign::LEFT);
  u8g2.drawStr(0, 16,  "ziomatrix corp.");
  u8g2.drawStr(0, 32, product_long.c_str());
  u8g2.drawStr(0, 48, "initialize or flash");
  u8g2.drawStr(0, 64, "Please wait");

  u8g2.sendBuffer();
}

/**
 * セットアップモード時のディスプレイ表示
 */
void disp_ssd1306_setup_startup_screen(String ipAddr, int disp_switch) {

  if (!has_ssd1306()) return;

  init_u8g2();

  // 3秒ごとに反転
  set_invert(int(disp_switch / 3) == 0 );

  u8g2.setFont(FONT_BOOT);
  // u8g2.setTextAlignment(TextAlign::LEFT);
  u8g2.drawStr(0, 0,  product_long.c_str());
  u8g2.drawStr(0, 16, "Setup mode.");
  u8g2.drawStr(0, 33, ("http://" + ipAddr + "/").c_str() );
  u8g2.setFont(FONT_SSID);
  u8g2.drawStr(0, 52, config.ssid.c_str());
  u8g2.sendBuffer();
}

/** 各種メッセージ表示 */
void disp_ssd1306_message(bool isError, String msg1, String msg2, String msg3, String msg4) {

  if (!has_ssd1306()) return;

  init_u8g2();

  u8g2.setFont(FONT_BOOT);
  u8g2.clear();
  u8g2.drawStr(0, 0,  msg1.c_str());
  u8g2.drawStr(0, 16, msg2.c_str());
  u8g2.drawStr(0, 32, msg3.c_str());
  u8g2.drawStr(0, 48, msg4.c_str());
  u8g2.sendBuffer();
}

/**
 * WiFi接続中表示
 * @param wait_print_row Please wait を何行目に表示するか(0,1,2)
 */
void disp_ssd1306_wifi_starting() {

  disp_ssd1306_message(false, "WiFi", "Connecting", "Please", "  wait...");
}

void disp_ssd1306_wifi_info(String ip, String mDNS) {

  disp_ssd1306_message(false, config.ssid, ip, mDNS, "Starting up...");
   
}

void disp_ssd1306_wifi_error() {

  if (!has_ssd1306()) return;

  disp_ssd1306_message(true, " *HALT* ", "* Please Restart *", "WiFi connect err", "Check Settings");
}

/**
 * wait for reconfigure 画面を出しながら待つ
 */
void disp_ssd1306_wait_for_reconfig_init() {

  if (!has_ssd1306()) return;

  init_u8g2();

  u8g2.setFont(FONT_BOOT);
  u8g2.drawStr(0, 0, "Wait for re-config");
  u8g2.drawStr(0, 33, "Power off now");
  u8g2.drawStr(0, 48, " to re-configure");

  // バーを生成する
  u8g2.setDrawColor(WHITE);
  u8g2.drawHLine(0, 16, 1);
  u8g2.drawHLine(0, 32, 1);
  u8g2.sendBuffer();
}

void disp_ssd1306_wait_for_reconfig_bar(int now, const int max) {
  float lengthF = 127.0 / max * now;
  int length = (int)lengthF;

  if (now >= max) {
    length = 127;
  }

  u8g2.setDrawColor(WHITE);
  u8g2.drawBox(0, 16, length, 16);
  u8g2.sendBuffer();
}

void disp_ssd1306_all_initialize_complete() {
  // do nothing. ssd1306 code is everytime rewrite entire screen.
}

TextDecoration get_decoration_from_alert(value_alert_t alert) {
  if (alert.warning) {
    return TextDecoration::INVERT;
  } else if (alert.caution) {
    return TextDecoration::BOX;
  } else {
    return TextDecoration::NONE;
  }
}

void write_value(int x, int y, String valueString, value_alert_t alert, TextAlign align) {
  String empty = String("");
  draw_value(x, y, get_decoration_from_alert(alert), align, valueString, FONT_PLAIN_10, empty, FONT_PLAIN_10);
}

void _draw_pressure_delta(int y) {
  // pressure delta
	pressure_delta_t delta = get_pressure_delta_struct(sensorValues.pressureDelta);

  const int ICON_X = 80;
	if (delta.drawIcon && delta.positive) {
    draw_string(ICON_X, y, FONT_ARROW_UP, TextAlign::LEFT, FONT_ARROW);
	} else if (delta.drawIcon && delta.negative) {
    draw_string(ICON_X, y, FONT_ARROW_DOWN, TextAlign::LEFT, FONT_ARROW);
	} else {
    // ほぼ変わらないのでアイコンなし (-0.09 ~ 0.09)
	}
  draw_string(127, y, delta.formattedValue, TextAlign::RIGHT, FONT_PLAIN_10);
}

/**
 * 通常画面
 */
void disp_ssd1306_sensor_value(disp_values_t values, value_alerts_t alerts) {

  // 値を書くy座標。
  const int R0 = 0;
  const int R1 = 12;
  const int R2 = 30;
  const int R3 = 48;

  if (!has_ssd1306()) return;

  init_u8g2();

  // 測定値表示部分
  u8g2.setFont(FONT_PLAIN_10);

  write_value(0, R1, values.temperature, alerts.temperature, TextAlign::LEFT);

  write_value(127, R1, values.humidity, alerts.humidity, TextAlign::RIGHT);

  // ２行目：ひだり：気圧
  int presWidth = 0;
  u8g2.setFont(FONT_PLAIN_10);
  presWidth += u8g2.getStrWidth(values.pressure.c_str());
  u8g2.setFont(FONT_SMALL_NARROW);
  presWidth += 4 + u8g2.getStrWidth("hPa");

  // write_value(0, R2, values.pressure, alerts.pressure, TextAlign::LEFT);
  // draw_value(0, R2, get_decoration_from_alert(alerts.pressure), TextAlign::LEFT, values.pressure, FONT_PLAIN_10, "hPa", FONT_PLAIN_10);
  draw_value(0, R2, TextDecoration::INVERT, TextAlign::LEFT, values.pressure, FONT_PLAIN_10, "hPa", FONT_SMALL_NARROW);

  // ２行目：みぎ：気圧の差
  _draw_pressure_delta(R2);

  // ３行目：CO2センサーがないならその場所に照度を表示する
  if (sensorCharacters.co2ppm) {
    write_value(127, R3, values.lux, alerts.lux, TextAlign::RIGHT);
    draw_value(0, R3, get_decoration_from_alert(alerts.co2), TextAlign::LEFT, values.co2ppm, FONT_PLAIN_10, "ppm", FONT_SMALL_NARROW); // 9999ppm
  } else {
    write_value(0, R3, values.lux, alerts.lux, TextAlign::LEFT);
  }

  // みぎ上、IPアドレス or mDNS名表示
  if (disp_switch < 3) {
    draw_string(127, R0, values.ip, TextAlign::RIGHT, FONT_SMALL_NARROW); 
  } else {
    draw_string(127, R0, values.mDNS, TextAlign::RIGHT, FONT_SMALL_NARROW); 
  }

  // 左上、EnvBoyX の表示
  draw_string(0 , R0 + 1, product_short, TextAlign::LEFT, FONT_SMALL); 
    // バージョン表示
  draw_string(40, R0, ver, TextAlign::RIGHT, FONT_SMALL_VERSION);

  
  disp_switch++;
  if (disp_switch > 5) {
    disp_switch = 0;
  }
  
  u8g2.sendBuffer();

}

void disp_ssd1306_power(bool on) {
  if (!has_ssd1306()) return;
  
  u8g2.setPowerSave((on ? 0 : 1));
}

/** バックライトオフ */
void disp_ssd1306_power_off() {
  disp_ssd1306_power(false);
}

/** バックライトオフ */
void disp_ssd1306_power_on() {
  disp_ssd1306_power(true);
}

String disp_ssd1306_set_brightness(int brightness) {
  const String PWR_OFF = "u8g2 Power-Off";
  const String MAX = "Set u8g2 brightness MAX(255)";
  const String VALUE = "Set u8g2 brightness (0-255) = ";

  if (brightness == 0) {
    ssdlog(PWR_OFF);
    disp_ssd1306_power_off();
    return PWR_OFF;
  } else if (brightness == 255) {
    disp_ssd1306_power_on();
    u8g2.setContrast(255);
    return MAX;
  } 

  String msg = VALUE + String(brightness);
  disp_ssd1306_power_on();
  u8g2.setContrast(brightness);
  return msg;
}

void disp_ssd1306_set_power(bool poweron) {
  disp_ssd1306_power(poweron);
}

void setup_disp_ssd1306() {

  ssdlog("initialize start.");
  if (has_ssd1306()) {
    if (config.oledType == OLED_SSD1306) {
      ssdlog("Using SSD1306");
      u8g2 = u8g2_ssd1306;
    } else {
      ssdlog("Using SH1106");
      u8g2 = u8g2_sh1106;
    }
    bool ret = u8g2.begin();
    init_u8g2();

    if (ret) {
      ssdlog(F("Initialized."));
      ssd1306_connected = true;
    } else {
      ssdlog(F("Initialization failed."));
      ssd1306_connected = false;
    }
  } else {
    ssdlog(F("SSD1306 NOT FOUND."));
    ssd1306_connected = false;
  }
}