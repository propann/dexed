#ifndef tests_h_
#define tests_h_

#include "Arduino.h"

extern "C" uint8_t external_psram_size;

bool psram_memory_ok = false;
uint32_t *memory_begin, *memory_end;

bool psram_check_fixed_pattern(uint32_t pattern);
bool psram_check_lfsr_pattern(uint32_t seed);

#endif
