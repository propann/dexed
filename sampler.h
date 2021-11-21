/*
   MicroDexed

   MicroDexed is a port of the Dexed sound engine
   Dexed ist heavily based on https://github.com/google/music-synthesizer-for-android

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
   LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
   OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
   THE SOFTWARE.
*/

#include <Arduino.h>
#include "config.h"
#include "sampleset.h"

#ifndef _SAMPLER_H
#define _SAMPLER_H

typedef struct sampler_config_s {
  uint8_t sampler_class; // Type of sample
  uint8_t midinote;   // Triggered by note
  char name[SAMPLER_NAME_LEN];
  const unsigned int* sample_data;
  char shortname[2];  // 1 char name for sequencer
  float32_t pitch;    // play pitch
  float32_t pan;      // panorama (-1.0 - +1.0)
  float32_t vol_max;  // max. volume (0.0 - 1.0)
  float32_t vol_min;  // min. volume (0.0 - 1.0, should be <= vol_max)
  float32_t reverb_send; // how much signal to send to the reverb (0.0 - 1.0)
} sampler_config_t;

enum {SAMPLER_NONE, SAMPLER_BASS, SAMPLER_SNARE, SAMPLER_HIHAT, SAMPLER_HANDCLAP, SAMPLER_RIDE, SAMPLER_CHRASH, SAMPLER_LOWTOM, SAMPLER_MIDTOM, SAMPLER_HIGHTOM, SAMPLER_PERCUSSION, SAMPLER_MONOPHONE, SAMPLER_POLYPHONE};

// DEFAULT MIDI CHANNEL FOR SAMPLER
uint8_t sampler_midi_channel = 10;

sampler_config_t sampler_config[NUM_SAMPLESET_CONFIG] =
{
  {
    SAMPLER_BASS,
    MIDI_C3,
    "bd01",
    AudioSampleBd01,
    "B",
    0.0,
    0.0,
    0.8,
    0.0,
    0.0
  },
 
  {
    SAMPLER_NONE,
    0,
    "EMPTY",
    NULL,
    "-",
    0.0,
    0.0,
    0.0,
    0.0,
    0.0
  }
};

#endif
