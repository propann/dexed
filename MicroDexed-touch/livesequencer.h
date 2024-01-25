#ifndef LIVESEQUENCER_H
#define LIVESEQUENCER_H

#include <string>
#include <vector>
#include <list>
#include "MIDI.h"
#include "TeensyTimerTool.h"
#include "ui_livesequencer.h"
#include "LCDMenuLib2_typedef.h"
#include <unordered_map>
#include <unordered_set>

class LiveSequencer {

public:
  enum EventSource : uint8_t {
    EVENT_PATTERN,
    EVENT_SONG,
  };

  struct MidiEvent {
    EventSource source;
    uint16_t patternMs;
    uint8_t patternNumber;
    uint8_t track;
    uint8_t layer;
    midi::MidiType event;
    uint8_t note_in;
    uint8_t note_in_velocity;

  };

  struct Track {
    midi::Channel channel;
    char name[4];
    LCDML_FuncPtr_pu8 screen;
    SetupFn screenSetupFn;
    uint8_t layerCount;
    uint8_t layerMutes;
    uint8_t quantisizeDenom;
    std::unordered_multiset<uint8_t> activeNotes[8];
  };

  enum AutomationType {
    TYPE_MUTE
  };

  struct FillNotes {
    uint8_t number;
    uint8_t offset;
  };

  struct LiveSeqData {
    Track tracks[LIVESEQUENCER_NUM_TRACKS];
    bool isRunning = false;
    bool isRecording = false;
    uint8_t activeTrack = 0;
    elapsedMillis patternTimer;
    unsigned long patternLengthMs;
    uint8_t patternCount = 0;
    bool trackLayersChanged = false;
    bool patternBeginFlag = false;
    std::list<MidiEvent> eventsList;
    std::vector<MidiEvent> pendingEvents;
    std::unordered_map<uint8_t, LiveSequencer::MidiEvent> notesOn;
    std::unordered_map<uint8_t, std::list<MidiEvent>> songEvents; // should use std::map but name clashes with map()..
    uint8_t lastSongEventPattern; // because using unordered map above we need to know last index to be able to know song length (eg. for song loop)
    FillNotes fillNotes = { 4, 0 }; // user default
    uint8_t numberOfBars = 4;
    uint8_t lastPlayedNote = 0;
    bool lastPlayedNoteChanged = false;
    bool isSongMode = false;
    uint8_t songPatternCount = 0;
    uint8_t functionPageIndex = 0; // PagePattern or PageSong
    uint8_t songLayerCount = 0;
    bool startedFlag = false;
  };

  LiveSequencer();
  LiveSequencer::LiveSeqData* getData(void);
  void trackLayerAction(uint8_t track, uint8_t layer, TrackLayerMode action);
  void handleMidiEvent(midi::MidiType event, uint8_t note, uint8_t velocity);
  void handlePatternBegin(void);
  void handleStart(void);
  void handleStop(void);
  void init(void);
  void setLayerMuted(uint8_t track, uint8_t layer, bool isMuted, bool recordToSong = false);
  void changeNumberOfBars(uint8_t num);
  void deleteAllSongEvents(void);
  void fillTrackLayer();
  uint32_t timeToMs(uint8_t patternNumber, uint16_t patternMs) const;

private:
  void onSongStopped(void);
  void updateTrackChannels(void);
  void addPendingNotes(void);

  LiveSeqData data;
  UI_LiveSequencer ui;

  static bool sortMidiEvent(MidiEvent &a, MidiEvent &b) {
    return ((a.patternNumber * 5000) + a.patternMs) < ((b.patternNumber * 5000) + b.patternMs); // FIXME: patternLengthMs
  }
  
  std::list<MidiEvent>::iterator playIterator;
  int currentBpm = 90;

  TeensyTimerTool::OneShotTimer liveTimer;
  
  const std::string getEventName(midi::MidiType event) const;
  const std::string getEventSource(LiveSequencer::EventSource source) const;
  void printEvent(int i, MidiEvent e);
  void printEvents();
  void loadNextEvent(int timeMs);
  void allNotesOff(void);
  
  void playNextEvent(void);
  void timeQuantization(uint8_t &patternNumber, uint16_t &patternMs, uint16_t multiple);
  void checkBpmChanged(void);
  void checkAddMetronome(void);
};

#endif //LIVESEQUENCER_H