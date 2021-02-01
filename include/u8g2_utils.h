#pragma once

#include <Arduino.h>

enum class TextAlign { LEFT = 1 , RIGHT = 2 };

enum class TextDecoration { NONE = 0 , BOX = 2, INVERT = 4 };

void draw_string(int x, int y, String valueString, TextAlign align, const uint8_t *font);

void draw_string(int x, int y, String valueString, TextAlign align);

void draw_value(int x, int y, String valueString, TextDecoration deco, TextAlign align);