#include "ui_livesequencer.h"
#include "LCDMenuLib2.h"
#include "ILI9341_t3n.h"
#include "sequencer.h"

extern LCDMenuLib2 LCDML;
extern ILI9341_t3n display;
extern void draw_button_on_grid(uint8_t x, uint8_t y, const char* t1, const char* t2, uint8_t color);
extern bool check_button_on_grid(uint8_t x, uint8_t y);
extern void handleStart();
extern void handleStop();
extern sequencer_t seq;

uint16_t *patternCount;
elapsedMillis *patternTimer;
bool *isRunning;
bool runningHere = false;

UI_LiveSequencer::UI_LiveSequencer(bool *running, uint16_t *patCount, elapsedMillis *patTimer) {
  patternCount = patCount;
  patternTimer = patTimer;
  isRunning = running;
}

static constexpr int NUM_TRACKS = 6;
bool buttonStates[NUM_TRACKS] = { false };

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
  bool updateButtons = (*isRunning != runningHere);
  runningHere = *isRunning;

  if (check_button_on_grid(0, 0)) {
    if(runningHere) {
      handleStop();
    } else {
      handleStart();
    }
    updateButtons = true;
  }

  for(int i = 0; i < NUM_TRACKS; i++) {
    if (check_button_on_grid(i * 9, 5)) {
      buttonStates[i] = !buttonStates[i];
      updateButtons = true;
    }
  }
  if(updateButtons) {
    drawButtons();
  }

  display.setCursor(1, 100);
  display.setTextSize(1);
  uint16_t patCount = 0;
  uint16_t timeMs = 0;
  if(runningHere) {
    patCount = *patternCount;
    timeMs = *patternTimer;
    
  }
  display.printf("Time: %i.%04i", patCount, timeMs);
}

void drawButtons(void) {
  char temp_char[4];
  draw_button_on_grid(0, 0, (runningHere ? "STOP" : "START"), "", runningHere ? 2 : 0);
 
  for(int i = 0; i < NUM_TRACKS; i++) {
    draw_button_on_grid(i * 9, 5, "TRACK", itoa(i + 1, temp_char, 10), buttonStates[i] ? 1 : 0);
  }
}
