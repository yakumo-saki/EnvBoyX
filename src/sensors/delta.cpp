//
// Air Pressure delta
//
#include <Arduino.h>

#include "display/display_util.h"

#include "global.h"
#include "log.h"

const int MAX_HISTORY = 30;

// ここに毎分のデータを保存する。
std::vector<sensor_values_t> history;

void store_history() {
    if (history.size() >= MAX_HISTORY) {
        history.erase(history.begin());
    }
    history.push_back(sensorValues);

    // deltalog("Pressure history pushed. historySize = " + String(history.size()));
}

void store_delta() {
    // 起動直後のhistoryが空の状態のときに、最新の測定値を
    // historyに追加して、差分の計算を単純にする
    if (history.empty()) {
        deltalog(F("No history. Create first history"));
        history.reserve(MAX_HISTORY);
        store_history();
    } else {
        // statlog("history size=" + String(history.size())
        //   + " Capacity="  + String(history.capacity()));
    }

    const sensor_values_t *base = nullptr;
    base = &history.front();

    // 計算
    deltaValues.pressure = sensorValues.pressure - base->pressure;
    deltaValues.temperature = sensorValues.temperature - base->temperature;
    deltaValues.humidity = sensorValues.humidity - base->humidity;
    deltaValues.lux = sensorValues.lux - base->lux;
    deltaValues.co2ppm = sensorValues.co2ppm - base->co2ppm;

#ifdef SENSOR_VALUE_LOG
    deltalog("Pressure Delta=" + String(sensorValues.pressure) 
            + " PRES=" + String(sensorValues.pressure, 2) 
            + " BASE=" + String(base->pressure, 2)
            + " FORMATTED=" + format_delta_value(deltaValues.pressure));
    deltalog("temperature Delta=" + String(sensorValues.temperature) 
            + " PRES=" + String(sensorValues.temperature, 2) 
            + " BASE=" + String(base->temperature, 2)
            + " FORMATTED=" + format_delta_value(deltaValues.temperature));
#endif
}
