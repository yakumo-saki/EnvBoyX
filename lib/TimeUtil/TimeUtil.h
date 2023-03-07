#include <Arduino.h>

// nowTimeがstartTime〜endTimeの間かどうかを返します.
// 例) now=12:00 start=10:00 end=13:00 -> true  
// 例) now=14:00 start=10:00 end=13:00 -> false  
// 例) now=02:00 start=22:00 end=06:00 -> true end が startより前の場合は翌日とみなして計算  
// @param nowTime, startTime, endTime 12:34形式(hh:mm) 形式の時刻。09:00 のようにゼロ埋め必要
bool betweenTime(String nowTime, String startTime, String endTime);

String getFormattedTime();

String getDate();
String getTime();

/** time is not set due to NTP failed or NTP disabled */
const String DATETIME_NOT_READY = "0000/00/00 00:00:00";

/** time is not set due to NTP failed or NTP disabled */
const String TIME_NOT_READY = "00:00:00";
