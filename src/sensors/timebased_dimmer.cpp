#include <Arduino.h>

#include "display/display_util.h"

#include "global.h"
#include "log.h"

#include "display/display.h"
#include "network/time_client.h"

#include "display/brightnessManager.h"

// 最後に見た結果と異なるときのみディスプレイON/OFFするために覚えておく
bool LAST_AUTODIMMER_RESULT = false;


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

int _timeToInt(String time) {
  String timeIntStr = "1" + time.substring(0,2) + time.substring(3,5); // prefix "1" to prevent deletion of ZERO
  return (timeIntStr).toInt();
}

/**
 * 時刻ベースのディスプレイ消灯。
 * 設定変更に対応するため、NTPが有効であれば常にタイマーで呼ばれる。
 * NTP有効無効は再起動でのみ変更可能なので、少なくともNTPは有効である前提。
*/
void timebased_dimmer() {

  String startTime = config->get(ConfigNames::NTP_AUTO_DIMMER_START);
  String endTime = config->get(ConfigNames::NTP_AUTO_DIMMER_END);

  if (!config->getAsBoolean(ConfigNames::NTP_AUTO_DIMMER)) {
    return;
  }

  if (startTime.length() == 0 || endTime.length() == 0) {
    return;  // bad config
  }

  String datetime = getFormattedTime();
  if (datetime.equals(TIME_NOT_READY)) {
    return;
  }

  int start = _timeToInt(startTime);
  int end = _timeToInt(endTime);
  int now = _timeToInt(datetime.substring(11, 16));  // 2023/01/20 12:34:56 -> 12:34

  // Serial.println(datetime + "->" + datetime.substring(11, 16));

  bool dimming = _inDimmingTime(now, start, end);

  if (dimming != LAST_AUTODIMMER_RESULT) {
    LAST_AUTODIMMER_RESULT = dimming;
    // disp_set_power(!dimming);
    if (dimming) {
      registerBrightness(MOD_BRIGHTNESS_AUTODIMMER, 0);
    } else {
      unregisterBrightness(MOD_BRIGHTNESS_AUTODIMMER);
    }
  }
  
}
