#include <Arduino.h>
#include "display/display.h"

#include "log.h"
#include "watchdog.h"
#include "TimerCall.h"

extern TimerCall timer;

void halt(String msg1, String msg2, String msg3) {
    
    haltlog("HALTing start");
    haltlog("cause " + msg1 + " " + msg2 + " " + msg3);
    timer.stop();

    haltlog("Showing HALT Screen");
    disp_message(true, "* STOP *", msg1, msg2, msg3);

    while(true) {
        haltlog("EnvBoy is HALTed");
        watchdog_feed();
        delay(60000);
    }
}