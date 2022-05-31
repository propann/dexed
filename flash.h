/*
   MicroDexed

   MicroDexed is a port of the Dexed sound engine
   Dexed ist heavily based on https://github.com/google/music-synthesizer-for-an
  droid

   (c)2018-2021 H. Wirtz <wirtz@parasitstudio.de>
   (c)2021      H. Wirtz <wirtz@parasitstudio.de>, M. Koslowski <positionhigh@gmx.de>

   Permission is hereby granted, free of charge, to any person obtaining a copy
   of this software and associated documentation files (the "Software"), to deal
   in the Software without restriction, including without limitation the rights
   to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
   copies of the Software, and to permit persons to whom the Software is
   furnished to do so, subject to the following conditions:

   The above copyright notice and this permission notice shall be included in
   all copies or substantial portions of the Software.

   THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
   IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
   FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
   AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
   THE SOFTWARE.
*/

#ifndef _FLASH_H
#define _FLASH_H

#include "config.h"
#include "sequencer.h"
#include "SerialFlash.h"

#define MAX_FLASH_FILENAME_LEN 16

extern multisample_zone_t msz[NUM_MULTISAMPLES][NUM_MULTISAMPLE_ZONES];

void fill_sample_zone_from_flash (const uint16_t entry_number, const uint8_t preset_number, const uint8_t zone_number);

#endif
