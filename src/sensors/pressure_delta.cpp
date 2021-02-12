//
// Air Pressure delta
//
#include <Arduino.h>

#include "global.h"
#include "log.h"

const int DELTA_MINUTE = 60;

// ここに毎分の気圧を保存する。
// なので、DELTA_MINUTE分前の気圧と今の気圧の差を表示する
std::vector<float> airPressureHistory(DELTA_MINUTE, 0.0);

void store_air_pressure_history() {
    if (airPressureHistory.size() >= DELTA_MINUTE) {
        airPressureHistory.erase(airPressureHistory.begin());
    }
    airPressureHistory.push_back(sensorValues.pressure);
    // airPressureHistory.push_back(sensorValues.pressure + random(-20.0, 20.0)); // for debug
    // airPressureHistory.push_back(sensorValues.pressure + 0.2); // for debug  

    pdeltalog("Pressure history pushed = " + String(sensorValues.pressure));
}

void store_air_pressure_delta() {
    if (airPressureHistory.size() == 0) {
        sensorValues.pressureDelta = 0.0;
        pdeltalog(F("No history. Assume delta = 0.0"));
        return;
    }

    float base = -1.0;

    for (const float& p : airPressureHistory) {
        if (p != 0.0) {
            base = p;
            break;
        }
    }

    if (base == -1.0) {
        sensorValues.pressureDelta = 0.0;
        pdeltalog(F("No history. Assume delta = 0.0"));
        return;
    }
  
    sensorValues.pressureDelta = sensorValues.pressure - base;
    pdeltalog("Pressure Delta=" + String(sensorValues.pressureDelta) 
              + " PRES=" + String(sensorValues.pressure, 2) 
              + " BASE=" + String(sensorValues.pressure, 2));
}
