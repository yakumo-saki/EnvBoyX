#include <Arduino.h>
#include "FS.h"

#include <ArduinoJson.h>

#include "log.h"
#include "global.h"
#include "structs.h"

#define CONF_JSON_SIZE 2000

const String CFG_SETTING_ID = "settingId";
const String CFG_SSID = "ssid";
const String CFG_PASSWORD = "password";
const String CFG_MDNS = "mDNS";
const String CFG_OPMODE = "opMode";
const String CFG_DISPLAY_FLIP = "displayFlip";
const String CFG_DISPLAY_BRIGHTNESS = "displayBrightness";
const String CFG_OLED_TYPE = "oledType";
const String CFG_ST7789 = "st7789";
const String CFG_ST7789_MODE = "st7789Mode";
const String CFG_MHZ19B = "mhz19b";
const String CFG_MHZ19B_PWM = "mhz19bPwmPin";
const String CFG_MHZ19B_RX = "mhz19bRxPin";
const String CFG_MHZ19B_TX = "mhz19bTxPin";
const String CFG_MQTT_BROKER = "mqttBroker";
const String CFG_MQTT_NAME = "mqttName";

const String CFG_TEMP_ALERT = "tempAlerts";
const String CFG_HUMI_ALERT = "humiAlerts";
const String CFG_LUX_ALERT = "luxAlerts";
const String CFG_PRES_ALERT = "presAlerts";
const String CFG_CO2_ALERT = "co2Alerts";

const String CFG_ALERT_WARN1_LO = "warn1.L";
const String CFG_ALERT_WARN1_HI = "warn1.H";
const String CFG_ALERT_WARN2_LO = "warn2.L";
const String CFG_ALERT_WARN2_HI = "warn2.H";
const String CFG_ALERT_CAUTION1_LO = "caut1.L";
const String CFG_ALERT_CAUTION1_HI = "caut1.H";
const String CFG_ALERT_CAUTION2_LO = "caut2.L";
const String CFG_ALERT_CAUTION2_HI = "caut2.H";

/**
 * とりあえずのデフォルト値をグローバル変数にセットする
 */
void set_default_config_value()
{
  config.ssid = "";
  config.password = "";
  config.mDNS = "ebx";
  config.opMode = OPMODE_DISPLAY;
  config.mhz19b = MHZ_NOUSE;
  config.mhz19bPwmPin = "14";
  #ifdef ESP32
  config.mhz19bRxPin = "32";
  config.mhz19bTxPin = "33";
  #elif defined(ESP8266)
  config.mhz19bRxPin = "14";
  config.mhz19bTxPin = "0";
  #endif

  config.displayFlip = DISPLAY_FLIP_OFF;
  config.displayBrightness = "255";

  config.oledType = OLED_SSD1306;

  config.st7789 = ST7789_NOUSE;
  config.st7789Mode = ST7789_MODE_NORMAL;

  config.mqttBroker = "";
  config.mqttName = "";
  
  config.temperatureAlerts.warning1.low = "-99";
  config.temperatureAlerts.warning1.high = "10";
  config.temperatureAlerts.caution1.low = "10";
  config.temperatureAlerts.caution1.high = "15";
  config.temperatureAlerts.caution2.low = "250";
  config.temperatureAlerts.caution2.high = "30";
  config.temperatureAlerts.warning2.low = "30";
  config.temperatureAlerts.warning2.high = "99";

  config.humidityAlerts.warning1.low = "0";
  config.humidityAlerts.warning1.high = "20";
  config.humidityAlerts.caution1.low = "20";
  config.humidityAlerts.caution1.high = "35";
  config.humidityAlerts.caution2.low = "65";
  config.humidityAlerts.caution2.high = "75";
  config.humidityAlerts.warning2.low = "75";
  config.humidityAlerts.warning2.high = "100";

  config.pressureAlerts.warning1.low = "0";
  config.pressureAlerts.warning1.high = "995";
  config.pressureAlerts.caution1.low = "995";
  config.pressureAlerts.caution1.high = "1000";
  config.pressureAlerts.caution2.low = "3000";
  config.pressureAlerts.caution2.high = "3000";
  config.pressureAlerts.warning2.low = "1025";
  config.pressureAlerts.warning2.high = "3000";

  config.luxAlerts.warning1.low = "0";
  config.luxAlerts.warning1.high = "1";
  config.luxAlerts.caution1.low = "-1";
  config.luxAlerts.caution1.high = "-1";
  config.luxAlerts.caution2.low = "-1";
  config.luxAlerts.caution2.high = "-1";
  config.luxAlerts.warning2.low = "2000";
  config.luxAlerts.warning2.high = "99999";

  config.co2Alerts.warning1.low = "0";
  config.co2Alerts.warning1.high = "390";
  config.co2Alerts.caution1.low = "390";
  config.co2Alerts.caution1.high = "398";
  config.co2Alerts.caution2.low = "800";
  config.co2Alerts.caution2.high = "1000";
  config.co2Alerts.warning2.low = "1000";
  config.co2Alerts.warning2.high = "9999";
}

String alerts_to_log_string(const config_alert_t& alerts) {
  String log = "";
  cfglog("    WARNING1 '" + alerts.warning1.low + "' ~ '" + alerts.warning1.high + "'");
  cfglog("    CAUTION1 '" + alerts.caution1.low + "' ~ '" + alerts.caution1.high + "'");
  cfglog("    CAUTION2 '" + alerts.caution2.low + "' ~ '" + alerts.caution2.high + "'");
  cfglog("    WARNING2 '" + alerts.warning2.low + "' ~ '" + alerts.warning2.high + "'");

  return log;
}

void print_config() {
  cfglog("SSID: " + config.ssid);
  cfglog("PASS: " + config.password);
  cfglog("mDNS: " + config.mDNS);
  cfglog("opMode: " + config.opMode);
  cfglog(F("DISPLAY:"));
  cfglog("   Flip: " + config.displayFlip);
  cfglog("   Brightness: " + config.displayBrightness);
  cfglog("I2C OLED TYPE: " + config.oledType);
  cfglog("ST7789: " + config.st7789);
  cfglog("   MODE: " + config.st7789Mode);
  cfglog("use MHZ19B: " + config.mhz19b);
  cfglog("   PWM PIN: " + config.mhz19bPwmPin);
  cfglog("   RX  PIN: " + config.mhz19bRxPin);
  cfglog("   TX  PIN: " + config.mhz19bTxPin);
  cfglog("MQTT Broker: " + config.mqttBroker);
  cfglog("MQTT Name  : " + config.mqttName);
  cfglog(F("Alerts:"));
  
  cfglog(F("  Temperature:"));
  alerts_to_log_string(config.temperatureAlerts);
  
  cfglog(F("  Humidity   :"));
  alerts_to_log_string(config.humidityAlerts);

  cfglog(F("  Pressure   :"));
  alerts_to_log_string(config.pressureAlerts);  

  cfglog(F("  Luminous   :"));
  alerts_to_log_string(config.luxAlerts);

  cfglog(F("  CO2        :"));
  alerts_to_log_string(config.co2Alerts);
}

void trim_alert_range(config_alert_range_t& range) {
  range.low.trim();
  range.high.trim();
}

void trim_alerts(config_alert_t& alerts) {
  trim_alert_range(alerts.warning1);
  trim_alert_range(alerts.warning2);
  trim_alert_range(alerts.caution1);
  trim_alert_range(alerts.caution2);
}

void trim_config() {
  config.settingId.trim();
  config.ssid.trim();
  config.password.trim();
  config.mDNS.trim();
  config.opMode.trim();
  config.displayFlip.trim();
  config.displayBrightness.trim();
  config.oledType.trim();
  config.st7789.trim();
  config.st7789Mode.trim();
  config.mhz19b.trim();
  config.mhz19bPwmPin.trim();
  config.mhz19bRxPin.trim();
  config.mhz19bTxPin.trim();
  config.mqttBroker.trim();
  config.mqttName.trim();

  trim_alerts(config.temperatureAlerts);
  trim_alerts(config.humidityAlerts);
  trim_alerts(config.pressureAlerts);
  trim_alerts(config.luxAlerts);
  trim_alerts(config.co2Alerts);
}

DynamicJsonDocument alerts_to_json(const config_alert_t& alerts, String logname) {
  DynamicJsonDocument json(300);
  json[CFG_ALERT_WARN1_LO] = alerts.warning1.low;
  json[CFG_ALERT_WARN1_HI] = alerts.warning1.high;
  json[CFG_ALERT_WARN2_LO] = alerts.warning2.low;
  json[CFG_ALERT_WARN2_HI] = alerts.warning2.high;
  json[CFG_ALERT_CAUTION1_LO] = alerts.caution1.low;
  json[CFG_ALERT_CAUTION1_HI] = alerts.caution1.high;
  json[CFG_ALERT_CAUTION2_LO] = alerts.caution2.low;
  json[CFG_ALERT_CAUTION2_HI] = alerts.caution2.high;
  json.shrinkToFit();

  size_t size = measureJson(json);

  cfglog("Json alerts " + logname + " section is " + String(size) + " bytes");

  return json;
}

void write_config_file(File f) {
  
  trim_config();

  DynamicJsonDocument doc(CONF_JSON_SIZE);
  doc[CFG_SETTING_ID] = SETTING_ID;  // これから書くConfigなので必ず想定しているconfig versionを書く
  doc[CFG_SSID] = config.ssid;
  doc[CFG_PASSWORD] = config.password;
  doc[CFG_MDNS] = config.mDNS;
  doc[CFG_OPMODE] = config.opMode;
  doc[CFG_DISPLAY_FLIP] = config.displayFlip;
  doc[CFG_DISPLAY_BRIGHTNESS] = config.displayBrightness;
  doc[CFG_OLED_TYPE] = config.oledType;
  doc[CFG_ST7789] = config.st7789;
  doc[CFG_ST7789_MODE] = config.st7789Mode;
  doc[CFG_MHZ19B] = config.mhz19b;
  doc[CFG_MHZ19B_PWM] = config.mhz19bPwmPin;
  doc[CFG_MHZ19B_RX] = config.mhz19bRxPin;
  doc[CFG_MHZ19B_TX] = config.mhz19bTxPin;
  doc[CFG_MQTT_BROKER] = config.mqttBroker;
  doc[CFG_MQTT_NAME] = config.mqttName;

  doc[CFG_TEMP_ALERT] = alerts_to_json(config.temperatureAlerts, "temperature");
  doc[CFG_HUMI_ALERT] = alerts_to_json(config.humidityAlerts, "humidity");
  doc[CFG_LUX_ALERT] = alerts_to_json(config.luxAlerts, "lux");
  doc[CFG_PRES_ALERT] = alerts_to_json(config.pressureAlerts , "pressure");
  doc[CFG_CO2_ALERT] = alerts_to_json(config.co2Alerts, "co2");

  cfglog(F("Writing config"));
  size_t size = serializeJson(doc, f);
  if (size == 0) {
    cfglog(F("Failed to write to file (size = 0)"));
  } else {
    cfglog("Overall JSON is " + String(size) + " bytes");
  }
}

// 指定されたキーが存在していれば値をセットする。存在しなければセットしない
void set_config_value(String& cfg, DynamicJsonDocument &json, String key) {
  // https://arduinojson.org/v6/api/jsonobject/containskey/
  JsonVariant value = json[key];
  if (value.isNull()) {
    cfglog("Config file not contains key:" + key);
    return;
  }
  cfg = value.as<String>();
}

void set_config_value(String& cfg, DynamicJsonDocument &json, String key1, String key2) {

  JsonVariant middleObj = json[key1];

  if (middleObj.isNull()) {
    
    cfglog("Config file not contains first key:" + key1);
    return;
  } else {
    JsonVariant value = middleObj[key2];

    if (value.isNull()) {
      cfglog("Config file not contains second key:" + key1 + "->" + key2);
      return;
    }

    cfg = value.as<String>();
  }
}

void read_config_alerts(config_alert_t& alerts, DynamicJsonDocument doc, String key1) {
  set_config_value(alerts.warning1.low ,doc, key1, CFG_ALERT_WARN1_LO);
  set_config_value(alerts.warning1.high ,doc, key1, CFG_ALERT_WARN1_HI);
  set_config_value(alerts.caution1.low ,doc, key1, CFG_ALERT_CAUTION1_LO);
  set_config_value(alerts.caution1.high ,doc, key1, CFG_ALERT_CAUTION1_HI);
  set_config_value(alerts.warning2.low ,doc, key1, CFG_ALERT_WARN2_LO);
  set_config_value(alerts.warning2.high ,doc, key1, CFG_ALERT_WARN2_HI);
  set_config_value(alerts.caution2.low ,doc, key1, CFG_ALERT_CAUTION2_LO);
  set_config_value(alerts.caution2.high ,doc, key1, CFG_ALERT_CAUTION2_HI);
}

void read_config_file(File f) {

  set_default_config_value(); // とりあえずデフォルト値をロードしておく。

  DynamicJsonDocument doc(CONF_JSON_SIZE);

  cfglog(F("Json deserialize start"));

  Serial.println("");
  while(f.available()){
      Serial.write(f.read());
  }
  Serial.println("");
  f.seek(0);

  DeserializationError error = deserializeJson(doc, f);

  if (error) {
    config.settingId = "INVALID";
    cfglog(F("Failed to read file or Parse as json failed"));
    cfglog("Reason: " + String(error.c_str()));
    return;
  } else {
    cfglog(F("Json deserialize done :)"));
  }

  set_config_value(config.settingId ,doc, CFG_SETTING_ID);
  set_config_value(config.ssid ,doc, CFG_SSID);
  set_config_value(config.password ,doc , CFG_PASSWORD);
  set_config_value(config.mDNS ,doc, CFG_MDNS);
  set_config_value(config.opMode ,doc, CFG_OPMODE);
  set_config_value(config.displayFlip,doc, CFG_DISPLAY_FLIP);
  set_config_value(config.displayBrightness, doc, CFG_DISPLAY_BRIGHTNESS);
  set_config_value(config.oledType, doc, CFG_OLED_TYPE);
  set_config_value(config.st7789 ,doc, CFG_ST7789);
  set_config_value(config.st7789Mode, doc, CFG_ST7789_MODE);
  set_config_value(config.mhz19b, doc, CFG_MHZ19B);
  set_config_value(config.mhz19bPwmPin, doc, CFG_MHZ19B_PWM);
  set_config_value(config.mhz19bRxPin, doc, CFG_MHZ19B_RX);
  set_config_value(config.mhz19bTxPin, doc, CFG_MHZ19B_TX);
  set_config_value(config.mqttBroker, doc, CFG_MQTT_BROKER);
  set_config_value(config.mqttName, doc, CFG_MQTT_NAME);

  read_config_alerts(config.temperatureAlerts, doc, CFG_TEMP_ALERT);
  read_config_alerts(config.humidityAlerts, doc, CFG_HUMI_ALERT);
  read_config_alerts(config.pressureAlerts, doc, CFG_PRES_ALERT);
  read_config_alerts(config.luxAlerts, doc, CFG_LUX_ALERT);
  read_config_alerts(config.co2Alerts, doc, CFG_CO2_ALERT);
}