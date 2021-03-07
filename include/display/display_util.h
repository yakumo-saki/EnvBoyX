#pragma once

#include "structs.h"
typedef struct {
    float value;
    bool positive = false; // 値が0.1以上のプラス。プラスでもマイナスでもない場合に注意
    bool negative = false; // 絶対値が0.1以上のマイナス。プラスでもマイナスでもない場合に注意
    bool drawIcon = true;  // 絶対値0.2以上
    String formattedValue; // プラス・マイナスを除去した数字だけフォーマット
} delta_value_t;

bool has_caution(value_alerts_t alerts);
bool has_warning(value_alerts_t alerts);

String format_delta_value(float value, bool limitTo2Keta = false);
delta_value_t get_delta_struct(float deltaValue, bool limitTo2Keta = false);