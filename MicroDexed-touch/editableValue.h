#ifndef EDITABLEVALUE_H
#define EDITABLEVALUE_H

#include <stdio.h>
#include <vector>

template<class T> class EditableValue {

public:
  EditableValue(T* value, std::vector<T> values);

  void next();

private:
    typename std::vector<T>::iterator it;
    T* value;
    std::vector<T> values;
};

template class EditableValue<uint8_t>;
#endif //EDITABLEVALUE_H