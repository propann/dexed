#include "ui_livesequencer.h"
#include "LCDMenuLib2.h"
#include "ILI9341_t3n.h"
#include "sequencer.h"
#include "livesequencer.h"

extern LCDMenuLib2 LCDML;
extern ILI9341_t3n display;
extern bool remote_active;
extern int numTouchPoints;

extern void draw_button_on_grid(uint8_t x, uint8_t y, const char* t1, const char* t2, uint8_t color);
extern bool check_button_on_grid(uint8_t x, uint8_t y);
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
static constexpr uint8_t BUTTON_OFFSET_X = 0; // not possible to distribute better with button grid
static constexpr uint8_t BUTTON_COLUMNS_X[LiveSequencer::LIVESEQUENCER_NUM_TRACKS] = { BUTTON_OFFSET_X, BUTTON_OFFSET_X + 9, BUTTON_OFFSET_X + 18, BUTTON_OFFSET_X + 27, BUTTON_OFFSET_X + 36, BUTTON_OFFSET_X + 45 };

UI_LiveSequencer* instance;
LiveSequencer::LiveSeqData *data;

UI_LiveSequencer::UI_LiveSequencer(LiveSequencer* sequencer) : liveSeqPtr(sequencer) {
  instance = this;
  data = sequencer->getData();

  songMuteQuant = new EditableValue<uint8_t>(data->songMuteQuantizeDenom, std::vector<uint8_t>({ 1, 2, 4, 8 }), 1);
  applyPatternLength = new TouchButton(BUTTON_COLUMNS_X[2], 20,
  [ this ] (auto *b) { // drawHandler
    const bool isSame = (data->numberOfBars == numberOfBarsTemp);
    b->draw("APPLY", "NOW", isSame ? 1 : 2);
    display.setTextSize(1);
    display.setTextColor(isSame ? GREY2 : RED, GREY3);
    display.setCursor(165, 165);
    display.printf("CHANGING PATTERN LENGTH");
    display.setCursor(165, 175);
    display.printf("WILL DELETE ALL DATA!");
  },
  [ this ](){ // clickedHandler
    if(data->numberOfBars != numberOfBarsTemp) {
      handleStop();
      liveSeqPtr->changeNumberOfBars(numberOfBarsTemp);
    }
  });

  buttonPatternLength = new ValueButton<uint8_t>(&currentValue, BUTTON_COLUMNS_X[1], 20, numberOfBarsTemp, std::vector<uint8_t>({ 1, 2, 4, 8 }), 4, 
  [ this ] (auto *b, auto *v) { // drawHandler
    b->draw("LENGTH", v->toString(), 1);
    applyPatternLength->drawNow();
  });

  buttonToggleTools = new TouchButton(BUTTON_COLUMNS_X[0], 15,
  [ this ](auto *b) { // drawHandler
    b->draw("TOOL", showingTools == TOOL_FILL ? "FILL" : "ARP", 1);
  },
  [ this ]() { // clickedHandler
    switch (showingTools) {
    case TOOL_FILL:
      showingTools = TOOL_ARP;
      break;
    
    case TOOL_ARP:
      showingTools = TOOL_FILL;
      break;
    }
    guiUpdateFlags |= drawTools;
  });

  // quantize
  for (int track = 0; track < LiveSequencer::LIVESEQUENCER_NUM_TRACKS; track++) {
    buttonsPattQuant.push_back(new ValueButton<uint8_t>(&currentValue, BUTTON_COLUMNS_X[track], 10, data->trackSettings[track].quantizeDenom, std::vector<uint8_t>({ 1, 2, 4, 8, 16, 32 }), 4,
    [ this ] (auto *b, auto *v) { // drawHandler
      const std::string text = (v->getValue() == 1) ? "NONE" : v->toString();
      b->draw("QUANT", text, v->getValue() == 1 ? 1 : 3);
    }));
  }

  // FILL TOOL
  buttonsFillTool.push_back(new ValueButton<uint8_t>(&currentValue, BUTTON_COLUMNS_X[2], 15, data->fillNotes.number, std::vector<uint8_t>({ 4, 6, 8, 12, 16, 24, 32 }), 16, 
  [ this ] (auto *b, auto *v) { // drawHandler
    b->draw("NUM", v->toString(), 3);
  }));
  buttonsFillTool.push_back(new ValueButton<uint8_t>(&currentValue, BUTTON_COLUMNS_X[3], 15, data->fillNotes.offset, 0, 7, 1, 0, 
  [ this ] (auto *b, auto *v) { // drawHandler
    b->draw("OFF", v->toString(), 3);
  }));
  buttonsFillTool.push_back(new TouchButton(BUTTON_COLUMNS_X[5], 15,
  [ ](auto *b) { // drawHandler
    b->draw("FILL", "NOW", 2);
  },
  [ this ]() { // clickedHandler
    liveSeqPtr->fillTrackLayer();
  }));

  // ARP TOOL
  buttonsArp.push_back(new ValueButton<uint8_t>(&currentValue, BUTTON_COLUMNS_X[1], 15, data->arpSettings.amount, std::vector<uint8_t>({ 0, 2, 4, 6, 8, 12, 16, 24, 32, 64 }), 8,
  [ this ] (auto *b, auto *v) { // drawHandler
    b->draw("NUM", v->toString(), v->getValue() == 0 ? 1 : 3);
    DBG_LOG(printf("outer act is %i\n", currentValue));
  }));
  buttonsArp.push_back(new ValueButton<uint8_t>(&currentValue, BUTTON_COLUMNS_X[2], 15, (uint8_t&)data->arpSettings.mode, 0, uint8_t(LiveSequencer::ARP_MODENUM-1), 1, uint8_t(LiveSequencer::ARP_DOWN),
  [ this ] (auto *b, auto *v) { // drawHandler
    b->draw("MODE", UI_LiveSequencer::getArpModeName(v->getValue()).c_str(), 3);
  }));
  buttonsArp.push_back(new ValueButton<uint16_t>(&currentValue, BUTTON_COLUMNS_X[3], 15, data->arpSettings.length, 50, 500, 10, 150,
  [ this ] (auto *b, auto *v) { // drawHandler
    b->draw("LEN", v->toString(), v->getValue() == 0 ? 1 : 3);
  }));
  buttonsArp.push_back(new ValueButton<int8_t>(&currentValue, BUTTON_COLUMNS_X[4], 15, data->arpSettings.swing, -8, 8, 1, 0,
  [ this ] (auto *b, auto *v) { // drawHandler
    b->draw("SWING", v->toString(), 3);
  }));
  buttonsArp.push_back(new ValueButton<uint8_t>(&currentValue, BUTTON_COLUMNS_X[5], 15, data->arpSettings.latch, 0, 1, 1, 1,
  [ this ] (auto *b, auto *v) { // drawHandler
    b->draw("LATCH", v->getValue() == 1 ? "ON" : "-", 3);
  }));
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
    draw_button_on_grid(42, 22, "REMAP", "MIDI", 1);
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
    applyScreenRedrawGuiFlags();
    drawGUI(guiUpdateFlags);
    // setup function
    LCDML.FUNC_setLoopInterval(40); // 25Hz gui refresh
  }
  // ****** LOOP *********
  if (LCDML.FUNC_loop()) {
    if(LCDML.BT_checkDown()) {
      if(currentValue != nullptr) {
        currentValue->next();
      }
    }
    if(LCDML.BT_checkUp()) {
      if(currentValue != nullptr) {
        currentValue->previous();
      }
    }
    if(LCDML.BT_checkEnter()) {
      DBG_LOG(printf("enter!\n"));
    }

    guiUpdateFlags |= data->trackLayersChanged ? (drawLayerButtons | drawTrackButtons) : 0;
    guiUpdateFlags |= (data->songLayersChanged && data->isSongMode) ? (drawSongSettings) : 0;
    guiUpdateFlags |= (data->isRunning || data->stoppedFlag) ? (drawActiveNotes | drawTime) : 0;

    if (data->currentPageIndex == PagePattern::PAGE_PATT_SETINGS) {
      guiUpdateFlags |= data->lastPlayedNoteChanged ? (drawTools) : 0;
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

void UI_LiveSequencer::applyScreenRedrawGuiFlags(void) {
  guiUpdateFlags |= (clearBottomArea | drawTopButtons | drawTrackButtons | drawTime);
  if (data->isSongMode) {
    switch (data->currentPageIndex) {
    case PageSong::PAGE_SONG_LAYERS:
      guiUpdateFlags |= drawLayerButtons;
      break;

    case PageSong::PAGE_SONG_SETTINGS:
      guiUpdateFlags |= drawSongSettings | drawDeleteSong | drawSongQuant;
      break;
    }
  }
  else {
    switch (data->currentPageIndex) {
    case PagePattern::PAGE_PATT_LAYERS:
      guiUpdateFlags |= drawLayerButtons;
      break;

    case PagePattern::PAGE_PATT_SETINGS:
      guiUpdateFlags |= (drawTools | drawQuantize | drawPattLength | drawDeleteAll);
      break;
    }
  }
  isLayerViewActive = (data->currentPageIndex == PagePattern::PAGE_PATT_LAYERS) || (data->currentPageIndex == PageSong::PAGE_SONG_LAYERS);
}

void UI_LiveSequencer::handleTouchscreen(void) {
  bool pressedChanged = (numTouchPoints != numPressedOld);
  if (showingHowTo) {
    if (check_button_on_grid(42,22)) {
      LCDML.FUNC_setGBAToLastFunc();
      LCDML.OTHER_jumpToFunc(UI_func_midi_channels);
    }
    return;
  }
  const bool runningChanged = (runningHere != data->isRunning);
  runningHere = data->isRunning;

  if (pressedChanged || runningChanged) {
    const bool runningPressed = check_button_on_grid(BUTTON_COLUMNS_X[0], 0);
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

    const bool recPressed = check_button_on_grid(BUTTON_COLUMNS_X[1], 0);
    if (recPressed) {
      data->isRecording = !data->isRecording;
      guiUpdateFlags |= (drawTopButtons | drawTrackButtons);
      trackLayerMode = LayerMode::LAYER_MUTE;

      if (isLayerViewActive) {
        guiUpdateFlags |= drawLayerButtons;
      }
    }

    const bool modePressed = check_button_on_grid(BUTTON_COLUMNS_X[5], 0);
    if (modePressed) {
      data->isSongMode = !data->isSongMode;
      if (data->isSongMode) {
        data->currentPageIndex = PageSong::PAGE_SONG_LAYERS;
      }
      else {
        data->currentPageIndex = PagePattern::PAGE_PATT_LAYERS;
      }
      applyScreenRedrawGuiFlags();
    }

    const bool funcPressed = check_button_on_grid(BUTTON_COLUMNS_X[4], 0);
    if (funcPressed) {
      data->currentPageIndex++;
      if (data->isSongMode && data->currentPageIndex == PageSong::PAGE_SONG_NUM) {
        data->currentPageIndex = PageSong::PAGE_SONG_LAYERS;
      }
      else if (data->isSongMode == false && data->currentPageIndex == PagePattern::PAGE_PATT_NUM) {
        data->currentPageIndex = PagePattern::PAGE_PATT_LAYERS;
      }
      applyScreenRedrawGuiFlags();
    }

    for (int track = 0; track < LiveSequencer::LIVESEQUENCER_NUM_TRACKS; track++) {
      const bool pressed = check_button_on_grid(BUTTON_COLUMNS_X[track], 5);
      if (pressed) {
        guiUpdateFlags |= drawTrackButtons;

        if (track == data->activeTrack) {
          if (data->isRecording) {
            if (data->pendingEvents.size()) {
              data->pendingEvents.clear(); // clear pending
            }
            else {
              // track layer actions only for pattern mode
              if (data->currentPageIndex == PagePattern::PAGE_PATT_LAYERS) {
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
          if (data->currentPageIndex == PagePattern::PAGE_PATT_LAYERS) {
            guiUpdateFlags |= drawLayerButtons;
          }
          DBG_LOG(printf("rec track now is %i\n", track + 1));
        }
      }
      switch (data->currentPageIndex) {
      case PageSong::PAGE_SONG_LAYERS:
      case PagePattern::PAGE_PATT_LAYERS:
        for (int layer = 0; layer < data->trackSettings[track].layerCount; layer++) {
          const bool pressed = check_button_on_grid(BUTTON_COLUMNS_X[track], 10 + layer * 5);
          if (pressed) {
            if (data->isRecording && (trackLayerMode != LayerMode::LAYER_MUTE) && (track == data->activeTrack)) {
              liveSeqPtr->trackLayerAction(track, layer, LayerMode(trackLayerMode));
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
        break;

      case PagePattern::PAGE_PATT_SETINGS:
        for(auto *b : buttonsPattQuant) {
          b->processPressed();
        }
        buttonToggleTools->processPressed();
        if(showingTools == TOOL_FILL) {
          for(auto *b : buttonsFillTool) {
            b->processPressed();
          }
        }
        if(showingTools == TOOL_ARP) {
          for(auto *b : buttonsArp) {
            b->processPressed();
          }
        }

        buttonPatternLength->processPressed();
        applyPatternLength->processPressed();
        break;

      case PageSong::PAGE_SONG_SETTINGS:
        if (check_button_on_grid(BUTTON_COLUMNS_X[1], 10)) {
          // song layer mode
          if (++songLayerMode == LayerMode::LAYER_MODE_NUM) {
            songLayerMode = LayerMode::LAYER_MUTE;
          }
          guiUpdateFlags |= drawSongSettings;
        }
        if (songLayerMode != LayerMode::LAYER_MUTE) { // song layers can not be muted
          for (uint8_t songLayer = 0; songLayer < data->songLayerCount; songLayer++) {
            if (check_button_on_grid(BUTTON_COLUMNS_X[2 + songLayer], 10)) {
              liveSeqPtr->songLayerAction(songLayer, LayerMode(songLayerMode));
              songLayerMode = LayerMode::LAYER_MUTE;
            }
          }
        }
        // song mute quantize denom
        if (check_button_on_grid(BUTTON_COLUMNS_X[1], 15)) {
          currentValue = songMuteQuant->cycle();
          guiUpdateFlags |= drawSongQuant;
        }

        break;
      }
      if (isLayerViewActive == false) {
        // common for PATTERN and SONG
        if (check_button_on_grid(BUTTON_COLUMNS_X[1], 25)) {
          // delete confirm
          deleteConfirming = !deleteConfirming;
          guiUpdateFlags |= data->isSongMode ? drawDeleteSong : drawDeleteAll;
        }
        if (check_button_on_grid(BUTTON_COLUMNS_X[2], 25)) {
          // really delete
          if (deleteConfirming) {
            deleteConfirming = false;
            if (data->isSongMode) {
              liveSeqPtr->deleteAllSongEvents();
              guiUpdateFlags |= drawDeleteSong;
            }
            else {
              liveSeqPtr->deleteLiveSequencerData();
              guiUpdateFlags |= drawDeleteAll;
            }
          }
        }
        if (check_button_on_grid(BUTTON_COLUMNS_X[4], 25)) {
          // load
          LCDML.FUNC_setGBAToLastFunc();
          display.setTextSize(2);
          display.setTextColor(COLOR_SYSTEXT, COLOR_BACKGROUND);
          DBG_LOG(printf("perf id %i\n", data->performanceID));
          LCDML.OTHER_jumpToFunc(UI_func_load_performance, data->performanceID);
        }
        if (check_button_on_grid(BUTTON_COLUMNS_X[5], 25)) {
          // save
          LCDML.FUNC_setGBAToLastFunc();
          display.setTextSize(2);
          display.setTextColor(COLOR_SYSTEXT, COLOR_BACKGROUND);
          LCDML.OTHER_jumpToFunc(UI_func_save_performance); // FIXME: should have current id selected
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
    draw_button_on_grid(BUTTON_COLUMNS_X[0], 0, (runningHere ? "STOP" : "START"), "", runningHere ? 2 : 0);
    draw_button_on_grid(BUTTON_COLUMNS_X[1], 0, "REC", "", data->isRecording ? 2 : 0);
    draw_button_on_grid(BUTTON_COLUMNS_X[4], 0, isLayerViewActive ? "LAYERS" : "TOOLS", "VIEW", 1);
    draw_button_on_grid(BUTTON_COLUMNS_X[5], 0, data->isSongMode ? "SONG" : "PATT", "MODE", 3);
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

  uint8_t trackButtonRecColor = 2; // red, or blinking
  const bool doBlink = data->notesOn.size() || data->pendingEvents.size();
  if (doBlink) {
    if (++guiCounter == 8) {
      guiCounter = 0;
      trackButtonRecColor = blinkPhase ? 1 : 2;
      guiFlags |= drawTrackButtons;
      blinkPhase = !blinkPhase;
    }
  }
  else {
    guiCounter = 0;
    blinkPhase = 0;
    trackButtonRecColor = 2;
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
        draw_button_on_grid(BUTTON_COLUMNS_X[track], 5, data->tracks[track].name, itoa(track + 1, temp_char, 10), (track == data->activeTrack) ? (data->isRecording ? trackButtonRecColor : 3) : 1);
      }
      if (isLayerViewActive) {
        const bool layerEditActive = !data->isSongMode && (data->activeTrack == track) && (trackLayerMode != LayerMode::LAYER_MUTE);
        // layer button
        for (int layer = 0; layer < LiveSequencer::LIVESEQUENCER_NUM_TRACKS; layer++) {
          const int buttonY = 10 + layer * 5;
          if (layer < data->trackSettings[track].layerCount) {
            const bool isMuted = data->tracks[track].layerMutes & (1 << layer);
            uint16_t layerBgColor = (isMuted ? GREY2 : (isSongRec ? RED : DX_DARKCYAN));
            uint8_t layerBgCode = (isMuted ? 0 : (isSongRec ? 2 : 1));
            if (layerEditActive) {
              // adapt button background if in layer edit mode
              handleLayerEditButtonColor(trackLayerMode, layerBgColor, layerBgCode);
            }
            if (guiFlags & drawLayerButtons) {
              drawLayerButton(data->isSongMode, trackLayerMode, layer, layerEditActive, layerBgCode, BUTTON_COLUMNS_X[track], buttonY);
            }
            if (guiFlags & drawActiveNotes) {
              // always draw notes when layers visible
              const int numNotesOn = data->tracks[track].activeNotes[layer].size();
              const int xStart = (BUTTON_COLUMNS_X[track] + button_size_x) * CHAR_width_small - 3;
              const int yStart = (10 + layer * 5) * CHAR_height_small;
              const int yFill = std::min(numNotesOn * 5, BUTTON_HEIGHT);
              display.console = true;
              display.fillRect(xStart, yStart, 3, BUTTON_HEIGHT - yFill, layerBgColor);
              display.fillRect(xStart, yStart + (BUTTON_HEIGHT - yFill), 3, yFill, COLOR_SYSTEXT);
            }
          }
          else {
            if (guiFlags & drawLayerButtons) {
              // clear button
              display.console = true;
              display.fillRect(BUTTON_COLUMNS_X[track] * CHAR_width_small, buttonY * CHAR_height_small, button_size_x * CHAR_width_small, CHAR_height_small * button_size_y, COLOR_BACKGROUND);
            }
          }
        }
      }
    }
  }
  if (isLayerViewActive == false) {
    if (guiFlags & drawQuantize) {
      for(auto *b : buttonsPattQuant) {
        b->drawNow();
      }
    }
    if (guiFlags & drawTools) {
      buttonToggleTools->drawNow();

      if(showingTools == TOOL_FILL) {
        // fill track
        draw_button_on_grid(BUTTON_COLUMNS_X[1], 15, "NOTE", itoa(data->lastPlayedNote, temp_char, 10), 0); // label only
        for(auto *b : buttonsFillTool) {
          b->drawNow();
        }
        draw_button_on_grid(BUTTON_COLUMNS_X[4], 15, "", "", 98); // spacer
      }
      if(showingTools == TOOL_ARP) {
        for(auto *b : buttonsArp) {
          b->drawNow();
        }
      }
    }

    if (guiFlags & drawPattLength) {
      // number of bars for one pattern
      draw_button_on_grid(BUTTON_COLUMNS_X[0], 20, "PATTERN", "LENGTH", 97); // label only
      buttonPatternLength->drawNow();
      applyPatternLength->drawNow();
    }

    if (guiFlags & drawSongSettings) {
      draw_button_on_grid(BUTTON_COLUMNS_X[0], 10, "SONG", "LAYERS", 97); // label only
      if (data->songLayerCount > 0) {
        draw_button_on_grid(BUTTON_COLUMNS_X[1], 10, "LAYER", "ACTION", 2); // switch modes
      }
      else {
        draw_button_on_grid(BUTTON_COLUMNS_X[1], 10, "NO", "LAYERS", 1); // switch modes
      }
      uint16_t layerBgColor = GREY2;
      uint8_t layerBgCode = 0;
      handleLayerEditButtonColor(songLayerMode, layerBgColor, layerBgCode);
      for (int songLayer = 0; songLayer < LiveSequencer::LIVESEQUENCER_NUM_TRACKS; songLayer++) {
        const uint8_t buttonY = 10;
        if (songLayer < data->songLayerCount) {
          drawLayerButton(data->isSongMode, songLayerMode, songLayer, true, layerBgCode, BUTTON_COLUMNS_X[2 + songLayer], buttonY);
        }
        else {
          // clear button
          display.console = true;
          display.fillRect(BUTTON_COLUMNS_X[2 + songLayer] * CHAR_width_small, buttonY * CHAR_height_small, button_size_x * CHAR_width_small, CHAR_height_small * button_size_y, GREY3);
        }
      }
    }

    if (guiFlags & drawSongQuant) {
      draw_button_on_grid(BUTTON_COLUMNS_X[0], 15, "MUTE", "QUANT", 97); // label only
      draw_button_on_grid(BUTTON_COLUMNS_X[1], 15, "QUANT", songMuteQuant->toString(), (songMuteQuant->getValue() == 1) ? 1 : 3);
    }

    if (guiFlags & (drawDeleteSong | drawDeleteAll)) {
      const bool isDeleteAll = guiFlags & drawDeleteAll;
      // delete all livesequencer event data
      draw_button_on_grid(BUTTON_COLUMNS_X[0], 25, "ACTIONS", "", 97); // label only
      draw_button_on_grid(BUTTON_COLUMNS_X[1], 25, "DELETE", isDeleteAll ? "ALL" : "SONG", 1);
      if (deleteConfirming) {
        draw_button_on_grid(BUTTON_COLUMNS_X[2], 25, "DO IT", "!", 2);
      }
      else {
        // clear button
        display.console = true;
        display.fillRect(BUTTON_COLUMNS_X[2] * CHAR_width_small, 25 * CHAR_height_small, button_size_x * CHAR_width_small, CHAR_height_small * button_size_y, GREY3);
      }
      draw_button_on_grid(BUTTON_COLUMNS_X[4], 25, "LOAD", "PERF", 1);
      draw_button_on_grid(BUTTON_COLUMNS_X[5], 25, "SAVE", "PERF", 1);
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

void UI_LiveSequencer::drawLayerButton(const bool horizontal, uint8_t layerMode, int layer, const bool layerEditActive, uint8_t layerBgCode, uint8_t gridX, uint8_t gridY) {
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
  draw_button_on_grid(gridX, gridY, label.c_str(), labelSub.c_str(), layerBgCode);
}

void UI_LiveSequencer::handleLayerEditButtonColor(uint8_t layerMode, uint16_t& layerBgColor, uint8_t& layerBgCode) {
  switch (layerMode) {
  case LayerMode::LAYER_MERGE:
    layerBgColor = MIDDLEGREEN;
    layerBgCode = 3;
    break;

  case LayerMode::LAYER_DELETE:
    layerBgColor = RED;
    layerBgCode = 2;
    break;
  }
}
