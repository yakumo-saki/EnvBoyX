#pragma once

#include <ArduinoJson.h>
#include "FS.h"

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

/** ボード依存：設定ファイルを削除 */
void config_factory_reset();

// --------------------------------------------------------------------
// ボード依存なし
// --------------------------------------------------------------------

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
