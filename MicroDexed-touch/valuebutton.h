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
  ValueButton(ActiveValue *active, uint16_t x_coord, uint16_t y_coord, EditableValue<T> *value, std::function<void(TouchButton*, EditableValue<T>*)> draw);
};

template<class T> class ValueButtonVector : public ValueButton<T> {
public:
  ValueButtonVector(ActiveValue *active, uint16_t x_coord, uint16_t y_coord, T &invalue, std::vector<T> invalues, T defaultValue, std::function<void(TouchButton*, EditableValue<T>*)> draw, std::function<void(EditableValue<T>*)> changed = 0) :
  ValueButton<T>(active, x_coord, y_coord, new EditableValueVector<T>(invalue, invalues, defaultValue, [ draw, changed, this ](EditableValue<T> *v) { if(changed != 0) { changed(v); } draw(this, v); }), draw) {}
};

template<class T> class ValueButtonRange : public ValueButton<T> {
public:
  ValueButtonRange(ActiveValue *active, uint16_t x_coord, uint16_t y_coord, T &invalue, T min, T max, T increment, T defaultValue, std::function<void(TouchButton*, EditableValue<T>*)> draw, std::function<void(EditableValue<T>*)> changed = 0) :
  ValueButton<T>(active, x_coord, y_coord, new EditableValueRange<T>(invalue, min, max, increment, defaultValue, [ draw, changed, this ](EditableValue<T> *v) { if(changed != 0) { changed(v); } draw(this, v); }), draw) {}
};

template class ValueButtonVector<uint16_t>;
template class ValueButtonVector<uint8_t>;
template class ValueButtonVector<int8_t>;

template class ValueButtonRange<uint16_t>;
template class ValueButtonRange<uint8_t>;
template class ValueButtonRange<int8_t>;
#endif //VALUEBUTTON_H