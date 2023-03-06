#include <Arduino.h>

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