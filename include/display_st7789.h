#include <Arduino.h>

// 使える使えないの判断フラグ ST7789_SUPPORTED -> define.cpp
// このフラグで呼び出される実態が空っぽなのと実装されているものに分かれる。

/** ST7789V セットアップモード  */
void disp_st7789_setup_startup_screen(String ipAddr);

void disp_st7789_wifi_info(String ip, String mDNS);
void disp_st7789_normal_startup_screen(String product_long);
void disp_st7789_wait_for_reconfig_init();
void disp_st7789_wait_for_reconfig_bar(int now, const int max);
void disp_st7789_wifi_starting();
void disp_st7789_all_initialize_complete(String ip, String mdns);
void disp_st7789_sensor_value(disp_values_t values, value_alerts_t alerts);
void disp_st7789_power_off();

/** 各種メッセージ表示 */
void disp_st7789_message(bool isError, String msg1, String msg2, String msg3, String msg4);

void disp_st7789_wifi_error();

String disp_st7789_set_brightness(int brightness);

/**
 * 液晶のON/OFFを設定する
 * @param poweron true = ONにする false = offにする
 */
void disp_st7789_set_power(bool poweron);

void setup_disp_st7789();
