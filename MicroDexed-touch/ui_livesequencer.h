#ifndef UI_LIVESEQUENCER_H
#define UI_LIVESEQUENCER_H

#include <stdio.h>
#include "editableValue.h"
#include "touchbutton.h"
#include "valuebutton.h"

#ifdef DEBUG
#define DBG_LOG(s) LOG.s
#else
#define DBG_LOG(s)
#endif

typedef void (*SetupFn)(void*);

class LiveSequencer;

class UI_LiveSequencer {
public:
  UI_LiveSequencer(LiveSequencer *sequencer);
  void showDirectMappingWarning(uint8_t inChannel);
  void processLCDM(void);
  void handleTouchscreen(void);

  enum LayerMode {
    LAYER_MUTE = 0,
    LAYER_MERGE,
    LAYER_DELETE,
    LAYER_MODE_NUM
  };

private:
  int numPressedOld = 0;
  bool initialized = false;
  bool runningHere = false;
  bool barPhases[2] = { 0 };
  uint8_t numberOfBarsTemp = 0;
  bool deleteConfirming = false;
  bool showingHowTo = false;

  bool isLayerViewActive = false;
  uint16_t guiUpdateFlags = 0;
  bool stayActive = false; // LiveSequencer stays active in instrument settings opened from here

  EditableValue<uint8_t> *fillNum;
  EditableValue<uint8_t> *fillOff;

  EditableValue<uint8_t> *arpAmount;
  EditableValue<uint16_t> *arpLength;
  EditableValue<uint8_t> *arpMode;
  EditableValue<int8_t> *arpSwing;
  EditableValue<uint8_t> *arpLatch;
  EditableValue<uint8_t> *songMuteQuant;
  EditableValue<uint8_t> *numBarsTemp;
  EditableValueBase *currentValue;

  ValueButton<uint8_t> *buttonPatternLength;

  LiveSequencer* liveSeqPtr;
  uint8_t guiCounter = 0;
  bool blinkPhase = 0;
  
  uint8_t trackLayerMode = LAYER_MUTE;
  uint8_t songLayerMode = LAYER_MUTE;

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

  enum ShowingTools : uint8_t {
    TOOL_FILL,
    TOOL_ARP
  } showingTools = TOOL_FILL;

  enum GuiUpdates : uint16_t {
    drawTopButtons = (1 << 0),
    drawTrackButtons = (1 << 1),
    drawLayerButtons = (1 << 2),
    drawTools = (1 << 3),
    drawQuantisize = (1 << 4),
    clearBottomArea = (1 << 5),
    drawSongSettings = (1 << 6),
    drawPattLength = (1 << 7),
    drawActiveNotes = (1 << 8),
    drawDeleteAll = (1 << 9),
    drawDeleteSong = (1 << 10),
    drawSongQuant = (1 << 11),
    drawTime = (1 << 12)
  };

  void applyScreenRedrawGuiFlags(void);
  void drawGUI(uint16_t& guiFlags);
  void getArpModeName(uint8_t mode, char* buf);
  void drawLayerButton(const bool horizontal, uint8_t layerMode, int layer, const bool layerEditActive, uint8_t layerBgCode, uint8_t gridX, uint8_t gridY);
  void handleLayerEditButtonColor(uint8_t layerMode, uint16_t& layerBgColor, uint8_t& layerBgCode);
};

void UI_func_livesequencer(uint8_t param);
void handle_touchscreen_live_sequencer(void);

#endif //UI_LIVESEQUENCER_H