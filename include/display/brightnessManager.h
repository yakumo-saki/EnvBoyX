#include <Arduino.h>

extern const String MOD_BRIGHTNESS_AUTODIMMER;
extern const String MOD_BRIGHTNESS_TIMEDIMMER;

// ディスプレイの明るさを変更したい時に明るさとモジュール名を登録する
// 実際の明るさは優先順位を加味して決まる
void registerBrightness(String moduleName, int brightness);

// ディスプレイの明るさセットリクエストを削除する
// 登録されていないモジュール名の場合は何も起きない
void unregisterBrightness(String moduleName);
