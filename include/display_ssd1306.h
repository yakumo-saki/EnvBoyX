#include <Arduino.h>

void disp_ssd1306_wifi_info(String ip, String mDNS);
void disp_ssd1306_normal_startup_screen(String product_long);
void disp_ssd1306_setup_startup_screen(String ipAddr);
void disp_ssd1306_wait_for_reconfig_init();
void disp_ssd1306_wait_for_reconfig_bar(int now, const int max);
void disp_ssd1306_wifi_starting();
void disp_ssd1306_wifi_error();
void disp_ssd1306_all_initialize_complete();
void disp_ssd1306_sensor_value(disp_values_t values, value_alerts_t alerts);
void disp_ssd1306_power_off();

/** 各種メッセージ表示 */
void disp_ssd1306_message(bool isError, String msg1, String msg2, String msg3, String msg4);

void disp_ssd1306_wifi_error();

/**
 * 液晶の明るさを変更する 
 * @return 応答メッセージ
 */
String disp_ssd1306_set_brightness(int brightness);

/**
 * 液晶のON/OFFを設定する
 * @param poweron true = ONにする false = offにする
 */
void disp_ssd1306_set_power(bool poweron);

/** SSD1306が接続されているか返す */
bool has_ssd1306();

void setup_disp_ssd1306();
