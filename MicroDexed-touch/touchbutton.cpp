#include "touchbutton.h"
#include "touch.h"
#include "ILI9341_t3n.h"

extern ILI9341_t3n display;

extern ts_t ts;
extern int numTouchPoints;
extern bool remote_active;

TouchButton::TouchButton(int16_t x_coord, int16_t y_coord, std::function<void(TouchButton*)> draw, std::function<void(TouchButton*)> clicked) : x(x_coord), y(y_coord), 
  drawHandler{draw},
  clickedHandler{clicked},
  pressedState(NOT_PRESSED) {
}

void TouchButton::drawNow() {
  drawHandler(this);
}

void TouchButton::setSelected(bool selected) {
  isSelected = selected;
  drawHandler(this);
  DBG_LOG(printf("%i is selected: %i\n", x, isSelected));
}

void TouchButton::draw(const std::string label, const std::string sub, ButtonColor colors) {
  drawButton(x, y, label.c_str(), sub.c_str(), colorMap[colors]);
  uint16_t barColor = isSelected ? COLOR_SYSTEXT : colorMap[colors].bg;
  display.fillRect(x * CHAR_width_small, (y + button_size_y) * CHAR_height_small, button_size_x * CHAR_width_small, 3, barColor);
}

void TouchButton::drawButton(uint8_t x, uint8_t y, const std::string label, const std::string sub, ColorCombo colors) {
  if (remote_active) {
    display.console = true;
  }

  display.setTextSize(1);
  display.setTextColor(colors.text, colors.bg);
  display.fillRect(x * CHAR_width_small, y * CHAR_height_small, button_size_x * CHAR_width_small, CHAR_height_small * button_size_y, colors.bg);
  
  display.setCursor(x * CHAR_width_small + CHAR_width_small / 2, y * CHAR_height_small + 6);
  display.print(label.c_str());

  display.setTextSize((sub.size() > 3) ? 1 : 2);

  switch(sub.size()) {
  case 1:
    display.setCursor((x + 2) * CHAR_width_small + CHAR_width_small / 2, y * CHAR_height_small + 6 + CHAR_height_small);
    break;
  case 2:
    display.setCursor((x + 2) * CHAR_width_small - 4, y * CHAR_height_small + 6 + CHAR_height_small);
    break;
  case 3:
    display.setCursor((x + 2) * CHAR_width_small - 8, y * CHAR_height_small + 6 + CHAR_height_small);
    break;
  default:
    display.setCursor(x * CHAR_width_small + CHAR_width_small / 2, y * CHAR_height_small + 10 + CHAR_height_small);
    break;
  }

  display.print(sub.c_str());
  display.setTextSize(1); // FIXME
}

void TouchButton::processPressed() {
  const bool isInArea = numTouchPoints && (ts.p.x > x * CHAR_width_small && ts.p.x < (x + button_size_x) * CHAR_width_small && ts.p.y > y * CHAR_height_small && ts.p.y < (y + button_size_y) * CHAR_height_small);

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
