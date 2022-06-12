//
// Created by Nicholas Newdigate on 18/07/2020.
//

#include "playQSPIresmp.h"

void AudioPlayQSPIResmp::begin()
{
    file_size = 0;
    QSPIReader.begin();
}

bool AudioPlayQSPIResmp::playWav(const char *filename)
{
    stop();
    bool playing = QSPIReader.playWav(filename);
    return playing;
}

void AudioPlayQSPIResmp::stop()
{
       QSPIReader.stop();
}

void AudioPlayQSPIResmp::update()
{
    int _numChannels = QSPIReader.getNumChannels();
    if (_numChannels == -1)
        return;

    unsigned int n;
    audio_block_t *blocks[_numChannels];
    int16_t *data[_numChannels];

    // only update if we're playing
    if (!QSPIReader.isPlaying()) return;

    if (QSPIReader.available())
    {
        // allocate the audio blocks to transmit
        for (int i=0; i < _numChannels; i++) {
            blocks[i] = allocate();
            if (blocks[i] == nullptr) return;
            data[i] = blocks[i]->data;
        }

        // we can read more data from the file...
        n = QSPIReader.read(data);

        for (int channel=0; channel < _numChannels; channel++) {
            memset( &blocks[channel]->data[n], 0, (AUDIO_BLOCK_SAMPLES - n) * 2);
            transmit(blocks[channel], channel);
        }

        if(_numChannels == 1) {
            transmit(blocks[0], 1);
        }

        if (n < AUDIO_BLOCK_SAMPLES) {
            QSPIReader.close();
        }
    } else
    {
        QSPIReader.close();
    }

    for (int channel=0; channel < _numChannels; channel++) {
        release(blocks[channel]);
        blocks[channel] = NULL;
    }
}

#define B2M (uint32_t)((double)4294967296000.0 / AUDIO_SAMPLE_RATE_EXACT / 2.0) // 97352592

uint32_t AudioPlayQSPIResmp::positionMillis()
{
    return ((uint64_t)file_size * B2M) >> 32;
}

uint32_t AudioPlayQSPIResmp::lengthMillis()
{
    return ((uint64_t)file_size * B2M) >> 32;
}
