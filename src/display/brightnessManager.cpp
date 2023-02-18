//
// Auto dimmer
//
#include <Arduino.h>

#include "display/display_util.h"

#include "global.h"
#include "log.h"
#include "config/config.h"
#include "ConfigClass.h"
#include "display/display.h"
#include "SimpleMap.h"

SimpleMap<int> brightnessRegisterMap;

extern const String MOD_BRIGHTNESS_AUTODIMMER = "AUTODIMMER";
extern const String MOD_BRIGHTNESS_TIMEDIMMER = "TIMEDIMMER";

// 優先順位順に最初に見つけた明るさをセットする
void _applyBrightness() {
  
  int brightness = -1;

  if (brightnessRegisterMap.hasKey(MOD_BRIGHTNESS_TIMEDIMMER)) {
    brightness = brightnessRegisterMap.get(MOD_BRIGHTNESS_TIMEDIMMER);
  } else if (brightnessRegisterMap.hasKey(MOD_BRIGHTNESS_AUTODIMMER)) {
    brightness = brightnessRegisterMap.get(MOD_BRIGHTNESS_AUTODIMMER);
  } 

  if (brightness != -1) {
    // timedimmer or autodimmer is active(dimming)
    disp_set_brightness(brightness);
  } else {
    // no dimmers are active.
    disp_set_brightness(config->getAsInteger(ConfigNames::DISPLAY_BRIGHTNESS));
  }
  
}

// ディスプレイの明るさを変更したい時に明るさとモジュール名を登録する
// 実際の明るさは優先順位を加味して決まる
void registerBrightness(String moduleName, int brightness) {
  brightnessRegisterMap.put(moduleName, brightness, false);
  _applyBrightness();
}

// ディスプレイの明るさセットリクエストを削除する
// 登録されていないモジュール名の場合は何も起きない
void unregisterBrightness(String moduleName) {
  brightnessRegisterMap.remove(moduleName);
  _applyBrightness();
}
