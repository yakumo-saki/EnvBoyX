#include <Arduino.h>
#include <TFT_eSPI.h>
#include "display/st7789_utils.h"

#include "log.h"

extern TFT_eSPI tft;

void _draw_arrow_up(int x, int y, int width, int height, uint16_t color) {
	int half = width / 2;
	int halfX = x + half;
	int rightX = x + width;
	int halfY = y + (height / 2);
	int bottomY = y + height;

	tft.drawLine(halfX, y, halfX, bottomY, color);
	tft.drawLine(x, halfY, halfX, y, color);
	tft.drawLine(halfX, y, rightX, halfY, color);
}

void _draw_arrow_down(int x, int y, int width, int height, uint16_t color) {
	int half = width / 2;
	int halfX = x + half;
	int rightX = x + width;
	int halfY = y + (height / 2);
	int bottomY = y + height;

	tft.drawLine(x, y, rightX, bottomY, color);
	tft.drawLine(x - 1, y, rightX, bottomY, color);
	tft.drawLine(x + 1, y, rightX, bottomY, color);
	tft.drawTriangle(rightX, halfY, rightX, bottomY, halfX, bottomY, color);

	halfX--;
	rightX--;
	halfY--;
	bottomY--;
	tft.drawTriangle(rightX, halfY, rightX, bottomY, halfX, bottomY, color);
}

void _draw_plus_minus(int x, int y, int width, int height, uint16_t color, bool isPlus) {
    const int RADIUS = 2;
    const int FUTOSA = 7;
	int half = width / 2;
	int halfX = x + half - (FUTOSA / 2.0);
	int halfY = y + (height / 2.0) - (FUTOSA / 2.0);

	tft.fillRoundRect(x, halfY, width, FUTOSA, RADIUS, color); // 横

    if (isPlus) {
	    tft.fillRoundRect(halfX, y, FUTOSA, height, RADIUS, color); // たて
    }
}


void draw_arrow(int x, int y, ARROW_ICON type, uint16_t color, int height, int width) {
	int HEIGHT = height;
	int WIDTH = width;

	tft.fillRect(x - 1, y, WIDTH + 3, HEIGHT + 1, TFT_BLACK);

	if (type == ARROW_ICON::UP) {
		_draw_arrow_up(x, y, WIDTH, HEIGHT, color);
		_draw_arrow_up(x + 1, y, WIDTH, HEIGHT, color);
	} else if (type == ARROW_ICON::DOWN) {
		_draw_arrow_down(x, y, WIDTH, HEIGHT, color);
	} else if (type == ARROW_ICON::PLUS) {
		_draw_plus_minus(x, y, WIDTH, HEIGHT, color, true);
	} else if (type == ARROW_ICON::MINUS) {
		_draw_plus_minus(x, y, WIDTH, HEIGHT, color, false);
	} else if (type == ARROW_ICON::NONE) {
        // 削除済みなのでOK
	} else {
        stlog("BUG: Unknown type");
    }
}

void draw_arrow(int x, int y, ARROW_ICON type, uint16_t color) {
    draw_arrow(x, y, type, color, 22, 12);
}