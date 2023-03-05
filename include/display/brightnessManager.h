#include <Arduino.h>

extern const String MOD_BRIGHTNESS_AUTODIMMER;
extern const String MOD_BRIGHTNESS_TIMEDIMMER;

/**
 * ディスプレイの明るさを変更したい時に明るさとモジュール名を登録する
 * 実際の明るさは優先順位を加味して決まる
 * @param moduleName MOD_BRIGHTNESS_* モジュール名
 * @param brightness 明るさ(0-255)
 */
void registerBrightness(String moduleName, int brightness);

/**
 * ディスプレイの明るさセットリクエストを削除する
 * 登録されていないモジュール名の場合は何も起きない
 * @param moduleName MOD_BRIGHTNESS_* モジュール名
 */
void unregisterBrightness(String moduleName);
