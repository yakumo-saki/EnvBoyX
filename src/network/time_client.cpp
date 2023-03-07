#include <Arduino.h>

#ifdef esp32
#include <esp_sntp.h>

#endif
#ifdef ESP8266
#include <time.h>
#endif

#include "log.h"
#include "global.h"
#include "network/time_client.h"
#include "utils.h"

char TIME_CLIENT_TZCHAR[30];
char TIME_CLIENT_NTPCHAR[30];

void ntp_setup() {

  cfglog("Communicating NTP server.");

  String tz = config->get(ConfigNames::NTP_TIMEZONE);
  String ntp = config->get(ConfigNames::NTP_ADDRESS);

  cfglog("timezone=" + tz + " ntpsrv=" + ntp);


  tz.toCharArray(TIME_CLIENT_TZCHAR, sizeof TIME_CLIENT_TZCHAR);
  ntp.toCharArray(TIME_CLIENT_NTPCHAR, sizeof TIME_CLIENT_TZCHAR);

  configTzTime(TIME_CLIENT_TZCHAR, TIME_CLIENT_NTPCHAR);

  cfglog("Communicating NTP server done.");
}
