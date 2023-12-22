#ifndef UI_LIVESEQUENCER_H
#define UI_LIVESEQUENCER_H

#include <stdio.h>
#include "elapsedMillis.h"

class LiveSequencer;

class UI_LiveSequencer {

public:
  UI_LiveSequencer(LiveSequencer *sequencer);

private:
};

void UI_func_livesequencer(uint8_t param);
void handle_touchscreen_live_sequencer(void);

#endif //UI_LIVESEQUENCER_H