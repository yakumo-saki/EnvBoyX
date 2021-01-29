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