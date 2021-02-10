#include <Arduino.h>

#include <U8g2lib.h>

#include "global.h"
#include "log.h"

extern U8G2 u8g2;

enum class TextAlign { LEFT = 1 , RIGHT = 2 };

enum class TextDecoration { NONE = 0 , BOX = 2, INVERT = 4 };

const uint8_t WHITE = 1;
const uint8_t BLACK = 0;

/**
 * u8g2.drawStrのラッパ。寄せ方向が指定できる。
 */
void draw_string(int x, int y, String valueString, TextAlign align) {
  if (align == TextAlign::LEFT) {
    u8g2.drawStr(x, y, valueString.c_str());
    return;
  }

  // 右寄せ
  int width = u8g2.getStrWidth(valueString.c_str());
  u8g2.drawStr(x - width, y, valueString.c_str());
}

/**
 * draw_stringのラッパ。fontが指定できる。
 */
void draw_string(int x, int y, String valueString, TextAlign align, const uint8_t *font) {
  u8g2.setFont(font);
  draw_string(x, y, valueString, align);
}

// 値を描画する
uint16_t draw_value(int x, int y, TextDecoration deco, TextAlign align, String valueString, const uint8_t *valueFont, String unitString, const uint8_t *unitFont) {

  const int HEIGHT = u8g2.getMaxCharHeight();
  
  // 警告表示の時用のマージン。通常表示と警告表示で場所が変わるのはイマイチなので通常時も適用する
  const int MARGIN = 1;
  const int RECT_MARGIN = 1 + MARGIN;  // 枠 1px + マージン 2px
  
  const int MARGIN_VAL_UNIT = 2;  // 単位と値の間のマージン

  // 幅を求める
  int valueWidth = 0, unitWidth = 0;
  u8g2.setFont(valueFont);
  valueWidth = u8g2.getStrWidth(valueString.c_str());
  if (unitString != "") {
    u8g2.setFont(unitFont);
    unitWidth = u8g2.getStrWidth(unitString.c_str());
  }

  uint16_t boxWidth = valueWidth + MARGIN_VAL_UNIT + unitWidth + (RECT_MARGIN * 2); // *2 = 左右

  int boxStartX = 0; // 枠を描画する位置
  int strStartX = 0; // 文字を描画する位置

  if (align == TextAlign::LEFT) {
    boxStartX = x;
    strStartX = boxStartX + MARGIN;
  } else if (align == TextAlign::RIGHT) {
    boxStartX = x - boxWidth; // 枠の描画位置は左から指定しないといけない マージンは左右にあるので二回
    strStartX = x - RECT_MARGIN;  // 文字は右端を指定する
  }

  // 背景塗りつぶし無し
  u8g2.setFontMode(1);

  if (deco == TextDecoration::BOX || deco == TextDecoration::INVERT) {

    // 枠 or 塗りつぶし
    if (deco == TextDecoration::INVERT) {
      u8g2.setDrawColor(WHITE);
      u8g2.drawBox(boxStartX, y - 1, boxWidth, HEIGHT); // わざと本来の位置より1dot上に書いている

      // 反転した際は太字にする（細いとよみづらい）
      u8g2.setDrawColor(BLACK);
      u8g2.setFontMode(1);  // これをしないと DrawColor = BLACK の時 背景の白も書かれてしまう

      u8g2.setFont(valueFont);
      draw_string(strStartX, y, valueString, align);
      draw_string(strStartX - 1, y, valueString, align);

      u8g2.setFont(unitFont);
      draw_string(strStartX + valueWidth + MARGIN_VAL_UNIT, y, unitString, align);
      draw_string(strStartX + valueWidth + MARGIN_VAL_UNIT - 1, y, unitString, align);
    } else if (deco == TextDecoration::BOX) {
      draw_string(strStartX, y, valueString, align);
      u8g2.drawFrame(boxStartX, y  - 1, boxWidth, HEIGHT);
    }
  } else {
    // 通常表示
    u8g2.setDrawColor(WHITE);
    draw_string(strStartX, y, valueString, align);

  }

  u8g2.setFontMode(0);
  u8g2.setDrawColor(WHITE);  // 
  return boxWidth;

}

void set_invert(bool inverse) {
  if (inverse) {
    u8g2.sendF("c", 0xa7);
  } else {
    u8g2.sendF("c", 0xa6);
  }
}