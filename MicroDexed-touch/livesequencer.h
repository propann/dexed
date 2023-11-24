#include <string>
#include "MIDI.h"

#ifndef LIVESEQUENCER_H
#define LIVESEQUENCER_H

class LiveSequencer {

public:
    void addEvent(midi::MidiType event, uint8_t note, uint8_t velocity);
    void handlePatternBegin(void);

private:
    static std::string getName(midi::MidiType event);
    static void printEvent(unsigned int i);
    static void printEvents();
    static void loadNextEvent(unsigned long timeMs);
    static void playNextEvent(void);
    
};

#endif //LIVESEQUENCER_H