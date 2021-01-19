#ifndef ESP32

// ST7789 非サポートのボード用。呼ばれることはない。コンパイルを通すためだけのもの

#include <Arduino.h>
#include "global.h"
#include "structs.h"

void disp_st7789_wifi_info(String ip, String mDNS) {}
void disp_st7789_normal_startup_screen(String product_long) {}
void disp_st7789_wait_for_reconfig_init() {}
void disp_st7789_wait_for_reconfig_bar(int now, const int max) {}
void disp_st7789_wifi_starting(int wait_print_row) {}
void disp_st7789_all_initialize_complete(String ip, String mdns) {};
void disp_st7789_sensor_value(disp_values_t new_values, disp_values_t last_values) {};
void disp_st7789_power_off() {};

void disp_st7789_wifi_error() {};

String disp_st7789_set_brightness(int brightness) { return "Not Supported"; }

/**
 * 液晶のON/OFFを設定する
 * @param poweron true = ONにする false = offにする
 */
void disp_st7789_set_power(bool poweron) {};

void setup_disp_st7789() {};

#endif