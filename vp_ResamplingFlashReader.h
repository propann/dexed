//
// Created by Nicholas Newdigate on 10/02/2019.
//

#ifndef TEENSYAUDIOLIBRARY_RESAMPLINGFLASHREADER_H
#define TEENSYAUDIOLIBRARY_RESAMPLINGFLASHREADER_H

#include <SerialFlash.h>
#include <cstdint>
#include "spi_interrupt.h"
#include "vp_loop_type.h"
#include "vp_interpolation.h"
#include "vp_IndexableFile.h"

#define RESAMPLE_BUFFER_SAMPLE_SIZE 128

class ResamplingFlashReader {
public:
    ResamplingFlashReader() {
    }

    void begin(void);
    bool playWav(const char *filename);

    bool play();
    void stop(void);
    bool isPlaying(void) { return _playing; }

    unsigned int read(void *buf, uint16_t nbyte);
    bool readNextValue(int16_t *value);

    void setPlaybackRate(double f) {
        _playbackRate = f;
        if (f < 0.0 && _bufferPosition == 0) {
            //_file.seek(_file_size);
            _bufferPosition = _file_size / 2 - _numChannels;
        }
    }

    double playbackRate() {
        return _playbackRate;
    }

    void setLoopType(loop_type loopType)
    {
        _loopType = loopType;
    }

    loop_type getLoopType(){
        return _loopType;    
    }

    int available(void);
    void reset(void);
    void close(void);

    void setLoopStart(uint32_t loop_start) {
        _loop_start = _header_offset + (loop_start * _numChannels);
    }

    void setLoopFinish(uint32_t loop_finish) {
        // sample number, (NOT byte number)
        _loop_finish = _header_offset + (loop_finish * _numChannels);
    }

    void setInterpolationType(ResampleInterpolationType interpolationType) {        
        if (interpolationType != _interpolationType) {            
            _interpolationType = interpolationType;
            initializeInterpolationPoints();
        }
    }

    int16_t getNumChannels() {
        return _numChannels;
    }

    void setNumChannels(uint16_t numChannels) {
        if (numChannels != _numChannels) {   
            _numChannels = numChannels;
            initializeInterpolationPoints();
        }
    }

    void setHeaderSize(uint32_t headerSizeInBytes) {
        _header_offset = headerSizeInBytes / 2;
        if (_bufferPosition < _header_offset) {
            if (_playbackRate >= 0) {
                _bufferPosition = _header_offset;
            }
        }
    }

private:
    volatile bool _playing = false;

    int32_t _file_size;
    int32_t _header_offset = 0; // == (header size in bytes ) / 2

    double _playbackRate = 1.0;
    double _remainder = 0.0;
    loop_type _loopType = looptype_none;
    int _bufferPosition = 0;
    int32_t _loop_start = 0;
    int32_t _loop_finish = 0;
    int16_t _numChannels = -1;
    uint16_t _numInterpolationPointsChannels = 0;
    
    newdigate::IndexableFile<128, 1> *_sourceBuffer = nullptr;

    ResampleInterpolationType _interpolationType = ResampleInterpolationType::resampleinterpolation_none;
    unsigned int _numInterpolationPoints = 0;
    InterpolationData *_interpolationPoints = nullptr;

    static bool isUsingSPI;
    void StartUsingSPI(){
        if (!isUsingSPI) {
            isUsingSPI = true;
            AudioStartUsingSPI();
        }
    }

    void StopUsingSPI() {
        if (isUsingSPI) {
            isUsingSPI = false;
            AudioStopUsingSPI();
        }
    }

    bool play(const char *filename);
    void initializeInterpolationPoints(void);
    void deleteInterpolationPoints(void);
};


#endif //TEENSYAUDIOLIBRARY_RESAMPLINGFLASHREADER_H
