#pragma once

#include <Arduino.h>

enum class TextAlign { LEFT = 1 , RIGHT = 2 };

enum class TextDecoration { NONE = 0 , BOX = 2, INVERT = 4 };

void draw_string(int x, int y, String valueString, TextAlign align, const uint8_t *font);
void draw_string(int x, int y, String valueString, TextAlign align);

uint16_t draw_value(int x, int y, TextDecoration deco, TextAlign align, String valueString, const uint8_t *valueFont, String unitString, const uint8_t *unitFont);

/**
 * 画面反転切り替え。画面の内容は変更せずに反転する。
 * @param inverse true　反転する false しない
 */
void set_invert(bool inverse);