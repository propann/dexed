#include "valuebutton.h"

template <class T>
ValueButton<T>::ValueButton(int16_t x_coord, int16_t y_coord, T &invalue, std::vector<T> invalues, T defaultValue, std::function<void(TouchButton*, EditableValue<T>*)> draw) :
  TouchButton(x_coord, y_coord,
  [ this, draw ](TouchButton *b) { // draw handler
    draw(b, &v);
  },
  [ this ]() { // clicked handler
    v.cycle();
  }),
  v(invalue, invalues, defaultValue,
  [ this, draw ]() { // value changed handler
    draw(this, &v);
  })
{
}

template <class T>
ValueButton<T>::ValueButton(int16_t x_coord, int16_t y_coord, T &invalue, T min, T max, T increment, T defaultValue, std::function<void(TouchButton*, EditableValue<T>*)> draw) :
  TouchButton(x_coord, y_coord,
  [ this, draw ](TouchButton *b) { // draw handler
    draw(b, &v);
  },
  [ this ]() { // clicked handler
    v.cycle();
  }),
  v(invalue, min, max, increment, defaultValue,
  [ this, draw ]() { // value changed handler
    draw(this, &v);
  })
{
}
