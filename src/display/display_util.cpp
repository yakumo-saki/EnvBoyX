#include <Arduino.h>
#include "structs.h"

#include "log.h"
#include "display/display_util.h"

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
String format_delta_value(float value) {

    String pressureDelta;

    // 0.05の計算は、String()で切り上げをしてほしくないため
	if (value == 0.0) {
        pressureDelta = "0.0";
    } else if (0.0 < value && value < 0.1) {
        pressureDelta = "0.0";
    } else if (-0.1 < value && value < 0.0) {
        pressureDelta = "0.0";
    } else if (value < 0.0) {
        float v = value + 0.09;
		pressureDelta = String(-1.0 * v, 1);
	} else if (value > 0.0) {
        float v = value - 0.09;
		pressureDelta = String(1.0 * v, 1);
	} else {
        deltalog("Unknown value " + String(value));
    }
     
    return pressureDelta;
}

delta_value_t get_delta_struct(float deltaValue) {

    delta_value_t ret;

    ret.value = deltaValue;

    if (-0.1 < deltaValue && deltaValue < 0.1) {
        // -0.1〜0.1までの間は表示上 0.0 になるので 0と同じ扱い
    } else if (deltaValue > 0) {
        ret.positive = true;
    } else if (deltaValue < 0) {
        ret.negative = true;
    }

    // アイコンを書くか否か
	if (deltaValue > 0.2) {
        ret.drawIcon = true;
	} else if (deltaValue < -0.2) {
        ret.drawIcon = true;
	} else {
        ret.drawIcon = false;
    }

    ret.formattedValue = format_delta_value(deltaValue);

    return ret;
}