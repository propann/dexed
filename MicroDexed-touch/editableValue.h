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
  EditableValue(T &value, T defaultValue, std::function<void(EditableValue<T> *v)> changed);
  virtual EditableValueBase* cycle() = 0;
  virtual bool next(void) = 0;
  virtual bool previous(void) = 0;
  char* toString(void);
  T getValue(void);

private:
  char charBuffer[6];

protected:
  bool checkChanged(T result);
  T &value;
  std::function<void(EditableValue *v)> changedHandler{};
};

template<class T> class EditableValueRange : public EditableValue<T> {
public:
  EditableValueRange(T &value, T min, T max, T increment, T defaultValue, std::function<void(EditableValue<T> *v)> changed) :
  EditableValue<T>(value, defaultValue, changed), rangeMin(min), rangeMax(max), rangeIncrement(increment), v(value) {
  }
  EditableValueBase* cycle(void) override;
  bool next(void) override;
  bool previous(void) override;

private:
  T rangeMin;
  T rangeMax;
  T rangeIncrement;
  T &v;
};

template<class T> class EditableValueVector : public EditableValue<T> {
public:
  EditableValueVector(T &value, std::vector<T> values, T defaultValue, std::function<void(EditableValue<T> *v)> changed) :
  EditableValue<T>(value, defaultValue, changed), valuesVector(values), v(value) {
    if(valuesVector.empty()) {
      valuesVector.push_back(0);
    }
    updateIterator();  
  }
  EditableValueBase* cycle(void) override;
  bool next(void) override;
  bool previous(void) override;

private:
  void updateIterator(void);
  std::vector<T> valuesVector;
  typename std::vector<T>::iterator it;
  T &v;
};

template class EditableValue<uint16_t>;
template class EditableValue<uint8_t>;
template class EditableValue<int8_t>;

template class EditableValueVector<uint16_t>;
template class EditableValueVector<uint8_t>;
template class EditableValueVector<int8_t>;

template class EditableValueRange<uint16_t>;
template class EditableValueRange<uint8_t>;
template class EditableValueRange<int8_t>;

#endif //EDITABLEVALUE_H