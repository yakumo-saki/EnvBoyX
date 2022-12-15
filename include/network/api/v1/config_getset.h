#include <Arduino.h>
#include <ArduinoJson.h>

// Config SET API のエントリポイント
String updateConfig();

// Config revertしたときに画面書き換え等を全部実行する
void reflectConfigAll();