//
// Created by Nicholas Newdigate on 18/07/2020.
//

#include "playflashresmp.h"
#include "spi_interrupt.h"
#include "waveheaderparser.h"

void AudioPlayFlashResmp::begin()
{
    file_size = 0;
   // FlashReader.begin();
  
}

bool AudioPlayFlashResmp::playWav(const char *filename)
{
    stop();
    bool playing = FlashReader.playWav(filename);
    return playing;
}

void AudioPlayFlashResmp::stop()
{
       FlashReader.stop();
}

void AudioPlayFlashResmp::update()
{
    unsigned int i, n;
    audio_block_t *block;
    int16_t *data[1];
    // only update if we're playing
    if (!FlashReader.isPlaying()) return;

    // allocate the audio block to transmit
        block = allocate();
        if (block == nullptr) return;
    data[0] = block->data;
   
    if (FlashReader.available())
    {
        // we can read more data from the file...
            n = FlashReader.read((void**)data, AUDIO_BLOCK_SAMPLES);
            for (i=n; i < AUDIO_BLOCK_SAMPLES; i++) {
                block->data[i] = 0;
            }
            transmit(block);
       
    } else
    {
        FlashReader.close();
    }
        release(block);
}

#define B2M (uint32_t)((double)4294967296000.0 / AUDIO_SAMPLE_RATE_EXACT / 2.0) // 97352592

uint32_t AudioPlayFlashResmp::positionMillis()
{
    return ((uint64_t)file_size * B2M) >> 32;
}

uint32_t AudioPlayFlashResmp::lengthMillis()
{
    return ((uint64_t)file_size * B2M) >> 32;
}
