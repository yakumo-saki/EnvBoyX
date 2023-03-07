#include <Arduino.h>

#include "TimeUtil.h"

/**
 * 時刻判定
 * @param now,start,end 01:00 -> 100 のような int 表記
 * @return true 範囲内 false 範囲外
*/
bool _inDimmingTime(int now, int start, int end) {
  bool overday = (start > end);  // 日付またぎ (ex 22:00 - 06:00)

  // Serial.println(now);
  // Serial.println(start);
  // Serial.println(end);

  if (!overday) {
    // simple pattern ex 05:00 - 17:00
    if (start <= now && now <= end) {
      return true;
    }
  } else {
    if (start <= now || now <= end) {
      return true;
    }
  }

  return false;
}

/**
 * 時刻文字列を計算用の内部形式のintに変換
 * @return "09:12" -> 10912。 先頭に1が追加されるのでゼロ埋めを気にする必要がない
*/
int _timeToInt(String time) {
  String timeIntStr = "1" + time.substring(0,2) + time.substring(3,5); // prefix "1" to prevent deletion of ZERO
  return (timeIntStr).toInt();
}

//  * 例) now=12:00 start=10:00 end=13:00 -> true
//  * 例) now=14:00 start=10:00 end=13:00 -> false
//  * 例) now=02:00 start=22:00 end=06:00 -> true end が startより前の場合は翌日とみなして計算
bool betweenTime(String nowTime, String startTime, String endTime) {
  int start = _timeToInt(startTime);
  int end = _timeToInt(endTime);
  int now = _timeToInt(nowTime);

  // Serial.println(datetime + "->" + datetime.substring(11, 16));

  bool dimming = _inDimmingTime(now, start, end);
  return dimming;
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
    return DATETIME_NOT_READY;
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
