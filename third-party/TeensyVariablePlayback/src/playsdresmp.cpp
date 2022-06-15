//
// Created by Nicholas Newdigate on 18/07/2020.
//

#include "playsdresmp.h"

void AudioPlaySdResmp::begin()
{
    file_size = 0;
    sdReader.begin();
}

bool AudioPlaySdResmp::playRaw(const char *filename, uint16_t numChannels)
{
    stop();
    bool playing = sdReader.playRaw(filename, numChannels);
    return playing;
}

bool AudioPlaySdResmp::playWav(const char *filename)
{
    stop();
    bool playing = sdReader.playWav(filename);
    return playing;

}

void AudioPlaySdResmp::stop()
{
    sdReader.stop();
}

void AudioPlaySdResmp::update()
{
    int _numChannels = sdReader.getNumChannels();
    if (_numChannels == -1)
        return;

    unsigned int n;
    audio_block_t *blocks[_numChannels];
    int16_t *data[_numChannels];

    // only update if we're playing
    if (!sdReader.isPlaying()) return;

    if (sdReader.available()) {
        // allocate the audio blocks to transmit
        for (int i=0; i < _numChannels; i++) {
            blocks[i] = allocate();
            if (blocks[i] == nullptr) return;
            data[i] = blocks[i]->data;
        }

        // we can read more data from the file...
        n = sdReader.read(data);

        for (int channel=0; channel < _numChannels; channel++) {
            memset( &blocks[channel]->data[n], 0, (AUDIO_BLOCK_SAMPLES - n) * 2);
            transmit(blocks[channel], channel);
        }

        if(_numChannels == 1) {
            transmit(blocks[0], 1);
        }

        if (n < AUDIO_BLOCK_SAMPLES) {
            sdReader.close();
        }
    } else {
        sdReader.close();
    }
    for (int channel=0; channel < _numChannels; channel++) {
        release(blocks[channel]);
        blocks[channel] = NULL;
    }
}

uint32_t AudioPlaySdResmp::positionMillis()
{
    return sdReader.positionMillis();
}

uint32_t AudioPlaySdResmp::lengthMillis()
{
    return sdReader.lengthMillis();
}