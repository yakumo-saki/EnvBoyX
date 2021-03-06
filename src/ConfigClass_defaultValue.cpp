#include <Arduino.h>
#include <unordered_map>

#include "ConfigClass.h"

#include "log.h"
#include "halt.h"
#include "config_names.h"
#include "config_values.h"

/** 
 * 初期値をセットする 
 * ここでセットしていない項目は存在しないものとして扱われる
 */
void Config::loadDefaultValue() {
  this->addConfig(ConfigNames::SETTING_ID, "");
  this->addConfig(ConfigNames::SSID, "");
  this->addConfig(ConfigNames::PASSWORD, "");

  this->addConfig(ConfigNames::MDNS, "ebx");
  this->addConfig(ConfigNames::OPMODE, ConfigValues::OPMODE_DISPLAY);

  this->addConfig(ConfigNames::DISPLAY_FLIP, ConfigValues::DISPLAY_FLIP_OFF);
  this->addConfig(ConfigNames::DISPLAY_BRIGHTNESS, "255");
  this->addConfig(ConfigNames::DISPLAY_RECONFIG, ConfigValues::DISPLAY_RECONFIG_ON);  // v44;
  this->addConfig(ConfigNames::DISPLAY_AUTODIM_LUX, "5");       // v45: この明るさ以下がDuration秒続けば消灯
  this->addConfig(ConfigNames::DISPLAY_AUTODIM_WAIT_SEC, "10"); // v45: 消灯までの時間 second

  this->addConfig(ConfigNames::OLED_TYPE, ConfigValues::OLED_SSD1306);

  this->addConfig(ConfigNames::ST7789, ConfigValues::ST7789_NOUSE);
  this->addConfig(ConfigNames::ST7789_MODE, ConfigValues::ST7789_MODE_NORMAL);

  this->addConfig(ConfigNames::MHZ19B, ConfigValues::MHZ_NOUSE);
  this->addConfig(ConfigNames::MHZ19B_PWM, "14");
  this->addConfig(ConfigNames::MHZ19B_ABC, ConfigValues::MHZ_ABC_OFF);  // v44;
#ifdef ESP32
  this->addConfig(ConfigNames::MHZ19B_RX, "32");
  this->addConfig(ConfigNames::MHZ19B_TX, "33");
#elif defined(ESP8266)
  this->addConfig(ConfigNames::MHZ19B_RX, "14");
  this->addConfig(ConfigNames::MHZ19B_TX, "0");
#endif
  
  this->addConfig(ConfigNames::MQTT_BROKER, "");
  this->addConfig(ConfigNames::MQTT_NAME, "");

  this->addConfig(ConfigNames::TEMP_ALERT, ConfigNames::ALERT_WARN1_LO, "-99");
  this->addConfig(ConfigNames::TEMP_ALERT, ConfigNames::ALERT_WARN1_HI, "10");
  this->addConfig(ConfigNames::TEMP_ALERT, ConfigNames::ALERT_WARN2_LO, "30");
  this->addConfig(ConfigNames::TEMP_ALERT, ConfigNames::ALERT_WARN2_HI, "99");
  this->addConfig(ConfigNames::TEMP_ALERT, ConfigNames::ALERT_CAUTION1_LO, "10");
  this->addConfig(ConfigNames::TEMP_ALERT, ConfigNames::ALERT_CAUTION1_HI, "15");
  this->addConfig(ConfigNames::TEMP_ALERT, ConfigNames::ALERT_CAUTION2_LO, "28");
  this->addConfig(ConfigNames::TEMP_ALERT, ConfigNames::ALERT_CAUTION2_HI, "30");

  this->addConfig(ConfigNames::HUMI_ALERT, ConfigNames::ALERT_WARN1_LO, "0");
  this->addConfig(ConfigNames::HUMI_ALERT, ConfigNames::ALERT_WARN1_HI, "20");
  this->addConfig(ConfigNames::HUMI_ALERT, ConfigNames::ALERT_WARN2_LO, "75");
  this->addConfig(ConfigNames::HUMI_ALERT, ConfigNames::ALERT_WARN2_HI, "100");
  this->addConfig(ConfigNames::HUMI_ALERT, ConfigNames::ALERT_CAUTION1_LO, "20");
  this->addConfig(ConfigNames::HUMI_ALERT, ConfigNames::ALERT_CAUTION1_HI, "35");
  this->addConfig(ConfigNames::HUMI_ALERT, ConfigNames::ALERT_CAUTION2_LO, "65");
  this->addConfig(ConfigNames::HUMI_ALERT, ConfigNames::ALERT_CAUTION2_HI, "75");

  this->addConfig(ConfigNames::PRES_ALERT, ConfigNames::ALERT_WARN1_LO, "0");
  this->addConfig(ConfigNames::PRES_ALERT, ConfigNames::ALERT_WARN1_HI, "995");
  this->addConfig(ConfigNames::PRES_ALERT, ConfigNames::ALERT_WARN2_LO, "1025");
  this->addConfig(ConfigNames::PRES_ALERT, ConfigNames::ALERT_WARN2_HI, "3000");
  this->addConfig(ConfigNames::PRES_ALERT, ConfigNames::ALERT_CAUTION1_LO, "995");
  this->addConfig(ConfigNames::PRES_ALERT, ConfigNames::ALERT_CAUTION1_HI, "1000");
  this->addConfig(ConfigNames::PRES_ALERT, ConfigNames::ALERT_CAUTION2_LO, "3000");
  this->addConfig(ConfigNames::PRES_ALERT, ConfigNames::ALERT_CAUTION2_HI, "3000");

  this->addConfig(ConfigNames::LUX_ALERT, ConfigNames::ALERT_WARN1_LO, "0");
  this->addConfig(ConfigNames::LUX_ALERT, ConfigNames::ALERT_WARN1_HI, "1");
  this->addConfig(ConfigNames::LUX_ALERT, ConfigNames::ALERT_WARN2_LO, "2000");
  this->addConfig(ConfigNames::LUX_ALERT, ConfigNames::ALERT_WARN2_HI, "99999");
  this->addConfig(ConfigNames::LUX_ALERT, ConfigNames::ALERT_CAUTION1_LO, "-1");
  this->addConfig(ConfigNames::LUX_ALERT, ConfigNames::ALERT_CAUTION1_HI, "-1");
  this->addConfig(ConfigNames::LUX_ALERT, ConfigNames::ALERT_CAUTION2_LO, "-1");
  this->addConfig(ConfigNames::LUX_ALERT, ConfigNames::ALERT_CAUTION2_HI, "-1");

  this->addConfig(ConfigNames::CO2_ALERT, ConfigNames::ALERT_WARN1_LO, "0");
  this->addConfig(ConfigNames::CO2_ALERT, ConfigNames::ALERT_WARN1_HI, "200");
  this->addConfig(ConfigNames::CO2_ALERT, ConfigNames::ALERT_WARN2_LO, "1000");
  this->addConfig(ConfigNames::CO2_ALERT, ConfigNames::ALERT_WARN2_HI, "9999");
  this->addConfig(ConfigNames::CO2_ALERT, ConfigNames::ALERT_CAUTION1_LO, "200");
  this->addConfig(ConfigNames::CO2_ALERT, ConfigNames::ALERT_CAUTION1_HI, "300");
  this->addConfig(ConfigNames::CO2_ALERT, ConfigNames::ALERT_CAUTION2_LO, "800");
  this->addConfig(ConfigNames::CO2_ALERT, ConfigNames::ALERT_CAUTION2_HI, "1000");
}
