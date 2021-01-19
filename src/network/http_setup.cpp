#include <Arduino.h>

#ifdef ESP32
#include <WiFi.h>
#elif defined(ESP8266)
#include <ESP8266WiFi.h>
#endif

#include "log.h"
#include "display.h"
#include "global.h"
#include "config.h"

#include "http_setup.h"

/**
 * GET 設定画面
 */
String http_setup_get_root_content() {

  if (has_valid_config_file()) {
    read_config();
  } else {
    set_default_config_value();
  }

  String html = "";
  html += "<!doctype html>";
  html += "<html>";
  html += "<head>";
  html += "<meta charset='UTF-8'>";
 	html += "<meta name='viewport' content='width=device-width'>";
  html += "<meta name='format-detection' content='telephone=no' />";
  html += "<title>" + product + " setting</title>";
  html += "<style>";
  html += "  input[type='text'] { width:200px }";
  html += "  input[type='submit'] { width:200px; height: 48px;}";
  html += "</style>";
  html += "</head>";
  html += "<h1>" + product + " Settings  (" + SETTING_ID + ")</h1>";
  html += "<form method='post'>";
  html += "  <br>";


  html += "<fieldset><legend>ネットワーク設定</legend>";
  html += "  <strong>WiFi接続情報</strong><br>";
  html += "  ※ 2.4GHz帯のみ対応しています。<br>";
  html += "  <input type='text' name='ssid' placeholder='ssid' autofocus value='" + config.ssid + "'><br>";
  html += "  <input type='text' name='pass' placeholder='pass' value='" + config.password + "'><br>";
  html += "  <br>";
  html += "  <strong>mDNS名(a.k.a Rendezvous, avahi-daemon)</strong><br>";
  html += "  ※ mDNS名.local で他端末から本機にアクセスすることができます。<br>";
  html += "  <input type='text' name='mdnsname' placeholder='mdnsname' value='" + config.mDNS + "'><br>";
  html += "</fieldset>";
 

  html += "<fieldset><legend>動作モード設定</legend>";
  html += "  <strong>動作モード</strong><br>";
  html += "  ※ ESP8266でMQTTモードを使用するにはIO16ピンとRSTピンを接続する必要があります。<br>";
  String opmode_always_checked = (config.opMode == OPMODE_DISPLAY ? " checked" : "");
  String opmode_mqtt_checked = (config.opMode == OPMODE_MQTT ? " checked" : "");
  html += "  <input type='radio' name='opmode' value='" + OPMODE_DISPLAY + "' id='opmode_always'" + opmode_always_checked + "><label for='opmode_always'>常時起動モード（測定値常時表示, HTTPサーバーあり）</label><br>";
  html += "  <input type='radio' name='opmode' value='" + OPMODE_MQTT + "' id='opmode_mqtt'" + opmode_mqtt_checked + "><label for='opmode_mqtt'>MQTTモード（間欠動作・MQTT送信後ディープスリープ）</label><br>";
  html += "</fieldset>";


  html += "<fieldset><legend>表示デバイス共通設定</legend>";

  String display_flip_no_checked = (config.displayFlip == DISPLAY_FLIP_OFF ? " checked" : "");
  String display_flip_yes_checked = (config.displayFlip == DISPLAY_FLIP_ON ? " checked" : "");

  html += "  <strong>画面反転</strong><br>";
  html += "  <input type='radio' name='displayFlip' value='" + DISPLAY_FLIP_OFF + "' id='display_flip_no'" + display_flip_no_checked + "><label for='display_flip_no'>反転しない</label><br>";
  html += "  <input type='radio' name='displayFlip' value='" + DISPLAY_FLIP_ON + "' id='display_flip_yes'" + display_flip_yes_checked + "><label for='display_flip_yes'>反転する</label><br>";

  html += "  <strong>明るさ(0-255)</strong><br>";
  html += "  <input type='text' name='displayBrightness' value='" + config.displayBrightness + "'><br>";
  html += "</fieldset>";


  html += "<fieldset><legend>ST7789 デバイス設定</legend>";

  String st7789_use_checked = (config.st7789 == ST7789_USE ? " checked" : "");
  String st7789_nouse_checked = (config.st7789 == ST7789_NOUSE ? " checked" : "");
  
  String st7789_big_checked = (config.st7789Mode == ST7789_MODE_BIG ? " checked" : "");
  String st7789_normal_checked = (config.st7789Mode == ST7789_MODE_NORMAL ? " checked" : "");

  html += "  <strong>ST7789 SPI液晶の有無</strong><br>";
  html += "  ※ MQTTモードでは無効。<br>";
  html += "  ※ SPIピンはビルドオプションで指定<br>";
  html += "  <input type='radio' name='st7789' value='" + ST7789_NOUSE + "' id='st7789_no'" + st7789_nouse_checked + "><label for='st7789_no'>使用しない</label><br>";
  html += "  <input type='radio' name='st7789' value='" + ST7789_USE + "' id='st7789_yes'" + st7789_use_checked + "><label for='st7789_yes'>使用する</label><br>";

  html += "  <strong>ST7789 表示モード</strong><br>";
  html += "  <input type='radio' name='st7789Mode' value='" + ST7789_MODE_NORMAL + "' id='st7789_normal'" + st7789_normal_checked + "><label for='st7789_normal'>横表示モード</label><br>";
  html += "  <input type='radio' name='st7789Mode' value='" + ST7789_MODE_BIG + "' id='st7789_big'" + st7789_big_checked + "><label for='st7789_big'>縦表示モード（デカ文字）</label><br>";

  html += "  <br>";


  html += "  <strong>MH-Z19B CO2センサー有無（金色のセンサー）</strong><br>";

  String mhz19b_nouse_checked = (config.use_mhz19b == MHZ_NOUSE ? " checked" : "");
  String mhz19b_uart_checked = (config.use_mhz19b == MHZ_USE_UART ? " checked" : "");
  String mhz19b_pwm_checked = (config.use_mhz19b == MHZ_USE_PWM ? " checked" : "");

  html += "  <input type='radio' name='use_mhz19b' value='" + String(MHZ_NOUSE)  + "' id='mhz19b_no'" + mhz19b_nouse_checked + "><label for='mhz19b_no'>使用しない（通常はこちら）</label><br>";
  html += "  <input type='radio' name='use_mhz19b' value='" + String(MHZ_USE_UART) + "' id='mhz19b_uart'" + mhz19b_uart_checked + "><label for='mhz19b_uart'>使用する（UARTモード）</label><br>";
  
  //html += "  <br>";
  // html += "  <strong>MH-Z19BのPWMピンが接続されているGPIOピン番号</strong><br>";
  // html += "  <input type='text' name='mhz19b_pwmpin' placeholder='GPIOピン番号' value='" + config.mhz19b_pwmpin + "' placeholder='14'><br>";
  html += "  <input type='hidden' name='mhz19b_pwmpin' value='14'>";
  html += "  <br>";

  html += "  <strong>MH-Z19BのUARTが接続されているGPIOピン番号</strong><br>";
  html += "  ※ MH-Z19Bを使用しない場合は設定不要<br>";
  html += "  ※ ESP8266では RX 14 TX 0 で固定<br>";
  html += "  ※ ボードによって使用可能なピンが異なるので動作しない場合は他のピンを試してください。<br>";
  html += "  RXピン <input type='text' name='mhz19b_rxpin' placeholder='MHZ-19B RXピン番号' value='" + config.mhz19b_rxpin + "' placeholder='16'><br>";
  html += "  TXピン <input type='text' name='mhz19b_txpin' placeholder='MHZ-19B TXピン番号' value='" + config.mhz19b_txpin + "' placeholder='17'><br>";
  html += "</fieldset>";
 

  html += "<fieldset><legend>MQTTモード専用設定</legend>";
  html += "  <strong>＜MQTTモード以外の場合は入力不要＞</strong><br>";
  html += "  <br>";
  html += "  <strong>MQTTブローカーのIPアドレス</strong><br>";
  html += "  ※ ホスト名も可能ですが、mDNS(bonjour, avahi)は使用出来ません。<br>";
  html += "  <input type='text' name='mqttbroker' placeholder='mqttbroker' value='" + config.mqttBroker + "'><br>";
  html += "  <br>";
  html += "  <strong>MQTT名</strong><br>";
  html += "  ※ クライアント名とトピック名として使用<br>";
  html += "  <input type='text' name='mqttname' placeholder='mqttname' value='" + config.mqttName + "'><br>";
  html += "</fieldset>";

  html += "  <br>";
  html += "  <input type='submit' value='設定する'><br>";
  html += "</form>";
  html += "<br>";
  html += "<hr>";
  html += product_long + ", Copyright 2018-2020 Yakumo Saki / ziomatrix.org.";
  html += "</html>";

  return html;
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
  html += "<style>";
  html += "  input { width:200px; }";
  html += "</style>";
  html += "</head>";

  html += "<h1>" + product + " setting done</h1>";
  if (config.opMode == "always") {
    html += "動作モード：常時稼働モード<br>";    
    html += "SSID " + config.ssid + "<br>";
    html += "PASS " + config.password + "<br>";
    html += "mDNS " + config.mDNS + "<br>";

    if (config.displayFlip == DISPLAY_FLIP_OFF) {
      html += "画面反転：反転しない<br>";
    } else if (config.displayFlip == DISPLAY_FLIP_ON) {
      html += "画面反転：反転する<br>";
    } else {
      html += "【バグ】画面反転の設定が異常です => " + config.displayFlip + "<br>";
    }

    html += "画面の明るさ：" + config.displayBrightness + "<br>";

    if (config.st7789 == ST7789_USE) {
      html += "ST7789 を使用する<br>";

      if (config.st7789Mode == ST7789_MODE_BIG) {
        html += "ST7789表示モード：縦（デカ文字）<br>";
      } else if (config.st7789Mode == ST7789_MODE_NORMAL) {
        html += "ST7789表示モード：横（標準）<br>";
      } else {
        html += "【バグ】ST7789表示モード設定が異常です => " + config.st7789Mode + "<br>";
      }
    } else if (config.st7789 == ST7789_NOUSE) {
      html += "ST7789 を使用しない<br>";
    } else {
      html += "【バグ】ST7789設定が異常です => " + config.st7789 + "<br>";
    }

    if (config.use_mhz19b == MHZ_USE_UART) {
      html += "MHZ19B を使用する（UART）<br>";
      html += "  RXピン番号=" + String(config.mhz19b_rxpin) + "<br>";
      html += "  TXピン番号=" + String(config.mhz19b_txpin) + "<br>";
    } else if (config.use_mhz19b == MHZ_USE_PWM) {
      html += "MHZ19B を使用する（PWM） ※ 廃止されたモードです";
      html += " GPIOピン番号=" + String(config.mhz19b_pwmpin) + "<br>";
    } else if (config.use_mhz19b == MHZ_NOUSE) {
      html += "MHZ19B を使用しない、または接続されていない<br>";
    } else {
      html += "【バグ】MHZ19B設定が異常です。 =>" + config.use_mhz19b + "<br>";           
    }

  } else if (config.opMode == "mqtt") {
    html += "動作モード：MQTT間欠動作モード<br>";
    html += "SSID " + config.ssid + "<br>";
    html += "PASS " + config.password + "<br>";
    html += "mDNS " + config.mDNS + "<br>";
    html += "MQTT broker " + config.mqttBroker + "<br>";
    html += "MQTT name   " + config.mqttName + "<br>";
    if (config.use_mhz19b == MHZ_USE_UART) {
      html += "MHZ19B を使用する（UARTモード）<br>";
    } else if (config.use_mhz19b == MHZ_USE_PWM) {
      html += "MHZ19B を使用する(PWMモード)<br>";     
      html += " GPIOピン番号=" + String(config.mhz19b_pwmpin) + "<br>";
    } else if (config.use_mhz19b == MHZ_NOUSE) {
      html += "MHZ19B を使用しない、または接続されていない<br>";     
    } else {
      html += "【バグ】MHZ19B設定が異常です。 =>" + config.use_mhz19b + "<br>";           
    }
  } else {
    html += "動作モード：不明" + config.opMode + "<br>";    
  }

  html += "<br>";
  html += "Restart (power off then power on) to use above setting.<br>";
  html += "設定が完了しました。リセットボタンを押して再起動して下さい。<br>";
  html += "<br>";
  html += "<a href='/'>setting again</a>";
  html += "</html>";
  
  return html;
}
