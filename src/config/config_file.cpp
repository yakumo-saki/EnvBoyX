#include <Arduino.h>

#include <LittleFS.h>

#include "log.h"
#include "halt.h"
#include "global.h"
#include "config/config.h"

/**
 * config済みフラグファイルを作成する
 */
void create_configure_flag_file()
{
  
  File f2 = LittleFS.open(ConfigValues::configured_file, "w");
  f2.println("ok");
  f2.close();
  cfglog(F("configured file created."));
}

/**
 * config済みフラグファイルを削除する
 */
void remove_configure_flag_file()
{
  LittleFS.remove(ConfigValues::configured_file);
  cfglog(ConfigValues::configured_file + " removed.");
}

/**
 * グローバル変数→設定ファイルに保存
 * 設定済みフラグファイルも作成される
 */
void save_config()
{
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
  cfglog(ConfigValues::settings + " filesize = " + String(f.size()));

  bool ret = read_config_file(f, false);
  f.close();

  return ret;
}

/**
 * configファイルが存在して、SETTING_IDが一致するかだけを判定する
 * ＝フラグファイルはみない
 */
CFG_VALIDATE_RESULT has_valid_config_file() {
  String settingId = "";
  if (!LittleFS.exists(ConfigValues::settings)) {
    cfglog(ConfigValues::settings + " not found.");
    return CFG_VALIDATE_RESULT::NOT_FOUND;
  } else {
    File f = LittleFS.open(ConfigValues::settings, "r");

    cfglog(F("Reading config to checking version."));
    settingId = read_config_setting_id(f);
    f.close();

    if (settingId.equals(CFG_VERSION_INVALID)) {
      cfglog("SETTING_ID not found. INVALID config.");
      return CFG_VALIDATE_RESULT::ERROR;
    } else if (String(SETTING_ID).equals(settingId)) {
      cfglog("SETTING_ID verified. " + settingId);
      return CFG_VALIDATE_RESULT::VALID;
    } else {
      cfglog("SETTING_ID NOT match! required:" + String(SETTING_ID) + " actual:" + settingId);
      return CFG_VALIDATE_RESULT::NEED_UPGRADE;
    }
  }

  cfglog("Unknown state. Assuming config not found. " + settingId);
  return CFG_VALIDATE_RESULT::ERROR;
}

/**
 * configとconfigフラグファイルの存在をチェックする
 */
bool has_configured_file() {

  bool exist = LittleFS.exists(ConfigValues::configured_file);

  if (!exist) {
    // reconfigure用ファイルがなければセットアップモード
    // => wait for reconfigure でリセットされたとき。
    cfglog(F("configured_file not found."));
    return false;
  } 

  cfglog(F("configured_file found"));
  return true;
}

/** ファイルシステムのマウント */
void config_setup() {

  if (!LittleFS.begin()) {
    cfglog(F("LittleFS Mount Failed. Try formatting."));
    LittleFS.format();
    if (LittleFS.begin()) {
      cfglog(F("LittleFS format completed."));    
    } else {
      cfglog(F("LittleFS Mount Failed. Halt"));
      halt("LittleFS","FAIL", "FORMAT");
    }
  } else {
    cfglog(F("LittleFS Mount success."));
  }
  list_dir();
}


void config_factory_reset() {
  remove_configure_flag_file();

  LittleFS.remove(ConfigValues::settings);
  cfglog("Factory reset executed.");
}

