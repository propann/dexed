#ifndef scope_h_
#define scope_h_

#include "Arduino.h"
#include "AudioStream.h"
class Realtime_Scope : public AudioStream
{
public:
  bool scope_is_drawing;
  uint8_t scope_delay;
  Realtime_Scope(void)
      : AudioStream(1, inputQueueArray)
  {
  }
  virtual void update(void);
  void FillArray(void);
  void AddtoBuffer(int16_t *);
  void draw_scope(uint16_t x, int y, uint8_t w);
  void clear(void);

private:
  audio_block_t *inputQueueArray[2];
  int16_t buffer[AUDIO_BLOCK_SAMPLES];
  uint32_t count = 0;
  elapsedMillis msecs;
  int16_t scopebuffer[AUDIO_BLOCK_SAMPLES];
  int16_t scopebuffer_old[AUDIO_BLOCK_SAMPLES];
};

#endif