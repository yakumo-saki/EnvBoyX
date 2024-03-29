#include <Arduino.h>

#include "log.h"

#include "global.h"
#include "config/config.h"
#include "ConfigClass.h"

#include "sensors/mhz19.h"
#include "sensors/mhz19_pwm.h"
#include "sensors/mhz19_uart.h"

extern const int CO2_PPM_INVALID = -999;

// ■ 動作モード ■
bool USE_PWM = true;

sensor_characters_t mhz_characters() {
	sensor_characters_t sensor;

	sensor.co2ppm = true;
  sensor.co2ppmAccuracy = true;
  
	return sensor;
}


void mhz_read_data() {

  if (config->get(ConfigNames::MHZ19B) == ConfigValues::MHZ_NOUSE) {
    return;
  }

  if (USE_PWM) {
    mhz_read_data_pwm();
  } else {
    mhz_read_data_uart();
  }

}

bool mhz_setup() {
  if (config->get(ConfigNames::MHZ19B) == ConfigValues::MHZ_NOUSE) {
    mhzlog(F("disabled."));
    sensorValues.co2ppm = CO2_PPM_INVALID;
    return false;
  } 
  
  USE_PWM = (config->get(ConfigNames::MHZ19B) == ConfigValues::MHZ_USE_PWM);

  if (USE_PWM) {
    mhzlog(F("Using PWM mode. Some features are disabled."));
    mhz_setup_pwm();
  } else {
    mhzlog(F("Using UART mode."));
    mhz_setup_uart();
  }
  return true;
}
