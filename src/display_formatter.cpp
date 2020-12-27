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
	return String(pres, 1) + "hpa";
}

String format_lux(float lux) {
	return String(lux, 0) + "lx";
}

String format_co2_ppm(int ppm) {
	// CO2の値がエラーであれば表示を変える
	String result = String(ppm);
	if (ppm < 0) {
	  result = "****";  // 計測エラー
	} else if (lastPpm < 399) {
	  result = "*" + String(ppm); // あり得ない値(最低399ppmなはず）
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
	ret.temperature = format_temparature(lastTemp);
	ret.humidity = format_humidity(lastHumidity);
	ret.pressure = format_pressure(lastPressure);
	ret.co2ppm = format_co2_ppm(lastPpm);
	ret.lux = format_lux(lastLuxFull);
	
	return ret;
}