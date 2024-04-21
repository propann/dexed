#ifndef EDITABLEVALUE_H
#define EDITABLEVALUE_H

#include <stdio.h>
#include <vector>
#include <functional>

class EditableValueBase {
public:
  virtual EditableValueBase* cycle() = 0;
  virtual bool next() = 0;
  virtual bool previous() = 0;
};

template<class T> class EditableValue : public EditableValueBase {

public:
  EditableValue(T &value, std::vector<T> values, T defaultValue, std::function<void(void)> changed = [](){});
  EditableValue(T &value, T min, T max, T increment, T defaultValue, std::function<void(void)> changed = [](){});

  EditableValueBase* cycle();
  bool next(void);
  bool previous(void);
  char* toString(void);
  T getValue(void);

private:
  void updateIterator(void);
  char charBuffer[6];
  enum Mode {
    MODE_FIXED,
    MODE_RANGE
  };
  Mode mode;
  typename std::vector<T>::iterator it;
  T &value;
  std::vector<T> values;

  T rangeMin;
  T rangeMax;
  T rangeIncrement;

  std::function<void()> changedHandler{};
};
template class EditableValue<uint16_t>;
template class EditableValue<uint8_t>;
template class EditableValue<int8_t>;
#endif //EDITABLEVALUE_H