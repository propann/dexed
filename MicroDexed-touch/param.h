/*
  param.h - unified struct field parameters

  This file provide a small class Param, which can be incooperated in any struct 
  to add a descriptor to the next field.

  These descriptors provide metadata (such as a name, default, minimal and maximal value)
  that can be used at runtime to access the structs field in a generic way, iterate over them
  for example to load and store them into files, list them on screen etc. by generic code.


  For example:
  
  let's look at the struct

    typedef struct{
      uint8_t some_byte = 5;
      int32_t some_int = 0;
    } something_t;

  we now change it to
  
    typedef struct : public Params{
      Puint8_t (some_byte, 0 ,10, 5);     // meaning "some_byte" has the range 0 - 10, and default value 5.
      Pint32_t (some_int, -1000, 1000, 0);
      Pend; // mark end, if parameters are iterated
    } something_t;

  We can now do things like this:

    something_t something;
    cout << something.some_byte; // prints 5, as given default value.

    Param* param = something.getParams(); // refers Param at start of something_t - that is something.some_byte
    cout << param->get(); // prints the value 5 of something.some_byte

    cout << param->desc->name;   // prints "some_byte"

    param->set(2); // something.some_byte is now 2.
    
    param->set(17); // something.some_byte is now 10, as it is limited to the range 0-10.
    
    param = param->next();  // get next one: param now refers to something.some_int.

    cout << param->desc->name;   // prints "some_int"

    param->set(500);  // something.some_int is now 500.

    param->set(param->desc->default);  // something.some_int is now 0 again.

    param = param->next();  // param is now NULL, as there is no more Parameter (marked by "Pend" in the struct)
    
    param = something.getParam(&something.some_int);  // param now refers to something.some_int again.

*/

#pragma once

#include <math.h>

enum ParamType {
  P_END=0, P_UINT8_T, P_INT8_T, P_UINT16_T, P_UINT32_T, P_INT32_T, P_FLOAT, P_TYPE_COUNT
};
const uint8_t param_sizes[P_TYPE_COUNT]={0,sizeof(uint8_t),sizeof(int8_t),sizeof(uint16_t),sizeof(uint32_t),sizeof(int32_t),sizeof(float)};

class 
Descriptor {
public:
  const ParamType type;

  const uint32_t count;
  const char* name;
  const float def,min,max;

  Descriptor(ParamType _type=P_UINT8_T, uint32_t _count=1, const char* _name="", float _min=0, float _max=99, float _default=50) : type(_type), count(_count), name(_name), def(_default), min(_min), max(_max) {};
};

class Param {
public:
  Descriptor* desc;

  Param(ParamType _type=P_UINT8_T, uint32_t _count=1, const char* _name="", float _min=0, float _max=99, float _default=50) : desc(
    new Descriptor(_type, _count, _name, _min, _max, _default )
  ) {
    // fill default values
    for(uint32_t i=0; i<_count; i++)
      set(_default,i);
  };

  Param* next() {
    // skip the Param struct and size of data described by it
    uintptr_t rawPtr = (uintptr_t)this + sizeof(Param) + param_sizes[desc->type] * desc->count;
    // round to next 4 byte border to handle struct padding
    uintptr_t max_padding = sizeof(rawPtr) - 1;
    Param * ptr = (Param*)((rawPtr + max_padding) & ~max_padding);
    
    if(ptr->desc->type != P_END) return ptr;
    else                         return NULL;
  };

  float get(uint32_t i=0) {
    uintptr_t ptr = ((uintptr_t)this) + sizeof(Param) + i * param_sizes[desc->type];
    if(desc->type == P_UINT8_T) return *((uint8_t*)ptr);
    if(desc->type == P_UINT16_T) return *((uint16_t*)ptr);
    if(desc->type == P_UINT32_T) return *((uint32_t*)ptr);
    if(desc->type == P_INT32_T) return *((int32_t*)ptr);
    if(desc->type == P_FLOAT) return *((float*)ptr);
    return 0;
  };

  void set(float value, uint32_t i=0) {
    uintptr_t ptr = ((uintptr_t)this) + sizeof(Param) + i * param_sizes[desc->type];
    if(value < desc->min) value = desc->min;
    if(value > desc->max) value = desc->max;
    if(desc->type == P_UINT8_T) *((uint8_t*)ptr) = round(value);
    if(desc->type == P_UINT16_T) *((uint16_t*)ptr) = round(value);
    if(desc->type == P_UINT32_T) *((uint32_t*)ptr) = round(value);
    if(desc->type == P_INT32_T) *((int32_t*)ptr) = round(value);
    if(desc->type == P_FLOAT) *((float*)ptr) = value;
  };
  void check() {
    float val = get();
    if(val < desc->min) set(desc->min);
    if(val > desc->max) set(desc->max);
  };
} __attribute__ ((aligned (4)));

#define P_bool(name,def) \
  Param name##_desc{P_UINT8_T, 1, #name, 0, 1, def}; uint8_t name = def

#define P_uint8_t(name,min,max,def) \
  Param name##_desc{P_UINT8_T, 1, #name, min, max, def}; uint8_t name = def

#define P_uint16_t(name,min,max,def) \
  Param name##_desc{P_UINT16_T, 1, #name,min,max,def}; uint16_t name = def

#define P_int32_t(name,min,max,def) \
  Param name##_desc{P_INT32_T, 1, #name,min,max,def}; int32_t name = def

#define P_uint32_t(name,min,max,def) \
  Param name##_desc{P_UINT32_T, 1, #name,min,max,def}; int32_t name = def

#define P_float(name,min,max,def) \
  Param name##_desc{P_FLOAT, 1, #name,min,max,def}; float name = def

#define P_end Param _p_end_desc{P_END};

class Params {
public:
  Param* getParams() {
    return (Param*) this;
  };
  static Param* getParam(void* field) {
    return (Param*)((char*)field - sizeof(Param));
  };
  static void check(void* field) {
    getParam(field)->check();
  };
  void check() {

    Param* prm = getParams();
    do {
      prm->check();
      prm = prm->next();
    } while (prm != NULL);
  }
};



