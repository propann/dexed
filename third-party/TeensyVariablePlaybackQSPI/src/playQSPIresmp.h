//
// Created by Nicholas Newdigate on 18/07/2020.
//

#ifndef TEENSY_RESAMPLING_QSPIREADER_H
#define TEENSY_RESAMPLING_QSPIREADER_H


#include "Arduino.h"
#include "AudioStream.h"
#include "stdint.h"
#include "ResamplingQSPIReader.h"
#include "playresmp.h"

class AudioPlayQSPIResmp : public AudioPlayResmp
{
public:
    AudioPlayQSPIResmp(void) :
            AudioPlayResmp(),
            QSPIReader()
    {
        begin();
    }

    void begin(void);
    bool playRaw(const char *filename, uint16_t numChannels);
    bool playWav(const char *filename);
    void stop(void);
    bool isPlaying(void) { return QSPIReader.isPlaying(); }
    uint32_t positionMillis(void);
    uint32_t lengthMillis(void);
    virtual void update(void);

    void setPlaybackRate(float f) {
        QSPIReader.setPlaybackRate(f);
    }

    void setLoopType(loop_type t) {
        QSPIReader.setLoopType(t);
    }

    void setLoopStart(uint32_t loop_start) {
        QSPIReader.setLoopStart(loop_start);
    }

    void setLoopFinish(uint32_t loop_finish) {
        QSPIReader.setLoopFinish(loop_finish);
    }

    void enableInterpolation(bool enable) {
        if (enable)
            QSPIReader.setInterpolationType(ResampleInterpolationType::resampleinterpolation_quadratic);
        else 
            QSPIReader.setInterpolationType(ResampleInterpolationType::resampleinterpolation_none);
    }
private:
    uint32_t file_size;
    ResamplingQSPIReader QSPIReader;
};


#endif //TEENSY_RESAMPLING_QSPIREADER_H
