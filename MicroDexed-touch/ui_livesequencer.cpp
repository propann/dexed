#include "ui_livesequencer.h"
#include "LCDMenuLib2.h"
#include "ILI9341_t3n.h"
#include "sequencer.h"
#include "livesequencer.h"
#include "editableValue.h"

extern LCDMenuLib2 LCDML;
extern ILI9341_t3n display;
extern void draw_button_on_grid(uint8_t x, uint8_t y, const char* t1, const char* t2, uint8_t color);
extern bool check_button_on_grid(uint8_t x, uint8_t y);
extern void handleStart();
extern void handleStop();

extern void UI_func_load_performance(uint8_t param);
extern void UI_func_save_performance(uint8_t param);
extern void UI_func_midi_channels(uint8_t param);

extern bool remote_active;
extern int numTouchPoints;

static constexpr int BUTTON_HEIGHT = CHAR_height_small * button_size_y;
static constexpr int BUTTON_WIDTH = CHAR_width_small * button_size_x;

extern void setCursor_textGrid(uint8_t pos_x, uint8_t pos_y);
extern void setCursor_textGrid_small(uint8_t pos_x, uint8_t pos_y);
extern void helptext_l(const char* str);


bool runningHere = false;
bool barPhases[2] = { 0 };
uint8_t numberOfBarsTemp = 0;
bool deleteConfirming = false;
bool showingHowTo = false;

enum ShowingTools : uint8_t {
  TOOL_FILL,
  TOOL_ARP
} showingTools = TOOL_FILL;

EditableValue<uint8_t> *fillNum;
EditableValue<uint8_t> *fillOff;

EditableValue<uint8_t> *arpAmount;
EditableValue<uint16_t> *arpLength;
EditableValue<uint8_t> *arpMode;
EditableValue<int8_t> *arpSwing;
EditableValue<uint8_t> *arpLatch;

EditableValueBase *currentValue;

LiveSequencer* liveSeqPtr;
LiveSequencer::LiveSeqData* liveSeqData;
uint8_t guiCounter = 0;
bool blinkPhase = 0;
static constexpr uint8_t BUTTON_OFFSET_X = 0; // not possible to distribute better with button grid
static constexpr uint8_t BUTTON_COLUMNS_X[LIVESEQUENCER_NUM_TRACKS] = { BUTTON_OFFSET_X, BUTTON_OFFSET_X + 9, BUTTON_OFFSET_X + 18, BUTTON_OFFSET_X + 27, BUTTON_OFFSET_X + 36, BUTTON_OFFSET_X + 45 };

uint8_t trackLayerMode = LAYER_MUTE;
uint8_t songLayerMode = LAYER_MUTE;

void applyScreenRedrawGuiFlags();

UI_LiveSequencer::UI_LiveSequencer(LiveSequencer* sequencer) {
  liveSeqPtr = sequencer;
  liveSeqData = sequencer->getData();

  fillNum = new EditableValue<uint8_t>(&liveSeqData->fillNotes.number, std::vector<uint8_t>({ 4, 6, 8, 12, 16, 24, 32 }), 16);
  fillOff = new EditableValue<uint8_t>(&liveSeqData->fillNotes.offset, 0, 7, 1, 0);

  arpAmount = new EditableValue<uint8_t>(&liveSeqData->arpSettings.amount, std::vector<uint8_t>({ 0, 2, 4, 6, 8, 12, 16, 24, 32, 64 }), 8);
  arpLength = new EditableValue<uint16_t>(&liveSeqData->arpSettings.length, 50, 500, 10, 150);
  arpMode = new EditableValue<uint8_t>((uint8_t*)&liveSeqData->arpSettings.mode, 0, uint8_t(LiveSequencer::ARP_MODENUM-1), 1, uint8_t(LiveSequencer::ARP_DOWN));
  arpSwing = new EditableValue<int8_t>(&liveSeqData->arpSettings.swing, -8, 8, 1, 0);
  arpLatch = new EditableValue<uint8_t>(&liveSeqData->arpSettings.latch, 0, 1, 1, 1);
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

enum GuiUpdates : uint16_t {
  drawTopButtons = (1 << 0),
  drawTrackButtons = (1 << 1),
  drawLayerButtons = (1 << 2),
  drawTools = (1 << 3),
  drawQuantisize = (1 << 4),
  clearBottomArea = (1 << 5),
  drawSongSettings = (1 << 6),
  drawPattLength = (1 << 7),
  drawActiveNotes = (1 << 8),
  drawDeleteAll = (1 << 9),
  drawDeleteSong = (1 << 10),
  drawSongQuant = (1 << 11),
  drawTime = (1 << 12)
};

bool isLayerViewActive = false;
uint16_t guiUpdateFlags = 0;
bool stayActive = false; // LiveSequencer stays active in instrument settings opened from here

void drawGUI(uint16_t& guiFlags);
void handleLayerEditButtonColor(uint8_t layerMode, uint16_t& layerBgColor, uint8_t& layerBgCode);
void drawLayerButton(const bool horizontal, uint8_t layerMode, int layer, const bool layerEditActive, uint8_t layerBgCode, uint8_t gridX, uint8_t gridY);

void UI_func_livesequencer(uint8_t param) {
  // ****** SETUP *********
  if (LCDML.FUNC_setup()) {
    liveSeqData->isActive = true;
    stayActive = false;
    display.fillScreen(COLOR_BACKGROUND);
    numberOfBarsTemp = liveSeqData->numberOfBars;
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
        guiUpdateFlags |= drawTools;
      }
    }
    if(LCDML.BT_checkUp()) {
      if(currentValue != nullptr) {
        currentValue->previous();
        guiUpdateFlags |= drawTools;
      }
    }

    guiUpdateFlags |= liveSeqData->trackLayersChanged ? (drawLayerButtons | drawTrackButtons) : 0;
    guiUpdateFlags |= (liveSeqData->songLayersChanged && liveSeqData->isSongMode) ? (drawSongSettings) : 0;
    guiUpdateFlags |= (liveSeqData->isRunning || liveSeqData->stoppedFlag) ? (drawActiveNotes | drawTime) : 0;

    if (liveSeqData->currentPageIndex == PagePattern::PAGE_PATT_SETINGS) {
      guiUpdateFlags |= liveSeqData->lastPlayedNoteChanged ? (drawTools) : 0;
    }
    liveSeqData->songLayersChanged = false;
    liveSeqData->trackLayersChanged = false;
    liveSeqData->lastPlayedNoteChanged = false;
    liveSeqData->stoppedFlag = false;
    if (showingHowTo == false) {
      drawGUI(guiUpdateFlags);
    }
  }

  // ****** STABLE END *********
  if (LCDML.FUNC_close()) {
    if (stayActive == false) {
      liveSeqData->isActive = false;
    }
    else {
      stayActive = false;
    }
    liveSeqData->isRecording = false;
    showingHowTo = false;
    display.fillScreen(COLOR_BACKGROUND);
  }
}

void applyScreenRedrawGuiFlags() {
  guiUpdateFlags |= (clearBottomArea | drawTopButtons | drawTrackButtons | drawTime);
  if (liveSeqData->isSongMode) {
    switch (liveSeqData->currentPageIndex) {
    case PageSong::PAGE_SONG_LAYERS:
      guiUpdateFlags |= drawLayerButtons;
      break;

    case PageSong::PAGE_SONG_SETTINGS:
      guiUpdateFlags |= drawSongSettings | drawDeleteSong | drawSongQuant;
      break;
    }
  }
  else {
    switch (liveSeqData->currentPageIndex) {
    case PagePattern::PAGE_PATT_LAYERS:
      guiUpdateFlags |= drawLayerButtons;
      break;

    case PagePattern::PAGE_PATT_SETINGS:
      guiUpdateFlags |= (drawTools | drawQuantisize | drawPattLength | drawDeleteAll);
      break;
    }
  }
  isLayerViewActive = (liveSeqData->currentPageIndex == PagePattern::PAGE_PATT_LAYERS) || (liveSeqData->currentPageIndex == PageSong::PAGE_SONG_LAYERS);
}

void handle_touchscreen_live_sequencer(void) {
  if (showingHowTo) {
    if (numTouchPoints > 0 && check_button_on_grid(42,22)) {
      LCDML.FUNC_setGBAToLastFunc();
      LCDML.OTHER_jumpToFunc(UI_func_midi_channels);
    }
    return;
  }
  const bool runningChanged = (runningHere != liveSeqData->isRunning);
  runningHere = liveSeqData->isRunning;

  if ((numTouchPoints > 0) || runningChanged) {
    const bool runningPressed = (numTouchPoints > 0) && check_button_on_grid(BUTTON_COLUMNS_X[0], 0);
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
      liveSeqData->isRecording = !liveSeqData->isRecording;
      guiUpdateFlags |= (drawTopButtons | drawTrackButtons);
      trackLayerMode = LayerMode::LAYER_MUTE;

      if (isLayerViewActive) {
        guiUpdateFlags |= drawLayerButtons;
      }
    }

    const bool modePressed = check_button_on_grid(BUTTON_COLUMNS_X[5], 0);
    if (modePressed) {
      liveSeqData->isSongMode = !liveSeqData->isSongMode;
      if (liveSeqData->isSongMode) {
        liveSeqData->currentPageIndex = PageSong::PAGE_SONG_LAYERS;
      }
      else {
        liveSeqData->currentPageIndex = PagePattern::PAGE_PATT_LAYERS;
      }
      applyScreenRedrawGuiFlags();
    }

    const bool funcPressed = check_button_on_grid(BUTTON_COLUMNS_X[4], 0);
    if (funcPressed) {
      liveSeqData->currentPageIndex++;
      if (liveSeqData->isSongMode && liveSeqData->currentPageIndex == PageSong::PAGE_SONG_NUM) {
        liveSeqData->currentPageIndex = PageSong::PAGE_SONG_LAYERS;
      }
      else if (liveSeqData->isSongMode == false && liveSeqData->currentPageIndex == PagePattern::PAGE_PATT_NUM) {
        liveSeqData->currentPageIndex = PagePattern::PAGE_PATT_LAYERS;
      }
      applyScreenRedrawGuiFlags();
    }

    for (int track = 0; track < LIVESEQUENCER_NUM_TRACKS; track++) {
      const bool pressed = check_button_on_grid(BUTTON_COLUMNS_X[track], 5);
      if (pressed) {
        guiUpdateFlags |= drawTrackButtons;

        if (track == liveSeqData->activeTrack) {
          if (liveSeqData->isRecording) {
            if (liveSeqData->pendingEvents.size()) {
              liveSeqData->pendingEvents.clear(); // clear pending
            }
            else {
              // track layer actions only for pattern mode
              if (liveSeqData->currentPageIndex == PagePattern::PAGE_PATT_LAYERS) {
                if (++trackLayerMode == LayerMode::LAYER_MODE_NUM) {
                  trackLayerMode = LayerMode::LAYER_MUTE;
                }
                guiUpdateFlags |= drawLayerButtons;
              }
            }
          }
          else {
            // open instrument settings
            if (liveSeqData->tracks[track].screenSetupFn != nullptr) {
              SetupFn f = (SetupFn)liveSeqData->tracks[track].screenSetupFn;
              f(0);
            }
            stayActive = true; // stay active for screens instrument settings opened in LiveSequencer
            LCDML.FUNC_setGBAToLastFunc();
            LCDML.OTHER_jumpToFunc(liveSeqData->tracks[track].screen);
          }
        }
        else {
          liveSeqData->activeTrack = track;
          trackLayerMode = LayerMode::LAYER_MUTE;
          if (liveSeqData->currentPageIndex == PagePattern::PAGE_PATT_LAYERS) {
            guiUpdateFlags |= drawLayerButtons;
          }
          DBG_LOG(printf("rec track now is %i\n", track + 1));
        }
      }
      switch (liveSeqData->currentPageIndex) {
      case PageSong::PAGE_SONG_LAYERS:
      case PagePattern::PAGE_PATT_LAYERS:
        for (int layer = 0; layer < liveSeqData->trackSettings[track].layerCount; layer++) {
          const bool pressed = check_button_on_grid(BUTTON_COLUMNS_X[track], 10 + layer * 5);
          if (pressed) {
            if (liveSeqData->isRecording && (trackLayerMode != LayerMode::LAYER_MUTE) && (track == liveSeqData->activeTrack)) {
              liveSeqPtr->trackLayerAction(track, layer, LayerMode(trackLayerMode));
              trackLayerMode = LayerMode::LAYER_MUTE;
            }
            else {
              const bool isMutedOld = liveSeqData->tracks[track].layerMutes & (1 << layer);
              const bool recordMuteToSong = liveSeqData->isSongMode && liveSeqData->isRecording && liveSeqData->isRunning;
              liveSeqPtr->setLayerMuted(track, layer, !isMutedOld, recordMuteToSong);
            }
            guiUpdateFlags |= (drawLayerButtons);
          }
        }
        break;

      case PagePattern::PAGE_PATT_SETINGS:
        for (int track = 0; track < LIVESEQUENCER_NUM_TRACKS; track++) {
          if (check_button_on_grid(BUTTON_COLUMNS_X[track], 10)) {
            liveSeqData->trackSettings[track].quantisizeDenom *= 2;
            if (liveSeqData->trackSettings[track].quantisizeDenom > 32) {
              liveSeqData->trackSettings[track].quantisizeDenom = 1;
            }
            guiUpdateFlags |= drawQuantisize;
          }
        }
        if (check_button_on_grid(BUTTON_COLUMNS_X[0], 15)) {
          switch (showingTools) {
          case TOOL_FILL:
            showingTools = TOOL_ARP;
            break;
          
          case TOOL_ARP:
            showingTools = TOOL_FILL;
            break;
          }
          guiUpdateFlags |= drawTools;
        }
        if(showingTools == TOOL_FILL) {
          if (check_button_on_grid(BUTTON_COLUMNS_X[2], 15)) {
            // fill number
            currentValue = fillNum->pressed();
            guiUpdateFlags |= drawTools;
          }
          if (check_button_on_grid(BUTTON_COLUMNS_X[3], 15)) {
            // fill offset
            currentValue = fillOff->pressed();
            guiUpdateFlags |= drawTools;
          }
          if (check_button_on_grid(BUTTON_COLUMNS_X[4], 15)) {
            // fill now
            liveSeqPtr->fillTrackLayer();
            guiUpdateFlags |= drawTools;
          }
        }
        if(showingTools == TOOL_ARP) {
          if (check_button_on_grid(BUTTON_COLUMNS_X[1], 15)) {
            // arp number
            currentValue = arpAmount->pressed();
            guiUpdateFlags |= drawTools;
          }
          if (check_button_on_grid(BUTTON_COLUMNS_X[2], 15)) {
            // arp mode
            currentValue = arpMode->pressed();
            guiUpdateFlags |= drawTools;
          }
          if (check_button_on_grid(BUTTON_COLUMNS_X[3], 15)) {
            // arp length
            currentValue = arpLength->pressed();
            guiUpdateFlags |= drawTools;
          }
          if (check_button_on_grid(BUTTON_COLUMNS_X[4], 15)) {
            // arp swing
            currentValue = arpSwing->pressed();
            guiUpdateFlags |= drawTools;
          }
          if (check_button_on_grid(BUTTON_COLUMNS_X[5], 15)) {
            // arp latch
            currentValue = arpLatch->pressed();
            guiUpdateFlags |= drawTools;
          }
        }

        if (check_button_on_grid(BUTTON_COLUMNS_X[1], 20)) {
          // number of bars cycle through
          numberOfBarsTemp *= 2;
          if (numberOfBarsTemp > 8) {
            numberOfBarsTemp = 1;
          }
          guiUpdateFlags |= drawPattLength;
        }
        if (check_button_on_grid(BUTTON_COLUMNS_X[2], 20)) {
          // apply changed number of bars
          if (numberOfBarsTemp != liveSeqData->numberOfBars) {
            handleStop();
            liveSeqPtr->changeNumberOfBars(numberOfBarsTemp);
            guiUpdateFlags |= drawPattLength;
          }
        }
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
          for (uint8_t songLayer = 0; songLayer < liveSeqData->songLayerCount; songLayer++) {
            if (check_button_on_grid(BUTTON_COLUMNS_X[2 + songLayer], 10)) {
              liveSeqPtr->songLayerAction(songLayer, LayerMode(songLayerMode));
              songLayerMode = LayerMode::LAYER_MUTE;
            }
          }
        }
        // song mute quantisize denom
        if (check_button_on_grid(BUTTON_COLUMNS_X[1], 15)) {
          liveSeqData->songMuteQuantisizeDenom *= 2;
          if (liveSeqData->songMuteQuantisizeDenom > 8) {
            liveSeqData->songMuteQuantisizeDenom = 1;
          }
          guiUpdateFlags |= drawSongQuant;
        }

        break;
      }
      if (isLayerViewActive == false) {
        // common for PATTERN and SONG
        if (check_button_on_grid(BUTTON_COLUMNS_X[1], 25)) {
          // delete confirm
          deleteConfirming = !deleteConfirming;
          guiUpdateFlags |= liveSeqData->isSongMode ? drawDeleteSong : drawDeleteAll;
        }
        if (check_button_on_grid(BUTTON_COLUMNS_X[2], 25)) {
          // really delete
          if (deleteConfirming) {
            deleteConfirming = false;
            if (liveSeqData->isSongMode) {
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
          DBG_LOG(printf("perf id %i\n", liveSeqData->performanceID));
          LCDML.OTHER_jumpToFunc(UI_func_load_performance, liveSeqData->performanceID);
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
  }
}

void drawGUI(uint16_t& guiFlags) {
  if (remote_active) {
    display.console = true;
  }
  if (guiFlags & drawTopButtons) {
    draw_button_on_grid(BUTTON_COLUMNS_X[0], 0, (runningHere ? "STOP" : "START"), "", runningHere ? 2 : 0);
    draw_button_on_grid(BUTTON_COLUMNS_X[1], 0, "REC", "", liveSeqData->isRecording ? 2 : 0);
    draw_button_on_grid(BUTTON_COLUMNS_X[4], 0, isLayerViewActive ? "LAYERS" : "TOOLS", "VIEW", 1);
    draw_button_on_grid(BUTTON_COLUMNS_X[5], 0, liveSeqData->isSongMode ? "SONG" : "PATT", "MODE", 3);
  }

  uint16_t patCount = 0;
  uint16_t timeMs = 0;
  if (runningHere) {
    patCount = liveSeqData->currentPattern;
    timeMs = liveSeqData->patternTimer;

    if (liveSeqData->patternBeginFlag) {
      liveSeqData->patternBeginFlag = false;

      display.fillRect(110, 5, 90, 5, barPhases[0] ? GREEN : COLOR_BACKGROUND);
      barPhases[0] = !barPhases[0];
      if (liveSeqData->currentPattern == 0) {
        display.fillRect(110, 10, 90, 5, barPhases[1] ? RED : COLOR_BACKGROUND);
        barPhases[1] = !barPhases[1];
      }
    }
    else {
      const float progressPattern = liveSeqData->patternTimer / float(liveSeqData->patternLengthMs);
      const float progressTotal = std::min(1.0f, (progressPattern + liveSeqData->currentPattern) / float(liveSeqData->numberOfBars));
      display.fillRect(110, 5, progressPattern * 90, 5, barPhases[0] ? GREEN : COLOR_BACKGROUND);
      display.fillRect(110, 10, progressTotal * 90, 5, barPhases[1] ? RED : COLOR_BACKGROUND);
    }
  }

  // print time
  if (guiFlags & drawTime) {
    display.setCursor(110, 20);
    display.setTextSize(1);
    display.setTextColor(COLOR_SYSTEXT, COLOR_BACKGROUND);
    if (liveSeqData->isSongMode) {
      const uint32_t songMs = (liveSeqData->songPatternCount * liveSeqData->numberOfBars + liveSeqData->currentPattern) * liveSeqData->patternLengthMs + timeMs;
      const uint32_t minutes = songMs / 60000;
      const uint32_t seconds = (songMs % 60000) / 1000;
      const uint32_t millis = songMs % 1000;
      display.printf("S %i:%02i.%03i", minutes, seconds, millis);
    }
    else {
      display.printf("P %i.%04i   ", patCount, timeMs);
    }
    display.setCursor(190, 20);
    if (liveSeqData->isSongMode) {
      display.printf("%02i", liveSeqData->songPatternCount);
    }
    else {
      display.printf("%02i", liveSeqData->currentPattern);
    }
  }

  char temp_char[6];

  uint8_t trackButtonRecColor = 2; // red, or blinking
  const bool doBlink = liveSeqData->notesOn.size() || liveSeqData->pendingEvents.size();
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
    const bool isSongRec = (liveSeqData->isSongMode && liveSeqData->isRecording);
    for (int track = 0; track < LIVESEQUENCER_NUM_TRACKS; track++) {
      if (guiFlags & drawTrackButtons) {
        draw_button_on_grid(BUTTON_COLUMNS_X[track], 5, liveSeqData->tracks[track].name, itoa(track + 1, temp_char, 10), (track == liveSeqData->activeTrack) ? (liveSeqData->isRecording ? trackButtonRecColor : 3) : 1);
      }
      if (isLayerViewActive) {
        const bool layerEditActive = !liveSeqData->isSongMode && (liveSeqData->activeTrack == track) && (trackLayerMode != LayerMode::LAYER_MUTE);
        // layer button
        for (int layer = 0; layer < LIVESEQUENCER_NUM_LAYERS; layer++) {
          const int buttonY = 10 + layer * 5;
          if (layer < liveSeqData->trackSettings[track].layerCount) {
            const bool isMuted = liveSeqData->tracks[track].layerMutes & (1 << layer);
            uint16_t layerBgColor = (isMuted ? GREY2 : (isSongRec ? RED : DX_DARKCYAN));
            uint8_t layerBgCode = (isMuted ? 0 : (isSongRec ? 2 : 1));
            if (layerEditActive) {
              // adapt button background if in layer edit mode
              handleLayerEditButtonColor(trackLayerMode, layerBgColor, layerBgCode);
            }
            if (guiFlags & drawLayerButtons) {
              drawLayerButton(liveSeqData->isSongMode, trackLayerMode, layer, layerEditActive, layerBgCode, BUTTON_COLUMNS_X[track], buttonY);
            }
            if (guiFlags & drawActiveNotes) {
              // always draw notes when layers visible
              const int numNotesOn = liveSeqData->tracks[track].activeNotes[layer].size();
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
    if (guiFlags & drawQuantisize) {
      // quantisize
      for (int track = 0; track < LIVESEQUENCER_NUM_TRACKS; track++) {
        const uint8_t denom = liveSeqData->trackSettings[track].quantisizeDenom;
        const std::string text = (denom == 1) ? "NONE" : itoa(denom, temp_char, 10);
        draw_button_on_grid(BUTTON_COLUMNS_X[track], 10, "QUANT", text.c_str(), (denom == 1) ? 1 : 3);
      }
    }

    if (guiFlags & drawTools) {
      draw_button_on_grid(BUTTON_COLUMNS_X[0], 15, "TOOL", showingTools == TOOL_FILL ? "FILL" : "ARP", 1);

      if(showingTools == TOOL_FILL) {
        // fill track
        draw_button_on_grid(BUTTON_COLUMNS_X[1], 15, "NOTE", itoa(liveSeqData->lastPlayedNote, temp_char, 10), 0); // label only
        draw_button_on_grid(BUTTON_COLUMNS_X[2], 15, "NUM", itoa(liveSeqData->fillNotes.number, temp_char, 10), 3);
        draw_button_on_grid(BUTTON_COLUMNS_X[3], 15, "OFF", itoa(liveSeqData->fillNotes.offset, temp_char, 10), 3);
        draw_button_on_grid(BUTTON_COLUMNS_X[4], 15, "", "", 97); // spacer
        draw_button_on_grid(BUTTON_COLUMNS_X[5], 15, "FILL", "NOW", 2);
      }
      if(showingTools == TOOL_ARP) {
        draw_button_on_grid(BUTTON_COLUMNS_X[1], 15, "NUM", itoa(liveSeqData->arpSettings.amount, temp_char, 10), 3);
        getArpModeName(liveSeqData->arpSettings.mode, temp_char);
        draw_button_on_grid(BUTTON_COLUMNS_X[2], 15, "MODE", temp_char, 3);
        draw_button_on_grid(BUTTON_COLUMNS_X[3], 15, "LEN", itoa(liveSeqData->arpSettings.length, temp_char, 10), 3);
        draw_button_on_grid(BUTTON_COLUMNS_X[4], 15, "SWING", itoa(liveSeqData->arpSettings.swing, temp_char, 10), 3);
        draw_button_on_grid(BUTTON_COLUMNS_X[5], 15, "LATCH", (liveSeqData->arpSettings.latch == 0) ? "NO" : "ON", 3);
      }
    }

    if (guiFlags & drawPattLength) {
      // number of bars for one pattern
      draw_button_on_grid(BUTTON_COLUMNS_X[0], 20, "PATTERN", "LENGTH", 97); // label only
      draw_button_on_grid(BUTTON_COLUMNS_X[1], 20, "LENGTH", itoa(numberOfBarsTemp, temp_char, 10), 1);
      draw_button_on_grid(BUTTON_COLUMNS_X[2], 20, "APPLY", "NOW", (liveSeqData->numberOfBars == numberOfBarsTemp) ? 1 : 2);

      display.setTextSize(1);
      display.setTextColor((liveSeqData->numberOfBars == numberOfBarsTemp) ? GREY2 : RED, GREY3);
      display.setCursor(165, 165);
      display.printf("CHANGING PATTERN LENGTH");
      display.setCursor(165, 175);
      display.printf("WILL DELETE ALL DATA!");
    }

    if (guiFlags & drawSongSettings) {
      draw_button_on_grid(BUTTON_COLUMNS_X[0], 10, "SONG", "LAYERS", 97); // label only
      if (liveSeqData->songLayerCount > 0) {
        draw_button_on_grid(BUTTON_COLUMNS_X[1], 10, "LAYER", "ACTION", 2); // switch modes
      }
      else {
        draw_button_on_grid(BUTTON_COLUMNS_X[1], 10, "NO", "LAYERS", 1); // switch modes
      }
      uint16_t layerBgColor = GREY2;
      uint8_t layerBgCode = 0;
      handleLayerEditButtonColor(songLayerMode, layerBgColor, layerBgCode);
      for (int songLayer = 0; songLayer < LIVESEQUENCER_NUM_LAYERS; songLayer++) {
        const uint8_t buttonY = 10;
        if (songLayer < liveSeqData->songLayerCount) {
          drawLayerButton(liveSeqData->isSongMode, songLayerMode, songLayer, true, layerBgCode, BUTTON_COLUMNS_X[2 + songLayer], buttonY);
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
      const std::string text = (liveSeqData->songMuteQuantisizeDenom == 1) ? "NONE" : itoa(liveSeqData->songMuteQuantisizeDenom, temp_char, 10);
      draw_button_on_grid(BUTTON_COLUMNS_X[1], 15, "QUANT", text.c_str(), (liveSeqData->songMuteQuantisizeDenom == 1) ? 1 : 3);
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

void getArpModeName(uint8_t mode, char* buf) {
  switch (mode) {
  case LiveSequencer::ArpMode::ARP_CHORD:
    sprintf(buf, "CHRD");
    break;
  case LiveSequencer::ArpMode::ARP_DOWN:
    sprintf(buf, "DOWN");
    break;
  case LiveSequencer::ArpMode::ARP_DOWNUP:
    sprintf(buf, "DNUP");
    break;
  case LiveSequencer::ArpMode::ARP_DOWNUP_P:
    sprintf(buf, "DNUP+");
    break;
  case LiveSequencer::ArpMode::ARP_RANDOM:
    sprintf(buf, "RAND");
    break;
  case LiveSequencer::ArpMode::ARP_UP:
    sprintf(buf, "UP");
    break;
  case LiveSequencer::ArpMode::ARP_UPDOWN:
    sprintf(buf, "UPDN");
    break;
  case LiveSequencer::ArpMode::ARP_UPDOWN_P:
    sprintf(buf, "UPDN+");
    break;
  }
}

void drawLayerButton(const bool horizontal, uint8_t layerMode, int layer, const bool layerEditActive, uint8_t layerBgCode, uint8_t gridX, uint8_t gridY) {
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

void handleLayerEditButtonColor(uint8_t layerMode, uint16_t& layerBgColor, uint8_t& layerBgCode) {
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
