#include "livesequencer.h"
#include "config.h"
#include "sequencer.h"

extern void handleNoteOn(byte, byte, byte, byte);
extern void handleNoteOff(byte, byte, byte, byte);
extern sequencer_t seq;
extern uint8_t drum_midi_channel;
extern config_t configuration;
extern microsynth_t microsynth[2];
extern braids_t braids_osc;

LiveSequencer *instance;

LiveSequencer::LiveSequencer() {
  instance = this;
}

void LiveSequencer::timerCallback() {
  instance->playNextEvent();
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
  playIndex = events.size();
  patternCount = 0;
  pendingEvents.clear();
  pendingEvents.shrink_to_fit();
  allNotesOff();
}

void LiveSequencer::allNotesOff(void) {
  for(auto &e : events) {
    if(e.event == midi::NoteOff) {
      handleNoteOff(trackChannels[e.track], e.note_in, e.note_in_velocity, 0);
    }
  }
}

void LiveSequencer::printEvent(int i, MidiEvent e) {
  Serial.printf("[%i]: %i‰, (%i), %s, %i, %i\n", i, int(e.time * 1000), e.track, getName(e.event).c_str(), e.note_in, e.note_in_velocity);
}

void LiveSequencer::printEvents() {
  Serial.printf("--- %i events (%i bytes with one be %i bytes)\n", events.size(), events.size() * sizeof(MidiEvent), sizeof(MidiEvent));
  int i = 0;
  for(auto &e : events) {
    printEvent(i++, e);
  }
  Serial.printf("--- %i pending events:\n", pendingEvents.size());
  i = 0;
  for(auto &e : pendingEvents) {
    printEvent(i++, e);
  }
}

void LiveSequencer::handleMidiEvent(midi::MidiType event, uint8_t note, uint8_t velocity) {
  if(seq.running) {
    switch(note) {
    case 48: // clear track
      clearTrackEvents(activeRecordingTrack);
      return;
    
    case 49: // track down
      activeRecordingTrack = 6;
      return;
    
    case 51: // track up
      activeRecordingTrack = 7;
      return;

    default:
      break;
    }

    float time = patternTimer / float(patternLengthMs);

    if((patternLengthMs - patternTimer) < 100 && event == midi::NoteOn) {
      Serial.printf("rounding up...\n");
      time = 0;
    }
    
    if(events.size()) {
      if(lastTrackEvent[activeRecordingTrack] > time) {
        clearTrackEvents(activeRecordingTrack);
      }
    }
  
    MidiEvent newEvent = { time, activeRecordingTrack, event, note, velocity };
    switch(newEvent.event) {
    default:
      // ignore all other types
      break;

    case midi::NoteOn:
      pendingEvents.push_back(newEvent);
      break;

    case midi::NoteOff:
      // check if it has a corresponding NoteOn
      for(std::vector<MidiEvent>::iterator it = pendingEvents.begin(); it != pendingEvents.end();) {
        if(it->note_in == newEvent.note_in) {
          // if so, insert sorted NoteOn and add this NoteOff at end
          insertSorted(*it);
          insertSorted(newEvent);
          playIndex += 2; // fixme--
          lastTrackEvent[activeRecordingTrack] = time;
          pendingEvents.erase(it);
          break;
        } else {
          it++;
        }
      }
      break;
    }
    
    printEvents();
    
  }
}

void LiveSequencer::clearTrackEvents(uint8_t track) {
  Serial.printf("clear events of track %i\n", track);
  for(std::vector<MidiEvent>::iterator it = events.begin(); it != events.end();) {
    if(it->track == track) {
      if(it->event == midi::NoteOff) {
        handleNoteOff(trackChannels[it->track], it->note_in, it->note_in_velocity, 0);
      }
      events.erase(it);
    } else {
      it++;
    }
  }
  lastTrackEvent[track] = 0;
}

void LiveSequencer::insertSorted(MidiEvent e) {
  uint insertIndex = events.size();
  for (uint i = 0; i < insertIndex; i++) {
    if(e.time < events.at(i).time) {
      insertIndex = i;
      break;
    }
  }
  events.insert(events.begin() + insertIndex, e);
}

void LiveSequencer::loadNextEvent(unsigned long timeMs) {
  if(timeMs > 0) {
    liveTimer.trigger(timeMs * 1000);
  } else {
    playNextEvent();
  }
}

void LiveSequencer::playNextEvent(void) {
  uint eventsSize = events.size();
  if(eventsSize > playIndex) {
    unsigned long now = patternTimer;
    //Serial.printf("PLAY: ");
    MidiEvent &e = events[playIndex];
    //printEvent(playIndex, e);
    midi::Channel channel = trackChannels[e.track];
    switch(e.event) {
    case midi::NoteOn:
      handleNoteOn(channel, e.note_in, e.note_in_velocity, 0);
      break;
    
    case midi::NoteOff:
      handleNoteOff(channel, e.note_in, e.note_in_velocity, 0);
      break;
    
    default:
      break;
    }
    if(eventsSize > ++playIndex) {
      unsigned long timeToNextEvent = max(events[playIndex].time * patternLengthMs - now, 0UL);
      loadNextEvent(timeToNextEvent);
    } 
  }
}

void LiveSequencer::handlePatternBegin(void) {
  // seq.tempo_ms = 60000000 / seq.bpm / 4; // rly?
  static constexpr int NUM_PATTERNS = 1; // needs GUI config
  Serial.printf("Sequence %i/%i\n", patternCount + 1, NUM_PATTERNS);
  if(patternCount == 0) {
    patternLengthMs = NUM_PATTERNS * (4 * 1000 * 60) / (seq.bpm); // for a 4/4 signature
    Serial.printf("seq len ms: %i\n", patternLengthMs);
    updateTrackChannels(); // only to be called initially and when track instruments are changed
    printEvents();
    patternTimer = 0;
    
    if(events.size() > 0) {
      playIndex = 0;
      liveTimer.begin(timerCallback);
      loadNextEvent(events[0].time * patternLengthMs);
    }
  }
  if(++patternCount == NUM_PATTERNS) {
    patternCount = 0;
  }
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
      trackChannels[i] = channel;
      Serial.printf("track %i has midi channel %i\n", i, channel);
    }
  }
}