#include "touchbutton.h"
#include "touch.h"
#include "ILI9341_t3n.h"

extern ILI9341_t3n display;

extern ts_t ts;
extern int numTouchPoints;
extern bool remote_active;

static const ColorCombo colorMap[TouchButton::BUTTONCOLOR_NUM] = {
  { GREY1, GREY2 },               // COLOR_NORMAL
  { COLOR_SYSTEXT, DX_DARKCYAN }, // COLOR_ACTIVE
  { COLOR_SYSTEXT, RED },         // COLOR_RED
  { COLOR_SYSTEXT, MIDDLEGREEN }, // COLOR_HIGHLIGHTED
  { COLOR_SYSTEXT, GREY3 }        // COLOR_LABEL
};

TouchButton::TouchButton(uint16_t x_coord, uint16_t y_coord, std::function<void(TouchButton*)> draw, std::function<void(TouchButton*)> clicked) : x(x_coord), y(y_coord), 
  drawHandler{draw},
  clickedHandler{clicked},
  pressedState(NOT_PRESSED) {
}

void TouchButton::drawNow() {
  drawHandler(this);
}

void TouchButton::setSelected(bool isSelected) {
  uint16_t barColor = isSelected ? COLOR_SYSTEXT : colorMap[currentColors].bg;
  display.fillRect(x, (y + BUTTON_SIZE_Y - 2), BUTTON_SIZE_X, 2, barColor);
}

static bool isButtonTouched = false;
bool TouchButton::isPressed(uint16_t x, uint16_t y) {
  bool result = false;
  if(numTouchPoints > 0) {
    if(isButtonTouched == false) {
      if(ts.p.x > x && ts.p.x < (x + BUTTON_SIZE_X) && ts.p.y > y && ts.p.y < (y + BUTTON_SIZE_Y)) {
        result = true;
        isButtonTouched = true;
      }
    }
  } else {
    isButtonTouched = false;
  }
  return result;
}

void TouchButton::clear(uint16_t color) {
  clearButton(x, y, color);
}

ColorCombo TouchButton::getColors(ButtonColor color) {
  return colorMap[color];
}

void TouchButton::clearButton(uint16_t x, uint16_t y, uint16_t color) {
  display.fillRect(x, y, BUTTON_SIZE_X, BUTTON_SIZE_Y, color);
}

void TouchButton::draw(const std::string label, const std::string sub, ButtonColor colors) {
  currentColors = colors;
  drawButton(x, y, label.c_str(), sub.c_str(), colors);
}

void TouchButton::drawButton(uint16_t x, uint16_t y, const std::string label, const std::string sub, ButtonColor colors) {
  if (remote_active) {
    display.console = true;
  }

  ColorCombo c = colorMap[colors];

  display.setTextSize(1);
  display.setTextColor(c.text, c.bg);
  display.fillRect(x, y, BUTTON_SIZE_X, BUTTON_SIZE_Y, c.bg);
  
  display.setCursor(x + CHAR_width_small / 2, y + 5);
  display.print(label.c_str());

  const bool bigSub = sub.size() <= 3;
  display.setTextSize(bigSub ? 2 : 1);
  if(bigSub) {
    const uint16_t subLengthPixels = sub.size() * CHAR_width;
    display.setCursor(x + (BUTTON_SIZE_X - subLengthPixels) / 2, y + 6 + CHAR_height_small);
  } else {
    display.setCursor(x + CHAR_width_small / 2, y + 10 + CHAR_height_small);
  }
  display.print(sub.c_str());
  display.setTextSize(1); // FIXME
}

void TouchButton::processPressed() {
  const bool isInArea = numTouchPoints && (ts.p.x > x && ts.p.x < (x + BUTTON_SIZE_X) && ts.p.y > y && ts.p.y < (y + BUTTON_SIZE_Y));

  switch (pressedState) {
  case NOT_PRESSED:
    if(isInArea) {
      pressedState = PRESSED;
      clickedHandler(this);
    }
    break;

  case PRESSED:
    if(isInArea == false) {
      pressedState = NOT_PRESSED;
    }
    break;
  }
}
