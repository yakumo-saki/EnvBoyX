#include <Arduino.h>

#include "log.h"
#include "global.h"
#include "config.h"

#include "http_setup.h"

#include "embed/style_css.h"

#ifdef ESP32
#include <WebServer.h>
extern WebServer server;
#endif

#ifdef ESP8266
#include <ESP8266WebServer.h>
extern ESP8266WebServer server;
#endif

/**
 * GET 設定画面
 */
void handle_get_root() {

  String html = http_setup_get_root_content();

  server.send(200, MimeType::HTML, html);
}

void alerts_to_config(config_alert_t& alerts, String prefix) {
  alerts.caution1.low = server.arg(prefix  + "." + ConfigNames::ALERT_CAUTION1_LO);
  alerts.caution1.high = server.arg(prefix + "." + ConfigNames::ALERT_CAUTION1_HI);
  alerts.caution2.low = server.arg(prefix  + "." + ConfigNames::ALERT_CAUTION2_LO);
  alerts.caution2.high = server.arg(prefix + "." + ConfigNames::ALERT_CAUTION2_HI);

  alerts.warning1.low = server.arg(prefix  + "." + ConfigNames::ALERT_WARN1_LO);
  alerts.warning1.high = server.arg(prefix + "." + ConfigNames::ALERT_WARN1_HI);
  alerts.warning2.low = server.arg(prefix  + "." + ConfigNames::ALERT_WARN2_LO);
  alerts.warning2.high = server.arg(prefix + "." + ConfigNames::ALERT_WARN2_HI);
}

/**
 * Post 設定 ( config の post。 ファイルに設定を保存）
 */
void handle_post_root() {
  
  config.ssid = server.arg(ConfigNames::SSID);
  config.password = server.arg(ConfigNames::PASSWORD);
  config.mDNS = server.arg(ConfigNames::MDNS);
  config.opMode = server.arg(ConfigNames::OPMODE);

  config.displayFlip = server.arg(ConfigNames::DISPLAY_FLIP);
  config.displayBrightness = server.arg(ConfigNames::DISPLAY_BRIGHTNESS);
  config.displayWaitForReconfigure = server.arg(ConfigNames::DISPLAY_RECONFIG);

  config.oledType = server.arg(ConfigNames::OLED_TYPE);

  config.st7789 = server.arg(ConfigNames::ST7789);
  config.st7789Mode = server.arg(ConfigNames::ST7789_MODE);

  config.mhz19b = server.arg(ConfigNames::MHZ19B);
  config.mhz19bPwmPin = server.arg(ConfigNames::MHZ19B_PWM);
  config.mhz19bRxPin = server.arg(ConfigNames::MHZ19B_RX);
  config.mhz19bTxPin = server.arg(ConfigNames::MHZ19B_TX);;
  config.mhz19bABC = server.arg(ConfigNames::MHZ19B_ABC);

  config.mqttBroker = server.arg(ConfigNames::MQTT_BROKER);
  config.mqttName = server.arg(ConfigNames::MQTT_NAME);

  alerts_to_config(config.temperatureAlerts, ConfigNames::TEMP_ALERT);
  alerts_to_config(config.humidityAlerts, ConfigNames::HUMI_ALERT);
  alerts_to_config(config.luxAlerts, ConfigNames::LUX_ALERT);
  alerts_to_config(config.pressureAlerts, ConfigNames::PRES_ALERT);
  alerts_to_config(config.co2Alerts, ConfigNames::CO2_ALERT);

  trim_config();
  String html = http_setup_post_root_content();

  server.send(200, MimeType::HTML, html);
}

void handle_get_style_css() {
  httplog(F("style.css accessed"));
  server.send(200, MimeType::CSS, STYLE_CSS);
}


/**
 * 初期化(設定用Webサーバモード)
 */
void setup_http_setup() {
  httplog(F("HTTP web server initializing"));
  server.on("/", HTTP_GET, handle_get_root);
  server.on("/", HTTP_POST, handle_post_root);
  server.on("/style.css", HTTP_GET, handle_get_style_css);
  server.begin();
  httplog(F("HTTP web server initialized"));
}

void loop_http_setup() {
  server.handleClient();
}
