//
// Air Pressure delta
//
#include <Arduino.h>

#include "display/display_util.h"

#include "global.h"
#include "log.h"

const int MAX_HISTORY = 30;

// ここに毎分のデータを保存する。
std::vector<sensor_values_t> history(MAX_HISTORY);

void store_history() {
    if (history.size() >= MAX_HISTORY) {
        history.erase(history.begin());
    }
    history.push_back(sensorValues);

    deltalog("Pressure history pushed. historySize = " + String(history.size()));
}

void store_zero_delta() {
    deltaValues.pressure = 0.0;
    deltaValues.temperature = 0.0;
    deltaValues.humidity = 0.0;
    deltaValues.pressure = 0.0;
    deltaValues.lux = 0.0;
    deltaValues.co2ppm = 0;
}

void store_delta() {
    if (history.size() == 0) {
        store_zero_delta();
        deltalog(F("No history. Assume delta = 0"));
        return; 
    }

    const sensor_values_t* base = nullptr;

    for (const sensor_values_t v : history) {
        // 有効なデータが入ってるかチェックして、入っていればdeltaを計算する元にする
        if (v.temperature != 0.0 || v.pressure != 0.0) {
            base = &v;
            break;
        }
    }

    if (base != nullptr) {
        // 計算
        deltaValues.pressure = sensorValues.pressure - base->pressure;
        deltaValues.temperature = sensorValues.temperature - base->temperature;
        deltaValues.humidity = sensorValues.humidity - base->humidity;
        deltaValues.lux = sensorValues.lux - base->lux;
        deltaValues.co2ppm = sensorValues.co2ppm - base->co2ppm;

        deltalog("Pressure Delta=" + String(sensorValues.pressure) 
                + " PRES=" + String(sensorValues.pressure, 2) 
                + " BASE=" + String(base->pressure, 2)
                + " FORMATTED=" + format_delta_value(deltaValues.pressure));
        deltalog("temperature Delta=" + String(sensorValues.temperature) 
                + " PRES=" + String(sensorValues.temperature, 2) 
                + " BASE=" + String(base->temperature, 2)
                + " FORMATTED=" + format_delta_value(deltaValues.temperature));

    } else {
        // 計算
        store_zero_delta();
    }
  

#ifdef SENSOR_VALUE_LOG
#endif
}
