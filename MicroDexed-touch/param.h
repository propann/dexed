#pragma once

class Param {
public:
  const uint8_t size, count;
  const char* name;
  const int32_t def,min,max;

  Param(uint8_t _size=1, uint8_t _count=1, const char* _name="", int32_t _min=0, int32_t _max=99, int32_t _default=50) : size(_size), count(_count), name(_name), def(_default), min(_min), max(_max) {};

  Param* next() {
    if(size == 0) return NULL;
    return (Param*)(((void*)this) + sizeof(Param) + size * count );
  }
  int32_t get() {
    void* ptr = ((void*)this) + sizeof(Param);
    if(size == 1) return *((uint8_t*)ptr);
    if(size == 2) return *((uint16_t*)ptr);
    if(size == 4) return *((int32_t*)ptr);
    return 0;
  }
  void set(int32_t value) {
    void* ptr = ((void*)this) + sizeof(Param);
    if(size == 1) *((uint8_t*)ptr) = value;
    if(size == 2) *((uint16_t*)ptr) = value;
    if(size == 4) *((uint32_t*)ptr) = value;
  }
} __attribute__((packed));

#define P_uint8_t(name) \
  Param name##_meta{1, 1, #name}; uint8_t name

#define P_uint16_t(name) \
  Param name##_meta{2, 1, #name}; uint16_t name

#define P_int32_t(name) \
  Param name##_meta{4, 1, #name}; int32_t name

#define P_end Param _p_end{0,0};

class Params {
public:
  Param* getParams() {
    return (Param*) this;
  }
} __attribute__((packed));

