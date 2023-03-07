#include <Arduino.h>
#include "config_names.h"

// config.html上の name （input要素の) を列挙する

const String ConfigNames::SETTING_ID = "settingId";
const String ConfigNames::SSID = "ssid";
const String ConfigNames::PASSWORD = "password";
const String ConfigNames::MDNS = "mDNS";
const String ConfigNames::OPMODE = "opMode";

const String ConfigNames::DISPLAY_FLIP = "displayFlip";
const String ConfigNames::DISPLAY_BRIGHTNESS = "displayBrightness";

/** NTP */
const String ConfigNames::NTP = "ntp"; // v47
const String ConfigNames::NTP_ADDRESS = "ntpAddress"; // v47
const String ConfigNames::NTP_TIMEZONE = "ntpTimezone";            // v47
const String ConfigNames::NTP_AUTO_DIMMER = "ntpAutoDimmer"; // v47
const String ConfigNames::NTP_AUTO_DIMMER_START = "ntpAutoDimmerStart"; // v47
const String ConfigNames::NTP_AUTO_DIMMER_END = "ntpAutoDimmerEnd"; // v47

/** Show and wait for reconfig screen on boot */
const String ConfigNames::DISPLAY_RECONFIG = "displayWaitForReconfigure";

const String ConfigNames::DISPLAY_AUTODIM_LUX = "displayAutoDimLux"; // v45
const String ConfigNames::DISPLAY_AUTODIM_WAIT_SEC = "displayAutoDimWaitSec"; // v45

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

const String ConfigNames::TEMP_ADJ = "adjustTemperature";  // v48 TEMP_ADJUSTにしたかったがMHZ19Bライブラリがdefineしている
const String ConfigNames::HUMI_ADJ = "adjustHumidity";     // v48
const String ConfigNames::LUX_ADJ = "adjustLux";           // v48
const String ConfigNames::PRES_ADJ = "adjustPressure";     // v48
const String ConfigNames::CO2_ADJ = "adjustCo2";           // v48

const String ConfigNames::TEMP_ALERT = "tempAlerts";
const String ConfigNames::HUMI_ALERT = "humiAlerts";
const String ConfigNames::LUX_ALERT = "luxAlerts";
const String ConfigNames::PRES_ALERT = "presAlerts";
const String ConfigNames::CO2_ALERT = "co2Alerts";

const String ConfigNames::ALERT_WARN1 = "warn1";
const String ConfigNames::ALERT_WARN2 = "warn2";
const String ConfigNames::ALERT_CAUTION1 = "caut1";
const String ConfigNames::ALERT_CAUTION2 = "caut2";
const String ConfigNames::ALERT_LO = "L";
const String ConfigNames::ALERT_HI = "H";

const String ConfigNames::ALERT_WARN1_LO = ConfigNames::ALERT_WARN1 + "." + ConfigNames::ALERT_LO;
const String ConfigNames::ALERT_WARN1_HI = ConfigNames::ALERT_WARN1 + "." + ConfigNames::ALERT_HI;
const String ConfigNames::ALERT_WARN2_LO = ConfigNames::ALERT_WARN2 + "." + ConfigNames::ALERT_LO;
const String ConfigNames::ALERT_WARN2_HI = ConfigNames::ALERT_WARN2 + "." + ConfigNames::ALERT_HI;
const String ConfigNames::ALERT_CAUTION1_LO = ConfigNames::ALERT_CAUTION1 + "." + ConfigNames::ALERT_LO;
const String ConfigNames::ALERT_CAUTION1_HI = ConfigNames::ALERT_CAUTION1 + "." + ConfigNames::ALERT_HI;
const String ConfigNames::ALERT_CAUTION2_LO = ConfigNames::ALERT_CAUTION2 + "." + ConfigNames::ALERT_LO;
const String ConfigNames::ALERT_CAUTION2_HI = ConfigNames::ALERT_CAUTION2 + "." + ConfigNames::ALERT_HI;
