#include <Arduino.h>

#ifdef ESP32
#include "FS.h"
#include <LITTLEFS.h>
#elif defined(ESP8266)
#include <LittleFS.h>
#endif

#include "log.h"
#include "global.h"

#define FORMAT_LITTLEFS_IF_FAILED true

/**
 * とりあえずのデフォルト値をグローバル変数にセットする
 */
void set_default_config_value()
{
  ssid = "";
  password = "";
  mDNS = "";
  opMode = OPMODE_DISPLAY;
  use_mhz19b = MHZ_NOUSE;
  mhz19b_pwmpin = "14";
  mqttBroker = "";
  mqttName = "";
}

/**
 * デバッグ用 ファイル一覧の表示
 */
void list_dir()
{
  cfglog(">>> LITTLEFS directory listing");

  File root = LITTLEFS.open("/");
  if (!root) {
      cfglog("- failed to open directory");
      return;
  }
  if (!root.isDirectory()){
      cfglog(" - not a directory");
      return;
  }

  File file = root.openNextFile();
  while(file){
      if(file.isDirectory()){
          cfglog("  DIR : " + String(file.name()));
      } else {
          cfglog("  FILE: " + String(file.name()) + "  SIZE: " + String(file.size()));
      }
      file = root.openNextFile();
  }

  cfglog("<<< End listing");
}

/**
 * config済みフラグファイルを作成する
 */
void create_configure_flag_file()
{
  File f2 = LITTLEFS.open(configured_file, "w");
  f2.println("ok");
  f2.close();
  cfglog("Create " + configured_file);
}

/**
 * config済みフラグファイルを削除する
 */
void remove_configure_flag_file()
{
  LITTLEFS.remove(configured_file);
  cfglog(configured_file + " removed.");
}

/**
 * グローバル変数→設定ファイルに保存
 * 設定済みフラグファイルも作成される
 */
void save_config()
{
  LITTLEFS.begin();

  // 設定ファイル
  File f = LITTLEFS.open(settings, "w");
  f.println(String(SETTING_ID));
  f.println(ssid);
  f.println(password);
  f.println(mDNS);
  f.println(opMode);
  f.println(use_mhz19b);
  f.println(mhz19b_pwmpin);
  f.println(mqttBroker);
  f.println(mqttName);
  f.close();

  create_configure_flag_file();
}

/**
 * 設定読込
 */
void read_config()
{
  File f = LITTLEFS.open(settings, "r");
  String settingId = f.readStringUntil('\n'); // 使わない
  ssid = f.readStringUntil('\n');
  password = f.readStringUntil('\n');
  mDNS = f.readStringUntil('\n');
  opMode = f.readStringUntil('\n');
  use_mhz19b = f.readStringUntil('\n');
  mhz19b_pwmpin = f.readStringUntil('\n');
  mqttBroker = f.readStringUntil('\n');
  mqttName = f.readStringUntil('\n');
  f.close();

  settingId.trim();
  ssid.trim();
  password.trim();
  mDNS.trim();
  opMode.trim();
  use_mhz19b.trim();
  mqttBroker.trim();
  mqttName.trim();

  cfglog("S-ID: " + settingId);
  cfglog("SSID: " + ssid);
  cfglog("PASS: " + password);
  cfglog("mDNS: " + mDNS);
  cfglog("opMode: " + opMode);
  cfglog("use MHZ19B: " + use_mhz19b);
  cfglog("   PWM PIN: " + mhz19b_pwmpin);
  cfglog("MQTT Broker: " + mqttBroker);
  cfglog("MQTT Name  : " + mqttName);
}

/**
 * configファイルが存在して、SETTING_IDが一致するかだけを判定する
 * ＝フラグファイルはみない
 */
bool has_valid_config_file() {
  LITTLEFS.begin();
  delay(50);

  if (!LITTLEFS.exists(settings)) {
    cfglog(settings + " not found.");
    return false;
  } else {
    File f = LITTLEFS.open(settings, "r");
    String settingId = f.readStringUntil('\n');   
    settingId.trim();
    f.close();

    if (String(SETTING_ID).equals(settingId)) {
      cfglog("SETTING_ID verified. " + settingId);
      return true;
    } else {
      cfglog("SETTING_ID NOT match!");
      return false;
    }
  }

  cfglog("Unknown state. Assuming config not found");
  return false;
}

/**
 * configファイルの存在とバージョン一致とconfigフラグファイルの存在をチェックする
 */
bool has_valid_config() {
  LITTLEFS.begin();
  delay(50);

  if (!LITTLEFS.exists(configured_file)) {
    // reconfigure用ファイルがなければセットアップモード
    // => wait for reconfigure でリセットされたとき。
    cfglog("configured_file not found.");
    return false;
  } 

  return has_valid_config_file();
}


void config_setup() {
  if (!LITTLEFS.begin(FORMAT_LITTLEFS_IF_FAILED)){
    cfglog("LITTLEFS Mount Failed.");
    return;
  }

  cfglog("LITTLEFS Mount success.");
  
}