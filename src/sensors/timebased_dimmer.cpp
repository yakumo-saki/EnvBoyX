#include <Arduino.h>

#include "display/display_util.h"

#include "global.h"
#include "log.h"

#include "display/display.h"
#include "network/time_client.h"

#include "display/brightnessManager.h"

#include <TimeUtil.h>

// 最後に見た結果と異なるときのみディスプレイON/OFFするために覚えておく
bool LAST_AUTODIMMER_RESULT = false;

/**
 * 時刻ベースのディスプレイ消灯。
 * 設定変更に対応するため、NTPが有効であれば常にタイマーで呼ばれる。
 * NTP有効無効は再起動でのみ変更可能なので、少なくともNTPは有効である前提。
*/
void timebased_dimmer() {

  if (!config->getAsBoolean(ConfigNames::NTP_AUTO_DIMMER)) {
    displog("Time-based dimmer is disabled");
    return;
  }

  String startTime = config->get(ConfigNames::NTP_AUTO_DIMMER_START);
  String endTime = config->get(ConfigNames::NTP_AUTO_DIMMER_END);

  if (startTime.length() == 0 || endTime.length() == 0) {
    displog("Time-based dimmer: time is invalid: " + startTime + ' ' + endTime);
    return;  // bad config
  }

  String datetime = getFormattedTime();
  if (datetime.equals(DATETIME_NOT_READY)) {
    displog("Time-based dimmer: datetime is not ready");
    return;
  }

  String nowTime = datetime.substring(11, 16);  // 2023/01/20 12:34:56 -> 12:34

  bool dimming = betweenTime(nowTime, startTime, endTime);

  if (dimming != LAST_AUTODIMMER_RESULT) {
    LAST_AUTODIMMER_RESULT = dimming;
    // disp_set_power(!dimming);
    if (dimming) {
      registerBrightness(MOD_BRIGHTNESS_TIMEDIMMER, 0);
    } else {
      unregisterBrightness(MOD_BRIGHTNESS_TIMEDIMMER);
    }
  }
  
}
