#include <Arduino.h>

/* 機種依存部分 */

/**
 * SETUP GET / 
 */
String http_setup_post_root_content();

/**
 * SETUP POST / 
 */
String http_setup_get_root_content();

/* 以下は共通 */

/**
 * 初期化
 */
void setup_http_setup();

/**
 * ループ
 */
void loop_http_setup();
