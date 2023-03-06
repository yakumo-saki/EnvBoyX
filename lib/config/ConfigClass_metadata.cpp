#include <Arduino.h>
#include <unordered_map>

#include "ConfigClass.h"

#include "log.h"
#include "config_names.h"
#include "config_values.h"

/** 
 * Configのメタデータをセットする 
 * ここでセットしていない項目は存在しないものとして扱われる
 * 正直なところ、メモリの無駄ではあるのでメモリがカツカツになったら
 * 素朴な実装に切り替えても良いかもしれない。
 * このデータだけでざっと4KBくらい使用している。
 */
void Config::loadMetadata() {
  {
    ConfigMeta meta;
    meta.key = ConfigNames::SETTING_ID;
    meta.type = ConfigValueType::String;
    meta.flags = RunningConfigChangeFlags::BLOCKED;
    this->addMeta(meta);
  }
  {
    ConfigMeta meta;
    meta.key = ConfigNames::SSID;
    meta.type = ConfigValueType::String;
    meta.flags = RunningConfigChangeFlags::REBOOT_REQ; // v46 BLOCKED to REBOOT_REQ
    this->addMeta(meta);
  }
  {
    ConfigMeta meta;
    meta.key = ConfigNames::PASSWORD;
    meta.type = ConfigValueType::String;
    meta.flags = RunningConfigChangeFlags::REBOOT_REQ; // v46 BLOCKED to REBOOT_REQ
    this->addMeta(meta);
  }
  {
    ConfigMeta meta;
    meta.key = ConfigNames::MDNS;
    meta.type = ConfigValueType::String;
    meta.flags = RunningConfigChangeFlags::MDNS_RESTART_REQ;
    this->addMeta(meta);
  }
  {
    ConfigMeta meta;
    meta.key = ConfigNames::OPMODE;
    meta.type = ConfigValueType::Choise;
    meta.flags = RunningConfigChangeFlags::REBOOT_REQ;
    meta.validValues = std::vector<String> {ConfigValues::OPMODE_DISPLAY, ConfigValues::OPMODE_MQTT};
    this->addMeta(meta);
  }
  { // v47
    ConfigMeta meta;
    meta.key = ConfigNames::NTP;
    meta.type = ConfigValueType::Choise;
    meta.flags = RunningConfigChangeFlags::REBOOT_REQ;
    meta.validValues = std::vector<String> {ConfigValues::NTP_ON, ConfigValues::NTP_OFF};
    this->addMeta(meta);
  }
  { // v47
    ConfigMeta meta;
    meta.key = ConfigNames::NTP_ADDRESS;
    meta.type = ConfigValueType::String;
    meta.flags = RunningConfigChangeFlags::REBOOT_REQ;
    this->addMeta(meta);
  }
  { // v47
    ConfigMeta meta;
    meta.key = ConfigNames::NTP_TIMEZONE;
    meta.type = ConfigValueType::String;
    meta.flags = RunningConfigChangeFlags::REBOOT_REQ;
    this->addMeta(meta);
  }
  { // v47
    ConfigMeta meta;
    meta.key = ConfigNames::NTP_AUTO_DIMMER;
    meta.type = ConfigValueType::Choise;
    meta.flags = RunningConfigChangeFlags::OK;
    meta.validValues = std::vector<String> {ConfigValues::NTP_AUTO_DIMMER_ON, ConfigValues::NTP_AUTO_DIMMER_OFF};
    this->addMeta(meta);
  }
  { // v47
    ConfigMeta meta;
    meta.key = ConfigNames::NTP_AUTO_DIMMER_START;
    meta.type = ConfigValueType::String;
    meta.flags = RunningConfigChangeFlags::OK;
    this->addMeta(meta);
  }
  { // v47
    ConfigMeta meta;
    meta.key = ConfigNames::NTP_AUTO_DIMMER_END;
    meta.type = ConfigValueType::String;
    meta.flags = RunningConfigChangeFlags::OK;
    this->addMeta(meta);
  }
  {
    // v45: この明るさ以下がDuration秒続けば消灯
    ConfigMeta meta;
    meta.key = ConfigNames::DISPLAY_AUTODIM_LUX;
    meta.type = ConfigValueType::Integer;
    meta.flags = RunningConfigChangeFlags::REBOOT_REQ;
    this->addMeta(meta);
  }
  {
    // v45: 消灯までの時間 second
    ConfigMeta meta;
    meta.key = ConfigNames::DISPLAY_AUTODIM_WAIT_SEC;
    meta.type = ConfigValueType::Integer;
    meta.flags = RunningConfigChangeFlags::REBOOT_REQ;
    this->addMeta(meta);
  }
  {
    ConfigMeta meta;
    meta.key = ConfigNames::DISPLAY_FLIP;
    meta.type = ConfigValueType::Choise;
    meta.flags = RunningConfigChangeFlags::DISPLAY_REDRAW_REQ;
    meta.validValues = std::vector<String> {ConfigValues::DISPLAY_FLIP_OFF, ConfigValues::DISPLAY_FLIP_ON};
    this->addMeta(meta);
  }
  {
    ConfigMeta meta;
    meta.key = ConfigNames::DISPLAY_BRIGHTNESS;
    meta.type = ConfigValueType::Integer;
    meta.flags = RunningConfigChangeFlags::DISPLAY_REDRAW_REQ;
    this->addMeta(meta);
  }
  {
    ConfigMeta meta;
    meta.key = ConfigNames::DISPLAY_RECONFIG;
    meta.type = ConfigValueType::Choise;
    meta.flags = RunningConfigChangeFlags::REBOOT_REQ;
    meta.validValues = std::vector<String> {ConfigValues::DISPLAY_RECONFIG_ON, ConfigValues::DISPLAY_RECONFIG_SKIP};
    this->addMeta(meta);
  }
  {
    ConfigMeta meta;
    meta.key = ConfigNames::OLED_TYPE;
    meta.type = ConfigValueType::Choise;
    meta.flags = RunningConfigChangeFlags::REBOOT_REQ;
    meta.validValues = std::vector<String> {ConfigValues::OLED_SSD1306, ConfigValues::OLED_SH1106};
    this->addMeta(meta);
  }
  {
    ConfigMeta meta;
    meta.key = ConfigNames::ST7789;
    meta.type = ConfigValueType::Choise;
    meta.flags = RunningConfigChangeFlags::REBOOT_REQ;
    meta.validValues = std::vector<String> {ConfigValues::ST7789_NOUSE, ConfigValues::ST7789_USE};
    this->addMeta(meta);
  }
  {
    ConfigMeta meta;
    meta.key = ConfigNames::ST7789_MODE;
    meta.type = ConfigValueType::Choise;
    meta.flags = RunningConfigChangeFlags::REBOOT_REQ;
    meta.validValues = std::vector<String> {ConfigValues::ST7789_MODE_NORMAL, ConfigValues::ST7789_MODE_BIG};
    this->addMeta(meta);
  }
  {
    ConfigMeta meta;
    meta.key = ConfigNames::MHZ19B;
    meta.type = ConfigValueType::Choise;
    meta.flags = RunningConfigChangeFlags::REBOOT_REQ;
    meta.validValues = std::vector<String> {ConfigValues::MHZ_NOUSE, ConfigValues::MHZ_USE_UART, ConfigValues::MHZ_USE_PWM};
    this->addMeta(meta);
  }
  {
    ConfigMeta meta;
    meta.key = ConfigNames::MHZ19B_PWM;
    meta.type = ConfigValueType::Integer;
    meta.flags = RunningConfigChangeFlags::REBOOT_REQ;
    this->addMeta(meta);
  }
  {
    ConfigMeta meta;
    meta.key = ConfigNames::MHZ19B_ABC;
    meta.type = ConfigValueType::Choise;
    meta.flags = RunningConfigChangeFlags::REBOOT_REQ;
    meta.validValues = std::vector<String> {ConfigValues::MHZ_ABC_OFF, ConfigValues::MHZ_ABC_ON};
    this->addMeta(meta);
  }
  {
    ConfigMeta meta;
    meta.key = ConfigNames::MHZ19B_RX;
    meta.type = ConfigValueType::Integer;
    meta.flags = RunningConfigChangeFlags::REBOOT_REQ;
    this->addMeta(meta);
  }
  {
    ConfigMeta meta;
    meta.key = ConfigNames::MHZ19B_TX;
    meta.type = ConfigValueType::Integer;
    meta.flags = RunningConfigChangeFlags::REBOOT_REQ;
    this->addMeta(meta);
  }
  {
    ConfigMeta meta;
    meta.key = ConfigNames::MQTT_BROKER;
    meta.type = ConfigValueType::String;
    meta.flags = RunningConfigChangeFlags::REBOOT_REQ;
    this->addMeta(meta);
  }
  {
    ConfigMeta meta;
    meta.key = ConfigNames::MQTT_NAME;
    meta.type = ConfigValueType::String;
    meta.flags = RunningConfigChangeFlags::REBOOT_REQ;
    this->addMeta(meta);
  }
  { // v48
    ConfigMeta meta;
    meta.key = ConfigNames::TEMP_ADJ;
    meta.type = ConfigValueType::Double;
    meta.flags = RunningConfigChangeFlags::OK;
    this->addMeta(meta);
  }
  { // v48
    ConfigMeta meta;
    meta.key = ConfigNames::HUMI_ADJ;
    meta.type = ConfigValueType::Double;
    meta.flags = RunningConfigChangeFlags::OK;
    this->addMeta(meta);
  }
  { // v48
    ConfigMeta meta;
    meta.key = ConfigNames::LUX_ADJ;
    meta.type = ConfigValueType::Integer;
    meta.flags = RunningConfigChangeFlags::OK;
    this->addMeta(meta);
  }
  { // v48
    ConfigMeta meta;
    meta.key = ConfigNames::PRES_ADJ;
    meta.type = ConfigValueType::Double;
    meta.flags = RunningConfigChangeFlags::OK;
    this->addMeta(meta);
  }
  { // v48
    ConfigMeta meta;
    meta.key = ConfigNames::CO2_ADJ;
    meta.type = ConfigValueType::Integer;
    meta.flags = RunningConfigChangeFlags::OK;
    this->addMeta(meta);
  }

  this->loadMetadataAlert(ConfigNames::TEMP_ALERT);
  this->loadMetadataAlert(ConfigNames::HUMI_ALERT);
  this->loadMetadataAlert(ConfigNames::PRES_ALERT);
  this->loadMetadataAlert(ConfigNames::LUX_ALERT);
  this->loadMetadataAlert(ConfigNames::CO2_ALERT);
}

void Config::loadMetadataAlert(String alertType) {
  this->_loadMetadataAlert(alertType, ConfigNames::ALERT_WARN1_LO);
  this->_loadMetadataAlert(alertType, ConfigNames::ALERT_WARN1_HI);
  this->_loadMetadataAlert(alertType, ConfigNames::ALERT_WARN2_LO);
  this->_loadMetadataAlert(alertType, ConfigNames::ALERT_WARN2_HI);
  this->_loadMetadataAlert(alertType, ConfigNames::ALERT_CAUTION1_LO);
  this->_loadMetadataAlert(alertType, ConfigNames::ALERT_CAUTION1_HI);
  this->_loadMetadataAlert(alertType, ConfigNames::ALERT_CAUTION2_LO);
  this->_loadMetadataAlert(alertType, ConfigNames::ALERT_CAUTION2_HI);
}

void Config::_loadMetadataAlert(String alertType, String suffix) {
  ConfigMeta meta;
  meta.key = this->getAlertKey(alertType, suffix);
  meta.type = ConfigValueType::Integer;
  meta.flags = RunningConfigChangeFlags::OK;
  this->addMeta(meta);
}