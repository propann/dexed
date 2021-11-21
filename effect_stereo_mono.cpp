/*
   Copyright (c) 2019-2021 H. Wirtz

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
#include "effect_stereo_mono.h"

/*************************************************************************/
//                A u d i o E f f e c t S t e r e o M o n o
// Written by Holger Wirtz
// 20191023 - inital version

void AudioEffectStereoMono::stereo(bool mode)
{
  _enabled = mode;
}

void AudioEffectStereoMono::update(void)
{
  audio_block_t *block[2];

  block[0] = receiveWritable(0);
  if (!block[0])
    return;

  block[1] = receiveWritable(1);
  if (!block[1])
    return;

  if (_enabled == false)
  {
    if (block[0] && block[1])
    {
#ifdef USE_OLD_STEREO_TO_MONO
      int16_t *bp[2] = { block[0]->data, block[1]->data };

      for (uint16_t i = 0; i < AUDIO_BLOCK_SAMPLES; i++)
      {
        *bp[0] = (*bp[0] >> 1) + (*bp[1] >> 1);
        *bp[1] = *bp[0];
        bp[0]++;
        bp[1]++;
      }
#else
      // devide every channel by 2
      arm_shift_q15(block[0]->data, -1, block[0]->data, AUDIO_BLOCK_SAMPLES);
      arm_shift_q15(block[1]->data, -1, block[1]->data, AUDIO_BLOCK_SAMPLES);
      // add channel 2 to channel 1
      arm_add_q15(block[0]->data, block[1]->data, block[0]->data, AUDIO_BLOCK_SAMPLES);
      // copy channel 1 to channel 2
      arm_copy_q15(block[0]->data, block[1]->data, AUDIO_BLOCK_SAMPLES);
#endif
    }
  }

  if (block[0])
  {
    transmit(block[0], 0);
    release(block[0]);
  }

  if (block[1])
  {
    transmit(block[1], 1);
    release(block[1]);
  }
}
