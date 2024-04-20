#ifndef VALUEBUTTON_H
#define VALUEBUTTON_H

#include <stdio.h>
#include <string>
#include <functional>
#include "editableValue.h"
#include "touchbutton.h"

template<class T> class ValueButton : public TouchButton {
public:
  EditableValue<T> *v;
  ValueButton(int16_t x_coord, int16_t y_coord, EditableValue<T> *value, std::function<void(TouchButton*, EditableValue<T>*)> draw, std::function<void(void)> clicked = 0);
  
private:
  
};
template class ValueButton<uint16_t>;
template class ValueButton<uint8_t>;
template class ValueButton<int8_t>;
#endif //VALUEBUTTON_H