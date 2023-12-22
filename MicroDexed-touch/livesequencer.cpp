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

std::map<uint8_t, LiveSequencer::MidiEvent> notesOn;

LiveSequencer::LiveSequencer() :
  ui(&data) {
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
      handleNoteOff(data.trackChannels[e.track], e.note_in, e.note_in_velocity, 0);
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
    switch(note) {
    case 48: // clear track
      if(event == midi::NoteOn) {
        clearTrackEvents(data.activeRecordingTrack);
      }
      return;

    default:
      break;
    }

    uint8_t patternNumber = data.patternCount;
    uint16_t patternMs = data.patternTimer;
    timeQuantization(patternNumber, patternMs, quantisizeMs);
  
    MidiEvent newEvent = { patternMs, patternNumber, data.activeRecordingTrack, data.trackLayers[data.activeRecordingTrack], event, note, velocity };
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
  midi::Channel ch = data.trackChannels[data.activeRecordingTrack];
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

void LiveSequencer::clearTrackEvents(uint8_t track) {
  if(pendingEvents.size()) {
    // if still in pending sequence, delete pending events
    pendingEvents.clear();
  } else {
    // if already finished sequence (pending have been added), delete highest layer
    if(data.trackLayers[data.activeRecordingTrack] > 0) {
      data.trackLayers[data.activeRecordingTrack]--;
    }

    for(auto &e : eventsList) {
       if(e.track == track && e.layer == data.trackLayers[data.activeRecordingTrack]) {
        if(e.event == midi::NoteOff) {
          handleNoteOff(data.trackChannels[e.track], e.note_in, e.note_in_velocity, 0);
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
    midi::Channel channel = data.trackChannels[playIterator->track];
    switch(playIterator->event) {
    case midi::NoteOn:
      // handle muted tracks
      handleNoteOn(channel, playIterator->note_in, data.trackMutes[playIterator->track] ? 0 : playIterator->note_in_velocity, 0);
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
      data.trackLayers[data.activeRecordingTrack]++;
    }

    // react to external bpm change
    if(currentBpm != seq.bpm) {
      onBpmChanged(seq.bpm);
    }

    updateTrackChannels(); // only to be called initially and when track instruments are changed
    
    if(eventsList.size() > 0) {
      // remove all invalidated notes
      eventsList.remove_if([](MidiEvent &e){ return e.event == midi::InvalidType; });
      //printEvents();
      playIterator = eventsList.begin();
      loadNextEvent(timeToMs(playIterator->patternNumber, playIterator->patternMs));
    }
  }
  Serial.printf("Sequence %i/%i @%ibpm : %ims with %i events\n", data.patternCount + 1, data.numberOfBars, currentBpm, data.patternLengthMs, eventsList.size());

}

void LiveSequencer::updateTrackChannels() {
  for(uint8_t i = 0; i < NUM_SEQ_TRACKS; i++) {
    midi::Channel channel = -1;
    switch(seq.track_type[i]) {
    case 0:
      channel = static_cast<midi::Channel>(drum_midi_channel);
      break;

    case 1:
      // dexed instance 0+1, 2 = epiano , 3+4 = MicroSynth, 5 = Braids
      switch(seq.instrument[i]) {
        case 0:
        case 1:
          channel = static_cast<midi::Channel>(configuration.dexed[seq.instrument[i]].midi_channel);
          break;

        case 2:
          channel = static_cast<midi::Channel>(configuration.epiano.midi_channel);
          break;

        case 3:
        case 4:
          channel = microsynth[seq.instrument[i] - 3].midi_channel;
          break;

        case 5:
          channel = braids_osc.midi_channel;
      }
      break;
    }
    if(channel != -1) {
      data.trackChannels[i] = channel;
      //Serial.printf("track %i has midi channel %i\n", i, channel);
    }
  }
}