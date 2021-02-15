//
// Air Pressure delta
//
#include <Arduino.h>

#include "display/display_util.h"

#include "global.h"
#include "log.h"

void store_free_heap() {
#ifdef ESP32
    sensorValues.freeHeap = ESP.getFreeHeap();
#endif
#ifdef ESP8266
    sensorValues.freeHeap = ESP.getFreeHeap();
#endif
}
