#ifndef scope_h_
#define scope_h_

#include "Arduino.h"
#include "AudioStream.h"
class Realtime_Scope : public AudioStream
{
public:
  Realtime_Scope(void)
      : AudioStream(1, inputQueueArray)
  {
  }
  virtual void update(void);
  void draw_scope(uint16_t x, int y, uint8_t w);

private:
  bool scope_is_drawing;
  audio_block_t *inputQueueArray[2];
  int16_t scopebuffer[AUDIO_BLOCK_SAMPLES];
  int16_t scopebuffer_old[AUDIO_BLOCK_SAMPLES] = { 32 };
};

#endif