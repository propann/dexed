#ifndef UI_LIVESEQUENCER_H
#define UI_LIVESEQUENCER_H

#include <stdio.h>
#include "elapsedMillis.h"

class UI_LiveSequencer {

public:

  UI_LiveSequencer(bool *running, uint16_t *patCount, elapsedMillis *patTimer);

private:
  
};

void UI_func_livesequencer(uint8_t param);
void handle_touchscreen_live_sequencer(void);
void drawButtons(void);

#endif //UI_LIVESEQUENCER_H