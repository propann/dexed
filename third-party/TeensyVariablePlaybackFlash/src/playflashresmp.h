//
// Created by Nicholas Newdigate on 18/07/2020.
//

#ifndef TEENSY_RESAMPLING_FLASHREADER_PLAYFLASHRAWRESMP_H
#define TEENSY_RESAMPLING_FLASHREADER_PLAYFLASHRAWRESMP_H


#include "Arduino.h"
#include "AudioStream.h"
#include "stdint.h"
#include "ResamplingFlashReader.h"
#include "playresmp.h"

class AudioPlayFlashResmp : public AudioPlayResmp
{
public:
    AudioPlayFlashResmp(void) :
            AudioPlayResmp(),
            FlashReader()
    {
        begin();
    }

    void begin(void);
    bool playRaw(const char *filename, uint16_t numChannels);
    bool playWav(const char *filename);
    void stop(void);
    bool isPlaying(void) { return FlashReader.isPlaying(); }
    uint32_t positionMillis(void);
    uint32_t lengthMillis(void);
    virtual void update(void);

    void setPlaybackRate(float f) {
        FlashReader.setPlaybackRate(f);
    }

    void setLoopType(loop_type t) {
        FlashReader.setLoopType(t);
    }

    void setLoopStart(uint32_t loop_start) {
        FlashReader.setLoopStart(loop_start);
    }

    void setLoopFinish(uint32_t loop_finish) {
        FlashReader.setLoopFinish(loop_finish);
    }

    void enableInterpolation(bool enable) {
        if (enable)
            FlashReader.setInterpolationType(ResampleInterpolationType::resampleinterpolation_quadratic);
        else 
            FlashReader.setInterpolationType(ResampleInterpolationType::resampleinterpolation_none);
    }
private:
   // SerialFlashFile rawfile; //
    uint32_t file_size;
    ResamplingFlashReader FlashReader;
  
};


#endif //TEENSY_RESAMPLING_FLASHREADER_PLAYFLASHRAWRESMP_H
