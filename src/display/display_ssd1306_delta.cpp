#include <Arduino.h>

#include "global.h"

#include "display/display_ssd1306_delta.h"

// 有効な差分の種類をリストにして返す
std::vector<DeltaType> get_delta_types() {
    std::vector<DeltaType> types;
    if (sensorCharacters.temperature) types.push_back(DeltaType::Temp);
    if (sensorCharacters.humidity) types.push_back(DeltaType::Humi);
    if (sensorCharacters.pressure) types.push_back(DeltaType::Pressure);
    if (sensorCharacters.co2ppm) types.push_back(DeltaType::co2);

    return types;
}

DeltaType get_delta_type() {

    static unsigned int type = 0;
    static unsigned int second = 0;

    std::vector<DeltaType> types = get_delta_types();

    if (types.empty()) return DeltaType::None;

    second++;
    if (second == 3) {
        type++;
        second = 0;
    }
    if (type >= types.size()) type = 0;  // size は indexより1大きい

    return types[type];
}

delta_types_t get_delta_type2() {

    static unsigned int type = 0;
    static unsigned int second = 0;

    std::vector<DeltaType> types = get_delta_types();

    delta_types_t ret;
    ret.deltaType1 = DeltaType::None;
    ret.deltaType2 = DeltaType::None;

    if (types.empty()) {
        return ret;
    } else if (types.size() == 1) {
        ret.deltaType1 = types[0];
        return ret;
    } else if (types.size() == 2) {
        ret.deltaType1 = types[0];
        ret.deltaType2 = types[1];
        return ret;
    }

    ret.deltaType1 = types[type];
    if (types.size() <= (type + 1)) {
        // None
    } else {
        ret.deltaType2 = types[type + 1];
    }

    second++;
    if (second == 3) {
        type = type + 2;
        second = 0;
    }
    if (type >= types.size()) type = 0;  // size は indexより1大きい

    return ret;
}
