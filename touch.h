
#ifndef _TOUCH_H
#define _TOUCH_H
#include <XPT2046_Touchscreen.h>
#include <SD.h>
extern XPT2046_Touchscreen touch;

typedef struct ts_s
{
  int slowdown_keyboard;
  uint8_t virtual_keyboard_octave = 3;
  TS_Point p;
  uint8_t virtual_keyboard_state_white[11];
  uint8_t virtual_keyboard_state_black[17];
  bool update_virtual_keyboard_octave;
  bool switch_active_instance;
  bool block_screen_update;
} ts_t;

// (Touch)File Manager

typedef struct fm_s
{
  File currentDirectoy;
  File entry;
  uint8_t cap_rows;
  bool is_folder;
  bool parent_folder = false;
  uint8_t folder_depth = 0;
  uint8_t selected_file = 0;
  uint8_t skip_files = 0;
  char temp_name[52];
  char new_name[52];
  char full_name[52];
  uint8_t mode = 1;
} fm_t;

#endif
