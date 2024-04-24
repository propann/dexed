#include "touchbutton.h"

#include "config.h"
#include "touch.h"
#include "ILI9341_t3n.h"

extern ILI9341_t3n display;

extern ts_t ts;
extern int numTouchPoints;
extern void draw_button_on_grid(uint8_t x, uint8_t y, const char* t1, const char* t2, uint8_t color);

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

void TouchButton::draw(std::string label, std::string sub, uint16_t color) {
  uint16_t bgColor;
  switch (color) { // fixme color translation needed...
  case 0:
    bgColor = GREY2;
    break;

    case 2:
    bgColor = RED;
    break;

    case 3:
    bgColor = MIDDLEGREEN;
    break;
  
  default:
    break;
  }
  draw_button_on_grid(x, y, label.c_str(), sub.c_str(), color);
  uint16_t barColor = isSelected ? COLOR_SYSTEXT : bgColor;
  display.fillRect(x * CHAR_width_small, (y + button_size_y) * CHAR_height_small, button_size_x * CHAR_width_small, 3, barColor);
  DBG_LOG(printf("update x %i selected: %i\n", x, isSelected));
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
