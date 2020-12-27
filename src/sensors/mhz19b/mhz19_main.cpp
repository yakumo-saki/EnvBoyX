#include <Arduino.h>

#include "log.h"

#include "global.h"
#include "sensors/mhz19_pwm.h"
#include "sensors/mhz19_uart.h"

// ■ 動作モード ■
bool USE_PWM = true;

int CO2_PPM_INVALID = -999;

void mhz_read_data() {

  if (config.use_mhz19b == MHZ_NOUSE) {
    return;
  }

  if (USE_PWM) {
    mhz_read_data_pwm();
  } else {
    mhz_read_data_uart();
  }

}

void mhz_setup() {
  if (config.use_mhz19b == MHZ_NOUSE) {
    mhzlog("disabled.");
    return;
  } 
  
  USE_PWM = (config.use_mhz19b == MHZ_USE_PWM);

  if (USE_PWM) {
    mhzlog("Using PWM mode. Some features are disabled.");
    mhz_setup_pwm();
  } else {
    mhzlog("Using UART mode.");
    mhz_setup_uart();
  }
}
