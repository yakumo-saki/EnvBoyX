#ifdef ESP32

#include <Arduino.h>
#include <esp_system.h>

#include "log.h"

hw_timer_t *watchdogTimer = NULL;
const int WATCHDOG_TIMER_NO = 0;

void IRAM_ATTR resetModule() {
    sectionlog("watchdog timer timeout. reboot");
    esp_restart();
}

void IRAM_ATTR watchdog_feed() {
    // debuglog("watchdog feeded");
    if (watchdogTimer != NULL && watchdogTimer != nullptr) {
        timerWrite(watchdogTimer, 0);
        yield();
    }
}

void setup_watchdog() {
    int WATCHDOG_TIMEOUT_MS = 10000;
    
    watchdogTimer = timerBegin(WATCHDOG_TIMER_NO, 80, true);
    timerAttachInterrupt(watchdogTimer, &resetModule, true);
    timerAlarmWrite(watchdogTimer, WATCHDOG_TIMEOUT_MS * 1000, false); // uSecond
    timerAlarmEnable(watchdogTimer);

    sectionlog("Watchdog timer start. timeout=" + String(WATCHDOG_TIMEOUT_MS) + "ms");
}

#endif