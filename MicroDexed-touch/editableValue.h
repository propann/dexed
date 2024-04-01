#ifndef EDITABLEVALUE_H
#define EDITABLEVALUE_H

#include <stdio.h>
#include <vector>

template<class T> class EditableValue {

public:
  EditableValue(T* value, std::vector<T> values, T defaultValue);
  EditableValue(T* value, T min, T max, T increment, T defaultValue);

  void next();

private:
  enum Mode {
    MODE_FIXED,
    MODE_RANGE
  };
  Mode mode;
  typename std::vector<T>::iterator it;
  T* value;
  std::vector<T> values;

  T rangeMin;
  T rangeMax;
  T rangeIncrement;
};

template class EditableValue<uint8_t>;
template class EditableValue<int8_t>;
#endif //EDITABLEVALUE_H