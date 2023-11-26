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

void LiveSequencer::printEvent(int i, MidiEvent e) {
  Serial.printf("[%i]: %0.3f, (%i), %s, %i, %i\n", i, e.time, e.track, getName(e.event).c_str(), e.note_in, e.note_in_velocity);
}

void LiveSequencer::printEvents() {
  Serial.printf("--- %i events:\n", events.size());
  int i = 0;
  for(auto &e : events) {
    printEvent(i++, e);
  }
}

void LiveSequencer::addEvent(midi::MidiType event, uint8_t note, uint8_t velocity) {
  if(seq.running) {
    float time = patternTimer / float(patternLenghtMs);

    static constexpr uint8_t track = 7;

    bool clearAll = false;
    clearAll |= note == 49;
    if(events.size()) {
      clearAll |= (events.back().time > time);
    }
  
    if(clearAll) {
      liveTimer.stop();
      events.clear();
      events.shrink_to_fit();
      Serial.printf("clear map\n");
      if(event == midi::NoteOff) {
        return;
      }
    }
    if(events.size() == 0 && event == midi::NoteOff) {
      return;
    }
    
    MidiEvent e = { time, track, event, note, velocity };
    events.push_back(e);
    
    printEvents();
    
  }
}

void LiveSequencer::loadNextEvent(unsigned long timeMs) {
  if(timeMs > 0) {
    liveTimer.trigger(timeMs * 1000);
  } else {
    playNextEvent();
  }
}

void LiveSequencer::playNextEvent(void) {
  if(events.size() > playIndex) {
    unsigned long now = patternTimer;
    Serial.printf("PLAY: ");
    MidiEvent e = events.at(playIndex);
    printEvent(playIndex, e);
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
    if(events.size() > ++playIndex) {
      unsigned long timeToNextEvent = max(events.at(playIndex).time * patternLenghtMs - now, 0UL);
      loadNextEvent(timeToNextEvent);
    } 
  }
}

void LiveSequencer::handlePatternBegin(void) {
  // seq.tempo_ms = 60000000 / seq.bpm / 4; // rly?
  patternLenghtMs = (4 * 1000 * 60) / (seq.bpm); // for a 4/4 signature
  Serial.printf("seq len ms: %i\n", patternLenghtMs);
  updateTrackChannels(); // only to be called initially and when track instruments are changed
  printEvents();
  patternTimer = 0;
  Serial.printf("total events size: %i bytes with one be %i bytes\n", events.size() * sizeof(MidiEvent), sizeof(MidiEvent));
  if(events.size() > 0) {
    playIndex = 0;
    liveTimer.begin(timerCallback);
    loadNextEvent(events.at(0).time * patternLenghtMs);
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