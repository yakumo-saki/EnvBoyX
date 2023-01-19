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

/**
 * 現在時刻のhh:mm:ss を返す。
 * 時刻がセットされていない場合 "00:00:00" を返す
 */
String getTime() {
  String now = getFormattedTime();
  return now.substring(11); // yyyy/mm/dd hh:mm:ss
}

/**
 * 現在日付を yyyy/mm/dd 形式で返す。
 * 時刻がセットされていない場合 "0000/00/00" を返す
 */
String getDate() {
  String now = getFormattedTime();
  return now.substring(0, 9); // yyyy/mm/dd hh:mm:ss
}


// ローカルタイムの取得がESP32とESP8266で異なるので切り分け

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

  if ( tm->tm_year < 100) {
    return TIME_NOT_READY;
  }

  sprintf(buf, "%04d/%02d/%02d %02d:%02d:%02d",
          tm->tm_year+1900, tm->tm_mon+1, tm->tm_mday,
          tm->tm_hour, tm->tm_min, tm->tm_sec);

  return String(buf);
}
#endif
