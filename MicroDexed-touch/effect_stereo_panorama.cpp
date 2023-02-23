/*
   Copyright (c) 2021 H. Wirtz

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
#include <Audio.h>
#include "limits.h"
#include "effect_stereo_panorama.h"

// Written by Holger Wirtz
// 20211124 - initial version

static const audio_block_t zeroblock = {
    0, 0, 0, {
                 0,
                 0,
                 0,
                 0,
                 0,
                 0,
                 0,
                 0,
                 0,
                 0,
                 0,
                 0,
                 0,
                 0,
                 0,
                 0,
#if AUDIO_BLOCK_SAMPLES > 16
                 0,
                 0,
                 0,
                 0,
                 0,
                 0,
                 0,
                 0,
                 0,
                 0,
                 0,
                 0,
                 0,
                 0,
                 0,
                 0,
#endif
#if AUDIO_BLOCK_SAMPLES > 32
                 0,
                 0,
                 0,
                 0,
                 0,
                 0,
                 0,
                 0,
                 0,
                 0,
                 0,
                 0,
                 0,
                 0,
                 0,
                 0,
#endif
#if AUDIO_BLOCK_SAMPLES > 48
                 0,
                 0,
                 0,
                 0,
                 0,
                 0,
                 0,
                 0,
                 0,
                 0,
                 0,
                 0,
                 0,
                 0,
                 0,
                 0,
#endif
#if AUDIO_BLOCK_SAMPLES > 64
                 0,
                 0,
                 0,
                 0,
                 0,
                 0,
                 0,
                 0,
                 0,
                 0,
                 0,
                 0,
                 0,
                 0,
                 0,
                 0,
#endif
#if AUDIO_BLOCK_SAMPLES > 80
                 0,
                 0,
                 0,
                 0,
                 0,
                 0,
                 0,
                 0,
                 0,
                 0,
                 0,
                 0,
                 0,
                 0,
                 0,
                 0,
#endif
#if AUDIO_BLOCK_SAMPLES > 96
                 0,
                 0,
                 0,
                 0,
                 0,
                 0,
                 0,
                 0,
                 0,
                 0,
                 0,
                 0,
                 0,
                 0,
                 0,
                 0,
#endif
#if AUDIO_BLOCK_SAMPLES > 112
                 0,
                 0,
                 0,
                 0,
                 0,
                 0,
                 0,
                 0,
                 0,
                 0,
                 0,
                 0,
                 0,
                 0,
                 0,
                 0,
#endif
#if AUDIO_BLOCK_SAMPLES > 128
#error AUDIO_BLOCK_SAMPLES > 128 is a problem for this class
#endif
             }};

#ifndef _MAPFLOAT
#define _MAPFLOAT
inline float mapfloat(float val, float in_min, float in_max, float out_min, float out_max)
{
  return (val - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}
#endif

void AudioEffectStereoPanorama::panorama(float p)
{
  pan = constrain(p, -1.0, 1.0);
}

void AudioEffectStereoPanorama::update(void)
{
  audio_block_t *in[2];
  audio_block_t *out[2];

  in[0] = receiveReadOnly(0);
  if (!in[0])
    in[0] = (audio_block_t *)&zeroblock;

  in[1] = receiveReadOnly(1);
  if (!in[1])
    in[1] = (audio_block_t *)&zeroblock;

  out[0] = allocate();
  out[1] = allocate();

  if (in[0] && in[1] && out[0] && out[1])
  {
    arm_q15_to_float(in[0]->data, in_f[0], AUDIO_BLOCK_SAMPLES);
    arm_q15_to_float(in[1]->data, in_f[1], AUDIO_BLOCK_SAMPLES);

    for (uint16_t n = 0; n < AUDIO_BLOCK_SAMPLES; n++)
    {
      if (pan != 0.0)
      {
        if (pan > 0.0)
        {
          out_f[1][n] = (pan * in_f[0][n]) + ((1.0 - pan) * in_f[1][n]);
          out_f[0][n] = (1.0 - pan) * in_f[0][n];
        }
        else
        {
          float _pan_ = fabs(pan);
          out_f[0][n] = (_pan_ * in_f[1][n]) + ((1.0 - _pan_) * in_f[0][n]);
          out_f[1][n] = (1.0 - _pan_) * in_f[1][n];
        }
      }
      else
      {
        out_f[0][n] = in_f[0][n];
        out_f[1][n] = in_f[1][n];
      }
    }

    arm_float_to_q15(out_f[0], out[0]->data, AUDIO_BLOCK_SAMPLES);
    arm_float_to_q15(out_f[1], out[1]->data, AUDIO_BLOCK_SAMPLES);
  }

  if (in[0] != (audio_block_t *)&zeroblock)
  {
    release(in[0]);
  }
  if (in[1] != (audio_block_t *)&zeroblock)
  {
    release(in[1]);
  }

  if (out[0])
  {
    transmit(out[0], 0);
    release(out[0]);
  }
  if (out[1])
  {
    transmit(out[1], 1);
    release(out[1]);
  }
}
