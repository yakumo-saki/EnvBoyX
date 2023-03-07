#include <Arduino.h>
#include <ArduinoJson.h>

#include "config/config.h"

#include "log.h"
#include "global.h"
#include "sensors/mhz19.h"

/**
 * SensorValueAdjustmentを適用した値を返す.
 * sensor_values_t を返すが、中身があるのはAdjustmentが適用される項目のみ。
*/
sensor_values_t applySenserValueAdjustment(sensor_values_t sensorvalues) {

  sensor_values_t adjusted;

  // v48 SenserValueAdjustment
  adjusted.temperature = sensorvalues.temperature + config->getAsFloat(ConfigNames::TEMP_ADJ);
  adjusted.humidity = sensorvalues.humidity + config->getAsFloat(ConfigNames::HUMI_ADJ);
  adjusted.pressure = sensorvalues.pressure + config->getAsFloat(ConfigNames::PRES_ADJ);
  adjusted.lux = sensorvalues.lux + config->getAsInteger(ConfigNames::LUX_ADJ);

  if (sensorValues.co2ppm == CO2_PPM_INVALID) {
    adjusted.co2ppm = CO2_PPM_INVALID;
  } else {
    adjusted.co2ppm = sensorvalues.co2ppm + config->getAsInteger(ConfigNames::CO2_ADJ);
  }

  return adjusted;
}