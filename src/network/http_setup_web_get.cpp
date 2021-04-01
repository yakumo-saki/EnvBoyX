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

typedef struct  {
  String label;
  String value;
} LabelValue;

String _create_input_impl(String name, String value, String placeholder = "", String type = "text", String cssClass = "") {
  String html = "<input ";
  html += "type='" + type + "'";
  html += " name='" + name + "'";
  if (placeholder != "") html += " placeholder='" + placeholder + "'";
  if (cssClass != "") html += " class='" + cssClass + "'";
  html += " value='" + value + "'>";
  return html;
}

String _create_input_nobr(String key, String placeholder = "", String type = "text", String cssClass = "") {
  String value = config->get(key);
  String html = _create_input_impl(key, value, placeholder, type, cssClass);

  return html + "\n";
}

String _create_input(String key, String placeholder = "", String type = "text", String cssClass = "") {
  String value = config->get(key);
  String html = _create_input_impl(key, value, placeholder, type, cssClass);
  html += "<br>\n";

  return html;
}

String _generate_alert(String nameLow, String nameHigh) {
  String html = "";
  html += _create_input_nobr(nameLow, "", "number", "num");
  html += " 以上\n";
  html += _create_input_nobr(nameHigh, "", "number", "num");
  html += "未満<br>\n";
  return html;
}

String _create_alert_name(String prefix, String name) {
  return prefix + "." + name;
}

String generate_http_setup_alerts_html(String name, String prefix) {
  String html;
  html += "<fieldset><legend>" + name + "</legend>\n";
  {
  html += "<span class='m_caution1'>注意１</span>:&nbsp;\n";
  String lo = _create_alert_name(prefix, ConfigNames::ALERT_CAUTION1_LO);
  String hi = _create_alert_name(prefix, ConfigNames::ALERT_CAUTION1_HI);
  html += _generate_alert(lo, hi);
  }

  {
  html += "<span class='m_caution1'>注意２</span>:&nbsp;\n";
  String lo = _create_alert_name(prefix, ConfigNames::ALERT_CAUTION2_LO);
  String hi = _create_alert_name(prefix, ConfigNames::ALERT_CAUTION2_HI);
  html += _generate_alert(lo, hi);
  }

  {
  html += "<span class='m_caution1'>警報１</span>:&nbsp;\n";
  String lo = _create_alert_name(prefix, ConfigNames::ALERT_WARN1_LO);
  String hi = _create_alert_name(prefix, ConfigNames::ALERT_WARN1_HI);
  html += _generate_alert(lo, hi);
  }

  {
  html += "<span class='m_caution1'>警報２</span>:&nbsp;\n";
  String lo = _create_alert_name(prefix, ConfigNames::ALERT_WARN2_LO);
  String hi = _create_alert_name(prefix, ConfigNames::ALERT_WARN2_HI);
  html += _generate_alert(lo, hi);
  }

  html += "</fieldset>\n";
  return html;
}

/**
 * @param name input tag name attribute
 * @param value current value
 */
String _create_radiobuttons(String name, std::vector<LabelValue> choises) {
  String html = "";

  String value = config->get(name);

  for (unsigned int i = 0; i < choises.size(); i++)
  {
    auto c = choises[i];
    String id = name + "__" + c.value;
    String checked = (value == c.value ? " checked" : "");

    html += "  <input type='radio' name='" + name + "'";
    html += " value='" + c.value + "'";
    html += " id='" + id + "'" + checked + ">";
    html += "<label for='" + id + "'>" + c.label + "</label><br>\n";
  }
  
  return html;
}

/**
 * GET 設定画面
 */
String http_setup_get_root_content() {

  // おかしなconfigファイルが入っていたとしても、デフォルト値が入ってくるので
  // チェックしなくても安全

  String html = "";
  html += "<!doctype html>";
  html += "<html>";
  html += "<head>";
  html += "<meta charset='UTF-8'>";
 	html += "<meta name='viewport' content='width=device-width'>";
  html += "<meta name='format-detection' content='telephone=no' />\n";
  html += "<title>" + product + " setting</title>\n";
  html += "<style>";
  html += "</style>\n";
  html += "<link rel='stylesheet' href='/style.css'>";
  html += "</head>\n";
  html += "<body>\n";
  html += "<h1>" + product + " Settings  (" + SETTING_ID + ")</h1>";
  html += "<form method='post'>\n";
  html += "  <br>";


  html += "<fieldset><legend class='m_legend_network'>ネットワーク設定</legend>\n";
  html += "  <strong class='m_wifi_info'>WiFi接続情報</strong><br>\n";
  html += "  <span class='m_wifi_msg2'>※ 2.4GHz帯のみ対応しています。</span><br>\n";
  html += _create_input(ConfigNames::SSID, "WiFi SSID");
  html += _create_input(ConfigNames::PASSWORD, "WiFi Password");
  html += "  <br>";
  html += "  <strong>mDNS名(a.k.a Rendezvous, avahi-daemon)</strong><br>\n";
  html += "  ※ mDNS名.local で他端末から本機にアクセスすることができます。<br>\n";
  html += _create_input(ConfigNames::MDNS);
  html += "</fieldset>\n";
 

  html += "<fieldset><legend>動作モード設定</legend>\n";
  html += "  <strong>動作モード</strong><br>\n";
  html += "  ※ ESP8266でMQTTモードを使用するにはIO16ピンとRSTピンを接続する必要があります。<br>\n";
  {
    std::vector<LabelValue> choises;
    choises.push_back(LabelValue{"常時起動モード（測定値常時表示, HTTPサーバーあり）", ConfigValues::OPMODE_DISPLAY});
    choises.push_back(LabelValue{"MQTTモード（間欠動作・MQTT送信後ディープスリープ）", ConfigValues::OPMODE_MQTT});
    html += _create_radiobuttons(ConfigNames::OPMODE, choises);
  }
  html += "</fieldset>\n";

  html += "<fieldset><legend>表示デバイス共通設定</legend>\n";
  html += "  <strong>画面反転</strong><br>\n";
  {
    std::vector<LabelValue> choises;
    choises.push_back(LabelValue{"反転しない", ConfigValues::DISPLAY_FLIP_OFF});
    choises.push_back(LabelValue{"反転する", ConfigValues::DISPLAY_FLIP_ON});
    html += _create_radiobuttons(ConfigNames::DISPLAY_FLIP, choises);
  }

  html += "  <strong>明るさ(0-255)</strong><br>\n";
  html += _create_input(ConfigNames::DISPLAY_BRIGHTNESS, "brightness 0-255", "number");

  html += "  <strong>Wait for reconfigure画面</strong><br>\n";
  html += "  ※ 表示しない場合、セットアップモードに入るためにはWeb APIを使用する必要があります。<br>\n";
  {
    std::vector<LabelValue> choises;
    choises.push_back(LabelValue{"表示する（推奨）", ConfigValues::DISPLAY_RECONFIG_ON});
    choises.push_back(LabelValue{"表示しない", ConfigValues::DISPLAY_RECONFIG_SKIP});
    html += _create_radiobuttons(ConfigNames::DISPLAY_RECONFIG, choises);
  }

  html += "  <strong>自動消灯する明るさ（暗いと判定するしきい値）</strong><br>\n";
  html += "  <span>※ 光量センサーがない場合は自動的に無効になります。</span><br>\n";
  html += "  <span>※ 自動消灯を使用しない場合は、999999を入力してください。</span><br>\n";
  html += _create_input(ConfigNames::DISPLAY_AUTODIM_LUX, "Lux", "number");

  html += "  <strong>自動消灯判定時間</strong><br>\n";
  html += "  <span>※ しきい値未満の明るさがこの時間継続したら消灯します。</span><br>\n";
  html += _create_input(ConfigNames::DISPLAY_AUTODIM_WAIT_SEC, "second to display sleep", "number");

  html += "</fieldset>\n";

  html += "<fieldset><legend>I2C OLED デバイス設定</legend>\n";
  html += "  ※ 接続されていない場合無視されます。画面端にゴミが表示されている、または数ドット欠けている場合は変更してください。<br>\n";
  {
    std::vector<LabelValue> choises;
    choises.push_back(LabelValue{"SSD1306", ConfigValues::OLED_SSD1306});
    choises.push_back(LabelValue{"SH1106", ConfigValues::OLED_SH1106});
    html += _create_radiobuttons(ConfigNames::OLED_TYPE, choises);
  }

  html += "</fieldset>\n";
 
  html += "<fieldset><legend>ST7789 デバイス設定</legend>\n";

  html += "  <strong>ST7789 SPI液晶の有無</strong><br>\n";
  html += "  ※ MQTTモードでは無効。<br>";
  html += "  ※ SPIピンはここでは指定できません。<br>\n";
  {
    std::vector<LabelValue> choises;
    choises.push_back(LabelValue{"使用しない", ConfigValues::ST7789_NOUSE});
    choises.push_back(LabelValue{"使用する", ConfigValues::ST7789_USE});
    html += _create_radiobuttons(ConfigNames::ST7789, choises);
  }

  html += "  <strong>ST7789 表示モード</strong><br>\n";
  {
    std::vector<LabelValue> choises;
    choises.push_back(LabelValue{"横表示モード", ConfigValues::ST7789_MODE_NORMAL});
    choises.push_back(LabelValue{"縦表示モード（デカ文字）", ConfigValues::ST7789_MODE_BIG});
    html += _create_radiobuttons(ConfigNames::ST7789_MODE, choises);
  }
  html += "</fieldset>\n";

  html += "<fieldset><legend>MH-Z19B デバイス設定</legend>\n";
  html += "  <strong>MH-Z19B CO2センサー有無（金色のセンサー）</strong><br>\n";
  {
    std::vector<LabelValue> choises;
    choises.push_back(LabelValue{"使用しない", ConfigValues::MHZ_NOUSE});
    choises.push_back(LabelValue{"使用する（UARTモード）", ConfigValues::MHZ_USE_UART});
    // choises.push_back(LabelValue{"使用する（PWMモード）", ConfigValues::MHZ_USE_PWM});

    html += _create_radiobuttons(ConfigNames::MHZ19B, choises);
  }
  
  //html += "  <br>";
  // html += "  <strong>MH-Z19BのPWMピンが接続されているGPIOピン番号</strong><br>";
  // html += "  <input type='text' name='mhz19bPwmPin' placeholder='GPIOピン番号' value='" + config.mhz19bPwmPin + "' placeholder='14'><br>";
  html += _create_input(ConfigNames::MHZ19B_PWM, "14", "", "hidden");

  html += "  <strong>MH-Z19BのUARTが接続されているGPIOピン番号</strong><br>\n";
  html += "  ※ MH-Z19Bを使用しない場合は設定不要<br>";
  html += "  ※ ESP8266では RX 14 TX 0 で固定<br>";
  html += "  ※ ボードによって使用可能なピンが異なるので動作しない場合は他のピンを試してください。<br>\n";
  html += "  RXピン&nbsp;\n";
  html += _create_input(ConfigNames::MHZ19B_RX, "16", "number");
  html += "  TXピン&nbsp;\n";
  html += _create_input(ConfigNames::MHZ19B_TX, "17", "number"); 
  html += "  <br>";

  html += "  <strong>起動時のAuto Baseline Correction</strong><br>\n";
  html += "  ※ MH-Z19Bを使用しない場合・MH-Z19BがUARTモード以外の場合は無視されます。<br>";
  {
    std::vector<LabelValue> choises;
    choises.push_back(LabelValue{"無効（標準）", ConfigValues::MHZ_ABC_OFF});
    choises.push_back(LabelValue{"有効", ConfigValues::MHZ_ABC_ON});
    html += _create_radiobuttons(ConfigNames::MHZ19B_ABC, choises);
  }

  html += "</fieldset>\n";

  html += "<fieldset><legend>MQTTモード専用設定</legend>\n";
  html += "  <strong>MQTTブローカーのIPアドレス</strong><br>\n";
  html += "  ※ ホスト名も可能ですが、mDNS(bonjour, avahi)は使用出来ません。<br>\n";
  html += _create_input(ConfigNames::MQTT_BROKER, "MQTT Broker"); 
  html += "  <br>";
  html += "  <strong>MQTT名</strong><br>\n";
  html += "  ※ クライアント名とトピック名として使用<br>";
  html += _create_input(ConfigNames::MQTT_NAME, "MQTT Client Name"); 
  html += "</fieldset>\n";

  html += "<fieldset><legend>アラート設定</legend>\n";
  html += generate_http_setup_alerts_html("気温", ConfigNames::TEMP_ALERT);
  html += generate_http_setup_alerts_html("湿度", ConfigNames::HUMI_ALERT);
  html += generate_http_setup_alerts_html("照度", ConfigNames::LUX_ALERT);
  html += generate_http_setup_alerts_html("気圧", ConfigNames::PRES_ALERT);
  html += generate_http_setup_alerts_html("CO2濃度", ConfigNames::CO2_ALERT);

  html += "</fieldset>\n";


  html += "  <br>";
  html += "  <input type='submit' value='設定する'><br>\n";
  html += "</form>\n";
  html += "<br>";
  html += "<hr>";
  html += product_long + ", Copyright 2018-2021 Yakumo Saki / ziomatrix.org.";
  html += "</body>\n";
  html += "</html>\n";

  return html;
}
