#include "editableValue.h"
#include <algorithm>

template <class T>
EditableValue<T>::EditableValue(T &invalue, T defaultValue, std::function<void(EditableValue<T> *v)> changed) : value(invalue), changedHandler{changed} {
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
bool EditableValue<T>::checkChanged(T result) {
  const bool changed = (value != result);
  value = result;
  if(changed) {
    changedHandler(this);
  }
  return changed;
}

template <class T>
void EditableValueVector<T>::updateIterator(void) {
  it = std::find(valuesVector.begin(), valuesVector.end(), v);
  if(it == valuesVector.end()) {
    it = valuesVector.begin();
  }
}

template <class T>
bool EditableValueRange<T>::previous(void) {
  changedHandler(this);
  T result = v;
  if(result - rangeIncrement >= rangeMin) {
    result -= rangeIncrement;
  }
  return checkChanged(result);
}

template <class T>
bool EditableValueRange<T>::next(void) {
  T result = v;
  if(result + rangeIncrement <= rangeMax) {
    result += rangeIncrement;
  }
  return checkChanged(result);
}

template <class T>
EditableValueBase* EditableValueRange<T>::cycle(void) {
  T result = v;
  if(next() == false) {
    v = rangeMin;
  }
  checkChanged(result);
  return this;
}

template <class T>
bool EditableValueVector<T>::previous(void) {
  if(it != valuesVector.begin()) {
    --it;
  }
  return checkChanged(*it);
}

template <class T>
bool EditableValueVector<T>::next(void) {
  if(++it == valuesVector.end()) {
    --it;
  }
  return checkChanged(*it);
}

template <class T>
EditableValueBase* EditableValueVector<T>::cycle(void) {
  if(next() == false) {
    it = valuesVector.begin();
  }
  checkChanged(*it);
  return this;
}
