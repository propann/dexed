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

bool runningHere = false;
bool barPhases[2] = { 0 };

LiveSequencer *liveSeqPtr;
LiveSequencer::LiveSeqData *liveSeqData;

UI_LiveSequencer::UI_LiveSequencer(LiveSequencer *sequencer) {
  liveSeqPtr = sequencer;
  liveSeqData = sequencer->getData();
}

void drawButtons();
void drawTrackLayers();

static constexpr int NUM_TRACKS = 6;

void UI_func_livesequencer(uint8_t param)
{

  // ****** SETUP *********
  if (LCDML.FUNC_setup()) {

    display.fillScreen(COLOR_BACKGROUND);
    liveSeqPtr->updateTrackChannels();
    barPhases[0] = 0;
    barPhases[1] = 0;
    drawButtons();
  }
  // ****** LOOP *********
  if (LCDML.FUNC_loop()) {
    
  }
  
  // ****** STABLE END *********
  if (LCDML.FUNC_close()) {
    liveSeqData->isRecording = false;
    display.fillScreen(COLOR_BACKGROUND);
  }
}

void handle_touchscreen_live_sequencer(void) {
  bool buttonsChanged = false;
  bool runningPressed = check_button_on_grid(0, 0);
  bool runningChanged = (runningHere != liveSeqData->isRunning);

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
    buttonsChanged = true;
  }

  bool recPressed = check_button_on_grid(9, 0);
  if(recPressed) {
    liveSeqData->isRecording = !liveSeqData->isRecording;
    buttonsChanged = true;
  }

  for(int i = 0; i < NUM_TRACKS; i++) {
    int x = i * 9;
    bool pressed = check_button_on_grid(x, 5);
    if(pressed) {
      if(i == liveSeqData->activeRecordingTrack) {
        if(liveSeqData->isRecording) {
          liveSeqPtr->clearLastTrackLayer(i);
        } else {
          // open instrument settings
          if(liveSeqData->tracks[i].screenSetupFn != nullptr) {
            void (*f)();
            f = (SetupFn)liveSeqData->tracks[i].screenSetupFn;
            f();
          }
          LCDML.FUNC_setGBAToLastFunc();
          LCDML.OTHER_jumpToFunc(liveSeqData->tracks[i].screen);
        }
      } else {
        liveSeqData->activeRecordingTrack = i;
        LOG.printf("rec track now is %i\n", i + 1);
      }
      
      buttonsChanged = true;
    }

    for(int y = 0; y < liveSeqData->tracks[i].layerCount; y++) {
      bool pressed = check_button_on_grid(x, 10 + y * 5);
      if(pressed) {
        uint8_t layerMask = (1 << y);
        if(liveSeqData->tracks[i].layerMutes & layerMask) {
          liveSeqData->tracks[i].layerMutes &= ~layerMask;
        } else {
          liveSeqData->tracks[i].layerMutes |= layerMask;
        }
        buttonsChanged = true;
      }
    }
  }

  uint16_t patCount = 0;
  uint16_t timeMs = 0;
  if(runningHere) {
    patCount = liveSeqData->patternCount;
    timeMs = liveSeqData->patternTimer;
    
    if(liveSeqData->patternBeginFlag) {
      liveSeqData->patternBeginFlag = false;

      display.fillRect(115, 5, 100, 5, barPhases[0] ? GREEN : COLOR_BACKGROUND);
      barPhases[0] = !barPhases[0];
      if(liveSeqData->patternCount == 0) {
        display.fillRect(115, 10, 100, 5, barPhases[1] ? RED : COLOR_BACKGROUND);
        barPhases[1] = !barPhases[1];
      }
    } else {
      float progressPattern = liveSeqData->patternTimer / float(liveSeqData->patternLengthMs);
      float progressTotal = (liveSeqData->patternCount * liveSeqData->patternLengthMs + liveSeqData->patternTimer) / float(liveSeqData->numberOfBars * liveSeqData->patternLengthMs);
      display.fillRect(115, 5, progressPattern * 100, 5, barPhases[0] ? GREEN : COLOR_BACKGROUND);
      display.fillRect(115, 10, progressTotal * 100, 5, barPhases[1] ? RED : COLOR_BACKGROUND);
    }
  }
  display.setCursor(115, 20);
  display.setTextSize(1);
  display.setTextColor(COLOR_SYSTEXT, COLOR_BACKGROUND);
  display.printf("Time: %i.%04i", patCount, timeMs);

  if(liveSeqData->trackLayersChanged || buttonsChanged) {
    liveSeqData->trackLayersChanged = false;
    drawButtons();
  }
}

void drawButtons() {
  draw_button_on_grid(0, 0, (runningHere ? "STOP" : "START"), "", runningHere ? 2 : 0);
  draw_button_on_grid(9, 0, "REC", "", liveSeqData->isRecording ? 2 : 0);

  char temp_char[4];
  for(int i = 0; i < NUM_TRACKS; i++) {
    int x = i * 9;
    draw_button_on_grid(x, 5, liveSeqData->tracks[i].name, itoa(i + 1, temp_char, 10), (i == liveSeqData->activeRecordingTrack) ? (liveSeqData->isRecording ? 2 : 3) : (liveSeqData->tracks[i].layerMutes ? 0 : 1));

    // layer button
    for(int y = 0; y < liveSeqData->tracks[i].layerCount; y++) {
      draw_button_on_grid(x, 10 + y * 5, "LAYER", itoa(y + 1, temp_char, 10), liveSeqData->tracks[i].layerMutes & (1 << y) ? 0 : 1);
    }
    // no button
    for(int y = liveSeqData->tracks[i].layerCount; y < 4; y++) {
      draw_button_on_grid(x, 10 + y * 5, "", "", 98); // clear button
    }
  }
}
