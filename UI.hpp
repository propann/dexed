/*
   MicroDexed

   MicroDexed is a port of the Dexed sound engine
   (https://github.com/asb2m10/dexed) for the Teensy-3.5/3.6/4.x with audio shield.
   Dexed ist heavily based on https://github.com/google/music-synthesizer-for-android

   (c)2018-2021 H. Wirtz <wirtz@parasitstudio.de>

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 3 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program; if not, write to the Free Software Foundation,
   Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301  USA
*/

#ifndef _UI_HPP_
#define _UI_HPP_

#include <LCDMenuLib2.h>
#include <MD_REncoder.h>
#include "config.h"
#include "disp_plus.h"
#include "synth_dexed.h"
#include "effect_modulated_delay.h"
#include "effect_stereo_mono.h"
#include "effect_platervbstereo.h"
#include "template_mixer.hpp"
#include "drumset.h"
#include "sequencer.h"
#include "touch.h"

#define _LCDML_DISP_cols  display_cols
#define _LCDML_DISP_rows  display_rows

#define _LCDML_DISP_cfg_cursor                     0xda   // cursor Symbol
#define _LCDML_DISP_cfg_scrollbar                  1      // enable a scrollbar

extern PeriodicTimer timer1;
extern ILI9486_Teensy display;
extern void sequencer(void);
extern bool check_sd_performance_exists(uint8_t number);

extern config_t configuration;
extern void set_volume(uint8_t v, uint8_t m);
extern bool load_sysex(uint8_t b, uint8_t v);
extern void generate_version_string(char* buffer, uint8_t len);
extern void _softRestart(void);
//extern float pseudo_log_curve(float value);
extern float midi_volume_transform(uint8_t midi_amp);
extern float volume_transform(float amp);
extern uint8_t selected_instance_id;
extern char receive_bank_filename[FILENAME_LEN];
extern void eeprom_update(void);

#if NUM_DRUMS > 0
#include "drums.h"
extern void get_sd_performance_name_json(uint8_t number);
extern bool save_sd_performance_json(uint8_t p);
extern void virtual_keyboard(int ypos);
extern uint8_t drum_midi_channel;
extern drum_config_t drum_config[NUM_DRUMSET_CONFIG];
extern sequencer_t seq;
extern ts_t ts;
extern fm_t fm;
uint8_t seq_active_function = 99;
uint8_t activesample;
#endif

int wave_spacing = 2;
int wave_spacing_old = 2;

#ifdef SGTL5000_AUDIO_ENHANCE
#include "control_sgtl5000plus.h"
extern AudioControlSGTL5000Plus       sgtl5000;
#else
extern AudioControlSGTL5000           sgtl5000;
#endif

#if defined(USE_FX)
extern AudioSynthWaveform*            chorus_modulator[NUM_DEXED];
extern AudioEffectModulatedDelay*     modchorus[NUM_DEXED];
extern AudioMixer<4>*                 chorus_mixer[NUM_DEXED];
extern AudioMixer<4>*                 delay_fb_mixer[NUM_DEXED];
extern AudioEffectDelay*              delay_fx[NUM_DEXED];
extern AudioMixer<4>*                 delay_mixer[NUM_DEXED];
#endif
extern AudioEffectMonoStereo*         mono2stereo[NUM_DEXED];
extern AudioMixer<4>                  microdexed_peak_mixer;
extern AudioAnalyzePeak               microdexed_peak;
#if defined(USE_FX)
extern AudioMixer<4>                  reverb_mixer_r;
extern AudioMixer<4>                  reverb_mixer_l;
#endif
#if defined(USE_PLATEREVERB)
extern AudioEffectPlateReverb         reverb;
#endif
extern AudioMixer<4>                  master_mixer_r;
extern AudioMixer<4>                  master_mixer_l;
extern AudioEffectStereoMono          stereo2mono;
extern AudioAnalyzePeak               master_peak_r;
extern AudioAnalyzePeak               master_peak_l;

extern char sd_string[display_cols + 1];
extern char g_voice_name[NUM_DEXED][VOICE_NAME_LEN];
extern char g_bank_name[NUM_DEXED][BANK_NAME_LEN];
extern int perform_attack_mod[NUM_DEXED];
extern int perform_release_mod[NUM_DEXED];
extern float midi_ticks_factor[10];
extern uint8_t midi_bpm;
extern bool save_sys_flag;
extern elapsedMillis save_sys;
extern bool save_sys_flag;

/***********************************************************************
   GLOBAL
************************************************************************/
elapsedMillis back_from_volume;
uint8_t instance_num[8][8];
const char accepted_chars[] = " ABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789-abcdefghijklmnopqrstuvwxyz";
const char noteNames[12][3] = { "C", "C#", "D", "D#", "E", "F", "F#", "G", "G#", "A", "A#", "B" };
uint8_t active_perform_page = 1;
uint8_t orig_attack_values[2][7];
uint8_t orig_release_values[2][7];
int temp_int;
bool menu_select_toggle;
float temp_float;

const uint8_t scroll_bar[5][17] = {
  {B10000001, B10000001, B10000001, B10000001, B10000001, B10000001, B10000001, B10000001, B10000001, B10000001, B10000001, B10000001, B10000001, B10000001, B10000001, B10000001, B10000001}, // scrollbar top
  {B11111111, B11111111, B11111111, B11111111, B10000001, B10000001, B10000001, B10000001, B10000001, B10000001, B10000001, B10000001, B10000001, B10000001, B10000001, B10000001, B10000001}, // scroll state 1
  {B10000001, B10000001, B10000001, B10000001, B11111111, B11111111, B11111111, B11111111, B11111111, B11111111, B10000001, B10000001, B10000001, B10000001, B10000001, B10000001, B10000001}, // scroll state 2
  {B10000001, B10000001, B10000001, B10000001, B10000001, B10000001, B10000001, B10000001, B11111111, B11111111, B11111111, B11111111, B11111111, B11111111, B10000001, B10000001, B10000001}, // scroll state 3
  {B10000001, B10000001, B10000001, B10000001, B10000001, B10000001, B10000001, B10000001, B10000001, B10000001, B10000001, B10000001, B10000001, B11111111, B11111111, B11111111, B11111111}  // scrollbar bottom
};

const uint8_t block_bar[8][15] = {
  {B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000},
  {B10000000, B10000000, B10000000, B10000000, B10000000, B10000000, B10000000, B10000000, B10000000, B10000000, B10000000, B10000000, B10000000, B10000000, B10000000},
  {B11000000, B11000000, B11000000, B11000000, B11000000, B11000000, B11000000, B11000000, B11000000, B11000000, B11000000, B11000000, B11000000, B11000000, B11000000},
  {B11100000, B11100000, B11100000, B11100000, B11100000, B11100000, B11100000, B11100000, B11100000, B11100000, B11100000, B11100000, B11100000, B11100000, B11100000},
  {B11110000, B11110000, B11110000, B11110000, B11110000, B11110000, B11110000, B11110000, B11110000, B11110000, B11110000, B11110000, B11110000, B11110000, B11110000},
  {B11111100, B11111100, B11111100, B11111100, B11111100, B11111100, B11111100, B11111100, B11111100, B11111100, B11111100, B11111100, B11111100, B11111100, B11111100},
  {B11111110, B11111110, B11111110, B11111110, B11111110, B11111110, B11111110, B11111110, B11111110, B11111110, B11111110, B11111110, B11111110, B11111110, B11111110},
  {B11111111, B11111111, B11111111, B11111111, B11111111, B11111111, B11111111, B11111111, B11111111, B11111111, B11111111, B11111111, B11111111, B11111111, B11111111}
};

const uint8_t meter_bar[8][8] = {
  {B10000000, B10000000, B10000000, B10000000, B10000000, B10000000, B10000000, B10000000},
  {B01000000, B01000000, B01000000, B01000000, B01000000, B01000000, B01000000, B01000000},
  {B00100000, B00100000, B00100000, B00100000, B00100000, B00100000, B00100000, B00100000},
  {B00010000, B00010000, B00010000, B00010000, B00010000, B00010000, B00010000, B00010000},
  {B00001000, B00001000, B00001000, B00001000, B00001000, B00001000, B00001000, B00001000},
  {B00000100, B00000100, B00000100, B00000100, B00000100, B00000100, B00000100, B00000100},
  {B00000010, B00000010, B00000010, B00000010, B00000010, B00000010, B00000010, B00000010},
  {B00000001, B00000001, B00000001, B00000001, B00000001, B00000001, B00000001, B00000001}
};

const uint8_t special_chars[24][8] = {
  {B11111111, B11110111, B11100111, B11110111, B11110111, B11110111, B11110111, B11111111}, //  [0] 1 small invers
  {B11111111, B11110111, B11101011, B11111011, B11110111, B11101111, B11100011, B11111111}, //  [1] 2 small invers
  {B11111, B11011, B10011, B11011, B11011, B11011, B11011, B11111}, //  [2] 1 OP invers
  {B11111, B11011, B10101, B11101, B11011, B10111, B10001, B11111}, //  [3] 2 OP invers
  {B11111, B10001, B11101, B11011, B11101, B10101, B11011, B11111}, //  [4] 3 OP invers
  {B11111, B10111, B10111, B10101, B10001, B11101, B11101, B11111}, //  [5] 4 OP invers
  {B11111, B10001, B10111, B10011, B11101, B11101, B10011, B11111}, //  [6] 5 OP invers
  {B11111, B11001, B10111, B10011, B10101, B10101, B11011, B11111}, //  [7] 6 OP invers
  {B00000, B00000, B00000, B00000, B00000, B00000, B00000, B11111}, //  [8] Level 1
  {B00000, B00000, B00000, B00000, B00000, B00000, B11111, B11111}, //  [9] Level 2
  {B00000, B00000, B00000, B00000, B00000, B11111, B11111, B11111}, // [10] Level 3
  {B00000, B00000, B00000, B00000, B11111, B11111, B11111, B11111}, // [11] Level 4
  {B00000, B00000, B00000, B11111, B11111, B11111, B11111, B11111}, // [12] Level 5
  {B00000, B00000, B11111, B11111, B11111, B11111, B11111, B11111}, // [13] Level 6
  {B00000, B11111, B11111, B11111, B11111, B11111, B11111, B11111}, // [14] Level 7
  {B11111, B11111, B11111, B11111, B11111, B11111, B11111, B11111}, // [15] Level 8
  {B00100, B00110, B00101, B00101, B01101, B11101, B11100, B11000}, // [16] Note
  {B01110, B10001, B10001, B01110, B00100, B00100, B00110, B00110}, // [17] Disabled 2nd instance symbol
  {B11111111, B11000011, B11011111, B11000011, B11011111, B11011111, B11011111, B11111111}, // [18] Favorites Icon
  {B01000000, B01100000, B01110000, B01111000, B01110000, B01100000, B01000000, B00000000}, // [19] Play Symbol
  {B00000000, B01111000, B11111100, B11111100, B11111100, B11111100, B01111000, B00000000}, // [20] Record Symbol
  {B00000000, B00000000, B01111100, B01111100, B01111100, B01111100, B01111100, B00000000}, // [21] Stop Symbol
  {B11111111, B11000011, B11011011, B11000011, B11011111, B11011111, B11011111, B11111111}, // [22] Pitched Sample
  {B00000000, B00000000, B00000000, B01110000, B11111111, B11111111, B11111111, B11111111}  // [23] File Folder
};

enum { SCROLLBAR, BLOCKBAR, METERBAR };
enum { ENC_R, ENC_L };
enum {MENU_VOICE_BANK, MENU_VOICE_SOUND};

void lcdml_menu_display(void);
void lcdml_menu_clear(void);
void lcdml_menu_control(void);
void mFunc_screensaver(uint8_t param);
#ifdef USE_FX
void UI_func_reverb_roomsize(uint8_t param);
void UI_func_reverb_damping(uint8_t param);
void UI_func_reverb_lowpass(uint8_t param);
void UI_func_reverb_lodamp(uint8_t param);
void UI_func_reverb_hidamp(uint8_t param);
void UI_func_reverb_diffusion(uint8_t param);
void UI_func_reverb_level(uint8_t param);
void UI_func_chorus_frequency(uint8_t param);
void UI_func_chorus_waveform(uint8_t param);
void UI_func_chorus_depth(uint8_t param);
void UI_func_chorus_level(uint8_t param);
void UI_func_delay_time(uint8_t param);
void UI_func_delay_feedback(uint8_t param);
void UI_func_delay_level(uint8_t param);
void UI_func_reverb_send(uint8_t param);
void UI_func_filter_cutoff(uint8_t param);
void UI_func_filter_resonance(uint8_t param);
void UI_func_drum_reverb_send(uint8_t param);
#endif
void UI_func_transpose(uint8_t param);
void UI_func_tune(uint8_t param);
void UI_func_midi_channel(uint8_t param);
void UI_func_lowest_note(uint8_t param);
void UI_func_highest_note(uint8_t param);
void UI_func_sound_intensity(uint8_t param);
void UI_func_panorama(uint8_t param);
void UI_func_stereo_mono(uint8_t param);
void UI_func_note_refresh(uint8_t param);
void UI_func_polyphony(uint8_t param);
void UI_func_mono_poly(uint8_t param);
void UI_func_pb_range(uint8_t param);
void UI_func_pb_step(uint8_t param);
void UI_func_mw_range(uint8_t param);
void UI_func_mw_assign(uint8_t param);
void UI_func_mw_mode(uint8_t param);
void UI_func_fc_range(uint8_t param);
void UI_func_fc_assign(uint8_t param);
void UI_func_fc_mode(uint8_t param);
void UI_func_bc_range(uint8_t param);
void UI_func_bc_assign(uint8_t param);
void UI_func_bc_mode(uint8_t param);
void UI_func_at_range(uint8_t param);
void UI_func_at_assign(uint8_t param);
void UI_func_at_mode(uint8_t param);
void UI_func_portamento_mode(uint8_t param);
void UI_func_portamento_glissando(uint8_t param);
void UI_func_portamento_time(uint8_t param);
void UI_handle_OP(uint8_t param);
void UI_func_information(uint8_t param);
void UI_func_file_manager(uint8_t param);
void UI_func_seq_pattern_editor(uint8_t param);
void UI_func_seq_vel_editor(uint8_t param);
void UI_func_seq_live_transpose_oct(uint8_t param);
void UI_func_arp_shift(uint8_t param);
void UI_func_seq_chord_keys_ammount(uint8_t param);
void UI_func_seq_lenght(uint8_t param);
void UI_func_seq_tempo(uint8_t param);
void UI_func_seq_pat_chain(uint8_t param);
void UI_func_seq_tracker(uint8_t param);
void UI_func_seq_pianoroll(uint8_t param);
void UI_func_arpeggio(uint8_t param);
void UI_func_seq_mute_matrix(uint8_t param);
void UI_func_seq_track_setup(uint8_t param);
void UI_func_dexed_assign(uint8_t param);
void UI_func_seq_display_style(uint8_t param);
void UI_func_set_performance_name(uint8_t param);
void UI_func_volume(uint8_t param);
void UI_func_smart_filter(uint8_t param);
void UI_func_drum_midi_channel(uint8_t param);
void UI_func_load_performance(uint8_t param);
void UI_func_save_performance(uint8_t param);
void UI_func_save_voice(uint8_t param);
void UI_func_midi_soft_thru(uint8_t param);
void UI_func_velocity_level(uint8_t param);
void UI_func_voice_select(uint8_t param);
void UI_func_sysex_send_voice(uint8_t param);
void UI_func_sysex_receive_bank(uint8_t param);
void UI_func_sysex_send_bank(uint8_t param);
void UI_func_eq_1(uint8_t param);
void UI_func_eq_2(uint8_t param);
void UI_func_eq_3(uint8_t param);
void UI_func_eq_4(uint8_t param);
void UI_func_eq_5(uint8_t param);
void UI_func_eq_6(uint8_t param);
void UI_func_eq_7(uint8_t param);
void UI_func_startup(uint8_t param);
void UI_function_not_enabled(void);
void UI_function_not_implemented(uint8_t param);
void UI_func_favorites(uint8_t param);
void UI_update_instance_icons();
bool UI_select_name(uint8_t y, uint8_t x, char* edit_string, uint8_t len, bool init);
uint8_t search_accepted_char(uint8_t c);
void display_int(int16_t var, uint8_t size, bool zeros, bool brackets, bool sign);
void display_float(float var, uint8_t size_number, uint8_t size_fraction, bool zeros, bool brackets, bool sign);
void display_bar_int(const char* title, uint32_t value, float factor, int32_t min_value, int32_t max_value, uint8_t size, bool zeros, bool sign, bool init);
void display_bar_float(const char* title, float value, float factor, int32_t min_value, int32_t max_value, uint8_t size_number, uint8_t size_fraction, bool zeros, bool sign, bool init);
void display_meter_int(const char* title, uint32_t value, float factor, float offset, int32_t min_value, int32_t max_value, uint8_t size, bool zeros, bool sign, bool init);
void display_meter_float(const char* title, float value, float factor, float offset, int32_t min_value, int32_t max_value, uint8_t size_number, uint8_t size_fraction, bool zeros, bool sign, bool init);
void display_active_instance_number(uint8_t instance_id);
void display_OP_active_instance_number(uint8_t instance_id, uint8_t op);
void display_special_chars(uint8_t mode);
void display_delay_sync(uint8_t sync);
void string_trim(char *s);
void save_favorite(uint8_t b, uint8_t v, uint8_t instance_id);
void draw_favorite_icon(uint8_t b, uint8_t v, uint8_t instance_id);
bool check_favorite(uint8_t b, uint8_t v, uint8_t instance_id);
bool quick_check_favorites_in_bank(uint8_t b, uint8_t instance_id);
void locate_previous_non_favorite();
void locate_previous_favorite();
void locate_next_favorite();
void locate_next_non_favorite();
void locate_random_non_favorite();
void UI_func_drums_main_volume(uint8_t param);
void UI_func_drum_volume(uint8_t param);
void UI_func_drum_pan(uint8_t param);
void UI_func_drum_pitch(uint8_t param);
void UI_func_drum_tune_offset(uint8_t param);
void UI_func_sample_editor(uint8_t param);

char* basename(const char* filename);
char* strip_extension(char* filename);

// normal menu
LCDMenuLib2_menu LCDML_0(255, 0, 0, NULL, NULL); // normal root menu element (do not change)
LCDMenuLib2 LCDML(LCDML_0, _LCDML_DISP_rows, _LCDML_DISP_cols, lcdml_menu_display, lcdml_menu_clear, lcdml_menu_control);

#include "UI.h"

int favsearcher = 0;

// create menu
LCDML_createMenu(_LCDML_DISP_cnt);

/***********************************************************************
   CONTROL
 ***********************************************************************/
class EncoderDirection
{
  public:
    EncoderDirection(void)
    {
      reset();
    }

    void reset(void)
    {
      button_short = false;
      button_long = false;
      button_pressed = false;
      left = false;
      right = false;
      up = false;
      down = false;
    }

    void ButtonShort(bool state)
    {
      button_short = state;
    }

    bool ButtonShort(void)
    {
      if (button_short == true)
      {
        button_short = false;
        return (true);
      }
      return (false);
    }

    void ButtonLong(bool state)
    {
      button_long = state;
    }

    bool ButtonLong(void)
    {
      if (button_long == true)
      {
        button_long = false;
        return (true);
      }
      return (false);
    }

    void ButtonPressed(bool state)
    {
      button_pressed = state;
    }

    bool ButtonPressed(void)
    {
      return (button_pressed);
    }

    void Left(bool state)
    {
      left = state;
    }

    bool Left(void)
    {
      if (left == true)
      {
        left = false;
        return (true);
      }
      return (false);
    }

    void Right(bool state)
    {
      right = state;
    }

    bool Right(void)
    {
      if (right == true)
      {
        right = false;
        return (true);
      }
      return (false);
    }

    void Up(bool state)
    {
      up = state;
    }

    bool Up(void)
    {
      if (up == true)
      {
        up = false;
        return (true);
      }
      return (false);
    }

    void Down(bool state)
    {
      down = state;
    }

    bool Down(void)
    {
      if (down == true)
      {
        down = false;
        return (true);
      }
      return (false);
    }

  private:
    bool button_short;
    bool button_long;
    bool button_pressed;
    bool left;
    bool right;
    bool up;
    bool down;
};

MD_REncoder ENCODER[NUM_ENCODER] = {MD_REncoder(ENC_R_PIN_B, ENC_R_PIN_A), MD_REncoder(ENC_L_PIN_B, ENC_L_PIN_A)};
EncoderDirection encoderDir[NUM_ENCODER];

long  g_LCDML_CONTROL_button_press_time[NUM_ENCODER] = {0, 0};
bool  g_LCDML_CONTROL_button_prev[NUM_ENCODER] = {HIGH, HIGH};
uint8_t g_LCDML_CONTROL_prev[NUM_ENCODER] = {0, 0};
bool menu_init = true;

void fill_up_with_spaces_right_window()
{
  do {
    display.print(" ");
  } while (display.getCursorX() < 430);
}

void fill_up_with_spaces_left_window()
{
  do
  {
    display.print(" ");
  } while (display.getCursorX() < 230);
}


void smart_filter(uint8_t dir)
{
  bool found = false;

  //search backwards
  if (dir == 0) {
    if (seq.smartfilter) {
      do {
        if (found == false)  activesample = constrain(activesample - 1, 0, NUM_DRUMSET_CONFIG - 2);
        for (uint8_t d = 0; d < 10; d++)
        {
          if (seq.content_type[d] == 0) {
            for (uint8_t y = 0; y < 16; y++)
            {
              if (drum_config[activesample].midinote == seq.note_data[d][y] || drum_config[activesample].midinote == seq.vel[d][y])
              {
                found = true;
                break;
              }
            }
          }
        }
      } while (found == false && activesample > 0 );
    }
    else
      activesample = constrain(activesample - 1, 0, NUM_DRUMSET_CONFIG - 2);
  }
  else //search forwards
  {
    if (seq.smartfilter) {
      do {
        if (found == false)  activesample = constrain(activesample + 1, 0, NUM_DRUMSET_CONFIG - 2);
        for (uint8_t d = 0; d < 10; d++)
        {
          if (seq.content_type[d] == 0) {

            for (uint8_t y = 0; y < 16; y++)
            {
              if (drum_config[activesample].midinote == seq.note_data[d][y] || drum_config[activesample].midinote == seq.vel[d][y])
              {
                found = true;
                break;
              }
            }
          }
        }
      } while (found == false && activesample < NUM_DRUMSET_CONFIG - 2 );
    }
    else
      activesample = constrain(activesample + 1, 0, NUM_DRUMSET_CONFIG - 2);
  }
}

void border0()  //main Border
{
  // display.drawRect(0, 0, 480, 320, GREY1);  //Main Border
  ;
}

void border1() //upper left
{
  display.drawRect(0, 0, CHAR_width * 20, CHAR_height * 6 - 3, GREY4);
}
void border2()  //upper right
{
  display.drawRect(CHAR_width * 20 - 1, 0, 480 - CHAR_width * 20 + 1, CHAR_height * 6 - 3,   GREY4);
}
void border3() //lower left
{
  display.drawRect(0, CHAR_height * 6 - 4 , CHAR_width * 20, 321 - CHAR_height * 6 - 3,  GREY4);
}
void border4()  //lower right
{
  display.drawRect(CHAR_width * 20 - 1, CHAR_height * 6 - 4, 480 - CHAR_width * 20 + 1, 321 - CHAR_height * 6 - 3 ,  GREY4);
}
void border1_clear() //upper left
{
  display.fillRect(0 + 1, 0 + 1, CHAR_width * 20 - 2, CHAR_height * 6 - 3 - 2, BLACK);
}
void border2_clear()  //upper right
{
  display.fillRect( CHAR_width * 20 - 1 + 1, 0 + 1, 480 - CHAR_width * 20 - 1, CHAR_height * 6 - 5, BLACK);
}
void border3_clear() //lower left
{
  display.fillRect(0 + 1, CHAR_height * 6 - 3 , CHAR_width * 20 - 2, 321 - CHAR_height * 6 - 5, BLACK);
}
void border4_clear()  //lower right
{
  display.fillRect(CHAR_width * 20 , CHAR_height * 6 - 3, 480 - CHAR_width * 20 - 1, 321 - CHAR_height * 6 - 5, BLACK);
}

void print_current_chord()
{
  for (uint8_t x = 0; x < 7; x++)
  {
    display.print( seq.chord_names[seq.arp_chord][x]);
  }
}

uint8_t seq_find_drum_data_from_note(uint8_t note)
{
  bool found = false;
  uint8_t samplenumber;
  for (uint8_t d = 0; d < NUM_DRUMSET_CONFIG - 1; d++)
  {
    if (note == drum_config[d].midinote)
    { samplenumber = d;
      found = true;
      break;
    }
  }
  if (found == false) samplenumber = 0;

  return samplenumber;
}

const char* seq_find_drum_name_from_note(uint8_t note)
{
  bool found = false;
  const char* shortname;
  for (uint8_t d = 0; d < NUM_DRUMSET_CONFIG - 1; d++)
  {
    if (note == drum_config[d].midinote)
    { shortname = basename(drum_config[d].name);
      found = true;
      break;
    }
  }
  if (found == false) shortname = " ";

  return shortname;
}

const char* find_long_drum_name_from_note(uint8_t note)
{
  bool found = false;
  const char* name;
  for (uint8_t d = 0; d < NUM_DRUMSET_CONFIG - 1; d++)
  {
    if (note == drum_config[d].midinote)
    { name = basename(drum_config[d].name);
      found = true;
      break;
    }
  }
  if (found == false) name = " ";

  return name;
}

const char* seq_find_shortname_in_track(uint8_t sstep, uint8_t track)
{
  const char* shortname;
  bool found = false;
  if (seq.content_type[track] == 0 && seq.vel[track][sstep] < 210) //is Drumtrack and not a pitched sample
  {
    for (uint8_t d = 0; d < NUM_DRUMSET_CONFIG - 1; d++)
    {
      if (seq.note_data[track][sstep] == drum_config[d].midinote)
      {
        shortname = drum_config[d].shortname;
        found = true;
        break;
      }
    }
    if (found == false) shortname = "-";
  }
  else
  {
    if (seq.vel[track][sstep] > 209)
      shortname = "P"; // pitched sample
    else if (seq.note_data[track][sstep] > 0  && seq.note_data[track][sstep] != 130)
      shortname = noteNames[seq.note_data[track][sstep] % 12];
    else if (seq.note_data[track][sstep] == 130)
      shortname = "~"; // note is a tie/latch
    else
      shortname = "-";
  }
  return shortname;
}

const char* seq_find_shortname(uint8_t sstep)
{
  const char* shortname;
  bool found = false;
  if (seq.content_type[seq.active_pattern] == 0 && seq.vel[seq.active_pattern][sstep] < 210) //is Drumtrack and not a pitched sample
  {
    for (uint8_t d = 0; d < NUM_DRUMSET_CONFIG - 1; d++)
    {
      if (seq.note_data[seq.active_pattern][sstep] == drum_config[d].midinote)
      {
        shortname = drum_config[d].shortname;
        found = true;
        break;
      }
    }
    if (found == false) shortname = "-";
  }
  else
  {
    if (seq.note_data[seq.active_pattern][sstep] > 0  && seq.note_data[seq.active_pattern][sstep] != 130)
    {
      shortname = noteNames[seq.note_data[seq.active_pattern][sstep] % 12];
    }
    else if (seq.note_data[seq.active_pattern][sstep] == 130)shortname = "~"; // note has tie/latch
    else
      shortname = "-";
  }
  return shortname;
}

void set_pattern_content_type_color(uint8_t pattern)
{
  if (seq.content_type[pattern] == 0) //Drumpattern
    display.setTextColor(DX_ORANGE, BLACK);
  else if (seq.content_type[pattern] == 1) //Instrument Pattern
    display.setTextColor(LIGHTBLUE, BLACK);
  else if (seq.content_type[pattern] == 2 || seq.content_type[pattern] == 3) //  chord or arp pattern
    display.setTextColor(DX_MAGENTA, BLACK);
}

int get_pattern_content_type_color(uint8_t pattern)
{
  int col = 0;

  if (seq.content_type[pattern] == 0) //Drumpattern
    col = DX_ORANGE;
  else if (seq.content_type[pattern] == 1) //Instrument Pattern
    col = LIGHTBLUE;
  else
    col = DX_MAGENTA;
  return col;
}

void seq_print_formatted_number (uint8_t number, uint8_t lenght)
{
  if (lenght == 3)
  {
    if (number < 10)
      display.print("0");
    if (number < 100)
      display.print("0");
    display.print(number);
  }
  else
    //if not 3 then lenght defaults to 2
  {
    if (number < 10)
      display.print("0");
    display.print(number);
  }
}

void print_chord_name (uint8_t currentstep)
{
  for (uint8_t i = 0; i < 7; i++)
  {
    if (seq.vel[ seq.active_pattern][currentstep] > 199)
      display.print(seq.chord_names[ seq.vel[ seq.active_pattern][currentstep] - 200 ][i] );
    else
      display.print(seq.chord_names[ 6 ][i] );
  }
}

void seq_print_step_numbers(int xpos, int ypos)
{
  uint8_t buffer[35] = {10, 11, 12, 13, 14, 15, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12 };
  uint8_t step = seq.step;
  uint8_t yspacer = 16;
  uint8_t count = 0;
  while ( count < 13 )
  {
    display.setCursor(xpos, ypos + count * yspacer);
    if (count == 6 )
      display.setTextColor(WHITE, BLACK);
    else
      display.setTextColor(GREEN, BLACK);
    seq_print_formatted_number (buffer[step + count] , 2);
    count++;
  }
}

void update_keyboard_current_step ( int ypos, uint8_t octave, uint8_t current_step)
{
  uint8_t piano[12 * 4] = {0, 1, 0, 1, 0, 0, 1, 0, 1, 0, 1, 0,  0, 1, 0, 1, 0, 0, 1, 0, 1, 0, 1, 0, 0, 1, 0, 1, 0, 0, 1, 0, 1, 0, 1, 0, 0, 1, 0, 1, 0, 0, 1, 0, 1, 0, 1, 0, };
  // draw grid
  for (uint8_t y = 0; y < 34; y++)
  {
    display.fillRect(34 + current_step * 7 , ypos + 6 - CHAR_height  - (y * 8.15 ), 5, 6, WHITE); // active step
    if (current_step > 0)
    {
      if (piano[y] == 0 ) // is a white key
        display.fillRect(34 - 7 + current_step * 7 , ypos + 6 - CHAR_height  - (y * 8.15 ), 5, 6, GREY3); // GRID white key
      else
        display.fillRect(34 - 7 + current_step * 7 , ypos + 6 - CHAR_height  - (y * 8.15 ), 5, 6, GREY4); // GRID black key
    }
    else if (current_step == 0)
    {
      if (piano[y] == 0 ) // is a white key
        display.fillRect(34  + 63 * 7 , ypos + 6 - CHAR_height  - (y * 8.15 ), 5, 6, GREY3); // GRID white key
      else
        display.fillRect(34  + 63 * 7 , ypos + 6 - CHAR_height  - (y * 8.15 ), 5, 6, GREY4); // GRID black key
    }
  }
}

void print_keyboard ( int ypos, uint8_t octave)
{
  uint8_t piano[12 * 4] = {0, 1, 0, 1, 0, 0, 1, 0, 1, 0, 1, 0,  0, 1, 0, 1, 0, 0, 1, 0, 1, 0, 1, 0, 0, 1, 0, 1, 0, 0, 1, 0, 1, 0, 1, 0, 0, 1, 0, 1, 0, 0, 1, 0, 1, 0, 1, 0, };
  uint8_t offset[5] = {12, 12, 14, 12, 11 }; //+ is up
  int offcount = 0;
  uint8_t oct_count = 0;
  display.setTextColor(BLACK, WHITE);
  display.setTextSize(1);
  //draw white keys
  for (uint8_t y = 0; y < 20; y++)
  {
    display.fillRect(0, ypos - CHAR_height - (y * 14 ), 30, 13, WHITE); // pianoroll white key
    if ( y == 0 || y == 7 || y == 14) {
      display.setCursor (17, ypos - 14 - (y * 14 )   );
      display.print("C");
      display.print(octave - 1 + oct_count);
      oct_count++;
    }
  }
  for (uint8_t y = 0; y < 33; y++)
  {
    if (piano[y] == 1)
    {
      display.fillRect(0, ypos - (y * 8.15 ) - offset[offcount] , 12, 8, BLACK);  // BLACK key
      offcount++;
      if (offcount == 5)offcount = 0;
    }
  }
  // draw grid

  for (uint8_t y = 0; y < 34; y++)
  {
    for (uint8_t x = 0; x < 64; x++)
    {
      if (piano[y] == 0 ) // is a white key
        display.fillRect(34 + x * 7 , ypos + 6 - CHAR_height  - (y * 8.15 ), 5, 6, GREY3); // GRID white key
      else
        display.fillRect(34 + x * 7 , ypos + 6 - CHAR_height  - (y * 8.15 ), 5, 6, GREY4); // GRID black key
    }
  }
}

void update_pianoroll (int xpos, int ypos, uint8_t track_number, uint8_t cur_step)
{
  uint8_t notes[64];
  uint8_t lowest_note = 127;
  int notes_display_shift = 0;

  if (seq.patternchain[0][track_number] < NUM_SEQ_PATTERN && seq.patternchain[1][track_number] < NUM_SEQ_PATTERN
      && seq.patternchain[2][track_number] < NUM_SEQ_PATTERN && seq.patternchain[3][track_number] < NUM_SEQ_PATTERN)
  {
    for (uint8_t f = 0; f < 16; f++)  // Fill array with complete data from all chain parts of track
    {
      notes[f] = seq.note_data[ seq.patternchain[0][track_number] ][f];
      notes[f + 16] = seq.note_data[ seq.patternchain[1][track_number] ][f];
      notes[f + 32] = seq.note_data[ seq.patternchain[2][track_number] ][f];
      notes[f + 48] = seq.note_data[ seq.patternchain[3][track_number] ][f];
    }
    //find lowest note
    for (uint8_t f = 0; f < 64; f++)
    {
      if (notes[f] < lowest_note && notes[f] > 0)
      {
        lowest_note = notes[f];
      }
    }
    if (lowest_note == 127)
      lowest_note = 24;
    notes_display_shift = lowest_note % 12;
    update_keyboard_current_step(ypos, lowest_note / 12 , cur_step);
    if (cur_step - 1 >= 0)
    {
      if (notes[cur_step - 1] > 0)
      {
        if (notes[cur_step - 1] == 130)
        {
          display.fillRect ( 34 + (cur_step - 1) * 7,  ypos - 10 - (8.15 * notes_display_shift )  - (8.15 * (seq.pianoroll_last_valid_note - lowest_note) ) , 5, 5, DX_CYAN  );
        }
        else
        {
          display.fillRect  ( 34 + (cur_step - 1) * 7,  ypos - 10 - (8.15 * notes_display_shift )  - (8.15 * (notes[cur_step - 1] - lowest_note) ) , 5, 5, WHITE  );
          seq.pianoroll_last_valid_note = notes[cur_step - 1];
        }
      }
    }
    else
    {
      if (notes[63] > 0)
      {
        if (notes[63] == 130)
        {
          display.fillRect ( 34 + (63) * 7,  ypos - 10 - (8.15 * notes_display_shift )  - (8.15 * (seq.pianoroll_last_valid_note - lowest_note) ) , 5, 5, DX_CYAN  );
        }
        else
        {
          display.fillRect  ( 34 + (63) * 7,  ypos - 10 - (8.15 * notes_display_shift )  - (8.15 * (notes[63] - lowest_note) ) , 5, 5, WHITE  );
          seq.pianoroll_last_valid_note = notes[63];
        }
      }
    }
  }
}

void print_merged_pattern_pianoroll (int xpos, int ypos, uint8_t track_number)
{
  uint8_t notes[64];
  uint8_t lowest_note = 127;
  int notes_display_shift = 0;
  uint8_t last_valid_note = 254;
  display.setTextSize(2);
  display.setCursor (0, 320 - CHAR_height);
  display.setTextColor(WHITE, DX_MAGENTA);
  display.print("MOVE Y");
  display.setCursor(480 - CHAR_width * 6, 320 - CHAR_height);
  display.print("MOVE X");
  display.setCursor (0, 0);
  display.print("TRACK: [");
  display.print(track_number + 1);
  display.print("] ");
  display.setTextColor(WHITE, BLACK);
  display.print(" ");
  display.setTextColor(WHITE, BLUE);
  display.print(" PATTERN CHAIN: ");

  seq_print_formatted_number( seq.patternchain[0][track_number]  , 2);
  display.write (0xf8);
  seq_print_formatted_number( seq.patternchain[1][track_number]  , 2);
  display.write (0xf8);
  seq_print_formatted_number( seq.patternchain[2][track_number]  , 2);
  display.write (0xf8);
  seq_print_formatted_number( seq.patternchain[3][track_number]  , 2);
  display.print(" ");
  if (seq.patternchain[0][track_number] < NUM_SEQ_PATTERN && seq.patternchain[1][track_number] < NUM_SEQ_PATTERN
      && seq.patternchain[2][track_number] < NUM_SEQ_PATTERN && seq.patternchain[3][track_number] < NUM_SEQ_PATTERN)
  {
    for (uint8_t f = 0; f < 16; f++)  // Fill array with complete data from all chain parts of track
    {
      notes[f] = seq.note_data[ seq.patternchain[0][track_number] ][f];
      notes[f + 16] = seq.note_data[ seq.patternchain[1][track_number] ][f];
      notes[f + 32] = seq.note_data[ seq.patternchain[2][track_number] ][f];
      notes[f + 48] = seq.note_data[ seq.patternchain[3][track_number] ][f];
    }
    //find lowest note
    for (uint8_t f = 0; f < 64; f++)
    {
      if (notes[f] < lowest_note && notes[f] > 0)
      {
        lowest_note = notes[f];
      }
    }
    if (lowest_note > 120)
      lowest_note = 24;
    notes_display_shift = lowest_note % 12;
    print_keyboard(ypos, lowest_note / 12 );
    for (uint8_t xcount = 0; xcount < 64; xcount++)
    {
      if (notes[xcount] > 0)
      {
        if (notes[xcount] == 130)
        {
          display.fillRect ( 34 + xcount * 7,  ypos - 10 - (8.15 * notes_display_shift )  - (8.15 * (last_valid_note - lowest_note) ) , 5, 5, DX_CYAN  );
        }
        else
        {
          display.fillRect  ( 34 + xcount * 7,  ypos - 10 - (8.15 * notes_display_shift )  - (8.15 * (notes[xcount] - lowest_note) ) , 5, 5, GREY2  );
          last_valid_note = notes[xcount];
        }
      }
    }
  }
}

void seq_print_current_note_from_step(uint8_t step)
{
  if (seq.note_data[seq.active_pattern][step] == 130) //it is a latched note
  {
    display.setTextColor(GREEN, BLACK);
    display.write (0x7E);
    display.print(" ");
    //display.print("LATCH "); //Tilde Symbol for latched note
  }
  else
  {
    if (seq.vel[seq.active_pattern][step] > 209) //pitched sample
    {
      // display.print("P");
      display.print(noteNames[seq.note_data[seq.active_pattern][step] % 12 ][0] );
    }
    else
    {
      display.print(noteNames[seq.note_data[seq.active_pattern][step] % 12 ][0] );
      if (noteNames[seq.note_data[seq.active_pattern][step] % 12 ][1] != '\0' )
      {
        display.print(noteNames[seq.note_data[seq.active_pattern][step] % 12 ][1] );
      }
      display.print( (seq.note_data[seq.active_pattern][step] / 12) - 1);// print octave
      display.print(" ");
    }
  }
}

void print_merged_pattern (int xpos, int ypos, uint8_t track_number)
{
  char names[48];
  uint8_t data[48];
  uint8_t step = seq.step;
  uint8_t yspacer = 16;
  uint8_t next_chain_step = 0;
  uint8_t last_chain_step = 0;
  if (seq.chain_active_step == 0)
  {
    next_chain_step = 1;
    last_chain_step = 3;
  }
  else if (seq.chain_active_step == 1)
  {
    next_chain_step = 2;
    last_chain_step = 0;
  }
  else if (seq.chain_active_step == 2)
  {
    next_chain_step = 3;
    last_chain_step = 1;
  }
  else if (seq.chain_active_step == 3) {
    next_chain_step = 0;
    last_chain_step = 2;
  }
  display.setTextSize(1);
  for (uint8_t x = 0; x < NUM_SEQ_TRACKS; x++)
  {
    display.setCursor_textGrid(7 + 6 * x, 4);
    display.setCursor(display.getCursorX(), display.getCursorY() + 9);
    set_pattern_content_type_color( seq.patternchain[seq.chain_active_step][x] );
    seq_print_formatted_number( seq.patternchain[seq.chain_active_step][x], 2);
  }
  display.setTextSize(2);
  for (uint8_t f = 0; f < 16; f++)
  {
    names[f] = seq_find_shortname_in_track( f , seq.patternchain[last_chain_step][track_number] )[0];
    names[f + 16] = seq_find_shortname_in_track( f , seq.patternchain[seq.chain_active_step][track_number] )[0];
    names[f + 32] = seq_find_shortname_in_track( f , seq.patternchain[next_chain_step][track_number] )[0];
    data[f] = seq.note_data[ seq.patternchain[last_chain_step][track_number] ][f];
    data[f + 16] = seq.note_data[ seq.patternchain[seq.chain_active_step][track_number] ][f];
    data[f + 32] = seq.note_data[ seq.patternchain[next_chain_step][track_number] ][f];
  }
  for (uint8_t ycount = 0; ycount < 13; ycount++)
  {
    if ( ycount == 6)
      display.setTextColor(WHITE, BLACK);
    else
      set_pattern_content_type_color( seq.patternchain[seq.chain_active_step][track_number] );
    {
      if (seq.tracker_names_cache[ track_number ][ycount  ] != names[ycount + step + 10 ])
      {
        display.setCursor(xpos, ypos + ycount * yspacer);
        display.print( names[ycount + step + 10] );
        seq.tracker_names_cache[ track_number ][ycount  ] = names[ycount + step + 10 ];
      }
      if  (seq.tracker_data_cache[ track_number ][ycount  ] != data[ycount + step + 10] )
      {
        display.setCursor(xpos + 24, ypos + ycount * yspacer);
        seq_print_formatted_number( data[ycount + step + 10], 3);
        seq.tracker_data_cache[ track_number ][ycount  ] = data[ycount + step + 10 ];
      }
    }
  }
}

void print_keyboard_small (int xpos, int ypos, uint8_t octave, uint8_t actstep, bool fullredraw)
{
  uint8_t piano[12 * 4] = {0, 1, 0, 1, 0, 0, 1, 0, 1, 0, 1, 0,  0, 1, 0, 1, 0, 0, 1, 0, 1, 0, 1, 0, 0, 1, 0, 1, 0, 0, 1, 0, 1, 0, 1, 0, 0, 1, 0, 1, 0, 0, 1, 0, 1, 0, 1, 0, };
  uint8_t offset[5] = {12, 12, 14, 12, 11 }; //+ is up
  int offcount = 0;
  uint8_t oct_count = 0;
  uint8_t to_step = 16;
  if (fullredraw || seq.pianoroll_octave != octave) {
    seq.pianoroll_octave = octave;
    display.setTextColor(BLACK, WHITE);
    //draw white keys
    for (uint8_t y = 0; y < 15; y++)
    {
      display.fillRect(xpos, ypos - CHAR_height - (y * 14 ), 30, 13, WHITE); // pianoroll white key
      if ( y == 0 || y == 7 || y == 14) {
        display.setCursor (xpos + 17, ypos - 14 - (y * 14 )   );
        display.print("C"); display.print(octave - 1 + oct_count);
        oct_count++;
      }
    }
    for (uint8_t y = 0; y < 26; y++)
    {
      if (piano[y] == 1)
      {
        display.fillRect(xpos, ypos - (y * 8.15 ) - offset[offcount] , 12, 8, BLACK);  // BLACK key
        offcount++;
        if (offcount == 5)offcount = 0;
      }
    }
  }
  else
  {
    if (actstep < 15)
      to_step = actstep + 1;
  }
  // draw grid
  for (uint8_t y = 0; y < 26; y++)
  {
    for (uint8_t x = 0; x < to_step; x++)
    {
      if (piano[y] == 0 ) // is a white key
        display.fillRect(xpos + 36 + x * 10 , ypos + 6 - CHAR_height  - (y * 8.15 ), 5, 6, GREY3); // GRID white key
      else
        display.fillRect(xpos + 36 + x * 10 , ypos + 6 - CHAR_height  - (y * 8.15 ), 5, 6, GREY4); // GRID black key
    }
  }
}

void print_single_pattern_pianoroll (int xpos, int ypos, uint8_t pattern,  uint8_t actstep, bool fullredraw)
{
  uint8_t lowest_note = 128;
  int notes_display_shift = 0;
  uint8_t last_valid_note = 254;
  uint8_t from_step = 0;
  uint8_t to_step = 16;
  display.setTextSize(1);
  display.setTextColor(WHITE, BLACK);
  //find lowest note
  for (uint8_t f = 0; f < 16; f++)
  {
    if (seq.note_data[pattern][f] < lowest_note && seq.note_data[pattern][f] > 0)
    {
      lowest_note = seq.note_data[pattern][f];
    }
  }
  notes_display_shift = lowest_note % 12;
  if (lowest_note > 120)
    lowest_note = 24;
  print_keyboard_small(xpos, ypos, lowest_note / 12 ,  actstep, fullredraw);
  display.setTextColor(WHITE);
  for (from_step = 0; from_step < to_step; from_step++)
  {
    if (seq.note_data[pattern][from_step] > 0 &&  (ypos - 10 - (8.15 * notes_display_shift )  - (8.15 * (seq.note_data[pattern][from_step] - lowest_note))) > 5 * CHAR_height + 10 )
    {
      if (seq.note_data[pattern][from_step] == 130)
      {
        if (actstep == from_step)
          display.fillRect ( xpos + 36 + from_step * 10,  ypos - 10 - (8.15 * notes_display_shift )  - (8.15 * (last_valid_note - lowest_note) ) , 5, 5, WHITE  );
        else
          display.fillRect ( xpos + 36 + from_step * 10,  ypos - 10 - (8.15 * notes_display_shift )  - (8.15 * (last_valid_note - lowest_note) ) , 5, 5, GREEN  );
      }
      else
      {
        if (actstep == from_step)
          display.fillRect  ( xpos + 36 + from_step * 10,  ypos - 10 - (8.15 * notes_display_shift )  - (8.15 * (seq.note_data[pattern][from_step] - lowest_note) ) , 5, 5, WHITE  );
        else
          display.fillRect  ( xpos + 36 + from_step * 10,  ypos - 10 - (8.15 * notes_display_shift )  - (8.15 * (seq.note_data[pattern][from_step] - lowest_note) ) , 5, 5, get_pattern_content_type_color(pattern)  );
        last_valid_note = seq.note_data[pattern][from_step];
      }
    }
  }
  display.setTextSize(2);
  display.setTextColor(WHITE, BLACK);
}

void print_track_steps_detailed_only_current_playing_note(int xpos, int ypos, uint8_t currentstep)
{
  uint8_t i = 0;
  int y = 0;
  int x = 0;
  uint8_t z = 0;
  uint8_t array[2] = {currentstep, 99};
  display.setTextSize(1);
  display.setTextColor(DX_PURPLE, BLACK);
  display.setCursor(xpos, ypos);
  if (currentstep == 0)
    array[1] = 15;
  else if (currentstep == 15)
    array[1] = 14;
  else
    array[1] = currentstep - 1;
  while ( z < 2 )
  {
    i = array[z];
    x = xpos;
    y = ypos + 2 * CHAR_height + i * (CHAR_height - 5);
    // Short Name
    if (i == currentstep)
      display.setTextColor(WHITE, BLUE);
    else
      display.setTextColor(DX_PURPLE, BLACK);
    display.setCursor(x + CHAR_width * 3 , y);
    if (seq.vel[seq.active_pattern][i] > 209)  //it is a pitched Drum Sample
    {
      seq_print_current_note_from_step(i);
    }
    else
    {
      display.print (seq_find_shortname_in_track( i , seq.active_pattern)[0]   );
    }
    // Data values
    if (i == currentstep)
      display.setTextColor(WHITE, BLUE);
    else
      display.setTextColor(GREY1, BLACK);
    display.setCursor(x + CHAR_width * 5, y);
    seq_print_formatted_number (seq.note_data[seq.active_pattern][i] , 3);
    // Velocity values
    if (i == currentstep)
      display.setTextColor(WHITE, BLUE);
    else
      display.setTextColor(GREY2, BLACK);
    display.setCursor(x + CHAR_width * 8, y);
    seq_print_formatted_number (seq.vel[seq.active_pattern][i] , 3);
    // Long Name / Note
    if (i == currentstep)
      display.setTextColor(WHITE, BLUE);
    else
      set_pattern_content_type_color(seq.active_pattern);
    if (seq.content_type[seq.active_pattern] == 0) //Drum Track
    {
      if (seq.vel[seq.active_pattern][i] > 209)  //it is a pitched Drum Sample
      {
        display.show_smallfont_noGrid(y, 144 , 11, basename(drum_config[seq.vel[seq.active_pattern][i] - 210].name) );
      }
      else // else it is a regular Drum Sample
        display.show_smallfont_noGrid(y, 144 , 11, find_long_drum_name_from_note( seq.note_data[seq.active_pattern][i] ));
    }
    else if (seq.content_type[seq.active_pattern] > 0 ) //Inst Track or Chord or Arp
    {
      display.setCursor(x + CHAR_width * 11, y);
      if (seq.note_data[seq.active_pattern][i] != 0)
      {
        if (seq.note_data[seq.active_pattern][i] == 130) //it is a latched note
        {
          if (i == currentstep)
            display.setTextColor(WHITE, BLUE);
          else
            display.setTextColor(GREEN, BLACK);
          display.write (0x7E); display.print("LATCH"); //Tilde Symbol for latched note
        }
        else
        {
          display.print(noteNames[seq.note_data[seq.active_pattern][i] % 12 ][0] );
          if (noteNames[seq.note_data[seq.active_pattern][i] % 12 ][1] != '\0' )
          {
            display.print(noteNames[seq.note_data[seq.active_pattern][i] % 12 ][1] );
          }
          if (seq.vel[ seq.active_pattern][i] < 200) //print octave is not a chord
          {
            display.print( (seq.note_data[seq.active_pattern][i] / 12) - 1);
            display.print(" ");
          }
          if (seq.vel[ seq.active_pattern][i] > 199)  //is a chord
          {
            display.print(" ");
            print_chord_name(i);
          }
        }
      }
    }
    z++;
    while (display.getCursorX() < 205)
    {
      if (i == currentstep)
        display.setTextColor(WHITE, BLUE);
      else
        display.setTextColor(GREY1, BLACK);
      display.print(" ");
    }
  }
  display.setTextColor(WHITE, BLACK);
}

void seq_sub_pat_chain_update_running_step(int x, int y)
{
  if (seq.step == 1) {
    display.setTextSize(1);
    display.setCursor(x + 9 * CHAR_width + 6, y + (CHAR_height - 5) );
    display.setTextColor(WHITE, BLACK);
    display.print(seq.chain_active_step + 1);
    for (uint8_t r = 0; r < 4; r++)
    {
      if (seq.chain_active_step == r)
        display.drawRect(x + 5 * CHAR_width - 2 + seq.chain_active_step * 42 , y + 31, 27, 78, WHITE);
      else
        display.drawRect(x + 5 * CHAR_width - 2 + r * 42, y + 31, 27, 78, BLACK);
    }
  }
}

void update_display_functions_while_seq_running()
{
  seq.UI_last_seq_step = seq.step;
  //is in UI_func_seq_pattern_editor or is in UI_func_seq_vel_editor
  if (LCDML.FUNC_getID() == LCDML.OTHER_getIDFromFunction(UI_func_seq_pattern_editor) ||
      LCDML.FUNC_getID() == LCDML.OTHER_getIDFromFunction(UI_func_seq_vel_editor) )
  {
    seq_sub_pat_chain_update_running_step(CHAR_width * 21, CHAR_height * 9);
    display.setTextSize(2);
    if (seq.step == 0)
    {
      set_pattern_content_type_color(seq.active_pattern);
      display.setCursor_textGrid(15 , 2);
      display.print(seq_find_shortname(14)[0]);
      display.setTextColor(BLACK, WHITE);
      display.setCursor_textGrid(16, 2);
      display.print(seq_find_shortname(15)[0]);
      if (seq.note_editor_view == 0)
        print_track_steps_detailed_only_current_playing_note(CHAR_width, CHAR_height * 5 + 3, 15);
    }
    else  if (seq.step == 1)
    {
      set_pattern_content_type_color(seq.active_pattern);
      display.setCursor_textGrid(16, 2);
      display.print(seq_find_shortname(15)[0]);
      display.setTextColor(BLACK, WHITE);
      display.setCursor_textGrid(1, 2);
      display.print(seq_find_shortname(0)[0]);
      if (seq.note_editor_view == 0)
        print_track_steps_detailed_only_current_playing_note(CHAR_width, CHAR_height * 5 + 3, 0);
    }
    else
    {
      set_pattern_content_type_color(seq.active_pattern);
      display.setCursor_textGrid(seq.step - 1 , 2);
      display.print(seq_find_shortname(seq.step - 2)[0]);
      display.setTextColor(BLACK, WHITE);
      display.setCursor_textGrid(seq.step , 2);
      display.print(seq_find_shortname(seq.step - 1)[0]);
      if (seq.note_editor_view == 0)
        print_track_steps_detailed_only_current_playing_note(CHAR_width, CHAR_height * 5 + 3, seq.step - 1);
    }
    display.setTextColor(WHITE, BLACK);
  }
  else if (LCDML.FUNC_getID() == LCDML.OTHER_getIDFromFunction(UI_func_arpeggio)) //is in UI of Arpeggiator
  {
    display.setCursor_textGrid(8, 1);
    print_current_chord();
  }
  else if (LCDML.FUNC_getID() == LCDML.OTHER_getIDFromFunction(UI_func_seq_tracker)) //is in UI of Tracker
  {
    seq_print_step_numbers(CHAR_width, 96);
    print_merged_pattern( 4 * CHAR_width , 96,  0);
    print_merged_pattern( 10 * CHAR_width , 96,  1);
    print_merged_pattern( 16 * CHAR_width , 96,  2);
    print_merged_pattern( 22 * CHAR_width , 96,  3);
    print_merged_pattern( 28 * CHAR_width , 96,  4);
    print_merged_pattern( 34 * CHAR_width , 96,  5);
  }
  else if (LCDML.FUNC_getID() == LCDML.OTHER_getIDFromFunction(UI_func_seq_pianoroll)) //is in UI of Pianoroll
  {
    if (seq.step  == 0 && seq.chain_active_step == 0)
    {
      update_pianoroll(1 * CHAR_width , 320 - CHAR_height,  seq.active_track, 63);
    }
    else  if (seq.step  == 1 && seq.chain_active_step == 0)
    {
      update_pianoroll(1 * CHAR_width , 320 - CHAR_height,  seq.active_track, 0 + (seq.chain_active_step * 16));
    }
    else
    {
      update_pianoroll(1 * CHAR_width , 320 - CHAR_height,  seq.active_track, seq.step - 1 + (seq.chain_active_step * 16));
    }
  }
}

uint16_t RGB24toRGB565(uint8_t r, uint8_t g, uint8_t b)
{
  return ((r / 8) << 11) | ((g / 4) << 5) | (b / 8);
}

uint32_t ColorHSV(uint16_t hue, uint8_t sat, uint8_t val) {

  uint8_t r, g, b;

  // Remap 0-65535 to 0-1529. Pure red is CENTERED on the 64K rollover;
  // 0 is not the start of pure red, but the midpoint...a few values above
  // zero and a few below 65536 all yield pure red (similarly, 32768 is the
  // midpoint, not start, of pure cyan). The 8-bit RGB hexcone (256 values
  // each for red, green, blue) really only allows for 1530 distinct hues
  // (not 1536, more on that below), but the full unsigned 16-bit type was
  // chosen for hue so that one's code can easily handle a contiguous color
  // wheel by allowing hue to roll over in either direction.
  hue = (hue * 1530L + 32768) / 65536;
  // Because red is centered on the rollover point (the +32768 above,
  // essentially a fixed-point +0.5), the above actually yields 0 to 1530,
  // where 0 and 1530 would yield the same thing. Rather than apply a
  // costly modulo operator, 1530 is handled as a special case below.
  // So you'd think that the color "hexcone" (the thing that ramps from
  // pure red, to pure yellow, to pure green and so forth back to red,
  // yielding six slices), and with each color component having 256
  // possible values (0-255), might have 1536 possible items (6*256),
  // but in reality there's 1530. This is because the last element in
  // each 256-element slice is equal to the first element of the next
  // slice, and keeping those in there this would create small
  // discontinuities in the color wheel. So the last element of each
  // slice is dropped...we regard only elements 0-254, with item 255
  // being picked up as element 0 of the next slice. Like this:
  // Red to not-quite-pure-yellow is:        255,   0, 0 to 255, 254,   0
  // Pure yellow to not-quite-pure-green is: 255, 255, 0 to   1, 255,   0
  // Pure green to not-quite-pure-cyan is:     0, 255, 0 to   0, 255, 254
  // and so forth. Hence, 1530 distinct hues (0 to 1529), and hence why
  // the constants below are not the multiples of 256 you might expect.

  // Convert hue to R,G,B (nested ifs faster than divide+mod+switch):
  if (hue < 510) {        // Red to Green-1
    b = 0;
    if (hue < 255) {      //   Red to Yellow-1
      r = 255;
      g = hue;            //     g = 0 to 254
    } else {              //   Yellow to Green-1
      r = 510 - hue;      //     r = 255 to 1
      g = 255;
    }
  } else if (hue < 1020) { // Green to Blue-1
    r = 0;
    if (hue <  765) {     //   Green to Cyan-1
      g = 255;
      b = hue - 510;      //     b = 0 to 254
    } else {              //   Cyan to Blue-1
      g = 1020 - hue;     //     g = 255 to 1
      b = 255;
    }
  } else if (hue < 1530) { // Blue to Red-1
    g = 0;
    if (hue < 1275) {     //   Blue to Magenta-1
      r = hue - 1020;     //     r = 0 to 254
      b = 255;
    } else {              //   Magenta to Red-1
      r = 255;
      b = 1530 - hue;     //     b = 255 to 1
    }
  } else {                // Last 0.5 Red (quicker than % operator)
    r = 255;
    g = b = 0;
  }
  // Apply saturation and value to R,G,B, pack into 32-bit result:
  uint32_t v1 =   1 + val; // 1 to 256; allows >>8 instead of /255
  uint16_t s1 =   1 + sat; // 1 to 256; same reason
  uint8_t  s2 = 255 - sat; // 255 to 0

  //  return ((((((r * s1) >> 8) + s2) * v1) & 0xff00) << 8) |
  //          (((((g * s1) >> 8) + s2) * v1) & 0xff00)       |
  //         ( ((((b * s1) >> 8) + s2) * v1)           >> 8);

  return RGB24toRGB565( ((((r * s1) >> 8) + s2) * v1),  ((((g * s1) >> 8) + s2) * v1),  ( (((b * s1) >> 8) + s2) * v1)    );
}

int const qix_num = 20;

typedef struct qix_s
{
  double counthue;
  int x0s[qix_num], y0s[qix_num], x1s[qix_num], y1s[qix_num];
  int dx0 = 2, dx1 = 3, dy0 = 3, dy1 = 2;
} qix_t;

qix_s qix;

// *********************************************************************
void mFunc_screensaver(uint8_t param) //qix screensaver
// *********************************************************************
{
  if (LCDML.FUNC_setup())         // ****** SETUP *********
  {
    // remmove compiler warnings when the param variable is not used:
    LCDML_UNUSED(param);
    display.fillScreen(BLACK);
    qix.counthue = random(60000);
    // setup function
    LCDML.FUNC_setLoopInterval(20);  // starts a trigger event for the loop function every 20 milliseconds
  }
  if (LCDML.FUNC_loop())          // ****** LOOP *********
  {
    if (LCDML.BT_checkAny() || touch.touched() == true   ) // check if any button is pressed (enter, up, down, left, right)
    {
      LCDML.FUNC_goBackToMenu();  // leave this function
    }
    qix.counthue = qix.counthue + 30;

    // Move lines around

    // draw each line
    display.drawLine (qix.x0s[qix_num - 1], qix.y0s[qix_num - 1], qix.x1s[qix_num - 1], qix.y1s[qix_num - 1] , BLACK );
    for (uint8_t j = 0; j < qix_num - 1; j++)
    {
      display.drawLine (qix.x0s[j], qix.y0s[j], qix.x1s[j], qix.y1s[j], ColorHSV(qix.counthue - j * 400, 254, 254)    );
    }
    // move down
    for (uint8_t j = qix_num - 1; j >= 1; j--)
    {
      qix.x0s[j] = qix.x0s[j - 1];
      qix.x1s[j] = qix.x1s[j - 1];
      qix.y0s[j] = qix.y0s[j - 1];
      qix.y1s[j] = qix.y1s[j - 1];
    }
    // calculate next
    qix.x0s[0] += qix.dx0;
    qix.x1s[0] += qix.dx1;
    qix.y0s[0] += qix.dy0;
    qix.y1s[0] += qix.dy1;
#define limit(v, dv, max_v) {\
    if (v < 0) { v = 0; dv = (rand() & 6) + 4; } \
    if (v >= max_v) { v = max_v - 1; dv = -(rand() & 6) - 4; } \
  }
    limit(qix.x0s[0], qix.dx0, TFT_HEIGHT);
    limit(qix.x1s[0], qix.dx1, TFT_HEIGHT);
    limit(qix.y0s[0], qix.dy0, TFT_WIDTH);
    limit(qix.y1s[0], qix.dy1, TFT_WIDTH);
#undef limit
  }
  if (LCDML.FUNC_close())         // ****** STABLE END *********
  {
    display.fillScreen(BLACK);
    if (LCDML.MENU_getLastActiveFunctionID() < _LCDML_DISP_cnt - 1 )
      LCDML.OTHER_jumpToID(LCDML.MENU_getLastActiveFunctionID());
    else
      LCDML.MENU_goRoot();
  }
}

void setup_ui(void)
{
  SPI.begin();
  display.begin();
  display.setRotation(1);
  //font
  display.setTextColor(WHITE, BLACK);
  display.setTextSize(2);
  display.fillScreen(BLACK);

  // LCDMenuLib Setup
  LCDML_setup(_LCDML_DISP_cnt);
  // Enable Menu Rollover
  //LCDML.MENU_enRollover();
  //Enable Screensaver (screensaver menu function, time to activate in ms)
  LCDML.SCREEN_enable(mFunc_screensaver, 100000);
}

void toggle_sequencer_play_status()
{
  if (seq.running == false && seq.recording == false)
  {
    handleStart();
  }
  else if (seq.running == true && seq.recording == false)
  {
    handleStop();
  }
}

/***********************************************************************
   MENU CONTROL
 ***********************************************************************/
uint8_t get_current_cursor_id(void)
{
  LCDMenuLib2_menu *tmp;

  if ((tmp = LCDML.MENU_getCurrentObj()) != NULL)
    return (tmp->getChild(LCDML.MENU_getCursorPosAbs())->getID());
  else
    return (0);
}

void lcdml_menu_control(void)
{
  // If something must init, put in in the setup condition
  if (LCDML.BT_setup())
  {
    pinMode(BUT_R_PIN, INPUT_PULLUP);
    pinMode(BUT_L_PIN, INPUT_PULLUP);

    ENCODER[ENC_R].begin();
    ENCODER[ENC_L].begin();
  }

  if (back_from_volume > BACK_FROM_VOLUME_MS && LCDML.FUNC_getID() == LCDML.OTHER_getIDFromFunction(UI_func_volume))
  {
    encoderDir[ENC_L].reset();
    encoderDir[ENC_R].reset();

    if (LCDML.MENU_getLastActiveFunctionID() < 0xff)
      LCDML.OTHER_jumpToID(LCDML.MENU_getLastActiveFunctionID());
    else
      LCDML.OTHER_setCursorToID(LCDML.MENU_getLastCursorPositionID());
    //LCDML.FUNC_goBackToMenu();
  }

  // Volatile Variables
  long g_LCDML_CONTROL_Encoder_position[NUM_ENCODER] = {ENCODER[ENC_R].read(), ENCODER[ENC_L].read()};
  bool button[NUM_ENCODER] = {digitalRead(BUT_R_PIN), digitalRead(BUT_L_PIN)};

  /************************************************************************************
    Basic encoder handling (from LCDMenuLib2)
   ************************************************************************************/

  // RIGHT
  if (g_LCDML_CONTROL_Encoder_position[ENC_R] <= -3)
  {
    if (!button[ENC_R])
    {
      LCDML.BT_left();
#ifdef DEBUG
      Serial.println(F("ENC-R left"));
#endif
      encoderDir[ENC_R].Left(true);
      g_LCDML_CONTROL_button_prev[ENC_R] = LOW;
      g_LCDML_CONTROL_button_press_time[ENC_R] = -1;
    }
    else
    {
#ifdef DEBUG
      Serial.println(F("ENC-R down"));
#endif
      encoderDir[ENC_R].Down(true);
      LCDML.BT_down();
    }
    ENCODER[ENC_R].write(g_LCDML_CONTROL_Encoder_position[ENC_R] + 4);
  }
  else if (g_LCDML_CONTROL_Encoder_position[ENC_R] >= 3)
  {
    if (!button[ENC_R])
    {
#ifdef DEBUG
      Serial.println(F("ENC-R right"));
#endif
      encoderDir[ENC_R].Right(true);
      LCDML.BT_right();
      g_LCDML_CONTROL_button_prev[ENC_R] = LOW;
      g_LCDML_CONTROL_button_press_time[ENC_R] = -1;
    }
    else
    {
#ifdef DEBUG
      Serial.println(F("ENC-R up"));
#endif
      encoderDir[ENC_R].Up(true);
      LCDML.BT_up();
    }
    ENCODER[ENC_R].write(g_LCDML_CONTROL_Encoder_position[ENC_R] - 4);
  }
  else
  {
    if (!button[ENC_R] && g_LCDML_CONTROL_button_prev[ENC_R]) //falling edge, button pressed
    {
      encoderDir[ENC_R].ButtonPressed(true);
      g_LCDML_CONTROL_button_prev[ENC_R] = LOW;
      g_LCDML_CONTROL_button_press_time[ENC_R] = millis();
    }
    else if (button[ENC_R] && !g_LCDML_CONTROL_button_prev[ENC_R]) //rising edge, button not active
    {
      encoderDir[ENC_R].ButtonPressed(false);
      g_LCDML_CONTROL_button_prev[ENC_R] = HIGH;

      if (g_LCDML_CONTROL_button_press_time[ENC_R] < 0)
      {
        g_LCDML_CONTROL_button_press_time[ENC_R] = millis();
        //Reset for left right action
      }
      else if ((millis() - g_LCDML_CONTROL_button_press_time[ENC_R]) >= LONG_BUTTON_PRESS)
      {
#ifdef DEBUG
        Serial.println("ENC-R long released");
#endif
        //LCDML.BT_quit();
        encoderDir[ENC_R].ButtonLong(true);
      }
      else if ((millis() - g_LCDML_CONTROL_button_press_time[ENC_R]) >= BUT_DEBOUNCE_MS)
      {
#ifdef DEBUG
        Serial.println(F("ENC-R short"));
#endif
        encoderDir[ENC_R].ButtonShort(true);

        LCDML.BT_enter();
      }
    }
  }
  if (encoderDir[ENC_R].ButtonPressed() == true && (millis() - g_LCDML_CONTROL_button_press_time[ENC_R]) >= LONG_BUTTON_PRESS)
  {
#ifdef DEBUG
    Serial.println("ENC-R long recognized");
#endif
    encoderDir[ENC_R].ButtonLong(true);

    if (LCDML.FUNC_getID() == LCDML.OTHER_getIDFromFunction(UI_func_voice_select))
    {
      LCDML.BT_enter();
      LCDML.OTHER_updateFunc();
      LCDML.loop_menu();
      encoderDir[ENC_R].ButtonPressed(false);
      encoderDir[ENC_R].ButtonLong(false);
    }
    else
    {
      if (LCDML.FUNC_getID() < 0xff)
        LCDML.FUNC_setGBAToLastFunc();
      else
        LCDML.FUNC_setGBAToLastCursorPos();

      LCDML.OTHER_jumpToFunc(UI_func_voice_select);
      encoderDir[ENC_R].reset();
    }
  }
  // LEFT
  if (g_LCDML_CONTROL_Encoder_position[ENC_L] <= -3)
  {
    if (!button[ENC_L])
    {
#ifdef DEBUG
      Serial.println(F("ENC-L left"));
#endif
      encoderDir[ENC_L].Left(true);
      LCDML.BT_left();
      g_LCDML_CONTROL_button_prev[ENC_L] = LOW;
      g_LCDML_CONTROL_button_press_time[ENC_L] = -1;
    }
    else
    {
#ifdef DEBUG
      Serial.println(F("ENC-L down"));
#endif
      encoderDir[ENC_L].Down(true);
      LCDML.BT_down();
      if (LCDML.FUNC_getID() != LCDML.OTHER_getIDFromFunction(UI_func_volume))
      {
        LCDML.OTHER_jumpToFunc(UI_func_volume);
      }
    }
    ENCODER[ENC_L].write(g_LCDML_CONTROL_Encoder_position[ENC_L] + 4);
  }
  else if (g_LCDML_CONTROL_Encoder_position[ENC_L] >= 3)
  {
    if (!button[ENC_L])
    {
#ifdef DEBUG
      Serial.println(F("ENC-L right"));
#endif
      encoderDir[ENC_L].Right(true);
      LCDML.BT_right();
      g_LCDML_CONTROL_button_prev[ENC_L] = LOW;
      g_LCDML_CONTROL_button_press_time[ENC_L] = -1;
    }
    else
    {
#ifdef DEBUG
      Serial.println(F("ENC-L up"));
#endif
      encoderDir[ENC_L].Up(true);
      LCDML.BT_up();
      if (LCDML.FUNC_getID() != LCDML.OTHER_getIDFromFunction(UI_func_volume))
      {
        LCDML.OTHER_jumpToFunc(UI_func_volume);
      }
    }
    ENCODER[ENC_L].write(g_LCDML_CONTROL_Encoder_position[ENC_L] - 4);
  }
  else
  {
    if (!button[ENC_L] && g_LCDML_CONTROL_button_prev[ENC_L]) //falling edge, button pressed
    {
      encoderDir[ENC_L].ButtonPressed(true);
      g_LCDML_CONTROL_button_prev[ENC_L] = LOW;
      g_LCDML_CONTROL_button_press_time[ENC_L] = millis();
    }
    else if (button[ENC_L] && !g_LCDML_CONTROL_button_prev[ENC_L]) //rising edge, button not active
    {
      encoderDir[ENC_L].ButtonPressed(false);
      g_LCDML_CONTROL_button_prev[ENC_L] = HIGH;

      if (g_LCDML_CONTROL_button_press_time[ENC_L] < 0)
      {
        g_LCDML_CONTROL_button_press_time[ENC_L] = millis();
        //Reset for left right action
      }
      else if ((millis() - g_LCDML_CONTROL_button_press_time[ENC_L]) >= LONG_BUTTON_PRESS)
      {
#ifdef DEBUG
        Serial.println(F("ENC-L long released"));
#endif
        //encoderDir[ENC_L].ButtonLong(true);
        //LCDML.BT_quit();
      }
      else if ((millis() - g_LCDML_CONTROL_button_press_time[ENC_L]) >= BUT_DEBOUNCE_MS)
      {
        //LCDML.BT_enter();
#ifdef DEBUG
        Serial.println(F("ENC-L short"));
#endif
        encoderDir[ENC_L].ButtonShort(true);

        if ((LCDML.MENU_getLastActiveFunctionID() == 0xff && LCDML.MENU_getLastCursorPositionID() == 0) || menu_init == true)
        {
          LCDML.MENU_goRoot();
          menu_init = false;
        }
        else if (LCDML.FUNC_getID() == LCDML.OTHER_getIDFromFunction(UI_func_volume))
        {
          encoderDir[ENC_L].reset();
          encoderDir[ENC_R].reset();

          if (LCDML.MENU_getLastActiveFunctionID() < 0xff)
            LCDML.OTHER_jumpToID(LCDML.MENU_getLastActiveFunctionID());
          else
            LCDML.OTHER_setCursorToID(LCDML.MENU_getLastCursorPositionID());
        }
        else
          LCDML.BT_quit();
      }
    }
  }
  if (encoderDir[ENC_L].ButtonPressed() == true && (millis() - g_LCDML_CONTROL_button_press_time[ENC_L]) >= LONG_BUTTON_PRESS)
  {
#ifdef DEBUG
    Serial.println(F("ENC-L long recognized"));
#endif

    // when in Voice select Menu, long left-press sets/unsets favorite
    if (LCDML.FUNC_getID() == LCDML.OTHER_getIDFromFunction(UI_func_voice_select))
      save_favorite(configuration.dexed[selected_instance_id].bank, configuration.dexed[selected_instance_id].voice, selected_instance_id);
    // when not in Voice select Menu, long left-press starts/stops sequencer
    else if (LCDML.FUNC_getID() != LCDML.OTHER_getIDFromFunction(UI_func_voice_select))
      toggle_sequencer_play_status();

    //for (uint8_t i = 0; i < NUM_DEXED; i++)
    //  MicroDexed[i]->panic();

    encoderDir[ENC_L].reset();
    encoderDir[ENC_R].reset();
  }
}

/***********************************************************************
   MENU DISPLAY
 ***********************************************************************/
void lcdml_menu_clear(void)
{
  if (seq.menu_status == 0)
    border1_clear();
}

void lcdml_menu_display(void)
{
  // update content
  // ***************
  if (LCDML.DISP_checkMenuUpdate()) {

    // declaration of some variables
    // ***************
    // content variable
    char content_text[_LCDML_DISP_cols];  // save the content text of every menu element
    // menu element object
    LCDMenuLib2_menu *tmp;
    // some limit values
    uint8_t i = LCDML.MENU_getScroll();
    uint8_t maxi = _LCDML_DISP_rows + i;
    uint8_t n = 0;

    // check if this element has children
    if ((tmp = LCDML.MENU_getDisplayedObj()) != NULL)
    {
      display.setTextSize(2);

      // loop to display lines
      do
      {
        // check if a menu element has a condition and if the condition be true
        if (tmp->checkCondition())
        {
          // check the type off a menu element
          if (tmp->checkType_menu() == true)
          {
            // display normal content
            LCDML_getContent(content_text, tmp->getID());

            if (n == LCDML.MENU_getCursorPos())
              display.setTextColor(WHITE, DX_CYAN);
            else
              display.setTextColor(GREY1, BLACK);
            display.show(n + 1, 1, display_cols - 3, content_text);
          }
          else
          {
            if (tmp->checkType_dynParam())
            {
              tmp->callback(n);
            }
          }
          // increment some values
          i++;
          n++;
        }
        // try to go to the next sibling and check the number of displayed rows
      } while (((tmp = tmp->getSibling(1)) != NULL) && (i < maxi));

      if (i == 2) //clear menu lines if menu content < display lines
      {
        display.fillRect(CHAR_width, CHAR_height * 3, CHAR_width * 18, CHAR_height * 2, BLACK);
      }
      else if (i == 3)
      {
        display.fillRect(CHAR_width, CHAR_height * 4, CHAR_width * 18, CHAR_height, BLACK);
      }
    }
  }

  if (LCDML.DISP_checkMenuCursorUpdate())
  {
    // init vars
    uint8_t n_max             = (LCDML.MENU_getChilds() >= _LCDML_DISP_rows) ? _LCDML_DISP_rows : (LCDML.MENU_getChilds());
    uint8_t scrollbar_min     = 0;
    uint8_t scrollbar_max     = LCDML.MENU_getChilds();
    uint8_t scrollbar_cur_pos = LCDML.MENU_getCursorPosAbs();
    uint8_t scroll_pos        = ((1.*n_max * _LCDML_DISP_rows) / (scrollbar_max - 1) * scrollbar_cur_pos);

    // display rows
    for (uint8_t n = 0; n < n_max; n++)
    {
      // delete or reset scrollbar
      if (_LCDML_DISP_cfg_scrollbar == 1)
      {
        if (scrollbar_max > n_max) {
          display.drawBitmap( (_LCDML_DISP_cols - 2) * CHAR_width , (n + 1) * CHAR_height, scroll_bar[0], 8, 17, WHITE, BLACK);
        }
        else
        {
          display.fillRect((_LCDML_DISP_cols - 2) * CHAR_width , (n + 1) * CHAR_height , 10, 17 , BLACK); // Clear scrollbar
        }
      }
    }
    // display scrollbar
    if (_LCDML_DISP_cfg_scrollbar == 1) {
      if (scrollbar_max > n_max) {
        //set scroll position
        if (scrollbar_cur_pos == scrollbar_min) {
          // min pos
          display.drawBitmap( (_LCDML_DISP_cols - 2) * CHAR_width , CHAR_height, scroll_bar[1], 8, 17, WHITE, BLACK);
        } else if (scrollbar_cur_pos == (scrollbar_max - 1)) {
          // max pos
          display.drawBitmap( (_LCDML_DISP_cols - 2) * CHAR_width , (n_max ) * CHAR_height, scroll_bar[4], 8, 17, WHITE, BLACK);
        } else {
          // between
          display.drawBitmap( (_LCDML_DISP_cols - 2) * CHAR_width , (scroll_pos / n_max + 1) * CHAR_height, scroll_bar[(uint8_t)(scroll_pos % n_max) + 1], 8, 17, WHITE, BLACK);
        }
      }
      else
        display.fillRect((_LCDML_DISP_cols - 2) * CHAR_width , CHAR_height * 4, 10, 17  , BLACK); //Clear scrollbar bottom if menu options are lower than row count
    }
  }
  display.setTextColor(WHITE, BLACK);
  display.fillRect(CHAR_width * 17 - 1, 5, CHAR_width * 2 + 6, 8, BLACK); //clear upper right chars
}

//####################################################################################################################################################################################################

/***********************************************************************
   MENU
 ***********************************************************************/

#ifdef USE_FX
void UI_func_reverb_roomsize(uint8_t param)
{
  if (LCDML.FUNC_setup())         // ****** SETUP *********
  {
    encoderDir[ENC_R].reset();

    display_bar_int("Reverb Room", configuration.fx.reverb_roomsize, 1.0, REVERB_ROOMSIZE_MIN, REVERB_ROOMSIZE_MAX, 3, false, false, true);
  }

  if (LCDML.FUNC_loop())          // ****** LOOP *********
  {
    if ((LCDML.BT_checkDown() && encoderDir[ENC_R].Down()) || (LCDML.BT_checkUp() && encoderDir[ENC_R].Up()))
    {
      if (LCDML.BT_checkDown())
        configuration.fx.reverb_roomsize = constrain(configuration.fx.reverb_roomsize + ENCODER[ENC_R].speed(), REVERB_ROOMSIZE_MIN, REVERB_ROOMSIZE_MAX);
      else if (LCDML.BT_checkUp())
        configuration.fx.reverb_roomsize = constrain(configuration.fx.reverb_roomsize - ENCODER[ENC_R].speed(), REVERB_ROOMSIZE_MIN, REVERB_ROOMSIZE_MAX);
    }
    display_bar_int("Reverb Room", configuration.fx.reverb_roomsize, 1.0, REVERB_ROOMSIZE_MIN, REVERB_ROOMSIZE_MAX, 3, false, false, false);

    reverb.size(mapfloat(configuration.fx.reverb_roomsize, REVERB_ROOMSIZE_MIN, REVERB_ROOMSIZE_MAX, 0.0, 1.0));
  }

  if (LCDML.FUNC_close())     // ****** STABLE END *********
  {
    encoderDir[ENC_R].reset();
  }
}

#if USE_PLATEREVERB != 1
void UI_func_reverb_damping(uint8_t param)
{
  if (LCDML.FUNC_setup())         // ****** SETUP *********
  {
    encoderDir[ENC_R].reset();

    display_bar_int("Reverb Damp.", configuration.fx.reverb_damping, 1.0, REVERB_DAMPING_MIN, REVERB_DAMPING_MAX, 3, false, false, true);
  }

  if (LCDML.FUNC_loop())          // ****** LOOP *********
  {
    if ((LCDML.BT_checkDown() && encoderDir[ENC_R].Down()) || (LCDML.BT_checkUp() && encoderDir[ENC_R].Up()))
    {
      if (LCDML.BT_checkDown())
        configuration.fx.reverb_damping = constrain(configuration.fx.reverb_damping + ENCODER[ENC_R].speed(), REVERB_DAMPING_MIN, REVERB_DAMPING_MAX);
      else if (LCDML.BT_checkUp())
        configuration.fx.reverb_damping = constrain(configuration.fx.reverb_damping - ENCODER[ENC_R].speed(), REVERB_DAMPING_MIN, REVERB_DAMPING_MAX);
    }

    display_bar_int("Reverb Damp.", configuration.fx.reverb_damping, 1.0, REVERB_DAMPING_MIN, REVERB_DAMPING_MAX, 3, false, false, false);
  }

  if (LCDML.FUNC_close())     // ****** STABLE END *********
  {
    encoderDir[ENC_R].reset();
  }
}
#else
void UI_func_reverb_lowpass(uint8_t param)
{
  if (LCDML.FUNC_setup())         // ****** SETUP *********
  {
    encoderDir[ENC_R].reset();


    display_bar_int("Reverb Lowpass", configuration.fx.reverb_lowpass, 1.0, REVERB_LOWPASS_MIN, REVERB_LOWPASS_MAX, 3, false, false, true);
  }

  if (LCDML.FUNC_loop())          // ****** LOOP *********
  {
    if ((LCDML.BT_checkDown() && encoderDir[ENC_R].Down()) || (LCDML.BT_checkUp() && encoderDir[ENC_R].Up()))
    {
      if (LCDML.BT_checkDown())
        configuration.fx.reverb_lowpass = constrain(configuration.fx.reverb_lowpass + ENCODER[ENC_R].speed(), REVERB_LOWPASS_MIN, REVERB_LOWPASS_MAX);
      else if (LCDML.BT_checkUp())
        configuration.fx.reverb_lowpass = constrain(configuration.fx.reverb_lowpass - ENCODER[ENC_R].speed(), REVERB_LOWPASS_MIN, REVERB_LOWPASS_MAX);
    }

    display_bar_int("Reverb Lowpass", configuration.fx.reverb_lowpass, 1.0, REVERB_LOWPASS_MIN, REVERB_LOWPASS_MAX, 3, false, false, false);

    reverb.lowpass(mapfloat(configuration.fx.reverb_lowpass, REVERB_LOWPASS_MIN, REVERB_LOWPASS_MAX, 0.0, 1.0));
  }

  if (LCDML.FUNC_close())     // ****** STABLE END *********
  {

    encoderDir[ENC_R].reset();
  }
}

void UI_func_reverb_lodamp(uint8_t param)
{
  if (LCDML.FUNC_setup())         // ****** SETUP *********
  {
    encoderDir[ENC_R].reset();

    display_bar_int("Reverb Lodamp.", configuration.fx.reverb_lodamp, 1.0, REVERB_LODAMP_MIN, REVERB_LODAMP_MAX, 3, false, false, true);
  }

  if (LCDML.FUNC_loop())          // ****** LOOP *********
  {
    if ((LCDML.BT_checkDown() && encoderDir[ENC_R].Down()) || (LCDML.BT_checkUp() && encoderDir[ENC_R].Up()))
    {
      if (LCDML.BT_checkDown())
        configuration.fx.reverb_lodamp = constrain(configuration.fx.reverb_lodamp + ENCODER[ENC_R].speed(), REVERB_LODAMP_MIN, REVERB_LODAMP_MAX);
      else if (LCDML.BT_checkUp())
        configuration.fx.reverb_lodamp = constrain(configuration.fx.reverb_lodamp - ENCODER[ENC_R].speed(), REVERB_LODAMP_MIN, REVERB_LODAMP_MAX);
    }

    display_bar_int("Reverb Lodamp.", configuration.fx.reverb_lodamp, 1.0, REVERB_LODAMP_MIN, REVERB_LODAMP_MAX, 3, false, false, false);

    reverb.lodamp(mapfloat(configuration.fx.reverb_lodamp, REVERB_LODAMP_MIN, REVERB_LODAMP_MAX, 0.0, 1.0));
  }

  if (LCDML.FUNC_close())     // ****** STABLE END *********
  {
    encoderDir[ENC_R].reset();
  }
}

void UI_func_reverb_hidamp(uint8_t param)
{
  if (LCDML.FUNC_setup())         // ****** SETUP *********
  {
    encoderDir[ENC_R].reset();

    display_bar_int("Reverb Hidamp.", configuration.fx.reverb_hidamp, 1.0, REVERB_HIDAMP_MIN, REVERB_HIDAMP_MAX, 3, false, false, true);
  }

  if (LCDML.FUNC_loop())          // ****** LOOP *********
  {
    if ((LCDML.BT_checkDown() && encoderDir[ENC_R].Down()) || (LCDML.BT_checkUp() && encoderDir[ENC_R].Up()))
    {
      if (LCDML.BT_checkDown())
        configuration.fx.reverb_hidamp = constrain(configuration.fx.reverb_hidamp + ENCODER[ENC_R].speed(), REVERB_HIDAMP_MIN, REVERB_HIDAMP_MAX);
      else if (LCDML.BT_checkUp())
        configuration.fx.reverb_hidamp = constrain(configuration.fx.reverb_hidamp - ENCODER[ENC_R].speed(), REVERB_HIDAMP_MIN, REVERB_HIDAMP_MAX);
    }

    display_bar_int("Reverb Hidamp.", configuration.fx.reverb_hidamp, 1.0, REVERB_HIDAMP_MIN, REVERB_HIDAMP_MAX, 3, false, false, false);

    reverb.hidamp(mapfloat(configuration.fx.reverb_hidamp, REVERB_HIDAMP_MIN, REVERB_HIDAMP_MAX, 0.0, 1.0));
  }

  if (LCDML.FUNC_close())     // ****** STABLE END *********
  {
    encoderDir[ENC_R].reset();
  }
}

void UI_func_reverb_diffusion(uint8_t param)
{
  if (LCDML.FUNC_setup())         // ****** SETUP *********
  {
    encoderDir[ENC_R].reset();

    display_bar_int("Reverb Diff.", configuration.fx.reverb_diffusion, 1.0, REVERB_DIFFUSION_MIN, REVERB_DIFFUSION_MAX, 3, false, false, true);
  }

  if (LCDML.FUNC_loop())          // ****** LOOP *********
  {
    if ((LCDML.BT_checkDown() && encoderDir[ENC_R].Down()) || (LCDML.BT_checkUp() && encoderDir[ENC_R].Up()))
    {
      if (LCDML.BT_checkDown())
        configuration.fx.reverb_diffusion = constrain(configuration.fx.reverb_diffusion + ENCODER[ENC_R].speed(), REVERB_DIFFUSION_MIN, REVERB_DIFFUSION_MAX);
      else if (LCDML.BT_checkUp())
        configuration.fx.reverb_diffusion = constrain(configuration.fx.reverb_diffusion - ENCODER[ENC_R].speed(), REVERB_DIFFUSION_MIN, REVERB_DIFFUSION_MAX);
    }

    display_bar_int("Reverb Diff.", configuration.fx.reverb_diffusion, 1.0, REVERB_DIFFUSION_MIN, REVERB_DIFFUSION_MAX, 3, false, false, false);

    reverb.diffusion(mapfloat(configuration.fx.reverb_diffusion, REVERB_DIFFUSION_MIN, REVERB_DIFFUSION_MAX, 0.0, 1.0));
  }

  if (LCDML.FUNC_close())     // ****** STABLE END *********
  {
    encoderDir[ENC_R].reset();
  }
}
#endif // PLATEREVERB != 1

void UI_func_reverb_level(uint8_t param)
{
  if (LCDML.FUNC_setup())         // ****** SETUP *********
  {
    encoderDir[ENC_R].reset();

    display_bar_int("Reverb Level", configuration.fx.reverb_level, 1.0, REVERB_LEVEL_MIN, REVERB_LEVEL_MAX, 3, false, false, true);
  }

  if (LCDML.FUNC_loop())          // ****** LOOP *********
  {
    if ((LCDML.BT_checkDown() && encoderDir[ENC_R].Down()) || (LCDML.BT_checkUp() && encoderDir[ENC_R].Up()))
    {
      if (LCDML.BT_checkDown())
        configuration.fx.reverb_level = constrain(configuration.fx.reverb_level + ENCODER[ENC_R].speed(), REVERB_LEVEL_MIN, REVERB_LEVEL_MAX);
      else if (LCDML.BT_checkUp())
        configuration.fx.reverb_level = constrain(configuration.fx.reverb_level - ENCODER[ENC_R].speed(), REVERB_LEVEL_MIN, REVERB_LEVEL_MAX);
    }

    display_bar_int("Reverb Level", configuration.fx.reverb_level, 1.0, REVERB_LEVEL_MIN, REVERB_LEVEL_MAX, 3, false, false, true);

    //master_mixer_r.gain(3, pseudo_log_curve(mapfloat(configuration.fx.reverb_level, REVERB_LEVEL_MIN, REVERB_LEVEL_MAX, 0.0, 1.0)));
    //master_mixer_l.gain(3, pseudo_log_curve(mapfloat(configuration.fx.reverb_level, REVERB_LEVEL_MIN, REVERB_LEVEL_MAX, 0.0, 1.0)));
    master_mixer_r.gain(3, volume_transform(mapfloat(configuration.fx.reverb_level, REVERB_LEVEL_MIN, REVERB_LEVEL_MAX, 0.0, VOL_MAX_FLOAT)));
    master_mixer_l.gain(3, volume_transform(mapfloat(configuration.fx.reverb_level, REVERB_LEVEL_MIN, REVERB_LEVEL_MAX, 0.0, VOL_MAX_FLOAT)));
  }

  if (LCDML.FUNC_close())     // ****** STABLE END *********
  {
    encoderDir[ENC_R].reset();
  }
}

void UI_func_chorus_frequency(uint8_t param)
{
  if (LCDML.FUNC_setup())         // ****** SETUP *********
  {
    encoderDir[ENC_R].reset();

    display_bar_float("Chorus Frq.", configuration.fx.chorus_frequency[selected_instance_id], 0.1, CHORUS_FREQUENCY_MIN, CHORUS_FREQUENCY_MAX, 2, 1, false, false, true);

    display_active_instance_number(selected_instance_id);
    UI_update_instance_icons();
  }

  if (LCDML.FUNC_loop())          // ****** LOOP *********
  {
    if ((LCDML.BT_checkDown() && encoderDir[ENC_R].Down()) || (LCDML.BT_checkUp() && encoderDir[ENC_R].Up()) || (LCDML.BT_checkEnter() && encoderDir[ENC_R].ButtonShort()))
    {
      if (LCDML.BT_checkDown())
        configuration.fx.chorus_frequency[selected_instance_id] = constrain(configuration.fx.chorus_frequency[selected_instance_id] + ENCODER[ENC_R].speed(), CHORUS_FREQUENCY_MIN, CHORUS_FREQUENCY_MAX);
      else if (LCDML.BT_checkUp())
        configuration.fx.chorus_frequency[selected_instance_id] = constrain(configuration.fx.chorus_frequency[selected_instance_id] - ENCODER[ENC_R].speed(), CHORUS_FREQUENCY_MIN, CHORUS_FREQUENCY_MAX);
#if NUM_DEXED > 1
      else if (LCDML.BT_checkEnter())
      {
        selected_instance_id = !selected_instance_id;
        display_active_instance_number(selected_instance_id);
        UI_update_instance_icons();
      }
#endif
    }
    display_bar_float("Chorus Frq.", configuration.fx.chorus_frequency[selected_instance_id], 0.1, CHORUS_FREQUENCY_MIN, CHORUS_FREQUENCY_MAX, 2, 1, false, false, false);

    chorus_modulator[selected_instance_id]->frequency(configuration.fx.chorus_frequency[selected_instance_id] / 10.0);
  }

  if (LCDML.FUNC_close())     // ****** STABLE END *********
  {
    encoderDir[ENC_R].reset();
  }
}

void UI_func_chorus_waveform(uint8_t param)
{
  if (LCDML.FUNC_setup())         // ****** SETUP *********
  {
    encoderDir[ENC_R].reset();

    display.setCursor_textGrid(1, 1);
    display.print(F("Chorus Wavefrm"));

    display_active_instance_number(selected_instance_id);
    UI_update_instance_icons();
  }

  if (LCDML.FUNC_loop())          // ****** LOOP *********
  {
    if (LCDML.BT_checkDown() && encoderDir[ENC_R].Down())
      configuration.fx.chorus_waveform[selected_instance_id] = constrain(configuration.fx.chorus_waveform[selected_instance_id] + 1, CHORUS_WAVEFORM_MIN, CHORUS_WAVEFORM_MAX);
    else if (LCDML.BT_checkUp() && encoderDir[ENC_R].Up())
      configuration.fx.chorus_waveform[selected_instance_id] = constrain(configuration.fx.chorus_waveform[selected_instance_id] - 1, CHORUS_WAVEFORM_MIN, CHORUS_WAVEFORM_MAX);
#if NUM_DEXED > 1
    else if (LCDML.BT_checkEnter() && encoderDir[ENC_R].ButtonShort())
    {
      selected_instance_id = !selected_instance_id;
      display_active_instance_number(selected_instance_id);
      UI_update_instance_icons();
    }
#endif

    display.setCursor_textGrid(1, 2);
    switch (configuration.fx.chorus_waveform[selected_instance_id])
    {
      case 0:
        chorus_modulator[selected_instance_id]->begin(WAVEFORM_TRIANGLE);
        display.print(F("[TRIANGLE]"));
        break;
      case 1:
        chorus_modulator[selected_instance_id]->begin(WAVEFORM_SINE);
        display.print(F("[SINE    ]"));
        break;
      default:
        chorus_modulator[selected_instance_id]->begin(WAVEFORM_TRIANGLE);
        display.print(F("[TRIANGLE]"));
        break;
    }
  }

  if (LCDML.FUNC_close())     // ****** STABLE END *********
  {
    encoderDir[ENC_R].reset();
  }
}

void UI_func_chorus_depth(uint8_t param)
{
  if (LCDML.FUNC_setup())         // ****** SETUP *********
  {
    encoderDir[ENC_R].reset();

    display_bar_int("Chorus Dpt.", configuration.fx.chorus_depth[selected_instance_id], 1.0, CHORUS_DEPTH_MIN, CHORUS_DEPTH_MAX, 3, false, false, true);

    display_active_instance_number(selected_instance_id);
    UI_update_instance_icons();
  }

  if (LCDML.FUNC_loop())          // ****** LOOP *********
  {
    if ((LCDML.BT_checkDown() && encoderDir[ENC_R].Down()) || (LCDML.BT_checkUp() && encoderDir[ENC_R].Up()) || (LCDML.BT_checkEnter() && encoderDir[ENC_R].ButtonShort()))
    {
      if (LCDML.BT_checkDown())
        configuration.fx.chorus_depth[selected_instance_id] = constrain(configuration.fx.chorus_depth[selected_instance_id] + ENCODER[ENC_R].speed(), CHORUS_DEPTH_MIN, CHORUS_DEPTH_MAX);
      else if (LCDML.BT_checkUp())
        configuration.fx.chorus_depth[selected_instance_id] = constrain(configuration.fx.chorus_depth[selected_instance_id] - ENCODER[ENC_R].speed(), CHORUS_DEPTH_MIN, CHORUS_DEPTH_MAX);
#if NUM_DEXED > 1
      else if (LCDML.BT_checkEnter())
      {
        selected_instance_id = !selected_instance_id;
        display_active_instance_number(selected_instance_id);
        UI_update_instance_icons();
      }
#endif
    }

    display_bar_int("Chorus Dpt.", configuration.fx.chorus_depth[selected_instance_id], 1.0, CHORUS_DEPTH_MIN, CHORUS_DEPTH_MAX, 3, false, false, false);

    chorus_modulator[selected_instance_id]->amplitude(configuration.fx.chorus_depth[selected_instance_id] / 100.0);
  }

  if (LCDML.FUNC_close())     // ****** STABLE END *********
  {
    encoderDir[ENC_R].reset();
  }
}

void UI_func_chorus_level(uint8_t param)
{
  if (LCDML.FUNC_setup())         // ****** SETUP *********
  {
    encoderDir[ENC_R].reset();

    display_bar_int("Chorus Lvl.", configuration.fx.chorus_level[selected_instance_id], 1.0, CHORUS_LEVEL_MIN, CHORUS_LEVEL_MAX, 3, false, false, true);

    display_active_instance_number(selected_instance_id);
    UI_update_instance_icons();
  }

  if (LCDML.FUNC_loop())          // ****** LOOP *********
  {
    if ((LCDML.BT_checkDown() && encoderDir[ENC_R].Down()) || (LCDML.BT_checkUp() && encoderDir[ENC_R].Up()) || (LCDML.BT_checkEnter() && encoderDir[ENC_R].ButtonShort()))
    {
      if (LCDML.BT_checkDown())
      {
        configuration.fx.chorus_level[selected_instance_id] = constrain(configuration.fx.chorus_level[selected_instance_id] + ENCODER[ENC_R].speed(), CHORUS_LEVEL_MIN, CHORUS_LEVEL_MAX);
        MD_sendControlChange(configuration.dexed[selected_instance_id].midi_channel, 93, configuration.fx.chorus_level[selected_instance_id]);
      }
      else if (LCDML.BT_checkUp())
      {
        configuration.fx.chorus_level[selected_instance_id] = constrain(configuration.fx.chorus_level[selected_instance_id] - ENCODER[ENC_R].speed(), CHORUS_LEVEL_MIN, CHORUS_LEVEL_MAX);
        MD_sendControlChange(configuration.dexed[selected_instance_id].midi_channel, 93, configuration.fx.chorus_level[selected_instance_id]);
      }
#if NUM_DEXED > 1
      else if (LCDML.BT_checkEnter())
      {
        selected_instance_id = !selected_instance_id;
        display_active_instance_number(selected_instance_id);
        UI_update_instance_icons();
      }
#endif
    }

    display_bar_int("Chorus Lvl.", configuration.fx.chorus_level[selected_instance_id], 1.0, CHORUS_LEVEL_MIN, CHORUS_LEVEL_MAX, 3, false, false, false);

    //chorus_mixer[selected_instance_id]->gain(0, pseudo_log_curve(1.0 - mapfloat(configuration.fx.chorus_level[selected_instance_id], CHORUS_LEVEL_MIN, CHORUS_LEVEL_MAX, 0.0, 0.5)));
    //chorus_mixer[selected_instance_id]->gain(1, pseudo_log_curve(mapfloat(configuration.fx.chorus_level[selected_instance_id], CHORUS_LEVEL_MIN, CHORUS_LEVEL_MAX, 0.0, 0.5)));
    //chorus_mixer[selected_instance_id]->gain(0, 1.0 - mapfloat(configuration.fx.chorus_level[selected_instance_id], CHORUS_LEVEL_MIN, CHORUS_LEVEL_MAX, 0.0, 0.5));
    chorus_mixer[selected_instance_id]->gain(1, mapfloat(configuration.fx.chorus_level[selected_instance_id], CHORUS_LEVEL_MIN, CHORUS_LEVEL_MAX, 0.0, 0.5));
  }

  if (LCDML.FUNC_close())     // ****** STABLE END *********
  {
    encoderDir[ENC_R].reset();
  }
}

void UI_func_delay_time(uint8_t param)
{
  if (LCDML.FUNC_setup())         // ****** SETUP *********
  {
    encoderDir[ENC_R].reset();
    if (configuration.fx.delay_sync[selected_instance_id] > 0)
    {
      display_delay_sync(configuration.fx.delay_sync[selected_instance_id]); //goto MIDI Sync
    }
    else
    {
#if DELAY_TIME_MAX >= 100
      display_bar_int("Delay Time", configuration.fx.delay_time[selected_instance_id], 10.0, DELAY_TIME_MIN, DELAY_TIME_MAX, 4, false, false, true);
#else
      display_bar_int("Delay Time", configuration.fx.delay_time[selected_instance_id], 10.0, DELAY_TIME_MIN, DELAY_TIME_MAX, 3, false, false, true);
#endif
    }
    display_active_instance_number(selected_instance_id);
    UI_update_instance_icons();
  }

  if (LCDML.FUNC_loop())          // ****** LOOP *********
  {
    if ((LCDML.BT_checkDown() && encoderDir[ENC_R].Down()) || (LCDML.BT_checkUp() && encoderDir[ENC_R].Up()) || (LCDML.BT_checkEnter() && encoderDir[ENC_R].ButtonShort()))
    {
      if (LCDML.BT_checkDown())
      {
        if (configuration.fx.delay_time[selected_instance_id] == DELAY_TIME_MIN && configuration.fx.delay_sync[selected_instance_id] > DELAY_SYNC_MIN)
        {
          // MIDI-sync delay
          configuration.fx.delay_sync[selected_instance_id] = constrain(configuration.fx.delay_sync[selected_instance_id] - 1, DELAY_SYNC_MIN, DELAY_SYNC_MAX);
        }
        else
        {
          configuration.fx.delay_time[selected_instance_id] = constrain(configuration.fx.delay_time[selected_instance_id] + ENCODER[ENC_R].speed(), DELAY_TIME_MIN, DELAY_TIME_MAX);
          MD_sendControlChange(configuration.dexed[selected_instance_id].midi_channel, 105, configuration.fx.delay_time[selected_instance_id]);
        }
      }
      else if (LCDML.BT_checkUp())
      {
        if (configuration.fx.delay_time[selected_instance_id] == DELAY_TIME_MIN && configuration.fx.delay_sync[selected_instance_id] > DELAY_SYNC_MIN)
        {
          // MIDI-sync delay
          configuration.fx.delay_sync[selected_instance_id] = constrain(configuration.fx.delay_sync[selected_instance_id] + 1, DELAY_SYNC_MIN, DELAY_SYNC_MAX);
        }
        else
        {
          if (configuration.fx.delay_time[selected_instance_id] == DELAY_TIME_MIN)
            configuration.fx.delay_sync[selected_instance_id] = DELAY_SYNC_MIN + 1;
          else
          {
            configuration.fx.delay_time[selected_instance_id] = constrain(configuration.fx.delay_time[selected_instance_id] - ENCODER[ENC_R].speed(), DELAY_TIME_MIN, DELAY_TIME_MAX);
            MD_sendControlChange(configuration.dexed[selected_instance_id].midi_channel, 105, configuration.fx.delay_time[selected_instance_id]);
          }
        }
      }
#if NUM_DEXED > 1
      else if (LCDML.BT_checkEnter())
      {
        selected_instance_id = !selected_instance_id;
        display_active_instance_number(selected_instance_id);
        UI_update_instance_icons();
      }
#endif
    }

    if (configuration.fx.delay_sync[selected_instance_id] > 0)
    {
      display_delay_sync(configuration.fx.delay_sync[selected_instance_id]);//MIDI Sync Delay
    }
    else
    {
#if DELAY_TIME_MAX >= 100
      display_bar_int("Delay Time", configuration.fx.delay_time[selected_instance_id], 10.0, DELAY_TIME_MIN, DELAY_TIME_MAX, 4, false, false, true);
#else
      display_bar_int("Delay Time", configuration.fx.delay_time[selected_instance_id], 10.0, DELAY_TIME_MIN, DELAY_TIME_MAX, 3, false, false, true);
#endif
      if (configuration.fx.delay_time[selected_instance_id] <= DELAY_TIME_MIN)
        delay_fx[selected_instance_id]->disable(0);
      else
        delay_fx[selected_instance_id]->delay(0, constrain(configuration.fx.delay_time[selected_instance_id], DELAY_TIME_MIN, DELAY_TIME_MAX) * 10);
    }
  }

  if (LCDML.FUNC_close())     // ****** STABLE END *********
  {
    encoderDir[ENC_R].reset();
  }
}

void UI_func_delay_feedback(uint8_t param)
{
  if (LCDML.FUNC_setup())         // ****** SETUP *********
  {
    encoderDir[ENC_R].reset();

    display_bar_int("Delay Feedb.", configuration.fx.delay_feedback[selected_instance_id], 1.0, DELAY_FEEDBACK_MIN, DELAY_FEEDBACK_MAX, 3, false, false, true);

    display_active_instance_number(selected_instance_id);
    UI_update_instance_icons();
  }

  if (LCDML.FUNC_loop())          // ****** LOOP *********
  {
    if ((LCDML.BT_checkDown() && encoderDir[ENC_R].Down()) || (LCDML.BT_checkUp() && encoderDir[ENC_R].Up()) || (LCDML.BT_checkEnter() && encoderDir[ENC_R].ButtonShort()))
    {
      if (LCDML.BT_checkDown())
      {
        configuration.fx.delay_feedback[selected_instance_id] = constrain(configuration.fx.delay_feedback[selected_instance_id] + ENCODER[ENC_R].speed(), DELAY_FEEDBACK_MIN, DELAY_FEEDBACK_MAX);
        MD_sendControlChange(configuration.dexed[selected_instance_id].midi_channel, 106, configuration.fx.delay_feedback[selected_instance_id]);
      }
      else if (LCDML.BT_checkUp())
      {
        configuration.fx.delay_feedback[selected_instance_id] = constrain(configuration.fx.delay_feedback[selected_instance_id] - ENCODER[ENC_R].speed(), DELAY_FEEDBACK_MIN, DELAY_FEEDBACK_MAX);
        MD_sendControlChange(configuration.dexed[selected_instance_id].midi_channel, 106, configuration.fx.delay_feedback[selected_instance_id]);
      }
#if NUM_DEXED > 1
      else if (LCDML.BT_checkEnter())
      {
        selected_instance_id = !selected_instance_id;
        display_active_instance_number(selected_instance_id);
        UI_update_instance_icons();
      }
#endif
    }

    display_bar_int("Delay Feedb.", configuration.fx.delay_feedback[selected_instance_id], 1.0, DELAY_FEEDBACK_MIN, DELAY_FEEDBACK_MAX, 3, false, false, false);

    delay_fb_mixer[selected_instance_id]->gain(1, midi_volume_transform(map(configuration.fx.delay_feedback[selected_instance_id], DELAY_FEEDBACK_MIN, DELAY_FEEDBACK_MAX, 0, 127))); // amount of feedback
  }

  if (LCDML.FUNC_close())     // ****** STABLE END *********
  {
    encoderDir[ENC_R].reset();
  }
}

void UI_func_delay_level(uint8_t param)
{
  if (LCDML.FUNC_setup())         // ****** SETUP *********
  {
    encoderDir[ENC_R].reset();

    display_bar_int("Delay Lvl.", configuration.fx.delay_level[selected_instance_id], 1.0, DELAY_LEVEL_MIN, DELAY_LEVEL_MAX, 3, false, false, true);

    display_active_instance_number(selected_instance_id);
    UI_update_instance_icons();
  }

  if (LCDML.FUNC_loop())          // ****** LOOP *********
  {
    if ((LCDML.BT_checkDown() && encoderDir[ENC_R].Down()) || (LCDML.BT_checkUp() && encoderDir[ENC_R].Up()) || (LCDML.BT_checkEnter() && encoderDir[ENC_R].ButtonShort()))
    {
      if (LCDML.BT_checkDown())
      {
        configuration.fx.delay_level[selected_instance_id] = constrain(configuration.fx.delay_level[selected_instance_id] + ENCODER[ENC_R].speed(), DELAY_LEVEL_MIN, DELAY_LEVEL_MAX);
        MD_sendControlChange(configuration.dexed[selected_instance_id].midi_channel, 107, configuration.fx.delay_level[selected_instance_id]);
      }
      else if (LCDML.BT_checkUp())
      {
        configuration.fx.delay_level[selected_instance_id] = constrain(configuration.fx.delay_level[selected_instance_id] - ENCODER[ENC_R].speed(), DELAY_LEVEL_MIN, DELAY_LEVEL_MAX);
        MD_sendControlChange(configuration.dexed[selected_instance_id].midi_channel, 107, configuration.fx.delay_level[selected_instance_id]);
      }
#if NUM_DEXED > 1
      else if (LCDML.BT_checkEnter())
      {
        selected_instance_id = !selected_instance_id;
        display_active_instance_number(selected_instance_id);
        UI_update_instance_icons();
      }
#endif
    }

    display_bar_int("Delay Lvl.", configuration.fx.delay_level[selected_instance_id], 1.0, DELAY_LEVEL_MIN, DELAY_LEVEL_MAX, 3, false, false, false);

    delay_mixer[selected_instance_id]->gain(1, midi_volume_transform(map(configuration.fx.delay_level[selected_instance_id], DELAY_LEVEL_MIN, DELAY_LEVEL_MAX, 0, 127)));
  }

  if (LCDML.FUNC_close())     // ****** STABLE END *********
  {
    encoderDir[ENC_R].reset();
  }
}

void UI_func_reverb_send(uint8_t param)
{
  if (LCDML.FUNC_setup())         // ****** SETUP *********
  {
    encoderDir[ENC_R].reset();

    display_bar_int("Reverb Send", configuration.fx.reverb_send[selected_instance_id], 1.0, REVERB_SEND_MIN, REVERB_SEND_MAX, 3, false, false, true);

    display_active_instance_number(selected_instance_id);
    UI_update_instance_icons();
  }

  if (LCDML.FUNC_loop())          // ****** LOOP *********
  {
    if ((LCDML.BT_checkDown() && encoderDir[ENC_R].Down()) || (LCDML.BT_checkUp() && encoderDir[ENC_R].Up()) || (LCDML.BT_checkEnter() && encoderDir[ENC_R].ButtonShort()))
    {
      if (LCDML.BT_checkDown())
      {
        configuration.fx.reverb_send[selected_instance_id] = constrain(configuration.fx.reverb_send[selected_instance_id] + ENCODER[ENC_R].speed(), REVERB_SEND_MIN, REVERB_SEND_MAX);
        MD_sendControlChange(configuration.dexed[selected_instance_id].midi_channel, 91, configuration.fx.reverb_send[selected_instance_id]);
      }
      else if (LCDML.BT_checkUp())
      {
        configuration.fx.reverb_send[selected_instance_id] = constrain(configuration.fx.reverb_send[selected_instance_id] - ENCODER[ENC_R].speed(), REVERB_SEND_MIN, REVERB_SEND_MAX);
        MD_sendControlChange(configuration.dexed[selected_instance_id].midi_channel, 91, configuration.fx.reverb_send[selected_instance_id]);
      }
#if NUM_DEXED > 1
      else if (LCDML.BT_checkEnter())
      {
        selected_instance_id = !selected_instance_id;
        display_active_instance_number(selected_instance_id);
        UI_update_instance_icons();
      }
#endif
    }

    display_bar_int("Reverb Send", configuration.fx.reverb_send[selected_instance_id], 1.0, REVERB_SEND_MIN, REVERB_SEND_MAX, 3, false, false, false);

    reverb_mixer_r.gain(selected_instance_id, volume_transform(mapfloat(configuration.fx.reverb_send[selected_instance_id], REVERB_SEND_MIN, REVERB_SEND_MAX, 0.0, VOL_MAX_FLOAT)));
    reverb_mixer_l.gain(selected_instance_id, volume_transform(mapfloat(configuration.fx.reverb_send[selected_instance_id], REVERB_SEND_MIN, REVERB_SEND_MAX, 0.0, VOL_MAX_FLOAT)));
  }

  if (LCDML.FUNC_close())     // ****** STABLE END *********
  {
    encoderDir[ENC_R].reset();
  }
}

void UI_func_filter_cutoff(uint8_t param)
{
  if (LCDML.FUNC_setup())         // ****** SETUP *********
  {
    encoderDir[ENC_R].reset();

    display_bar_int("Filter Cutoff", configuration.fx.filter_cutoff[selected_instance_id], 1.0, FILTER_CUTOFF_MIN, FILTER_CUTOFF_MAX, 3, false, false, true);

    display_active_instance_number(selected_instance_id);
    UI_update_instance_icons();
  }

  if (LCDML.FUNC_loop())          // ****** LOOP *********
  {
    if ((LCDML.BT_checkDown() && encoderDir[ENC_R].Down()) || (LCDML.BT_checkUp() && encoderDir[ENC_R].Up()) || (LCDML.BT_checkEnter() && encoderDir[ENC_R].ButtonShort()))
    {
      if (LCDML.BT_checkDown())
      {
        configuration.fx.filter_cutoff[selected_instance_id] = constrain(configuration.fx.filter_cutoff[selected_instance_id] + ENCODER[ENC_R].speed(), FILTER_CUTOFF_MIN, FILTER_CUTOFF_MAX);
        MD_sendControlChange(configuration.dexed[selected_instance_id].midi_channel, 104, configuration.fx.filter_cutoff[selected_instance_id]);
      }
      else if (LCDML.BT_checkUp())
      {
        configuration.fx.filter_cutoff[selected_instance_id] = constrain(configuration.fx.filter_cutoff[selected_instance_id] - ENCODER[ENC_R].speed(), FILTER_CUTOFF_MIN, FILTER_CUTOFF_MAX);
        MD_sendControlChange(configuration.dexed[selected_instance_id].midi_channel, 104, configuration.fx.filter_cutoff[selected_instance_id]);
      }
#if NUM_DEXED > 1
      else if (LCDML.BT_checkEnter())
      {
        selected_instance_id = !selected_instance_id;
        display_active_instance_number(selected_instance_id);
        UI_update_instance_icons();
      }
#endif
    }

    display_bar_int("Filter Cutoff", configuration.fx.filter_cutoff[selected_instance_id], 1.0, FILTER_CUTOFF_MIN, FILTER_CUTOFF_MAX, 3, false, false, false);

    MicroDexed[selected_instance_id]->setFilterCutoff(mapfloat(configuration.fx.filter_cutoff[selected_instance_id], FILTER_CUTOFF_MIN, FILTER_CUTOFF_MAX, 1.0, 0.0));
  }

  if (LCDML.FUNC_close())     // ****** STABLE END *********
  {
    encoderDir[ENC_R].reset();
  }
}

void UI_func_filter_resonance(uint8_t param)
{
  if (LCDML.FUNC_setup())         // ****** SETUP *********
  {
    encoderDir[ENC_R].reset();

    display_bar_int("Filter Reso.", configuration.fx.filter_resonance[selected_instance_id], 1.0, FILTER_RESONANCE_MIN, FILTER_RESONANCE_MAX, 3, false, false, true);

    display_active_instance_number(selected_instance_id);
    UI_update_instance_icons();
  }

  if (LCDML.FUNC_loop())          // ****** LOOP *********
  {
    if ((LCDML.BT_checkDown() && encoderDir[ENC_R].Down()) || (LCDML.BT_checkUp() && encoderDir[ENC_R].Up()) || (LCDML.BT_checkEnter() && encoderDir[ENC_R].ButtonShort()))
    {
      if (LCDML.BT_checkDown())
      {
        configuration.fx.filter_resonance[selected_instance_id] = constrain(configuration.fx.filter_resonance[selected_instance_id] + ENCODER[ENC_R].speed(), FILTER_RESONANCE_MIN, FILTER_RESONANCE_MAX);
        MD_sendControlChange(configuration.dexed[selected_instance_id].midi_channel, 103, configuration.fx.filter_resonance[selected_instance_id]);
      }
      else if (LCDML.BT_checkUp())
      {
        configuration.fx.filter_resonance[selected_instance_id] = constrain(configuration.fx.filter_resonance[selected_instance_id] - ENCODER[ENC_R].speed(), FILTER_RESONANCE_MIN, FILTER_RESONANCE_MAX);
        MD_sendControlChange(configuration.dexed[selected_instance_id].midi_channel, 103, configuration.fx.filter_resonance[selected_instance_id]);
      }
#if NUM_DEXED > 1
      else if (LCDML.BT_checkEnter())
      {
        selected_instance_id = !selected_instance_id;
        display_active_instance_number(selected_instance_id);
        UI_update_instance_icons();
      }
#endif
    }

    display_bar_int("Filter Reso.", configuration.fx.filter_resonance[selected_instance_id], 1.0, FILTER_RESONANCE_MIN, FILTER_RESONANCE_MAX, 3, false, false, false);

    MicroDexed[selected_instance_id]->setFilterResonance(mapfloat(configuration.fx.filter_resonance[selected_instance_id], FILTER_RESONANCE_MIN, FILTER_RESONANCE_MAX, 1.0, 0.0));
  }

  if (LCDML.FUNC_close())     // ****** STABLE END *********
  {
    encoderDir[ENC_R].reset();
  }
}
#endif

void UI_func_transpose(uint8_t param)
{
  if (LCDML.FUNC_setup())         // ****** SETUP *********
  {
    encoderDir[ENC_R].reset();

    display_meter_int("Transpose", configuration.dexed[selected_instance_id].transpose, 1.0, -24.0, TRANSPOSE_MIN, TRANSPOSE_MAX, 2, false, true, true);

    display_active_instance_number(selected_instance_id);
    UI_update_instance_icons();
  }

  if (LCDML.FUNC_loop())          // ****** LOOP *********
  {
    if ((LCDML.BT_checkDown() && encoderDir[ENC_R].Down()) || (LCDML.BT_checkUp() && encoderDir[ENC_R].Up()) || (LCDML.BT_checkEnter() && encoderDir[ENC_R].ButtonShort()))
    {
      if (LCDML.BT_checkDown())
        configuration.dexed[selected_instance_id].transpose = constrain(configuration.dexed[selected_instance_id].transpose + ENCODER[ENC_R].speed(), TRANSPOSE_MIN, TRANSPOSE_MAX);
      else if (LCDML.BT_checkUp())
        configuration.dexed[selected_instance_id].transpose = constrain(configuration.dexed[selected_instance_id].transpose - ENCODER[ENC_R].speed(), TRANSPOSE_MIN, TRANSPOSE_MAX);
#if NUM_DEXED > 1
      else if (LCDML.BT_checkEnter())
      {
        selected_instance_id = !selected_instance_id;
        display_active_instance_number(selected_instance_id);
        UI_update_instance_icons();
      }
#endif
    }

    display_meter_int("Transpose", configuration.dexed[selected_instance_id].transpose, 1.0, -24.0, TRANSPOSE_MIN, TRANSPOSE_MAX, 2, false, true, true);

    MicroDexed[selected_instance_id]->setTranspose(configuration.dexed[selected_instance_id].transpose);
    MicroDexed[selected_instance_id]->notesOff();
    send_sysex_param(configuration.dexed[selected_instance_id].midi_channel, 144, configuration.dexed[selected_instance_id].transpose, 0);
  }

  if (LCDML.FUNC_close())     // ****** STABLE END *********
  {
    encoderDir[ENC_R].reset();
  }
}

void UI_func_tune(uint8_t param)
{
  if (LCDML.FUNC_setup())         // ****** SETUP *********
  {
    encoderDir[ENC_R].reset();

    display_meter_int("Fine Tune", configuration.dexed[selected_instance_id].tune, 1.0, -100.0, TUNE_MIN, TUNE_MAX, 3, false, true, true);

    display_active_instance_number(selected_instance_id);
    UI_update_instance_icons();
  }

  if (LCDML.FUNC_loop())          // ****** LOOP *********
  {
    if ((LCDML.BT_checkDown() && encoderDir[ENC_R].Down()) || (LCDML.BT_checkUp() && encoderDir[ENC_R].Up()) || (LCDML.BT_checkEnter() && encoderDir[ENC_R].ButtonShort()))
    {
      if (LCDML.BT_checkDown())
      {
        configuration.dexed[selected_instance_id].tune = constrain(configuration.dexed[selected_instance_id].tune + ENCODER[ENC_R].speed(), TUNE_MIN, TUNE_MAX);
        MD_sendControlChange(configuration.dexed[selected_instance_id].midi_channel, 94, configuration.dexed[selected_instance_id].tune);
      }
      else if (LCDML.BT_checkUp())
      {
        configuration.dexed[selected_instance_id].tune = constrain(configuration.dexed[selected_instance_id].tune - ENCODER[ENC_R].speed(), TUNE_MIN, TUNE_MAX);
        MD_sendControlChange(configuration.dexed[selected_instance_id].midi_channel, 94, configuration.dexed[selected_instance_id].tune);
      }
#if NUM_DEXED > 1
      else if (LCDML.BT_checkEnter())
      {
        selected_instance_id = !selected_instance_id;
        display_active_instance_number(selected_instance_id);
        UI_update_instance_icons();
      }
#endif
    }

    display_meter_int("Fine Tune", configuration.dexed[selected_instance_id].tune, 1.0, -100.0, TUNE_MIN, TUNE_MAX, 3, false, true, false);

    MicroDexed[selected_instance_id]->setMasterTune((int((configuration.dexed[selected_instance_id].tune - 100) / 100.0 * 0x4000) << 11) * (1.0 / 12));
    MicroDexed[selected_instance_id]->doRefreshVoice();
  }

  if (LCDML.FUNC_close())     // ****** STABLE END *********
  {
    encoderDir[ENC_R].reset();
  }
}

void UI_func_midi_channel(uint8_t param)
{
  if (LCDML.FUNC_setup())         // ****** SETUP *********
  {
    encoderDir[ENC_R].reset();

    display.setCursor_textGrid(1, 1);
    display.print(F("MIDI Channel"));

    display_active_instance_number(selected_instance_id);
    UI_update_instance_icons();
  }

  if (LCDML.FUNC_loop())          // ****** LOOP *********
  {
    if (LCDML.BT_checkDown() && encoderDir[ENC_R].Down())
      configuration.dexed[selected_instance_id].midi_channel = constrain(configuration.dexed[selected_instance_id].midi_channel + ENCODER[ENC_R].speed(), MIDI_CHANNEL_MIN, MIDI_CHANNEL_MAX);
    else if (LCDML.BT_checkUp() && encoderDir[ENC_R].Up())
      configuration.dexed[selected_instance_id].midi_channel = constrain(configuration.dexed[selected_instance_id].midi_channel - ENCODER[ENC_R].speed(), MIDI_CHANNEL_MIN, MIDI_CHANNEL_MAX);
#if NUM_DEXED > 1
    else if (LCDML.BT_checkEnter() && encoderDir[ENC_R].ButtonShort())
    {
      selected_instance_id = !selected_instance_id;
      display_active_instance_number(selected_instance_id);
      UI_update_instance_icons();
    }
#endif

    display.setCursor_textGrid(1, 2);
    if (configuration.dexed[selected_instance_id].midi_channel == 0)
    {
      display.print(F("[OMNI]"));
    }
    else
    {
      display_int(configuration.dexed[selected_instance_id].midi_channel, 4, false, true, false);
    }
  }

  if (LCDML.FUNC_close())     // ****** STABLE END *********
  {
    encoderDir[ENC_R].reset();
  }
}

void getNoteName(char* noteName, uint8_t noteNumber)
{
  char notes [12][3] = {"A", "A#", "B", "C", "C#", "D", "D#", "E", "F", "F#", "G", "G#"};
  uint8_t oct_index = noteNumber - 12;
  if (noteNumber == 130) //it is a latched note
    sprintf(noteName, "L ");
  else
  {
    noteNumber -= 21;
    sprintf(noteName, "%2s%1d", notes[noteNumber % 12], oct_index / 12);
  }
}

void UI_func_lowest_note(uint8_t param)
{
  char note_name[4];

  if (LCDML.FUNC_setup())         // ****** SETUP *********
  {
    encoderDir[ENC_R].reset();

    getNoteName(note_name, configuration.dexed[selected_instance_id].lowest_note);
    display.setCursor_textGrid(1, 1);
    display.print(F("Lowest Note"));
    display.setCursor_textGrid(1, 2);
    display.print(F("["));
    display.print(note_name);
    display.print(F("]"));

    display_active_instance_number(selected_instance_id);
    UI_update_instance_icons();
  }

  if (LCDML.FUNC_loop())          // ****** LOOP *********
  {
    if ((LCDML.BT_checkDown() && encoderDir[ENC_R].Down()) || (LCDML.BT_checkUp() && encoderDir[ENC_R].Up()) || (LCDML.BT_checkEnter() && encoderDir[ENC_R].ButtonShort()))
    {
      if (LCDML.BT_checkDown())
        configuration.dexed[selected_instance_id].lowest_note = constrain(configuration.dexed[selected_instance_id].lowest_note + ENCODER[ENC_R].speed(), INSTANCE_LOWEST_NOTE_MIN, INSTANCE_LOWEST_NOTE_MAX);
      else if (LCDML.BT_checkUp())
        configuration.dexed[selected_instance_id].lowest_note = constrain(configuration.dexed[selected_instance_id].lowest_note - ENCODER[ENC_R].speed(), INSTANCE_LOWEST_NOTE_MIN, INSTANCE_LOWEST_NOTE_MAX);
#if NUM_DEXED > 1
      else if (LCDML.BT_checkEnter())
      {
        selected_instance_id = !selected_instance_id;
        display_active_instance_number(selected_instance_id);
        UI_update_instance_icons();
      }
#endif
    }

    getNoteName(note_name, configuration.dexed[selected_instance_id].lowest_note);
    display.setCursor_textGrid(2, 2);
    display.print(note_name);
  }

  if (LCDML.FUNC_close())     // ****** STABLE END *********
  {
    encoderDir[ENC_R].reset();
  }
}

void UI_func_highest_note(uint8_t param)
{
  char note_name[4];

  if (LCDML.FUNC_setup())         // ****** SETUP *********
  {
    encoderDir[ENC_R].reset();

    getNoteName(note_name, configuration.dexed[selected_instance_id].highest_note);
    display.setCursor_textGrid(1, 1);
    display.print(F("Highest Note"));
    display.setCursor_textGrid(1, 2);
    display.print(F("["));
    display.print(note_name);
    display.print(F("]"));

    display_active_instance_number(selected_instance_id);
    UI_update_instance_icons();
  }

  if (LCDML.FUNC_loop())          // ****** LOOP *********
  {
    if ((LCDML.BT_checkDown() && encoderDir[ENC_R].Down()) || (LCDML.BT_checkUp() && encoderDir[ENC_R].Up()) || (LCDML.BT_checkEnter() && encoderDir[ENC_R].ButtonShort()))
    {
      if (LCDML.BT_checkDown())
        configuration.dexed[selected_instance_id].highest_note = constrain(configuration.dexed[selected_instance_id].highest_note + ENCODER[ENC_R].speed(), INSTANCE_HIGHEST_NOTE_MIN, INSTANCE_HIGHEST_NOTE_MAX);
      else if (LCDML.BT_checkUp())
        configuration.dexed[selected_instance_id].highest_note = constrain(configuration.dexed[selected_instance_id].highest_note - ENCODER[ENC_R].speed(), INSTANCE_HIGHEST_NOTE_MIN, INSTANCE_HIGHEST_NOTE_MAX);
#if NUM_DEXED > 1
      else if (LCDML.BT_checkEnter())
      {
        selected_instance_id = !selected_instance_id;
        display_active_instance_number(selected_instance_id);
        UI_update_instance_icons();
      }
#endif
    }

    getNoteName(note_name, configuration.dexed[selected_instance_id].highest_note);
    display.setCursor_textGrid(2, 2);
    display.print(note_name);
  }

  if (LCDML.FUNC_close())     // ****** STABLE END *********
  {
    encoderDir[ENC_R].reset();
  }
}

void UI_func_sound_intensity(uint8_t param)
{
  if (LCDML.FUNC_setup())         // ****** SETUP *********
  {
    encoderDir[ENC_R].reset();

    display_bar_int("Voice Level", configuration.dexed[selected_instance_id].sound_intensity, 1.0, SOUND_INTENSITY_MIN, SOUND_INTENSITY_MAX, 3, false, false, true);

    display_active_instance_number(selected_instance_id);
    UI_update_instance_icons();
  }

  if (LCDML.FUNC_loop())          // ****** LOOP *********
  {
    if ((LCDML.BT_checkDown() && encoderDir[ENC_R].Down()) || (LCDML.BT_checkUp() && encoderDir[ENC_R].Up()) || (LCDML.BT_checkEnter() && encoderDir[ENC_R].ButtonShort()))
    {
      encoderDir[ENC_R].reset();

      if (LCDML.BT_checkDown())
      {
        configuration.dexed[selected_instance_id].sound_intensity = constrain(configuration.dexed[selected_instance_id].sound_intensity + ENCODER[ENC_R].speed(), SOUND_INTENSITY_MIN, SOUND_INTENSITY_MAX);
        MD_sendControlChange(configuration.dexed[selected_instance_id].midi_channel, 7, configuration.dexed[selected_instance_id].sound_intensity);
      }
      else if (LCDML.BT_checkUp())
      {
        configuration.dexed[selected_instance_id].sound_intensity = constrain(configuration.dexed[selected_instance_id].sound_intensity - ENCODER[ENC_R].speed(), SOUND_INTENSITY_MIN, SOUND_INTENSITY_MAX);
        MD_sendControlChange(configuration.dexed[selected_instance_id].midi_channel, 7, configuration.dexed[selected_instance_id].sound_intensity);
      }

#if NUM_DEXED > 1
      else if (LCDML.BT_checkEnter())
      {
        selected_instance_id = !selected_instance_id;
        display_active_instance_number(selected_instance_id);
        UI_update_instance_icons();
      }
#endif
    }

    display_bar_int("Voice Level", configuration.dexed[selected_instance_id].sound_intensity, 1.0, SOUND_INTENSITY_MIN, SOUND_INTENSITY_MAX, 3, false, false, false);
    MicroDexed[selected_instance_id]->setGain(midi_volume_transform(map(configuration.dexed[selected_instance_id].sound_intensity, SOUND_INTENSITY_MIN, SOUND_INTENSITY_MAX, 0, 127)));
  }

  if (LCDML.FUNC_close())     // ****** STABLE END *********
  {
    encoderDir[ENC_R].reset();
  }
}

void UI_func_panorama(uint8_t param)
{
  if (LCDML.FUNC_setup())         // ****** SETUP *********
  {
    encoderDir[ENC_R].reset();

    if (configuration.sys.mono > 0)
    {
      display.setCursor_textGrid(1, 1);
      display.print(F("Panorama"));
      display.setCursor_textGrid(1, 2);
      display.print(F("MONO-disabled"));
      return;
    }
    display_meter_float("Panorama", configuration.dexed[selected_instance_id].pan, 0.05, -20.0, PANORAMA_MIN, PANORAMA_MAX, 1, 1, false, true, true);

    display_active_instance_number(selected_instance_id);
    UI_update_instance_icons();
  }

  if (LCDML.FUNC_loop())          // ****** LOOP *********
  {
    if (LCDML.BT_checkDown() && encoderDir[ENC_R].Down() && configuration.sys.mono == 0)
    {
      configuration.dexed[selected_instance_id].pan = constrain(configuration.dexed[selected_instance_id].pan + ENCODER[ENC_R].speed(), PANORAMA_MIN, PANORAMA_MAX);
      MD_sendControlChange(configuration.dexed[selected_instance_id].midi_channel, 10, map(configuration.dexed[selected_instance_id].pan, PANORAMA_MIN, PANORAMA_MAX, 0, 127));
    }
    else if (LCDML.BT_checkUp() && encoderDir[ENC_R].Up() && configuration.sys.mono == 0)
    {
      configuration.dexed[selected_instance_id].pan = constrain(configuration.dexed[selected_instance_id].pan - ENCODER[ENC_R].speed(), PANORAMA_MIN, PANORAMA_MAX);
      MD_sendControlChange(configuration.dexed[selected_instance_id].midi_channel, 10, map(configuration.dexed[selected_instance_id].pan, PANORAMA_MIN, PANORAMA_MAX, 0, 127));
    }
#if NUM_DEXED > 1
    else if (LCDML.BT_checkEnter() && encoderDir[ENC_R].ButtonShort())
    {
      selected_instance_id = !selected_instance_id;
      display_active_instance_number(selected_instance_id);
      UI_update_instance_icons();
    }
#endif

    if (configuration.sys.mono == 0)
    {
      display_meter_float("Panorama", configuration.dexed[selected_instance_id].pan, 0.05, -20.0, PANORAMA_MIN, PANORAMA_MAX, 1, 1, false, true, false);
      mono2stereo[selected_instance_id]->panorama(mapfloat(configuration.dexed[selected_instance_id].pan, PANORAMA_MIN, PANORAMA_MAX, -1.0, 1.0));
    }
  }

  if (LCDML.FUNC_close())     // ****** STABLE END *********
  {
    encoderDir[ENC_R].reset();
  }
}

void UI_func_favorites(uint8_t param)
{
  static uint8_t old_favorites;

  if (LCDML.FUNC_setup())         // ****** SETUP *********
  {
    encoderDir[ENC_L].reset();
    old_favorites = configuration.sys.favorites;
    display.setCursor_textGrid(1, 1);
    display.print(F("Favorites"));
    display.setCursor_textGrid(1, 2);
    switch (configuration.sys.favorites)
    {
      case 0:
        display.print(F("[ All  presets ]"));
        break;
      case 1:
        display.print(F("[  FAVs. only  ]"));
        break;
      case 2:
        display.print(F("[non-FAVs. only]"));
        break;
      case 3:
        display.print(F("[random non-FAV]"));
        break;
    }
  }

  if (LCDML.FUNC_loop())          // ****** LOOP *********
  {
    if (LCDML.BT_checkDown())
      configuration.sys.favorites = constrain(configuration.sys.favorites + 1, 0, 3);
    else if (LCDML.BT_checkUp())
      configuration.sys.favorites = constrain(configuration.sys.favorites - 1, 0, 3);

    display.setCursor_textGrid(1, 2);
    switch (configuration.sys.favorites)
    {
      case 0:
        display.print(F("[ All  presets ]"));
        break;
      case 1:
        display.print(F("[  FAVs. only  ]"));
        break;
      case 2:
        display.print(F("[non-FAVs. only]"));
        break;
      case 3:
        display.print(F("[random non-FAV]"));
        break;
    }
  }

  if (LCDML.FUNC_close())     // ****** STABLE END *********
  {
    encoderDir[ENC_L].reset();
    if (old_favorites != configuration.sys.favorites)
    {
      save_sys_flag = true;
      save_sys = 0;
    }
  }
}


void UI_func_stereo_mono(uint8_t param)
{
  static uint8_t old_mono;

  if (LCDML.FUNC_setup())         // ****** SETUP *********
  {
    encoderDir[ENC_R].reset();
    old_mono = configuration.sys.mono;

    display.setCursor_textGrid(1, 1);
    display.print(F("Stereo/Mono"));
    display.setCursor_textGrid(1, 2);
    switch (configuration.sys.mono)
    {
      case 0:
        display.print(F("[STEREO]"));
        stereo2mono.stereo(true);
        break;
      case 1:
        display.print(F("[MONO  ]"));
        stereo2mono.stereo(false);
        break;
      case 2:
        display.print(F("[MONO-R]"));
        stereo2mono.stereo(false);
        break;
      case 3:
        display.print(F("[MONO-L]"));
        stereo2mono.stereo(false);
        break;
    }
  }

  if (LCDML.FUNC_loop())          // ****** LOOP *********
  {
    if (LCDML.BT_checkDown())
      configuration.sys.mono = constrain(configuration.sys.mono + 1, MONO_MIN, MONO_MAX);
    else if (LCDML.BT_checkUp())
      configuration.sys.mono = constrain(configuration.sys.mono - 1, MONO_MIN, MONO_MAX);

    display.setCursor_textGrid(1, 2);
    switch (configuration.sys.mono)
    {
      case 0:
        display.print(F("[STEREO]"));
        stereo2mono.stereo(true);
        break;
      case 1:
        display.print(F("[MONO  ]"));
        stereo2mono.stereo(false);
        break;
      case 2:
        display.print(F("[MONO-R]"));
        stereo2mono.stereo(false);
        break;
      case 3:
        display.print(F("[MONO-L]"));
        stereo2mono.stereo(false);
        break;
    }
    set_volume(configuration.sys.vol, configuration.sys.mono);
  }

  if (LCDML.FUNC_close())     // ****** STABLE END *********
  {
    encoderDir[ENC_L].reset();

    if (old_mono != configuration.sys.mono)
    {
      save_sys_flag = true;
      save_sys = 0;
    }
  }
}

void UI_func_polyphony(uint8_t param)
{
  if (LCDML.FUNC_setup())         // ****** SETUP *********
  {
    encoderDir[ENC_R].reset();

    //fix later   display_special_chars(BLOCKBAR);
#if NUM_DEXED>1
    display_bar_int("Polyphony", configuration.dexed[selected_instance_id].polyphony, 1.0, POLYPHONY_MIN, POLYPHONY_MAX - configuration.dexed[(selected_instance_id + 1) % NUM_DEXED].polyphony, 2, false, false, true);
#else
    display_bar_int("Polyphony", configuration.dexed[selected_instance_id].polyphony, 1.0, POLYPHONY_MIN, POLYPHONY_MAX, 2, false, false, true);
#endif
    display_active_instance_number(selected_instance_id);
    UI_update_instance_icons();
  }

  if (LCDML.FUNC_loop())          // ****** LOOP *********
  {
    if ((LCDML.BT_checkDown() && encoderDir[ENC_R].Down()) || (LCDML.BT_checkUp() && encoderDir[ENC_R].Up()) || (LCDML.BT_checkEnter() && encoderDir[ENC_R].ButtonShort()))
    {
      if (LCDML.BT_checkDown())
      {
#if NUM_DEXED>1
        if (configuration.dexed[selected_instance_id].polyphony < POLYPHONY_MAX - configuration.dexed[(selected_instance_id + 1) % NUM_DEXED].polyphony)
          configuration.dexed[selected_instance_id].polyphony = constrain(configuration.dexed[selected_instance_id].polyphony + 1, POLYPHONY_MIN, POLYPHONY_MAX - configuration.dexed[(selected_instance_id + 1) % NUM_DEXED].polyphony);
#else
        configuration.dexed[selected_instance_id].polyphony = constrain(configuration.dexed[selected_instance_id].polyphony + 1, POLYPHONY_MIN, POLYPHONY_MAX);
#endif
      }
      else if (LCDML.BT_checkUp())
      {
        if (configuration.dexed[selected_instance_id].polyphony - 1 < 0)
          configuration.dexed[selected_instance_id].polyphony = 0;
        else
        {
#if NUM_DEXED>1
          configuration.dexed[selected_instance_id].polyphony = constrain(configuration.dexed[selected_instance_id].polyphony - 1, POLYPHONY_MIN, POLYPHONY_MAX - configuration.dexed[(selected_instance_id + 1) % NUM_DEXED].polyphony);
#else
          configuration.dexed[selected_instance_id].polyphony = constrain(configuration.dexed[selected_instance_id].polyphony - 1, POLYPHONY_MIN, POLYPHONY_MAX);
#endif
        }
      }
#if NUM_DEXED > 1
      else if (LCDML.BT_checkEnter())
      {
        selected_instance_id = !selected_instance_id;
        display_active_instance_number(selected_instance_id);
        UI_update_instance_icons();
      }
      display_active_instance_number(selected_instance_id);
#endif
    }

#if NUM_DEXED>1
    display_bar_int("Polyphony", configuration.dexed[selected_instance_id].polyphony, 1.0, POLYPHONY_MIN, POLYPHONY_MAX - configuration.dexed[(selected_instance_id + 1) % NUM_DEXED].polyphony, 2, false, false, false);
#else
    display_bar_int("Polyphony", configuration.dexed[selected_instance_id].polyphony, 1.0, POLYPHONY_MIN, POLYPHONY_MAX, 2, false, false, false);
#endif
    MicroDexed[selected_instance_id]->setMaxNotes(configuration.dexed[selected_instance_id].polyphony);
  }

  if (LCDML.FUNC_close())     // ****** STABLE END *********
  {
    encoderDir[ENC_R].reset();
  }
}

void UI_func_mono_poly(uint8_t param)
{
  if (LCDML.FUNC_setup())         // ****** SETUP *********
  {
    encoderDir[ENC_R].reset();

    display.setCursor_textGrid(1, 1);
    display.print(F("Mono/Poly"));

    display_active_instance_number(selected_instance_id);
    UI_update_instance_icons();
  }

  if (LCDML.FUNC_loop())          // ****** LOOP *********
  {
    if ((LCDML.BT_checkDown() && encoderDir[ENC_R].Down()) || (LCDML.BT_checkUp() && encoderDir[ENC_R].Up()))
    {
      if (LCDML.BT_checkDown())
        configuration.dexed[selected_instance_id].monopoly = constrain(configuration.dexed[selected_instance_id].monopoly + 1, MONOPOLY_MIN, MONOPOLY_MAX);
      else if (LCDML.BT_checkUp())
        configuration.dexed[selected_instance_id].monopoly = constrain(configuration.dexed[selected_instance_id].monopoly - 1, MONOPOLY_MIN, MONOPOLY_MAX);

      MicroDexed[selected_instance_id]->setMonoMode(!configuration.dexed[selected_instance_id].monopoly);
      configuration.dexed[selected_instance_id].monopoly = MicroDexed[selected_instance_id]->getMonoMode();
    }
#if NUM_DEXED > 1
    if (LCDML.BT_checkEnter() && encoderDir[ENC_R].ButtonShort())
    {
      selected_instance_id = !selected_instance_id;
      display_active_instance_number(selected_instance_id);
      UI_update_instance_icons();
    }
    display_active_instance_number(selected_instance_id);
#endif
  }

  display.setCursor_textGrid(1, 2);
  switch (configuration.dexed[selected_instance_id].monopoly)
  {
    case 1:
      display.print(F("[MONOPHONIC]"));
      break;
    case 0:
      display.print(F("[POLYPHONIC]"));
      break;
  }

  if (LCDML.FUNC_close())     // ****** STABLE END *********
  {
    // fix_later   display_special_chars(SCROLLBAR);

    encoderDir[ENC_R].reset();
  }
}

void UI_func_note_refresh(uint8_t param)
{
  if (LCDML.FUNC_setup())         // ****** SETUP *********
  {
    encoderDir[ENC_R].reset();

    display.setCursor_textGrid(1, 1);
    display.print(F("Note Refresh"));

    display_active_instance_number(selected_instance_id);
    UI_update_instance_icons();
  }

  if (LCDML.FUNC_loop())          // ****** LOOP *********
  {
    if ((LCDML.BT_checkDown() && encoderDir[ENC_R].Down()) || (LCDML.BT_checkUp() && encoderDir[ENC_R].Up()) || (LCDML.BT_checkEnter() && encoderDir[ENC_R].ButtonShort()))
    {
      if (LCDML.BT_checkDown())
        configuration.dexed[selected_instance_id].note_refresh = constrain(configuration.dexed[selected_instance_id].note_refresh + 1, NOTE_REFRESH_MIN, NOTE_REFRESH_MAX);
      else if (LCDML.BT_checkUp())
        configuration.dexed[selected_instance_id].note_refresh = constrain(configuration.dexed[selected_instance_id].note_refresh - 1, NOTE_REFRESH_MIN, NOTE_REFRESH_MAX);
#if NUM_DEXED > 1
      else if (LCDML.BT_checkEnter())
      {
        selected_instance_id = !selected_instance_id;
        display_active_instance_number(selected_instance_id);
        UI_update_instance_icons();
      }
#endif
    }

    MicroDexed[selected_instance_id]->setRefreshMode(configuration.dexed[selected_instance_id].note_refresh);

    display.setCursor_textGrid(1, 2);
    switch (configuration.dexed[selected_instance_id].note_refresh)
    {
      case 0:
        display.print(F("[NORMAL     ]"));
        break;
      case 1:
        display.print(F("[RETRIGGERED]"));
        break;
    }
  }

  if (LCDML.FUNC_close())     // ****** STABLE END *********
  {
    // fix_later   display_special_chars(SCROLLBAR);

    encoderDir[ENC_R].reset();
  }
}

void UI_func_pb_range(uint8_t param)
{
  if (LCDML.FUNC_setup())         // ****** SETUP *********
  {
    encoderDir[ENC_R].reset();

    display_bar_int("PB Range", configuration.dexed[selected_instance_id].pb_range, 1.0, PB_RANGE_MIN, PB_RANGE_MAX, 2, false, false, true);

    display_active_instance_number(selected_instance_id);
    UI_update_instance_icons();
  }

  if (LCDML.FUNC_loop())          // ****** LOOP *********
  {
    if ((LCDML.BT_checkDown() && encoderDir[ENC_R].Down()) || (LCDML.BT_checkUp() && encoderDir[ENC_R].Up()) || (LCDML.BT_checkEnter() && encoderDir[ENC_R].ButtonShort()))
    {
      if (LCDML.BT_checkDown())
        configuration.dexed[selected_instance_id].pb_range = constrain(configuration.dexed[selected_instance_id].pb_range + ENCODER[ENC_R].speed(), PB_RANGE_MIN, PB_RANGE_MAX);
      else if (LCDML.BT_checkUp())
        configuration.dexed[selected_instance_id].pb_range = constrain(configuration.dexed[selected_instance_id].pb_range - ENCODER[ENC_R].speed(), PB_RANGE_MIN, PB_RANGE_MAX);
#if NUM_DEXED > 1
      else if (LCDML.BT_checkEnter())
      {
        selected_instance_id = !selected_instance_id;
        display_active_instance_number(selected_instance_id);
        UI_update_instance_icons();
      }
#endif
    }

    display_bar_int("PB Range", configuration.dexed[selected_instance_id].pb_range, 1.0, PB_RANGE_MIN, PB_RANGE_MAX, 2, false, false, false);

    MicroDexed[selected_instance_id]->setPBController(configuration.dexed[selected_instance_id].pb_range, configuration.dexed[selected_instance_id].pb_step);
    send_sysex_param(configuration.dexed[selected_instance_id].midi_channel, 65, configuration.dexed[selected_instance_id].pb_range, 2);
  }

  if (LCDML.FUNC_close())     // ****** STABLE END *********
  {
    encoderDir[ENC_R].reset();
  }
}

void UI_func_pb_step(uint8_t param)
{
  if (LCDML.FUNC_setup())         // ****** SETUP *********
  {
    encoderDir[ENC_R].reset();

    display_bar_int("PB Step", configuration.dexed[selected_instance_id].pb_step, 1.0, PB_STEP_MIN, PB_STEP_MAX, 2, false, false, true);

    display_active_instance_number(selected_instance_id);
    UI_update_instance_icons();
  }

  if (LCDML.FUNC_loop())          // ****** LOOP *********
  {
    if ((LCDML.BT_checkDown() && encoderDir[ENC_R].Down()) || (LCDML.BT_checkUp() && encoderDir[ENC_R].Up()) || (LCDML.BT_checkEnter() && encoderDir[ENC_R].ButtonShort()))
    {
      if (LCDML.BT_checkDown())
        configuration.dexed[selected_instance_id].pb_step = constrain(configuration.dexed[selected_instance_id].pb_step + ENCODER[ENC_R].speed(), PB_STEP_MIN, PB_STEP_MAX);
      else if (LCDML.BT_checkUp())
        configuration.dexed[selected_instance_id].pb_step = constrain(configuration.dexed[selected_instance_id].pb_step - ENCODER[ENC_R].speed(), PB_STEP_MIN, PB_STEP_MAX);
#if NUM_DEXED > 1
      else if (LCDML.BT_checkEnter())
      {
        selected_instance_id = !selected_instance_id;
        display_active_instance_number(selected_instance_id);
        UI_update_instance_icons();
      }
#endif
    }

    display_bar_int("PB Step", configuration.dexed[selected_instance_id].pb_step, 1.0, PB_STEP_MIN, PB_STEP_MAX, 2, false, false, false);

    MicroDexed[selected_instance_id]->setPBController(configuration.dexed[selected_instance_id].pb_range, configuration.dexed[selected_instance_id].pb_step);
    send_sysex_param(configuration.dexed[selected_instance_id].midi_channel, 66, configuration.dexed[selected_instance_id].pb_step, 2);
  }

  if (LCDML.FUNC_close())     // ****** STABLE END *********
  {
    encoderDir[ENC_R].reset();
  }
}

void UI_func_mw_range(uint8_t param)
{
  if (LCDML.FUNC_setup())         // ****** SETUP *********
  {
    encoderDir[ENC_R].reset();

    display_bar_int("MW Range", configuration.dexed[selected_instance_id].mw_range, 1.0, MW_RANGE_MIN, MW_RANGE_MAX, 2, false, false, true);

    display_active_instance_number(selected_instance_id);
    UI_update_instance_icons();
  }

  if (LCDML.FUNC_loop())          // ****** LOOP *********
  {
    if ((LCDML.BT_checkDown() && encoderDir[ENC_R].Down()) || (LCDML.BT_checkUp() && encoderDir[ENC_R].Up()) || (LCDML.BT_checkEnter() && encoderDir[ENC_R].ButtonShort()))
    {
      if (LCDML.BT_checkDown())
        configuration.dexed[selected_instance_id].mw_range = constrain(configuration.dexed[selected_instance_id].mw_range + ENCODER[ENC_R].speed(), MW_RANGE_MIN, MW_RANGE_MAX);
      else if (LCDML.BT_checkUp())
        configuration.dexed[selected_instance_id].mw_range = constrain(configuration.dexed[selected_instance_id].mw_range - ENCODER[ENC_R].speed(), MW_RANGE_MIN, MW_RANGE_MAX);
#if NUM_DEXED > 1
      else if (LCDML.BT_checkEnter())
      {
        selected_instance_id = !selected_instance_id;
        display_active_instance_number(selected_instance_id);
        UI_update_instance_icons();
      }
#endif
    }

    display_bar_int("MW Range", configuration.dexed[selected_instance_id].mw_range, 1.0, MW_RANGE_MIN, MW_RANGE_MAX, 2, false, false, false);

    MicroDexed[selected_instance_id]->setMWController(configuration.dexed[selected_instance_id].mw_range, configuration.dexed[selected_instance_id].mw_assign, configuration.dexed[selected_instance_id].mw_mode);
    send_sysex_param(configuration.dexed[selected_instance_id].midi_channel, 70, configuration.dexed[selected_instance_id].mw_range, 2);
  }

  if (LCDML.FUNC_close())     // ****** STABLE END *********
  {
    encoderDir[ENC_R].reset();
  }
}

void UI_func_mw_assign(uint8_t param)
{
  if (LCDML.FUNC_setup())         // ****** SETUP *********
  {
    encoderDir[ENC_R].reset();

    display.setCursor_textGrid(1, 1);
    display.print(F("MW Assign"));

    display_active_instance_number(selected_instance_id);
    UI_update_instance_icons();
  }

  if (LCDML.FUNC_loop())          // ****** LOOP *********
  {
    if ((LCDML.BT_checkDown() && encoderDir[ENC_R].Down()) || (LCDML.BT_checkUp() && encoderDir[ENC_R].Up()) || (LCDML.BT_checkEnter() && encoderDir[ENC_R].ButtonShort()))
    {
      if (LCDML.BT_checkDown())
        configuration.dexed[selected_instance_id].mw_assign = constrain(configuration.dexed[selected_instance_id].mw_assign + 1, MW_ASSIGN_MIN, MW_ASSIGN_MAX);
      else if (LCDML.BT_checkUp())
        configuration.dexed[selected_instance_id].mw_assign = constrain(configuration.dexed[selected_instance_id].mw_assign - 1, MW_ASSIGN_MIN, MW_ASSIGN_MAX);
#if NUM_DEXED > 1
      else if (LCDML.BT_checkEnter())
      {
        selected_instance_id = !selected_instance_id;
        display_active_instance_number(selected_instance_id);
        UI_update_instance_icons();
      }
#endif
    }

    MicroDexed[selected_instance_id]->setMWController(configuration.dexed[selected_instance_id].mw_range, configuration.dexed[selected_instance_id].mw_assign, configuration.dexed[selected_instance_id].mw_mode);
    send_sysex_param(configuration.dexed[selected_instance_id].midi_channel, 71, configuration.dexed[selected_instance_id].mw_assign, 2);

    display.setCursor_textGrid(1, 2);
    switch (configuration.dexed[selected_instance_id].mw_assign)
    {
      case 0:
        display.print(F("[   NONE    ]"));
        break;
      case 1:
        display.print(F("[PTCH       ]"));
        break;
      case 2:
        display.print(F("[     AMP   ]"));
        break;
      case 3:
        display.print(F("[PTCH AMP   ]"));
        break;
      case 4:
        display.print(F("[         EG]"));
        break;
      case 5:
        display.print(F("[PTCH     EG]"));
        break;
      case 6:
        display.print(F("[     AMP EG]"));
        break;
      case 7:
        display.print(F("[PTCH AMP EG]"));
        break;
    }
  }

  if (LCDML.FUNC_close())     // ****** STABLE END *********
  {
    encoderDir[ENC_R].reset();
  }
}

void UI_func_mw_mode(uint8_t param)
{
  if (LCDML.FUNC_setup())         // ****** SETUP *********
  {
    encoderDir[ENC_R].reset();

    display.setCursor_textGrid(1, 1);
    display.print(F("MW Mode"));

    display_active_instance_number(selected_instance_id);
    UI_update_instance_icons();
  }

  if (LCDML.FUNC_loop())          // ****** LOOP *********
  {
    if ((LCDML.BT_checkDown() && encoderDir[ENC_R].Down()) || (LCDML.BT_checkUp() && encoderDir[ENC_R].Up()) || (LCDML.BT_checkEnter() && encoderDir[ENC_R].ButtonShort()))
    {
      if (LCDML.BT_checkDown())
        configuration.dexed[selected_instance_id].mw_mode = constrain(configuration.dexed[selected_instance_id].mw_mode + 1, MW_MODE_MIN, MW_MODE_MAX);
      else if (LCDML.BT_checkUp())
        configuration.dexed[selected_instance_id].mw_mode = constrain(configuration.dexed[selected_instance_id].mw_mode - 1, MW_MODE_MIN, MW_MODE_MAX);
#if NUM_DEXED > 1
      else if (LCDML.BT_checkEnter())
      {
        selected_instance_id = !selected_instance_id;
        display_active_instance_number(selected_instance_id);
        UI_update_instance_icons();
      }
#endif
    }

    MicroDexed[selected_instance_id]->setMWController(configuration.dexed[selected_instance_id].mw_range, configuration.dexed[selected_instance_id].mw_assign, configuration.dexed[selected_instance_id].mw_mode);
    MicroDexed[selected_instance_id]->ControllersRefresh();

    display.setCursor_textGrid(1, 2);
    switch (configuration.dexed[selected_instance_id].mw_mode)
    {
      case 0:
        display.print(F("[LINEAR      ]"));
        break;
      case 1:
        display.print(F("[REVERSE LIN.]"));
        break;
      case 2:
        display.print(F("[DIRECT      ]"));
        break;
    }
  }

  if (LCDML.FUNC_close())     // ****** STABLE END *********
  {
    encoderDir[ENC_R].reset();
  }
}

void UI_func_fc_range(uint8_t param)
{
  if (LCDML.FUNC_setup())         // ****** SETUP *********
  {
    encoderDir[ENC_R].reset();

    display_bar_int("FC Range", configuration.dexed[selected_instance_id].fc_range, 1.0, FC_RANGE_MIN, FC_RANGE_MAX, 2, false, false, true);

    display_active_instance_number(selected_instance_id);
    UI_update_instance_icons();
  }

  if (LCDML.FUNC_loop())          // ****** LOOP *********
  {
    if ((LCDML.BT_checkDown() && encoderDir[ENC_R].Down()) || (LCDML.BT_checkUp() && encoderDir[ENC_R].Up()) || (LCDML.BT_checkEnter() && encoderDir[ENC_R].ButtonShort()))
    {
      if (LCDML.BT_checkDown())
        configuration.dexed[selected_instance_id].fc_range = constrain(configuration.dexed[selected_instance_id].fc_range + ENCODER[ENC_R].speed(), FC_RANGE_MIN, FC_RANGE_MAX);
      else if (LCDML.BT_checkUp())
        configuration.dexed[selected_instance_id].fc_range = constrain(configuration.dexed[selected_instance_id].fc_range - ENCODER[ENC_R].speed(), FC_RANGE_MIN, FC_RANGE_MAX);
#if NUM_DEXED > 1
      else if (LCDML.BT_checkEnter())
      {
        selected_instance_id = !selected_instance_id;
        display_active_instance_number(selected_instance_id);
        UI_update_instance_icons();
      }
#endif
    }

    display_bar_int("FC Range", configuration.dexed[selected_instance_id].fc_range, 1.0, FC_RANGE_MIN, FC_RANGE_MAX, 2, false, false, false);

    MicroDexed[selected_instance_id]->setFCController(configuration.dexed[selected_instance_id].fc_range, configuration.dexed[selected_instance_id].fc_assign, configuration.dexed[selected_instance_id].fc_mode);
    send_sysex_param(configuration.dexed[selected_instance_id].midi_channel, 72, configuration.dexed[selected_instance_id].fc_range, 2);
  }

  if (LCDML.FUNC_close())     // ****** STABLE END *********
  {
    encoderDir[ENC_R].reset();
  }
}

void UI_func_fc_assign(uint8_t param)
{
  if (LCDML.FUNC_setup())         // ****** SETUP *********
  {
    encoderDir[ENC_R].reset();

    display.setCursor_textGrid(1, 1);
    display.print(F("FC Assign"));

    display_active_instance_number(selected_instance_id);
    UI_update_instance_icons();
  }

  if (LCDML.FUNC_loop())          // ****** LOOP *********
  {
    if ((LCDML.BT_checkDown() && encoderDir[ENC_R].Down()) || (LCDML.BT_checkUp() && encoderDir[ENC_R].Up()) || (LCDML.BT_checkEnter() && encoderDir[ENC_R].ButtonShort()))
    {
      if (LCDML.BT_checkDown())
        configuration.dexed[selected_instance_id].fc_assign = constrain(configuration.dexed[selected_instance_id].fc_assign + 1, FC_ASSIGN_MIN, FC_ASSIGN_MAX);
      else if (LCDML.BT_checkUp())
        configuration.dexed[selected_instance_id].fc_assign = constrain(configuration.dexed[selected_instance_id].fc_assign - 1, FC_ASSIGN_MIN, FC_ASSIGN_MAX);
#if NUM_DEXED > 1
      else if (LCDML.BT_checkEnter())
      {
        selected_instance_id = !selected_instance_id;
        display_active_instance_number(selected_instance_id);
        UI_update_instance_icons();
      }
#endif
    }

    MicroDexed[selected_instance_id]->setFCController(configuration.dexed[selected_instance_id].fc_range, configuration.dexed[selected_instance_id].fc_assign, configuration.dexed[selected_instance_id].fc_mode);
    send_sysex_param(configuration.dexed[selected_instance_id].midi_channel, 73, configuration.dexed[selected_instance_id].fc_assign, 2);

    display.setCursor_textGrid(1, 2);
    switch (configuration.dexed[selected_instance_id].fc_assign)
    {
      case 0:
        display.print(F("[   NONE    ]"));
        break;
      case 1:
        display.print(F("[PTCH       ]"));
        break;
      case 2:
        display.print(F("[     AMP   ]"));
        break;
      case 3:
        display.print(F("[PTCH AMP   ]"));
        break;
      case 4:
        display.print(F("[         EG]"));
        break;
      case 5:
        display.print(F("[PTCH     EG]"));
        break;
      case 6:
        display.print(F("[     AMP EG]"));
        break;
      case 7:
        display.print(F("[PTCH AMP EG]"));
        break;
    }
  }

  if (LCDML.FUNC_close())     // ****** STABLE END *********
  {
    encoderDir[ENC_R].reset();
  }
}

void UI_func_fc_mode(uint8_t param)
{
  if (LCDML.FUNC_setup())         // ****** SETUP *********
  {
    encoderDir[ENC_R].reset();

    display.setCursor_textGrid(1, 1);
    display.print(F("FC Mode"));

    display_active_instance_number(selected_instance_id);
    UI_update_instance_icons();
  }

  if (LCDML.FUNC_loop())          // ****** LOOP *********
  {
    if ((LCDML.BT_checkDown() && encoderDir[ENC_R].Down()) || (LCDML.BT_checkUp() && encoderDir[ENC_R].Up()) || (LCDML.BT_checkEnter() && encoderDir[ENC_R].ButtonShort()))
    {
      if (LCDML.BT_checkDown())
        configuration.dexed[selected_instance_id].fc_mode = constrain(configuration.dexed[selected_instance_id].fc_mode + 1, FC_MODE_MIN, FC_MODE_MAX);
      else if (LCDML.BT_checkUp())
        configuration.dexed[selected_instance_id].fc_mode = constrain(configuration.dexed[selected_instance_id].fc_mode - 1, FC_MODE_MIN, FC_MODE_MAX);
#if NUM_DEXED > 1
      else if (LCDML.BT_checkEnter())
      {
        selected_instance_id = !selected_instance_id;
        display_active_instance_number(selected_instance_id);
        UI_update_instance_icons();
      }
#endif
    }

    MicroDexed[selected_instance_id]->setFCController(configuration.dexed[selected_instance_id].fc_range, configuration.dexed[selected_instance_id].fc_assign, configuration.dexed[selected_instance_id].fc_mode);
    MicroDexed[selected_instance_id]->ControllersRefresh();

    display.setCursor_textGrid(1, 2);
    switch (configuration.dexed[selected_instance_id].fc_mode)
    {
      case 0:
        display.print(F("[LINEAR      ]"));
        break;
      case 1:
        display.print(F("[REVERSE LIN.]"));
        break;
      case 2:
        display.print(F("[DIRECT      ]"));
        break;
    }
  }

  if (LCDML.FUNC_close())     // ****** STABLE END *********
  {
    encoderDir[ENC_R].reset();
  }
}

void UI_func_bc_range(uint8_t param)
{
  if (LCDML.FUNC_setup())         // ****** SETUP *********
  {
    encoderDir[ENC_R].reset();

    display_bar_int("BC Range", configuration.dexed[selected_instance_id].bc_range, 1.0, BC_RANGE_MIN, BC_RANGE_MAX, 2, false, false, true);

    display_active_instance_number(selected_instance_id);
    UI_update_instance_icons();
  }

  if (LCDML.FUNC_loop())          // ****** LOOP *********
  {
    if ((LCDML.BT_checkDown() && encoderDir[ENC_R].Down()) || (LCDML.BT_checkUp() && encoderDir[ENC_R].Up()) || (LCDML.BT_checkEnter() && encoderDir[ENC_R].ButtonShort()))
    {
      if (LCDML.BT_checkDown())
        configuration.dexed[selected_instance_id].bc_range = constrain(configuration.dexed[selected_instance_id].bc_range + ENCODER[ENC_R].speed(), BC_RANGE_MIN, BC_RANGE_MAX);
      else if (LCDML.BT_checkUp())
        configuration.dexed[selected_instance_id].bc_range = constrain(configuration.dexed[selected_instance_id].bc_range - ENCODER[ENC_R].speed(), BC_RANGE_MIN, BC_RANGE_MAX);
#if NUM_DEXED > 1
      else if (LCDML.BT_checkEnter())
      {
        selected_instance_id = !selected_instance_id;
        display_active_instance_number(selected_instance_id);
        UI_update_instance_icons();
      }
#endif
    }

    display_bar_int("BC Range", configuration.dexed[selected_instance_id].bc_range, 1.0, BC_RANGE_MIN, BC_RANGE_MAX, 2, false, false, false);

    MicroDexed[selected_instance_id]->setBCController(configuration.dexed[selected_instance_id].bc_range, configuration.dexed[selected_instance_id].bc_assign, configuration.dexed[selected_instance_id].bc_mode);
    send_sysex_param(configuration.dexed[selected_instance_id].midi_channel, 74, configuration.dexed[selected_instance_id].bc_range, 2);
  }

  if (LCDML.FUNC_close())     // ****** STABLE END *********
  {
    encoderDir[ENC_R].reset();
  }
}

void UI_func_bc_assign(uint8_t param)
{
  if (LCDML.FUNC_setup())         // ****** SETUP *********
  {
    encoderDir[ENC_R].reset();

    display.setCursor_textGrid(1, 1);
    display.print(F("BC Assign"));

    display_active_instance_number(selected_instance_id);
    UI_update_instance_icons();
  }

  if (LCDML.FUNC_loop())          // ****** LOOP *********
  {
    if ((LCDML.BT_checkDown() && encoderDir[ENC_R].Down()) || (LCDML.BT_checkUp() && encoderDir[ENC_R].Up()) || (LCDML.BT_checkEnter() && encoderDir[ENC_R].ButtonShort()))
    {
      if (LCDML.BT_checkDown())
        configuration.dexed[selected_instance_id].bc_assign = constrain(configuration.dexed[selected_instance_id].bc_assign + 1, BC_ASSIGN_MIN, BC_ASSIGN_MAX);
      else if (LCDML.BT_checkUp())
        configuration.dexed[selected_instance_id].bc_assign = constrain(configuration.dexed[selected_instance_id].bc_assign - 1, BC_ASSIGN_MIN, BC_ASSIGN_MAX);
#if NUM_DEXED > 1
      else if (LCDML.BT_checkEnter())
      {
        selected_instance_id = !selected_instance_id;
        display_active_instance_number(selected_instance_id);
        UI_update_instance_icons();
      }
#endif
    }

    MicroDexed[selected_instance_id]->setBCController(configuration.dexed[selected_instance_id].bc_range, configuration.dexed[selected_instance_id].bc_assign, configuration.dexed[selected_instance_id].bc_mode);
    send_sysex_param(configuration.dexed[selected_instance_id].midi_channel, 75, configuration.dexed[selected_instance_id].bc_assign, 2);

    display.setCursor_textGrid(1, 2);
    switch (configuration.dexed[selected_instance_id].bc_assign)
    {
      case 0:
        display.print(F("[   NONE    ]"));
        break;
      case 1:
        display.print(F("[PTCH       ]"));
        break;
      case 2:
        display.print(F("[     AMP   ]"));
        break;
      case 3:
        display.print(F("[PTCH AMP   ]"));
        break;
      case 4:
        display.print(F("[         EG]"));
        break;
      case 5:
        display.print(F("[PTCH     EG]"));
        break;
      case 6:
        display.print(F("[     AMP EG]"));
        break;
      case 7:
        display.print(F("[PTCH AMP EG]"));
        break;
    }
  }

  if (LCDML.FUNC_close())     // ****** STABLE END *********
  {
    encoderDir[ENC_R].reset();
  }
}

void UI_func_bc_mode(uint8_t param)
{
  if (LCDML.FUNC_setup())         // ****** SETUP *********
  {
    encoderDir[ENC_R].reset();

    display.setCursor_textGrid(1, 1);
    display.print(F("BC Mode"));

    display_active_instance_number(selected_instance_id);
    UI_update_instance_icons();
  }

  if (LCDML.FUNC_loop())          // ****** LOOP *********
  {
    if ((LCDML.BT_checkDown() && encoderDir[ENC_R].Down()) || (LCDML.BT_checkUp() && encoderDir[ENC_R].Up()) || (LCDML.BT_checkEnter() && encoderDir[ENC_R].ButtonShort()))
    {
      if (LCDML.BT_checkDown())
        configuration.dexed[selected_instance_id].bc_mode = constrain(configuration.dexed[selected_instance_id].bc_mode + 1, BC_MODE_MIN, BC_MODE_MAX);
      else if (LCDML.BT_checkUp())
        configuration.dexed[selected_instance_id].bc_mode = constrain(configuration.dexed[selected_instance_id].bc_mode - 1, BC_MODE_MIN, BC_MODE_MAX);
#if NUM_DEXED > 1
      else if (LCDML.BT_checkEnter())
      {
        selected_instance_id = !selected_instance_id;
        display_active_instance_number(selected_instance_id);
        UI_update_instance_icons();
      }
#endif
    }

    MicroDexed[selected_instance_id]->setBCController(configuration.dexed[selected_instance_id].bc_range, configuration.dexed[selected_instance_id].bc_assign, configuration.dexed[selected_instance_id].bc_mode);
    MicroDexed[selected_instance_id]->ControllersRefresh();

    display.setCursor_textGrid(1, 2);
    switch (configuration.dexed[selected_instance_id].bc_mode)
    {
      case 0:
        display.print(F("[LINEAR      ]"));
        break;
      case 1:
        display.print(F("[REVERSE LIN.]"));
        break;
      case 2:
        display.print(F("[DIRECT      ]"));
        break;
    }
  }

  if (LCDML.FUNC_close())     // ****** STABLE END *********
  {
    encoderDir[ENC_R].reset();
  }
}

void UI_func_at_range(uint8_t param)
{
  if (LCDML.FUNC_setup())         // ****** SETUP *********
  {
    encoderDir[ENC_R].reset();

    display_bar_int("AT Range", configuration.dexed[selected_instance_id].at_range, 1.0, AT_RANGE_MIN, AT_RANGE_MAX, 2, false, false, true);

    display_active_instance_number(selected_instance_id);
    UI_update_instance_icons();
  }

  if (LCDML.FUNC_loop())          // ****** LOOP *********
  {
    if ((LCDML.BT_checkDown() && encoderDir[ENC_R].Down()) || (LCDML.BT_checkUp() && encoderDir[ENC_R].Up()) || (LCDML.BT_checkEnter() && encoderDir[ENC_R].ButtonShort()))
    {
      if (LCDML.BT_checkDown())
        configuration.dexed[selected_instance_id].at_range = constrain(configuration.dexed[selected_instance_id].at_range + ENCODER[ENC_R].speed(), AT_RANGE_MIN, AT_RANGE_MAX);
      else if (LCDML.BT_checkUp())
        configuration.dexed[selected_instance_id].at_range = constrain(configuration.dexed[selected_instance_id].at_range - ENCODER[ENC_R].speed(), AT_RANGE_MIN, AT_RANGE_MAX);
#if NUM_DEXED > 1
      else if (LCDML.BT_checkEnter())
      {
        selected_instance_id = !selected_instance_id;
        display_active_instance_number(selected_instance_id);
        UI_update_instance_icons();
      }
#endif
    }

    display_bar_int("AT Range", configuration.dexed[selected_instance_id].at_range, 1.0, AT_RANGE_MIN, AT_RANGE_MAX, 2, false, false, false);

    MicroDexed[selected_instance_id]->setATController(configuration.dexed[selected_instance_id].at_range, configuration.dexed[selected_instance_id].at_assign, configuration.dexed[selected_instance_id].at_mode);
    send_sysex_param(configuration.dexed[selected_instance_id].midi_channel, 76, configuration.dexed[selected_instance_id].at_range, 2);
  }

  if (LCDML.FUNC_close())     // ****** STABLE END *********
  {
    encoderDir[ENC_R].reset();
  }
}

void UI_func_at_assign(uint8_t param)
{
  if (LCDML.FUNC_setup())         // ****** SETUP *********
  {
    encoderDir[ENC_R].reset();

    display.setCursor_textGrid(1, 1);
    display.print(F("AT Assign"));

    display_active_instance_number(selected_instance_id);
    UI_update_instance_icons();
  }

  if (LCDML.FUNC_loop())          // ****** LOOP *********
  {
    if ((LCDML.BT_checkDown() && encoderDir[ENC_R].Down()) || (LCDML.BT_checkUp() && encoderDir[ENC_R].Up()) || (LCDML.BT_checkEnter() && encoderDir[ENC_R].ButtonShort()))
    {
      if (LCDML.BT_checkDown())
        configuration.dexed[selected_instance_id].at_assign = constrain(configuration.dexed[selected_instance_id].at_assign + 1, AT_ASSIGN_MIN, AT_ASSIGN_MAX);
      else if (LCDML.BT_checkUp())
        configuration.dexed[selected_instance_id].at_assign = constrain(configuration.dexed[selected_instance_id].at_assign - 1, AT_ASSIGN_MIN, AT_ASSIGN_MAX);
#if NUM_DEXED > 1
      else if (LCDML.BT_checkEnter())
      {
        selected_instance_id = !selected_instance_id;
        display_active_instance_number(selected_instance_id);
        UI_update_instance_icons();
      }
#endif
    }

    MicroDexed[selected_instance_id]->setATController(configuration.dexed[selected_instance_id].at_range, configuration.dexed[selected_instance_id].at_assign, configuration.dexed[selected_instance_id].at_mode);
    send_sysex_param(configuration.dexed[selected_instance_id].midi_channel, 77, configuration.dexed[selected_instance_id].at_assign, 2);

    display.setCursor_textGrid(1, 2);
    switch (configuration.dexed[selected_instance_id].at_assign)
    {
      case 0:
        display.print(F("[   NONE    ]"));
        break;
      case 1:
        display.print(F("[PTCH       ]"));
        break;
      case 2:
        display.print(F("[     AMP   ]"));
        break;
      case 3:
        display.print(F("[PTCH AMP   ]"));
        break;
      case 4:
        display.print(F("[         EG]"));
        break;
      case 5:
        display.print(F("[PTCH     EG]"));
        break;
      case 6:
        display.print(F("[     AMP EG]"));
        break;
      case 7:
        display.print(F("[PTCH AMP EG]"));
        break;
    }
  }

  if (LCDML.FUNC_close())     // ****** STABLE END *********
  {
    encoderDir[ENC_R].reset();
  }
}

void UI_func_at_mode(uint8_t param)
{
  if (LCDML.FUNC_setup())         // ****** SETUP *********
  {
    encoderDir[ENC_R].reset();

    display.setCursor_textGrid(1, 1);
    display.print(F("AT Mode"));

    display_active_instance_number(selected_instance_id);
    UI_update_instance_icons();
  }

  if (LCDML.FUNC_loop())          // ****** LOOP *********
  {
    if ((LCDML.BT_checkDown() && encoderDir[ENC_R].Down()) || (LCDML.BT_checkUp() && encoderDir[ENC_R].Up()) || (LCDML.BT_checkEnter() && encoderDir[ENC_R].ButtonShort()))
    {
      if (LCDML.BT_checkDown())
        configuration.dexed[selected_instance_id].at_mode = constrain(configuration.dexed[selected_instance_id].at_mode + 1, AT_MODE_MIN, AT_MODE_MAX);
      else if (LCDML.BT_checkUp())
        configuration.dexed[selected_instance_id].at_mode = constrain(configuration.dexed[selected_instance_id].at_mode - 1, AT_MODE_MIN, AT_MODE_MAX);
#if NUM_DEXED > 1
      else if (LCDML.BT_checkEnter())
      {
        selected_instance_id = !selected_instance_id;
        display_active_instance_number(selected_instance_id);
        UI_update_instance_icons();
      }
#endif
    }

    MicroDexed[selected_instance_id]->setATController(configuration.dexed[selected_instance_id].at_range, configuration.dexed[selected_instance_id].at_assign, configuration.dexed[selected_instance_id].at_mode);
    MicroDexed[selected_instance_id]->ControllersRefresh();

    display.setCursor_textGrid(1, 2);
    switch (configuration.dexed[selected_instance_id].at_mode)
    {
      case 0:
        display.print(F("[LINEAR      ]"));
        break;
      case 1:
        display.print(F("[REVERSE LIN.]"));
        break;
      case 2:
        display.print(F("[DIRECT      ]"));
        break;
    }
  }

  if (LCDML.FUNC_close())     // ****** STABLE END *********
  {
    encoderDir[ENC_R].reset();
  }
}

void UI_func_portamento_mode(uint8_t param)
{
  if (LCDML.FUNC_setup())         // ****** SETUP *********
  {
    encoderDir[ENC_R].reset();

    display.setCursor_textGrid(1, 1);
    display.print(F("Port. Mode"));

    display_active_instance_number(selected_instance_id);
    UI_update_instance_icons();
  }

  if (LCDML.FUNC_loop())          // ****** LOOP *********
  {
    if ((LCDML.BT_checkDown() && encoderDir[ENC_R].Down()) || (LCDML.BT_checkUp() && encoderDir[ENC_R].Up()) || (LCDML.BT_checkEnter() && encoderDir[ENC_R].ButtonShort()))
    {
      if (LCDML.BT_checkDown())
        configuration.dexed[selected_instance_id].portamento_mode = constrain(configuration.dexed[selected_instance_id].portamento_mode + 1, PORTAMENTO_MODE_MIN, PORTAMENTO_MODE_MAX);
      else if (LCDML.BT_checkUp())
        configuration.dexed[selected_instance_id].portamento_mode = constrain(configuration.dexed[selected_instance_id].portamento_mode - 1, PORTAMENTO_MODE_MIN, PORTAMENTO_MODE_MAX);
#if NUM_DEXED > 1
      else if (LCDML.BT_checkEnter())
      {
        selected_instance_id = !selected_instance_id;
        display_active_instance_number(selected_instance_id);
        UI_update_instance_icons();
      }
#endif
    }

    MicroDexed[selected_instance_id]->setPortamentoMode(configuration.dexed[selected_instance_id].portamento_mode, configuration.dexed[selected_instance_id].portamento_glissando, configuration.dexed[selected_instance_id].portamento_time);
    send_sysex_param(configuration.dexed[selected_instance_id].midi_channel, 67, configuration.dexed[selected_instance_id].portamento_mode, 2);

    display.setCursor_textGrid(1, 2);
    switch (configuration.dexed[selected_instance_id].portamento_mode)
    {
      case 0:
        if (configuration.dexed[selected_instance_id].monopoly == 1)
          display.print(F("[RETAIN  ]"));
        else
          display.print(F("[FINGERED]"));
        break;
      case 1:
        if (configuration.dexed[selected_instance_id].monopoly == 1)
          display.print(F("[FOLLOW  ]"));
        else
          display.print(F("[FULL    ]"));
        break;
    }
  }

  if (LCDML.FUNC_close())     // ****** STABLE END *********
  {
    encoderDir[ENC_R].reset();
  }
}

void UI_func_portamento_glissando(uint8_t param)
{
  if (LCDML.FUNC_setup())         // ****** SETUP *********
  {
    encoderDir[ENC_R].reset();

    display.setCursor_textGrid(1, 1);
    display.print(F("Port. Gliss."));

    display_active_instance_number(selected_instance_id);
    UI_update_instance_icons();
  }

  if (LCDML.FUNC_loop())          // ****** LOOP *********
  {
    if ((LCDML.BT_checkDown() && encoderDir[ENC_R].Down()) || (LCDML.BT_checkUp() && encoderDir[ENC_R].Up()) || (LCDML.BT_checkEnter() && encoderDir[ENC_R].ButtonShort()))
    {
      if (LCDML.BT_checkDown())
        configuration.dexed[selected_instance_id].portamento_glissando = constrain(configuration.dexed[selected_instance_id].portamento_glissando + 1, PORTAMENTO_GLISSANDO_MIN, PORTAMENTO_GLISSANDO_MAX);
      else if (LCDML.BT_checkUp())
        configuration.dexed[selected_instance_id].portamento_glissando = constrain(configuration.dexed[selected_instance_id].portamento_glissando - 1, PORTAMENTO_GLISSANDO_MIN, PORTAMENTO_GLISSANDO_MAX);
#if NUM_DEXED > 1
      else if (LCDML.BT_checkEnter())
      {
        selected_instance_id = !selected_instance_id;
        display_active_instance_number(selected_instance_id);
        UI_update_instance_icons();
      }
#endif
    }

    MicroDexed[selected_instance_id]->setPortamentoMode(configuration.dexed[selected_instance_id].portamento_mode, configuration.dexed[selected_instance_id].portamento_glissando, configuration.dexed[selected_instance_id].portamento_time);
    send_sysex_param(configuration.dexed[selected_instance_id].midi_channel, 68, configuration.dexed[selected_instance_id].portamento_glissando, 2);

    display.setCursor_textGrid(1, 2);
    switch (configuration.dexed[selected_instance_id].portamento_glissando)
    {
      case 0:
        display.print(F("[OFF]"));
        break;
      case 1:
        display.print(F("[ON ]"));
        break;
    }
  }

  if (LCDML.FUNC_close())     // ****** STABLE END *********
  {
    encoderDir[ENC_R].reset();
  }
}

void UI_func_portamento_time(uint8_t param)
{
  if (LCDML.FUNC_setup())         // ****** SETUP *********
  {
    encoderDir[ENC_R].reset();
    display_bar_int("Port. Time", configuration.dexed[selected_instance_id].portamento_time, 1.0, PORTAMENTO_TIME_MIN, PORTAMENTO_TIME_MAX, 2, false, false, true);
    display_active_instance_number(selected_instance_id);
    UI_update_instance_icons();
  }

  if (LCDML.FUNC_loop())          // ****** LOOP *********
  {
    if ((LCDML.BT_checkDown() && encoderDir[ENC_R].Down()) || (LCDML.BT_checkUp() && encoderDir[ENC_R].Up()) || (LCDML.BT_checkEnter() && encoderDir[ENC_R].ButtonShort()))
    {
      if (LCDML.BT_checkDown())
        configuration.dexed[selected_instance_id].portamento_time = constrain(configuration.dexed[selected_instance_id].portamento_time + ENCODER[ENC_R].speed(), PORTAMENTO_TIME_MIN, PORTAMENTO_TIME_MAX);
      else if (LCDML.BT_checkUp())
        configuration.dexed[selected_instance_id].portamento_time = constrain(configuration.dexed[selected_instance_id].portamento_time - ENCODER[ENC_R].speed(), PORTAMENTO_TIME_MIN, PORTAMENTO_TIME_MAX);
#if NUM_DEXED > 1
      else if (LCDML.BT_checkEnter())
      {
        selected_instance_id = !selected_instance_id;
        display_active_instance_number(selected_instance_id);
        UI_update_instance_icons();
      }
#endif
    }
    display_bar_int("Portam. Time", configuration.dexed[selected_instance_id].portamento_time, 1.0, PORTAMENTO_TIME_MIN, PORTAMENTO_TIME_MAX, 2, false, false, false);
    MicroDexed[selected_instance_id]->setPortamentoMode(configuration.dexed[selected_instance_id].portamento_mode, configuration.dexed[selected_instance_id].portamento_glissando, configuration.dexed[selected_instance_id].portamento_time);
    send_sysex_param(configuration.dexed[selected_instance_id].midi_channel, 69, configuration.dexed[selected_instance_id].portamento_time, 2);
  }

  if (LCDML.FUNC_close())     // ****** STABLE END *********
  {
    encoderDir[ENC_R].reset();
  }
}

void UI_handle_OP(uint8_t param)
{
  static uint8_t op_selected;

  display_OP_active_instance_number(selected_instance_id, configuration.dexed[selected_instance_id].op_enabled);

  if (LCDML.FUNC_setup())         // ****** SETUP *********
  {
    encoderDir[ENC_R].reset();

    display.setCursor_textGrid(1, 1);
    display.print(F("OP Enable"));
    display.setCursor_textGrid(1, 2);
    for (uint8_t i = 2; i < 8; i++)
      display.print(i);

    UI_update_instance_icons();

    display.setCursor_textGrid(op_selected, 1);
    // fix_later   lcd.blink();
  }

  if (LCDML.FUNC_loop())          // ****** LOOP *********
  {
    if (LCDML.BT_checkUp() && encoderDir[ENC_R].Up())
    {
#if NUM_DEXED>1
      if (op_selected == 0)
      {
        selected_instance_id = !selected_instance_id;
        op_selected = 5;
        display_OP_active_instance_number(selected_instance_id, configuration.dexed[selected_instance_id].op_enabled);
      }
      else
#endif
        op_selected = constrain(op_selected - 1, 0, 5);
    }
    else if (LCDML.BT_checkDown() && encoderDir[ENC_R].Down())
    {
#if NUM_DEXED>1
      if (op_selected == 5)
      {
        selected_instance_id = !selected_instance_id;
        op_selected = 0;
        display_OP_active_instance_number(selected_instance_id, configuration.dexed[selected_instance_id].op_enabled);
      }
      else
#endif
        op_selected = constrain(op_selected + 1, 0, 5);
    }
    else if (LCDML.BT_checkEnter() && encoderDir[ENC_R].ButtonShort())
    {
      if (bitRead(configuration.dexed[selected_instance_id].op_enabled, op_selected))
        bitClear(configuration.dexed[selected_instance_id].op_enabled, op_selected);
      else
        bitSet(configuration.dexed[selected_instance_id].op_enabled, op_selected);

      display_OP_active_instance_number(selected_instance_id, configuration.dexed[selected_instance_id].op_enabled);
    }
    display.setCursor_textGrid(op_selected, 1);
    MicroDexed[selected_instance_id]->setOPAll(configuration.dexed[selected_instance_id].op_enabled);
    MicroDexed[selected_instance_id]->doRefreshVoice();
    send_sysex_param(configuration.dexed[selected_instance_id].midi_channel, 155, configuration.dexed[selected_instance_id].op_enabled, 0);
  }

  if (LCDML.FUNC_close())     // ****** STABLE END *********
  {
    encoderDir[ENC_R].reset();
  }
}

void UI_func_drum_reverb_send(uint8_t param)
{
  char displayname[8] = {0, 0, 0, 0, 0, 0, 0};
  if (LCDML.FUNC_setup())         // ****** SETUP *********
  {
    encoderDir[ENC_R].reset();
    temp_int = (int)(drum_config[activesample].reverb_send * 100);
    display.setCursor_textGrid(1, 1);
    display.print("Drum Rev. Send");
    display.setCursor_textGrid(2, 2);
    sprintf(displayname, "%02d", activesample);
    display.print(displayname);
    display.show(2, 5, 7, basename(drum_config[activesample].name));
  }
  if (LCDML.FUNC_loop())          // ****** LOOP *********
  {
    if (menu_select_toggle == false) {
      if ((LCDML.BT_checkDown() && encoderDir[ENC_R].Down()) || (LCDML.BT_checkUp() && encoderDir[ENC_R].Up()) || (LCDML.BT_checkEnter() && encoderDir[ENC_R].ButtonShort()))
      {
        if (LCDML.BT_checkDown())
        {
          // activesample = constrain(activesample + ENCODER[ENC_R].speed(), 0, NUM_DRUMSET_CONFIG - 2);
          smart_filter(1);
        }
        else if (LCDML.BT_checkUp())
        {
          // activesample = constrain(activesample - ENCODER[ENC_R].speed(), 0, NUM_DRUMSET_CONFIG - 2);
          smart_filter(0);
        }
      }
    } else {
      if ((LCDML.BT_checkDown() && encoderDir[ENC_R].Down()) || (LCDML.BT_checkUp() && encoderDir[ENC_R].Up()) || (LCDML.BT_checkEnter() && encoderDir[ENC_R].ButtonShort()))
      {
        if (LCDML.BT_checkDown())
        {
          temp_int = constrain(temp_int + ENCODER[ENC_R].speed(), 0, REVERB_SEND_MAX);
        }
        else if (LCDML.BT_checkUp())
        {
          temp_int = constrain(temp_int - ENCODER[ENC_R].speed(), 0, REVERB_SEND_MAX);
        }
      }
    }
    if (LCDML.BT_checkEnter())
    {
      if (menu_select_toggle) {
        menu_select_toggle = false;
      } else
      { menu_select_toggle = true;
        temp_int = (int)(drum_config[activesample].reverb_send * 100);
      }
    }
    if (menu_select_toggle == false) {
      display.setCursor_textGrid(12, 2);
      display.print(" ");
      display.setCursor_textGrid(16, 2);
      display.print(" ");
      display.setCursor_textGrid(1, 2);
      display.print("[");
      display.setCursor_textGrid(4, 2);
      display.print("]");
      display.setCursor_textGrid(2, 2);
      sprintf(displayname, "%02d", activesample);
      display.print(displayname);
      display.show(2, 5, 7, basename(drum_config[activesample].name));
      sprintf(displayname, "%03d", (int)(drum_config[activesample].reverb_send * 100) );
      display.setCursor_textGrid(13, 2);
      display.print(displayname);
    } else {
      display.setCursor_textGrid(1, 2);
      display.print(" ");
      display.setCursor_textGrid(4, 2);
      display.print(" ");
      display.setCursor_textGrid(12, 2);
      display.print("[");
      display.setCursor_textGrid(16, 2);
      display.print("]");
      sprintf(displayname, "%03d", temp_int);
      display.setCursor_textGrid(13, 2);
      display.print(displayname);
      drum_config[activesample].reverb_send = mapfloat(temp_int, 0, 100, 0.0, 1.0);
    }
  }
  if (LCDML.FUNC_close())     // ****** STABLE END *********
  {
    encoderDir[ENC_R].reset();
  }
}

void UI_func_drum_midi_channel(uint8_t param)
{
  if (LCDML.FUNC_setup())         // ****** SETUP *********
  {
    encoderDir[ENC_R].reset();
    display.setCursor_textGrid(1, 1);
    display.print(F("MIDI Channel"));
  }
  if (LCDML.FUNC_loop())          // ****** LOOP *********
  {
    if (LCDML.BT_checkDown() && encoderDir[ENC_R].Down())
      drum_midi_channel = constrain(drum_midi_channel + ENCODER[ENC_R].speed(), MIDI_CHANNEL_MIN, MIDI_CHANNEL_MAX);
    else if (LCDML.BT_checkUp() && encoderDir[ENC_R].Up())
      drum_midi_channel = constrain(drum_midi_channel - ENCODER[ENC_R].speed(), MIDI_CHANNEL_MIN, MIDI_CHANNEL_MAX);

    display.setCursor_textGrid(1, 2);
    if (drum_midi_channel == 0)
    {
      display.print(F("[OMNI]"));
    }
    else
    {
      display_int(drum_midi_channel, 4, false, true, false);
    }
  }
  if (LCDML.FUNC_close())     // ****** STABLE END *********
  {
    encoderDir[ENC_R].reset();
  }
}
void UI_func_drums_main_volume(uint8_t param)
{
  char displayname[4] = {0, 0, 0, 0};
  if (LCDML.FUNC_setup())         // ****** SETUP *********
  {
    encoderDir[ENC_R].reset();
    temp_int = mapfloat(seq.drums_volume, 0.0, VOL_MAX_FLOAT, 0, 100);
    display.setCursor(1, 1);
    display.print(" Drums M.Volume ");
  }
  if (LCDML.FUNC_loop())          // ****** LOOP *********
  {
    if ((LCDML.BT_checkDown() && encoderDir[ENC_R].Down()) || (LCDML.BT_checkUp() && encoderDir[ENC_R].Up()) || (LCDML.BT_checkEnter() && encoderDir[ENC_R].ButtonShort()))
    {
      if (LCDML.BT_checkDown())
      {
        temp_int = constrain(temp_int + ENCODER[ENC_R].speed(), 0, 100);
      }
      else if (LCDML.BT_checkUp())
      {
        temp_int = constrain(temp_int - ENCODER[ENC_R].speed(), 0, 100);
      }
    }
    display.setCursor_textGrid(5, 1);
    sprintf(displayname, "%03d", temp_int);
    display.print(displayname);
    display.setCursor_textGrid(8, 1);
    display.print("/100");
    master_mixer_r.gain (2, volume_transform(mapfloat(temp_int, 0, 100, 0.0, VOL_MAX_FLOAT)));
    master_mixer_l.gain (2, volume_transform(mapfloat(temp_int, 0, 100, 0.0, VOL_MAX_FLOAT)));
    seq.drums_volume = mapfloat(temp_int, 0, 100, 0.0, VOL_MAX_FLOAT);
  }
  if (LCDML.FUNC_close())     // ****** STABLE END *********
  {
    encoderDir[ENC_R].reset();
  }
}
void UI_func_drum_tune_offset(uint8_t param)
{
  char displayname[8] = {0, 0, 0, 0, 0, 0, 0};
  if (LCDML.FUNC_setup())         // ****** SETUP *********
  {
    encoderDir[ENC_R].reset();
    temp_int = (int)(drum_config[activesample].p_offset * 200);
    display.setCursor_textGrid(1, 1);
    display.print("DrumSmp. Tune");
    display.setCursor_textGrid(2, 2);
    sprintf(displayname, "%02d", activesample);
    display.print(displayname);
    display.show(2, 5, 8, basename(drum_config[activesample].name));
  }
  if (LCDML.FUNC_loop())          // ****** LOOP *********
  {
    if (menu_select_toggle == false) {
      if ((LCDML.BT_checkDown() && encoderDir[ENC_R].Down()) || (LCDML.BT_checkUp() && encoderDir[ENC_R].Up()) || (LCDML.BT_checkEnter() && encoderDir[ENC_R].ButtonShort()))
      {
        if (LCDML.BT_checkDown())
        {
          activesample = constrain(activesample + ENCODER[ENC_R].speed(), 0, NUM_DRUMSET_CONFIG - 2);
        }
        else if (LCDML.BT_checkUp())
        {
          activesample = constrain(activesample - ENCODER[ENC_R].speed(), 0, NUM_DRUMSET_CONFIG - 2);
        }
      }
    } else {
      if ((LCDML.BT_checkDown() && encoderDir[ENC_R].Down()) || (LCDML.BT_checkUp() && encoderDir[ENC_R].Up()) || (LCDML.BT_checkEnter() && encoderDir[ENC_R].ButtonShort()))
      {
        if (LCDML.BT_checkDown())
        {
          temp_int = constrain(temp_int + ENCODER[ENC_R].speed(), 0, 400);
        }
        else if (LCDML.BT_checkUp())
        {
          temp_int = constrain(temp_int - ENCODER[ENC_R].speed(), 0, 400);
        }
      }
    }
    if (LCDML.BT_checkEnter())
    {
      if (menu_select_toggle) {
        menu_select_toggle = false;
      } else
      { menu_select_toggle = true;
        temp_int = (int)(drum_config[activesample].p_offset * 200);
      }
    }
    if (menu_select_toggle == false) {
      display.setCursor_textGrid(14, 2);
      display.print(" ");
      display.setCursor_textGrid(18, 2);
      display.print(" ");
      display.setCursor_textGrid(1, 2);
      display.print("[");
      display.setCursor_textGrid(4, 2);
      display.print("]");
      display.setCursor_textGrid(2, 2);
      sprintf(displayname, "%02d", activesample);
      display.print(displayname);
      display.show(2, 5, 8, basename(drum_config[activesample].name));
      sprintf(displayname, "%03d", (int)(drum_config[activesample].p_offset * 200) );
      display.setCursor_textGrid(15, 2);
      display.print(displayname);
    } else {
      temp_float = mapfloat(temp_int, 0, 400, 0.0, 2.0);
      display.setCursor_textGrid(1, 2);
      display.print(" ");
      display.setCursor_textGrid(4, 2);
      display.print(" ");
      display.setCursor_textGrid(14, 2);
      display.print("[");
      display.setCursor_textGrid(18, 2);
      display.print("]");
      sprintf(displayname, "%03d", temp_int);
      display.setCursor_textGrid(15, 2);
      display.print(displayname);
      drum_config[activesample].p_offset = temp_float;
    }
  }
  if (LCDML.FUNC_close())     // ****** STABLE END *********
  {
    encoderDir[ENC_R].reset();
  }
}
void UI_func_drum_pitch(uint8_t param)
{
  char displayname[8] = {0, 0, 0, 0, 0, 0, 0};
  if (LCDML.FUNC_setup())         // ****** SETUP *********
  {
    encoderDir[ENC_R].reset();
    temp_int = (int)(drum_config[activesample].pitch * 200);
    display.setCursor_textGrid(1, 1);
    display.print("DrumSmp. Pitch");
    display.setCursor_textGrid(2, 2);
    sprintf(displayname, "%02d", activesample);
    display.print(displayname);
    display.show(2, 5, 8, basename(drum_config[activesample].name));
  }
  if (LCDML.FUNC_loop())          // ****** LOOP *********
  {
    if (menu_select_toggle == false) {
      if ((LCDML.BT_checkDown() && encoderDir[ENC_R].Down()) || (LCDML.BT_checkUp() && encoderDir[ENC_R].Up()) || (LCDML.BT_checkEnter() && encoderDir[ENC_R].ButtonShort()))
      {
        if (LCDML.BT_checkDown())
        {
          activesample = constrain(activesample + ENCODER[ENC_R].speed(), 0, NUM_DRUMSET_CONFIG - 2);
        }
        else if (LCDML.BT_checkUp())
        {
          activesample = constrain(activesample - ENCODER[ENC_R].speed(), 0, NUM_DRUMSET_CONFIG - 2);
        }
      }
    } else {
      if ((LCDML.BT_checkDown() && encoderDir[ENC_R].Down()) || (LCDML.BT_checkUp() && encoderDir[ENC_R].Up()) || (LCDML.BT_checkEnter() && encoderDir[ENC_R].ButtonShort()))
      {
        if (LCDML.BT_checkDown())
        {
          temp_int = constrain(temp_int + ENCODER[ENC_R].speed(), 0, 400);
        }
        else if (LCDML.BT_checkUp())
        {
          temp_int = constrain(temp_int - ENCODER[ENC_R].speed(), 0, 400);
        }
      }
    }
    if (LCDML.BT_checkEnter())
    {
      if (menu_select_toggle) {
        menu_select_toggle = false;
      } else
      { menu_select_toggle = true;
        temp_int = (int)(drum_config[activesample].pitch * 200);
      }
    }
    if (menu_select_toggle == false) {
      display.setCursor_textGrid(13, 2);
      display.print(" ");
      display.setCursor_textGrid(17, 2);
      display.print(" ");
      display.setCursor_textGrid(1, 2);
      display.print("[");
      display.setCursor_textGrid(4, 2);
      display.print("]");
      display.setCursor_textGrid(2, 2);
      sprintf(displayname, "%02d", activesample);
      display.print(displayname);
      display.show(2, 5, 8, basename(drum_config[activesample].name));
      sprintf(displayname, "%03d", (int)(drum_config[activesample].pitch * 200) );
      display.setCursor_textGrid(14, 2);
      display.print(displayname);
    } else {
      temp_float = mapfloat(temp_int, 0, 400, 0.0, 2.0);
      display.setCursor_textGrid(1, 2);
      display.print(" ");
      display.setCursor_textGrid(4, 2);
      display.print(" ");
      display.setCursor_textGrid(13, 2);
      display.print("[");
      display.setCursor_textGrid(17, 2);
      display.print("]");
      sprintf(displayname, "%03d", temp_int);
      display.setCursor_textGrid(14, 2);
      display.print(displayname);
      drum_config[activesample].pitch = temp_float;
    }
  }
  if (LCDML.FUNC_close())     // ****** STABLE END *********
  {
    encoderDir[ENC_R].reset();
  }
}
void UI_func_drum_volume(uint8_t param)
{
  char displayname[8] = {0, 0, 0, 0, 0, 0, 0};
  if (LCDML.FUNC_setup())         // ****** SETUP *********
  {
    encoderDir[ENC_R].reset();
    temp_int = (int)(drum_config[activesample].vol_max * 100);
    display.setCursor_textGrid(1, 1);
    display.print("DrumSmp. Volume");

    display.setCursor_textGrid(2, 2);
    sprintf(displayname, "%02d", activesample);
    display.print(displayname);
    display.show(2, 5, 7, basename(drum_config[activesample].name));

  }
  if (LCDML.FUNC_loop())          // ****** LOOP *********
  {
    if (menu_select_toggle == false) {
      if ((LCDML.BT_checkDown() && encoderDir[ENC_R].Down()) || (LCDML.BT_checkUp() && encoderDir[ENC_R].Up()) || (LCDML.BT_checkEnter() && encoderDir[ENC_R].ButtonShort()))
      {
        if (LCDML.BT_checkDown())
        {
          // activesample = constrain(activesample + ENCODER[ENC_R].speed(), 0, NUM_DRUMSET_CONFIG - 2);
          smart_filter(1);
        }
        else if (LCDML.BT_checkUp())
        {
          //  activesample = constrain(activesample - ENCODER[ENC_R].speed(), 0, NUM_DRUMSET_CONFIG - 2);
          smart_filter(0);
        }
      }
    } else {
      if ((LCDML.BT_checkDown() && encoderDir[ENC_R].Down()) || (LCDML.BT_checkUp() && encoderDir[ENC_R].Up()) || (LCDML.BT_checkEnter() && encoderDir[ENC_R].ButtonShort()))
      {
        if (LCDML.BT_checkDown())
        {
          temp_int = constrain(temp_int + ENCODER[ENC_R].speed(), 0, 100);
        }
        else if (LCDML.BT_checkUp())
        {
          temp_int = constrain(temp_int - ENCODER[ENC_R].speed(), 0, 100);
        }
      }
    }
    if (LCDML.BT_checkEnter())
    {
      if (menu_select_toggle) {
        menu_select_toggle = false;
      } else
      { menu_select_toggle = true;
        temp_int = (int)(drum_config[activesample].vol_max * 100);
      }
    }
    if (menu_select_toggle == false) {
      display.setCursor_textGrid(12, 2);
      display.print(" ");
      display.setCursor_textGrid(16, 2);
      display.print(" ");
      display.setCursor_textGrid(1, 2);
      display.print("[");
      display.setCursor_textGrid(4, 2);
      display.print("]");
      display.setCursor_textGrid(2, 2);
      sprintf(displayname, "%02d", activesample);
      display.print(displayname);
      display.show(2, 5, 7, basename(drum_config[activesample].name));
      sprintf(displayname, "%03d", (int)(drum_config[activesample].vol_max * 100) );
      display.setCursor_textGrid(13, 2);
      display.print(displayname);
    } else {
      temp_float = mapfloat(temp_int, 0, 100, 0.0, 1.0);
      display.setCursor_textGrid(1, 2);
      display.print(" ");
      display.setCursor_textGrid(4, 2);
      display.print(" ");
      display.setCursor_textGrid(12, 2);
      display.print("[");
      display.setCursor_textGrid(16, 2);
      display.print("]");
      sprintf(displayname, "%03d", temp_int);
      display.setCursor_textGrid(13, 2);
      display.print(displayname);
      drum_config[activesample].vol_max = temp_float;
    }
  }
  if (LCDML.FUNC_close())     // ****** STABLE END *********
  {
    encoderDir[ENC_R].reset();
  }
}

void UI_func_arp_shift(uint8_t param)
{
  char displayname[4] = {0, 0, 0, 0};
  if (LCDML.FUNC_setup())         // ****** SETUP *********
  {
    encoderDir[ENC_R].reset();
    display.setCursor_textGrid(1, 1);
    display.print("Arp/Chord Transp");
    display.setCursor_textGrid(1, 2);
    display.print("Oct");
    display.setCursor_textGrid(4, 2);
    sprintf(displayname, "%02d", seq.oct_shift);
    display.print(displayname);
    display.setCursor_textGrid(8, 2);
    display.print("Shift");
    display.setCursor_textGrid(14, 2);
    display.print(seq.element_shift);
  }
  if (LCDML.FUNC_loop())          // ****** LOOP *********
  {
    if (menu_select_toggle == false) {
      if ((LCDML.BT_checkDown() && encoderDir[ENC_R].Down()) || (LCDML.BT_checkUp() && encoderDir[ENC_R].Up()) || (LCDML.BT_checkEnter() && encoderDir[ENC_R].ButtonShort()))
      {
        if (LCDML.BT_checkDown())
        {
          seq.oct_shift = constrain(seq.oct_shift + ENCODER[ENC_R].speed(), -2, 2);
        }
        else if (LCDML.BT_checkUp())
        {
          seq.oct_shift = constrain(seq.oct_shift - ENCODER[ENC_R].speed(), -2, 2);
        }
      }
    } else {
      if ((LCDML.BT_checkDown() && encoderDir[ENC_R].Down()) || (LCDML.BT_checkUp() && encoderDir[ENC_R].Up()) || (LCDML.BT_checkEnter() && encoderDir[ENC_R].ButtonShort()))
      {
        if (LCDML.BT_checkDown())
        {
          seq.element_shift = constrain(seq.element_shift + ENCODER[ENC_R].speed(), 0, 6);
        }
        else if (LCDML.BT_checkUp())
        {
          seq.element_shift = constrain(seq.element_shift - ENCODER[ENC_R].speed(), 0, 6);
        }
      }
    }
    if (LCDML.BT_checkEnter())
    {
      menu_select_toggle = !menu_select_toggle;
    }
    if (menu_select_toggle == false)
    { display.setCursor_textGrid(13, 2);
      display.print(" ");
      display.setCursor_textGrid(15, 2);
      display.print(" ");
      display.setCursor_textGrid(3, 2);
      display.print("[");
      display.setCursor_textGrid(6, 2);
      display.print("]");
      display.setCursor_textGrid(4, 2);
      sprintf(displayname, "%02d", seq.oct_shift);
      display.print(displayname);
    } else {
      display.setCursor_textGrid(3, 2);
      display.print(" ");
      display.setCursor_textGrid(6, 2);
      display.print(" ");
      display.setCursor_textGrid(13, 2);
      display.print("[");
      display.setCursor_textGrid(15, 2);
      display.print("]");
      display.setCursor_textGrid(14, 2);
      display.print(seq.element_shift);
    }
  }
  if (LCDML.FUNC_close())     // ****** STABLE END *********
  {
    encoderDir[ENC_R].reset();
  }
}

void UI_func_drum_pan(uint8_t param)
{
  char displayname[8] = {0, 0, 0, 0, 0, 0, 0};
  if (LCDML.FUNC_setup())         // ****** SETUP *********
  {
    encoderDir[ENC_R].reset();
    temp_int = mapfloat(drum_config[activesample].pan, -1.0, 1.0, -99, 99);
    display.setCursor_textGrid(1, 1);
    display.print("DrmSmp. Panorama");
    display.setCursor_textGrid(2, 2);
    sprintf(displayname, "%02d", activesample);
    display.print(displayname);
    display.show(2, 5, 6, basename(drum_config[activesample].name));
  }
  if (LCDML.FUNC_loop())          // ****** LOOP *********
  {
    if (menu_select_toggle == false) {
      if ((LCDML.BT_checkDown() && encoderDir[ENC_R].Down()) || (LCDML.BT_checkUp() && encoderDir[ENC_R].Up()) || (LCDML.BT_checkEnter() && encoderDir[ENC_R].ButtonShort()))
      {
        if (LCDML.BT_checkDown())
        {
          //activesample = constrain(activesample + ENCODER[ENC_R].speed(), 0, NUM_DRUMSET_CONFIG - 2);
          smart_filter(1);
        }
        else if (LCDML.BT_checkUp())
        {
          //activesample = constrain(activesample - ENCODER[ENC_R].speed(), 0, NUM_DRUMSET_CONFIG - 2);
          smart_filter(0);
        }
      }
    } else {
      if ((LCDML.BT_checkDown() && encoderDir[ENC_R].Down()) || (LCDML.BT_checkUp() && encoderDir[ENC_R].Up()) || (LCDML.BT_checkEnter() && encoderDir[ENC_R].ButtonShort()))
      {
        if (LCDML.BT_checkDown())
        {
          temp_int = constrain(temp_int + ENCODER[ENC_R].speed(), -99, 99);
        }
        else if (LCDML.BT_checkUp())
        {
          temp_int = constrain(temp_int - ENCODER[ENC_R].speed(), -99, 99);
        }
      }
    }
    if (LCDML.BT_checkEnter())
    {
      if (menu_select_toggle) {
        menu_select_toggle = false;
      } else
      { menu_select_toggle = true;
      }
    }
    if (menu_select_toggle == false) {
      display.setCursor_textGrid(12, 2);
      display.print(" ");
      display.setCursor_textGrid(16, 2);
      display.print(" ");
      display.setCursor_textGrid(1, 2);
      display.print("[");
      display.setCursor_textGrid(4, 2);
      display.print("]");
      display.setCursor_textGrid(2, 2);
      sprintf(displayname, "%02d", activesample);
      display.print(displayname);
      display.show(2, 5, 6, basename(drum_config[activesample].name));
    } else {
      display.setCursor_textGrid(1, 2);
      display.print(" ");
      display.setCursor_textGrid(4, 2);
      display.print(" ");
      display.setCursor_textGrid(12, 2);
      display.print("[");
      display.setCursor_textGrid(16, 2);
      display.print("]");
      drum_config[activesample].pan = mapfloat(temp_int, -99, 99, -1.0, 1.0);
    }
    temp_int = mapfloat(drum_config[activesample].pan, -1.0, 1.0, -99, 99);
    display.setCursor_textGrid(13, 2);
    if (temp_int > 1) {
      display.print("R");
    } else if (temp_int < 0) {
      display.print("L");
    }
    else {
      display.print("C");
    }
    sprintf(displayname, "%02d", abs(temp_int));
    display.setCursor_textGrid(14, 2);
    display.print( displayname);
  }
  if (LCDML.FUNC_close())     // ****** STABLE END *********
  {
    encoderDir[ENC_R].reset();
  }
}

void set_pattern_content_type_color_with_green_highlight(uint8_t pattern)
{
  if (pattern == seq.active_pattern )
  {
    if (seq.content_type[pattern] == 0) //Drumpattern
      display.setTextColor(DX_ORANGE, DARKGREEN);
    else if (seq.content_type[pattern] == 1) //Instrument Pattern
      display.setTextColor(LIGHTBLUE, DARKGREEN);
    else if (seq.content_type[pattern] == 2 || seq.content_type[pattern] == 3) //  chord or arp pattern
      display.setTextColor(DX_MAGENTA, DARKGREEN);
  }
  else
  {
    if (seq.content_type[pattern] == 0) //Drumpattern
      display.setTextColor(DX_ORANGE, BLACK);
    else if (seq.content_type[pattern] == 1) //Instrument Pattern
      display.setTextColor(LIGHTBLUE, BLACK);
    else if (seq.content_type[pattern] == 2 || seq.content_type[pattern] == 3) //  chord or arp pattern
      display.setTextColor(DX_MAGENTA, BLACK);
  }
}

void set_pattern_content_type_color_inverted(uint8_t pattern)
{
  if (seq.content_type[pattern] == 0) //Drumpattern
    display.setTextColor(  BLACK, DX_ORANGE);
  else if (seq.content_type[pattern] == 1) //Instrument Pattern
    display.setTextColor( BLACK, LIGHTBLUE);
  else if (seq.content_type[pattern] == 2 || seq.content_type[pattern] == 3) //  chord or arp pattern
    display.setTextColor( BLACK, DX_MAGENTA);
}

void set_track_type_color(uint8_t track)
{
  if ( seq.track_type[track] == 0) //Drums
    display.setTextColor(DX_ORANGE, BLACK);
  else if ( seq.track_type[track] == 1) //Inst
    display.setTextColor(LIGHTBLUE, BLACK);
  else if ( seq.track_type[track] == 2 || seq.track_type[track] == 3 ) //Chord or Arp
    display.setTextColor(DX_MAGENTA, BLACK);
}

void set_track_type_color_inverted(uint8_t track)
{
  if ( seq.track_type[track] == 0) //Drums
    display.setTextColor(BLACK, DX_ORANGE);
  else if ( seq.track_type[track] == 1) //Inst
    display.setTextColor(BLACK, LIGHTBLUE);
  else if ( seq.track_type[track] == 2 || seq.track_type[track] == 3) //Chord or Arp
    display.setTextColor(BLACK, DX_MAGENTA);
}

void print_color_map(int x, int y)
{
  display.setTextSize(1);
  display.setCursor(x, y +  0 * (CHAR_height - 5));
  display.setTextColor(DX_PURPLE, BLACK);
  display.print( "PERFORMANCE# ");
  display.setTextColor(DX_CYAN, BLACK);
  seq_print_formatted_number(configuration.sys.performance_number, 2);
  display.print( " ");
  display.setTextColor(WHITE, GREY3);
  display.print(seq.name);
  display.setCursor(x, y +  2 * (CHAR_height - 5));
  display.setTextColor(GREY1, BLACK);
  display.print("COLOR CODES ");
  display.setTextColor(DX_ORANGE, BLACK);
  display.print("DRUM / DRUM TRACK");
  display.setCursor(x, y +  3 * (CHAR_height - 5));
  display.setTextColor(DX_CYAN, BLACK);
  display.print("PITCHED SAMPLE ");
  display.setTextColor(DX_MAGENTA, BLACK);
  display.print("CHORD/ARP ");
  display.setTextColor(LIGHTBLUE, BLACK);
  display.print("INST");
  //display.setTextColor(DX_PURPLE, BLACK);
  //display.print("OTHER");
  display.setTextSize(2);
}

void seq_printAllSeqSteps()
{
  display.setCursor_textGrid(1, 2);
  for (uint8_t i = 0; i < 16; i++)
  {
    if ( seq.vel[seq.active_pattern][i] > 209 )
      display.setTextColor(DX_CYAN, BLACK);
    else
      set_pattern_content_type_color(seq.active_pattern);
    display.print(seq_find_shortname(i)[0]);
  }
}

void seq_sub_pat_chain(int x, int y, bool init)
{
  display.setTextSize(1);
  if (init) {
    display.setTextColor(GREY2, BLACK);
    display.setCursor(x, y);
    display.print("PATTERN CHAIN");
    display.setCursor(x + 18 * 6, y);
    display.print("LEN: ");
    display.print(seq.chain_lenght + 1);
    display.print("P/");
    display.print( (seq.chain_lenght + 1) * 16);
    display.print(" STEPS");
    display.setCursor(x, y + (CHAR_height - 5) );
    display.print("CURRENT CHAIN STEP  /");
    display.print(seq.chain_lenght + 1);
  }
  if (seq.menu == 20)
  {
    display.setCursor(x, y);
    display.setTextColor(GREY2, BLACK);
    display.setCursor(x + 18 * 6, y);
    display.setTextColor(WHITE, BLUE);
    display.print("LEN: ");
    display.print(seq.chain_lenght + 1);
    display.print("P/");
    display.print( (seq.chain_lenght + 1) * 16);
    display.print(" STEPS");
  }
  if ( seq.menu == 19 || seq.menu == 21 )
  {
    display.setCursor(x, y);
    display.setTextColor(GREY2, BLACK);
    display.setCursor(x + 18 * 6, y);
    display.print("LEN: ");
    display.print(seq.chain_lenght + 1);
    display.print("P/");
    display.print( (seq.chain_lenght + 1) * 16);
    display.print(" STEPS");
  }
  display.setCursor(x + 9 * CHAR_width + 6, y + (CHAR_height - 5) );
  display.setTextColor(WHITE, BLACK);
  display.print(seq.chain_active_step + 1);
  display.setTextColor(GREY1, BLACK);
  for (uint8_t track = 0; track < NUM_SEQ_TRACKS; track++)
  {
    display.setCursor(x, y + 3 * (CHAR_height - 5) + track * (CHAR_height - 5));
    if (seq.menu - 21 == track)
      set_track_type_color_inverted(track);
    else
      set_track_type_color(track);
    display.print("TRACK ");
    display.print (track + 1);
    display.print(":");
    display.setCursor(x + 10 * 6  , y + 3 * (CHAR_height - 5) + track * (CHAR_height - 5));
    if (seq.menu - 27 == track)
      set_pattern_content_type_color_inverted(seq.patternchain[0][track]);
    else
      set_pattern_content_type_color_with_green_highlight(seq.patternchain[0][track]);
    display.print(" ");
    seq_print_formatted_number( seq.patternchain[0][track], 2) ;
    display.print(" ");
    if (init) {
      display.setCursor(x + 15 * 6  , y + 3 * (CHAR_height - 5) + track * (CHAR_height - 5));
      display.setTextColor(GREY1, BLACK);
      display.print("-");
    }
    display.setCursor(x + 17 * 6  , y + 3 * (CHAR_height - 5) + track * (CHAR_height - 5));
    if (seq.menu - 27 - 6 == track)
      set_pattern_content_type_color_inverted(seq.patternchain[1][track]);
    else
      set_pattern_content_type_color_with_green_highlight(seq.patternchain[1][track]);
    display.print(" ");
    seq_print_formatted_number( seq.patternchain[1][track], 2) ;
    display.print(" ");
    if (init) {
      display.setCursor(x + 22 * 6  , y + 3 * (CHAR_height - 5) + track * (CHAR_height - 5));
      display.setTextColor(GREY1, BLACK);
      display.print("-");
    }
    display.setCursor(x + 24 * 6  , y + 3 * (CHAR_height - 5) + track * (CHAR_height - 5));
    if (seq.menu - 27 - 12 == track)
      set_pattern_content_type_color_inverted(seq.patternchain[2][track]);
    else
      set_pattern_content_type_color_with_green_highlight(seq.patternchain[2][track]);
    display.print(" ");
    seq_print_formatted_number( seq.patternchain[2][track], 2) ;
    display.print(" ");
    display.setCursor(x + 29 * 6  , y + 3 * (CHAR_height - 5) + track * (CHAR_height - 5));
    if (init) {
      display.setTextColor(GREY1, BLACK);
      display.print("-");
    }
    display.setCursor(x + 31 * 6  , y + 3 * (CHAR_height - 5) + track * (CHAR_height - 5));
    if (seq.menu - 27 - 18 == track)
      set_pattern_content_type_color_inverted(seq.patternchain[3][track]);
    else
      set_pattern_content_type_color_with_green_highlight(seq.patternchain[3][track]);
    display.print(" ");
    seq_print_formatted_number( seq.patternchain[3][track], 2) ;
    display.print(" ");
  }
  if (init)
    print_color_map(CHAR_width * 21, 266);
  display.setTextSize(2);
}

void seq_printVelGraphBar()
{
  display.fillRect(CHAR_width, 3 * CHAR_height , 17 * CHAR_width, 17, BLACK);
  for (uint8_t i = 0; i < 16; i++)
  {
    if ( seq.vel[seq.active_pattern][i] < 130) {
      display.drawLine (CHAR_width + i * CHAR_width + 3 , CHAR_height * 4 - 3  , CHAR_width + i * CHAR_width + 3 , CHAR_height * 4 - 3 - (seq.vel[seq.active_pattern][i] / 10)   , GREY2);
      display.drawLine (CHAR_width + i * CHAR_width + 4 , CHAR_height * 4 - 3  , CHAR_width + i * CHAR_width + 4 , CHAR_height * 4 - 3 - (seq.vel[seq.active_pattern][i] / 10)   , GREY2);
      display.drawLine (CHAR_width + i * CHAR_width + 5 , CHAR_height * 4 - 3  , CHAR_width + i * CHAR_width + 5 , CHAR_height * 4 - 3 - (seq.vel[seq.active_pattern][i] / 10)   , GREY2);
    }
    else
    {
      if ( seq.vel[seq.active_pattern][i] > 209)
      {
        display.drawBitmap(i * CHAR_width + 13, 3 * CHAR_height + 7, special_chars[22], 8, 8, DX_CYAN);
        //display.print("P");
      }
    }
  }
}

void seq_printVelGraphBar_single_step(uint8_t step, int color)
{
  display.fillRect(CHAR_width + step * CHAR_width + 3 , CHAR_height * 3 + 1  , 3, 14, BLACK);

  if ( seq.vel[seq.active_pattern][step] < 130)
  {
    display.drawLine (CHAR_width + step * CHAR_width + 3 , CHAR_height * 4 - 3  , CHAR_width + step * CHAR_width + 3 , CHAR_height * 4 - 3 - (seq.vel[seq.active_pattern][step] / 10)   , color);
    display.drawLine (CHAR_width + step * CHAR_width + 4 , CHAR_height * 4 - 3  , CHAR_width + step * CHAR_width + 4 , CHAR_height * 4 - 3 - (seq.vel[seq.active_pattern][step] / 10)   , color);
    display.drawLine (CHAR_width + step * CHAR_width + 5 , CHAR_height * 4 - 3  , CHAR_width + step * CHAR_width + 5 , CHAR_height * 4 - 3 - (seq.vel[seq.active_pattern][step] / 10)   , color);
  }
  else
  {
    if ( seq.vel[seq.active_pattern][step] > 209)
    {
      //display.print("P");
      display.drawBitmap(step * CHAR_width + 13, 3 * CHAR_height + 7, special_chars[22], 8, 8, DX_CYAN);
    }
  }
}

void UI_func_seq_display_style(uint8_t param)
{
  if (LCDML.FUNC_setup())         // ****** SETUP *********
  {
    encoderDir[ENC_R].reset();
    display.setCursor_textGrid(1, 1);
    display.print("Seq. Disp. Style");
    display.setCursor_textGrid(1, 2);
    display.print("Pat.");
    display.setCursor_textGrid(9, 1);
    display.print("=");
  }
  if (LCDML.FUNC_loop())          // ****** LOOP *********
  {
    if (menu_select_toggle == false) {
      if ((LCDML.BT_checkDown() && encoderDir[ENC_R].Down()) || (LCDML.BT_checkUp() && encoderDir[ENC_R].Up()) || (LCDML.BT_checkEnter() && encoderDir[ENC_R].ButtonShort()))
      {
        if (LCDML.BT_checkDown())
        {
          seq.active_pattern = constrain(seq.active_pattern + ENCODER[ENC_R].speed(), 0, NUM_SEQ_PATTERN - 1);
        }
        else if (LCDML.BT_checkUp())
        {
          seq.active_pattern = constrain(seq.active_pattern - ENCODER[ENC_R].speed(), 0, NUM_SEQ_PATTERN - 1);
        }
      }
    } else {
      if ((LCDML.BT_checkDown() && encoderDir[ENC_R].Down()) || (LCDML.BT_checkUp() && encoderDir[ENC_R].Up()) || (LCDML.BT_checkEnter() && encoderDir[ENC_R].ButtonShort()))
      {
        if (LCDML.BT_checkDown())
        {
          seq.content_type[seq.active_pattern] = constrain(seq.content_type[seq.active_pattern] + ENCODER[ENC_R].speed(), 0, 2);
        }
        else if (LCDML.BT_checkUp())
        {
          seq.content_type[seq.active_pattern] = constrain(seq.content_type[seq.active_pattern] - ENCODER[ENC_R].speed(), 0, 2);
        }
      }
    }
    if (LCDML.BT_checkEnter())
    {
      menu_select_toggle = !menu_select_toggle;
    }
    if (menu_select_toggle == false) {
      display.setCursor_textGrid(10, 1);
      display.print(" ");
      display.setCursor_textGrid(15, 2);
      display.print(" ");
      display.setCursor_textGrid(4, 2);
      display.print("[");
      if (seq.active_pattern < 10)
        display.print ("0");
      display.print(seq.active_pattern);
      display.print("]");
    } else {
      display.setCursor_textGrid(4, 2);
      display.print(" ");
      display.setCursor_textGrid(7, 1);
      display.print(" ");
      display.setCursor_textGrid(10, 1);
      display.print("[");
      display.setCursor_textGrid(15, 2);
      display.print("]");
    }
    display.setCursor_textGrid(11, 2);
    if (seq.content_type[seq.active_pattern] == 0)
      display.print("Drum");
    else  if (seq.content_type[seq.active_pattern] == 1)
      display.print("Inst");
    else
      display.print("Chrd");
  }
  if (LCDML.FUNC_close())     // ****** STABLE END *********
  {
    encoderDir[ENC_R].reset();
  }
}

void UI_func_seq_live_transpose_oct(uint8_t param)
{ //Select octave for live transpose of sequencer instrument track. Should be one of the lowest octaves available on the keyboard.
  //Allowed range: C1-C5 to not restrict too much, even for very unusual user setups/configurations.

  char note_name[3];
  if (LCDML.FUNC_setup())         // ****** SETUP *********
  {

    encoderDir[ENC_R].reset();
    display.setCursor_textGrid(1, 1);
    display.print("Live Transp.Oct.");
    display.setCursor_textGrid(1, 2);
    display.print("[");
    if (seq.transpose != 0)
    {
      getNoteName(note_name, seq.transpose);
      display.print(note_name);
    }
    else
      display.print(" - ");
    display.print("] - [");
    if (seq.transpose != 0)
    {
      getNoteName(note_name, seq.transpose);
      display.print(note_name);
    }
    else
      display.print(" - ");
    display.print("] ");
  }
  if (LCDML.FUNC_loop())          // ****** LOOP *********
  {

    if ((LCDML.BT_checkDown() && encoderDir[ENC_R].Down()) || (LCDML.BT_checkUp() && encoderDir[ENC_R].Up()) || (LCDML.BT_checkEnter() && encoderDir[ENC_R].ButtonShort()))
    {
      if (LCDML.BT_checkDown())
        seq.transpose = constrain(seq.transpose + ENCODER[ENC_R].speed(), 24, 60);
      else if (LCDML.BT_checkUp())
        seq.transpose = constrain(seq.transpose - ENCODER[ENC_R].speed(), 24, 60);
    }
    display.setCursor_textGrid(1, 2);
    display.print("[");
    if (seq.transpose != 0)
    {
      getNoteName(note_name, seq.transpose);
      display.print(note_name);
    }
    else
      display.print(" - ");
    display.print("] - [");
    if (seq.transpose != 0)
    {
      getNoteName(note_name, seq.transpose);
      display.print(note_name);
    }
    else
      display.print(" - ");
    display.print("] ");
  }
  if (LCDML.FUNC_close())     // ****** STABLE END *********
  {
    encoderDir[ENC_R].reset();
  }
}

void UI_draw_waveform(uint8_t samplenumber)
{
  int xspace = 241;
  int samplevalue = 0;
  int  old_y = 48;
  if (seq.last_drawn_sample != samplenumber && samplenumber < NUM_DRUMSET_CONFIG - 1)
  {
    int scale = drum_config[samplenumber].len / (236 / 2);

    border2_clear();

    for (int i = 0; i < 236; i++)
    {
      samplevalue = (( (drum_config[samplenumber].drum_data[i * scale + 1]) * 256) + drum_config[samplenumber].drum_data[i * scale] )  / 800;

      if (drum_config[samplenumber].drum_data[i * scale + 1] < 128)
      {
        display.drawLine(xspace, old_y,  xspace, samplevalue + 50  , GREY2 );
        old_y = samplevalue + 50;
      }
      else
      {
        display.drawLine(xspace, old_y,  xspace, samplevalue - 32 , GREY2 );
        old_y = samplevalue - 32 ;
      }
      xspace = xspace + 1;
    }

    if (seq.content_type[seq.active_pattern] == 0) //Drum Track
    {
      display.setTextColor(GREEN, GREY4);
      display.show_smallfont_noGrid(5, 419 , 9, drum_config[samplenumber].name);
      display.setTextColor(WHITE, BLACK);
    }
    seq.last_drawn_sample = samplenumber;
  }
  display.setTextSize(2);
}

void UI_draw_waveform_large()
{
  int xspace = 0;
  int i = 4;
  short samplevalue = 0;
  int oldx = 0, oldy = 180;

  display.setCursor_textGrid(31, 0);
  display.setTextColor(WHITE, DX_CYAN );
  display.print("SCALE");
  display.setCursor_textGrid(37, 0);
  display.setTextColor(WHITE, BLACK );
  display.print(wave_spacing);
  display.print (" ");
  do {
    {
      samplevalue = (( (DRUM_SQBass[i * wave_spacing_old + 1]) * 256) + DRUM_SQBass[i * wave_spacing_old] )  / 710 + 120;
      if (DRUM_SQBass[i * wave_spacing_old + 1] < 128)
      {
        display.drawLine(oldx, oldy,  xspace, samplevalue + 60  , BLACK );
        oldx = xspace; oldy = samplevalue + 60;
      }
      else
      {
        display.drawLine(oldx, oldy,  xspace, samplevalue  - 40 , BLACK );
        oldx = xspace;  oldy = samplevalue - 40;
      }
      xspace = xspace + 1; i++;
    }
  } while (oldx < TFT_HEIGHT);
  xspace = 0;
  i = 4;
  samplevalue = 0;
  oldx = 0;  oldy = 180;
  do {
    {
      samplevalue = (( (DRUM_SQBass[i * wave_spacing + 1]) * 256) + DRUM_SQBass[i * wave_spacing] )  / 710 + 120;

      if (DRUM_SQBass[i * wave_spacing + 1] < 128)
      {
        display.drawLine(oldx, oldy,  xspace, samplevalue + 60  , WHITE );
        oldx = xspace; oldy = samplevalue + 60;
      }
      else
      {
        display.drawLine(oldx, oldy,  xspace, samplevalue  - 40 , WHITE );
        oldx = xspace;  oldy = samplevalue - 40;
      }
      xspace = xspace + 1;  i++;
    }
  } while (oldx < TFT_HEIGHT);
  wave_spacing_old = wave_spacing;
}


void UI_print_voice_info()
{

  display.setTextColor(WHITE, BLACK);

  //UI_draw_waveform(0);

  display.setTextSize(1);
  display.setCursor(415, 6);
  display.setTextColor(BLACK, DX_CYAN);
  display.print("01");
  display.setTextColor(BLACK, BLACK);
  display.print(" ");
  display.setTextColor(WHITE, GREY1);
  display.print("SQBASS");

  display_active_instance_number(selected_instance_id);

  char bank_name[BANK_NAME_LEN];
  char voice_name[VOICE_NAME_LEN];

  if (!get_bank_name(configuration.dexed[selected_instance_id].bank, bank_name, sizeof(bank_name)))
    strcpy(bank_name, "*ERROR*");
  if (!get_voice_by_bank_name(configuration.dexed[selected_instance_id].bank, bank_name, configuration.dexed[selected_instance_id].voice, voice_name, sizeof(voice_name)))
    strcpy(voice_name, "*ERROR*");
  UI_update_instance_icons();

  if (strlen(g_bank_name[selected_instance_id]) > 0)
  {
    strcpy(bank_name, g_bank_name[selected_instance_id]);
  }
  else
  {
    if (!get_bank_name(configuration.dexed[selected_instance_id].bank, bank_name, sizeof(bank_name)))
      strcpy(bank_name, "*ERROR*");
  }

  if (strlen(g_voice_name[selected_instance_id]) > 0)
  {
    strcpy(voice_name, g_voice_name[selected_instance_id]);
  }
  else
  {
    if (!get_voice_by_bank_name(configuration.dexed[selected_instance_id].bank, bank_name, configuration.dexed[selected_instance_id].voice, voice_name, sizeof(voice_name)))
      strcpy(voice_name, "*ERROR*");
  }

  display.setTextColor(DX_CYAN, BLACK);
  display.setCursor_textGrid(1, 2);
  seq_print_formatted_number(configuration.dexed[selected_instance_id].bank, 2);
  display.setCursor_textGrid(1, 2);
  seq_print_formatted_number(configuration.dexed[selected_instance_id].voice + 1, 2);
  display.setTextColor(WHITE, BLACK);
  string_toupper(bank_name);
  display.show(2, 5, 8, bank_name);

  string_toupper(voice_name);
  display.show(2, 5, 10, voice_name);

  display.setTextColor(WHITE, BLACK);
  draw_favorite_icon(configuration.dexed[selected_instance_id].bank, configuration.dexed[selected_instance_id].voice, selected_instance_id);
  display.setTextSize(2);
}

void UI_func_seq_chord_keys_ammount(uint8_t param)
{
  char displayname[4] = {0, 0, 0, 0};
  if (LCDML.FUNC_setup())         // ****** SETUP *********
  {
    encoderDir[ENC_R].reset();
    display.setCursor_textGrid(1, 1);
    display.print("ChordTrack Keys:");
    display.setCursor_textGrid(9, 2);
    display.print("Keys");
  }
  if (LCDML.FUNC_loop())          // ****** LOOP *********
  {
    if ((LCDML.BT_checkDown() && encoderDir[ENC_R].Down()) || (LCDML.BT_checkUp() && encoderDir[ENC_R].Up()) || (LCDML.BT_checkEnter() && encoderDir[ENC_R].ButtonShort()))
    {
      if (LCDML.BT_checkDown())
        seq.chord_key_ammount = constrain(seq.chord_key_ammount + ENCODER[ENC_R].speed(), 1, 7);
      else if (LCDML.BT_checkUp())
        seq.chord_key_ammount = constrain(seq.chord_key_ammount - ENCODER[ENC_R].speed(), 1, 7);
    }
    display.setCursor_textGrid(4, 2);
    display.print("[");
    sprintf(displayname, "%02d", seq.chord_key_ammount);
    display.print(displayname);
    display.print("]");
  }
  if (LCDML.FUNC_close())     // ****** STABLE END *********
  {
    encoderDir[ENC_R].reset();
  }
}

void UI_func_seq_lenght(uint8_t param)
{
  if (LCDML.FUNC_setup())         // ****** SETUP *********
  {
    encoderDir[ENC_R].reset();
    display.setCursor_textGrid(1, 1);
    display.print("Seq. lenght     ");
    display.setCursor_textGrid(1, 2);
    display.print("[  ]");
    display.setCursor_textGrid(6, 2);
    display.print("Steps");
  }
  if (LCDML.FUNC_loop())          // ****** LOOP *********
  {
    if ((LCDML.BT_checkDown() && encoderDir[ENC_R].Down()) || (LCDML.BT_checkUp() && encoderDir[ENC_R].Up()) || (LCDML.BT_checkEnter() && encoderDir[ENC_R].ButtonShort()))
    {
      if (LCDML.BT_checkDown())
        seq.chain_lenght = constrain(seq.chain_lenght + ENCODER[ENC_R].speed(), 0, 3);
      else if (LCDML.BT_checkUp())
        seq.chain_lenght = constrain(seq.chain_lenght - ENCODER[ENC_R].speed(), 0, 3);
    }
    display.setCursor_textGrid(2, 2);
    display.print((seq.chain_lenght + 1) * 16 );

  }
  if (LCDML.FUNC_close())     // ****** STABLE END *********
  {
    encoderDir[ENC_R].reset();
  }
}

void UI_func_seq_tempo(uint8_t param)
{
  char tmp[7];
  if (LCDML.FUNC_setup())         // ****** SETUP *********
  {
    encoderDir[ENC_R].reset();
    display.setCursor_textGrid(1, 1);
    display.print("  Seq. Tempo   ");
    display.setCursor_textGrid(6, 2);
    display.print("BPM");
    display.setCursor_textGrid(14, 2);
    display.print("ms");
  }
  if (LCDML.FUNC_loop())          // ****** LOOP *********
  {
    if ((LCDML.BT_checkDown() && encoderDir[ENC_R].Down()) || (LCDML.BT_checkUp() && encoderDir[ENC_R].Up()) || (LCDML.BT_checkEnter() && encoderDir[ENC_R].ButtonShort()))
    {
      if (LCDML.BT_checkDown())
        seq.bpm = constrain(seq.bpm + ENCODER[ENC_R].speed(), 50, 190);
      else if (LCDML.BT_checkUp())
        seq.bpm = constrain(seq.bpm - ENCODER[ENC_R].speed(), 50, 190);
    }
    seq.tempo_ms = 60000000 / seq.bpm / 4;
    display.setCursor_textGrid(1, 2);
    sprintf(tmp, "[%3d]", seq.bpm);
    display.print(tmp);
    display.setCursor_textGrid(11, 2);
    sprintf(tmp, "%3d", seq.tempo_ms / 1000);
    display.print(tmp);
#ifdef USE_FX
    for (uint8_t i = 0; i < MAX_DEXED; i++)
    {
      if (configuration.fx.delay_sync[i] > 0)
      {
        uint16_t midi_sync_delay_time = uint16_t(60000.0 *  midi_ticks_factor[configuration.fx.delay_sync[i]] / seq.bpm);
        delay_fx[i]->delay(0, constrain(midi_sync_delay_time, DELAY_TIME_MIN, DELAY_TIME_MAX * 10));
      }
    }
#endif
    //timer1.stop();
    timer1.begin(sequencer, seq.tempo_ms / 2);
  }
  if (LCDML.FUNC_close())     // ****** STABLE END *********
  {
    encoderDir[ENC_R].reset();
  }
}

void UI_func_sample_editor(uint8_t param)
{
  char tmp[5];
  if (LCDML.FUNC_setup())         // ****** SETUP *********
  {
    // setup function
    encoderDir[ENC_R].reset();
    display.fillScreen(BLACK);
    display.setCursor_textGrid(1, 1);
    display.setTextColor(WHITE, DX_CYAN);
    display.print("SAMPLE:");
    display.setCursor_textGrid(9, 1);
    display.setTextColor(WHITE, BLACK);
    display.print("SQRBASS1");


    display.setCursor_textGrid(1, 2);
    display.print("V:");
    sprintf(tmp, "%03d", seq.vel[seq.active_pattern][seq.menu - 1]);
    display.setCursor_textGrid(3, 2);
    display.print(tmp);
    display.setCursor_textGrid(21, 0);
    display.print("[");
    display.setCursor_textGrid(22, 0);
    if (seq.active_pattern < 10)
      display.print("0");
    display.print(seq.active_pattern);
    display.setCursor_textGrid(24, 0);
    display.print("]");

  }
  if (LCDML.FUNC_loop())          // ****** LOOP *********
  {

    if ((LCDML.BT_checkDown() && encoderDir[ENC_R].Down()) || (LCDML.BT_checkUp() && encoderDir[ENC_R].Up()))
    {
      if (LCDML.BT_checkDown())
        wave_spacing = constrain(wave_spacing + 2, 2, 500);
      else if (LCDML.BT_checkUp())
        wave_spacing = constrain(wave_spacing - 2, 2, 500);
    }

    if (LCDML.BT_checkEnter())  //handle button presses during menu >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
    {
      if ( seq.menu == 0 && seq_active_function == 99)
      {
        seq_active_function = 0;
      } else if ( seq.menu == 0 && seq_active_function == 0)
      {
        seq_active_function = 99;
      }
      else if (seq.menu > 0 && seq_active_function == 99)
      {
        seq_active_function = 1;
      }
      else
        seq_active_function = 99;
    }
    //button check end <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<

    //wave_spacing=260;

    UI_draw_waveform_large();
  }
  if (LCDML.FUNC_close())     // ****** STABLE END *********
  {
    encoderDir[ENC_R].reset();
    display.fillScreen(BLACK);
  }
}

void print_content_type ()
{
  set_pattern_content_type_color(seq.active_pattern);
  if (seq.content_type[seq.active_pattern] == 0)
    display.print("Drum     ");
  else if (seq.content_type[seq.active_pattern] == 1)
    display.print("Instr    ");
  else if (seq.content_type[seq.active_pattern] == 2  )
    display.print("Chord/Arp");

  display.setTextColor(GREY2, BLACK);
}

void print_edit_mode()
{
  display.setTextSize(1);
  display.setCursor(136, 71);

  if (LCDML.FUNC_getID() == LCDML.OTHER_getIDFromFunction(UI_func_seq_vel_editor))
  {

    if (seq.menu < 19)
    {
      display.setTextColor(DX_MAGENTA, BLUE);
      display.print("VEL./CHORDS");
    }
    else if (seq.menu == 19)
    {
      display.setTextColor(GREY1, BLACK);
      display.print("(PATTERN ");
      display.setTextColor(GREY2, BLACK);
      display.print(seq.active_pattern);
      display.setTextColor(GREY1, BLACK);
      display.print(")");
    }
    else if (seq.menu == 20)
    {
      display.setTextColor(WHITE, BLUE);
      display.print("SEQ. LENGHT");
    }
    else if (seq.menu > 20 && seq.menu < 27)
    {
      display.setTextColor(WHITE, BLUE);
      display.print("TRACK TYPE ");
    }
    else if (seq.menu > 26)
    {
      display.setTextColor(WHITE, BLUE);
      display.print("SELECT PAT.");
    }
  }
  else if (LCDML.FUNC_getID() == LCDML.OTHER_getIDFromFunction(UI_func_seq_pattern_editor))
  {
    display.setTextColor(WHITE, BLUE);
    display.print("NOTE EDITOR");
  }
  display.setTextColor(WHITE, BLACK);
  display.setTextSize(2);
}

void print_track_steps_detailed(int xpos, int ypos, uint8_t currentstep, bool init, bool allsteps)
{
  //bool init = only print static content one time. if true, print the static content
  //allsteps ==  print all lines , allsteps == false print just the current step +-1 steps
  uint8_t i = 0;
  uint8_t laststep = 16;
  int y = 0;
  int x = 0;

  display.setTextSize(1);
  display.setTextColor(DX_PURPLE, BLACK);
  display.setCursor(xpos, ypos);

  if (init)
  { //only needs to be drawn at first run

    display.setTextColor(DX_PURPLE, BLACK);
    display.setCursor(xpos, ypos + 20);
    display.print("Step  N   DATA  VEL   NAME / NOTE");

  }
  if (allsteps == false)
  {
    if (currentstep == 0)
      laststep = 2;
    else if (currentstep == 15)
    {
      i = 14;
      laststep = 16;
    }
    else
    {
      i = currentstep - 1;
      laststep = currentstep + 2;
    }
  }

  while ( i < laststep )
  {
    x = xpos;
    y = ypos + 2 * CHAR_height + i * (CHAR_height - 5);

    if (init)
    { //only needs to be drawn at first run

      if (i % 4 == 0)
        display.setTextColor(GREY2, BLACK);
      else
        display.setTextColor(MIDDLEGREEN, BLACK);
      display.setCursor(x, y);
      seq_print_formatted_number (i , 2);
    }
    // Short Name
    if (i == currentstep)
      display.setTextColor(WHITE, BLUE);
    else
      display.setTextColor(DX_PURPLE, BLACK);
    display.setCursor(x + CHAR_width * 3 , y);
    if (seq.vel[seq.active_pattern][i] > 209)  //it is a pitched Drum Sample
    {
      seq_print_current_note_from_step(i);
    }
    else
    {
      display.print (seq_find_shortname_in_track( i , seq.active_pattern)[0]   );
    }
    // Data values
    if (i == currentstep)
      display.setTextColor(WHITE, BLUE);
    else
      display.setTextColor(GREY1, BLACK);
    display.setCursor(x + CHAR_width * 5, y);
    seq_print_formatted_number (seq.note_data[seq.active_pattern][i] , 3);

    // Velocity values
    if (i == currentstep)
      display.setTextColor(WHITE, BLUE);
    else
      display.setTextColor(GREY2, BLACK);
    display.setCursor(x + CHAR_width * 8, y);
    seq_print_formatted_number (seq.vel[seq.active_pattern][i] , 3);
    // Long Name / Note
    if (i == currentstep)
      display.setTextColor(WHITE, BLUE);
    else
      set_pattern_content_type_color(seq.active_pattern);
    if (seq.content_type[seq.active_pattern] == 0) //Drum Track
    {
      if (seq.vel[seq.active_pattern][i] > 209)  //it is a pitched Drum Sample
      {
        display.show_smallfont_noGrid(y, 144 , 11, basename(drum_config[seq.vel[seq.active_pattern][i] - 210].name) );
      }
      else // else it is a regular Drum Sample
        display.show_smallfont_noGrid(y, 144 , 11, find_long_drum_name_from_note( seq.note_data[seq.active_pattern][i] ));
    }
    else if (seq.content_type[seq.active_pattern] > 0 ) //Inst Track or Chord or Arp
    {
      display.setCursor(x + CHAR_width * 11, y);
      if (seq.note_data[seq.active_pattern][i] != 0)
      {
        if (seq.note_data[seq.active_pattern][i] == 130) //it is a latched note
        {
          if (i == currentstep)
            display.setTextColor(WHITE, BLUE);
          else
            display.setTextColor(GREEN, BLACK);
          display.write (0x7E); display.print("LATCH"); //Tilde Symbol for latched note
        }
        else
        {
          display.print(noteNames[seq.note_data[seq.active_pattern][i] % 12 ][0] );
          if (noteNames[seq.note_data[seq.active_pattern][i] % 12 ][1] != '\0' )
          {
            display.print(noteNames[seq.note_data[seq.active_pattern][i] % 12 ][1] );
          }
          if (seq.vel[ seq.active_pattern][i] < 200) //print octave is not a chord
          {
            display.print( (seq.note_data[seq.active_pattern][i] / 12) - 1);
          }
          if (seq.vel[ seq.active_pattern][i] > 199)  //is a chord
          {
            display.print(" ");
            print_chord_name(i);
          }
        }
      }
    }
    while (display.getCursorX() < 205)
    {
      if (i == currentstep)
        display.setTextColor(WHITE, BLUE);
      else
        display.setTextColor(GREY1, BLACK);
      display.print(" ");
    }
    i++;
  }
  display.setTextColor(WHITE, BLACK);
  display.setTextSize(2);
}

void seq_sub_display_menu_logic()
{
  if (seq_active_function == 1 && seq.menu == 19) // edit content type of current pattern
  {
    if ((LCDML.BT_checkDown() && encoderDir[ENC_R].Down()) || (LCDML.BT_checkUp() && encoderDir[ENC_R].Up()))
    {
      if (LCDML.BT_checkDown())
        seq.content_type[seq.active_pattern] = constrain(seq.content_type[seq.active_pattern] + 1, 0, 2);
      else if (LCDML.BT_checkUp())
        seq.content_type[seq.active_pattern] = constrain(seq.content_type[seq.active_pattern] - 1, 0, 2);
    }
  } else
    if (seq_active_function == 1 && seq.menu == 20) // edit chain length
    {
      if ((LCDML.BT_checkDown() && encoderDir[ENC_R].Down()) || (LCDML.BT_checkUp() && encoderDir[ENC_R].Up()))
      {
        if (LCDML.BT_checkDown())
          seq.chain_lenght = constrain(seq.chain_lenght + 1, 0, 3);
        else if (LCDML.BT_checkUp())
          seq.chain_lenght = constrain(seq.chain_lenght - 1, 0, 3);
      }
    } else
      for (uint8_t i = 0; i < NUM_SEQ_TRACKS; i++)  // select track type

        if (seq_active_function == 1 && seq.menu == 21 + i) // edit
        {
          if ((LCDML.BT_checkDown() && encoderDir[ENC_R].Down()) || (LCDML.BT_checkUp() && encoderDir[ENC_R].Up()))
          {
            if (LCDML.BT_checkDown())
              seq.track_type[i] = constrain(seq.track_type[i] + 1, 0, 3);
            else if (LCDML.BT_checkUp())
              seq.track_type[i] = constrain(seq.track_type[i] - 1, 0, 3);
          }
        }
  for (uint8_t i = 0; i < NUM_SEQ_TRACKS; i++)  // assign patterns to pattern chain
  {
    for (uint8_t j = 0; j < seq.chain_lenght + 1; j++)
    {
      if (seq_active_function == 1 && seq.menu == 27 + i + (j * 6) )
      {
        if ((LCDML.BT_checkDown() && encoderDir[ENC_R].Down()) || (LCDML.BT_checkUp() && encoderDir[ENC_R].Up()))
        {
          if (LCDML.BT_checkDown())
            seq.patternchain[j][i] = constrain(seq.patternchain[j][i] + 1, 0, NUM_SEQ_PATTERN - 1);
          else if (LCDML.BT_checkUp())
            seq.patternchain[j][i] = constrain(seq.patternchain[j][i] - 1, 0, NUM_SEQ_PATTERN - 1);
        }
      }
    }
  }
}

void UI_func_seq_vel_editor(uint8_t param)
{
  char tmp[5];
  if (LCDML.FUNC_setup())         // ****** SETUP *********
  {
    // 0-15 steps 1-16
    // 19 content type
    // 20 seq lenght
    // tracktype               21 track 1   22 track 2  23 track 3   24 track 4   25 track 5   26 track 6
    // 27-50 assign patterns
    // 51 performance select
    
    // setup function
    encoderDir[ENC_R].reset();
    print_edit_mode();
    seq.menu = 1;
    //clear "Insert xyz ?"  message
    display.fillRect(1 * CHAR_width, 4 * CHAR_height + 17, 222, 8, BLACK);
    display.setCursor_textGrid(15, 1);
    display.setTextColor(GREY1, BLACK);
    display.print("[");
    display.setCursor_textGrid(16, 1);
    seq_print_formatted_number(seq.active_pattern, 2);
    display.setCursor_textGrid(18, 1);
    display.print("]");
    if (seq.menu_status != 1)
    {
      display.setCursor(CHAR_width, CHAR_height * 4 + 3);
      display.setTextSize(1);
      display.setTextColor(GREY2, BLACK);
      display.print("CONT.TYPE:");
      display.setCursor(CHAR_width * 6, CHAR_height * 4 + 3);
      print_content_type();
      seq_printVelGraphBar();
    }
    display.setTextSize(2);
    seq_printAllSeqSteps();
  }
  if (LCDML.FUNC_loop())          // ****** LOOP *********
  {
    seq_sub_display_menu_logic();
    if (seq_active_function == 99 && seq.menu > 18) // sub_menus
    {
      if ((LCDML.BT_checkDown() && encoderDir[ENC_R].Down()) || (LCDML.BT_checkUp() && encoderDir[ENC_R].Up()))
      {
        if (LCDML.BT_checkDown())
          seq.menu = constrain(seq.menu + 1, 19, 50);
        else if (LCDML.BT_checkUp())
        {
          if (seq.menu == 19)
          {
            seq.menu = 16;  //return to vel_editor
          }
          else
            seq.menu = constrain(seq.menu - 1, 19, 50);
        }
      }
    }
    if (seq_active_function == 99 && seq.menu < 17)
    {
      if ((LCDML.BT_checkDown() && encoderDir[ENC_R].Down()) || (LCDML.BT_checkUp() && encoderDir[ENC_R].Up()))
      {
        if (LCDML.BT_checkDown())
        {
          if (seq.menu == 16)
          {
            seq.menu = 19;
            seq_sub_display_menu_logic();
          }
          else
            seq.menu = constrain(seq.menu + 1, 0, 16);
        }
        else if (LCDML.BT_checkUp())
        {
          if (seq.menu == 0)
          { seq.menu_status = 2;
            LCDML.OTHER_jumpToFunc(UI_func_seq_pattern_editor);
          }
          else
          {
            seq.menu_status = 0;
            seq.menu = constrain(seq.menu - 1, 0, 16);
          }
        }
      }
    }
    else if (seq_active_function == 0)
    {
      if ((LCDML.BT_checkDown() && encoderDir[ENC_R].Down()) || (LCDML.BT_checkUp() && encoderDir[ENC_R].Up()))
      {
        if (LCDML.BT_checkDown())
          seq.active_pattern = constrain(seq.active_pattern + 1, 0, NUM_SEQ_PATTERN - 1);
        else if (LCDML.BT_checkUp())
          seq.active_pattern = constrain(seq.active_pattern - 1, 0, NUM_SEQ_PATTERN - 1);
      }
    }
    if ( seq.note_data[seq.active_pattern][seq.menu - 1] > 0 )
    {
      if (seq.vel[seq.active_pattern][seq.menu - 1] < 210) //it is a normal sample
      {
        if (seq_active_function == 1 && seq.content_type[seq.active_pattern] < 2 )
        { //if is Drum or normal Instrument Track
          if ((LCDML.BT_checkDown() && encoderDir[ENC_R].Down()) || (LCDML.BT_checkUp() && encoderDir[ENC_R].Up()))
          {
            if (LCDML.BT_checkDown())
              seq.vel[seq.active_pattern][seq.menu - 1] = constrain(seq.vel[seq.active_pattern][seq.menu - 1] + 1, 0, 127);
            else if (LCDML.BT_checkUp())
              seq.vel[seq.active_pattern][seq.menu - 1] = constrain(seq.vel[seq.active_pattern][seq.menu - 1] - 1, 0, 127);
          }
        }
        else if (seq_active_function == 1 && seq.content_type[seq.active_pattern] > 1 )
        { //is in Chord or Arp Mode
          if ((LCDML.BT_checkDown() && encoderDir[ENC_R].Down()) || (LCDML.BT_checkUp() && encoderDir[ENC_R].Up()))
          {
            if (LCDML.BT_checkDown())
              seq.vel[seq.active_pattern][seq.menu - 1] = constrain(seq.vel[seq.active_pattern][seq.menu - 1] + 1, 200, 205);
            else if (LCDML.BT_checkUp())
              seq.vel[seq.active_pattern][seq.menu - 1] = constrain(seq.vel[seq.active_pattern][seq.menu - 1] - 1, 200, 205);
          }
        }
      }
      else
      {
        //is in pitched Sample Mode
        if ((LCDML.BT_checkDown() && encoderDir[ENC_R].Down()) || (LCDML.BT_checkUp() && encoderDir[ENC_R].Up()))
        {
          if (LCDML.BT_checkDown())
            seq.vel[seq.active_pattern][seq.menu - 1] = constrain(seq.vel[seq.active_pattern][seq.menu - 1] + 1, 210, 215);
          else if (LCDML.BT_checkUp())
            seq.vel[seq.active_pattern][seq.menu - 1] = constrain(seq.vel[seq.active_pattern][seq.menu - 1] - 1, 210, 215);
        }
      }
    }
    if (LCDML.BT_checkEnter())  //handle button presses during menu >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
    {
      if ( seq.menu == 0 && seq_active_function == 99)
      {
        seq_active_function = 0;

      } else if ( seq.menu == 0 && seq_active_function == 0)
      {
        seq_active_function = 99;
      }
      else if (seq.menu > 0 && seq.menu < 20 && seq_active_function == 99)
      {
        seq_active_function = 1;
        //clear "Insert xyz ?"  message
        //  display.fillRect(1 * CHAR_width, 4 * CHAR_height + 17, 222, 8, BLACK);
        display.setCursor(1 * CHAR_width, 4 * CHAR_height + 17);
        display.setTextSize(1);
        display.setTextColor(GREEN, BLACK);
        display.print("EDIT ");
        display.setTextColor(WHITE, BLACK);
        display.print("VALUE");
        display.setTextColor(GREEN, BLACK);
        display.print(" WITH ");
        display.setTextColor(BLACK, WHITE);
        display.write (0x1B);
        display.setTextColor(GREEN, BLACK);
        display.print(" ");
        display.setTextColor(BLACK, WHITE);
        display.write (0x1A);
        display.setTextColor(GREEN, BLACK);
        display.print(" ENCODER_R ");
        display.setTextColor(WHITE, BLACK);
        display.setTextSize(2);
      }
      else if (seq.menu > 19 && seq.menu < 51 && seq_active_function == 99)
      {
        seq_active_function = 1;
      }
      else
      {
        seq_active_function = 99;
      }
    }
    //button check end <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
    if ( seq.content_type[seq.active_pattern] > 1 && seq.vel[seq.active_pattern][seq.menu - 1] < 200)
      seq.vel[seq.active_pattern][seq.menu - 1] = 200;
    if (seq_active_function == 0)
    {
      display.setTextSize(1);
      display.setCursor(CHAR_width * 6, CHAR_height * 4 + 3);
      print_content_type();
      display.setTextSize(2);
    }
    if (seq.menu == 0)
    {
      display.setTextSize(2);
      display.setTextColor(WHITE, BLACK);
      display.setCursor_textGrid(15, 1);
      display.print("[");
      seq_print_formatted_number(seq.active_pattern, 2);
      display.print("]");
      display.setCursor_textGrid(1, 2);
      seq_printAllSeqSteps();
      seq_printVelGraphBar();
      if (seq.content_type[seq.active_pattern] == 0) //Drum Mode
      {
        if (seq.note_editor_view != 0)
        {
          seq.note_editor_view = 0;
          border3_clear();
          print_track_steps_detailed(CHAR_width, CHAR_height * 5 + 3, 254, true, true);
        }
        else
          print_track_steps_detailed(CHAR_width, CHAR_height * 5 + 3, 254, false, true);
      }
      else
      {
        if (seq.note_editor_view != 1)
        {
          seq.note_editor_view = 1;
          border3_clear();
          print_single_pattern_pianoroll(CHAR_width, 316, seq.active_pattern, seq.menu - 3, true);
        }
        else
          print_single_pattern_pianoroll(CHAR_width, 316, seq.active_pattern, seq.menu - 3, false);
      }
    }
    else if (seq.menu == 1)
    {
      display.setTextSize(2);
      display.setCursor_textGrid(15, 1);
      display.print(" ");
      display.setCursor_textGrid(18, 1);
      display.print(" ");

      display.setCursor_textGrid(1, 2);
      display.setTextColor(GREEN, DX_PURPLE);
      display.setCursor_textGrid(1, 2);
      display.print(seq_find_shortname(0)[0] );

      set_pattern_content_type_color(seq.active_pattern);
      display.setCursor_textGrid(2, 2);
      display.print(seq_find_shortname(1)[0] );
      display.setTextColor(WHITE, BLACK);

      if (seq.vel[seq.active_pattern][seq.menu - 1] < 210 && seq.content_type[seq.active_pattern] < 2) //it is a normal sample
      {
        sprintf(tmp, "%03d", seq.vel[seq.active_pattern][seq.menu - 1]);
        display.setCursor_textGrid(2, 1);
        display.print(tmp);
      }
    }
    else if (seq.menu > 1 && seq.menu < 17 )
    {
      display.setTextSize(2);
      set_pattern_content_type_color(seq.active_pattern);
      display.setCursor_textGrid(seq.menu - 1, 2);
      if (seq.vel[seq.active_pattern][seq.menu - 2] > 209) //if pitched sample, change color
        display.setTextColor(DX_CYAN, BLACK);
      else
        set_pattern_content_type_color(seq.active_pattern);
      display.print(seq_find_shortname(seq.menu - 2)[0] );

      display.setCursor_textGrid(seq.menu , 2);
      display.setTextColor(GREEN, DX_PURPLE);
      display.print(seq_find_shortname(seq.menu - 1)[0] );
      set_pattern_content_type_color(seq.active_pattern);

      if (seq.menu < 16)
      {
        display.setCursor_textGrid(seq.menu + 1, 2);
        if (seq.vel[seq.active_pattern][seq.menu] > 209) //if pitched sample, change color
          display.setTextColor(DX_CYAN, BLACK);
        else
          set_pattern_content_type_color(seq.active_pattern);
        display.print(seq_find_shortname(seq.menu)[0] );
      }
    }
    if (seq.menu > 0 && seq.menu < 17 )
    {
      display.setTextSize(2);
      if (seq.content_type[seq.active_pattern] == 0) //Drum Mode
      {
        if (seq.menu - 1 == 0)
          print_track_steps_detailed(CHAR_width, CHAR_height * 5 + 3, seq.menu - 1, false, true);
        else
          print_track_steps_detailed(CHAR_width, CHAR_height * 5 + 3, seq.menu - 1, false, false);
      }
      else
        print_single_pattern_pianoroll(CHAR_width, 316, seq.active_pattern, seq.menu - 1, false);

      display.setCursor_textGrid(4, 1);
      if (seq.note_data[seq.active_pattern][seq.menu - 1] > 0)
      {
        if (seq.content_type[seq.active_pattern] == 0) //is Drumtrack
        {
          display.setCursor_textGrid(1, 1);
          if (seq.vel[seq.active_pattern][seq.menu - 1] < 210) //it is a normal sample
          {
            display.setTextColor(WHITE, BLACK);
            display.print("V:");
            sprintf(tmp, "%03d", seq.vel[seq.active_pattern][seq.menu - 1]);
            display.print(tmp);
            display.print(" ");
            display.setTextColor(DX_ORANGE, BLACK);
            display.show(1, 7, 7, seq_find_drum_name_from_note( seq.note_data[seq.active_pattern][seq.menu - 1]) );
            if (seq_active_function == 99)
            {
              display.setCursor(1 * CHAR_width, 4 * CHAR_height + 17);
              display.setTextSize(1);
              display.setTextColor(GREEN, BLACK);
              display.print("EDIT VELOCITY OF STEP ");
              display.setTextColor(WHITE, BLACK);
              display.print(seq.menu);
              display.setTextColor(GREEN, BLACK);
              display.print(" ? ");
              display.setTextSize(2);
              display.setTextColor(WHITE, BLACK);
            }
          }
          else
          { // else it is a live-pitched sample
            display.setTextColor(WHITE, BLACK);
            display.setCursor_textGrid(1, 1);
            display.print("Smp:[");
            display.setCursor_textGrid(14, 1);
            display.print("]");
            display.setTextColor(DX_CYAN, BLACK);
            display.show(1, 6, 8, basename(drum_config[seq.vel[seq.active_pattern][seq.menu - 1] - 210].name));

            if (seq_active_function == 99)
            {
              display.setCursor(1 * CHAR_width, 4 * CHAR_height + 17);
              display.setTextSize(1);
              display.setTextColor(GREEN, BLACK);
              display.print("REPLACE ");
              display.setTextColor(BLACK, GREEN);
              display.print( basename(drum_config[seq.vel[seq.active_pattern][seq.menu - 1] - 210].name));
              display.setTextColor(GREEN, BLACK);
              display.print(" IN STEP ");
              display.setTextColor(WHITE, BLACK);
              display.print(seq.menu);
              display.setTextColor(GREEN, BLACK);
              display.print(" ? ");
              display.setTextColor(WHITE, BLACK);
              display.setTextSize(2);
            }
          }
        } else
        {
          if ( seq.note_data[seq.active_pattern][seq.menu - 1] != 130  ) //note not latched
          {
            if (seq.content_type[seq.active_pattern] < 2)
            {
              display.setCursor_textGrid(1, 1);
              display.print("Vel:");
              sprintf(tmp, "%03d", seq.vel[seq.active_pattern][seq.menu - 1]);
              display.setCursor_textGrid(5, 1);
              display.print(tmp);
              display.print(" ");

            }
            set_pattern_content_type_color(seq.active_pattern);

            display.setCursor_textGrid(11, 1);
            display.print(noteNames[seq.note_data[seq.active_pattern][seq.menu - 1] % 12 ][0] );
            if (noteNames[seq.note_data[seq.active_pattern][seq.menu - 1] % 12 ][1] != '\0' ) {
              display.print(noteNames[seq.note_data[seq.active_pattern][seq.menu - 1] % 12 ][1] );
            }
            display.print( (seq.note_data[seq.active_pattern][seq.menu - 1] / 12) - 1);
            display.print("  ");
          }
          else
          { //note is latched
            display.setCursor_textGrid(1, 1);
            display.print("latched note ");
          }
          if (seq.content_type[seq.active_pattern] > 1) // is not drum or inst, print chord
          {
            display.setCursor_textGrid(1, 1);
            display.setTextColor(WHITE, BLACK);
            display.print("[");
            set_pattern_content_type_color(seq.active_pattern);
            print_chord_name(seq.vel[seq.active_pattern][seq.menu - 1 - 200]);
            display.setTextColor(WHITE, BLACK);
            display.print("]");
          }
        }
        seq_printVelGraphBar_single_step(seq.menu - 1, GREEN);
        if (seq.menu - 1 > 0)
          seq_printVelGraphBar_single_step(seq.menu - 2, GREY2); //previous
        if (seq.menu - 1 < 15)
          seq_printVelGraphBar_single_step(seq.menu, GREY2 ); //next
      }
      else
      {
        display.setCursor_textGrid(1, 1);
        display.print("              ");
        if (seq.menu - 1 > 0)
          seq_printVelGraphBar_single_step(seq.menu - 2, GREY2); //previous
        if (seq.menu - 1 < 15)
          seq_printVelGraphBar_single_step(seq.menu, GREY2 ); //next

        //clear "Insert xyz ?"  message
        display.fillRect(1 * CHAR_width, 4 * CHAR_height + 17, 222, 8, BLACK);
      }
    }
    if (seq.menu == 19 ) //edit content type of pattern
    {
      if (seq_active_function != 1)
      {
        //fix last step of sequence when switching to sub menus functions
        print_track_steps_detailed(CHAR_width, CHAR_height * 5 + 3, seq.menu - 1, false, true);
        seq_printVelGraphBar_single_step(15, GREY2);
        set_pattern_content_type_color(seq.active_pattern);
        display.setTextSize(2);
        display.setCursor_textGrid(16 , 2);
        display.print(seq_find_shortname(15)[0] );
        display.setCursor(1 * CHAR_width, 4 * CHAR_height + 17);
        display.setTextSize(1);
        display.setTextColor(GREEN, BLACK);
        display.print("EDIT CONTENT TYPE OF PATTERN ");
        display.setTextColor(WHITE, BLACK);
        display.print(seq.active_pattern);
        display.setTextColor(GREEN, BLACK);
        display.print(" ? ");
      }
      print_edit_mode();
      display.setTextSize(1);
      display.setTextColor(WHITE, BLUE);
      display.setCursor(CHAR_width, CHAR_height * 4 + 3);
      display.print("CONT.TYPE:");
      display.setCursor(CHAR_width * 6, CHAR_height * 4 + 3);
      print_content_type();
      display.setTextSize(2);
      seq_printAllSeqSteps();
      seq_printVelGraphBar();
      border3_clear();
      if (seq.content_type[seq.active_pattern] == 0) //Drum Mode
      {
        print_track_steps_detailed(CHAR_width, CHAR_height * 5 + 3, seq.menu - 1, true, true);
      }
      else
        print_single_pattern_pianoroll(CHAR_width, 316, seq.active_pattern, seq.menu - 1, true);
      seq_sub_pat_chain(CHAR_width * 21, CHAR_height * 9, false);
    }
    else if (seq.menu == 20 )
    {
      //disable menu 19
      display.setCursor(CHAR_width, CHAR_height * 4 + 3);
      display.setTextSize(1);
      display.setTextColor(GREY2, BLACK);
      display.print("CONT.TYPE:");
      display.setCursor(CHAR_width * 6, CHAR_height * 4 + 3);
      print_content_type();
      display.setTextSize(2);
      // disable menu 19 end
      display.setTextSize(1);
      display.setCursor_textGrid(16 , 2);
      display.print(seq_find_shortname(15)[0] );
      display.setCursor(1 * CHAR_width, 4 * CHAR_height + 17);
      display.setTextSize(1);
      display.setTextColor(GREEN, BLACK);
      display.print("EDIT PATTERN CHAIN LENGTH ?      ");
      display.setTextColor(WHITE, BLACK);
      display.setTextSize(2);
      print_edit_mode();
      seq_sub_pat_chain(CHAR_width * 21, CHAR_height * 9, false);
    }
    else if (seq.menu > 20  && seq.menu < 27) // sub menus functions: track type
    {
      print_edit_mode();
      display.setTextSize(1);
      display.setCursor(1 * CHAR_width, 4 * CHAR_height + 17);
      if (seq_active_function != 1)
      {
        display.setTextColor(GREEN, BLACK);
        display.print("EDIT TRACK TYPE OF ");
        display.setTextColor( WHITE, BLACK);
        display.print("TRACK ");
        display.print( seq.menu - 20);
        display.setTextColor(GREEN, BLACK);
        display.print(" ?    ");

        display.setTextColor(WHITE, BLACK);
        display.setTextSize(2);
      } else
      {
        display.setTextColor(GREEN, BLACK);
        display.print("SET TRACK TYPE = ");

        set_track_type_color(seq.menu - 21);

        if (seq.track_type[seq.menu - 21] == 0 )   display.print("Drums    ");
        else if (seq.track_type[seq.menu - 21] == 1 ) display.print("Instr.   ");
        else if (seq.track_type[seq.menu - 21] == 2 ) display.print("Chord    ");
        else display.print("Arpeggio ");
        display.setTextSize(2);
      }
      seq_sub_pat_chain(CHAR_width * 21, CHAR_height * 9, false);
    }
    else if (seq.menu > 26  && seq.menu < 51) // sub menus functions: pattern select
    {
      print_edit_mode();
      display.setTextSize(1);
      display.setCursor(1 * CHAR_width, 4 * CHAR_height + 17);
      if (seq_active_function != 1)
      {
        display.setTextColor(GREEN, BLACK);
        display.print("ASSIGN PATTERN ?             ");
        display.setTextColor(WHITE, BLACK);
        display.setTextSize(2);
      } else
      {
        display.setTextColor(GREEN, BLACK);
        display.print("SELECT PATTERN FOR THIS STEP ");

        display.setTextSize(2);
      }
      if (seq.running == false)
        seq.chain_active_step = (seq.menu - 27) / NUM_SEQ_TRACKS;

      seq_sub_pat_chain(CHAR_width * 21, CHAR_height * 9, false);
    }
    else if (seq.menu == 16 )
    {
      display.setTextSize(1);
      display.setTextColor(GREY2, BLACK);
      display.setCursor(CHAR_width, CHAR_height * 4 + 3);
      display.print("CONT.TYPE:");
      display.setCursor(CHAR_width * 6, CHAR_height * 4 + 3);
      print_content_type();
      display.setTextSize(2);
      print_edit_mode();
      seq_sub_pat_chain(CHAR_width * 21, CHAR_height * 9, false);
    }
  }
  if (LCDML.FUNC_close())     // ****** STABLE END *********
  {
    encoderDir[ENC_R].reset();

    if (seq.menu_status != 2) // don't clear screen when jumping to pattern editor
    {
      display.fillScreen(BLACK);
    }
  }
}

void seq_clear_active_pattern()
{
  memset(seq.note_data[seq.active_pattern], 0, sizeof(seq.note_data[seq.active_pattern]));
  memset(seq.vel[seq.active_pattern], 0, sizeof(seq.vel[seq.active_pattern]));
}
void seq_clear_all_patterns()
{
  for (uint8_t i = 0; i < NUM_SEQ_PATTERN - 1; i++)
  {
    memset(seq.note_data[i], 0, sizeof(seq.note_data[i]));
    memset(seq.vel[i], 0, sizeof(seq.vel[i]));
  }
}

void seq_refresh_display_play_status()
{
  display.fillRect(13 * CHAR_width, CHAR_height, 13, 16, BLACK);
  if (seq.running == false && seq.recording == false)
  {
    //  lcd.createChar(0, (uint8_t*)special_chars[19]); //play symbol
    display.drawBitmap(13 * CHAR_width + 3, CHAR_height + 3, special_chars[19], 8, 8, GREEN);
  } else if (seq.running == true && seq.recording == false)
  {
    seq.note_in = 0;
    //  lcd.createChar(0, (uint8_t*)special_chars[20]); //record symbol
    display.drawBitmap(13 * CHAR_width + 3, CHAR_height + 3, special_chars[20], 8, 8, RED);
  } else if (seq.running == true && seq.recording == true)
  {
    seq.note_in = 0;
    //   lcd.createChar(0, (uint8_t*)special_chars[21]); //stop symbol
    display.drawBitmap(13 * CHAR_width + 3, CHAR_height + 3, special_chars[21], 8, 8, DX_CYAN);
  }
}

void arp_refresh_display_play_status()
{

  display.fillRect(17 * CHAR_width, CHAR_height, 13, 16, BLACK);
  if (seq.running == false )
  {
    //play symbol
    display.drawBitmap(17 * CHAR_width , CHAR_height + 3, special_chars[19], 8, 8, GREEN);
  }  else if (seq.running == true )
  {
    seq.note_in = 0;
    //stop symbol
    display.drawBitmap(17 * CHAR_width , CHAR_height + 3, special_chars[21], 8, 8, DX_CYAN);
  }
}

void check_variable_samples_basespeed()
{
  for (uint8_t i = 0; i < 6; i++)
  {
    if (drum_config[i].p_offset == 0)
      drum_config[i].p_offset = 1;
  }
}

void print_sample_type ()
{
  if (activesample < NUM_DRUMSET_CONFIG - 1)
  {
    if (drum_config[activesample].drum_class == 0)
      display.print("none      ");
    else if (drum_config[activesample].drum_class == 1)
      display.print("BASSDRUM  ");
    else if (drum_config[activesample].drum_class == 2)
      display.print("SNARE     ");
    else if (drum_config[activesample].drum_class == 3)
      display.print("HIHAT     ");
    else if (drum_config[activesample].drum_class == 4)
      display.print("HANDCLAP  ");
    else if (drum_config[activesample].drum_class == 5)
      display.print("RIDE      ");
    else if (drum_config[activesample].drum_class == 6)
      display.print("CRASH     ");
    else if (drum_config[activesample].drum_class == 7)
      display.print("LOW TOM   ");
    else if (drum_config[activesample].drum_class == 8)
      display.print("MID TOM   ");
    else if (drum_config[activesample].drum_class == 9)
      display.print("HIGH TOM  ");
    else if (drum_config[activesample].drum_class == 10)
      display.print("PERCUSSION");
    else if (drum_config[activesample].drum_class == 11)
      display.print("POLY/SYNTH");
    else
      display.print("other     ");
  }
}

void seq_sub_copy_swap ()
{
  if (seq.menu == 30 || seq.menu == 31 )
  { // is in sub-function - swap pattern or copy pattern
    seq_active_function = 98;
    if ((LCDML.BT_checkDown() && encoderDir[ENC_R].Down()) || (LCDML.BT_checkUp() && encoderDir[ENC_R].Up()))
    {
      if (LCDML.BT_checkDown()) {

        temp_int = constrain(temp_int + 1, 0, NUM_SEQ_PATTERN - 1);
        if (temp_int == seq.active_pattern)temp_int++;
        if (temp_int > NUM_SEQ_PATTERN - 1)temp_int = 0;
      }
      else if (LCDML.BT_checkUp()) {
        temp_int = constrain(temp_int - 1, 0, NUM_SEQ_PATTERN - 1);
        if (temp_int == seq.active_pattern)temp_int--;
        if (temp_int < 0)temp_int = NUM_SEQ_PATTERN - 1;
      }
    }
  }
  if (LCDML.BT_checkEnter())  //handle button presses during menu >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
  {
    if (seq.menu == 31 && seq_active_function != 40) //copy patterns
    {
      memcpy( seq.note_data[temp_int], seq.note_data[seq.active_pattern], sizeof(seq.note_data[0]));
      memcpy( seq.vel[temp_int], seq.vel[seq.active_pattern], sizeof(seq.vel[0]));
      seq.content_type[temp_int] = seq.content_type[seq.active_pattern];
      seq.menu = 0;
      seq_active_function = 0;
      activesample = 0;
      temp_int = seq.note_data[seq.active_pattern][0];
      display.setCursor_textGrid(15, 1);
      display.print(" ");
      display.setCursor_textGrid(18, 1);
      display.print(" ");
      display.setCursor_textGrid(2, 1);
      display.print("         ");
      display.setCursor_textGrid(17, 2);
      display.print("  ");
      seq_refresh_display_play_status();
      seq_printAllSeqSteps();
    }
    else if (seq.menu == 30 && seq_active_function != 40) //swap patterns
    {
      uint8_t data_temp[1][16];
      uint8_t vel_temp[1][16];
      uint8_t content_type_temp;
      memcpy( data_temp[0], seq.note_data[seq.active_pattern], sizeof(data_temp[0]));
      memcpy( vel_temp[0], seq.vel[seq.active_pattern], sizeof(vel_temp[0]));
      content_type_temp = seq.content_type[seq.active_pattern];
      memcpy( seq.note_data[seq.active_pattern], seq.note_data[temp_int], sizeof(data_temp[0]));
      memcpy( seq.vel[seq.active_pattern], seq.vel[temp_int], sizeof(vel_temp[0]));
      seq.content_type[seq.active_pattern] = seq.content_type[temp_int];
      memcpy( seq.note_data[temp_int], data_temp[0], sizeof(data_temp[0]));
      memcpy( seq.vel[temp_int], vel_temp[0], sizeof(vel_temp[0]));
      seq.content_type[temp_int] = content_type_temp;
      seq.menu = 0;
      seq_active_function = 0;
      activesample = 0;
      display.setCursor_textGrid(15, 1);
      display.print(" ");
      display.setCursor_textGrid(18, 1);
      display.print(" ");
      display.setCursor_textGrid(2, 1);
      display.print("         ");
      display.setCursor_textGrid(17, 2);
      display.print("  ");
      temp_int = seq.note_data[seq.active_pattern][0];
      seq_refresh_display_play_status();
      seq_printAllSeqSteps();
      print_track_steps_detailed(CHAR_width, CHAR_height * 5 + 3, 254, false, true);
    }

    if ( seq.menu == 0 && seq_active_function == 0)
    {
      if ( (seq.content_type[seq.active_pattern] == 0 && activesample == NUM_DRUMSET_CONFIG + 3) || (seq.content_type[seq.active_pattern] > 0 && temp_int == 114) )
      { //swap patterns: Active pattern <-> destination pattern
        display.setCursor_textGrid(1, 1);
        display.print("SwapPattern:");
        temp_int = seq.active_pattern + 1;
        if (temp_int > NUM_SEQ_PATTERN - 1)temp_int = 0;
        seq.menu = 30;
      }
      else if ( (  seq.content_type[seq.active_pattern] == 0 && activesample == NUM_DRUMSET_CONFIG + 2) || ( seq.content_type[seq.active_pattern] > 0 && temp_int == 113) )
      { //copy pattern
        display.setCursor_textGrid(1, 1);
        display.print("Copy Pattern:");
        temp_int = seq.active_pattern + 1;
        if (temp_int > NUM_SEQ_PATTERN - 1)temp_int = 0;
        seq.menu = 31;
      }
    }
  }
  //button check end <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<

  if (seq.menu == 31)
  { //copy pattern
    display.setCursor_textGrid(15, 1);
    display.print("[");
    display.setCursor_textGrid(18, 1);
    display.print("]");
    display.setCursor_textGrid(1, 2);
    display.print("          to: [  ]");
    display.setCursor_textGrid(16, 2);
    seq_print_formatted_number(temp_int, 2);
  }
  else if (seq.menu == 30)
  { //swap pattern
    display.setCursor_textGrid(15, 1);
    display.print("[");
    display.setCursor_textGrid(18, 1);
    display.print("]");
    display.setCursor_textGrid(1, 2);
    display.print("        with: [  ]");
    display.setCursor_textGrid(16, 2);
    seq_print_formatted_number(temp_int, 2);
  }
}

void seq_sub_pattern_fill ()
{
  if (seq.menu == 33)
  { // is in sub-function - fill pattern
    seq_active_function = 95;
    if ((LCDML.BT_checkDown() && encoderDir[ENC_R].Down()) || (LCDML.BT_checkUp() && encoderDir[ENC_R].Up()))
    {
      if (LCDML.BT_checkDown())
      {
        seq.temp_active_menu = constrain(seq.temp_active_menu + 1, 0, 2);  //fill step 1/4 , 1/8, 1/16
      }
      else if (LCDML.BT_checkUp())
      {
        seq.temp_active_menu = constrain(seq.temp_active_menu - 1, 0, 2);  //fill step 1/4 , 1/8, 1/16
      }
    }
  }
  else if (seq.menu == 32)
  { // is in sub-function - fill pattern
    seq_active_function = 97;
    if (seq.content_type[seq.active_pattern] == 0)
    {
      if ((LCDML.BT_checkDown() && encoderDir[ENC_R].Down()) || (LCDML.BT_checkUp() && encoderDir[ENC_R].Up()))
      {
        if (LCDML.BT_checkDown())
        {
          seq.temp_select_menu = constrain(seq.temp_select_menu + 1, 0, NUM_DRUMSET_CONFIG - 1);
        }
        else if (LCDML.BT_checkUp())
        {
          seq.temp_select_menu = constrain(seq.temp_select_menu - 1, 0, NUM_DRUMSET_CONFIG - 1);
        }
      }
    }
    else
    {
      if ((LCDML.BT_checkDown() && encoderDir[ENC_R].Down()) || (LCDML.BT_checkUp() && encoderDir[ENC_R].Up()))
      {
        if (LCDML.BT_checkDown())
          seq.temp_select_menu = constrain(seq.temp_select_menu + 1, 0, 108 );
        else if (LCDML.BT_checkUp())
          seq.temp_select_menu = constrain(seq.temp_select_menu - 1, 0, 108 );
      }
    }
  }

  if (LCDML.BT_checkEnter())  //handle button presses during menu >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
  {
    if (seq.menu == 32 && seq_active_function == 97) //fill pattern every 1/4, 1/8, 1/16 step with active sample/note  step 1
    {
      seq_active_function = 96;
      seq.menu = 33;
    }
    else if (seq.menu == 33 && seq_active_function == 95) //fill pattern every 1/4, 1/8, 1/16 step with active sample/note  step 2
    {
      if (seq.content_type[seq.active_pattern] == 0)
      { //Drumtrack
        for (uint8_t i = 0; i < 16; i++)
        {
          seq.note_data[seq.active_pattern][i] = drum_config[seq.temp_select_menu].midinote;
          seq.vel[seq.active_pattern][i] = 120;
          if (seq.temp_active_menu == 0) i = i + 3; else if (seq.temp_active_menu == 1) i = i + 1;
        }
      }
      else
      { //Inst. Track
        for (uint8_t i = 0; i < 16; i++)
        {
          seq.note_data[seq.active_pattern][i] = seq.temp_select_menu;
          seq.vel[seq.active_pattern][i] = 120;
          if (seq.temp_active_menu == 0) i = i + 3;
          else if (seq.temp_active_menu == 1) i = i + 1;
        }
      }
      seq.menu = 0;
      seq_active_function = 0;
      activesample = 0;
      temp_int = seq.note_data[seq.active_pattern][0];
      display.setCursor_textGrid(2, 1);
      display.print("         ");
      display.setCursor_textGrid(17, 2);
      display.print("  ");
      seq_refresh_display_play_status();
      seq_printAllSeqSteps();
      seq_printVelGraphBar();
      print_track_steps_detailed(CHAR_width, CHAR_height * 5 + 3, 254, false, true);

    }
    else if ( seq.menu == 0 && seq_active_function == 0)
    {

      if ( (seq.content_type[seq.active_pattern] == 0 && activesample == NUM_DRUMSET_CONFIG + 4) || (seq.content_type[seq.active_pattern] > 0 && temp_int == 115) )
      { //fill patterns
        display.setCursor_textGrid(1, 1);
        display.print("Fill Pattern:");
        seq.menu = 32;
        seq.temp_select_menu = 0;
        seq.temp_active_menu = 0;
      }
    }
  }
  //button check end <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<

  if (seq.menu == 33)  //editor step 2
  { //fill pattern 2nd parameter
    display.setCursor_textGrid(6, 2);
    display.print(" ");
    display.setCursor_textGrid(13, 2);
    display.print(" ");
    display.setCursor_textGrid(13, 2);
    display.print("[");
    if (seq.temp_active_menu == 0) display.print(" 1/4");
    else if (seq.temp_active_menu == 1) display.print(" 1/8");
    else if (seq.temp_active_menu == 2) display.print("1/16");
    display.print("]");
  }
  else if (seq.menu == 32  )  //editor step 1
  { //fill pattern
    if (seq.content_type[seq.active_pattern] == 0)
    { //drum
      display.setCursor_textGrid(1, 2);
      display.print("with [");
      display.show(2, 7, 6, basename(drum_config[seq.temp_select_menu].name));
      display.setCursor_textGrid(13, 2);
      display.print("]");
      display.setCursor_textGrid(14, 2);
      if (seq.temp_active_menu == 0) display.print(" 1/4");
      else if (seq.temp_active_menu == 1) display.print(" 1/8");
      else if (seq.temp_active_menu == 2) display.print("1/16");
    }
    else
    { //inst
      display.setCursor_textGrid(1, 2);
      display.print("with [");
      display.print(noteNames[seq.temp_select_menu % 12 ]);
      display.print( (seq.temp_select_menu / 12) - 1);
      display.print(" ");
      display.setCursor_textGrid(13, 2);
      display.print("]");
      display.setCursor_textGrid(14, 2);
      if (seq.temp_active_menu == 0) display.print(" 1/4");
      else if (seq.temp_active_menu == 1) display.print(" 1/8");
      else if (seq.temp_active_menu == 2) display.print("1/16");
    }
  }
}

void seq_sub_pattern_transpose ()
{
  if (seq.menu == 34)
  { // is in transpose edit
    seq_active_function = 94;
    if ((LCDML.BT_checkDown() && encoderDir[ENC_R].Down()) || (LCDML.BT_checkUp() && encoderDir[ENC_R].Up()))
    {
      if (LCDML.BT_checkDown()) {
        temp_int = constrain(temp_int + 1, -36, 36);
        if (temp_int > 36)temp_int = 36;
      }
      else if (LCDML.BT_checkUp()) {
        temp_int = constrain(temp_int - 1, -36, 36);
        if (temp_int < -36)temp_int = -36;
      }
    }
  }
  if (LCDML.BT_checkEnter())  //handle button presses during menu >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
  {
    if (seq.menu == 34 ) //transpose pattern
    {
      seq.menu = 0;
      seq_active_function = 0;
      activesample = 0;
      temp_int = seq.note_data[seq.active_pattern][0];
      display.setCursor_textGrid(16, 1);
      seq_print_formatted_number(seq.active_pattern, 2);
      seq_refresh_display_play_status();
      seq_printAllSeqSteps();
      display.setCursor_textGrid(2, 1);
      display.print("         ");
      display.setCursor_textGrid(15, 1);
      display.print(" ");
      display.setCursor_textGrid(18, 1);
      display.print(" ");
    }
    if ( seq.menu == 0 && seq_active_function == 0)
    {
      if ( (seq.content_type[seq.active_pattern] == 0 && activesample == NUM_DRUMSET_CONFIG + 5) || (seq.content_type[seq.active_pattern] > 0 && temp_int == 116) )
      { //transpose pattern
        display.setCursor_textGrid(1, 1);
        display.print("Transpose:   [ 00]");
        for (uint8_t i = 0; i < 16; i++)
        {
          seq.data_buffer[i] = seq.note_data[seq.active_pattern][i];
        }
        seq.menu = 34;
        temp_int = 0;
        seq.temp_select_menu = 0;
        seq.temp_active_menu = 0;
      }
    }
  }
  //button check end <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<

  if (seq.menu == 34 && seq_active_function == 94)
  { //transpose
    char displayname[4] = {0, 0, 0, 0};
    display.setCursor_textGrid(14, 1);
    display.print("[");
    if (temp_int > 0) {
      display.print("+");
    } else if (temp_int < 0)
    {
      display.print("-");
    }
    else {
      display.print(" ");
    }
    sprintf(displayname, "%02d", abs(temp_int));
    display.print( displayname);
    display.print("]");
    for (uint8_t i = 0; i < 16; i++)
    {
      if (seq.content_type[seq.active_pattern] == 0) { //drums
        if (seq.data_buffer[i] != 0 && seq.data_buffer[i] + temp_int >= 0 && seq.data_buffer[i] + temp_int < 254
            && seq.vel[seq.active_pattern][i] >= 210 ) // pitched drums only
          seq.note_data[seq.active_pattern][i] = seq.data_buffer[i] + temp_int;
      }
      else
        //instruments
        if (seq.content_type[seq.active_pattern] > 0) {
          if (seq.data_buffer[i] != 0 && seq.data_buffer[i] != 130 && seq.data_buffer[i] + temp_int > 0 && seq.data_buffer[i] + temp_int < 254)
            seq.note_data[seq.active_pattern][i] = seq.data_buffer[i] + temp_int;
        }
    }
    seq_printAllSeqSteps();
    print_track_steps_detailed(CHAR_width, CHAR_height * 5 + 3, 254, false, true);
  }
}

void seq_sub_clear_pattern_or_clear_all ()
{
  if (LCDML.BT_checkEnter())  //handle button presses during menu >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
  {
    if ( seq.menu == 0 && seq_active_function == 0)
    {
      if ( (seq.content_type[seq.active_pattern] == 0 && activesample == NUM_DRUMSET_CONFIG + 1) ||
           (seq.content_type[seq.active_pattern] > 0 && temp_int == 112) )
      { //clear all patterns
        seq_clear_all_patterns();
        seq_printAllSeqSteps();
      }
      else if ( (seq.content_type[seq.active_pattern] == 0 && activesample == NUM_DRUMSET_CONFIG) ||
                (seq.content_type[seq.active_pattern] > 0 && temp_int == 111) )
      { //clear pattern
        seq_clear_active_pattern();
        seq_printAllSeqSteps();
      }
      seq_active_function = 99;
    }
  }
  //button check end <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
}

void seq_sub_pitch_edit_pitched_sample ()
{
  if (seq_active_function == 40 )
  { // pitch edit sample
    if ((LCDML.BT_checkDown() && encoderDir[ENC_R].Down()) || (LCDML.BT_checkUp() && encoderDir[ENC_R].Up()))
    {
      if (LCDML.BT_checkDown()) {
        seq.note_data[seq.active_pattern][seq.menu - 3] = constrain(seq.note_data[seq.active_pattern][seq.menu - 3] + 1, 12, 108);
      }
      else if (LCDML.BT_checkUp()) {
        seq.note_data[seq.active_pattern][seq.menu - 3] = constrain(seq.note_data[seq.active_pattern][seq.menu - 3] - 1, 12, 108);
      }
    }
  }
  if (LCDML.BT_checkEnter())  //handle button presses during menu >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
  {
    if (seq.menu > 2 && seq.menu < 19 && seq.vel[seq.active_pattern][seq.menu - 3] > 209
        && seq_active_function != 40 && activesample != NUM_DRUMSET_CONFIG - 1) //enter edit pitch of sample mode
    {
      seq_active_function = 40;
    } else if ( seq_active_function == 40 ) // get out of pitch edit for samples
    {
      if (seq.note_data[seq.active_pattern][seq.menu - 3] == 12)
      {
        seq.note_data[seq.active_pattern][seq.menu - 3] = 0;
        seq.vel[seq.active_pattern][seq.menu - 3] = 0;
      }
      display.setCursor_textGrid(11, 1);
      display.print(" ");
      seq_active_function = 0;
      activesample = 0;
      seq_refresh_display_play_status();
      seq_printAllSeqSteps();
    }
  }
  //button check end <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<

  if (seq.vel[seq.active_pattern][seq.menu - 3] > 209 && activesample != NUM_DRUMSET_CONFIG - 1
      && seq_active_function == 40) //is pitched sample and selected item is not set to EMPTY
  {
    display.setTextSize(2);
    display.setCursor_textGrid(1, 1);
    if (seq.note_data[seq.active_pattern][seq.menu - 3] != 12)
    {
      display.setTextColor(WHITE, BLACK);
      display.print("[EDIT ");
      //seq_print_current_note_from_step(seq.menu - 3);
      display.print(noteNames[seq.note_data[seq.active_pattern][seq.menu - 3] % 12 ][0] );
      if (noteNames[seq.note_data[seq.active_pattern][seq.menu - 3] % 12 ][1] != '\0' ) {
        display.print(noteNames[seq.note_data[seq.active_pattern][seq.menu - 3] % 12 ][1] );
      }
      display.print( (seq.note_data[seq.active_pattern][seq.menu - 3] / 12) - 1);
      display.print(" ");
      display.setCursor_textGrid(10, 1);
      display.print("?]");
    }
    else
    {
      display.setTextColor(WHITE, BLACK);
      display.print("[  ");
      display.setTextColor(RED, BLACK);
      display.print("CLEAR");
      display.setTextColor(WHITE, BLACK);
      display.print("  ]");
      display.setTextColor(WHITE, BLACK);
    }
    display.setTextSize(1);
    display.setCursor(1 * CHAR_width, 4 * CHAR_height + 17);
    display.setTextSize(1);
    display.setTextColor(WHITE, BLACK);
    display.print("SELECT [ ");
    display.setTextColor(RED, BLACK);
    display.print("C0");
    display.setTextColor(WHITE, BLACK);
    display.print(" ] TO CLEAR STEP!  ");
    display.setTextSize(2);
  }
}

void print_current_sample_and_pitch_buffer()
{
  display.setCursor(CHAR_width * 21, CHAR_height * 5 + 23);
  display.setTextSize(1);
  display.setTextColor(DX_PURPLE, BLACK);
  display.print("SAMPLE BUFFER: ");
  display.setTextColor(WHITE, BLACK);
  if (activesample < NUM_DRUMSET_CONFIG - 1 && seq.vel[seq.active_pattern][seq.menu - 3] < 210) //normal sample
  {
    display.print( basename(drum_config[activesample].name) );
    fill_up_with_spaces_right_window();
  }
  else if (activesample < NUM_DRUMSET_CONFIG - 1 && seq.vel[seq.active_pattern][seq.menu - 3] > 209) //pitched sample
  {
    display.print( basename(drum_config[ seq.vel[seq.active_pattern][seq.menu - 3] - 210  ].name));
    fill_up_with_spaces_right_window();
  }
  display.setCursor(CHAR_width * 21, CHAR_height * 5 + 23 + CHAR_height - 5);
  display.setTextColor(DX_PURPLE);
  display.print("SAMPLE TYPE: ");
  display.setTextColor(GREY1, BLACK);
  print_sample_type();
  display.setTextColor(DX_PURPLE, BLACK);
  display.setCursor(CHAR_width * 21, CHAR_height * 5 + 23 + 2 * CHAR_height - 10);
  display.print("NOTE BUFFER: ");
  display.setTextColor(WHITE, BLACK);
  display.print(noteNames[temp_int % 12 ]);
  display.print( (temp_int / 12) - 1);
  display.print(" ");

  display.setTextSize(2);
}

void UI_func_seq_pat_chain(uint8_t param)
{
  if (LCDML.FUNC_setup())         // ****** SETUP *********
  {
    //setup function
    seq.temp_select_menu = 0;
    seq.temp_active_menu = 99;
    seq.menu = 0;
    display.setCursor_textGrid( 1, 1);
    display.print("ChainStep:");
    display.setCursor_textGrid( 14, 1);
    display.print(seq.chain_active_step + 1);
    display.setCursor_textGrid( 15, 1);
    display.print("/");
    display.setCursor_textGrid( 16, 1);
    display.print(seq.chain_lenght + 1);
    display.setCursor_textGrid( 1, 2);
    display.print("T");
    display.setCursor_textGrid(11 , 2);
    display.print("=P");
  }
  if (LCDML.FUNC_loop())          // ****** LOOP *********
  {
    if (seq.temp_active_menu == 99) {
      if ((LCDML.BT_checkDown() && encoderDir[ENC_R].Down()) || (LCDML.BT_checkUp() && encoderDir[ENC_R].Up()))
      {
        if (LCDML.BT_checkDown())
          seq.menu = constrain(seq.menu + 1, 0, 2); //menu select
        else if (LCDML.BT_checkUp())
          seq.menu = constrain(seq.menu - 1, 0, 2);
      }
    }
    else if (seq.temp_active_menu == 90) {
      if ((LCDML.BT_checkDown() && encoderDir[ENC_R].Down()) || (LCDML.BT_checkUp() && encoderDir[ENC_R].Up()))
      {
        if (LCDML.BT_checkDown())
          seq.temp_select_menu = constrain(seq.temp_select_menu + 1, 0, NUM_SEQ_TRACKS - 1);
        else if (LCDML.BT_checkUp())
          seq.temp_select_menu = constrain(seq.temp_select_menu - 1, 0, NUM_SEQ_TRACKS - 1);
      }
    } else if (seq.temp_active_menu == 20) {
      if ((LCDML.BT_checkDown() && encoderDir[ENC_R].Down()) || (LCDML.BT_checkUp() && encoderDir[ENC_R].Up()))
      {
        if (LCDML.BT_checkDown())
          seq.chain_active_step = constrain(seq.chain_active_step + 1, 0, seq.chain_lenght);
        else if (LCDML.BT_checkUp())
          seq.chain_active_step = constrain(seq.chain_active_step - 1, 0, seq.chain_lenght);
      }
    }
    else if (seq.temp_active_menu == 18) {
      if ((LCDML.BT_checkDown() && encoderDir[ENC_R].Down()) || (LCDML.BT_checkUp() && encoderDir[ENC_R].Up()))
      {
        if (LCDML.BT_checkDown())
        {
          seq.patternchain[seq.chain_active_step][seq.temp_select_menu] = constrain(seq.patternchain[seq.chain_active_step][seq.temp_select_menu] + 1, 0, NUM_SEQ_PATTERN );
        }
        else if (LCDML.BT_checkUp())
        {
          seq.patternchain[seq.chain_active_step][seq.temp_select_menu] = constrain(seq.patternchain[seq.chain_active_step][seq.temp_select_menu] - 1, 0, NUM_SEQ_PATTERN );
        }
      }
    }
    if (LCDML.BT_checkEnter())  //handle button presses during menu >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
    {
      if (seq.temp_active_menu == 99 && seq.menu == 0 )
      {
        seq.temp_active_menu = 20;
      }
      else if (seq.temp_active_menu == 99 && seq.menu == 1 )
      {
        seq.temp_active_menu = 90;
      }
      else if (seq.temp_active_menu == 99 && seq.menu == 2 )
      {
        seq.temp_active_menu = 18;
      }
      else
        seq.temp_active_menu = 99;
    }
    if (seq.temp_active_menu == 99)
    {
      if (seq.menu == 0)
      {
        display.setCursor_textGrid( 2, 2);
        display.print(" ");
        display.setCursor_textGrid( 4, 2);
        display.print(" ");
        display.setCursor_textGrid( 13, 1);
        display.print("[");
        display.setCursor_textGrid( 17, 1);
        display.print("]");
      }
      else if (seq.menu == 1)
      {
        display.setCursor_textGrid( 13, 1);
        display.print(" ");
        display.setCursor_textGrid( 17, 1);
        display.print(" ");

        display.setCursor_textGrid( 2, 2);
        display.print("[");
        display.setCursor_textGrid( 4, 2);
        display.print("]");
        display.setCursor_textGrid( 13, 2);
        display.print(" ");
        display.setCursor_textGrid( 16, 2);
        display.print(" ");
      }
      else if (seq.menu == 2)
      {
        display.setCursor_textGrid( 13, 1);
        display.print(" ");
        display.setCursor_textGrid( 17, 1);
        display.print(" ");
        display.setCursor_textGrid( 2, 2);
        display.print(" ");
        display.setCursor_textGrid( 4, 2);
        display.print(" ");

        display.setCursor_textGrid( 13, 2);
        display.print("[");
        display.setCursor_textGrid( 16, 2);
        display.print("]");
      }
    }
    display.setCursor_textGrid( 14, 1);
    display.print(seq.chain_active_step + 1);
    display.setCursor_textGrid(3 , 2);
    display.print( seq.temp_select_menu + 1);
    display.setCursor_textGrid(5 , 2);
    if (seq.track_type[seq.temp_select_menu] == 0 ) display.print("(Drum)");
    else if (seq.track_type[seq.temp_select_menu] == 1 ) display.print("(Inst)");
    else if (seq.track_type[seq.temp_select_menu] == 2 ) display.print("(Chrd)"); else display.print("(Arp.)");
    display.setCursor_textGrid( 14, 2);
    if (seq.patternchain[seq.chain_active_step][seq.temp_select_menu] == NUM_SEQ_PATTERN)display.print("--"); //empty pattern
    else
    {
      if (seq.patternchain[seq.chain_active_step][seq.temp_select_menu] < 10)
        display.print("0");
      display.print(seq.patternchain[seq.chain_active_step][seq.temp_select_menu] );
    }
  }
  if (LCDML.FUNC_close())     // ****** STABLE END *********
  {
    encoderDir[ENC_R].reset();
    seq.menu = 0;
    seq_active_function = 99;
    display.setTextSize(2);
    display.fillScreen(BLACK);
  }
}

void set_sample_type_color()
{
  if ( drum_config[activesample].midinote > 209 )
    display.setTextColor(BLACK, DX_CYAN);
  else
    display.setTextColor(BLACK, DX_ORANGE);
}

void set_sample_type_color_of(uint8_t samplekey)
{
  if ( samplekey > 209 )
    display.setTextColor(BLACK, DX_CYAN);
  else
    display.setTextColor(BLACK, DX_ORANGE);
}

void UI_func_seq_pattern_editor(uint8_t param)
{
  if (LCDML.FUNC_setup())         // ****** SETUP *********
  {
    // setup function
    seq.menu = 3;
    if (seq.menu_status != 2)
    {
      display.fillScreen(BLACK);
      border1();
      border2();
      border3();
      border4();
      display.setCursor(CHAR_width, CHAR_height * 4 + 3);
      display.setTextSize(1);
      display.setTextColor(GREY2, BLACK);
      display.print("CONT.TYPE:");
      display.setCursor(CHAR_width * 6, CHAR_height * 4 + 3);
      print_content_type ();
    }
    print_edit_mode();
    seq_sub_pat_chain(CHAR_width * 21, CHAR_height * 9, true);
    display.setTextSize(2);
    if (seq.content_type[seq.active_pattern] == 0) //Drum Mode
    {
      print_track_steps_detailed(CHAR_width, CHAR_height * 5 + 3, 254, true, true);
      seq.note_editor_view = 0;
    }
    else
    {
      print_single_pattern_pianoroll(CHAR_width, 316, seq.active_pattern, seq.menu - 3, true);
      seq.note_editor_view = 1;
    }
    check_variable_samples_basespeed();
    temp_int = seq.note_data[seq.active_pattern][0];
    encoderDir[ENC_R].reset();
    seq.note_in = 0;
    display.setTextSize(2);
    seq_refresh_display_play_status();
    display.setCursor_textGrid(16, 1);
    display.setTextColor(GREY1, BLACK);
    seq_print_formatted_number(seq.active_pattern, 2);
    print_current_sample_and_pitch_buffer();
    if (seq.menu_status != 2)
    {
      seq_printAllSeqSteps();
      seq_printVelGraphBar();
    }
  }
  if (LCDML.FUNC_loop())          // ****** LOOP *********
  {
    seq_sub_pitch_edit_pitched_sample();
    seq_sub_pattern_fill ();
    seq_sub_copy_swap();
    seq_sub_pattern_transpose();

    if (seq_active_function == 99 )
    {
      if ((LCDML.BT_checkDown() && encoderDir[ENC_R].Down()) || (LCDML.BT_checkUp() && encoderDir[ENC_R].Up()))
      {
        if (LCDML.BT_checkDown())
        {
          if (seq.menu == 18)
          {
            seq.menu_status = 1;
            LCDML.OTHER_jumpToFunc(UI_func_seq_vel_editor);
          }
          else
          {
            seq.menu_status = 0;
            seq.menu = constrain(seq.menu + 1, 0, 18);
          }
        }
        else if (LCDML.BT_checkUp())
          seq.menu = constrain(seq.menu - 1, 0, 18);
      }
    }
    else if (seq_active_function == 0 ) {
      if (seq.content_type[seq.active_pattern] == 0) // is in Drumedit mode
      {
        if ((LCDML.BT_checkDown() && encoderDir[ENC_R].Down()) || (LCDML.BT_checkUp() && encoderDir[ENC_R].Up()))
        {
          if (LCDML.BT_checkDown())
            activesample = constrain(activesample + 1, 0, NUM_DRUMSET_CONFIG + 5 );
          else if (LCDML.BT_checkUp())
            activesample = constrain(activesample - 1, 0, NUM_DRUMSET_CONFIG + 5 );
        }
      }
      else //is in Instrument Mode
      {
        if ((LCDML.BT_checkDown() && encoderDir[ENC_R].Down()) || (LCDML.BT_checkUp() && encoderDir[ENC_R].Up()))
        {
          if (LCDML.BT_checkDown())
            temp_int = constrain(temp_int + 1, 0, 116 );
          else if (LCDML.BT_checkUp())
            temp_int = constrain(temp_int - 1, 0, 116 );
        }
      }
    } else if (seq_active_function == 2)
    {
      if ((LCDML.BT_checkDown() && encoderDir[ENC_R].Down()) || (LCDML.BT_checkUp() && encoderDir[ENC_R].Up()))
      {
        if (LCDML.BT_checkDown())
          seq.active_pattern = constrain(seq.active_pattern + 1, 0, NUM_SEQ_PATTERN - 1);
        else if (LCDML.BT_checkUp())
          seq.active_pattern = constrain(seq.active_pattern - 1, 0, NUM_SEQ_PATTERN - 1);

        display.setTextSize(1);
        display.setCursor(CHAR_width * 6, CHAR_height * 4 + 3);
        print_content_type ();
      }
    }
    if (LCDML.BT_checkEnter())  //handle button presses during menu >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
    {
      if ( seq.menu == 0 && seq_active_function == 99)
      {
        seq_active_function = 0;
      }
      else if ( seq.menu == 0 && seq_active_function == 0)
      {
        seq_sub_clear_pattern_or_clear_all ();
      }
      if ( seq.menu == 1 && seq_active_function != 40)
      {
        if (seq.running == false && seq.recording == false)
        {
          handleStart();
        }
        else if (seq.running == true && seq.recording == false)
        {
          seq.running = true;
          seq.recording = true;
          seq.note_in = 0;
        }
        else if (seq.running == true && seq.recording == true)
        {
          handleStop();
        }
      }
      else if ( seq.menu == 2 && seq_active_function != 40)
      {
        if (seq_active_function != 2)
          seq_active_function = 2;
        else
          seq_active_function = 99;

        if (seq.content_type[seq.active_pattern] == 0)  // drumtrack
        {
          if (activesample < NUM_DRUMSET_CONFIG - 1 && seq.vel[seq.active_pattern][seq.menu - 3] < 210) //normal sample
          {
            //display.show(1, 2, 9, basename(drum_config[activesample].name));
            ;
          }
          else
          {
            if (activesample < NUM_DRUMSET_CONFIG - 1 && seq.vel[seq.active_pattern][seq.menu - 3] > 209) //pitched sample
              // display.show(1, 2, 9, basename(drum_config[ seq.vel[seq.active_pattern][seq.menu - 3]-210  ].name));
              ;
          }
        }
        else
        {
          //          if (temp_int < 109)
          //          {
          //            display.setCursor_textGrid(3, 1);
          //            display.print("    ");
          //            display.setCursor_textGrid(1, 1);
          //            display.print(noteNames[temp_int % 12 ]);
          //            display.print( (temp_int / 12) - 1);
          //          }
          ;
        }
      }
      else if (seq.menu > 2 && seq.menu < 30 && seq_active_function != 40)
      {
        if (seq_active_function == 99)
        {
          if (seq.content_type[seq.active_pattern] == 0)
          { //Drumtrack

            if ( drum_config[activesample].midinote > 209 ) //it selected sample is a pitched sample
            {
              //Step is empty - put in selected pitched sample with selected note value
              if (seq.note_data[seq.active_pattern][seq.menu - 3] == 0)
              {
                seq.vel[seq.active_pattern][seq.menu - 3] = drum_config[activesample].midinote;
                if (temp_int > 12)
                  seq.note_data[seq.active_pattern][seq.menu - 3] = temp_int;
                else
                  seq.note_data[seq.active_pattern][seq.menu - 3] = 12;  //else insert C2
              }
              else
              { //step is an other sample, replace with selected pitched sample
                seq.vel[seq.active_pattern][seq.menu - 3] = drum_config[activesample].midinote;
              }
            }
            else

              if ( drum_config[activesample].midinote < 210 )
              {
                //normal sample: check if note is already there, if not -> insert it, else remove it from grid.
                if (seq.note_data[seq.active_pattern][seq.menu - 3] == drum_config[activesample].midinote)
                {
                  seq.note_data[seq.active_pattern][seq.menu - 3] = 0;
                  seq.vel[seq.active_pattern][seq.menu - 3] = 0;
                } else
                {
                  seq.note_data[seq.active_pattern][seq.menu - 3] = drum_config[activesample].midinote;
                  seq.vel[seq.active_pattern][seq.menu - 3] = 120;
                }
              }
          }
          else
          { //Inst. Track
            if (temp_int == 109 || seq.note_data[seq.active_pattern][seq.menu - 3] == temp_int )
            { //clear note
              seq.note_data[seq.active_pattern][seq.menu - 3] = 0;
              seq.vel[seq.active_pattern][seq.menu - 3] = 0;
            }
            else if (temp_int == 110)
            { //latch note
              seq.note_data[seq.active_pattern][seq.menu - 3] = 130;
              //seq.vel[seq.active_pattern][seq.menu - 3] = 0;
            }
            else
            {
              if (seq.note_data[seq.active_pattern][seq.menu - 3] != temp_int)
              {

                seq.note_data[seq.active_pattern][seq.menu - 3] = temp_int;
                seq.vel[seq.active_pattern][seq.menu - 3] = 120;
              }
            }
          }
        }
        else
          seq_active_function = 99;
        display.setTextSize(2);
        seq_printAllSeqSteps();
        seq_printVelGraphBar();
      }
    }
    //button check end <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<

    display.setTextSize(2);
    if (seq.menu == 0)  //sound select menu
    {
      print_current_sample_and_pitch_buffer();
      display.setTextColor(WHITE, BLACK);
      display.setCursor_textGrid(12, 1);
      display.print(" ");
      display.setCursor_textGrid(14, 1);
      display.print(" ");
      display.setCursor_textGrid(1, 1);
      display.print("[");
      if (seq.content_type[seq.active_pattern] == 0) //Drum Mode
      {
        UI_draw_waveform(activesample);
        if (activesample < NUM_DRUMSET_CONFIG - 1)
        {
          if (drum_config[activesample].midinote < 210)
            display.setTextColor(DX_ORANGE, BLACK);
          else
            display.setTextColor(DX_CYAN, BLACK);

          display.show(1, 2, 9, basename(drum_config[activesample].name));
          display.setTextColor(WHITE, BLACK);
        } else if (activesample == NUM_DRUMSET_CONFIG - 1) {
          display.setCursor_textGrid(2, 1);
          display.print("EMPTY    ");
        } else if (activesample == NUM_DRUMSET_CONFIG ) {
          display.setCursor_textGrid(2, 1);
          display.print("Clear Pat");
        } else if (activesample == NUM_DRUMSET_CONFIG + 1) {
          display.setCursor_textGrid(2, 1);
          display.print("Clear All");
        }
        else if (activesample == NUM_DRUMSET_CONFIG + 2) {
          display.setCursor_textGrid(2, 1);
          display.print("Copy Pat.");
        }
        else if (activesample == NUM_DRUMSET_CONFIG + 3) {
          display.setCursor_textGrid(2, 1);
          display.print("Swap Pat");
        }
        else if (activesample == NUM_DRUMSET_CONFIG + 4) {
          display.setCursor_textGrid(2, 1);
          display.print("Fill Pat.");
        }
        else if (activesample == NUM_DRUMSET_CONFIG + 5) {
          display.setCursor_textGrid(2, 1);
          display.print("Transpose");
        }
        display.setCursor_textGrid(11, 1);
        display.print("]");
      } else //Inst. Mode
      {
        if (temp_int < 109)
        {
          display.setCursor_textGrid(3, 1);
          display.print("    ");
          display.setCursor_textGrid(2, 1);
          if (seq.content_type[seq.active_pattern] == 1) //Inst
          {
            display.print(noteNames[temp_int % 12 ]);
            display.print( (temp_int / 12) - 1);
          }
          else if (seq.content_type[seq.active_pattern] == 2) //Chord
          {
            print_chord_name(temp_int - 200);
          }
        } else if (temp_int == 109)
        {
          display.setCursor_textGrid(2, 1);
          display.print("EMPTY    ");
        } else if (temp_int == 110)
        {
          display.setCursor_textGrid(2, 1);
          display.print("LATCH    ");
        } else if (temp_int == 111)
        {
          display.setCursor_textGrid(2, 1);
          display.print("ClearPat.");
        } else if (temp_int == 112)
        {
          display.setCursor_textGrid(2, 1);
          display.print("Clear All");
        } else if (temp_int == 113)
        {
          display.setCursor_textGrid(2, 1);
          display.print("Copy Pat.");
        }  else if (temp_int == 114)
        {
          display.setCursor_textGrid(2, 1);
          display.print("Swap Pat.");
        } else if (temp_int == 115)
        {
          display.setCursor_textGrid(2, 1);
          display.print("Fill Pat.");
        } else if (temp_int == 116)
        {
          display.setCursor_textGrid(2, 1);
          display.print("Transpose");
        }
        display.setCursor_textGrid(11, 1);
        display.print("]");
      }
    }
    else if (seq.menu == 1)
    {
      display.setTextColor(GREY1, BLACK);
      display.setCursor_textGrid(16, 1);
      seq_print_formatted_number(seq.active_pattern, 2);
      display.setTextColor(WHITE, BLACK);
      display.setCursor_textGrid(1, 1);
      display.print(" ");
      display.setCursor_textGrid(11, 1);
      display.print(" ");
      display.setCursor_textGrid(15, 1);
      display.print(" ");
      display.setCursor_textGrid(18, 1);
      display.print(" ");
      display.setCursor_textGrid(12, 1);
      display.print("[");
      display.setCursor_textGrid(14, 1);
      display.print("]");
      seq_refresh_display_play_status();
      if (activesample < NUM_DRUMSET_CONFIG - 1 && seq.content_type[seq.active_pattern] == 0)
      {
        if (drum_config[activesample].midinote < 210)
          display.setTextColor(DX_ORANGE, BLACK);
        else
          display.setTextColor(DX_CYAN, BLACK);

        display.show(1, 2, 9, basename(drum_config[activesample].name));
        display.setTextColor(WHITE, BLACK);
      }
    }
    if (seq.menu == 2)
    {
      display.fillRect(1 * CHAR_width, 4 * CHAR_height + 17, 222, 8, BLACK);
      display.setTextColor(WHITE, BLACK);
      display.setTextSize(2);
      display.setCursor_textGrid(12, 1);
      display.print(" ");
      display.setCursor_textGrid(14, 1);
      display.print(" ");
      display.setCursor_textGrid(15, 1);
      display.print("[");
      display.setCursor_textGrid(16, 1);
      seq_print_formatted_number(seq.active_pattern, 2);
      display.setCursor_textGrid(18, 1);
      display.print("]");
      if (seq.content_type[seq.active_pattern] == 0) //Drum Mode
      {
        display.setCursor_textGrid(1, 1);  // Print current sample name when switching track and track is drum track
        display.print(" ");
        if (drum_config[activesample].midinote < 210)
          display.setTextColor(DX_ORANGE, BLACK);
        else
          display.setTextColor(DX_CYAN, BLACK);
        display.show(1, 2, 9, basename(drum_config[activesample].name));
        display.setTextColor(WHITE, BLACK);
        display.print("  ");
        if (seq.note_editor_view != 0)
        {
          seq.note_editor_view = 0;
          border3_clear();
          print_track_steps_detailed(CHAR_width, CHAR_height * 5 + 3, 254, true, true);
        }
        else
          print_track_steps_detailed(CHAR_width, CHAR_height * 5 + 3, 254, false, true);
      }
      else
      { // Print note buffer when switching track and track is an instrument track
        display.setCursor_textGrid(1, 1);
        display.print(" ");
        display.setTextColor(LIGHTBLUE, BLACK);
        display.print(noteNames[temp_int % 12 ][0] );
        if (noteNames[temp_int % 12 ][1] != '\0' )
        {
          display.print(noteNames[temp_int % 12 ][1] );
        }
        display.print( (temp_int / 12) - 1);
        display.print("       ");
        display.setTextColor(WHITE, BLACK);

        if (seq.note_editor_view != 1)
        {
          seq.note_editor_view = 1;
          border3_clear();
          print_single_pattern_pianoroll(CHAR_width, 316, seq.active_pattern, seq.menu - 3, true);
        }
        else
          print_single_pattern_pianoroll(CHAR_width, 316, seq.active_pattern, seq.menu - 3, false);
      }
      display.setTextSize(2);
      seq_printAllSeqSteps();
      seq_printVelGraphBar();
      print_current_sample_and_pitch_buffer();
      seq_sub_pat_chain(CHAR_width * 21, CHAR_height * 9, false);
    }
    if (seq.menu == 3)
    {
      display.setTextColor(GREY1, BLACK);
      display.setCursor_textGrid(16, 1);
      seq_print_formatted_number(seq.active_pattern, 2);

      display.setCursor_textGrid(15, 1);
      display.print(" ");
      display.setCursor_textGrid(18, 1);
      display.print(" ");
    }
    if (seq.menu > 2 && seq.menu < 19 && seq_active_function != 40)
    {
      if (seq.content_type[seq.active_pattern] == 0) //Drum Mode
        print_track_steps_detailed(CHAR_width, CHAR_height * 5 + 3, seq.menu - 3, false, false);
      else
        print_single_pattern_pianoroll(CHAR_width, 316, seq.active_pattern, seq.menu - 3, false);
      if (seq.menu == 3)
        display.setCursor_textGrid(1, 2);
      else
        display.setCursor_textGrid(seq.menu - 2, 2);
      display.setTextColor(GREEN, DX_PURPLE);
      display.print(seq_find_shortname(seq.menu - 3)[0]);
      if (seq.menu > 3)
      {
        display.setCursor_textGrid(seq.menu - 3, 2);
        if (seq.vel[seq.active_pattern][seq.menu - 4] > 209) //if pitched sample, change color
          display.setTextColor(DX_CYAN, BLACK);
        else
          set_pattern_content_type_color(seq.active_pattern);
        display.print(seq_find_shortname(seq.menu - 4)[0]);
      }
      if (seq.menu < 18)
      {
        display.setCursor_textGrid(seq.menu - 1, 2);
        if (seq.vel[seq.active_pattern][seq.menu - 2] > 209) //if pitched sample, change color
          display.setTextColor(DX_CYAN, BLACK);
        else
          set_pattern_content_type_color(seq.active_pattern);
        display.print(seq_find_shortname(seq.menu - 2)[0]);
      }
      if (seq.content_type[seq.active_pattern] > 0 && seq.vel[seq.active_pattern][seq.menu - 3] < 210
          && seq.note_data[seq.active_pattern][seq.menu - 3] != 0) //is not Drum Mode and not empty, print note
      {
        display.setCursor_textGrid(1, 1);
        display.print(" ");
        seq_print_current_note_from_step( seq.menu - 3 );
      }
      else if (seq.content_type[seq.active_pattern] == 0)  // is drum pattern
      {
        //print current sample name on active step if not empty
        if (seq.note_data[seq.active_pattern][seq.menu - 3] > 0)
        {
          display.setCursor_textGrid(1, 1);
          display.print(" ");

          if (activesample < NUM_DRUMSET_CONFIG - 1 && seq.vel[seq.active_pattern][seq.menu - 3] < 210) //normal sample
          {
            display.show(1, 2, 9, seq_find_drum_name_from_note( seq.note_data[seq.active_pattern][seq.menu - 3]) );
            UI_draw_waveform(seq_find_drum_data_from_note  (seq.note_data[seq.active_pattern][seq.menu - 3])  );

            //check if the same note is already there, if so ask to clear it
            if (seq.note_data[seq.active_pattern][seq.menu - 3] == drum_config[activesample].midinote)
            {
              display.setTextColor(WHITE, BLACK);
              display.setCursor(1 * CHAR_width, 4 * CHAR_height + 17);
              display.setTextSize(1);
              display.setTextColor(GREEN, BLACK);
              display.print("CLEAR ");
              set_sample_type_color();
              display.print( basename(drum_config[activesample].name));
              display.setTextColor(GREEN, BLACK);
              display.print(" FROM STEP ");
              display.setTextColor(WHITE, BLACK);
              display.print(seq.menu - 2);
              display.setTextColor(GREEN, BLACK);
              display.print(" ?");
              fill_up_with_spaces_left_window();
              display.setTextColor(WHITE, BLACK);
            }
            //else if there is an other note, ask to replace it
            else if (seq.note_data[seq.active_pattern][seq.menu - 3] != drum_config[activesample].midinote &&
                     seq.note_data[seq.active_pattern][seq.menu - 3] > 0 )
            {
              display.setTextColor(WHITE, BLACK);
              display.setCursor(1 * CHAR_width, 4 * CHAR_height + 17);
              display.setTextSize(1);
              display.setTextColor(GREEN, BLACK);
              display.print("REPLACE ");
              set_sample_type_color_of(seq.note_data[seq.active_pattern][seq.menu - 3]);
              display.print  ( find_long_drum_name_from_note(  seq.note_data[seq.active_pattern][seq.menu - 3])   )   ;
              display.setTextColor(GREEN, BLACK);
              display.print(" WITH ");
              set_sample_type_color();
              display.print( basename(drum_config[activesample].name));
              display.setTextColor(GREEN, BLACK);
              display.print(" ?");
              fill_up_with_spaces_left_window();
              display.setTextColor(WHITE, BLACK);
            }
          }
          else
          {
            if (activesample < NUM_DRUMSET_CONFIG - 1 && seq.vel[seq.active_pattern][seq.menu - 3] > 209) //pitched sample
            {
              display.show(1, 2, 9, basename(drum_config[ seq.vel[seq.active_pattern][seq.menu - 3] - 210  ].name));
              UI_draw_waveform(seq.vel[seq.active_pattern][seq.menu - 3] - 210 );

              if (seq.vel[seq.active_pattern][seq.menu - 3] == drum_config[activesample].midinote)  // pitched sample is idential, ask to edit pitch/delete
              {
                display.setTextColor(WHITE, BLACK);
                display.setCursor(1 * CHAR_width, 4 * CHAR_height + 17);
                display.setTextSize(1);
                display.setTextColor(GREEN, BLACK);
                display.print("EDIT/");
                display.setTextColor(RED, BLACK);
                display.print("DELETE ");
                set_sample_type_color();
                display.print( basename(drum_config[activesample].name));
                display.setTextColor(GREEN, BLACK);
                display.print(" IN STEP ");
                display.setTextColor(WHITE, BLACK);
                display.print(seq.menu - 2);
                display.setTextColor(GREEN, BLACK);
                display.print(" ?");
                display.setTextColor(WHITE, BLACK);
              }
              else if (seq.vel[seq.active_pattern][seq.menu - 3] != drum_config[activesample].midinote) // pitched sample is different, ask to replace
              {
                display.setTextColor(WHITE, BLACK);
                display.setCursor(1 * CHAR_width, 4 * CHAR_height + 17);
                display.setTextSize(1);
                display.setTextColor(GREEN, BLACK);
                display.print("REPLACE ");
                set_sample_type_color_of(seq.vel[seq.active_pattern][seq.menu - 3]);
                display.print( basename(drum_config[ seq.vel[seq.active_pattern][seq.menu - 3] - 210  ].name)  );
                display.setTextColor(GREEN, BLACK);
                display.print(" WITH ");
                set_sample_type_color();
                display.print( basename(drum_config[activesample].name));

                display.setTextColor(GREEN, BLACK);
                display.print(" ?");
                display.setTextColor(WHITE, BLACK);
              }
            }
          }
        }
        else if (seq.note_data[seq.active_pattern][seq.menu - 3] == 0
                 && seq.vel[seq.active_pattern][seq.menu - 3] < 210)  //if step empty, print selected, active sample and ask if should be inserted
        {
          display.setCursor_textGrid(1, 1);
          display.print(" ");
          display.setTextColor(GREY1, BLACK);
          display.show(1, 2, 9, "--EMPTY--");
          display.setTextColor(WHITE, BLACK);
          display.setCursor(1 * CHAR_width, 4 * CHAR_height + 17);
          display.setTextSize(1);
          display.setTextColor(GREEN, BLACK);
          display.print("INSERT ");
          set_sample_type_color();
          display.print( basename(drum_config[activesample].name));
          display.setTextColor(GREEN, BLACK);
          display.print(" IN STEP ");
          display.setTextColor(WHITE, BLACK);
          display.print(seq.menu - 2);
          display.setTextColor(GREEN, BLACK);
          display.print(" ?");
          fill_up_with_spaces_left_window();
          display.setTextColor(WHITE, BLACK);
        }
        else if  (seq_active_function == 40 && activesample != NUM_DRUMSET_CONFIG - 1)
        { //is in pitch edit function 40
          display.setCursor_textGrid(1, 1);
          display.show(1, 2, 4, basename(drum_config[activesample].name));
          display.setCursor_textGrid(7, 1);
          seq_print_current_note_from_step(seq.menu - 3);
        }
      }
    }
    // instr. pitch edit/delete/exchange
    if ( seq.menu > 2 && seq.menu - 3 < 16 && seq.content_type[seq.active_pattern] > 0)
    {
      if (seq.note_data[seq.active_pattern][seq.menu - 3] == 0)  //insert note buffer if step is empty
      {
        display.setCursor(1 * CHAR_width, 4 * CHAR_height + 17);
        display.setTextSize(1);

        display.setTextColor(GREEN, BLACK);
        display.print("INSERT NOTE ");
        display.setTextColor(LIGHTBLUE, BLACK);
        display.print(noteNames[temp_int % 12]  [0] );
        if (noteNames[temp_int % 12 ][1] != '\0' )
        {
          display.print(noteNames[temp_int % 12 ][1] );
        }
        display.print( temp_int / 12 - 1);
        display.setTextColor(GREEN, BLACK);
        display.print(" IN STEP ");
        display.setTextColor(WHITE, BLACK);
        display.print(seq.menu - 2);
        display.setTextColor(GREEN, BLACK);
        display.print(" ?   ");
        display.setTextSize(2);
      }
      else  //change note if different
        if (seq.note_data[seq.active_pattern][seq.menu - 3] > 0 &&
            seq.note_data[seq.active_pattern][seq.menu - 3] != temp_int)
        {
          display.setCursor(1 * CHAR_width, 4 * CHAR_height + 17);
          display.setTextSize(1);
          display.setTextColor(GREEN, BLACK);
          display.print("CHANGE ");
          display.setTextColor(LIGHTBLUE, BLACK);
          display.print(noteNames[seq.note_data[seq.active_pattern][seq.menu - 3] % 12 ][0] );
          if (noteNames[seq.note_data[seq.active_pattern][seq.menu - 3] % 12 ][1] != '\0' ) {
            display.print(noteNames[seq.note_data[seq.active_pattern][seq.menu - 3] % 12 ][1] );
          }
          display.print( (seq.note_data[seq.active_pattern][seq.menu - 3] / 12) - 1);
          display.setTextColor(GREEN, BLACK);
          display.print(" > ");
          display.setTextColor(LIGHTBLUE, BLACK);
          display.print(noteNames[temp_int % 12]  [0] );
          if (noteNames[temp_int % 12 ][1] != '\0' )
          {
            display.print(noteNames[temp_int % 12 ][1] );
          }
          display.print( temp_int / 12 - 1);
          display.setTextColor(GREEN, BLACK);
          display.print(" IN STEP ");
          display.setTextColor(WHITE, BLACK);
          display.print(seq.menu - 2);
          display.setTextColor(GREEN, BLACK);
          display.print(" ? ");
          display.setTextSize(2);
        }
        else if (seq.note_data[seq.active_pattern][seq.menu - 3] == temp_int)  //ask to delete note if it is the same
        {
          display.setCursor(1 * CHAR_width, 4 * CHAR_height + 17);
          display.setTextSize(1);
          display.setTextColor(RED, BLACK);
          display.print("DELETE");
          display.setTextColor(GREEN, BLACK);
          display.print(" NOTE ");
          display.setTextColor(LIGHTBLUE, BLACK);
          display.print(noteNames[seq.note_data[seq.active_pattern][seq.menu - 3] % 12 ][0] );
          if (noteNames[seq.note_data[seq.active_pattern][seq.menu - 3] % 12 ][1] != '\0' )
          {
            display.print(noteNames[seq.note_data[seq.active_pattern][seq.menu - 3] % 12 ][1] );
          }
          display.print( (seq.note_data[seq.active_pattern][seq.menu - 3] / 12) - 1);
          display.setTextColor(GREEN, BLACK);
          display.print(" IN STEP ");
          display.print(seq.menu - 2);
          display.setTextColor(GREEN, BLACK);
          display.print(" ?   ");
          display.setTextSize(2);
        }
    }
  }
  if (LCDML.FUNC_close())     // ****** STABLE END *********
  {
    encoderDir[ENC_R].reset();
    seq.menu = 0;
    seq_active_function = 99;
    display.setTextSize(2);
    if (seq.menu_status != 1) // do not clear screen, jumping to velocity edit
      display.fillScreen(BLACK);
  }
}

void UI_toplineInfoText(uint8_t ypos)
{
  display.fillRect(CHAR_width, ypos * CHAR_height - 1, 38 * CHAR_width, CHAR_height, BLUE);
  display.setTextColor(DX_PURPLE);

  display.setCursor(18 * CHAR_width + 6, CHAR_height * ypos + 4);
  display.setTextSize(1);

  display.print( "PERFORMANCE");
  display.setTextSize(2);
  display.setCursor(24 * CHAR_width + 6, CHAR_height * ypos);
  display.setTextColor(DX_CYAN);
  seq_print_formatted_number(configuration.sys.performance_number, 2);
  display.setTextColor(WHITE);
  display.show(ypos, 28, 11, seq.name);
  display.setTextSize(1);
  display.setCursor(108, CHAR_height + CHAR_height * ypos + 1);
  display.setTextColor(GREY1);
  display.print("COLOR CODES ");
  display.setTextColor(LIGHTBLUE);
  display.print("INSTR ");
  display.setTextColor(DX_ORANGE);
  display.print("DRUM / DRUM TRACK ");
  display.setTextColor(DX_CYAN);
  display.print("PITCHED SAMPLE ");
  display.setTextColor(DX_MAGENTA);
  display.print("CHORD/ARP");

  display.setTextSize(2);
}
void UI_func_seq_tracker(uint8_t param)
{
  if (LCDML.FUNC_setup())         // ****** SETUP *********
  {
    // setup function
    //check_variable_samples_basespeed();
    memset(seq.tracker_data_cache, 254, sizeof(seq.tracker_data_cache));
    memset(seq.tracker_names_cache, 254, sizeof(seq.tracker_names_cache));
    display.fillScreen(BLACK);
    UI_toplineInfoText(1);
    display.setTextSize(2);
    display.setCursor_textGrid(1, 1);
    display.setTextColor(WHITE);
    display.print ("TRACKER");
    display.setTextSize(1);
    for (uint8_t x = 0; x < NUM_SEQ_TRACKS; x++)
    {
      display.setCursor_textGrid(4 + 6 * x, 3);
      display.setCursor(display.getCursorX(), display.getCursorY() + 15);
      display.setTextColor(WHITE, BLACK);
      display.print ("TRACK: ");
      display.print (x + 1);
      display.setCursor_textGrid(4 + 6 * x, 4);
      display.setCursor(display.getCursorX(), display.getCursorY() + 9);
      display.print ("PAT:");
      display.setCursor_textGrid(7 + 6 * x, 4);
      display.setCursor(display.getCursorX(), display.getCursorY() + 9);
      set_pattern_content_type_color( seq.patternchain[seq.chain_active_step][x] );
      seq_print_formatted_number( seq.patternchain[seq.chain_active_step][x], 2);
    }
    display.setTextSize(2);
    seq_print_step_numbers(CHAR_width, 96);
    print_merged_pattern( 4 * CHAR_width , 96,  0);
    print_merged_pattern( 10 * CHAR_width , 96,  1);
    print_merged_pattern( 16 * CHAR_width , 96,  2);
    print_merged_pattern( 22 * CHAR_width , 96,  3);
    print_merged_pattern( 28 * CHAR_width , 96,  4);
    print_merged_pattern( 34 * CHAR_width , 96,  5);
    encoderDir[ENC_R].reset();
  }
  if (LCDML.FUNC_loop())          // ****** LOOP *********
  {
    ;
  }
  if (LCDML.FUNC_close())     // ****** STABLE END *********
  {
    encoderDir[ENC_R].reset();
    display.fillScreen(BLACK);
  }
}

void UI_func_seq_pianoroll(uint8_t param)
{
  if (LCDML.FUNC_setup())         // ****** SETUP *********
  {
    // setup function
    display.fillScreen(BLACK);
    encoderDir[ENC_R].reset();
    display.setTextSize(2);
    display.fillRect(1, 72, 480, 320 - 72, BLACK);
    print_merged_pattern_pianoroll( 1 * CHAR_width , 320 - CHAR_height,  seq.active_track);
  }
  if (LCDML.FUNC_loop())          // ****** LOOP *********
  {
    if (LCDML.BT_checkDown())
    {
      seq.active_track = constrain(seq.active_track + ENCODER[ENC_R].speed(), 0, NUM_SEQ_TRACKS - 1);
    }
    else if (LCDML.BT_checkUp())
    {
      seq.active_track = constrain(seq.active_track - ENCODER[ENC_R].speed(), 0, NUM_SEQ_TRACKS - 1);
    }
    print_merged_pattern_pianoroll( 1 * CHAR_width , 320 - CHAR_height,  seq.active_track);
  }
  if (LCDML.FUNC_close())     // ****** STABLE END *********
  {
    display.fillScreen(BLACK);
    encoderDir[ENC_R].reset();
  }
}

void UI_func_arpeggio(uint8_t param)
{
  if (LCDML.FUNC_setup())         // ****** SETUP *********
  {
    encoderDir[ENC_R].reset();
    seq.temp_select_menu = 0;
    seq.temp_active_menu = 0;
    display.setCursor_textGrid( 1, 1);
    display.print("Len");
    display.setCursor_textGrid(8, 1);
    print_current_chord();
    display.setCursor_textGrid( 1, 2);
    display.print("Style");
    display.setCursor_textGrid( 14, 2);
    display.print("1/16");
    arp_refresh_display_play_status();
  }
  if (LCDML.FUNC_loop())          // ****** LOOP *********
  {
    if ((LCDML.BT_checkDown() && encoderDir[ENC_R].Down()) || (LCDML.BT_checkUp() && encoderDir[ENC_R].Up()) || (LCDML.BT_checkEnter() && encoderDir[ENC_R].ButtonShort()))
    {
      if (seq.temp_active_menu == 0)
      {
        if (LCDML.BT_checkDown())
          seq.temp_select_menu = constrain(seq.temp_select_menu + ENCODER[ENC_R].speed(), 0, 3);
        else if (LCDML.BT_checkUp())
          seq.temp_select_menu = constrain(seq.temp_select_menu - ENCODER[ENC_R].speed(), 0, 3);
      }
      else if (seq.temp_active_menu == 1)  // Octave setting
      {
        if (LCDML.BT_checkDown())
          seq.arp_lenght = constrain(seq.arp_lenght + ENCODER[ENC_R].speed(), 0, 9);
        else if (LCDML.BT_checkUp())
          seq.arp_lenght = constrain(seq.arp_lenght - ENCODER[ENC_R].speed(), 0, 9);
      }
      else if (seq.temp_active_menu == 2)  // Style setting
      {
        if (LCDML.BT_checkDown())
          seq.arp_style = constrain(seq.arp_style + ENCODER[ENC_R].speed(), 0, 3);
        else if (LCDML.BT_checkUp())
          seq.arp_style = constrain(seq.arp_style - ENCODER[ENC_R].speed(), 0, 3);
      }
      else if (seq.temp_active_menu == 3)  // Arp Speed setting
      {
        if (LCDML.BT_checkDown())
          seq.arp_speed = constrain(seq.arp_speed + ENCODER[ENC_R].speed(), 0, 1);
        else if (LCDML.BT_checkUp())
          seq.arp_speed = constrain(seq.arp_speed - ENCODER[ENC_R].speed(), 0, 1);
      }

      if (LCDML.BT_checkEnter())  //handle button presses during menu >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
      {
        if ( seq.temp_select_menu == 0 && seq.temp_active_menu == 0 )
        {
          seq.temp_active_menu = 1;
        }
        else if ( seq.temp_select_menu == 0 && seq.temp_active_menu == 1 )
        {
          seq.temp_active_menu = 0;
        }
        if ( seq.temp_select_menu == 1 && seq.temp_active_menu == 0 )
        {
          seq.temp_active_menu = 2;
        }
        else if ( seq.temp_select_menu == 1 && seq.temp_active_menu == 2 )
        {
          seq.temp_active_menu = 0;
        }
        else if ( seq.temp_select_menu == 2 )
        {
          if (seq.running) {
            seq.running = !seq.running;
            handleStop();
            arp_refresh_display_play_status();
            seq.step = 0;
            seq.arp_octave = 0;
            seq.arp_step = 0;
            seq.chain_active_step = 0;
          } else
          {
            seq.running = !seq.running;
            arp_refresh_display_play_status();
            handleStart();
          }
        }
        else if ( seq.temp_select_menu == 3 && seq.temp_active_menu == 0 )
        {
          seq.temp_active_menu = 3;
        }
        else if ( seq.temp_select_menu == 3 && seq.temp_active_menu == 3 )
        {
          seq.temp_active_menu = 0;
        }
      }
    }
    display.setCursor_textGrid( 5, 1);
    if (seq.arp_lenght == 0)   display.print("A"); else  display.print(seq.arp_lenght); //play all elements or from 1-xx elements
    display.setCursor_textGrid( 7, 2);
    display.print( seq.arp_style_names[seq.arp_style][0] );
    display.print( seq.arp_style_names[seq.arp_style][1] );
    display.print( seq.arp_style_names[seq.arp_style][2] );
    display.setCursor_textGrid( 14, 2);
    if (seq.arp_speed == 0)display.print("1/16"); else if (seq.arp_speed == 1)display.print("1/8 ");
    if (seq.temp_select_menu == 0) {
      display.setCursor_textGrid( 4, 1);
      display.print("[");
      display.setCursor_textGrid( 6, 1);
      display.print("]");
      display.setCursor_textGrid( 6, 2);
      display.print(" ");
      display.setCursor_textGrid( 10, 2);
      display.print(" ");
      display.setCursor_textGrid( 16, 1);
      display.print(" ");
      display.setCursor_textGrid( 18, 1);
      display.print(" ");
      display.setCursor_textGrid( 13, 2);
      display.print(" ");
      display.setCursor_textGrid( 18, 2);
      display.print(" ");
    }
    else if (seq.temp_select_menu == 1)
    {
      display.setCursor_textGrid( 6, 2);
      display.print("[");
      display.setCursor_textGrid( 10, 2);
      display.print("]");
      display.setCursor_textGrid( 4, 1);
      display.print(" ");
      display.setCursor_textGrid( 6, 1);
      display.print(" ");
      display.setCursor_textGrid( 16, 1);
      display.print(" ");
      display.setCursor_textGrid( 18, 1);
      display.print(" ");
    }
    else if (seq.temp_select_menu == 2)
    {
      display.setCursor_textGrid( 6, 2);
      display.print(" ");
      display.setCursor_textGrid( 10, 2);
      display.print(" ");
      display.setCursor_textGrid( 16, 1);
      display.print("[");
      display.setCursor_textGrid( 18, 1);
      display.print("]");
      display.setCursor_textGrid( 13, 2);
      display.print(" ");
      display.setCursor_textGrid( 18, 2);
      display.print(" ");
    }
    else if (seq.temp_select_menu == 3)
    {
      display.setCursor_textGrid( 16, 1);
      display.print(" ");
      display.setCursor_textGrid( 18, 1);
      display.print(" ");
      display.setCursor_textGrid( 13, 2);
      display.print("[");
      display.setCursor_textGrid( 18, 2);
      display.print("]");
      display.setCursor_textGrid( 4, 1);
      display.print(" ");
      display.setCursor_textGrid( 6, 1);
      display.print(" ");
    }
    display.setCursor_textGrid(8, 1);
    print_current_chord();
  }
  if (LCDML.FUNC_close())     // ****** STABLE END *********
  {
    encoderDir[ENC_R].reset();
  }
}

void UI_func_seq_mute_matrix(uint8_t param)
{
  if (LCDML.FUNC_setup())         // ****** SETUP *********
  {
    // setup function
    display.fillScreen(BLACK);
    UI_toplineInfoText(0);
    display.setTextSize(2);
    display.setCursor_textGrid(1, 0);
    display.setTextColor(WHITE);
    display.print ("MUTE MATRIX");

    seq.temp_active_menu = 99;
    seq.temp_select_menu = 1;
    seq.menu = 0;
    temp_int = 0; //y offset

    display.setTextColor(WHITE, BLUE);
  }
  if (LCDML.FUNC_loop())          // ****** LOOP *********
  {
    for (uint8_t y = 0; y < 4; y++)
    {
      for (uint8_t x = 0; x < NUM_SEQ_TRACKS; x++)
      {
        if (seq.patternchain[y][x] < NUM_SEQ_PATTERN)
          display.fillRect( CHAR_width + x * (480 / 6 - 3)  , 2 * CHAR_height + y * (320 / 4 - 7),  68, 62, BLUE);
        else
          display.fillRect( CHAR_width + x * (480 / 6 - 3)  , 2 * CHAR_height + y * (320 / 4 - 7),  68, 62, GREY4);
        display.setCursor(  CHAR_width + x * (480 / 6 - 3) + 3  , 2 * CHAR_height + y * (320 / 4 - 7) + 3  );
        display.setTextSize(2);
        if (seq.patternchain[y][x] < NUM_SEQ_PATTERN)
          display.setTextColor(WHITE, BLUE);
        else
          display.setTextColor(GREY1, GREY4);
        display.print ("P");
        if (seq.patternchain[y][x] < NUM_SEQ_PATTERN || seq.patternchain[y][x] == 99 )
          display.print (seq.patternchain[y][x]);
        else if (seq.patternchain[y][x] != 99)
          display.print ( seq.patternchain[y][x] - (NUM_SEQ_PATTERN + 10) );
        display.print (" ");
        display.setTextSize(1);
        display.setCursor(  CHAR_width + x * (480 / 6 - 3) + 3  , 2 * CHAR_height + y * (320 / 4 - 7) + 51  );
        if (seq.content_type[seq.patternchain[y][x]] == 0) //Drumpattern
          display.setTextColor(DX_ORANGE, BLUE);
        else if (seq.content_type[seq.patternchain[y][x]] == 1) //Instrument Pattern
          display.setTextColor(LIGHTBLUE, BLUE);
        else if (seq.content_type[seq.patternchain[y][x]] == 2 || seq.content_type[seq.patternchain[y][x]] == 3) //  chord or arp pattern
          display.setTextColor(DX_MAGENTA, BLUE);
        if (seq.content_type[seq.patternchain[y][x]] == 0)
          display.print("DRUM");
        else if (seq.content_type[seq.patternchain[y][x]] == 1)
          display.print("INSTR");
        else if (seq.content_type[seq.patternchain[y][x]] == 2  )
          display.print("CHORD/ARP");
      }
    }
  }
  if (LCDML.FUNC_close())     // ****** STABLE END *********
  {
    encoderDir[ENC_R].reset();
    display.fillScreen(BLACK);
    display.setTextSize(2);
  }
}

void UI_func_seq_track_setup(uint8_t param)
{
  if (LCDML.FUNC_setup())         // ****** SETUP *********
  {
    // setup function
    seq.temp_active_menu = 99;
    seq.temp_select_menu = 0;
    display.setCursor_textGrid(1 , 1);
    display.print("Track Setup  ");
  }
  if (LCDML.FUNC_loop())          // ****** LOOP *********
  {
    if (seq.temp_active_menu == 99) {
      if ((LCDML.BT_checkDown() && encoderDir[ENC_R].Down()) || (LCDML.BT_checkUp() && encoderDir[ENC_R].Up()))
      {
        if (LCDML.BT_checkDown())
          seq.temp_select_menu = constrain(seq.temp_select_menu + 1, 0, NUM_SEQ_TRACKS - 1);
        else if (LCDML.BT_checkUp())
          seq.temp_select_menu = constrain(seq.temp_select_menu - 1, 0, NUM_SEQ_TRACKS - 1);
      }
    } else  {
      if ((LCDML.BT_checkDown() && encoderDir[ENC_R].Down()) || (LCDML.BT_checkUp() && encoderDir[ENC_R].Up()))
      {
        if (LCDML.BT_checkDown())
          seq.track_type[seq.temp_active_menu] = constrain(seq.track_type[seq.temp_active_menu] + 1, 0, 3);
        else if (LCDML.BT_checkUp())
          seq.track_type[seq.temp_active_menu] = constrain(seq.track_type[seq.temp_active_menu] - 1, 0, 3);
      }
    }
    if (LCDML.BT_checkEnter())  //handle button presses during menu >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
    {
      if (seq.temp_active_menu == 99) {
        seq.temp_active_menu = seq.temp_select_menu;
      }  else
      {
        seq.temp_active_menu = 99;
      }
    }
    if (seq.temp_active_menu == 99)
    {
      display.setCursor_textGrid(5 , 2);
      display.print(" ");
      display.setCursor_textGrid(12 , 2);
      display.print(" ");
      display.setCursor_textGrid(1 , 2);
      display.print("[");
      display.print(seq.temp_select_menu + 1);
      display.print("]");

    } else
    {
      display.setCursor_textGrid(1 , 2);
      display.print(" ");
      display.setCursor_textGrid(3 , 2);
      display.print(" ");
      display.setCursor_textGrid(5 , 2);
      display.print("[");
      display.setCursor_textGrid(12 , 2);
      display.print("]");

    }
    display.setCursor_textGrid(6 , 2);
    if (seq.track_type[seq.temp_select_menu] == 0 ) display.print("Drums "); else if (seq.track_type[seq.temp_select_menu] == 1 ) display.print("Instru");
    else if (seq.track_type[seq.temp_select_menu] == 2 )display.print("Chords"); else display.print("Arp   ");

  }
  if (LCDML.FUNC_close())     // ****** STABLE END *********
  {
    encoderDir[ENC_R].reset();
  }
}

void UI_func_dexed_assign(uint8_t param)
{
  if (LCDML.FUNC_setup())         // ****** SETUP *********
  {
    // setup function
    seq.temp_active_menu = 99;
    seq.temp_select_menu = 0;
    display.setCursor_textGrid(1 , 1);
    display.print("Dexed Assign");
    display.setCursor_textGrid(1 , 2);
    display.print("Track           ");
  }
  if (LCDML.FUNC_loop())          // ****** LOOP *********
  {
    if (seq.temp_active_menu == 99) {
      if ((LCDML.BT_checkDown() && encoderDir[ENC_R].Down()) || (LCDML.BT_checkUp() && encoderDir[ENC_R].Up()))
      {
        if (LCDML.BT_checkDown())
          seq.temp_select_menu = constrain(seq.temp_select_menu + 1, 0, NUM_SEQ_TRACKS - 1);
        else if (LCDML.BT_checkUp())
          seq.temp_select_menu = constrain(seq.temp_select_menu - 1, 0, NUM_SEQ_TRACKS - 1);
      }
    } else  {
      if ((LCDML.BT_checkDown() && encoderDir[ENC_R].Down()) || (LCDML.BT_checkUp() && encoderDir[ENC_R].Up()))
      {
        if (LCDML.BT_checkDown())
          seq.inst_dexed[seq.temp_active_menu] = constrain(seq.inst_dexed[seq.temp_active_menu] + 1, 0, 1);
        else if (LCDML.BT_checkUp())
          seq.inst_dexed[seq.temp_active_menu] = constrain(seq.inst_dexed[seq.temp_active_menu] - 1, 0, 1);
      }
    }
    if (LCDML.BT_checkEnter())  //handle button presses during menu >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
    {
      if (seq.temp_active_menu == 99) {
        seq.temp_active_menu = seq.temp_select_menu;
      }  else
      {
        seq.temp_active_menu = 99;
      }
    }
    if (seq.temp_active_menu == 99)
    {
      display.setCursor_textGrid(9 , 2);
      display.print(" ");
      display.setCursor_textGrid(16 , 2);
      display.print(" ");
      display.setCursor_textGrid(6 , 2);
      display.print("[");
      display.print(seq.temp_select_menu + 1);
      display.print("]");
    } else
    {
      display.setCursor_textGrid(6 , 2);
      display.print(" ");
      display.setCursor_textGrid(8 , 2);
      display.print(" ");
      display.setCursor_textGrid(9 , 2);
      display.print("[");
      display.setCursor_textGrid(16 , 2);
      display.print("]");
    }
    display.setCursor_textGrid(10 , 2);
    if (seq.inst_dexed[seq.temp_select_menu] == 0 ) display.print("dexed0"); else if (seq.inst_dexed[seq.temp_select_menu] == 1 ) display.print("dexed1"); else display.print("??????");

  }
  if (LCDML.FUNC_close())     // ****** STABLE END *********
  {
    encoderDir[ENC_R].reset();
  }
}

void UI_func_load_performance(uint8_t param)
{
  static uint8_t mode;
  if (LCDML.FUNC_setup())         // ****** SETUP *********
  {
    char tmp[10];
    if (seq.state_last_loadsave != 200)temp_int = seq.state_last_loadsave; else temp_int = param;
    mode = 0;
    encoderDir[ENC_R].reset();
    display.setCursor_textGrid(1, 1);
    display.print(F("Load Performance"));
    display.setCursor_textGrid(1, 2);
    sprintf(tmp, "[%2d]", param);
    display.print(tmp);
  }
  if (LCDML.FUNC_loop())          // ****** LOOP *********
  {
    if ((LCDML.BT_checkDown() && encoderDir[ENC_R].Down()) || (LCDML.BT_checkUp() && encoderDir[ENC_R].Up()) || (LCDML.BT_checkEnter() && encoderDir[ENC_R].ButtonShort()))
    {
      if (LCDML.BT_checkDown())
      {
        temp_int = constrain(temp_int + ENCODER[ENC_L].speed(), 0, 99);
      }
      else if (LCDML.BT_checkUp())
      {
        temp_int = constrain(temp_int - ENCODER[ENC_L].speed(), 0, 99);
      }
      else if (LCDML.BT_checkEnter())
      {
        mode = 0xff;
        display.setCursor_textGrid(1, 2);
        if (load_sd_performance_json(temp_int) == false)
          display.print("Does not exist.");
        else
        {
          load_sd_performance_json(temp_int);
          seq.state_last_loadsave = temp_int;
          display.setCursor_textGrid(1, 2);
          display.print("Done.           ");
        }
        delay(MESSAGE_WAIT_TIME);
        LCDML.FUNC_goBackToMenu();
      }
    }
    display.setCursor_textGrid(1, 2);
    char tmp[10];
    sprintf(tmp, "[%2d] ", temp_int);
    display.print(tmp);
    if (check_sd_performance_exists(temp_int))
    {
      get_sd_performance_name_json(temp_int);
      if ( seq.name_temp[0] != 0 )
        display.show(2, 6, 11, seq.name_temp);
      else
        display.print(" -- DATA --");
    }
    else display.print("           ");
  }
  if (LCDML.FUNC_close())     // ****** STABLE END *********
  {
    if (mode < 0xff)
    {
      display.show(2, 1, 16, "Canceled.");
      delay(MESSAGE_WAIT_TIME);
    }
    else
    {
      configuration.sys.performance_number = temp_int;
      save_sd_sys_json();
      encoderDir[ENC_R].reset();
    }
  }
}

void UI_func_save_performance(uint8_t param)
{
  static bool overwrite;
  static bool yesno;
  static uint8_t mode;
  if (LCDML.FUNC_setup())         // ****** SETUP *********
  {
    char tmp[FILENAME_LEN];
    yesno = false;
    if (seq.state_last_loadsave != 200)
      temp_int = seq.state_last_loadsave;
    else
      temp_int = 0;
    mode = 0;
    encoderDir[ENC_R].reset();
    display.setCursor_textGrid(1, 1);
    display.print(F("Save Performance"));
    display.setCursor_textGrid(1, 2);
    sprintf(tmp, "[%2d] ", temp_int);
    display.print(tmp);

    sprintf(tmp, "/%s/%d/%s.json", PERFORMANCE_CONFIG_PATH, temp_int, SEQUENCER_CONFIG_NAME);
    if (SD.exists(tmp))
      overwrite = true;
    else
      overwrite = false;
    if (check_sd_performance_exists(temp_int))
    {
      get_sd_performance_name_json(temp_int);
      if ( seq.name_temp[0] != 0 )
        display.show(2, 6, 11, seq.name_temp);
      else
        display.print(" -- DATA --");
    }
  }
  if (LCDML.FUNC_loop())          // ****** LOOP *********
  {
    if ((LCDML.BT_checkDown() && encoderDir[ENC_R].Down()) || (LCDML.BT_checkUp() && encoderDir[ENC_R].Up()) || (LCDML.BT_checkEnter() && encoderDir[ENC_R].ButtonShort()))
    {
      if (LCDML.BT_checkDown())
      {
        if (mode == 0)
          temp_int = constrain(temp_int + ENCODER[ENC_L].speed(), 0, 99);
        else
          yesno = true;
      }
      else if (LCDML.BT_checkUp())
      {
        if (mode == 0)
          temp_int = constrain(temp_int - ENCODER[ENC_L].speed(), 0, 99);
        else
          yesno = false;
      }
      else if (LCDML.BT_checkEnter())
      {
        if (mode == 0 && overwrite == true)
        {
          mode = 1;
          display.setCursor_textGrid(1, 2);
          display.print(F("Overwrite: [   ]"));
        }
        else
        {
          mode = 0xff;
          if (overwrite == false || yesno == true)
          {
            if (yesno == true)
            {
              char tmp[FILENAME_LEN];
              sprintf(tmp, "/%s/%d/%s.json", PERFORMANCE_CONFIG_PATH, temp_int, SEQUENCER_CONFIG_NAME);
              SD.remove(tmp);
            }
            save_sd_performance_json(temp_int);
            display.show(2, 1, 16, "Done.");
            seq.state_last_loadsave = temp_int;
            delay(MESSAGE_WAIT_TIME);
            LCDML.FUNC_goBackToMenu();
          }
          else if (overwrite == true && yesno == false)
          {
            char tmp[10];

            mode = 0;
            display.setCursor_textGrid(1, 2);
            sprintf(tmp, "[%2d]   ", temp_int);
            display.print(tmp);
          }
        }
      }
      if (mode == 0)
      {
        char tmp[FILENAME_LEN];
        //sprintf(tmp, "/%s/%d/%s.json", PERFORMANCE_CONFIG_PATH, temp_int, SEQUENCER_CONFIG_NAME);
        //if (SD.exists(tmp))  //don't know why above is not working
        if (check_sd_performance_exists(temp_int))
          overwrite = true;
        else
          overwrite = false;
        display.setCursor_textGrid(1, 2);
        sprintf(tmp, "[%2d] ", temp_int);
        display.print(tmp);
        display.setCursor_textGrid(6, 2);
        if (overwrite == false)
        {
          display.print("-- EMPTY --");
        }
        else if (check_sd_performance_exists(temp_int))
        {
          get_sd_performance_name_json(temp_int);
          if ( seq.name_temp[0] != 0 )
            display.show(2, 6, 11, seq.name_temp);
          else
            display.print("-- DATA --");
        }
        else display.print("          ");
      }
      else
      {
        display.setCursor_textGrid(13, 2);
        if (yesno == true)
          display.print(F("YES"));
        else
          display.print(F("NO "));
      }
    }
    encoderDir[ENC_R].reset();
  }
  if (LCDML.FUNC_close())     // ****** STABLE END *********
  {
    if (mode < 0xff)
    {
      display.show(2, 1, 16, "Canceled.");
      delay(MESSAGE_WAIT_TIME);
    }
    encoderDir[ENC_R].reset();
  }
}

void UI_func_information(uint8_t param)
{
  if (LCDML.FUNC_setup())         // ****** SETUP *********
  {
    char version_string[display_cols + 1];
    encoderDir[ENC_R].reset();
    generate_version_string(version_string, sizeof(version_string));
    // setup function
    display.setCursor_textGrid(1, 1);
    display.print(version_string);
    display.setCursor_textGrid(1, 2);
    display.print(sd_string);
  }

  if (LCDML.FUNC_loop())          // ****** LOOP *********
  {
    ;
  }

  if (LCDML.FUNC_close())     // ****** STABLE END *********
  {
    encoderDir[ENC_R].reset();
  }
}

void sd_printDirectory(File currentDirectory)
{
  fm.is_folder = false;
  fm.cap_rows = 9;
  for (uint8_t f = 0; f < fm.skip_files; f++)
  {
    fm.entry.close();
    fm.entry = currentDirectory.openNextFile();

    if (! fm.entry)  break;
  }
  if (fm.parent_folder)
  {
    fm.is_folder = true;
    strcpy(fm.temp_name, ".." );
    display.setTextColor(WHITE, DX_CYAN);
  }
  else
    display.setTextColor(DX_CYAN, BLACK);

  display.drawBitmap(CHAR_width,  10 * 11 - 1, special_chars[23], 8, 8, YELLOW);
  display.setCursor (CHAR_width * 2,   + 10 * 11  );
  display.print("..");

  for (uint8_t f = 0; f < 10; f++)
  {
    fm.entry =  currentDirectory.openNextFile();
    if (! fm.entry)
    {
      fm.cap_rows = f - 1;
      display.fillRect(10, f * 11   + 11 * 11 - 1 , CHAR_width * 15, (10 - f) * 11, BLACK);
      break;
    }
    if (fm.entry.isDirectory() )
    {
      display.drawBitmap(CHAR_width, f * 11 - 1 + 11 * 11 , special_chars[23], 8, 8, YELLOW);
      if (f == fm.selected_file && fm.parent_folder == false)
        display.setTextColor(BLACK, DX_CYAN);
      else
        display.setTextColor(DX_CYAN, BLACK);
      display.show_smallfont_noGrid( f * 11  + 11 * 11, CHAR_width * 2 , 18, fm.entry.name() );
      display.setCursor (CHAR_width * 12, f * 11  + 11 * 11  );
      display.setTextColor(DARKCYAN, BLACK);
      display.print("DIR    ");
    } else
    {
      display.drawBitmap(CHAR_width, f * 11 - 1 + 11 * 11 , special_chars[23], 8, 8, BLACK);
      if (f == fm.selected_file && fm.parent_folder == false)
        display.setTextColor(BLACK, WHITE);
      else
        display.setTextColor(WHITE, BLACK);
      display.show_smallfont_noGrid(f * 11  + 11 * 11, CHAR_width * 2 , 18, fm.entry.name() );
      display.setTextColor(DX_ORANGE, BLACK);
      display.setCursor (CHAR_width * 12, f * 11  + 11 * 11  );
      display.print(fm.entry.size(), DEC);
      display.print("  ");
    }
    if (f == fm.selected_file && fm.parent_folder == false) strcpy(fm.temp_name, fm.entry.name() );
    if (f == fm.selected_file &&  fm.entry.isDirectory() )
      fm.is_folder = true;
    fm.entry.close();
  }
}

void UI_func_file_manager(uint8_t param)
{
  if (LCDML.FUNC_setup())         // ****** SETUP *********
  {
    AudioInterrupts();
    display.fillScreen(BLACK);
    encoderDir[ENC_R].reset();
    display.setTextSize(2);
    border1();
    border2();
    border3();
    border4();
    display.setCursor_textGrid(1, 1);
    display.print("SD File Manager");
    display.setCursor_textGrid(21, 1);
    display.print("Serial Flash");
    display.setTextSize(1);
    display.setCursor (CHAR_width , 2 * CHAR_height + 5  );
    display.setTextColor(DX_MAGENTA, BLACK);
    display.print("PATH:");
    display.setCursor (CHAR_width, 3 * CHAR_height + 3 );
    display.setTextColor(GREY1);
    display.print("CURRENT FILE/FOLDER :");
    //fm_mode
    display.fillRect (CHAR_width,       240, 100, 25, BLUE);
    display.fillRect (CHAR_width + 114 , 240, 100, 25, BLUE);
    display.fillRect (CHAR_width,       280, 100, 25, BLUE);
    display.fillRect (CHAR_width + 114 , 280, 100, 25, BLUE);
  }
  if (LCDML.FUNC_loop())          // ****** LOOP *********
  {
    if ((LCDML.BT_checkDown() && encoderDir[ENC_R].Down()) || (LCDML.BT_checkUp() && encoderDir[ENC_R].Up()))
    {
      if (LCDML.BT_checkDown())
      {
        if (fm.selected_file == fm.cap_rows && fm.cap_rows > 8 )
          fm.skip_files++;

        if (fm.selected_file == 0 && fm.parent_folder == true)
        {
          fm.is_folder = true;
          fm.selected_file = 0;
          fm.parent_folder = false;
        } else
          fm.selected_file = constrain(fm.selected_file + 1, 0, fm.cap_rows);
      }
      else if (LCDML.BT_checkUp())
      {
        if (fm.selected_file == 0 && fm.skip_files > 0)
          fm.skip_files--;
        else if (fm.selected_file == 0 && fm.skip_files == 0)
        {
          fm.parent_folder = true;

        }
        fm.selected_file = constrain(fm.selected_file - 1, 0, fm.cap_rows);
      }
    }
    if (LCDML.BT_checkEnter())
    {
      if (fm.is_folder)
      {
        if  (fm.temp_name[0] == 0x2E && fm.temp_name[1] == 0x2E ) // return to parent folder
        {
          if  (fm.folder_depth < 2)
          {
            fm.folder_depth = 0;
            fm.skip_files = 0;
            strcpy(fm.new_name, "/");
          }
          else
          {
            // path has at least one parent folder
            for (uint8_t count = strlen(fm.new_name); count > 0; count--)
            {
              if (fm.new_name[count] == 0x2f)
              {
                fm.new_name[count] = '\0';
                break;
              }
            }
            fm.folder_depth--;
          }
        }
        else
        {
          fm.skip_files = 0;
          if (fm.folder_depth > 0)
            strcat(fm.new_name, "/");
          strcat(fm.new_name, fm.temp_name);
          fm.folder_depth++;
        }
        fm.selected_file = 0;
      }
      else
        //is a file
      {
        if (fm.mode == 2) //preview
        {
          strcpy(fm.full_name, fm.new_name);
          strcat(fm.full_name, "/");
          strcat(fm.full_name, fm.temp_name);
          //playWAVFile(fm.full_name);
        }
        else if (fm.mode == 1) //delete file
        {
          strcpy(fm.full_name, fm.new_name);
          strcat(fm.full_name, "/");
          strcat(fm.full_name, fm.temp_name);
          SD.remove(fm.full_name);
        }
      }
    }
    if (fm.new_name[0] != 0x2f)
      fm.new_name[0] = 0x2f;
    AudioNoInterrupts();
    fm.currentDirectoy = SD.open(fm.new_name);
    sd_printDirectory(fm.currentDirectoy);
    AudioInterrupts();
    display.setTextColor(DX_ORANGE, BLACK);
    display.show_smallfont_noGrid(2 * CHAR_height + 5 , CHAR_width * 4 , 31, fm.new_name  );
    if (fm.is_folder)
    {
      display.setCursor (CHAR_width + 13 * 6 , 3 * CHAR_height + 3 );
      display.setTextColor(GREEN);
      display.print("FOLDER");
      display.setCursor (CHAR_width + 8 * 6, 3 * CHAR_height + 3 );
      display.setTextColor(GREY1);
      display.print("FILE");
    }
    else
    {
      display.setCursor (CHAR_width + 8 * 6, 3 * CHAR_height + 3 );
      display.setTextColor(GREEN);
      display.print("FILE");
      display.setCursor (CHAR_width + 13 * 6 , 3 * CHAR_height + 3 );
      display.setTextColor(GREY1);
      display.print("FOLDER");
    }
    if (fm.is_folder)
      display.setTextColor(DX_CYAN, BLACK);
    else
      display.setTextColor(WHITE, BLACK);
    display.show_smallfont_noGrid(4 * CHAR_height - 4, CHAR_width, 20, fm.temp_name );
  }
  if (LCDML.FUNC_close())     // ****** STABLE END *********
  {
    encoderDir[ENC_R].reset();
  }
}

void UI_func_midi_soft_thru(uint8_t param)
{
  static uint8_t old_soft_midi_thru;
  if (LCDML.FUNC_setup())         // ****** SETUP *********
  {
    encoderDir[ENC_R].reset();
    display.setCursor_textGrid(1, 1);
    display.print(F("MIDI Soft THRU"));
  }
  if (LCDML.FUNC_loop())          // ****** LOOP *********
  {
    if ((LCDML.BT_checkDown() && encoderDir[ENC_R].Down()) || (LCDML.BT_checkUp() && encoderDir[ENC_R].Up()))
    {
      if (LCDML.BT_checkDown())
        configuration.sys.soft_midi_thru = constrain(configuration.sys.soft_midi_thru + 1, SOFT_MIDI_THRU_MIN, SOFT_MIDI_THRU_MAX);
      else if (LCDML.BT_checkUp())
        configuration.sys.soft_midi_thru = constrain(configuration.sys.soft_midi_thru - 1, SOFT_MIDI_THRU_MIN, SOFT_MIDI_THRU_MAX);
    }
    display.setCursor_textGrid(1, 2);
    switch (configuration.sys.soft_midi_thru)
    {
      case 0:
        display.print(F("[OFF]"));
        break;
      case 1:
        display.print(F("[ON ]"));
        break;
    }
  }
  if (LCDML.FUNC_close())     // ****** STABLE END *********
  {
    encoderDir[ENC_R].reset();
    if (old_soft_midi_thru != configuration.sys.soft_midi_thru)
    {
      save_sys_flag = true;
      save_sys = 0;
    }
  }
}

void UI_func_smart_filter(uint8_t param)
{
  if (LCDML.FUNC_setup())         // ****** SETUP *********
  {
    encoderDir[ENC_R].reset();

    display.setCursor_textGrid(1, 1);
    display.print(F("Drm Smart Filter"));
  }
  if (LCDML.FUNC_loop())          // ****** LOOP *********
  {
    if ((LCDML.BT_checkDown() && encoderDir[ENC_R].Down()) || (LCDML.BT_checkUp() && encoderDir[ENC_R].Up()))
    {
      if (LCDML.BT_checkDown())
        seq.smartfilter = !seq.smartfilter;
      else if (LCDML.BT_checkUp())
        seq.smartfilter = !seq.smartfilter;
    }
    display.setCursor_textGrid(1, 2);
    if (seq.smartfilter) display.print(F("[ON ]")); else  display.print(F("[OFF]"));
  }
  if (LCDML.FUNC_close())     // ****** STABLE END *********
  {
    encoderDir[ENC_R].reset();
  }
}

void UI_func_velocity_level(uint8_t param)
{
  if (LCDML.FUNC_setup())         // ****** SETUP *********
  {
    encoderDir[ENC_R].reset();
    display_bar_int("Velocity Lvl", configuration.dexed[selected_instance_id].velocity_level, 1.0, VELOCITY_LEVEL_MIN, VELOCITY_LEVEL_MAX, 3, false, false, true);

    display_active_instance_number(selected_instance_id);
    UI_update_instance_icons();
  }

  if (LCDML.FUNC_loop())          // ****** LOOP *********
  {
    if ((LCDML.BT_checkDown() && encoderDir[ENC_R].Down()) || (LCDML.BT_checkUp() && encoderDir[ENC_R].Up()))
    {
      if (LCDML.BT_checkDown())
        configuration.dexed[selected_instance_id].velocity_level = constrain(configuration.dexed[selected_instance_id].velocity_level + ENCODER[ENC_R].speed(), VELOCITY_LEVEL_MIN, VELOCITY_LEVEL_MAX);
      else if (LCDML.BT_checkUp())
        configuration.dexed[selected_instance_id].velocity_level = constrain(configuration.dexed[selected_instance_id].velocity_level - ENCODER[ENC_R].speed(), VELOCITY_LEVEL_MIN, VELOCITY_LEVEL_MAX);
    }
#if NUM_DEXED > 1
    else if (LCDML.BT_checkEnter())
    {
      selected_instance_id = !selected_instance_id;
      display_active_instance_number(selected_instance_id);
      UI_update_instance_icons();
    }
#endif
    display_bar_int("Velocity Lvl", configuration.dexed[selected_instance_id].velocity_level, 1.0, VELOCITY_LEVEL_MIN, VELOCITY_LEVEL_MAX, 3, false, false, false);
  }
  if (LCDML.FUNC_close())     // ****** STABLE END *********
  {
    encoderDir[ENC_R].reset();
  }
}

void UI_update_instance_icons()
{
  display.fillRect(17 * CHAR_width + 11, 5, 2 * CHAR_width, CHAR_height, BLACK );
  display.drawBitmap(17 * CHAR_width + 11, 5, instance_num[0], 8, 8, WHITE);
  display.drawBitmap(18 * CHAR_width + 11, 5, instance_num[1], 8, 8, WHITE);
}

void print_voice_settings(int x, int y, uint8_t instance_id, bool fullrefresh)
{
  int yspacer = 16;
  char bank_name[BANK_NAME_LEN];
  char voice_name[VOICE_NAME_LEN];

  if (!get_bank_name(configuration.dexed[instance_id].bank, bank_name, sizeof(bank_name)))
    strcpy(bank_name, "*ERROR*");
  if (!get_voice_by_bank_name(configuration.dexed[instance_id].bank, bank_name, configuration.dexed[instance_id].voice, voice_name, sizeof(voice_name)))
    strcpy(voice_name, "*ERROR*");
  if (strlen(g_bank_name[selected_instance_id]) > 0)
  {
    strcpy(bank_name, g_bank_name[instance_id]);
  }
  else
  {
    if (!get_bank_name(configuration.dexed[instance_id].bank, bank_name, sizeof(bank_name)))
      strcpy(bank_name, "*ERROR*");
  }
  if (strlen(g_voice_name[instance_id]) > 0)
  {
    strcpy(voice_name, g_voice_name[instance_id]);
  }
  else
  {
    if (!get_voice_by_bank_name(configuration.dexed[instance_id].bank, bank_name, configuration.dexed[instance_id].voice, voice_name, sizeof(voice_name)))
      strcpy(voice_name, "*ERROR*");
  }

  display.setTextSize(2);
  display.setCursor(x, y);
  if (selected_instance_id == instance_id)
    display.setTextColor(WHITE, BLACK);
  else
    display.setTextColor(GREY1, BLACK);
  display.print("Instance"); display.setTextColor(DX_CYAN, BLACK);
  if (selected_instance_id == instance_id)
    display.setTextColor(GREEN, BLACK);
  else
    display.setTextColor(GREY1, BLACK);
  display.setCursor(x + 101, y);
  display.print(instance_id + 1);
  display.setTextSize(1);

  if (selected_instance_id == instance_id)
    display.setTextColor(DX_CYAN, BLACK);
  else
    display.setTextColor(GREY1, BLACK);
  display.setCursor(x + 118, y - 1);
  seq_print_formatted_number(configuration.dexed[instance_id].bank, 2);
  display.setCursor(x + 118, y + 7);
  seq_print_formatted_number(configuration.dexed[instance_id].voice + 1, 2);
  if (selected_instance_id == instance_id)
    display.setTextColor(WHITE, BLACK);
  else
    display.setTextColor(GREY1, BLACK);
  display.setCursor(x + 120 + 16, y - 1);
  string_toupper(bank_name);
  display.print( bank_name);
  display.setCursor(x + 120 + 16, y + 7);
  string_toupper(voice_name);
  display.print(voice_name);

  // static content
  if (fullrefresh)
  {
    display.setCursor(x + 140, y + 7 );
    display.setTextSize(1);
    display.setTextColor(GREY1, BLACK);
    yspacer = yspacer + 16; display.setCursor(x, y + yspacer);
    display.setTextColor(GREY1, BLACK); display.print("Volume ");
    display.setCursor(x + 118, y + yspacer);
    display.print("Chorus Level ");
    yspacer = yspacer + 9; display.setCursor(x, y + yspacer);
    display.setCursor(x, y + yspacer);
    display.print("Transpose ");
    display.setCursor(x + 118, y + yspacer);
    display.print("Delay Send ");
    yspacer = yspacer + 9; display.setCursor(x, y + yspacer);
    display.setCursor(x , y + yspacer);
    display.print("MIDI Channel ");
    display.setCursor(x + 118, y + yspacer);
    display.print("Reverb Send ");
    yspacer = yspacer + 9; display.setCursor(x, y + yspacer);
    display.setTextSize(1);
    display.print("Panorama ");
    display.setCursor(x + 118, y + yspacer);
    display.print("Delay Time ");
    yspacer = yspacer + 9; display.setCursor(x, y + yspacer);
    display.print("Lowest Note ");
    display.setCursor(x + 118, y + yspacer);
    display.print("Highest Note ");
    yspacer = yspacer + 9; display.setCursor(x, y + yspacer);
    display.print("Attack Mod ");
    display.setCursor(x + 118, y + yspacer);
    display.print("Release Mod ");
  }
  display.setCursor(x + 144 + 4 * CHAR_width, y + 7 );
  display.setTextSize(1);
  if (selected_instance_id == instance_id)
    display.setTextColor(DX_CYAN, BLACK);
  else
    display.setTextColor(GREY1, BLACK);
  yspacer = 16;
  yspacer = yspacer + 16;
  display.setCursor(x + 7 * CHAR_width,  y + yspacer);
  display.print(configuration.dexed[instance_id].sound_intensity);
  display.setCursor(x + 8 + 16 * CHAR_width, y + yspacer);
  display.print(configuration.fx.chorus_level[instance_id]);
  yspacer = yspacer + 9;
  display.setCursor(x + 8 + 16 * CHAR_width, y + yspacer);
  display.print(configuration.fx.delay_level[instance_id]);
  display.setCursor(x + 7 * CHAR_width, y + yspacer);
  display.print(configuration.dexed[instance_id].transpose);
  yspacer = yspacer + 9;
  display.setCursor(x + 7 * CHAR_width, y + yspacer);
  seq_print_formatted_number( configuration.dexed[instance_id].midi_channel, 2 );
  display.setCursor(x + 8 + 16 * CHAR_width, y + yspacer);
  display.print(configuration.fx.reverb_send[instance_id]);
  yspacer = yspacer + 9;
  display.setCursor(x + 7 * CHAR_width, y + yspacer);
  display.print(configuration.dexed[instance_id].pan);
  display.setCursor(x + 8 + 16 * CHAR_width, y + yspacer);
  display.print(configuration.fx.delay_time[instance_id]);
  yspacer = yspacer + 9;
  display.setCursor(x + 7 * CHAR_width, y + yspacer);
  display.print(configuration.dexed[instance_id].lowest_note);
  display.setCursor(x + 8 + 16 * CHAR_width, y + yspacer);
  display.print(configuration.dexed[instance_id].highest_note);
  yspacer = yspacer + 9;
  display.setCursor(x + 7 * CHAR_width, y + yspacer);
  display.print(perform_attack_mod[instance_id]);
  display.setCursor(x + 8 + 16 * CHAR_width, y + yspacer);
  display.print(perform_release_mod[instance_id]);
  yspacer = yspacer + 9;
  display.setCursor(x + 13 * CHAR_width, y + yspacer);
  display.setTextColor(WHITE, BLACK);
  display.setTextSize(2);
}

void UI_func_voice_select(uint8_t param)
{
  static uint8_t menu_voice_select = MENU_VOICE_SOUND;

  if (LCDML.FUNC_setup())         // ****** SETUP *********
  {
    print_voice_settings(12, 115, 0, true);
    print_voice_settings(252, 115, 1, true);
    //border1_clear();
    border1();
    border2();
    border3();
    border4();
    virtual_keyboard(220);
    display.setTextColor(DX_PURPLE, BLACK);
    display.show_smallfont_noGrid( 4 * CHAR_height - 10, CHAR_width, 13, "PERFORMANCE #");
    display.setCursor(CHAR_width, 4 * CHAR_height  );
    display.setTextColor(DX_CYAN, BLACK);
    display.setTextSize(1);
    seq_print_formatted_number(configuration.sys.performance_number, 2);
    display.setTextColor(WHITE, GREY3);
    display.show_smallfont_noGrid( 4 * CHAR_height, CHAR_width + 17, 11, seq.name);
    display.setTextColor(WHITE, BLACK);
    UI_draw_waveform(activesample);
    seq.last_drawn_sample = 254;
    encoderDir[ENC_R].reset();
    display_active_instance_number(selected_instance_id);
    char bank_name[BANK_NAME_LEN];
    char voice_name[VOICE_NAME_LEN];
    if (!get_bank_name(configuration.dexed[selected_instance_id].bank, bank_name, sizeof(bank_name)))
      strcpy(bank_name, "*ERROR*");
    if (!get_voice_by_bank_name(configuration.dexed[selected_instance_id].bank, bank_name, configuration.dexed[selected_instance_id].voice, voice_name, sizeof(voice_name)))
      strcpy(voice_name, "*ERROR*");
    UI_update_instance_icons();
  }

  if (LCDML.FUNC_loop())          // ****** LOOP *********
  {
    char bank_name[BANK_NAME_LEN];
    char voice_name[VOICE_NAME_LEN];
    if ((LCDML.BT_checkDown() && encoderDir[ENC_R].Down()) || (LCDML.BT_checkUp() && encoderDir[ENC_R].Up()) || (LCDML.BT_checkEnter() && (encoderDir[ENC_R].ButtonShort() || encoderDir[ENC_R].ButtonLong())))
    {
      uint8_t bank_tmp;
      int8_t voice_tmp;

      // Reset Performance Modifiers to 0 after every preset change
      for (uint8_t count_tmp = 0; count_tmp < NUM_DEXED; count_tmp++)
      {
        perform_attack_mod[count_tmp] = 0;
        perform_release_mod[count_tmp] = 0;
      }
      active_perform_page = 1;
      if (LCDML.BT_checkUp())
      {
        //start : show all presets
        if (configuration.sys.favorites == 0)
        {
          switch (menu_voice_select)
          {
            case MENU_VOICE_BANK:
              memset(g_bank_name[selected_instance_id], 0, BANK_NAME_LEN);
              bank_tmp = constrain(configuration.dexed[selected_instance_id].bank - ENCODER[ENC_R].speed(), 0, MAX_BANKS - 1);
              configuration.dexed[selected_instance_id].bank = bank_tmp;

              load_sd_voice(configuration.dexed[selected_instance_id].bank, configuration.dexed[selected_instance_id].voice, selected_instance_id);

              break;
            case MENU_VOICE_SOUND:
              memset(g_voice_name[selected_instance_id], 0, VOICE_NAME_LEN);
              voice_tmp = configuration.dexed[selected_instance_id].voice - ENCODER[ENC_R].speed();
              if (voice_tmp < 0 && configuration.dexed[selected_instance_id].bank - 1 >= 0)
              {
                configuration.dexed[selected_instance_id].bank--;
                configuration.dexed[selected_instance_id].bank = constrain(configuration.dexed[selected_instance_id].bank, 0, MAX_BANKS - 1);
              }
              else if (voice_tmp < 0 && configuration.dexed[selected_instance_id].voice - 1 <= 0)
              {
                voice_tmp = 0;
              }
              if (voice_tmp < 0)
                voice_tmp = MAX_VOICES + voice_tmp;
              configuration.dexed[selected_instance_id].voice = constrain(voice_tmp, 0, MAX_VOICES - 1);
              load_sd_voice(configuration.dexed[selected_instance_id].bank, configuration.dexed[selected_instance_id].voice, selected_instance_id);
              break;
          }
        }
        else //only Favs
          if (configuration.sys.favorites == 1)
          {
            locate_previous_favorite();
            load_sd_voice(configuration.dexed[selected_instance_id].bank, configuration.dexed[selected_instance_id].voice, selected_instance_id);
          }
          else  //only non-Favs
            if (configuration.sys.favorites == 2)
            {
              locate_previous_non_favorite();
              load_sd_voice(configuration.dexed[selected_instance_id].bank, configuration.dexed[selected_instance_id].voice, selected_instance_id);
              //break;

            } else  //random non-Favs
              if (configuration.sys.favorites == 3)
              {
                locate_random_non_favorite();
                load_sd_voice(configuration.dexed[selected_instance_id].bank, configuration.dexed[selected_instance_id].voice, selected_instance_id);
              }
      }  //end UP
      else if (LCDML.BT_checkDown())
      {
        //start : show all presets
        if (configuration.sys.favorites == 0)
        {
          switch (menu_voice_select)
          {
            case MENU_VOICE_BANK:
              memset(g_bank_name[selected_instance_id], 0, BANK_NAME_LEN);
              bank_tmp = constrain(configuration.dexed[selected_instance_id].bank + ENCODER[ENC_R].speed(), 0, MAX_BANKS - 1);
              configuration.dexed[selected_instance_id].bank = bank_tmp;
              load_sd_voice(configuration.dexed[selected_instance_id].bank, configuration.dexed[selected_instance_id].voice, selected_instance_id);
              break;
            case MENU_VOICE_SOUND:
              memset(g_voice_name[selected_instance_id], 0, VOICE_NAME_LEN);
              voice_tmp = configuration.dexed[selected_instance_id].voice + ENCODER[ENC_R].speed();
              if (voice_tmp >= MAX_VOICES && configuration.dexed[selected_instance_id].bank + 1 < MAX_BANKS)
              {
                voice_tmp %= MAX_VOICES;
                configuration.dexed[selected_instance_id].bank++;
                configuration.dexed[selected_instance_id].bank = constrain(configuration.dexed[selected_instance_id].bank, 0, MAX_BANKS - 1);

              }
              else if (voice_tmp >= MAX_VOICES && configuration.dexed[selected_instance_id].bank + 1 >= MAX_BANKS)
              {
                voice_tmp = MAX_VOICES - 1;
              }
              configuration.dexed[selected_instance_id].voice =  constrain(voice_tmp, 0, MAX_VOICES - 1);
              load_sd_voice(configuration.dexed[selected_instance_id].bank, configuration.dexed[selected_instance_id].voice, selected_instance_id);
              break;
          }
        }
        else //only Favs
          if (configuration.sys.favorites == 1)
          {

            locate_next_favorite();
            load_sd_voice(configuration.dexed[selected_instance_id].bank, configuration.dexed[selected_instance_id].voice, selected_instance_id);
            //break;
          }
          else  //only non-Favs
            if (configuration.sys.favorites == 2)
            {
              locate_next_non_favorite();
              load_sd_voice(configuration.dexed[selected_instance_id].bank, configuration.dexed[selected_instance_id].voice, selected_instance_id);
              //break;
            } else  //random non-Favs
              if (configuration.sys.favorites == 3)
              {
                locate_random_non_favorite();
                load_sd_voice(configuration.dexed[selected_instance_id].bank, configuration.dexed[selected_instance_id].voice, selected_instance_id);
              }
      }
      else if (LCDML.BT_checkEnter() && encoderDir[ENC_R].ButtonPressed())
      {
        if (menu_voice_select == MENU_VOICE_BANK)
          menu_voice_select = MENU_VOICE_SOUND;
        else
          menu_voice_select = MENU_VOICE_BANK;
      }
#if NUM_DEXED > 1
      else if (LCDML.BT_checkEnter())
      {
        selected_instance_id = !selected_instance_id;
        display_active_instance_number(selected_instance_id);
        UI_update_instance_icons();
      }
#endif
    }
    if (strlen(g_bank_name[selected_instance_id]) > 0)
    {
      strcpy(bank_name, g_bank_name[selected_instance_id]);
    }
    else
    {
      if (!get_bank_name(configuration.dexed[selected_instance_id].bank, bank_name, sizeof(bank_name)))
        strcpy(bank_name, "*ERROR*");
    }
    if (strlen(g_voice_name[selected_instance_id]) > 0)
    {
      strcpy(voice_name, g_voice_name[selected_instance_id]);
    }
    else
    {
      if (!get_voice_by_bank_name(configuration.dexed[selected_instance_id].bank, bank_name, configuration.dexed[selected_instance_id].voice, voice_name, sizeof(voice_name)))
        strcpy(voice_name, "*ERROR*");
    }
    display.setTextSize(2);
    display.setTextColor(DX_CYAN, BLACK);
    display.setCursor_textGrid(1, 1);
    seq_print_formatted_number(configuration.dexed[selected_instance_id].bank, 2);
    display.setCursor_textGrid(1, 2);
    seq_print_formatted_number(configuration.dexed[selected_instance_id].voice + 1, 2);
    display.setTextColor(WHITE, BLACK);
    string_toupper(bank_name);
    display.show(1, 5, 8, bank_name);
    string_toupper(voice_name);
    display.show(2, 5, 10, voice_name);

    display.setTextColor(DX_PURPLE, BLACK);
    switch (menu_voice_select)
    {
      case MENU_VOICE_BANK:
        display.show(1, 4, 1, "[");
        display.show(1, 13, 1, "]");
        display.show(2, 4, 1, " ");
        display.show(2, 15, 1, " ");
        break;
      case MENU_VOICE_SOUND:
        display.show(1, 4, 1, " ");
        display.show(1, 13, 1, " ");
        display.show(2, 4, 1, "[");
        display.show(2, 15, 1, "]");
        break;
    }
    display.setTextColor(WHITE, BLACK);
    draw_favorite_icon(configuration.dexed[selected_instance_id].bank, configuration.dexed[selected_instance_id].voice, selected_instance_id);
    if (selected_instance_id == 0)
      print_voice_settings(12, 115, 0, false);
    else
      print_voice_settings(252, 115, 1, false);
  }
  if (LCDML.FUNC_close())     // ****** STABLE END *********
  {
    encoderDir[ENC_R].reset();
  }
}

void UI_func_volume(uint8_t param)
{
  char tmp[6];
  static uint8_t old_volume;

  if (LCDML.FUNC_setup())         // ****** SETUP *********
  {
    old_volume = configuration.sys.vol;
    display.setTextSize(2);
    display.setTextColor(WHITE, BLACK);
    encoderDir[ENC_L].reset();
    if (active_perform_page == 1)
    { //Master Volume
      //display_bar_int("Master Vol.", configuration.sys.vol, 1.0, VOLUME_MIN, VOLUME_MAX, 3, false, false, true);
      back_from_volume = 0;
    }
  }

  if (LCDML.FUNC_loop())          // ****** LOOP *********
  {
    if ( LCDML.BT_checkDown() && encoderDir[ENC_R].Down() )
    {
      back_from_volume = 0;
      active_perform_page++;
      if (active_perform_page > 3)active_perform_page = 1;
    }
    else if ( LCDML.BT_checkUp() && encoderDir[ENC_R].Up() )
    {
      back_from_volume = 0;
      active_perform_page--;
      if (active_perform_page < 1)active_perform_page = 3;
    }
    if ((LCDML.BT_checkDown() && encoderDir[ENC_L].Down() ) || (LCDML.BT_checkUp() && encoderDir[ENC_L].Up() ))
    {
      if (active_perform_page == 1) {
        back_from_volume = 0;

        if (LCDML.BT_checkDown() )
        {
          configuration.sys.vol = constrain(configuration.sys.vol + ENCODER[ENC_L].speed(), VOLUME_MIN, VOLUME_MAX);
        }
        else if (LCDML.BT_checkUp() )
        {
          configuration.sys.vol = constrain(configuration.sys.vol - ENCODER[ENC_L].speed(), VOLUME_MIN, VOLUME_MAX);
        }
      }
      else if ( active_perform_page == 2)
      { //Attack

        if (LCDML.BT_checkDown()   )
        {
          if (perform_attack_mod[selected_instance_id] == 0)
            for (uint8_t i = 0; i < 6; i++) {
              orig_attack_values[selected_instance_id][i] = MicroDexed[selected_instance_id]->getOPRate(i, ATTACK);
            }
          perform_attack_mod[selected_instance_id] = constrain(perform_attack_mod[selected_instance_id] + ENCODER[ENC_L].speed(), -MAX_PERF_MOD, MAX_PERF_MOD);
          for (uint8_t i = 0; i < 6; i++)
            MicroDexed[selected_instance_id]->setOPRate(i, ATTACK, orig_attack_values[selected_instance_id][i] - perform_attack_mod[selected_instance_id] );
        }
        else if (LCDML.BT_checkUp() )
        {
          if (perform_attack_mod[selected_instance_id] == 0)  // Save initial Values
            for (uint8_t i = 0; i < 6; i++) {
              orig_attack_values[selected_instance_id][i] = MicroDexed[selected_instance_id]->getOPRate(i, ATTACK);
            }

          perform_attack_mod[selected_instance_id] = constrain(perform_attack_mod[selected_instance_id] - ENCODER[ENC_L].speed(), -MAX_PERF_MOD, MAX_PERF_MOD);
          for (uint8_t i = 0; i < 6; i++)
            MicroDexed[selected_instance_id]->setOPRate(i, ATTACK, orig_attack_values[selected_instance_id][i] - perform_attack_mod[selected_instance_id] );
        }
      }
      else if (active_perform_page == 3)
      { //Release

        if (LCDML.BT_checkDown() )
        {
          if (perform_release_mod[selected_instance_id] == 0) // Save initial Values
            for (uint8_t i = 0; i < 6; i++) {
              orig_release_values[selected_instance_id][i] = MicroDexed[selected_instance_id]->getOPRate(i, RELEASE);
            }
          perform_release_mod[selected_instance_id] = constrain(perform_release_mod[selected_instance_id] + ENCODER[ENC_L].speed(), -MAX_PERF_MOD, MAX_PERF_MOD);
          for (uint8_t i = 0; i < 6; i++)
            MicroDexed[selected_instance_id]->setOPRate(i, RELEASE, orig_release_values[selected_instance_id][i] - perform_release_mod[selected_instance_id] );
        }
        else if (LCDML.BT_checkUp() )
        {
          if (perform_release_mod[selected_instance_id] == 0)
            for (uint8_t i = 0; i < 6; i++) {
              orig_release_values[selected_instance_id][i] = MicroDexed[selected_instance_id]->getOPRate(i, RELEASE);
            }
          perform_release_mod[selected_instance_id] = constrain(perform_release_mod[selected_instance_id] - ENCODER[ENC_L].speed(), -MAX_PERF_MOD, MAX_PERF_MOD);
          for (uint8_t i = 0; i < 6; i++)
            MicroDexed[selected_instance_id]->setOPRate(i, RELEASE, orig_release_values[selected_instance_id][i] - perform_release_mod[selected_instance_id] );
        }
      }
    }
    display.setTextSize(2);
    display.setTextColor(WHITE, BLACK);
    display.setCursor_textGrid(15, 1);
    display.print("P");
    display.print(active_perform_page);
    display.print("/3");
    if (active_perform_page == 1)
    { //Master Volume
      display.setTextSize(2);
      display.setCursor_textGrid(1, 1);
      display.print("Master Volume");
      display_bar_int("Master Vol.", configuration.sys.vol, 1.0, VOLUME_MIN, VOLUME_MAX, 3, false, false, false);
      set_volume(configuration.sys.vol, configuration.sys.mono);
    }
    else if (active_perform_page == 2)
    { //Attack
      display.setCursor_textGrid(1, 1);
      display.print("Live Modify  ");
      display.setCursor_textGrid(1, 2);
      display.print("Attack Mod.  =");
      display.setCursor_textGrid(16, 2);
      sprintf(tmp, "%03d", perform_attack_mod[selected_instance_id]);
      display.print(tmp);
      back_from_volume = 0;
    }
    else if (active_perform_page == 3)
    { //Release
      display.setCursor_textGrid(1, 1);
      display.print("Live Modify  ");
      display.setCursor_textGrid(1, 2);
      display.print("Release Mod. =");
      display.setCursor_textGrid(16, 2);
      sprintf(tmp, "%03d", perform_release_mod[selected_instance_id]);
      display.print(tmp);
      back_from_volume = 0;
    }
  }
  if (LCDML.FUNC_close())     // ****** STABLE END *********
  {
    EEPROM.update(EEPROM_START_ADDRESS + offsetof(configuration_s, sys.vol), configuration.sys.vol);
    encoderDir[ENC_L].reset();

    if (old_volume != configuration.sys.vol)
    {
      eeprom_update();
      save_sys_flag = true;
      save_sys = 0;
    }
    display.setTextSize(2);
  }
}

void UI_func_save_voice(uint8_t param)
{
  static bool yesno;
  static uint8_t mode;

  if (LCDML.FUNC_setup())         // ****** SETUP *********
  {
    encoderDir[ENC_R].reset();

    yesno = false;
#if NUM_DEXED == 1
    mode = 1;
#else
    mode = 0;
#endif

#if NUM_DEXED == 1
    char bank_name[BANK_NAME_LEN];
    if (!get_bank_name(configuration.dexed[selected_instance_id].bank, bank_name, sizeof(bank_name)))
      strcpy(bank_name, "*ERROR*");

    display.setCursor_textGrid(1, 1);
    display.print(F("Save to Bank"));
    display.show(2, 1, 2, configuration.dexed[selected_instance_id].bank);
    display.show(2, 3, 10, bank_name);
    display.show(2, 2, 1, "[");
    display.show(2, 13, 1, "]");
#else
    display.setCursor_textGrid(1, 1);
    display.print(F("Save Instance"));
    display_active_instance_number(selected_instance_id);
    display.setCursor_textGrid(15, 1);
    display.print("x");
    display.setCursor_textGrid(16, 1);
    display.print("y");
#endif
  }
  if (LCDML.FUNC_loop())          // ****** LOOP *********
  {
    char bank_name[BANK_NAME_LEN];
    char voice_name[VOICE_NAME_LEN];
    if ((LCDML.BT_checkDown() && encoderDir[ENC_R].Down()) || (LCDML.BT_checkUp() && encoderDir[ENC_R].Up()))
    {
      switch (mode)
      {
        case 0: // Instance selection
          if (LCDML.BT_checkDown() || LCDML.BT_checkUp())
            selected_instance_id = !selected_instance_id;

          display_active_instance_number(selected_instance_id);
          display.setCursor_textGrid(5, 1);
          display.print(0);
          display.setCursor_textGrid(10, 1);
          display.print(1);
          break;
        case 1: // Bank selection
          if (LCDML.BT_checkDown())
            configuration.dexed[selected_instance_id].bank = constrain(configuration.dexed[selected_instance_id].bank + ENCODER[ENC_R].speed(), 0, MAX_BANKS - 1);
          else if (LCDML.BT_checkUp() && configuration.dexed[selected_instance_id].voice > 0)
            configuration.dexed[selected_instance_id].bank = constrain(configuration.dexed[selected_instance_id].bank - ENCODER[ENC_R].speed(), 0, MAX_BANKS - 1);

          if (!get_bank_name(configuration.dexed[selected_instance_id].bank, bank_name, sizeof(bank_name)))
            strcpy(bank_name, "*ERROR*");

          display.show(2, 1, 2, configuration.dexed[selected_instance_id].bank);
          display.show(2, 4, 10, bank_name);
          break;
        case 2: // Voice selection
          if (LCDML.BT_checkDown() && configuration.dexed[selected_instance_id].voice < MAX_VOICES - 1)
            configuration.dexed[selected_instance_id].voice = constrain(configuration.dexed[selected_instance_id].voice + ENCODER[ENC_R].speed(), 0, MAX_VOICES - 1);
          else if (LCDML.BT_checkUp() && configuration.dexed[selected_instance_id].voice > 0)
            configuration.dexed[selected_instance_id].voice = constrain(configuration.dexed[selected_instance_id].voice - ENCODER[ENC_R].speed(), 0, MAX_VOICES - 1);

          if (!get_bank_name(configuration.dexed[selected_instance_id].bank, bank_name, sizeof(bank_name)))
            strncpy(bank_name, "*ERROR*", sizeof(bank_name));
          if (!get_voice_by_bank_name(configuration.dexed[selected_instance_id].bank, bank_name, configuration.dexed[selected_instance_id].voice, voice_name, sizeof(voice_name)))
            strncpy(voice_name, "*ERROR*", sizeof(voice_name));

          display.show(2, 1, 2, configuration.dexed[selected_instance_id].voice + 1);
          display.show(2, 4, 10, voice_name);
          break;
        case 3: // Yes/No selection
          yesno = !yesno;
          if (yesno == true)
          {
            display.show(2, 1, 3, "YES");
          }
          else
          {
            display.show(2, 1, 3, "NO");
          }
          break;
      }
    }
    else if (LCDML.BT_checkEnter())
    {
      if (encoderDir[ENC_R].ButtonShort())
        mode++;
      switch (mode)
      {
        case 1:
          if (!get_bank_name(configuration.dexed[selected_instance_id].bank, bank_name, sizeof(bank_name)))
            strncpy(bank_name, "*ERROR*", sizeof(bank_name));
          display.setCursor_textGrid(1, 1);
          display.print(F("Save to Bank"));
          display.show(2, 1, 2, configuration.dexed[selected_instance_id].bank);
          display.show(2, 3, 10, bank_name);
          display.show(2, 2, 2, " [");
          display.show(2, 14, 1, "]");
          break;
        case 2:
          if (!get_bank_name(configuration.dexed[selected_instance_id].bank, bank_name, sizeof(bank_name)))
            strncpy(bank_name, "*ERROR*", sizeof(bank_name));
          if (!get_voice_by_bank_name(configuration.dexed[selected_instance_id].bank, bank_name, configuration.dexed[selected_instance_id].voice, voice_name, sizeof(voice_name)))
            strncpy(voice_name, "*ERROR*", sizeof(voice_name));

          display.show(1, 0, 16, "Save to Bank");
          display.show(1, 13, 2, configuration.dexed[selected_instance_id].bank);
          display.show(2, 0, 2, configuration.dexed[selected_instance_id].voice + 1);
          display.show(2, 3, 10, voice_name);
          break;
        case 3:
          display.show(1, 0, 16, "Overwrite?");
          display.show(2, 1, 15, "[NO");
          display.show(2, 4, 1, "]");
          break;
        default:
          if (yesno == true)
          {
#ifdef DEBUG
            bool ret = save_sd_voice(configuration.dexed[selected_instance_id].bank, configuration.dexed[selected_instance_id].voice, selected_instance_id);

            if (ret == true)
              Serial.println(F("Saving voice OK."));
            else
              Serial.println(F("Error while saving voice."));
#else
            save_sd_voice(configuration.dexed[selected_instance_id].bank, configuration.dexed[selected_instance_id].voice, selected_instance_id);
#endif
            display.show(2, 0, 16, "Done.");
            delay(MESSAGE_WAIT_TIME);

            mode = 0xff;
            break;
          }

          LCDML.FUNC_goBackToMenu();
      }
    }
  }
  if (LCDML.FUNC_close())     // ****** STABLE END *********
  {
    if (mode < 0xff)
    {
      display.show(2, 1, 16, "Canceled.");
      delay(MESSAGE_WAIT_TIME);
    }
    encoderDir[ENC_R].reset();
  }
}

void UI_func_sysex_receive_bank(uint8_t param)
{
  static bool yesno;
  static uint8_t mode;
  static uint8_t bank_number;
  static uint8_t ui_select_name_state;

  if (LCDML.FUNC_setup())         // ****** SETUP *********
  {
    encoderDir[ENC_R].reset();
    yesno = false;
    mode = 0;
    bank_number = configuration.dexed[selected_instance_id].bank;
    memset(receive_bank_filename, 0, sizeof(receive_bank_filename));

    display.setCursor_textGrid(1, 1);
    display.print(F("MIDI Recv Bank"));
    display.setCursor_textGrid(2, 2);
    display.print(F("["));
    display.setCursor_textGrid(14, 1);
    display.print(F("]"));
    if (!get_bank_name(configuration.dexed[selected_instance_id].bank, receive_bank_filename, sizeof(receive_bank_filename)))
      strcpy(receive_bank_filename, "*ERROR*");
    display.show(2, 0, 2, bank_number);
    display.show(2, 3, 10, receive_bank_filename);
  }
  if (LCDML.FUNC_loop())          // ****** LOOP *********
  {
    if ((LCDML.BT_checkDown() && encoderDir[ENC_R].Down()) || (LCDML.BT_checkUp() && encoderDir[ENC_R].Up()))
    {
      if (LCDML.BT_checkDown())
      {
        switch (mode)
        {
          case 0:
            bank_number = constrain(bank_number + ENCODER[ENC_R].speed(), 0, MAX_BANKS - 1);
            if (!get_bank_name(bank_number, receive_bank_filename, sizeof(receive_bank_filename)))
              strcpy(receive_bank_filename, "*ERROR*");
            display.show(2, 1, 2, bank_number);
            display.show(2, 3, 10, receive_bank_filename);
            break;
          case 1:
            yesno = !yesno;
            if (yesno)
              display.show(2, 12, 3, "YES");
            else
              display.show(2, 12, 3, "NO");
            break;
          case 2:
            ui_select_name_state = UI_select_name(1, 1, receive_bank_filename, BANK_NAME_LEN - 1, false);
            break;
        }
      }
      else if (LCDML.BT_checkUp())
      {
        switch (mode)
        {
          case 0:
            bank_number = constrain(bank_number - ENCODER[ENC_R].speed(), 0, MAX_BANKS - 1);
            if (!get_bank_name(bank_number, receive_bank_filename, sizeof(receive_bank_filename)))
              strcpy(receive_bank_filename, "*ERROR*");
            display.show(2, 0, 2, bank_number);
            display.show(2, 3, 10, receive_bank_filename);
            break;
          case 1:
            yesno = !yesno;
            if (yesno)
              display.show(2, 12, 3, "YES");
            else
              display.show(2, 12, 3, "NO");
            break;
          case 2:
            ui_select_name_state = UI_select_name(1, 1, receive_bank_filename, BANK_NAME_LEN - 1, false);
            break;
        }
      }
    }
    else if (LCDML.BT_checkEnter() && encoderDir[ENC_R].ButtonShort())
    {
      if (mode == 0)
      {
        if (!strcmp(receive_bank_filename, "*ERROR*"))
        {
          yesno = true;
          strcpy(receive_bank_filename, "NONAME");
          mode = 2;
          display.setCursor_textGrid(1, 2);
          display.print(F("[          ]    "));
          ui_select_name_state = UI_select_name(1, 1, receive_bank_filename, BANK_NAME_LEN - 1, true);
          // fix_later   lcd.blink();
        }
        else
        {
          mode = 1;
          display.setCursor_textGrid(1, 2);
          display.print(F("Overwrite: [NO ]"));
        }
      }
      else if (mode == 1 && yesno == true)
      {
        mode = 2;
        display.setCursor_textGrid(1, 2);
        display.print(F("[          ]    "));
        ui_select_name_state = UI_select_name(1, 1, receive_bank_filename, BANK_NAME_LEN - 1, true);
        // fix_later   lcd.blink();
      }
      else if (mode == 2)
      {
        ui_select_name_state = UI_select_name(1, 1, receive_bank_filename, BANK_NAME_LEN - 1, false);
        if (ui_select_name_state == true)
        {
          if (yesno == true)
          {
#ifdef DEBUG
            Serial.print(F("Bank name: ["));
            Serial.print(receive_bank_filename);
            Serial.println(F("]"));
#endif
            char tmp[FILENAME_LEN];
            strcpy(tmp, receive_bank_filename);
            sprintf(receive_bank_filename, "/%d/%s.syx", bank_number, tmp);
#ifdef DEBUG
            Serial.print(F("Receiving into bank "));
            Serial.print(bank_number);
            Serial.print(F(" as filename "));
            Serial.print(receive_bank_filename);
            Serial.println(F("."));
#endif
            mode = 0xff;
            // fix_later   lcd.noBlink();
            display.setCursor_textGrid(1, 2);
            display.print(F("Waiting...      "));
            /// Storing is done in SYSEX code
          }
        }
      }
      else if (mode >= 1 && yesno == false)
      {
        Serial.println(mode, DEC);
        memset(receive_bank_filename, 0, sizeof(receive_bank_filename));
        mode = 0xff;
        // fix_later   lcd.noBlink();
        display.setCursor_textGrid(1, 2);
        display.print(F("Canceled.       "));
        delay(MESSAGE_WAIT_TIME);
        LCDML.FUNC_goBackToMenu();
      }
    }
    encoderDir[ENC_R].reset();
  }

  if (LCDML.FUNC_close())     // ****** STABLE END *********
  {
    encoderDir[ENC_R].reset();

    memset(receive_bank_filename, 0, sizeof(receive_bank_filename));
    // fix_later   lcd.noBlink();

    if (mode < 0xff)
    {
      display.setCursor_textGrid(1, 2);
      display.print(F("Canceled.       "));
      delay(MESSAGE_WAIT_TIME);
    }
  }
}

void UI_func_set_performance_name(uint8_t param)
{
  static uint8_t mode;
  static uint8_t ui_select_name_state;
  if (LCDML.FUNC_setup())         // ****** SETUP *********
  {
    encoderDir[ENC_R].reset();
    mode = 0;
    display.setCursor_textGrid(1, 1);
    display.print(F("Perform. Name"));
  }
  if (LCDML.FUNC_loop())    // ****** LOOP *********
  {
    if ((LCDML.BT_checkDown() && encoderDir[ENC_R].Down()) || (LCDML.BT_checkUp() && encoderDir[ENC_R].Up()))
    {
      if (LCDML.BT_checkDown()) {
        if (mode == 1)  ui_select_name_state = UI_select_name(2, 2, seq.name_temp, BANK_NAME_LEN - 1, false);
      }
      else if (LCDML.BT_checkUp()) {
        if (mode == 1)  ui_select_name_state = UI_select_name(2, 2, seq.name_temp, BANK_NAME_LEN - 1, false);
      }
    }
    else if (LCDML.BT_checkEnter() && encoderDir[ENC_R].ButtonShort())
    {
      if (mode == 1)
      {
        ui_select_name_state = UI_select_name(2, 2, seq.name_temp, BANK_NAME_LEN - 1, false);
        if (ui_select_name_state == true)
        {
          strcpy( seq.name, seq.name_temp);
          mode = 0xff;
          // fix_later   lcd.noBlink();
          display.setCursor_textGrid(1, 2);
          display.print(F("OK.             "));
          delay(MESSAGE_WAIT_TIME);
          LCDML.FUNC_goBackToMenu();
        }
      }
    }
    if (mode == 0 )
    {
      mode = 1;
      strcpy(seq.name_temp, seq.name);
      display.setCursor_textGrid(1, 2);
      display.print(F("[          ]    "));
      ui_select_name_state = UI_select_name(2, 2, seq.name_temp, BANK_NAME_LEN - 1, true);
      //lcd.blink();
    }
  }
  if (LCDML.FUNC_close())     // ****** STABLE END *********
  {
    encoderDir[ENC_R].reset();
    // fix_later   lcd.noBlink();
  }
}

void UI_func_sysex_send_bank(uint8_t param)
{
  char bank_name[BANK_NAME_LEN];
  static uint8_t bank_number;

  if (LCDML.FUNC_setup())         // ****** SETUP *********
  {
    encoderDir[ENC_R].reset();
    bank_number = configuration.dexed[selected_instance_id].bank;
    display.setCursor_textGrid(1, 1);
    display.print(F("MIDI Send Bank"));
    if (!get_bank_name(configuration.dexed[selected_instance_id].bank, bank_name, sizeof(bank_name)))
      strncpy(bank_name, "*ERROR*", sizeof(bank_name));
    display.show(2, 2, 1, "[");
    display.show(2, 14, 1, "]");
    display.show(2, 0, 2, configuration.dexed[selected_instance_id].bank);
    display.show(2, 3, 10, bank_name);
  }

  if (LCDML.FUNC_loop())          // ****** LOOP *********
  {
    if ((LCDML.BT_checkDown() && encoderDir[ENC_R].Down()) || (LCDML.BT_checkUp() && encoderDir[ENC_R].Up()))
    {
      if (LCDML.BT_checkDown())
      {
        bank_number = constrain(bank_number + ENCODER[ENC_R].speed(), 0, MAX_BANKS - 1);
      }
      else if (LCDML.BT_checkUp())
      {
        bank_number = constrain(bank_number - ENCODER[ENC_R].speed(), 0, MAX_BANKS - 1);
      }
      if (!get_bank_name(bank_number, bank_name, sizeof(bank_name)))
        strcpy(bank_name, "*ERROR*");
      display.show(2, 0, 2, bank_number);
      display.show(2, 3, 10, bank_name);
    }
    else if (LCDML.BT_checkEnter() && encoderDir[ENC_R].ButtonShort())
    {
      File sysex;
      char filename[FILENAME_LEN];

      if (get_bank_name(bank_number, bank_name, sizeof(bank_name)))
      {
        sprintf(filename, "/%d/%s.syx", bank_number, bank_name);
#ifdef DEBUG
        Serial.print(F("Send bank "));
        Serial.print(filename);
        Serial.println(F(" from SD."));
#endif
        sysex = SD.open(filename);
        if (!sysex)
        {
#ifdef DEBUG
          Serial.println(F("Connot read from SD."));
#endif
          display.show(2, 0, 16, "Read error.");
          bank_number = 0xff;
        }
        else
        {
          uint8_t bank_data[4104];
          sysex.read(bank_data, 4104);
          sysex.close();
          display.show(2, 0, 16, "Sending Ch");
          if (configuration.dexed[selected_instance_id].midi_channel == MIDI_CHANNEL_OMNI)
          {
            display.show(2, 11, 2, "01");
            send_sysex_bank(1, bank_data);
          }
          else
          {
            display.show(2, 11, 2, configuration.dexed[selected_instance_id].midi_channel + 1);
            send_sysex_bank(configuration.dexed[selected_instance_id].midi_channel, bank_data);
          }
          display.show(2, 0, 16, "Done.");
          bank_number = 0xff;
        }
      }
      else
      {
        display.show(2, 0, 16, "No bank.");
        bank_number = 0xff;
      }

      delay(MESSAGE_WAIT_TIME);
      LCDML.FUNC_goBackToMenu();
    }
  }
  if (LCDML.FUNC_close())     // ****** STABLE END *********
  {
    encoderDir[ENC_R].reset();

    if (bank_number < 0xff)
    {
      display.setCursor_textGrid(1, 2);
      display.print(F("Canceled.       "));
      delay(MESSAGE_WAIT_TIME);
    }
  }
}

void UI_func_sysex_send_voice(uint8_t param)
{
  static uint8_t mode;
  static uint8_t bank_number;
  static uint8_t voice_number;

  if (LCDML.FUNC_setup())         // ****** SETUP *********
  {
    encoderDir[ENC_R].reset();
    mode = 0;
    bank_number = configuration.dexed[selected_instance_id].bank;
    voice_number = configuration.dexed[selected_instance_id].voice;

    char bank_name[BANK_NAME_LEN];

    if (!get_bank_name(bank_number, bank_name, sizeof(bank_name)))
      strcpy(bank_name, "*ERROR*");
    display.setCursor_textGrid(1, 1);
    display.print(F("MIDI Send Voice"));
    display.show(2, 1, 2, bank_number);
    display.show(2, 5, 10, bank_name);
    display.show(2, 4, 1, "[");
    display.show(2, 15, 1, "]");
  }

  if (LCDML.FUNC_loop())          // ****** LOOP *********
  {
    char bank_name[BANK_NAME_LEN];
    char voice_name[VOICE_NAME_LEN];

    if ((LCDML.BT_checkDown() && encoderDir[ENC_R].Down()) || (LCDML.BT_checkUp() && encoderDir[ENC_R].Up()))
    {
      switch (mode)
      {
        case 0: // Bank selection
          if (LCDML.BT_checkDown())
            bank_number = constrain(bank_number + ENCODER[ENC_R].speed(), 0, MAX_BANKS - 1);
          else if (LCDML.BT_checkUp() && bank_number > 0)
            bank_number = constrain(bank_number - ENCODER[ENC_R].speed(), 0, MAX_BANKS - 1);

          if (!get_bank_name(bank_number, bank_name, sizeof(bank_name)))
            strcpy(bank_name, "*ERROR*");

          display.show(2, 1, 2, bank_number);
          display.show(2, 5, 10, bank_name);
          break;
        case 1: // Voice selection
          if (LCDML.BT_checkDown() && voice_number < MAX_VOICES - 1)
            voice_number = constrain(voice_number + ENCODER[ENC_R].speed(), 0, MAX_VOICES - 1);
          else if (LCDML.BT_checkUp() && voice_number > 0)
            voice_number = constrain(voice_number - ENCODER[ENC_R].speed(), 0, MAX_VOICES - 1);
          if (!get_bank_name(bank_number, bank_name, sizeof(bank_name)))
            strncpy(bank_name, "*ERROR*", sizeof(bank_name));
          if (!get_voice_by_bank_name(bank_number, bank_name, voice_number, voice_name, sizeof(voice_name)))
            strncpy(voice_name, "*ERROR*", sizeof(voice_name));
          display.show(2, 1, 2, voice_number + 1);
          display.show(2, 5, 10, voice_name);
          break;
      }
    }
    else if (LCDML.BT_checkEnter())
    {
      if (encoderDir[ENC_R].ButtonShort())
        mode++;
      switch (mode)
      {
        case 1:
          if (!get_bank_name(bank_number, bank_name, sizeof(bank_name)))
            strncpy(bank_name, "*ERROR*", sizeof(bank_name));
          if (!get_voice_by_bank_name(bank_number, bank_name, voice_number, voice_name, sizeof(voice_name)))
            strncpy(voice_name, "*ERROR*", sizeof(voice_name));

          display.show(2, 1, 2, voice_number + 1);
          display.show(2, 5, 10, voice_name);
          break;
        case 2:
          File sysex;
          char filename[FILENAME_LEN];

          if (get_bank_name(bank_number, bank_name, sizeof(bank_name)))
          {
            sprintf(filename, "/%d/%s.syx", bank_number, bank_name);
#ifdef DEBUG
            Serial.print(F("Send voice "));
            Serial.print(voice_number);
            Serial.print(F(" of "));
            Serial.print(filename);
            Serial.println(F(" from SD."));
#endif
            sysex = SD.open(filename);
            if (!sysex)
            {
#ifdef DEBUG
              Serial.println(F("Connot read from SD."));
#endif
              display.show(2, 1, 16, "Read error.");
              bank_number = 0xff;
            }
            else
            {
              uint8_t voice_data[155];
              uint8_t encoded_voice_data[128];

              sysex.seek(6 + (voice_number * 128));
              sysex.read(encoded_voice_data, 128);

              MicroDexed[selected_instance_id]->decodeVoice(voice_data, encoded_voice_data);

              display.show(2, 1, 16, "Sending Ch");
              if (configuration.dexed[selected_instance_id].midi_channel == MIDI_CHANNEL_OMNI)
              {
                display.show(2, 12, 2, "01");
                send_sysex_voice(1, voice_data);
              }
              else
              {
                display.show(2, 12, 2, configuration.dexed[selected_instance_id].midi_channel + 1);
                send_sysex_voice(configuration.dexed[selected_instance_id].midi_channel, voice_data);
              }
              delay(MESSAGE_WAIT_TIME);
              display.show(2, 1, 16, "Done.");
              sysex.close();

              bank_number = 0xff;
            }
          }
          else
          {
            display.show(2, 1, 16, "No voice.");
            bank_number = 0xff;
          }
          mode = 0xff;
          delay(MESSAGE_WAIT_TIME);
          LCDML.FUNC_goBackToMenu();
          break;
      }
    }
  }

  if (LCDML.FUNC_close())     // ****** STABLE END *********
  {
    if (mode < 0xff)
    {
      display.show(2, 1, 16, "Canceled.");
      delay(MESSAGE_WAIT_TIME);
    }
    encoderDir[ENC_R].reset();
  }
}

void UI_func_eq_1(uint8_t param)
{
#ifndef SGTL5000_AUDIO_ENHANCE
  if (LCDML.FUNC_setup())         // ****** SETUP *********
  {
    encoderDir[ENC_R].reset();
    display.setCursor_textGrid(1, 1);
    display.print(F("EQ Low-Cut"));
    display.setCursor_textGrid(1, 2);
    display.print(F("Not implemented."));
  }
#else
  if (LCDML.FUNC_setup())         // ****** SETUP *********
  {
    encoderDir[ENC_R].reset();
  }
  if (LCDML.FUNC_loop())          // ****** LOOP *********
  {
    if ((LCDML.BT_checkDown() && encoderDir[ENC_R].Down()) || (LCDML.BT_checkUp() && encoderDir[ENC_R].Up()))
    {
      if (LCDML.BT_checkDown())
      {
        configuration.fx.eq_1 = constrain(configuration.fx.eq_1 + ENCODER[ENC_R].speed(), EQ_1_MIN, EQ_1_MAX);
      }
      else if (LCDML.BT_checkUp())
      {
        configuration.fx.eq_1 = constrain(configuration.fx.eq_1 - ENCODER[ENC_R].speed(), EQ_1_MIN, EQ_1_MAX);
      }
    }
    display_meter_int("EQ Low-Cut [Hz]", configuration.fx.eq_1, 1.0, 0.0, EQ_1_MIN, EQ_1_MAX, 3, false, false, true);
    sgtl5000.setEQFc(1, float(configuration.fx.eq_1));
    sgtl5000.setEQGain(1, 6.0);
    sgtl5000.commitFilter(1);
#ifdef DEBUG
    sgtl5000.show_params(1);
#endif
  }
  if (LCDML.FUNC_close())     // ****** STABLE END *********
  {
    encoderDir[ENC_R].reset();
  }
#endif
}

void UI_func_eq_2(uint8_t param)
{
#ifndef SGTL5000_AUDIO_ENHANCE
  if (LCDML.FUNC_setup())         // ****** SETUP *********
  {
    encoderDir[ENC_R].reset();
    display.setCursor_textGrid(1, 1);
    display.print(F("EQ 120Hz"));
    display.setCursor_textGrid(1, 2);
    display.print(F("Not implemented."));
  }
#else
  if (LCDML.FUNC_setup())         // ****** SETUP *********
  {
    encoderDir[ENC_R].reset();
  }

  if (LCDML.FUNC_loop())          // ****** LOOP *********
  {
    if ((LCDML.BT_checkDown() && encoderDir[ENC_R].Down()) || (LCDML.BT_checkUp() && encoderDir[ENC_R].Up()))
    {
      if (LCDML.BT_checkDown())
      {
        configuration.fx.eq_2 = constrain(configuration.fx.eq_2 + ENCODER[ENC_R].speed(), EQ_2_MIN, EQ_2_MAX);
      }
      else if (LCDML.BT_checkUp())
      {
        configuration.fx.eq_2 = constrain(configuration.fx.eq_2 - ENCODER[ENC_R].speed(), EQ_2_MIN, EQ_2_MAX);
      }
    }
    display_meter_float("EQ 120Hz [dB]", configuration.fx.eq_2, 0.1, 0.0, EQ_2_MIN, EQ_2_MAX, 1, 1, false, true, true);
    sgtl5000.setEQGain(2, mapfloat(configuration.fx.eq_2, EQ_2_MIN, EQ_2_MAX, -9.9, 9.9));
    sgtl5000.commitFilter(2);
#ifdef DEBUG
    sgtl5000.show_params(2);
#endif
  }
  if (LCDML.FUNC_close())     // ****** STABLE END *********
  {
    encoderDir[ENC_R].reset();
  }
#endif
}

void UI_func_eq_3(uint8_t param)
{
#ifndef SGTL5000_AUDIO_ENHANCE
  if (LCDML.FUNC_setup())         // ****** SETUP *********
  {
    encoderDir[ENC_R].reset();
    display.setCursor_textGrid(1, 1);
    display.print(F("EQ 220Hz"));
    display.setCursor_textGrid(1, 2);
    display.print(F("Not implemented."));
  }
#else
  if (LCDML.FUNC_setup())         // ****** SETUP *********
  {
    encoderDir[ENC_R].reset();
  }
  if (LCDML.FUNC_loop())          // ****** LOOP *********
  {
    if ((LCDML.BT_checkDown() && encoderDir[ENC_R].Down()) || (LCDML.BT_checkUp() && encoderDir[ENC_R].Up()))
    {
      if (LCDML.BT_checkDown())
      {
        configuration.fx.eq_3 = constrain(configuration.fx.eq_3 + ENCODER[ENC_R].speed(), EQ_3_MIN, EQ_3_MAX);
      }
      else if (LCDML.BT_checkUp())
      {
        configuration.fx.eq_3 = constrain(configuration.fx.eq_3 - ENCODER[ENC_R].speed(), EQ_3_MIN, EQ_3_MAX);
      }
    }
    display_meter_float("EQ 220Hz [dB]", configuration.fx.eq_3, 0.1, 0.0, EQ_3_MIN, EQ_3_MAX, 1, 1, false, true, true);
    sgtl5000.setEQGain(3, mapfloat(configuration.fx.eq_3, EQ_3_MIN, EQ_3_MAX, -9.9, 9.9));
    sgtl5000.commitFilter(3);
#ifdef DEBUG
    sgtl5000.show_params(3);
#endif
  }
  if (LCDML.FUNC_close())     // ****** STABLE END *********
  {
    encoderDir[ENC_R].reset();
  }
#endif
}

void UI_func_eq_4(uint8_t param)
{
#ifndef SGTL5000_AUDIO_ENHANCE
  if (LCDML.FUNC_setup())         // ****** SETUP *********
  {
    encoderDir[ENC_R].reset();
    display.setCursor_textGrid(1, 1);
    display.print(F("EQ 1000Hz"));
    display.setCursor_textGrid(1, 2);
    display.print(F("Not implemented."));
  }
#else
  if (LCDML.FUNC_setup())         // ****** SETUP *********
  {
    encoderDir[ENC_R].reset();
  }
  if (LCDML.FUNC_loop())          // ****** LOOP *********
  {
    if ((LCDML.BT_checkDown() && encoderDir[ENC_R].Down()) || (LCDML.BT_checkUp() && encoderDir[ENC_R].Up()))
    {
      if (LCDML.BT_checkDown())
      {
        configuration.fx.eq_4 = constrain(configuration.fx.eq_4 + ENCODER[ENC_R].speed(), EQ_4_MIN, EQ_4_MAX);
      }
      else if (LCDML.BT_checkUp())
      {
        configuration.fx.eq_4 = constrain(configuration.fx.eq_4 - ENCODER[ENC_R].speed(), EQ_4_MIN, EQ_4_MAX);
      }
    }
    display_meter_float("EQ 1000Hz [dB]", configuration.fx.eq_4, 0.1, 0.0, EQ_4_MIN, EQ_4_MAX, 1, 1, false, true, true);
    sgtl5000.setEQGain(4, mapfloat(configuration.fx.eq_4, EQ_4_MIN, EQ_4_MAX, -9.9, 9.9));
    sgtl5000.commitFilter(4);
#ifdef DEBUG
    sgtl5000.show_params(4);
#endif
  }
  if (LCDML.FUNC_close())     // ****** STABLE END *********
  {
    encoderDir[ENC_R].reset();
  }
#endif
}

void UI_func_eq_5(uint8_t param)
{
#ifndef SGTL5000_AUDIO_ENHANCE
  if (LCDML.FUNC_setup())         // ****** SETUP *********
  {
    encoderDir[ENC_R].reset();
    display.setCursor_textGrid(1, 1);
    display.print(F("EQ 2000Hz"));
    display.setCursor_textGrid(1, 2);
    display.print(F("Not implemented."));
  }
#else
  if (LCDML.FUNC_setup())         // ****** SETUP *********
  {
    encoderDir[ENC_R].reset();
  }

  if (LCDML.FUNC_loop())          // ****** LOOP *********
  {
    if ((LCDML.BT_checkDown() && encoderDir[ENC_R].Down()) || (LCDML.BT_checkUp() && encoderDir[ENC_R].Up()))
    {
      if (LCDML.BT_checkDown())
      {
        configuration.fx.eq_5 = constrain(configuration.fx.eq_5 + ENCODER[ENC_R].speed(), EQ_5_MIN, EQ_5_MAX);
      }
      else if (LCDML.BT_checkUp())
      {
        configuration.fx.eq_5 = constrain(configuration.fx.eq_5 - ENCODER[ENC_R].speed(), EQ_5_MIN, EQ_5_MAX);
      }
    }
    display_meter_float("EQ 2000Hz [dB]", configuration.fx.eq_5, 0.1, 0.0, EQ_5_MIN, EQ_5_MAX, 1, 1, false, true, true);
    sgtl5000.setEQGain(5, mapfloat(configuration.fx.eq_5, EQ_5_MIN, EQ_5_MAX, -9.9, 9.9));
    sgtl5000.commitFilter(5);
#ifdef DEBUG
    sgtl5000.show_params(5);
#endif
  }

  if (LCDML.FUNC_close())     // ****** STABLE END *********
  {
    encoderDir[ENC_R].reset();
  }
#endif
}

void UI_func_eq_6(uint8_t param)
{
#ifndef SGTL5000_AUDIO_ENHANCE
  if (LCDML.FUNC_setup())         // ****** SETUP *********
  {
    encoderDir[ENC_R].reset();
    display.setCursor_textGrid(1, 1);
    display.print(F("EQ 7000Hz"));
    display.setCursor_textGrid(1, 2);
    display.print(F("Not implemented."));
  }
#else
  if (LCDML.FUNC_setup())         // ****** SETUP *********
  {
    encoderDir[ENC_R].reset();
  }

  if (LCDML.FUNC_loop())          // ****** LOOP *********
  {
    if ((LCDML.BT_checkDown() && encoderDir[ENC_R].Down()) || (LCDML.BT_checkUp() && encoderDir[ENC_R].Up()))
    {
      if (LCDML.BT_checkDown())
      {
        configuration.fx.eq_6 = constrain(configuration.fx.eq_6 + ENCODER[ENC_R].speed(), EQ_6_MIN, EQ_6_MAX);
      }
      else if (LCDML.BT_checkUp())
      {
        configuration.fx.eq_6 = constrain(configuration.fx.eq_6 - ENCODER[ENC_R].speed(), EQ_6_MIN, EQ_6_MAX);
      }
    }
    display_meter_float("EQ 7000Hz [dB]", configuration.fx.eq_6, 0.1, 0.0, EQ_6_MIN, EQ_6_MAX, 1, 1, false, true, true);
    sgtl5000.setEQGain(6, mapfloat(configuration.fx.eq_6, EQ_6_MIN, EQ_6_MAX, -9.9, 9.9));
    sgtl5000.commitFilter(6);
#ifdef DEBUG
    sgtl5000.show_params(6);
#endif
  }

  if (LCDML.FUNC_close())     // ****** STABLE END *********
  {
    encoderDir[ENC_R].reset();
  }
#endif
}

void UI_func_eq_7(uint8_t param)
{
#ifndef SGTL5000_AUDIO_ENHANCE
  if (LCDML.FUNC_setup())         // ****** SETUP *********
  {
    encoderDir[ENC_R].reset();
    display.setCursor_textGrid(1, 1);
    display.print(F("EQ High-Cut"));
    display.setCursor_textGrid(1, 2);
    display.print(F("Not implemented."));
  }
#else
  if (LCDML.FUNC_setup())         // ****** SETUP *********
  {
    encoderDir[ENC_R].reset();

  }

  if (LCDML.FUNC_loop())          // ****** LOOP *********
  {
    if ((LCDML.BT_checkDown() && encoderDir[ENC_R].Down()) || (LCDML.BT_checkUp() && encoderDir[ENC_R].Up()))
    {
      if (LCDML.BT_checkDown())
      {
        configuration.fx.eq_7 = constrain(configuration.fx.eq_7 + ENCODER[ENC_R].speed(), EQ_7_MIN, EQ_7_MAX);
      }
      else if (LCDML.BT_checkUp())
      {
        configuration.fx.eq_7 = constrain(configuration.fx.eq_7 - ENCODER[ENC_R].speed(), EQ_7_MIN, EQ_7_MAX);
      }
    }
    display_meter_float("EQ High-Cut[kHz]", configuration.fx.eq_7, 1.0, 0.0, EQ_7_MIN, EQ_7_MAX, 3, 1, false, false, true);
    sgtl5000.setEQFc(7, float(configuration.fx.eq_7) * 1000.0);
    sgtl5000.commitFilter(7);
#ifdef DEBUG
    sgtl5000.show_params(7);
#endif
  }

  if (LCDML.FUNC_close())     // ****** STABLE END *********
  {

    encoderDir[ENC_R].reset();
  }
#endif
}

void UI_func_startup(uint8_t param)
{
  bool stored = false;
  static uint8_t old_load_at_startup;

  if (LCDML.FUNC_setup())         // ****** SETUP *********
  {
    old_load_at_startup = configuration.sys.load_at_startup;

    encoderDir[ENC_R].reset();
    display.setCursor(0, 0);
    display.show(1, 1, 16, "Load at startup");
    if (configuration.sys.load_at_startup == 255)
      display.show(2, 1, 16, "Last Performance");
    else if (configuration.sys.load_at_startup <= PERFORMANCE_NUM_MAX)
    {
      display.show(2, 1, 16, "Fixed Perf. [");
      display.setCursor_textGrid(14, 2);
      seq_print_formatted_number(configuration.sys.load_at_startup, 2);
      display.show(2, 16, 1, "]");
    }
  }

  if (LCDML.FUNC_loop())          // ****** LOOP *********
  {
    if ((LCDML.BT_checkDown() && encoderDir[ENC_R].Down()) || (LCDML.BT_checkUp() && encoderDir[ENC_R].Up()) || (LCDML.BT_checkEnter() && encoderDir[ENC_R].ButtonShort()))
    {
      if (LCDML.BT_checkDown())
      {
        if (configuration.sys.load_at_startup == 255)
          configuration.sys.load_at_startup = PERFORMANCE_NUM_MIN;
        else if (configuration.sys.load_at_startup >= 0 && configuration.sys.load_at_startup <= PERFORMANCE_NUM_MAX)
          configuration.sys.load_at_startup++;
        if (configuration.sys.load_at_startup > PERFORMANCE_NUM_MAX)
          configuration.sys.load_at_startup = 255;
      }
      else if (LCDML.BT_checkUp())
      {
        if (configuration.sys.load_at_startup == 255)
          configuration.sys.load_at_startup = PERFORMANCE_NUM_MAX;
        else if (configuration.sys.load_at_startup >= PERFORMANCE_NUM_MIN && configuration.sys.load_at_startup <= PERFORMANCE_NUM_MAX)
          configuration.sys.load_at_startup--;
      }
      else if (LCDML.BT_checkEnter())
      {
        stored = true;
        display.show(2, 1, 16, "Done.");
        save_sd_sys_json();
        if (configuration.sys.load_at_startup <= PERFORMANCE_NUM_MAX && configuration.sys.load_at_startup != configuration.sys.performance_number)
          load_sd_performance_json(configuration.sys.load_at_startup);
        delay(MESSAGE_WAIT_TIME);
        LCDML.FUNC_goBackToMenu();
      }

      display.setCursor(1, 2);
      if (configuration.sys.load_at_startup == 255)
        display.show(2, 1, 16, "Last Performance");
      else if (configuration.sys.load_at_startup <= PERFORMANCE_NUM_MAX)
      {
        display.show(2, 1, 16, "Fixed Perf. [");
        display.setCursor_textGrid(14, 2);
        seq_print_formatted_number(configuration.sys.load_at_startup, 2);
        display.show(2, 16, 1, "]");
        display.setTextColor(DX_CYAN, BLACK);
        if (check_sd_performance_exists(configuration.sys.load_at_startup))
        {
          get_sd_performance_name_json(configuration.sys.load_at_startup);
          if ( seq.name_temp[0] != 0 )
            display.show(3, 1, 16, seq.name_temp);
          else
            display.show(3, 1, 16, "----  DATA  ----");
        }
        else
          display.show(3, 1, 16,   "-- EMPTY SLOT --");
      }
      display.setTextColor(WHITE, BLACK);
    }
  }

  if (LCDML.FUNC_close())     // ****** STABLE END *********
  {
    //display_special_chars(SCROLLBAR);
    if (stored == false)
    {
      display.show(2, 1, 16, "Canceled.");
      configuration.sys.load_at_startup = old_load_at_startup;
      delay(MESSAGE_WAIT_TIME);
    }
    encoderDir[ENC_R].reset();
  }
}

void UI_function_not_enabled(void)
{
  if (LCDML.FUNC_setup())         // ****** SETUP *********
  {
    encoderDir[ENC_R].reset();

    display.setCursor_textGrid(1, 1);
    display.print(F("Function not"));
    display.setCursor_textGrid(1, 2);
    display.print(F("enbaled!"));
  }

  if (LCDML.FUNC_loop())          // ****** LOOP *********
  {
    ;
  }

  if (LCDML.FUNC_close())     // ****** STABLE END *********
  {
    encoderDir[ENC_R].reset();
  }
}

void UI_function_not_implemented(uint8_t param)
{
  if (LCDML.FUNC_setup())         // ****** SETUP *********
  {
    encoderDir[ENC_R].reset();

    display.setCursor_textGrid(1, 1);
    display.print(F("Function not"));
    display.setCursor_textGrid(1, 2);
    display.print(F("implemented!"));
  }

  if (LCDML.FUNC_loop())          // ****** LOOP *********
  {
    ;
  }

  if (LCDML.FUNC_close())     // ****** STABLE END *********
  {
    encoderDir[ENC_R].reset();
  }
}

bool UI_select_name(uint8_t y, uint8_t x, char* edit_string, uint8_t len, bool init)
{
  static int8_t edit_pos;
  static bool edit_mode;
  static uint8_t edit_value;
  static uint8_t last_char_pos;
  display.setTextColor( WHITE, BLACK);
  if (init == true)
  {
    edit_mode = false;
    edit_pos = 0;
    edit_value = search_accepted_char(edit_string[edit_pos]);
    last_char_pos = strlen(edit_string);
    string_trim(edit_string); // just to be sure
    //draw full string
    display.setCursor_textGrid(x, y);
    display.print(edit_string);
    //overwrite first char with inversed character
    display.setCursor_textGrid(x, y);
    display.setTextColor(BLACK, DX_CYAN);
    display.print(edit_string[0]);
    display.setTextColor(GREY3, BLACK);
    display.setCursor_textGrid(x + len + 2, y);
    display.print(" OK ");
    display.setTextColor( WHITE, BLACK);
    display.setCursor_textGrid(x, y);
    return (false);
  }

  if (LCDML.BT_checkDown() || LCDML.BT_checkUp())
  {
    if (LCDML.BT_checkDown())
    {
      if (edit_mode == true)
      {
        edit_value = search_accepted_char(edit_string[edit_pos]);

        if (edit_value < sizeof(accepted_chars) - 2)
          edit_value++;
        if (edit_value == 0 && edit_string[constrain(edit_pos + 1, 0, len)] > 0)
          edit_value = 1;

        edit_string[edit_pos] = accepted_chars[edit_value];

        display.setCursor_textGrid(x + edit_pos, y);
        display.print(edit_string[edit_pos]);
      }
      else
      {
        if (edit_string[edit_pos] != 0 && edit_string[edit_pos] != 32)
          edit_pos = constrain(edit_pos + 1, 0, len);
        else
        {
          if (edit_pos + 1 > last_char_pos)
            edit_pos = len;
        }

        if (edit_pos == len)
        {
          display.setTextColor(BLACK, DX_CYAN);
          display.setCursor_textGrid(x + len + 2, y);
          display.print("[OK]");
          display.setTextColor(WHITE, BLACK);
        }

      }
    }
    else if (LCDML.BT_checkUp())
    {
      if (edit_mode == true)
      {
        edit_value = search_accepted_char(edit_string[edit_pos]);

        if (edit_value >= 1)
          edit_value--;
        if (edit_value == 0 && edit_string[constrain(edit_pos + 1, 0, len)] > 0)
          edit_value = 1;
        edit_string[edit_pos] = accepted_chars[edit_value];

        display.setCursor_textGrid(x + edit_pos, y);
        display.print(edit_string[edit_pos]);
      }
      else
      {
        if (edit_pos - 1 > last_char_pos)
          edit_pos = last_char_pos;
        else
          edit_pos = constrain(edit_pos - 1, 0, len - 1);

        if (edit_pos == last_char_pos)
        {
          display.setCursor_textGrid(x - 1, y);
          display.print("[");
          display.setCursor_textGrid(x + len, y);
          display.print("]");
          display.setCursor_textGrid(x + len + 1, y);
          display.print("    ");
          // fix_later   lcd.blink();
        }
      }
    }
  }
  else if (LCDML.BT_checkEnter())
  {
    last_char_pos = strlen(edit_string);
    if (edit_pos >= len)
    {
      edit_pos = 0;
      edit_mode = false;

      return (true);
    }
    else
    {
      last_char_pos = strlen(edit_string);
      edit_mode = !edit_mode;
    }
    if (edit_mode == false && edit_pos < len && edit_string[edit_pos] != 0 && edit_string[edit_pos] != 32)
      edit_pos++;
    if (edit_mode == true)
    {
      display.setCursor_textGrid(x + len + 1, y);
      display.print("*");
    }
    else
    {
      display.setCursor_textGrid(x + len + 1, y);
      display.print(" ");
    }
  }
  if (edit_pos < len)
  {
    display.setCursor_textGrid(x + edit_pos, y);
    display.setTextColor(BLACK, DX_CYAN);
    display.print(edit_string[edit_pos]);
    display.setTextColor( WHITE, BLACK);
  }
  if (edit_pos == 0)
  {
    display.setCursor_textGrid(x + edit_pos + 1, y);
    display.print(edit_string[edit_pos + 1]);
  }
  else if (edit_pos > 0 && edit_pos < len - 1)
  {
    display.setCursor_textGrid(x + edit_pos - 1, y);
    display.print(edit_string[edit_pos - 1]);

    display.setCursor_textGrid(x + edit_pos + 1, y);
    display.print(edit_string[edit_pos + 1]);
  }
  else if (edit_pos == len - 1 || edit_pos == len)
  {
    display.setCursor_textGrid(x + edit_pos - 1, y);
    display.print(edit_string[edit_pos - 1]);
  }
  if (edit_pos == len - 1 )
  {
    display.setTextColor(GREY3, BLACK);
    display.setCursor_textGrid(x + len + 2, y);
    display.print(" OK ");
  }
  display.setTextColor( WHITE, BLACK);
  encoderDir[ENC_R].reset();
  return (false);
}

uint8_t search_accepted_char(uint8_t c)
{
  //if (c == 0)
  //  c = 32;

  for (uint8_t i = 0; i < sizeof(accepted_chars) - 1; i++)
  {
    Serial.print(i, DEC);
    Serial.print(":");
    Serial.print(c);
    Serial.print("==");
    Serial.println(accepted_chars[i], DEC);
    if (c == accepted_chars[i])
      return (i);
  }
  return (0);
}

void display_int(int16_t var, uint8_t size, bool zeros, bool brackets, bool sign)
{
  display_float(float(var), size, 0, zeros, brackets, sign);
}

void display_float(float var, uint8_t size_number, uint8_t size_fraction, bool zeros, bool brackets, bool sign)
{
  char s[display_cols + 1];
  char f[display_cols + 1];

  if (size_fraction > 0)
  {
    if (zeros == true && sign == true)
      sprintf(f, "%%+0%d.%df", size_number + size_fraction + 2, size_fraction);
    else if (zeros == true && sign == false)
      sprintf(f, "%%+0%d.%df", size_number + size_fraction + 1, size_fraction);
    else if (zeros == false && sign == true)
      sprintf(f, "%%+%d.%df", size_number + size_fraction + 2, size_fraction);
    else if (zeros == false && sign == false)
      sprintf(f, "%%%d.%df", size_number + size_fraction + 1, size_fraction);

    sprintf(s, f, var);
  }
  else
  {
    if (zeros == true && sign == true)
      sprintf(f, "%%+0%dd", size_number + 1);
    else if (zeros == true && sign == false)
      sprintf(f, "%%%0dd", size_number);
    else if (zeros == false && sign == true)
      sprintf(f, "%%+%dd", size_number + 1);
    else if (zeros == false && sign == false)
      sprintf(f, "%%%dd", size_number);

    sprintf(s, f, int(var));
  }

  if (brackets == true)
  {
    char tmp[display_cols + 1];

    strcpy(tmp, s);
    sprintf(s, "[%s]", tmp);
  }

  display.print(s);
}

inline void display_bar_int(const char* title, uint32_t value, float factor, int32_t min_value, int32_t max_value, uint8_t size, bool zeros, bool sign, bool init)
{
  display_bar_float(title, float(value), factor, min_value, max_value, size, 0, zeros, sign, init);
}

void display_bar_float(const char* title, float value, float factor, int32_t min_value, int32_t max_value, uint8_t size_number, uint8_t size_fraction, bool zeros, bool sign, bool init)
{
  uint8_t size;
  float v;
  float _vi = 0.0;
  uint8_t vf;
  uint8_t vi;

  if (size_fraction == 0)
    size = size_number;
  else
    size = size_number + size_fraction + 1;
  if (sign == true)
    size++;

  v = float((value - min_value) * (display_cols + 4 - size)) / (max_value - min_value);
  vf = uint8_t(modff(v, &_vi) * 10.0 + 0.5);
  vi = uint8_t(_vi);

  if (sign == true)
    size += 1;

  // Title
  if (init == true)
    display.show(1, 1, display_cols - 2, title);

  // Value
  display.setCursor( CHAR_width * (display_cols - size - 1), CHAR_height * 2);
  display_float(value * factor, size_number, size_fraction, zeros, false, sign); // TBD

  // Bar

  if (vi == 0)
  {
    display.drawBitmap(  CHAR_width , 2 * CHAR_height, block_bar[ (uint8_t)(vf / 1.25 - 0.5)  ], 8, 15, WHITE, BLACK);
    for (uint8_t i = vi + 1; i < display_cols - size + 5; i++)
      display.fillRect( CHAR_width + i * 8, 2 * CHAR_height, 8, 15, BLACK ); //empty block
  }
  else
  {
    for (uint8_t i = 0; i < vi; i++)
      display.drawBitmap(  CHAR_width + i * 8 , 2 * CHAR_height, block_bar[7], 8, 15, WHITE, BLACK); // full block
    if (vi < display_cols - size)
      display.drawBitmap(  CHAR_width + vi * 8 , 2 * CHAR_height, block_bar[ (uint8_t)(vf / 1.25 - 0.5)  ], 8, 15, WHITE, BLACK);
    for (uint8_t i = vi + 1; i < display_cols + 4 - size; i++)
      display.fillRect( CHAR_width + i * 8, 2 * CHAR_height, 8, 15, BLACK ); //empty block
  }
}


inline void display_meter_int(const char* title, uint32_t value, float factor, float offset, int32_t min_value, int32_t max_value, uint8_t size, bool zeros, bool sign, bool init)
{
  display_meter_float(title, float(value), factor, offset, min_value, max_value, size, 0, zeros, sign, init);
}

void display_meter_float(const char* title, float value, float factor, float offset, int32_t min_value, int32_t max_value, uint8_t size_number, uint8_t size_fraction, bool zeros, bool sign, bool init)
{
  uint8_t size = 0;
  float v;
  float _vi = 0.0;
  uint8_t vf;
  uint8_t vi;

  if (size_fraction == 0)
    size = size_number;
  else
    size = size_number + size_fraction + 1;
  if (sign == true)
    size++;

  v = float((value - min_value) * (display_cols + 4 - size)) / (max_value - min_value);
  vf = uint8_t(modff(v, &_vi) * 10.0 + 0.5);
  vi = uint8_t(_vi);

  if (init == true)
  {
    // Title
    display.setCursor(CHAR_width, CHAR_height);
    display.print(title);
  }

  // Value
  display.setCursor( (display_cols - size - 1)*CHAR_width, CHAR_height * 2);
  display_float((value + offset) * factor, size_number, size_fraction, zeros, false, sign);

  // Bar

  if (vi == 0)
  {
    display.drawBitmap(  CHAR_width , 2 * CHAR_height + 4, meter_bar[ (uint8_t)(vf / 1.25 - 0.5)  ], 8, 8, WHITE, BLACK);
    for (uint8_t i = 1; i < display_cols - size; i++)
      display.fillRect( CHAR_width + i * 8, 2 * CHAR_height + 4, 8, 8, BLACK ); //empty block
  }
  else if (vi == display_cols - size)
  {
    for (uint8_t i = 0; i < display_cols - size - 1; i++)
      display.fillRect( CHAR_width + i * 8, 2 * CHAR_height + 4, 8, 8, BLACK ); //empty block
    display.drawBitmap(  CHAR_width + (display_cols - size - 1) * 8 , 2 * CHAR_height + 4, meter_bar[7], 8, 8, WHITE, BLACK);
  }
  else
  {
    for (uint8_t i = 0; i < display_cols - size + 4; i++)
      display.fillRect( CHAR_width + i * 8, 2 * CHAR_height + 4, 8, 8, BLACK ); //empty block
    display.drawBitmap(  CHAR_width + vi * 8 , 2 * CHAR_height + 4, meter_bar[ (uint8_t)(vf / 1.25 - 0.5)  ], 8, 8, WHITE, BLACK);
    for (uint8_t i = vi + 1; i < display_cols - size + 5; i++)
      display.fillRect( CHAR_width + i * 8, 2 * CHAR_height + 4, 8, 8, BLACK ); //empty block
  }
}


uint8_t bit_reverse8(uint8_t v)
{
  uint8_t result = 0;
  for ( ; v > 0; v >>= 1 )
    (result <<= 1) |= (v & 1);
  return (result);
}

void display_active_instance_number(uint8_t instance_id)
{
  for (uint8_t i = 0; i < 8; i++)
  {
    if (instance_id == 0)
    {
      if (configuration.dexed[instance_id].polyphony == 0)
        instance_num[0][i] = bit_reverse8(special_chars[0][i]);
      else
        instance_num[0][i] = special_chars[0][i];

      if (configuration.dexed[!instance_id].polyphony == 0)
      {
        instance_num[1][i] = bit_reverse8(special_chars[1][i]);
        instance_num[1][i] = ~instance_num[1][i];
      }
      else
        instance_num[1][i] = ~special_chars[1][i];
    }
    else
    {
      if (configuration.dexed[!instance_id].polyphony == 0)
      {
        instance_num[0][i] = bit_reverse8(special_chars[0][i]);
        instance_num[0][i] = ~instance_num[0][i];
      }
      else
        instance_num[0][i] = ~special_chars[0][i];

      if (configuration.dexed[instance_id].polyphony == 0)
        instance_num[1][i] = bit_reverse8(special_chars[1][i]);
      else
        instance_num[1][i] = special_chars[1][i];
    }
  }
}

void display_OP_active_instance_number(uint8_t instance_id, uint8_t op)
{
  uint8_t i, n;

  for (n = 2; n < 8; n++)
  {
    for (i = 0; i < 8; i++)
    {
      if (bitRead(op, n - 2))
        instance_num[n][i] = special_chars[n][i];
      else
        instance_num[n][i] = ~special_chars[n][i];
    }
    // lcd.createChar(n, instance_num[n]);
  }

  for (i = 0; i < 8; i++)
  {
    if (instance_id == 0)
    {
      if (configuration.dexed[instance_id].polyphony == 0)
        instance_num[0][i] = bit_reverse8(special_chars[0][i]);
      else
        instance_num[0][i] = special_chars[0][i];

      if (configuration.dexed[!instance_id].polyphony == 0)
      {
        instance_num[1][i] = bit_reverse8(special_chars[1][i]);
        instance_num[1][i] = ~instance_num[1][i];
      }
      else
        instance_num[1][i] = ~special_chars[1][i];
    }
    else
    {
      if (configuration.dexed[!instance_id].polyphony == 0)
      {
        instance_num[0][i] = bit_reverse8(special_chars[0][i]);
        instance_num[0][i] = ~instance_num[0][i];
      }
      else
        instance_num[0][i] = ~special_chars[0][i];

      if (configuration.dexed[instance_id].polyphony == 0)
        instance_num[1][i] = bit_reverse8(special_chars[1][i]);
      else
        instance_num[1][i] = special_chars[1][i];
    }
  }

}

#ifdef USE_FX
void display_delay_sync(uint8_t sync)
{
  display.show(1, 1, display_cols - 2, "Delay Sync");
  if (seq.running == false)display.show(2, 1, 10, "MIDI Sync   "); else display.show(2, 0, 10, "Seq. Sync   ");
  switch (sync)
  {
    case 1:
      display.show(2, 13, 6, "1/16");
      break;
    case 2:
      display.show(2, 13, 6, "1/16T");
      break;
    case 3:
      display.show(2, 13, 6, "1/8");
      break;
    case 4:
      display.show(2, 13, 6, "1/8T");
      break;
    case 5:
      display.show(2, 13, 6, "1/4");
      break;
    case 6:
      display.show(2, 13, 6, "1/4T");
      break;
    case 7:
      display.show(2, 13, 6, "1/2");
      break;
    case 8:
      display.show(2, 13, 6, "1/2T");
      break;
    case 9:
      display.show(2, 13, 6, "1/1");
      break;
  }
  if (seq.running == false) {

    uint16_t midi_sync_delay_time = uint16_t(60000.0 * midi_ticks_factor[sync] / midi_bpm + 0.5);
    if (midi_sync_delay_time > DELAY_MAX_TIME)
    {
#ifdef DEBUG
      Serial.println(F("Calculated MIDI-Sync delay: "));
      Serial.print(round(60000.0 * midi_ticks_factor[sync] / midi_bpm), DEC);
      Serial.println(F("ms"));
      Serial.println(F("MIDI-Sync delay: midi_sync_delay_time"));
      Serial.print(midi_sync_delay_time, DEC);
      Serial.println(F("ms"));
#endif
    }
  } else
  {
    uint16_t midi_sync_delay_time = uint16_t(60000.0 *  midi_ticks_factor[sync] / seq.bpm);
    delay_fx[selected_instance_id]->delay(0, constrain(midi_sync_delay_time, DELAY_TIME_MIN, DELAY_TIME_MAX * 10));
  }
  display.show(2, 18, 1, "!");
}
#endif

void string_trim(char *s)
{
  int i;

  while (isspace (*s)) s++;   // skip left side white spaces
  for (i = strlen (s) - 1; (isspace (s[i])); i--) ;   // skip right side white spaces
  s[i + 1] = '\0';
}

void locate_previous_non_favorite()
{
  //find prev. non fav in current bank
  display.setCursor_textGrid(4, 1);
  display.setTextColor(DX_CYAN, BLACK);
  display.print("<SEARCHING");
  display.setTextColor(WHITE, BLACK);
  do
  {
    if (configuration.dexed[selected_instance_id].voice == 0)
    {
      configuration.dexed[selected_instance_id].voice = 32; //+1
      if (configuration.dexed[selected_instance_id].bank < 1 )
        configuration.dexed[selected_instance_id].bank = MAX_BANKS - 1;

      do
      { //seek for previous bank
        configuration.dexed[selected_instance_id].bank--;
        if (configuration.dexed[selected_instance_id].bank < 1 )
          configuration.dexed[selected_instance_id].bank = MAX_BANKS - 1;
        favsearcher++;
      } while (quick_check_favorites_in_bank(configuration.dexed[selected_instance_id].bank, selected_instance_id) == true && favsearcher < 132);
    }
    configuration.dexed[selected_instance_id].voice--;
    favsearcher++;
  } while (check_favorite(configuration.dexed[selected_instance_id].bank, configuration.dexed[selected_instance_id].voice,
                          selected_instance_id) == true &&  favsearcher < 170);
  favsearcher = 0;
}

void locate_previous_favorite()
{

  // worst case, nothing found below voice 0 /  bank 0 - start loop at last bank
  if (  configuration.dexed[selected_instance_id].voice < 2  &&
        configuration.dexed[selected_instance_id].bank == 0 && favsearcher < 170)
  {
    configuration.dexed[selected_instance_id].bank = MAX_BANKS - 1;
    configuration.dexed[selected_instance_id].voice = 32;
  } else

    if (configuration.dexed[selected_instance_id].voice == 0 && configuration.dexed[selected_instance_id].bank < MAX_BANKS - 1 )
    { //if at begin of any other bank
      configuration.dexed[selected_instance_id].bank--;
      configuration.dexed[selected_instance_id].voice = 32;

    }

  if (configuration.dexed[selected_instance_id].voice >= 0 && configuration.dexed[selected_instance_id].bank >= 0) {

    display.setCursor_textGrid(4, 1);
    display.setTextColor(DX_CYAN, BLACK);
    display.print("<SEARCHING");
    display.setTextColor(WHITE, BLACK);

    do {  //first find previous fav in current bank

      if (configuration.dexed[selected_instance_id].voice == 0) {

        if (configuration.dexed[selected_instance_id].bank == 0) {
          configuration.dexed[selected_instance_id].bank = MAX_BANKS - 1;
          configuration.dexed[selected_instance_id].voice = 32;
        } else
          configuration.dexed[selected_instance_id].bank--;
        configuration.dexed[selected_instance_id].voice = 32;

      } else

        configuration.dexed[selected_instance_id].voice--;
      favsearcher++;

    } while ( check_favorite(configuration.dexed[selected_instance_id].bank, configuration.dexed[selected_instance_id].voice,
                             selected_instance_id) == false && configuration.dexed[selected_instance_id].voice >= 1 && favsearcher < 36);

    // if found, we are done. else quick check in previous banks

    if ( check_favorite(configuration.dexed[selected_instance_id].bank, configuration.dexed[selected_instance_id].voice,
                        selected_instance_id) == false && configuration.dexed[selected_instance_id].voice >= 0  &&
         configuration.dexed[selected_instance_id].bank >= 0 && favsearcher < 170)
    {
      configuration.dexed[selected_instance_id].voice = 32;

      do { //seek for previous bank
        configuration.dexed[selected_instance_id].bank--;
        favsearcher++;
      } while (quick_check_favorites_in_bank(configuration.dexed[selected_instance_id].bank, selected_instance_id) == false &&
               favsearcher < 132 &&  configuration.dexed[selected_instance_id].bank >= 0);

      do {  //last try to search if a bank with fav was found

        configuration.dexed[selected_instance_id].voice--;
        favsearcher++;
      } while ( check_favorite(configuration.dexed[selected_instance_id].bank, configuration.dexed[selected_instance_id].voice,
                               selected_instance_id) == false && configuration.dexed[selected_instance_id].voice >= 1 && favsearcher < 170);
    }
  }
  favsearcher = 0;
}

void locate_next_favorite()

{

  bool   RollOver = false;
  if (configuration.dexed[selected_instance_id].voice > 30 && configuration.dexed[selected_instance_id].bank >= MAX_BANKS - 1 )
  { //if at end of all banks
    configuration.dexed[selected_instance_id].bank = 0;
    configuration.dexed[selected_instance_id].voice = 0;
    RollOver = true;

  } else if (configuration.dexed[selected_instance_id].voice > 30 && configuration.dexed[selected_instance_id].bank < MAX_BANKS - 1 )
  { //if at end of any other bank
    configuration.dexed[selected_instance_id].bank++;
    configuration.dexed[selected_instance_id].voice = 0;
  }

  if (configuration.dexed[selected_instance_id].voice <= 30 && configuration.dexed[selected_instance_id].bank <= MAX_BANKS ) {

    display.setCursor_textGrid(4, 1);
    display.setTextColor(DX_CYAN, BLACK);
    display.print(">SEARCHING");
    display.setTextColor(WHITE, BLACK);
    do {  //first find next fav in current bank

      if (RollOver == false)  configuration.dexed[selected_instance_id].voice++; else RollOver = true;
      favsearcher++;

    } while ( check_favorite(configuration.dexed[selected_instance_id].bank, configuration.dexed[selected_instance_id].voice,
                             selected_instance_id) == false && configuration.dexed[selected_instance_id].voice <= 32 && favsearcher < 36);

    // if found, we are done. else quick check in next banks

    if ( check_favorite(configuration.dexed[selected_instance_id].bank, configuration.dexed[selected_instance_id].voice,
                        selected_instance_id) == false   &&
         configuration.dexed[selected_instance_id].bank < MAX_BANKS  && favsearcher < 170)
    {
      configuration.dexed[selected_instance_id].voice = 0;

      do { //seek in next bank

        configuration.dexed[selected_instance_id].bank++;
        if (  configuration.dexed[selected_instance_id].bank > MAX_BANKS - 1  && favsearcher < 190)
        {
          configuration.dexed[selected_instance_id].bank = 0;
          configuration.dexed[selected_instance_id].voice = 0;
        }
        favsearcher++;
      } while (quick_check_favorites_in_bank(configuration.dexed[selected_instance_id].bank, selected_instance_id) == false &&
               favsearcher < 132 );

      if ( check_favorite(configuration.dexed[selected_instance_id].bank, configuration.dexed[selected_instance_id].voice,
                          selected_instance_id) == false && configuration.dexed[selected_instance_id].voice <= 32 && favsearcher < 190)
      {
        do {  //last bank to search if a fav can be found

          configuration.dexed[selected_instance_id].voice++;
          favsearcher++;
        } while ( check_favorite(configuration.dexed[selected_instance_id].bank, configuration.dexed[selected_instance_id].voice,
                                 selected_instance_id) == false  && favsearcher < 170);
      }
    }
  }
  favsearcher = 0;

}

void locate_next_non_favorite()
{
  //find next non-fav in current bank
  display.setCursor_textGrid(4, 1);
  display.setTextColor(DX_CYAN, BLACK);
  display.print(">SEARCHING");
  display.setTextColor(WHITE, BLACK);
  do {
    configuration.dexed[selected_instance_id].voice++;
    if (configuration.dexed[selected_instance_id].voice > 31) {
      configuration.dexed[selected_instance_id].voice = 0;
      //configuration.dexed[selected_instance_id].bank++;
      if (configuration.dexed[selected_instance_id].bank > MAX_BANKS - 1)
        configuration.dexed[selected_instance_id].bank = 0;
      do { //seek for next bank
        configuration.dexed[selected_instance_id].bank++;
        if (configuration.dexed[selected_instance_id].bank > MAX_BANKS - 1)
          configuration.dexed[selected_instance_id].bank = 0;
        favsearcher++;
      } while (quick_check_favorites_in_bank(configuration.dexed[selected_instance_id].bank, selected_instance_id) == true && favsearcher < 132);
    }
    favsearcher++;
  } while ( check_favorite(configuration.dexed[selected_instance_id].bank, configuration.dexed[selected_instance_id].voice,
                           selected_instance_id) == true &&  favsearcher < 170);
  favsearcher = 0;
}

void locate_random_non_favorite()
{
  //find random non-fav
  do
  {
    configuration.dexed[selected_instance_id].voice = random(32);
    configuration.dexed[selected_instance_id].bank = random(MAX_BANKS - 1);
    favsearcher++;
  } while ( check_favorite(configuration.dexed[selected_instance_id].bank, configuration.dexed[selected_instance_id].voice,
                           selected_instance_id) == true &&  favsearcher < 100);
  favsearcher = 0;
}

bool check_favorite(uint8_t b, uint8_t v, uint8_t instance_id)
{
  b = constrain(b, 0, MAX_BANKS - 1);
  v = constrain(v, 0, MAX_VOICES - 1);
  char tmp[18];
  File myFav;
  if (sd_card > 0)
  {
    sprintf(tmp, "/%s/%d/%d.fav", FAV_CONFIG_PATH, b, v);
#ifdef DEBUG
    Serial.print("check if Voice is a Favorite: ");
    Serial.print(tmp);
    Serial.println();
#endif
    if (SD.exists(tmp))
    { //is Favorite
#ifdef DEBUG
      Serial.println(" - It is in Favorites.");
#endif
      return true;
    }
    else
    { // it was not a favorite

#ifdef DEBUG
      Serial.println(" - It is not in Favorites.");
#endif
      return false;
    }
  }
  else
    return false;
}

void draw_favorite_icon(uint8_t b, uint8_t v, uint8_t instance_id)
{
  b = constrain(b, 0, MAX_BANKS - 1);
  v = constrain(v, 0, MAX_VOICES - 1);
  char tmp[18];
  File myFav;
  if (sd_card > 0)
  {
    sprintf(tmp, "/%s/%d/%d.fav", FAV_CONFIG_PATH, b, v);
    if (SD.exists(tmp))
    { //is Favorite
      display.drawBitmap(16 * CHAR_width + 11, 5, special_chars[18], 8, 8, GREEN); //fav symbol
    }
    else
    { // it is not a favorite
      display.drawBitmap(16 * CHAR_width + 11, 5, special_chars[18], 8, 8, GREY1); //fav symbol
      //display.fillRect(16 * CHAR_width + 11, 5,8,8,BLACK);
    }
  }
}

bool quick_check_favorites_in_bank(uint8_t b, uint8_t instance_id)
{
  b = constrain(b, 0, MAX_BANKS - 1);
  char tmp[18];

  if (sd_card > 0)
  {
    sprintf(tmp, "/%s/%d", FAV_CONFIG_PATH, b);
#ifdef DEBUG
    Serial.print("check if there is a Favorite in Bank: ");
    Serial.print(tmp);
    Serial.println();
#endif
    if (SD.exists(tmp) )
    { // this bank HAS at least 1 favorite(s)
#ifdef DEBUG
      Serial.println("quickcheck found a FAV in bank!");
#endif
      return (true);
    }
    else
    { // no favorites in bank stored
      return (false);
#ifdef DEBUG
      Serial.println(" - It is no Favorite in current Bank.");
#endif
    }
  }
  else
    return false;
}

void save_favorite(uint8_t b, uint8_t v, uint8_t instance_id)
{
#ifdef DEBUG
  Serial.println("Starting saving Favorite.");
#endif
  b = constrain(b, 0, MAX_BANKS - 1);
  v = constrain(v, 0, MAX_VOICES - 1);
  char tmp[18];
  char tmpfolder[18];
  File myFav;
  uint8_t i = 0, countfavs = 0;
  if (sd_card > 0)
  {
    sprintf(tmp, "/%s/%d/%d.fav", FAV_CONFIG_PATH, b, v);
    sprintf(tmpfolder, "/%s/%d", FAV_CONFIG_PATH, b);
#ifdef DEBUG
    Serial.println("Save Favorite to SD card...");
    Serial.println(tmp);
#endif
    if (!SD.exists(tmp))
    { //create Favorite Semaphore
      if (!SD.exists(tmpfolder))
      {
        SD.mkdir(tmpfolder);
      }
      myFav = SD.open(tmp, FILE_WRITE);
      myFav.close();
      Serial.println("Favorite saved...");

      display.drawBitmap(16 * CHAR_width + 11, 5, special_chars[18], 8, 8, GREEN); //fav symbol
#ifdef DEBUG
      Serial.println("Added to Favorites...");
#endif
    }
    else
    { // delete the file, is no longer a favorite
      SD.remove(tmp);
#ifdef DEBUG
      Serial.println("Removed from Favorites...");
#endif
      for (i = 0; i < 32; i++) { //if no other favs exist in current bank, remove folder
        sprintf(tmp, "/%s/%d/%d.fav", FAV_CONFIG_PATH, b, i);
        if (SD.exists(tmp)) countfavs++;
      }
      if (countfavs == 0) {
        sprintf(tmp, "/%s/%d", FAV_CONFIG_PATH, b);
        SD.rmdir(tmp);
#ifdef DEBUG
        Serial.println("Fav count in bank:");
        Serial.print(countfavs);
        Serial.println("Removed folder since no voice in bank flagged as favorite any more");
#endif
      }

      display.fillRect(16 * CHAR_width + 11, 5, 8, 8, BLACK); ////remove fav symbol
      // display.print(" "); //remove fav symbol

#ifdef DEBUG
      Serial.println("Removed from Favorites...");
#endif
    }
  }
}

char* basename(const char* filename)
{
  char* p = strrchr (filename, '/');
  return p ? p + 1 : (char *) filename;
}
#endif
