#include "valuebutton.h"

template <class T>
ValueButton<T>::ValueButton(int16_t x_coord, int16_t y_coord, EditableValue<T>* value, std::function<void(TouchButton*)> draw, std::function<void(void)> clicked) :
  TouchButton(x_coord, y_coord, draw, [ value, clicked ](){
    value->pressed();
    clicked();
  }),
  v(value)
{

}
