#include "ui_livesequencer.h"
#include "LCDMenuLib2.h"
#include "ILI9341_t3n.h"
#include "sequencer.h"
#include "livesequencer.h"

extern LCDMenuLib2 LCDML;
extern ILI9341_t3n display;
extern bool remote_active;
extern int numTouchPoints;

extern void handleStart();
extern void handleStop();

extern void UI_func_load_performance(uint8_t param);
extern void UI_func_save_performance(uint8_t param);
extern void UI_func_midi_channels(uint8_t param);

extern void setCursor_textGrid(uint8_t pos_x, uint8_t pos_y);
extern void setCursor_textGrid_small(uint8_t pos_x, uint8_t pos_y);
extern void helptext_l(const char* str);

PROGMEM UI_LiveSequencer::UI_LiveSequencer(LiveSequencer& sequencer, LiveSequencer::LiveSeqData &d) : instance(this), liveSeq(sequencer), data(d) {
  static constexpr uint8_t BUTTON_OFFSET_X = 4; // center in screen
  static constexpr uint8_t BUTTON_OFFSET_Y = 2; // center in screen

  for(int i = 0; i < 6; i++) {
    GRID_X[i] = BUTTON_OFFSET_X + (i * 54);
    GRID_Y[i] = BUTTON_OFFSET_Y + (i * 40);
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
  TouchButton *applyPatternLength = new TouchButton(GRID_X[2], GRID_Y[3],
  [ this ] (auto *b) { // drawHandler
    const bool isSame = (data.numberOfBars == numberOfBarsTemp);
    b->draw("APPLY", "NOW", isSame ? TouchButton::BUTTON_ACTIVE : TouchButton::BUTTON_RED);
    display.setTextSize(1);
    display.setTextColor(isSame ? GREY2 : RED, GREY3);
    display.setCursor(GRID_X[3], GRID_Y[3] + 5);
    display.printf("CHANGING PATTERN LENGTH");
    display.setCursor(GRID_X[3], GRID_Y[3] + 20);
    display.printf("WILL DELETE ALL DATA!");
  },
  [ this ] (auto *b){ // clickedHandler
    if(data.numberOfBars != numberOfBarsTemp) {
      handleStop();
      liveSeq.changeNumberOfBars(numberOfBarsTemp);
      b->drawNow();
    }
  });
  toolsPages[TOOLS_SEQ].push_back(applyPatternLength);
  toolsPages[TOOLS_SEQ].push_back(new TouchButton(GRID_X[0], GRID_Y[3],
  [ ] (auto *b) { // drawHandler
    b->draw("PATTERN", "LENGTH", TouchButton::BUTTON_LABEL);
  }));
  toolsPages[TOOLS_SEQ].push_back(new ValueButtonVector<uint8_t>(&currentValue, GRID_X[1], GRID_Y[3], numberOfBarsTemp, std::vector<uint8_t>({ 1, 2, 4, 8 }), 4, 
  [ applyPatternLength ] (auto *b, auto *v) { // drawHandler
    b->draw("LENGTH", v->toString(), TouchButton::BUTTON_ACTIVE);
    applyPatternLength->drawNow();
  }));

  toolsPages[TOOLS_SEQ].push_back(new TouchButton(GRID_X[0], GRID_Y[5],
  [ ] (auto *b) { // drawHandler
    b->draw("ACTIONS", "", TouchButton::BUTTON_LABEL);
  }));

  TouchButton *confirmDelete = new TouchButton(GRID_X[2], GRID_Y[5],
  [ this ] (auto *b) { // drawHandler
    if(deleteConfirming) {
      b->draw("DO IT", "!", TouchButton::BUTTON_RED);
    } else {
      b->clear(GREY3);
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
    b->draw("DELETE", data.isSongMode ? "SONG" : "ALL", TouchButton::BUTTON_NORMAL);
  },
  [ this, confirmDelete ] (auto *b) { // clickedHandler
    deleteConfirming = !deleteConfirming;
    confirmDelete->drawNow();
  }));
  toolsPages[TOOLS_SEQ].push_back(new TouchButton(GRID_X[4], GRID_Y[5],
  [ ] (auto *b) { // drawHandler
    b->draw("LOAD", "PERF", TouchButton::BUTTON_NORMAL);
  },
  [ this ] (auto *b) { // clickedHandler
    // load
    LCDML.FUNC_setGBAToLastFunc();
    display.setTextSize(2);
    display.setTextColor(COLOR_SYSTEXT, COLOR_BACKGROUND);
    LCDML.OTHER_jumpToFunc(UI_func_load_performance, data.performanceID);
  }));
  toolsPages[TOOLS_SEQ].push_back(new TouchButton(GRID_X[5], GRID_Y[5],
  [ ] (auto *b) { // drawHandler
    b->draw("SAVE", "PERF", TouchButton::BUTTON_NORMAL);
  },
  [ ] (auto *b) { // clickedHandler
    // save
    LCDML.FUNC_setGBAToLastFunc();
    display.setTextSize(2);
    display.setTextColor(COLOR_SYSTEXT, COLOR_BACKGROUND);
    LCDML.OTHER_jumpToFunc(UI_func_save_performance); // FIXME: should have current id selected
  }));

  // PATTERN TOOLS
  for (int track = 0; track < LiveSequencer::LIVESEQUENCER_NUM_TRACKS; track++) {
    toolsPages[TOOLS_PATTERN].push_back(new ValueButtonVector<uint8_t>(&currentValue, GRID_X[track], GRID_Y[3], data.trackSettings[track].quantizeDenom, std::vector<uint8_t>({ 1, 2, 4, 8, 16, 32 }), 4,
    [ ] (auto *b, auto *v) { // drawHandler
      b->draw("QUANT", (v->getValue() == 1) ? "NONE" : v->toString(), (v->getValue() == 1) ? TouchButton::BUTTON_ACTIVE : TouchButton::BUTTON_HIGHLIGHTED);
    }));
  }
  toolsPages[TOOLS_PATTERN].push_back(new TouchButton(GRID_X[0], GRID_Y[4],
  [ ] (auto *b) { // drawHandler
    b->draw("FILL", "NOTES", TouchButton::BUTTON_LABEL);
  }));
  lastNoteLabel = new TouchButton(GRID_X[1], GRID_Y[4],
  [ this ] (auto *b) { // drawHandler
    char temp_char[4];
    b->draw("NOTE", itoa(data.lastPlayedNote, temp_char, 10), TouchButton::BUTTON_NORMAL);
  });
  toolsPages[TOOLS_PATTERN].push_back(lastNoteLabel);

  toolsPages[TOOLS_PATTERN].push_back(new ValueButtonVector<uint8_t>(&currentValue, GRID_X[2], GRID_Y[4], data.fillNotes.number, std::vector<uint8_t>({ 4, 6, 8, 12, 16, 24, 32 }), 16, 
  [ ] (auto *b, auto *v) { // drawHandler
    b->draw("NUM", v->toString(), TouchButton::BUTTON_HIGHLIGHTED);
  }));
  toolsPages[TOOLS_PATTERN].push_back(new ValueButtonRange<uint8_t>(&currentValue, GRID_X[3], GRID_Y[4], data.fillNotes.offset, 0, 7, 1, 0, 
  [ ] (auto *b, auto *v) { // drawHandler
    b->draw("OFF", v->toString(), TouchButton::BUTTON_HIGHLIGHTED);
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
  [ ](auto *b) { // drawHandler
    b->draw("MUTE", "QUANT", TouchButton::BUTTON_LABEL);
  }));
  toolsPages[TOOLS_SONG].push_back(new ValueButtonVector<uint8_t>(&currentValue, GRID_X[1], GRID_Y[3], data.songMuteQuantizeDenom, std::vector<uint8_t>({ 1, 2, 4, 8 }), 1,
  [ ] (auto *b, auto *v) { // drawHandler
    b->draw("QUANT", (v->getValue() == 1) ? "NONE" : v->toString(), (v->getValue() == 1) ? TouchButton::BUTTON_ACTIVE : TouchButton::BUTTON_HIGHLIGHTED);
  }));
  toolsPages[TOOLS_SONG].push_back(new TouchButton(GRID_X[0], GRID_Y[4],
  [ ](auto *b) { // drawHandler
    b->draw("SONG", "LAYERS", TouchButton::BUTTON_LABEL);
  }));
  toolsPages[TOOLS_SONG].push_back(new ValueButtonVector<uint8_t>(&currentValue, GRID_X[1], GRID_Y[4], songLayerMode, std::vector<uint8_t>({ uint8_t(LiveSequencer::LAYER_MUTE), uint8_t(LiveSequencer::LAYER_MERGE), uint8_t(LiveSequencer::LAYER_DELETE) }), uint8_t(LiveSequencer::LAYER_MUTE),
  [ this ](auto *b, auto *v) { // drawHandler
    std::string t1 = (data.songLayerCount == 0) ? "NO" : "LAYER";
    std::string t2 = (data.songLayerCount == 0) ? "LAYERS" : "ACTION";
    b->draw(t1, t2, (data.songLayerCount == 0) ? TouchButton::BUTTON_LABEL : TouchButton::BUTTON_ACTIVE);
    if(data.songLayerCount > 0) {
      guiUpdateFlags |= drawSongLayers;
    }
  }));

  // ARP TOOL
  toolsPages[TOOLS_ARP].push_back(new ValueButtonVector<uint8_t>(&currentValue, GRID_X[0], GRID_Y[3], data.arpSettings.amount, std::vector<uint8_t>({ 0, 2, 4, 6, 8, 12, 16, 24, 32, 64 }), 0,
  [ ] (auto *b, auto *v) { // drawHandler
    b->draw("SPEED", (v->getValue() == 0) ? "OFF" : v->toString(), (v->getValue() == 0) ? TouchButton::BUTTON_ACTIVE : TouchButton::BUTTON_HIGHLIGHTED);
  }));
  toolsPages[TOOLS_ARP].push_back(new ValueButtonVector<uint8_t>(&currentValue, GRID_X[1], GRID_Y[3], data.arpSettings.octaves, std::vector<uint8_t>({ 1, 2, 3 }), 1,
  [ ] (auto *b, auto *v) { // drawHandler
    b->draw("OCT", v->toString(), TouchButton::BUTTON_HIGHLIGHTED);
  }));
  toolsPages[TOOLS_ARP].push_back(new ValueButtonRange<uint8_t>(&currentValue, GRID_X[2], GRID_Y[3], (uint8_t&)data.arpSettings.mode, 0, uint8_t(LiveSequencer::ARP_MODENUM-1), 1, uint8_t(LiveSequencer::ARP_DOWN),
  [ ] (auto *b, auto *v) { // drawHandler
    b->draw("MODE", UI_LiveSequencer::getArpModeName(v->getValue()).c_str(), TouchButton::BUTTON_HIGHLIGHTED);
  }));
  toolsPages[TOOLS_ARP].push_back(new ValueButtonRange<uint16_t>(&currentValue, GRID_X[3], GRID_Y[3], data.arpSettings.length, 50, 500, 10, 150,
  [ ] (auto *b, auto *v) { // drawHandler
    b->draw("LEN", v->toString(), v->getValue() == 0 ? TouchButton::BUTTON_ACTIVE : TouchButton::BUTTON_HIGHLIGHTED);
  }));
  toolsPages[TOOLS_ARP].push_back(new ValueButtonRange<int8_t>(&currentValue, GRID_X[4], GRID_Y[3], data.arpSettings.swing, -8, 8, 1, 0,
  [ ] (auto *b, auto *v) { // drawHandler
    b->draw("SWING", v->toString(), TouchButton::BUTTON_HIGHLIGHTED);
  }));
  toolsPages[TOOLS_ARP].push_back(new ValueButtonRange<uint8_t>(&currentValue, GRID_X[5], GRID_Y[3], data.arpSettings.volume, 0, 127, 1, 127,
  [ ] (auto *b, auto *v) { // drawHandler
    b->draw("VELO", v->toString(), TouchButton::BUTTON_HIGHLIGHTED);
  }));
  toolsPages[TOOLS_ARP].push_back(new ValueButtonRange<uint8_t>(&currentValue, GRID_X[0], GRID_Y[4], data.arpSettings.latch, 0, 1, 1, 1,
  [ ] (auto *b, auto *v) { // drawHandler
    b->draw("LATCH", v->getValue() == 1 ? "ON" : "-", TouchButton::BUTTON_HIGHLIGHTED);
  }));
  toolsPages[TOOLS_ARP].push_back(new ValueButtonVector<uint8_t>(&currentValue, GRID_X[1], GRID_Y[4], data.arpSettings.loadPerBar, std::vector<uint8_t>({ 1, 2, 4 }), 2,
  [ ] (auto *b, auto *v) { // drawHandler
    b->draw("SAMPLE", std::string(v->toString()) + "x", TouchButton::BUTTON_HIGHLIGHTED);
  }));
  toolsPages[TOOLS_ARP].push_back(new ValueButtonRange<uint8_t>(&currentValue, GRID_X[2], GRID_Y[4], data.arpSettings.noteRepeat, 0, 4, 1, 0,
  [ ] (auto *b, auto *v) { // drawHandler
    b->draw("REPEAT", std::string(v->toString()) + "x", TouchButton::BUTTON_HIGHLIGHTED);
  }));
}

PROGMEM void UI_LiveSequencer::selectTools(Tools tools) {
  if(currentTools != tools) {
    if(currentValue.button != nullptr) {
      currentValue.button->setSelected(false);
    }
    currentValue.valueBase = nullptr;
    currentTools = tools;
    guiUpdateFlags |= clearBottomArea | drawTools;
  }
}

PROGMEM bool UI_LiveSequencer::isModeToolActive(void) {
  bool result = false;
  result |= (currentTools == TOOLS_PATTERN) && (data.isSongMode == false);
  result |= (currentTools == TOOLS_SONG) && (data.isSongMode == true);
  return result;
}

PROGMEM void UI_LiveSequencer::showDirectMappingWarning(uint8_t inChannel) {
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

PROGMEM void UI_LiveSequencer::processLCDM(void) {
// ****** SETUP *********
  if (LCDML.FUNC_setup()) {
    data.isActive = true;
    stayActive = false;
    display.fillScreen(COLOR_BACKGROUND);
    numberOfBarsTemp = data.numberOfBars;
    liveSeq.onGuiInit();

    barPhases[0] = 0;
    barPhases[1] = 0;
    guiUpdateFlags = 0;
    redrawScreen();
    drawGUI(guiUpdateFlags);
    // setup function
    LCDML.FUNC_setLoopInterval(40); // 25Hz gui refresh
  }
  // ****** LOOP *********
  if (LCDML.FUNC_loop()) {
    if(LCDML.BT_checkDown()) {
      if(currentValue.valueBase != nullptr) {
        currentValue.valueBase->next();
      }
    }
    if(LCDML.BT_checkUp()) {
      if(currentValue.valueBase != nullptr) {
        currentValue.valueBase->previous();
      }
    }
    if(LCDML.BT_checkEnter()) {
      DBG_LOG(printf("enter!\n"));
    }

    guiUpdateFlags |= data.trackLayersChanged ? (drawLayerButtons | drawTrackButtons) : 0;
    guiUpdateFlags |= (data.isRunning || data.stoppedFlag) ? (drawActiveNotes | drawTime) : 0;

    if((isLayerViewActive == false) && (currentTools == TOOLS_PATTERN) && data.lastPlayedNoteChanged) {
      lastNoteLabel->drawNow();
    }

    data.songLayersChanged = false;
    data.trackLayersChanged = false;
    data.lastPlayedNoteChanged = false;
    data.stoppedFlag = false;
    if (showingHowTo == false) {
      drawGUI(guiUpdateFlags);
    }
  }

  // ****** STABLE END *********
  if (LCDML.FUNC_close()) {
    if (stayActive == false) {
      data.isActive = false;
    }
    else {
      stayActive = false;
    }
    data.isRecording = false;
    showingHowTo = false;
    display.fillScreen(COLOR_BACKGROUND);
  }
}

PROGMEM void UI_LiveSequencer::redrawScreen(void) {
  guiUpdateFlags |= (clearBottomArea | drawTopButtons | drawTrackButtons | drawTime);
  isLayerViewActive = (showingTools == false);
  if(isLayerViewActive) {
    guiUpdateFlags |= drawLayerButtons;
  } else {
    guiUpdateFlags |= drawTools;
  }
}

PROGMEM void UI_LiveSequencer::handleTouchscreen(void) {
  bool pressedChanged = (numTouchPoints != numPressedOld);
  if (showingHowTo) {
    if (TouchButton::isPressed(GRID_X[5], GRID_Y[5])) {
      LCDML.FUNC_setGBAToLastFunc();
      LCDML.OTHER_jumpToFunc(UI_func_midi_channels);
    }
    return;
  }
  const bool runningChanged = (runningHere != data.isRunning);
  runningHere = data.isRunning;

  if (pressedChanged || runningChanged) {
    const bool runningPressed = TouchButton::isPressed(GRID_X[0], GRID_Y[0]);
    if (runningPressed) {
      if (runningHere) {
        handleStop();
      }
      else {
        handleStart();
      }
    }

    if (runningPressed || runningChanged) {
      guiUpdateFlags |= drawTopButtons;
      if (trackLayerMode != LiveSequencer::LayerMode::LAYER_MUTE) {
        trackLayerMode = LiveSequencer::LayerMode::LAYER_MUTE;
        guiUpdateFlags |= drawLayerButtons;
      }
    }

    const bool recPressed = TouchButton::isPressed(GRID_X[1], GRID_Y[0]);
    if (recPressed) {
      data.isRecording = !data.isRecording;
      guiUpdateFlags |= (drawTopButtons | drawTrackButtons);
      trackLayerMode = LiveSequencer::LayerMode::LAYER_MUTE;

      if (isLayerViewActive) {
        guiUpdateFlags |= drawLayerButtons;
      }
    }

    const bool modePressed = TouchButton::isPressed(GRID_X[5], GRID_Y[0]);
    if (modePressed) {
      const bool newIsSongMode = !data.isSongMode;
      if(showingTools == false) {
        currentPage = newIsSongMode ? PAGE_SONG : PAGE_PATTERN;
      }
      if (isModeToolActive()) { // switch pattern / song tools if selected
        currentTools = newIsSongMode ? TOOLS_SONG : TOOLS_PATTERN;
      }
      data.isSongMode = newIsSongMode;
      redrawScreen();
    }

    const bool funcPressed = TouchButton::isPressed(GRID_X[4], GRID_Y[0]);
    if (funcPressed) {
      // possible switch to song / pattern tools if mode changed
      currentTools = data.isSongMode && currentTools == TOOLS_PATTERN ? TOOLS_SONG : currentTools;
      currentTools = !data.isSongMode && currentTools == TOOLS_SONG ? TOOLS_PATTERN : currentTools;
      showingTools = !showingTools;
      redrawScreen();
    }

    for (int track = 0; track < LiveSequencer::LIVESEQUENCER_NUM_TRACKS; track++) {
      const bool pressed = TouchButton::isPressed(GRID_X[track], GRID_Y[1]);
      if (pressed) {
        guiUpdateFlags |= drawTrackButtons;

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
                guiUpdateFlags |= drawLayerButtons;
              }
            }
          }
          else {
            // open instrument settings
            if (data.tracks[track].screenSetupFn != nullptr) {
              SetupFn f = (SetupFn)data.tracks[track].screenSetupFn;
              f(0);
            }
            stayActive = true; // stay active for screens instrument settings opened in LiveSequencer
            LCDML.FUNC_setGBAToLastFunc();
            LCDML.OTHER_jumpToFunc(data.tracks[track].screen);
          }
        }
        else {
          data.activeTrack = track;
          trackLayerMode = LiveSequencer::LayerMode::LAYER_MUTE;
          if (currentPage == PAGE_PATTERN) {
            guiUpdateFlags |= drawLayerButtons;
          }
          DBG_LOG(printf("rec track now is %i\n", track + 1));
        }
      }
      if(isLayerViewActive) {
        for (int layer = 0; layer < data.trackSettings[track].layerCount; layer++) {
          const bool pressed = TouchButton::isPressed(GRID_X[track], GRID_Y[2 + layer]);
          if (pressed) {
            if (data.isRecording && (trackLayerMode != LiveSequencer::LayerMode::LAYER_MUTE) && (track == data.activeTrack)) {
              liveSeq.trackLayerAction(track, layer, LiveSequencer::LayerMode(trackLayerMode));
              // one less layer now
              TouchButton::clearButton(GRID_X[track], GRID_Y[2 + layer], COLOR_BACKGROUND);
              trackLayerMode = LiveSequencer::LayerMode::LAYER_MUTE;
            }
            else {
              const bool isMutedOld = data.tracks[track].layerMutes & (1 << layer);
              const bool recordMuteToSong = data.isSongMode && data.isRecording && data.isRunning;
              liveSeq.setLayerMuted(track, layer, !isMutedOld, recordMuteToSong);
            }
            guiUpdateFlags |= (drawLayerButtons);
          }
        }
      } else {
        // process TOOLS MENU if tools view active
        if(isLayerViewActive == false) {
          for(auto *b : buttonsToolSelect) {
            b->processPressed();
          }
          for(TouchButton *b : toolsPages[currentTools]) {
            b->processPressed();
          }

          // 
          if(currentTools == TOOLS_SONG) {
            if (songLayerMode != LiveSequencer::LayerMode::LAYER_MUTE) { // song layers can not be muted
              for (uint8_t songLayer = 0; songLayer < data.songLayerCount; songLayer++) {
                if (TouchButton::isPressed(GRID_X[2 + songLayer], GRID_Y[4])) {
                  liveSeq.songLayerAction(songLayer, LiveSequencer::LayerMode(songLayerMode));
                  songLayerMode = LiveSequencer::LayerMode::LAYER_MUTE;
                  TouchButton::clearButton(GRID_X[2 + songLayer], GRID_Y[4], GREY3); // FIXME clear last layer
                  guiUpdateFlags |= drawSongLayers;
                  break;
                }
              }
            }
          }
        }
      }
    }
    numPressedOld = numTouchPoints;
  }
}

PROGMEM void UI_LiveSequencer::drawGUI(uint16_t& guiFlags) {
  if (remote_active) {
    display.console = true;
  }
  if (guiFlags & drawTopButtons) {
    TouchButton::drawButton(GRID_X[0], GRID_Y[0], (runningHere ? "STOP" : "START"), "", runningHere ? TouchButton::BUTTON_RED : TouchButton::BUTTON_NORMAL);
    TouchButton::drawButton(GRID_X[1], GRID_Y[0], "REC", "", data.isRecording ? TouchButton::BUTTON_RED : TouchButton::BUTTON_NORMAL);
    TouchButton::drawButton(GRID_X[4], GRID_Y[0], isLayerViewActive ? "LAYERS" : "TOOLS", "VIEW", TouchButton::BUTTON_NORMAL);
    TouchButton::drawButton(GRID_X[5], GRID_Y[0], data.isSongMode ? "SONG" : "PATT", "MODE", TouchButton::BUTTON_HIGHLIGHTED);
  }

  uint16_t patCount = 0;
  uint16_t timeMs = 0;
  if (runningHere) {
    patCount = data.currentPattern;
    timeMs = data.patternTimer;

    if (data.patternBeginFlag) {
      data.patternBeginFlag = false;

      display.fillRect(110, 5, 90, 5, barPhases[0] ? GREEN : COLOR_BACKGROUND);
      barPhases[0] = !barPhases[0];
      if (data.currentPattern == 0) {
        display.fillRect(110, 10, 90, 5, barPhases[1] ? RED : COLOR_BACKGROUND);
        barPhases[1] = !barPhases[1];
      }
    }
    else {
      const float progressPattern = data.patternTimer / float(data.patternLengthMs);
      const float progressTotal = std::min(1.0f, (progressPattern + data.currentPattern) / float(data.numberOfBars));
      display.fillRect(110, 5, progressPattern * 90, 5, barPhases[0] ? GREEN : COLOR_BACKGROUND);
      display.fillRect(110, 10, progressTotal * 90, 5, barPhases[1] ? RED : COLOR_BACKGROUND);
    }
  }

  // print time
  if (guiFlags & drawTime) {
    display.setCursor(110, 20);
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
    display.setCursor(190, 20);
    if (data.isSongMode) {
      display.printf("%02i", data.songPatternCount);
    }
    else {
      display.printf("%02i", data.currentPattern);
    }
  }

  char temp_char[6];

  TouchButton::ButtonColor trackButtonRecColor = TouchButton::BUTTON_HIGHLIGHTED; // red, or blinking
  const bool doBlink = data.notesOn.size() || data.pendingEvents.size();
  if (doBlink) {
    if (++guiCounter == 8) {
      guiCounter = 0;
      trackButtonRecColor = blinkPhase ? TouchButton::BUTTON_RED : TouchButton::BUTTON_HIGHLIGHTED;
      guiFlags |= drawTrackButtons;
      blinkPhase = !blinkPhase;
    }
  }
  else {
    guiCounter = 0;
    blinkPhase = 0;
    trackButtonRecColor = TouchButton::BUTTON_RED;
  }

  if (guiFlags & clearBottomArea) {
    const uint16_t bgColor = isLayerViewActive ? COLOR_BACKGROUND : GREY3; // gray for tools
    display.console = true;
    display.fillRect(1, 78, DISPLAY_WIDTH - 2, DISPLAY_HEIGHT - 75, bgColor);
    display.drawRect(0, 77, DISPLAY_WIDTH, DISPLAY_HEIGHT - 77, isLayerViewActive ? bgColor : GREY2);

    DBG_LOG(printf("clear bottom\n"));
  }
  if (isLayerViewActive || (guiUpdateFlags & drawTrackButtons)) {
    const bool isSongRec = (data.isSongMode && data.isRecording);
    for (int track = 0; track < LiveSequencer::LIVESEQUENCER_NUM_TRACKS; track++) {
      if (guiFlags & drawTrackButtons) {
        TouchButton::drawButton(GRID_X[track], GRID_Y[1], data.tracks[track].name, itoa(track + 1, temp_char, 10), (track == data.activeTrack) ? (data.isRecording ? trackButtonRecColor : TouchButton::BUTTON_HIGHLIGHTED) : TouchButton::BUTTON_ACTIVE);
      }
      if (isLayerViewActive) {
        const bool layerEditActive = !data.isSongMode && (data.activeTrack == track) && (trackLayerMode != LiveSequencer::LayerMode::LAYER_MUTE);
        // layer button
        for (int layer = 0; layer < LiveSequencer::LIVESEQUENCER_NUM_TRACKS; layer++) {
          if (layer < data.trackSettings[track].layerCount) {
            const bool isMuted = data.tracks[track].layerMutes & (1 << layer);
            TouchButton::ButtonColor color = (isMuted ? TouchButton::BUTTON_NORMAL : (isSongRec ? TouchButton::BUTTON_RED : TouchButton::BUTTON_ACTIVE));
            if (layerEditActive) {
              // adapt button background if in layer edit mode
              handleLayerEditButtonColor(trackLayerMode, color);
            }
            if (guiFlags & drawLayerButtons) {
              drawLayerButton(data.isSongMode, trackLayerMode, layer, layerEditActive, color, GRID_X[track], GRID_Y[2 + layer]);
            }
            if (guiFlags & drawActiveNotes) {
              // always draw notes when layers visible
              const uint16_t barHeight = 5 * data.tracks[track].activeNotes[layer].size();
              const uint16_t xStart = GRID_X[track] + TouchButton::BUTTON_SIZE_X - 3;
              const uint16_t yStart = GRID_Y[2 + layer];
              
              const uint16_t yFill = std::min(barHeight, TouchButton::BUTTON_SIZE_Y);
              display.console = true;
              display.fillRect(xStart, yStart, 3, TouchButton::BUTTON_SIZE_Y - yFill, TouchButton::getColors(color).bg);
              display.fillRect(xStart, yStart + (TouchButton::BUTTON_SIZE_Y - yFill), 3, yFill, COLOR_SYSTEXT);
            }
          }
        }
      }
    }
  }
  if (isLayerViewActive == false) {
    if (guiFlags & drawTools) {
      for(auto *b : buttonsToolSelect) {
        b->drawNow();
      }

      for(TouchButton *b : toolsPages[currentTools]) {
        b->drawNow();
      }

      //
      if(guiFlags & drawSongLayers) {
        TouchButton::ButtonColor color = TouchButton::BUTTON_NORMAL;
        handleLayerEditButtonColor(songLayerMode, color);
        for (int songLayer = 0; songLayer < LiveSequencer::LIVESEQUENCER_NUM_TRACKS; songLayer++) {
          if (songLayer < data.songLayerCount) {
            drawLayerButton(data.isSongMode, songLayerMode, songLayer, true, color, GRID_X[2 + songLayer], GRID_Y[4]);
          }
        }
      }
    }
  }

  guiFlags = 0;
}

PROGMEM std::string UI_LiveSequencer::getArpModeName(uint8_t mode) {
  switch (mode) {
  case LiveSequencer::ArpMode::ARP_CHORD:
    return "CHRD";
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

PROGMEM void UI_LiveSequencer::drawLayerButton(const bool horizontal, uint8_t layerMode, int layer, const bool layerEditActive, TouchButton::ButtonColor color, uint16_t x, uint16_t y) {
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

PROGMEM void UI_LiveSequencer::handleLayerEditButtonColor(uint8_t layerMode, TouchButton::ButtonColor &color) {
  switch (layerMode) {
  case LiveSequencer::LayerMode::LAYER_MERGE:
    color = TouchButton::BUTTON_HIGHLIGHTED;
    break;

  case LiveSequencer::LayerMode::LAYER_DELETE:
    color = TouchButton::BUTTON_RED;
    break;
  }
}
