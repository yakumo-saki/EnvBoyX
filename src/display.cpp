#include <Arduino.h>

#include "global.h"
#include "log.h"

#include <SSD1306.h>
#include <WiFiClient.h> 

// 最後に取得した温度
extern float lastTemp;
extern float lastHumidity;
extern float lastPressure;
extern float lastLuxFull;
extern float lastLuxIr;
extern int lastPpm;

const byte DISP_ADDR = 0x3c;

const int WAIT_PER_BAR = 30;

//SSD1306 display(0x3c, 5, 4);
SSD1306 display(DISP_ADDR, I2C_SDA, I2C_SCL);

// 右下のver表示を点滅させる為のフラグ
bool blink_ver = true;

// 画面反転がいるかどうか。Envboy 3までは true。 3.5からは不要
bool needFlip = false;

bool has_ssd1306() {
  return true;
  Wire.beginTransmission(DISP_ADDR);
  int error = Wire.endTransmission();

  if (error != 0) {
    displog("Error display connection: " + String(error));
  }
  return (error == 0);
   
}

/**
 * 起動時の画面表示（共通）
 */
void disp_normal_startup_screen(String product_long) {

  if (!has_ssd1306()) return;
  
  display.init();

  if (needFlip) {
    display.flipScreenVertically();
  }
  display.setFont(ArialMT_Plain_16);
  display.clear();
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
void disp_setup_startup_screen(String ipAddr) {

  if (!has_ssd1306()) return;

  // display init and show initial screen
  display.init();
  if (needFlip) {
    display.flipScreenVertically();
  }

  display.setFont(ArialMT_Plain_16);
  display.clear();
  display.setTextAlignment(TEXT_ALIGN_LEFT);
  display.drawString(0, 0,  product_long);
  display.drawString(0, 16, "Setup mode.");
  display.drawString(0, 33, "http://" + ipAddr + "/" );
  display.setFont(ArialMT_Plain_10);
  display.drawString(0, 52, ssid);
  display.display();
}

void disp_wifi_info(String ip, String mDNS) {

  if (!has_ssd1306()) return;

  display.init();

//  String ssidStr = "SSID ";
//  ssidStr.concat(ssid);
  
  if (needFlip) {
    display.flipScreenVertically();
  }
  display.clear();
  display.setTextAlignment(TEXT_ALIGN_LEFT);
  display.setFont(ArialMT_Plain_10);
  display.setFont(ArialMT_Plain_16);
  display.drawString(0, 0, ssid);
  display.drawString(0, 16, ip);
  display.drawString(0, 32, mDNS);
  display.drawString(0, 48, "Starting up...");
  display.display();

  delay(300);
   
}

void disp_wifi_error() {

  if (!has_ssd1306()) return;

  // display init and show initial screen
  display.init();
  
  if (needFlip) {
    display.flipScreenVertically();
  }
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
void disp_wait_for_reconfig() {

  if (!has_ssd1306()) return;

  display.init();
 
  if (needFlip) {
    display.flipScreenVertically();
  }
  display.clear();
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

  int MAX_BAR = 31;  // _ の数。プロポーショナルフォントなので幅注意  
  int now = 1;
  for (int i = 0; i < MAX_BAR; i++) {
    bar = "|";
    
    for (int n = 0; n < now ; n++) {
      bar = bar + "|";
    }

    display.drawString(0, 16, bar);
    display.display();
    delay(WAIT_PER_BAR);
    now = now + 1;
  }

}

void disp_sensor_value(String ip, String mdns) {

  if (!has_ssd1306()) return;

  display.clear();

  // ヘッダ1行目
  display.setTextAlignment(TEXT_ALIGN_RIGHT);
  display.setFont(ArialMT_Plain_10);
  display.drawString(127, 0, "IP:" + ip); 
  display.setTextAlignment(TEXT_ALIGN_LEFT);
  display.drawString(0, 0, product); 

  // ヘッダ2行目
  // display.drawString(127, 9, mDNS); 

  // データ ヘッダ2行時のY座標19,34,49
  display.setFont(ArialMT_Plain_16);
  String ppm = String(lastPpm);
  if (lastPpm < 0) {
    ppm = "****";  // 計測エラー
  } else if (lastPpm < 399) {
    ppm = "*" + String(lastPpm); // あり得ない値(最低399ppmなはず）
  } else {
    ppm = String(lastPpm);       // OK
  }

  display.drawString(0, 12, "  " + String(lastTemp, 2) + "c" + "    " + String(lastHumidity, 2) + "%" ); 
  display.drawString(0, 29, "" + String(lastPressure, 1) + "hpa " + String(lastLuxFull, 0) + "lx"); 
  display.drawString(0, 47, String("CO2:") + ppm + "ppm" ); 
  // "L:" + String(lastLuxFull, 0) + " " + "Ir:" + String(lastLuxIr, 0)

  display.setTextAlignment(TEXT_ALIGN_RIGHT);
  display.setFont(ArialMT_Plain_10);
  display.drawString(127, 54, ver);

  if (blink_ver) {
    display.setTextAlignment(TEXT_ALIGN_RIGHT);
    display.setFont(ArialMT_Plain_10);
    display.drawString(49, 0, "*");
  }
  blink_ver = !blink_ver;
  
  display.display();

}

void disp_power_off() {
  if (!has_ssd1306()) return;

  display.displayOff();
}
