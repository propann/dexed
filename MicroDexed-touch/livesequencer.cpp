#include "livesequencer.h"
#include "config.h"
#include "sequencer.h"
#include <algorithm>
#include "TeensyTimerTool.h"
#include <map>

extern void handleNoteOnInput(byte, byte, byte, byte);
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

std::set<uint8_t> pressedArpKeys;
std::list<LiveSequencer::ArpNote> activeArps;

using namespace TeensyTimerTool;
PeriodicTimer tickTimer(TMR1);  // only 16bit needed
OneShotTimer arpTimer(TCK);     // one tick timer of 20
OneShotTimer liveTimer(TCK);    // one tick timer of 20

LiveSequencer::LiveSequencer() :
  ui(this) {
  updateTrackChannels(true);
  data.isActive = false;
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
  data.isRunning = false;

  if(data.isSongMode) {
    onSongStopped();
  }
  
  playIterator = data.eventsList.end();
  allNotesOff();
  data.songPatternCount = data.lastSongEventPattern; // show song length
  data.currentPattern = data.numberOfBars; // show num bars
  data.stoppedFlag = true;
  data.arpSettings.arpNotes.clear();
}

void LiveSequencer::handleStart(void) {
  tickTimer.begin([] { TeensyTimerTool::tick(); }, 1ms);
  liveTimer.begin([this] { playNextEvent(); });
  arpTimer.begin([this] { playNextArpNote(); });
  data.startedFlag = true;
  data.isRunning = true;
  data.recordedToSong = false;
  data.eventsList.remove_if([](MidiEvent &e){ return e.source == EventSource::EVENT_SONG; });
}

void LiveSequencer::allNotesOff(void) {
  for(int track = 0; track < LIVESEQUENCER_NUM_TRACKS; track++) {
    for(uint8_t layer = 0; layer < data.trackSettings[track].layerCount; layer++) {
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

bool LiveSequencer::timeQuantization(MidiEvent &e, uint8_t denom) {
  bool overflow = false; // overflow if event rounded to start of next pattern
  
  if(denom > 1) {
    const uint16_t quantisizeMs = data.patternLengthMs / denom;
    const uint16_t halfStep = quantisizeMs / 2;
    uint8_t resultNumber = e.patternNumber;
    uint16_t resultMs = e.patternMs;
    resultMs = ((e.patternMs + halfStep) / quantisizeMs) * quantisizeMs;
    if(resultMs == data.patternLengthMs) {
      resultMs = 0;
      if(++resultNumber == data.numberOfBars) {
        resultNumber = 0;
        overflow = true;
      }
    }
    DBG_LOG(printf("round %i.%i to %i.%i\n", e.patternNumber, e.patternMs, resultNumber, resultMs));
    e.patternNumber = resultNumber;
    e.patternMs = resultMs;
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

void LiveSequencer::refreshSongLength(void) {
  uint8_t lastSongPattern = 0;
  for(auto &e : data.songEvents) {
    // remove all invalidated song events and repopulate song length
    e.second.remove_if([](MidiEvent &e) { return (e.event == midi::InvalidType); });
    if(e.second.size()) {
      if(e.first > lastSongPattern) {
        lastSongPattern = e.first;
      }
    }
  }
  data.lastSongEventPattern = lastSongPattern;
  DBG_LOG(printf("last song pattern: %i\n", lastSongPattern));
}

void LiveSequencer::onSongStopped(void) {
  if(data.recordedToSong) {
    data.songLayerCount++;
    data.songLayersChanged = true;
    data.recordedToSong = false;
  }
  
  refreshSongLength();
  applySongStartLayerMutes();
}

void LiveSequencer::applySongStartLayerMutes(void) {
  if(data.songLayerCount > 0) {
    for(int track = 0; track < LIVESEQUENCER_NUM_TRACKS; track++) {
      data.tracks[track].layerMutes = data.trackSettings[track].songStartLayerMutes;
    }
    data.trackLayersChanged = true;
  }
}

void LiveSequencer::handleMidiEvent(uint8_t inChannel, midi::MidiType event, uint8_t note, uint8_t velocity) {
  if(data.isActive) {
    if(data.instrumentChannels.count(inChannel) == 0) {
      if(data.isRecording && data.isRunning) {
        const EventSource source = data.isSongMode ? EVENT_SONG : EVENT_PATTERN;
        MidiEvent newEvent = { source, uint16_t(data.patternTimer), data.currentPattern, data.activeTrack, data.trackSettings[data.activeTrack].layerCount, event, note, velocity };
        if(data.isSongMode) {
          if(data.songLayerCount < LIVESEQUENCER_NUM_LAYERS) {
            // in song mode, simply add event, no rounding and checking needed
            newEvent.layer = data.songLayerCount; 
            uint8_t patternCount = data.songPatternCount;
            if(newEvent.event == midi::NoteOn) {
              if(timeQuantization(newEvent, data.trackSettings[data.activeTrack].quantisizeDenom)) {
                patternCount++; // event rounded up to start of next song pattern
              }
            }
            data.recordedToSong = true;
            data.songEvents[patternCount].emplace_back(newEvent);
          }
        } else {
          if(data.trackSettings[data.activeTrack].layerCount < LIVESEQUENCER_NUM_LAYERS) {    
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
                  timeQuantization(on->second, data.trackSettings[data.activeTrack].quantisizeDenom);
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

      // forward incoming midi event to correct channel
      // ignore events directly mapped to an instrument
      const bool arpActive = data.isRunning && (data.arpSettings.amount > 0);
      if(arpActive) {
        switch(event) {
        case midi::NoteOn:
          pressedArpKeys.insert(note);
          break;

          case midi::NoteOff:
          pressedArpKeys.erase(note);
        
        default:
          break;
        }
      } else {
        const midi::Channel ch = data.tracks[data.activeTrack].channel;
        switch(event) {
        case midi::NoteOn:
          handleNoteOnInput(ch, note, velocity, 0);
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
    } else {
      ui.showDirectMappingWarning(inChannel);
      DBG_LOG(printf("LiveSeq: drop event as directly assigned to an instument\n"));
    }
  } else {
    DBG_LOG(printf("LiveSeq: drop event as not active\n"));
  }
}

void LiveSequencer::fillTrackLayer(void) {
  if(data.trackSettings[data.activeTrack].layerCount < LIVESEQUENCER_NUM_LAYERS) {
    const float msIncrement = data.patternLengthMs / float(data.fillNotes.number);
    const uint8_t msOffset = round(data.fillNotes.offset * msIncrement / 8.0f);
    const uint16_t noteLength = round(msIncrement / 2.0f); // ...
    for(uint8_t bar = 0; bar < data.numberOfBars; bar++) {
      for(uint16_t note = 0; note < data.fillNotes.number; note++) {
        const uint16_t noteOnTime = round(note * msIncrement) + msOffset;
        const uint16_t noteOffTime = noteOnTime + noteLength;
        data.pendingEvents.emplace_back(MidiEvent { EVENT_PATTERN, noteOnTime, bar, data.activeTrack, data.trackSettings[data.activeTrack].layerCount, midi::NoteOn, data.lastPlayedNote, 127 } );
        data.pendingEvents.emplace_back(MidiEvent { EVENT_PATTERN, noteOffTime, bar, data.activeTrack, data.trackSettings[data.activeTrack].layerCount, midi::NoteOff, data.lastPlayedNote, 0 } );
      }
    }
    addPendingNotes();
  }
}

void LiveSequencer::changeNumberOfBars(uint8_t num) {
  if(num != data.numberOfBars) {
    data.numberOfBars = num;
    deleteLiveSequencerData();
  }
}

void LiveSequencer::deleteLiveSequencerData(void) {
  data.pendingEvents.clear();
  data.eventsList.clear();
  deleteAllSongEvents();
  for(int track = 0; track < LIVESEQUENCER_NUM_TRACKS; track++) {
    data.trackSettings[track].layerCount = 0;
    data.tracks[track].layerMutes = 0;
  }
  data.trackLayersChanged = true;
  init();
}

void LiveSequencer::deleteAllSongEvents(void) {
  data.songEvents.clear();
  data.lastSongEventPattern = 0;
  data.songLayerCount = 0;

  for(auto &e : data.eventsList) {
    if(e.source == EVENT_SONG) {
      e.event = midi::InvalidType; // mark as invalid
    }
  }
  for(uint8_t track = 0; track < LIVESEQUENCER_NUM_TRACKS; track++) {
    data.trackSettings[track].songStartLayerMutes = 0;
  }
  data.songLayersChanged = true;
}

void LiveSequencer::songLayerAction(uint8_t layer, LayerMode action) {
  if((layer == 0) && (action == LayerMode::LAYER_MERGE)) {
    return; // avoid merge up top layer
  }
  for(auto &e : data.songEvents) {
    for(auto &a : e.second) {
      performLayerAction(action, a, layer);
    }
  }
  refreshSongLength();
  data.songLayerCount--;
  data.songLayersChanged = true;
}

void LiveSequencer::trackLayerAction(uint8_t track, uint8_t layer, LayerMode action) {
  if((layer == 0) && (action == LayerMode::LAYER_MERGE)) {
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

  data.trackSettings[track].layerCount--;
  data.trackLayersChanged = true;
}

void LiveSequencer::performLayerAction(LayerMode action, LiveSequencer::MidiEvent &e, uint8_t layer) {
  if(e.layer == layer) {
    switch(action) {
    case LayerMode::LAYER_MERGE:
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
      const unsigned long now = ((data.currentPattern * data.patternLengthMs) + data.patternTimer);
      int timeToNextEvent = timeToMs(playIterator->patternNumber, playIterator->patternMs) - now;
      loadNextEvent(timeToNextEvent);
    }
  }
}

inline uint32_t LiveSequencer::timeToMs(uint8_t patternNumber, uint16_t patternMs) const {
  return (patternNumber * data.patternLengthMs) + patternMs;
}

void LiveSequencer::checkLoadNewArpNotes(void) {
  if(data.arpSettings.latch == 0) {
    data.arpSettings.arpNotes.clear();
  }

  if(pressedArpKeys.size()) {
    data.arpSettings.arpNotes.assign(pressedArpKeys.begin(), pressedArpKeys.end());;
    switch(data.arpSettings.mode) {
    case ArpMode::ARP_DOWN:
    case ArpMode::ARP_DOWNUP:
    case ArpMode::ARP_DOWNUP_P:
      std::sort(data.arpSettings.arpNotes.begin(), data.arpSettings.arpNotes.end(), std::less<>());
      break;
    case ArpMode::ARP_UP:
    case ArpMode::ARP_UPDOWN:
    case ArpMode::ARP_UPDOWN_P:
      std::sort(data.arpSettings.arpNotes.begin(), data.arpSettings.arpNotes.end(), std::greater<>());
      break;
    default:
      break;
    }
    if(data.arpSettings.arpNotes.size()) {
      data.arpSettings.arpIt = data.arpSettings.arpNotes.begin();
    }
  }
}

void LiveSequencer::printActiveArps(void) {
  for(ArpNote &n : activeArps) {
    DBG_LOG(printf("%i\n", n.offDelay));
  }
}

void LiveSequencer::playNextArpNote(void) {
  // finish and erase elapsed notes
  for(auto it = activeArps.begin(); it != activeArps.end();) {
    if((it->offDelay == 0)) {
      const midi::Channel channel = data.tracks[it->track].channel;
      for(auto &p : it->notes) {
        handleNoteOff(channel, p, 0, 0);
      }
      //DBG_LOG(printf("off and remove time %i\n", it->offDelay));
      it = activeArps.erase(it);
    } else {
      ++it;
    }
  }
  const uint8_t arpAmount = data.arpSettings.amount;
  const uint16_t nowMs = uint16_t(data.patternTimer);
  if(data.arpSettings.arpNotes.size() && arpAmount > 0) { 
    if(data.arpSettings.startNewNote) {
      // start next note
      ArpNote newArp;
      newArp.track = data.activeTrack;

      if(data.arpSettings.mode != ArpMode::ARP_CHORD) {
        const uint8_t currentNote = *data.arpSettings.arpIt;
        newArp.notes.emplace_back(currentNote);

        if(data.arpSettings.mode != ArpMode::ARP_CHORD) {
          if(data.arpSettings.arpNotes.size() > 1) {
            if(++data.arpSettings.arpIt == data.arpSettings.arpNotes.end()) {
              data.arpSettings.arpIt = data.arpSettings.arpNotes.begin();
              bool doubleEndNote = false;
              switch(data.arpSettings.mode) {
              case ArpMode::ARP_DOWNUP_P:
              case ArpMode::ARP_UPDOWN_P:
                doubleEndNote = true;
              case ArpMode::ARP_DOWNUP:
              case ArpMode::ARP_UPDOWN:
                std::reverse(data.arpSettings.arpNotes.begin(), data.arpSettings.arpNotes.end());
                if(doubleEndNote == false) {
                  data.arpSettings.arpIt++;
                }
                break;
              case ArpMode::ARP_RANDOM:
                std::random_shuffle(data.arpSettings.arpNotes.begin(), data.arpSettings.arpNotes.end());
                break;
              default:
                break;
              }
            }
          }
        }
      } else {
        for(uint8_t note : data.arpSettings.arpNotes) {
          newArp.notes.emplace_back(note);
        }
      }
      const midi::Channel channel = data.tracks[data.activeTrack].channel;
      for(auto &n : newArp.notes) {
        handleNoteOn(channel, n, 127, 0);
      }
      
      if(data.arpSettings.currentAmount != arpAmount) {
        // adapt arpCount to changed amount
        data.arpSettings.currentAmount = arpAmount;
        data.arpSettings.arpCount = nowMs / round(data.patternLengthMs / float(arpAmount));
      }
      data.arpSettings.arpCount++;
      const float arpIntervalMs = data.patternLengthMs / float(arpAmount);
      newArp.offDelay = (arpIntervalMs * data.arpSettings.length) / 100;
      activeArps.push_back(newArp); // assume newArp.offDelay is biggest - not a big problem if not

      // calc time to next noteOn
      uint16_t nextArpEventOnTimeMs = uint16_t(data.arpSettings.arpCount * arpIntervalMs);
      if(data.arpSettings.arpCount & 0x01) {
        // swing: odd beats NoteOn is variable
        nextArpEventOnTimeMs += round(data.arpSettings.swing * arpIntervalMs / 15.0); // swing from -10 to +10;
      }
      data.arpSettings.delayToNextArpOnMs = (nextArpEventOnTimeMs - nowMs);    
    }
  } else {
    data.arpSettings.delayToNextArpOnMs = 0xFFFF;
  }
  
  data.arpSettings.startNewNote = true;
  uint16_t delayToNextTimerCall = data.arpSettings.delayToNextArpOnMs;

  if(activeArps.size() && (activeArps.front().offDelay < data.arpSettings.delayToNextArpOnMs)) {
    // next call will be a finishing note
    delayToNextTimerCall = activeArps.front().offDelay;
    data.arpSettings.startNewNote = false;
  }
  const uint16_t delayToNextPatternStart = uint16_t(data.patternLengthMs - nowMs);
  const bool nextIsPatternStart = (delayToNextTimerCall > delayToNextPatternStart);

  if(nextIsPatternStart) {
    delayToNextTimerCall = delayToNextPatternStart;
  }

  for(auto &n : activeArps) {
    n.offDelay -= std::min(delayToNextTimerCall, n.offDelay);
  }
  data.arpSettings.delayToNextArpOnMs -= std::min(delayToNextTimerCall, data.arpSettings.delayToNextArpOnMs);

  //DBG_LOG(printf("@%i:\tnext arp event in %ims\n", delayToNextArpEventMs));
  if((activeArps.size() || data.arpSettings.startNewNote == true) && nextIsPatternStart == false) {
    DBG_LOG(printf("@%i:\ttrigger again in %ims\n", nowMs, delayToNextTimerCall));
    arpTimer.trigger(delayToNextTimerCall * 1000);
  }
}

void LiveSequencer::init(void) {
  data.patternLengthMs = (4 * 1000 * 60) / seq.bpm; // for a 4/4 signature
  checkBpmChanged();
  updateTrackChannels();
  DBG_LOG(printf("init has %i events\n", data.eventsList.size()));
  //printEvents();
  data.pendingEvents.reserve(50);
  refreshSongLength();
}

void LiveSequencer::onGuiInit(void) {
  init();
  checkAddMetronome();
}

void LiveSequencer::checkBpmChanged(void) {
  if(seq.bpm != data.currentBpm) {
    float resampleFactor =  data.currentBpm / float(seq.bpm);
    data.currentBpm = seq.bpm;
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
    data.trackSettings[data.activeTrack].layerCount++;
    data.trackLayersChanged = true;
  }
}

void LiveSequencer::handlePatternBegin(void) {
  data.patternTimer = 0;

  printActiveArps();

  if(data.startedFlag) {
    data.startedFlag = false;
    // just started, do not increment
    data.currentPattern = 0;
    data.songPatternCount = 0;

    activeArps.clear();
    data.arpSettings.startNewNote = true;

    if(data.isSongMode && data.isRecording) {
      // save current song start layer mutes
      for(uint8_t track = 0; track < LIVESEQUENCER_NUM_TRACKS; track++) {
        data.trackSettings[track].songStartLayerMutes = data.tracks[track].layerMutes;
      }
    }   
  } else {
    if((data.currentPattern + 1) == data.numberOfBars) {
      data.currentPattern = 0;
      if(data.isSongMode) {
        if((data.isRecording == false) && (data.songPatternCount == data.lastSongEventPattern)) {
          DBG_LOG(printf("song ended. restart from beginning...\n"));
          data.songPatternCount = 0;
        } else {
          data.songPatternCount++;
        }
      }
    } else {
      data.currentPattern++;
    }
  }

  checkLoadNewArpNotes();

  if(data.currentPattern == 0) {
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
        if(data.songPatternCount == 0) {
          // load previously saved song start layer mutes
          applySongStartLayerMutes();
        }
      }
#ifdef DEBUG
      //printEvents();
#endif
      playIterator = data.eventsList.begin();
      loadNextEvent(timeToMs(playIterator->patternNumber, playIterator->patternMs));
    }
  }
  // restart arp on pattern start
  data.arpSettings.arpCount = 0;
  playNextArpNote();

  DBG_LOG(printf("Sequence %i/%i @%ibpm : %ims with %i events\n", data.currentPattern + 1, data.numberOfBars, data.currentBpm, data.patternLengthMs, data.eventsList.size()));
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
      data.recordedToSong = true;
      const AutomationType type = isMuted ? AutomationType::TYPE_MUTE_ON : AutomationType::TYPE_MUTE_OFF;
      MidiEvent e = { EVENT_SONG, uint16_t(data.patternTimer), data.currentPattern, track, data.songLayerCount, midi::MidiType::ControlChange, layer, type };
      uint8_t patternCount = data.songPatternCount;
      if(timeQuantization(e, data.songMuteQuantisizeDenom)) {
        patternCount++; // event rounded up to start of next song pattern
      }
      data.songEvents[patternCount].emplace_back(e);
      DBG_LOG(printf("record muted %i at %i of song pattern count %i\n", isMuted, timeToMs(data.currentPattern, data.patternTimer), data.songPatternCount));
    }
  }
}

void LiveSequencer::checkAddMetronome(void) {
  // always assure we have a drum track with some tempo to begin
  if(data.eventsList.empty()) {
    const uint8_t activeTrack = data.activeTrack;
    for(uint8_t i = 0; i < LIVESEQUENCER_NUM_TRACKS; i++) {
      if(data.tracks[i].screen == UI_func_drums) {
        data.activeTrack = i;
        data.fillNotes.number = 8;
        data.fillNotes.offset = 0;
        data.lastPlayedNote = 78; // hats
        fillTrackLayer();
        data.fillNotes.number = 1;
        data.fillNotes.offset = 0;
        data.lastPlayedNote = 48; // kick
        fillTrackLayer();
        trackLayerAction(i, 1, LayerMode::LAYER_MERGE); // merge them
        // reset fillNotes to user values
        data.fillNotes.number = 4;
        data.fillNotes.offset = 0;
        data.activeTrack = activeTrack;
        return;
      }
    }
  }
}

void LiveSequencer::updateTrackChannels(bool initial) {
  data.instrumentChannels.clear();
  for(uint8_t i = 0; i < LIVESEQUENCER_NUM_TRACKS; i++) {
    data.tracks[i].screenSetupFn = nullptr;
    if(initial) {
      data.trackSettings[i].quantisizeDenom = 1; // default: no quantization
    }
    switch(seq.track_type[i]) {
    case 0:
      data.tracks[i].channel = static_cast<midi::Channel>(drum_midi_channel);
      data.tracks[i].screen = UI_func_drums;
      if(initial) {
        data.trackSettings[i].quantisizeDenom = 16; // default: drum quantisize to 1/16
      }
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
    data.instrumentChannels.insert(data.tracks[i].channel);
  }
}
