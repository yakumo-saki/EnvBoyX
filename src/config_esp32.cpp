#ifdef ESP32

#include <Arduino.h>

#include "FS.h"
#include <LITTLEFS.h>

#include "log.h"
#include "global.h"
#include "config.h"

#define FORMAT_LITTLEFS_IF_FAILED true

/**
 * デバッグ用 ファイル一覧の表示
 */
void list_dir()
{
  cfglog(F(">>> LITTLEFS directory listing"));

  File root = LITTLEFS.open("/");
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
  File f2 = LITTLEFS.open(configured_file, "w");
  f2.println("ok");
  f2.close();
  cfglog(F("configured file created."));
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
  trim_config();

  // 設定ファイル
  File f = LITTLEFS.open(settings, "w");
  write_config_file(f);
  f.close();

  create_configure_flag_file();
}

/**
 * 設定読込
 */
void read_config()
{
  File f = LITTLEFS.open(settings, "r");
  cfglog(settings + " filesize = " + String(f.size()));

  read_config_file(f);
  f.close();

  trim_config();
  print_config();
}

/**
 * configファイルが存在して、SETTING_IDが一致するかだけを判定する
 * ＝フラグファイルはみない
 */
bool has_valid_config_file() {

  if (!LITTLEFS.exists(settings)) {
    cfglog(settings + " not found.");
    LITTLEFS.end();
    return false;
  } else {
    File f = LITTLEFS.open(settings, "r");

    cfglog(F("Reading config to checking version."));
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

  cfglog("Unknown state. Assuming config not found");
  return false;
}

/**
 * configファイルの存在とバージョン一致とconfigフラグファイルの存在をチェックする
 */
bool has_valid_config() {

  bool exist = LITTLEFS.exists(configured_file);

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
  if (!LITTLEFS.begin(FORMAT_LITTLEFS_IF_FAILED)){
    cfglog(F("LITTLEFS Mount Failed."));
  } else {
    cfglog(F("LITTLEFS Mount success."));
  }
  list_dir();
}

#endif
