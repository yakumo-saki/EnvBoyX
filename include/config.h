#include "FS.h"

void list_dir();
void read_config();
void save_config();
void create_configure_flag_file();
void remove_configure_flag_file();
bool has_valid_config_file();
bool has_valid_config();
void config_setup();

void set_default_config_value();
void print_config();
void trim_config();

/** config値をファイルに書き出す。closeはしない */
void write_config_file(File f);

/** ファイルを読んでconfig値にセット。closeはしない */
void read_config_file(File f);