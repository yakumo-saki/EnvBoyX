#pragma once

#include <ArduinoJson.h>
#include <LittleFS.h>

#include "config_names.h"
#include "config_values.h"
#include "ConfigClass.h"

extern const String CFG_VERSION_INVALID;

enum struct CFG_VALIDATE_RESULT {
  // 存在していて、バージョンも一致している
  VALID,
  // 存在しているが、バージョンが一致していない
  NEED_UPGRADE,
  // 存在していない
  NOT_FOUND,
  // 不明
  ERROR
};

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

/** configファイルの存在とバージョンをチェックする */
CFG_VALIDATE_RESULT has_valid_config_file();

bool has_configured_file();

void config_setup();

/** ボード依存：設定ファイルを削除 */
void config_factory_reset();

// --------------------------------------------------------------------
// ボード依存なし
// --------------------------------------------------------------------

void trim_config();

/** config値をファイルに書き出す。closeはしない */
void write_config_file(File f);

/** ファイルを読んでconfig値にセット。closeはしない */
bool read_config_file(File f, bool readForUpdate);

/** ファイルを読んでSETTING_IDを返す。
 * 取得できない場合はINVALIDが返る。closeはしない 
 */
String read_config_setting_id(File f);


// APIコール用 Config->JSON 
DynamicJsonDocument create_config_json(std::vector<String>& keys);

// 設定保存用 Config->JSON 
DynamicJsonDocument create_config_json_all();
