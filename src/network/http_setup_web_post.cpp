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

String generate_alerts_html(String name, const config_alert_t& alerts) {
  String log = name + "：<br>";
  log += "警報１ '" + alerts.warning1.low + "' 〜 '" + alerts.warning1.high + "'<br>";
  log += "警報２ '" + alerts.warning2.low + "' 〜 '" + alerts.warning2.high + "'<br>";
  log += "注意１ '" + alerts.caution1.low + "' 〜 '" + alerts.caution1.high + "'<br>";
  log += "注意２ '" + alerts.caution2.low + "' 〜 '" + alerts.caution2.high + "'<br>";
  return log;
}

/**
 * Post 設定 ( config の post。 ファイルに設定を保存）
 */
String http_setup_post_root_content() {

  save_config();

  String html = "";
  html += "<html>";
  html += "<head>";
  html += "<title>" + product + " setting done. please restart.</title>";
  html += "<meta charset='UTF-8'>";
  html += "<link rel='stylesheet' href='/style.css'>";
  html += "<style>";
  html += "  input { width:200px; }";
  html += "</style>";
  html += "</head>";
  html += "<body class='setup_done'>";

  html += "<h1>" + product + " setting done</h1>";
  if (config.opMode == "always") {
    html += "動作モード：常時稼働モード<br>";    
    html += "SSID " + config.ssid + "<br>";
    html += "PASS " + config.password + "<br>";
    html += "mDNS " + config.mDNS + "<br>";

    if (config.displayFlip == ConfigValues::DISPLAY_FLIP_OFF) {
      html += "画面反転：反転しない<br>";
    } else if (config.displayFlip == ConfigValues::DISPLAY_FLIP_ON) {
      html += "画面反転：反転する<br>";
    } else {
      html += "【バグ】画面反転の設定が異常です => " + config.displayFlip + "<br>";
    }

    html += "画面の明るさ：" + config.displayBrightness + "<br>";

    if (config.oledType == ConfigValues::OLED_SSD1306) {
      html += "I2C OLED デバイス： SSD1306<br>";
    } else if (config.oledType == ConfigValues::OLED_SH1106) {
      html += "I2C OLED デバイス： SH1106<br>";
    } else if (config.oledType == ConfigValues::OLED_SH1106) {
      html += "【バグ】I2C OLED デバイスの指定が異常です。 " + config.oledType + "<br>";
    }

    if (config.displaySkipReconfigure == ConfigValues::DISPLAY_RECONFIG_ON) {
      html += "Wait for Reconfigure: 表示する<br>";
    } else if (config.displaySkipReconfigure == ConfigValues::DISPLAY_RECONFIG_SKIP) {
      html += "Wait for Reconfigure: スキップする<br>";
    } else {
      html += "【バグ】Wait for Reconfigure画面の設定が異常です => " + config.displaySkipReconfigure + "<br>";
    }


    if (config.st7789 == ConfigValues::ST7789_USE) {
      html += "ST7789 を使用する<br>";

      if (config.st7789Mode == ConfigValues::ST7789_MODE_BIG) {
        html += "ST7789表示モード：縦（デカ文字）<br>";
      } else if (config.st7789Mode == ConfigValues::ST7789_MODE_NORMAL) {
        html += "ST7789表示モード：横（標準）<br>";
      } else {
        html += "【バグ】ST7789表示モード設定が異常です => " + config.st7789Mode + "<br>";
      }
    } else if (config.st7789 == ConfigValues::ST7789_NOUSE) {
      html += "ST7789 を使用しない<br>";
    } else {
      html += "【バグ】ST7789設定が異常です => " + config.st7789 + "<br>";
    }

    if (config.mhz19b == ConfigValues::MHZ_USE_UART) {
      html += "MHZ19B を使用する（UART）<br>";
      html += "  RXピン番号=" + String(config.mhz19bRxPin) + "<br>";
      html += "  TXピン番号=" + String(config.mhz19bTxPin) + "<br>";
      if (config.mhz19bABC == ConfigValues::MHZ_ABC_ON) {
        html += "  Auto Baseline Correction=有効<br>";
      } else if (config.mhz19bABC == ConfigValues::MHZ_ABC_OFF) {
        html += "  Auto Baseline Correction=オフ<br>";
      } else {
        html += "【バグ】MHZ19B Auto Baseline Correction設定が異常です。 =>" + config.mhz19bABC + "<br>";           
      }
    } else if (config.mhz19b == ConfigValues::MHZ_USE_PWM) {
      html += "MHZ19B を使用する（PWM） ※ 廃止されたモードです";
      html += " GPIOピン番号=" + String(config.mhz19bPwmPin) + "<br>";
    } else if (config.mhz19b == ConfigValues::MHZ_NOUSE) {
      html += "MHZ19B を使用しない、または接続されていない<br>";
    } else {
      html += "【バグ】MHZ19B設定が異常です。 =>" + config.mhz19b + "<br>";           
    }


  } else if (config.opMode == "mqtt") {
    html += "動作モード：MQTT間欠動作モード<br>";
    html += "SSID " + config.ssid + "<br>";
    html += "PASS " + config.password + "<br>";
    html += "mDNS " + config.mDNS + "<br>";
    html += "MQTT broker " + config.mqttBroker + "<br>";
    html += "MQTT name   " + config.mqttName + "<br>";
    if (config.mhz19b == ConfigValues::MHZ_USE_UART) {
      html += "MHZ19B を使用する（UARTモード）<br>";
    } else if (config.mhz19b == ConfigValues::MHZ_USE_PWM) {
      html += "MHZ19B を使用する(PWMモード)<br>";     
      html += " GPIOピン番号=" + String(config.mhz19bPwmPin) + "<br>";
    } else if (config.mhz19b == ConfigValues::MHZ_NOUSE) {
      html += "MHZ19B を使用しない、または接続されていない<br>";     
    } else {
      html += "【バグ】MHZ19B設定が異常です。 =>" + config.mhz19b + "<br>";           
    }
  } else {
    html += "【バグ】動作モード：不明" + config.opMode + "<br>";    
  }

  html += generate_alerts_html("気温", config.temperatureAlerts);
  html += generate_alerts_html("湿度", config.humidityAlerts);
  html += generate_alerts_html("照度", config.luxAlerts);
  html += generate_alerts_html("気圧", config.pressureAlerts);
  html += generate_alerts_html("CO2濃度", config.co2Alerts);

  html += "<br>";
  html += "Restart (power off then power on) to use above setting.<br>";
  html += "設定が完了しました。リセットボタンを押して再起動して下さい。<br>";
  html += "<br>";
  html += "<a class='setup_again' href='/'>setting again</a>";
  html += "</body>";
  html += "</html>";
  
  return html;
}
