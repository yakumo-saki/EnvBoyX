#include <Arduino.h>

#include <SoftwareSerial.h>
#include <MHZ19.h>

#include "global.h"
#include "mhz19_pwm.h"
#include "mhz19_uart.h"

// ■ 動作モード ■
bool USE_PWM = true;

int CO2_PPM_INVALID = -999;

void mhzlog(String msg) {
  Serial.println("MHZ19B: " + msg);
}

void mhz_read_data() {

  if (use_mhz19b == MHZ_NOUSE) {
    return;
  }

  if (USE_PWM) {
    mhz_read_data_pwm();
  } else {
    mhz_read_data_uart();
  }

}

void mhz_setup() {
  if (use_mhz19b == MHZ_NOUSE) {
    mhzlog("disabled.");
    return;
  } 
  
  USE_PWM = (use_mhz19b == MHZ_USE_PWM);

  if (USE_PWM) {
    mhzlog("Using PWM mode. Some features are disabled.");
    mhz_setup_pwm();
  } else {
    mhzlog("Using UART mode.");
    mhz_setup_uart();
  }
}
