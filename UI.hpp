/*
   MicroDexed

   MicroDexed is a port of the Dexed sound engine
   (https://github.com/asb2m10/dexed) for the Teensy-3.5/3.6/4.x with audio shield.
   Dexed ist heavily based on https://github.com/google/music-synthesizer-for-android

   (c)2018-2021 H. Wirtz <wirtz@parasitstudio.de>
   (c)2021      H. Wirtz <wirtz@parasitstudio.de>, M. Koslowski <positionhigh@gmx.de>

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
#if defined (USE_EPIANO)
#include "synth_mda_epiano.h"
#include "effect_stereo_panorama.h"
#endif

#define _LCDML_DISP_cols  display_cols
#define _LCDML_DISP_rows  display_rows

#define _LCDML_DISP_cfg_cursor                     0xda   // cursor Symbol
#define _LCDML_DISP_cfg_scrollbar                  1      // enable a scrollbar

#ifdef USE_SEQUENCER
extern PeriodicTimer sequencer_timer;
#endif
extern ILI9486_Teensy display;
extern void sequencer(void);
extern bool check_sd_performance_exists(uint8_t number);
extern SdVolume volume;

extern config_t configuration;
extern void set_volume(uint8_t v, uint8_t m);
extern bool load_sysex(uint8_t b, uint8_t v);
extern void generate_version_string(char* buffer, uint8_t len);
extern void _softRestart(void);
extern float midi_volume_transform(uint8_t midi_amp);
extern float volume_transform(float amp);
extern uint8_t selected_instance_id;
extern char receive_bank_filename[FILENAME_LEN];
extern void eeprom_update(void);
extern void reset_tracker_edit_cache(void);
extern void reset_tracker_edit_cache_current_step(void);
extern void reset_tracker_edit_cache_single_track(void);
extern void print_current_chord(void);
extern void print_merged_pattern_for_editor(int xpos, int ypos, uint8_t track_number);
extern void print_merged_pattern_fast_play_only(int xpos, int ypos, uint8_t track_number);
extern void print_merged_pattern_pianoroll(int xpos, int ypos, uint8_t track_number);
extern void update_pianoroll (int xpos, int ypos, uint8_t track_number, uint8_t cur_step);
extern void set_pattern_content_type_color(uint8_t pattern);
extern void seq_print_formatted_number(uint8_t v, uint8_t l);
extern void seq_print_current_note_from_step(uint8_t s);
extern void seq_print_step_numbers(int xpos, int ypos);
extern void print_single_pattern_pianoroll(int xpos, int ypos, uint8_t pattern,  uint8_t actstep, bool fullredraw);
extern void print_chord_name (uint8_t currentstep);
extern void print_file_manager_buttons(void);
extern uint16_t RGB24toRGB565(uint8_t r, uint8_t g, uint8_t b);
extern uint32_t ColorHSV(uint16_t hue, uint8_t sat, uint8_t val);
extern uint32_t ColorHSV2(uint16_t hue, uint8_t sat, uint8_t val);
extern void playWAVFile(const char *filename);
extern uint8_t get_song_length(void);
extern void helptext_l (const char *str);
extern void helptext_r (const char *str);
extern void seq_pattern_editor_update_dynamic_elements();

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
extern custom_midi_map_t custom_midi_map[NUM_CUSTOM_MIDI_MAPPINGS];
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
extern AudioMixer<2>*                 chorus_mixer[NUM_DEXED];
extern AudioMixer<2>*                 delay_fb_mixer[NUM_DEXED];
extern AudioEffectDelay*              delay_fx[NUM_DEXED];
extern AudioMixer<2>*                 delay_mixer[NUM_DEXED];
#endif
extern AudioEffectMonoStereo*         mono2stereo[NUM_DEXED];
//extern AudioMixer<2>                  microdexed_peak_mixer;
extern AudioAnalyzePeak               microdexed_peak_0;
extern AudioAnalyzePeak               microdexed_peak_1;
#if defined(USE_FX)
#if defined(USE_EPIANO)
extern AudioSynthEPiano               ep;
extern AudioMixer<4>                  reverb_mixer_r;
extern AudioMixer<4>                  reverb_mixer_l;
#else
extern AudioMixer<3>                  reverb_mixer_r;
extern AudioMixer<3>                  reverb_mixer_l;
#endif
#endif
extern AudioEffectPlateReverb         reverb;

#if defined(USE_FX) && defined(USE_EPIANO)
extern AudioEffectStereoPanorama       ep_stereo_panorama;
extern AudioSynthWaveform              ep_chorus_modulator;
#if MOD_FILTER_OUTPUT != MOD_NO_FILTER_OUTPUT
extern AudioFilterBiquad               ep_modchorus_filter;
#endif
extern AudioEffectModulatedDelayStereo ep_modchorus;
extern AudioMixer<2>                   ep_chorus_mixer_r;
extern AudioMixer<2>                   ep_chorus_mixer_l;
extern AudioMixer<2>                   ep_delay_fb_mixer_r;
extern AudioMixer<2>                   ep_delay_fb_mixer_l;
extern AudioEffectDelay                ep_delay_fx_r;
extern AudioEffectDelay                ep_delay_fx_l;
extern AudioMixer<2>                   ep_delay_mixer_r;
extern AudioMixer<2>                   ep_delay_mixer_l;
#endif

#if defined(USE_EPIANO)
extern AudioMixer<5>                   master_mixer_r;
extern AudioMixer<5>                   master_mixer_l;
#else
extern AudioMixer<4>                   master_mixer_r;
extern AudioMixer<4>                   master_mixer_l;
#endif
extern AudioEffectStereoMono          stereo2mono;
extern AudioAnalyzePeak               master_peak_r;
extern AudioAnalyzePeak               master_peak_l;

extern char sd_string[display_cols + 1];
extern char g_voice_name[NUM_DEXED][VOICE_NAME_LEN];
extern char g_bank_name[NUM_DEXED][BANK_NAME_LEN];
extern int perform_attack_mod[NUM_DEXED];
extern int perform_release_mod[NUM_DEXED];
extern const float midi_ticks_factor[10];
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

const char cc_names[8][12] = { "Volume     ",
                               "Panorama   ",
                               "Bank Select",
                               "Reverb Send",
                               "Seq. Start ",
                               "Seq. Stop  ",
                               "Seq. RECORD",
                               "Panic Dexed"
                             };

const uint8_t cc_dest_values[8] = { 7, 10, 32, 91, 200, 201, 202, 203 };

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
void UI_func_phSampler(uint8_t param);
void UI_func_custom_mappings(uint8_t param);
void UI_func_cc_mappings(uint8_t param);
void UI_func_seq_pattern_editor(uint8_t param);
void UI_func_seq_vel_editor(uint8_t param);
void UI_func_seq_live_transpose_oct(uint8_t param);
void UI_func_arp_shift(uint8_t param);
void UI_func_seq_chord_keys_ammount(uint8_t param);
void UI_func_seq_tempo(uint8_t param);
void UI_func_seq_tracker(uint8_t param);
void UI_func_seq_tracker_edit(uint8_t param);
void UI_func_seq_pianoroll(uint8_t param);
void UI_func_arpeggio(uint8_t param);
void UI_func_seq_mute_matrix(uint8_t param);
void UI_func_seq_track_setup(uint8_t param);
void UI_func_dexed_assign(uint8_t param);
void UI_func_seq_display_style(uint8_t param);
void UI_func_set_performance_name(uint8_t param);
void UI_func_volume(uint8_t param);
void UI_func_smart_filter(uint8_t param);
void UI_func_mixer(uint8_t param);
void UI_func_song(uint8_t param);
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
void UI_func_colors(uint8_t param);
void UI_function_not_enabled(void);
void UI_function_not_implemented(uint8_t param);
void UI_func_favorites(uint8_t param);
void UI_func_epiano_sound_intensity(uint8_t param);
void UI_func_epiano_panorama(uint8_t param);
void UI_func_epiano_decay(uint8_t param);
void UI_func_epiano_release(uint8_t param);
void UI_func_epiano_hardness(uint8_t param);
void UI_func_epiano_treble(uint8_t param);
void UI_func_epiano_stereo(uint8_t param);
void UI_func_epiano_tune(uint8_t param);
void UI_func_epiano_detune(uint8_t param);
void UI_func_epiano_pan_tremolo(uint8_t param);
void UI_func_epiano_pan_lfo(uint8_t param);
void UI_func_epiano_overdrive(uint8_t param);
void UI_func_epiano_reverb_send(uint8_t param);
void UI_func_epiano_midi_channel(uint8_t param);
void UI_func_epiano_lowest_note(uint8_t param);
void UI_func_epiano_highest_note(uint8_t param);
void UI_func_epiano_transpose(uint8_t param);
void UI_func_epiano_polyphony(uint8_t param);
void UI_func_epiano_velocity_sense(uint8_t param);
void UI_func_epiano_chorus_frequency(uint8_t param);
void UI_func_epiano_chorus_waveform(uint8_t param);
void UI_func_epiano_chorus_depth(uint8_t param);
void UI_func_epiano_chorus_level(uint8_t param);
void UI_update_instance_icons();
bool UI_select_name(uint8_t y, uint8_t x, char* edit_string, uint8_t len, bool init);
uint8_t search_accepted_char(uint8_t c);
void display_int(int16_t var, uint8_t size, bool zeros, bool brackets, bool sign);
void display_float(float var, uint8_t size_number, uint8_t size_fraction, bool zeros, bool brackets, bool sign);
void display_bar_int(const char* title, uint32_t value, float factor, int32_t min_value, int32_t max_value, uint8_t size, bool zeros, bool sign, bool init);
void display_bar_float(const char* title, float value, float factor, int32_t min_value, int32_t max_value, uint8_t size_number, uint8_t size_fraction, bool zeros, bool sign, bool init);
void display_meter_int(const char* title, uint32_t value, float factor, float offset, int32_t min_value, int32_t max_value, uint8_t size, bool zeros, bool sign, bool init);
void display_meter_float(const char* title, float value, float factor, float offset, int32_t min_value, int32_t max_value, uint8_t size_number, uint8_t size_fraction, bool zeros, bool sign, bool init);
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
void UI_func_format_flash(uint8_t param);

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
  display.drawRect(0, 0, 480, 320, GREY2);  //Main Border
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
  display.drawRect(0, CHAR_height * 6 - 4 , CHAR_width * 20, 320 - CHAR_height * 6 + 4,  GREY4);
}
void border4()  //lower right
{
  display.drawRect(CHAR_width * 20 - 1, CHAR_height * 6 - 4, 480 - CHAR_width * 20 + 1, 320 - CHAR_height * 6 + 4 ,  GREY4);
}
void border3_systext() //lower left
{
  display.drawRect(0, CHAR_height * 6 - 4 , CHAR_width * 20, 320 - CHAR_height * 6 + 4,  COLOR_SYSTEXT);
}
void border4_systext()  //lower right
{
  display.drawRect(CHAR_width * 20 - 1, CHAR_height * 6 - 4, 480 - CHAR_width * 20 + 1, 320 - CHAR_height * 6 + 4 ,  COLOR_SYSTEXT);
}
void border1_clear() //upper left
{
  display.fillRect(0 + 1, 0 + 1, CHAR_width * 20 - 2, CHAR_height * 6 - 3 - 2, COLOR_BACKGROUND);
}
void border2_clear()  //upper right
{
  display.fillRect( CHAR_width * 20 - 1 + 1, 0 + 1, 480 - CHAR_width * 20 - 1, CHAR_height * 6 - 5, COLOR_BACKGROUND);
}
void border2_half_clear()  //upper right
{
  display.fillRect( CHAR_width * 20 - 1 + 1, 0 + 1, 171, CHAR_height * 6 - 5, COLOR_BACKGROUND);
}
void border3_clear() //lower left
{
  display.fillRect(0 + 1, CHAR_height * 6 - 3 , CHAR_width * 20 - 2, 321 - CHAR_height * 6 - 5, COLOR_BACKGROUND);
}
void border4_clear()  //lower right
{
  display.fillRect(CHAR_width * 20 , CHAR_height * 6 - 3, 480 - CHAR_width * 20 - 1, 321 - CHAR_height * 6 - 5, COLOR_BACKGROUND);
}
void border3_large()  //lower left+right as one window
{
  display.drawRect(0, CHAR_height * 6 - 4 , 480, 320 - CHAR_height * 6 + 4,  GREY4);
}
void border3_large_with_buttons()  //lower left+right as one window but one line below for button info
{
  display.drawRect(0, CHAR_height * 6 - 4 , 480, 320 - CHAR_height * 7 + 3,  GREY4);
}

void border3_large_clear()  //lower left+right as one window
{
  display.fillRect(1, CHAR_height * 6 - 3 , 479, 320 - CHAR_height * 6 + 3,  COLOR_BACKGROUND);
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

void seq_sub_pat_chain_update_running_step(int x, int y)
{
  // SEQUENCER REWRITE

  //  if (seq.step == 1) {
  //    display.setTextSize(1);
  //    display.setCursor(x + 9 * CHAR_width + 6, y + (CHAR_height - 5) );
  //    display.setTextColor(COLOR_SYSTEXT, COLOR_BACKGROUND);
  //    display.print(seq.chain_active_step + 1);
  //    for (uint8_t r = 0; r < 4; r++)
  //    {
  //      if (seq.chain_active_step == r)
  //        display.drawRect(x + 5 * CHAR_width - 2 + seq.chain_active_step * 42 , y + 31, 27, 78, COLOR_SYSTEXT);
  //      else
  //        display.drawRect(x + 5 * CHAR_width - 2 + r * 42, y + 31, 27, 78, COLOR_BACKGROUND);
  //    }
  //  }

}

void print_voice_settings_in_pattern_editor(int x, int y)
{
  char bank_name[BANK_NAME_LEN];
  char voice_name[VOICE_NAME_LEN];

  if (!get_bank_name(configuration.dexed[selected_instance_id].bank, bank_name, sizeof(bank_name)))
    strcpy(bank_name, "*ERROR*");
  if (!get_voice_by_bank_name(configuration.dexed[selected_instance_id].bank, bank_name, configuration.dexed[selected_instance_id].voice, voice_name, sizeof(voice_name)))
    strcpy(voice_name, "*ERROR*");
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
  display.setCursor(x, y);
  display.setTextColor(COLOR_SYSTEXT, COLOR_BACKGROUND);
  display.print("INSTANCE");
  display.setTextColor(GREEN, COLOR_BACKGROUND);
  display.setCursor(x + 101, y);
  display.print(selected_instance_id + 1);
  display.setTextSize(1);
  display.setTextColor(COLOR_PITCHSMP, COLOR_BACKGROUND);
  display.setCursor(x + 118, y - 1);
  seq_print_formatted_number(configuration.dexed[selected_instance_id].bank, 2);
  display.setCursor(x + 118, y + 7);
  seq_print_formatted_number(configuration.dexed[selected_instance_id].voice + 1, 2);
  display.setTextColor(COLOR_SYSTEXT, COLOR_BACKGROUND);
  display.setCursor(x + 120 + 16, y - 1);
  string_toupper(bank_name);
  display.print( bank_name);
  display.setCursor(x + 120 + 16, y + 7);
  string_toupper(voice_name);
  display.print(voice_name);
  display.setTextSize(2);
}


void print_track_steps_detailed_only_current_playing_note(int xpos, int ypos, uint8_t currentstep)
{
  uint8_t i = 0;
  int y = 0;
  int x = 0;
  uint8_t z = 0;
  uint8_t array[2] = {currentstep, 99};
  display.setTextSize(1);
  display.setTextColor(GREY2, COLOR_BACKGROUND);
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
      display.setTextColor(COLOR_SYSTEXT, COLOR_PITCHSMP);
    else
      display.setTextColor(GREY2, COLOR_BACKGROUND);
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
      display.setTextColor(COLOR_SYSTEXT, COLOR_PITCHSMP);
    else
      display.setTextColor(GREY2, COLOR_BACKGROUND);
    display.setCursor(x + CHAR_width * 5, y);
    seq_print_formatted_number (seq.note_data[seq.active_pattern][i] , 3);
    // Velocity values
    if (i == currentstep)
      display.setTextColor(COLOR_SYSTEXT, COLOR_PITCHSMP);
    else
      display.setTextColor(GREY1, COLOR_BACKGROUND);
    display.setCursor(x + CHAR_width * 8, y);
    seq_print_formatted_number (seq.vel[seq.active_pattern][i] , 3);
    // Long Name / Note
    if (i == currentstep)
      display.setTextColor(COLOR_SYSTEXT, COLOR_PITCHSMP);
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
            display.setTextColor(COLOR_SYSTEXT, COLOR_PITCHSMP);
          else
            display.setTextColor(GREEN, COLOR_BACKGROUND);
          display.write (0x7E); display.print(F("LATCH")); //Tilde Symbol for latched note
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
        display.setTextColor(COLOR_SYSTEXT, COLOR_PITCHSMP);
      else
        display.setTextColor(GREY2, COLOR_BACKGROUND);
      display.print(" ");
    }
  }
  display.setTextColor(COLOR_SYSTEXT, COLOR_BACKGROUND);
}

void update_display_functions_while_seq_running()
{
  seq.UI_last_seq_step = seq.step;
  //is in UI_func_seq_pattern_editor or is in UI_func_seq_vel_editor
  if ( LCDML.FUNC_getID() == LCDML.OTHER_getIDFromFunction(UI_func_seq_pattern_editor) ||
       LCDML.FUNC_getID() == LCDML.OTHER_getIDFromFunction(UI_func_seq_vel_editor) )
  {
    if (seq.cycle_touch_element == 0)
      seq_sub_pat_chain_update_running_step(CHAR_width * 21, CHAR_height * 9);
    display.setTextSize(2);
    if (seq.step == 0)
    {
      set_pattern_content_type_color(seq.active_pattern);
      display.setCursor_textGrid(15 , 2);
      display.print(seq_find_shortname(14)[0]);
      display.setTextColor(COLOR_BACKGROUND, COLOR_SYSTEXT);
      display.setCursor_textGrid(16, 2);
      display.print(seq_find_shortname(15)[0]);
      if (seq.note_editor_view == 0 && seq.cycle_touch_element == 0)
        print_track_steps_detailed_only_current_playing_note(CHAR_width, CHAR_height * 5 + 3, 15);
    }
    else  if (seq.step == 1)
    {
      set_pattern_content_type_color(seq.active_pattern);
      display.setCursor_textGrid(16, 2);
      display.print(seq_find_shortname(15)[0]);
      display.setTextColor(COLOR_BACKGROUND, COLOR_SYSTEXT);
      display.setCursor_textGrid(1, 2);
      display.print(seq_find_shortname(0)[0]);
      if (seq.note_editor_view == 0 && seq.cycle_touch_element == 0)
        print_track_steps_detailed_only_current_playing_note(CHAR_width, CHAR_height * 5 + 3, 0);
    }
    else
    {
      set_pattern_content_type_color(seq.active_pattern);
      display.setCursor_textGrid(seq.step - 1 , 2);
      display.print(seq_find_shortname(seq.step - 2)[0]);
      display.setTextColor(COLOR_BACKGROUND, COLOR_SYSTEXT);
      display.setCursor_textGrid(seq.step , 2);
      display.print(seq_find_shortname(seq.step - 1)[0]);
      if (seq.note_editor_view == 0 && seq.cycle_touch_element == 0)
        print_track_steps_detailed_only_current_playing_note(CHAR_width, CHAR_height * 5 + 3, seq.step - 1);
    }
    display.setTextColor(COLOR_SYSTEXT, COLOR_BACKGROUND);
  }
  else if (LCDML.FUNC_getID() == LCDML.OTHER_getIDFromFunction(UI_func_arpeggio)) //is in UI of Arpeggiator
  {
    display.setCursor_textGrid(8, 1);
    print_current_chord();
  }
  else if (LCDML.FUNC_getID() == LCDML.OTHER_getIDFromFunction(UI_func_seq_tracker)) //is in UI of Tracker
  {
    seq_print_step_numbers(CHAR_width, 95);
    print_merged_pattern_fast_play_only( 4 * CHAR_width , 95,  0);
    print_merged_pattern_fast_play_only( 10 * CHAR_width , 95,  1);
    print_merged_pattern_fast_play_only( 16 * CHAR_width , 95,  2);
    print_merged_pattern_fast_play_only( 22 * CHAR_width , 95,  3);
    print_merged_pattern_fast_play_only( 28 * CHAR_width , 95,  4);
    print_merged_pattern_fast_play_only( 34 * CHAR_width , 95,  5);
  }
  else if (LCDML.FUNC_getID() == LCDML.OTHER_getIDFromFunction(UI_func_song))
  {
    display.setTextColor( GREEN, COLOR_BACKGROUND);  //play indicator song view

    if (  5 * (CHAR_height) - 5 + (CHAR_height + 1) *  ( seq.current_song_step - 1 - seq.scrollpos)    > CHAR_height * 4)
    {
      display.setCursor(CHAR_width * 3 + 8, 5 * (CHAR_height) - 5 + (CHAR_height + 1) *  ( seq.current_song_step - 1 - seq.scrollpos)  );
      display.print(" ");
    }
    if ( 5 * (CHAR_height) - 5 + (CHAR_height + 1) *  (seq.current_song_step - seq.scrollpos)   > CHAR_height * 4)
    {
      display.setCursor(CHAR_width * 3 + 8, 5 * (CHAR_height) - 5 + (CHAR_height + 1) *  (seq.current_song_step - seq.scrollpos)  );
      display.print(">");
    }
    if ( 5 * (CHAR_height) - 5 + (CHAR_height + 1) *  ( seq.current_song_step + 1 - seq.scrollpos)  > CHAR_height * 4)
    {
      display.setCursor(CHAR_width * 3 + 8, 5 * (CHAR_height) - 5 + (CHAR_height + 1) *  ( seq.current_song_step + 1 - seq.scrollpos)  );
      display.print(" ");
    }
    if (seq.current_song_step == 0)
    {
      if ( 5 * (CHAR_height) - 5 + (CHAR_height + 1) *  (get_song_length() - seq.scrollpos - 1) > CHAR_height * 4)
      {
        display.setCursor(CHAR_width * 3 + 8, 5 * (CHAR_height) - 5 + (CHAR_height + 1) *  (get_song_length() - seq.scrollpos - 1)  );
        // display.setTextColor( RED, COLOR_BACKGROUND);
        // display.print(">");
        display.print(" ");
      }
    }

  }


  // SEQUENCER REWRITE

  //  else if (LCDML.FUNC_getID() == LCDML.OTHER_getIDFromFunction(UI_func_seq_pianoroll)) //is in UI of Pianoroll
  //  {
  //    if (seq.step  == 0 && seq.chain_active_step == 0)
  //    {
  //      update_pianoroll(1 * CHAR_width , 320 - CHAR_height,  seq.active_track, 63);
  //    }
  //    else  if (seq.step  == 1 && seq.chain_active_step == 0)
  //    {
  //      update_pianoroll(1 * CHAR_width , 320 - CHAR_height,  seq.active_track, 0 + (seq.chain_active_step * 16));
  //    }
  //    else
  //    {
  //      update_pianoroll(1 * CHAR_width , 320 - CHAR_height,  seq.active_track, seq.step - 1 + (seq.chain_active_step * 16));
  //    }
  //  }

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
    display.fillScreen(COLOR_BACKGROUND);
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
    display.drawLine (qix.x0s[qix_num - 1], qix.y0s[qix_num - 1], qix.x1s[qix_num - 1], qix.y1s[qix_num - 1] , COLOR_BACKGROUND );
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
    display.fillScreen(COLOR_BACKGROUND);
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
  display.setTextColor(COLOR_SYSTEXT, COLOR_BACKGROUND);
  display.setTextSize(2);
  display.fillScreen(COLOR_BACKGROUND);

  // LCDMenuLib Setup
  LCDML_setup(_LCDML_DISP_cnt);
  // Enable Menu Rollover
  //LCDML.MENU_enRollover();
  //Enable Screensaver (screensaver menu function, time to activate in ms)

  LCDML.SCREEN_enable(mFunc_screensaver, 200000);
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
        //special case : if is in tracker edit, left ENC controls x axis, right ENC controls y axis
        if (LCDML.FUNC_getID() == LCDML.OTHER_getIDFromFunction(UI_func_seq_tracker_edit) ||
            LCDML.FUNC_getID() == LCDML.OTHER_getIDFromFunction(UI_func_song)  )
        {
          if (seq.cycle_touch_element < 5 || seq.cycle_touch_element > 9) //is not in song/chain edit
          {
            seq.selected_track++;
            if (seq.selected_track > 5)
              seq.selected_track = 0;
            reset_tracker_edit_cache_current_step();
          }
          else  if (seq.cycle_touch_element == 5 ) //is in song edit
          {
            seq.cycle_touch_element = 6; //switch to chain edit
          }
          else  if (seq.cycle_touch_element == 6 ) //is in chain edit
          {
            seq.cycle_touch_element = 8; //switch to transpose edit
          }
        }
        else
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
        //special case : if is in tracker edit, left ENC controls x axis, right ENC controls y axis
        if (LCDML.FUNC_getID() == LCDML.OTHER_getIDFromFunction(UI_func_seq_tracker_edit) ||
            LCDML.FUNC_getID() == LCDML.OTHER_getIDFromFunction(UI_func_song)  )
        {
          if (seq.cycle_touch_element < 5 || seq.cycle_touch_element > 9) //is not in song/chain edit
          {
            if (seq.selected_track == 0)
              seq.selected_track = 5;
            else
              seq.selected_track--;
            reset_tracker_edit_cache_current_step();
          }
          else  if (seq.cycle_touch_element == 8 ) //is in transpose edit
          {
            seq.cycle_touch_element = 6; //switch to chain edit
          }
          else  if (seq.cycle_touch_element == 6 ) //is in chain edit
          {
            seq.cycle_touch_element = 5; //switch to song edit
          }
        }
        else
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
              display.setTextColor(COLOR_SYSTEXT, COLOR_SYSTEXT_ACCENT);
            else
              display.setTextColor(GREY2, COLOR_BACKGROUND);
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
        display.fillRect(CHAR_width, CHAR_height * 3, CHAR_width * 18, CHAR_height * 2, COLOR_BACKGROUND);
      }
      else if (i == 3)
      {
        display.fillRect(CHAR_width, CHAR_height * 4, CHAR_width * 18, CHAR_height, COLOR_BACKGROUND);
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
          display.drawBitmap( (_LCDML_DISP_cols - 2) * CHAR_width , (n + 1) * CHAR_height, scroll_bar[0], 8, 17, COLOR_SYSTEXT, COLOR_BACKGROUND);
        }
        else
        {
          display.fillRect((_LCDML_DISP_cols - 2) * CHAR_width , (n + 1) * CHAR_height , 10, 17 , COLOR_BACKGROUND); // Clear scrollbar
        }
      }
    }
    // display scrollbar
    if (_LCDML_DISP_cfg_scrollbar == 1) {
      if (scrollbar_max > n_max) {
        //set scroll position
        if (scrollbar_cur_pos == scrollbar_min) {
          // min pos
          display.drawBitmap( (_LCDML_DISP_cols - 2) * CHAR_width , CHAR_height, scroll_bar[1], 8, 17, COLOR_SYSTEXT, COLOR_BACKGROUND);
        } else if (scrollbar_cur_pos == (scrollbar_max - 1)) {
          // max pos
          display.drawBitmap( (_LCDML_DISP_cols - 2) * CHAR_width , (n_max ) * CHAR_height, scroll_bar[4], 8, 17, COLOR_SYSTEXT, COLOR_BACKGROUND);
        } else {
          // between
          display.drawBitmap( (_LCDML_DISP_cols - 2) * CHAR_width , (scroll_pos / n_max + 1) * CHAR_height, scroll_bar[(uint8_t)(scroll_pos % n_max) + 1], 8, 17, COLOR_SYSTEXT, COLOR_BACKGROUND);
        }
      }
      else
        display.fillRect((_LCDML_DISP_cols - 2) * CHAR_width , CHAR_height * 4, 10, 17  , COLOR_BACKGROUND); //Clear scrollbar bottom if menu options are lower than row count
    }
  }
  display.setTextColor(COLOR_SYSTEXT, COLOR_BACKGROUND);
  display.fillRect(CHAR_width * 17 - 1, 5, CHAR_width * 2 + 9, 9, COLOR_BACKGROUND); //clear upper right chars/icons
}

//####################################################################################################################################################################################################

/***********************************************************************
   MENU
 ***********************************************************************/


void colors_screen_update()
{
  int y = CHAR_height;
  do
  {
    display.drawLine ( 350, y , 390, y, ColorHSV( ts.temp_col_hue * 1.15, (y - CHAR_height) * 0.95, 245)  );
    display.drawLine ( 440, y , 480, y, ColorHSV( ts.temp_col_hue * 1.15, 245, (y - CHAR_height) * 0.95)   );
    y++;
  } while (y < TFT_WIDTH - CHAR_height * 2 );


  if (seq.temp_select_menu == 0)
  {
    COLOR_SYSTEXT = ColorHSV( ts.temp_col_hue * 1.15, ts.temp_col_sat, ts.temp_col_bright) ;
    display.fillRect(CHAR_width * 11, CHAR_height, 60, CHAR_height, COLOR_SYSTEXT);
    display.setTextColor(COLOR_SYSTEXT, COLOR_BACKGROUND);
    display.setCursor_textGrid(1, 4);
    display.print(F("SYSTEM TEXT"));
  }
  else if (seq.temp_select_menu == 1)
  {
    COLOR_SYSTEXT_ACCENT = ColorHSV( ts.temp_col_hue * 1.15, ts.temp_col_sat, ts.temp_col_bright) ;
    display.fillRect(CHAR_width * 11, CHAR_height, 60, CHAR_height, COLOR_SYSTEXT_ACCENT);
    display.setTextColor(COLOR_SYSTEXT_ACCENT, COLOR_BACKGROUND);
    display.setCursor_textGrid(1, 6);
    display.print(F("SYSTEM ACCENT"));
    helptext_l("BACK");
    helptext_r("SELECT COLOR");

  }
  else if (seq.temp_select_menu == 2)
  {
    COLOR_BACKGROUND = ColorHSV( ts.temp_col_hue * 1.15, ts.temp_col_sat, ts.temp_col_bright) ;
    display.fillRect(CHAR_width * 11, CHAR_height, 60, CHAR_height, COLOR_BACKGROUND);
    display.setTextColor(COLOR_BACKGROUND, COLOR_SYSTEXT);
    display.setCursor_textGrid(1, 6);
    display.print(F("SYS BACKGRND"));

  }
  else if (seq.temp_select_menu == 3)
  {
    COLOR_DRUMS = ColorHSV( ts.temp_col_hue * 1.15, ts.temp_col_sat, ts.temp_col_bright) ;
    display.fillRect(CHAR_width * 11, CHAR_height, 60, CHAR_height, COLOR_DRUMS);
    display.setTextColor(COLOR_DRUMS, COLOR_BACKGROUND);
    display.setCursor_textGrid(1, 8);
    display.print(F("DRUMS"));
    display.setTextColor(COLOR_BACKGROUND, COLOR_DRUMS);
    display.setCursor_textGrid(11, 8);
    display.print(F("DRUMS"));
  }
  else if (seq.temp_select_menu == 4)
  {
    COLOR_PITCHSMP = ColorHSV( ts.temp_col_hue * 1.15, ts.temp_col_sat, ts.temp_col_bright) ;
    display.fillRect(CHAR_width * 11, CHAR_height, 60, CHAR_height, COLOR_PITCHSMP);
    display.setTextColor(COLOR_PITCHSMP, COLOR_BACKGROUND);
    display.setCursor_textGrid(1, 10);
    display.print(F("PITCH SMP"));
    display.setTextColor(COLOR_BACKGROUND, COLOR_PITCHSMP);
    display.setCursor_textGrid(11, 10);
    display.print(F("PITCH SMP"));
  }
  else if (seq.temp_select_menu == 5)
  {
    COLOR_CHORDS = ColorHSV( ts.temp_col_hue * 1.15, ts.temp_col_sat, ts.temp_col_bright) ;
    display.fillRect(CHAR_width * 11, CHAR_height, 60, CHAR_height, COLOR_CHORDS);
    display.setTextColor(COLOR_CHORDS, COLOR_BACKGROUND);
    display.setCursor_textGrid(1, 12);
    display.print(F("CHORD/ARP"));
    display.setTextColor(COLOR_BACKGROUND, COLOR_CHORDS);
    display.setCursor_textGrid(11, 12);
    display.print(F("CHORD/ARP"));
  }
  else if (seq.temp_select_menu == 6)
  {
    COLOR_INSTR = ColorHSV( ts.temp_col_hue * 1.15, ts.temp_col_sat, ts.temp_col_bright) ;
    display.fillRect(CHAR_width * 11, CHAR_height, 60, CHAR_height, COLOR_INSTR);
    display.setTextColor(COLOR_INSTR, COLOR_BACKGROUND);
    display.setCursor_textGrid(1, 14);
    display.print(F("INSTR"));
    display.setTextColor(COLOR_BACKGROUND, COLOR_INSTR);
    display.setCursor_textGrid(11, 14);
    display.print(F("INSTR"));
  }

}

void UI_func_colors(uint8_t param)
{
  if (LCDML.FUNC_setup())         // ****** SETUP *********
  {
    encoderDir[ENC_R].reset();
    display.setTextColor(GREY2, COLOR_BACKGROUND);
    border0();
    display.fillScreen(COLOR_BACKGROUND);
    display.setTextSize(2);

    helptext_l("BACK");
    helptext_r("SELECT COLOR");

    display.setTextColor(COLOR_SYSTEXT, COLOR_BACKGROUND);
    display.setCursor_textGrid(1, 1);
    display.print(F("COLORS"));

    display.setTextColor(COLOR_SYSTEXT, COLOR_BACKGROUND);
    display.setCursor_textGrid(1, 4);
    display.print(F("SYSTEM TEXT"));

    display.setTextColor(COLOR_SYSTEXT_ACCENT, COLOR_BACKGROUND);
    display.setCursor_textGrid(1, 6);
    display.print(F("SYSTEM ACCENT"));

    display.setTextColor(COLOR_BACKGROUND, COLOR_SYSTEXT);
    display.setCursor_textGrid(1, 8);
    display.print(F("SYS BACKGRND"));

    display.setTextColor(COLOR_DRUMS, COLOR_BACKGROUND);
    display.setCursor_textGrid(1, 10);
    display.print(F("DRUMS"));
    display.setTextColor(COLOR_BACKGROUND, COLOR_DRUMS);
    display.setCursor_textGrid(11, 10);
    display.print(F("DRUMS"));

    display.setTextColor(COLOR_PITCHSMP, COLOR_BACKGROUND);
    display.setCursor_textGrid(1, 12);
    display.print(F("PITCH SMP"));
    display.setTextColor(COLOR_BACKGROUND, COLOR_PITCHSMP);
    display.setCursor_textGrid(11, 12);
    display.print(F("PITCH SMP"));

    display.setTextColor(COLOR_CHORDS, COLOR_BACKGROUND);
    display.setCursor_textGrid(1, 14);
    display.print(F("CHORD/ARP"));
    display.setTextColor(COLOR_BACKGROUND, COLOR_CHORDS);
    display.setCursor_textGrid(11, 14);
    display.print(F("CHORD/ARP"));

    display.setTextColor(COLOR_INSTR, COLOR_BACKGROUND);
    display.setCursor_textGrid(1, 16);
    display.print(F("INSTR"));
    display.setTextColor(COLOR_BACKGROUND, COLOR_INSTR);
    display.setCursor_textGrid(11, 16);
    display.print(F("INSTR"));

    display.setTextSize(1);
    display.setTextColor(COLOR_SYSTEXT, COLOR_BACKGROUND);
    display.setCursor(260, TFT_WIDTH - CHAR_height * 2 + 3);
    display.print(F("HUE"));
    display.setCursor(350, TFT_WIDTH - CHAR_height * 2 + 3);
    display.print(F("SAT"));
    display.setCursor(440, TFT_WIDTH - CHAR_height * 2 + 3);
    display.print(F("BRIGHT"));

    display.setTextSize(2);


    int y = CHAR_height;
    do
    {
      display.drawLine ( 260, y , 300, y,  ColorHSV( (y - CHAR_height) * 1.34 , 235, 235)   );
      display.drawLine ( 350, y , 390, y,  ColorHSV( 1 , 235, (y - CHAR_height) * 0.95) );
      display.drawLine ( 440, y , 480, y,  ColorHSV( 1 , (y - CHAR_height) * 0.95, 235) );
      y++;
    } while (y < TFT_WIDTH - CHAR_height * 2 );

  }

  if (LCDML.FUNC_loop())          // ****** LOOP *********
  {
    if ((LCDML.BT_checkDown() && encoderDir[ENC_R].Down()) || (LCDML.BT_checkUp() && encoderDir[ENC_R].Up()))
    {
      if (LCDML.BT_checkDown())
        if (seq.temp_select_menu == 6)
          seq.temp_select_menu = 0;
        else
          seq.temp_select_menu = constrain(seq.temp_select_menu + ENCODER[ENC_R].speed(), 0, 6);
      else if (LCDML.BT_checkUp())
        seq.temp_select_menu = constrain(seq.temp_select_menu - ENCODER[ENC_R].speed(), 0, 6);
    }

    uint8_t y = 0;
    do
    {
      if (y == seq.temp_select_menu)
        display.drawRect ( 7, 62 + y * (CHAR_height * 2), 237, 25, COLOR_SYSTEXT_ACCENT );
      else
        display.drawRect ( 7, 62 + y * (CHAR_height * 2), 237, 25, COLOR_BACKGROUND  );
      y++;
    } while (y < 7 );

  }

  if (LCDML.FUNC_close())     // ****** STABLE END *********
  {
    encoderDir[ENC_R].reset();
    display.fillScreen(COLOR_BACKGROUND);
  }
}


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

    //master_mixer_r.gain(MASTER_MIX_CH_REVERB, pseudo_log_curve(mapfloat(configuration.fx.reverb_level, REVERB_LEVEL_MIN, REVERB_LEVEL_MAX, 0.0, 1.0)));
    //master_mixer_l.gain(MASTER_MIX_CH_REVERB, pseudo_log_curve(mapfloat(configuration.fx.reverb_level, REVERB_LEVEL_MIN, REVERB_LEVEL_MAX, 0.0, 1.0)));
    master_mixer_r.gain(MASTER_MIX_CH_REVERB, volume_transform(mapfloat(configuration.fx.reverb_level, REVERB_LEVEL_MIN, REVERB_LEVEL_MAX, 0.0, VOL_MAX_FLOAT)));
    master_mixer_l.gain(MASTER_MIX_CH_REVERB, volume_transform(mapfloat(configuration.fx.reverb_level, REVERB_LEVEL_MIN, REVERB_LEVEL_MAX, 0.0, VOL_MAX_FLOAT)));
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

void UI_func_epiano_midi_channel(uint8_t param)
{
  if (LCDML.FUNC_setup())         // ****** SETUP *********
  {
    encoderDir[ENC_R].reset();

    display.setCursor_textGrid(1, 1);
    display.print(F("EP MIDI Channel"));
  }

  if (LCDML.FUNC_loop())          // ****** LOOP *********
  {
    if (LCDML.BT_checkDown() && encoderDir[ENC_R].Down())
      configuration.epiano.midi_channel = constrain(configuration.epiano.midi_channel + ENCODER[ENC_R].speed(), EP_MIDI_CHANNEL_MIN, EP_MIDI_CHANNEL_MAX);
    else if (LCDML.BT_checkUp() && encoderDir[ENC_R].Up())
      configuration.epiano.midi_channel = constrain(configuration.epiano.midi_channel - ENCODER[ENC_R].speed(), EP_MIDI_CHANNEL_MIN, EP_MIDI_CHANNEL_MAX);

    display.setCursor_textGrid(1, 2);
    if (configuration.epiano.midi_channel == 0)
    {
      display.print(F("[OMNI]"));
    }
    else
    {
      display_int(configuration.epiano.midi_channel, 4, false, true, false);
    }
  }

  if (LCDML.FUNC_close())     // ****** STABLE END *********
  {
    encoderDir[ENC_R].reset();
  }
}

void UI_func_epiano_lowest_note(uint8_t param)
{
  char note_name[4];

  if (LCDML.FUNC_setup())         // ****** SETUP *********
  {
    encoderDir[ENC_R].reset();

    getNoteName(note_name, configuration.epiano.lowest_note);
    display.setCursor_textGrid(1, 1);
    display.print(F("EP Lowest Note"));
    display.setCursor_textGrid(1, 2);
    display.print("[");
    display.print(note_name);
    display.print("]");
  }

  if (LCDML.FUNC_loop())          // ****** LOOP *********
  {
    if ((LCDML.BT_checkDown() && encoderDir[ENC_R].Down()) || (LCDML.BT_checkUp() && encoderDir[ENC_R].Up()) || (LCDML.BT_checkEnter() && encoderDir[ENC_R].ButtonShort()))
    {
      if (LCDML.BT_checkDown())
        configuration.epiano.lowest_note = constrain(configuration.epiano.lowest_note + ENCODER[ENC_R].speed(), EP_LOWEST_NOTE_MIN, EP_LOWEST_NOTE_MAX);
      else if (LCDML.BT_checkUp())
        configuration.epiano.lowest_note = constrain(configuration.epiano.lowest_note - ENCODER[ENC_R].speed(), EP_LOWEST_NOTE_MIN, EP_LOWEST_NOTE_MAX);
    }

    getNoteName(note_name, configuration.epiano.lowest_note);
    display.setCursor_textGrid(2, 2);
    display.print(note_name);
  }

  if (LCDML.FUNC_close())     // ****** STABLE END *********
  {
    //lcd_special_chars(SCROLLBAR);
    encoderDir[ENC_R].reset();
  }
}

void UI_func_epiano_highest_note(uint8_t param)
{
  char note_name[4];

  if (LCDML.FUNC_setup())         // ****** SETUP *********
  {
    encoderDir[ENC_R].reset();

    getNoteName(note_name, configuration.dexed[selected_instance_id].highest_note);
    display.setCursor_textGrid(1, 1);
    display.print(F("EP Highest Note"));
    display.setCursor_textGrid(1, 2);
    display.print("[");
    display.print(note_name);
    display.print("]");
  }

  if (LCDML.FUNC_loop())          // ****** LOOP *********
  {
    if ((LCDML.BT_checkDown() && encoderDir[ENC_R].Down()) || (LCDML.BT_checkUp() && encoderDir[ENC_R].Up()) || (LCDML.BT_checkEnter() && encoderDir[ENC_R].ButtonShort()))
    {
      if (LCDML.BT_checkDown())
        configuration.epiano.highest_note = constrain(configuration.epiano.highest_note + ENCODER[ENC_R].speed(), EP_HIGHEST_NOTE_MIN, EP_HIGHEST_NOTE_MAX);
      else if (LCDML.BT_checkUp())
        configuration.epiano.highest_note = constrain(configuration.epiano.highest_note - ENCODER[ENC_R].speed(), EP_HIGHEST_NOTE_MIN, EP_HIGHEST_NOTE_MAX);
    }

    getNoteName(note_name, configuration.epiano.highest_note);
    display.setCursor_textGrid(2, 2);
    display.print(note_name);
  }

  if (LCDML.FUNC_close())     // ****** STABLE END *********
  {
    //lcd_special_chars(SCROLLBAR);
    encoderDir[ENC_R].reset();
  }
}

void UI_func_epiano_sound_intensity(uint8_t param)
{
  if (LCDML.FUNC_setup())         // ****** SETUP *********
  {
    encoderDir[ENC_R].reset();

    //lcd_special_chars(BLOCKBAR);
#if defined(USE_EPIANO)
    display_bar_int("EP Level", configuration.epiano.sound_intensity, 1.0, EP_SOUND_INTENSITY_MIN, EP_SOUND_INTENSITY_MAX, 3, false, false, true);
#else
    {
      display.fillScreen(COLOR_BACKGROUND);
      display.setCursor_textGrid(1, 1);
      display.print(F("EP Disabled"));
    }
#endif
  }

  if (LCDML.FUNC_loop())          // ****** LOOP *********
  {
    if ((LCDML.BT_checkDown() && encoderDir[ENC_R].Down()) || (LCDML.BT_checkUp() && encoderDir[ENC_R].Up()) || (LCDML.BT_checkEnter() && encoderDir[ENC_R].ButtonShort()))
    {
      encoderDir[ENC_R].reset();

      if (LCDML.BT_checkDown())
      {
        configuration.epiano.sound_intensity = constrain(configuration.epiano.sound_intensity + ENCODER[ENC_R].speed(), EP_SOUND_INTENSITY_MIN, EP_SOUND_INTENSITY_MAX);
        MD_sendControlChange(configuration.epiano.midi_channel, 7, configuration.epiano.sound_intensity);
      }
      else if (LCDML.BT_checkUp())
      {
        configuration.epiano.sound_intensity = constrain(configuration.epiano.sound_intensity - ENCODER[ENC_R].speed(), EP_SOUND_INTENSITY_MIN, EP_SOUND_INTENSITY_MAX);
        MD_sendControlChange(configuration.epiano.midi_channel, 7, configuration.epiano.sound_intensity);
      }
    }

    display_bar_int("EP Level", configuration.epiano.sound_intensity, 1.0, EP_SOUND_INTENSITY_MIN, EP_SOUND_INTENSITY_MAX, 3, false, false, false);
#if defined(USE_EPIANO)
    ep.setVolume(mapfloat(configuration.epiano.sound_intensity, EP_SOUND_INTENSITY_MIN, EP_SOUND_INTENSITY_MAX, 0, 1.0));
#endif
  }

  if (LCDML.FUNC_close())     // ****** STABLE END *********
  {
    //lcd_special_chars(SCROLLBAR);
    encoderDir[ENC_R].reset();
  }
}

void UI_func_epiano_panorama(uint8_t param)
{
  if (LCDML.FUNC_setup())         // ****** SETUP *********
  {
    encoderDir[ENC_R].reset();

    if (configuration.sys.mono > 0)
    {
      display.setCursor_textGrid(1, 1);
      display.print(F("EP Panorama"));
      display.setCursor_textGrid(1, 2);
      display.print(F("MONO-disabled"));
      return;
    }

    //lcd_special_chars(METERBAR);
    display_meter_float("EP Panorama", configuration.epiano.pan, 0.05, -20.0, EP_PANORAMA_MIN, EP_PANORAMA_MAX, 1, 1, false, true, true);
  }

  if (LCDML.FUNC_loop())          // ****** LOOP *********
  {
    if (LCDML.BT_checkDown() && encoderDir[ENC_R].Down() && configuration.sys.mono == 0)
    {
      configuration.epiano.pan = constrain(configuration.epiano.pan + ENCODER[ENC_R].speed(), EP_PANORAMA_MIN, EP_PANORAMA_MAX);
      MD_sendControlChange(configuration.epiano.midi_channel, 10, map(configuration.epiano.pan, EP_PANORAMA_MIN, EP_PANORAMA_MAX, 0, 127));
    }
    else if (LCDML.BT_checkUp() && encoderDir[ENC_R].Up() && configuration.sys.mono == 0)
    {
      configuration.epiano.pan = constrain(configuration.epiano.pan - ENCODER[ENC_R].speed(), EP_PANORAMA_MIN, EP_PANORAMA_MAX);
      MD_sendControlChange(configuration.epiano.midi_channel, 10, map(configuration.epiano.pan, EP_PANORAMA_MIN, EP_PANORAMA_MAX, 0, 127));
    }

    if (configuration.sys.mono == 0)
    {
      display_meter_float("EP Panorama", configuration.epiano.pan, 0.05, -20.0, EP_PANORAMA_MIN, EP_PANORAMA_MAX, 1, 1, false, true, false);
#if defined(USE_EPIANO)
      ep_stereo_panorama.panorama(mapfloat(configuration.epiano.pan, PANORAMA_MIN, PANORAMA_MAX, -1.0, 1.0));
#endif
    }
  }

  if (LCDML.FUNC_close())     // ****** STABLE END *********
  {
    //lcd_special_chars(SCROLLBAR);
    encoderDir[ENC_R].reset();
  }
}

void UI_func_epiano_decay(uint8_t param)
{
  if (LCDML.FUNC_setup())         // ****** SETUP *********
  {
    encoderDir[ENC_R].reset();

    //lcd_special_chars(BLOCKBAR);
    display_bar_int("EP Decay", configuration.epiano.decay, 1.0, EP_DECAY_MIN, EP_DECAY_MAX, 3, false, false, true);
  }

  if (LCDML.FUNC_loop())          // ****** LOOP *********
  {
    if ((LCDML.BT_checkDown() && encoderDir[ENC_R].Down()) || (LCDML.BT_checkUp() && encoderDir[ENC_R].Up()) || (LCDML.BT_checkEnter() && encoderDir[ENC_R].ButtonShort()))
    {
      encoderDir[ENC_R].reset();

      if (LCDML.BT_checkDown())
      {
        configuration.epiano.decay = constrain(configuration.epiano.decay + ENCODER[ENC_R].speed(), EP_DECAY_MIN, EP_DECAY_MAX);
      }
      else if (LCDML.BT_checkUp())
      {
        configuration.epiano.decay = constrain(configuration.epiano.decay - ENCODER[ENC_R].speed(), EP_DECAY_MIN, EP_DECAY_MAX);
      }
    }

    display_bar_int("EP Decay", configuration.epiano.decay, 1.0, EP_DECAY_MIN, EP_DECAY_MAX, 3, false, false, false);
#if defined(USE_EPIANO)
    ep.setDecay(mapfloat(configuration.epiano.decay, EP_DECAY_MIN, EP_DECAY_MAX, 0, 1.0));
#endif
  }

  if (LCDML.FUNC_close())     // ****** STABLE END *********
  {
    encoderDir[ENC_R].reset();
  }
}

void UI_func_epiano_release(uint8_t param)
{
  if (LCDML.FUNC_setup())         // ****** SETUP *********
  {
    encoderDir[ENC_R].reset();
    display_bar_int("EP Release", configuration.epiano.release, 1.0, EP_RELEASE_MIN, EP_RELEASE_MAX, 3, false, false, true);
  }

  if (LCDML.FUNC_loop())          // ****** LOOP *********
  {
    if ((LCDML.BT_checkDown() && encoderDir[ENC_R].Down()) || (LCDML.BT_checkUp() && encoderDir[ENC_R].Up()))
    {
      encoderDir[ENC_R].reset();

      if (LCDML.BT_checkDown())
      {
        configuration.epiano.release = constrain(configuration.epiano.release + ENCODER[ENC_R].speed(), EP_RELEASE_MIN, EP_RELEASE_MAX);
      }
      else if (LCDML.BT_checkUp())
      {
        configuration.epiano.release = constrain(configuration.epiano.release - ENCODER[ENC_R].speed(), EP_RELEASE_MIN, EP_RELEASE_MAX);
      }
    }

    display_bar_int("EP Release", configuration.epiano.release, 1.0, EP_RELEASE_MIN, EP_RELEASE_MAX, 3, false, false, false);
#if defined(USE_EPIANO)
    ep.setRelease(mapfloat(configuration.epiano.release, EP_RELEASE_MIN, EP_RELEASE_MAX, 0, 1.0));
#endif
  }

  if (LCDML.FUNC_close())     // ****** STABLE END *********
  {
    //lcd_special_chars(SCROLLBAR);
    encoderDir[ENC_R].reset();
  }
}

void UI_func_epiano_hardness(uint8_t param)
{
  if (LCDML.FUNC_setup())         // ****** SETUP *********
  {
    encoderDir[ENC_R].reset();
    display_bar_int("EP Hardness", configuration.epiano.hardness, 1.0, EP_HARDNESS_MIN, EP_HARDNESS_MAX, 3, false, false, true);
  }

  if (LCDML.FUNC_loop())          // ****** LOOP *********
  {
    if ((LCDML.BT_checkDown() && encoderDir[ENC_R].Down()) || (LCDML.BT_checkUp() && encoderDir[ENC_R].Up()) || (LCDML.BT_checkEnter() && encoderDir[ENC_R].ButtonShort()))
    {
      encoderDir[ENC_R].reset();

      if (LCDML.BT_checkDown())
      {
        configuration.epiano.hardness = constrain(configuration.epiano.hardness + ENCODER[ENC_R].speed(), EP_HARDNESS_MIN, EP_HARDNESS_MAX);
      }
      else if (LCDML.BT_checkUp())
      {
        configuration.epiano.hardness = constrain(configuration.epiano.hardness - ENCODER[ENC_R].speed(), EP_HARDNESS_MIN, EP_HARDNESS_MAX);
      }
    }

    display_bar_int("EP Hardness", configuration.epiano.hardness, 1.0, EP_HARDNESS_MIN, EP_HARDNESS_MAX, 3, false, false, false);
#if defined(USE_EPIANO)
    ep.setHardness(mapfloat(configuration.epiano.hardness, EP_HARDNESS_MIN, EP_HARDNESS_MAX, 0, 1.0));
#endif
  }

  if (LCDML.FUNC_close())     // ****** STABLE END *********
  {
    encoderDir[ENC_R].reset();
  }
}

void UI_func_epiano_treble(uint8_t param)
{
  if (LCDML.FUNC_setup())         // ****** SETUP *********
  {
    encoderDir[ENC_R].reset();

    display_bar_int("EP Treble", configuration.epiano.treble, 1.0, EP_TREBLE_MIN, EP_TREBLE_MAX, 3, false, false, true);
  }

  if (LCDML.FUNC_loop())          // ****** LOOP *********
  {
    if ((LCDML.BT_checkDown() && encoderDir[ENC_R].Down()) || (LCDML.BT_checkUp() && encoderDir[ENC_R].Up()) || (LCDML.BT_checkEnter() && encoderDir[ENC_R].ButtonShort()))
    {
      encoderDir[ENC_R].reset();

      if (LCDML.BT_checkDown())
      {
        configuration.epiano.treble = constrain(configuration.epiano.treble + ENCODER[ENC_R].speed(), EP_TREBLE_MIN, EP_TREBLE_MAX);
      }
      else if (LCDML.BT_checkUp())
      {
        configuration.epiano.treble = constrain(configuration.epiano.treble - ENCODER[ENC_R].speed(), EP_TREBLE_MIN, EP_TREBLE_MAX);
      }
    }

    display_bar_int("EP Treble", configuration.epiano.treble, 1.0, EP_TREBLE_MIN, EP_TREBLE_MAX, 3, false, false, false);
#if defined(USE_EPIANO)
    ep.setTreble(mapfloat(configuration.epiano.treble, EP_TREBLE_MIN, EP_TREBLE_MAX, 0, 1.0));
#endif
  }

  if (LCDML.FUNC_close())     // ****** STABLE END *********
  {
    encoderDir[ENC_R].reset();
  }
}

void UI_func_epiano_stereo(uint8_t param)
{
  if (LCDML.FUNC_setup())         // ****** SETUP *********
  {
    encoderDir[ENC_R].reset();
    display_bar_int("EP Stereo", configuration.epiano.stereo, 1.0, EP_STEREO_MIN, EP_STEREO_MAX, 3, false, false, true);
  }

  if (LCDML.FUNC_loop())          // ****** LOOP *********
  {
    if ((LCDML.BT_checkDown() && encoderDir[ENC_R].Down()) || (LCDML.BT_checkUp() && encoderDir[ENC_R].Up()) || (LCDML.BT_checkEnter() && encoderDir[ENC_R].ButtonShort()))
    {
      encoderDir[ENC_R].reset();

      if (LCDML.BT_checkDown())
      {
        configuration.epiano.stereo = constrain(configuration.epiano.stereo + ENCODER[ENC_R].speed(), EP_STEREO_MIN, EP_STEREO_MAX);
      }
      else if (LCDML.BT_checkUp())
      {
        configuration.epiano.stereo = constrain(configuration.epiano.stereo - ENCODER[ENC_R].speed(), EP_STEREO_MIN, EP_STEREO_MAX);
      }
    }

    display_bar_int("EP Stereo", configuration.epiano.stereo, 1.0, EP_STEREO_MIN, EP_STEREO_MAX, 3, false, false, false);
#if defined(USE_EPIANO)
    ep.setStereo(mapfloat(configuration.epiano.stereo, EP_STEREO_MIN, EP_STEREO_MAX, 0, 1.0));
#endif
  }

  if (LCDML.FUNC_close())     // ****** STABLE END *********
  {
    encoderDir[ENC_R].reset();
  }
}

void UI_func_epiano_tune(uint8_t param)
{
  if (LCDML.FUNC_setup())         // ****** SETUP *********
  {
    encoderDir[ENC_R].reset();
    display_meter_int("EP Tune", configuration.epiano.tune, 1.0, -100.0, EP_TUNE_MIN, EP_TUNE_MAX, 3, false, true, true);
  }

  if (LCDML.FUNC_loop())          // ****** LOOP *********
  {
    if ((LCDML.BT_checkDown() && encoderDir[ENC_R].Down()) || (LCDML.BT_checkUp() && encoderDir[ENC_R].Up()) || (LCDML.BT_checkEnter() && encoderDir[ENC_R].ButtonShort()))
    {
      if (LCDML.BT_checkDown())
      {
        configuration.epiano.tune = constrain(configuration.epiano.tune + ENCODER[ENC_R].speed(), EP_TUNE_MIN, EP_TUNE_MAX);
        MD_sendControlChange(configuration.epiano.midi_channel, 94, configuration.epiano.tune);
      }
      else if (LCDML.BT_checkUp())
      {
        configuration.epiano.tune = constrain(configuration.epiano.tune - ENCODER[ENC_R].speed(), EP_TUNE_MIN, EP_TUNE_MAX);
        MD_sendControlChange(configuration.epiano.midi_channel, 94, configuration.epiano.tune);
      }
    }

    display_meter_int("EP Tune", configuration.epiano.tune, 1.0, -100.0, EP_TUNE_MIN, EP_TUNE_MAX, 3, false, true, false);
#if defined(USE_EPIANO)
    ep.setTune((configuration.epiano.tune - 100) / 100.0);
#endif
  }

  if (LCDML.FUNC_close())     // ****** STABLE END *********
  {
    encoderDir[ENC_R].reset();
  }
}

void UI_func_epiano_detune(uint8_t param)
{
  if (LCDML.FUNC_setup())         // ****** SETUP *********
  {
    encoderDir[ENC_R].reset();
    display_bar_int("EP Detune", configuration.epiano.detune, 1.0, EP_DETUNE_MIN, EP_DETUNE_MAX, 3, false, false, true);
  }

  if (LCDML.FUNC_loop())          // ****** LOOP *********
  {
    if ((LCDML.BT_checkDown() && encoderDir[ENC_R].Down()) || (LCDML.BT_checkUp() && encoderDir[ENC_R].Up()) || (LCDML.BT_checkEnter() && encoderDir[ENC_R].ButtonShort()))
    {
      encoderDir[ENC_R].reset();

      if (LCDML.BT_checkDown())
      {
        configuration.epiano.detune = constrain(configuration.epiano.detune + ENCODER[ENC_R].speed(), EP_DETUNE_MIN, EP_DETUNE_MAX);
      }
      else if (LCDML.BT_checkUp())
      {
        configuration.epiano.detune = constrain(configuration.epiano.detune - ENCODER[ENC_R].speed(), EP_DETUNE_MIN, EP_DETUNE_MAX);
      }
    }

    display_bar_int("EP Detune", configuration.epiano.detune, 1.0, EP_DETUNE_MIN, EP_DETUNE_MAX, 3, false, false, false);
#if defined(USE_EPIANO)
    ep.setDetune(mapfloat(configuration.epiano.detune, EP_DETUNE_MIN, EP_DETUNE_MAX, 0, 1.0));
#endif
  }

  if (LCDML.FUNC_close())     // ****** STABLE END *********
  {
    encoderDir[ENC_R].reset();
  }
}

void UI_func_epiano_pan_tremolo(uint8_t param)
{
  if (LCDML.FUNC_setup())         // ****** SETUP *********
  {
    encoderDir[ENC_R].reset();
    display_bar_int("EP Trem. Width", configuration.epiano.pan_tremolo, 1.0, EP_PAN_TREMOLO_MIN, EP_PAN_TREMOLO_MAX, 3, false, false, true);
  }

  if (LCDML.FUNC_loop())          // ****** LOOP *********
  {
    if ((LCDML.BT_checkDown() && encoderDir[ENC_R].Down()) || (LCDML.BT_checkUp() && encoderDir[ENC_R].Up()))
    {
      encoderDir[ENC_R].reset();

      if (LCDML.BT_checkDown())
      {
        configuration.epiano.pan_tremolo = constrain(configuration.epiano.pan_tremolo + ENCODER[ENC_R].speed(), EP_PAN_TREMOLO_MIN, EP_PAN_TREMOLO_MAX);
      }
      else if (LCDML.BT_checkUp())
      {
        configuration.epiano.pan_tremolo = constrain(configuration.epiano.pan_tremolo - ENCODER[ENC_R].speed(), EP_PAN_TREMOLO_MIN, EP_PAN_TREMOLO_MAX);
      }
    }

    display_bar_int("EP Trem. Width", configuration.epiano.pan_tremolo, 1.0, EP_PAN_TREMOLO_MIN, EP_PAN_TREMOLO_MAX, 3, false, false, false);
#if defined(USE_EPIANO)
    if (configuration.epiano.pan_tremolo == 0)
      ep.setPanTremolo(0.0);
    else
      ep.setPanTremolo(mapfloat(configuration.epiano.pan_tremolo, EP_PAN_TREMOLO_MIN, EP_PAN_TREMOLO_MAX, 0.0, 1.0));
#endif
  }

  if (LCDML.FUNC_close())     // ****** STABLE END *********
  {
    encoderDir[ENC_R].reset();
  }
}

void UI_func_epiano_pan_lfo(uint8_t param)
{
  if (LCDML.FUNC_setup())         // ****** SETUP *********
  {
    encoderDir[ENC_R].reset();
    display_bar_int("EP LFO", configuration.epiano.pan_lfo, 1.0, EP_PAN_LFO_MIN, EP_PAN_LFO_MAX, 3, false, false, true);
  }

  if (LCDML.FUNC_loop())          // ****** LOOP *********
  {
    if ((LCDML.BT_checkDown() && encoderDir[ENC_R].Down()) || (LCDML.BT_checkUp() && encoderDir[ENC_R].Up()))
    {
      encoderDir[ENC_R].reset();

      if (LCDML.BT_checkDown())
      {
        configuration.epiano.pan_lfo = constrain(configuration.epiano.pan_lfo + ENCODER[ENC_R].speed(), EP_PAN_LFO_MIN, EP_PAN_LFO_MAX);
      }
      else if (LCDML.BT_checkUp())
      {
        configuration.epiano.pan_lfo = constrain(configuration.epiano.pan_lfo - ENCODER[ENC_R].speed(), EP_PAN_LFO_MIN, EP_PAN_LFO_MAX);
      }
    }

    display_bar_int("EP LFO", configuration.epiano.pan_lfo, 1.0, EP_PAN_LFO_MIN, EP_PAN_LFO_MAX, 3, false, false, false);
#if defined(USE_EPIANO)
    if (configuration.epiano.pan_lfo == 0)
      ep.setPanLFO(0.0);
    else
      ep.setPanLFO(mapfloat(configuration.epiano.pan_lfo, EP_PAN_LFO_MIN, EP_PAN_LFO_MAX, 0.0, 1.0));
#endif
  }

  if (LCDML.FUNC_close())     // ****** STABLE END *********
  {
    encoderDir[ENC_R].reset();
  }
}

void UI_func_epiano_overdrive(uint8_t param)
{
  if (LCDML.FUNC_setup())         // ****** SETUP *********
  {
    encoderDir[ENC_R].reset();

    //lcd_special_chars(BLOCKBAR);
    display_bar_int("EP Overdrive", configuration.epiano.overdrive, 1.0, EP_OVERDRIVE_MIN, EP_OVERDRIVE_MAX, 3, false, false, true);
  }

  if (LCDML.FUNC_loop())          // ****** LOOP *********
  {
    if ((LCDML.BT_checkDown() && encoderDir[ENC_R].Down()) || (LCDML.BT_checkUp() && encoderDir[ENC_R].Up()))
    {
      encoderDir[ENC_R].reset();

      if (LCDML.BT_checkDown())
      {
        configuration.epiano.overdrive = constrain(configuration.epiano.overdrive + ENCODER[ENC_R].speed(), EP_OVERDRIVE_MIN, EP_OVERDRIVE_MAX);
      }
      else if (LCDML.BT_checkUp())
      {
        configuration.epiano.overdrive = constrain(configuration.epiano.overdrive - ENCODER[ENC_R].speed(), EP_OVERDRIVE_MIN, EP_OVERDRIVE_MAX);
      }
    }

    display_bar_int("EP Overdrive", configuration.epiano.overdrive, 1.0, EP_OVERDRIVE_MIN, EP_OVERDRIVE_MAX, 3, false, false, false);
#if defined(USE_EPIANO)
    ep.setOverdrive(mapfloat(configuration.epiano.overdrive, EP_OVERDRIVE_MIN, EP_OVERDRIVE_MAX, 0, 1.0));
#endif
  }

  if (LCDML.FUNC_close())     // ****** STABLE END *********
  {
    //lcd_special_chars(SCROLLBAR);
    encoderDir[ENC_R].reset();
  }
}

void UI_func_epiano_transpose(uint8_t param)
{
  if (LCDML.FUNC_setup())         // ****** SETUP *********
  {
    encoderDir[ENC_R].reset();

    //lcd_special_chars(METERBAR);
    display_meter_int("EP Transpose", configuration.epiano.transpose, 1.0, -24.0, EP_TRANSPOSE_MIN, EP_TRANSPOSE_MAX, 2, false, true, true);
  }

  if (LCDML.FUNC_loop())          // ****** LOOP *********
  {
    if ((LCDML.BT_checkDown() && encoderDir[ENC_R].Down()) || (LCDML.BT_checkUp() && encoderDir[ENC_R].Up()) || (LCDML.BT_checkEnter() && encoderDir[ENC_R].ButtonShort()))
    {
      if (LCDML.BT_checkDown())
        configuration.epiano.transpose = constrain(configuration.epiano.transpose + ENCODER[ENC_R].speed(), EP_TRANSPOSE_MIN, EP_TRANSPOSE_MAX);
      else if (LCDML.BT_checkUp())
        configuration.epiano.transpose = constrain(configuration.epiano.transpose - ENCODER[ENC_R].speed(), EP_TRANSPOSE_MIN, EP_TRANSPOSE_MAX);
    }

    display_meter_int("EP Transpose", configuration.epiano.transpose, 1.0, -24.0, EP_TRANSPOSE_MIN, EP_TRANSPOSE_MAX, 2, false, true, true);
  }

  if (LCDML.FUNC_close())     // ****** STABLE END *********
  {
    //lcd_special_chars(SCROLLBAR);
    encoderDir[ENC_R].reset();
  }
}

void UI_func_epiano_polyphony(uint8_t param)
{
  if (LCDML.FUNC_setup())         // ****** SETUP *********
  {
    encoderDir[ENC_R].reset();

    //lcd_special_chars(BLOCKBAR);
    display_bar_int("EP Polyphony", configuration.epiano.polyphony, 1.0, EP_POLYPHONY_MIN, EP_POLYPHONY_MAX, 2, false, false, true);
  }

  if (LCDML.FUNC_loop())          // ****** LOOP *********
  {
    if ((LCDML.BT_checkDown() && encoderDir[ENC_R].Down()) || (LCDML.BT_checkUp() && encoderDir[ENC_R].Up()))
    {
      if (LCDML.BT_checkDown())
      {
        configuration.epiano.polyphony = constrain(configuration.epiano.polyphony + 1, EP_POLYPHONY_MIN, EP_POLYPHONY_MAX);
      }
      else if (LCDML.BT_checkUp())
      {
        if (configuration.epiano.polyphony - 1 < 1)
          configuration.epiano.polyphony = 1;
        else
        {
          configuration.epiano.polyphony = constrain(configuration.epiano.polyphony - 1, EP_POLYPHONY_MIN, EP_POLYPHONY_MAX);
        }
      }
    }

    display_bar_int("EP Polyphony", configuration.epiano.polyphony, 1.0, EP_POLYPHONY_MIN, EP_POLYPHONY_MAX, 2, false, false, false);
#if defined(USE_EPIANO)
    ep.setPolyphony(configuration.epiano.polyphony);
#endif
  }

  if (LCDML.FUNC_close())     // ****** STABLE END *********
  {
    encoderDir[ENC_R].reset();
  }
}

void UI_func_epiano_velocity_sense(uint8_t param)
{
  if (LCDML.FUNC_setup())         // ****** SETUP *********
  {
    encoderDir[ENC_R].reset();
    display_bar_int("EP Vel. Sense", configuration.epiano.velocity_sense, 1.0, EP_VELOCITY_SENSE_MIN, EP_VELOCITY_SENSE_MAX, 3, false, false, true);
  }

  if (LCDML.FUNC_loop())          // ****** LOOP *********
  {
    if ((LCDML.BT_checkDown() && encoderDir[ENC_R].Down()) || (LCDML.BT_checkUp() && encoderDir[ENC_R].Up()) || (LCDML.BT_checkEnter() && encoderDir[ENC_R].ButtonShort()))
    {
      encoderDir[ENC_R].reset();

      if (LCDML.BT_checkDown())
      {
        configuration.epiano.velocity_sense = constrain(configuration.epiano.velocity_sense + ENCODER[ENC_R].speed(), EP_VELOCITY_SENSE_MIN, EP_VELOCITY_SENSE_MAX);
      }
      else if (LCDML.BT_checkUp())
      {
        configuration.epiano.velocity_sense = constrain(configuration.epiano.velocity_sense - ENCODER[ENC_R].speed(), EP_VELOCITY_SENSE_MIN, EP_VELOCITY_SENSE_MAX);
      }
    }

    display_bar_int("EP Vel. Sense", configuration.epiano.velocity_sense, 1.0, EP_VELOCITY_SENSE_MIN, EP_VELOCITY_SENSE_MAX, 3, false, false, false);
#if defined(USE_EPIANO)
    ep.setVelocitySense(mapfloat(configuration.epiano.velocity_sense, EP_VELOCITY_SENSE_MIN, EP_VELOCITY_SENSE_MAX, 0, 1.0));
#endif
  }

  if (LCDML.FUNC_close())     // ****** STABLE END *********
  {
    encoderDir[ENC_R].reset();
  }
}

#ifdef USE_PLATEREVERB
void UI_func_epiano_reverb_send(uint8_t param)
{
  if (LCDML.FUNC_setup())         // ****** SETUP *********
  {
    encoderDir[ENC_R].reset();
    display_bar_int("EP Reverb Send", configuration.fx.ep_reverb_send, 1.0, EP_REVERB_SEND_MIN, EP_REVERB_SEND_MAX, 3, false, false, true);
  }

  if (LCDML.FUNC_loop())          // ****** LOOP *********
  {
    if ((LCDML.BT_checkDown() && encoderDir[ENC_R].Down()) || (LCDML.BT_checkUp() && encoderDir[ENC_R].Up()) || (LCDML.BT_checkEnter() && encoderDir[ENC_R].ButtonShort()))
    {
      if (LCDML.BT_checkDown())
      {
        configuration.fx.ep_reverb_send = constrain(configuration.fx.ep_reverb_send + ENCODER[ENC_R].speed(), EP_REVERB_SEND_MIN, EP_REVERB_SEND_MAX);
        MD_sendControlChange(configuration.epiano.midi_channel, 91, configuration.fx.ep_reverb_send);
      }
      else if (LCDML.BT_checkUp())
      {
        configuration.fx.ep_reverb_send = constrain(configuration.fx.ep_reverb_send - ENCODER[ENC_R].speed(), EP_REVERB_SEND_MIN, EP_REVERB_SEND_MAX);
        MD_sendControlChange(configuration.epiano.midi_channel, 91, configuration.fx.ep_reverb_send);
      }
    }

    display_bar_int("EP Reverb Send", configuration.fx.ep_reverb_send, 1.0, EP_REVERB_SEND_MIN, EP_REVERB_SEND_MAX, 3, false, false, false);

    reverb_mixer_r.gain(REVERB_MIX_CH_EPIANO, volume_transform(mapfloat(configuration.fx.ep_reverb_send, EP_REVERB_SEND_MIN, EP_REVERB_SEND_MAX, 0.0, VOL_MAX_FLOAT)));
    reverb_mixer_l.gain(REVERB_MIX_CH_EPIANO, volume_transform(mapfloat(configuration.fx.ep_reverb_send, EP_REVERB_SEND_MIN, EP_REVERB_SEND_MAX, 0.0, VOL_MAX_FLOAT)));
  }

  if (LCDML.FUNC_close())     // ****** STABLE END *********
  {
    //lcd_special_chars(SCROLLBAR);
    encoderDir[ENC_R].reset();
  }
}
#endif

void UI_func_epiano_chorus_frequency(uint8_t param)
{
  if (LCDML.FUNC_setup())         // ****** SETUP *********
  {
    encoderDir[ENC_R].reset();

    //lcd_special_chars(BLOCKBAR);
    display_bar_float("EP Chorus Frq.", configuration.fx.ep_chorus_frequency, 0.1, EP_CHORUS_FREQUENCY_MIN, EP_CHORUS_FREQUENCY_MAX, 2, 1, false, false, true);
  }

  if (LCDML.FUNC_loop())          // ****** LOOP *********
  {
    if ((LCDML.BT_checkDown() && encoderDir[ENC_R].Down()) || (LCDML.BT_checkUp() && encoderDir[ENC_R].Up()) || (LCDML.BT_checkEnter() && encoderDir[ENC_R].ButtonShort()))
    {
      if (LCDML.BT_checkDown())
        configuration.fx.ep_chorus_frequency = constrain(configuration.fx.ep_chorus_frequency + ENCODER[ENC_R].speed(), EP_CHORUS_FREQUENCY_MIN, EP_CHORUS_FREQUENCY_MAX);
      else if (LCDML.BT_checkUp())
        configuration.fx.ep_chorus_frequency = constrain(configuration.fx.ep_chorus_frequency - ENCODER[ENC_R].speed(), EP_CHORUS_FREQUENCY_MIN, EP_CHORUS_FREQUENCY_MAX);
    }
    display_bar_float("EP Chorus Frq.", configuration.fx.ep_chorus_frequency, 0.1, EP_CHORUS_FREQUENCY_MIN, EP_CHORUS_FREQUENCY_MAX, 2, 1, false, false, false);
#if defined(USE_EPIANO)
    ep_chorus_modulator.frequency(configuration.fx.ep_chorus_frequency / 10.0);
#endif
  }

  if (LCDML.FUNC_close())     // ****** STABLE END *********
  {
    //lcd_special_chars(SCROLLBAR);
    encoderDir[ENC_R].reset();
  }
}

void UI_func_epiano_chorus_waveform(uint8_t param)
{
  if (LCDML.FUNC_setup())         // ****** SETUP *********
  {
    encoderDir[ENC_R].reset();

    display.setCursor_textGrid(1, 1);
    display.print(F("EP Ch. Wavefrm"));
  }

  if (LCDML.FUNC_loop())          // ****** LOOP *********
  {
    if (LCDML.BT_checkDown() && encoderDir[ENC_R].Down())
      configuration.fx.ep_chorus_waveform = constrain(configuration.fx.ep_chorus_waveform + 1, EP_CHORUS_WAVEFORM_MIN, EP_CHORUS_WAVEFORM_MAX);
    else if (LCDML.BT_checkUp() && encoderDir[ENC_R].Up())
      configuration.fx.ep_chorus_waveform = constrain(configuration.fx.ep_chorus_waveform - 1, EP_CHORUS_WAVEFORM_MIN, EP_CHORUS_WAVEFORM_MAX);

    display.setCursor_textGrid(1, 2);
    switch (configuration.fx.ep_chorus_waveform)
    {
      case 0:
#if defined(USE_EPIANO)
        ep_chorus_modulator.begin(WAVEFORM_TRIANGLE);
#endif
        display.print(F("[TRIANGLE]"));
        break;
      case 1:
#if defined(USE_EPIANO)
        ep_chorus_modulator.begin(WAVEFORM_SINE);
#endif
        display.print(F("[SINE    ]"));
        break;
      default:
#if defined(USE_EPIANO)
        ep_chorus_modulator.begin(WAVEFORM_TRIANGLE);
#endif
        display.print(F("[TRIANGLE]"));
        break;
    }
  }

  if (LCDML.FUNC_close())     // ****** STABLE END *********
  {
    //lcd_special_chars(SCROLLBAR);
    encoderDir[ENC_R].reset();
  }
}

void UI_func_epiano_chorus_depth(uint8_t param)
{
  if (LCDML.FUNC_setup())         // ****** SETUP *********
  {
    encoderDir[ENC_R].reset();

    //lcd_special_chars(BLOCKBAR);
    display_bar_int("EP Ch. Depth", configuration.fx.ep_chorus_depth, 1.0, EP_CHORUS_DEPTH_MIN, EP_CHORUS_DEPTH_MAX, 3, false, false, true);
  }

  if (LCDML.FUNC_loop())          // ****** LOOP *********
  {
    if ((LCDML.BT_checkDown() && encoderDir[ENC_R].Down()) || (LCDML.BT_checkUp() && encoderDir[ENC_R].Up()))
    {
      if (LCDML.BT_checkDown())
        configuration.fx.ep_chorus_depth = constrain(configuration.fx.ep_chorus_depth + ENCODER[ENC_R].speed(), EP_CHORUS_DEPTH_MIN, EP_CHORUS_DEPTH_MAX);
      else if (LCDML.BT_checkUp())
        configuration.fx.ep_chorus_depth = constrain(configuration.fx.ep_chorus_depth - ENCODER[ENC_R].speed(), EP_CHORUS_DEPTH_MIN, EP_CHORUS_DEPTH_MAX);
    }

    display_bar_int("EP Ch. Depth", configuration.fx.ep_chorus_depth, 1.0, EP_CHORUS_DEPTH_MIN, EP_CHORUS_DEPTH_MAX, 3, false, false, false);
#if defined(USE_EPIANO)
    ep_chorus_modulator.amplitude(mapfloat(configuration.fx.ep_chorus_depth, EP_CHORUS_DEPTH_MIN, EP_CHORUS_DEPTH_MAX, 0.0, 1.0));
#endif
  }

  if (LCDML.FUNC_close())     // ****** STABLE END *********
  {
    encoderDir[ENC_R].reset();
  }
}

void UI_func_epiano_chorus_level(uint8_t param)
{
  if (LCDML.FUNC_setup())         // ****** SETUP *********
  {
    encoderDir[ENC_R].reset();
    display_bar_int("EP Ch. Level", configuration.fx.ep_chorus_level, 1.0, EP_CHORUS_LEVEL_MIN, EP_CHORUS_LEVEL_MAX, 3, false, false, true);
  }

  if (LCDML.FUNC_loop())          // ****** LOOP *********
  {
    if ((LCDML.BT_checkDown() && encoderDir[ENC_R].Down()) || (LCDML.BT_checkUp() && encoderDir[ENC_R].Up()) || (LCDML.BT_checkEnter() && encoderDir[ENC_R].ButtonShort()))
    {
      if (LCDML.BT_checkDown())
      {
        configuration.fx.ep_chorus_level = constrain(configuration.fx.ep_chorus_level + ENCODER[ENC_R].speed(), EP_CHORUS_LEVEL_MIN, EP_CHORUS_LEVEL_MAX);
        MD_sendControlChange(configuration.epiano.midi_channel, 93, configuration.fx.ep_chorus_level);
      }
      else if (LCDML.BT_checkUp())
      {
        configuration.fx.ep_chorus_level = constrain(configuration.fx.ep_chorus_level - ENCODER[ENC_R].speed(), EP_CHORUS_LEVEL_MIN, EP_CHORUS_LEVEL_MAX);
        MD_sendControlChange(configuration.epiano.midi_channel, 93, configuration.fx.ep_chorus_level);
      }
    }

    display_bar_int("EP Ch. Level", configuration.fx.ep_chorus_level, 1.0, EP_CHORUS_LEVEL_MIN, EP_CHORUS_LEVEL_MAX, 3, false, false, false);
#if defined(USE_EPIANO)
    ep_chorus_mixer_r.gain(1, mapfloat(configuration.fx.ep_chorus_level, EP_CHORUS_LEVEL_MIN, EP_CHORUS_LEVEL_MAX, 0.0, 0.5));
    ep_chorus_mixer_l.gain(1, mapfloat(configuration.fx.ep_chorus_level, EP_CHORUS_LEVEL_MIN, EP_CHORUS_LEVEL_MAX, 0.0, 0.5));
#endif
  }

  if (LCDML.FUNC_close())     // ****** STABLE END *********
  {
    encoderDir[ENC_R].reset();
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
#if NUM_DEXED>1
    display_bar_int("Polyphony", configuration.dexed[selected_instance_id].polyphony, 1.0, POLYPHONY_MIN, POLYPHONY_MAX - configuration.dexed[(selected_instance_id + 1) % NUM_DEXED].polyphony, 2, false, false, true);
#else
    display_bar_int("Polyphony", configuration.dexed[selected_instance_id].polyphony, 1.0, POLYPHONY_MIN, POLYPHONY_MAX, 2, false, false, true);
#endif

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

        UI_update_instance_icons();
      }

#endif
    }

#if NUM_DEXED>1
    display_bar_int("Polyphony", configuration.dexed[selected_instance_id].polyphony, 1.0, POLYPHONY_MIN, POLYPHONY_MAX - configuration.dexed[(selected_instance_id + 1) % NUM_DEXED].polyphony, 2, false, false, false);
    if ( configuration.dexed[0].polyphony < 2 || configuration.dexed[1].polyphony < 2)
      UI_update_instance_icons();
#else
    display_bar_int("Polyphony", configuration.dexed[selected_instance_id].polyphony, 1.0, POLYPHONY_MIN, POLYPHONY_MAX, 2, false, false, false);
    if ( configuration.dexed[0].polyphony < 2)
      UI_update_instance_icons();
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

      UI_update_instance_icons();
    }

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
    display.print(F("Drum Rev. Send"));
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
    display.setCursor_textGrid(1, 1);
    display.print(F(" Drums M.Volume "));
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
    display.setCursor_textGrid(5, 2);
    sprintf(displayname, "%03d", temp_int);
    display.print(displayname);
    display.setCursor_textGrid(8, 2);
    display.print(F("/100"));
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
    display.print(F("DrumSmp. Tune"));
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
    display.print(F("DrumSmp. Pitch"));
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

void print_custom_mappings()
{

  display.setTextSize(2);
  display.setTextColor(COLOR_SYSTEXT, COLOR_BACKGROUND);

  for (uint8_t y = 0; y < 10; y++)
  {
    display.setCursor_textGrid(1, y + 8);
    display.setTextColor(COLOR_SYSTEXT, COLOR_BACKGROUND);
    seq_print_formatted_number(y + 1, 2); //entry no.

    if (custom_midi_map[y].type == 0)
    {
      display.setTextColor(GREY2, COLOR_BACKGROUND);
      display.show(y + 8, 4, 5,  "NONE"  );
    }
    else if (custom_midi_map[y].type == 1)
    {
      display.setTextColor(COLOR_PITCHSMP, COLOR_BACKGROUND);
      display.show(y + 8, 4, 7, "KEY/PAD" );
    }
    else if (custom_midi_map[y].type == 2)
    {
      display.setTextColor(COLOR_PITCHSMP, COLOR_BACKGROUND);
      display.show(y + 8, 4, 7, "MIDI CC" );
    }
    display.setTextColor(COLOR_DRUMS, COLOR_BACKGROUND);
    display.show(y + 8, 12, 3,  custom_midi_map[y].in   );

    display.setTextColor(COLOR_CHORDS, COLOR_BACKGROUND);
    display.show(y + 8, 16, 3, custom_midi_map[y].out );

    display.setTextColor(COLOR_INSTR, COLOR_BACKGROUND);
    display.show(y + 8, 20, 3, custom_midi_map[y].channel );


    display.setTextColor(GREY2, COLOR_BACKGROUND);
    if (custom_midi_map[y].in == 0)
      display.show(y + 8, 24, 12, "EMPTY SLOT");
    else if (custom_midi_map[y].type == 1)
    {
      display.setTextColor(PINK, COLOR_BACKGROUND);
      display.show(y + 8, 24, 13, find_long_drum_name_from_note(custom_midi_map[y].out) );
    }
    else if (custom_midi_map[y].type == 2)
    {
      display.setTextColor(COLOR_INSTR, COLOR_BACKGROUND);
      for (uint8_t i = 0; i < sizeof(cc_dest_values); i++)
      {
        if (custom_midi_map[y].out == cc_dest_values[i])
          display.show(y + 8, 24, 13, cc_names[i] );
      }
    }
  }
}

void UI_func_custom_mappings(uint8_t param)
{
  char displayname[8] = {0, 0, 0, 0, 0, 0, 0};
  if (LCDML.FUNC_setup())         // ****** SETUP *********
  {
    encoderDir[ENC_R].reset();
    display.fillScreen(COLOR_BACKGROUND);
    display.setTextColor(COLOR_SYSTEXT, COLOR_BACKGROUND);
    display.setTextSize(2);
    border1();
    border2();
    border3_large();
    display.setCursor_textGrid(1, 1);
    display.print(F("CUSTOM MAPPINGS"));

    display.setTextColor(COLOR_SYSTEXT, COLOR_PITCHSMP);
    display.setTextSize(2);
    display.fillRect (240 + CHAR_width, CHAR_height, 8 * CHAR_width, 4 * CHAR_height, COLOR_PITCHSMP);
    display.setCursor(240 + CHAR_width * 2 + 5, 2 * CHAR_height );
    display.print(F("TOUCH"));

    display.drawBitmap(240 + CHAR_width * 2 + 4, CHAR_height * 4 - 12, special_chars[19], 8, 8, GREEN);
    display.setCursor(240 + CHAR_width * 3 + 10, CHAR_height * 4 - 12 );
    display.setTextSize(1);
    display.print(F("PREVIEW"));

    display.fillRect (240 + CHAR_width + CHAR_width * 10 - 2, CHAR_height, 8 * CHAR_width, 4 * CHAR_height, COLOR_PITCHSMP);
    display.setCursor(240 + CHAR_width * 12 + 4, 2 * CHAR_height);
    display.setTextSize(2);
    display.print(F("TOUCH"));
    display.setCursor(240 + CHAR_width * 12 + 3, CHAR_height * 4 - 12 );
    display.setTextSize(1);
    display.print(F("MIDI LEARN"));

    //scrollbar
    display.fillRect (480 - 28, 8 * CHAR_height, 14, 10 * CHAR_height, COLOR_SYSTEXT);
    display.fillRect (480 - 27, 8 * CHAR_height + 1, 12, 4 * CHAR_height, GREY2);

    display.setTextSize(2);
    display.setTextColor(COLOR_SYSTEXT, COLOR_BACKGROUND);
    display.setCursor_textGrid(1, 7);
    display.print(F("NO TYPE    IN  OUT CH. NAME"));

    print_custom_mappings();

  }
  if (LCDML.FUNC_loop())          // ****** LOOP *********
  {
    if ((LCDML.BT_checkDown() && encoderDir[ENC_R].Down()) || (LCDML.BT_checkUp() && encoderDir[ENC_R].Up()) || (LCDML.BT_checkEnter() && encoderDir[ENC_R].ButtonShort()))
    {
      if (LCDML.BT_checkDown())
      {
        smart_filter(1);
      }
      else if (LCDML.BT_checkUp())
      {
        smart_filter(0);
      }
    }
    if (LCDML.BT_checkEnter())
    {
      ;
    }
    display.setTextColor(COLOR_SYSTEXT, COLOR_BACKGROUND);
    display.setTextSize(2);
    display.setCursor_textGrid(1, 2);
    display.print("[");
    display.setCursor_textGrid(4, 2);
    display.print("]");
    display.setCursor_textGrid(2, 2);

    sprintf(displayname, "%02d", activesample);
    display.print(displayname);
    display.setTextColor(COLOR_PITCHSMP, COLOR_BACKGROUND);
    display.show(2, 5, 13, basename(drum_config[activesample].name));

  }
  if (LCDML.FUNC_close())     // ****** STABLE END *********
  {
    seq.midi_learn_active = false;
    encoderDir[ENC_R].reset();
  }
}

void UI_func_cc_mappings(uint8_t param)
{
  if (LCDML.FUNC_setup())         // ****** SETUP *********
  {

    encoderDir[ENC_R].reset();
    display.fillScreen(COLOR_BACKGROUND);
    display.setTextColor(COLOR_SYSTEXT, COLOR_BACKGROUND);

    display.setTextSize(2);
    border1();
    border2();
    border3_large();

    UI_update_instance_icons();

    display.setCursor_textGrid(1, 1);
    display.print("CUSTOM DEXED CC");

    display.setTextColor(COLOR_SYSTEXT, COLOR_PITCHSMP);
    display.setTextSize(2);

    display.fillRect (240 + CHAR_width + CHAR_width * 10 - 2, CHAR_height, 8 * CHAR_width, 4 * CHAR_height, COLOR_PITCHSMP);
    display.setCursor(240 + CHAR_width * 12 + 4, 2 * CHAR_height);
    display.setTextSize(2);
    display.print(F("TOUCH"));
    display.setCursor(240 + CHAR_width * 12 + 3, CHAR_height * 4 - 12 );
    display.setTextSize(1);
    display.print(F("MIDI LEARN"));

    //scrollbar
    display.fillRect (480 - 28, 8 * CHAR_height, 14, 10 * CHAR_height, COLOR_SYSTEXT);
    display.fillRect (480 - 27, 8 * CHAR_height + 1, 12, 4 * CHAR_height, GREY2);

    display.setTextSize(2);
    display.setTextColor(COLOR_SYSTEXT, COLOR_BACKGROUND);
    display.setCursor_textGrid(1, 7);
    display.print(F("NO TYPE    IN  OUT CH. NAME"));

    print_custom_mappings();

  }
  if (LCDML.FUNC_loop())          // ****** LOOP *********
  {
    if ((LCDML.BT_checkDown() && encoderDir[ENC_R].Down()) || (LCDML.BT_checkUp() && encoderDir[ENC_R].Up()) || (LCDML.BT_checkEnter() && encoderDir[ENC_R].ButtonShort()))
    {
      if (LCDML.BT_checkDown())
      {
        seq.temp_select_menu = constrain(seq.temp_select_menu + ENCODER[ENC_R].speed(), 0, 7);
      }
      else if (LCDML.BT_checkUp())
      {
        seq.temp_select_menu = constrain(seq.temp_select_menu - ENCODER[ENC_R].speed(), 0, 7);
      }
    }
    if (LCDML.BT_checkEnter())
    {
      selected_instance_id = !selected_instance_id;

      UI_update_instance_icons();

    }

    display.setTextSize(2);
    display.setTextColor(COLOR_PITCHSMP, COLOR_BACKGROUND);
    display.show(2, 1, 13, cc_names[seq.temp_select_menu] );

  }
  if (LCDML.FUNC_close())     // ****** STABLE END *********
  {
    seq.midi_learn_active = false;
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
    display.print(F("DrumSmp. Volume"));

    display.setCursor_textGrid(2, 2);
    sprintf(displayname, "%02d", activesample);
    display.print(displayname);
    display.show(2, 4, 8, basename(drum_config[activesample].name));

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
    display.print(F("Shift"));
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
    display.print(F("DrmSmp. Panorama"));
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
      display.setTextColor(COLOR_DRUMS, DARKGREEN);
    else if (seq.content_type[pattern] == 1) //Instrument Pattern
      display.setTextColor(COLOR_INSTR, DARKGREEN);
    else if (seq.content_type[pattern] == 2 || seq.content_type[pattern] == 3) //  chord or arp pattern
      display.setTextColor(COLOR_CHORDS, DARKGREEN);
  }
  else
  {
    if (seq.content_type[pattern] == 0) //Drumpattern
      display.setTextColor(COLOR_DRUMS, COLOR_BACKGROUND);
    else if (seq.content_type[pattern] == 1) //Instrument Pattern
      display.setTextColor(COLOR_INSTR, COLOR_BACKGROUND);
    else if (seq.content_type[pattern] == 2 || seq.content_type[pattern] == 3) //  chord or arp pattern
      display.setTextColor(COLOR_CHORDS, COLOR_BACKGROUND);
  }
}

void set_pattern_content_type_color_inverted(uint8_t pattern)
{
  if (seq.content_type[pattern] == 0) //Drumpattern
    display.setTextColor(  COLOR_BACKGROUND, COLOR_DRUMS);
  else if (seq.content_type[pattern] == 1) //Instrument Pattern
    display.setTextColor( COLOR_BACKGROUND, COLOR_INSTR);
  else if (seq.content_type[pattern] == 2 || seq.content_type[pattern] == 3) //  chord or arp pattern
    display.setTextColor( COLOR_BACKGROUND, COLOR_CHORDS);
}

void set_track_type_color(uint8_t track)
{
  if ( seq.track_type[track] == 0) //Drums
    display.setTextColor(COLOR_DRUMS, COLOR_BACKGROUND);
  else if ( seq.track_type[track] == 1) //Inst
    display.setTextColor(COLOR_INSTR, COLOR_BACKGROUND);
  else if ( seq.track_type[track] == 2 || seq.track_type[track] == 3 ) //Chord or Arp
    display.setTextColor(COLOR_CHORDS, COLOR_BACKGROUND);
}

void set_track_type_color_inverted(uint8_t track)
{
  if ( seq.track_type[track] == 0) //Drums
    display.setTextColor(COLOR_BACKGROUND, COLOR_DRUMS);
  else if ( seq.track_type[track] == 1) //Inst
    display.setTextColor(COLOR_BACKGROUND, COLOR_INSTR);
  else if ( seq.track_type[track] == 2 || seq.track_type[track] == 3) //Chord or Arp
    display.setTextColor(COLOR_BACKGROUND, COLOR_CHORDS);
}

void print_color_map(int x, int y)
{
  display.setTextSize(1);
  display.setCursor(CHAR_width * 29,  15 * (CHAR_height - 5) + 2 );
  display.setTextColor(GREY1, COLOR_BACKGROUND);
  display.print(F( "DEF.CHRD SND:"));
  display.setCursor(CHAR_width * 29,  16 * (CHAR_height - 5) + 2 );
  display.setTextColor(GREY1, COLOR_BACKGROUND);
  display.print(F( "CHORD N. STK:"));
  display.setCursor(CHAR_width * 29,  17 * (CHAR_height - 5) + 2 );
  display.setTextColor(GREY1, COLOR_BACKGROUND);
  display.print(F( "CHRD/ARP VEL:"));
  display.setCursor(CHAR_width * 29,  18 * (CHAR_height - 5) + 2 );
  display.setTextColor(GREY1, COLOR_BACKGROUND);
  display.print(F( "ARP NOTE MAX:"));
  display.setCursor(CHAR_width * 29,  13 * (CHAR_height - 5) + 2 );
  display.setTextColor(GREY2, COLOR_BACKGROUND);
  display.print(F( "PERF#"));
  display.setTextColor(COLOR_PITCHSMP, COLOR_BACKGROUND);
  seq_print_formatted_number(configuration.sys.performance_number, 2);
  display.print( ": ");
  display.setTextColor(COLOR_PITCHSMP, COLOR_BACKGROUND);
  display.print( "[");
  display.setTextColor(COLOR_SYSTEXT, COLOR_BACKGROUND);
  display.print(seq.name);

  display.setTextColor(COLOR_PITCHSMP, COLOR_BACKGROUND);
  display.print( "]");
  display.setCursor(x, y +  2 * (CHAR_height - 5));

  display.drawLine(TFT_HEIGHT / 2, y +  2 * (CHAR_height - 5) + 2 , TFT_HEIGHT, y +  2 * (CHAR_height - 5) + 2 , GREY4);
  display.setCursor(x, y +  3 * (CHAR_height - 5));
  display.setTextColor(COLOR_DRUMS, COLOR_BACKGROUND);
  display.print(F("DRUMS "));
  display.setTextColor(COLOR_PITCHSMP, COLOR_BACKGROUND);
  display.print(F("PITCHED SAMPLE "));
  display.setTextColor(COLOR_CHORDS, COLOR_BACKGROUND);
  display.print(F("CHORD/ARP "));
  display.setTextColor(COLOR_INSTR, COLOR_BACKGROUND);
  display.print(F("INST"));
  display.setTextSize(2);
}

void seq_printAllSeqSteps()
{
  display.setCursor_textGrid(1, 2);
  for (uint8_t i = 0; i < 16; i++)
  {
    if ( seq.vel[seq.active_pattern][i] > 209 )
      display.setTextColor(COLOR_PITCHSMP, COLOR_BACKGROUND);
    else
      set_pattern_content_type_color(seq.active_pattern);
    display.print(seq_find_shortname(i)[0]);
  }
}

void seq_sub_pat_chain(int x, int y, bool init)
{
  display.setTextSize(1);
  display.setTextColor(GREY2, COLOR_BACKGROUND);
  for (uint8_t track = 0; track < NUM_SEQ_TRACKS; track++)
  {
    display.setCursor(x,   13 * (CHAR_height - 5)  +   track * (CHAR_height - 5) + 2   );
    if (seq.menu - 21 == track)
      set_track_type_color_inverted(track);
    else
      set_track_type_color(track);
    display.print(F("TRK"));
    display.print (track + 1);
    display.setCursor(x + 5 * 6,   13 * (CHAR_height - 5)  +   track * (CHAR_height - 5) + 2   );
    if (seq.menu - 21 - 6 == track)
      set_track_type_color_inverted(track);
    else
      set_track_type_color(track);
    if (seq.track_type[track] > 0 && seq.inst_dexed[track] < 2)
    {
      display.print ("dexedI");
      display.print (seq.inst_dexed[track] + 1);
    }
    else if (seq.track_type[track] > 0 && seq.inst_dexed[track] == 2) //epiano
    {
      display.print ("ElPiano");
    }
    else if (seq.track_type[track] > 0 && seq.inst_dexed[track] == 3)
    {
      display.print ("SID Saw");
    }
    else if (seq.track_type[track] > 0 && seq.inst_dexed[track] == 4)
    {
      display.print ("SID Sqr");
    }
    else if (seq.track_type[track] == 0 ) //drums/samples
    {
      display.print ("DRM/SMP");
    }
    else
    {
      display.print ("- - - -");
    }
  }
  display.setCursor(CHAR_width * 36,  15 * (CHAR_height - 5) + 2 );
  if (seq.menu == 33)
    display.setTextColor(COLOR_BACKGROUND, COLOR_DRUMS);
  else
    display.setTextColor(COLOR_SYSTEXT, COLOR_BACKGROUND);
  if (seq.chord_dexed_inst == 0)
    display.print(F( "DEXED1"));
  else if (seq.chord_dexed_inst == 1)
    display.print(F( "DEXED2"));
  else if (seq.chord_dexed_inst == 2)
    display.print(F( "EPIANO"));
  else if (seq.chord_dexed_inst == 3)
    display.print(F( "SIDsaw"));
  else if (seq.chord_dexed_inst == 4)
    display.print(F( "SIDsqr"));

  display.setCursor(CHAR_width * 36,  16 * (CHAR_height - 5) + 2 );
  if (seq.menu == 34)
    display.setTextColor(COLOR_BACKGROUND, COLOR_DRUMS);
  else
    display.setTextColor(COLOR_SYSTEXT, COLOR_BACKGROUND);
  seq_print_formatted_number(seq.arp_chord, 2);

  display.setCursor(CHAR_width * 36,  17 * (CHAR_height - 5) + 2 );
  if (seq.menu == 35)
    display.setTextColor(COLOR_BACKGROUND, COLOR_DRUMS);
  else
    display.setTextColor(COLOR_SYSTEXT, COLOR_BACKGROUND);
  seq_print_formatted_number(seq.chord_velocity, 3);
  display.setCursor(CHAR_width * 36,  18 * (CHAR_height - 5) + 2 );
  if (seq.menu == 36)
    display.setTextColor(COLOR_BACKGROUND, COLOR_DRUMS);
  else
    display.setTextColor(COLOR_SYSTEXT, COLOR_BACKGROUND);
  seq_print_formatted_number(seq.arp_num_notes_max, 2);
  if (init)
    print_color_map(CHAR_width * 21, 266);
  display.setTextSize(2);
}

void seq_printVelGraphBar()
{
  display.fillRect(CHAR_width, 3 * CHAR_height , 17 * CHAR_width, 17, COLOR_BACKGROUND);
  for (uint8_t i = 0; i < 16; i++)
  {
    if ( seq.vel[seq.active_pattern][i] < 130) {
      display.drawLine (CHAR_width + i * CHAR_width + 3 , CHAR_height * 4 - 3  , CHAR_width + i * CHAR_width + 3 , CHAR_height * 4 - 3 - (seq.vel[seq.active_pattern][i] / 10)   , GREY1);
      display.drawLine (CHAR_width + i * CHAR_width + 4 , CHAR_height * 4 - 3  , CHAR_width + i * CHAR_width + 4 , CHAR_height * 4 - 3 - (seq.vel[seq.active_pattern][i] / 10)   , GREY1);
      display.drawLine (CHAR_width + i * CHAR_width + 5 , CHAR_height * 4 - 3  , CHAR_width + i * CHAR_width + 5 , CHAR_height * 4 - 3 - (seq.vel[seq.active_pattern][i] / 10)   , GREY1);
    }
    else
    {
      if ( seq.vel[seq.active_pattern][i] > 209)
      {
        display.drawBitmap(i * CHAR_width + 13, 3 * CHAR_height + 7, special_chars[22], 8, 8, COLOR_PITCHSMP);
        //display.print("P");
      }
    }
  }
}

void seq_printVelGraphBar_single_step(uint8_t step, int color)
{
  display.fillRect(CHAR_width + step * CHAR_width + 3 , CHAR_height * 3 + 1  , 3, 14, COLOR_BACKGROUND);

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
      display.drawBitmap(step * CHAR_width + 13, 3 * CHAR_height + 7, special_chars[22], 8, 8, COLOR_PITCHSMP);
    }
  }
}

void UI_func_seq_display_style(uint8_t param)
{
  if (LCDML.FUNC_setup())         // ****** SETUP *********
  {
    encoderDir[ENC_R].reset();
    display.setCursor_textGrid(1, 1);
    display.print(F("Seq. Disp. Style"));
    display.setCursor_textGrid(1, 2);
    display.print(F("Pat."));
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
    display.print(F("Live Transp.Oct."));
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
      display.print(F(" - "));
    display.print(F("] - ["));
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
  //  int xspace = 241;
  //  int samplevalue = 0;
  //  int  old_y = 48;
  //  if (seq.last_drawn_sample != samplenumber && samplenumber < NUM_DRUMSET_CONFIG - 1)
  //  {
  //    int scale = drum_config[samplenumber].len / (170 / 2);
  //
  //    border2_half_clear();

  //    for (int i = 0; i < 170; i++)
  //    {
  //      samplevalue = (( (drum_config[samplenumber].drum_data[i * scale + 1]) * 256) + drum_config[samplenumber].drum_data[i * scale] )  / 800;
  //
  //      if (drum_config[samplenumber].drum_data[i * scale + 1] < 128)
  //      {
  //        display.drawLine(xspace, old_y,  xspace, samplevalue + 50  , GREY1 );
  //        old_y = samplevalue + 50;
  //      }
  //      else
  //      {
  //        display.drawLine(xspace, old_y,  xspace, samplevalue - 32 , GREY1 );
  //        old_y = samplevalue - 32 ;
  //      }
  //      xspace = xspace + 1;
  //    }

  //    if (seq.content_type[seq.active_pattern] == 0) //Drum Track
  //    {
  //      display.setTextColor(GREEN, GREY4);
  //      display.show_smallfont_noGrid(5, 353 , 9, drum_config[samplenumber].name);
  //      display.setTextColor(COLOR_SYSTEXT, COLOR_BACKGROUND);
  //    }
  //    seq.last_drawn_sample = samplenumber;
  //  }
  //  display.setTextSize(2);
}

//void UI_draw_waveform(uint8_t samplenumber)
//{
//  int xspace = 241;
//  int samplevalue = 0;
//  int  old_y = 48;
//  if (seq.last_drawn_sample != samplenumber && samplenumber < NUM_DRUMSET_CONFIG - 1)
//  {
//    int scale = drum_config[samplenumber].len / (236 / 2);
//
//    border2_clear();
//
//    for (int i = 0; i < 236; i++)
//    {
//      samplevalue = (( (drum_config[samplenumber].drum_data[i * scale + 1]) * 256) + drum_config[samplenumber].drum_data[i * scale] )  / 800;
//
//      if (drum_config[samplenumber].drum_data[i * scale + 1] < 128)
//      {
//        display.drawLine(xspace, old_y,  xspace, samplevalue + 50  , GREY1 );
//        old_y = samplevalue + 50;
//      }
//      else
//      {
//        display.drawLine(xspace, old_y,  xspace, samplevalue - 32 , GREY1 );
//        old_y = samplevalue - 32 ;
//      }
//      xspace = xspace + 1;
//    }
//
//    if (seq.content_type[seq.active_pattern] == 0) //Drum Track
//    {
//      display.setTextColor(GREEN, GREY4);
//      display.show_smallfont_noGrid(5, 419 , 9, drum_config[samplenumber].name);
//      display.setTextColor(COLOR_SYSTEXT, COLOR_BACKGROUND);
//    }
//    seq.last_drawn_sample = samplenumber;
//  }
//  display.setTextSize(2);
//}

void UI_draw_waveform_large()
{
  //  int xspace = 0;
  //  int i = 4;
  //  short samplevalue = 0;
  //  int oldx = 0, oldy = 180;
  //
  //  display.setCursor_textGrid(31, 0);
  //  display.setTextColor(COLOR_SYSTEXT, COLOR_PITCHSMP );
  //  display.print("SCALE");
  //  display.setCursor_textGrid(37, 0);
  //  display.setTextColor(COLOR_SYSTEXT, COLOR_BACKGROUND );
  //  display.print(wave_spacing);
  //  display.print (" ");
  //  do {
  //    {
  //      samplevalue = (( (DRUM_SQBass[i * wave_spacing_old + 1]) * 256) + DRUM_SQBass[i * wave_spacing_old] )  / 710 + 120;
  //      if (DRUM_SQBass[i * wave_spacing_old + 1] < 128)
  //      {
  //        display.drawLine(oldx, oldy,  xspace, samplevalue + 60  , COLOR_BACKGROUND );
  //        oldx = xspace; oldy = samplevalue + 60;
  //      }
  //      else
  //      {
  //        display.drawLine(oldx, oldy,  xspace, samplevalue  - 40 , COLOR_BACKGROUND );
  //        oldx = xspace;  oldy = samplevalue - 40;
  //      }
  //      xspace = xspace + 1; i++;
  //    }
  //  } while (oldx < TFT_HEIGHT);
  //  xspace = 0;
  //  i = 4;
  //  samplevalue = 0;
  //  oldx = 0;  oldy = 180;
  //  do {
  //    {
  //      samplevalue = (( (DRUM_SQBass[i * wave_spacing + 1]) * 256) + DRUM_SQBass[i * wave_spacing] )  / 710 + 120;
  //
  //      if (DRUM_SQBass[i * wave_spacing + 1] < 128)
  //      {
  //        display.drawLine(oldx, oldy,  xspace, samplevalue + 60  , COLOR_SYSTEXT );
  //        oldx = xspace; oldy = samplevalue + 60;
  //      }
  //      else
  //      {
  //        display.drawLine(oldx, oldy,  xspace, samplevalue  - 40 , COLOR_SYSTEXT );
  //        oldx = xspace;  oldy = samplevalue - 40;
  //      }
  //      xspace = xspace + 1;  i++;
  //    }
  //  } while (oldx < TFT_HEIGHT);
  //wave_spacing_old = wave_spacing;
}


void UI_print_voice_info()
{

  display.setTextColor(COLOR_SYSTEXT, COLOR_BACKGROUND);

  //UI_draw_waveform(0);

  display.setTextSize(1);
  display.setCursor(415, 6);
  display.setTextColor(COLOR_BACKGROUND, COLOR_PITCHSMP);
  display.print("01");
  display.setTextColor(COLOR_BACKGROUND, COLOR_BACKGROUND);
  display.print(" ");
  display.setTextColor(COLOR_SYSTEXT, GREY2);
  display.print("SQBASS");



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

  display.setTextColor(COLOR_PITCHSMP, COLOR_BACKGROUND);
  display.setCursor_textGrid(1, 2);
  seq_print_formatted_number(configuration.dexed[selected_instance_id].bank, 2);
  display.setCursor_textGrid(1, 2);
  seq_print_formatted_number(configuration.dexed[selected_instance_id].voice + 1, 2);
  display.setTextColor(COLOR_SYSTEXT, COLOR_BACKGROUND);
  string_toupper(bank_name);
  display.show(2, 5, 8, bank_name);

  string_toupper(voice_name);
  display.show(2, 5, 10, voice_name);

  display.setTextColor(COLOR_SYSTEXT, COLOR_BACKGROUND);
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
    display.print(F("ChordTrack Keys:"));
    display.setCursor_textGrid(9, 2);
    display.print(F("Keys"));
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

void UI_func_seq_tempo(uint8_t param)
{
  char tmp[7];
  if (LCDML.FUNC_setup())         // ****** SETUP *********
  {
    encoderDir[ENC_R].reset();
    display.setCursor_textGrid(1, 1);
    display.print(F("  Seq. Tempo   "));
    display.setCursor_textGrid(6, 2);
    display.print(F("BPM"));
    display.setCursor_textGrid(14, 2);
    display.print(F("ms"));
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
    if (seq.running)
      sequencer_timer.begin(sequencer, seq.tempo_ms / 8);
    else
      sequencer_timer.begin(sequencer, seq.tempo_ms / 8, false);
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
    display.fillScreen(COLOR_BACKGROUND);
    display.setCursor_textGrid(1, 1);
    display.setTextColor(COLOR_SYSTEXT, COLOR_PITCHSMP);
    display.print(F("SAMPLE:"));
    display.setCursor_textGrid(9, 1);
    display.setTextColor(COLOR_SYSTEXT, COLOR_BACKGROUND);
    display.print(F("SQRBASS1"));


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
    display.fillScreen(COLOR_BACKGROUND);
  }
}

void print_content_type ()
{
  set_pattern_content_type_color(seq.active_pattern);
  if (seq.content_type[seq.active_pattern] == 0)
    display.print(F("Drum     "));
  else if (seq.content_type[seq.active_pattern] == 1)
    display.print(F("Instr    "));
  else if (seq.content_type[seq.active_pattern] == 2  )
    display.print(F("Chord/Arp"));

  display.setTextColor(GREY1, COLOR_BACKGROUND);
}

void print_edit_mode()
{
  display.setTextSize(1);
  display.setCursor(136, 71);

  if (LCDML.FUNC_getID() == LCDML.OTHER_getIDFromFunction(UI_func_seq_vel_editor))
  {

    if (seq.menu < 19)
    {
      display.setTextColor(COLOR_BACKGROUND, COLOR_SYSTEXT);
      display.print(F("VEL./CHORDS"));
    }
    else if (seq.menu == 19)
    {
      display.setTextColor(COLOR_BACKGROUND, COLOR_SYSTEXT);
      display.print(F("(PATTERN "));
      display.setTextColor(GREY1, COLOR_SYSTEXT);
      display.print(seq.active_pattern);
      display.setTextColor(GREY2, COLOR_SYSTEXT);
      display.print(")");
    }
    else if (seq.menu > 20 && seq.menu < 27)
    {
      display.setTextColor(COLOR_BACKGROUND, COLOR_SYSTEXT);
      display.print(F("TRACK TYPE "));
    }
    else if (seq.menu > 20 + 6 && seq.menu < 27 + 6)
    {
      display.setTextColor(COLOR_BACKGROUND, COLOR_SYSTEXT);
      display.print(F("DEXED/EP A."));
    }

  }
  else if (LCDML.FUNC_getID() == LCDML.OTHER_getIDFromFunction(UI_func_seq_pattern_editor))
  {
    display.setTextColor(COLOR_BACKGROUND, COLOR_SYSTEXT);
    display.print(F("NOTE EDITOR"));
  }
  display.setTextColor(COLOR_SYSTEXT, COLOR_BACKGROUND);
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
  display.setTextColor(GREY2, COLOR_BACKGROUND);
  display.setCursor(xpos, ypos);

  if (init)
  { //only needs to be drawn at first run

    display.setTextColor(GREY2, COLOR_BACKGROUND);
    display.setCursor(xpos, ypos + 20);
    display.print(F("Step  N   DATA  VEL   NAME / NOTE"));

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
        display.setTextColor(GREY1, COLOR_BACKGROUND);
      else
        display.setTextColor(MIDDLEGREEN, COLOR_BACKGROUND);
      display.setCursor(x, y);
      seq_print_formatted_number (i , 2);
    }
    // Short Name
    if (i == currentstep)
      display.setTextColor(COLOR_BACKGROUND, COLOR_SYSTEXT);
    else
      display.setTextColor(GREY2, COLOR_BACKGROUND);
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
      display.setTextColor(COLOR_BACKGROUND, COLOR_SYSTEXT);
    else
      display.setTextColor(GREY2, COLOR_BACKGROUND);
    display.setCursor(x + CHAR_width * 5, y);
    seq_print_formatted_number (seq.note_data[seq.active_pattern][i] , 3);

    // Velocity values
    if (i == currentstep)
      display.setTextColor(COLOR_BACKGROUND, COLOR_SYSTEXT);
    else
      display.setTextColor(GREY1, COLOR_BACKGROUND);
    display.setCursor(x + CHAR_width * 8, y);
    seq_print_formatted_number (seq.vel[seq.active_pattern][i] , 3);
    // Long Name / Note
    if (i == currentstep)
      display.setTextColor(COLOR_BACKGROUND, COLOR_SYSTEXT);
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
            display.setTextColor(COLOR_BACKGROUND, COLOR_SYSTEXT);
          else
            display.setTextColor(GREEN, COLOR_BACKGROUND);
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
        display.setTextColor(COLOR_BACKGROUND, COLOR_SYSTEXT);
      else
        display.setTextColor(GREY2, COLOR_BACKGROUND);
      display.print(" ");
    }
    i++;
  }
  display.setTextColor(COLOR_SYSTEXT, COLOR_BACKGROUND);
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
  }
  for (uint8_t i = 0; i < NUM_SEQ_TRACKS; i++)  // select track type
  {
    if (seq_active_function == 1 && seq.menu == 21 + i) // edit track type
    {
      if ((LCDML.BT_checkDown() && encoderDir[ENC_R].Down()) || (LCDML.BT_checkUp() && encoderDir[ENC_R].Up()))
      {
        if (LCDML.BT_checkDown())
          seq.track_type[i] = constrain(seq.track_type[i] + 1, 0, 3);
        else if (LCDML.BT_checkUp())
          seq.track_type[i] = constrain(seq.track_type[i] - 1, 0, 3);
      }
    }
    else if (seq_active_function == 1 && seq.menu == 21 + i + 6) // edit dexed assign
    {
      if ((LCDML.BT_checkDown() && encoderDir[ENC_R].Down()) || (LCDML.BT_checkUp() && encoderDir[ENC_R].Up()))
      {
        if (LCDML.BT_checkDown())
          seq.inst_dexed[i] = constrain(seq.inst_dexed[i] + 1, 0, 4);
        else if (LCDML.BT_checkUp())
          seq.inst_dexed[i] = constrain(seq.inst_dexed[i] - 1, 0, 4);
      }
    }
  }

  if (seq_active_function == 1 && seq.menu == 33) // edit default chord instrument
  {
    if ((LCDML.BT_checkDown() && encoderDir[ENC_R].Down()) || (LCDML.BT_checkUp() && encoderDir[ENC_R].Up()))
    {
      if (LCDML.BT_checkDown())
        seq.chord_dexed_inst = constrain(seq.chord_dexed_inst + 1, 0, 2);
      else if (LCDML.BT_checkUp())
        seq.chord_dexed_inst = constrain(seq.chord_dexed_inst - 1, 0, 2);
    }
  }
  else if (seq_active_function == 1 && seq.menu == 34) // edit ammount of note for chords
  {
    if ((LCDML.BT_checkDown() && encoderDir[ENC_R].Down()) || (LCDML.BT_checkUp() && encoderDir[ENC_R].Up()))
    {
      if (LCDML.BT_checkDown())
        seq.arp_chord = constrain(seq.arp_chord + 1, 1, 6);
      else if (LCDML.BT_checkUp())
        seq.arp_chord = constrain(seq.arp_chord - 1, 1, 6);
    }
  }
  else if (seq_active_function == 1 && seq.menu == 35) // edit chord velocity
  {
    if ((LCDML.BT_checkDown() && encoderDir[ENC_R].Down()) || (LCDML.BT_checkUp() && encoderDir[ENC_R].Up()))
    {
      if (LCDML.BT_checkDown())
        seq.chord_velocity = constrain(seq.chord_velocity + 1, 10, 120);
      else if (LCDML.BT_checkUp())
        seq.chord_velocity = constrain(seq.chord_velocity - 1, 10, 120);
    }
  }
  else if (seq_active_function == 1 && seq.menu == 36) // edit max arp notes in 1/32 1/64 arps
  {
    if ((LCDML.BT_checkDown() && encoderDir[ENC_R].Down()) || (LCDML.BT_checkUp() && encoderDir[ENC_R].Up()))
    {
      if (LCDML.BT_checkDown())
        seq.arp_num_notes_max = constrain(seq.arp_num_notes_max + 1, 1, 32);
      else if (LCDML.BT_checkUp())
        seq.arp_num_notes_max = constrain(seq.arp_num_notes_max - 1, 1, 32);
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

    if (seq.cycle_touch_element != 0) //hide Keyboard in Velocity editor
    {
      seq.cycle_touch_element = 0;
      seq_pattern_editor_update_dynamic_elements();
    }
    encoderDir[ENC_R].reset();
    print_edit_mode();
    seq.menu = 1;
    //clear "Insert xyz ?"  message
    display.fillRect(1 * CHAR_width, 4 * CHAR_height + 17, 222, 8, COLOR_BACKGROUND);
    display.setCursor_textGrid(15, 1);
    display.setTextColor(GREY2, COLOR_BACKGROUND);
    display.print("[");
    display.setCursor_textGrid(16, 1);
    seq_print_formatted_number(seq.active_pattern, 2);
    display.setCursor_textGrid(18, 1);
    display.print("]");
    if (seq.menu_status != 1)
    {
      display.setCursor(CHAR_width, CHAR_height * 4 + 3);
      display.setTextSize(1);
      display.setTextColor(GREY1, COLOR_BACKGROUND);
      display.print(F("CONT.TYPE:"));
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
        {
          if (seq.menu == 19)
            seq.menu = 21;
          else
            seq.menu = constrain(seq.menu + 1, 19, 36);
        }
        else if (LCDML.BT_checkUp())
        {
          if (seq.menu == 19)
          {
            seq.menu = 16;  //return to vel_editor
          }
          else
            seq.menu = constrain(seq.menu - 1, 19, 36);
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
        else  if (seq_active_function == 1 && seq.content_type[seq.active_pattern] > 1 )
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
            seq.vel[seq.active_pattern][seq.menu - 1] = constrain(seq.vel[seq.active_pattern][seq.menu - 1] + 1, 210, 217);
          else if (LCDML.BT_checkUp())
            seq.vel[seq.active_pattern][seq.menu - 1] = constrain(seq.vel[seq.active_pattern][seq.menu - 1] - 1, 210, 217);
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
        //  display.fillRect(1 * CHAR_width, 4 * CHAR_height + 17, 222, 8, COLOR_BACKGROUND);
        display.setCursor(1 * CHAR_width, 4 * CHAR_height + 17);
        display.setTextSize(1);
        display.setTextColor(GREEN, COLOR_BACKGROUND);
        display.print(F("EDIT "));
        display.setTextColor(COLOR_SYSTEXT, COLOR_BACKGROUND);
        display.print(F("VALUE"));
        display.setTextColor(GREEN, COLOR_BACKGROUND);
        display.print(F(" WITH "));
        display.setTextColor(COLOR_BACKGROUND, COLOR_SYSTEXT);
        display.write (0x1B);
        display.setTextColor(GREEN, COLOR_BACKGROUND);
        display.print(" ");
        display.setTextColor(COLOR_BACKGROUND, COLOR_SYSTEXT);
        display.write (0x1A);
        display.setTextColor(GREEN, COLOR_BACKGROUND);
        display.print(F(" ENCODER_R "));
        display.setTextColor(COLOR_SYSTEXT, COLOR_BACKGROUND);
        display.setTextSize(2);
      }
      else if (seq.menu > 19 && seq.menu < 60 && seq_active_function == 99)
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
      display.setTextColor(COLOR_SYSTEXT, COLOR_BACKGROUND);
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
      display.setTextColor(GREEN, GREY2);
      display.setCursor_textGrid(1, 2);
      display.print(seq_find_shortname(0)[0] );

      set_pattern_content_type_color(seq.active_pattern);
      display.setCursor_textGrid(2, 2);
      display.print(seq_find_shortname(1)[0] );
      display.setTextColor(COLOR_SYSTEXT, COLOR_BACKGROUND);

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
        display.setTextColor(COLOR_PITCHSMP, COLOR_BACKGROUND);
      else
        set_pattern_content_type_color(seq.active_pattern);
      display.print(seq_find_shortname(seq.menu - 2)[0] );

      display.setCursor_textGrid(seq.menu , 2);
      display.setTextColor(GREEN, GREY2);
      display.print(seq_find_shortname(seq.menu - 1)[0] );
      set_pattern_content_type_color(seq.active_pattern);

      if (seq.menu < 16)
      {
        display.setCursor_textGrid(seq.menu + 1, 2);
        if (seq.vel[seq.active_pattern][seq.menu] > 209) //if pitched sample, change color
          display.setTextColor(COLOR_PITCHSMP, COLOR_BACKGROUND);
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
            display.setTextColor(COLOR_SYSTEXT, COLOR_BACKGROUND);
            display.print("V:");
            sprintf(tmp, "%03d", seq.vel[seq.active_pattern][seq.menu - 1]);
            display.print(tmp);
            display.print(" ");
            display.setTextColor(COLOR_DRUMS, COLOR_BACKGROUND);
            display.show(1, 7, 7, seq_find_drum_name_from_note( seq.note_data[seq.active_pattern][seq.menu - 1]) );
            if (seq_active_function == 99)
            {
              display.setCursor(1 * CHAR_width, 4 * CHAR_height + 17);
              display.setTextSize(1);
              display.setTextColor(GREEN, COLOR_BACKGROUND);
              display.print(F("EDIT VELOCITY OF STEP "));
              display.setTextColor(COLOR_SYSTEXT, COLOR_BACKGROUND);
              display.print(seq.menu);
              display.setTextColor(GREEN, COLOR_BACKGROUND);
              display.print(" ? ");
              display.setTextSize(2);
              display.setTextColor(COLOR_SYSTEXT, COLOR_BACKGROUND);
            }
          }
          else
          { // else it is a live-pitched sample
            display.setTextColor(COLOR_SYSTEXT, COLOR_BACKGROUND);
            display.setCursor_textGrid(1, 1);
            display.print(F("Smp:["));
            display.setCursor_textGrid(14, 1);
            display.print("]");
            display.setTextColor(COLOR_PITCHSMP, COLOR_BACKGROUND);
            display.show(1, 6, 8, basename(drum_config[seq.vel[seq.active_pattern][seq.menu - 1] - 210].name));

            if (seq_active_function == 99)
            {
              display.setCursor(1 * CHAR_width, 4 * CHAR_height + 17);
              display.setTextSize(1);
              display.setTextColor(GREEN, COLOR_BACKGROUND);
              display.print(F("REPLACE "));
              display.setTextColor(COLOR_BACKGROUND, GREEN);
              display.print( basename(drum_config[seq.vel[seq.active_pattern][seq.menu - 1] - 210].name));
              display.setTextColor(GREEN, COLOR_BACKGROUND);
              display.print(F(" IN STEP "));
              display.setTextColor(COLOR_SYSTEXT, COLOR_BACKGROUND);
              display.print(seq.menu);
              display.setTextColor(GREEN, COLOR_BACKGROUND);
              display.print(F(" ? "));
              display.setTextColor(COLOR_SYSTEXT, COLOR_BACKGROUND);
              display.setTextSize(2);
            }
          }
        }
        else
        {
          if ( seq.note_data[seq.active_pattern][seq.menu - 1] != 130  ) //note not latched
          {
            if (seq.content_type[seq.active_pattern] < 2)
            {
              display.setCursor_textGrid(1, 1);
              display.print(F("Vel:"));
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
            display.print(F("latched note "));
          }
          if (seq.content_type[seq.active_pattern] > 1) // is not drum or inst, print chord
          {
            display.setCursor_textGrid(1, 1);
            display.setTextColor(COLOR_SYSTEXT, COLOR_BACKGROUND);
            display.print("[");
            set_pattern_content_type_color(seq.active_pattern);
            print_chord_name(seq.menu - 1);
            display.setTextColor(COLOR_SYSTEXT, COLOR_BACKGROUND);
            display.print("]");
          }
        }
        seq_printVelGraphBar_single_step(seq.menu - 1, GREEN);
        if (seq.menu - 1 > 0)
          seq_printVelGraphBar_single_step(seq.menu - 2, GREY1); //previous
        if (seq.menu - 1 < 15)
          seq_printVelGraphBar_single_step(seq.menu, GREY1 ); //next
      }
      else
      {
        display.setCursor_textGrid(1, 1);
        display.print("              ");
        if (seq.menu - 1 > 0)
          seq_printVelGraphBar_single_step(seq.menu - 2, GREY1); //previous
        if (seq.menu - 1 < 15)
          seq_printVelGraphBar_single_step(seq.menu, GREY1 ); //next

        //clear "Insert xyz ?"  message
        display.fillRect(1 * CHAR_width, 4 * CHAR_height + 17, 222, 8, COLOR_BACKGROUND);
      }
    }
    if (seq.menu == 19 ) //edit content type of pattern
    {
      if (seq_active_function != 1)
      {
        //fix last step of sequence when switching to sub menus functions
        print_track_steps_detailed(CHAR_width, CHAR_height * 5 + 3, seq.menu - 1, false, true);
        seq_printVelGraphBar_single_step(15, GREY1);
        set_pattern_content_type_color(seq.active_pattern);
        display.setTextSize(2);
        display.setCursor_textGrid(16 , 2);
        display.print(seq_find_shortname(15)[0] );
        display.setCursor(1 * CHAR_width, 4 * CHAR_height + 17);
        display.setTextSize(1);
        display.setTextColor(GREEN, COLOR_BACKGROUND);
        display.print(F("EDIT CONTENT TYPE OF PATTERN "));
        display.setTextColor(COLOR_SYSTEXT, COLOR_BACKGROUND);
        display.print(seq.active_pattern);
        display.setTextColor(GREEN, COLOR_BACKGROUND);
        display.print(" ? ");
      }
      print_edit_mode();
      display.setTextSize(1);
      display.setTextColor(COLOR_SYSTEXT, COLOR_PITCHSMP);
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
      seq_sub_pat_chain(CHAR_width * 21, CHAR_height * 7, false);
    }
    else if (seq.menu > 20  && seq.menu < 27) // sub menus functions: track type
    {
      //disable menu 19
      display.setCursor(CHAR_width, CHAR_height * 4 + 3);
      display.setTextSize(1);
      display.setTextColor(GREY1, COLOR_BACKGROUND);
      display.print("CONT.TYPE:");
      display.setCursor(CHAR_width * 6, CHAR_height * 4 + 3);
      print_content_type();
      display.setTextSize(2);
      // disable menu 19 end

      print_edit_mode();
      display.setTextSize(1);
      display.setCursor(1 * CHAR_width, 4 * CHAR_height + 17);
      if (seq_active_function != 1)
      {
        display.setTextColor(GREEN, COLOR_BACKGROUND);
        display.print(F("EDIT TRACK TYPE OF "));
        display.setTextColor( COLOR_SYSTEXT, COLOR_BACKGROUND);
        display.print(F("TRACK "));
        display.print( seq.menu - 20);
        display.setTextColor(GREEN, COLOR_BACKGROUND);
        display.print(F(" ?    "));

        display.setTextColor(COLOR_SYSTEXT, COLOR_BACKGROUND);
        display.setTextSize(2);
      } else
      {
        display.setTextColor(GREEN, COLOR_BACKGROUND);
        display.print("ASSIGN TRACK TYPE = ");

        set_track_type_color(seq.menu - 21);

        if (seq.track_type[seq.menu - 21] == 0 )   display.print(F("Drums"));
        else if (seq.track_type[seq.menu - 21] == 1 ) display.print(F("Instr."));
        else if (seq.track_type[seq.menu - 21] == 2 ) display.print(F("Chord"));
        else if (seq.track_type[seq.menu - 21] == 3 ) display.print(F("Arpeggio"));
        display.setTextColor(GREEN, COLOR_BACKGROUND);
        display.print(" ?    ");
        display.setTextSize(2);
      }
      seq_sub_pat_chain(CHAR_width * 21, CHAR_height * 7, false);
    }
    else if (seq.menu > 20 + 6  && seq.menu < 27 + 6) // sub menus functions: assign dexed instance to tracks
    {
      print_edit_mode();
      display.setTextSize(1);
      display.setCursor(1 * CHAR_width, 4 * CHAR_height + 17);
      if (seq_active_function != 1)
      {
        if (seq.track_type[seq.menu - 21 - 6] > 0 )
        {
          display.setTextColor(GREEN, COLOR_BACKGROUND);
          display.print(F("ASSIGN DEXED/EPIANO TO "));
          display.setTextColor( COLOR_SYSTEXT, COLOR_BACKGROUND);
          display.print(F("TRK "));
          display.print( seq.menu - 20 - 6);
          display.setTextColor(GREEN, COLOR_BACKGROUND);
          display.print(F(" ?    "));

          display.setTextColor(COLOR_SYSTEXT, COLOR_BACKGROUND);
          display.setTextSize(2);
        }
        else
        {
          display.setTextColor(COLOR_SYSTEXT, COLOR_BACKGROUND);
          display.print(F("TRACK TYPE "));
          display.setTextColor(RED, COLOR_BACKGROUND);
          display.print(F("INVALID "));
          display.setTextColor(COLOR_SYSTEXT, COLOR_BACKGROUND);
          display.print(F("FOR DEXED/EP "));
        }
      }
      else
      {
        display.setTextColor(COLOR_SYSTEXT, COLOR_BACKGROUND);
        display.print(F("SET TRACK TO INSTR: "));
        display.setTextColor(GREEN, COLOR_BACKGROUND);
        if (seq.inst_dexed[seq.menu - 21 - 6] < 2)
        {
          display.print(F("DEXED "));
          display.print(seq.inst_dexed[seq.menu - 21 - 6] + 1);
        }
        else if (seq.inst_dexed[seq.menu - 21 - 6] == 2)
        {
          display.print(F("EPIANO"));
        }
        else if (seq.inst_dexed[seq.menu - 21 - 6] == 3)
        {
          display.print(F("SID saw"));
        }
        else if (seq.inst_dexed[seq.menu - 21 - 6] == 4)
        {
          display.print(F("SID sqr"));
        }
        display.setTextColor(COLOR_SYSTEXT, COLOR_BACKGROUND);
        display.print(F(" ?  "));
      }
      seq_sub_pat_chain(CHAR_width * 21, CHAR_height * 7, false);
    }

    else if (seq.menu == 33) // sub menus function: default chord instrument (dexed or epiano)
    {
      print_edit_mode();

      display.setCursor(1 * CHAR_width, 4 * CHAR_height + 17);
      if (seq_active_function != 1)
      {
        display.setTextColor(GREEN, COLOR_BACKGROUND);
        display.setTextSize(1);
        display.print(F("CHANGE DEFAULT CHORD INSTR. ?  "));
        display.setTextColor(COLOR_SYSTEXT, COLOR_BACKGROUND);
        display.setTextSize(2);
      } else
      {
        display.setTextSize(1);
        display.setTextColor(GREEN, COLOR_BACKGROUND);
        display.print(F("ASSIGN CHORD INSTR. = "));
        display.setTextColor(COLOR_SYSTEXT, COLOR_BACKGROUND);
        if (seq.chord_dexed_inst == 0)
          display.print(F( "DEXED1"));
        else if (seq.chord_dexed_inst == 1)
          display.print(F( "DEXED2"));
        else if (seq.chord_dexed_inst == 2)
          display.print(F( "EPIANO"));
        display.print(F( "  "));
        display.setTextSize(2);
      }
      seq_sub_pat_chain(CHAR_width * 21, CHAR_height * 7, false);
    }
    else if (seq.menu == 34) // sub menus function: arp chord size
    {
      print_edit_mode();
      display.setCursor(1 * CHAR_width, 4 * CHAR_height + 17);
      if (seq_active_function != 1)
      {
        display.setTextColor(GREEN, COLOR_BACKGROUND);
        display.setTextSize(1);
        display.print(F("CHANGE NUMBER OF CHORD NOTES ?  "));
        display.setTextColor(COLOR_SYSTEXT, COLOR_BACKGROUND);
        display.setTextSize(2);
      } else
      {
        display.setTextSize(1);
        display.setTextColor(GREEN, COLOR_BACKGROUND);
        display.print(F("SET NUMBER OF CHORD NOTES = "));
        display.setTextColor(COLOR_SYSTEXT, COLOR_BACKGROUND);
        seq_print_formatted_number(seq.arp_chord, 2);
        display.print(F("   "));
        display.setTextSize(2);
      }
      seq_sub_pat_chain(CHAR_width * 21, CHAR_height * 7, false);
    }
    else if (seq.menu == 35) // sub menus function: arp / chord velocity
    {
      print_edit_mode();
      display.setCursor(1 * CHAR_width, 4 * CHAR_height + 17);
      if (seq_active_function != 1)
      {
        display.setTextColor(GREEN, COLOR_BACKGROUND);
        display.setTextSize(1);
        display.print(F("SET ARP/CHORD VELOCITY ?       "));
        display.setTextColor(COLOR_SYSTEXT, COLOR_BACKGROUND);
        display.setTextSize(2);
      } else
      {
        display.setTextSize(1);
        display.setTextColor(GREEN, COLOR_BACKGROUND);
        display.print(F("ASSIGN ARP/CHORD VELOCITY = "));
        display.setTextColor(COLOR_SYSTEXT, COLOR_BACKGROUND);
        seq_print_formatted_number(seq.chord_velocity, 3);
        display.print(F(" "));
        display.setTextSize(2);
      }
      seq_sub_pat_chain(CHAR_width * 21, CHAR_height * 7, false);
    }
    else if (seq.menu == 36) // sub menus function: max notes in fast arps
    {
      print_edit_mode();
      display.setCursor(1 * CHAR_width, 4 * CHAR_height + 17);
      if (seq_active_function != 1)
      {
        display.setTextColor(GREEN, COLOR_BACKGROUND);
        display.setTextSize(1);
        display.print(F("SET MAX. ARP/CHORD NOTES ?    "));
        display.setTextColor(COLOR_SYSTEXT, COLOR_BACKGROUND);
        display.setTextSize(2);
      } else
      {
        display.setTextSize(1);
        display.setTextColor(GREEN, COLOR_BACKGROUND);
        display.print(F("MAX ARP NOTES STEPS/EVENT = "));
        display.setTextColor(COLOR_SYSTEXT, COLOR_BACKGROUND);
        display.print(seq.arp_num_notes_max);
        display.print(F(" "));
        display.setTextSize(2);
      }
      seq_sub_pat_chain(CHAR_width * 21, CHAR_height * 7, false);
    }
    else if (seq.menu == 16 )
    {
      display.setTextSize(1);
      display.setTextColor(GREY1, COLOR_BACKGROUND);
      display.setCursor(CHAR_width, CHAR_height * 4 + 3);
      display.print(F("CONT.TYPE:"));
      display.setCursor(CHAR_width * 6, CHAR_height * 4 + 3);
      print_content_type();
      display.setTextSize(2);
      print_edit_mode();
      seq_sub_pat_chain(CHAR_width * 21, CHAR_height * 7, false);
    }
  }
  if (LCDML.FUNC_close())     // ****** STABLE END *********
  {
    encoderDir[ENC_R].reset();

    if (seq.menu_status != 2) // don't clear screen when jumping to pattern editor
    {
      display.fillScreen(COLOR_BACKGROUND);
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
  display.fillRect(13 * CHAR_width, CHAR_height, 13, 16, COLOR_BACKGROUND);
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
    display.drawBitmap(13 * CHAR_width + 3, CHAR_height + 3, special_chars[21], 8, 8, COLOR_PITCHSMP);
  }
}

void arp_refresh_display_play_status()
{

  display.fillRect(17 * CHAR_width, CHAR_height, 13, 16, COLOR_BACKGROUND);
  if (seq.running == false )
  {
    //play symbol
    display.drawBitmap(17 * CHAR_width , CHAR_height + 3, special_chars[19], 8, 8, GREEN);
  }  else if (seq.running == true )
  {
    seq.note_in = 0;
    //stop symbol
    display.drawBitmap(17 * CHAR_width , CHAR_height + 3, special_chars[21], 8, 8, COLOR_PITCHSMP);
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
      display.print(F("none      "));
    else if (drum_config[activesample].drum_class == 1)
      display.print(F("BASSDRUM  "));
    else if (drum_config[activesample].drum_class == 2)
      display.print(F("SNARE     "));
    else if (drum_config[activesample].drum_class == 3)
      display.print(F("HIHAT     "));
    else if (drum_config[activesample].drum_class == 4)
      display.print(F("HANDCLAP  "));
    else if (drum_config[activesample].drum_class == 5)
      display.print(F("RIDE      "));
    else if (drum_config[activesample].drum_class == 6)
      display.print(F("CRASH     "));
    else if (drum_config[activesample].drum_class == 7)
      display.print(F("LOW TOM   "));
    else if (drum_config[activesample].drum_class == 8)
      display.print(F("MID TOM   "));
    else if (drum_config[activesample].drum_class == 9)
      display.print(F("HIGH TOM  "));
    else if (drum_config[activesample].drum_class == 10)
      display.print(F("PERCUSSION"));
    else if (drum_config[activesample].drum_class == 11)
      display.print(F("POLY/SYNTH"));
    else
      display.print(F("other     "));
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
      display.print(F("         "));
      display.setCursor_textGrid(17, 2);
      display.print(F("  "));
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
      display.print(F("         "));
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
        display.print(F("Copy Pattern:"));
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
    display.print(F("          to: [  ]"));
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
    display.print(F("        with: [  ]"));
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
        display.print(F("Fill Pattern:"));
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
    if (seq.temp_active_menu == 0) display.print(F(" 1/4"));
    else if (seq.temp_active_menu == 1) display.print(F(" 1/8"));
    else if (seq.temp_active_menu == 2) display.print(F("1/16"));
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
      if (seq.temp_active_menu == 0) display.print(F(" 1/4"));
      else if (seq.temp_active_menu == 1) display.print(F(" 1/8"));
      else if (seq.temp_active_menu == 2) display.print(F("1/16"));
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
      if (seq.temp_active_menu == 0) display.print(F(" 1/4"));
      else if (seq.temp_active_menu == 1) display.print(F(" 1/8"));
      else if (seq.temp_active_menu == 2) display.print(F("1/16"));
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
        display.print(F("Transpose:   [ 00]"));
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
      display.setTextColor(COLOR_SYSTEXT, COLOR_BACKGROUND);
      display.print(F("[EDIT "));
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
      display.setTextColor(COLOR_SYSTEXT, COLOR_BACKGROUND);
      display.print("[  ");
      display.setTextColor(RED, COLOR_BACKGROUND);
      display.print(F("CLEAR"));
      display.setTextColor(COLOR_SYSTEXT, COLOR_BACKGROUND);
      display.print("  ]");
      display.setTextColor(COLOR_SYSTEXT, COLOR_BACKGROUND);
    }
    display.setTextSize(1);
    display.setCursor(1 * CHAR_width, 4 * CHAR_height + 17);
    display.setTextSize(1);
    display.setTextColor(COLOR_SYSTEXT, COLOR_BACKGROUND);
    display.print(F("SELECT [ "));
    display.setTextColor(RED, COLOR_BACKGROUND);
    display.print("C0");
    display.setTextColor(COLOR_SYSTEXT, COLOR_BACKGROUND);
    display.print(F(" ] TO CLEAR STEP!  "));
    display.setTextSize(2);
  }
}

void print_current_sample_and_pitch_buffer()
{
  display.setCursor(CHAR_width * 21, CHAR_height * 5 + 23);
  display.setTextSize(1);
  display.setTextColor(GREY2, COLOR_BACKGROUND);
  display.print(F("SAMPLE BUFFER: "));
  display.setTextColor(COLOR_SYSTEXT, COLOR_BACKGROUND);
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
  display.setTextColor(GREY2);
  display.print(F("SAMPLE TYPE: "));
  display.setTextColor(GREY2, COLOR_BACKGROUND);
  print_sample_type();
  display.setTextColor(GREY2, COLOR_BACKGROUND);
  display.setCursor(CHAR_width * 21, CHAR_height * 5 + 23 + 2 * CHAR_height - 10);
  display.print(F("NOTE BUFFER: "));
  display.setTextColor(COLOR_SYSTEXT, COLOR_BACKGROUND);
  display.print(noteNames[temp_int % 12 ]);
  display.print( (temp_int / 12) - 1);
  display.print(" ");

  display.setTextSize(2);
}



void set_sample_type_color()
{
  if ( drum_config[activesample].midinote > 209 )
    display.setTextColor(COLOR_BACKGROUND, COLOR_PITCHSMP);
  else
    display.setTextColor(COLOR_BACKGROUND, COLOR_DRUMS);
}

void set_sample_type_color_of(uint8_t samplekey)
{
  if ( samplekey > 209 )
    display.setTextColor(COLOR_BACKGROUND, COLOR_PITCHSMP);
  else
    display.setTextColor(COLOR_BACKGROUND, COLOR_DRUMS);
}

void seq_pattern_editor_update_disp_instr()
{
  if (seq.cycle_touch_element == 1)
  {
    display.setCursor(CHAR_width, 110);
    display.setTextColor(COLOR_SYSTEXT, COLOR_BACKGROUND);
    display.setTextSize(1);
    display.print(F("PATTERN "));
    display.setTextColor(GREEN, COLOR_BACKGROUND);
    seq_print_formatted_number(seq.active_pattern, 2);
    display.setTextColor(COLOR_SYSTEXT, COLOR_BACKGROUND);
    display.print(F(" IS PLAYING:"));
    if (seq.content_type[seq.active_pattern] == 1) //Inst Pattern
    {
      //UI_draw_waveform(activesample);


      print_voice_settings_in_pattern_editor(12, 125);
    }
    else if (seq.content_type[seq.active_pattern] == 0)
    {
      display.setTextColor(COLOR_SYSTEXT, COLOR_BACKGROUND);
      display.setCursor(CHAR_width, 125);
      display.setTextSize(2);
      display.print(F("DRUMS"));
      display.fillRect(CHAR_width * 6, 124, 129, CHAR_height, COLOR_BACKGROUND);
    }
    else
      display.fillRect(CHAR_width, 110, 70, 0, COLOR_PITCHSMP);

  }
}
void seq_pattern_editor_update_dynamic_elements()
{

  display.setTextSize(1);

  display.fillRect(366, 0 + 8, 50, 51 - 13, COLOR_PITCHSMP);
  display.fillRect(366, 50 + 5, 50, 49 - 13, COLOR_PITCHSMP);

  display.fillRect(422, 0 + 8, 51, 51 - 13, COLOR_PITCHSMP);
  display.fillRect(422, 50 + 5, 51, 49 - 13, COLOR_PITCHSMP);

  if (seq.cycle_touch_element == 0)
  {
    border3_large_clear();
    border3();
    border4();

    //touch buttons
    display.setCursor(432, CHAR_height * 1 );
    display.setTextColor(COLOR_SYSTEXT, COLOR_PITCHSMP);
    display.print(F("TOUCH"));
    display.setCursor(424, CHAR_height * 2 - 8);
    display.print(F("KEYBOARD"));

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
    seq_sub_pat_chain(CHAR_width * 21, CHAR_height * 7, true);
    print_current_sample_and_pitch_buffer();
  }
  else if (seq.cycle_touch_element == 1)
  {
    border3_large_clear();
    border3_large();

    //oct +- buttons
    display.setTextColor(COLOR_SYSTEXT, COLOR_PITCHSMP);
    display.fillRect(411 + 7, 171, 55, 51 - 13, COLOR_PITCHSMP);
    display.fillRect( 7, 171, 55, 51 - 13, COLOR_PITCHSMP);

    display.setTextSize(1);
    display.setCursor(421 + 7, 179 );
    display.print(F("OCTAVE"));
    display.setCursor( 17, 179 );
    display.print(F("OCTAVE"));

    display.setTextSize(2);
    display.setCursor(411 + 7 + 22, 189 );
    display.print(F("+"));
    display.setCursor( 7 + 22, 189 );
    display.print(F("-"));

    //touch buttons
    display.setTextSize(1);
    display.setCursor(432, CHAR_height * 1 );
    display.setTextColor(COLOR_SYSTEXT, COLOR_PITCHSMP);
    display.print(F("TOUCH"));
    display.setCursor(424, CHAR_height * 2 - 8);
    display.print(F("PAT.EDIT"));
    virtual_keyboard(220);

    seq_pattern_editor_update_disp_instr();
  }

  display.setTextSize(2);


}
void UI_func_seq_pattern_editor(uint8_t param)
{
  if (LCDML.FUNC_setup())         // ****** SETUP *********
  {
    // setup function
    seq.menu = 3;
    if (seq.menu_status != 2)
    {
      display.fillScreen(COLOR_BACKGROUND);
      border1();
      border2();

      seq_pattern_editor_update_dynamic_elements();

      // display.drawLine(411, 0, 411, 99, GREY4);
      display.drawLine(360, 0, 360, 99, GREY4);

      display.setCursor(CHAR_width, CHAR_height * 4 + 3);
      display.setTextSize(1);
      display.setTextColor(GREY1, COLOR_BACKGROUND);
      display.print(F("CONT.TYPE:"));
      display.setCursor(CHAR_width * 6, CHAR_height * 4 + 3);
      print_content_type ();
    }
    print_edit_mode();

    display.setTextSize(2);

    check_variable_samples_basespeed();
    temp_int = seq.note_data[seq.active_pattern][0];
    encoderDir[ENC_R].reset();
    seq.note_in = 0;
    display.setTextSize(2);
    seq_refresh_display_play_status();
    display.setCursor_textGrid(16, 1);
    display.setTextColor(GREY2, COLOR_BACKGROUND);
    seq_print_formatted_number(seq.active_pattern, 2);

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
                if (seq.content_type[seq.active_pattern] == 2  ) //Chords
                  seq.vel[seq.active_pattern][seq.menu - 3] = 200;
                else // normal note
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
      if (seq.cycle_touch_element == 0)
        print_current_sample_and_pitch_buffer();
      display.setTextColor(COLOR_SYSTEXT, COLOR_BACKGROUND);
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
            display.setTextColor(COLOR_DRUMS, COLOR_BACKGROUND);
          else
            display.setTextColor(COLOR_PITCHSMP, COLOR_BACKGROUND);

          display.show(1, 2, 9, basename(drum_config[activesample].name));
          display.setTextColor(COLOR_SYSTEXT, COLOR_BACKGROUND);
        } else if (activesample == NUM_DRUMSET_CONFIG - 1) {
          display.setCursor_textGrid(2, 1);
          display.print(F("EMPTY    "));
        } else if (activesample == NUM_DRUMSET_CONFIG ) {
          display.setCursor_textGrid(2, 1);
          display.print(F("Clear Pat"));
        } else if (activesample == NUM_DRUMSET_CONFIG + 1) {
          display.setCursor_textGrid(2, 1);
          display.print(F("Clear All"));
        }
        else if (activesample == NUM_DRUMSET_CONFIG + 2) {
          display.setCursor_textGrid(2, 1);
          display.print(F("Copy Pat."));
        }
        else if (activesample == NUM_DRUMSET_CONFIG + 3) {
          display.setCursor_textGrid(2, 1);
          display.print(F("Swap Pat"));
        }
        else if (activesample == NUM_DRUMSET_CONFIG + 4) {
          display.setCursor_textGrid(2, 1);
          display.print(F("Fill Pat."));
        }
        else if (activesample == NUM_DRUMSET_CONFIG + 5) {
          display.setCursor_textGrid(2, 1);
          display.print(F("Transpose"));
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
            // print_chord_name(seq.menu - 3);
            display.setCursor_textGrid(2, 1);
            display.print(noteNames[temp_int % 12 ]);
            display.print( (temp_int / 12) - 1);
          }
        } else if (temp_int == 109)
        {
          display.setCursor_textGrid(2, 1);
          display.print(F("EMPTY    "));
        } else if (temp_int == 110)
        {
          display.setCursor_textGrid(2, 1);
          display.print(F("LATCH    "));
        } else if (temp_int == 111)
        {
          display.setCursor_textGrid(2, 1);
          display.print(F("ClearPat."));
        } else if (temp_int == 112)
        {
          display.setCursor_textGrid(2, 1);
          display.print(F("Clear All"));
        } else if (temp_int == 113)
        {
          display.setCursor_textGrid(2, 1);
          display.print(F("Copy Pat."));
        }  else if (temp_int == 114)
        {
          display.setCursor_textGrid(2, 1);
          display.print(F("Swap Pat."));
        } else if (temp_int == 115)
        {
          display.setCursor_textGrid(2, 1);
          display.print(F("Fill Pat."));
        } else if (temp_int == 116)
        {
          display.setCursor_textGrid(2, 1);
          display.print(F("Transpose"));
        }
        display.setCursor_textGrid(11, 1);
        display.print("]");
      }
    }
    else if (seq.menu == 1)
    {
      display.setTextColor(GREY2, COLOR_BACKGROUND);
      display.setCursor_textGrid(16, 1);
      seq_print_formatted_number(seq.active_pattern, 2);
      display.setTextColor(COLOR_SYSTEXT, COLOR_BACKGROUND);
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
          display.setTextColor(COLOR_DRUMS, COLOR_BACKGROUND);
        else
          display.setTextColor(COLOR_PITCHSMP, COLOR_BACKGROUND);

        display.show(1, 2, 9, basename(drum_config[activesample].name));
        display.setTextColor(COLOR_SYSTEXT, COLOR_BACKGROUND);
      }
    }
    if (seq.menu == 2)
    {
      seq_pattern_editor_update_disp_instr();
      display.fillRect(1 * CHAR_width, 4 * CHAR_height + 17, 222, 8, COLOR_BACKGROUND);
      display.setTextColor(COLOR_SYSTEXT, COLOR_BACKGROUND);
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
          display.setTextColor(COLOR_DRUMS, COLOR_BACKGROUND);
        else
          display.setTextColor(COLOR_PITCHSMP, COLOR_BACKGROUND);
        display.show(1, 2, 9, basename(drum_config[activesample].name));
        display.setTextColor(COLOR_SYSTEXT, COLOR_BACKGROUND);
        display.print("  ");
        if (seq.cycle_touch_element == 0)
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
      }
      else
      {
        if (seq.cycle_touch_element == 0)
        {
          // Print note buffer when switching track and track is an instrument track
          display.setCursor_textGrid(1, 1);
          display.print(" ");
          display.setTextColor(COLOR_INSTR, COLOR_BACKGROUND);
          display.print(noteNames[temp_int % 12 ][0] );
          if (noteNames[temp_int % 12 ][1] != '\0' )
          {
            display.print(noteNames[temp_int % 12 ][1] );
          }
          display.print( (temp_int / 12) - 1);
          display.print(F("       "));
          display.setTextColor(COLOR_SYSTEXT, COLOR_BACKGROUND);

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
      display.setTextSize(2);
      seq_printAllSeqSteps();
      seq_printVelGraphBar();

      if (seq.cycle_touch_element == 0)
      {
        print_current_sample_and_pitch_buffer();
        seq_sub_pat_chain(CHAR_width * 21, CHAR_height * 7, false);
      }
    }
    if (seq.menu == 3)
    {
      display.setTextColor(GREY2, COLOR_BACKGROUND);
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
      {
        if (seq.cycle_touch_element == 0)
          print_track_steps_detailed(CHAR_width, CHAR_height * 5 + 3, seq.menu - 3, false, false);
      }
      else
      {
        if (seq.cycle_touch_element == 0)
          print_single_pattern_pianoroll(CHAR_width, 316, seq.active_pattern, seq.menu - 3, false);
      }
      if (seq.menu == 3)
        display.setCursor_textGrid(1, 2);
      else
        display.setCursor_textGrid(seq.menu - 2, 2);
      display.setTextColor(GREEN, GREY2);
      display.print(seq_find_shortname(seq.menu - 3)[0]);
      if (seq.menu > 3)
      {
        display.setCursor_textGrid(seq.menu - 3, 2);
        if (seq.vel[seq.active_pattern][seq.menu - 4] > 209) //if pitched sample, change color
          display.setTextColor(COLOR_PITCHSMP, COLOR_BACKGROUND);
        else
          set_pattern_content_type_color(seq.active_pattern);
        display.print(seq_find_shortname(seq.menu - 4)[0]);
      }
      if (seq.menu < 18)
      {
        display.setCursor_textGrid(seq.menu - 1, 2);
        if (seq.vel[seq.active_pattern][seq.menu - 2] > 209) //if pitched sample, change color
          display.setTextColor(COLOR_PITCHSMP, COLOR_BACKGROUND);
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
              display.setTextColor(COLOR_SYSTEXT, COLOR_BACKGROUND);
              display.setCursor(1 * CHAR_width, 4 * CHAR_height + 17);
              display.setTextSize(1);
              display.setTextColor(GREEN, COLOR_BACKGROUND);
              display.print("CLEAR ");
              set_sample_type_color();
              display.print( basename(drum_config[activesample].name));
              display.setTextColor(GREEN, COLOR_BACKGROUND);
              display.print(" FROM STEP ");
              display.setTextColor(COLOR_SYSTEXT, COLOR_BACKGROUND);
              display.print(seq.menu - 2);
              display.setTextColor(GREEN, COLOR_BACKGROUND);
              display.print(" ?");
              fill_up_with_spaces_left_window();
              display.setTextColor(COLOR_SYSTEXT, COLOR_BACKGROUND);
            }
            //else if there is an other note, ask to replace it
            else if (seq.note_data[seq.active_pattern][seq.menu - 3] != drum_config[activesample].midinote &&
                     seq.note_data[seq.active_pattern][seq.menu - 3] > 0 )
            {
              display.setTextColor(COLOR_SYSTEXT, COLOR_BACKGROUND);
              display.setCursor(1 * CHAR_width, 4 * CHAR_height + 17);
              display.setTextSize(1);
              display.setTextColor(GREEN, COLOR_BACKGROUND);
              display.print(F("REPLACE "));
              set_sample_type_color_of(seq.note_data[seq.active_pattern][seq.menu - 3]);
              display.print  ( find_long_drum_name_from_note(  seq.note_data[seq.active_pattern][seq.menu - 3])   )   ;
              display.setTextColor(GREEN, COLOR_BACKGROUND);
              display.print(F(" WITH "));
              set_sample_type_color();
              display.print( basename(drum_config[activesample].name));
              display.setTextColor(GREEN, COLOR_BACKGROUND);
              display.print(F(" ?"));
              fill_up_with_spaces_left_window();
              display.setTextColor(COLOR_SYSTEXT, COLOR_BACKGROUND);
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
                display.setTextColor(COLOR_SYSTEXT, COLOR_BACKGROUND);
                display.setCursor(1 * CHAR_width, 4 * CHAR_height + 17);
                display.setTextSize(1);
                display.setTextColor(GREEN, COLOR_BACKGROUND);
                display.print(F("EDIT/"));
                display.setTextColor(RED, COLOR_BACKGROUND);
                display.print(F("DELETE "));
                set_sample_type_color();
                display.print( basename(drum_config[activesample].name));
                display.setTextColor(GREEN, COLOR_BACKGROUND);
                display.print(F(" IN STEP "));
                display.setTextColor(COLOR_SYSTEXT, COLOR_BACKGROUND);
                display.print(seq.menu - 2);
                display.setTextColor(GREEN, COLOR_BACKGROUND);
                display.print(F(" ?"));
                display.setTextColor(COLOR_SYSTEXT, COLOR_BACKGROUND);
              }
              else if (seq.vel[seq.active_pattern][seq.menu - 3] != drum_config[activesample].midinote) // pitched sample is different, ask to replace
              {
                display.setTextColor(COLOR_SYSTEXT, COLOR_BACKGROUND);
                display.setCursor(1 * CHAR_width, 4 * CHAR_height + 17);
                display.setTextSize(1);
                display.setTextColor(GREEN, COLOR_BACKGROUND);
                display.print(F("REPLACE "));
                set_sample_type_color_of(seq.vel[seq.active_pattern][seq.menu - 3]);
                display.print( basename(drum_config[ seq.vel[seq.active_pattern][seq.menu - 3] - 210  ].name)  );
                display.setTextColor(GREEN, COLOR_BACKGROUND);
                display.print(F(" WITH "));
                set_sample_type_color();
                display.print( basename(drum_config[activesample].name));

                display.setTextColor(GREEN, COLOR_BACKGROUND);
                display.print(" ?");
                display.setTextColor(COLOR_SYSTEXT, COLOR_BACKGROUND);
              }
            }
          }
        }
        else if (seq.note_data[seq.active_pattern][seq.menu - 3] == 0
                 && seq.vel[seq.active_pattern][seq.menu - 3] < 210)  //if step empty, print selected, active sample and ask if should be inserted
        {
          display.setCursor_textGrid(1, 1);
          display.print(" ");
          display.setTextColor(GREY2, COLOR_BACKGROUND);
          display.show(1, 2, 9, "--EMPTY--");
          display.setTextColor(COLOR_SYSTEXT, COLOR_BACKGROUND);
          display.setCursor(1 * CHAR_width, 4 * CHAR_height + 17);
          display.setTextSize(1);
          display.setTextColor(GREEN, COLOR_BACKGROUND);
          display.print(F("INSERT "));
          set_sample_type_color();
          display.print( basename(drum_config[activesample].name));
          display.setTextColor(GREEN, COLOR_BACKGROUND);
          display.print(F(" IN STEP "));
          display.setTextColor(COLOR_SYSTEXT, COLOR_BACKGROUND);
          display.print(seq.menu - 2);
          display.setTextColor(GREEN, COLOR_BACKGROUND);
          display.print(F(" ?"));
          fill_up_with_spaces_left_window();
          display.setTextColor(COLOR_SYSTEXT, COLOR_BACKGROUND);
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

        display.setTextColor(GREEN, COLOR_BACKGROUND);
        display.print(F("INSERT NOTE "));
        display.setTextColor(COLOR_INSTR, COLOR_BACKGROUND);
        display.print(noteNames[temp_int % 12]  [0] );
        if (noteNames[temp_int % 12 ][1] != '\0' )
        {
          display.print(noteNames[temp_int % 12 ][1] );
        }
        display.print( temp_int / 12 - 1);
        display.setTextColor(GREEN, COLOR_BACKGROUND);
        display.print(F(" IN STEP "));
        display.setTextColor(COLOR_SYSTEXT, COLOR_BACKGROUND);
        display.print(seq.menu - 2);
        display.setTextColor(GREEN, COLOR_BACKGROUND);
        display.print(F(" ?   "));
        display.setTextSize(2);
      }
      else  //change note if different
        if (seq.note_data[seq.active_pattern][seq.menu - 3] > 0 &&
            seq.note_data[seq.active_pattern][seq.menu - 3] != temp_int)
        {
          display.setCursor(1 * CHAR_width, 4 * CHAR_height + 17);
          display.setTextSize(1);
          display.setTextColor(GREEN, COLOR_BACKGROUND);
          display.print(F("CHANGE "));
          display.setTextColor(COLOR_INSTR, COLOR_BACKGROUND);
          display.print(noteNames[seq.note_data[seq.active_pattern][seq.menu - 3] % 12 ][0] );
          if (noteNames[seq.note_data[seq.active_pattern][seq.menu - 3] % 12 ][1] != '\0' ) {
            display.print(noteNames[seq.note_data[seq.active_pattern][seq.menu - 3] % 12 ][1] );
          }
          display.print( (seq.note_data[seq.active_pattern][seq.menu - 3] / 12) - 1);
          display.setTextColor(GREEN, COLOR_BACKGROUND);
          display.print(" > ");
          display.setTextColor(COLOR_INSTR, COLOR_BACKGROUND);
          display.print(noteNames[temp_int % 12]  [0] );
          if (noteNames[temp_int % 12 ][1] != '\0' )
          {
            display.print(noteNames[temp_int % 12 ][1] );
          }
          display.print( temp_int / 12 - 1);
          display.setTextColor(GREEN, COLOR_BACKGROUND);
          display.print(F(" IN STEP "));
          display.setTextColor(COLOR_SYSTEXT, COLOR_BACKGROUND);
          display.print(seq.menu - 2);
          display.setTextColor(GREEN, COLOR_BACKGROUND);
          display.print(" ? ");
          display.setTextSize(2);
        }
        else if (seq.note_data[seq.active_pattern][seq.menu - 3] == temp_int)  //ask to delete note if it is the same
        {
          display.setCursor(1 * CHAR_width, 4 * CHAR_height + 17);
          display.setTextSize(1);
          display.setTextColor(RED, COLOR_BACKGROUND);
          display.print(F("DELETE"));
          display.setTextColor(GREEN, COLOR_BACKGROUND);
          display.print(F(" NOTE "));
          display.setTextColor(COLOR_INSTR, COLOR_BACKGROUND);
          display.print(noteNames[seq.note_data[seq.active_pattern][seq.menu - 3] % 12 ][0] );
          if (noteNames[seq.note_data[seq.active_pattern][seq.menu - 3] % 12 ][1] != '\0' )
          {
            display.print(noteNames[seq.note_data[seq.active_pattern][seq.menu - 3] % 12 ][1] );
          }
          display.print( (seq.note_data[seq.active_pattern][seq.menu - 3] / 12) - 1);
          display.setTextColor(GREEN, COLOR_BACKGROUND);
          display.print(" IN STEP ");
          display.print(seq.menu - 2);
          display.setTextColor(GREEN, COLOR_BACKGROUND);
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
      display.fillScreen(COLOR_BACKGROUND);
  }
}

void UI_toplineInfoText(uint8_t ypos)
{
  display.fillRect(CHAR_width, ypos * CHAR_height - 1, 38 * CHAR_width, CHAR_height, COLOR_PITCHSMP);
  display.setTextColor(GREY2);

  display.setCursor(18 * CHAR_width + 6, CHAR_height * ypos + 4);
  display.setTextSize(1);

  display.print(F( "PERFORMANCE"));
  display.setTextSize(2);
  display.setCursor(24 * CHAR_width + 6, CHAR_height * ypos);
  display.setTextColor(COLOR_PITCHSMP);
  seq_print_formatted_number(configuration.sys.performance_number, 2);
  display.setTextColor(COLOR_SYSTEXT);
  display.show(ypos, 28, 11, seq.name);
  display.setTextSize(1);
  display.setCursor(108, CHAR_height + CHAR_height * ypos + 1);
  display.setTextColor(GREY2);
  display.print(F("COLOR CODES "));
  display.setTextColor(COLOR_INSTR);
  display.print(F("INSTR "));
  display.setTextColor(COLOR_DRUMS);
  display.print(F("DRUM / DRUM TRACK "));
  display.setTextColor(COLOR_PITCHSMP);
  display.print(F("PITCHED SAMPLE "));
  display.setTextColor(COLOR_CHORDS);
  display.print(F("CHORD/ARP"));

  display.setTextSize(2);
}
void UI_func_seq_tracker(uint8_t param)
{
  // SEQUENCER REWRITE

  //  if (LCDML.FUNC_setup())         // ****** SETUP *********
  //  {
  //    // setup function
  //    //check_variable_samples_basespeed();
  //    memset(seq.tracker_data_cache, 254, sizeof(seq.tracker_data_cache));
  //    memset(seq.tracker_names_cache, 254, sizeof(seq.tracker_names_cache));
  //    display.fillScreen(COLOR_BACKGROUND);
  //    UI_toplineInfoText(1);
  //    display.setTextSize(2);
  //    display.setCursor_textGrid(1, 1);
  //    display.setTextColor(COLOR_SYSTEXT);
  //    display.print (F("TRACKER"));
  //    display.setTextSize(1);
  //    for (uint8_t x = 0; x < NUM_SEQ_TRACKS; x++)
  //    {
  //      display.setCursor_textGrid(4 + 6 * x, 3);
  //      display.setCursor(display.getCursorX(), display.getCursorY() + 15);
  //      display.setTextColor(COLOR_SYSTEXT, COLOR_BACKGROUND);
  //      display.print (F("TRACK: "));
  //      display.print (x + 1);
  //      display.setCursor_textGrid(4 + 6 * x, 4);
  //      display.setCursor(display.getCursorX(), display.getCursorY() + 9);
  //      display.print ("PAT:");
  //      display.setCursor_textGrid(7 + 6 * x, 4);
  //      display.setCursor(display.getCursorX(), display.getCursorY() + 9);
  //      set_pattern_content_type_color( seq.patternchain[seq.chain_active_step][x] );
  //      seq_print_formatted_number( seq.patternchain[seq.chain_active_step][x], 2);
  //    }
  //    display.setTextSize(2);
  //    seq_print_step_numbers(CHAR_width, 96);
  //
  //    encoderDir[ENC_R].reset();
  //  }
  //  if (LCDML.FUNC_loop())          // ****** LOOP *********
  //  {
  //
  //    if ((LCDML.BT_checkDown() && encoderDir[ENC_R].Down()) || (LCDML.BT_checkUp() && encoderDir[ENC_R].Up()) || (LCDML.BT_checkEnter() && encoderDir[ENC_R].ButtonShort()))
  //    {
  //      if (LCDML.BT_checkDown())
  //        seq.scrollpos = constrain(seq.scrollpos + ENCODER[ENC_R].speed(), 0, 45);
  //      else if (LCDML.BT_checkUp())
  //        seq.scrollpos = constrain(seq.scrollpos - ENCODER[ENC_R].speed(), 0, 45);
  //    }
  //    display.setTextSize(2);
  //    seq_print_step_numbers(CHAR_width, 96);
  //    print_merged_pattern_fast_play_only( 4 * CHAR_width , 95,  0);
  //    print_merged_pattern_fast_play_only( 10 * CHAR_width , 95,  1);
  //    print_merged_pattern_fast_play_only( 16 * CHAR_width , 95,  2);
  //    print_merged_pattern_fast_play_only( 22 * CHAR_width , 95,  3);
  //    print_merged_pattern_fast_play_only( 28 * CHAR_width , 95,  4);
  //    print_merged_pattern_fast_play_only( 34 * CHAR_width , 95,  5);
  //  }
  //  if (LCDML.FUNC_close())     // ****** STABLE END *********
  //  {
  //    encoderDir[ENC_R].reset();
  //    display.fillScreen(COLOR_BACKGROUND);
  //  }
}

void pattern_preview_in_song(uint8_t patternno)
{
  display.setTextSize(2);
  seq.active_pattern = patternno;
  display.setCursor (0, 320 - CHAR_height);
  display.setTextColor(COLOR_SYSTEXT, COLOR_CHORDS);
  display.print("PAT:");
  seq_print_formatted_number( patternno , 2);
  display.setTextColor(GREY1, GREY4);
  display.print("[");

  for (uint8_t i = 0; i < 16; i++)
  {
    if ( seq.vel[seq.active_pattern][i] > 209 )
      display.setTextColor(COLOR_PITCHSMP, GREY4);
    else
    {
      if (seq.content_type[patternno] == 0) //Drumpattern
        display.setTextColor(COLOR_DRUMS, GREY4);
      else if (seq.content_type[patternno] == 1) //Instrument Pattern
        display.setTextColor(COLOR_INSTR, GREY4);
      else if (seq.content_type[patternno] == 2 || seq.content_type[patternno] == 3) //  chord or arp pattern
        display.setTextColor(COLOR_CHORDS, GREY4);
    }
    display.print(seq_find_shortname(i)[0]);
  }
  display.setTextColor(GREY1, GREY4);
  display.print("]");

}

void UI_func_song(uint8_t param)
{
  if (LCDML.FUNC_setup())         // ****** SETUP *********
  {
    // setup function
    encoderDir[ENC_R].reset();
    seq.edit_state = false;
    temp_int = 0;
    seq.cycle_touch_element = 0;
    seq.menu = 0;
    memset(seq.tracker_data_cache, 254, sizeof(seq.tracker_data_cache));
    display.fillScreen(COLOR_BACKGROUND);
    UI_toplineInfoText(1);
    display.setTextSize(2);
    display.setCursor_textGrid(1, 1);
    display.setTextColor(COLOR_SYSTEXT);
    display.print (F("SONG"));
    helptext_l("MOVE X");
    helptext_r("MOVE Y");
    for (uint8_t x = 0; x < NUM_SEQ_TRACKS; x++)
    {
      display.setCursor_textGrid(5 + 4 * x, 3);
      set_track_type_color(x);
      display.print (F("T"));
      display.print (x + 1);
    }
    display.setCursor_textGrid(30, 3);
    display.setTextColor(GREY1, COLOR_BACKGROUND);
    display.print (F("CHAIN:"));
    display.setTextSize(1);
    display.setTextColor(COLOR_SYSTEXT);
    display.setCursor(CHAR_width * 10 - 3, CHAR_height + 4);
    display.print (F("LENGTH"));
    display.setTextColor(GREY2, COLOR_BACKGROUND);
    display.setCursor_textGrid(30, 4);
    display.print (F("ST"));
    display.setCursor_textGrid(32, 4);
    display.print (F("PATTERN"));
    display.setCursor_textGrid(36, 4);
    display.print (F("TRANSP"));
    display.setTextColor(COLOR_SYSTEXT);

    display.setTextColor(GREY2, COLOR_BACKGROUND);
    for (uint8_t y = 0; y < 16; y++)  // chain
    {
      display.setCursor(CHAR_width * 30, CHAR_height * 5 + y * 13);
      seq_print_formatted_number( y + 1 , 2);
      display.setCursor(CHAR_width * 32, CHAR_height * 5 + y * 13);
      display.print("P");
      display.setCursor(CHAR_width * 36, CHAR_height * 5 + y * 13);
      display.print("T");
    }
    display.setTextSize(2);
  }
  if (LCDML.FUNC_loop())          // ****** LOOP *********
  {
    if ((LCDML.BT_checkDown() && encoderDir[ENC_R].Down()) || (LCDML.BT_checkUp() && encoderDir[ENC_R].Up()) || (LCDML.BT_checkEnter() && encoderDir[ENC_R].ButtonShort()))
    {
      if (LCDML.BT_checkDown())
      {
        if (seq.edit_state == false)
        {
          if (seq.cursor_scroll == 11)
          {
            seq.scrollpos++;
            if (seq.scrollpos > SONG_LENGHT - 12)
              seq.scrollpos = SONG_LENGHT - 12;
          }
          seq.cursor_scroll = constrain(seq.cursor_scroll + ENCODER[ENC_R].speed(), 0, 11);
        }
        else  if (seq.edit_state == true && seq.cycle_touch_element == 5)
        {
          if (temp_int == NUM_CHAINS)
            temp_int = 0;
          else
            temp_int = constrain( temp_int + ENCODER[ENC_R].speed(), 0, NUM_CHAINS );  // not -1: last element is for empty step
        }
        else  if ( (  seq.edit_state == true && seq.cycle_touch_element == 6) ||  (seq.edit_state == true && seq.cycle_touch_element == 8 )  )
        {
          seq.menu = constrain(  seq.menu + ENCODER[ENC_R].speed(), 0, 15 );
        }
        else  if ( (seq.edit_state == true && seq.cycle_touch_element == 7 ) || (seq.edit_state == true && seq.cycle_touch_element == 9 ) )
        {
          if (seq.sub_menu == NUM_CHAINS)
            seq.sub_menu = 0;
          else
            seq.sub_menu = constrain(  seq.sub_menu + ENCODER[ENC_R].speed(), 0, NUM_CHAINS );// not -1:last element is for empty step
        }

      }
      else if (LCDML.BT_checkUp())
      {
        if (seq.edit_state == false)
        {
          if (seq.cursor_scroll == 0 && seq.scrollpos > 0 )
          {
            seq.scrollpos--;
          }
          seq.cursor_scroll = constrain(seq.cursor_scroll - ENCODER[ENC_R].speed(), 0, 11);
        }
        else  if (seq.edit_state == true && seq.cycle_touch_element == 5)
        {
          if (temp_int == 0)
            temp_int = NUM_CHAINS;
          else
            temp_int = constrain( temp_int - ENCODER[ENC_R].speed(), 0, NUM_CHAINS ); // not -1:last element is for empty step
        }
        else  if ( ( seq.edit_state == true && seq.cycle_touch_element == 6) || (seq.edit_state == true && seq.cycle_touch_element == 8 ) )
        {
          seq.menu = constrain(  seq.menu - ENCODER[ENC_R].speed(), 0, 15 );
        }
        else  if ( ( seq.edit_state == true && seq.cycle_touch_element == 7) || (seq.edit_state == true && seq.cycle_touch_element == 9 ) )
        {
          if (seq.sub_menu == 0)
            seq.sub_menu = NUM_CHAINS;
          else
            seq.sub_menu = constrain(  seq.sub_menu - ENCODER[ENC_R].speed(), 0, NUM_CHAINS );// not -1:last element is for empty step
        }
      }
      else  if (LCDML.BT_checkEnter())  //handle button presses during menu >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
      {
        if (seq.cycle_touch_element == 5 && seq.edit_state)
        {
          seq.edit_state = !seq.edit_state;
          seq.cycle_touch_element = 0;
        }
        else if (seq.cycle_touch_element == 0)
        {
          seq.edit_state = !seq.edit_state;
          seq.cycle_touch_element = 5;
          temp_int = seq.song[seq.selected_track][seq.cursor_scroll + seq.scrollpos];
        }
        else if (seq.edit_state && seq.cycle_touch_element == 6)
        {
          seq.cycle_touch_element = 7;  //edit chain
          seq.sub_menu = seq.chain[  seq.song[seq.selected_track][seq.cursor_scroll + seq.scrollpos] ][seq.menu];
        }
        else if (seq.edit_state && seq.cycle_touch_element == 7)
        {
          seq.cycle_touch_element = 6;  //go back from chain
        }


        else if (seq.edit_state && seq.cycle_touch_element == 8)
        {
          seq.cycle_touch_element = 9;  //edit transpose
          seq.sub_menu = seq.chain_transpose[  seq.song[seq.selected_track][seq.cursor_scroll + seq.scrollpos] ][seq.menu];
        }
        else if (seq.edit_state && seq.cycle_touch_element == 9)
        {
          seq.cycle_touch_element = 8;  //go back from transpose
        }


        else if (seq.edit_state == false )
        {
          helptext_l("MOVE X");
          helptext_r("MOVE Y");
          seq.cycle_touch_element = 0;
        }
      }
    }
    if (seq.edit_state && seq.cycle_touch_element == 5)
    {
      helptext_l("SONG < > CHAIN");
      helptext_r("< > SELECT CHAIN");
    }
    else if (seq.edit_state && seq.cycle_touch_element == 6)
    {
      helptext_l("SONG < > TRANSPOSE");
      display.setCursor (CHAR_width * 18, 320 - CHAR_height);
      display.setTextColor(COLOR_SYSTEXT, COLOR_BACKGROUND);
      display.print("     ");
      helptext_r("< > SEL. CH. STEP");
    }
    else if (seq.edit_state && seq.cycle_touch_element == 7)
    {
      helptext_r ("< > SEL. PATTERN");
    }
    else if (seq.edit_state && seq.cycle_touch_element == 9)
    {
      helptext_l("");
      helptext_r ("< > EDIT STEP");
    }
    else if (seq.edit_state && seq.cycle_touch_element == 8)
    {
      helptext_l("< CHAIN ");
      helptext_r ("< > SEL. STEP");
    }


    for (uint8_t x = 0; x < NUM_SEQ_TRACKS; x++)
    {
      for (uint8_t y = 0; y < 12; y++) //song steps
      {
        display.setTextColor(GREEN, COLOR_BACKGROUND);  // green step number
        display.setCursor(CHAR_width, y * (CHAR_height + 1) + 4 * CHAR_height + 12 );
        seq_print_formatted_number( y + 1 + seq.scrollpos , 2);
        if ( seq.tracker_data_cache[x][y] != seq.song[x][y + seq.scrollpos] || x == seq.selected_track || y == seq.cursor_scroll)
        {
          display.setCursor(5 * CHAR_width + (4 * CHAR_width)*x , y * (CHAR_height + 1) + 4 * CHAR_height + 12 );
          if (y == seq.cursor_scroll && x == seq.selected_track  && seq.edit_state == false)
            set_track_type_color_inverted(x);
          else if (y == seq.cursor_scroll && x == seq.selected_track  && seq.edit_state && seq.cycle_touch_element == 5)
            display.setTextColor(COLOR_SYSTEXT, RED);
          else
            set_track_type_color(x);
          if (seq.edit_state && seq.cycle_touch_element == 5 && x == seq.selected_track && y == seq.cursor_scroll) // is in song edit mode
          {
            if (temp_int == NUM_CHAINS)
            {
              temp_int = 99; //Select empty step/chain
              seq.song[x][y + seq.scrollpos] = temp_int;
              temp_int = NUM_CHAINS ;
            }
            else
              seq.song[x][y + seq.scrollpos] = temp_int;
          }
          {
            if (seq.song[x][y + seq.scrollpos]  < 99)
              seq_print_formatted_number( seq.song[x][y + seq.scrollpos]  , 2);
            else
              display.print(F("--"));
            seq.tracker_data_cache[x][y] = seq.song[x][y + seq.scrollpos];
          }
        }
      }
    }

    uint8_t endline = 99;
    display.setTextSize(1);
    if ( seq.cycle_touch_element < 10)
    {
      for (uint8_t y = 0; y < 16; y++)  // chain
      {

        if (seq.cycle_touch_element != 7)
        {
          display.setCursor(CHAR_width * 33, CHAR_height * 5 + y * 13);

          if (endline == 99 && seq.chain[  seq.song[seq.selected_track][seq.cursor_scroll + seq.scrollpos]   ][y] == 99 )
          {
            display.setTextColor(RED, COLOR_BACKGROUND);
            display.print(F("END"));
            endline = y;
          }
        }
        if (seq.edit_state && seq.cycle_touch_element == 6)
          display.setTextColor(COLOR_SYSTEXT, COLOR_BACKGROUND);
        else
          display.setTextColor(GREY1, COLOR_BACKGROUND);
        display.setCursor(CHAR_width * 33, CHAR_height * 5 + y * 13);
        if ( seq.song[seq.selected_track][seq.cursor_scroll + seq.scrollpos] < 99 &&
             seq.chain[  seq.song[seq.selected_track][seq.cursor_scroll + seq.scrollpos]    ][y]  < 99)
        {
          seq_print_formatted_number( seq.chain[  seq.song[seq.selected_track][seq.cursor_scroll + seq.scrollpos]   ][y]   , 2);
          display.print(F(" "));
        }
        else if (endline != y)
          display.print(F("-- "));
        display.setCursor(CHAR_width * 38, CHAR_height * 5 + y * 13);  // chain transpose

        if (seq.edit_state && seq.cycle_touch_element == 8)
          display.setTextColor(COLOR_SYSTEXT, COLOR_BACKGROUND);
        else
          display.setTextColor(GREY1, COLOR_BACKGROUND);
        if ( seq.song[seq.selected_track][seq.cursor_scroll + seq.scrollpos] != 99 &&
             seq.chain_transpose[  seq.song[seq.selected_track][seq.cursor_scroll + seq.scrollpos ]     ][y] < 99 )
        {
          seq_print_formatted_number( seq.chain_transpose[ seq.song[seq.selected_track][seq.cursor_scroll + seq.scrollpos]     ][y]   , 2);
          display.setCursor(CHAR_width * 37, CHAR_height * 5 + y * 13);
          if ( seq.chain_transpose[  seq.song[seq.selected_track][seq.cursor_scroll + seq.scrollpos ]     ][y] != 0)
            display.print(F("+"));
          else if ( seq.chain_transpose[  seq.song[seq.selected_track][seq.cursor_scroll + seq.scrollpos ]     ][y] == 0)
            display.print(F(" "));
        }
        else
        {
          display.print(F("--"));
          display.setCursor(CHAR_width * 37, CHAR_height * 5 + y * 13);
          display.print(F(" "));
        }
      }
    }

    if (seq.edit_state && seq.cycle_touch_element == 6)
    {
      display.setTextColor(COLOR_SYSTEXT, RED);
      display.setCursor(CHAR_width * 33, CHAR_height * 5 + seq.menu * 13);
      if ( seq.chain[  seq.song[seq.selected_track][seq.cursor_scroll + seq.scrollpos]   ][seq.menu]   == 99 && endline == seq.menu)
        display.print(F("END"));
      else if ( seq.chain[  seq.song[seq.selected_track][seq.cursor_scroll + seq.scrollpos]   ][seq.menu]   == 99 )
        display.print(F("--"));
      else
        seq_print_formatted_number( seq.chain[  seq.song[seq.selected_track][seq.cursor_scroll + seq.scrollpos]   ][seq.menu]   , 2);
    }
    else if (seq.edit_state && seq.cycle_touch_element == 7)
    {
      if (seq.sub_menu == NUM_CHAINS)
      {
        seq.sub_menu = 99; //Select empty step/chain
        seq.chain[  seq.song[seq.selected_track][seq.cursor_scroll + seq.scrollpos]   ][seq.menu] = seq.sub_menu;

        seq.sub_menu = NUM_CHAINS ;
      }
      else
        seq.chain[  seq.song[seq.selected_track][seq.cursor_scroll + seq.scrollpos]   ][seq.menu]  = seq.sub_menu;

      display.setTextColor(COLOR_SYSTEXT, COLOR_BACKGROUND);
      display.setCursor(CHAR_width * 33, CHAR_height * 5 + seq.menu * 13);

      if ( seq.chain[  seq.song[seq.selected_track][seq.cursor_scroll + seq.scrollpos]   ][seq.menu]   == 99 )
        display.print(F("-- "));
      else
      {
        seq_print_formatted_number( seq.chain[  seq.song[seq.selected_track][seq.cursor_scroll + seq.scrollpos]   ][seq.menu]   , 2);
        display.print(F(" "));
      }
      //pattern preview
      pattern_preview_in_song(seq.chain[  seq.song[seq.selected_track][seq.cursor_scroll + seq.scrollpos]   ][seq.menu] );

    }

    //transpose

    if (seq.edit_state && seq.cycle_touch_element == 8)
    {
      display.setTextColor(COLOR_SYSTEXT, RED);
      display.setCursor(CHAR_width * 38, CHAR_height * 5 + seq.menu * 13);
      if ( seq.chain_transpose[  seq.song[seq.selected_track][seq.cursor_scroll + seq.scrollpos]   ][seq.menu]   == 99 )
        display.print(F("--"));
      else
        seq_print_formatted_number( seq.chain_transpose[  seq.song[seq.selected_track][seq.cursor_scroll + seq.scrollpos]   ][seq.menu]   , 2);
    }
    else if (seq.edit_state && seq.cycle_touch_element == 9)
    {
      if (seq.sub_menu == NUM_CHAINS)
      {
        seq.sub_menu = 99; //Select empty
        seq.chain_transpose[  seq.song[seq.selected_track][seq.cursor_scroll + seq.scrollpos]   ][seq.menu] = seq.sub_menu;

        seq.sub_menu = NUM_CHAINS ;
      }
      else
        seq.chain_transpose[  seq.song[seq.selected_track][seq.cursor_scroll + seq.scrollpos]   ][seq.menu]  = seq.sub_menu;

      display.setTextColor(COLOR_SYSTEXT, RED);
      display.setCursor(CHAR_width * 38, CHAR_height * 5 + seq.menu * 13);
      if ( seq.chain_transpose[  seq.song[seq.selected_track][seq.cursor_scroll + seq.scrollpos]   ][seq.menu]   == 99 )
      {
        display.print(F("--"));
        display.setTextColor(COLOR_SYSTEXT, COLOR_BACKGROUND);
        display.print(F(" "));
      }
      else
        seq_print_formatted_number( seq.chain_transpose[  seq.song[seq.selected_track][seq.cursor_scroll + seq.scrollpos]   ][seq.menu]   , 2);
    }

    //print CHAIN indicator
    display.setTextSize(2);
    display.setCursor_textGrid(37, 3);
    if (seq.edit_state && seq.cycle_touch_element > 5 && seq.cycle_touch_element < 9)
      display.setTextColor(COLOR_SYSTEXT, COLOR_BACKGROUND);
    else
      display.setTextColor(GREY1, COLOR_BACKGROUND);

    if ( seq.song[seq.selected_track][seq.cursor_scroll + seq.scrollpos] < 99 )
      seq_print_formatted_number(seq.song[seq.selected_track][seq.cursor_scroll + seq.scrollpos] , 2);
    else
      display.print("--");

    //print song length

    display.setCursor_textGrid(13, 1);
    display.setTextColor(COLOR_SYSTEXT, COLOR_PITCHSMP);
    display.print ( get_song_length() );
  }

  if (LCDML.FUNC_close())     // ****** STABLE END *********
  {
    encoderDir[ENC_R].reset();
    seq.cycle_touch_element = 0;
    display.setTextColor(COLOR_SYSTEXT, COLOR_BACKGROUND);
    display.fillScreen(COLOR_BACKGROUND);
  }
}


void UI_func_seq_tracker_edit(uint8_t param)
{
  if (LCDML.FUNC_setup())         // ****** SETUP *********
  {
    // setup function
    reset_tracker_edit_cache();
    encoderDir[ENC_R].reset();
    display.fillScreen(COLOR_BACKGROUND);
    UI_toplineInfoText(1);
    display.setTextSize(2);
    display.setCursor_textGrid(1, 1);
    display.setTextColor(COLOR_SYSTEXT);
    display.print (F("TRACKER"));
    display.setTextSize(1);
    for (uint8_t x = 0; x < NUM_SEQ_TRACKS; x++)
    {
      display.setCursor_textGrid(4 + 6 * x, 3);
      display.setCursor(display.getCursorX(), display.getCursorY() + 15);
      display.setTextColor(COLOR_SYSTEXT, COLOR_BACKGROUND);
      display.print (F("TRACK: "));
      display.print (x + 1);
      display.setCursor_textGrid(4 + 6 * x, 4);
      display.setCursor(display.getCursorX(), display.getCursorY() + 9);
      display.print (F("PAT:"));

    }
    display.setTextSize(2);

  }
  if (LCDML.FUNC_loop())          // ****** LOOP *********
  {
    if ((LCDML.BT_checkDown() && encoderDir[ENC_R].Down()) || (LCDML.BT_checkUp() && encoderDir[ENC_R].Up()) || (LCDML.BT_checkEnter() && encoderDir[ENC_R].ButtonShort()))
    {
      if (LCDML.BT_checkDown())
      {
        if (seq.scrollpos == 52)
        {
          seq.cursor_scroll++;
          if (seq.cursor_scroll > 5)
            seq.cursor_scroll = 5;
          reset_tracker_edit_cache_single_track();
        }
        seq.scrollpos = constrain(seq.scrollpos + ENCODER[ENC_R].speed(), 0, 52);
      }
      else if (LCDML.BT_checkUp())
      {
        if (seq.scrollpos == 0)
        {
          seq.cursor_scroll--;
          if (seq.cursor_scroll < -6)
            seq.cursor_scroll = -6;
          reset_tracker_edit_cache_single_track();
        }
        seq.scrollpos = constrain(seq.scrollpos - ENCODER[ENC_R].speed(), 0, 52);
      }
      else  if (LCDML.BT_checkEnter())  //handle button presses during menu >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
      {
        ;
      }
    }

    print_merged_pattern_for_editor( 4 * CHAR_width , 95,  0);
    print_merged_pattern_for_editor( 10 * CHAR_width , 95,  1);
    print_merged_pattern_for_editor( 16 * CHAR_width , 95,  2);
    print_merged_pattern_for_editor( 22 * CHAR_width , 95,  3);
    print_merged_pattern_for_editor( 28 * CHAR_width , 95,  4);
    print_merged_pattern_for_editor( 34 * CHAR_width , 95,  5);
  }
  if (LCDML.FUNC_close())     // ****** STABLE END *********
  {
    encoderDir[ENC_R].reset();
    display.fillScreen(COLOR_BACKGROUND);
  }
}

void UI_func_seq_pianoroll(uint8_t param)
{
  if (LCDML.FUNC_setup())         // ****** SETUP *********
  {
    // setup function
    display.fillScreen(COLOR_BACKGROUND);
    encoderDir[ENC_R].reset();

    display.setTextColor(COLOR_SYSTEXT, COLOR_CHORDS);
    display.setTextSize(1);
    display.setCursor (0, 0);
    display.print("SONG");
    display.setCursor (0, CHAR_height / 2);
    display.print("STEP");
    display.fillRect(1, 72, 480, 320 - 72, COLOR_BACKGROUND);
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
    display.fillScreen(COLOR_BACKGROUND);
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
    display.print(F("Len"));
    display.setCursor_textGrid(8, 1);
    print_current_chord();
    display.setCursor_textGrid( 1, 2);
    display.print(F("Style"));
    display.setCursor_textGrid( 14, 2);
    display.print(F("1/16"));
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
          seq.arp_speed = constrain(seq.arp_speed + ENCODER[ENC_R].speed(), 0, 3);
        else if (LCDML.BT_checkUp())
          seq.arp_speed = constrain(seq.arp_speed - ENCODER[ENC_R].speed(), 0, 3);
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

            // SEQUENCER REWRITE
            //seq.chain_active_step = 0;

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
    if (seq.arp_speed == 0)display.print("1/16");
    else if (seq.arp_speed == 1)display.print("1/8 ");
    else if (seq.arp_speed == 2)display.print("1/32");
    else if (seq.arp_speed == 3)display.print("1/64 ");

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
    display.setTextColor(COLOR_SYSTEXT, COLOR_BACKGROUND);
  }
}

void UI_func_seq_mute_matrix(uint8_t param)
{
  if (LCDML.FUNC_setup())         // ****** SETUP *********
  {
    // setup function
    display.fillScreen(COLOR_BACKGROUND);
    UI_toplineInfoText(0);
    display.setTextSize(2);
    display.setCursor_textGrid(1, 0);
    display.setTextColor(COLOR_SYSTEXT);
    display.print ("MUTE MATRIX");
    display.drawRect( 11, 2 * CHAR_height - 1, 455, 64, COLOR_SYSTEXT  );
  }
  if (LCDML.FUNC_loop())          // ****** LOOP *********
  {
    for (uint8_t y = 0; y < 4; y++)
    {
      for (uint8_t x = 0; x < NUM_SEQ_TRACKS; x++)
      {
        display.fillRect( CHAR_width + x * (480 / 6 - 3)  , 2 * CHAR_height + y * (320 / 4 - 7),  68, 62, GREY2);
      }
    }
  }
  if (LCDML.FUNC_close())     // ****** STABLE END *********
  {
    encoderDir[ENC_R].reset();
    display.fillScreen(COLOR_BACKGROUND);
    display.setTextColor(COLOR_SYSTEXT, COLOR_BACKGROUND);
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
    display.print(F("Dexed Assign"));
    display.setCursor_textGrid(1 , 2);
    display.print(F("Track           "));
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
          seq.inst_dexed[seq.temp_active_menu] = constrain(seq.inst_dexed[seq.temp_active_menu] + 1, 0, 4);
        else if (LCDML.BT_checkUp())
          seq.inst_dexed[seq.temp_active_menu] = constrain(seq.inst_dexed[seq.temp_active_menu] - 1, 0, 4);
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
    if (seq.inst_dexed[seq.temp_select_menu] == 0 ) display.print("dexed0");
    else if (seq.inst_dexed[seq.temp_select_menu] == 1 ) display.print("dexed1");
    else if (seq.inst_dexed[seq.temp_select_menu] == 2 ) display.print("ePiano");
    else if (seq.inst_dexed[seq.temp_select_menu] == 3 ) display.print("SIDsaw");
    else if (seq.inst_dexed[seq.temp_select_menu] == 4 ) display.print("SIDsqr");
    else display.print("??????");

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
    border1_clear();
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
          display.print(F("Does not exist."));
        else
        {
          load_sd_performance_json(temp_int);
          seq.state_last_loadsave = temp_int;
          display.setCursor_textGrid(1, 2);
          display.print(F("Done.           "));
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
        display.print(F(" -- DATA --"));
    }
    else display.print(F("           "));
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
    border1_clear();
    encoderDir[ENC_R].reset();
    display.setCursor_textGrid(1, 1);
    display.print(F("Save Performance"));
    display.setCursor_textGrid(1, 2);
    sprintf(tmp, "[%2d] ", temp_int);
    display.print(tmp);

    if (check_sd_performance_exists(temp_int))
    {
      overwrite = true;
      get_sd_performance_name_json(temp_int);
      if ( seq.name_temp[0] != 0 )
        display.show(2, 6, 11, seq.name_temp);
      else
        display.print(F(" -- DATA --"));
    }
    else
      overwrite = false;
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
      else if (mode == 1)
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
    border1_clear();
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
  currentDirectory.rewindDirectory();
  fm.sd_is_folder = false;
  fm.sd_cap_rows = 9;
  for (uint8_t f = 0; f < fm.sd_skip_files; f++)
  {
    fm.sd_entry.close();
    fm.sd_entry = currentDirectory.openNextFile();

    if (! fm.sd_entry)  break;
  }
  if (fm.sd_parent_folder  && fm.sd_folder_depth > 0)
  {
    fm.sd_is_folder = true;
    strcpy(fm.sd_temp_name, ".." );
    display.setTextColor(COLOR_SYSTEXT, COLOR_PITCHSMP);
  }
  else
    display.setTextColor(COLOR_PITCHSMP, COLOR_BACKGROUND);

  if ( fm.sd_folder_depth > 0)
  {
    display.drawBitmap(CHAR_width,  10 * 11 - 1, special_chars[23], 8, 8, YELLOW);
    display.setCursor (CHAR_width * 2,   + 10 * 11  );
    display.print("..");
  }
  else
  {
    display.drawBitmap(CHAR_width,  10 * 11 - 1, special_chars[23], 8, 8, COLOR_BACKGROUND);
    display.setCursor (CHAR_width * 2,   + 10 * 11  );
    display.print("/ ");
  }
  for (uint8_t f = 0; f < 10; f++)
  {
    fm.sd_entry =  currentDirectory.openNextFile();
    if (! fm.sd_entry)
    {
      fm.sd_cap_rows = f - 1;
      display.fillRect(10, f * 11   + 11 * 11 - 1 , CHAR_width * 15, (10 - f) * 11, COLOR_BACKGROUND);
      break;
    }
    if (fm.sd_entry.isDirectory() )
    {
      display.drawBitmap(CHAR_width, f * 11 - 1 + 11 * 11 , special_chars[23], 8, 8, YELLOW);
      if (f == fm.sd_selected_file && fm.sd_parent_folder == false && fm.active_window == 0)
        display.setTextColor(COLOR_BACKGROUND, COLOR_PITCHSMP);
      else
        display.setTextColor(COLOR_PITCHSMP, COLOR_BACKGROUND);
      display.show_smallfont_noGrid( f * 11  + 11 * 11, CHAR_width * 2 , 18, fm.sd_entry.name() );
      display.setCursor (CHAR_width * 12, f * 11  + 11 * 11  );
      display.setTextColor(DX_DARKCYAN, COLOR_BACKGROUND);
      display.print("DIR    ");
    } else
    {
      display.drawBitmap(CHAR_width, f * 11 - 1 + 11 * 11 , special_chars[23], 8, 8, COLOR_BACKGROUND);
      if (f == fm.sd_selected_file && fm.sd_parent_folder == false && fm.active_window == 0)
        display.setTextColor(COLOR_BACKGROUND, COLOR_SYSTEXT);
      else
        display.setTextColor(COLOR_SYSTEXT, COLOR_BACKGROUND);
      display.show_smallfont_noGrid(f * 11  + 11 * 11, CHAR_width * 2 , 18, fm.sd_entry.name() );
      display.setTextColor(COLOR_DRUMS, COLOR_BACKGROUND);
      display.setCursor (CHAR_width * 12, f * 11  + 11 * 11  );
      display.print(fm.sd_entry.size(), DEC);
      display.print("  ");
    }
    if (f == fm.sd_selected_file && fm.sd_parent_folder == false) strcpy(fm.sd_temp_name, fm.sd_entry.name() );
    if (f == fm.sd_selected_file &&  fm.sd_entry.isDirectory() )
      fm.sd_is_folder = true;
    fm.sd_entry.close();
  }
}

void flash_printDirectory()
{

  fm.flash_cap_rows = 9;
  uint8_t f = 0;
  char filename[26];
  uint32_t filesize;

  SerialFlash.opendir();

  if (fm.flash_skip_files > 0)
  {
    for (f = 0; f < fm.flash_skip_files; f++)
    {
      if (SerialFlash.readdir(filename, sizeof(filename), filesize))
        ;
      else
        break;
    }
  }
  f = 0;
  while (1)
  {
    if (f > 9)
    {
      fm.flash_cap_rows = f - 1;
      break;
    }

    if (SerialFlash.readdir(filename, sizeof(filename), filesize))
    {
      if (f == fm.flash_selected_file && fm.active_window == 1)
        display.setTextColor(COLOR_BACKGROUND, COLOR_PITCHSMP);
      else
        display.setTextColor(COLOR_PITCHSMP, COLOR_BACKGROUND);

      display.show_smallfont_noGrid( f * 11  + 11 * 11, CHAR_width * 2 + 240, 22, filename );

      display.setTextColor(COLOR_DRUMS, COLOR_BACKGROUND);
      display.setCursor (CHAR_width * 15 + 240, f * 11  + 11 * 11  );
      display.print(filesize, DEC);
      display.print("  ");
    }
    else
    {
      fm.flash_cap_rows = f - 1;
      //      display.fillRect(10 + 240, f * 11   + 11 * 11 - 1 , CHAR_width * 15, (10 - f) * 11, COLOR_BACKGROUND);
      break; // no more files

    }
    f++;
  }

  //    }
  //    if (f == fm.flash_selected_file && fm.flash_parent_folder == false) strcpy(fm.flash_temp_name, fm.flash_entry.name() );
  //    if (f == fm.flash_selected_file &&  fm.flash_entry.isDirectory() )
  //      fm.flash_is_folder = true;
  //    fm.flash_entry.close();
  //  }
}

bool compareFiles(File & file, SerialFlashFile & ffile) {
  file.seek(0);
  ffile.seek(0);
  unsigned long count = file.size();
  while (count > 0) {
    char buf1[128], buf2[128];
    unsigned long n = count;
    if (n > 128) n = 128;
    file.read(buf1, n);
    ffile.read(buf2, n);
    if (memcmp(buf1, buf2, n) != 0) return false; // differ
    count = count - n;
  }
  return true;  // all data identical
}


void print_flash_stats()
{
  unsigned char buf[256];
  unsigned long chipsize;
  uint32_t sum_used = 0;
  uint16_t sum_files = 0;
  SerialFlash.readID(buf);
  SerialFlash.opendir();
  while (1)
  {
    char filename[25];
    uint32_t filesize;
    if (SerialFlash.readdir(filename, sizeof(filename), filesize))
    {
      sum_used = sum_used + filesize / 1024;
      sum_files++;
    } else {
      break; // no more files
    }
  }
  display.setTextSize(1);
  display.setCursor (CHAR_width * 21 , 2 * CHAR_height + 5  );
  display.setTextColor(GREY2, COLOR_BACKGROUND);
  display.print("USED: ");
  display.setTextColor(COLOR_PITCHSMP, COLOR_BACKGROUND);
  display.print(sum_used );
  display.setTextColor(COLOR_CHORDS, COLOR_BACKGROUND);
  display.print(" KB");
  display.setCursor (CHAR_width * 31 , 1 * CHAR_height + 7  );
  display.setTextColor(GREY2, COLOR_BACKGROUND);
  display.print("TOTAL: ");
  display.setTextColor(COLOR_PITCHSMP, COLOR_BACKGROUND);
  chipsize = SerialFlash.capacity(buf);
  display.print(chipsize / 1024 );
  display.setTextColor(COLOR_CHORDS, COLOR_BACKGROUND);
  display.print(" KB");
  display.setCursor (CHAR_width * 31 , 0 * CHAR_height + 11  );
  display.setTextColor(GREY2, COLOR_BACKGROUND);
  display.print("FILES: ");
  display.setTextColor(COLOR_PITCHSMP, COLOR_BACKGROUND);
  display.print(sum_files );
  display.setCursor (CHAR_width * 31 + 6 , 2 * CHAR_height + 5  );
  display.setTextColor(GREY2, COLOR_BACKGROUND);
  display.print("FREE: ");
  display.setTextColor(COLOR_PITCHSMP, COLOR_BACKGROUND);
  display.print(chipsize / 1024 - sum_used );
  display.setTextColor(COLOR_CHORDS, COLOR_BACKGROUND);
  display.print(" KB");
}

void UI_func_phSampler(uint8_t param)
{
  if (LCDML.FUNC_setup())         // ****** SETUP *********
  {
    display.fillScreen(COLOR_BACKGROUND);
    encoderDir[ENC_R].reset();
    display.setTextSize(2);
    border1();
    border2();
    border3();
    display.setTextColor(COLOR_SYSTEXT);
    border4();
    display.setCursor_textGrid(1, 1);
    display.print(F("phSAMPLER"));
    display.setCursor_textGrid(1, 2);
    display.print(F("["));
    display.setCursor_textGrid(13, 2);
    display.print(F("]"));
    display.setTextSize(2);

    display.setCursor_textGrid(21, 1);
    display.print(F("SPI Flash"));
    print_flash_stats();

  }
  if (LCDML.FUNC_loop())          // ****** LOOP *********
  {
    if ((LCDML.BT_checkDown() && encoderDir[ENC_R].Down()) || (LCDML.BT_checkUp() && encoderDir[ENC_R].Up()))
    {
      if (LCDML.BT_checkDown())
      {
        temp_int = constrain(temp_int + 1, 0, 1);

      }
      else if (LCDML.BT_checkUp())
      {
        temp_int = constrain(temp_int - 1, 0, 1);
      }
    }
    if (LCDML.BT_checkEnter())
    {

    }
    display.setTextSize(2);
    display.setCursor_textGrid(2, 2);
    display.setTextColor(COLOR_PITCHSMP, COLOR_BACKGROUND);
    if (temp_int == 0)
      display.print(F("MultiPiano "));
    else if (temp_int == 1)
      display.print(F("StringPad  "));


  }
  if (LCDML.FUNC_close())     // ****** STABLE END *********
  {
    encoderDir[ENC_R].reset();
    display.setTextColor(COLOR_SYSTEXT, COLOR_BACKGROUND);
  }
}


void UI_func_file_manager(uint8_t param)
{
  uint32_t volumesize;

  if (LCDML.FUNC_setup())         // ****** SETUP *********
  {
    display.fillScreen(COLOR_BACKGROUND);
    encoderDir[ENC_R].reset();
    display.setTextSize(2);
    border1();
    border2();
    border3();
    border4();
    display.setCursor_textGrid(1, 1);
    display.print(F("SD CARD"));
    display.setCursor (CHAR_width * 11 , 1 * CHAR_height   );
    display.setTextSize(1);
    // print the type of card
    display.setTextColor(GREY2);
    display.print(F("CARD TYPE: "));
    display.setTextColor(COLOR_PITCHSMP);
    switch (card.type())
    {
      case SD_CARD_TYPE_SD1:
        display.println("SD1");
        break;
      case SD_CARD_TYPE_SD2:
        display.println("SD2");
        break;
      case SD_CARD_TYPE_SDHC:
        display.println("SDHC");
        break;
      default:
        display.println(F("Unknown"));
    }
    display.setCursor (CHAR_width * 11 , 1 * CHAR_height + 8  );
    volumesize = volume.blocksPerCluster();    // clusters are collections of blocks
    volumesize *= volume.clusterCount();       // we'll have a lot of clusters
    display.setTextColor(GREY2);
    display.print("TOTAL: ");
    display.setTextColor(COLOR_PITCHSMP);
    volumesize /= 1024;
    display.print(volumesize);
    display.setTextColor(COLOR_CHORDS);
    display.print(" MB");
    display.setTextSize(2);
    display.setTextColor(COLOR_SYSTEXT);
    display.setCursor_textGrid(21, 1);
    display.print(F("SPI Flash"));
    print_flash_stats();
    display.setTextSize(1);
    display.setCursor (CHAR_width , 2 * CHAR_height + 5  );
    display.setTextColor(COLOR_CHORDS, COLOR_BACKGROUND);
    display.print(F("PATH:"));
    display.setCursor (CHAR_width, 3 * CHAR_height + 3 );
    display.setTextColor(GREY2);
    display.print(F("CURRENT FILE/FOLDER :"));
    //fm_mode

    display.fillRect (CHAR_width,       240, 100, 25, COLOR_PITCHSMP);
    display.fillRect (CHAR_width + 114 , 240, 100, 25, COLOR_PITCHSMP);
    display.fillRect (CHAR_width,       280, 100, 25, COLOR_PITCHSMP);
    display.fillRect (CHAR_width + 114 , 280, 100, 25, COLOR_PITCHSMP);

    print_file_manager_buttons();
    flash_printDirectory();
  }
  if (LCDML.FUNC_loop())          // ****** LOOP *********
  {
    if ((LCDML.BT_checkDown() && encoderDir[ENC_R].Down()) || (LCDML.BT_checkUp() && encoderDir[ENC_R].Up()))
    {
      if (LCDML.BT_checkDown() && fm.active_window == 0) // left window, SDCARD
      {
        if (fm.sd_selected_file == fm.sd_cap_rows && fm.sd_cap_rows > 8 )
          fm.sd_skip_files++;

        if (fm.sd_selected_file == 0 && fm.sd_parent_folder == true)
        {
          fm.sd_is_folder = true;
          fm.sd_selected_file = 0;
          fm.sd_parent_folder = false;
        } else
          fm.sd_selected_file = constrain(fm.sd_selected_file + 1, 0, fm.sd_cap_rows);
      }
      else if (LCDML.BT_checkUp() && fm.active_window == 0) // left window, SDCARD
      {
        if (fm.sd_selected_file == 0 && fm.sd_skip_files > 0)
          fm.sd_skip_files--;
        else if (fm.sd_selected_file == 0 && fm.sd_skip_files == 0)
        {
          if (fm.sd_folder_depth > 0 )
            fm.sd_parent_folder = true;
        }
        fm.sd_selected_file = constrain(fm.sd_selected_file - 1, 0, fm.sd_cap_rows);
      }

      if (LCDML.BT_checkDown() && fm.active_window == 1) // right window, FLASH
      {
        if (fm.flash_selected_file == fm.flash_cap_rows && fm.flash_cap_rows > 8 )
          fm.flash_skip_files++;
        else
          fm.flash_selected_file = constrain(fm.flash_selected_file + 1, 0, fm.flash_cap_rows);
      }
      else if (LCDML.BT_checkUp() && fm.active_window == 1) // // right window, FLASH
      {
        if (fm.flash_selected_file == 0 && fm.flash_skip_files > 0)
          fm.flash_skip_files--;

        fm.flash_selected_file = constrain(fm.flash_selected_file - 1, 0, fm.flash_cap_rows);
      }
    }

    if (LCDML.BT_checkEnter() && fm.active_window == 0) // left window, SDCARD
    {
      if (fm.sd_mode == 4) //copy presets dir from SD to flash
      {
        border3_large_clear();
        encoderDir[ENC_R].reset();
        uint8_t screenline = 0;

        File rootdir = SD.open("/DRUMS");
        while (1)
        {
          // open a file from the SD card

          File f = rootdir.openNextFile();
          if (!f) break;
          const char *filename = f.name();

          if (screenline > 10)
            screenline = 0;
          display.setCursor_textGrid(1, 6 + screenline);

          display.setTextColor(COLOR_SYSTEXT, COLOR_BACKGROUND);
          //if (filename[0] != 46 && filename[1] != 95)
          if (filename[0] != 46)
          {
            display.print(filename);
            display.print("       ");
            screenline++;
          }
          unsigned long length = f.size();
          //Serial.println(length);

          // check if this file is already on the Flash chip
          if (SerialFlash.exists(filename))
          {
#ifdef DEBUG
            Serial.println(F("  already exists on the Flash chip"));
#endif
            SerialFlashFile ff = SerialFlash.open(filename);
            if (ff && ff.size() == f.size())
            {
#ifdef DEBUG
              Serial.println(F("  size is the same, comparing data..."));
#endif
              if (compareFiles(f, ff) == true)
              {
#ifdef DEBUG
                Serial.println(F("  files are identical :)"));
#endif
                f.close();
                ff.close();
                continue;  // advance to next file
              } else
              {
#ifdef DEBUG
                Serial.println(F("  files are different"));
#endif
              }
            } else
            {
#ifdef DEBUG
              Serial.print(F("  size is different, "));
              Serial.print(ff.size());
              Serial.println(F(" bytes"));
#endif
            }
            // delete the copy on the Flash chip, if different
#ifdef DEBUG
            Serial.println(F("  delete file from Flash chip"));
#endif
            SerialFlash.remove(filename);
          }
          //if (filename[0] != 46 && filename[1] != 95)
          if (filename[0] != 46)

          {
            // create the file on the Flash chip and copy data
            if (SerialFlash.create(filename, length))
            {
              SerialFlashFile ff = SerialFlash.open(filename);
              if (ff)
              {
#ifdef DEBUG
                Serial.print(F("  copying"));
#endif
                // copy data loop
                unsigned long count = 0;
                while (count < length)
                {
                  char buf[256];
                  unsigned int n;
                  n = f.read(buf, 256);
                  ff.write(buf, n);
                  count = count + n;
                  if (count % 5120 == 0)
                    display.fillRect(241, 80, count / (f.size() / 240) - 2, 8, RED);
                }
                ff.close();
                display.fillRect(241, 80, 238, 8, COLOR_BACKGROUND);
                print_flash_stats();
                flash_printDirectory();
              } else
              {
#ifdef DEBUG
                Serial.println(F("  error opening freshly created file!"));
#endif
              }
            } else
            {
#ifdef DEBUG
              Serial.println(F("  unable to create file"));
#endif
            }
          }
          f.close();
          display.fillRect(241, 80, 238, 8, COLOR_BACKGROUND);
        }
        rootdir.close();
        border3_large_clear();
        print_flash_stats();
        flash_printDirectory();
#ifdef DEBUG
        Serial.println(F("Finished All Files"));
#endif
      }
      else if (fm.sd_is_folder)
      {
        if  (fm.sd_temp_name[0] == 0x2E && fm.sd_temp_name[1] == 0x2E ) // return to parent folder
        {
          if  (fm.sd_folder_depth < 2)
          {
            fm.sd_folder_depth = 0;
            fm.sd_skip_files = 0;
            strcpy(fm.sd_new_name, "/");
          }
          else
          {
            // path has at least one parent folder
            for (uint8_t count = strlen(fm.sd_new_name); count > 0; count--)
            {
              if (fm.sd_new_name[count] == 0x2f)
              {
                fm.sd_new_name[count] = '\0';
                break;
              }
            }
            fm.sd_folder_depth--;
          }
        }
        else
        {
          fm.sd_skip_files = 0;
          if (fm.sd_folder_depth > 0)
            strcat(fm.sd_new_name, "/");
          strcat(fm.sd_new_name, fm.sd_temp_name);
          fm.sd_folder_depth++;
        }
        fm.sd_selected_file = 0;
      }
      else
        //is a file
      {
        if (fm.sd_mode == 1) //delete file
        {
          strcpy(fm.sd_full_name, fm.sd_new_name);
          strcat(fm.sd_full_name, "/");
          strcat(fm.sd_full_name, fm.sd_temp_name);
          SD.remove(fm.sd_full_name);
        }
        else if (fm.sd_mode == 2) //preview
        {
          strcpy(fm.sd_full_name, fm.sd_new_name);
          strcat(fm.sd_full_name, "/");
          strcat(fm.sd_full_name, fm.sd_temp_name);
          playWAVFile(fm.sd_full_name);
        }
        else if (fm.sd_mode == 3) //copy to flash
        {
          strcpy(fm.sd_full_name, fm.sd_new_name);
          strcat(fm.sd_full_name, "/");
          strcat(fm.sd_full_name, fm.sd_temp_name);
          File f = SD.open(fm.sd_full_name);
          const char *filename = f.name();
          unsigned long length = f.size();
          // check if this file is already on the Flash chip
          if (SerialFlash.exists(filename))
          {
#ifdef DEBUG
            Serial.println(F("  already exists on the Flash chip"));
#endif
            SerialFlashFile ff = SerialFlash.open(filename);
            if (ff && ff.size() == f.size())
            {
#ifdef DEBUG
              Serial.println(F("  size is the same, comparing data..."));
#endif
              if (compareFiles(f, ff) == true)
              {
#ifdef DEBUG
                Serial.println(F("  files are identical :)"));
#endif
                f.close();
                ff.close();

              } else
              {
#ifdef DEBUG
                Serial.println(F("  files are different"));
#endif
              }
            } else
            {
#ifdef DEBUG
              Serial.print(F("  size is different, "));
              Serial.print(ff.size());
              Serial.println(F(" bytes"));
#endif
            }
            // delete the copy on the Flash chip, if different
#ifdef DEBUG
            Serial.println(F("  delete file from Flash chip"));
#endif
            SerialFlash.remove(filename);
          }
          else
            // create the file on the Flash chip and copy data
            if (SerialFlash.create(filename, length)) {
              SerialFlashFile ff = SerialFlash.open(filename);
              if (ff)
              {
#ifdef DEBUG
                Serial.print(F("  copying"));
#endif
                // copy data loop
                unsigned long count = 0;
                while (count < length) {
                  char buf[256];
                  unsigned int n;
                  n = f.read(buf, 256);
                  ff.write(buf, n);
                  count = count + n;
                  if (count % 5120 == 0)
                    display.fillRect(241, 80, count / (f.size() / 240) - 2, 8, RED);
                }
                ff.close();
              } else
              {
#ifdef DEBUG
                Serial.println(F("  error opening freshly created file!"));
#endif
              }
              f.close();
              display.fillRect(241, 80, 238, 8, COLOR_BACKGROUND);
              print_flash_stats();
              flash_printDirectory();
            }
        }
      }
    }
    if (fm.active_window == 0)
    {
      if (fm.sd_new_name[0] != 0x2f)
        fm.sd_new_name[0] = 0x2f;
      fm.sd_currentDirectoy = SD.open(fm.sd_new_name);
      sd_printDirectory(fm.sd_currentDirectoy);
    }
    else if (fm.active_window == 1)
    {
      flash_printDirectory();
    }
    //display.setTextSize(1);
    display.setTextColor(COLOR_DRUMS, COLOR_BACKGROUND);
    display.show_smallfont_noGrid(2 * CHAR_height + 5 , CHAR_width * 4 , 21, fm.sd_new_name  );
    if (fm.sd_is_folder)
    {
      display.setCursor (CHAR_width + 13 * 6 , 3 * CHAR_height + 3 );
      display.setTextColor(GREEN);
      display.print(F("FOLDER"));
      display.setCursor (CHAR_width + 8 * 6, 3 * CHAR_height + 3 );
      display.setTextColor(GREY2);
      display.print(F("FILE"));
    }
    else
    {
      display.setCursor (CHAR_width + 8 * 6, 3 * CHAR_height + 3 );
      display.setTextColor(GREEN);
      display.print(F("FILE"));
      display.setCursor (CHAR_width + 13 * 6 , 3 * CHAR_height + 3 );
      display.setTextColor(GREY2);
      display.print(F("FOLDER"));
    }
    if (fm.sd_is_folder)
      display.setTextColor(COLOR_PITCHSMP, COLOR_BACKGROUND);
    else
      display.setTextColor(COLOR_SYSTEXT, COLOR_BACKGROUND);
    //  display.show_smallfont_noGrid(4 * CHAR_height - 4, CHAR_width, 20, fm.sd_temp_name );
  }
  if (LCDML.FUNC_close())     // ****** STABLE END *********
  {
    encoderDir[ENC_R].reset();
    display.setTextColor(COLOR_SYSTEXT, COLOR_BACKGROUND);
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

void UI_func_mixer(uint8_t param)
{
  if (LCDML.FUNC_setup())         // ****** SETUP *********
  {
    encoderDir[ENC_R].reset();
    display.fillScreen(COLOR_BACKGROUND);
    border1();
    border2();
    border3_large_with_buttons();

    display.setCursor_textGrid(1, 1);
    display.print(F("Master Mixer"));

    display.setTextSize(2);
    helptext_l("BACK");
    helptext_r ("< > SELECT CH");
    display.setTextSize(1);
    display.setTextColor(COLOR_SYSTEXT, COLOR_BACKGROUND);

    display.setCursor_textGrid(1, 15);
    display.print(F("DEXED"));
    display.setCursor_textGrid(4, 15);
    display.print(F("DEXED"));

    display.setCursor_textGrid(7, 15);
    display.print(F("DRUMS"));
    display.setCursor_textGrid(10, 15);
    display.print(F("DRUMS"));

    display.setCursor_textGrid(13, 15);
    display.print(F("REVB"));
    display.setCursor_textGrid(16, 15);
    display.print(F("REVB"));

    display.setCursor_textGrid(32, 15);
    display.print(F("MASTER"));
    display.setCursor_textGrid(36, 15);
    display.print(F("MASTER"));

    display.setCursor_textGrid(1, 16);
    display.print(F("#1"));
    display.setCursor_textGrid(4, 16);
    display.print(F("#2"));

    display.setCursor_textGrid(7, 16);
    display.print(F("L"));
    display.setCursor_textGrid(10, 16);
    display.print(F("R"));

    display.setCursor_textGrid(13, 16);
    display.print(F("L"));
    display.setCursor_textGrid(16, 16);
    display.print(F("R"));

    display.setCursor_textGrid(32, 16);
    display.print(F("L CH"));
    display.setCursor_textGrid(36, 16);
    display.print(F("R CH"));


  }
  if (LCDML.FUNC_loop())          // ****** LOOP *********
  {
    if ((LCDML.BT_checkDown() && encoderDir[ENC_R].Down()) || (LCDML.BT_checkUp() && encoderDir[ENC_R].Up()))
    {
      if (LCDML.BT_checkDown())
        ;
      else if (LCDML.BT_checkUp())
        ;
    }


  }
  if (LCDML.FUNC_close())     // ****** STABLE END *********
  {
    encoderDir[ENC_R].reset();
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

  display.setTextSize(1);
  if (selected_instance_id == 0)
  {
    display.fillRect(17 * CHAR_width + 11, 5, 9, 9, COLOR_SYSTEXT );
    display.setCursor(17 * CHAR_width + 13, 6);
    if (configuration.dexed[0].polyphony == 0)
      display.setTextColor(RED);
    else
      display.setTextColor(COLOR_BACKGROUND);
    display.print(F("1"));

    display.fillRect(18 * CHAR_width + 11, 5, 9, 9, COLOR_BACKGROUND );
    display.setCursor(18 * CHAR_width + 13, 6);
    if (configuration.dexed[1].polyphony == 0)
      display.setTextColor(RED);
    else
      display.setTextColor(COLOR_SYSTEXT);
    display.print(F("2"));
  }
  else
  {
    display.fillRect(17 * CHAR_width + 11, 5, 9, 9, COLOR_BACKGROUND );
    display.setCursor(17 * CHAR_width + 13, 6);
    if (configuration.dexed[0].polyphony == 0)
      display.setTextColor(RED);
    else
      display.setTextColor(COLOR_SYSTEXT);
    display.print(F("1"));

    display.fillRect(18 * CHAR_width + 11, 5, 9, 9, COLOR_SYSTEXT );
    display.setCursor(18 * CHAR_width + 13, 6);
    if (configuration.dexed[1].polyphony == 0)
      display.setTextColor(RED);
    else
      display.setTextColor(COLOR_BACKGROUND);
    display.print(F("2"));
  }
  display.setTextSize(2);
  display.setTextColor(COLOR_SYSTEXT, COLOR_BACKGROUND);
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
    display.setTextColor(COLOR_SYSTEXT, COLOR_BACKGROUND);
  else
    display.setTextColor(GREY2, COLOR_BACKGROUND);
  display.print("INSTANCE"); display.setTextColor(COLOR_PITCHSMP, COLOR_BACKGROUND);
  if (selected_instance_id == instance_id)
    display.setTextColor(GREEN, COLOR_BACKGROUND);
  else
    display.setTextColor(GREY2, COLOR_BACKGROUND);
  display.setCursor(x + 101, y);
  display.print(instance_id + 1);
  display.setTextSize(1);

  if (selected_instance_id == instance_id)
    display.setTextColor(COLOR_PITCHSMP, COLOR_BACKGROUND);
  else
    display.setTextColor(GREY2, COLOR_BACKGROUND);
  display.setCursor(x + 118, y - 1);
  seq_print_formatted_number(configuration.dexed[instance_id].bank, 2);
  display.setCursor(x + 118, y + 7);
  seq_print_formatted_number(configuration.dexed[instance_id].voice + 1, 2);
  if (selected_instance_id == instance_id)
    display.setTextColor(COLOR_SYSTEXT, COLOR_BACKGROUND);
  else
    display.setTextColor(GREY2, COLOR_BACKGROUND);
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
    display.setTextColor(GREY2, COLOR_BACKGROUND);
    yspacer = yspacer + 16;
    display.setCursor(x, y + yspacer);
    display.setTextColor(GREY2, COLOR_BACKGROUND);
    display.print(F("VOLUME "));
    display.setCursor(x + 118, y + yspacer);
    display.print(F("CHORUS LEVEL "));
    yspacer = yspacer + 9; display.setCursor(x, y + yspacer);
    display.setCursor(x, y + yspacer);
    display.print(F("TRANSPOSE "));
    display.setCursor(x + 118, y + yspacer);
    display.print(F("DELAY SEND "));
    yspacer = yspacer + 9; display.setCursor(x, y + yspacer);
    display.setCursor(x , y + yspacer);
    display.print(F("MIDI CHANNEL "));
    display.setCursor(x + 118, y + yspacer);
    display.print(F("REVERB SEND "));
    yspacer = yspacer + 9; display.setCursor(x, y + yspacer);
    display.setTextSize(1);
    display.print(F("PANORAMA "));
    display.setCursor(x + 118, y + yspacer);
    display.print(F("DELAY TIME "));
    yspacer = yspacer + 9; display.setCursor(x, y + yspacer);
    display.print(F("LOWEST NOTE "));
    display.setCursor(x + 118, y + yspacer);
    display.print(F("HIGHEST NOTE "));
    yspacer = yspacer + 9; display.setCursor(x, y + yspacer);
    display.print(F("ATTACK MOD "));
    display.setCursor(x + 118, y + yspacer);
    display.print(F("RELEASE MOD "));
  }
  display.setCursor(x + 144 + 4 * CHAR_width, y + 7 );
  display.setTextSize(1);
  if (selected_instance_id == instance_id)
    display.setTextColor(COLOR_PITCHSMP, COLOR_BACKGROUND);
  else
    display.setTextColor(GREY2, COLOR_BACKGROUND);
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
  display.setTextColor(COLOR_SYSTEXT, COLOR_BACKGROUND);
  display.setTextSize(2);
}



void UI_func_voice_select(uint8_t param)
{
  static uint8_t menu_voice_select = MENU_VOICE_SOUND;

  if (LCDML.FUNC_setup())         // ****** SETUP *********
  {
    display.fillScreen(COLOR_BACKGROUND);
    border1();
    border2();
    border3();
    border4();
    display.fillRect(366, 0 + 8, 51, 51 - 13, COLOR_PITCHSMP);
    display.fillRect(366, 50 + 5, 51, 49 - 13, COLOR_PITCHSMP);

    display.fillRect(423, 0 + 8, 50, 51 - 13, COLOR_PITCHSMP);
    display.fillRect(423, 50 + 5, 50, 49 - 13, COLOR_PITCHSMP);

    display.drawLine(360, 0, 360, 99, GREY4);
    display.setTextSize(1);
    display.setTextColor(COLOR_SYSTEXT, COLOR_PITCHSMP);
    display.setCursor(369, 12);
    display.print("OCT-");
    display.setCursor(426, 12);
    display.print("OCT+");

    display.setCursor(369, 59);
    display.print("SEQ");
    display.setCursor(426, 59);
    display.print("MIXER");


    print_voice_settings(12, 115, 0, true);
    print_voice_settings(252, 115, 1, true);
    virtual_keyboard(220);
    display.setTextColor(GREY2, COLOR_BACKGROUND);
    display.show_smallfont_noGrid( 4 * CHAR_height - 10, CHAR_width, 13, "PERFORMANCE #");
    display.setCursor(CHAR_width, 4 * CHAR_height  );
    display.setTextColor(COLOR_PITCHSMP, COLOR_BACKGROUND);
    display.setTextSize(1);
    seq_print_formatted_number(configuration.sys.performance_number, 2);
    display.setTextColor(COLOR_SYSTEXT, GREY3);
    display.show_smallfont_noGrid( 4 * CHAR_height, CHAR_width + 17, 11, seq.name);
    display.setTextColor(COLOR_SYSTEXT, COLOR_BACKGROUND);
    UI_draw_waveform(activesample);
    seq.last_drawn_sample = 254;
    encoderDir[ENC_R].reset();

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
    display.setTextColor(COLOR_PITCHSMP, COLOR_BACKGROUND);
    display.setCursor_textGrid(1, 1);
    seq_print_formatted_number(configuration.dexed[selected_instance_id].bank, 2);
    display.setCursor_textGrid(1, 2);
    seq_print_formatted_number(configuration.dexed[selected_instance_id].voice + 1, 2);
    display.setTextColor(COLOR_SYSTEXT, COLOR_BACKGROUND);
    string_toupper(bank_name);
    display.show(1, 5, 8, bank_name);
    string_toupper(voice_name);
    display.show(2, 5, 10, voice_name);

    display.setTextColor(GREY2, COLOR_BACKGROUND);
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
    display.setTextColor(COLOR_SYSTEXT, COLOR_BACKGROUND);
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
    display.setTextColor(COLOR_SYSTEXT, COLOR_BACKGROUND);
    encoderDir[ENC_L].reset();
    if (active_perform_page == 1)
    {
      //Master Volume
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
    display.setTextColor(COLOR_SYSTEXT, COLOR_BACKGROUND);
    display.setCursor_textGrid(15, 1);
    display.print("P");
    display.print(active_perform_page);
    display.print("/3");
    if (active_perform_page == 1)
    { //Master Volume
      display.setTextSize(2);
      display.setCursor_textGrid(1, 1);
      display.print(F("Master Volume"));
      display_bar_int("Master Vol.", configuration.sys.vol, 1.0, VOLUME_MIN, VOLUME_MAX, 3, false, false, false);
      set_volume(configuration.sys.vol, configuration.sys.mono);
    }
    else if (active_perform_page == 2)
    { //Attack
      display.setCursor_textGrid(1, 1);
      display.print(F("Live Modify  "));
      display.setCursor_textGrid(1, 2);
      display.print(F("Attack Mod.  ="));
      display.setCursor_textGrid(16, 2);
      sprintf(tmp, "%03d", perform_attack_mod[selected_instance_id]);
      display.print(tmp);
      back_from_volume = 0;
    }
    else if (active_perform_page == 3)
    { //Release
      display.setCursor_textGrid(1, 1);
      display.print(F("Live Modify  "));
      display.setCursor_textGrid(1, 2);
      display.print(F("Release Mod. ="));
      display.setCursor_textGrid(16, 2);
      sprintf(tmp, "%03d", perform_release_mod[selected_instance_id]);
      display.print(tmp);
      back_from_volume = 0;
    }
  }
  if (LCDML.FUNC_close())     // ****** STABLE END *********
  {
    //EEPROM.update(EEPROM_START_ADDRESS + offsetof(configuration_s, sys.vol), configuration.sys.vol);
    encoderDir[ENC_L].reset();

    if (old_volume != configuration.sys.vol)
    {
      // eeprom_update();
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
        display.setTextColor(COLOR_PITCHSMP, COLOR_BACKGROUND);
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
      display.setTextColor(COLOR_SYSTEXT, COLOR_BACKGROUND);
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
    display.print(F("enabled!"));
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
  display.setTextColor( COLOR_SYSTEXT, COLOR_BACKGROUND);
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
    display.setTextColor(COLOR_BACKGROUND, COLOR_PITCHSMP);
    display.print(edit_string[0]);
    display.setTextColor(GREY3, COLOR_BACKGROUND);
    display.setCursor_textGrid(x + len + 2, y);
    display.print(" OK ");
    display.setTextColor( COLOR_SYSTEXT, COLOR_BACKGROUND);
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
          display.setTextColor(COLOR_BACKGROUND, COLOR_PITCHSMP);
          display.setCursor_textGrid(x + len + 2, y);
          display.print("[OK]");
          display.setTextColor(COLOR_SYSTEXT, COLOR_BACKGROUND);
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
    display.setTextColor(COLOR_BACKGROUND, COLOR_PITCHSMP);
    display.print(edit_string[edit_pos]);
    display.setTextColor( COLOR_SYSTEXT, COLOR_BACKGROUND);
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
    display.setTextColor(GREY3, COLOR_BACKGROUND);
    display.setCursor_textGrid(x + len + 2, y);
    display.print(" OK ");
  }
  display.setTextColor( COLOR_SYSTEXT, COLOR_BACKGROUND);
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
  display.setTextColor(COLOR_SYSTEXT, COLOR_BACKGROUND);
  display.print(s);
  display.setTextColor(COLOR_SYSTEXT);
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
    display.drawBitmap(  CHAR_width , 2 * CHAR_height, block_bar[ (uint8_t)(vf / 1.25 - 0.5)  ], 8, 15, COLOR_SYSTEXT, COLOR_BACKGROUND);
    for (uint8_t i = vi + 1; i < display_cols - size + 5; i++)
      display.fillRect( CHAR_width + i * 8, 2 * CHAR_height, 8, 15, COLOR_BACKGROUND ); //empty block
  }
  else
  {
    for (uint8_t i = 0; i < vi; i++)
      display.drawBitmap(  CHAR_width + i * 8 , 2 * CHAR_height, block_bar[7], 8, 15, COLOR_SYSTEXT, COLOR_BACKGROUND); // full block
    if (vi < display_cols - size)
      display.drawBitmap(  CHAR_width + vi * 8 , 2 * CHAR_height, block_bar[ (uint8_t)(vf / 1.25 - 0.5)  ], 8, 15, COLOR_SYSTEXT, COLOR_BACKGROUND);
    for (uint8_t i = vi + 1; i < display_cols + 4 - size; i++)
      display.fillRect( CHAR_width + i * 8, 2 * CHAR_height, 8, 15, COLOR_BACKGROUND ); //empty block
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
    display.drawBitmap(  CHAR_width , 2 * CHAR_height + 4, meter_bar[ (uint8_t)(vf / 1.25 - 0.5)  ], 8, 8, COLOR_SYSTEXT, COLOR_BACKGROUND);
    for (uint8_t i = 1; i < display_cols - size; i++)
      display.fillRect( CHAR_width + i * 8, 2 * CHAR_height + 4, 8, 8, COLOR_BACKGROUND ); //empty block
  }
  else if (vi == display_cols - size)
  {
    for (uint8_t i = 0; i < display_cols - size - 1; i++)
      display.fillRect( CHAR_width + i * 8, 2 * CHAR_height + 4, 8, 8, COLOR_BACKGROUND ); //empty block
    display.drawBitmap(  CHAR_width + (display_cols - size - 1) * 8 , 2 * CHAR_height + 4, meter_bar[7], 8, 8, COLOR_SYSTEXT, COLOR_BACKGROUND);
  }
  else
  {
    for (uint8_t i = 0; i < display_cols - size + 4; i++)
      display.fillRect( CHAR_width + i * 8, 2 * CHAR_height + 4, 8, 8, COLOR_BACKGROUND ); //empty block
    display.drawBitmap(  CHAR_width + vi * 8 , 2 * CHAR_height + 4, meter_bar[ (uint8_t)(vf / 1.25 - 0.5)  ], 8, 8, COLOR_SYSTEXT, COLOR_BACKGROUND);
    for (uint8_t i = vi + 1; i < display_cols - size + 5; i++)
      display.fillRect( CHAR_width + i * 8, 2 * CHAR_height + 4, 8, 8, COLOR_BACKGROUND ); //empty block
  }
}


uint8_t bit_reverse8(uint8_t v)
{
  uint8_t result = 0;
  for ( ; v > 0; v >>= 1 )
    (result <<= 1) |= (v & 1);
  return (result);
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
  display.setTextColor(COLOR_PITCHSMP, COLOR_BACKGROUND);
  display.print(F("<SEARCHING"));
  display.setTextColor(COLOR_SYSTEXT, COLOR_BACKGROUND);
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
    display.setTextColor(COLOR_PITCHSMP, COLOR_BACKGROUND);
    display.print(F("<SEARCHING"));
    display.setTextColor(COLOR_SYSTEXT, COLOR_BACKGROUND);

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
    display.setTextColor(COLOR_PITCHSMP, COLOR_BACKGROUND);
    display.print(">SEARCHING");
    display.setTextColor(COLOR_SYSTEXT, COLOR_BACKGROUND);
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
  display.setTextColor(COLOR_PITCHSMP, COLOR_BACKGROUND);
  display.print(F(">SEARCHING"));
  display.setTextColor(COLOR_SYSTEXT, COLOR_BACKGROUND);
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
    Serial.print(F("check if Voice is a Favorite: "));
    Serial.print(tmp);
    Serial.println();
#endif
    if (SD.exists(tmp))
    { //is Favorite
#ifdef DEBUG
      Serial.println(F(" - It is in Favorites."));
#endif
      return true;
    }
    else
    { // it was not a favorite

#ifdef DEBUG
      Serial.println(F(" - It is not in Favorites."));
#endif
      return false;
    }
  }
  else
    return false;
}


float eraseBytesPerSecond(const unsigned char *id)
{
  if (id[0] == 0x20) return 152000.0; // Micron
  if (id[0] == 0x01) return 500000.0; // Spansion
  if (id[0] == 0xEF) return 419430.0; // Winbond
  if (id[0] == 0xC2) return 279620.0; // Macronix
  return 320000.0; // guess?
}


void UI_func_format_flash(uint8_t param)
{
  if (LCDML.FUNC_setup())         // ****** SETUP *********
  {
    border3_large_clear();
    encoderDir[ENC_R].reset();
    display.setCursor_textGrid(1, 1);
    display.print(F("Format FLASH?  "));
    display.setCursor_textGrid(1, 2);
    display.print(F("PUSH TO CONFIRM"));
  }
  if (LCDML.FUNC_loop())          // ****** LOOP *********
  {
    if (LCDML.BT_checkEnter())
    {
      uint8_t screenline = 0;
      display.setCursor_textGrid(1, 1);
      display.print(F("Formatting     "));
      display.setCursor_textGrid(1, 2);
      display.print(F("                "));
      unsigned char id[5];
      SerialFlash.readID(id);
      unsigned long size = SerialFlash.capacity(id);
      if (size > 0) {
        display.setCursor_textGrid(1, 6);
        display.setTextColor(COLOR_SYSTEXT, COLOR_BACKGROUND);
        display.print(F("Flash Memory has "));
        display.print(size);
        display.print(F(" bytes."));
        display.setCursor_textGrid(1, 7);
        display.setTextColor(RED, COLOR_BACKGROUND);
        display.print(F("Erasing ALL Flash Memory"));
        display.setCursor_textGrid(1, 9);
        display.setTextColor(GREY1, COLOR_BACKGROUND);
        display.print(F("Estimated time: "));
        int seconds = (float)size / eraseBytesPerSecond(id) + 0.5;
        display.setTextColor(COLOR_SYSTEXT, COLOR_BACKGROUND);
        display.print(seconds);
        display.setTextColor(GREY1, COLOR_BACKGROUND);
        display.print(F(" seconds."));
        SerialFlash.eraseAll();
        unsigned long dotMillis = millis();
        unsigned char dotcount = 0;
        display.setTextColor(RED, COLOR_BACKGROUND);
        while (SerialFlash.ready() == false)
        {
          if (millis() - dotMillis > 1000)
          {
            dotMillis = dotMillis + 1000;
            display.setCursor_textGrid(1 + dotcount, 12 + screenline);
            display.print(".");
            dotcount = dotcount + 1;
            if (dotcount >= 32)
            {
              screenline++;
              dotcount = 0;
            }
          }
        }
        if (dotcount > 0)
        {
          border3_large_clear();
          display.setCursor_textGrid(1, 2);
          display.setTextColor(GREEN, COLOR_BACKGROUND);
          display.print(F("done!           "));
          display.setTextColor(COLOR_SYSTEXT, COLOR_BACKGROUND);
        }
      }
    }
  }
  if (LCDML.FUNC_close())     // ****** STABLE END *********
  {
    encoderDir[ENC_R].reset();
  }
}

void draw_favorite_icon(uint8_t b, uint8_t v, uint8_t instance_id)
{
  b = constrain(b, 0, MAX_BANKS - 1);
  v = constrain(v, 0, MAX_VOICES - 1);
  char tmp[18];
  File myFav;
  if (sd_card > 0)
  {
    display.setTextSize(1);
    sprintf(tmp, "/%s/%d/%d.fav", FAV_CONFIG_PATH, b, v);
    if (SD.exists(tmp))
    { //is Favorite
      //display.drawBitmap(16 * CHAR_width + 11, 5, special_chars[18], 8, 8, GREEN); //fav symbol
      display.fillRect(16 * CHAR_width + 11, 5, 9, 9, GREEN);
      display.setCursor(16 * CHAR_width + 13, 6);
      display.setTextColor(COLOR_BACKGROUND);
      display.print(F("F"));

    }
    else
    { // it is not a favorite
      // display.drawBitmap(16 * CHAR_width + 11, 5, special_chars[18], 8, 8, GREY2); //fav symbol
      display.fillRect(16 * CHAR_width + 11, 5, 9, 9, GREY2);
      display.setCursor(16 * CHAR_width + 13, 6);
      display.setTextColor(COLOR_BACKGROUND);
      display.print(F("F"));

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
    Serial.print(F("check if there is a Favorite in Bank: "));
    Serial.print(tmp);
    Serial.println();
#endif
    if (SD.exists(tmp) )
    { // this bank HAS at least 1 favorite(s)
#ifdef DEBUG
      Serial.println(F("quickcheck found a FAV in bank!"));
#endif
      return (true);
    }
    else
    { // no favorites in bank stored
      return (false);
#ifdef DEBUG
      Serial.println(F(" - It is no Favorite in current Bank."));
#endif
    }
  }
  else
    return false;
}

void save_favorite(uint8_t b, uint8_t v, uint8_t instance_id)
{
#ifdef DEBUG
  Serial.println(F("Starting saving Favorite."));
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
    Serial.println(F("Save Favorite to SD card..."));
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
#ifdef DEBUG
      Serial.println(F("Favorite saved..."));
#endif
      //display.drawBitmap(16 * CHAR_width + 11, 5, special_chars[18], 8, 8, GREEN); //fav symbol
      display.setTextSize(1);
      display.fillRect(16 * CHAR_width + 11, 5, 9, 9, GREEN);
      display.setCursor(16 * CHAR_width + 13, 6);
      display.setTextColor(COLOR_BACKGROUND);
      display.print(F("F"));


#ifdef DEBUG
      Serial.println(F("Added to Favorites..."));
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
        Serial.println(F("Fav count in bank:"));
        Serial.print(countfavs);
        Serial.println(F("Removed folder since no voice in bank flagged as favorite any more"));
#endif
      }

      // display.fillRect(16 * CHAR_width + 11, 5, 8, 8, COLOR_BACKGROUND); ////remove fav symbol
      display.setTextSize(1);
      display.fillRect(16 * CHAR_width + 11, 5, 9, 9, GREY2);
      display.setCursor(16 * CHAR_width + 13, 6);
      display.setTextColor(COLOR_BACKGROUND);
      display.print(F("F"));

      // display.print(" "); //remove fav symbol

#ifdef DEBUG
      Serial.println(F("Removed from Favorites..."));
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
