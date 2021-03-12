#include <Arduino.h>

#include "log.h"
#include "global.h"
#include "config.h"

String _alerts_to_backup_text(config_alert_t& alerts, String keyPrefix) {
  String ret = "";
  ret +=       keyPrefix + ".warning1.low=" + alerts.warning1.low;
  ret += "&" + keyPrefix + ".warning1.high" + "=" + alerts.warning1.high;
  ret += "&" + keyPrefix + ".warning2.low"  + "=" + alerts.warning2.low;
  ret += "&" + keyPrefix + ".warning2.high" + "=" + alerts.warning2.high;

  ret += "&" + keyPrefix + ".caution1.low"  + "=" + alerts.caution1.low;
  ret += "&" + keyPrefix + ".caution1.high" + "=" + alerts.caution1.high;
  ret += "&" + keyPrefix + ".caution2.low"  + "=" + alerts.caution2.low;
  ret += "&" + keyPrefix + ".caution2.high" + "=" + alerts.caution2.high;
  return ret;
}

String http_api_backup_config() {
  String ret = "curl -X POST ";
  ret += "-d \"";
  
  ret += CFG_MDNS + "=" + config.mDNS;
  // ret += "&" + CFG_SSID + "=" + config.ssid; // blocked running change
  // ret += "&" + CFG_OPMODE + "=" + config.opMode; // blocked running change
  ret += "&" + CFG_DISPLAY_FLIP + "=" + config.displayFlip;
  ret += "&" + CFG_DISPLAY_BRIGHTNESS + "=" + config.displayBrightness;
  ret += "&" + CFG_OLED_TYPE + "=" + config.oledType;

  ret += "&" + CFG_ST7789 + "=" + config.st7789;
  ret += "&" + CFG_ST7789_MODE + "=" + config.st7789Mode;

  ret += "&" + CFG_MHZ19B + "=" + config.mhz19b;
  ret += "&" + CFG_MHZ19B_PWM + "=" + config.mhz19bPwmPin;
  ret += "&" + CFG_MHZ19B_RX + "=" + config.mhz19bRxPin;
  ret += "&" + CFG_MHZ19B_TX + "=" + config.mhz19bTxPin;

  ret += "&" + CFG_MQTT_BROKER + "=" + config.mqttBroker;
  ret += "&" + CFG_MQTT_NAME + "=" + config.mqttName;

  ret += "&" + CFG_TEMP_ALERT + "=" + config.mqttName;

  ret += "&" + _alerts_to_backup_text(config.temperatureAlerts, CFG_TEMP_ALERT);
  ret += "&" + _alerts_to_backup_text(config.humidityAlerts, CFG_HUMI_ALERT);
  ret += "&" + _alerts_to_backup_text(config.luxAlerts, CFG_LUX_ALERT);
  ret += "&" + _alerts_to_backup_text(config.pressureAlerts, CFG_PRES_ALERT);
  ret += "&" + _alerts_to_backup_text(config.co2Alerts, CFG_CO2_ALERT);

  ret += "\"";
  ret += " http://" + config.mDNS + ".local/config";

  return ret;
}
