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

static constexpr int BUTTON_HEIGHT = CHAR_height_small * button_size_y;
static constexpr int BUTTON_WIDTH = CHAR_width_small * button_size_x;

UI_LiveSequencer* instance;
LiveSequencer::LiveSeqData *data;

UI_LiveSequencer::UI_LiveSequencer(LiveSequencer* sequencer) : liveSeqPtr(sequencer) {
  instance = this;
  data = sequencer->getData();
  static constexpr uint8_t BUTTON_OFFSET_X = 4; // center in screen
  static constexpr uint8_t BUTTON_OFFSET_Y = 2; // center in screen

  for(int i = 0; i < 6; i++) {
    GRID_X[i] = BUTTON_OFFSET_X + (i * 54);
    GRID_Y[i] = BUTTON_OFFSET_Y + (i * 40);
  }
  buttonsSongTools.push_back(new TouchButton(GRID_X[0], GRID_Y[3],
  [ ](auto *b) { // drawHandler
    b->draw("TOOL", (data->isSongMode) ? "MUTE": "QUANT", instance->isModeToolActive() ? TouchButton::BUTTON_HIGHLIGHTED : TouchButton::BUTTON_NORMAL);
  }));
  buttonsSongTools.push_back(new ValueButtonVector<uint8_t>(&currentValue, GRID_X[1], GRID_Y[3], data->songMuteQuantizeDenom, std::vector<uint8_t>({ 1, 2, 4, 8 }), 1,
  [ ] (auto *b, auto *v) { // drawHandler
    b->draw("QUANT", (v->getValue() == 1) ? "NONE" : v->toString(), (v->getValue() == 1) ? TouchButton::BUTTON_ACTIVE : TouchButton::BUTTON_HIGHLIGHTED);
  }));

  // TOOL MENU
  buttonsToolSelect.push_back(new TouchButton(GRID_X[0], GRID_Y[2],
  [ this ](auto *b) { // drawHandler
    b->draw("TOOL", (data->isSongMode) ? "SNG": "PAT", instance->isModeToolActive() ? TouchButton::BUTTON_HIGHLIGHTED : TouchButton::BUTTON_NORMAL);
  },
  [ this ](auto *b) { // clickedHandler
    data->currentTools = (data->isSongMode) ? TOOLS_SONG : TOOLS_PATTERN;
    guiUpdateFlags |= clearBottomArea | drawTools;
  }));
  buttonsToolSelect.push_back(new TouchButton(GRID_X[1], GRID_Y[2],
  [ ](auto *b) { // drawHandler
    b->draw("TOOL", "ARP", data->currentTools == TOOLS_ARP ? TouchButton::BUTTON_HIGHLIGHTED : TouchButton::BUTTON_NORMAL);
  },
  [ this ](auto *b) { // clickedHandler
    data->currentTools = TOOLS_ARP;
    guiUpdateFlags |= clearBottomArea | drawTools;
  }));
  buttonsToolSelect.push_back(new TouchButton(GRID_X[2], GRID_Y[2],
  [ ](auto *b) { // drawHandler
    b->draw("TOOL", "SEQ", data->currentTools == TOOLS_SEQ ? TouchButton::BUTTON_HIGHLIGHTED : TouchButton::BUTTON_NORMAL);
  },
  [ this ](auto *b) { // clickedHandler
    data->currentTools = TOOLS_SEQ;
    guiUpdateFlags |= clearBottomArea | drawTools;
  }));

  // SEQUENCER TOOLS
  TouchButton *applyPatternLength = new TouchButton(GRID_X[2], GRID_Y[3],
  [ this ] (auto *b) { // drawHandler
    const bool isSame = (data->numberOfBars == numberOfBarsTemp);
    b->draw("APPLY", "NOW", isSame ? TouchButton::BUTTON_ACTIVE : TouchButton::BUTTON_RED);
    display.setTextSize(1);
    display.setTextColor(isSame ? GREY2 : RED, GREY3);
    display.setCursor(GRID_X[3], GRID_Y[3] + 5);
    display.printf("CHANGING PATTERN LENGTH");
    display.setCursor(GRID_X[3], GRID_Y[3] + 20);
    display.printf("WILL DELETE ALL DATA!");
  },
  [ this ](auto *b){ // clickedHandler
    if(data->numberOfBars != numberOfBarsTemp) {
      handleStop();
      liveSeqPtr->changeNumberOfBars(numberOfBarsTemp);
      b->drawNow();
    }
  });
  buttonsSeqTools.push_back(applyPatternLength);

  buttonsSeqTools.push_back(new TouchButton(GRID_X[0], GRID_Y[3],
  [ ] (auto *b) { // drawHandler
    b->draw("PATTERN", "LENGTH", TouchButton::BUTTON_LABEL);
  }));
  buttonsSeqTools.push_back(new ValueButtonVector<uint8_t>(&currentValue, GRID_X[1], GRID_Y[3], numberOfBarsTemp, std::vector<uint8_t>({ 1, 2, 4, 8 }), 4, 
  [ applyPatternLength ] (auto *b, auto *v) { // drawHandler
    b->draw("LENGTH", v->toString(), TouchButton::BUTTON_ACTIVE);
    applyPatternLength->drawNow();
  }));

  buttonsSeqTools.push_back(new TouchButton(GRID_X[0], GRID_Y[5],
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
      if (data->isSongMode) {
        liveSeqPtr->deleteAllSongEvents();
      }
      else {
        liveSeqPtr->deleteLiveSequencerData();
      }
      b->draw("DELETE", "OK", TouchButton::BUTTON_LABEL);
    }
  });
  buttonsSeqTools.push_back(new TouchButton(GRID_X[1], GRID_Y[5],
  [ this ] (auto *b) { // drawHandler
    b->draw("DELETE", data->isSongMode ? "SONG" : "ALL", TouchButton::BUTTON_NORMAL);
  },
  [ this, confirmDelete ] (auto *b) { // clickedHandler
    deleteConfirming = !deleteConfirming;
    confirmDelete->drawNow();
  }));
  buttonsSeqTools.push_back(new TouchButton(GRID_X[4], GRID_Y[5],
  [ ] (auto *b) { // drawHandler
    b->draw("LOAD", "PERF", TouchButton::BUTTON_NORMAL);
  },
  [ this ] (auto *b) { // clickedHandler
    // load
    LCDML.FUNC_setGBAToLastFunc();
    display.setTextSize(2);
    display.setTextColor(COLOR_SYSTEXT, COLOR_BACKGROUND);
    DBG_LOG(printf("perf id %i\n", data->performanceID));
    LCDML.OTHER_jumpToFunc(UI_func_load_performance, data->performanceID);
  }));
  buttonsSeqTools.push_back(new TouchButton(GRID_X[5], GRID_Y[5],
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
    buttonsPattTools.push_back(new ValueButtonVector<uint8_t>(&currentValue, GRID_X[track], GRID_Y[3], data->trackSettings[track].quantizeDenom, std::vector<uint8_t>({ 1, 2, 4, 8, 16, 32 }), 4,
    [ ] (auto *b, auto *v) { // drawHandler
      b->draw("QUANT", (v->getValue() == 1) ? "NONE" : v->toString(), (v->getValue() == 1) ? TouchButton::BUTTON_ACTIVE : TouchButton::BUTTON_HIGHLIGHTED);
    }));
  }
  buttonsPattTools.push_back(new TouchButton(GRID_X[0], GRID_Y[4],
  [ ] (auto *b) { // drawHandler
    b->draw("FILL", "NOTES", TouchButton::BUTTON_LABEL);
  }));
  lastNoteLabel = new TouchButton(GRID_X[1], GRID_Y[4],
  [ this ] (auto *b) { // drawHandler
    char temp_char[4];
    b->draw("NOTE", itoa(data->lastPlayedNote, temp_char, 10), TouchButton::BUTTON_NORMAL);
  });
  buttonsPattTools.push_back(lastNoteLabel);

  buttonsPattTools.push_back(new ValueButtonVector<uint8_t>(&currentValue, GRID_X[2], GRID_Y[4], data->fillNotes.number, std::vector<uint8_t>({ 4, 6, 8, 12, 16, 24, 32 }), 16, 
  [ ] (auto *b, auto *v) { // drawHandler
    b->draw("NUM", v->toString(), TouchButton::BUTTON_HIGHLIGHTED);
  }));
  buttonsPattTools.push_back(new ValueButtonRange<uint8_t>(&currentValue, GRID_X[3], GRID_Y[4], data->fillNotes.offset, 0, 7, 1, 0, 
  [ ] (auto *b, auto *v) { // drawHandler
    b->draw("OFF", v->toString(), TouchButton::BUTTON_HIGHLIGHTED);
  }));
  buttonsPattTools.push_back(new TouchButton(GRID_X[5], GRID_Y[4],
  [ ](auto *b) { // drawHandler
    b->draw("FILL", "NOW", TouchButton::BUTTON_RED);
  },
  [ this ](auto *b) { // clickedHandler
    liveSeqPtr->fillTrackLayer();
  }));

  // ARP TOOL
  buttonsArpTools.push_back(new ValueButtonVector<uint8_t>(&currentValue, GRID_X[0], GRID_Y[3], data->arpSettings.amount, std::vector<uint8_t>({ 0, 2, 4, 6, 8, 12, 16, 24, 32, 64 }), 0,
  [ this ] (auto *b, auto *v) { // drawHandler
    b->draw("SPEED", (v->getValue() == 0) ? "OFF" : v->toString(), (v->getValue() == 0) ? TouchButton::BUTTON_ACTIVE : TouchButton::BUTTON_HIGHLIGHTED);
  }));
  buttonsArpTools.push_back(new ValueButtonVector<uint8_t>(&currentValue, GRID_X[1], GRID_Y[3], data->arpSettings.octaves, std::vector<uint8_t>({ 1, 2, 3 }), 1,
  [ this ] (auto *b, auto *v) { // drawHandler
    b->draw("OCT", v->toString(), TouchButton::BUTTON_HIGHLIGHTED);
  }));
  buttonsArpTools.push_back(new ValueButtonRange<uint8_t>(&currentValue, GRID_X[2], GRID_Y[3], (uint8_t&)data->arpSettings.mode, 0, uint8_t(LiveSequencer::ARP_MODENUM-1), 1, uint8_t(LiveSequencer::ARP_DOWN),
  [ this ] (auto *b, auto *v) { // drawHandler
    b->draw("MODE", UI_LiveSequencer::getArpModeName(v->getValue()).c_str(), TouchButton::BUTTON_HIGHLIGHTED);
  }));
  buttonsArpTools.push_back(new ValueButtonRange<uint16_t>(&currentValue, GRID_X[3], GRID_Y[3], data->arpSettings.length, 50, 500, 10, 150,
  [ this ] (auto *b, auto *v) { // drawHandler
    b->draw("LEN", v->toString(), v->getValue() == 0 ? TouchButton::BUTTON_ACTIVE : TouchButton::BUTTON_HIGHLIGHTED);
  }));
  buttonsArpTools.push_back(new ValueButtonRange<int8_t>(&currentValue, GRID_X[4], GRID_Y[3], data->arpSettings.swing, -8, 8, 1, 0,
  [ this ] (auto *b, auto *v) { // drawHandler
    b->draw("SWING", v->toString(), TouchButton::BUTTON_HIGHLIGHTED);
  }));
  buttonsArpTools.push_back(new ValueButtonRange<uint8_t>(&currentValue, GRID_X[5], GRID_Y[3], data->arpSettings.latch, 0, 1, 1, 1,
  [ this ] (auto *b, auto *v) { // drawHandler
    b->draw("LATCH", v->getValue() == 1 ? "ON" : "-", TouchButton::BUTTON_HIGHLIGHTED);
  }));
  buttonsArpTools.push_back(new ValueButtonRange<uint8_t>(&currentValue, GRID_X[0], GRID_Y[4], data->arpSettings.loadPerBar, 1, 2, 4, 2,
  [ this ] (auto *b, auto *v) { // drawHandler
    b->draw("SAMPLE", std::string(v->toString()) + "x", TouchButton::BUTTON_HIGHLIGHTED);
  }));
  buttonsArpTools.push_back(new ValueButtonRange<uint8_t>(&currentValue, GRID_X[1], GRID_Y[4], data->arpSettings.noteRepeat, 0, 4, 1, 0,
  [ this ] (auto *b, auto *v) { // drawHandler
    b->draw("REPEAT", std::string(v->toString()) + "x", TouchButton::BUTTON_HIGHLIGHTED);
  }));
}

bool UI_LiveSequencer::isModeToolActive(void) {
  bool result = false;
  result |= (data->currentTools == TOOLS_PATTERN) && (data->isSongMode == false);
  result |= (data->currentTools == TOOLS_SONG) && (data->isSongMode == true);

  return result;
}

void UI_func_livesequencer(uint8_t param) {
  instance->processLCDM();
}

void handle_touchscreen_live_sequencer(void) {
  instance->handleTouchscreen();
}

void UI_LiveSequencer::showDirectMappingWarning(uint8_t inChannel) {
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

void UI_LiveSequencer::processLCDM(void) {
// ****** SETUP *********
  if (LCDML.FUNC_setup()) {
    data->isActive = true;
    stayActive = false;
    display.fillScreen(COLOR_BACKGROUND);
    numberOfBarsTemp = data->numberOfBars;
    liveSeqPtr->onGuiInit();

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

    guiUpdateFlags |= data->trackLayersChanged ? (drawLayerButtons | drawTrackButtons) : 0;
    guiUpdateFlags |= (data->isRunning || data->stoppedFlag) ? (drawActiveNotes | drawTime) : 0;

    if((data->currentTools == TOOLS_PATTERN) && data->lastPlayedNoteChanged) {
      lastNoteLabel->drawNow();
    }

    data->songLayersChanged = false;
    data->trackLayersChanged = false;
    data->lastPlayedNoteChanged = false;
    data->stoppedFlag = false;
    if (showingHowTo == false) {
      drawGUI(guiUpdateFlags);
    }
  }

  // ****** STABLE END *********
  if (LCDML.FUNC_close()) {
    if (stayActive == false) {
      data->isActive = false;
    }
    else {
      stayActive = false;
    }
    data->isRecording = false;
    showingHowTo = false;
    display.fillScreen(COLOR_BACKGROUND);
  }
}

void UI_LiveSequencer::redrawScreen(void) {
  guiUpdateFlags |= (clearBottomArea | drawTopButtons | drawTrackButtons | drawTime);
  isLayerViewActive = data->currentTools == TOOLS_NONE;
  if(isLayerViewActive) {
    guiUpdateFlags |= drawLayerButtons;
  } else {
    guiUpdateFlags |= drawTools;
  }
}

void UI_LiveSequencer::handleTouchscreen(void) {
  bool pressedChanged = (numTouchPoints != numPressedOld);
  if (showingHowTo) {
    if (TouchButton::isPressed(GRID_X[5], GRID_Y[5])) {
      LCDML.FUNC_setGBAToLastFunc();
      LCDML.OTHER_jumpToFunc(UI_func_midi_channels);
    }
    return;
  }
  const bool runningChanged = (runningHere != data->isRunning);
  runningHere = data->isRunning;

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
      if (trackLayerMode != LayerMode::LAYER_MUTE) {
        trackLayerMode = LayerMode::LAYER_MUTE;
        guiUpdateFlags |= drawLayerButtons;
      }
    }

    const bool recPressed = TouchButton::isPressed(GRID_X[1], GRID_Y[0]);
    if (recPressed) {
      data->isRecording = !data->isRecording;
      guiUpdateFlags |= (drawTopButtons | drawTrackButtons);
      trackLayerMode = LayerMode::LAYER_MUTE;

      if (isLayerViewActive) {
        guiUpdateFlags |= drawLayerButtons;
      }
    }

    const bool modePressed = TouchButton::isPressed(GRID_X[5], GRID_Y[0]);
    if (modePressed) {
      const bool newIsSongMode = !data->isSongMode;
      if(data->currentTools == TOOLS_NONE) {
        data->currentPage = newIsSongMode ? PAGE_SONG : PAGE_PATTERN;
      } else if (isModeToolActive()) {
        data->currentTools = newIsSongMode ? TOOLS_SONG : TOOLS_PATTERN;
      }
      data->isSongMode = newIsSongMode;
      
      redrawScreen();
    }

    const bool funcPressed = TouchButton::isPressed(GRID_X[4], GRID_Y[0]);
    if (funcPressed) {
      if(isLayerViewActive) {
        data->currentTools = data->isSongMode ? TOOLS_SONG : TOOLS_PATTERN;
      } else {
        data->currentTools = TOOLS_NONE;
      }
      redrawScreen();
    }

    for (int track = 0; track < LiveSequencer::LIVESEQUENCER_NUM_TRACKS; track++) {
      const bool pressed = TouchButton::isPressed(GRID_X[track], GRID_Y[1]);
      if (pressed) {
        guiUpdateFlags |= drawTrackButtons;

        if (track == data->activeTrack) {
          if (data->isRecording) {
            if (data->pendingEvents.size()) {
              data->pendingEvents.clear(); // clear pending
            }
            else {
              // track layer actions only for pattern mode
              if (data->currentPage == PAGE_PATTERN) {
                if (++trackLayerMode == LayerMode::LAYER_MODE_NUM) {
                  trackLayerMode = LayerMode::LAYER_MUTE;
                }
                guiUpdateFlags |= drawLayerButtons;
              }
            }
          }
          else {
            // open instrument settings
            if (data->tracks[track].screenSetupFn != nullptr) {
              SetupFn f = (SetupFn)data->tracks[track].screenSetupFn;
              f(0);
            }
            stayActive = true; // stay active for screens instrument settings opened in LiveSequencer
            LCDML.FUNC_setGBAToLastFunc();
            LCDML.OTHER_jumpToFunc(data->tracks[track].screen);
          }
        }
        else {
          data->activeTrack = track;
          trackLayerMode = LayerMode::LAYER_MUTE;
          if (data->currentPage == PAGE_PATTERN) {
            guiUpdateFlags |= drawLayerButtons;
          }
          DBG_LOG(printf("rec track now is %i\n", track + 1));
        }
      }
      if(isLayerViewActive) {
        for (int layer = 0; layer < data->trackSettings[track].layerCount; layer++) {
          const bool pressed = TouchButton::isPressed(GRID_X[track], GRID_Y[2 + layer]);
          if (pressed) {
            if (data->isRecording && (trackLayerMode != LayerMode::LAYER_MUTE) && (track == data->activeTrack)) {
              liveSeqPtr->trackLayerAction(track, layer, LayerMode(trackLayerMode));
              // one less layer now
              TouchButton::clearButton(GRID_X[track], GRID_Y[2 + layer], COLOR_BACKGROUND);
              trackLayerMode = LayerMode::LAYER_MUTE;
            }
            else {
              const bool isMutedOld = data->tracks[track].layerMutes & (1 << layer);
              const bool recordMuteToSong = data->isSongMode && data->isRecording && data->isRunning;
              liveSeqPtr->setLayerMuted(track, layer, !isMutedOld, recordMuteToSong);
            }
            guiUpdateFlags |= (drawLayerButtons);
          }
        }
      } else {
        // process TOOLS MENU
        for(auto *b : buttonsToolSelect) {
          b->processPressed();
        }

        // process active tool
        switch(data->currentTools) {
        case TOOLS_PATTERN:
          for(auto *b : buttonsPattTools) {
            b->processPressed();
          }
          break;

        case TOOLS_ARP:
          for(auto *b : buttonsArpTools) {
            b->processPressed();
          }
          break;

        case TOOLS_SONG:
          if (TouchButton::isPressed(GRID_X[1], GRID_Y[3])) {
            // song layer mode
            if (++songLayerMode == LayerMode::LAYER_MODE_NUM) {
              songLayerMode = LayerMode::LAYER_MUTE;
            }
          }
          if (songLayerMode != LayerMode::LAYER_MUTE) { // song layers can not be muted
            for (uint8_t songLayer = 0; songLayer < data->songLayerCount; songLayer++) {
              if (TouchButton::isPressed(GRID_X[2 + songLayer], GRID_Y[3])) {
                liveSeqPtr->songLayerAction(songLayer, LayerMode(songLayerMode));
                songLayerMode = LayerMode::LAYER_MUTE;
              }
            }
          }
          // song mute quantize denom
          for(auto *b : buttonsSongTools) {
            b->processPressed();
          }
          break;

        case TOOLS_SEQ:
          for(auto *b : buttonsSeqTools) {
            b->processPressed();
          }
        }
      }
    }
    numPressedOld = numTouchPoints;
  }
}

void UI_LiveSequencer::drawGUI(uint16_t& guiFlags) {
  if (remote_active) {
    display.console = true;
  }
  if (guiFlags & drawTopButtons) {
    TouchButton::drawButton(GRID_X[0], GRID_Y[0], (runningHere ? "STOP" : "START"), "", runningHere ? TouchButton::BUTTON_RED : TouchButton::BUTTON_NORMAL);
    TouchButton::drawButton(GRID_X[1], GRID_Y[0], "REC", "", data->isRecording ? TouchButton::BUTTON_RED : TouchButton::BUTTON_NORMAL);
    TouchButton::drawButton(GRID_X[4], GRID_Y[0], isLayerViewActive ? "LAYERS" : "TOOLS", "VIEW", TouchButton::BUTTON_NORMAL);
    TouchButton::drawButton(GRID_X[5], GRID_Y[0], data->isSongMode ? "SONG" : "PATT", "MODE", TouchButton::BUTTON_HIGHLIGHTED);
  }

  uint16_t patCount = 0;
  uint16_t timeMs = 0;
  if (runningHere) {
    patCount = data->currentPattern;
    timeMs = data->patternTimer;

    if (data->patternBeginFlag) {
      data->patternBeginFlag = false;

      display.fillRect(110, 5, 90, 5, barPhases[0] ? GREEN : COLOR_BACKGROUND);
      barPhases[0] = !barPhases[0];
      if (data->currentPattern == 0) {
        display.fillRect(110, 10, 90, 5, barPhases[1] ? RED : COLOR_BACKGROUND);
        barPhases[1] = !barPhases[1];
      }
    }
    else {
      const float progressPattern = data->patternTimer / float(data->patternLengthMs);
      const float progressTotal = std::min(1.0f, (progressPattern + data->currentPattern) / float(data->numberOfBars));
      display.fillRect(110, 5, progressPattern * 90, 5, barPhases[0] ? GREEN : COLOR_BACKGROUND);
      display.fillRect(110, 10, progressTotal * 90, 5, barPhases[1] ? RED : COLOR_BACKGROUND);
    }
  }

  // print time
  if (guiFlags & drawTime) {
    display.setCursor(110, 20);
    display.setTextSize(1);
    display.setTextColor(COLOR_SYSTEXT, COLOR_BACKGROUND);
    if (data->isSongMode) {
      const uint32_t songMs = (data->songPatternCount * data->numberOfBars + data->currentPattern) * data->patternLengthMs + timeMs;
      const uint32_t minutes = songMs / 60000;
      const uint32_t seconds = (songMs % 60000) / 1000;
      const uint32_t millis = songMs % 1000;
      display.printf("S %i:%02i.%03i", minutes, seconds, millis);
    }
    else {
      display.printf("P %i.%04i   ", patCount, timeMs);
    }
    display.setCursor(190, 20);
    if (data->isSongMode) {
      display.printf("%02i", data->songPatternCount);
    }
    else {
      display.printf("%02i", data->currentPattern);
    }
  }

  char temp_char[6];

  TouchButton::ButtonColor trackButtonRecColor = TouchButton::BUTTON_HIGHLIGHTED; // red, or blinking
  const bool doBlink = data->notesOn.size() || data->pendingEvents.size();
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
    display.fillRect(0, 76, DISPLAY_WIDTH, DISPLAY_HEIGHT - 75, bgColor);
    DBG_LOG(printf("clear bottom\n"));
  }
  if (isLayerViewActive || (guiUpdateFlags & drawTrackButtons)) {
    const bool isSongRec = (data->isSongMode && data->isRecording);
    for (int track = 0; track < LiveSequencer::LIVESEQUENCER_NUM_TRACKS; track++) {
      if (guiFlags & drawTrackButtons) {
        TouchButton::drawButton(GRID_X[track], GRID_Y[1], data->tracks[track].name, itoa(track + 1, temp_char, 10), (track == data->activeTrack) ? (data->isRecording ? trackButtonRecColor : TouchButton::BUTTON_HIGHLIGHTED) : TouchButton::BUTTON_ACTIVE);
      }
      if (isLayerViewActive) {
        const bool layerEditActive = !data->isSongMode && (data->activeTrack == track) && (trackLayerMode != LayerMode::LAYER_MUTE);
        // layer button
        for (int layer = 0; layer < LiveSequencer::LIVESEQUENCER_NUM_TRACKS; layer++) {
          if (layer < data->trackSettings[track].layerCount) {
            const bool isMuted = data->tracks[track].layerMutes & (1 << layer);
            TouchButton::ButtonColor color = (isMuted ? TouchButton::BUTTON_NORMAL : (isSongRec ? TouchButton::BUTTON_RED : TouchButton::BUTTON_ACTIVE));
            if (layerEditActive) {
              // adapt button background if in layer edit mode
              handleLayerEditButtonColor(trackLayerMode, color);
            }
            if (guiFlags & drawLayerButtons) {
              drawLayerButton(data->isSongMode, trackLayerMode, layer, layerEditActive, color, GRID_X[track], GRID_Y[2 + layer]);
            }
            if (guiFlags & drawActiveNotes) {
              // always draw notes when layers visible
              const int numNotesOn = data->tracks[track].activeNotes[layer].size();
              const int xStart = GRID_X[track] + BUTTON_WIDTH - 3;
              const int yStart = GRID_Y[2 + layer];
              const int yFill = std::min(numNotesOn * 5, BUTTON_HEIGHT);
              display.console = true;
              display.fillRect(xStart, yStart, 3, BUTTON_HEIGHT - yFill, TouchButton::getColors(color).bg);
              display.fillRect(xStart, yStart + (BUTTON_HEIGHT - yFill), 3, yFill, COLOR_SYSTEXT);
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
      switch (data->currentTools) {
      case TOOLS_ARP:
        for(auto *b : buttonsArpTools) {
          b->drawNow();
        }
        break;

        case TOOLS_PATTERN:
        for(auto *b : buttonsPattTools) {
          b->drawNow();
        }
        break;

      case TOOLS_SEQ:
        for(auto *b : buttonsSeqTools) {
          b->drawNow();
        }

        break;

      case TOOLS_SONG:
        TouchButton::drawButton(GRID_X[0], GRID_Y[4], "SONG", "LAYERS", TouchButton::BUTTON_LABEL);
        if (data->songLayerCount > 0) {
          TouchButton::drawButton(GRID_X[1], GRID_Y[4], "LAYER", "ACTION", TouchButton::BUTTON_HIGHLIGHTED); // switch modes
        }
        else {
          TouchButton::drawButton(GRID_X[1], GRID_Y[4], "NO", "LAYERS", TouchButton::BUTTON_NORMAL); // switch modes
        }
        TouchButton::ButtonColor color = TouchButton::BUTTON_NORMAL;
        handleLayerEditButtonColor(songLayerMode, color);
        for (int songLayer = 0; songLayer < LiveSequencer::LIVESEQUENCER_NUM_TRACKS; songLayer++) {
          if (songLayer < data->songLayerCount) {
            drawLayerButton(data->isSongMode, songLayerMode, songLayer, true, color, GRID_X[2 + songLayer], GRID_Y[3]);
          }
        }
        for(auto *b : buttonsSongTools) {
          b->drawNow();
        }
        break;
      }
    }
  }

  guiFlags = 0;
}

std::string UI_LiveSequencer::getArpModeName(uint8_t mode) {
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

void UI_LiveSequencer::drawLayerButton(const bool horizontal, uint8_t layerMode, int layer, const bool layerEditActive, TouchButton::ButtonColor color, uint16_t x, uint16_t y) {
  char temp_char[4];
  std::string label = "LAYER";
  std::string labelSub = itoa(layer + 1, temp_char, 10);
  if (layerEditActive) {
    switch (layerMode) {
    case LayerMode::LAYER_MERGE:
      if (layer > 0) {
        label = "MERGE";
        labelSub = horizontal ? "<" : "^";
      }
      break;
    case LayerMode::LAYER_DELETE:
      label = "DELETE";
      labelSub = "x";
      break;
    }
  }
  TouchButton::drawButton(x, y, label.c_str(), labelSub.c_str(), color);
}

void UI_LiveSequencer::handleLayerEditButtonColor(uint8_t layerMode, TouchButton::ButtonColor &color) {
  switch (layerMode) {
  case LayerMode::LAYER_MERGE:
    color = TouchButton::BUTTON_HIGHLIGHTED;
    break;

  case LayerMode::LAYER_DELETE:
    color = TouchButton::BUTTON_RED;
    break;
  }
}
