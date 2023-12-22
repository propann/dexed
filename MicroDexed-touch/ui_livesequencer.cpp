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
bool flushBars = false;
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
  scope.draw_scope(220, -20, 90);
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
      if(liveSeqData->isRecording) {
        if(liveSeqData->activeRecordingTrack == i) {
          liveSeqPtr->clearLastTrackLayer(i);
        } else {
          liveSeqData->activeRecordingTrack = i;
          Serial.printf("rec track now is %i\n", i + 1);
        }
      } else {
        liveSeqData->trackMutes[i] = !liveSeqData->trackMutes[i]; // fixme now
      }
      buttonsChanged = true;
    }

    for(int y = 0; y < liveSeqData->trackLayers[i]; y++) {
      bool pressed = check_button_on_grid(x, 10 + y * 5);
      if(pressed) {
        if(liveSeqData->trackMutes[i] & (1 << y)) {
          liveSeqData->trackMutes[i] &= ~(1 << y);
        } else {
          liveSeqData->trackMutes[i] |= (1 << y);
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
    // bar
    //display.drawRect(110, 0, 200, 30, GREY1);
    //float progress = liveSeqData->patternTimer / liveSeqData->patternLengthMs;
    if(liveSeqData->patternBeginFlag) {
      liveSeqData->patternBeginFlag = false;

      barPhases[0] = !barPhases[0];
      if(liveSeqData->patternCount == 0) {
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
    draw_button_on_grid(x, 5, "TRACK", itoa(i + 1, temp_char, 10), liveSeqData->isRecording && (i == liveSeqData->activeRecordingTrack) ? 2 : (liveSeqData->trackMutes[i] ? 0 : 1));

    // layer button
    for(int y = 0; y < liveSeqData->trackLayers[i]; y++) {
      draw_button_on_grid(x, 10 + y * 5, "LAYER", itoa(y + 1, temp_char, 10), liveSeqData->trackMutes[i] & (1 << y) ? 0 : 1);
    }
    // no button
    for(int y = liveSeqData->trackLayers[i]; y < 4; y++) {
      draw_button_on_grid(x, 10 + y * 5, "", "", 98); // clear button
    }
  }
}
