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

const std::string LiveSequencer::getName(midi::MidiType event) const {
  switch(event) {
  case midi::NoteOn:
    return "NoteOn";
  case midi::NoteOff:
    return "NoteOff";
  case midi::InvalidType:
    return "Invalid";
  default:
    return "None";
  }
}

void LiveSequencer::handleStop(void) {
  playIterator = data.eventsList.end();
  allNotesOff();
  data.isRunning = false;
}

void LiveSequencer::handleStart(void) {
  data.patternCount = data.numberOfBars - 1;
  data.isRunning = true;
}

void LiveSequencer::allNotesOff(void) {
  for(int track = 0; track < LIVESEQUENCER_NUM_TRACKS; track++) {
    for(uint8_t layer = 0; layer < data.tracks[track].layerCount; layer++) {
      for(auto note : data.tracks[track].activeNotes[layer]) {
        handleNoteOff(data.tracks[track].channel, note, 0, 0);
      }
    }
  }
}

void LiveSequencer::printEvent(int i, MidiEvent e) {
  DBG_LOG(printf("[%i]: %i:%04i, (%i):%i, %s, %i, %i\n", i, e.patternNumber, e.patternMs, e.track, e.layer, getName(e.event).c_str(), e.note_in, e.note_in_velocity));
}

void LiveSequencer::timeQuantization(uint8_t &patternNumber, uint16_t &patternMs, uint16_t multiple) {
  const uint16_t halfStep = multiple / 2;
  uint8_t resultNumber = patternNumber;
  uint16_t resultMs = patternMs;
  // first round up an event just at end that was meant to be played at 1
  if((data.patternLengthMs - patternMs) < halfStep) {
    resultNumber++;
    if(resultNumber == data.numberOfBars) {
      resultNumber = 0;
    }
    resultMs = 0;
  }
  if(seq.track_type[data.activeTrack] == 0) {
    // drum track
    resultMs = ((patternMs + halfStep) / multiple) * multiple;
    DBG_LOG(printf("round %i to %i\n", patternMs, resultMs));
  }
  patternNumber = resultNumber;
  patternMs = resultMs;
}

void LiveSequencer::printEvents() {
  DBG_LOG(printf("--- %i events (%i bytes with one be %i bytes)\n", data.eventsList.size(), data.eventsList.size() * sizeof(MidiEvent), sizeof(MidiEvent)));
  uint i = 0;
  for(auto &e : data.eventsList) {
    printEvent(i++, e);
  }
}

void LiveSequencer::handleMidiEvent(midi::MidiType event, uint8_t note, uint8_t velocity) {
  if(data.isRecording && data.isRunning) {
    const MidiEvent newEvent = { uint16_t(data.patternTimer), data.patternCount, data.activeTrack, data.tracks[data.activeTrack].layerCount, event, note, velocity };
    switch(newEvent.event) {
    default:
      // ignore all other types
      break;

    case midi::NoteOn:
      if(data.tracks[data.activeTrack].layerCount < LIVESEQUENCER_NUM_LAYERS) {
        data.notesOn.insert(std::pair<uint8_t, MidiEvent>(note, newEvent));
      }
      break;

    case midi::NoteOff:
      // check if it has a corresponding NoteOn
      const auto on = data.notesOn.find(note);
      if(on != data.notesOn.end()) {
          // if so, insert NoteOn and this NoteOff to pending
          data.pendingEvents.push_back(on->second);
          data.pendingEvents.push_back(newEvent);
          data.notesOn.erase(on);
      }
      break;
    }
  }

  // forward midi with correct channel
  const midi::Channel ch = data.tracks[data.activeTrack].channel;
  switch(event) {
  case midi::NoteOn:
    handleNoteOn(ch, note, velocity, 0);
    break;
  
  case midi::NoteOff:
    handleNoteOff(ch, note, velocity, 0);
    break;
  
  default:
    break;
  }
}

void LiveSequencer::clearLastTrackLayer(uint8_t track) {
  if(data.pendingEvents.size()) {
    // if still in pending sequence, delete pending events
    data.pendingEvents.clear();
  } else {
    // if already finished sequence (pending have been added), delete highest layer
    if(data.tracks[data.activeTrack].layerCount > 0) {
      data.tracks[data.activeTrack].layerCount--;
      data.trackLayersChanged = true;
    }

    for(auto &e : data.eventsList) {
       if(e.track == track && e.layer == data.tracks[data.activeTrack].layerCount) {
        if(e.event == midi::NoteOff) {
          handleNoteOff(data.tracks[e.track].channel, e.note_in, e.note_in_velocity, 0);
        }
        e.event = midi::InvalidType; // delete later
      }
    }
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
    const bool isMuted = data.tracks[playIterator->track].layerMutes & (1 << playIterator->layer);
    if(isMuted == false) {
      const midi::Channel channel = data.tracks[playIterator->track].channel;

      switch(playIterator->event) {   
      case midi::NoteOff: {
        // erase one instance of this note going off
        const auto it = data.tracks[playIterator->track].activeNotes[playIterator->layer].find(playIterator->note_in);
        if(it != data.tracks[playIterator->track].activeNotes[playIterator->layer].end()) {
          data.tracks[playIterator->track].activeNotes[playIterator->layer].erase(it);
        } else {
          // FIXME: noteOn not registered..
        }
        handleNoteOff(channel, playIterator->note_in, playIterator->note_in_velocity, 0);
        }
        break;

      case midi::NoteOn:
        // add active note to layer track set
        data.tracks[playIterator->track].activeNotes[playIterator->layer].insert(playIterator->note_in);
        // handle muted tracks
        handleNoteOn(channel, playIterator->note_in, playIterator->note_in_velocity, 0);
        break;

      default:
        break;
      }
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
  liveTimer.begin([this] { playNextEvent(); });
  data.pendingEvents.reserve(50);
}

void LiveSequencer::checkBpmChanged() {
  if(seq.bpm != currentBpm) {
    float resampleFactor =  currentBpm / float(seq.bpm);
    quantisizeMs = data.patternLengthMs / data.quantisizeDenom;
    currentBpm = seq.bpm;
    for(auto &e : data.eventsList) {
      e.patternMs *= resampleFactor;
      timeQuantization(e.patternNumber, e.patternMs, quantisizeMs);
    }
  }
}

void LiveSequencer::handlePatternBegin(void) {
  // seq.tempo_ms = 60000000 / seq.bpm / 4; // rly?
  data.patternTimer = 0;

  if(++data.patternCount == data.numberOfBars) {
    data.patternCount = 0;
    
    // first insert pending to events and sort
    if(data.pendingEvents.size()) {
      for(auto &e : data.pendingEvents) {
        timeQuantization(e.patternNumber, e.patternMs, quantisizeMs);
        data.eventsList.emplace_back(e);
      }
      data.pendingEvents.clear();

      data.eventsList.sort(sortMidiEvent);
      data.tracks[data.activeTrack].layerMutes &= ~(1 << data.tracks[data.activeTrack].layerCount); // set new unmuted
      data.tracks[data.activeTrack].layerCount++;
      data.trackLayersChanged = true;
    }

    if(data.eventsList.size() > 0) {
      // remove all invalidated notes
      data.eventsList.remove_if([](MidiEvent &e){ return e.event == midi::InvalidType; });
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

void LiveSequencer::handleLayerMuteChanged(uint8_t track, uint8_t layer, bool isMuted) {
  if(isMuted == true) {
    for(auto note : data.tracks[track].activeNotes[layer]) {
      handleNoteOff(data.tracks[track].channel, note, 0, 0);
    }
  }
}

void LiveSequencer::updateTrackChannels() {
  for(uint8_t i = 0; i < LIVESEQUENCER_NUM_TRACKS; i++) {
    data.tracks[i].screenSetupFn = nullptr;
    switch(seq.track_type[i]) {
    case 0:
      data.tracks[i].channel = static_cast<midi::Channel>(drum_midi_channel);
      data.tracks[i].screen = UI_func_drums;
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