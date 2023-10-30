#ifndef _TOUCH_H
#define _TOUCH_H

#if defined GENERIC_DISPLAY
#include "XPT2046_Touchscreen.h"
extern XPT2046_Touchscreen touch;
#endif

#ifdef CAPACITIVE_TOUCH_DISPLAY
#include "Adafruit_FT6206.h"
extern Adafruit_FT6206 touch;
#endif

#include <SD.h>

typedef struct dexed_live_mod_s
{
  uint8_t active_button = 0;
  uint8_t orig_attack_values[2][7];
  uint8_t orig_release_values[2][7];

#if NUM_DEXED > 1
  int attack_mod[NUM_DEXED] = { 0, 0 };
  int release_mod[NUM_DEXED] = { 0, 0 };
#else
  int attack_mod[NUM_DEXED] = { 0 };
  int release_mod[NUM_DEXED] = { 0 };
#endif

} dexed_live_mod_t;

typedef struct ts_s
{
  bool finished_calibration = false;
  uint16_t temp_col_hue = 1;
  uint8_t temp_col_sat = 240, temp_col_bright = 240;
  uint8_t virtual_keyboard_octave = 3;
  uint8_t virtual_keyboard_instrument = 1; // 1 = dexed0  2=dexed1  3=microsynth0  4=microsynth1   5=epiano  6=drums 7=braids, 8- pitched samples
  uint8_t virtual_keyboard_midi_channel = 1;
  uint8_t virtual_keyboard_velocity = 120;
  uint8_t msp_peak[NUM_MULTISAMPLES];

  TS_Point p;

  uint32_t virtual_keyboard_state_white = 0;
  uint32_t virtual_keyboard_state_black = 0;

  uint8_t displayed_peak[20]; // volmeter peak levels, currently displayed level
  uint8_t old_helptext_length[3];
  bool touch_ui_drawn_in_menu = false;
  bool keyb_in_menu_activated = false;
  uint8_t fav_buttton_state = 0;
} ts_t;

// (Touch)File Manager

typedef struct fm_s
{
  uint8_t wav_recorder_mode = 0;

#ifdef COMPILE_FOR_SDCARD
  uint8_t sample_source = 0; // 0 = SD, 1 = FLASH
#endif

#ifdef COMPILE_FOR_PROGMEM
  uint8_t sample_source = 0; // 0 = SD, 1 = FLASH
#endif

#ifdef COMPILE_FOR_FLASH
  uint8_t sample_source = 1; // 0 = SD, 1 = FLASH
#endif

  int sample_screen_position_x = 0;
  uint8_t active_window = 0; // 0 = left window (SDCARD) , 1 = FLASH
  uint16_t sd_sum_files = 0;
  uint8_t sd_cap_rows;
  uint8_t sd_folder_depth = 0;
  uint16_t sd_selected_file = 0;
  uint16_t sd_skip_files = 0;
  uint8_t sd_mode = 0;
  bool sd_is_folder;
  bool sd_parent_folder = false;
  char sd_new_name[52];
  char sd_full_name[52];
  char sd_prev_dir[52];
  char sd_temp_name[52];

  uint16_t flash_sum_files = 0;
  uint16_t flash_cap_rows;
  uint16_t flash_selected_file = 0;
  uint16_t flash_skip_files = 0;
  uint8_t flash_mode = 4;
  uint8_t flash_preview_slot;
  char flash_temp_name[52];
} fm_t;

#endif
