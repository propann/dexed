#include <string>
#include <vector>
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
    midi::Channel channel;
    midi::MidiType event;
    uint8_t note_in;
    uint8_t note_in_velocity;
  };

  std::vector<MidiEvent> events;
  elapsedMillis patternTimer;

  unsigned int playIndex = 0;

  TeensyTimerTool::OneShotTimer liveTimer;
  
  std::string getName(midi::MidiType event);
  void printEvent(int i, MidiEvent e);
  void printEvents();
  void loadNextEvent(unsigned long timeMs);

  static void timerCallback();
  void playNextEvent(void);
};

#endif //LIVESEQUENCER_H