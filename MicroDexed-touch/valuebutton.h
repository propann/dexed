#ifndef VALUEBUTTON_H
#define VALUEBUTTON_H

#include <stdio.h>
#include <string>
#include <functional>
#include "editableValue.h"
#include "touchbutton.h"

struct ActiveValue {
  EditableValueBase *valueBase;
  TouchButton *button;
};

template<class T> class ValueButton : public TouchButton {
public:
  ValueButton(ActiveValue *active, int16_t x_coord, int16_t y_coord, T &invalue, std::vector<T> invalues, T defaultValue, std::function<void(TouchButton*, EditableValue<T>*)> draw);
  ValueButton(ActiveValue *active, int16_t x_coord, int16_t y_coord, T &invalue, T min, T max, T increment, T defaultValue, std::function<void(TouchButton*, EditableValue<T>*)> draw);

private:
  EditableValue<T> v;
};
template class ValueButton<uint16_t>;
template class ValueButton<uint8_t>;
template class ValueButton<int8_t>;
#endif //VALUEBUTTON_H