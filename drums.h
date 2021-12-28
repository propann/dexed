/*
   MicroDexed

   MicroDexed is a port of the Dexed sound engine
   Dexed ist heavily based on https://github.com/google/music-synthesizer-for-an
  droid

   (c)2018-2021 H. Wirtz <wirtz@parasitstudio.de>

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

#include <Arduino.h>

#ifndef _DRUMS_H
#define _DRUMS_H

typedef struct drum_config_s {
  uint8_t drum_class; // Type of drum
  uint8_t midinote;   // Triggered by note
  char name[DRUM_NAME_LEN];
#ifdef COMPILE_FOR_PROGMEM
  const uint8_t* drum_data;
#endif
  char shortname[2];  // 1 char name for sequencer
  uint32_t len;       // number of elements in drum_data
  float32_t pitch;    // variable pitch per note for sequencer
  float32_t p_offset; // "static" pitch offset to correct root note to root of other samples
  float32_t pan;      // Panorama (-1.0 - +1.0)
  float32_t vol_max;  // max. Volume (0.0 - 1.0)
  float32_t vol_min;  // min. Volume (0.0 - 1.0, should be <= vol_max)
  float32_t reverb_send; // how much signal to send to the reverb (0.0 - 1.0)
} drum_config_t;

enum {DRUM_NONE, DRUM_BASS, DRUM_SNARE, DRUM_HIHAT, DRUM_HANDCLAP, DRUM_RIDE, DRUM_CRASH, DRUM_LOWTOM, DRUM_MIDTOM, DRUM_HIGHTOM, DRUM_PERCUSSION, DRUM_POLY};

typedef struct custom_midi_map_s {
  uint8_t type;      // 0 = empty, 1 = Key/Pad, 2 = CC / Values, 3 = Button push on/off
  uint8_t channel;   // Midi Channel
  uint8_t in;        // Midi Input Key/Pad / Value
  uint8_t out;       // Destination Key / Value

} custom_midi_map_t;

#endif
