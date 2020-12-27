#ifdef ESP8266

#include <Arduino.h>

#include <LittleFS.h>

#include "log.h"
#include "global.h"
#include "config.h"

/**
 * デバッグ用 ファイル一覧の表示
 */
void list_dir()
{
  char cwdName[2];

  cfglog(">>> LittleFS directory listing");

  strcpy(cwdName, "/");
  Dir dir = LittleFS.openDir(cwdName);

  while (dir.next())
  {
    String fn, fs;
    fn = dir.fileName();
    // fn.remove(0, 1);
    fs = String(dir.fileSize());
    cfglog("name=" + fn + " size=" + fs);
  } // end while

  cfglog("<<< End listing");
}

/**
 * config済みフラグファイルを作成する
 */
void create_configure_flag_file()
{
  File f2 = LittleFS.open(configured_file, "w");
  f2.println("ok");
  f2.close();
  cfglog("Create " + configured_file);
}

/**
 * config済みフラグファイルを削除する
 */
void remove_configure_flag_file()
{
  LittleFS.remove(configured_file);
  cfglog(configured_file + " removed.");
}

/**
 * グローバル変数→設定ファイルに保存
 * 設定済みフラグファイルも作成される
 */
void save_config()
{
  LittleFS.begin();
  delay(50);

  trim_config();

  // 設定ファイル
  File f = LittleFS.open(settings, "w");
  f.println(String(SETTING_ID));
  f.println(config.ssid);
  f.println(config.password);
  f.println(config.mDNS);
  f.println(config.opMode);
  f.println(config.st7789);
  f.println(config.use_mhz19b);
  f.println(config.mhz19b_pwmpin);
  f.println(config.mhz19b_rxpin);
  f.println(config.mhz19b_txpin);
  f.println(config.mqttBroker);
  f.println(config.mqttName);
  f.close();

  create_configure_flag_file();
}

/**
 * 設定読込
 */
void read_config()
{
  File f = LittleFS.open(settings, "r");
  String settingId = f.readStringUntil('\n'); // 使わない
  config.ssid = f.readStringUntil('\n');
  config.password = f.readStringUntil('\n');
  config.mDNS = f.readStringUntil('\n');
  config.opMode = f.readStringUntil('\n');
  config.st7789 = f.readStringUntil('\n');
  config.use_mhz19b = f.readStringUntil('\n');
  config.mhz19b_pwmpin = f.readStringUntil('\n');
  config.mhz19b_rxpin = f.readStringUntil('\n');
  config.mhz19b_txpin = f.readStringUntil('\n');
  config.mqttBroker = f.readStringUntil('\n');
  config.mqttName = f.readStringUntil('\n');
  f.close();

  trim_config();
  print_config();
}

/**
 * configファイルが存在して、SETTING_IDが一致するかだけを判定する
 * ＝フラグファイルはみない
 */
bool has_valid_config_file() {
  LittleFS.begin();
  delay(50);

  if (!LittleFS.exists(settings)) {
    cfglog(settings + " not found.");
    return false;
  } else {
    File f = LittleFS.open(settings, "r");
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
  LittleFS.begin();
  delay(50);

  if (!LittleFS.exists(configured_file)) {
    // reconfigure用ファイルがなければセットアップモード
    // => wait for reconfigure でリセットされたとき。
    cfglog("configured_file not found.");
    return false;
  } 

  return has_valid_config_file();
}


void config_setup() { 
}

#endif