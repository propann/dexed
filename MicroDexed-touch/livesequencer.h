#ifndef LIVESEQUENCER_H
#define LIVESEQUENCER_H

#include <string>
#include <vector>
#include "MIDI.h"
#include "TeensyTimerTool.h"

class LiveSequencer {

public:
  LiveSequencer();
  void handleMidiEvent(midi::MidiType event, uint8_t note, uint8_t velocity);
  void handlePatternBegin(void);
  void handleStop(void);

private:
  struct MidiEvent {
    float time;
    uint8_t track;
    midi::MidiType event;
    uint8_t note_in;
    uint8_t note_in_velocity;
  };

  std::vector<MidiEvent> events;
  std::vector<MidiEvent> pendingEvents;

  elapsedMillis patternTimer;

  unsigned int playIndex = 0;
  midi::Channel trackChannels[8];

  TeensyTimerTool::OneShotTimer liveTimer;
  
  std::string getName(midi::MidiType event);
  void printEvent(int i, MidiEvent e);
  void printEvents();
  void loadNextEvent(unsigned long timeMs);
  void insertSorted(MidiEvent e);
  void allNotesOff(void);

  static void timerCallback();
  void playNextEvent(void);
  void updateTrackChannels();

  unsigned long patternLengthMs = 2000;
  int patternCount = 0;
  
};

#endif //LIVESEQUENCER_H