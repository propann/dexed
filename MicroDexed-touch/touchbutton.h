#ifndef TOUCHBUTTON_H
#define TOUCHBUTTON_H

#include <stdio.h>
#include <string>
#include <functional>

class TouchButton {
public:
  enum PressedState {
    NOT_PRESSED = 0,
    PRESSED,
    LONGPRESSED,
  };

  TouchButton(int16_t x_coord, int16_t y_coord, std::function<void(TouchButton*)> draw, std::function<void(void)> clicked);
  void processPressed();
  void drawNow();
  int16_t x;
  int16_t y;

private:
  std::function<void(TouchButton*)> drawHandler{};
  std::function<void(void)> clickedHandler{};
  
  PressedState pressedState;
};

#endif //TOUCHBUTTON_H