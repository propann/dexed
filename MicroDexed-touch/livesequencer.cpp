#include "livesequencer.h"
#include "config.h"
#include "sequencer.h"
#include <map>
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

std::map<uint8_t, LiveSequencer::MidiEvent> notesOn;

LiveSequencer::LiveSequencer() :
  ui(this) {
}

LiveSequencer::LiveSeqData* LiveSequencer::getData(void) {
  return &data;
}

std::string LiveSequencer::getName(midi::MidiType event) {
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
  playIterator = eventsList.end();
  allNotesOff();
  data.isRunning = false;
}

void LiveSequencer::handleStart(void) {
  data.patternCount = data.numberOfBars - 1;
  data.isRunning = true;
}

void LiveSequencer::allNotesOff(void) {
  for(auto &e : eventsList) {
    if(e.event == midi::NoteOff) {
      handleNoteOff(data.tracks[e.track].channel, e.note_in, e.note_in_velocity, 0);
    }
  }
}

void LiveSequencer::printEvent(int i, MidiEvent e) {
  Serial.printf("[%i]: %i:%04i, (%i):%i, %s, %i, %i\n", i, e.patternNumber, e.patternMs, e.track, e.layer, getName(e.event).c_str(), e.note_in, e.note_in_velocity);
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
  if(seq.track_type[data.activeRecordingTrack] == 0) {
    // drum track
    resultMs = ((patternMs + halfStep) / multiple) * multiple;
    Serial.printf("round %i to %i\n", patternMs, resultMs);
  }
  patternNumber = resultNumber;
  patternMs = resultMs;
}

void LiveSequencer::printEvents() {
  Serial.printf("--- %i events (%i bytes with one be %i bytes)\n", eventsList.size(), eventsList.size() * sizeof(MidiEvent), sizeof(MidiEvent));
  uint i = 0;
  for(auto &e : eventsList) {
    printEvent(i++, e);
  }
}

void LiveSequencer::handleMidiEvent(midi::MidiType event, uint8_t note, uint8_t velocity) {
  if(data.isRecording && data.isRunning) {
    uint8_t patternNumber = data.patternCount;
    uint16_t patternMs = data.patternTimer;
    timeQuantization(patternNumber, patternMs, quantisizeMs);
  
    MidiEvent newEvent = { patternMs, patternNumber, data.activeRecordingTrack, data.tracks[data.activeRecordingTrack].layerCount, event, note, velocity };
    switch(newEvent.event) {
    default:
      // ignore all other types
      break;

    case midi::NoteOn:
      notesOn.insert(std::pair<uint8_t, MidiEvent>(note, newEvent));
      break;

    case midi::NoteOff:
      // check if it has a corresponding NoteOn
      const auto on = notesOn.find(note);
      if(on != notesOn.end()) {
          // if so, insert NoteOn and this NoteOff to pending
          pendingEvents.push_back(on->second);
          pendingEvents.push_back(newEvent);
          notesOn.erase(on);
      }
      break;
    }
  }

  // forward midi with correct channel
  midi::Channel ch = data.tracks[data.activeRecordingTrack].channel;
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
  if(pendingEvents.size()) {
    // if still in pending sequence, delete pending events
    pendingEvents.clear();
  } else {
    // if already finished sequence (pending have been added), delete highest layer
    if(data.tracks[data.activeRecordingTrack].layerCount > 0) {
      data.tracks[data.activeRecordingTrack].layerCount--;
      data.trackLayersChanged = true;
    }

    for(auto &e : eventsList) {
       if(e.track == track && e.layer == data.tracks[data.activeRecordingTrack].layerCount) {
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
    //Serial.printf("trigger in %ims\n", timeMs);
    liveTimer.trigger(timeMs * 1000);
  } else {
    playNextEvent();
  }
}

void LiveSequencer::playNextEvent(void) {
  if(playIterator != eventsList.end()) {
    const unsigned long now = ((data.patternCount * data.patternLengthMs) + data.patternTimer);
    //Serial.printf("PLAY: ");
    //printEvent(1, *playIterator);
    midi::Channel channel = data.tracks[playIterator->track].channel;
    switch(playIterator->event) {
    case midi::NoteOn:
      // handle muted tracks
      handleNoteOn(channel, playIterator->note_in, data.tracks[playIterator->track].layerMutes & (1 << playIterator->layer) ? 0 : playIterator->note_in_velocity, 0);
      break;
    
    case midi::NoteOff:
      handleNoteOff(channel, playIterator->note_in, playIterator->note_in_velocity, 0);
      break;
    
    default:
      break;
    }
    if(++playIterator != eventsList.end()) {
      int timeToNextEvent = timeToMs(playIterator->patternNumber, playIterator->patternMs) - now;
      loadNextEvent(timeToNextEvent);
    }
  }
}

inline uint32_t LiveSequencer::timeToMs(uint8_t patternNumber, uint16_t patternMs) {
  return (patternNumber * data.patternLengthMs) + patternMs;
}

//void LiveSequencer::init(int bpm, std::vector<MidiEvent> loadedEvents) {
void LiveSequencer::init(int bpm) {
  //events = loadedEvents;
  onBpmChanged(bpm);
  liveTimer.begin([this] { playNextEvent(); });
  pendingEvents.reserve(50);
}

void LiveSequencer::onBpmChanged(int bpm) {
  float resampleFactor =  currentBpm / float(bpm);
  data.patternLengthMs = (4 * 1000 * 60) / bpm; // for a 4/4 signature
  quantisizeMs = data.patternLengthMs / quantisizeDenom;
  currentBpm = bpm;
  for(auto &e : eventsList) {
    e.patternMs *= resampleFactor;
    timeQuantization(e.patternNumber, e.patternMs, quantisizeMs);
  }
}

void LiveSequencer::handlePatternBegin(void) {
  // seq.tempo_ms = 60000000 / seq.bpm / 4; // rly?
  data.patternTimer = 0;

  if(++data.patternCount == data.numberOfBars) {
    data.patternCount = 0;
    
    // first insert pending to events and sort
    if(pendingEvents.size()) {
      for(auto &e : pendingEvents) {
        eventsList.emplace_back(e);
      }
      pendingEvents.clear();

      eventsList.sort(sortMidiEvent);
      data.tracks[data.activeRecordingTrack].layerCount++;
      data.trackLayersChanged = true;
    }

    // react to external bpm change
    if(currentBpm != seq.bpm) {
      onBpmChanged(seq.bpm);
    }
    
    if(eventsList.size() > 0) {
      // remove all invalidated notes
      eventsList.remove_if([](MidiEvent &e){ return e.event == midi::InvalidType; });
      //printEvents();
      playIterator = eventsList.begin();
      loadNextEvent(timeToMs(playIterator->patternNumber, playIterator->patternMs));
    }
  }
  Serial.printf("Sequence %i/%i @%ibpm : %ims with %i events\n", data.patternCount + 1, data.numberOfBars, currentBpm, data.patternLengthMs, eventsList.size());
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

void LiveSequencer::updateTrackChannels() {
  for(uint8_t i = 0; i < NUM_SEQ_TRACKS; i++) {
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