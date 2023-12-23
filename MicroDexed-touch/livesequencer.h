#ifndef LIVESEQUENCER_H
#define LIVESEQUENCER_H

#include <string>
#include <vector>
#include <list>
#include "MIDI.h"
#include "TeensyTimerTool.h"
#include "ui_livesequencer.h"
#include "LCDMenuLib2_typedef.h"

class LiveSequencer {

public:
  struct MidiEvent {
    uint16_t patternMs;
    uint8_t patternNumber;
    uint8_t track;
    uint8_t layer;
    midi::MidiType event;
    uint8_t note_in;
    uint8_t note_in_velocity;
  };

  struct TrackInfo {
    midi::Channel channel;
    char name[4];
    LCDML_FuncPtr_pu8 screen;
    uint8_t layerCount;
    uint8_t layerMutes;
  };

  struct LiveSeqData {
    TrackInfo tracks[8];
    bool isRunning = false;
    bool isRecording = false;
    uint8_t activeRecordingTrack = 0;
    elapsedMillis patternTimer;
    unsigned long patternLengthMs;
    int numberOfBars = 4;
    uint16_t patternCount = 0;
    bool trackLayersChanged = false;
    bool patternBeginFlag = false;
  };

  LiveSequencer();
  LiveSequencer::LiveSeqData* getData(void);
  void clearLastTrackLayer(uint8_t track);
  void updateTrackChannels();
  void handleMidiEvent(midi::MidiType event, uint8_t note, uint8_t velocity);
  void handlePatternBegin(void);
  void handleStart(void);
  void handleStop(void);
  void init(int bpm);

private:
  LiveSeqData data;
  UI_LiveSequencer ui;
  uint32_t timeToMs(uint8_t patternNumber, uint16_t patternMs);

  static bool sortMidiEvent(MidiEvent &a, MidiEvent &b) {
    return ((a.patternNumber * 2000) + a.patternMs) < ((b.patternNumber * 2000) + b.patternMs); // FIXME: patternLengthMs
  }
  
  std::list<MidiEvent> eventsList;
  std::vector<MidiEvent> pendingEvents;
  std::list<MidiEvent>::iterator playIterator;

  int currentBpm = 90;

  TeensyTimerTool::OneShotTimer liveTimer;
  
  std::string getName(midi::MidiType event);
  void printEvent(int i, MidiEvent e);
  void printEvents();
  void loadNextEvent(int timeMs);
  void allNotesOff(void);
  
  void playNextEvent(void);
  void timeQuantization(uint8_t &patternNumber, uint16_t &patternMs, uint16_t multiple);
  void onBpmChanged(int bpm);

  uint16_t quantisizeDenom = 16; // 1/x
  uint16_t quantisizeMs = 150;
  
};

#endif //LIVESEQUENCER_H