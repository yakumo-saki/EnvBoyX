#pragma once

#include <ArduinoJson.h>
#include "FS.h"

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

// --------------------------------------------------------------------
// ボード依存あり
// --------------------------------------------------------------------

/** 
 * CONFIGをファイルから読み取る。
 * @return 読み込めたか否か。（false=ファイルがない、JSONパースエラー、必要なのに存在しない項目があった）
 */
bool read_config();
void save_config();

void list_dir();

/** config ok フラグファイルを作成する */
void create_configure_flag_file();

/** config ok フラグファイルを削除する */
void remove_configure_flag_file();
bool has_valid_config_file();
bool has_valid_config();

void config_setup();

// --------------------------------------------------------------------
// ボード依存なし
// --------------------------------------------------------------------

/** デフォルト値をセット */
void set_default_config_value();

void print_config();

void trim_config();

/** config値をファイルに書き出す。closeはしない */
void write_config_file(File f);

/** ファイルを読んでconfig値にセット。closeはしない */
bool read_config_file(File f, bool dump_config);
bool read_config_file(File f);

// APIコール用 Config->JSON 
DynamicJsonDocument create_config_json(std::vector<String> keys);

// 設定保存用 Config->JSON 
DynamicJsonDocument create_config_json_all();
