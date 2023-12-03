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

  std::vector<MidiEvent>::iterator playIterator;

  elapsedMillis patternTimer;
  uint8_t activeRecordingTrack = 7;

  midi::Channel trackChannels[8] = { 0 };

  float lastTrackEvent[8] = { 0 };

  TeensyTimerTool::OneShotTimer liveTimer;
  
  std::string getName(midi::MidiType event);
  void printEvent(int i, MidiEvent e);
  void printEvents();
  void loadNextEvent(unsigned long timeMs);
  void insertSorted(MidiEvent &e);
  void allNotesOff(void);
  void clearTrackEvents(uint8_t track);

  static void timerCallback();
  void playNextEvent(void);
  void updateTrackChannels();

  unsigned long patternLengthMs = 2000;
  int patternCount = 0;
  
};

#endif //LIVESEQUENCER_H