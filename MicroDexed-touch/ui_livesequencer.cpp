#include "ui_livesequencer.h"
#include "LCDMenuLib2.h"
#include "ILI9341_t3n.h"
#include "sequencer.h"

#include "livesequencer.h"
#include "scope.h"

extern LCDMenuLib2 LCDML;
extern ILI9341_t3n display;
extern void draw_button_on_grid(uint8_t x, uint8_t y, const char* t1, const char* t2, uint8_t color);
extern bool check_button_on_grid(uint8_t x, uint8_t y);
extern void handleStart();
extern void handleStop();

extern bool remote_active;
extern int numTouchPoints;

static constexpr int BUTTON_HEIGHT = CHAR_height_small * button_size_y;
static constexpr int BUTTON_WIDTH = CHAR_width_small * button_size_x;

bool runningHere = false;
bool barPhases[2] = { 0 };
uint8_t numberOfBarsTemp = 0;

LiveSequencer *liveSeqPtr;
LiveSequencer::LiveSeqData *liveSeqData;
uint8_t guiCounter = 0;
bool blinkPhase = 0;
static constexpr uint8_t BUTTON_OFFSET_X = 0; // not possible to distribute better with button grid
static constexpr uint8_t BUTTON_COLUMNS_X[LIVESEQUENCER_NUM_TRACKS] = { BUTTON_OFFSET_X, BUTTON_OFFSET_X + 9, BUTTON_OFFSET_X + 18, BUTTON_OFFSET_X + 27, BUTTON_OFFSET_X + 36, BUTTON_OFFSET_X + 45 };

uint8_t trackLayerMode = LAYER_MUTE;
uint8_t songLayerMode = LAYER_MUTE;

void applyScreenRedrawGuiFlags();

UI_LiveSequencer::UI_LiveSequencer(LiveSequencer *sequencer) {
  liveSeqPtr = sequencer;
  liveSeqData = sequencer->getData();
  numberOfBarsTemp = liveSeqData->numberOfBars;
}

bool isLayerView() {
  return liveSeqData->functionPageIndex == PagePattern::PAGE_PATT_LAYERS || liveSeqData->functionPageIndex == PageSong::PAGE_SONG_LAYERS;
}

enum GuiUpdates : uint16_t {
  drawTopButtons    = (1 << 0),
  drawTrackButtons  = (1 << 1),
  drawLayerButtons  = (1 << 2),
  drawFillNotes     = (1 << 3),
  drawQuantisize    = (1 << 4),
  clearBottomArea   = (1 << 5),
  drawSongAuto      = (1 << 6),
  drawPattLength    = (1 << 7),
  updateAll         = 0xFFFF
};
uint16_t guiUpdateFlags = 0;

uint8_t fillNotesSteps[] = { 4, 6, 8, 12, 16, 24, 32 };

void drawGUI(uint16_t &guiFlags);
void handleLayerEditButtonColor(uint8_t layerMode, uint16_t &layerBgColor, uint8_t &layerBgCode);
void drawLayerButton(uint8_t layerMode, int layer, const bool layerEditActive, uint8_t layerBgCode, uint8_t gridX, uint8_t gridY);

void UI_func_livesequencer(uint8_t param) {
  // ****** SETUP *********
  if (LCDML.FUNC_setup()) {

    display.fillScreen(COLOR_BACKGROUND);
    liveSeqPtr->init();

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
    guiUpdateFlags |= liveSeqData->trackLayersChanged ? (drawLayerButtons | drawTrackButtons) : 0;
    guiUpdateFlags |= liveSeqData->songLayersChanged ? (drawSongAuto) : 0;

    if(liveSeqData->functionPageIndex == PagePattern::PAGE_PATT_SETINGS) {
      guiUpdateFlags |= liveSeqData->lastPlayedNoteChanged ? (drawFillNotes) : 0;
    }
    liveSeqData->songLayersChanged = false;
    liveSeqData->trackLayersChanged = false;
    liveSeqData->lastPlayedNoteChanged = false;
    drawGUI(guiUpdateFlags);
  }
  
  // ****** STABLE END *********
  if (LCDML.FUNC_close()) {
    liveSeqData->isRecording = false;
    display.fillScreen(COLOR_BACKGROUND);
  }
}

void applyScreenRedrawGuiFlags() {
  guiUpdateFlags |= (clearBottomArea | drawTopButtons | drawTrackButtons);
  if(liveSeqData->isSongMode) {
    switch(liveSeqData->functionPageIndex) {
    case PageSong::PAGE_SONG_LAYERS:
      guiUpdateFlags |= drawLayerButtons;
      break;

    case PageSong::PAGE_SONG_AUTOMATIONS:
      // todo
      guiUpdateFlags |= drawSongAuto;
      break;
    }
  } else {
    switch(liveSeqData->functionPageIndex) {
    case PagePattern::PAGE_PATT_LAYERS:
      guiUpdateFlags |= drawLayerButtons;
      break;

    case PagePattern::PAGE_PATT_SETINGS:
      guiUpdateFlags |= (drawFillNotes | drawQuantisize | drawPattLength);
      break;
    }
  }
}

void handle_touchscreen_live_sequencer(void) {  
  const bool runningChanged = (runningHere != liveSeqData->isRunning);
  runningHere = liveSeqData->isRunning;
  
  if((numTouchPoints > 0) || runningChanged) {
    const bool runningPressed = check_button_on_grid(BUTTON_COLUMNS_X[0], 0);
    if(runningPressed) {
      if(runningHere) {
        handleStop();
      } else {
        handleStart();
      }
    }
    
    if(runningPressed || runningChanged) {
      guiUpdateFlags |= drawTopButtons;
      if(trackLayerMode != LayerMode::LAYER_MUTE) {
        trackLayerMode = LayerMode::LAYER_MUTE;
        guiUpdateFlags |= drawLayerButtons;
      }
    }

    const bool recPressed = check_button_on_grid(BUTTON_COLUMNS_X[1], 0);
    if(recPressed) {
      liveSeqData->isRecording = !liveSeqData->isRecording;
      guiUpdateFlags |= (drawTopButtons | drawTrackButtons);
      trackLayerMode = LayerMode::LAYER_MUTE;

      if(isLayerView()) {
        guiUpdateFlags |= drawLayerButtons;
      }
    }

    const bool modePressed = check_button_on_grid(BUTTON_COLUMNS_X[5], 0);
    if(modePressed) {
      liveSeqData->isSongMode = !liveSeqData->isSongMode;
      if(liveSeqData->isSongMode) {
        liveSeqData->functionPageIndex = PageSong::PAGE_SONG_LAYERS;
      } else {
        liveSeqData->functionPageIndex = PagePattern::PAGE_PATT_LAYERS;
      }
      applyScreenRedrawGuiFlags();
    }

    const bool funcPressed = check_button_on_grid(BUTTON_COLUMNS_X[4], 0);
    if(funcPressed) {
      liveSeqData->functionPageIndex++;
      if(liveSeqData->isSongMode && liveSeqData->functionPageIndex == PageSong::PAGE_SONG_NUM) {
        liveSeqData->functionPageIndex = PageSong::PAGE_SONG_LAYERS;
      } else if(liveSeqData->isSongMode == false && liveSeqData->functionPageIndex == PagePattern::PAGE_PATT_NUM) {
        liveSeqData->functionPageIndex = PagePattern::PAGE_PATT_LAYERS;
      }
      applyScreenRedrawGuiFlags();
    }

    for(int track = 0; track < LIVESEQUENCER_NUM_TRACKS; track++) {
      const bool pressed = check_button_on_grid(BUTTON_COLUMNS_X[track], 5);
      if(pressed) {
        guiUpdateFlags |= drawTrackButtons;

        if(track == liveSeqData->activeTrack) {
          if(liveSeqData->isRecording) {
            if(liveSeqData->pendingEvents.size()) {
              liveSeqData->pendingEvents.clear(); // clear pending
            } else {
              if(++trackLayerMode == LayerMode::LAYER_MODE_NUM) {
                trackLayerMode = LayerMode::LAYER_MUTE;
              }
              if(liveSeqData->functionPageIndex == PagePattern::PAGE_PATT_LAYERS) {
                guiUpdateFlags |= drawLayerButtons;
              }
            }
          } else {
            // open instrument settings
            if(liveSeqData->tracks[track].screenSetupFn != nullptr) {
              void (*f)();
              f = (SetupFn)liveSeqData->tracks[track].screenSetupFn;
              f();
            }
            LCDML.FUNC_setGBAToLastFunc();
            LCDML.OTHER_jumpToFunc(liveSeqData->tracks[track].screen);
          }
        } else {
          liveSeqData->activeTrack = track;
          trackLayerMode = LayerMode::LAYER_MUTE;
          if(liveSeqData->functionPageIndex == PagePattern::PAGE_PATT_LAYERS) {
            guiUpdateFlags |= drawLayerButtons;
          }
          DBG_LOG(printf("rec track now is %i\n", track + 1));
        }
      }
      switch(liveSeqData->functionPageIndex) {
        case PageSong::PAGE_SONG_LAYERS:
        case PagePattern::PAGE_PATT_LAYERS:
          for(int layer = 0; layer < liveSeqData->tracks[track].layerCount; layer++) {
            const bool pressed = check_button_on_grid(BUTTON_COLUMNS_X[track], 10 + layer * 5);
            if(pressed) {
              if(liveSeqData->isRecording && (trackLayerMode != LayerMode::LAYER_MUTE) && (track == liveSeqData->activeTrack)) {
                liveSeqPtr->trackLayerAction(track, layer, LayerMode(trackLayerMode));
                trackLayerMode = LayerMode::LAYER_MUTE;
              } else {
                const bool isMutedOld =  liveSeqData->tracks[track].layerMutes & (1 << layer);
                const bool recordMuteToSong = liveSeqData->isSongMode && liveSeqData->isRecording && liveSeqData->isRunning;
                liveSeqPtr->setLayerMuted(track, layer, !isMutedOld, recordMuteToSong);
              }
              guiUpdateFlags |= (drawLayerButtons);
            }
          }
          break;

        case PagePattern::PAGE_PATT_SETINGS:
          for(int track = 0; track < LIVESEQUENCER_NUM_TRACKS; track++) {
            if(check_button_on_grid(BUTTON_COLUMNS_X[track], 10)) {
              liveSeqData->tracks[track].quantisizeDenom *= 2;
              if(liveSeqData->tracks[track].quantisizeDenom > 32) {
                liveSeqData->tracks[track].quantisizeDenom = 1;
              }
              guiUpdateFlags |= drawQuantisize;
            }
          }
          if(check_button_on_grid(BUTTON_COLUMNS_X[1], 15)) {
            // number of bars cycle through
            numberOfBarsTemp *= 2;
            if(numberOfBarsTemp > 8) {
              numberOfBarsTemp = 1;
            }
            guiUpdateFlags |= drawPattLength;
          }
          if(check_button_on_grid(BUTTON_COLUMNS_X[2], 15)) {
            // apply changed number of bars
            if(numberOfBarsTemp != liveSeqData->numberOfBars) {
              handleStop();
              liveSeqPtr->changeNumberOfBars(numberOfBarsTemp);
              guiUpdateFlags |= drawPattLength;
            }
          }
          if(check_button_on_grid(BUTTON_COLUMNS_X[2], 20)) {
            // fill number
            switch(liveSeqData->fillNotes.number) {
              case  4: liveSeqData->fillNotes.number =  6; break;
              case  6: liveSeqData->fillNotes.number =  8; break;
              case  8: liveSeqData->fillNotes.number = 12; break;
              case 12: liveSeqData->fillNotes.number = 16; break;
              case 16: liveSeqData->fillNotes.number = 24; break;
              case 24: liveSeqData->fillNotes.number = 32; break;
              case 32: liveSeqData->fillNotes.number =  8; break;
            }
            guiUpdateFlags |= drawFillNotes;
          }
          if(check_button_on_grid(BUTTON_COLUMNS_X[3], 20)) {
            // fill offset
            liveSeqData->fillNotes.offset++;
            if(liveSeqData->fillNotes.offset > 7) {
              liveSeqData->fillNotes.offset = 0;
            }
            guiUpdateFlags |= drawFillNotes;
          }
          if(check_button_on_grid(BUTTON_COLUMNS_X[4], 20)) {
            // fill now
            liveSeqPtr->fillTrackLayer();
            guiUpdateFlags |= drawFillNotes;
          }
          break;

        case PageSong::PAGE_SONG_AUTOMATIONS:
          if(check_button_on_grid(BUTTON_COLUMNS_X[0], 10)) {
            // song layer mode
            if(++songLayerMode == LayerMode::LAYER_MODE_NUM) {
              songLayerMode = LayerMode::LAYER_MUTE;
            }
            guiUpdateFlags |= drawSongAuto;
          }
          if(songLayerMode != LayerMode::LAYER_MUTE) { // song layers can not be muted
            for(uint8_t songLayer = 0; songLayer < liveSeqData->songLayerCount; songLayer++) {
              if(check_button_on_grid(BUTTON_COLUMNS_X[1], 10 + songLayer * 5)) {
                liveSeqPtr->songLayerAction(songLayer, LayerMode(songLayerMode));
                songLayerMode = LayerMode::LAYER_MUTE;
              }
            }
          }
          break;
      }
    }
  }
}

void drawGUI(uint16_t &guiFlags) {
  if(remote_active) {
    display.console = true;
  }
  if(guiFlags & drawTopButtons) {
    draw_button_on_grid(BUTTON_COLUMNS_X[0], 0, (runningHere ? "STOP" : "START"), "", runningHere ? 2 : 0);
    draw_button_on_grid(BUTTON_COLUMNS_X[1], 0, "REC", "", liveSeqData->isRecording ? 2 : 0);
    draw_button_on_grid(BUTTON_COLUMNS_X[4], 0, isLayerView() ? "LAYERS" : "TOOLS", "VIEW", 1);
    draw_button_on_grid(BUTTON_COLUMNS_X[5], 0, liveSeqData->isSongMode ? "SONG" : "PATT", "MODE", 3);
  }

  uint16_t patCount = 0;
  uint16_t timeMs = 0;
  if(runningHere) {
    patCount = liveSeqData->patternCount;
    timeMs = liveSeqData->patternTimer;
    
    if(liveSeqData->patternBeginFlag) {
      liveSeqData->patternBeginFlag = false;

      display.fillRect(110, 5, 90, 5, barPhases[0] ? GREEN : COLOR_BACKGROUND);
      barPhases[0] = !barPhases[0];
      if(liveSeqData->patternCount == 0) {
        display.fillRect(110, 10, 90, 5, barPhases[1] ? RED : COLOR_BACKGROUND);
        barPhases[1] = !barPhases[1];
      }
    } else {
      const float progressPattern = liveSeqData->patternTimer / float(liveSeqData->patternLengthMs);
      // fixme progress >1.0 when stopping
      const float progressTotal = std::min(1.0, (progressPattern + liveSeqData->patternCount) / float(liveSeqData->numberOfBars));

      display.fillRect(110, 5, progressPattern * 90, 5, barPhases[0] ? GREEN : COLOR_BACKGROUND);
      display.fillRect(110, 10, progressTotal * 90, 5, barPhases[1] ? RED : COLOR_BACKGROUND);
    }
  }

  // print time
  display.setCursor(110, 20);
  display.setTextSize(1);
  display.setTextColor(COLOR_SYSTEXT, COLOR_BACKGROUND);
  if(liveSeqData->isSongMode) {
    const uint32_t songMs = (liveSeqData->songPatternCount * liveSeqData->numberOfBars + liveSeqData->patternCount) * liveSeqData->patternLengthMs + timeMs;
    const uint32_t minutes = songMs / 60000;
    const uint32_t seconds = (songMs % 60000) / 1000;
    const uint32_t millis = songMs % 1000;
    display.printf("S %i:%02i.%03i", minutes, seconds, millis);
  } else {
    display.printf("P %i.%04i   ", patCount, timeMs);
  }
  display.setCursor(190, 20);
  if(liveSeqData->isSongMode) {
      display.printf("%02i", liveSeqData->songPatternCount);
  } else {
    display.printf("%02i", liveSeqData->patternCount);
  }

  char temp_char[4];

  uint8_t trackButtonRecColor = 2; // red, or blinking
  const bool doBlink = liveSeqData->notesOn.size() || liveSeqData->pendingEvents.size();
  if(doBlink) {
    if(++guiCounter == 8) {
      guiCounter = 0;
      trackButtonRecColor = blinkPhase ? 1 : 2;
      guiFlags |= drawTrackButtons;
      blinkPhase = !blinkPhase;
    }
  } else {
    guiCounter = 0;
    blinkPhase = 0;
    trackButtonRecColor = 2;
  }
  
  if(guiFlags & clearBottomArea) {
    const uint16_t bgColor = isLayerView() ? COLOR_BACKGROUND : GREY2; // gray for tools
    display.fillRect(0, 76, DISPLAY_WIDTH, DISPLAY_HEIGHT - 75, bgColor);
    DBG_LOG(printf("clear bottom\n"));
  }
  if(isLayerView() || (guiUpdateFlags & drawTrackButtons)) {
    const bool isSongRec = (liveSeqData->isSongMode && liveSeqData->isRecording);
    for(int track = 0; track < LIVESEQUENCER_NUM_TRACKS; track++) {
      if(guiFlags & drawTrackButtons) {
        draw_button_on_grid(BUTTON_COLUMNS_X[track], 5, liveSeqData->tracks[track].name, itoa(track + 1, temp_char, 10), (track == liveSeqData->activeTrack) ? ((liveSeqData->isSongMode == false) && liveSeqData->isRecording ? trackButtonRecColor : 3) : 1);
      }
      if(isLayerView()) {
        const bool layerEditActive = !liveSeqData->isSongMode && (liveSeqData->activeTrack == track) && (trackLayerMode != LayerMode::LAYER_MUTE);
        // layer button
        for(int layer = 0; layer < LIVESEQUENCER_NUM_LAYERS; layer++) {
          const int buttonY = 10 + layer * 5;
          if (layer < liveSeqData->tracks[track].layerCount) {
            const bool isMuted = liveSeqData->tracks[track].layerMutes & (1 << layer);
            uint16_t layerBgColor = (isMuted ? GREY2 : (isSongRec ? RED : DX_DARKCYAN));
            uint8_t layerBgCode = (isMuted ? 0 : (isSongRec ? 2 : 1));
            if(layerEditActive) {
              // adapt button background if in layer edit mode
              handleLayerEditButtonColor(trackLayerMode, layerBgColor, layerBgCode); 
            }
            if(guiFlags & drawLayerButtons) {
              drawLayerButton(trackLayerMode, layer, layerEditActive, layerBgCode, BUTTON_COLUMNS_X[track], buttonY);
            }
            // always draw notes when layers visible
            const int numNotesOn = liveSeqData->tracks[track].activeNotes[layer].size();
            const int xStart = (BUTTON_COLUMNS_X[track] + button_size_x) * CHAR_width_small - 3;
            const int yStart = (10 + layer * 5) * CHAR_height_small;
            const int yFill = std::min(numNotesOn * 5, BUTTON_HEIGHT);
            display.fillRect(xStart, yStart, 3, BUTTON_HEIGHT - yFill, layerBgColor);
            display.fillRect(xStart, yStart + (BUTTON_HEIGHT - yFill), 3, yFill, COLOR_SYSTEXT);
          } else {
            if (guiFlags & drawLayerButtons) {
              // clear button
              display.fillRect(BUTTON_COLUMNS_X[track] * CHAR_width_small, buttonY * CHAR_height_small, button_size_x * CHAR_width_small, CHAR_height_small * button_size_y, COLOR_BACKGROUND);
            }
          }
        }
      }
    }
  } 
  if(isLayerView() == false) {
    if(guiFlags & drawQuantisize) {
      // quantisize
      for(int track = 0; track < LIVESEQUENCER_NUM_TRACKS; track++) {
        const uint8_t denom = liveSeqData->tracks[track].quantisizeDenom;
        const std::string text = (denom == 1) ? "NONE" : itoa(denom, temp_char, 10);
        draw_button_on_grid(BUTTON_COLUMNS_X[track], 10, "QUANT", text.c_str(), (denom == 1) ? 1 : 3);
      }
    }

    if(guiFlags & drawPattLength) {
      // number of bars for one pattern
      draw_button_on_grid(BUTTON_COLUMNS_X[0], 15, "PATTERN", "LENGTH", 0); // label only
      draw_button_on_grid(BUTTON_COLUMNS_X[1], 15, "LENGTH", itoa(numberOfBarsTemp, temp_char, 10), 1);
      draw_button_on_grid(BUTTON_COLUMNS_X[2], 15, "APPLY", "NOW", (liveSeqData->numberOfBars == numberOfBarsTemp) ? 1 : 2);
      
      display.setTextSize(1);
      display.setTextColor((liveSeqData->numberOfBars == numberOfBarsTemp) ? COLOR_SYSTEXT : RED, GREY2);
      display.setCursor(165, 125);
      display.printf("Changing pattern length");
      display.setCursor(165, 135);
      display.printf("will delete all data!");
    }
    
    if(guiFlags & drawFillNotes) {
      // fill track
      draw_button_on_grid(BUTTON_COLUMNS_X[0], 20, "FILL", "NOTES", 0); // label only
      draw_button_on_grid(BUTTON_COLUMNS_X[1], 20, "NOTE", itoa(liveSeqData->lastPlayedNote, temp_char, 10), 0); // label only
      draw_button_on_grid(BUTTON_COLUMNS_X[2], 20, "NUM", itoa(liveSeqData->fillNotes.number, temp_char, 10), 3);
      draw_button_on_grid(BUTTON_COLUMNS_X[3], 20, "OFF", itoa(liveSeqData->fillNotes.offset, temp_char, 10), 3);
      draw_button_on_grid(BUTTON_COLUMNS_X[4], 20, "FILL", "NOW", 2);
    }

    if(guiFlags & drawSongAuto) {
      draw_button_on_grid(BUTTON_COLUMNS_X[0], 10, "LAYER", "ACTION", 2); // switch modes
      uint16_t layerBgColor;
      uint8_t layerBgCode;
      handleLayerEditButtonColor(songLayerMode, layerBgColor, layerBgCode);
      for(int songLayer = 0; songLayer < LIVESEQUENCER_NUM_LAYERS; songLayer++) {
        const int buttonY = 10 + songLayer * 5;
        if(songLayer < liveSeqData->songLayerCount) {
          drawLayerButton(songLayerMode, songLayer, true, layerBgCode, BUTTON_COLUMNS_X[1], buttonY);
        } else {
          // clear button
          display.fillRect(BUTTON_COLUMNS_X[1] * CHAR_width_small, buttonY * CHAR_height_small, button_size_x * CHAR_width_small, CHAR_height_small * button_size_y, GREY2);
        }
      }
    }
  }
  
  guiFlags = 0;
}

void drawLayerButton(uint8_t layerMode, int layer, const bool layerEditActive, uint8_t layerBgCode, uint8_t gridX, uint8_t gridY) {
  char temp_char[4];
  std::string label = "LAYER";
  std::string labelSub = itoa(layer + 1, temp_char, 10);
  if(layerEditActive) {
    switch (layerMode) {
    case LayerMode::LAYER_MERGE_UP:
      if (layer > 0) {
        label = "MERGE";
        labelSub = "^";
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

void handleLayerEditButtonColor(uint8_t layerMode, uint16_t &layerBgColor, uint8_t &layerBgCode) {
  switch(layerMode) {
    case LayerMode::LAYER_MERGE_UP:
      layerBgColor = MIDDLEGREEN;
      layerBgCode = 3;
      break;

    case LayerMode::LAYER_DELETE:
      layerBgColor = RED;
      layerBgCode = 2;
      break;
  }
}
