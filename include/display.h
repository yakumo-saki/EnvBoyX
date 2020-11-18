#include <Arduino.h>

void disp_wifi_info(String ip, String mDNS);
void disp_normal_startup_screen(String product_long);
void disp_setup_startup_screen(String ipAddr);
void disp_wait_for_reconfig();
void disp_wifi_starting(int wait_print_row);
void disp_sensor_value(String ip, String mdns);
void disp_power_off();

/**
 * 液晶の明るさを変更する 
 * @return 応答メッセージ
 */
String disp_set_brightness(int brightness);
