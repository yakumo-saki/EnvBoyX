#include <Arduino.h>

#include "global.h"
#include "log.h"
#include "structs.h"

String format_temparature(float temp) {
	return String(temp, 2) + "c";
}

String format_humidity(float humi) {
	return String(humi, 2) + "%";
}

String format_pressure(float pres) {
	return String(pres, 0) + "hpa";
}

String format_lux(float lux) {
	return String(lux, 0) + "Lx";
}

String format_co2_ppm(int ppm) {
	// CO2の値がエラーであれば表示を変える
	String result = String(ppm);
	if (ppm < 0) {
	  result = "****";  // 計測エラー
	} else {
	  result = String(ppm);       // OK
	}

	return result + "ppm";
}


/**
 * Global last**** から disp_values_tを作る
 */
disp_values_t create_disp_values() {
	disp_values_t ret;
	ret.temperature = format_temparature(sensorValues.temperature);
	ret.humidity = format_humidity(sensorValues.humidity);
	ret.pressure = format_pressure(sensorValues.pressure);
	ret.co2ppm = format_co2_ppm(sensorValues.co2ppm);
	ret.lux = format_lux(sensorValues.lux);
	
	return ret;
}