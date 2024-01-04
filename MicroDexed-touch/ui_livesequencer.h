#ifndef UI_LIVESEQUENCER_H
#define UI_LIVESEQUENCER_H

#include <stdio.h>
#include "elapsedMillis.h"

#ifdef DEBUG
#define DBG_LOG(s) LOG.s
#else
#define DBG_LOG(s)
#endif

typedef void (*SetupFn)( void*);

class LiveSequencer;

static constexpr int LIVESEQUENCER_NUM_TRACKS = 6;
static constexpr int LIVESEQUENCER_NUM_LAYERS = 4;

class UI_LiveSequencer {

public:
  UI_LiveSequencer(LiveSequencer *sequencer);

private:

};

void UI_func_livesequencer(uint8_t param);
void handle_touchscreen_live_sequencer(void);

#endif //UI_LIVESEQUENCER_H