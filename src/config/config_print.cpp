#include <Arduino.h>
#include <LittleFS.h>

#include <ArduinoJson.h>

#include "log.h"
#include "global.h"
#include "structs.h"
#include "utils.h"

#include "ConfigClass.h"
#include "config_names.h"

void alerts_to_log_string(const String &alertType) {
  cfglog("    WARNING1 '" + config->get(Config::getAlertKey(alertType, ConfigNames::ALERT_WARN1_LO)) 
         + "' ~ '" + config->get(Config::getAlertKey(alertType, ConfigNames::ALERT_WARN1_HI)) + "'");
  cfglog("    CAUTION1 '" + config->get(Config::getAlertKey(alertType, ConfigNames::ALERT_CAUTION1_LO)) 
         + "' ~ '" + config->get(Config::getAlertKey(alertType, ConfigNames::ALERT_CAUTION1_HI)) + "'");
  cfglog("    CAUTION2 '" + config->get(Config::getAlertKey(alertType, ConfigNames::ALERT_CAUTION2_LO)) 
         + "' ~ '" + config->get(Config::getAlertKey(alertType, ConfigNames::ALERT_CAUTION2_HI)) + "'");
  cfglog("    WARNING2 '" + config->get(Config::getAlertKey(alertType, ConfigNames::ALERT_WARN2_LO))
         + "' ~ '" + config->get(Config::getAlertKey(alertType, ConfigNames::ALERT_WARN2_HI)) + "'");
}

void print_config() {

  cfglog("SSID: " + config->get(ConfigNames::SSID));
  cfglog("PASS: " + config->get(ConfigNames::PASSWORD));
  cfglog("mDNS: " + config->get(ConfigNames::MDNS));
  cfglog("opMode: " + config->get(ConfigNames::OPMODE));
  cfglog(F("DISPLAY:"));
  cfglog("   Flip: " + config->get(ConfigNames::DISPLAY_FLIP));
  cfglog("   Brightness: " + config->get(ConfigNames::DISPLAY_BRIGHTNESS));
  cfglog("   Wait for reconfigure: " + config->get(ConfigNames::DISPLAY_RECONFIG));
  cfglog("I2C OLED TYPE: " + config->get(ConfigNames::OLED_TYPE));
  cfglog("ST7789: " + config->get(ConfigNames::ST7789));
  cfglog("   MODE: " + config->get(ConfigNames::ST7789_MODE));
  cfglog("use MHZ19B: " + config->get(ConfigNames::MHZ19B));
  cfglog("   PWM PIN: " + config->get(ConfigNames::MHZ19B_PWM));
  cfglog("   RX  PIN: " + config->get(ConfigNames::MHZ19B_RX));
  cfglog("   TX  PIN: " + config->get(ConfigNames::MHZ19B_TX));
  cfglog("   ABC    : " + config->get(ConfigNames::MHZ19B_ABC));
  cfglog("MQTT Broker: " + config->get(ConfigNames::MQTT_BROKER));
  cfglog("MQTT Name  : " + config->get(ConfigNames::MQTT_NAME));
  cfglog(F("Alerts:"));
  
  cfglog(F("  Temperature:"));
  alerts_to_log_string(ConfigNames::TEMP_ALERT);
  
  cfglog(F("  Humidity   :"));
  alerts_to_log_string(ConfigNames::HUMI_ALERT);

  cfglog(F("  Pressure   :"));
  alerts_to_log_string(ConfigNames::PRES_ALERT);  

  cfglog(F("  Luminous   :"));
  alerts_to_log_string(ConfigNames::LUX_ALERT);

  cfglog(F("  CO2        :"));
  alerts_to_log_string(ConfigNames::CO2_ALERT);
}
