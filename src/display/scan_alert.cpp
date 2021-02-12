#include <Arduino.h>

#include "global.h"
#include "log.h"
#include "structs.h"

/** 指定された値が範囲内かチェックする */
bool in_range(float value, config_alert_range_t range) {
	float low = atof(range.low.c_str());
	float high = atof(range.high.c_str());

	if (low <= value && value < high) {
		return true;
	}

	return false;
}

/** 指定された値がアラートor警告範囲内かチェックして結果を返す */
value_alert_t check_alert(float value, config_alert_t alerts) {

	value_alert_t ans;
	ans.caution = in_range(value, alerts.caution1) || in_range(value, alerts.caution2);
	ans.warning = in_range(value, alerts.warning1) || in_range(value, alerts.warning2);
	return ans;
}

void log_alert(value_alert_t alert, String name) {
	displog(name + " CAUTION = " + String(alert.caution) + " WARNING=" + String(alert.warning));
}

/** 
 * アラートをスキャンするエントリポイント
 */
value_alerts_t check_for_alerts() {

	value_alerts_t alerts;
	if (sensorCharacters.temperature) {
		alerts.temperature = check_alert(sensorValues.temperature, config.temperatureAlerts);
	}

	if (sensorCharacters.humidity) {
		alerts.humidity = check_alert(sensorValues.humidity, config.humidityAlerts);
	}

	if (sensorCharacters.lux) {
		alerts.lux = check_alert(sensorValues.lux, config.luxAlerts);
	}

	if (sensorCharacters.pressure) {
		alerts.pressure = check_alert(sensorValues.pressure, config.pressureAlerts);
	}

	if (sensorCharacters.co2ppm) {
		alerts.co2 = check_alert(sensorValues.co2ppm, config.co2Alerts);
	}

	return alerts;
}