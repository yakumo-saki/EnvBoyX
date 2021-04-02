#include <Arduino.h>

#ifdef ESP32
#include <WiFi.h>
#elif defined(ESP8266)
#include <ESP8266WiFi.h>
#endif

#include "log.h"
#include "display/display.h"
#include "global.h"
#include "config.h"

#include "http_setup.h"
#include "network/webserver.h"
#include "network/http_utils.h"

/**
 * Post 設定 ( config の post。 ファイルに設定を保存）
 */
void http_send_setup_post_root_html() {

  sendHttpHeader();
  sendHtmlHeader();

  String html = "";
  html += "<body class='setup_done'>";
  html += "<h1>" + product + " setting done</h1>";
  if (config->get(ConfigNames::OPMODE) == "always") {
    html += "動作モード：常時稼働モード<br>";    
    html += "SSID " + config->get(ConfigNames::SSID) + "<br>";
    html += "PASS " + config->get(ConfigNames::PASSWORD) + "<br>";
    html += "mDNS " + config->get(ConfigNames::MDNS) + "<br>";

    String oledType = config->get(ConfigNames::OLED_TYPE);
    if (oledType == ConfigValues::OLED_SSD1306) {
      html += "I2C OLED デバイス： SSD1306<br>";
    } else if (oledType == ConfigValues::OLED_SH1106) {
      html += "I2C OLED デバイス： SH1106<br>";
    } else if (oledType == ConfigValues::OLED_SH1106) {
      html += "【バグ】I2C OLED デバイスの指定が異常です。 " + oledType + "<br>";
    }

    String st7789 = config->get(ConfigNames::ST7789);
    String st7789Mode = config->get(ConfigNames::ST7789_MODE);
    if (config->get(ConfigNames::ST7789) == ConfigValues::ST7789_USE) {
      html += "ST7789 を使用する<br>";

      if (st7789Mode == ConfigValues::ST7789_MODE_BIG) {
        html += "ST7789表示モード：縦（デカ文字）<br>";
      } else if (st7789Mode == ConfigValues::ST7789_MODE_NORMAL) {
        html += "ST7789表示モード：横（標準）<br>";
      } else {
        html += "【バグ】ST7789表示モード設定が異常です => " + st7789Mode + "<br>";
      }
    } else if (st7789 == ConfigValues::ST7789_NOUSE) {
      html += "ST7789 を使用しない<br>";
    } else {
      html += "【バグ】ST7789設定が異常です => " + st7789 + "<br>";
    }

    String mhz19b = config->get(ConfigNames::MHZ19B);
    String mhz19bABC = config->get(ConfigNames::MHZ19B_ABC);
    String mhz19bPwmPin = config->get(ConfigNames::MHZ19B_PWM);
    String mhz19bRxPin = config->get(ConfigNames::MHZ19B_RX);
    String mhz19bTxPin = config->get(ConfigNames::MHZ19B_TX);

    if (mhz19b == ConfigValues::MHZ_USE_UART) {
      html += "MHZ19B を使用する（UART）<br>";
      html += "  RXピン番号=" + String(mhz19bRxPin) + "<br>";
      html += "  TXピン番号=" + String(mhz19bTxPin) + "<br>";
      if (mhz19bABC == ConfigValues::MHZ_ABC_ON) {
        html += "  Auto Baseline Correction=有効<br>";
      } else if (mhz19bABC == ConfigValues::MHZ_ABC_OFF) {
        html += "  Auto Baseline Correction=オフ<br>";
      } else {
        html += "【バグ】MHZ19B Auto Baseline Correction設定が異常です。 =>" + mhz19bABC + "<br>";           
      }
    } else if (mhz19b == ConfigValues::MHZ_USE_PWM) {
      html += "MHZ19B を使用する（PWM） ※ 廃止されたモードです";
      html += " GPIOピン番号=" + String(mhz19bPwmPin) + "<br>";
    } else if (mhz19b == ConfigValues::MHZ_NOUSE) {
      html += "MHZ19B を使用しない、または接続されていない<br>";
    } else {
      html += "【バグ】MHZ19B設定が異常です。 =>" + mhz19b + "<br>";           
    }
  } else if (config->get(ConfigNames::OPMODE) == ConfigValues::OPMODE_MQTT) {
    html += "動作モード：MQTT間欠動作モード<br>";
    html += "SSID " + config->get(ConfigNames::SSID) + "<br>";
    html += "PASS " + config->get(ConfigNames::PASSWORD) + "<br>";
    html += "mDNS " + config->get(ConfigNames::MDNS) + "<br>";
    html += "MQTT broker " + config->get(ConfigNames::MQTT_BROKER) + "<br>";
    html += "MQTT name   " + config->get(ConfigNames::MQTT_NAME) + "<br>";
  } else {
    html += "【バグ】動作モード：不明" + config->get(ConfigNames::OPMODE) + "<br>";
  }

  html += "<br>";
  html += "Restart (power off then power on) to use above setting.<br>";
  html += "設定が完了しました。リセットボタンを押して再起動して下さい。<br>";
  html += "<br>";
  html += "<a class='setup_again' href='/'>setting again</a>";
  html += "</body>";
  html += "</html>";
  
  server.sendContent(html);
}
