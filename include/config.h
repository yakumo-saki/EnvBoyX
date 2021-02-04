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

void create_configure_flag_file();
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
bool read_config_file(File f);