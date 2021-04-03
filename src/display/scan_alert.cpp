#include <Arduino.h>

#include "global.h"
#include "log.h"
#include "structs.h"
#include "config_names.h"

/** 指定された値がアラートor警告範囲内かチェックして結果を返す */
bool in_range(float value, String alertType, String alertLevel) {
	String strLow = config->get(alertType + "." + alertLevel + "." + ConfigNames::ALERT_LO);
	String strHigh = config->get(alertType + "." + alertLevel + "." + ConfigNames::ALERT_HI);

	float low = strLow.toFloat();
	float high = strHigh.toFloat();

	if (low <= value && value < high) {
		return true;
	}

	return false;
}

/** 指定された値がアラートor警告範囲内かチェックして結果を返す */
value_alert_t check_alert(float value, String alertType) {
	value_alert_t ans;

	ans.caution = (in_range(value, alertType, ConfigNames::ALERT_CAUTION1) || in_range(value, alertType, ConfigNames::ALERT_CAUTION2));
	ans.warning = (in_range(value, alertType, ConfigNames::ALERT_WARN1) || in_range(value, alertType, ConfigNames::ALERT_WARN2));
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
		alerts.temperature = check_alert(sensorValues.temperature, ConfigNames::TEMP_ALERT);
	}

	if (sensorCharacters.humidity) {
		alerts.humidity = check_alert(sensorValues.humidity, ConfigNames::HUMI_ALERT);
	}

	if (sensorCharacters.lux) {
		alerts.lux = check_alert(sensorValues.lux, ConfigNames::LUX_ALERT);
	}

	if (sensorCharacters.pressure) {
		alerts.pressure = check_alert(sensorValues.pressure, ConfigNames::PRES_ALERT);
	}

	if (sensorCharacters.co2ppm) {
		alerts.co2 = check_alert(sensorValues.co2ppm, ConfigNames::CO2_ALERT);
	}

	return alerts;
}