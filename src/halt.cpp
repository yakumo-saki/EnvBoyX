#include <Arduino.h>
#include "display/display.h"

#include "log.h"
#include "watchdog.h"
#include "TimerCall.h"

extern TimerCall timer;

void halt(String msg1, String msg2, String msg3) {
    
    haltlog("HALTing start");
    timer.stop();
    disp_message(true, "* STOP *", msg1, msg2, msg3);

    while(true) {
        watchdog_feed();
        delay(3000);
        haltlog("EnvBoy is HALTed");
    }
}