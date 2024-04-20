#ifndef LIVESEQUENCER_H
#define LIVESEQUENCER_H

#include <string>
#include <vector>
#include <list>
#include "MIDI.h"
#include "ui_livesequencer.h"
#include "LCDMenuLib2_typedef.h"
#include <unordered_map>
#include <unordered_set>
#include <set>

class LiveSequencer {

public:
  static constexpr int LIVESEQUENCER_NUM_TRACKS = 6;
  static constexpr int LIVESEQUENCER_NUM_LAYERS = 4;

  enum EventSource : uint8_t {
    EVENT_SONG = 0,
    EVENT_PATTERN = 1,
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
    uint8_t layerMutes;
    LCDML_FuncPtr_pu8 screen;
    SetupFn screenSetupFn;
    std::unordered_multiset<uint8_t> activeNotes[LiveSequencer::LIVESEQUENCER_NUM_LAYERS];
  };

  struct TrackSettings {
    uint8_t layerCount;
    uint8_t quantisizeDenom;
    uint8_t songStartLayerMutes;
  };

  enum AutomationType {
    TYPE_MUTE_ON,
    TYPE_MUTE_OFF
  };

  struct FillNotes {
    uint8_t number;
    uint8_t offset;
  };

  enum ArpMode : uint8_t {
    ARP_DOWN = 0,
    ARP_UP,
    ARP_DOWNUP,
    ARP_DOWNUP_P,
    ARP_UPDOWN,
    ARP_UPDOWN_P,
    ARP_RANDOM,
    ARP_CHORD,
    ARP_MODENUM
  };

  struct ArpNote {
    uint16_t offDelay;
    uint8_t track;
    std::list<uint8_t> notes;
  };

  struct ArpSettings {
    uint16_t delayToNextArpOnMs;
    uint8_t amount; // 0, 1, 2, ... per bar
    uint8_t octaves;
    ArpMode mode;
    uint8_t loadPerBar;
    uint8_t noteRepeat;
    uint8_t notePlayCount;
    uint16_t length; // >100% pulse width possible
    int8_t swing;
    uint8_t latch; // keep notes or drop them
    bool keysChanged;
    std::vector<uint8_t> arpNotes;
    std::vector<uint8_t>::iterator arpIt;
  };

  struct LiveSeqData {
    // non - volatile
    TrackSettings trackSettings[LiveSequencer::LIVESEQUENCER_NUM_TRACKS];
    std::list<MidiEvent> eventsList;
    std::unordered_map<uint8_t, std::list<MidiEvent>> songEvents; // should use std::map but name clashes with map()..
    uint8_t numberOfBars = 4;

    // volatile
    bool isActive;
    std::unordered_set<uint8_t> instrumentChannels;
    Track tracks[LiveSequencer::LIVESEQUENCER_NUM_TRACKS];
    ArpSettings arpSettings;
    uint8_t lastSongEventPattern; // because using unordered map above we need to know last index to be able to know song length (eg. for song loop)
    uint8_t currentPattern = 0;
    FillNotes fillNotes = { 4, 0 }; // user default
    unsigned long patternLengthMs;
    uint8_t activeTrack = 0;
    elapsedMillis patternTimer;
    std::unordered_map<uint8_t, LiveSequencer::MidiEvent> notesOn;
    std::vector<MidiEvent> pendingEvents;
    uint8_t songPatternCount = 0;
    uint8_t currentPageIndex = 0; // PagePattern or PageSong
    uint8_t songLayerCount = 0;
    uint8_t lastPlayedNote = 0;
    bool trackLayersChanged = false;
    bool songLayersChanged = false;
    bool patternBeginFlag = false;
    bool startedFlag = false;
    bool stoppedFlag = false;
    bool isRunning = false;
    bool isRecording = false;
    bool lastPlayedNoteChanged = false;
    bool isSongMode = false;
    bool recordedToSong = false;
    int currentBpm = 90;
    uint8_t performanceID = 0;
    uint8_t songMuteQuantisizeDenom = 1;
  };

  LiveSequencer();
  LiveSequencer::LiveSeqData* getData(void);
  void songLayerAction(uint8_t layer, UI_LiveSequencer::LayerMode action);
  void trackLayerAction(uint8_t track, uint8_t layer, UI_LiveSequencer::LayerMode action);
  void handleMidiEvent(uint8_t inChannel, midi::MidiType event, uint8_t note, uint8_t velocity);
  void handlePatternBegin(void);
  void handleStart(void);
  void handleStop(void);
  void init(void);
  void onGuiInit(void);
  void setLayerMuted(uint8_t track, uint8_t layer, bool isMuted, bool recordToSong = false);
  void changeNumberOfBars(uint8_t num);
  void deleteAllSongEvents(void);
  void fillTrackLayer();
  void deleteLiveSequencerData(void);
  uint32_t timeToMs(uint8_t patternNumber, uint16_t patternMs) const;

private:
  LiveSeqData data;
  UI_LiveSequencer ui;

  std::set<uint8_t> pressedArpKeys;
  std::list<LiveSequencer::ArpNote> activeArps;

  void checkLoadNewArpNotes(void);
  void onSongStopped(void);
  void updateTrackChannels(bool initial = false);
  void addPendingNotes(void);
  void refreshSongLength(void);
  void applySongStartLayerMutes(void);
  
  static bool sortMidiEvent(MidiEvent &a, MidiEvent &b) {
    // + a.source is a hack to sort song events before pattern events if the have same time
    return ((a.patternNumber * 5000) + a.patternMs + a.source) < ((b.patternNumber * 5000) + b.patternMs + b.source); // FIXME: patternLengthMs
  }
  
  std::list<MidiEvent>::iterator playIterator;  
  const std::string getEventName(midi::MidiType event) const;
  const std::string getEventSource(LiveSequencer::EventSource source) const;

  void printEvent(int i, MidiEvent e);
  void printEvents();
  void loadNextEvent(int timeMs);
  void allNotesOff(void);
  
  void playNextEvent(void);
  void playNextArpNote(void);
  bool timeQuantization(MidiEvent &e, uint8_t denom);
  void checkBpmChanged(void);
  void checkAddMetronome(void);
  void performLayerAction(UI_LiveSequencer::LayerMode action, LiveSequencer::MidiEvent &e, uint8_t layer);
};

#endif // LIVESEQUENCER_H