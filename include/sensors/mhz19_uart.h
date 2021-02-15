#include <Arduino.h>

int mhz_read_data_uart();

void mhz_setup_uart();

/**
 * Auto Baseline calibration を設定する
 * @return 設定に成功したか否か
 */
bool mhz_set_abc(bool onoff);

/**
 * Auto Baseline calibration の状態を取得
 * @return ON or OFF
 */
bool mhz_get_abc();

/**
 * Zero Calibration (現在のppmを400ppmとみなす)
 * 外気で20分動作させたあとに実行する。
 */
bool mhz_do_zero_calibration();
