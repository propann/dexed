#ifndef TOUCHBUTTON_H
#define TOUCHBUTTON_H

#include <stdio.h>
#include <string>
#include <functional>
#include "config.h"

class TouchButton {
public:
  enum PressedState {
    NOT_PRESSED = 0,
    PRESSED,
    //LONGPRESSED,
  };

  enum ButtonColor {
    BUTTON_NORMAL = 0,
    BUTTON_ACTIVE = 1,
    BUTTON_RED = 2,
    BUTTON_HIGHLIGHTED = 3,
    BUTTON_LABEL = 4,
    BUTTONCOLOR_NUM = 5
  };

  TouchButton(uint16_t x_coord, uint16_t y_coord, std::function<void(TouchButton*)> draw, std::function<void(TouchButton *b)> clicked = [](TouchButton *b){});
  void processPressed();
  void drawNow();
  void draw(const std::string label, const std::string sub, ButtonColor colors);
  void setSelected(bool selected);

  static void drawButton(uint16_t x, uint16_t y, const std::string label, const std::string sub, ButtonColor colors);
  static bool isPressed(uint16_t x, uint16_t y);

private:
  bool isSelected = false;

  static constexpr uint16_t BUTTON_SIZE_X = button_size_x * CHAR_width_small;
  static constexpr uint16_t BUTTON_SIZE_Y = button_size_y * CHAR_height_small;
  uint16_t x;
  uint16_t y;
  std::function<void(TouchButton*)> drawHandler{};
  std::function<void(TouchButton*)> clickedHandler{};
  PressedState pressedState;
};

#endif //TOUCHBUTTON_H