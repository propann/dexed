
#ifndef microsynth_h_
#define microsynth_h_

#include "Arduino.h"

void microsynth_update_settings(uint8_t instance_id);


//class microsynth : public AudioStream {
//  public:
//   bool scope_is_drawing;
//  uint8_t scope_delay;
//    Realtime_Scope(void) : AudioStream(1, inputQueueArray) {
//    }
//    virtual void update(void);
//    void FillArray(void);
//    void AddtoBuffer(int16_t*);
//    void draw_scope(uint16_t x, uint8_t y, uint8_t w);
//    
//  private:
//    audio_block_t *inputQueueArray[2];
//    int16_t buffer[AUDIO_BLOCK_SAMPLES];
//    int16_t buffer_old[AUDIO_BLOCK_SAMPLES];
//    uint32_t count = 0;
//    elapsedMillis msecs;
//
//    int16_t scopebuffer[AUDIO_BLOCK_SAMPLES];
//    int16_t scopebuffer_old[AUDIO_BLOCK_SAMPLES];
//};

#endif
