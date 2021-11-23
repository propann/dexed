
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
  File sd_currentDirectoy;
  File sd_entry;
  uint8_t sd_cap_rows;
  uint8_t sd_folder_depth = 0;
  uint8_t sd_selected_file = 0;
  uint8_t sd_skip_files = 0;
  uint8_t sd_mode = 4;
  uint8_t sd_preview_slot;
  bool sd_is_folder;
  bool sd_parent_folder = false;
  char sd_temp_name[52];
  char sd_new_name[52];
  char sd_full_name[52];

  File flash_currentDirectoy;
  File flash_entry;
  uint8_t flash_cap_rows;
  uint8_t flash_folder_depth = 0;
  uint8_t flash_selected_file = 0;
  uint8_t flash_skip_files = 0;
  uint8_t flash_mode = 4;
  uint8_t flash_preview_slot;
  bool flash_is_folder;
  bool flash_parent_folder = false;
  char flash_temp_name[52];
  char flash_new_name[52];
  char flash_full_name[52];


} fm_t;

#endif
