#include <Arduino.h>

#include "log.h"
#include "global.h"
#include "config.h"

String _alerts_to_backup_text(config_alert_t& alerts, String keyPrefix) {
  String ret = "";
  ret +=       keyPrefix + "." + ConfigNames::ALERT_WARN1_LO +"=" + alerts.warning1.low;
  ret += "&" + keyPrefix + "." + ConfigNames::ALERT_WARN1_HI + "=" + alerts.warning1.high;
  ret += "&" + keyPrefix + "." + ConfigNames::ALERT_WARN2_LO + "=" + alerts.warning2.low;
  ret += "&" + keyPrefix + "." + ConfigNames::ALERT_WARN2_HI + "=" + alerts.warning2.high;

  ret += "&" + keyPrefix + "." + ConfigNames::ALERT_CAUTION1_LO + "=" + alerts.caution1.low;
  ret += "&" + keyPrefix + "." + ConfigNames::ALERT_CAUTION1_HI + "=" + alerts.caution1.high;
  ret += "&" + keyPrefix + "." + ConfigNames::ALERT_CAUTION2_LO + "=" + alerts.caution2.low;
  ret += "&" + keyPrefix + "." + ConfigNames::ALERT_CAUTION2_HI + "=" + alerts.caution2.high;
  return ret;
}

String http_api_backup_config() {
  String ret = "curl -X POST ";
  ret += "-d \"";
  
  ret += ConfigNames::MDNS + "=" + config.mDNS;
  // ret += "&" + ConfigNames::SSID + "=" + config.ssid; // blocked running change
  // ret += "&" + ConfigNames::OPMODE + "=" + config.opMode; // blocked running change
  ret += "&" + ConfigNames::DISPLAY_FLIP + "=" + config.displayFlip;
  ret += "&" + ConfigNames::DISPLAY_BRIGHTNESS + "=" + config.displayBrightness;
  ret += "&" + ConfigNames::DISPLAY_RECONFIG + "=" + config.displaySkipReconfigure;

  ret += "&" + ConfigNames::OLED_TYPE + "=" + config.oledType;

  ret += "&" + ConfigNames::ST7789 + "=" + config.st7789;
  ret += "&" + ConfigNames::ST7789_MODE + "=" + config.st7789Mode;

  ret += "&" + ConfigNames::MHZ19B + "=" + config.mhz19b;
  ret += "&" + ConfigNames::MHZ19B_PWM + "=" + config.mhz19bPwmPin;
  ret += "&" + ConfigNames::MHZ19B_RX + "=" + config.mhz19bRxPin;
  ret += "&" + ConfigNames::MHZ19B_TX + "=" + config.mhz19bTxPin;
  ret += "&" + ConfigNames::MHZ19B_ABC + "=" + config.mhz19bABC;

  ret += "&" + ConfigNames::MQTT_BROKER + "=" + config.mqttBroker;
  ret += "&" + ConfigNames::MQTT_NAME + "=" + config.mqttName;

  ret += "&" + _alerts_to_backup_text(config.temperatureAlerts, ConfigNames::TEMP_ALERT);
  ret += "&" + _alerts_to_backup_text(config.humidityAlerts, ConfigNames::HUMI_ALERT);
  ret += "&" + _alerts_to_backup_text(config.luxAlerts, ConfigNames::LUX_ALERT);
  ret += "&" + _alerts_to_backup_text(config.pressureAlerts, ConfigNames::PRES_ALERT);
  ret += "&" + _alerts_to_backup_text(config.co2Alerts, ConfigNames::CO2_ALERT);

  ret += "\"";
  ret += " http://" + config.mDNS + ".local/config";

  return ret;
}
