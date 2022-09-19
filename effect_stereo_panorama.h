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

#ifndef effect_stereo_panorama_h_
#define effect_stereo_panorama_h_

#include "Arduino.h"
#include "AudioStream.h"

// Written by Holger Wirtz
// 20211124 - inital version

class AudioEffectStereoPanorama : public AudioStream {
public:
  AudioEffectStereoPanorama(void)
    : AudioStream(2, inputQueueArray) {
    pan = 0.0;
  }

  virtual void update(void);
  virtual void panorama(float p);

private:
  audio_block_t *inputQueueArray[2];
  audio_block_t *out[2];
  float in_f[2][AUDIO_BLOCK_SAMPLES];
  float out_f[2][AUDIO_BLOCK_SAMPLES];
  float pan;
};

#endif