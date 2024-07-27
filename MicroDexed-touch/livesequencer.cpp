#include "livesequencer.h"
#include "config.h"
#include "sequencer.h"
#include <algorithm>
#include "TeensyTimerTool.h"
#include "ui_livesequencer.h"
#include <map>

extern sequencer_t seq;
extern uint8_t drum_midi_channel;
extern config_t configuration;
extern microsynth_t microsynth[NUM_MICROSYNTH];
extern multisample_s msp[NUM_MULTISAMPLES];
extern braids_t braids_osc;
extern uint8_t microsynth_selected_instance;
extern uint8_t selected_instance_id; // dexed

extern void handleNoteOnInput(byte, byte, byte, byte);
extern void handleNoteOn(byte, byte, byte, byte);
extern void handleNoteOff(byte, byte, byte, byte);
extern void handleAfterTouch(byte, byte);
extern void UI_func_drums(uint8_t param);
extern void UI_func_voice_select(uint8_t param);
extern void UI_func_microsynth(uint8_t param);
extern void UI_func_epiano(uint8_t param);
extern void UI_func_braids(uint8_t param);
extern void handleStart();
extern void handleStop();

using namespace TeensyTimerTool;

PeriodicTimer tickTimer(TMR1);  // only 16bit needed
OneShotTimer arpTimer(TCK);     // one tick timer of 20
OneShotTimer liveTimer(TCK);    // one tick timer of 20

UI_LiveSequencer* ui_liveSeq;

FLASHMEM LiveSequencer::LiveSequencer() {
  ui_liveSeq = new UI_LiveSequencer(*this, data);
  updateTrackChannels(true);
  data.isActive = false;
  playIterator = data.eventsList.end();
}

FLASHMEM void UI_func_livesequencer(uint8_t param) {
  ui_liveSeq->processLCDM();
}

FLASHMEM void handle_touchscreen_live_sequencer(void) {
  ui_liveSeq->handleTouchscreen();
}

FLASHMEM LiveSequencer::LiveSeqData* LiveSequencer::getData(void) {
  return &data;
}

FLASHMEM const std::string LiveSequencer::getEventName(midi::MidiType event) {
  switch (event) {
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

FLASHMEM const std::string LiveSequencer::getEventSource(EventSource source) {
  switch (source) {
  case EVENT_PATTERN:
    return "PATT";
  case EVENT_SONG:
    return "SONG";
  default:
    return "NONE";
  }
}

FLASHMEM void LiveSequencer::start(void) {
  handleStart();
}

FLASHMEM void LiveSequencer::stop(void) {
  handleStop();
}

FLASHMEM void LiveSequencer::onStopped(void) {
  data.isRunning = false;

  if (data.isSongMode) {
    onSongStopped();
  }

  playIterator = data.eventsList.end();
  allNotesOff();
  ui_liveSeq->onStopped();
  pressedArpKeys.clear();
  data.arpSettings.arpNotes.clear();
}

FLASHMEM void LiveSequencer::onStarted(void) {
  tickTimer.begin([] { TeensyTimerTool::tick(); }, 0.1ms);
  liveTimer.begin([this] { playNextEvent(); });
  arpTimer.begin([this] { playNextArpNote(); });
  data.startedFlag = true;
  data.isRunning = true;
  data.recordedToSong = false;
  data.eventsList.remove_if([](MidiEvent& e) { return e.source == EventSource::EVENT_SONG; });
}

FLASHMEM void LiveSequencer::allNotesOff(void) {
  for (int track = 0; track < LIVESEQUENCER_NUM_TRACKS; track++) {
    allTrackNotesOff(track);
  }
}

FLASHMEM void LiveSequencer::allTrackNotesOff(const uint8_t track) {
  for (uint8_t layer = 0; layer < data.trackSettings[track].layerCount; layer++) {
    allLayerNotesOff(track, layer);
  }
}

FLASHMEM void LiveSequencer::allLayerNotesOff(const uint8_t track, const uint8_t layer) {
  for (auto note : data.tracks[track].activeNotes[layer]) {
    handleNoteOff(data.tracks[track].channel, note, 0, data.trackSettings[track].device);
  }
  data.tracks[track].activeNotes[layer].clear();
}

FLASHMEM void LiveSequencer::printEvent(int i, MidiEvent e) {
  DBG_LOG(printf("[%i]: %i:%04i {%s} (%i):%i, %s, %i, %i\n", i, e.patternNumber, e.patternMs, getEventSource(e.source).c_str(), e.track, e.layer, getEventName(e.event).c_str(), e.note_in, e.note_in_velocity));
}

FLASHMEM bool LiveSequencer::timeQuantization(MidiEvent& e, uint8_t denom) {
  bool overflow = false; // overflow if event rounded to start of next pattern

  if (denom > 1) {
    const uint16_t quantizeMs = data.patternLengthMs / denom;
    const uint16_t halfStep = quantizeMs / 2;
    uint8_t resultNumber = e.patternNumber;
    uint16_t resultMs = e.patternMs;
    resultMs = ((e.patternMs + halfStep) / quantizeMs) * quantizeMs;
    if (resultMs == data.patternLengthMs) {
      resultMs = 0;
      if (++resultNumber == data.numberOfBars) {
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

FLASHMEM void LiveSequencer::printEvents() {
  DBG_LOG(printf("--- %i pattern events (%i bytes with one be %i bytes)\n", data.eventsList.size(), data.eventsList.size() * sizeof(MidiEvent), sizeof(MidiEvent)));
  uint i = 0;
  for (auto& e : data.eventsList) {
    printEvent(i++, e);
  }
  DBG_LOG(printf("--- song events on %i layers until pattern %i\n", data.songLayerCount, data.lastSongEventPattern));
  for (int i = 0; i <= data.lastSongEventPattern; i++) {
    for (auto& e : data.songEvents[i]) {
      printEvent(i++, e);
    }
  }
}

FLASHMEM void LiveSequencer::onArpSourceChanged(void) {
  // for now the best way to avoid pending notes / arp keys 
  if (data.isRunning && data.arpSettings.enabled) {
    stop();
    start();
  }
}

FLASHMEM void LiveSequencer::refreshSongLength(void) {
  uint8_t lastSongPattern = 0;
  for (auto& e : data.songEvents) {
    // remove all invalidated song events and repopulate song length
    e.second.remove_if([](MidiEvent& e) { return (e.event == midi::InvalidType); });
    if (e.second.size()) {
      if (e.first > lastSongPattern) {
        lastSongPattern = e.first;
      }
    }
  }
  data.lastSongEventPattern = lastSongPattern;
  DBG_LOG(printf("last song pattern: %i\n", lastSongPattern));
}

FLASHMEM void LiveSequencer::onSongStopped(void) {
  if (data.recordedToSong) {
    data.songLayerCount++;
    data.songLayersChanged = true;
    data.recordedToSong = false;
  }

  refreshSongLength();
  applySongStartLayerMutes();
}

FLASHMEM void LiveSequencer::applySongStartLayerMutes(void) {
  if (data.songLayerCount > 0) {
    for (int track = 0; track < LIVESEQUENCER_NUM_TRACKS; track++) {
      data.tracks[track].layerMutes = data.trackSettings[track].songStartLayerMutes;
    }
    data.trackLayersChanged = true;
  }
}

FLASHMEM void LiveSequencer::handleMidiEvent(uint8_t inChannel, midi::MidiType event, uint8_t note, uint8_t velocity) {
  if (data.isActive) {
    if (data.instrumentChannels.count(inChannel) == 0) {
      // velocity adjustment for keyboard events (live playing or recording)
      const uint8_t velocitySetting = data.trackSettings[data.activeTrack].velocityLevel;
      const uint8_t velocityActive = (velocitySetting == 0) ? velocity : velocitySetting * 1.27f; // 100% * 1.27 = 127

      if (data.isRecording && data.isRunning) {
        const EventSource source = data.isSongMode ? EVENT_SONG : EVENT_PATTERN;

        MidiEvent newEvent = { source, uint16_t(data.patternTimer), data.currentPattern, data.activeTrack, data.trackSettings[data.activeTrack].layerCount, event, note, velocityActive };
        if (data.isSongMode) {
          if (data.songLayerCount < LIVESEQUENCER_NUM_LAYERS) {
            // in song mode, simply add event, no rounding and checking needed
            newEvent.layer = data.songLayerCount;
            uint8_t patternCount = data.songPatternCount;
            if (newEvent.event == midi::NoteOn) {
              if (timeQuantization(newEvent, data.trackSettings[data.activeTrack].quantizeDenom)) {
                patternCount++; // event rounded up to start of next song pattern
              }
            }
            data.recordedToSong = true;
            data.songEvents[patternCount].emplace_back(newEvent);
          }
        }
        else {
          if (data.trackSettings[data.activeTrack].layerCount < LIVESEQUENCER_NUM_LAYERS) {
            switch (newEvent.event) {
            default:
              // ignore all other types
              break;

              // TODO: record aftertouch

            case midi::NoteOn:
              static constexpr int ROUND_UP_MS = 100;
              // round up events just at end probably meant to be played at start
              if (newEvent.patternNumber == (data.numberOfBars - 1) && newEvent.patternMs > (data.patternLengthMs - ROUND_UP_MS)) {
                newEvent.patternNumber = 0;
                newEvent.patternMs = 0;
              }
              data.notesOn.insert(std::pair<uint8_t, MidiEvent>(note, newEvent));
              break;

            case midi::NoteOff:
              // check if it has a corresponding NoteOn
              const auto on = data.notesOn.find(note);
              if (on != data.notesOn.end()) {
                timeQuantization(on->second, data.trackSettings[data.activeTrack].quantizeDenom);
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
      const bool arpSamplesKeyboard = data.isRunning && (data.arpSettings.enabled) && (data.arpSettings.source == 0);
      if (arpSamplesKeyboard) {
        switch (event) {
        case midi::NoteOn:
          pressedArpKeys.insert(note);
          data.arpSettings.keysChanged = true;
          break;

        case midi::NoteOff:
          pressedArpKeys.erase(note);
          data.arpSettings.keysChanged = true;
          break;

        default:
          break;
        }
      }
      else {
        const midi::Channel ch = data.tracks[data.activeTrack].channel;
        const uint8_t device = data.trackSettings[data.activeTrack].device;
        switch (event) {
        case midi::AfterTouchChannel:
          handleAfterTouch(ch, note);
          break;

        case midi::NoteOn:
          //DBG_LOG(printf("play on ch%i of device %i\n", ch, device));
          handleNoteOnInput(ch, note, velocityActive, device);

          if (data.lastPlayedNote != note) {
            data.lastPlayedNote = note;
            data.lastPlayedNoteChanged = true;
          }
          break;

        case midi::NoteOff:
          handleNoteOff(ch, note, velocityActive, device);
          break;

        default:
          break;
        }
      }
    }
    else {
      ui_liveSeq->showDirectMappingWarning(inChannel);
      DBG_LOG(printf("LiveSeq: drop event as directly assigned to an instrument\n"));
    }
  }
  else {
    DBG_LOG(printf("LiveSeq: drop event as not active\n"));
  }
}

FLASHMEM void LiveSequencer::fillTrackLayer(void) {
  if (data.trackSettings[data.activeTrack].layerCount < LIVESEQUENCER_NUM_LAYERS) {
    const float msIncrement = data.patternLengthMs / float(data.fillNotes.number);
    const uint8_t msOffset = round(data.fillNotes.offset * msIncrement / 8.0f);
    const uint16_t noteLength = round(msIncrement / 2.0f); // ...
    for (uint8_t bar = 0; bar < data.numberOfBars; bar++) {
      for (uint16_t note = 0; note < data.fillNotes.number; note++) {
        const uint16_t noteOnTime = round(note * msIncrement) + msOffset;
        const uint16_t noteOffTime = noteOnTime + noteLength;
        data.pendingEvents.emplace_back(MidiEvent{ EVENT_PATTERN, noteOnTime, bar, data.activeTrack, data.trackSettings[data.activeTrack].layerCount, midi::NoteOn, data.lastPlayedNote, 127 });
        data.pendingEvents.emplace_back(MidiEvent{ EVENT_PATTERN, noteOffTime, bar, data.activeTrack, data.trackSettings[data.activeTrack].layerCount, midi::NoteOff, data.lastPlayedNote, 0 });
      }
    }
    if (data.isRunning == false) {
      addPendingNotes();
    }
  }
}

FLASHMEM void LiveSequencer::changeNumberOfBars(uint8_t num) {
  if (num != data.numberOfBars) {
    data.numberOfBars = num;
    deleteLiveSequencerData();
  }
}

FLASHMEM void LiveSequencer::deleteLiveSequencerData(void) {
  data.pendingEvents.clear();
  data.eventsList.clear();
  deleteAllSongEvents();
  for (int track = 0; track < LIVESEQUENCER_NUM_TRACKS; track++) {
    data.trackSettings[track].layerCount = 0;
    data.tracks[track].layerMutes = 0;
  }
  data.trackLayersChanged = true;
  init();
}

FLASHMEM void LiveSequencer::deleteAllSongEvents(void) {
  data.songEvents.clear();
  data.lastSongEventPattern = 0;
  data.songLayerCount = 0;

  for (auto& e : data.eventsList) {
    if (e.source == EVENT_SONG) {
      e.event = midi::InvalidType; // mark as invalid
    }
  }
  for (uint8_t track = 0; track < LIVESEQUENCER_NUM_TRACKS; track++) {
    data.trackSettings[track].songStartLayerMutes = 0;
  }
  data.songLayersChanged = true;
}

FLASHMEM void LiveSequencer::songLayerAction(uint8_t layer, LayerMode action) {
  if ((layer == 0) && (action == LayerMode::LAYER_MERGE)) {
    return; // avoid merge up top layer
  }
  for (auto& e : data.songEvents) {
    for (auto& a : e.second) {
      performLayerAction(action, a, layer);
    }
  }
  refreshSongLength();
  data.songLayerCount--;
  data.songLayersChanged = true;
}

FLASHMEM void LiveSequencer::trackLayerAction(uint8_t track, uint8_t layer, LayerMode action) {
  if ((layer == 0) && (action == LayerMode::LAYER_MERGE)) {
    return; // avoid merge up top layer
  }

  // play noteOff for active layer notes
  allLayerNotesOff(track, layer);

  for (auto& e : data.eventsList) {
    if (e.track == track) {
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

FLASHMEM void LiveSequencer::performLayerAction(LayerMode action, MidiEvent& e, uint8_t layer) {
  if (e.layer == layer) {
    switch (action) {
    case LayerMode::LAYER_MERGE:
      // layer 0 must not be shifted up
      if (e.layer > 0) {
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

//extern int compensate_seq_delay;

FLASHMEM void LiveSequencer::loadNextEvent(int timeMs) {
  if (timeMs > 0) {
    //LOG.printf("trigger in %ims\n", timeMs);
    liveTimer.trigger(timeMs * 1000);
    //liveTimer.trigger(timeMs * 1000 + compensate_seq_delay);
  }
  else {
    playNextEvent();
  }
}

FLASHMEM void LiveSequencer::playNextEvent(void) {
  if (playIterator != data.eventsList.end()) {
    const bool isMuted = (playIterator->source == EventSource::EVENT_PATTERN) && (data.tracks[playIterator->track].layerMutes & (1 << playIterator->layer));
    const midi::Channel channel = data.tracks[playIterator->track].channel;
    const uint8_t device = data.trackSettings[playIterator->track].device;

    switch (playIterator->event) {
    case midi::ControlChange:
      switch (playIterator->note_in_velocity) {
      case AutomationType::TYPE_MUTE_ON:
      case AutomationType::TYPE_MUTE_OFF:
        //DBG_LOG(printf("mute %s\n", playIterator->note_in_velocity ? "MUTE" : "UNMUTE"));
        setLayerMuted(playIterator->track, playIterator->note_in, playIterator->note_in_velocity == AutomationType::TYPE_MUTE_ON);
        data.trackLayersChanged = true;
        break;
      }
      break;
    case midi::NoteOff:
      if (isMuted == false) {
        // erase one instance of this note going off
        if (data.arpSettings.enabled && (data.arpSettings.source - 1) == playIterator->track) {
          pressedArpKeys.erase(playIterator->note_in);
          data.arpSettings.keysChanged = true;
        }
        else {
          const auto it = data.tracks[playIterator->track].activeNotes[playIterator->layer].find(playIterator->note_in);
          if (it != data.tracks[playIterator->track].activeNotes[playIterator->layer].end()) {
            data.tracks[playIterator->track].activeNotes[playIterator->layer].erase(it);
          }
          handleNoteOff(channel, playIterator->note_in, playIterator->note_in_velocity, device);
        }
      }
      break;

    case midi::NoteOn:
      if (isMuted == false) {
        if (data.arpSettings.enabled && (data.arpSettings.source - 1) == playIterator->track) {
          pressedArpKeys.insert(playIterator->note_in);
          data.arpSettings.keysChanged = true;
        }
        else {
          // add active note to layer track set
          data.tracks[playIterator->track].activeNotes[playIterator->layer].insert(playIterator->note_in);
          // velocity adjustment for playing from performance
          const uint8_t velocitySetting = data.trackSettings[playIterator->track].velocityLevel;
          const uint8_t velocityActive = (velocitySetting == 0) ? playIterator->note_in_velocity : velocitySetting * 1.27f; // 100% * 1.27 = 127
          handleNoteOn(channel, playIterator->note_in, velocityActive, device);
        }
      }
      break;

    default:
      break;
    }
    if (playIterator->source == EVENT_SONG) {
      playIterator->event = midi::InvalidType; // mark song events to delete after execution
    }
    if (++playIterator != data.eventsList.end()) {
      const unsigned long now = ((data.currentPattern * data.patternLengthMs) + data.patternTimer);
      int timeToNextEvent = timeToMs(playIterator->patternNumber, playIterator->patternMs) - now;
      loadNextEvent(timeToNextEvent);
    }
  }
}

inline uint32_t LiveSequencer::timeToMs(uint8_t patternNumber, uint16_t patternMs) const {
  return (patternNumber * data.patternLengthMs) + patternMs;
}

FLASHMEM void LiveSequencer::checkLoadNewArpNotes(void) {
  bool reloadArpNotes = false;

  if (data.arpSettings.latch) {
    reloadArpNotes = data.arpSettings.keysChanged && pressedArpKeys.size();
  }
  else {
    reloadArpNotes = data.arpSettings.keysChanged; // TODO: maybe immediadely stop playing with no latch?
  }
  data.arpSettings.keysChanged = false;

  if (reloadArpNotes) {
    data.arpSettings.arpNotesIn.assign(pressedArpKeys.begin(), pressedArpKeys.end());
  }

  if (reloadArpNotes || data.arpSettings.arpSettingsChanged) {
    data.arpSettings.arpSettingsChanged = false;
    data.arpSettings.arpNotes.assign(data.arpSettings.arpNotesIn.begin(), data.arpSettings.arpNotesIn.end());
    const uint8_t numNotes = data.arpSettings.arpNotes.size();

    for (uint octave = 1; octave < data.arpSettings.octaves; octave++) {
      for (uint number = 0; number < numNotes; number++) {
        data.arpSettings.arpNotes.emplace_back(data.arpSettings.arpNotes[number] + (octave * 12));
      }
    }

    switch (data.arpSettings.mode) {
    case ArpMode::ARP_RANDOM:
      std::random_shuffle(data.arpSettings.arpNotes.begin(), data.arpSettings.arpNotes.end());
      break;
    case ArpMode::ARP_DOWN:
    case ArpMode::ARP_DOWNUP:
    case ArpMode::ARP_DOWNUP_P:
      std::sort(data.arpSettings.arpNotes.begin(), data.arpSettings.arpNotes.end(), std::greater<>());
      break;
    case ArpMode::ARP_UP:
    case ArpMode::ARP_UPDOWN:
    case ArpMode::ARP_UPDOWN_P:
      std::sort(data.arpSettings.arpNotes.begin(), data.arpSettings.arpNotes.end(), std::less<>());
      break;
    default:
      break;
    }

    data.arpSettings.arpIt = data.arpSettings.arpNotes.begin();
    data.arpSettings.notePlayCount = 0;
  }
}

FLASHMEM void LiveSequencer::setArpEnabled(bool enabled) {
  data.arpSettings.enabled = enabled;
  if (enabled && data.arpSettings.source != 0) {
    // finish active pattern notes on arp enable
    allTrackNotesOff(data.arpSettings.source - 1);
  }
}

FLASHMEM bool sortedArpNote(const LiveSequencer::ArpNote& n1, const LiveSequencer::ArpNote& n2) {
  return (n1.offDelay < n2.offDelay);
}

FLASHMEM void LiveSequencer::playNextArpNote(void) {
  const uint16_t nowMs = uint16_t(data.patternTimer);

  if (data.arpSettings.delayToNextArpOnMs == 0) {
    const uint8_t arpAmount = data.arpSettings.amount;
    const uint8_t loadPerBar = data.arpSettings.loadPerBar;
    const float arpIntervalMs = data.patternLengthMs / float(arpAmount);
    uint8_t arpIndex = (nowMs + (arpIntervalMs / 2)) / arpIntervalMs;
    if (((arpIndex * loadPerBar) % arpAmount) == 0) { // check if reload pressed keys
      checkLoadNewArpNotes();
    }

    if (data.arpSettings.arpNotes.empty()) {
      data.arpSettings.delayToNextArpOnMs = data.patternLengthMs / loadPerBar; // bypass loading timer until next pattern start
    }
    else {
      ArpNote newArp; // play a new note...
      newArp.track = (data.arpSettings.source == 0) ? data.activeTrack : (data.arpSettings.source - 1);

      if (data.arpSettings.mode != ArpMode::ARP_CHORD) {
        newArp.notes.emplace_back(*data.arpSettings.arpIt);

        if (data.arpSettings.arpNotes.size() > 1) {
          if (++data.arpSettings.notePlayCount > data.arpSettings.noteRepeat) {
            data.arpSettings.notePlayCount = 0;
            if (++data.arpSettings.arpIt == data.arpSettings.arpNotes.end()) {
              data.arpSettings.arpIt = data.arpSettings.arpNotes.begin();
              bool doubleEndNote = false;
              switch (data.arpSettings.mode) {
              case ArpMode::ARP_DOWNUP_P:
              case ArpMode::ARP_UPDOWN_P:
                doubleEndNote = true;
              case ArpMode::ARP_DOWNUP:
              case ArpMode::ARP_UPDOWN:
                std::reverse(data.arpSettings.arpNotes.begin(), data.arpSettings.arpNotes.end());
                if (doubleEndNote == false) {
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
      }
      else {
        for (uint8_t note : data.arpSettings.arpNotes) {
          newArp.notes.emplace_back(note);
        }
      }
      if (data.arpSettings.enabled) {
        playArp(midi::NoteOn, newArp);
        newArp.offDelay = (arpIntervalMs * data.arpSettings.length) / 100;
        activeArps.emplace_back(newArp);
      }

      // calc time to next noteOn with incremented
      uint16_t nextArpEventOnTimeMs = uint16_t(++arpIndex * arpIntervalMs);
      if (arpIndex & 0x01) {
        // swing: odd beats NoteOn is variable
        nextArpEventOnTimeMs += round(data.arpSettings.swing * arpIntervalMs / 20.0); // swing from -8 to +8;
      }
      data.arpSettings.delayToNextArpOnMs = (nextArpEventOnTimeMs - nowMs);
    }
  }
  else {
    // finish and erase elapsed note
    for (auto it = activeArps.begin(); it != activeArps.end();) {
      if (it->offDelay == 0) {
        playArp(midi::NoteOff, *it);
        activeArps.erase(it);
      }
      else {
        ++it;
      }
    }
  }
  uint16_t delayToNextTimerCall = data.arpSettings.delayToNextArpOnMs;

  if (activeArps.size() && (activeArps.front().offDelay < data.arpSettings.delayToNextArpOnMs)) {
    // next call will be a finishing note
    delayToNextTimerCall = activeArps.front().offDelay;
  }
  const uint16_t delayToNextPatternStart = uint16_t(data.patternLengthMs - nowMs);
  const bool nextIsPatternStart = (delayToNextTimerCall >= delayToNextPatternStart);

  if (nextIsPatternStart) {
    delayToNextTimerCall = delayToNextPatternStart;
  }

  for (auto& n : activeArps) {
    n.offDelay -= std::min(delayToNextTimerCall, n.offDelay);
  }
  data.arpSettings.delayToNextArpOnMs -= std::min(delayToNextTimerCall, data.arpSettings.delayToNextArpOnMs);
  if (nextIsPatternStart == false) {
    //DBG_LOG(printf("@%i:\ttrigger again in %ims\n", nowMs, delayToNextTimerCall));
    // delay all arp events by 1ms to make sure when in track source mode, quantized notes are already on when sampling
    arpTimer.trigger((delayToNextTimerCall + 1) * 1000);
  }
}

FLASHMEM void LiveSequencer::playArp(const midi::MidiType type, const ArpNote arp) {
  const midi::Channel channel = data.tracks[arp.track].channel;
  const midi::Channel device = data.trackSettings[arp.track].device;

  for (auto& n : arp.notes) {
    switch (type) {
    case midi::NoteOn:
      handleNoteOn(channel, n, data.arpSettings.velocity, device);
      break;

    case midi::NoteOff:
      handleNoteOff(channel, n, 0, device);
      break;

    default:
      break;
    }
    // FIXME: fast arp recording crashes...
    /*const uint8_t layer = data.trackSettings[data.activeTrack].layerCount;
    if(data.isRecording && layer < LIVESEQUENCER_NUM_LAYERS) {
      const EventSource source = data.isSongMode ? EVENT_SONG : EVENT_PATTERN;
      const MidiEvent newEvent = { source, uint16_t(data.patternTimer), data.currentPattern, data.activeTrack, layer, type, n, data.arpSettings.velocity };
      data.pendingEvents.emplace_back(newEvent);
    }*/
  }
}

FLASHMEM void LiveSequencer::initOnce(void) {
  ui_liveSeq->init();
}

FLASHMEM void LiveSequencer::init(void) {
  data.patternLengthMs = (4 * 1000 * 60) / seq.bpm; // for a 4/4 signature
  checkBpmChanged();
  updateTrackChannels();
  DBG_LOG(printf("init has %i events\n", data.eventsList.size()));
  //printEvents();
  data.pendingEvents.reserve(200);
  refreshSongLength();
}

FLASHMEM void LiveSequencer::onGuiInit(void) {
  init();
  checkAddMetronome();
}

FLASHMEM void LiveSequencer::checkBpmChanged(void) {
  if (seq.bpm != data.currentBpm) {
    data.patternLengthMs = (4 * 1000 * 60) / seq.bpm; // for a 4/4 signature
    DBG_LOG(printf("bpm changed from %i to %i\n", data.currentBpm, seq.bpm));
    float resampleFactor = data.currentBpm / float(seq.bpm);
    data.currentBpm = seq.bpm;
    // resample pattern events - not lossless
    for (auto& e : data.eventsList) {
      e.patternMs = round(resampleFactor * e.patternMs);
    }
    // resample song events - not lossless
    for (auto& e : data.songEvents) {
      for (auto& a : e.second) {
        a.patternMs = round(resampleFactor * a.patternMs);
      }
    }
  }
}

FLASHMEM void LiveSequencer::addPendingNotes(void) {
  // finish active notes at end of all bars
  for (auto it = data.notesOn.begin(); it != data.notesOn.end();) {
    if (timeToMs(it->second.patternNumber, it->second.patternMs) != 0) {
      data.pendingEvents.emplace_back(it->second);
      it->second.event = midi::NoteOff;
      it->second.note_in_velocity = 0;
      it->second.patternNumber = data.numberOfBars - 1;
      it->second.patternMs = data.patternLengthMs - 1;
      data.pendingEvents.emplace_back(it->second);
      data.notesOn.erase(it++);
    }
    else {
      ++it; // ignore notesOn at 0.0000, those were round up just before
    }
  }
  // then add all notes to events and sort
  if (data.pendingEvents.size()) {
    for (auto& e : data.pendingEvents) {
      data.eventsList.emplace_back(e);
    }
    data.pendingEvents.clear();
    data.eventsList.sort(sortMidiEvent);
    setLayerMuted(data.activeTrack, data.trackSettings[data.activeTrack].layerCount, false); // new layer is unmuted
    data.trackSettings[data.activeTrack].layerCount++;
    data.trackLayersChanged = true;
  }
}

FLASHMEM void LiveSequencer::handlePatternBegin(void) {
  data.patternTimer = 0;

  if (data.startedFlag) {
    data.startedFlag = false;
    // just started, do not increment
    data.currentPattern = 0;
    data.songPatternCount = 0;

    activeArps.clear();
    data.arpSettings.notePlayCount = 0;
    data.arpSettings.delayToNextArpOnMs = 0;

    if (data.isSongMode && data.isRecording) {
      // save current song start layer mutes
      for (uint8_t track = 0; track < LIVESEQUENCER_NUM_TRACKS; track++) {
        data.trackSettings[track].songStartLayerMutes = data.tracks[track].layerMutes;
      }
    }
  }
  else {
    if ((data.currentPattern + 1) == data.numberOfBars) {
      data.currentPattern = 0;
      if (data.isSongMode) {
        if ((data.isRecording == false) && (data.songPatternCount == data.lastSongEventPattern)) {
          DBG_LOG(printf("song ended. restart from beginning...\n"));
          data.songPatternCount = 0;
        }
        else {
          data.songPatternCount++;
        }
      }
    }
    else {
      data.currentPattern++;
    }
  }

  if (data.currentPattern == 0) {
    if (data.isSongMode == false) {
      // first insert pending EVENT_PATT events to events and sort
      addPendingNotes();
    }

    if (data.eventsList.size() > 0) {
      // remove all invalidated notes
      data.eventsList.remove_if([](MidiEvent& e) { return e.event == midi::InvalidType; });

      // finish possibly not played events at end
      while (playIterator != data.eventsList.end()) {
        DBG_LOG(printf("about to finish event: "));
        printEvent(0, *playIterator);
        playNextEvent();
      }

      // for song mode, add song events for this pattern
      if (data.isSongMode) {
        for (auto& e : data.songEvents[data.songPatternCount]) {
          data.eventsList.emplace_back(e);
        }
        data.eventsList.sort(sortMidiEvent);
        if (data.songPatternCount == 0) {
          // load previously saved song start layer mutes
          applySongStartLayerMutes();
        }
      }
      //printEvents();
      playIterator = data.eventsList.begin();
      loadNextEvent(timeToMs(playIterator->patternNumber, playIterator->patternMs));
    }
  }
  // restart arp on pattern start
  if (data.arpSettings.enabled) {
    if (data.arpSettings.arpNotes.size() && data.arpSettings.freerun == false) {
      data.arpSettings.arpSettingsChanged = true; // force reload
    }
    playNextArpNote();
  }

  DBG_LOG(printf("Sequence %i/%i @%ibpm : %ims with %i events\n", data.currentPattern + 1, data.numberOfBars, data.currentBpm, data.patternLengthMs, data.eventsList.size()));
}

FLASHMEM void selectDexed0() {
  selected_instance_id = 0;
}

FLASHMEM void selectDexed1() {
  selected_instance_id = 1;
}

FLASHMEM void selectMs0() {
  microsynth_selected_instance = 0;
}

FLASHMEM void selectMs1() {
  microsynth_selected_instance = 1;
}

FLASHMEM void LiveSequencer::setLayerMuted(uint8_t track, uint8_t layer, bool isMuted, bool recordToSong) {
  if (isMuted) {
    data.tracks[track].layerMutes |= (1 << layer);
    allLayerNotesOff(track, layer);
  }
  else {
    data.tracks[track].layerMutes &= ~(1 << layer);
  }
  if (recordToSong) {
    if (data.songLayerCount < LIVESEQUENCER_NUM_LAYERS) {
      data.recordedToSong = true;
      const AutomationType type = isMuted ? AutomationType::TYPE_MUTE_ON : AutomationType::TYPE_MUTE_OFF;
      MidiEvent e = { EVENT_SONG, uint16_t(data.patternTimer), data.currentPattern, track, data.songLayerCount, midi::MidiType::ControlChange, layer, type };
      uint8_t patternCount = data.songPatternCount;
      if (timeQuantization(e, data.songMuteQuantizeDenom)) {
        patternCount++; // event rounded up to start of next song pattern
      }
      data.songEvents[patternCount].emplace_back(e);
      DBG_LOG(printf("record muted %i at %i of song pattern count %i\n", isMuted, timeToMs(data.currentPattern, data.patternTimer), data.songPatternCount));
    }
  }
}

FLASHMEM void LiveSequencer::checkAddMetronome(void) {
  // always assure we have a drum track with some tempo to begin
  if (data.eventsList.empty()) {
    const uint8_t activeTrack = data.activeTrack;

    uint8_t fillNumOld = data.fillNotes.number;
    uint8_t fillOffOld = data.fillNotes.offset;

    for (uint8_t i = 0; i < LiveSequencer::LIVESEQUENCER_NUM_TRACKS; i++) {
      if (data.tracks[i].screen == UI_func_drums) {
        data.activeTrack = i;
        data.fillNotes.number = 8;
        data.fillNotes.offset = 0;
        data.lastPlayedNote = 78; // hats
        fillTrackLayer();
        data.fillNotes.number = 1;
        data.fillNotes.offset = 0;
        data.lastPlayedNote = 48; // kick
        fillTrackLayer();
        if (data.isRunning == false) {
          // only merge if not added to pending in fillTrackLayer above
          trackLayerAction(i, 1, LayerMode::LAYER_MERGE);
        }
        // reset fillNotes to user values
        data.fillNotes.number = fillNumOld;
        data.fillNotes.offset = fillOffOld;
        data.activeTrack = activeTrack;
        return;
      }
    }
  }
}

FLASHMEM void LiveSequencer::changeTrackInstrument(uint8_t track, uint8_t newDevice, uint8_t newInstrument) {
  DBG_LOG(printf("change track %i to device %i and instrument %i\n", track, newDevice, newInstrument));
  allTrackNotesOff(track);
  data.trackSettings[track].device = newDevice;
  data.trackSettings[track].instrument = newInstrument;
  updateTrackChannels();
}

FLASHMEM void LiveSequencer::loadOldTrackInstruments(void) {
  for (uint8_t i = 0; i < LIVESEQUENCER_NUM_TRACKS; i++) {
    data.trackSettings[i].device = DEVICE_INTERNAL;
    
    switch (seq.track_type[i]) {
    case 0:
      data.trackSettings[i].instrument = INSTR_DRUM;
      break;

    case 1:
      // dexed instance 0+1, 2 = epiano , 3+4 = MicroSynth, 5 = Braids
      switch (seq.instrument[i]) {
      case 0:
        data.trackSettings[i].instrument = INSTR_DX1;
        break;

      case 1:
        data.trackSettings[i].instrument = INSTR_DX2;
        break;

      case 2:
        data.trackSettings[i].instrument = INSTR_EP;
        break;

      case 3:
        data.trackSettings[i].instrument = INSTR_MS1;
        break;

      case 4:
        data.trackSettings[i].instrument = INSTR_MS2;
        break;

      case 5:
        data.trackSettings[i].instrument = INSTR_BRD;
        break;

      default:
        // new mappings not backwards compatible
        break;
      }
      break;
    }
  }
}

FLASHMEM void LiveSequencer::updateTrackChannels(bool initial) {
  updateInstrumentChannels();
  char temp_char[10];

  for (uint8_t i = 0; i < LIVESEQUENCER_NUM_TRACKS; i++) {
    const uint8_t device = data.trackSettings[i].device;
    const uint8_t instrument = data.trackSettings[i].instrument;

    data.tracks[i].screen = nullptr;
    data.tracks[i].screenSetupFn = nullptr;

    getInstrumentName(device, instrument, temp_char, data.tracks[i].name);

    if (initial) {
      data.trackSettings[i].quantizeDenom = 1; // default: no quantization
    }
    switch (device) {
    case DEVICE_INTERNAL:
      switch (instrument) {
      case INSTR_DRUM:
        data.tracks[i].channel = static_cast<midi::Channel>(drum_midi_channel);
        data.tracks[i].screen = UI_func_drums;
        if (initial) {
          data.trackSettings[i].quantizeDenom = 16; // default: drum quantize to 1/16
        }
        break;

      case INSTR_DX1:
      case INSTR_DX2:
        data.tracks[i].channel = static_cast<midi::Channel>(configuration.dexed[instrument - 1].midi_channel);
        data.tracks[i].screen = UI_func_voice_select;
        data.tracks[i].screenSetupFn = (instrument == 1) ? (SetupFn)selectDexed0 : (SetupFn)selectDexed1;
        break;

      case INSTR_EP:
        data.tracks[i].channel = static_cast<midi::Channel>(configuration.epiano.midi_channel);
        data.tracks[i].screen = UI_func_epiano;
        break;

      case INSTR_MS1:
      case INSTR_MS2:
        data.tracks[i].channel = microsynth[instrument - 4].midi_channel;
        data.tracks[i].screen = UI_func_microsynth;
        data.tracks[i].screenSetupFn = (instrument == 4) ? (SetupFn)selectMs0 : (SetupFn)selectMs1;
        break;

      case INSTR_BRD:
        data.tracks[i].channel = braids_osc.midi_channel;
        data.tracks[i].screen = UI_func_braids;
        break;

      default:
        data.tracks[i].channel = 99; // probably unused
        break;
      }
      break;

    case DEVICE_MIDI_USB:
    case DEVICE_MIDI_DIN:
    case DEVICE_MIDI_INT:
      data.tracks[i].channel = (instrument + 1); // 1 - 16
      break;
    }
  }
}

FLASHMEM void LiveSequencer::getDeviceName(uint8_t device, char *name, char *sub) const {
  switch (device) {
  case DEVICE_INTERNAL:
    sprintf(name, "DEVICE");
    sprintf(sub, "MDT");
    break;

  case DEVICE_MIDI_USB:
    sprintf(name, "MIDI");
    sprintf(sub, "USB");
    break;

  case DEVICE_MIDI_DIN:
    sprintf(name, "MIDI");
    sprintf(sub, "DIN");
    break;

  case DEVICE_MIDI_INT:
    sprintf(name, "MIDI");
    sprintf(sub, "INT");
    break;

  default:
    sprintf(name, "NONE");
    sprintf(sub, " ");
    break;
  }
}

FLASHMEM void LiveSequencer::getInstrumentName(uint8_t device, uint8_t instrument, char *name, char *sub) const {
  switch (device) {
  case DEVICE_INTERNAL:
    sprintf(name, "INSTR");

    switch (instrument) {
    case INSTR_DRUM:
      sprintf(sub, "DRM");
      break;

    case INSTR_DX1:
    case INSTR_DX2:
      sprintf(sub, "DX%i", instrument);
      break;

    case INSTR_EP:
      sprintf(sub, "EP");
      break;

    case INSTR_MS1:
    case INSTR_MS2:
      sprintf(sub, "MS%i", instrument - 3);
      break;

    case INSTR_BRD:
      sprintf(sub, "BRD");
      break;

    default:
      sprintf(sub, "-");
      break;
    }
    break;
  
  case DEVICE_MIDI_USB:
    sprintf(name, "CHANNEL");
    sprintf(sub, "USB %02i", (instrument + 1));
    break;

  case DEVICE_MIDI_DIN:
    sprintf(name, "CHANNEL");
    sprintf(sub, "DIN %02i", (instrument + 1));
    break;

  case DEVICE_MIDI_INT:
    sprintf(name, "CHANNEL");
    sprintf(sub, "INT %02i", (instrument + 1));
    break;

  default:
    sprintf(name, "NONE");
    sprintf(sub, " ");
    break;
  }
}

FLASHMEM void LiveSequencer::updateInstrumentChannels(void) {
  data.instrumentChannels.clear();
  data.instrumentChannels.insert(drum_midi_channel);
  for (int i = 0; i < NUM_DEXED; i++) {
    data.instrumentChannels.insert(configuration.dexed[i].midi_channel);
  }
  data.instrumentChannels.insert(configuration.epiano.midi_channel);
  for (int i = 0; i < NUM_MICROSYNTH; i++) {
    data.instrumentChannels.insert(microsynth[i].midi_channel);
  }
  for (int i = 0; i < NUM_MULTISAMPLES; i++) {
    data.instrumentChannels.insert(msp[i].midi_channel);
  }
  data.instrumentChannels.insert(braids_osc.midi_channel);
}

FLASHMEM void LiveSequencer::cleanEvents(void) {
  if (data.eventsList.size() > 0) {
    // remove all invalidated notes
    data.eventsList.remove_if([](MidiEvent& e) { return e.event == midi::InvalidType; });
    data.eventsList.sort(sortMidiEvent);
  }
}

FLASHMEM void LiveSequencer::addNotePair(MidiEvent noteOn, MidiEvent noteOff) {
  DBG_LOG(printf("adding note pair:\n"));
  printEvent(1, noteOn);
  printEvent(2, noteOff);
  data.pendingEvents.emplace_back(noteOn);
  data.pendingEvents.emplace_back(noteOff);
  if (data.isRunning == false) {
    addPendingNotes();
  }
}

FLASHMEM std::vector<LiveSequencer::NotePair> LiveSequencer::getNotePairsFromTrack(uint8_t track) {
  std::vector<LiveSequencer::NotePair> result;

  for (std::list<MidiEvent>::iterator it = data.eventsList.begin(); it != data.eventsList.end(); it++) {
    if (it->track == track && it->event == midi::NoteOn) {
      for (std::list<MidiEvent>::iterator itOff = it; itOff != data.eventsList.end(); itOff++) {
        const bool sameNote = itOff->note_in == it->note_in;
        const bool sameTrack = itOff->track == it->track;
        const bool sameLayer = itOff->layer == it->layer;
        const bool isNoteOff = itOff->event == midi::NoteOff;
        if (sameTrack && sameLayer && sameNote && isNoteOff) {
          NotePair p = {
            .noteOn = *it,
            .noteOff = *itOff
          };
          result.emplace_back(p);
          break;
        }
      }
    }
  }
  return result;
}

FLASHMEM void LiveSequencer::printNotePairs(std::vector<std::vector<NotePair>> notePairs) {
  int pattern = 0;
  for (std::vector<LiveSequencer::NotePair> patternPairs : notePairs) {
    DBG_LOG(printf("\n***********************\nPAIRS OF PATTERN #%i\n***********************", pattern));
    int pair = 0;

    for (LiveSequencer::NotePair p : patternPairs) {
      DBG_LOG(printf("\nON-OFF Pair:\n"));
      LiveSequencer::printEvent(pair, p.noteOn);
      LiveSequencer::printEvent(pair, p.noteOff);
      pair++;
    }
    pattern++;
  }
}
