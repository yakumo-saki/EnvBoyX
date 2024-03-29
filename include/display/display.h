#include <Arduino.h>

// セットアップモード表示
void disp_setup_startup_screen(String ipAddr, String ssid);

void disp_wifi_info(String ip, String mDNS);
void disp_wait_for_reconfig();
void disp_wifi_starting();
void disp_wifi_error();

/** メッセージ画面を表示 */
void disp_message(bool isError, String msg1, String msg2, String msg3, String msg4);

void disp_all_initialize_complete(String ip, String mdns);
void disp_sensor_value(String ip, String mdns);
void disp_power_off();

// APIで設定が変更された際、画面をすべて再描画する
void disp_redraw_sensor_value_screen();

/**
 * 液晶の明るさを変更する 
 * @return 応答メッセージ
 */
String disp_set_brightness(int brightness);

/**
 * 液晶のON/OFFを設定する
 * @param poweron true = ONにする false = offにする
 */
void disp_set_power(bool poweron);

void setup_display();
