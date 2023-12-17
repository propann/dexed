#ifndef LIVESEQUENCER_H
#define LIVESEQUENCER_H

#include <string>
#include <vector>
#include "MIDI.h"
#include "TeensyTimerTool.h"

class LiveSequencer {

public:
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

  LiveSequencer();
  void handleMidiEvent(midi::MidiType event, uint8_t note, uint8_t velocity);
  void handlePatternBegin(void);
  void handleStop(void);
//  void init(int bpm, std::vector<MidiEvents> loadedEvents);
  void init(int bpm);

private:
  uint32_t eventTimeToMs(EventTime &t);


  static bool sortMidiEvent(MidiEvent &a, MidiEvent &b) {
    return ((a.time.patternNumber * 2000) + a.time.patternMs) < ((b.time.patternNumber * 2000) + b.time.patternMs);
  }
  
  std::vector<MidiEvent> events;
  std::vector<MidiEvent> pendingEvents;
  std::vector<MidiEvent>::iterator playIterator;

  elapsedMillis patternTimer;
  midi::Channel trackChannels[8] = { 0 };

  int currentBpm = 90;

  TeensyTimerTool::OneShotTimer liveTimer;
  
  std::string getName(midi::MidiType event);
  void printEvent(int i, MidiEvent e);
  void printEvents();
  void loadNextEvent(int timeMs);
  void allNotesOff(void);
  void clearTrackEvents(uint8_t track);
  void playNextEvent(void);
  void updateTrackChannels();
  EventTime timeQuantization(uint16_t patternNumber, uint16_t patternMs, uint16_t multiple);
  void onBpmChanged(int bpm);

  unsigned long patternLengthMs;
  static constexpr int NUM_PATTERNS = 4; // needs GUI config
  uint16_t quantisizeDenom = 16; // 1/x
  uint16_t quantisizeMs = 150;
  uint16_t patternCount = NUM_PATTERNS - 1;

  static constexpr uint8_t MIN_TRACK_CHANNEL = 5;
  static constexpr uint8_t MAX_TRACK_CHANNEL = 7;
  uint8_t activeRecordingTrack = MAX_TRACK_CHANNEL;

};

#endif //LIVESEQUENCER_H