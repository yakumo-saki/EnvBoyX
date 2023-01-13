#include <Arduino.h>

void ntp_setup();
String getFormattedTime();

/** time is not set due to NTP failed or NTP disabled */
const String TIME_NOT_READY = "0000/00/00 00:00:00";