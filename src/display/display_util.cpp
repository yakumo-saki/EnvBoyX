#include <Arduino.h>
#include "structs.h"

bool has_caution(value_alerts_t alerts) {
    if (alerts.temperature.caution
        || alerts.humidity.caution 
        || alerts.lux.caution 
        || alerts.pressure.caution 
        || alerts.co2.caution) {
            return true;
    }
    return false;
}

bool has_warning(value_alerts_t alerts) {
    if (alerts.temperature.warning
        || alerts.humidity.warning 
        || alerts.lux.warning 
        || alerts.pressure.warning 
        || alerts.co2.warning) {
            return true;
    }
    return false;
}

/** 
 * 気圧の差を表示形式にして返す
 * マイナス値はアイコンで表示するので符号は不要
 */
String format_air_pressure_delta(float value) {
    String pressureDelta;

	if (value < 0) {
		pressureDelta = String(-1.0 * value, 1);
	} else {
		pressureDelta = String(1.0 * value, 1);
	}
    return pressureDelta;
}