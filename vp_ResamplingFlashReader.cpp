#include "vp_ResamplingFlashReader.h"
#include "vp_interpolation.h"

#include <SerialFlash.h>
bool ResamplingFlashReader::isUsingSPI = false;

// read n samples into each buffer (1 buffer per channel)
unsigned int ResamplingFlashReader::read(void *buf, uint16_t nsamples) {
    if (!_playing) return 0;

    int16_t *index;
    unsigned int count = 0;
   
        index = (int16_t*)buf;
   

    while (count < nsamples) {
        
        for (int channel=0; channel < _numChannels; channel++) {
            if (readNextValue(index)) {
               
                    count++;
                index++;
            }
            else {
                // we have reached the end of the file

                switch (_loopType) {
                    case looptype_repeat:
                    {
                        if (_playbackRate >= 0.0) 
                            _bufferPosition = _loop_start;
                        else
                            _bufferPosition = _loop_finish;

                        break;
                    }

                    case looptype_pingpong:
                    {
                        if (_playbackRate >= 0.0) {
                            _bufferPosition = _loop_finish;
                            //printf("switching to reverse playback...\n");
                        }
                        else {
                            _bufferPosition = _header_offset;
                            //printf("switching to forward playback...\n");
                        }
                        _playbackRate = -_playbackRate;
                        break;
                    }            

                    case looptype_none:            
                    default:
                    {
                        //Serial.printf("end of loop...\n");
                        /* no looping - return the number of (resampled) bytes returned... */
                        _playing = false;
                        if (_sourceBuffer) _sourceBuffer->close();
                        _sourceBuffer = nullptr;
                        StopUsingSPI();
                        return count;
                    }
                }   
            }
        }
    }
    return count;
}

// read the sample value for given channel and store it at the location pointed to by the pointer 'value'
bool ResamplingFlashReader::readNextValue(int16_t *value) {

    if (_playbackRate >= 0 ) {
        //forward playback
        if (_bufferPosition >=  _loop_finish )
            return false;

    } else if (_playbackRate < 0) {
        // reverse playback    
        if (_bufferPosition < _header_offset)
            return false;
    }
        
    newdigate::IndexableFile<128, 1> &sourceBuffer = (*_sourceBuffer);

    int16_t result = sourceBuffer[_bufferPosition];
    if (_interpolationType == ResampleInterpolationType::resampleinterpolation_linear) {

        double abs_remainder = abs(_remainder);
        if (abs_remainder > 0.0) {

            if (_playbackRate > 0) {
                if (_remainder - _playbackRate < 0.0){
                    // we crossed over a whole number, make sure we update the samples for interpolation

                    if (_playbackRate > 1.0) {
                        // need to update last sample
                        _interpolationPoints[1].y = sourceBuffer[_bufferPosition-1];
                    }

                    _interpolationPoints[0].y = _interpolationPoints[1].y;
                    _interpolationPoints[1].y = result;
                    if (_numInterpolationPoints < 2)
                        _numInterpolationPoints++;
                }
            } 
            else if (_playbackRate < 0) {
                if (_remainder - _playbackRate > 0.0){
                    // we crossed over a whole number, make sure we update the samples for interpolation

                    if (_playbackRate < -1.0) {
                        // need to update last sample
                        _interpolationPoints[1].y = sourceBuffer[_bufferPosition+1];
                    }

                    _interpolationPoints[0].y = _interpolationPoints[1].y;
                    _interpolationPoints[1].y = result;
                    if (_numInterpolationPoints < 2)
                        _numInterpolationPoints++;
                }
            }

            if (_numInterpolationPoints > 1) {
                result = abs_remainder * _interpolationPoints[1].y + (1.0 - abs_remainder) * _interpolationPoints[0].y;
                //Serial.printf("[%f]\n", interpolation);
            }
        } else {
            _interpolationPoints[0].y = _interpolationPoints[1].y;
            _interpolationPoints[1].y = result;
            if (_numInterpolationPoints < 2)
                _numInterpolationPoints++;

            result =_interpolationPoints[0].y;
            //Serial.printf("%f\n", result);
        }
    } 
    else if (_interpolationType == ResampleInterpolationType::resampleinterpolation_quadratic) {
        double abs_remainder = abs(_remainder);
        if (abs_remainder > 0.0) {
            if (_playbackRate > 0) {                
                if (_remainder - _playbackRate < 0.0){
                    // we crossed over a whole number, make sure we update the samples for interpolation
                    int numberOfSamplesToUpdate = - floor(_remainder - _playbackRate);
                    if (numberOfSamplesToUpdate > 4) 
                        numberOfSamplesToUpdate = 4; // if playbackrate > 4, only need to pop last 4 samples
                    for (int i=numberOfSamplesToUpdate; i > 0; i--) {
                        _interpolationPoints[0].y = _interpolationPoints[1].y;
                        _interpolationPoints[1].y = _interpolationPoints[2].y;
                        _interpolationPoints[2].y = _interpolationPoints[3].y;
                        _interpolationPoints[3].y =  sourceBuffer[_bufferPosition-(i)+1];
                        if (_numInterpolationPoints < 4) _numInterpolationPoints++;
                    }
                }
            } 
            else if (_playbackRate < 0) {                
                if (_remainder - _playbackRate > 0.0){
                    // we crossed over a whole number, make sure we update the samples for interpolation
                    int numberOfSamplesToUpdate =  ceil(_remainder - _playbackRate);
                    if (numberOfSamplesToUpdate > 4) 
                        numberOfSamplesToUpdate = 4; // if playbackrate > 4, only need to pop last 4 samples
                    for (int i=numberOfSamplesToUpdate; i > 0; i--) {
                        _interpolationPoints[0].y = _interpolationPoints[1].y;
                        _interpolationPoints[1].y = _interpolationPoints[2].y;
                        _interpolationPoints[2].y = _interpolationPoints[3].y;
                        _interpolationPoints[3].y =  sourceBuffer[_bufferPosition+(i)-1];
                        if (_numInterpolationPoints < 4) _numInterpolationPoints++;
                    }
                }
            }
            
            if (_numInterpolationPoints >= 4) {
                //int16_t interpolation = interpolate(_interpolationPoints, 1.0 + abs_remainder, 4);
                int16_t interpolation 
                    = fastinterpolate(
                        _interpolationPoints[0].y, 
                        _interpolationPoints[1].y, 
                        _interpolationPoints[2].y, 
                        _interpolationPoints[3].y, 
                        1.0 + abs_remainder); 
                result = interpolation;
                //Serial.printf("[%f]\n", interpolation);
            } else 
                result = 0;
        } else {
            _interpolationPoints[0].y = _interpolationPoints[1].y;
            _interpolationPoints[1].y = _interpolationPoints[2].y;
            _interpolationPoints[2].y = _interpolationPoints[3].y;
            _interpolationPoints[3].y = result;
            if (_numInterpolationPoints < 4) {
                _numInterpolationPoints++;
                result = 0;
            } else 
                result = _interpolationPoints[1].y;
            //Serial.printf("%f\n", result);
        }
    }

   
        _remainder += _playbackRate;

        auto delta = static_cast<signed int>(_remainder);
        _remainder -= static_cast<double>(delta);
        _bufferPosition +=  (delta );
   
    *value = result;
    return true;
}

void ResamplingFlashReader::initializeInterpolationPoints(void) {
    if (_numChannels < 0)
        return;
        
    deleteInterpolationPoints();
   // _interpolationPoints = new InterpolationData*[_numChannels];
    for (int channel=0; channel < _numChannels; channel++) {        
        InterpolationData *interpolation = new InterpolationData[4];
        interpolation[0].y = 0.0;
        interpolation[1].y = 0.0;    
        interpolation[2].y = 0.0;    
        interpolation[3].y = 0.0;
        _interpolationPoints = interpolation ;
    }
   // _numInterpolationPointsChannels = _numChannels;
}

void ResamplingFlashReader::deleteInterpolationPoints(void) {
    if (!_interpolationPoints) return;
            delete _interpolationPoints;
    _interpolationPoints = nullptr;
    _numInterpolationPointsChannels = 0;
}

void ResamplingFlashReader::begin(void)
{
    if (_interpolationType != ResampleInterpolationType::resampleinterpolation_none) {
        initializeInterpolationPoints();
    }
    _playing = false;
    _bufferPosition = _header_offset;
    _file_size = 0;
}

bool ResamplingFlashReader::playWav(const char *filename) {
    return play(filename);
}

bool ResamplingFlashReader::play(const char *filename)
{
    stop();

    if (_sourceBuffer) {
        //Serial.printf("closing %s\n", filename);
        __disable_irq();
        _sourceBuffer->close();
        __enable_irq();
    }
    if (_sourceBuffer) delete _sourceBuffer;
    
    StartUsingSPI();

    __disable_irq();
    SerialFlashFile file = SerialFlash.open(filename);
    __enable_irq();

    if (!file) {
        StopUsingSPI();
        Serial.printf("Not able to open file: %s\n", filename);
        
        return false;
    }

    __disable_irq();
    _file_size = file.size();
    __enable_irq();

     
        setNumChannels(1);
        _header_offset = 22;
        _loop_finish = ((_file_size) / 2) + 22; 
    
    
    __disable_irq();
    file.close();
    __enable_irq();

    _sourceBuffer = new newdigate::IndexableFile<128, 1>(filename);
    _loop_start = _header_offset;
    if (_file_size <= _header_offset * newdigate::IndexableFile<128, 1>::element_size) {
        _playing = false;
       
        Serial.printf("Wave file contains no samples: %s\n", filename);
        return false;
    }

    reset();
    _playing = true;
    return true;
}

bool ResamplingFlashReader::play()
{
    stop();
    reset();
    _playing = true;
    return true;
}

void ResamplingFlashReader::reset(){
    initializeInterpolationPoints();
    if (_playbackRate > 0.0) {
        // forward playabck - set _file_offset to first audio block in file
        _bufferPosition = _header_offset;
    } else {
        // reverse playback - forward _file_offset to last audio block in file
        _bufferPosition = _loop_finish / _numChannels - _numChannels;
    }
}

void ResamplingFlashReader::stop()
{
    if (_playing) {
        __disable_irq();
        _playing = false;
        __enable_irq();
    }
}

int ResamplingFlashReader::available(void) {
    return _playing;
}

void ResamplingFlashReader::close(void) {
    if (_playing)
        stop();
    if (_sourceBuffer) {
        _sourceBuffer->close();
        delete _sourceBuffer;
        
    }
    StopUsingSPI();
    _sourceBuffer = nullptr;
    //TODO: dispose _sourceBuffer properly
    deleteInterpolationPoints();
}
