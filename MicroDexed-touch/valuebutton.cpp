#include "valuebutton.h"

template <class T>
ValueButton<T>::ValueButton(int16_t x_coord, int16_t y_coord, EditableValue<T>* value, std::function<void(TouchButton*, EditableValue<T>*)> draw, std::function<void(void)> clicked) :
  
  TouchButton(x_coord, y_coord,
  [ draw, value ](TouchButton *b) { // draw handler
    draw(b, value);
  },
  [ value, clicked ]() { // clicked handler
    value->pressed();
    if(clicked) {
      clicked();
    }
  }),
  v(value)
{

}
