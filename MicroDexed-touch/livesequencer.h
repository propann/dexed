#include <string>
#include "MIDI.h"
#include "TeensyTimerTool.h"

#ifndef LIVESEQUENCER_H
#define LIVESEQUENCER_H

class LiveSequencer {

public:
    LiveSequencer();
    void addEvent(midi::MidiType event, uint8_t note, uint8_t velocity);
    void handlePatternBegin(void);

private:
    struct MidiEvent {
        unsigned long time;
        midi::MidiType event;
        uint8_t note_in;
        uint8_t note_in_velocity;
    };

    static constexpr int EVENTS_SIZE = 50;
    MidiEvent midiEvents[EVENTS_SIZE];
    elapsedMillis patternTimer;

    unsigned int playIndex = 0;
    unsigned int eventsSize = 0;

    TeensyTimerTool::OneShotTimer liveTimer;
    
    std::string getName(midi::MidiType event);
    void printEvent(unsigned int i);
    void printEvents();
    void loadNextEvent(unsigned long timeMs);

    static void timerCallback();
    void playNextEvent(void);
};

#endif //LIVESEQUENCER_H