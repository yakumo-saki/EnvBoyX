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

String generate_http_setup_alerts_html(String name, String prefix, const config_alert_t& alerts) {
  String html;
  html += "<fieldset><legend>" + name + "</legend>";
  html += "<span class='m_caution1'>注意１</span>:&nbsp;<input type='number' class='num' name='" + prefix + "caution1.low' value='" + alerts.caution1.low + "'> 以上";
  html += "<input type='number' class='num' name='" + prefix + "caution1.high' value='" + alerts.caution1.high + "'>未満<br>";

  html += "<span class='m_caution2'>注意２</span>:&nbsp;<input type='number' class='num' name='" + prefix + "caution2.low' value='" + alerts.caution2.low + "'> 以上";
  html += "<input type='number' class='num' name='" + prefix + "caution2.high' value='" + alerts.caution2.high + "'>未満<br>";

  html += "<span class='m_alert1'>警報１</span>:&nbsp;<input type='number' class='num'name='" + prefix + "warning1.low' value='" + alerts.warning1.low + "'> 以上";
  html += "<input type='number' class='num' name='" + prefix + "warning1.high' value='" + alerts.warning1.high + "'>未満<br>";

  html += "<span class='m_alert2'>警報２</span>:&nbsp;<input type='number' class='num' name='" + prefix + "warning2.low' value='" + alerts.warning2.low + "'> 以上";
  html += "<input type='number' class='num' name='" + prefix + "warning2.high' value='" + alerts.warning2.high + "'>未満<br>";

  html += "</fieldset>";
  return html;
}

typedef struct  {
  String label;
  String value;
} LabelValue;

/**
 * @param name input tag name attribute
 * @param value current value
 */
String _create_radiobuttons(String name, String value, std::vector<LabelValue> choises) {
  String html = "";

  for (int i = 0; i < choises.size(); i++)
  {
    auto c = choises[i];
    String id = name + "__" + c.value;
    String checked = (value == c.value ? " checked" : "");

    html += "  <input type='radio' name='" + name + "'";
    html += " value='" + c.value + "'";
    html += " id='" + id + "'" + checked + ">";
    html += "<label for='" + id + "'>" + c.label + "</label><br>";
  }
  
  return html;
}

/**
 * GET 設定画面
 */
String http_setup_get_root_content() {

  // おかしなconfigファイルが入っていたとしても、デフォルト値が入ってくるので
  // チェックしなくても安全
  read_config();

  String html = "";
  html += "<!doctype html>";
  html += "<html>";
  html += "<head>";
  html += "<meta charset='UTF-8'>";
 	html += "<meta name='viewport' content='width=device-width'>";
  html += "<meta name='format-detection' content='telephone=no' />";
  html += "<title>" + product + " setting</title>";
  html += "<style>";
  html += "</style>";
  html += "<link rel='stylesheet' href='/style.css'>";
  html += "</head>";
  html += "<h1>" + product + " Settings  (" + ConfigValues::SETTING_ID + ")</h1>";
  html += "<form method='post'>";
  html += "  <br>";


  html += "<fieldset><legend class='m_legend_network'>ネットワーク設定</legend>";
  html += "  <strong class='m_wifi_info'>WiFi接続情報</strong><br>";
  html += "  <span class='m_wifi_msg2'>※ 2.4GHz帯のみ対応しています。</span><br>";
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
  {
    std::vector<LabelValue> choises;
    choises.push_back(LabelValue{"常時起動モード（測定値常時表示, HTTPサーバーあり）", ConfigValues::OPMODE_DISPLAY});
    choises.push_back(LabelValue{"MQTTモード（間欠動作・MQTT送信後ディープスリープ）", ConfigValues::OPMODE_MQTT});
    html += _create_radiobuttons(ConfigValues::OPMODE_NAME, config.opMode, choises);
  }
  html += "</fieldset>";

  html += "<fieldset><legend>表示デバイス共通設定</legend>";
  html += "  <strong>画面反転</strong><br>";
  {
    std::vector<LabelValue> choises;
    choises.push_back(LabelValue{"反転しない", ConfigValues::DISPLAY_FLIP_OFF});
    choises.push_back(LabelValue{"反転する", ConfigValues::DISPLAY_FLIP_ON});
    html += _create_radiobuttons(ConfigValues::DISPLAY_FLIP_NAME, config.displayFlip, choises);
  }

  html += "  <strong>明るさ(0-255)</strong><br>";
  html += "  <input type='text' name='displayBrightness' value='" + config.displayBrightness + "'><br>";

  html += "  <strong>Wait for reconfigure画面</strong><br>";
  html += "  ※ 表示しない場合、セットアップモードに入るためにはWeb APIを使用する必要があります。<br>";
  {
    std::vector<LabelValue> choises;
    choises.push_back(LabelValue{"表示する（推奨）", ConfigValues::DISPLAY_RECONFIG_ON});
    choises.push_back(LabelValue{"表示しない", ConfigValues::DISPLAY_RECONFIG_SKIP});
    html += _create_radiobuttons(ConfigValues::DISPLAY_RECONFIG_NAME, config.displaySkipReconfigure, choises);
  }

  html += "</fieldset>";

  html += "<fieldset><legend>I2C OLED デバイス設定</legend>";
  html += "  ※ 接続されていない場合無視されます。画面端にゴミが表示されている、または数ドット欠けている場合は変更してください。<br>";
  {
    std::vector<LabelValue> choises;
    choises.push_back(LabelValue{"SSD1306", ConfigValues::OLED_SSD1306});
    choises.push_back(LabelValue{"SH1106", ConfigValues::OLED_SH1106});
    html += _create_radiobuttons(ConfigValues::OLED_NAME, config.oledType, choises);
  }

  html += "</fieldset>";
 
  html += "<fieldset><legend>ST7789 デバイス設定</legend>";

  html += "  <strong>ST7789 SPI液晶の有無</strong><br>";
  html += "  ※ MQTTモードでは無効。<br>";
  html += "  ※ SPIピンはビルドオプションで指定<br>";
  {
    std::vector<LabelValue> choises;
    choises.push_back(LabelValue{"使用しない", ConfigValues::ST7789_NOUSE});
    choises.push_back(LabelValue{"使用する", ConfigValues::ST7789_USE});
    html += _create_radiobuttons(ConfigValues::ST7789_NAME, config.st7789, choises);
  }

  html += "  <strong>ST7789 表示モード</strong><br>";
  {
    std::vector<LabelValue> choises;
    choises.push_back(LabelValue{"横表示モード", ConfigValues::ST7789_MODE_NORMAL});
    choises.push_back(LabelValue{"縦表示モード（デカ文字）", ConfigValues::ST7789_MODE_BIG});
    html += _create_radiobuttons(ConfigValues::ST7789_MODE_NAME, config.st7789Mode, choises);
  }
  html += "</fieldset>";

  html += "<fieldset><legend>MH-Z19B デバイス設定</legend>";
  html += "  <strong>MH-Z19B CO2センサー有無（金色のセンサー）</strong><br>";
  {
    std::vector<LabelValue> choises;
    choises.push_back(LabelValue{"使用しない", ConfigValues::MHZ_NOUSE});
    choises.push_back(LabelValue{"使用する（UARTモード）", ConfigValues::MHZ_USE_UART});
    // choises.push_back(LabelValue{"使用する（PWMモード）", ConfigValues::MHZ_USE_PWM});

    html += _create_radiobuttons(ConfigValues::MHZ_NAME, config.mhz19b, choises);
  }
  
  //html += "  <br>";
  // html += "  <strong>MH-Z19BのPWMピンが接続されているGPIOピン番号</strong><br>";
  // html += "  <input type='text' name='mhz19bPwmPin' placeholder='GPIOピン番号' value='" + config.mhz19bPwmPin + "' placeholder='14'><br>";
  html += "  <input type='hidden' name='mhz19bPwmPin' value='14'>";
  html += "  <br>";

  html += "  <strong>MH-Z19BのUARTが接続されているGPIOピン番号</strong><br>";
  html += "  ※ MH-Z19Bを使用しない場合は設定不要<br>";
  html += "  ※ ESP8266では RX 14 TX 0 で固定<br>";
  html += "  ※ ボードによって使用可能なピンが異なるので動作しない場合は他のピンを試してください。<br>";
  html += "  RXピン <input type='text' name='mhz19bRxPin' placeholder='MHZ-19B RXピン番号' value='" + config.mhz19bRxPin + "' placeholder='16'><br>";
  html += "  TXピン <input type='text' name='mhz19bTxPin' placeholder='MHZ-19B TXピン番号' value='" + config.mhz19bTxPin + "' placeholder='17'><br>";
  html += "  <br>";

  html += "  <strong>起動時のAuto Baseline Correction</strong><br>";
  html += "  ※ MH-Z19Bを使用しない場合・MH-Z19BがUARTモード以外の場合は無視されます。<br>";
  {
    std::vector<LabelValue> choises;
    choises.push_back(LabelValue{"無効（標準）", ConfigValues::MHZ_ABC_OFF});
    choises.push_back(LabelValue{"有効", ConfigValues::MHZ_ABC_ON});
    html += _create_radiobuttons(ConfigValues::MHZ_ABC_NAME, config.mhz19bABC, choises);
  }

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

  html += "<fieldset><legend>アラート設定</legend>";
  html += generate_http_setup_alerts_html("気温", "tempAlerts.", config.temperatureAlerts);
  html += generate_http_setup_alerts_html("湿度", "humiAlerts.", config.humidityAlerts);
  html += generate_http_setup_alerts_html("照度", "luxAlerts.", config.luxAlerts);
  html += generate_http_setup_alerts_html("気圧", "presAlerts.", config.pressureAlerts);
  html += generate_http_setup_alerts_html("CO2濃度", "co2Alerts.", config.co2Alerts);

  html += "</fieldset>";


  html += "  <br>";
  html += "  <input type='submit' value='設定する'><br>";
  html += "</form>";
  html += "<br>";
  html += "<hr>";
  html += product_long + ", Copyright 2018-2021 Yakumo Saki / ziomatrix.org.";
  html += "</html>";

  return html;
}

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
    html += "動作モード：不明" + config.opMode + "<br>";    
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
