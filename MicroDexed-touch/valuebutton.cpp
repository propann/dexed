#include "valuebutton.h"
#include "config.h"

template <class T>
ValueButton<T>::ValueButton(ActiveValue *active, int16_t x_coord, int16_t y_coord, EditableValue<T> *v, std::function<void(TouchButton*, EditableValue<T>*)> draw) :
  TouchButton(x_coord, y_coord,
  [ this, draw, v ](TouchButton *b) { // draw handler
    draw(b, v);
  },
  [ this, active, v ](TouchButton *b) mutable { // clicked handler
    if(active->valueBase == v) {
      v->cycle();
    } else {
      if(active->valueBase != nullptr) {
        (*active).button->setSelected(false);        
      }
      (*active).valueBase = v;
      DBG_LOG(printf("selected new\n"));
      (*active).button = b;
      b->setSelected(true);
    }
  })
{
}
