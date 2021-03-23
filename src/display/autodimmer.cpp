//
// Auto dimmer
//
#include <Arduino.h>

#include "display/display_util.h"

#include "global.h"
#include "log.h"

void autodimmer_loop() {

    displog("Autodimmer: lux=" + String(sensorValues.lux));

}
