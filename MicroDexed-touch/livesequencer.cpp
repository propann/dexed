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

LiveSequencer::LiveSequencer() {
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
  playIterator = events.end();
  patternCount = NUM_PATTERNS - 1;
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
  Serial.printf("[%i]: %i:%04i\t%i:%04i, (%i), %s, %i, %i\n", i, e.timeRecord.patternNumber, e.timeRecord.patternMs, e.timePlay.patternNumber, e.timePlay.patternMs, e.track, getName(e.event).c_str(), e.note_in, e.note_in_velocity);
}

void LiveSequencer::timeQuantization(EventTime &timeRec, EventTime &timePlay, uint16_t multiple) {
  const uint16_t halfStep = multiple / 2;
  // first round up an event just at end that was meant to be played at 1
  if((patternLengthMs - timeRec.patternMs) < halfStep) {
    timeRec.patternNumber++;
    if(timeRec.patternNumber == NUM_PATTERNS) {
      timeRec.patternNumber = 0;
    }
    timeRec.patternMs = 0;
  }
  // then do quantization
  timePlay.patternMs = ((timeRec.patternMs + halfStep) / multiple) * multiple;
  timePlay.patternNumber = timeRec.patternNumber;
  Serial.printf("round %i to %i\n", timeRec.patternMs, timePlay.patternMs);
}

void LiveSequencer::printEvents() {
  Serial.printf("--- %i events (%i bytes with one be %i bytes)\n", events.size(), events.size() * sizeof(MidiEvent), sizeof(MidiEvent));
  int i = 0;
  for(auto &e : events) {
    printEvent(i++, e);
  }
}

void LiveSequencer::handleMidiEvent(midi::MidiType event, uint8_t note, uint8_t velocity) {
  switch(note) {
  case 48: // clear track
    clearTrackEvents(activeRecordingTrack);
    Serial.printf("cleared track %i\n", activeRecordingTrack);
    return;
  
  case 49: // track down
    activeRecordingTrack = 6;
    Serial.printf("rec track now is %i\n", activeRecordingTrack);
    return;
  
  case 51: // track up
    activeRecordingTrack = 7;
    Serial.printf("rec track now is %i\n", activeRecordingTrack);
    return;

  default:
    break;
  }

  midi::Channel ch = trackChannels[activeRecordingTrack];
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

  if(seq.running) {
    const EventTime timeRecord = { patternCount, uint16_t(patternTimer) };
  
    MidiEvent newEvent = { timeRecord, timeRecord, activeRecordingTrack, event, note, velocity };
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
          // if so, insert NoteOn and this NoteOff at end
          timeQuantization(on->second.timeRecord, on->second.timePlay, quantisizeMs);
          pendingEvents.push_back(on->second);
          timeQuantization(newEvent.timeRecord, newEvent.timePlay, quantisizeMs);
          if(on->second.timePlay.patternMs == newEvent.timePlay.patternMs) {
            newEvent.timePlay.patternMs += quantisizeMs;
          }
          pendingEvents.push_back(newEvent);
          notesOn.erase(on);
      }
      break;
    }
    
    //printEvents();
    
  }
}

void LiveSequencer::clearTrackEvents(uint8_t track) {
  for(std::vector<MidiEvent>::iterator it = events.begin(); it != events.end();) {
    if(it->track == track) {
      if(it->event == midi::NoteOff) {
        handleNoteOff(trackChannels[it->track], it->note_in, it->note_in_velocity, 0);
      }
      events.erase(it);
      playIterator--; // FIXME: only if event is in the future
    } else {
      it++;
    }
  }
  events.shrink_to_fit();
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
  if(playIterator != events.end()) {
    const unsigned long now = ((patternCount * patternLengthMs) + patternTimer);
    //Serial.printf("PLAY: ");
    //printEvent(1, *playIterator);
    midi::Channel channel = trackChannels[playIterator->track];
    switch(playIterator->event) {
    case midi::NoteOn:
      handleNoteOn(channel, playIterator->note_in, playIterator->note_in_velocity, 0);
      break;
    
    case midi::NoteOff:
      handleNoteOff(channel, playIterator->note_in, playIterator->note_in_velocity, 0);
      break;
    
    default:
      break;
    }
    if(++playIterator != events.end()) {
      int timeToNextEvent = eventTimeToMs(playIterator->timePlay) - now;
      loadNextEvent(timeToNextEvent);
    }
  }
}

uint32_t LiveSequencer::eventTimeToMs(EventTime &t) {
  return (t.patternNumber * patternLengthMs) + t.patternMs;
}

//void LiveSequencer::init(int bpm, std::vector<MidiEvent> loadedEvents) {
void LiveSequencer::init(int bpm) {
  //events = loadedEvents;
  
  currentBpm = bpm;
  liveTimer.begin([this] { playNextEvent(); });
  pendingEvents.resize(50);
}

void LiveSequencer::handlePatternBegin(void) {
  // seq.tempo_ms = 60000000 / seq.bpm / 4; // rly?
  patternTimer = 0;

  if(++patternCount == NUM_PATTERNS) {
    patternCount = 0;

    if(currentBpm != seq.bpm) {
      float resampleFactor =  currentBpm / float(seq.bpm);
      for(auto &e : events) {
        e.timeRecord.patternMs *= resampleFactor;
        timeQuantization(e.timeRecord, e.timePlay, quantisizeMs);
      }
      currentBpm = seq.bpm;
    }

    patternLengthMs = (4 * 1000 * 60) / (seq.bpm); // for a 4/4 signature
    quantisizeMs = patternLengthMs / quantisizeDenom;
    
    updateTrackChannels(); // only to be called initially and when track instruments are changed
    
    if(pendingEvents.size()) {
      // insert pending to events and sort
      events.resize(events.size() + pendingEvents.size());
      events.insert(events.end(), pendingEvents.begin(), pendingEvents.end());
      pendingEvents.clear();
      std::sort(events.begin(), events.end(), LiveSequencer::sortMidiEvent);
    }

    //printEvents();
    
    if(events.size() > 0) {
      playIterator = events.begin();
      loadNextEvent(eventTimeToMs(playIterator->timePlay));
    }
  }
  Serial.printf("Sequence %i/%i @%ibpm : %ims\n", patternCount + 1, NUM_PATTERNS, currentBpm, patternLengthMs);

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
      //Serial.printf("track %i has midi channel %i\n", i, channel);
    }
  }
}