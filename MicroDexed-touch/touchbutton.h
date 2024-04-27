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


  TouchButton(int16_t x_coord, int16_t y_coord, std::function<void(TouchButton*)> draw, std::function<void(TouchButton *b)> clicked = [](TouchButton *b){});
  void processPressed();
  void drawNow();
  void draw(const std::string label, const std::string sub, ButtonColor colors);
  void setSelected(bool selected);

private:

  struct ColorCombo {
    uint16_t text;
    uint16_t bg;
  };

  void drawButton(uint8_t x, uint8_t y, const std::string label, const std::string sub, ColorCombo colors);
  bool isSelected = false;

  const ColorCombo colorMap[BUTTONCOLOR_NUM] = {
    { GREY1, GREY2 },               // COLOR_NORMAL
    { COLOR_SYSTEXT, DX_DARKCYAN }, // COLOR_ACTIVE
    { COLOR_SYSTEXT, RED },         // COLOR_RED
    { COLOR_SYSTEXT, MIDDLEGREEN }, // COLOR_HIGHLIGHTED
    { COLOR_SYSTEXT, GREY3 }        // COLOR_LABEL
  };

  int16_t x;
  int16_t y;
  std::function<void(TouchButton*)> drawHandler{};
  std::function<void(TouchButton*)> clickedHandler{};
  PressedState pressedState;
};

#endif //TOUCHBUTTON_H