#ifndef VALUEBUTTON_H
#define VALUEBUTTON_H

#include <stdio.h>
#include <string>
#include <functional>
#include "editableValue.h"
#include "touchbutton.h"

template<class T> class ValueButton : public TouchButton {
public:
  ValueButton(int16_t x_coord, int16_t y_coord, EditableValue<T> *value, std::function<void(TouchButton*, EditableValue<T>*)> draw);

  ValueButton(int16_t x_coord, int16_t y_coord, T &invalue, std::vector<T> invalues, T defaultValue, std::function<void(TouchButton*, EditableValue<T>*)> draw);

private:
  EditableValue<T> v;
  T inval;
};
template class ValueButton<uint16_t>;
template class ValueButton<uint8_t>;
template class ValueButton<int8_t>;
#endif //VALUEBUTTON_H