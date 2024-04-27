#ifndef TOUCHBUTTON_H
#define TOUCHBUTTON_H

#include <stdio.h>
#include <string>
#include <functional>
#include "config.h"

  struct ColorCombo {
    uint16_t text;
    uint16_t bg;
  };

  static constexpr ColorCombo COLOR_NORMAL      = { GREY1, GREY2 };
  static constexpr ColorCombo COLOR_ACTIVE      = { COLOR_SYSTEXT, DX_DARKCYAN };
  static constexpr ColorCombo COLOR_RED         = { COLOR_SYSTEXT, RED };
  static constexpr ColorCombo COLOR_HIGHLIGHTED = { COLOR_SYSTEXT, MIDDLEGREEN };
  static constexpr ColorCombo COLOR_LABEL       = { COLOR_SYSTEXT, GREY3 };

class TouchButton {
public:
  enum PressedState {
    NOT_PRESSED = 0,
    PRESSED,
    LONGPRESSED,
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


  static void drawButton(uint16_t x, uint16_t y, const std::string label, const std::string sub, ColorCombo colors);
  static bool isPressed(uint16_t x, uint16_t y);
private:


  bool isSelected = false;

  const ColorCombo colorMap[BUTTONCOLOR_NUM] = {
    COLOR_NORMAL,      // COLOR_NORMAL
    COLOR_ACTIVE,      // COLOR_ACTIVE
    COLOR_RED,         // COLOR_RED
    COLOR_HIGHLIGHTED, // COLOR_HIGHLIGHTED
    COLOR_LABEL        // COLOR_LABEL
  };

  static constexpr uint16_t BUTTON_SIZE_X = button_size_x * CHAR_width_small;
  static constexpr uint16_t BUTTON_SIZE_Y = button_size_y * CHAR_height_small;
  uint16_t x;
  uint16_t y;
  std::function<void(TouchButton*)> drawHandler{};
  std::function<void(TouchButton*)> clickedHandler{};
  PressedState pressedState;
};

#endif //TOUCHBUTTON_H