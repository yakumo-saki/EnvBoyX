//
// BME280 Temparature , 
//

#include <Arduino.h>
#include <Wire.h>

#include "Seeed_BME280.h"

#include "global.h"
#include "log.h"

BME280 bme;   // Default : forced mode, standby time = 1000 ms
                 // Oversampling = pressure ×1, temperature ×1, humidity ×1, filter off,
                 // Address 0x76

const uint8_t BME_ADDR = 0x76;

static bool use_bme = false;

sensor_characters_t bme_characters() {
	sensor_characters_t sensor;

  sensor.temperature = true;
  sensor.humidity = true;
  sensor.pressure = true;

	return sensor;
}

bool has_bme() {
  // check i2c 0x76
  Wire.beginTransmission(BME_ADDR);
  byte error = Wire.endTransmission();

  if (error != 0) {
    // bmelog("Error bme280 connection: " + String(error));
    return false;
  }

  return true;
}

bool bme_setup() {

  if (!has_bme()) {
    use_bme = false;
    bmelog(F("BME280 disabled."));
    return false;
  }
 
  // init BME
  while(!bme.init()){
    bmelog(F("Initloop: Could not find BME280 sensor!"));
    return false;
  }

  bmelog(F("BME280 Enabled"));
  use_bme = true;
  return true;
}

void bme_read_data() {

  if (!use_bme) {
    return;
  }
  
  float temp(NAN), hum(NAN), pres(NAN);

  // unit: B000 = Pa, B001 = hPa, B010 = Hg, B011 = atm, B100 = bar, B101 = torr, B110 = N/m^2, B111 = psi
  // BME280::TempUnit tempUnit(BME280::TempUnit_Celsius);
  // BME280::PresUnit presUnit(BME280::PresUnit_Pa);
   
  // Parameters: (float& pressure, float& temp, float& humidity, bool celsius = false, uint8_t pressureUnit = 0x0)
  // bme.read(pres, temp, hum, tempUnit, presUnit); 

  // pres = pres / 100;

  temp = bme.getTemperature();
  pres = bme.getPressure() / 100.0;
  hum = bme.getHumidity();

#ifdef SENSOR_VALUE_LOG
  char log[80];
  snprintf(log, sizeof log, "Temp: %.2fc   Humidity: %.2f%%   Pressure: %.2fhPa", temp, hum, pres);
  bmelog(String(log));
#endif

  sensorValues.temperature = temp;
  sensorValues.humidity = hum;
  sensorValues.pressure = pres;
}
