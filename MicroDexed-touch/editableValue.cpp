#include "editableValue.h"
#include <algorithm>

template <class T>
EditableValue<T>::EditableValue(T &invalue, std::vector<T> invalues, T defaultValue) : mode(EditableValue::MODE_FIXED), value(invalue), values(invalues) {
  if(values.empty()) {
    values.push_back(0);
  }
  it = std::find(values.begin(), values.end(), defaultValue);
  if(it == values.end()) {
      it = values.begin();
  }
  value = defaultValue;
}

template <class T>
EditableValue<T>::EditableValue(T &invalue, T min, T max, T increment, T defaultValue) : mode(EditableValue::MODE_RANGE), value(invalue), rangeMin(min), rangeMax(max), rangeIncrement(increment) {
  value = defaultValue;
}

template <class T>
char* EditableValue<T>::toString(void) {
  return itoa(value, charBuffer, 10);
}

template <class T>
T EditableValue<T>::getValue(void) {
  return value;
}

template <class T>
EditableValueBase* EditableValue<T>::pressed() {
  T result = value;
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
  value = result;
  return this;
}

template <class T>
bool EditableValue<T>::next() {
  T result = value;
  switch(mode) {
  case MODE_FIXED:
    if(++it == values.end()) {
      --it;
    }
    result = *it;
    break;
  case MODE_RANGE:
    if(result + rangeIncrement <= rangeMax) {
      result += rangeIncrement;
    }
    break;
  }
  const bool changed = (value != result);
  value = result;
  return changed;
}

template <class T>
bool EditableValue<T>::previous() {
  T result = value;
  switch(mode) {
  case MODE_FIXED:
    if(it != values.begin()) {
      --it;
    }
    result = *it;
    break;
  case MODE_RANGE:
    if(result - rangeIncrement >= rangeMin) {
      result -= rangeIncrement;
    }
    break;
  }
  const bool changed = (value != result);
  value = result;
  return changed;
}


