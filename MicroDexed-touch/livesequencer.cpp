#include "livesequencer.h"
#include "config.h"
#include "sequencer.h"
#include <algorithm>

extern void handleNoteOn(byte, byte, byte, byte);
extern void handleNoteOff(byte, byte, byte, byte);
extern sequencer_t seq;
extern uint8_t drum_midi_channel;
extern config_t configuration;
extern microsynth_t microsynth[2];
extern braids_t braids_osc;

extern void UI_func_drums(uint8_t param);
extern void UI_func_voice_select(uint8_t param);
extern void UI_func_microsynth(uint8_t param);
extern void UI_func_epiano(uint8_t param);
extern void UI_func_braids(uint8_t param);

extern uint8_t microsynth_selected_instance;
extern uint8_t selected_instance_id; // dexed

LiveSequencer::LiveSequencer() :
  ui(this) {
}

LiveSequencer::LiveSeqData* LiveSequencer::getData(void) {
  return &data;
}

const std::string LiveSequencer::getEventName(midi::MidiType event) const {
  switch(event) {
  case midi::NoteOn:
    return "NoteOn";
  case midi::NoteOff:
    return "NoteOff";
  case midi::ControlChange:
    return "Automation";
  case midi::InvalidType:
    return "Invalid";
  default:
    return "None";
  }
}

const std::string LiveSequencer::getEventSource(LiveSequencer::EventSource source) const {
  switch(source) {
  case EVENT_PATTERN:
    return "PATT";
  case EVENT_SONG:
    return "SONG";
  default:
    return "NONE";
  }
}

void LiveSequencer::handleStop(void) {
  if(data.isSongMode) {
    onSongStopped();
  }
  data.isRunning = false;
  playIterator = data.eventsList.end();
  allNotesOff();
  data.songPatternCount = data.lastSongEventPattern; // show song length
  data.patternCount = data.numberOfBars; // show num bars
}

void LiveSequencer::handleStart(void) {
  data.startedFlag = true;
  data.isRunning = true;
  if(data.isSongMode) {
    data.eventsList.remove_if([](MidiEvent &e){ return e.source == EventSource::EVENT_SONG; });
    // when using std::map for songEvents, simply use std::prev(map.end())->first
    for(uint i = 0; i < data.songEvents.size(); i++) {
      if(data.songEvents[i].size()) {
        data.lastSongEventPattern = i;
      }
    }
  }
}

void LiveSequencer::allNotesOff(void) {
  for(int track = 0; track < LIVESEQUENCER_NUM_TRACKS; track++) {
    for(uint8_t layer = 0; layer < data.tracks[track].layerCount; layer++) {
      for(auto note : data.tracks[track].activeNotes[layer]) {
        handleNoteOff(data.tracks[track].channel, note, 0, 0);
      }
      data.tracks[track].activeNotes[layer].clear();
    }
  }
}

void LiveSequencer::printEvent(int i, MidiEvent e) {
  DBG_LOG(printf("[%i]: %i:%04i {%s} (%i):%i, %s, %i, %i\n", i, e.patternNumber, e.patternMs, getEventSource(e.source).c_str(), e.track, e.layer, getEventName(e.event).c_str(), e.note_in, e.note_in_velocity));
}

bool LiveSequencer::timeQuantization(uint8_t &patternNumber, uint16_t &patternMs, uint16_t multiple) {
  bool overflow = false; // overflow if event rounded to start of next pattern
  if(data.patternLengthMs != multiple) {
    const uint16_t halfStep = multiple / 2;
    uint8_t resultNumber = patternNumber;
    uint16_t resultMs = patternMs;
    resultMs = ((patternMs + halfStep) / multiple) * multiple;
    if(resultMs == data.patternLengthMs) {
      resultMs = 0;
      if(++resultNumber == data.numberOfBars) {
        resultNumber = 0;
        overflow = true;
      }
    }
    DBG_LOG(printf("round %i.%i to %i.%i\n", patternNumber, patternMs, resultNumber, resultMs));
    patternNumber = resultNumber;
    patternMs = resultMs;
  }
  return overflow;
}

void LiveSequencer::printEvents() {
  DBG_LOG(printf("--- %i pattern events (%i bytes with one be %i bytes)\n", data.eventsList.size(), data.eventsList.size() * sizeof(MidiEvent), sizeof(MidiEvent)));
  uint i = 0;
  for(auto &e : data.eventsList) {
    printEvent(i++, e);
  }
  DBG_LOG(printf("--- song events on %i layers until pattern %i\n", data.songLayerCount, data.lastSongEventPattern));
  for(int i = 0; i <= data.lastSongEventPattern; i++) {
    for(auto &e : data.songEvents[i]) {
      printEvent(i++, e);
    }
  }
}

bool LiveSequencer::isEventMute(const MidiEvent e) const {
  return (e.event == midi::ControlChange) && (e.note_in_velocity == TYPE_MUTE_ON || e.note_in_velocity == TYPE_MUTE_OFF);
}

void LiveSequencer::onSongStopped(void) {
  bool incrementSongLayer = false;
  for(auto &e : data.songEvents) {
    e.second.remove_if([](MidiEvent &e) { return (e.event == midi::InvalidType); });
    for(auto &a : e.second) {
      const bool isSongMuteBegin = isEventMute(a) && (e.first == 0) && (a.patternMs == 0) && (a.patternNumber == 0);
      if(isSongMuteBegin) {
        // apply possible existing layer mutes from song recording to not change song start layer mutes at next song recording
        setLayerMuted(a.track, a.note_in, a.note_in_velocity == TYPE_MUTE_ON);
        data.trackLayersChanged = true;
      } else {
        // check if we have to increment song recording layer
        if(a.layer == data.songLayerCount) {
          incrementSongLayer = true;
        }
      }
    }
  }
  if((data.songLayerCount < LIVESEQUENCER_NUM_LAYERS - 1) && incrementSongLayer) {
    data.songLayerCount++;
    data.songLayersChanged = true;
  }
}

void LiveSequencer::handleMidiEvent(midi::MidiType event, uint8_t note, uint8_t velocity) {
  if(data.isRecording && data.isRunning) {
    const uint16_t quantisizeMs = data.patternLengthMs / data.tracks[data.activeTrack].quantisizeDenom;
    const EventSource source = data.isSongMode ? EVENT_SONG : EVENT_PATTERN;
    MidiEvent newEvent = { source, uint16_t(data.patternTimer), data.patternCount, data.activeTrack, data.tracks[data.activeTrack].layerCount, event, note, velocity };
    if(data.isSongMode) {
      if(data.songLayerCount < LIVESEQUENCER_NUM_LAYERS) {
        // in song mode, simply add event, no rounding and checking needed
        newEvent.layer = data.songLayerCount; 
        uint8_t patternCount = data.songPatternCount;
        if(newEvent.event == midi::NoteOn) {
          if(timeQuantization(newEvent.patternNumber, newEvent.patternMs, quantisizeMs)) {
            patternCount++; // event rounded up to start of next song pattern
          }
        }
        if(patternCount > data.lastSongEventPattern) {
          data.lastSongEventPattern = patternCount;
          DBG_LOG(printf("song increased to %i patterns...\n", data.lastSongEventPattern));
        }
        data.songEvents[patternCount].emplace_back(newEvent);
      }
    } else {
      if(data.tracks[data.activeTrack].layerCount < LIVESEQUENCER_NUM_LAYERS) {    
        switch(newEvent.event) {
        default:
          // ignore all other types
          break;

        case midi::NoteOn:
          static constexpr int ROUND_UP_MS = 100;
          // round up events just at end probably meant to be played at start
          if(newEvent.patternNumber == (data.numberOfBars - 1) && newEvent.patternMs > (data.patternLengthMs - ROUND_UP_MS)) {
            newEvent.patternNumber = 0;
            newEvent.patternMs = 0;
          }
          data.notesOn.insert(std::pair<uint8_t, MidiEvent>(note, newEvent));
          break;

        case midi::NoteOff:
          // check if it has a corresponding NoteOn
          const auto on = data.notesOn.find(note);
          if(on != data.notesOn.end()) {
              timeQuantization(on->second.patternNumber, on->second.patternMs, quantisizeMs);
              // if so, insert NoteOn and this NoteOff to pending
              data.pendingEvents.emplace_back(on->second);
              data.pendingEvents.emplace_back(newEvent);
              data.notesOn.erase(on);
          }
          break;
        }
      }
    }
  }

  // forward midi with correct channel
  const midi::Channel ch = data.tracks[data.activeTrack].channel;
  switch(event) {
  case midi::NoteOn:
    handleNoteOn(ch, note, velocity, 0);

    if(data.lastPlayedNote != note) {
      data.lastPlayedNote = note;
      data.lastPlayedNoteChanged = true;
    }
    break;
  
  case midi::NoteOff:
    handleNoteOff(ch, note, velocity, 0);
    break;
  
  default:
    break;
  }
}

void LiveSequencer::fillTrackLayer(void) {
  if(data.tracks[data.activeTrack].layerCount < LIVESEQUENCER_NUM_LAYERS) {
    const uint16_t msIncrement = data.patternLengthMs / data.fillNotes.number;
    const uint16_t msOffset = data.fillNotes.offset * msIncrement / 8;
    const uint16_t noteLength = msIncrement / 2; // ...
    for(uint8_t bar = 0; bar < data.numberOfBars; bar++) {
      for(uint16_t note = 0; note < data.fillNotes.number; note++) {
        // { uint16_t(data.patternTimer), data.patternCount, data.activeTrack, data.tracks[data.activeTrack].layerCount, event, note, velocity }
        data.pendingEvents.emplace_back(MidiEvent { EVENT_PATTERN, uint16_t(note * msIncrement + msOffset), bar, data.activeTrack, data.tracks[data.activeTrack].layerCount, midi::NoteOn, data.lastPlayedNote, 127 } );
        data.pendingEvents.emplace_back(MidiEvent { EVENT_PATTERN, uint16_t(note * msIncrement + noteLength + msOffset), bar, data.activeTrack, data.tracks[data.activeTrack].layerCount, midi::NoteOff, data.lastPlayedNote, 0 } );
      }
    }
    addPendingNotes();
  }
}

void LiveSequencer::changeNumberOfBars(uint8_t num) {
  if(num != data.numberOfBars) {
    data.pendingEvents.clear();
    data.eventsList.clear();
    deleteAllSongEvents();
    for(int track = 0; track < LIVESEQUENCER_NUM_TRACKS; track++) {
      data.tracks[track].layerCount = 0;
      data.tracks[track].layerMutes = 0;
    }
    data.numberOfBars = num;
    init();
  }
}

void LiveSequencer::deleteAllSongEvents(void) {
  data.songEvents.clear();
  data.lastSongEventPattern = 0;

  for(auto &e : data.eventsList) {
    if(e.source == EVENT_SONG) {
      e.event = midi::InvalidType; // mark as invalid
    }
  }
  // TODO: set to state when start was pressed (remember this)
  for(uint8_t i = 0; i < LIVESEQUENCER_NUM_TRACKS; i++) {
    data.tracks[i].layerMutes = 0;
  }
}

void LiveSequencer::songLayerAction(uint8_t layer, LayerMode action) {
  if((layer == 0) && (action == LayerMode::LAYER_MERGE_UP)) {
    return; // avoid merge up top layer
  }
  for(auto &e : data.songEvents) {
    for(auto &a : e.second) {
      performLayerAction(action, a, layer);
    }
  }
  data.songLayerCount--;
  data.songLayersChanged = true;
}

void LiveSequencer::trackLayerAction(uint8_t track, uint8_t layer, LayerMode action) {
  if((layer == 0) && (action == LayerMode::LAYER_MERGE_UP)) {
    return; // avoid merge up top layer
  }

  // play noteOff for active layer notes
  for(auto &note : data.tracks[track].activeNotes[layer]){
    handleNoteOff(data.tracks[track].channel, note, 0, 0);
  }
  data.tracks[track].activeNotes[layer].clear();

  for(auto &e : data.eventsList) {
    if(e.track == track) {
      performLayerAction(action, e, layer);
    }
  }
  // handle layer mutes. example with layer 2 deleted
  // old: 0010 1101
  // new: 0001 0101 -> lower layers stay same, higher layers shifted down by one
  const uint8_t bitmask = pow(2, layer) - 1;
  const uint8_t layerMutesLo = data.tracks[track].layerMutes & bitmask;         // 0010 1101 &  0000 0011 = 0000 0001
  const uint8_t layerMutesHi = (data.tracks[track].layerMutes >> 1) & ~bitmask; // 0001 0110 & ~0000 0011 = 0001 0100
  data.tracks[track].layerMutes = (layerMutesLo | layerMutesHi);                // 0000 0001 |  0001 0100 = 0001 0101

  data.tracks[track].layerCount--;
  data.trackLayersChanged = true;
}

void LiveSequencer::performLayerAction(LayerMode action, LiveSequencer::MidiEvent &e, uint8_t layer) {
  if(e.layer == layer) {
    switch(action) {
    case LayerMode::LAYER_MERGE_UP:
      // layer 0 must not be shifted up
      if(e.layer > 0) { 
        e.layer--;
      }
      break;

    case LayerMode::LAYER_DELETE:
      e.event = midi::InvalidType; // mark layer notes to delete later
      break;

    default:
      break;
    }
  }
  
  // both actions above shift upper layers one lower
  if (e.layer > layer) {
    e.layer--;
  }
}

void LiveSequencer::loadNextEvent(int timeMs) {
  if(timeMs > 0) {
    //LOG.printf("trigger in %ims\n", timeMs);
    liveTimer.trigger(timeMs * 1000);
  } else {
    playNextEvent();
  }
}

void LiveSequencer::playNextEvent(void) {
  if(playIterator != data.eventsList.end()) {
    //LOG.printf("PLAY: ");
    //printEvent(1, *playIterator);
    const bool isMuted = (playIterator->source == EventSource::EVENT_PATTERN) && (data.tracks[playIterator->track].layerMutes & (1 << playIterator->layer));
    const midi::Channel channel = data.tracks[playIterator->track].channel;

    switch(playIterator->event) {   
    case midi::ControlChange:
      switch(playIterator->note_in_velocity) {
      case AutomationType::TYPE_MUTE_ON:
      case AutomationType::TYPE_MUTE_OFF:
        //DBG_LOG(printf("mute %s\n", playIterator->note_in_velocity ? "MUTE" : "UNMUTE"));
        setLayerMuted(playIterator->track, playIterator->note_in, playIterator->note_in_velocity == AutomationType::TYPE_MUTE_ON);
        data.trackLayersChanged = true;
        break;
      }
      break;
    case midi::NoteOff: 
      if(isMuted == false) {
        // erase one instance of this note going off
        const auto it = data.tracks[playIterator->track].activeNotes[playIterator->layer].find(playIterator->note_in);
        if(it != data.tracks[playIterator->track].activeNotes[playIterator->layer].end()) {
          data.tracks[playIterator->track].activeNotes[playIterator->layer].erase(it);
        }
        handleNoteOff(channel, playIterator->note_in, playIterator->note_in_velocity, 0);
      }
      break;

    case midi::NoteOn:
      if(isMuted == false) {
        // add active note to layer track set
        data.tracks[playIterator->track].activeNotes[playIterator->layer].insert(playIterator->note_in);
        // handle muted tracks
        handleNoteOn(channel, playIterator->note_in, playIterator->note_in_velocity, 0);
      }
      break;

    default:
      break;
    }
    if(playIterator->source == EVENT_SONG) {
      playIterator->event = midi::InvalidType; // mark song events to delete after execution
    }
    if(++playIterator != data.eventsList.end()) {
      const unsigned long now = ((data.patternCount * data.patternLengthMs) + data.patternTimer);
      int timeToNextEvent = timeToMs(playIterator->patternNumber, playIterator->patternMs) - now;
      loadNextEvent(timeToNextEvent);
    }
  }
}

inline uint32_t LiveSequencer::timeToMs(uint8_t patternNumber, uint16_t patternMs) const {
  return (patternNumber * data.patternLengthMs) + patternMs;
}

//void LiveSequencer::init(int bpm, std::vector<MidiEvent> loadedEvents) {
void LiveSequencer::init(void) {
  //events = loadedEvents;
  data.patternLengthMs = (4 * 1000 * 60) / seq.bpm; // for a 4/4 signature
  checkBpmChanged();
  updateTrackChannels();
  checkAddMetronome();
  liveTimer.begin([this] { playNextEvent(); });
  data.pendingEvents.reserve(50);
}

void LiveSequencer::checkBpmChanged() {
  if(seq.bpm != currentBpm) {
    float resampleFactor =  currentBpm / float(seq.bpm);
    currentBpm = seq.bpm;
    // resample pattern events
    for(auto &e : data.eventsList) {
      e.patternMs *= resampleFactor;
    }
    // resample song events
    for(auto &e : data.songEvents) {
      for(auto &a : e.second) {
        a.patternMs *= resampleFactor;
      }
    }
  }
}

void LiveSequencer::addPendingNotes(void) {
  // finish active notes at end of all bars
  for(auto it = data.notesOn.begin(); it != data.notesOn.end();) {
    if(timeToMs(it->second.patternNumber, it->second.patternMs) != 0) {
      data.pendingEvents.emplace_back(it->second);
      it->second.event = midi::NoteOff;
      it->second.note_in_velocity = 0;
      it->second.patternNumber = data.numberOfBars - 1;
      it->second.patternMs = data.patternLengthMs - 1;
      data.pendingEvents.emplace_back(it->second);
      data.notesOn.erase(it++);
    } else {
      ++it; // ignore notesOn at 0.0000, those were round up just before
    }
  }
  // then add all notes to events and sort
  if(data.pendingEvents.size()) {
    for(auto &e : data.pendingEvents) {
      data.eventsList.emplace_back(e);
    }
    data.pendingEvents.clear();
    data.eventsList.sort(sortMidiEvent);
    data.tracks[data.activeTrack].layerCount++;
    data.trackLayersChanged = true;
  }
}

void LiveSequencer::handlePatternBegin(void) {
  data.patternTimer = 0;

  if(data.startedFlag) {
    // just started, do not increment
    data.startedFlag = false;
    data.patternCount = 0;
    data.songPatternCount = 0;

    // store current track mutes for song mode and replace with possibly previously stored ones
    if(data.isSongMode && data.isRecording) {
      // delete previous mutes at beginning
      data.songEvents[0].remove_if([ this ](MidiEvent &e) { return (e.patternMs == 0) && (e.patternNumber == 0) && isEventMute(e); });
      // store all track mute states
      for(uint8_t track = 0; track < LIVESEQUENCER_NUM_TRACKS; track++) {
        for(uint8_t layer = 0; layer < data.tracks[track].layerCount; layer++) {
          const bool isLayerMuted = data.tracks[track].layerMutes & (1 << layer);
          setLayerMuted(track, layer, isLayerMuted, true);
        }
      }
    }
  } else {
    if((data.patternCount + 1) == data.numberOfBars) {
      data.patternCount = 0;
      if(data.isSongMode) {
        if((data.isRecording == false) && (data.songPatternCount == data.lastSongEventPattern)) {
          DBG_LOG(printf("song ended. restart from beginning...\n"));
          data.songPatternCount = 0;
        } else {
          data.songPatternCount++;
        }
      }
    } else {
      data.patternCount++;
    }
  }

  if(data.patternCount == 0) {
    if(data.isSongMode == false) {
      // first insert pending EVENT_PATT events to events and sort
      addPendingNotes();
    }

    if(data.eventsList.size() > 0) {
      // remove all invalidated notes
      data.eventsList.remove_if([](MidiEvent &e) { return e.event == midi::InvalidType; });

      // for song mode, add song events for this pattern
      if(data.isSongMode) {
        for(auto &e : data.songEvents[data.songPatternCount]) {
          data.eventsList.emplace_back(e);
        }
        data.eventsList.sort(sortMidiEvent);
      }

      printEvents();
      playIterator = data.eventsList.begin();
      loadNextEvent(timeToMs(playIterator->patternNumber, playIterator->patternMs));
    }
  }
  DBG_LOG(printf("Sequence %i/%i @%ibpm : %ims with %i events\n", data.patternCount + 1, data.numberOfBars, currentBpm, data.patternLengthMs, data.eventsList.size()));
  data.patternBeginFlag = true;
}

void selectDexed0() {
  selected_instance_id = 0;
}

void selectDexed1() {
  selected_instance_id = 1;
}

void selectMs0() {
  microsynth_selected_instance = 0;
}

void selectMs1() {
  microsynth_selected_instance = 1;
}

void LiveSequencer::setLayerMuted(uint8_t track, uint8_t layer, bool isMuted, bool recordToSong) {
  if(isMuted) {
    data.tracks[track].layerMutes |= (1 << layer);
    for(auto note : data.tracks[track].activeNotes[layer]) {
      handleNoteOff(data.tracks[track].channel, note, 0, 0);
    }
    data.tracks[track].activeNotes[layer].clear();
  } else {
    data.tracks[track].layerMutes &= ~(1 << layer);
  }
  if(recordToSong) {
    if(data.songLayerCount < LIVESEQUENCER_NUM_LAYERS) {
      const AutomationType type = isMuted ? AutomationType::TYPE_MUTE_ON : AutomationType::TYPE_MUTE_OFF;
      MidiEvent e = { EVENT_SONG, uint16_t(data.patternTimer), data.patternCount, track, data.songLayerCount, midi::MidiType::ControlChange, layer, type };
      data.songEvents[data.songPatternCount].emplace_back(e);
      DBG_LOG(printf("record muted %i at %i of song pattern count %i\n", isMuted, timeToMs(data.patternCount, data.patternTimer), data.songPatternCount));
    }
  }
}

void LiveSequencer::checkAddMetronome(void) {
  // if we have a fresh start, add layers to have some tempo
  if(data.eventsList.empty()) {
    for(uint8_t i = 0; i < LIVESEQUENCER_NUM_TRACKS; i++) {
      if(data.tracks[i].screen == UI_func_drums) {
        data.activeTrack = i;
        data.fillNotes.number = 8;
        data.fillNotes.offset = 0;
        data.lastPlayedNote = 54; // hats
        fillTrackLayer();
        data.fillNotes.number = 1;
        data.fillNotes.offset = 0;
        data.lastPlayedNote = 48; // kick
        fillTrackLayer();
        trackLayerAction(i, 1, LayerMode::LAYER_MERGE_UP); // merge them
        // reset fillNotes to user values
        data.fillNotes.number = 4;
        data.fillNotes.offset = 0;
        return;
      }
    }
  }
}

void LiveSequencer::updateTrackChannels() {
  for(uint8_t i = 0; i < LIVESEQUENCER_NUM_TRACKS; i++) {
    data.tracks[i].screenSetupFn = nullptr;
    data.tracks[i].quantisizeDenom = 1; // default: no quantization
    switch(seq.track_type[i]) {
    case 0:
      data.tracks[i].channel = static_cast<midi::Channel>(drum_midi_channel);
      data.tracks[i].screen = UI_func_drums;
      data.tracks[i].quantisizeDenom = 16; // default: drum quantisize to 1/16
      sprintf(data.tracks[i].name, "DRM");
      break;

    case 1:
      // dexed instance 0+1, 2 = epiano , 3+4 = MicroSynth, 5 = Braids
      switch(seq.instrument[i]) {
        case 0:
        case 1:
          data.tracks[i].channel = static_cast<midi::Channel>(configuration.dexed[seq.instrument[i]].midi_channel);
          data.tracks[i].screen = UI_func_voice_select;
          if(seq.instrument[i] == 0) {
            data.tracks[i].screenSetupFn = (SetupFn)selectDexed0;
          } else {
            data.tracks[i].screenSetupFn = (SetupFn)selectDexed1;
          }
          
          sprintf(data.tracks[i].name, "DX%i", seq.instrument[i] + 1);
          break;

        case 2:
          data.tracks[i].channel = static_cast<midi::Channel>(configuration.epiano.midi_channel);
          data.tracks[i].screen = UI_func_epiano;
          sprintf(data.tracks[i].name, "EP");
          break;

        case 3:
        case 4:
          data.tracks[i].channel = microsynth[seq.instrument[i] - 3].midi_channel;
          data.tracks[i].screen = UI_func_microsynth;
          if(seq.instrument[i] == 3) {
            data.tracks[i].screenSetupFn = (SetupFn)selectMs0;
          } else {
            data.tracks[i].screenSetupFn = (SetupFn)selectMs1;
          }
          sprintf(data.tracks[i].name, "MS%i", seq.instrument[i] - 2);
          break;

        case 5:
          data.tracks[i].channel = braids_osc.midi_channel;
          data.tracks[i].screen = UI_func_braids;
          sprintf(data.tracks[i].name, "BRD");
      }
      break;
    }
  }
}