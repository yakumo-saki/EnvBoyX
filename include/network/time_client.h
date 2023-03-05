#include <Arduino.h>

void ntp_setup();
String getFormattedTime();

String getDate();
String getTime();

/** time is not set due to NTP failed or NTP disabled */
const String DATETIME_NOT_READY = "0000/00/00 00:00:00";

/** time is not set due to NTP failed or NTP disabled */
const String TIME_NOT_READY = "00:00:00";