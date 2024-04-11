#ifndef EDITABLEVALUE_H
#define EDITABLEVALUE_H

#include <stdio.h>
#include <vector>

class EditableValueBase {
public:
  virtual EditableValueBase* pressed() = 0;
  virtual bool next() = 0;
  virtual bool previous() = 0;
};

template<class T> class EditableValue : EditableValueBase {

public:
  EditableValue(T* value, std::vector<T> values, T defaultValue);
  EditableValue(T* value, T min, T max, T increment, T defaultValue);

  EditableValueBase* pressed();
  bool next();
  bool previous();
  char* toString(void);

private:
  char charBuffer[6];
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
template class EditableValue<uint16_t>;
template class EditableValue<uint8_t>;
template class EditableValue<int8_t>;
#endif //EDITABLEVALUE_H