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

LiveSequencer *liveSeqPtr;
LiveSequencer::LiveSeqData *liveSeqData;
uint8_t guiCounter = 0;
bool blinkPhase = 0;

uint8_t trackLayerMode = LAYER_MUTE;

void applyScreenRedrawGuiFlags();

UI_LiveSequencer::UI_LiveSequencer(LiveSequencer *sequencer) {
  liveSeqPtr = sequencer;
  liveSeqData = sequencer->getData();
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
  updateAll         = 0xFF
};
uint16_t guiUpdateFlags = 0;

void drawGUI(uint16_t &guiFlags);

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
    if(isLayerView()) {
      guiUpdateFlags |= liveSeqData->trackLayersChanged ? (drawLayerButtons | drawTrackButtons) : 0;
    }
    if(liveSeqData->functionPageIndex == PagePattern::PAGE_PATT_SETINGS) {
      guiUpdateFlags |= liveSeqData->lastPlayedNoteChanged ? (drawFillNotes) : 0;
    }
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
  guiUpdateFlags |= clearBottomArea | drawTopButtons | drawTrackButtons;
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
      guiUpdateFlags |= (drawFillNotes | drawQuantisize);
      break;
    }
  }
}

void handle_touchscreen_live_sequencer(void) {  
  const bool runningChanged = (runningHere != liveSeqData->isRunning);

  if((numTouchPoints > 0) || runningChanged) {
    const bool runningPressed = check_button_on_grid(0, 0);
    if (runningPressed) {
      if(runningHere) {
        handleStop();
      } else {
        handleStart();
      }
    }
    if(runningChanged) {
      runningHere = liveSeqData->isRunning;
    }
    if(runningPressed || runningChanged) {
      guiUpdateFlags |= (drawTopButtons);
      trackLayerMode = TrackLayerMode::LAYER_MUTE;
    }

    const bool recPressed = check_button_on_grid(9, 0);
    if(recPressed) {
      liveSeqData->isRecording = !liveSeqData->isRecording;
      guiUpdateFlags |= (drawTopButtons | drawTrackButtons);
      trackLayerMode = TrackLayerMode::LAYER_MUTE;
      if(liveSeqData->isRecording) {
        if(liveSeqData->isSongMode) {
          if(liveSeqData->functionPageIndex != PageSong::PAGE_SONG_LAYERS) {
            liveSeqData->functionPageIndex = PageSong::PAGE_SONG_LAYERS;
            guiUpdateFlags |= clearBottomArea;
          }
        } else {
          if(liveSeqData->functionPageIndex != PagePattern::PAGE_PATT_LAYERS) {
            liveSeqData->functionPageIndex = PagePattern::PAGE_PATT_LAYERS;
            guiUpdateFlags |= clearBottomArea;
          }
        }
      }
      if(isLayerView()) {
        guiUpdateFlags |= drawLayerButtons;
      }
    }

    const bool modePressed = check_button_on_grid(45, 0);
    if(modePressed) {
      liveSeqData->isSongMode = !liveSeqData->isSongMode;
      if(liveSeqData->isSongMode) {
        liveSeqData->functionPageIndex = PageSong::PAGE_SONG_LAYERS;
      } else {
        liveSeqData->functionPageIndex = PagePattern::PAGE_PATT_LAYERS;
      }
       applyScreenRedrawGuiFlags();
    }

    const bool funcPressed = check_button_on_grid(36, 0);
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
      const int buttonX = track * 9;
      const bool pressed = check_button_on_grid(buttonX, 5);
      if(pressed) {
        guiUpdateFlags |= drawTrackButtons;

        if(track == liveSeqData->activeTrack) {
          if(liveSeqData->isRecording) {
            if(liveSeqData->pendingEvents.size()) {
              liveSeqData->pendingEvents.clear(); // clear pending
            } else {
              if(++trackLayerMode == TrackLayerMode::LAYER_MODE_NUM) {
                trackLayerMode = TrackLayerMode::LAYER_MUTE;
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
          trackLayerMode = TrackLayerMode::LAYER_MUTE;
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
            const bool pressed = check_button_on_grid(buttonX, 10 + layer * 5);
            if(pressed) {
              if(liveSeqData->isRecording && (trackLayerMode != TrackLayerMode::LAYER_MUTE) && (track == liveSeqData->activeTrack)) {
                liveSeqPtr->trackLayerAction(track, layer, TrackLayerMode(trackLayerMode));
                trackLayerMode = TrackLayerMode::LAYER_MUTE;
              } else {
                const bool isMutedOld =  liveSeqData->tracks[track].layerMutes & (1 << layer);
                liveSeqPtr->setLayerMuted(track, layer, !isMutedOld);
              }
              guiUpdateFlags |= (drawLayerButtons);
            }
          }
          break;

        case PagePattern::PAGE_PATT_SETINGS:
          for(int track = 0; track < LIVESEQUENCER_NUM_TRACKS; track++) {
            if(check_button_on_grid(track * 9, 10)) {
              liveSeqData->tracks[track].quantisizeDenom *= 2;
              if(liveSeqData->tracks[track].quantisizeDenom > 32) {
                liveSeqData->tracks[track].quantisizeDenom = 1;
              }
              guiUpdateFlags |= drawQuantisize;
            }
          }
          if(check_button_on_grid(9, 15)) {
            // fill now
            liveSeqPtr->fillTrackLayer();
            guiUpdateFlags |= drawFillNotes;
          }
          if(check_button_on_grid(18, 15)) {
            // fill number
            liveSeqData->fillNotes.number *= 2;
            if(liveSeqData->fillNotes.number > 32) {
              liveSeqData->fillNotes.number = 4;
            }
            guiUpdateFlags |= drawFillNotes;
          }
          if(check_button_on_grid(27, 15)) {
            // fill offset
            liveSeqData->fillNotes.offset++;
            if(liveSeqData->fillNotes.offset > 7) {
              liveSeqData->fillNotes.offset = 0;
            }
            guiUpdateFlags |= drawFillNotes;
          }
          break;

        case PageSong::PAGE_SONG_AUTOMATIONS:
          if(check_button_on_grid(0, 10)) {
            liveSeqPtr->deleteAllAutomations();
            guiUpdateFlags |= (drawSongAuto | clearBottomArea);

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
    draw_button_on_grid(0, 0, (runningHere ? "STOP" : "START"), "", runningHere ? 2 : 0);
    draw_button_on_grid(9, 0, "REC", "", liveSeqData->isRecording ? 2 : 0);
    draw_button_on_grid(36, 0, isLayerView() ? "LAYERS" : "TOOLS", "VIEW", 1);
    draw_button_on_grid(45, 0, liveSeqData->isSongMode ? "SONG" : "PATT", "MODE", 3);
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
      float progressPattern = liveSeqData->patternTimer / float(liveSeqData->patternLengthMs);
      float progressTotal = (liveSeqData->patternCount * liveSeqData->patternLengthMs + liveSeqData->patternTimer) / float(liveSeqData->numberOfBars * liveSeqData->patternLengthMs);
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
  display.printf("%02i", liveSeqData->songPatternCount);

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
    display.fillRect(0, 75, DISPLAY_WIDTH, DISPLAY_HEIGHT - 75, COLOR_BACKGROUND);
    DBG_LOG(printf("clear bottom\n"));
  }
  if(isLayerView()) {
    const bool isSongRec = (liveSeqData->isSongMode && liveSeqData->isRecording);
    for(int track = 0; track < LIVESEQUENCER_NUM_TRACKS; track++) {
      const int buttonX = track * 9;
      if(guiFlags & drawTrackButtons) {
        draw_button_on_grid(buttonX, 5, liveSeqData->tracks[track].name, itoa(track + 1, temp_char, 10), !liveSeqData->isSongMode && (track == liveSeqData->activeTrack) ? (liveSeqData->isRecording ? trackButtonRecColor : 3) : 1);
      }
      const bool layerEditActive = !liveSeqData->isSongMode && (liveSeqData->activeTrack == track) && (trackLayerMode != TrackLayerMode::LAYER_MUTE);
      // layer button
      for(int layer = 0; layer < LIVESEQUENCER_NUM_LAYERS; layer++) {
        const int buttonY = 10 + layer * 5;
        if(layer < liveSeqData->tracks[track].layerCount) {
          const bool isMuted = liveSeqData->tracks[track].layerMutes & (1 << layer);
          uint16_t layerBgColor = (isMuted ? GREY2 : (isSongRec ? MIDDLEGREEN : DX_DARKCYAN));
          uint8_t layerBgCode = (isMuted ? 0 : (isSongRec ? 3 : 1));
          std::string label = "LAYER";
          std::string labelSub = itoa(layer + 1, temp_char, 10);
          if(layerEditActive) {
            switch(trackLayerMode) {
            case TrackLayerMode::LAYER_MERGE_UP:
              layerBgColor = MIDDLEGREEN;
              layerBgCode = 3;
              if(layer > 0) {
                label = "MERGE";
                labelSub = "^";
              }
              break;
            case TrackLayerMode::LAYER_DELETE:
              layerBgColor = RED;
              layerBgCode = 2;
              label = "DELETE";
              labelSub = "x";
              break;
            }
          }
          if(guiFlags & drawLayerButtons) {
            draw_button_on_grid(buttonX, buttonY, label.c_str(), labelSub.c_str(), layerBgCode);
          }
          // always draw notes when layers visible
          const int numNotesOn = liveSeqData->tracks[track].activeNotes[layer].size();
          const int xStart = (buttonX + button_size_x) * CHAR_width_small - 3;
          const int yStart = (10 + layer * 5) * CHAR_height_small;
          const int yFill = std::min(numNotesOn * 5, BUTTON_HEIGHT);
          display.fillRect(xStart, yStart, 3, BUTTON_HEIGHT - yFill, layerBgColor);
          display.fillRect(xStart, yStart + (BUTTON_HEIGHT - yFill), 3, yFill, COLOR_SYSTEXT);
        } else {
          if(guiFlags & drawLayerButtons) {
            // no button
            draw_button_on_grid(buttonX, buttonY, "", "", 98); // clear button
          }
        }
      }
    }
  } else {
    if(guiFlags & drawQuantisize) {
      // quantisize
      for(int track = 0; track < LIVESEQUENCER_NUM_TRACKS; track++) {
        const uint8_t denom = liveSeqData->tracks[track].quantisizeDenom;
        const std::string text = (denom == 1) ? "NONE" : itoa(denom, temp_char, 10);
        draw_button_on_grid(track * 9, 10, "QUANT", text.c_str(), (denom == 1) ? 0 : 3);
      }
    }
    
    if(guiFlags & drawFillNotes) {
      // fill track
      display.setTextSize(2);
      display.setTextColor(COLOR_SYSTEXT, COLOR_BACKGROUND);
      display.setCursor(0, 17 * CHAR_height_small);
      display.printf("FILL");
      draw_button_on_grid(9, 15, "Note", itoa(liveSeqData->lastPlayedNote, temp_char, 10), 2);
      draw_button_on_grid(18, 15, "Num", itoa(liveSeqData->fillNotes.number, temp_char, 10), 3);
      draw_button_on_grid(27, 15, "Off", itoa(liveSeqData->fillNotes.offset, temp_char, 10), 3);    
    }

    if(guiFlags & drawSongAuto) {
      draw_button_on_grid(0, 10, "Clear", "ALL", 2);
      uint8_t y = 120;
      uint8_t line = 0;
      display.setTextColor(COLOR_SYSTEXT, COLOR_BACKGROUND);
      display.setTextSize(1);
      for(auto i : liveSeqData->songAutomations) {
        for(auto t : i.second) {
          display.setCursor(0, y + line * CHAR_height_small);
          display.printf("%02i %02i %04i %s", i.first, t.patternNumber, t.patternMs, t.note_in_velocity ? "MUTE" : "UNMUTE");
          line++;
        }
      }
    }
  }
  
  guiFlags = 0;
}
