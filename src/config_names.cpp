#include <Arduino.h>
#include "config_names.h"

const String ConfigNames::SETTING_ID = "settingId";
const String ConfigNames::SSID = "ssid";
const String ConfigNames::PASSWORD = "password";
const String ConfigNames::MDNS = "mDNS";
const String ConfigNames::OPMODE = "opMode";

const String ConfigNames::DISPLAY_FLIP = "displayFlip";
const String ConfigNames::DISPLAY_BRIGHTNESS = "displayBrightness";
const String ConfigNames::DISPLAY_RECONFIG = "displayWaitForReconfigure";

const String ConfigNames::DISPLAY_AUTODIM_LUX = "5"; // v45
const String ConfigNames::DISPLAY_AUTODIM_WAIT_SEC = "10"; // v45

const String ConfigNames::OLED_TYPE = "oledType";
const String ConfigNames::ST7789 = "st7789";
const String ConfigNames::ST7789_MODE = "st7789Mode";

const String ConfigNames::MHZ19B = "mhz19b";
const String ConfigNames::MHZ19B_PWM = "mhz19bPwmPin";
const String ConfigNames::MHZ19B_RX = "mhz19bRxPin";
const String ConfigNames::MHZ19B_TX = "mhz19bTxPin";
const String ConfigNames::MHZ19B_ABC = "mhz19bAbc";

const String ConfigNames::MQTT_BROKER = "mqttBroker";
const String ConfigNames::MQTT_NAME = "mqttName";

const String ConfigNames::TEMP_ALERT = "tempAlerts";
const String ConfigNames::HUMI_ALERT = "humiAlerts";
const String ConfigNames::LUX_ALERT = "luxAlerts";
const String ConfigNames::PRES_ALERT = "presAlerts";
const String ConfigNames::CO2_ALERT = "co2Alerts";

const String ConfigNames::ALERT_WARN1_LO = "warn1.L";
const String ConfigNames::ALERT_WARN1_HI = "warn1.H";
const String ConfigNames::ALERT_WARN2_LO = "warn2.L";
const String ConfigNames::ALERT_WARN2_HI = "warn2.H";
const String ConfigNames::ALERT_CAUTION1_LO = "caut1.L";
const String ConfigNames::ALERT_CAUTION1_HI = "caut1.H";
const String ConfigNames::ALERT_CAUTION2_LO = "caut2.L";
const String ConfigNames::ALERT_CAUTION2_HI = "caut2.H";