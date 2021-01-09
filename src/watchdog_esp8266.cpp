#ifdef ESP8266

#include "log.h"

void resetModule() {
}

void watchdog_feed() {
    yield();
}

void setup_watchdog() {
    sectionlog("Watchdog timer not supported.");
}

#endif