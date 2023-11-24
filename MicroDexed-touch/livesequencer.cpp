#include "livesequencer.h"
#include "config.h"
#include "sequencer.h"

extern void handleNoteOn(byte, byte, byte, byte);
extern void handleNoteOff(byte, byte, byte, byte);
extern sequencer_t seq;

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
  Serial.printf("[%i]: %i, %s, %i, %i\n", i, e.time, getName(e.event).c_str(), e.note_in, e.note_in_velocity);
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
    unsigned long now = patternTimer;
    static constexpr midi::Channel channel = 16;
    

    for (auto it = events.begin(); it != events.end(); ++it) {
      if(it->time < now) {
        //events.erase(it);
      }
    }

    bool clearAll = false;
    clearAll |= note == 49;
   
    if(clearAll) {
      events.clear();
      events.shrink_to_fit();
      Serial.printf("clear map\n");
      liveTimer.stop();
      if(event == midi::NoteOff) {
        return;
      }
    }
    if(events.size() == 0 && event == midi::NoteOff) {
      return;
    }
    MidiEvent e = { now, channel, event, note, velocity };
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
    switch(e.event) {
    case midi::NoteOn:
      handleNoteOn(e.channel, e.note_in, e.note_in_velocity, 0);
      break;
    
    case midi::NoteOff:
      handleNoteOff(e.channel, e.note_in, e.note_in_velocity, 0);
      break;
    
    default:
      break;
    }
    if(events.size() > ++playIndex) {
      unsigned long timeToNextEvent = max(events.at(playIndex).time - now, 0);
      loadNextEvent(timeToNextEvent);
    } 
  }
}

void LiveSequencer::handlePatternBegin(void) {
  printEvents();
  patternTimer = 0;
  Serial.printf("total events size: %i bytes with one be %i bytes\n", events.size() * sizeof(MidiEvent), sizeof(MidiEvent));
  if(events.size() > 0) {
    playIndex = 0;
    liveTimer.begin(timerCallback);
    loadNextEvent(events.at(0).time);
  }
}