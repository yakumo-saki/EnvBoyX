#include <Arduino.h>

#include "global.h"
#include "log.h"

#include <SSD1306.h>
#include <SH1106.h>
#include <WiFiClient.h> 

extern int disp_switch;

//SSD1306 display(0x3c, 5, 4);
SSD1306 display(SSD1306_I2C_ADDR, I2C_SDA, I2C_SCL);
// SH1106 display(SSD1306_I2C_ADDR, I2C_SDA, I2C_SCL);


int ssd1306_connected = -1;

bool has_ssd1306_i2c() {
  
  Wire.beginTransmission(SSD1306_I2C_ADDR);
  byte error = Wire.endTransmission();

  if (error != 0) {
    ssdlog("Error display connection Addr: " + String(SSD1306_I2C_ADDR) + " ErrorNo: " + String(error));
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

void init_display() {
  display.clear();

  if (config.displayFlip == DISPLAY_FLIP_ON) {
    display.flipScreenVertically();
  }
}

/**
 * 起動時の画面表示（共通）
 */
void disp_ssd1306_normal_startup_screen(String product_long) {

  if (!has_ssd1306()) return;

  init_display();
  
  display.setFont(ArialMT_Plain_16);
  display.setTextAlignment(TEXT_ALIGN_LEFT);
  display.drawString(0, 0,  "ziomatrix corp.");
  display.drawString(0, 16, product_long);
  display.drawString(0, 32, "initialize or flash");
  display.drawString(0, 48, "Please wait");
  display.display();

  delay(1000);  
}

/**
 * セットアップモード時のディスプレイ表示
 */
void disp_ssd1306_setup_startup_screen(String ipAddr) {

  if (!has_ssd1306()) return;

  init_display();

  display.setFont(ArialMT_Plain_16);
  display.setTextAlignment(TEXT_ALIGN_LEFT);
  display.drawString(0, 0,  product_long);
  display.drawString(0, 16, "Setup mode.");
  display.drawString(0, 33, "http://" + ipAddr + "/" );
  display.setFont(ArialMT_Plain_10);
  display.drawString(0, 52, config.ssid);
  display.display();
}

/**
 * WiFi接続中表示
 * @param wait_print_row Please wait を何行目に表示するか(0,1,2)
 */
void disp_ssd1306_wifi_starting(int wait_print_row) {

  if (!has_ssd1306()) return;

  int row = (wait_print_row % 3) + 1;

  init_display();
 
  display.setTextAlignment(TEXT_ALIGN_LEFT);
  display.setFont(ArialMT_Plain_10);
  display.setFont(ArialMT_Plain_16);
  display.drawString(0, 0, "WiFi Connecting");
  display.drawString(0, 16*row, "Please wait...");

  display.display();
}

void disp_ssd1306_wifi_info(String ip, String mDNS) {

  if (!has_ssd1306()) return;

  init_display();

  display.setTextAlignment(TEXT_ALIGN_LEFT);
  display.setFont(ArialMT_Plain_10);
  display.setFont(ArialMT_Plain_16);
  display.drawString(0, 0, config.ssid);
  display.drawString(0, 16, ip);
  display.drawString(0, 32, mDNS);
  display.drawString(0, 48, "Starting up...");
  display.display();

  delay(300);
   
}

void disp_ssd1306_wifi_error() {

  if (!has_ssd1306()) return;

  init_display();

  display.setFont(ArialMT_Plain_16);
  display.clear();
  display.setTextAlignment(TEXT_ALIGN_LEFT);
  display.drawString(0, 0, product + " *HALT*");
  display.drawString(0, 16, "* Please Restart *");
  display.drawString(0, 32, "WiFi connect err");
  display.drawString(0, 48, "Check Settings");
  display.display();

}

/**
 * wait for reconfigure 画面を出しながら待つ
 */
void disp_ssd1306_wait_for_reconfig_init() {

  if (!has_ssd1306()) return;

  init_display();

  display.setTextAlignment(TEXT_ALIGN_LEFT);
  display.setFont(ArialMT_Plain_16);
  display.drawString(0, 0, "Wait for re-config");
  display.drawString(0, 33, "Power off now");
  display.drawString(0, 48, " to re-configure");

  // バーを生成する
  // 下の幅の部分を先に書かせてしまう。
  int MAX_UNDER_BAR = 20;  // _ の数。プロポーショナルフォントなので幅注意
  String bar = "";
  for (int n = 0; n < MAX_UNDER_BAR ; n++) {
    bar = bar + "_";
    display.drawString(0, 16, bar);
    display.drawString(0, 0, bar);
    display.display();
  }
}

void disp_ssd1306_wait_for_reconfig_bar(int now, const int max) {
  String bar = "|";
  
  for (int n = 0; n < now ; n++) {
    bar = bar + "|";
  }

  display.drawString(0, 16, bar);
  display.display();
}

void disp_ssd1306_all_initialize_complete() {
  // do nothing. ssd1306 code is everytime rewrite entire screen.
}

void write_value(int x, int y, String value, value_alert_t alert, OLEDDISPLAY_TEXT_ALIGNMENT align) {
  const int HEIGHT = 17;

  display.setTextAlignment(align);
  int diffX = (align == TEXT_ALIGN_RIGHT ? -1 : 1);

  // // 注意表示：太字
  // 微妙なのでコメントアウト
  // display.drawString(x, y, value);
  // display.drawString(x + diffX, y, value);

  if (alert.warning || alert.caution) {

    // 警告表示：反転
    const int MARGIN = 2;
    int RECT_MARGIN = 1 + MARGIN;  // 枠 1px + マージン 2px
    
    // 右揃えの場合はマージンが逆
    int STR_MARGIN = (align == TEXT_ALIGN_RIGHT ? -MARGIN : MARGIN);  

    uint16_t width = display.getStringWidth(value) + (RECT_MARGIN * 2); // *2 = 左右
    int startX = (align == TEXT_ALIGN_RIGHT ? x - width: x);  // 右揃えの場合、左端のXを求める

    // 枠 or 塗りつぶし
    display.setColor(WHITE);
    if (alert.warning) {
      display.fillRect(startX, y, width, HEIGHT);
      display.setColor(BLACK);
      display.drawString(x + STR_MARGIN, y, value);
      display.drawString(x + STR_MARGIN + diffX, y, value);
    } else if (alert.caution) {
      display.drawRect(startX, y, width, HEIGHT);
      display.setColor(WHITE);
      display.drawString(x + STR_MARGIN, y, value);
    }   
  } else {
    // 通常表示
    display.drawString(x, y, value);
  }

  display.setColor(WHITE);  // 

}

/**
 * 通常画面
 */
void disp_ssd1306_sensor_value(disp_values_t values, value_alerts_t alerts) {

  const int R1 = 12;
  const int R2 = 30;
  const int R3 = 48;

  if (!has_ssd1306()) return;

  init_display();

  // 測定値表示部分
  display.setFont(ArialMT_Plain_16);

  write_value(0, R1, values.temperature, alerts.temperature, TEXT_ALIGN_LEFT);

  write_value(127, R1, values.humidity, alerts.humidity, TEXT_ALIGN_RIGHT);

  write_value(0, R2, values.pressure, alerts.pressure, TEXT_ALIGN_LEFT);

  write_value(127, R2, values.lux, alerts.lux, TEXT_ALIGN_RIGHT);

  display.setTextAlignment(TEXT_ALIGN_LEFT);
  display.drawString(0, R3, "CO2:");
  write_value(38, R3, values.co2ppm, alerts.co2, TEXT_ALIGN_LEFT); // 9999ppm

  // バージョン表示
  display.setTextAlignment(TEXT_ALIGN_RIGHT);
  display.setFont(ArialMT_Plain_10);
  display.drawString(127, 54, ver);

  // 左上、EnvBoyX の表示
  display.setTextAlignment(TEXT_ALIGN_LEFT);
  display.setFont(ArialMT_Plain_10);
  display.drawString(0, 0, product); 
  
  // みぎ上、IPアドレス or mDNS名表示
  display.setTextAlignment(TEXT_ALIGN_RIGHT);
  display.setFont(ArialMT_Plain_10);
  if (disp_switch < 3) {
    display.drawString(127, 0, values.ip); 
  } else {
    display.drawString(127, 0, values.mDNS); 
  }

  disp_switch++;
  if (disp_switch > 5) {
    disp_switch = 0;
  }
  
  display.display();

}

/** バックライトオフ */
void disp_ssd1306_power_off() {
  if (!has_ssd1306()) return;

  display.displayOff();
}

String disp_ssd1306_set_brightness(int brightness) {
  const String PWR_OFF = "Display Power-Off";
  const String MAX = "Set Display brightness MAX(255)";
  const String VALUE = "Set Display brightness (0-255) = ";

  if (brightness == 0) {
    ssdlog(PWR_OFF);
    display.displayOff();
    return PWR_OFF;
  } else if (brightness == 255) {
    display.displayOn();
    display.setContrast(255, 255, 255);
    return MAX;
  } 

  String msg = VALUE + String(brightness);
  display.displayOn();
  display.setBrightness(brightness);
  return msg;
}

void disp_ssd1306_set_power(bool poweron) {
  if (!poweron) {
    display.displayOff();
  } else { 
    display.displayOn();
  }

}

void setup_disp_ssd1306() {

  if (has_ssd1306) {
    bool ret = display.init();

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