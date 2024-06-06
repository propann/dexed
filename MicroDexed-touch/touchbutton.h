#ifndef TOUCHBUTTON_H
#define TOUCHBUTTON_H

#include <stdio.h>
#include <string>
#include <functional>
#include "config.h"
#include "TeensyTimerTool.h"

using namespace TeensyTimerTool;

struct ColorCombo {
  uint16_t text;
  uint16_t bg;
};

class TouchButton {
public:
  enum PressedState {
    NOT_PRESSED = 0,
    PRESSED
  };

  enum Color {
    BUTTON_NORMAL = 0,
    BUTTON_ACTIVE = 1,
    BUTTON_RED = 2,
    BUTTON_HIGHLIGHTED = 3,
    BUTTON_LABEL = 4,
    BUTTONCOLOR_NUM = 5
  };

  static constexpr uint16_t BUTTON_SIZE_X = 50;
  static constexpr uint16_t BUTTON_SIZE_Y = 36;

  TouchButton(uint16_t x_coord, uint16_t y_coord, std::function<void(TouchButton*)> draw, std::function<void(TouchButton *b)> clicked = [](TouchButton *b){}, std::function<void(TouchButton *b)> longPressed = [](TouchButton *b){});
  void processPressed();
  void drawNow();
  void draw(const std::string label, const std::string sub, Color Color);
  void setSelected(bool selected);
  void clear(uint16_t color);

  static void drawButton(uint16_t x, uint16_t y, const std::string label, const std::string sub, Color color);
  static bool isPressed(uint16_t x, uint16_t y);
  static ColorCombo getColors(Color color);
  static void clearButton(uint16_t x, uint16_t y, uint16_t color);

private:
  bool isSelected = false;
  uint16_t x;
  uint16_t y;
  std::function<void(TouchButton*)> drawHandler{};
  std::function<void(TouchButton*)> clickedHandler{};
  std::function<void(TouchButton*)> longPressedHandler{};
  PressedState pressedState;

};

#endif //TOUCHBUTTON_H