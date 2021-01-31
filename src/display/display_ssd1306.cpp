#include <Arduino.h>

#include <Wire.h>
#include <U8g2lib.h>

#include "global.h"
#include "log.h"


// The complete list is available here: https://github.com/olikraus/u8g2/wiki/u8g2setupcpp
U8G2_SH1106_128X64_NONAME_F_HW_I2C u8g2_sh1106(U8G2_R0);
U8G2_SSD1306_128X64_NONAME_F_HW_I2C u8g2_ssd1306(U8G2_R0, U8X8_PIN_NONE, I2C_SCL, I2C_SDA); // artifact

U8G2 u8g2;

extern int disp_switch;

#define FONT_BOOT u8g2_font_crox5t_tr
// const uint8_t FONT_BOOT = *u8g2_font_ncenB10_tr;
#define FONT_PLAIN_10 u8g2_font_9x15_mr
// const uint8_t FONT_PLAIN_10 = *u8g2_font_ncenB10_tr;

const uint8_t TEXT_ALIGN_LEFT = 1;
const uint8_t TEXT_ALIGN_RIGHT = 2;

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
  
  u8g2.setFont(u8g2_font_ncenB10_tr);
  // u8g2.setTextAlignment(TEXT_ALIGN_LEFT);
  u8g2.drawStr(0, 16,  "ziomatrix corp.");
  u8g2.drawStr(0, 32, product_long.c_str());
  u8g2.drawStr(0, 48, "initialize or flash");
  u8g2.drawStr(0, 64, "Please wait");

  u8g2.sendBuffer();
}

/**
 * セットアップモード時のディスプレイ表示
 */
void disp_ssd1306_setup_startup_screen(String ipAddr) {

  if (!has_ssd1306()) return;

  init_u8g2();

  u8g2.setFont(FONT_BOOT);
  // u8g2.setTextAlignment(TEXT_ALIGN_LEFT);
  u8g2.drawStr(0, 0,  product_long.c_str());
  u8g2.drawStr(0, 16, "Setup mode.");
  u8g2.drawStr(0, 33, ("http://" + ipAddr + "/").c_str() );
  u8g2.setFont(FONT_PLAIN_10);
  u8g2.drawStr(0, 52, config.ssid.c_str());
  u8g2.sendBuffer();
}

/**
 * WiFi接続中表示
 * @param wait_print_row Please wait を何行目に表示するか(0,1,2)
 */
void disp_ssd1306_wifi_starting(int wait_print_row) {

  if (!has_ssd1306()) return;

  int row = (wait_print_row % 3) + 1;

  init_u8g2();
 
  // u8g2.setTextAlignment(TEXT_ALIGN_LEFT);
  u8g2.setFont(FONT_BOOT);
  u8g2.drawStr(0, 0, "WiFi Connecting");
  u8g2.drawStr(0, 16*row, "Please wait...");

  u8g2.sendBuffer();
}

void disp_ssd1306_wifi_info(String ip, String mDNS) {

  if (!has_ssd1306()) return;

  init_u8g2();

  // u8g2.setTextAlignment(TEXT_ALIGN_LEFT);
  u8g2.setFont(FONT_BOOT);
  u8g2.drawStr(0, 0, config.ssid.c_str());
  u8g2.drawStr(0, 16, ip.c_str());
  u8g2.drawStr(0, 32, mDNS.c_str());
  u8g2.drawStr(0, 48, "Starting up...");
  u8g2.sendBuffer();

  delay(300);
   
}

void disp_ssd1306_wifi_error() {

  if (!has_ssd1306()) return;

  init_u8g2();

  u8g2.setFont(FONT_BOOT);
  u8g2.clear();
  u8g2.drawStr(0, 0,  " *HALT* ");
  u8g2.drawStr(0, 16, "* Please Restart *");
  u8g2.drawStr(0, 32, "WiFi connect err");
  u8g2.drawStr(0, 48, "Check Settings");
  u8g2.sendBuffer();

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
  // 下の幅の部分を先に書かせてしまう。
  int MAX_UNDER_BAR = 20;  // _ の数。プロポーショナルフォントなので幅注意
  String bar = "";
  for (int n = 0; n < MAX_UNDER_BAR ; n++) {
    bar = bar + "_";
    u8g2.drawStr(0, 16, bar.c_str());
    u8g2.drawStr(0, 0, bar.c_str());
    u8g2.sendBuffer();
  }
}

void disp_ssd1306_wait_for_reconfig_bar(int now, const int max) {
  String bar = "|";
  
  for (int n = 0; n < now ; n++) {
    bar = bar + "|";
  }

  u8g2.drawStr(0, 16, bar.c_str());
  u8g2.sendBuffer();
}

void disp_ssd1306_all_initialize_complete() {
  // do nothing. ssd1306 code is everytime rewrite entire screen.
}

void write_value(int x, int y, String valueString, value_alert_t alert, uint8_t align) {
  
  const int HEIGHT = 17;
  
  const char value = *valueString.c_str();

  // u8g2.setTextAlignment(align);
  // int diffX = (align == TEXT_ALIGN_RIGHT ? -1 : 1);
  int diffX = 1;

  // // 注意表示：太字
  // 微妙なのでコメントアウト
  // u8g2.drawStr(x, y, value);
  // u8g2.drawStr(x + diffX, y, value);

  if (alert.warning || alert.caution) {

    // 警告表示：反転
    const int MARGIN = 2;
    int RECT_MARGIN = 1 + MARGIN;  // 枠 1px + マージン 2px
    
    // 右揃えの場合はマージンが逆
    int STR_MARGIN = (align == TEXT_ALIGN_RIGHT ? -MARGIN : MARGIN);  

    uint16_t width = u8g2.getStrWidth(&value) + (RECT_MARGIN * 2); // *2 = 左右
    int startX = (align == TEXT_ALIGN_RIGHT ? x - width: x);  // 右揃えの場合、左端のXを求める

    // 枠 or 塗りつぶし
    u8g2.setDrawColor(WHITE);
    if (alert.warning) {
      u8g2.drawBox(startX, y, width, HEIGHT);
      u8g2.setDrawColor(BLACK);
      u8g2.drawStr(x + STR_MARGIN, y, &value);
      u8g2.drawStr(x + STR_MARGIN + diffX, y, &value);
    } else if (alert.caution) {
      u8g2.drawFrame(startX, y, width, HEIGHT);
      u8g2.drawStr(x + STR_MARGIN, y, &value);
    }   
  } else {
    // 通常表示
    u8g2.setDrawColor(WHITE);
    u8g2.drawStr(x, y, &value);
  }

  u8g2.setDrawColor(WHITE);  // 

}

/**
 * 通常画面
 */
void disp_ssd1306_sensor_value(disp_values_t values, value_alerts_t alerts) {

  const int R1 = 12 + 16;
  const int R2 = 30 + 16;
  const int R3 = 48 + 16;

  if (!has_ssd1306()) return;

  init_u8g2();

  // 測定値表示部分
  u8g2.setFont(FONT_PLAIN_10);

  write_value(0, R1, values.temperature, alerts.temperature, TEXT_ALIGN_LEFT);

  write_value(127, R1, values.humidity, alerts.humidity, TEXT_ALIGN_RIGHT);

  write_value(0, R2, values.pressure, alerts.pressure, TEXT_ALIGN_LEFT);

  write_value(127, R2, values.lux, alerts.lux, TEXT_ALIGN_RIGHT);

  // u8g2.setTextAlignment(TEXT_ALIGN_LEFT);
  u8g2.drawStr(0, R3, "CO2:");
  write_value(38, R3, values.co2ppm, alerts.co2, TEXT_ALIGN_LEFT); // 9999ppm

  // バージョン表示
  // u8g2.setTextAlignment(TEXT_ALIGN_RIGHT);
  u8g2.setFont(FONT_PLAIN_10);
  u8g2.drawStr(116, 64, ver.c_str());

  // 左上、EnvBoyX の表示u8g2_font_ncenB10_tr
  // u8g2.setTextAlignment(TEXT_ALIGN_LEFT);
  u8g2.setFont(FONT_PLAIN_10);
  u8g2.drawStr(0, 10, product.c_str()); 
  
  // みぎ上、IPアドレス or mDNS名表示
  // u8g2.setTextAlignment(TEXT_ALIGN_RIGHT);
  u8g2.setFont(FONT_PLAIN_10);
  if (disp_switch < 3) {
    u8g2.drawStr(127, 10, values.ip.c_str()); 
  } else {
    u8g2.drawStr(127, 10, values.mDNS.c_str()); 
  }

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
    u8g2 = u8g2_ssd1306;
    bool ret = u8g2.begin();

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