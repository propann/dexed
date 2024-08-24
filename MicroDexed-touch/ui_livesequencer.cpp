#include "ui_livesequencer.h"
#include "LCDMenuLib2.h"
#include "ILI9341_t3n.h"
#include "sequencer.h"
#include "livesequencer.h"
#include "touch.h"

extern LCDMenuLib2 LCDML;
extern ILI9341_t3n display;
extern bool remote_active;
extern int numTouchPoints;

extern void UI_func_load_performance(uint8_t param);
extern void UI_func_save_performance(uint8_t param);
extern void UI_func_midi_channels(uint8_t param);
extern void UI_func_seq_settings(uint8_t param);
extern void UI_func_liveseq_pianoroll(uint8_t param);
extern void UI_func_file_manager(uint8_t param);
extern void UI_func_master_effects(uint8_t param);
extern void UI_func_multiband_dynamics(uint8_t param);

extern void setCursor_textGrid(uint8_t pos_x, uint8_t pos_y);
extern void setCursor_textGrid_small(uint8_t pos_x, uint8_t pos_y);
extern void helptext_l(const char* str);

#define SCREEN_TRACK_INDEX(t) (t % LiveSequencer::LIVESEQUENCER_TRACKS_PER_SCREEN)

bool runningInBackground; // LiveSequencer stays active in instrument settings opened from here

FLASHMEM UI_LiveSequencer::UI_LiveSequencer(LiveSequencer& sequencer, LiveSequencer::LiveSeqData &d) : instance(this), liveSeq(sequencer), data(d) {
  const uint8_t BUTTON_SPACING = 4;  // center in screen

  for (uint8_t track = 0; track < LiveSequencer::LIVESEQUENCER_TRACKS_PER_SCREEN; track++) {
    const uint8_t OFFSET_Y = (track > 1) ? 10 : 0; // separate areas a bit
    GRID_X[track] = track * (TouchButton::BUTTON_SIZE_X + BUTTON_SPACING);
    GRID_Y[track] = track * (TouchButton::BUTTON_SIZE_Y + BUTTON_SPACING) + OFFSET_Y;
  }
  runningInBackground = false;
}

FLASHMEM void UI_LiveSequencer::checkApplyTrackInstrument(void) {
  const LiveSequencer::TrackSettings &trackSettings = data.trackSettings[data.activeTrack];
  const bool hasChanged = (trackSettings.device != selectedTrackSetup.device) || (trackSettings.instrument != selectedTrackSetup.instrument);
  if (hasChanged) {
    liveSeq.changeTrackInstrument(data.activeTrack, selectedTrackSetup.device, selectedTrackSetup.instrument);
    data.guiUpdateFlags |= drawTrackButtons;
  }
}

FLASHMEM void UI_LiveSequencer::drawTrackSubtext(uint8_t track, uint8_t trackOffset, bool isActiveTrack) {
  display.fillRect(GRID_X[track], GRID_Y[1] + TouchButton::BUTTON_SIZE_Y + 3, TouchButton::BUTTON_SIZE_X, CHAR_height_small, COLOR_BACKGROUND);
        display.setTextSize(1);
        display.setTextColor(isActiveTrack ? GREY1 : GREY2);
        display.setCursor(GRID_X[track], GRID_Y[1] + TouchButton::BUTTON_SIZE_Y + 3);

        const uint8_t denom = data.trackSettings[(trackOffset + track)].quantizeDenom;
        
        if (denom == 1) {
          display.print("Q-");
        }
        else {
          display.printf("Q%i", denom);
        }

        const uint8_t velocity = data.trackSettings[(trackOffset + track)].velocityLevel;
        const uint8_t digits = (velocity == 0) ? 3 : floor(log10(velocity)) + 2;
        display.setCursor(GRID_X[track] + TouchButton::BUTTON_SIZE_X - CHAR_width_small * digits, GRID_Y[1] + TouchButton::BUTTON_SIZE_Y + 3);
        
        if (velocity == 0) {
          display.print("KEY");
        }
        else {
          display.printf("%i%%", velocity);
        }
}

FLASHMEM void UI_LiveSequencer::init(void) {
    // TRACK BUTTONS
  for (uint8_t track = 0; track < LiveSequencer::LIVESEQUENCER_TRACKS_PER_SCREEN; track++) {
    trackButtons.push_back(new TouchButton(GRID_X[track], GRID_Y[1],
      [ this, track ] (auto *b) { // drawHandler
        const bool isActiveTrack = ((trackOffset + track) == data.activeTrack);
        const TouchButton::Color color = isActiveTrack ? (data.isRecording ? TouchButton::BUTTON_RED : TouchButton::BUTTON_HIGHLIGHTED) : TouchButton::BUTTON_ACTIVE;
        char temp_char[4];
        b->draw(data.tracks[(trackOffset + track)].name, itoa((trackOffset + track) + 1, temp_char, 10), color);
        instance->drawTrackSubtext(track, trackOffset, isActiveTrack);
      },
      [ this, track ] (auto *b) { // clickedHandler
        instance->onTrackButtonPressed(trackOffset + track);
      },
      [ this, track ] (auto *b) { // longPressHandler
        openScreen(UI_func_liveseq_pianoroll, (trackOffset + track));
      }));
  }

  // TOOL MENU
  buttonsToolSelect.push_back(new TouchButton(GRID_X[0], GRID_Y[2],
  [ this ] (auto *b) { // drawHandler
    b->draw("TOOL", (data.isSongMode) ? "SNG": "PAT", instance->isModeToolActive() ? TouchButton::BUTTON_HIGHLIGHTED : TouchButton::BUTTON_NORMAL);
  },
  [ this ] (auto *b) { // clickedHandler
    instance->selectTools((data.isSongMode) ? TOOLS_SONG : TOOLS_PATTERN);
  }));
  buttonsToolSelect.push_back(new TouchButton(GRID_X[1], GRID_Y[2],
  [ this ] (auto *b) { // drawHandler
    b->draw("TOOL", "ARP", currentTools == TOOLS_ARP ? TouchButton::BUTTON_HIGHLIGHTED : TouchButton::BUTTON_NORMAL);
  },
  [ this ] (auto *b) { // clickedHandler
    instance->selectTools(TOOLS_ARP);
  }));
  buttonsToolSelect.push_back(new TouchButton(GRID_X[2], GRID_Y[2],
  [ this ] (auto *b) { // drawHandler
    b->draw("TOOL", "SEQ", currentTools == TOOLS_SEQ ? TouchButton::BUTTON_HIGHLIGHTED : TouchButton::BUTTON_NORMAL);
  },
  [ this ] (auto *b) { // clickedHandler
    instance->selectTools(TOOLS_SEQ);
  }));

  // SEQUENCER TOOLS
  toolsPages[TOOLS_SEQ].push_back(new TouchButton(GRID_X[0], GRID_Y[3],
  [ ] (auto *b) { // drawHandler
    b->draw("TRACK", "SETUP", TouchButton::BUTTON_LABEL);
  }));
  
  selectedTrackSetup.label = new TouchButton(GRID_X[1], GRID_Y[3],
  [ this ] (auto *b) { // drawHandler
    char temp_char[2];
    itoa(data.activeTrack + 1, temp_char, 10);
    b->draw("TRACK", temp_char, TouchButton::BUTTON_LABEL);
  });
  toolsPages[TOOLS_SEQ].push_back(selectedTrackSetup.label);

  // track instrument
  ValueButtonRange<uint8_t> *currentTrackInstrument = new ValueButtonRange<uint8_t>(&currentValue, GRID_X[3], GRID_Y[3], selectedTrackSetup.instrument, 0, 15, 1, data.trackSettings[data.activeTrack].instrument, 
  [ this ] (auto *b, auto *v) { // drawHandler
    char name[10];
    char sub[10];
    // MDT has instruments 0-8 / MIDI has instuments 0-15
    const uint8_t maxInstrument = (selectedTrackSetup.device == LiveSequencer::DEVICE_INTERNAL) ? LiveSequencer::INSTR_MSP2 : 15;
    static_cast<EditableValueRange<uint8_t>*>(v)->changeRange(0, maxInstrument); 
    
    liveSeq.getInstrumentName(selectedTrackSetup.device, v->getValue(), name, sub);
    b->draw(name, sub, TouchButton::BUTTON_ACTIVE);
    instance->checkApplyTrackInstrument();
  });
  toolsPages[TOOLS_SEQ].push_back(currentTrackInstrument);

  // track device
  toolsPages[TOOLS_SEQ].push_back(new ValueButtonRange<uint8_t>(&currentValue, GRID_X[2], GRID_Y[3], selectedTrackSetup.device, LiveSequencer::DEVICE_INTERNAL, LiveSequencer::DEVICE_MIDI_INT, 1, data.trackSettings[data.activeTrack].device, 
  [ this, currentTrackInstrument ] (auto *b, auto *v) { // drawHandler
    char name[10];
    char sub[10];
    liveSeq.getDeviceName(v->getValue(), name, sub);
    b->draw(name, sub, TouchButton::BUTTON_ACTIVE);
    currentTrackInstrument->drawNow();
    instance->checkApplyTrackInstrument();
  }));

  // track quantize
  toolsPages[TOOLS_SEQ].push_back(new ValueButtonVector<uint8_t>(&currentValue, GRID_X[4], GRID_Y[3], selectedTrackSetup.quantizeDenom, { 1, 2, 4, 8, 16, 32 }, data.trackSettings[data.activeTrack].quantizeDenom,
  [ this ] (auto *b, auto *v) { // drawHandler
    b->draw("QUANT", (v->getValue() == 1) ? "NONE" : v->toString(), (v->getValue() == 1) ? TouchButton::BUTTON_NORMAL : TouchButton::BUTTON_ACTIVE);
    if (data.trackSettings[data.activeTrack].quantizeDenom != selectedTrackSetup.quantizeDenom) {
      data.trackSettings[data.activeTrack].quantizeDenom = selectedTrackSetup.quantizeDenom;
      data.guiUpdateFlags |= drawActiveTrackSubLabel;
    }
  }));

  // track velocity
  toolsPages[TOOLS_SEQ].push_back(new ValueButtonRange<uint8_t>(&currentValue, GRID_X[5], GRID_Y[3], selectedTrackSetup.velocity, 0, 100, 5, data.trackSettings[data.activeTrack].velocityLevel,
  [ this ] (auto *b, auto *v) { // drawHandler
    b->draw("VELOCTY", (v->getValue() == 0) ? "KEY" : v->toString() + std::string("%"), (v->getValue() == 0) ? TouchButton::BUTTON_NORMAL : TouchButton::BUTTON_ACTIVE);
    if (data.trackSettings[data.activeTrack].velocityLevel != selectedTrackSetup.velocity) {
      data.trackSettings[data.activeTrack].velocityLevel = selectedTrackSetup.velocity;
      data.guiUpdateFlags |= drawActiveTrackSubLabel;
    }
  }));

  // jump to other pages
  toolsPages[TOOLS_SEQ].push_back(new TouchButton(GRID_X[0], GRID_Y[4],
  [ ] (auto *b) { // drawHandler
    b->draw("JUMP", "PAGE", TouchButton::BUTTON_LABEL);
  }));
  toolsPages[TOOLS_SEQ].push_back(new TouchButton(GRID_X[1], GRID_Y[4],
  [ ] (auto *b) { // drawHandler
    b->draw("FILE", "MANAGER", TouchButton::BUTTON_ACTIVE);
  },
  [ this ] (auto *b) { // clickedHandler
    openScreen(UI_func_file_manager, data.performanceID);
  }));
  toolsPages[TOOLS_SEQ].push_back(new TouchButton(GRID_X[2], GRID_Y[4],
  [ ] (auto *b) { // drawHandler
    b->draw("MASTER", "EFFECTS", TouchButton::BUTTON_ACTIVE);
  },
  [ this ] (auto *b) { // clickedHandler
    openScreen(UI_func_master_effects, data.performanceID);
  }));
  toolsPages[TOOLS_SEQ].push_back(new TouchButton(GRID_X[3], GRID_Y[4],
  [ ] (auto *b) { // drawHandler
    b->draw("MULTI", "BAND", TouchButton::BUTTON_ACTIVE);
  },
  [ this ] (auto *b) { // clickedHandler
    openScreen(UI_func_multiband_dynamics, data.performanceID);
  }));

  toolsPages[TOOLS_SEQ].push_back(new TouchButton(GRID_X[0], GRID_Y[5],
  [ ] (auto *b) { // drawHandler
    b->draw("ACTIONS", "", TouchButton::BUTTON_LABEL);
  }));

  TouchButton *confirmDelete = new TouchButton(GRID_X[2], GRID_Y[5],
  [ this ] (auto *b) { // drawHandler
    if (deleteConfirming) {
      b->draw("DO IT", "!", TouchButton::BUTTON_RED);
    } else {
      b->clear(COLOR_BACKGROUND);
    }
  },
  [ this ] (auto *b) { // clickedHandler
    // really delete
    if (deleteConfirming) {
      deleteConfirming = false;
      if (data.isSongMode) {
        liveSeq.deleteAllSongEvents();
      }
      else {
        liveSeq.deleteLiveSequencerData();
      }
      b->draw("DELETE", "OK", TouchButton::BUTTON_LABEL);
    }
  });
  toolsPages[TOOLS_SEQ].push_back(confirmDelete);
  toolsPages[TOOLS_SEQ].push_back(new TouchButton(GRID_X[1], GRID_Y[5],
  [ this ] (auto *b) { // drawHandler
    b->draw("DELETE", data.isSongMode ? "SONG" : "ALL", TouchButton::BUTTON_ACTIVE);
  },
  [ this, confirmDelete ] (auto *b) { // clickedHandler
    deleteConfirming = !deleteConfirming;
    confirmDelete->drawNow();
  }));
  toolsPages[TOOLS_SEQ].push_back(new TouchButton(GRID_X[3], GRID_Y[5],
  [ ] (auto *b) { // drawHandler
    b->draw("SEQ", "SETTING", TouchButton::BUTTON_ACTIVE);
  },
  [ ] (auto *b) { // clickedHandler
    // open sequencer settings
    display.setTextSize(2);
    display.setTextColor(COLOR_SYSTEXT, COLOR_BACKGROUND);
    openScreen(UI_func_seq_settings);
  }));
  toolsPages[TOOLS_SEQ].push_back(new TouchButton(GRID_X[4], GRID_Y[5],
  [ ] (auto *b) { // drawHandler
    b->draw("LOAD", "PERF", TouchButton::BUTTON_ACTIVE);
  },
  [ this ] (auto *b) { // clickedHandler
    // load
    display.setTextSize(2);
    display.setTextColor(COLOR_SYSTEXT, COLOR_BACKGROUND);
    openScreen(UI_func_load_performance, data.performanceID);
  }));
  toolsPages[TOOLS_SEQ].push_back(new TouchButton(GRID_X[5], GRID_Y[5],
  [ ] (auto *b) { // drawHandler
    b->draw("SAVE", "PERF", TouchButton::BUTTON_ACTIVE);
  },
  [ this ] (auto *b) { // clickedHandler
    // save
    display.setTextSize(2);
    display.setTextColor(COLOR_SYSTEXT, COLOR_BACKGROUND);
    openScreen(UI_func_save_performance, data.performanceID);
  }));

  // PATTERN TOOLS
  TouchButton *applyPatternLength = new TouchButton(GRID_X[2], GRID_Y[3],
  [ this ] (auto *b) { // drawHandler
    const bool isSame = (data.numberOfBars == numberOfBarsTemp);
    b->draw("APPLY", "NOW", isSame ? TouchButton::BUTTON_NORMAL : TouchButton::BUTTON_RED);
    display.setTextSize(1);
    display.setTextColor(isSame ? COLOR_SYSTEXT : RED, COLOR_BACKGROUND);
    display.setCursor(GRID_X[3] + 2, GRID_Y[3] + 5);
    display.print(F("CHANGING PATTERN LENGTH"));
    display.setCursor(GRID_X[3] + 2, GRID_Y[3] + 20);
    display.print(F("WILL DELETE ALL DATA!"));
  },
  [ this ] (auto *b){ // clickedHandler
    if (data.numberOfBars != numberOfBarsTemp) {
      liveSeq.stop();
      liveSeq.changeNumberOfBars(numberOfBarsTemp);
      b->drawNow();
    }
  });
  toolsPages[TOOLS_PATTERN].push_back(applyPatternLength);
  toolsPages[TOOLS_PATTERN].push_back(new TouchButton(GRID_X[0], GRID_Y[3],
  [ ] (auto *b) { // drawHandler
    b->draw("PATTERN", "LENGTH", TouchButton::BUTTON_LABEL);
  }));
  toolsPages[TOOLS_PATTERN].push_back(new ValueButtonVector<uint8_t>(&currentValue, GRID_X[1], GRID_Y[3], numberOfBarsTemp, { 1, 2, 4, 8 }, 4, 
  [ applyPatternLength ] (auto *b, auto *v) { // drawHandler
    b->draw("LENGTH", v->toString(), TouchButton::BUTTON_ACTIVE);
    applyPatternLength->drawNow();
  }));

  // fill notes
  toolsPages[TOOLS_PATTERN].push_back(new TouchButton(GRID_X[0], GRID_Y[4],
  [ ] (auto *b) { // drawHandler
    b->draw("FILL", "NOTES", TouchButton::BUTTON_LABEL);
  }));
  lastNoteLabel = new TouchButton(GRID_X[1], GRID_Y[4],
  [ this ] (auto *b) { // drawHandler
    char temp_char[4];
    b->draw("NOTE", itoa(data.lastPlayedNote, temp_char, 10), TouchButton::BUTTON_LABEL);
  });
  toolsPages[TOOLS_PATTERN].push_back(lastNoteLabel);

  toolsPages[TOOLS_PATTERN].push_back(new ValueButtonVector<uint8_t>(&currentValue, GRID_X[2], GRID_Y[4], data.fillNotes.number, { 4, 6, 8, 12, 16, 24, 32 }, 16, 
  [ ] (auto *b, auto *v) { // drawHandler
    b->draw("NUMBER", v->toString(), TouchButton::BUTTON_ACTIVE);
  }));
  toolsPages[TOOLS_PATTERN].push_back(new ValueButtonRange<uint8_t>(&currentValue, GRID_X[3], GRID_Y[4], data.fillNotes.offset, 0, 7, 1, 0, 
  [ ] (auto *b, auto *v) { // drawHandler
    b->draw("OFFSET", v->toString(), TouchButton::BUTTON_ACTIVE);
  }));
  toolsPages[TOOLS_PATTERN].push_back(new ValueButtonRange<uint8_t>(&currentValue, GRID_X[4], GRID_Y[4], data.fillNotes.velocityLevel, 0, 100, 5, 100, 
  [ ] (auto *b, auto *v) { // drawHandler
    b->draw("VELOCTY", v->toString() + std::string("%"), TouchButton::BUTTON_ACTIVE);
  }));
  toolsPages[TOOLS_PATTERN].push_back(new TouchButton(GRID_X[5], GRID_Y[4],
  [ ] (auto *b) { // drawHandler
    b->draw("FILL", "NOW", TouchButton::BUTTON_RED);
  },
  [ this ](auto *b) { // clickedHandler
    liveSeq.fillTrackLayer();
  }));

  // SONG TOOLS
  toolsPages[TOOLS_SONG].push_back(new TouchButton(GRID_X[0], GRID_Y[3],
  [ ] (auto *b) { // drawHandler
    b->draw("MUTE", "QUANT", TouchButton::BUTTON_LABEL);
  }));
  toolsPages[TOOLS_SONG].push_back(new ValueButtonVector<uint8_t>(&currentValue, GRID_X[1], GRID_Y[3], data.songMuteQuantizeDenom, { 1, 2, 4, 8 }, 1,
  [ ] (auto *b, auto *v) { // drawHandler
    b->draw("QUANT", (v->getValue() == 1) ? "NONE" : v->toString(), (v->getValue() == 1) ? TouchButton::BUTTON_ACTIVE : TouchButton::BUTTON_HIGHLIGHTED);
  }));
  toolsPages[TOOLS_SONG].push_back(new TouchButton(GRID_X[0], GRID_Y[4],
  [ ] (auto *b) { // drawHandler
    b->draw("SONG", "LAYERS", TouchButton::BUTTON_LABEL);
  }));
  toolsPages[TOOLS_SONG].push_back(new TouchButton(GRID_X[1], GRID_Y[4],
  [ this ] (auto *b) { // drawHandler
    std::string t1 = (data.songLayerCount == 0) ? "NO" : "LAYER";
    std::string t2 = (data.songLayerCount == 0) ? "LAYERS" : "ACTION";
    b->draw(t1, t2, (data.songLayerCount == 0) ? TouchButton::BUTTON_LABEL : TouchButton::BUTTON_ACTIVE);
    data.guiUpdateFlags |= drawSongLayers;
  }, [ this ] (auto *b) { // clickedHandler
    if (data.songLayerCount > 0) {
      if (++songLayerMode == LiveSequencer::LayerMode::LAYER_MODE_NUM) {
        songLayerMode = LiveSequencer::LayerMode::LAYER_MUTE;
      }
      data.guiUpdateFlags |= drawSongLayers;
    }
  }));

  // ARP TOOL
  toolsPages[TOOLS_ARP].push_back(new TouchButton(GRID_X[0], GRID_Y[3],
  [ this ] (auto *b) { // drawHandler
    b->draw("ENABLE", (data.arpSettings.enabled == 0) ? "OFF" : "ON", (data.arpSettings.enabled == 0) ? TouchButton::BUTTON_ACTIVE : TouchButton::BUTTON_HIGHLIGHTED);
  }, [ this ] (auto *b) { // clickedHandler
    liveSeq.setArpEnabled(!data.arpSettings.enabled);
    b->drawNow();
  }));
  toolsPages[TOOLS_ARP].push_back(new ValueButtonVector<uint8_t>(&currentValue, GRID_X[1], GRID_Y[3], data.arpSettings.amount, { 1, 2, 3, 4, 6, 8, 12, 16, 24, 32, 48, 64 }, 8,
  [ ] (auto *b, auto *v) { // drawHandler
    b->draw("SPEED", v->toString(), TouchButton::BUTTON_ACTIVE);
  }));
  toolsPages[TOOLS_ARP].push_back(new ValueButtonVector<uint8_t>(&currentValue, GRID_X[2], GRID_Y[3], data.arpSettings.octaves, { 1, 2, 3, 4 }, 1,
  [ ] (auto *b, auto *v) { // drawHandler
    b->draw("OCTAVES", v->toString(), TouchButton::BUTTON_ACTIVE);
  },
  [ this ] (auto *v) { // changedHandler
    data.arpSettings.arpSettingsChanged = true;
  }));
  toolsPages[TOOLS_ARP].push_back(new ValueButtonRange<uint8_t>(&currentValue, GRID_X[3], GRID_Y[3], (uint8_t&)data.arpSettings.mode, 0, uint8_t(LiveSequencer::ARP_MODENUM-1), 1, uint8_t(LiveSequencer::ARP_UP),
  [ ] (auto *b, auto *v) { // drawHandler
    b->draw("MODE", UI_LiveSequencer::getArpModeName(v->getValue()).c_str(), TouchButton::BUTTON_ACTIVE);
  },
  [ this ] (auto *v) { // changedHandler
    data.arpSettings.arpSettingsChanged = true;
  }));
  toolsPages[TOOLS_ARP].push_back(new ValueButtonRange<uint16_t>(&currentValue, GRID_X[4], GRID_Y[3], data.arpSettings.length, 10, 420, 10, 150,
  [ ] (auto *b, auto *v) { // drawHandler
    b->draw("LENGTH", v->toString(), v->getValue() == 0 ? TouchButton::BUTTON_ACTIVE : TouchButton::BUTTON_ACTIVE);
  }));
  toolsPages[TOOLS_ARP].push_back(new ValueButtonRange<int8_t>(&currentValue, GRID_X[5], GRID_Y[3], data.arpSettings.swing, -8, 8, 1, 0,
  [ ] (auto *b, auto *v) { // drawHandler
    b->draw("SWING", v->toString(), TouchButton::BUTTON_ACTIVE);
  }));
  toolsPages[TOOLS_ARP].push_back(new ValueButtonRange<uint8_t>(&currentValue, GRID_X[0], GRID_Y[4], data.arpSettings.source, 0, uint8_t(LiveSequencer::LIVESEQUENCER_NUM_TRACKS), 1, 0,
  [ ] (auto *b, auto *v) { // drawHandler
    b->draw("SOURCE", (v->getValue() == 0) ? "KEY" : std::string("TK") + v->toString(), TouchButton::BUTTON_ACTIVE);
  },
  [ this ] (auto *v) { // changedHandler
    liveSeq.onArpSourceChanged();
  }));
  toolsPages[TOOLS_ARP].push_back(new ValueButtonRange<uint8_t>(&currentValue, GRID_X[1], GRID_Y[4], data.arpSettings.velocityLevel, 0, 100, 5, 100,
  [ ] (auto *b, auto *v) { // drawHandler
    b->draw("VELOCTY", v->toString() + std::string("%"), TouchButton::BUTTON_ACTIVE);
  }));
  toolsPages[TOOLS_ARP].push_back(new ValueButtonRange<uint8_t>(&currentValue, GRID_X[2], GRID_Y[4], data.arpSettings.latch, 0, 1, 1, 1,
  [ ] (auto *b, auto *v) { // drawHandler
    b->draw("LATCH", v->getValue() == 1 ? "ON" : "-", TouchButton::BUTTON_ACTIVE);
  }));
  toolsPages[TOOLS_ARP].push_back(new ValueButtonVector<uint8_t>(&currentValue, GRID_X[3], GRID_Y[4], data.arpSettings.loadPerBar, { 1, 2, 4 }, 2,
  [ ] (auto *b, auto *v) { // drawHandler
    b->draw("SAMPLE", std::string(v->toString()) + "x", TouchButton::BUTTON_ACTIVE);
  }));
  toolsPages[TOOLS_ARP].push_back(new ValueButtonRange<uint8_t>(&currentValue, GRID_X[4], GRID_Y[4], data.arpSettings.noteRepeat, 0, 4, 1, 0,
  [ ] (auto *b, auto *v) { // drawHandler
    b->draw("REPEAT", std::string(v->toString()) + "x", TouchButton::BUTTON_ACTIVE);
  }));
  toolsPages[TOOLS_ARP].push_back(new ValueButtonRange<uint8_t>(&currentValue, GRID_X[5], GRID_Y[4], data.arpSettings.freerun, 0, 1, 1, 0,
  [ ] (auto *b, auto *v) { // drawHandler
    b->draw("FREERUN", v->getValue() == 1 ? "ON" : "-", TouchButton::BUTTON_ACTIVE);
  }));
}

FLASHMEM void UI_LiveSequencer::selectTools(Tools tools) {
  if (currentTools != tools) {
    if (currentValue.button != nullptr) {
      currentValue.button->setSelected(false);
    }
    currentValue.valueBase = nullptr;
    currentTools = tools;

    clearBottomArea();    
    data.guiUpdateFlags |= drawTools;
  }
}

FLASHMEM bool UI_LiveSequencer::isModeToolActive(void) {
  bool result = false;
  result |= (currentTools == TOOLS_PATTERN) && (data.isSongMode == false);
  result |= (currentTools == TOOLS_SONG) && (data.isSongMode == true);
  return result;
}

FLASHMEM void UI_LiveSequencer::showDirectMappingWarning(uint8_t inChannel) {
  if (showingHowTo == false) {
    showingHowTo = true;
    display.fillScreen(COLOR_BACKGROUND);
    display.setTextSize(2);
    display.setTextColor(RED);
    setCursor_textGrid(1, 1);
    display.printf("ADAPT MIDI MAPPING");
    display.setTextSize(1);
    display.setTextColor(GREY1);
    setCursor_textGrid_small(2, 6);
    display.print(F("LIVESEQUENCER FORWARDS MIDI EVENTS TO SELECTED"));
    setCursor_textGrid_small(2, 7);
    display.print(F("INSTRUMENT CHANNELS AND DOES NOT WORK CORRECTLY"));
    setCursor_textGrid_small(2, 8);
    display.print(F("ON DIRECTLY MAPPED CHANNELS AS MIDI INPUT"));
    setCursor_textGrid_small(2, 10);
    display.setTextColor(COLOR_SYSTEXT);
    display.print(F("PLEASE MAP ALL INSTRUMENTS TO CHANNELS"));
    setCursor_textGrid_small(2, 11);
    display.print(F("OTHER THAN MIDI INPUT CHANNEL"));
    display.setTextColor(RED);
    setCursor_textGrid_small(32, 11);
    display.printf("%02i", inChannel);
    setCursor_textGrid_small(2, 13);
    display.setTextColor(COLOR_SYSTEXT);
    display.print(F("OR"));
    display.setTextColor(GREY1);
    setCursor_textGrid_small(5, 13);
    display.print(F("CHANGE THE MIDI CHANNEL ON YOUR"));
    setCursor_textGrid_small(2, 14);
    display.print(F("EXTERNAL INPUT DEVICE TO AN UNUSED CHANNEL"));
    TouchButton::drawButton(GRID_X[5], GRID_Y[5], "REMAP", "MIDI", TouchButton::BUTTON_ACTIVE);
    helptext_l(back_text);
  }
}

FLASHMEM void UI_LiveSequencer::resetProgressBars(void) {
  barPattern.currentPhase = 0;
  barPattern.drawnLength = 0;
  barTotal.currentPhase = 0;
  barTotal.drawnLength = 0;
}

FLASHMEM void UI_LiveSequencer::onStopped(void) {
  if (data.isActive && (runningInBackground == false)) {
    resetProgressBars();
    data.guiUpdateFlags |= (drawActiveNotes | drawTime);
    drawGUI(data.guiUpdateFlags);
  }
}

FLASHMEM void UI_LiveSequencer::processLCDM(void) {
// ****** SETUP *********
  if (LCDML.FUNC_setup()) {
    registerTouchHandler(handle_touchscreen_live_sequencer);
    data.isActive = true;
    runningInBackground = false;
    display.fillScreen(COLOR_BACKGROUND);
    numberOfBarsTemp = data.numberOfBars;
    liveSeq.onGuiInit();

    display.setTextSize(1);
    display.setCursor(GRID_X[2], 3);
    display.setTextColor(COLOR_SYSTEXT, COLOR_BACKGROUND);
    display.print(data.performanceName.c_str());

    data.guiUpdateFlags = 0;
    resetProgressBars();
    redrawScreen();
    drawGUI(data.guiUpdateFlags);
    // setup function
    LCDML.FUNC_setLoopInterval(40); // 25Hz gui refresh
  }
  // ****** LOOP *********
  if (LCDML.FUNC_loop()) {
    if (LCDML.BT_checkDown()) {
      if (currentValue.valueBase != nullptr) {
        currentValue.valueBase->next();
      } else if ((trackOffset == 0) && LiveSequencer::LIVESEQUENCER_NUM_TRACKS > LiveSequencer::LIVESEQUENCER_TRACKS_PER_SCREEN) {
        trackOffset = LiveSequencer::LIVESEQUENCER_TRACKS_PER_SCREEN;
        data.guiUpdateFlags |= drawTrackButtons;
        if (isLayerViewActive) {
          clearBottomArea();
          data.guiUpdateFlags |= drawLayerButtons;
        }
      } 
    }
    if (LCDML.BT_checkUp()) {
      if (currentValue.valueBase != nullptr) {
        currentValue.valueBase->previous();
      } else if (trackOffset == LiveSequencer::LIVESEQUENCER_TRACKS_PER_SCREEN) {
        trackOffset = 0;
        data.guiUpdateFlags |= drawTrackButtons;
        if (isLayerViewActive) {
          clearBottomArea();
          data.guiUpdateFlags |= drawLayerButtons;
        }
      }
    }

    if (LCDML.BT_checkEnter()) {
      if (currentValue.valueBase != nullptr) {
        currentValue.button->setSelected(false);
        currentValue.valueBase = nullptr;
      }
    }
    
    data.guiUpdateFlags |= data.isRunning ? (drawActiveNotes | drawTime) : 0;

    if ((isLayerViewActive == false) && (currentTools == TOOLS_SEQ) && (data.guiUpdateFlags & drawLastPlayedNote)) {
      lastNoteLabel->drawNow();
    }

    if (showingHowTo == false) {
      drawGUI(data.guiUpdateFlags);
    }
  }

  // ****** STABLE END *********
  if (LCDML.FUNC_close()) {
    unregisterTouchHandler();
    if (runningInBackground == false) {
      data.isActive = false;
    }
    data.isRecording = false;
    showingHowTo = false;
    display.fillScreen(COLOR_BACKGROUND);
  }
}

FLASHMEM void UI_LiveSequencer::clearBottomArea(void) {
  display.console = true;
  display.fillRect(0, GRID_Y[2], DISPLAY_WIDTH, DISPLAY_HEIGHT - GRID_Y[2], COLOR_BACKGROUND);
}

FLASHMEM void UI_LiveSequencer::redrawScreen(void) {
  data.guiUpdateFlags |= (drawTopButtons | drawTrackButtons | drawTime);
  isLayerViewActive = (showingTools == false);
  if (isLayerViewActive) {
    data.guiUpdateFlags |= drawLayerButtons;
  } else {
    data.guiUpdateFlags |= drawTools;
  }
  clearBottomArea();
}

FLASHMEM void UI_LiveSequencer::openScreen(LCDML_FuncPtr_pu8 screen, uint8_t param) {
  // stay active in background and keep processing midi inputs for screens opened in LiveSequencer
  runningInBackground = true; 
  LCDML.FUNC_setGBAToLastFunc();
  LCDML.OTHER_jumpToFunc(screen, param);
}

FLASHMEM void UI_LiveSequencer::onTrackButtonPressed(uint8_t track) {
  if (track == data.activeTrack) {
    if (data.isRecording) {
      if (data.pendingEvents.size()) {
        data.pendingEvents.clear(); // clear pending
      }
      else {
        // track layer actions only for pattern mode
        if (currentPage == PAGE_PATTERN) {
          if (++trackLayerMode == LiveSequencer::LayerMode::LAYER_MODE_NUM) {
            trackLayerMode = LiveSequencer::LayerMode::LAYER_MUTE;
          }
          layerUpdates[SCREEN_TRACK_INDEX(data.activeTrack)] = 0xFF;
        }
      }
    }
    else {
      // open instrument settings
      if (data.tracks[track].screenSetupFn != nullptr) {
        SetupFn f = (SetupFn)data.tracks[track].screenSetupFn;
        f(0);
      }
      openScreen(data.tracks[track].screen);
    }
  }
  else {
    const uint8_t activeOld = data.activeTrack;
    data.activeTrack = track;
    trackButtons[SCREEN_TRACK_INDEX(activeOld)]->drawNow();
    trackButtons[SCREEN_TRACK_INDEX(track)]->drawNow();

    //check if update track instrument selection
    if ((isLayerViewActive == false) && (currentTools == TOOLS_SEQ)) {
      updateTrackChannelSetupButtons();
    }
    trackLayerMode = LiveSequencer::LayerMode::LAYER_MUTE;
    DBG_LOG(printf("active track now is %i\n", track + 1));
  }
}

FLASHMEM void UI_LiveSequencer::updateTrackChannelSetupButtons(void) {
  selectedTrackSetup.device = data.trackSettings[data.activeTrack].device;
  selectedTrackSetup.instrument = data.trackSettings[data.activeTrack].instrument;
  selectedTrackSetup.quantizeDenom = data.trackSettings[data.activeTrack].quantizeDenom;
  selectedTrackSetup.velocity = data.trackSettings[data.activeTrack].velocityLevel;
  // update currently selected track
  data.guiUpdateFlags |= drawTools;
}

FLASHMEM void UI_LiveSequencer::handleTouchscreen(void) {
  if (showingHowTo) {
    if (TouchButton::isPressed(GRID_X[5], GRID_Y[5])) {
      openScreen(UI_func_midi_channels);
      runningInBackground = false; // do not process midi in background
    }
    return;
  }

  const bool runningChanged = (runningHere != data.isRunning);
  runningHere = data.isRunning;
  
  const bool runningPressed = TouchButton::isPressed(GRID_X[0], GRID_Y[0]);
  if (runningPressed) {
    if (runningHere) {
      liveSeq.stop();
    }
    else {
      liveSeq.start();
    }
  }

  if (runningPressed || runningChanged) {
    data.guiUpdateFlags |= drawTopButtons;
    if (trackLayerMode != LiveSequencer::LayerMode::LAYER_MUTE) {
      trackLayerMode = LiveSequencer::LayerMode::LAYER_MUTE;
      layerUpdates[data.activeTrack] = 0xFF;
    }
  }

  const bool recPressed = TouchButton::isPressed(GRID_X[1], GRID_Y[0]);
  if (recPressed) {
    data.isRecording = !data.isRecording;
    data.guiUpdateFlags |= (drawTopButtons | drawTrackButtons);
    trackLayerMode = LiveSequencer::LayerMode::LAYER_MUTE;

    if (isLayerViewActive && data.isSongMode) {
      data.guiUpdateFlags |= drawLayerButtons;
    }
  }

  const bool toolsPressed = TouchButton::isPressed(GRID_X[4], GRID_Y[0]);
  if (toolsPressed) {
    // possible switch to song / pattern tools if mode changed
    currentTools = data.isSongMode && currentTools == TOOLS_PATTERN ? TOOLS_SONG : currentTools;
    currentTools = !data.isSongMode && currentTools == TOOLS_SONG ? TOOLS_PATTERN : currentTools;
    showingTools = !showingTools;
    if (showingTools == false) {
      if (currentValue.button != nullptr) {
        currentValue.button->setSelected(false);
      }
      currentValue.valueBase = nullptr;
    }
    redrawScreen();
  }

  const bool modePressed = TouchButton::isPressed(GRID_X[5], GRID_Y[0]);
  if (modePressed) {
    const bool newIsSongMode = !data.isSongMode;
    if (showingTools == false) {
      currentPage = newIsSongMode ? PAGE_SONG : PAGE_PATTERN;
    }
    if (isModeToolActive()) { // switch pattern / song tools if selected
      currentTools = newIsSongMode ? TOOLS_SONG : TOOLS_PATTERN;
    }
    data.isSongMode = newIsSongMode;
    redrawScreen();
  }

  for (TouchButton *b : trackButtons) {
    b->processPressed();
  }
  
  if (isLayerViewActive) {
    for (uint8_t track = 0; track < LiveSequencer::LIVESEQUENCER_TRACKS_PER_SCREEN; track++) {      
      for (uint8_t layer = 0; layer < data.trackSettings[(trackOffset + track)].layerCount; layer++) {
        const bool pressed = TouchButton::isPressed(GRID_X[track], GRID_Y[2 + layer]);
        if (pressed) {
          if (trackLayerMode != LiveSequencer::LayerMode::LAYER_MUTE) {
            if (data.isRecording && (trackOffset + track == data.activeTrack)) {
              const bool success = liveSeq.trackLayerAction(trackOffset + track, layer, LiveSequencer::LayerMode(trackLayerMode));
              if(success) {
                // one less layer now, clear last layer button
                TouchButton::clearButton(GRID_X[SCREEN_TRACK_INDEX(track)], GRID_Y[2 + data.trackSettings[trackOffset + track].layerCount], COLOR_BACKGROUND);
                trackLayerMode = LiveSequencer::LayerMode::LAYER_MUTE;
              }
            }
          }
          else {
            const bool isMutedOld = data.tracks[trackOffset + track].layerMutes & (1 << layer);
            const bool recordMuteToSong = data.isSongMode && data.isRecording && data.isRunning;
            liveSeq.setLayerMuted(trackOffset + track, layer, !isMutedOld, recordMuteToSong);
          }
        }
      }
    }
  } else {
    // process TOOLS MENU if tools view active
    for (auto *b : buttonsToolSelect) {
      b->processPressed();
    }
    for (TouchButton *b : toolsPages[currentTools]) {
      b->processPressed();
    }

    if (currentTools == TOOLS_SONG) {
      if (songLayerMode != LiveSequencer::LayerMode::LAYER_MUTE) { // song layers can not be muted
        for (uint8_t songLayer = 0; songLayer < data.songLayerCount; songLayer++) {
          if (TouchButton::isPressed(GRID_X[2 + songLayer], GRID_Y[4])) {
            liveSeq.songLayerAction(songLayer, LiveSequencer::LayerMode(songLayerMode));
            songLayerMode = LiveSequencer::LayerMode::LAYER_MUTE;
            TouchButton::clearButton(GRID_X[2 + data.songLayerCount], GRID_Y[4], COLOR_BACKGROUND);
            data.guiUpdateFlags |= drawSongLayers;
            break;
          }
        }
      }
    }
  }
}

FLASHMEM void UI_LiveSequencer::drawBar(uint16_t x, uint16_t y, uint16_t w, uint16_t h, uint16_t color) {
  // this way it looks good on console and on MDT...
  for (uint8_t yloc = y; yloc < (y + h); yloc++) {
    display.drawLine(x, yloc, x + w, yloc, color);
  }
}

FLASHMEM void UI_LiveSequencer::processBar(const float progress, const uint16_t y, ProgressBar &bar, const uint16_t color) {
  const uint8_t totalBarWidth = progress * BAR_WIDTH;
  uint8_t drawWidth = totalBarWidth - bar.drawnLength;

  if (bar.drawnLength > totalBarWidth) {
    drawBar(GRID_X[2] + bar.drawnLength, y, BAR_WIDTH - bar.drawnLength, BAR_HEIGHT, bar.currentPhase ? color : GREY2);
    bar.currentPhase = !bar.currentPhase;
    bar.drawnLength = 0;
    drawWidth = totalBarWidth;
  }

  if (drawWidth > 0) {
    drawBar(GRID_X[2] + bar.drawnLength, y, drawWidth, BAR_HEIGHT, bar.currentPhase ? color : GREY2);
    bar.drawnLength = totalBarWidth;
  }
}

FLASHMEM void UI_LiveSequencer::drawGUI(uint16_t& guiFlags) {
  display.console = true;
  
  if (guiFlags & drawTopButtons) {
    TouchButton::drawButton(GRID_X[0], GRID_Y[0], (runningHere ? "STOP" : "START"), "", runningHere ? TouchButton::BUTTON_RED : TouchButton::BUTTON_NORMAL);
    TouchButton::drawButton(GRID_X[1], GRID_Y[0], "REC", "", data.isRecording ? TouchButton::BUTTON_RED : TouchButton::BUTTON_NORMAL);
    TouchButton::drawButton(GRID_X[4], GRID_Y[0], isLayerViewActive ? "LAYERS" : "TOOLS", "VIEW", TouchButton::BUTTON_NORMAL);
    TouchButton::drawButton(GRID_X[5], GRID_Y[0], data.isSongMode ? "SONG" : "PATT", "MODE", TouchButton::BUTTON_HIGHLIGHTED);
  }

  if (guiFlags & drawActiveTrackSubLabel) {
    drawTrackSubtext(data.activeTrack, trackOffset, true);
  }

  // print time
  if (guiFlags & drawTime) {
    uint16_t timeMs = data.isRunning ? uint16_t(data.patternTimer) : data.patternLengthMs;
    if (data.isRunning == false) {
        data.songPatternCount = data.lastSongEventPattern; // show song length
        data.currentPattern = data.numberOfBars - 1; // show num bars
    }

    const float progressPattern = timeMs / float(data.patternLengthMs);
    const float progressTotal = (progressPattern + data.currentPattern) / float(data.numberOfBars);

    processBar(progressPattern, 15, barPattern, GREEN);
    processBar(progressTotal, 20, barTotal, RED);

    uint16_t patCount = data.isRunning ? data.currentPattern : 0;
    display.setCursor(GRID_X[2], 28);
    display.setTextSize(1);
    display.setTextColor(COLOR_SYSTEXT, COLOR_BACKGROUND);
    if (data.isSongMode) {
      const uint32_t songMs = (data.songPatternCount * data.numberOfBars + data.currentPattern) * data.patternLengthMs + timeMs;
      const uint32_t minutes = songMs / 60000;
      const uint32_t seconds = (songMs % 60000) / 1000;
      const uint32_t millis = songMs % 1000;
      display.printf("S %i:%02i.%03i", minutes, seconds, millis);
    }
    else {
      display.printf("P %i.%04i   ", patCount, timeMs);
    }
    display.setCursor(200, 28);
    display.printf("%02i", data.isSongMode ? data.songPatternCount : data.currentPattern);
  }

  const bool doBlink = data.notesOn.size() || data.pendingEvents.size();
  if (doBlink) {
    if (guiCounter-- == 0) {
      guiCounter = 8;
      TouchButton::Color trackButtonRecColor = blinkPhase ? TouchButton::BUTTON_RED : TouchButton::BUTTON_HIGHLIGHTED;
      blinkPhase = !blinkPhase;
      char temp_char[4];
      trackButtons[SCREEN_TRACK_INDEX(data.activeTrack)]->draw(data.tracks[data.activeTrack].name, itoa(data.activeTrack + 1, temp_char, 10), trackButtonRecColor);
    }
  }
  else {
    guiCounter = 0;
    blinkPhase = 0;
  }

  if (data.guiUpdateFlags & drawTrackButtons) {
    DBG_LOG(printf("draw tracks\n"));
    for (TouchButton *b : trackButtons) {
      b->drawNow();
    }
  }

  if (isLayerViewActive) {
    const bool isSongRec = (data.isSongMode && data.isRecording);
    const bool drawAllLayers = guiFlags & drawLayerButtons;

    for (uint8_t track = 0; track < LiveSequencer::LIVESEQUENCER_TRACKS_PER_SCREEN; track++) {
      const bool layerEditActive = !data.isSongMode && (data.activeTrack == (trackOffset + track)) && (trackLayerMode != LiveSequencer::LayerMode::LAYER_MUTE);

      // layer button
      for (uint8_t layer = 0; layer < data.trackSettings[(trackOffset + track)].layerCount; layer++) {
        const bool isMuted = data.tracks[(trackOffset + track)].layerMutes & (1 << layer);
        const bool drawThisLayer = (layerUpdates[track] & (1 << layer)); // only used for song automation mute toggles
        TouchButton::Color color = (isMuted ? TouchButton::BUTTON_NORMAL : (isSongRec ? TouchButton::BUTTON_RED : TouchButton::BUTTON_ACTIVE));
        if (layerEditActive) {
          // adapt button background if in layer edit mode
          handleLayerEditButtonColor(trackLayerMode, color);
        }
        if (drawAllLayers || drawThisLayer) {
          drawLayerButton(data.isSongMode, trackLayerMode, layer, layerEditActive, color, GRID_X[track], GRID_Y[2 + layer]);
        }
        if (guiFlags & drawActiveNotes) {
          // always draw notes when layers visible
          const uint16_t barHeight = 6 * data.tracks[trackOffset + track].activeNotes[layer].size();
          const uint16_t xStart = GRID_X[track] + TouchButton::BUTTON_SIZE_X - 3;
          const uint16_t yStart = GRID_Y[2 + layer];
          
          const uint16_t yFill = std::min(barHeight, TouchButton::BUTTON_SIZE_Y);
          display.console = true;
          display.fillRect(xStart, yStart, 3, TouchButton::BUTTON_SIZE_Y - yFill, TouchButton::getColors(color).bg);
          display.fillRect(xStart, yStart + (TouchButton::BUTTON_SIZE_Y - yFill), 3, yFill, COLOR_SYSTEXT);
        }
      }
      layerUpdates[track] = 0;
    }
  }

  if (guiFlags & drawTools) {
    for (auto *b : buttonsToolSelect) {
      b->drawNow();
    }
    
    display.console = true;
    display.fillRect(0, GRID_Y[2] + TouchButton::BUTTON_SIZE_Y, DISPLAY_WIDTH, 4, MIDDLEGREEN);

    refreshToolsElements(static_cast<Tools>(currentTools));

    for (TouchButton *b : toolsPages[currentTools]) {
      b->drawNow();
    }
  }
  if (guiFlags & drawSongLayers) {
    TouchButton::Color color = TouchButton::BUTTON_ACTIVE;
    handleLayerEditButtonColor(songLayerMode, color);
    for (int songLayer = 0; songLayer < data.songLayerCount; songLayer++) {
      drawLayerButton(data.isSongMode, songLayerMode, songLayer, true, color, GRID_X[2 + songLayer], GRID_Y[4]);
    }
  }
  guiFlags = 0;
}

FLASHMEM void UI_LiveSequencer::drawSingleLayer(uint8_t track, uint8_t layer) {
  layerUpdates[SCREEN_TRACK_INDEX(track)] |= (1 << layer);
}

FLASHMEM void UI_LiveSequencer::refreshToolsElements(Tools tools) {
  // refresh contents of current tools elements
  switch (tools) {
  case TOOLS_PATTERN:
    lastNoteLabel->drawNow();
    updateTrackChannelSetupButtons();
    break;
    
  default:
    break;
  }
}

FLASHMEM std::string UI_LiveSequencer::getArpModeName(uint8_t mode) {
  switch (mode) {
  case LiveSequencer::ArpMode::ARP_CHORD:
    return "CHORD";
  case LiveSequencer::ArpMode::ARP_DOWN:
    return "DN";
  case LiveSequencer::ArpMode::ARP_DOWNUP:
    return "DNUP";
  case LiveSequencer::ArpMode::ARP_DOWNUP_P:
    return "DNUP+";
  case LiveSequencer::ArpMode::ARP_RANDOM:
    return "RAND";
  case LiveSequencer::ArpMode::ARP_UP:
    return "UP";
  case LiveSequencer::ArpMode::ARP_UPDOWN:
    return "UPDN";
  case LiveSequencer::ArpMode::ARP_UPDOWN_P:
    return "UPDN+";
  default:
    return "NONE";
  }
}

FLASHMEM void UI_LiveSequencer::drawLayerButton(const bool horizontal, uint8_t layerMode, int layer, const bool layerEditActive, TouchButton::Color color, uint16_t x, uint16_t y) {
  char temp_char[4];
  std::string label = "LAYER";
  std::string labelSub = itoa(layer + 1, temp_char, 10);
  if (layerEditActive) {
    switch (layerMode) {
    case LiveSequencer::LayerMode::LAYER_MERGE:
      if (layer > 0) {
        label = "MERGE";
        labelSub = horizontal ? "<" : "^";
      }
      break;
    case LiveSequencer::LayerMode::LAYER_DELETE:
      label = "DELETE";
      labelSub = "x";
      break;
    }
  }
  TouchButton::drawButton(x, y, label.c_str(), labelSub.c_str(), color);
}

FLASHMEM void UI_LiveSequencer::handleLayerEditButtonColor(uint8_t layerMode, TouchButton::Color &color) {
  switch (layerMode) {
  case LiveSequencer::LayerMode::LAYER_MERGE:
    color = TouchButton::BUTTON_HIGHLIGHTED;
    break;

  case LiveSequencer::LayerMode::LAYER_DELETE:
    color = TouchButton::BUTTON_RED;
    break;
  }
}

