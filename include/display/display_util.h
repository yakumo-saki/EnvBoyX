#pragma once

#include "structs.h"
typedef struct {
    float value;
    bool positive = false; // 値が0.1以上のプラス。プラスでもマイナスでもない場合に注意
    bool negative = false; // 絶対値が0.1以上のマイナス。プラスでもマイナスでもない場合に注意
    bool drawIcon = true;  // 絶対値0.2以上
    String formattedValue; // プラス・マイナスを除去した数字だけフォーマット
} pressure_delta_t;

bool has_caution(value_alerts_t alerts);
bool has_warning(value_alerts_t alerts);

String format_air_pressure_delta(float value);
pressure_delta_t get_pressure_delta_struct(float pressureDelta);