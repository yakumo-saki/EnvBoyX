#include <Arduino.h>

/* 機種依存部分 */

/**
 * SETUP GET / 
 */
void http_send_setup_post_root_html();

/**
 * SETUP POST / 
 */
void http_send_setup_get_root_html();

/* 以下は共通 */

/**
 * 初期化
 */
void setup_http_setup();

/**
 * ループ
 */
void loop_http_setup();
