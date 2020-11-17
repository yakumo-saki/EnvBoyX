/*
 * http_normal.cpp 非機種依存部分
 */
String http_normal_not_found_html();

String http_normal_data_json();

String http_normal_ping_json();

/*
 * http_normal_board.cpp 機種依存部分
 */

/**
 * 初期化
 */
void http_setup_normal();

/**
 * ループ
 */
void http_loop_normal();