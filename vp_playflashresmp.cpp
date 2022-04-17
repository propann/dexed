//
// Created by Nicholas Newdigate on 18/07/2020.
//

#include "vp_playflashresmp.h"
#include "spi_interrupt.h"

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
    audio_block_t *block;
    int16_t *data;
    // only update if we're playing
    if (!FlashReader.isPlaying()) return;

    // allocate the audio block to transmit
        block = allocate();
        if (block == nullptr) return;
    data = block->data;
   
    if (FlashReader.available())
    {
        // we can read more data from the file...
             FlashReader.read(data, AUDIO_BLOCK_SAMPLES);
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
