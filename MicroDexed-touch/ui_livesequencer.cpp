#include "ui_livesequencer.h"
#include "LCDMenuLib2.h"
#include "ILI9341_t3n.h"
#include "sequencer.h"

#include "livesequencer.h"

extern LCDMenuLib2 LCDML;
extern ILI9341_t3n display;
extern void draw_button_on_grid(uint8_t x, uint8_t y, const char* t1, const char* t2, uint8_t color);
extern bool check_button_on_grid(uint8_t x, uint8_t y);
extern void handleStart();
extern void handleStop();

bool runningHere = false;

LiveSequencer::LiveSeqData *liveSeqData;

UI_LiveSequencer::UI_LiveSequencer(void *ldata) {
  liveSeqData = ldata;
}

void drawButtons();

static constexpr int NUM_TRACKS = 6;
bool trackEnabled[NUM_TRACKS] = { false };

void UI_func_livesequencer(uint8_t param)
{

  // ****** SETUP *********
  if (LCDML.FUNC_setup()) {

    display.fillScreen(COLOR_BACKGROUND);
    drawButtons();
  }
  // ****** LOOP *********
  if (LCDML.FUNC_loop()) {
    
  }
  
  // ****** STABLE END *********
  if (LCDML.FUNC_close()) {
    display.fillScreen(COLOR_BACKGROUND);
  }
}

void handle_touchscreen_live_sequencer(void) {
  bool buttonsChanged = false;

  bool runningPressed = check_button_on_grid(0, 0);
  bool runningChanged = (runningHere != liveSeqData->isRunning);
  
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
    bool pressed = check_button_on_grid(i * 9, 5);
    if(pressed) {
      if(liveSeqData->isRecording) {
        liveSeqData->activeRecordingTrack = i;
        Serial.printf("rec track now is %i\n", i + 1);
      } else {
        trackEnabled[i] = !trackEnabled[i];
      }
      buttonsChanged = true;
    }
  }

  display.setCursor(1, 100);
  display.setTextSize(1);
  uint16_t patCount = 0;
  uint16_t timeMs = 0;
  if(runningHere) {
    patCount = liveSeqData->patternCount;
    timeMs = liveSeqData->patternTimer;
  }
  display.setTextColor(COLOR_SYSTEXT, COLOR_BACKGROUND);
  display.printf("Time: %i.%04i", patCount, timeMs);

  if(buttonsChanged) {
    drawButtons();
  }
}

void drawButtons() {
  draw_button_on_grid(0, 0, (runningHere ? "STOP" : "START"), "", runningHere ? 2 : 0);
  draw_button_on_grid(9, 0, "REC", "", liveSeqData->isRecording ? 2 : 0);

  char temp_char[4];
  for(int i = 0; i < NUM_TRACKS; i++) {
    draw_button_on_grid(i * 9, 5, "TRACK", itoa(i + 1, temp_char, 10), liveSeqData->isRecording && (i == liveSeqData->activeRecordingTrack) ? 2 : (trackEnabled[i] ? 1 : 0));
  }
}