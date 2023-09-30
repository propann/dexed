#pragma once



class Param {
public:
  const uint8_t size, count;
  const char* name;
  const int32_t def,min,max;

  const enum Type {
    P_END, P_UINT8_T, P_UINT16_T, P_INT32_T, P_FLOAT, P_TYPE_COUNT
  } type;
  const uint8_t sizes[P_TYPE_COUNT]={0,1,2,4,4};

  Param(Type _type=P_UINT8_T, uint8_t _count=1, const char* _name="", int32_t _min=0, int32_t _max=99, int32_t _default=50) : type(_type), count(_count), name(_name), def(_default), min(_min), max(_max) {};

  Param* next() {
    Param* ptr = (Param*)(((char*)this) + sizeof(Param) + sizes[type] * count );
    if(ptr->type != P_END) return ptr;
    else                   return NULL;
  };
  template<typename T=float> T get() {
    char* ptr = ((char*)this) + sizeof(Param);
    if(type == P_UINT8_T) return *((uint8_t*)ptr);
    if(type == P_UINT16_T) return *((uint16_t*)ptr);
    if(type == P_INT32_T) return *((int32_t*)ptr);
    if(type == P_FLOAT) return *((float*)ptr);
    return 0;
  };
  template<typename T> set(T value) {
    char* ptr = ((char*)this) + sizeof(Param);
    if(type == P_UINT8_T) *((uint8_t*)ptr) = value;
    if(type == P_UINT16_T) *((uint16_t*)ptr) = value;
    if(type == P_INT32_T) *((uint32_t*)ptr) = value;
    if(type == P_FLOAT) *((float*)ptr) = value;
  };
  void check() {
    int32_t val = get<int32_t>();
    if(val < min) set<int32_t>(min);
    if(val > max) set<int32_t>(max);
    
  };
} __attribute__((packed));

#define P_bool(name,min,max,def) \
  Param name##_meta{Param::P_UINT8_T, 1, #name, min, max, def}; uint8_t name = def

#define P_uint8_t(name,min,max,def) \
  Param name##_meta{Param::P_UINT8_T, 1, #name, min, max, def}; uint8_t name = def

#define P_uint16_t(name,min,max,def) \
  Param name##_meta{Param::P_UINT16_T, 1, #name,min,max,def}; uint16_t name = def

#define P_int32_t(name,min,max,def) \
  Param name##_meta{Param::P_INT32_T, 1, #name,min,max,def}; int32_t name = def

#define P_float(name,min,max,def) \
  Param name##_meta{Param::P_FLOAT, 1, #name,min,max,def}; float name = def

#define P_end Param _p_end{Param::P_END};

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
} __attribute__((packed));



