#include "touchbutton.h"

#include "config.h"
#include "touch.h"

extern ts_t ts;
extern int numTouchPoints;
extern void draw_button_on_grid(uint8_t x, uint8_t y, const char* t1, const char* t2, uint8_t color);

TouchButton::TouchButton(int16_t x_coord, int16_t y_coord, std::function<void(TouchButton*)> draw, std::function<void(void)> clicked) : x(x_coord), y(y_coord), 
  drawHandler{draw},
  clickedHandler{clicked},
  pressedState(NOT_PRESSED) {
}

void TouchButton::drawNow() {
  drawHandler(this);
}

void TouchButton::processPressed() {
  const bool isInArea = numTouchPoints && (ts.p.x > x * CHAR_width_small && ts.p.x < (x + button_size_x) * CHAR_width_small && ts.p.y > y * CHAR_height_small && ts.p.y < (y + button_size_y) * CHAR_height_small);

  switch (pressedState) {
  case NOT_PRESSED:
    if(isInArea) {
      pressedState = PRESSED;
      clickedHandler();
      drawHandler(this);
    }
    break;

  case PRESSED:
    if(isInArea == false) {
      pressedState = NOT_PRESSED;
    }
    break;
  }
}
