#ifndef UI_LIVESEQUENCER_H
#define UI_LIVESEQUENCER_H

#include <stdio.h>
#include "editableValue.h"
#include "touchbutton.h"
#include "valuebutton.h"
#include "livesequencer.h"

class UI_LiveSequencer {
public:
  UI_LiveSequencer(LiveSequencer &sequencer, LiveSequencer::LiveSeqData &d);
  void init(void);
  void showDirectMappingWarning(uint8_t inChannel);
  void processLCDM(void);
  void handleTouchscreen(void);
  void onStopped(void);
  void drawSingleLayer(uint8_t track, uint8_t layer); // used for layer mute GUI updates
  void drawTrackLayers(uint8_t track);

  enum GuiUpdates : uint16_t {
    drawTopButtons = (1 << 0),
    drawTrackButtons = (1 << 1),
    drawLayerButtons = (1 << 2),
    drawTools = (1 << 3),
    drawActiveNotes = (1 << 5),
    drawTime = (1 << 6),
    drawSongLayers = (1 << 7),
    drawLastPlayedNote = (1 << 8),
    drawActiveTrackSubLabel = (1 << 9)
  };

private:
  uint8_t layerUpdates[LiveSequencer::LIVESEQUENCER_TRACKS_PER_SCREEN] = { 0 };

  uint8_t trackOffset = 0;
  struct ProgressBar {
    bool currentPhase;
    uint8_t drawnLength;  
  };
  ProgressBar barPattern;
  ProgressBar barTotal;

  bool runningHere = false;

  uint8_t numberOfBarsTemp = 0;
  bool deleteConfirming = false;
  bool showingHowTo = false;

  bool isLayerViewActive = false;

  ActiveValue currentValue = { nullptr, nullptr };
  TouchButton *applyPatternLength;

  std::vector<TouchButton*> buttonsToolSelect;

  TouchButton *lastNoteLabel;

  struct SelectedTrackSetup {
    TouchButton *label;
    uint8_t device;
    uint8_t instrument;
    uint8_t quantizeDenom;
    uint8_t velocity;
  } selectedTrackSetup;

  UI_LiveSequencer* instance;
  LiveSequencer& liveSeq;
  LiveSequencer::LiveSeqData &data;

  uint8_t currentPage = 0; // PagePattern, PageSong or PageTools
  uint8_t currentTools = 0;
  bool showingTools = false;

  uint8_t guiCounter = 0;
  bool blinkPhase = 0;
  
  uint8_t trackLayerMode = LiveSequencer::LAYER_MUTE;
  uint8_t songLayerMode = LiveSequencer::LAYER_MUTE;

  enum Pages {
    PAGE_PATTERN = 0,
    PAGE_SONG,
    PAGE_NUM
  };

  enum Tools : uint8_t {
    TOOLS_PATTERN,
    TOOLS_SONG,
    TOOLS_ARP,
    TOOLS_SEQ,
    TOOLS_NUM
  };

  std::vector<TouchButton*> toolsPages[TOOLS_NUM];
  std::vector<TouchButton*> trackButtons;

  uint16_t GRID_X[LiveSequencer::LIVESEQUENCER_TRACKS_PER_SCREEN];
  uint16_t GRID_Y[LiveSequencer::LIVESEQUENCER_NUM_LAYERS + 2];
 
  static constexpr uint8_t BAR_WIDTH = 104; // 2 * BUTTON_WIDTH + spacing
  static constexpr uint8_t BAR_HEIGHT = 3;
  static void openScreen(LCDML_FuncPtr_pu8 screen, uint8_t p_param = 0);
  void onTrackButtonPressed(uint8_t track);
  void clearBottomArea(void);
  void selectTools(Tools tools);
  void refreshToolsElements(Tools tools);
  bool isModeToolActive(void);
  void redrawScreen(void);
  void drawGUI(uint16_t &guiFlags);
  void processBar(const float progressTotal, const uint16_t y, ProgressBar &bar, const uint16_t color);
  static std::string getArpModeName(uint8_t mode);
  void drawLayerButton(const bool horizontal, uint8_t layerMode, int layer, const bool layerEditActive, TouchButton::Color color, uint16_t x, uint16_t y);
  void handleLayerEditButtonColor(uint8_t layerMode, TouchButton::Color &color);
  void resetProgressBars(void);
  void drawBar(uint16_t x, uint16_t y, uint16_t w, uint16_t h, uint16_t color);
  void updateTrackChannelSetupButtons(uint8_t track);
  void drawTrackSubtext(uint8_t track, uint8_t trackOffset, bool isActiveTrack);
  void checkApplyTrackInstrument(void);
};

#endif //UI_LIVESEQUENCER_H