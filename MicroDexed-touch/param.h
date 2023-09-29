#pragma once

class Param {
public:
  const uint8_t size, count;
  const char* name;
  const int32_t def,min,max;

  Param(uint8_t _size=1, uint8_t _count=1, const char* _name="", int32_t _min=0, int32_t _max=99, int32_t _default=50) : size(_size), count(_count), name(_name), def(_default), min(_min), max(_max) {};

  Param* next() {
    if(size == 0) return NULL;
    return (Param*)(((char*)this) + sizeof(Param) + size * count );
  };
  int32_t get() {
    char* ptr = ((char*)this) + sizeof(Param);
    if(size == 1) return *((uint8_t*)ptr);
    if(size == 2) return *((uint16_t*)ptr);
    if(size == 4) return *((int32_t*)ptr);
    return 0;
  };
  void set(int32_t value) {
    char* ptr = ((char*)this) + sizeof(Param);
    if(size == 1) *((uint8_t*)ptr) = value;
    if(size == 2) *((uint16_t*)ptr) = value;
    if(size == 4) *((uint32_t*)ptr) = value;
  };
  void check() {
    int32_t val = get();
    val = val < min ? min : val > max ? max : val;
  };
} __attribute__((packed));

#define P_uint8_t(name,min,max,def) \
  Param name##_meta{1, 1, #name, min, max, def}; uint8_t name = def

#define P_uint16_t(name,min,max,def) \
  Param name##_meta{2, 1, #name,min,max,def}; uint16_t name = def

#define P_int32_t(name,min,max,def) \
  Param name##_meta{4, 1, #name,min,max,def}; int32_t name = def

#define P_end Param _p_end{0,0};

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



