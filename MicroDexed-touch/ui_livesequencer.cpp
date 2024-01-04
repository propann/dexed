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
extern Realtime_Scope scope;

extern int numTouchPoints;

static constexpr int BUTTON_HEIGHT = CHAR_height_small * button_size_y;
static constexpr int BUTTON_WIDTH = CHAR_width_small * button_size_x;

bool runningHere = false;
bool barPhases[2] = { 0 };

LiveSequencer *liveSeqPtr;
LiveSequencer::LiveSeqData *liveSeqData;
uint8_t guiCounter = 0;
bool blinkPhase = 0;
bool enableLayerDelete = false;

UI_LiveSequencer::UI_LiveSequencer(LiveSequencer *sequencer) {
  liveSeqPtr = sequencer;
  liveSeqData = sequencer->getData();
}

struct GuiUpdateFlags {
  bool drawTopButtons;
  bool drawTrackButtons;
  bool drawLayerButtons;
  bool drawFillNotes;
  bool clearBottomArea;
} guiFlags = { false };

void drawGUI(GuiUpdateFlags &guiFlags);
void UI_func_livesequencer(uint8_t param)
{

  // ****** SETUP *********
  if (LCDML.FUNC_setup()) {

    display.fillScreen(COLOR_BACKGROUND);
    liveSeqPtr->init();

    barPhases[0] = 0;
    barPhases[1] = 0;
    GuiUpdateFlags init = { true, true, true, true, true };
    drawGUI(init);
    // setup function
    LCDML.FUNC_setLoopInterval(40); // 25Hz gui refresh
  }
  // ****** LOOP *********
  if (LCDML.FUNC_loop()) {
    guiFlags.drawLayerButtons |= liveSeqData->trackLayersChanged;
    guiFlags.drawTrackButtons |= liveSeqData->trackLayersChanged;
    liveSeqData->trackLayersChanged = false;
    guiFlags.drawFillNotes |= liveSeqData->lastPlayedNoteChanged;
    liveSeqData->lastPlayedNoteChanged = false;
    drawGUI(guiFlags);
  }
  
  // ****** STABLE END *********
  if (LCDML.FUNC_close()) {
    liveSeqData->isRecording = false;
    display.fillScreen(COLOR_BACKGROUND);
  }
}

void handle_touchscreen_live_sequencer(void) {
  bool runningChanged = (runningHere != liveSeqData->isRunning);
  guiFlags.drawLayerButtons |= runningChanged;

  if((numTouchPoints > 0) || runningChanged) {
    bool runningPressed = check_button_on_grid(0, 0);

    //scope.draw_scope(220, -20, 90); // rly?
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
      guiFlags.drawTopButtons = true;
      guiFlags.clearBottomArea = true;
    }

    bool recPressed = check_button_on_grid(9, 0);
    if(recPressed) {
      liveSeqData->isRecording = !liveSeqData->isRecording;
      guiFlags.drawTopButtons = true;
      guiFlags.drawTrackButtons = true;
    }

    for(int track = 0; track < LIVESEQUENCER_NUM_TRACKS; track++) {
      int buttonX = track * 9;
      bool pressed = check_button_on_grid(buttonX, 5);
      if(pressed) {
        if(track == liveSeqData->activeTrack) {
          if(liveSeqData->isRecording) {
            if(liveSeqData->pendingEvents.size()) {
              liveSeqPtr->clearTrackLayer(track, 255); // pending events
            } else {
              enableLayerDelete = !enableLayerDelete;
              guiFlags.drawLayerButtons = true;
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
          enableLayerDelete = false;
          guiFlags.drawLayerButtons = true;
          DBG_LOG(printf("rec track now is %i\n", track + 1));
        }
        guiFlags.drawTrackButtons = true;
      }
      if(liveSeqData->isRunning) {
        for(int layer = 0; layer < liveSeqData->tracks[track].layerCount; layer++) {
          const bool pressed = check_button_on_grid(buttonX, 10 + layer * 5);
          if(pressed) {
            if(enableLayerDelete && liveSeqData->isRecording && (track == liveSeqData->activeTrack)) {
              liveSeqPtr->clearTrackLayer(track, layer);
            } else {
              uint8_t layerMask = (1 << layer);
              const bool isMuted = liveSeqData->tracks[track].layerMutes & layerMask;
              if(isMuted) {
                liveSeqData->tracks[track].layerMutes &= ~layerMask;
              } else {
                liveSeqData->tracks[track].layerMutes |= layerMask;
              }
              liveSeqPtr->handleLayerMuteChanged(track, layer, !isMuted);
            }
            guiFlags.drawLayerButtons = true;
            guiFlags.drawTrackButtons = true;
          }
        }
      } else {
        if(check_button_on_grid(9, 10)) {
          guiFlags.drawFillNotes = true;
          // fill number
          liveSeqData->fillNotes.number *= 2;
          if(liveSeqData->fillNotes.number > 32) {
            liveSeqData->fillNotes.number = 4;
          }
        }
        if(check_button_on_grid(18, 10)) {
          guiFlags.drawFillNotes = true;
          // fill offset
          liveSeqData->fillNotes.offset++;
          if(liveSeqData->fillNotes.offset > 7) {
            liveSeqData->fillNotes.offset = 0;
          }
        }
        if(check_button_on_grid(0, 10)) {
          guiFlags.drawFillNotes = true;
          // fill now
          liveSeqPtr->fillTrackLayer();
        }
      }
    }
  }
}

void drawGUI(GuiUpdateFlags &guiFlags) {
  if(guiFlags.drawTopButtons) {
    draw_button_on_grid(0, 0, (runningHere ? "STOP" : "START"), "", runningHere ? 2 : 0);
    draw_button_on_grid(9, 0, "REC", "", liveSeqData->isRecording ? 2 : 0);
    draw_button_on_grid(36, 0, "ABC", "", 1);
    draw_button_on_grid(45, 0, "DSR", "", 3);
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
    
    // print time
    display.setCursor(110, 20);
    display.setTextSize(1);
    display.setTextColor(COLOR_SYSTEXT, COLOR_BACKGROUND);
    display.printf("%i.%04i", patCount, timeMs);
  }

  uint8_t trackButtonRecColor = 2; // red, or blinking
  if(liveSeqData->pendingEvents.size()) {
    if(++guiCounter == 8) {
      guiCounter = 0;
      trackButtonRecColor = (liveSeqData->pendingEvents.size() && blinkPhase) ? 1 : 2;
      guiFlags.drawTrackButtons = true;
      blinkPhase = !blinkPhase;
    }
  } else {
    guiCounter = 0;
    blinkPhase = 0;
    trackButtonRecColor = 2;
  }
  
  const bool showLayers = liveSeqData->isRunning;
  if(guiFlags.clearBottomArea) {
    display.fillRect(0, 75, DISPLAY_WIDTH, DISPLAY_HEIGHT - 75, COLOR_BACKGROUND);
    guiFlags.drawFillNotes = true;
    guiFlags.drawLayerButtons = true;
  }
  
  char temp_char[4];
  for(int track = 0; track < LIVESEQUENCER_NUM_TRACKS; track++) {
    const int buttonX = track * 9;
    if(guiFlags.drawTrackButtons) {
      draw_button_on_grid(buttonX, 5, liveSeqData->tracks[track].name, itoa(track + 1, temp_char, 10), (track == liveSeqData->activeTrack) ? (liveSeqData->isRecording ? trackButtonRecColor : 3) : 1);
    }

    if(showLayers) {
      bool layerDeleteActive = (liveSeqData->activeTrack == track) && enableLayerDelete;
      // layer button
      for(int layer = 0; layer < LIVESEQUENCER_NUM_LAYERS; layer++) {
        const int buttonY = 10 + layer * 5;
        if(layer < liveSeqData->tracks[track].layerCount) {
          const bool isMuted = liveSeqData->tracks[track].layerMutes & (1 << layer);
          if(guiFlags.drawLayerButtons) {
            draw_button_on_grid(buttonX, buttonY, "LAYER", itoa(layer + 1, temp_char, 10), layerDeleteActive ? 2 : (isMuted ? 0 : 1));
          }
          const int numNotesOn = liveSeqData->tracks[track].activeNotes[layer].size();
          const int xStart = (buttonX + button_size_x) * CHAR_width_small - 3;
          const int yStart = (10 + layer * 5) * CHAR_height_small;
          const int yFill = std::min(numNotesOn * 5, BUTTON_HEIGHT);
          display.fillRect(xStart, yStart, 3, BUTTON_HEIGHT - yFill, layerDeleteActive ? RED : (isMuted ? GREY2 : DX_DARKCYAN));
          display.fillRect(xStart, yStart + (BUTTON_HEIGHT - yFill), 3, yFill, COLOR_SYSTEXT);
        } else {
          if(guiFlags.drawLayerButtons) {
            // no button
            draw_button_on_grid(buttonX, buttonY, "", "", 98); // clear button
          }
        }
      }
    }
  }
  if((showLayers == false) && guiFlags.drawFillNotes) {
    // FILL label
    draw_button_on_grid(0, 10, "Note", itoa(liveSeqData->lastPlayedNote, temp_char, 10), 2);
    draw_button_on_grid(9, 10, "Num", itoa(liveSeqData->fillNotes.number, temp_char, 10), 3);
    draw_button_on_grid(18, 10, "Off", itoa(liveSeqData->fillNotes.offset, temp_char, 10), 3);
  }
  guiFlags.drawTopButtons = false;
  guiFlags.drawTrackButtons = false;
  guiFlags.drawLayerButtons = false;
  guiFlags.drawFillNotes = false;
  guiFlags.clearBottomArea = false;
}
