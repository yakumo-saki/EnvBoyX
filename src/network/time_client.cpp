#include <Arduino.h>

#ifdef esp32
#include <esp_sntp.h>

#endif
#ifdef ESP8266
#include <time.h>
#endif

#include "log.h"
#include "network/time_client.h"

void ntp_setup() {

  cfglog("Communicating NTP server.");

  configTzTime("JST-9", "10.1.0.10");

  cfglog("Communicating NTP server done.");
}

#ifdef ESP32
struct tm timeinfo;

String getFormattedTime() {
  if (!getLocalTime(&timeinfo, 1)) {
    return TIME_NOT_READY;
  }

  char buf[64];
  strftime(buf, 64, "%Y/%m/%d %H:%M:%S", &timeinfo);
  // String s = fmt.Sprintf(&timeinfo, "%A, %B %d %Y %H:%M:%S");
  return String(buf);
}
#endif
#ifdef ESP8266
time_t t;
struct tm *tm;

String getFormattedTime() {

  char buf[50];

  t = time(NULL);
  tm = localtime(&t);

  sprintf(buf, " %04d/%02d/%02d %02d:%02d:%02d",
          tm->tm_year+1900, tm->tm_mon+1, tm->tm_mday,
          tm->tm_hour, tm->tm_min, tm->tm_sec);

  return String(buf);
}
#endif
