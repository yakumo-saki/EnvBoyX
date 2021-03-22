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

  cfglog(F(">>> LittleFS directory listing"));

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

  cfglog(F("<<< End listing"));
}

/**
 * config済みフラグファイルを作成する
 */
void create_configure_flag_file()
{
  File f2 = LittleFS.open(ConfigValues::configured_file, "w");
  f2.println(F("ok"));
  f2.close();
  cfglog(F("configured file created."));
}

/**
 * config済みフラグファイルを削除する
 */
void remove_configure_flag_file()
{
  LittleFS.remove(ConfigValues::configured_file);
  cfglog(F("configured file removed."));
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
  File f = LittleFS.open(ConfigValues::settings, "w");
  write_config_file(f);
  f.close();

  create_configure_flag_file();
}

/**
 * 設定読込
 */
bool read_config()
{
  File f = LittleFS.open(ConfigValues::settings, "r");
  bool ret = read_config_file(f);
  f.close();

  trim_config();
  print_config();

  return ret;
}

/**
 * configファイルが存在して、SETTING_IDが一致するかだけを判定する
 * ＝フラグファイルはみない
 */
bool has_valid_config_file() {
  LittleFS.begin();
  delay(50);

  if (!LittleFS.exists(ConfigValues::settings)) {
    cfglog(ConfigValues::settings + " not found.");
    return false;
  } else {
    File f = LittleFS.open(ConfigValues::settings, "r");
    cfglog(ConfigValues::settings + " filesize = " + String(f.size()));

    read_config_file(f);
    f.close();

    if (String(SETTING_ID).equals(config.settingId)) {
      cfglog("SETTING_ID verified. " + config.settingId);
      return true;
    } else {
      cfglog("SETTING_ID NOT match! required:" + String(SETTING_ID) + " actual:" + config.settingId);
      return false;
    }
  }

  cfglog(F("Unknown state. Assuming config not found"));
  return false;
}

/**
 * configファイルの存在とバージョン一致とconfigフラグファイルの存在をチェックする
 */
bool has_valid_config() {
  LittleFS.begin();
  delay(50);

  if (!LittleFS.exists(ConfigValues::configured_file)) {
    // reconfigure用ファイルがなければセットアップモード
    // => wait for reconfigure でリセットされたとき。
    cfglog(F("configured_file not found."));
    return false;
  } 

  return has_valid_config_file();
}

void config_factory_reset() {
  remove_configure_flag_file();

  LittleFS.remove(ConfigValues::settings);
  cfglog("Factory reset executed.");
}

void config_setup() { 
}

#endif