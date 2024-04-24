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

  TouchButton(int16_t x_coord, int16_t y_coord, std::function<void(TouchButton*)> draw, std::function<void(TouchButton *b)> clicked);
  void processPressed();
  void drawNow();
  void draw(std::string label, std::string sub, uint16_t color);
  void setSelected(bool selected);

private:
  bool isSelected = false;;

  int16_t x;
  int16_t y;
  std::function<void(TouchButton*)> drawHandler{};
  std::function<void(TouchButton*)> clickedHandler{};
  PressedState pressedState;
};

#endif //TOUCHBUTTON_H