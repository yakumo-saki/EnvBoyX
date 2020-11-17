#include <Arduino.h>

#ifdef ESP32
#include <WiFi.h>
#elif defined(ESP8266)
#include <ESP8266WiFi.h>
#endif

#include "http.h"
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
  html += "<title>" + product + " setting</title>";
  html += "<meta charset='UTF-8'>";
  html += "<style>";
  html += "  input[type='text'] { width:200px }";
  html += "  input[type='submit'] { width:200px;}";
  html += "</style>";
  html += "</head>";
  html += "<h1>" + product + " Settings</h1>";
  html += "<form method='post'>";
  html += "  <br>";
  html += "  接続先WiFiの情報です。面倒ですが正しく設定して下さい。<br>";
  html += "  2.4GHz帯のみ対応しています。<br>";
  html += "  <input type='text' name='ssid' placeholder='ssid' autofocus value='" + ssid + "'><br>";
  html += "  <input type='text' name='pass' placeholder='pass' value='" + password + "'><br>";
  html += "  mDNS(Rendezvous) の名前です。LAN内の他の端末等と重複しないようにして下さい。<br>";
  html += "  ハイフン、アンダースコアを使用すると名前解決に失敗する可能性があるため非推奨です。<br>";
  html += "  <input type='text' name='mdnsname' placeholder='mdnsname' value='" + mDNS + "'><br>";
  html += "  <br>";
  html += "  動作モード<br>";
  html += "  <br>";
  
  String opmode_always_checked = (opMode == OPMODE_DISPLAY ? " checked" : "");
  String opmode_mqtt_checked = (opMode == OPMODE_MQTT ? " checked" : "");
  html += "  <input type='radio' name='opmode' value='" + OPMODE_DISPLAY + "' id='opmode_always'" + opmode_always_checked + "><label for='opmode_always'>常時起動モード（測定値常時表示）</label></label>";
  html += "  <input type='radio' name='opmode' value='" + OPMODE_MQTT + "' id='opmode_mqtt'" + opmode_mqtt_checked + "><label for='opmode_mqtt'>MQTTモード（間欠動作・ディープスリープ）</label><br>";
  html += "  <br>";
  html += "  MH-Z19B CO2センサー有無（金色のセンサー）<br>";

  String mhz19b_nouse_checked = (use_mhz19b == MHZ_NOUSE ? " checked" : "");
  String mhz19b_uart_checked = (use_mhz19b == MHZ_USE_UART ? " checked" : "");
  String mhz19b_pwm_checked = (use_mhz19b == MHZ_USE_PWM ? " checked" : "");

  html += "  <input type='radio' name='use_mhz19b' value='" + String(MHZ_NOUSE)  + "' id='mhz19b_no'" + mhz19b_nouse_checked + "><label for='mhz19b_no'>使用しない（通常はこちら）</label><br>";
  html += "  <input type='radio' name='use_mhz19b' value='" + String(MHZ_USE_UART) + "' id='mhz19b_uart'" + mhz19b_uart_checked + "><label for='mhz19b_uart'>使用する（UARTモード）</label><br>";
  html += "  <input type='radio' name='use_mhz19b' value='" + String(MHZ_USE_PWM) + "' id='mhz19b_pwm'" + mhz19b_pwm_checked + "><label for='mhz19b_pwm'>使用する（PWMモード・5000ppm）</label><br>";
  html += "  <br>";

  html += "  ＜MH-Z19B PWM モード専用。それ以外では入力不要です＞<br>";
  html += "  MH-Z19BのPWMピンが接続されているGPIOピン番号<br>";
  html += "  <input type='text' name='mhz19b_pwmpin' placeholder='GPIOピン番号' value='" + mhz19b_pwmpin + "' placeholder='14'><br>";
  html += "  <br>";
  html += "  ＜MH-Z19B UART モード専用。変更できません＞<br>";
  html += "  MH-Z19BのUART Rxピンが接続されているGPIOピン番号(ex 13)<br>";
  html += "  ESP8266 Rx = 14 Tx = 0<br>";
  html += "  ESP32   Rx = 16 Tx = 17<br>";
  html += "  <br>";
  html += "  ＜MQTTモード専用。それ以外では入力不要です＞<br>";
  html += "  MQTTブローカーのIPアドレスです。ホスト名も可能ですが、mDNS(bonjour, avahi)は使用出来ません。<br>";
  html += "  <input type='text' name='mqttbroker' placeholder='mqttbroker' value='" + mqttBroker + "'><br>";
  html += "  <br>";
  html += "  ＜MQTTモード専用。それ以外では入力不要です＞<br>";
  html += "  MQTT名です。クライアント名とトピック名に使用されます。<br>";
  html += "  <input type='text' name='mqttname' placeholder='mqttname' value='" + mqttName + "'><br>";
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
  if (opMode == "always") {
    html += "動作モード：常時稼働モード<br>";    
    html += "SSID " + ssid + "<br>";
    html += "PASS " + password + "<br>";
    html += "mDNS " + mDNS + "<br>";
    if (use_mhz19b == MHZ_USE_UART) {
      html += "MHZ19B を使用する（UART）<br>";
    } else if (use_mhz19b == MHZ_USE_PWM) {
      html += "MHZ19B を使用する（PWM）";
      html += " GPIOピン番号=" + String(mhz19b_pwmpin) + "<br>";
    } else if (use_mhz19b == MHZ_NOUSE) {
      html += "MHZ19B を使用しない、または接続されていない<br>";
    } else {
      html += "【バグ】MHZ19B設定が異常です。 =>" + use_mhz19b + "<br>";           
    }

  } else if (opMode == "mqtt") {
    html += "動作モード：MQTT間欠動作モード<br>";
    html += "SSID " + ssid + "<br>";
    html += "PASS " + password + "<br>";
    html += "mDNS " + mDNS + "<br>";
    html += "MQTT broker " + mqttBroker + "<br>";
    html += "MQTT name   " + mqttName + "<br>";
    if (use_mhz19b == MHZ_USE_UART) {
      html += "MHZ19B を使用する（UARTモード）<br>";
    } else if (use_mhz19b == MHZ_USE_PWM) {
      html += "MHZ19B を使用する(PWMモード)<br>";     
      html += " GPIOピン番号=" + String(mhz19b_pwmpin) + "<br>";
    } else if (use_mhz19b == MHZ_NOUSE) {
      html += "MHZ19B を使用しない、または接続されていない<br>";     
    } else {
      html += "【バグ】MHZ19B設定が異常です。 =>" + use_mhz19b + "<br>";           
    }
  } else {
    html += "動作モード：不明" + opMode + "<br>";    
  }

  html += "<br>";
  html += "Restart (power off then power on) to use above setting.<br>";
  html += "設定が完了しました。リセットボタンを押して再起動して下さい。<br>";
  html += "<br>";
  html += "<a href='/'>setting again</a>";
  html += "</html>";
  
  return html;
}
