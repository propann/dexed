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

  struct EventTime {
    uint16_t patternNumber;
    uint16_t patternMs;
  };
  
  struct MidiEvent {
    EventTime time;
    uint8_t track;
    midi::MidiType event;
    uint8_t note_in;
    uint8_t note_in_velocity;
  };

private:
  uint32_t eventTimeToMs(EventTime &t);


  static bool sortMidiEvent(MidiEvent &a, MidiEvent &b) {
    return ((a.time.patternNumber * 2000) + a.time.patternMs) < ((b.time.patternNumber * 2000) + b.time.patternMs);
  }
  
  std::vector<MidiEvent> events;
  std::vector<MidiEvent> pendingEvents;
  std::vector<MidiEvent>::iterator playIterator;

  elapsedMillis patternTimer;
  uint8_t activeRecordingTrack = 7;

  midi::Channel trackChannels[8] = { 0 };

  EventTime lastTrackEvent[8] = { { 0, 0 } };

  TeensyTimerTool::OneShotTimer liveTimer;
  
  std::string getName(midi::MidiType event);
  void printEvent(int i, MidiEvent e);
  void printEvents();
  void loadNextEvent(uint32_t timeMs);
  void allNotesOff(void);
  void clearTrackEvents(uint8_t track);

  static void timerCallback();
  void playNextEvent(void);
  void updateTrackChannels();
  uint16_t roundUpDownToMultiple(uint16_t number, uint16_t multiple);

  unsigned long patternLengthMs;
  uint16_t patternQuantisizeMs = 20;
  static constexpr int NUM_PATTERNS = 4; // needs GUI config
  uint16_t patternCount = NUM_PATTERNS - 1;
  
};

#endif //LIVESEQUENCER_H