#ifndef LIVESEQUENCER_H
#define LIVESEQUENCER_H

#include <string>
#include <vector>
#include "MIDI.h"
#include "TeensyTimerTool.h"


class LiveSequencer {

public:
  LiveSequencer();
  void addEvent(midi::MidiType event, uint8_t note, uint8_t velocity);
  void handlePatternBegin(void);

private:
  struct MidiEvent {
    float time;
    uint8_t track;
    midi::MidiType event;
    uint8_t note_in;
    uint8_t note_in_velocity;
  };

  std::vector<MidiEvent> events;
  elapsedMillis patternTimer;

  unsigned int playIndex = 0;
  midi::Channel trackChannels[8];

  TeensyTimerTool::OneShotTimer liveTimer;
  
  std::string getName(midi::MidiType event);
  void printEvent(int i, MidiEvent e);
  void printEvents();
  void loadNextEvent(unsigned long timeMs);

  static void timerCallback();
  void playNextEvent(void);
  void updateTrackChannels();

  unsigned long patternLenghtMs = 2000;
  
};

#endif //LIVESEQUENCER_H