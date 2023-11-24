#include "livesequencer.h"
#include "config.h"
#include "sequencer.h"

extern void handleNoteOn(byte, byte, byte, byte);
extern void handleNoteOff(byte, byte, byte, byte);

extern "C" {
  extern sequencer_t seq;
}

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

void LiveSequencer::printEvent(unsigned int i) {
  MidiEvent e = midiEvents[i];
  Serial.printf("[%i]: %i, %s, %i, %i\n", i, e.time, getName(e.event).c_str(), e.note_in, e.note_in_velocity);
}

void LiveSequencer::printEvents() {
  Serial.printf("--- %i events:\n", eventsSize);
  for(unsigned int i = 0; i < eventsSize; i++) {
    printEvent(i);
  }
}

void LiveSequencer::addEvent(midi::MidiType event, uint8_t note, uint8_t velocity) {
  if(seq.running) {
    unsigned long now = patternTimer;
    MidiEvent e = { now, event, note, velocity };

    bool clearAll = false;
    clearAll |= note == 49;
    if(eventsSize > 0) {
      clearAll |= now < midiEvents[eventsSize - 1].time;
    }
    if(clearAll) {
      memset(midiEvents, 0, EVENTS_SIZE * sizeof(MidiEvent));
      Serial.printf("clear map\n");
      eventsSize = 0;
      liveTimer.stop();
      if(event == midi::NoteOff) {
        return;
      }
    }
    if(eventsSize == 0 && event == midi::NoteOff) {
      return;
    }
    if(eventsSize < EVENTS_SIZE) {
      midiEvents[eventsSize] = e;
      eventsSize++;
    } else {
      Serial.printf("events buffer full! dropping...\n");
    }
    printEvents();
    
  } else {
    //Serial.printf("ignoring event since not running...\n");
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
  if(eventsSize > playIndex) {
    Serial.printf("PLAY: ");
    printEvent(playIndex);
    switch(midiEvents[playIndex].event) {
    case midi::NoteOn:
      handleNoteOn(16, midiEvents[playIndex].note_in, midiEvents[playIndex].note_in_velocity, 0);
      break;
    
    case midi::NoteOff:
      handleNoteOff(16, midiEvents[playIndex].note_in, midiEvents[playIndex].note_in_velocity, 0);
      break;
    
    default:
      break;
    }
    playIndex++;
    unsigned long timeToNextEvent = midiEvents[playIndex].time - midiEvents[playIndex - 1].time;
    loadNextEvent(timeToNextEvent);
  }
}

void LiveSequencer::handlePatternBegin(void) {
  printEvents();
  patternTimer = 0;
  Serial.printf("total events size: %i bytes with one be %i bytes\n", EVENTS_SIZE * sizeof(MidiEvent), sizeof(MidiEvent));
  if(eventsSize > 0) {
    playIndex = 0;
    liveTimer.begin(timerCallback);
    loadNextEvent(midiEvents[0].time);
  }
}