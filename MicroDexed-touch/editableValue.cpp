#include "editableValue.h"

template <class T>
EditableValue<T>::EditableValue(T* invalue, std::vector<T> invalues) : value(invalue), values(invalues) {
  it = values.begin();
  *value = *it;
}

template <class T>
void EditableValue<T>::next() {
    it++;
    if(it == values.end()) {
        it = values.begin();
    }
    *value = *it;
}


