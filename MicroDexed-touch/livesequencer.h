#ifndef LIVESEQUENCER_H
#define LIVESEQUENCER_H

#include <string>
#include <vector>
#include <list>
#include "MIDI.h"
#include "LCDMenuLib2_typedef.h"
#include <unordered_map>
#include <unordered_set>
#include <set>

typedef void (*SetupFn)(void*);

class UI_LiveSequencer;

class LiveSequencer {

public:
  static constexpr int LIVESEQUENCER_NUM_TRACKS = 6;
  static constexpr int LIVESEQUENCER_NUM_LAYERS = 4;

  enum EventSource : uint8_t {
    EVENT_SONG = 0,
    EVENT_PATTERN = 1,
  };

  enum LayerMode {
    LAYER_MUTE = 0,
    LAYER_MERGE,
    LAYER_DELETE,
    LAYER_MODE_NUM
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
    std::unordered_multiset<uint8_t> activeNotes[LIVESEQUENCER_NUM_LAYERS];
  };

  struct TrackSettings {
    uint8_t layerCount;
    uint8_t quantizeDenom;
    uint8_t velocityLevel; // 0: original, 1 - 10: 10-100%
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
    ARP_UP = 0,
    ARP_DOWN,
    ARP_UPDOWN,
    ARP_UPDOWN_P,
    ARP_DOWNUP,
    ARP_DOWNUP_P,
    ARP_RANDOM,
    ARP_CHORD,
    ARP_MODENUM
  };

  struct ArpNote {
    uint16_t offDelay;
    uint8_t track;
    std::vector<uint8_t> notes;
  };

  struct ArpSettings {
    uint16_t delayToNextArpOnMs;
    uint8_t enabled;
    uint8_t amount; // 1, 2, ... per bar
    uint8_t octaves;
    uint8_t source;
    ArpMode mode;
    uint8_t loadPerBar;
    uint8_t noteRepeat;
    uint8_t velocity;
    uint8_t notePlayCount;
    uint16_t length; // >100% pulse width possible
    int8_t swing;
    uint8_t latch; // keep notes or drop them
    uint8_t freerun; // do not restart arp on pattern start
    bool keysChanged;
    bool arpSettingsChanged;
    std::vector<uint8_t> arpNotesIn;
    std::vector<uint8_t> arpNotes;
    std::vector<uint8_t>::iterator arpIt;
  };

  struct LiveSeqData {
    // non - volatile
    TrackSettings trackSettings[LIVESEQUENCER_NUM_TRACKS];
    std::list<MidiEvent> eventsList;
    std::unordered_map<uint8_t, std::list<MidiEvent>> songEvents; // should use std::map but name clashes with map()..
    uint8_t numberOfBars = 4;

    // volatile
    bool isActive;
    std::unordered_set<uint8_t> instrumentChannels;
    Track tracks[LIVESEQUENCER_NUM_TRACKS];
    ArpSettings arpSettings;
    uint8_t lastSongEventPattern; // because using unordered map above we need to know last index to be able to know song length (eg. for song loop)
    uint8_t currentPattern = 0;
    FillNotes fillNotes = { 4, 0 }; // user default
    unsigned long patternLengthMs;
    uint8_t activeTrack = 0;
    elapsedMillis patternTimer;
    std::unordered_map<uint8_t, MidiEvent> notesOn;
    std::vector<MidiEvent> pendingEvents;
    uint8_t songPatternCount = 0;
    uint8_t songLayerCount = 0;
    uint8_t lastPlayedNote = 0;
    bool trackLayersChanged = false;
    bool songLayersChanged = false;
    bool startedFlag = false;
    bool isRunning = false;
    bool isRecording = false;
    bool lastPlayedNoteChanged = false;
    bool isSongMode = false;
    bool recordedToSong = false;
    int currentBpm = 90;
    uint8_t performanceID = 0;
    uint8_t songMuteQuantizeDenom = 1;
  };

  LiveSequencer();
  LiveSequencer::LiveSeqData* getData(void);
  void songLayerAction(uint8_t layer, LayerMode action);
  void trackLayerAction(uint8_t track, uint8_t layer, LayerMode action);
  void handleMidiEvent(uint8_t inChannel, midi::MidiType event, uint8_t note, uint8_t velocity);
  void handlePatternBegin(void);
  void start(void);
  void stop(void);
  void onStarted(void);
  void onStopped(void);
  void init(void);
  void onGuiInit(void);
  void onArpSourceChanged(void);
  void setLayerMuted(uint8_t track, uint8_t layer, bool isMuted, bool recordToSong = false);
  void changeNumberOfBars(uint8_t num);
  void deleteAllSongEvents(void);
  void fillTrackLayer();
  void deleteLiveSequencerData(void);
  void setArpEnabled(bool enabled);
  void cleanEvents(void);
  uint32_t timeToMs(uint8_t patternNumber, uint16_t patternMs) const;

  struct NotePair {
    MidiEvent &noteOn;
    MidiEvent &noteOff;
  };

  void addNotePair(MidiEvent noteOn, MidiEvent noteOff);
  std::vector<NotePair> getNotePairsFromTrack(uint8_t track);
  static void printNotePairs(std::vector<std::vector<NotePair>> notePairs);
  static void printEvent(int i, MidiEvent e);
  static const std::string getEventName(midi::MidiType event);
  static const std::string getEventSource(EventSource source);

private:
  LiveSeqData data;
  std::set<uint8_t> pressedArpKeys;
  std::vector<ArpNote> activeArps;
  std::list<MidiEvent>::iterator playIterator;

  void checkLoadNewArpNotes(void);
  void onSongStopped(void);
  void updateTrackChannels(bool initial = false);
  void updateInstrumentChannels(void);
  void addPendingNotes(void);
  void refreshSongLength(void);
  void applySongStartLayerMutes(void);
  void printEvents();
  void loadNextEvent(int timeMs);
  void allTrackNotesOff(const uint8_t track);
  void allNotesOff(void);
  void playNextEvent(void);
  void playNextArpNote(void);
  void playArp(const midi::MidiType type, const ArpNote arp);
  bool timeQuantization(MidiEvent &e, uint8_t denom);
  void checkBpmChanged(void);
  void checkAddMetronome(void);
  void performLayerAction(LayerMode action, MidiEvent &e, uint8_t layer);

  static bool sortMidiEvent(MidiEvent &a, MidiEvent &b) {
    // + a.source is a hack to sort song events before pattern events if the have same time
    return ((a.patternNumber * 5000) + a.patternMs + a.source) < ((b.patternNumber * 5000) + b.patternMs + b.source); // FIXME: patternLengthMs
  }
};

void UI_func_livesequencer(uint8_t param);
void handle_touchscreen_live_sequencer(void);

#endif // LIVESEQUENCER_H
