#include <Arduino.h>
#include <TFT_eSPI.h>

extern TFT_eSPI tft;

enum class ARROW_ICON {
	NONE = 0, UP = 1, DOWN = 2, PLUS = 3, MINUS = 4
};

/**
 * 矢印を書く
 */
void draw_arrow(int x, int y, ARROW_ICON type, uint16_t color, int height, int width);

/**
 * 矢印を書く（12 x 22)
 */
void draw_arrow(int x, int y, ARROW_ICON type, uint16_t color);