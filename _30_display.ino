// 最後に取得した温度
extern float lastTemp;
extern float lastHumidity;
extern float lastPressure;
extern float lastLuxFull;
extern float lastLuxIr;
extern int lastPpm;

const byte DISP_ADDR = 0x3c;

// LCD Display https://github.com/ThingPulse/esp8266-oled-ssd1306
//SSD1306 display(0x3c, 5, 4);
SSD1306 display(DISP_ADDR, 5, 4);

// 画面反転がいるかどうか。Envboy 3までは true。 3.5からは不要
bool needFlip = false;

bool has_ssd1306() {
  Wire.beginTransmission(DISP_ADDR);
  byte error = Wire.endTransmission();

  if (error != 0) {
    Serial.print("Error display connection: " + error);
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
  display.drawString(0, 32, "initializing.");
  display.drawString(0, 48, "Please wait");
  display.display();

  delay(1000);  
}

/**
 * セットアップモード時のディスプレイ表示
 */
void disp_setup_startup_screen() {

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
  display.drawString(0, 33, "http://" + WiFi.softAPIP().toString() + "/" );
  display.setFont(ArialMT_Plain_10);
  display.drawString(0, 52, ssid);
  display.display();

  delay(1000);  
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

  delay(1000);
   
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

// wait for reconfigure 画面を出しながら待つ
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
  display.drawString(0, 32, "Power off now");
  display.drawString(0, 48, " to re-configure");

  int MAX_BAR = 20;
  String bar = "*";
  for (int i = 0; i < MAX_BAR; i++) {
    String bar = "*";
    for(int j = 0; j < MAX_BAR; j++) {
      bar = bar + ( (j <= i) ? "-" : " ");
    }

    display.drawString(0, 16, bar);
    display.display();
    delay(120);
  }

  bar = bar + "*";
  display.drawString(0, 16, bar);
  display.display();
  delay(500);

}

void disp_sensor_value(String ip, String mdns) {

  if (!has_ssd1306()) return;

  display.clear();
  display.setTextAlignment(TEXT_ALIGN_RIGHT);
  display.setFont(ArialMT_Plain_10);
  display.drawString(127, 0, "IP:" + ip); 
  display.drawString(127, 9, mDNS); 

  display.setTextAlignment(TEXT_ALIGN_LEFT);
  display.drawString(0, 0, product); 

  display.setFont(ArialMT_Plain_16);
  display.drawString(0, 19, "T:" + String(lastTemp, 1) + "c" + " H:" + String(lastHumidity, 1) + "%" ); 
  display.drawString(0, 34, "P:" + String(lastPressure, 1) + " " + "L:" + String(lastLuxFull, 0)); 
  display.drawString(0, 49, String("CO2:") + String(lastPpm) + "ppm" ); 
  // "L:" + String(lastLuxFull, 0) + " " + "Ir:" + String(lastLuxIr, 0)

  display.setTextAlignment(TEXT_ALIGN_RIGHT);
  display.setFont(ArialMT_Plain_10);
  display.drawString(127, 54, ver); 
  
  display.display();

}

void disp_power_off() {
  if (!has_ssd1306()) return;

  display.displayOff();
}
