#include "editableValue.h"
#include <algorithm>

template <class T>
EditableValue<T>::EditableValue(T &invalue, std::vector<T> invalues, T defaultValue, std::function<void(void)> changed) : mode(EditableValue::MODE_FIXED), value(invalue), values(invalues), changedHandler{changed} {
  if(values.empty()) {
    values.push_back(0);
  }
  value = defaultValue;
  updateIterator();
}

template <class T>
EditableValue<T>::EditableValue(T &invalue, T min, T max, T increment, T defaultValue, std::function<void(void)> changed) : mode(EditableValue::MODE_RANGE), value(invalue), rangeMin(min), rangeMax(max), rangeIncrement(increment), changedHandler{changed} {
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
  updateIterator();
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
  changedHandler();
  return this;
}

template <class T>
bool EditableValue<T>::next() {
  updateIterator();
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
  if(changed) {
    changedHandler();
  }
  value = result;
  return changed;
}

template <class T>
bool EditableValue<T>::previous() {
  updateIterator();
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
  if(changed) {
    changedHandler();
  }
  value = result;
  return changed;
}

template <class T>
void EditableValue<T>::updateIterator(void) {
  it = std::find(values.begin(), values.end(), value);
  if(it == values.end()) {
    it = values.begin();
  }
}


