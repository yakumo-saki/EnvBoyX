#include <Arduino.h>

// ST7789V はセットアップモードをサポートしない
// これは接続されていない場合に初期化しようとしてしまうと何が起きるかわからないため
// void disp_st7789_setup_startup_screen(String ipAddr);


void disp_st7789_wifi_info(String ip, String mDNS);
void disp_st7789_normal_startup_screen(String product_long);
void disp_st7789_wait_for_reconfig();
void disp_st7789_wifi_starting(int wait_print_row);
void disp_st7789_sensor_value(String ip, String mdns);
void disp_st7789_power_off();

void disp_st7789_wifi_error();

String disp_st7789_set_brightness(int brightness);

/**
 * 液晶のON/OFFを設定する
 * @param poweron true = ONにする false = offにする
 */
void disp_st7789_set_power(bool poweron);

void setup_disp_st7789();
