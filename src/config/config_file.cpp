#include <Arduino.h>

#include "FS.h"
#include <SPIFFS.h>

#include "log.h"
#include "global.h"
#include "config/config.h"

#ifdef ESP32
#define EBXFS SPIFFS
#endif

#ifdef ESP8266
#define EBXFS LITTLEFS
#endif

/**
 * デバッグ用 ファイル一覧の表示
 */
void list_dir()
{
  cfglog(F(">>> SPIFFS directory listing"));
 
  File root = EBXFS.open("/");
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
  
  File f2 = EBXFS.open(ConfigValues::configured_file, FILE_WRITE);
  f2.println("ok");
  f2.close();
  cfglog(F("configured file created."));
}

/**
 * config済みフラグファイルを削除する
 */
void remove_configure_flag_file()
{
  EBXFS.remove(ConfigValues::configured_file);
  cfglog(ConfigValues::configured_file + " removed.");
}

/**
 * グローバル変数→設定ファイルに保存
 * 設定済みフラグファイルも作成される
 */
void save_config()
{
  // 設定ファイル
  File f = EBXFS.open(ConfigValues::settings, "w");
  write_config_file(f);
  f.close();

  create_configure_flag_file();
}

/**
 * 設定読込
 */
bool read_config()
{
  File f = EBXFS.open(ConfigValues::settings, "r");
  cfglog(ConfigValues::settings + " filesize = " + String(f.size()));

  bool ret = read_config_file(f, false);
  f.close();

  print_config();
  return ret;
}

/**
 * configファイルが存在して、SETTING_IDが一致するかだけを判定する
 * ＝フラグファイルはみない
 */
CFG_VALIDATE_RESULT has_valid_config_file() {
  String settingId = "";
  if (!EBXFS.exists(ConfigValues::settings)) {
    cfglog(ConfigValues::settings + " not found.");
    SPIFFS.end();
    return CFG_VALIDATE_RESULT::NOT_FOUND;
  } else {
    File f = SPIFFS.open(ConfigValues::settings, "r");

    cfglog(F("Reading config to checking version."));
    settingId = read_config_setting_id(f);
    f.close();

    if (String(SETTING_ID).equals(settingId)) {
      cfglog("SETTING_ID verified. " + settingId);
      return CFG_VALIDATE_RESULT::VALID;
    } else {
      cfglog("SETTING_ID NOT match! required:" + String(SETTING_ID) + " actual:" + settingId);
      return CFG_VALIDATE_RESULT::NEED_UPGRADE;
    }
  }

  cfglog("Unknown state. Assuming config not found");
  return CFG_VALIDATE_RESULT::ERROR;
}

/**
 * configとconfigフラグファイルの存在をチェックする
 */
bool has_configured_file() {

  bool exist = EBXFS.exists(ConfigValues::configured_file);

  if (!exist) {
    // reconfigure用ファイルがなければセットアップモード
    // => wait for reconfigure でリセットされたとき。
    cfglog(F("configured_file not found."));
    return false;
  } 

  cfglog(F("configured_file found"));
  return true;
}


void config_setup() {
  if (!SPIFFS.begin(true)){
    cfglog(F("SPIFFS Mount Failed."));
  } else {
    cfglog(F("SPIFFS Mount success."));
  }
  list_dir();
}

void config_factory_reset() {
  remove_configure_flag_file();

  EBXFS.remove(ConfigValues::settings);
  cfglog("Factory reset executed.");
}

