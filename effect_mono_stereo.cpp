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
#include "effect_mono_stereo.h"

/*************************************************************************/
//                A u d i o E f f e c t M o n o S t e r e o
// Written by Holger Wirtz
// 20191122 - initial version

static const audio_block_t zeroblock = {
  0, 0, 0, {
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
#if AUDIO_BLOCK_SAMPLES > 16
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
#endif
#if AUDIO_BLOCK_SAMPLES > 32
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
#endif
#if AUDIO_BLOCK_SAMPLES > 48
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
#endif
#if AUDIO_BLOCK_SAMPLES > 64
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
#endif
#if AUDIO_BLOCK_SAMPLES > 80
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
#endif
#if AUDIO_BLOCK_SAMPLES > 96
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
#endif
#if AUDIO_BLOCK_SAMPLES > 112
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
#endif
  }
};

#if !defined(_MAPFLOAT)
#define _MAPFLOAT
inline float mapfloat(float val, float in_min, float in_max, float out_min, float out_max)
{
  return (val - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}
#endif

void AudioEffectMonoStereo::panorama(float p)
{
  pan = mapfloat(p, -1.0, 1.0, 1.0, 0.0);
}

void AudioEffectMonoStereo::update(void)
{
  audio_block_t *in;
  audio_block_t *out[2];

  in = receiveReadOnly(0);
  if (!in)
    in = (audio_block_t*)&zeroblock;

  out[0] = allocate();
  out[1] = allocate();

  if (in && out[0] && out[1])
  {
    arm_q15_to_float(in->data, in_f, AUDIO_BLOCK_SAMPLES);

    float fsin = arm_sin_f32(pan * PI / 2.0);
    float fcos = arm_cos_f32(pan * PI / 2.0);
    int16_t* out_p[2] = {&out[0]->data[0], &out[1]->data[0]};
    for (uint8_t n = 0; n < AUDIO_BLOCK_SAMPLES; n++)
    {
      *out_p[0]++ = int16_t(in_f[n] * _pseudo_log * fsin * SHRT_MAX);
      *out_p[1]++ = int16_t(in_f[n] * _pseudo_log * fcos * SHRT_MAX);
    }
  }

  if (in != (audio_block_t*)&zeroblock)
  {
    release(in);
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
