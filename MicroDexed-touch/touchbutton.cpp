#include "touchbutton.h"
#include "touch.h"
#include "ILI9341_t3n.h"

extern ILI9341_t3n display;
extern ts_t ts;
extern int numTouchPoints;

static const ColorCombo colorMap[TouchButton::BUTTONCOLOR_NUM] = {
  { GREY1, GREY2 },               // COLOR_NORMAL
  { COLOR_SYSTEXT, DX_DARKCYAN }, // COLOR_ACTIVE
  { COLOR_SYSTEXT, RED },         // COLOR_RED
  { COLOR_SYSTEXT, MIDDLEGREEN }, // COLOR_HIGHLIGHTED
  { COLOR_SYSTEXT, COLOR_BACKGROUND } // COLOR_LABEL
};

FLASHMEM TouchButton::TouchButton(uint16_t x_coord, uint16_t y_coord, std::function<void(TouchButton*)> draw, std::function<void(TouchButton*)> clicked, std::function<void(TouchButton*)> longPressed) : x(x_coord), y(y_coord), 
  drawHandler{draw},
  clickedHandler{clicked},
  longPressedHandler{longPressed},
  pressedMs(0),
  pressedState(NOT_PRESSED) {
}

FLASHMEM void TouchButton::drawNow() {
  drawHandler(this);
}

FLASHMEM void TouchButton::setSelected(bool selected) {
  isSelected = selected;
  drawHandler(this);
}

static bool isButtonTouched = false;
FLASHMEM bool TouchButton::isPressed(uint16_t x, uint16_t y) {
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

FLASHMEM void TouchButton::clear(uint16_t color) {
  clearButton(x, y, color);
}

FLASHMEM ColorCombo TouchButton::getColors(Color color) {
  return colorMap[color];
}

FLASHMEM void TouchButton::clearButton(uint16_t x, uint16_t y, uint16_t color) {
  display.console = true;
  display.fillRect(x, y, BUTTON_SIZE_X, BUTTON_SIZE_Y, color);
}

FLASHMEM void TouchButton::draw(const std::string label, const std::string sub, Color color) {
  drawButton(x, y, label.c_str(), sub.c_str(), color);
  uint16_t barColor = isSelected ? COLOR_SYSTEXT : colorMap[color].bg;
  display.console = true;
  display.fillRect(x, (y + BUTTON_SIZE_Y - 2), BUTTON_SIZE_X, 2, barColor);
}

FLASHMEM void TouchButton::drawButton(uint16_t x, uint16_t y, const std::string label, const std::string sub, Color color) {
  ColorCombo c = colorMap[color];

  display.setTextSize(1);
  display.setTextColor(c.text, c.bg);

  display.console = true;
  display.fillRect(x, y, BUTTON_SIZE_X, BUTTON_SIZE_Y, c.bg);
  
  display.setCursor(x + 5, y + 5);
  display.console = true;
  display.print(label.c_str());

  const bool bigSub = sub.size() <= 3;
  display.setTextSize(bigSub ? 2 : 1);
  if(bigSub) {
    const uint16_t subLengthPixels = sub.size() * CHAR_width;
    display.setCursor(x + (BUTTON_SIZE_X - subLengthPixels) / 2, y + 15);
  } else {
    display.setCursor(x + 5, y + 20);
  }
  display.console = true;
  display.print(sub.c_str());
  display.setTextSize(1); // FIXME
}

FLASHMEM void TouchButton::processPressed() {
  const bool isInArea = (ts.p.x > x && ts.p.x < (x + BUTTON_SIZE_X) && ts.p.y > y && ts.p.y < (y + BUTTON_SIZE_Y));

  switch (pressedState) {
  case NOT_PRESSED:
    if(numTouchPoints && isInArea) {
      pressedState = PRESSED;
      pressedMs = 0;
    }
    break;

  case PRESSED:
    if(longPressedHandler && numTouchPoints) {
      if(isInArea) {
        pressedMs += TOUCH_MAX_REFRESH_RATE_MS;
        if(pressedMs >= 800) {
          longPressedHandler(this);
          pressedState = WAIT_RELEASED;
        }
      }
    }
    if(numTouchPoints == 0) {
      // released
      if(isInArea) {
        clickedHandler(this);
        pressedState = NOT_PRESSED;
      }
    }
    break;

  case WAIT_RELEASED:
    if(numTouchPoints == 0) {
      pressedState = NOT_PRESSED;
    }
    break;
  }
}
