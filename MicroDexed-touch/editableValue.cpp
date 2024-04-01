#include "editableValue.h"

template <class T>
EditableValue<T>::EditableValue(T* invalue, std::vector<T> invalues) : mode(EditableValue::MODE_FIXED), value(invalue), values(invalues) {
  if(values.empty()) {
    values.push_back(0);
  }
  it = values.begin();
  *value = *it;
}

template <class T>
EditableValue<T>::EditableValue(T* invalue, T min, T max, T increment) : mode(EditableValue::MODE_RANGE), value(invalue), rangeMin(min), rangeMax(max), rangeIncrement(increment) {
  *value = min;
}

template <class T>
void EditableValue<T>::next() {
  T result = *value;
  switch(mode) {
  case MODE_FIXED:
    it++;
    if(it == values.end()) {
        it = values.begin();
    }
    result = *it;
    break;
  case MODE_RANGE:
    if(result + rangeIncrement > rangeMax) {
      result = rangeMin;
    } else {
      result += rangeIncrement;
    }
    break;
  }
  *value = result;
}


