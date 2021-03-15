#ifdef ESP32

#include <Arduino.h>

#include "FS.h"
#include <SPIFFS.h>

#include "log.h"
#include "global.h"
#include "config.h"

/**
 * デバッグ用 ファイル一覧の表示
 */
void list_dir()
{
  cfglog(F(">>> SPIFFS directory listing"));
 
  File root = SPIFFS.open("/");
  if (!root) {
      cfglog(F("- failed to open directory"));
      return;
  }
  if (!root.isDirectory()){
      cfglog(F(" - not a directory"));
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

  cfglog(F("<<< End listing"));
}

/**
 * config済みフラグファイルを作成する
 */
void create_configure_flag_file()
{
  File f2 = SPIFFS.open(ConfigValues::configured_file, "w");
  f2.println("ok");
  f2.close();
  cfglog(F("configured file created."));
}

/**
 * config済みフラグファイルを削除する
 */
void remove_configure_flag_file()
{
  SPIFFS.remove(ConfigValues::configured_file);
  cfglog(ConfigValues::configured_file + " removed.");
}

/**
 * グローバル変数→設定ファイルに保存
 * 設定済みフラグファイルも作成される
 */
void save_config()
{
  trim_config();

  // 設定ファイル
  File f = SPIFFS.open(ConfigValues::settings, "w");
  write_config_file(f);
  f.close();

  create_configure_flag_file();
}

/**
 * 設定読込
 */
bool read_config()
{
  File f = SPIFFS.open(ConfigValues::settings, "r");
  cfglog(ConfigValues::settings + " filesize = " + String(f.size()));

  bool ret = read_config_file(f, false);
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

  if (!SPIFFS.exists(ConfigValues::settings)) {
    cfglog(ConfigValues::settings + " not found.");
    SPIFFS.end();
    return false;
  } else {
    File f = SPIFFS.open(ConfigValues::settings, "r");

    cfglog(F("Reading config to checking version."));
    read_config_file(f);
    f.close();

    if (String(ConfigValues::SETTING_ID).equals(config.settingId)) {
      cfglog("SETTING_ID verified. " + config.settingId);
      return true;
    } else {
      cfglog("SETTING_ID NOT match! required:" + String(ConfigValues::SETTING_ID) + " actual:" + config.settingId);
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

  bool exist = SPIFFS.exists(ConfigValues::configured_file);

  if (!exist) {
    // reconfigure用ファイルがなければセットアップモード
    // => wait for reconfigure でリセットされたとき。
    cfglog(F("configured_file not found."));
    return false;
  } else {
    cfglog(F("configured_file found"));
  }

  return has_valid_config_file();
}


void config_setup() {
  if (!SPIFFS.begin(true)){
    cfglog(F("SPIFFS Mount Failed."));
  } else {
    cfglog(F("SPIFFS Mount success."));
  }
  list_dir();
}

#endif
