#pragma once



class Descriptor {
public:
  const enum Type {
    P_END=0, P_UINT8_T, P_UINT16_T, P_INT32_T, P_FLOAT, P_TYPE_COUNT
  } type;
  const uint8_t sizes[P_TYPE_COUNT]={0,sizeof(uint8_t),sizeof(uint16_t),sizeof(int32_t),sizeof(float)};

  const uint8_t count;
  const char* name;
  const float def,min,max;

  Descriptor(Type _type=P_UINT8_T, uint8_t _count=1, const char* _name="", float _min=0, float _max=99, float _default=50) : type(_type), count(_count), name(_name), def(_default), min(_min), max(_max) {};
};

class Param {
public:
  Descriptor* desc;

  Param(Descriptor::Type _type=Descriptor::P_UINT8_T, uint8_t _count=1, const char* _name="", float _min=0, float _max=99, float _default=50) : desc(
    new Descriptor(_type, _count, _name, _min, _max, _default )
  ) {};

  Param* next() {
    Param* ptr = (Param*)(((char*)this) + sizeof(Param) + 4 * desc->count );
    if(ptr->desc->type != Descriptor::P_END) return ptr;
    else                         return NULL;
  };
  float get() {
    char* ptr = ((char*)this) + sizeof(Param);
    if(desc->type == Descriptor::P_UINT8_T) return *((uint8_t*)ptr);
    if(desc->type == Descriptor::P_UINT16_T) return *((uint16_t*)ptr);
    if(desc->type == Descriptor::P_INT32_T) return *((int32_t*)ptr);
    if(desc->type == Descriptor::P_FLOAT) return *((float*)ptr);
    return 0;
  };
  void set(float value) {
    char* ptr = ((char*)this) + sizeof(Param);
    if(desc->type == Descriptor::P_UINT8_T) *((uint8_t*)ptr) = round(value);
    if(desc->type == Descriptor::P_UINT16_T) *((uint16_t*)ptr) = round(value);
    if(desc->type == Descriptor::P_INT32_T) *((uint32_t*)ptr) = round(value);
    if(desc->type == Descriptor::P_FLOAT) *((float*)ptr) = value;
  };
  void check() {
    float val = get();
    if(val < desc->min) set(desc->min);
    if(val > desc->max) set(desc->max);
    
  };
} __attribute__ ((aligned (4)));

#define P_bool(name,min,max,def) \
  Param name##_meta{Descriptor::P_UINT8_T, 1, #name, min, max, def}; uint8_t name = def

#define P_uint8_t(name,min,max,def) \
  Param name##_meta{Descriptor::P_UINT8_T, 1, #name, min, max, def}; uint8_t name = def

#define P_uint16_t(name,min,max,def) \
  Param name##_meta{Descriptor::P_UINT16_T, 1, #name,min,max,def}; uint16_t name = def

#define P_int32_t(name,min,max,def) \
  Param name##_meta{Descriptor::P_INT32_T, 1, #name,min,max,def}; int32_t name = def

#define P_float(name,min,max,def) \
  Param name##_meta{Descriptor::P_FLOAT, 1, #name,min,max,def}; float name = def

#define P_end Param _p_end{Descriptor::P_END};

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



