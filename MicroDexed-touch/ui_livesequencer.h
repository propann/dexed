#ifndef UI_LIVESEQUENCER_H
#define UI_LIVESEQUENCER_H

#include <stdio.h>
#include "elapsedMillis.h"

#ifdef DEBUG
#define DBG_LOG(s) LOG.s
#else
#define DBG_LOG(s)
#endif

typedef void (*SetupFn)(void*);

class LiveSequencer;

static constexpr int LIVESEQUENCER_NUM_TRACKS = 6;
static constexpr int LIVESEQUENCER_NUM_LAYERS = 4;

enum LayerMode {
  LAYER_MUTE = 0,
  LAYER_MERGE,
  LAYER_DELETE,
  LAYER_MODE_NUM
};

enum PagePattern {
  PAGE_PATT_LAYERS = 0,
  PAGE_PATT_SETINGS,
  PAGE_PATT_NUM
};

enum PageSong {
  PAGE_SONG_LAYERS = PAGE_PATT_NUM + 1,
  PAGE_SONG_SETTINGS,
  PAGE_SONG_NUM
};

class UI_LiveSequencer {

public:
  UI_LiveSequencer(LiveSequencer *sequencer);
  void showHowTo(uint8_t inChannel);
};

void UI_func_livesequencer(uint8_t param);
void handle_touchscreen_live_sequencer(void);

#endif //UI_LIVESEQUENCER_H