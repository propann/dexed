/*
   MicroDexed-touch

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
#include <Regexp.h>

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

extern PeriodicTimer sequencer_timer;
extern ILI9341_t3n display;
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
extern void print_current_chord(void);
extern void tracker_print_pattern(int xpos, int ypos, uint8_t track_number);
extern void print_merged_pattern_pianoroll(int xpos, int ypos, uint8_t track_number);
extern void update_pianoroll();
extern void set_pattern_content_type_color(uint8_t pattern);
extern void seq_print_formatted_number(uint16_t v, uint8_t l);
extern void seq_print_formatted_number_signed(int v, uint8_t l);
extern void seq_print_current_note_from_step(uint8_t s);
extern void seq_print_step_numbers(int xpos, int ypos);
extern void print_single_pattern_pianoroll_in_pattern_editor(int xpos, int ypos, uint8_t pattern,  uint8_t actstep, bool fullredraw);
extern void print_chord_name (uint8_t currentstep);
extern void print_file_manager_buttons(void);
extern uint16_t RGB24toRGB565(uint8_t r, uint8_t g, uint8_t b);
extern uint32_t ColorHSV(uint16_t hue, uint8_t sat, uint8_t val);
extern uint32_t ColorHSV2(uint16_t hue, uint8_t sat, uint8_t val);
extern uint8_t get_chain_length_from_current_track(uint8_t tr);
extern void playWAVFile(const char *filename);
extern uint8_t get_song_length(void);
extern void helptext_l (const char *str);
extern void helptext_r (const char *str);
extern void seq_pattern_editor_update_dynamic_elements();
extern uint8_t microsynth_selected_instance;
extern AudioMixer<2>   microsynth_mixer_reverb;
extern void virtual_keyboard_print_current_instrument();
extern uint8_t find_longest_chain();
extern bool get_bank_name(uint8_t b, char* bank_name);
extern bool get_voice_name(uint8_t b, uint8_t v, char* voice_name);
extern void fill_msz_from_flash_filename (const uint16_t entry_number, const uint8_t preset_number, const uint8_t zone_number);
extern void stop_all_drum_slots();
extern char noteNames[12][3];

#if NUM_DRUMS > 0
#include "drums.h"
extern void get_sd_performance_name_json(uint8_t number);
extern bool save_sd_performance_json(uint8_t p);
extern void virtual_keyboard();
extern uint8_t drum_midi_channel;
extern drum_config_t drum_config[NUM_DRUMSET_CONFIG];
extern sequencer_t seq;
extern ts_t ts;
extern fm_t fm;
extern dexed_live_mod_t dexed_live_mod;
extern multisample_t ms[NUM_MULTISAMPLES];
extern multisample_zone_t msz[NUM_MULTISAMPLES][NUM_MULTISAMPLE_ZONES];

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

#if defined(USE_EPIANO) && defined(USE_MICROSYNTH)
extern AudioSynthEPiano               ep;
extern AudioMixer<5>                  reverb_mixer_r;
extern AudioMixer<5>                  reverb_mixer_l;
extern microsynth_t             microsynth[NUM_MICROSYNTH];
extern AudioSynthWaveform       microsynth_waveform[NUM_MICROSYNTH];
extern AudioEffectEnvelope      microsynth_envelope_osc[NUM_MICROSYNTH];
extern AudioFilterStateVariable microsynth_filter_osc[NUM_MICROSYNTH];
extern AudioFilterStateVariable microsynth_filter_noise[NUM_MICROSYNTH];
extern AudioSynthNoisePink      microsynth_noise[NUM_MICROSYNTH];
extern AudioEffectEnvelope      microsynth_envelope_noise[NUM_MICROSYNTH];
extern AudioMixer<4>            microsynth_mixer_filter_osc[NUM_MICROSYNTH];
extern AudioMixer<4>            microsynth_mixer_filter_noise[NUM_MICROSYNTH];
#elif defined(USE_EPIANO)
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

#if defined(USE_EPIANO) && defined(USE_MICROSYNTH)
extern AudioMixer<7>                   master_mixer_r;
extern AudioMixer<7>                   master_mixer_l;
#elif defined(USE_EPIANO)
extern AudioMixer<6>                   master_mixer_r;
extern AudioMixer<6>                   master_mixer_l;
#else
extern AudioMixer<5>                   master_mixer_r;
extern AudioMixer<5>                   master_mixer_l;
#endif
extern AudioEffectStereoMono          stereo2mono;
extern AudioAnalyzePeak               master_peak_r;
extern AudioAnalyzePeak               master_peak_l;

extern char sd_string[display_cols + 1];
extern char g_voice_name[NUM_DEXED][VOICE_NAME_LEN];
extern char g_bank_name[NUM_DEXED][BANK_NAME_LEN];
extern char tmp_voice_name[VOICE_NAME_LEN];
extern char tmp_bank_name[BANK_NAME_LEN];
extern const float midi_ticks_factor[10];
extern uint8_t midi_bpm;
extern bool save_sys_flag;
extern elapsedMillis save_sys;
extern bool save_sys_flag;

/***********************************************************************
   GLOBAL
************************************************************************/

uint16_t COLOR_BACKGROUND = 0x0000;
uint16_t COLOR_SYSTEXT = 0xFFFF;
uint16_t COLOR_SYSTEXT_ACCENT = 0x159A;
uint16_t COLOR_INSTR = 0x7BBD;
uint16_t COLOR_CHORDS = 0xE2FA;
uint16_t COLOR_ARP = 0xFC80;
uint16_t COLOR_DRUMS = 0xFE4F;
uint16_t COLOR_PITCHSMP = 0x159A;


elapsedMillis back_from_volume;
uint8_t instance_num[8][8];
const char accepted_chars[] = " _ABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789-abcdefghijklmnopqrstuvwxyz";

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

short wave_type[9] = {
  WAVEFORM_SINE,
  WAVEFORM_TRIANGLE,
  WAVEFORM_SAWTOOTH,
  WAVEFORM_SQUARE,
  WAVEFORM_PULSE,
  WAVEFORM_BANDLIMIT_SAWTOOTH,
  WAVEFORM_BANDLIMIT_SQUARE,
  WAVEFORM_BANDLIMIT_PULSE,
  WAVEFORM_SAMPLE_HOLD
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
void UI_func_custom_mappings(uint8_t param);
void UI_func_cc_mappings(uint8_t param);
void UI_func_microsynth(uint8_t param);
void UI_func_seq_pattern_editor(uint8_t param);
void UI_func_seq_vel_editor(uint8_t param);
void UI_func_clear_song(uint8_t param);
void UI_func_clear_song_chains(uint8_t param);
void UI_func_seq_settings(uint8_t param);
void UI_func_seq_tracker(uint8_t param);
void UI_func_seq_pianoroll(uint8_t param);
void UI_func_arpeggio(uint8_t param);
void UI_func_seq_mute_matrix(uint8_t param);
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
void UI_func_startup_performance(uint8_t param);
void UI_func_startup_page(uint8_t param);
void UI_func_colors(uint8_t param);
void UI_func_favorites(uint8_t param);
void UI_func_epiano(uint8_t param);
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
void UI_func_format_flash(uint8_t param);
void UI_func_MultiSamplePlay(uint8_t param);
void UI_func_sample_editor(uint8_t param);


char* basename(const char* filename);

// normal menu
LCDMenuLib2_menu LCDML_0(255, 0, 0, NULL, NULL); // normal root menu element (do not change)
LCDMenuLib2 LCDML(LCDML_0, _LCDML_DISP_rows, _LCDML_DISP_cols, lcdml_menu_display, lcdml_menu_clear, lcdml_menu_control);

#include "UI.h"

int favsearcher = 0;

void draw_button_on_grid(uint8_t x, uint8_t y, const char *t1, const char *t2, uint8_t color )
{
  if (color == 99) //special case, draw virtual keyboard button (icon)
  {
    display.fillRect(x * CHAR_width_small , y * CHAR_height_small, button_size_x * CHAR_width_small, CHAR_height_small * button_size_y / 2 - 2, GREY2);

    uint8_t offset[5] = {1, 2, 2, 4, 6 }; //+ is the offset to left
    int offcount = 0;
    display.setTextSize(1);
    display.setTextColor(GREY1, GREY2);
    display.setCursor(x * CHAR_width_small + CHAR_width_small / 2 , y * CHAR_height_small + 4 );
    display.print("V.KEYB");
    //draw white keys
    for (uint8_t i = 0; i < 7; i++)
    {
      display.fillRect( x * CHAR_width_small + 6 * i, y + 23, 5, 15, COLOR_SYSTEXT); // pianoroll white key
    }
    for (uint8_t i = 0; i < 11; i++)
    {
      if (seq.piano[i] == 1)
      {
        display.fillRect( x * CHAR_width_small + 4 * i - offset[offcount], y + 23, 4, 8, COLOR_BACKGROUND); // BLACK key
        offcount++;
        if (offcount == 5)offcount = 0;
      }
    }
  }
  else
  {
    display.setTextSize(1);
    if (color == 0) //standard grey
    {
      display.setTextColor(GREY1, GREY2);
      display.fillRect(x * CHAR_width_small , y * CHAR_height_small, button_size_x * CHAR_width_small, CHAR_height_small * button_size_y, GREY2);
    }
    else if (color == 1) //button has active color
    {
      display.setTextColor(COLOR_SYSTEXT, DX_DARKCYAN);
      display.fillRect(x * CHAR_width_small , y * CHAR_height_small, button_size_x * CHAR_width_small, CHAR_height_small * button_size_y, DX_DARKCYAN);
    }
    else if (color == 2) //button has RED color
    {
      display.setTextColor(COLOR_SYSTEXT, RED);
      display.fillRect(x * CHAR_width_small , y * CHAR_height_small, button_size_x * CHAR_width_small, CHAR_height_small * button_size_y, RED);
    }
    display.setCursor(x * CHAR_width_small + CHAR_width_small / 2 , y * CHAR_height_small + 6 );
    display.print(t1);
    if (t2[1] == '\0')
    {
      display.setCursor((x + 2)* CHAR_width_small + CHAR_width_small / 2 , y * CHAR_height_small + 6 + CHAR_height_small );
      display.setTextSize(2);
    }
    else
    {
      display.setCursor(x * CHAR_width_small + CHAR_width_small / 2 , y * CHAR_height_small + 10 + CHAR_height_small );
    }
    display.print(t2);
    display.setTextSize(1);
    // display.setTextColor(COLOR_SYSTEXT,COLOR_BACKGROUND);
  }
}

void show_smallfont_noGrid(int pos_y, int pos_x, uint8_t field_size, const char *str)
{
  display.setTextSize(1);
  char tmp[STRING_BUFFER_SIZE];
  char *s = tmp;
  uint8_t l = strlen(str);
  memset(tmp, 0, sizeof(tmp));
  memset(tmp, 0x20, field_size); // blank
  if (l > field_size)
    l = field_size;
  strncpy(s, str, l);
  display.setCursor(pos_x , pos_y  );
  display.print(tmp);
}

void setCursor_textGrid(uint8_t pos_x, uint8_t pos_y)
{
  display.setCursor(pos_x * CHAR_width, pos_y * CHAR_height);
}

void setCursor_textGrid_mini(uint8_t pos_x, uint8_t pos_y)
{
  display.setCursor(pos_x * 6, pos_y * 10);
}

void setCursor_textGrid_large(uint8_t pos_x, uint8_t pos_y)
{
  display.setCursor(pos_x * CHAR_width, pos_y * (CHAR_height + 1));
}

void show(uint8_t pos_y, uint8_t pos_x, uint8_t field_size, const char *str)
{
  char tmp[STRING_BUFFER_SIZE];
  char *s = tmp;
  uint8_t l = strlen(str);
  memset(tmp, 0, sizeof(tmp));
  memset(tmp, 0x20, field_size); // blank
  if (l > field_size)
    l = field_size;
  strncpy(s, str, l);
  display.setCursor(pos_x * CHAR_width, pos_y * CHAR_height );
  display.print(tmp);
}

void show_small_font(int pos_y, int pos_x, uint8_t field_size, const char *str)
{
  char tmp[STRING_BUFFER_SIZE];
  char *s = tmp;
  uint8_t l = strlen(str);
  memset(tmp, 0, sizeof(tmp));
  memset(tmp, 0x20, field_size); // blank
  if (l > field_size)
    l = field_size;
  strncpy(s, str, l);
  display.setCursor(pos_x, pos_y );
  display.print(tmp);
}

void show(uint8_t pos_y, uint8_t pos_x, uint8_t field_size, long num)
{
  char _buf10[STRING_BUFFER_SIZE];
  show(pos_y, pos_x, field_size, itoa(num, _buf10, 10) );
}
void show_small_font(uint8_t pos_y, uint8_t pos_x, uint8_t field_size, long num)
{
  char _buf10[STRING_BUFFER_SIZE];
  show_small_font(pos_y, pos_x, field_size, itoa(num, _buf10, 10) );
}

void drawBitmap(int16_t x, int16_t y, const uint8_t bitmap[],
                int16_t w, int16_t h, uint16_t color) {

  int16_t byteWidth = (w + 7) / 8; // Bitmap scanline pad = whole byte
  uint8_t byte = 0;

  for (int16_t j = 0; j < h; j++, y++) {
    for (int16_t i = 0; i < w; i++) {
      if (i & 7)
        byte <<= 1;
      else
        byte = pgm_read_byte(&bitmap[j * byteWidth + i / 8]);
      if (byte & 0x80)
        display.drawPixel(x + i, y, color);
    }
  }
}

void drawBitmap(int16_t x, int16_t y, const uint8_t bitmap[],
                int16_t w, int16_t h, uint16_t color,
                uint16_t bg) {
  int16_t byteWidth = (w + 7) / 8; // Bitmap scanline pad = whole byte
  uint8_t byte = 0;

  for (int16_t j = 0; j < h; j++, y++) {
    for (int16_t i = 0; i < w; i++) {
      if (i & 7)
        byte <<= 1;
      else
        byte = pgm_read_byte(&bitmap[j * byteWidth + i / 8]);
      display.drawPixel(x + i, y, (byte & 0x80) ? color : bg);
    }
  }

}


void drawBitmap(int16_t x, int16_t y, uint8_t *bitmap, int16_t w,
                int16_t h, uint16_t color) {

  int16_t byteWidth = (w + 7) / 8; // Bitmap scanline pad = whole byte
  uint8_t byte = 0;

  for (int16_t j = 0; j < h; j++, y++) {
    for (int16_t i = 0; i < w; i++) {
      if (i & 7)
        byte <<= 1;
      else
        byte = bitmap[j * byteWidth + i / 8];
      if (byte & 0x80)
        display.drawPixel(x + i, y, color);
    }
  }

}

void drawBitmap(int16_t x, int16_t y, uint8_t *bitmap, int16_t w,
                int16_t h, uint16_t color, uint16_t bg) {

  int16_t byteWidth = (w + 7) / 8; // Bitmap scanline pad = whole byte
  uint8_t byte = 0;

  for (int16_t j = 0; j < h; j++, y++) {
    for (int16_t i = 0; i < w; i++) {
      if (i & 7)
        byte <<= 1;
      else
        byte = bitmap[j * byteWidth + i / 8];
      display.drawPixel(x + i, y, (byte & 0x80) ? color : bg);
    }
  }

}





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
  } while (display.getCursorX() < DISPLAY_WIDTH - 8);
}

void fill_up_with_spaces_left_window()
{
  do
  {
    display.print(" ");
  } while (display.getCursorX() < 35 * CHAR_width_small);
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

void border0()  //main Border  default Colors = GREY2/4
{
  display.drawRect(0, 0, DISPLAY_WIDTH, DISPLAY_HEIGHT, GREY4);  //Main Border
}

void border1() //upper left
{
  display.drawRect(0, 0, CHAR_width * 18, CHAR_height * 6 - 3, GREY4);
}
void border2()  //upper right
{
  display.drawRect(CHAR_width * 18 - 1, 0, DISPLAY_WIDTH - CHAR_width * 18 + 1, CHAR_height * 6 - 3,   GREY4);
}

void border1_clear() //upper left
{
  display.fillRect(1,  1, CHAR_width * 19 - 1, CHAR_height * 5 - 1, COLOR_BACKGROUND);
}
void border2_clear()  //upper right
{
  display.fillRect( CHAR_width * 18 - 1 + 1, 0 + 1, DISPLAY_WIDTH - CHAR_width * 18 - 1, CHAR_height * 6 - 5, COLOR_BACKGROUND);
}
void border2_half_clear()  //upper right
{
  display.fillRect( CHAR_width * 18 - 1 + 1, 0 + 1, 171, CHAR_height * 6 - 5, COLOR_BACKGROUND);
}
void border3_clear() //lower left
{
  display.fillRect(0 , CHAR_height * 5 - 4 , CHAR_width * 17 , DISPLAY_HEIGHT - CHAR_height * 5 + 3, COLOR_BACKGROUND);
}
void border4_clear()  //lower right
{
  display.fillRect(CHAR_width * 18 , CHAR_height * 6 - 3, DISPLAY_WIDTH - CHAR_width * 18 - 1, 321 - CHAR_height * 6 - 5, COLOR_BACKGROUND);
}
void border3_large()  //lower left+right as one window
{
  display.drawRect(0, CHAR_height * 6 - 4 , DISPLAY_WIDTH, DISPLAY_HEIGHT - CHAR_height * 6 + 4,  GREY4);
}

void border3_large_clear()  //lower left+right as one window
{
  display.fillRect(1, CHAR_height * 6 - 3 , DISPLAY_WIDTH, DISPLAY_HEIGHT - CHAR_height * 6 + 3,  COLOR_BACKGROUND);
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

const char* tracker_find_shortname_from_pattern_step(uint8_t track, uint8_t pattern, uint8_t sstep )
{
  const char* shortname;
  bool found = false;
  if (seq.content_type[ seq.current_pattern[track]  ] == 0 && seq.vel[ seq.current_pattern[track] ][sstep] < 210) //is Drumtrack and not a pitched sample
  {
    for (uint8_t d = 0; d < NUM_DRUMSET_CONFIG - 1; d++)
    {
      if (seq.note_data[ seq.current_pattern[track] ][sstep] == drum_config[d].midinote)
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
    if (seq.vel[ seq.current_pattern[track] ][sstep] > 209)
      shortname = "P"; // pitched sample
    else if (seq.note_data[ seq.current_pattern[track] ][sstep] > 0  && seq.note_data[ seq.current_pattern[track] ][sstep] != 130)
      shortname = noteNames[seq.note_data[ seq.current_pattern[track] ][sstep] % 12];
    else if (seq.note_data[ seq.current_pattern[track] ][sstep] == 130)
      shortname = "~"; // note is a tie/latch
    else
      shortname = "-";
  }
  return shortname;
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

void print_voice_settings_in_pattern_editor(int x, int y)
{
  display.setTextSize(1);
  display.setCursor(x, y);
  display.setTextColor(COLOR_SYSTEXT, COLOR_BACKGROUND);
  display.print("INST");
  display.setTextColor(GREEN, COLOR_BACKGROUND);
  display.setCursor(x + 101, y);
  display.print(selected_instance_id + 1);
  display.setTextColor(COLOR_PITCHSMP, COLOR_BACKGROUND);
  display.setCursor(x + 118, y - 1);
  seq_print_formatted_number(configuration.dexed[selected_instance_id].bank, 2);
  display.setCursor(x + 118, y + 7);
  seq_print_formatted_number(configuration.dexed[selected_instance_id].voice + 1, 2);
  display.setTextColor(COLOR_SYSTEXT, COLOR_BACKGROUND);
  display.setCursor(x + 120 + 16, y - 1);
  display.print(g_bank_name[selected_instance_id]);
  display.setCursor(x + 120 + 16, y + 7);
  display.print(g_voice_name[selected_instance_id]);
  display.setTextSize(2);
}

void update_pattern_number_in_tracker(uint8_t tracknumber)
{
  setCursor_textGrid_mini(9 + 6 * tracknumber, 3);

  if (seq.current_pattern[tracknumber] < 100)
  {
    set_pattern_content_type_color( seq.current_pattern[tracknumber] );
    seq_print_formatted_number( seq.current_pattern[tracknumber], 2);
  }
  else
  {
    display.setTextColor(GREY2, COLOR_BACKGROUND);
    display.print (F("--"));
  }
}

void print_track_steps_detailed_only_current_playing_note(int xpos, int ypos, uint8_t currentstep)
{
  if (seq.cycle_touch_element == 0)  // touch keyboard is off
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

      y = ypos + 10 + i * (CHAR_height_small + 2);
      // Short Name
      if ( (array[1] == seq.menu - 3 && LCDML.FUNC_getID() == LCDML.OTHER_getIDFromFunction(UI_func_seq_pattern_editor) ) ||
           ( array[1] == seq.menu - 1 && LCDML.FUNC_getID() == LCDML.OTHER_getIDFromFunction(UI_func_seq_vel_editor)  ) )
        display.setTextColor(COLOR_BACKGROUND, COLOR_SYSTEXT);
      else if (i == currentstep)
        display.setTextColor(COLOR_SYSTEXT, COLOR_PITCHSMP);
      else
        display.setTextColor(GREY2, COLOR_BACKGROUND);
      display.setCursor(CHAR_width_small * 4 , y);
      if (seq.vel[seq.active_pattern][i] > 209)  //it is a pitched Drum Sample
      {
        seq_print_current_note_from_step(i);
      }
      else
      {
        display.print (seq_find_shortname_in_track( i , seq.active_pattern)[0]   );
      }
      // Data values
      if ( (array[1] == seq.menu - 3 && LCDML.FUNC_getID() == LCDML.OTHER_getIDFromFunction(UI_func_seq_pattern_editor) ) ||
           ( array[1] == seq.menu - 1 && LCDML.FUNC_getID() == LCDML.OTHER_getIDFromFunction(UI_func_seq_vel_editor)  ) )
        display.setTextColor(COLOR_BACKGROUND, COLOR_SYSTEXT);
      else if (i == currentstep)
        display.setTextColor(COLOR_SYSTEXT, COLOR_PITCHSMP);
      else
        display.setTextColor(GREY2, COLOR_BACKGROUND);
      display.setCursor(CHAR_width_small * 7 , y);
      seq_print_formatted_number (seq.note_data[seq.active_pattern][i] , 3);
      // Velocity values
      if ( (array[1] == seq.menu - 3 && LCDML.FUNC_getID() == LCDML.OTHER_getIDFromFunction(UI_func_seq_pattern_editor) ) ||
           ( array[1] == seq.menu - 1 && LCDML.FUNC_getID() == LCDML.OTHER_getIDFromFunction(UI_func_seq_vel_editor)  ) )
        display.setTextColor(COLOR_BACKGROUND, COLOR_SYSTEXT);
      else if (i == currentstep)
        display.setTextColor(COLOR_SYSTEXT, COLOR_PITCHSMP);
      else
        display.setTextColor(GREY1, COLOR_BACKGROUND);
      display.setCursor(CHAR_width_small * 12 , y);
      seq_print_formatted_number (seq.vel[seq.active_pattern][i] , 3);
      // Long Name / Note

      if ( (array[1] == seq.menu - 3 && LCDML.FUNC_getID() == LCDML.OTHER_getIDFromFunction(UI_func_seq_pattern_editor) ) ||
           ( array[1] == seq.menu - 1 && LCDML.FUNC_getID() == LCDML.OTHER_getIDFromFunction(UI_func_seq_vel_editor)  ) )
        display.setTextColor(COLOR_BACKGROUND, COLOR_SYSTEXT);
      else if (i == currentstep)
        display.setTextColor(COLOR_SYSTEXT, COLOR_PITCHSMP);
      else
        set_pattern_content_type_color(seq.active_pattern);
      if (seq.content_type[seq.active_pattern] == 0) //Drum Track
      {
        if (seq.vel[seq.active_pattern][i] > 209)  //it is a pitched Drum Sample
        {
          show_smallfont_noGrid(y, CHAR_width_small * 17 , 10, basename(drum_config[seq.vel[seq.active_pattern][i] - 210].name) );
        }
        else // else it is a regular Drum Sample
          show_smallfont_noGrid(y, CHAR_width_small * 17 , 10, find_long_drum_name_from_note( seq.note_data[seq.active_pattern][i] ));
      }
      else if (seq.content_type[seq.active_pattern] > 0 ) //Inst Track or Chord or Arp
      {
        display.setCursor(x + CHAR_width_small * 17, y);
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
            if (seq.vel[ seq.active_pattern][i] < 200) //print octave when it is not a chord
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
      while (display.getCursorX() < CHAR_width_small * 32)
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
}

void print_playing_chains()
{

  for (uint8_t x = 0; x < NUM_SEQ_TRACKS; x++)
  {
    display.setTextColor( COLOR_SYSTEXT, COLOR_PITCHSMP);
    display.setCursor(30 * CHAR_width_small + (x * 3 * CHAR_width_small) ,  1);
    if ( seq.current_chain[x] < 99 )
      seq_print_formatted_number( seq.current_chain[x], 2);
    else
    {
      display.setTextColor( DX_DARKCYAN, COLOR_PITCHSMP);
      display.print(F("--"));
    }
    display.setTextColor( COLOR_BACKGROUND, COLOR_PITCHSMP);
    display.setCursor(30 * CHAR_width_small + (x * 3 * CHAR_width_small) ,  10);
    if (  get_chain_length_from_current_track(x) > 0 )
      seq_print_formatted_number(  get_chain_length_from_current_track(x), 2);
    else
    {
      display.setTextColor( DX_DARKCYAN, COLOR_PITCHSMP);
      display.print(F("--"));
    }
    //debug, show chain counter for all tracks
    display.setTextColor( GREY1, COLOR_BACKGROUND);
    display.setCursor(30 * CHAR_width_small + (x * 3 * CHAR_width_small) ,  20);
    seq_print_formatted_number(  seq.chain_counter[x], 2);
  }
  //show longest current chain
  display.setTextColor( COLOR_SYSTEXT, COLOR_PITCHSMP);
  display.setCursor(26 * CHAR_width_small  ,  10);
  seq_print_formatted_number(find_longest_chain(), 2);
}

void update_display_functions_while_seq_running()
{
  seq.UI_last_seq_step = seq.step;
  //is in UI_func_seq_pattern_editor or is in UI_func_seq_vel_editor
  if ( LCDML.FUNC_getID() == LCDML.OTHER_getIDFromFunction(UI_func_seq_pattern_editor) ||
       LCDML.FUNC_getID() == LCDML.OTHER_getIDFromFunction(UI_func_seq_vel_editor) )
  {
    display.setTextSize(2);
    if (seq.step == 0)
    {
      set_pattern_content_type_color(seq.active_pattern);
      setCursor_textGrid(14 , 1);
      display.print(seq_find_shortname(14)[0]);
      display.setTextColor(COLOR_BACKGROUND, COLOR_SYSTEXT);
      setCursor_textGrid(15, 1);
      display.print(seq_find_shortname(15)[0]);
      if (seq.note_editor_view == 0 && seq.cycle_touch_element == 0)
        print_track_steps_detailed_only_current_playing_note(0, CHAR_height * 4 + 3, 15);
    }
    else  if (seq.step == 1)
    {
      set_pattern_content_type_color(seq.active_pattern);
      setCursor_textGrid(15, 1);
      display.print(seq_find_shortname(15)[0]);
      display.setTextColor(COLOR_BACKGROUND, COLOR_SYSTEXT);
      setCursor_textGrid(0, 1);
      display.print(seq_find_shortname(0)[0]);
      if (seq.note_editor_view == 0 && seq.cycle_touch_element == 0)
        print_track_steps_detailed_only_current_playing_note(0, CHAR_height * 4 + 3, 0);
    }
    else
    {
      set_pattern_content_type_color(seq.active_pattern);
      setCursor_textGrid(seq.step - 2 , 1);
      display.print(seq_find_shortname(seq.step - 2)[0]);
      display.setTextColor(COLOR_BACKGROUND, COLOR_SYSTEXT);
      setCursor_textGrid(seq.step - 1 , 1);
      display.print(seq_find_shortname(seq.step - 1)[0]);
      if (seq.note_editor_view == 0 && seq.cycle_touch_element == 0)
        print_track_steps_detailed_only_current_playing_note(0, CHAR_height * 4 + 3, seq.step - 1);

    }
    if (seq.menu > 2 && seq.menu < 19 && LCDML.FUNC_getID() == LCDML.OTHER_getIDFromFunction(UI_func_seq_pattern_editor))
    { //update static cursor while moving cursor running in pattern editor
      setCursor_textGrid(seq.menu - 3, 1);
      display.setTextSize(2);
      display.setTextColor(GREEN, GREY2);
      display.print(seq_find_shortname(seq.menu - 3)[0]);
    }
    else  if ( seq.menu < 16 && LCDML.FUNC_getID() == LCDML.OTHER_getIDFromFunction(UI_func_seq_vel_editor))
    { //update static cursor while moving cursor running in vel editor
      setCursor_textGrid(seq.menu - 1, 1);
      display.setTextSize(2);
      display.setTextColor(GREEN, GREY2);
      display.print(seq_find_shortname(seq.menu - 1)[0]);
    }

    display.setTextColor(COLOR_SYSTEXT, COLOR_BACKGROUND);
  }
  else if (LCDML.FUNC_getID() == LCDML.OTHER_getIDFromFunction(UI_func_arpeggio)) //is in UI of Arpeggiator
  {
    display.setTextSize(2);
    setCursor_textGrid_large(11, 8);
    display.setTextColor(COLOR_SYSTEXT, COLOR_BACKGROUND);
    print_current_chord();
  }
  else if (LCDML.FUNC_getID() == LCDML.OTHER_getIDFromFunction(UI_func_seq_tracker)) //is in UI of Tracker
  {
    display.setTextColor(GREEN, COLOR_BACKGROUND);
    display.setCursor(5 * CHAR_width_small, (5 + seq.step) * (CHAR_height_small + 3) - 7);
    display.print (F(">"));
    if (seq.step == 1)
    {
      display.setCursor(CHAR_width_small * 46, 2); //print song step at each 1. pattern step
      display.setTextColor(COLOR_DRUMS, COLOR_PITCHSMP);
      seq_print_formatted_number(  seq.current_song_step , 2);
      display.setCursor(CHAR_width_small * 51, 2);
      seq_print_formatted_number(  get_song_length() , 2);

      for (uint8_t d = 0; d < NUM_SEQ_TRACKS; d++)
      {
        display.setTextColor(COLOR_BACKGROUND, COLOR_PITCHSMP);
        display.setCursor(CHAR_width_small * 16 + (CHAR_width_small * 3)*d , 2);
        seq_print_formatted_number(seq.chain_counter[d], 2);
        tracker_print_pattern(  (6 + 6 * d) * CHAR_width_small , 48,  d);
        update_pattern_number_in_tracker(d);
      }
    }

    if (seq.step > 0)
    {
      display.setCursor(5 * CHAR_width_small, (5 + seq.step - 1) * (CHAR_height_small + 3) - 7);
      display.print (F(" "));
    }
    else  if (seq.step == 0)
    {
      display.setCursor(5 * CHAR_width_small, (5 + 15) * (CHAR_height_small + 3) - 7);
      display.print (F(" "));
    }

  }
  else if (LCDML.FUNC_getID() == LCDML.OTHER_getIDFromFunction(UI_func_song))
  {
    display.setTextColor( GREEN, COLOR_BACKGROUND);  //play indicator song view

    if (CHAR_height_small * 8  + 10 *  ( seq.current_song_step - 1 - seq.scrollpos) > CHAR_height_small * 6)
    {
      display.setCursor(CHAR_width_small * 4 , CHAR_height_small * 8  + 10 *  ( seq.current_song_step - 1 - seq.scrollpos)  );
      display.print(" ");
    }
    if (CHAR_height_small * 8  + 10 *  (seq.current_song_step - seq.scrollpos) > CHAR_height_small * 7)
    {
      display.setCursor(CHAR_width_small * 4 , CHAR_height_small * 8  + 10 *  (seq.current_song_step - seq.scrollpos)  );
      display.print(">");
    }
    if (CHAR_height_small * 8  + 10  *  ( seq.current_song_step + 1 - seq.scrollpos)  > CHAR_height_small * 6)
    {
      display.setCursor(CHAR_width_small * 4 , CHAR_height_small * 8  + 10  *  ( seq.current_song_step + 1 - seq.scrollpos)  );
      display.print(" ");
    }

    if (seq.loop_start == 99)  // no loop start set, start at step 0
    {
      if (seq.current_song_step == 0 && get_song_length() > 1) // clear last cursor step after first cycle when song longer than 1 Chainstep
      {
        display.setCursor(CHAR_width_small * 4 , CHAR_height_small * 8 + 10 *  (get_song_length() - seq.scrollpos - 1 )  );
        display.print(" ");
      }
    }
    else
    {
      if (seq.current_song_step == seq.loop_start && seq.loop_start != seq.loop_end) // clear last cursor step after first cycle when song is in loop mode
      {

        display.setCursor(CHAR_width_small * 4 , CHAR_height_small * 8  + 10 *  (seq.loop_end - seq.scrollpos )  );
        display.print(" ");
      }
      //special case, loop start is higher than loop end
      if ( seq.loop_start > seq.loop_end &&  seq.current_song_step == 0) // clear cursor on loop start since we are cycling to step 0
      {
        display.setCursor(CHAR_width_small * 4 , CHAR_height_small * 8  + 10  *  (seq.loop_start - seq.scrollpos )  );
        display.print(" ");
      }
    }
    for (uint8_t d = 0; d < NUM_SEQ_TRACKS; d++)  // print currently playing notes/chords/drums
    {
      display.setCursor(CHAR_width_small * 6 + (4 * d)*CHAR_width_small , CHAR_height_small * 4   );
      set_pattern_content_type_color( seq.current_pattern[d] );
      if (seq.content_type[seq.current_pattern[d]] > 0) //it is a Inst. pattern
      {
        if (seq.note_data [seq.current_pattern[d]][seq.step] > 12 &&
            seq.note_data [seq.current_pattern[d]][seq.step] != 130 &&
            seq.note_data[seq.current_pattern[d]][seq.step] != 99)
        {
          display.print(noteNames[seq.note_data [seq.current_pattern[d]][seq.step] % 12 ][0] );
          if (noteNames[seq.note_data [seq.current_pattern[d]][seq.step] % 12 ][1] != '\0' )
          {
            display.print(noteNames[seq.note_data [seq.current_pattern[d]][seq.step] % 12 ][1] );
          }
          display.print( (seq.note_data [seq.current_pattern[d]][seq.step] / 12) - 1);
        }
        else if ( seq.note_data [seq.current_pattern[d]][seq.step] == 130) //latch
          display.print(F("LAT"));
        else
          display.print(F("   "));

      } else //it is a drum pattern

        if ( seq.vel[seq.current_pattern[d]][seq.step] < 210 ) //is Drumtrack and not a pitched sample
        {
          bool found = false;
          for (uint8_t n = 0; n < NUM_DRUMSET_CONFIG - 1; n++)
          {
            if (seq.note_data[seq.current_pattern[d]][seq.step] == drum_config[n].midinote)
            {
              display.print( drum_config[n].shortname);
              found = true;
              break;
            }
          }
          if (found == false) display.print( "- ");
        }
        else if ( seq.vel[seq.current_pattern[d]][seq.step] > 209) //pitched sample
          display.print(F("PS"));
    }
    //print currently playing chain steps
    print_playing_chains();
  }
  else if (LCDML.FUNC_getID() == LCDML.OTHER_getIDFromFunction(UI_func_seq_mute_matrix))
  {
    uint8_t track_count = 0;
    for (uint8_t y = 0; y < 2; y++)
    {
      for (uint8_t x = 0; x < 4; x++)
      {
        display.setCursor( (2 * CHAR_width_small) + x * (CHAR_width_small * 14), 4 + (10 * CHAR_height_small) + (y * (CHAR_height_small * 8)) );
        set_pattern_content_type_color( seq.current_pattern[track_count] );
        if (seq.content_type[seq.current_pattern[track_count]] > 0) //it is a Inst. pattern
        {
          if (seq.note_data [seq.current_pattern[track_count]][seq.step] > 12 &&
              seq.note_data [seq.current_pattern[track_count]][seq.step] != 130 &&
              seq.note_data[seq.current_pattern[track_count]][seq.step] != 99)
          {
            display.print(noteNames[seq.note_data [seq.current_pattern[track_count]][seq.step] % 12 ][0] );
            if (noteNames[seq.note_data [seq.current_pattern[track_count]][seq.step] % 12 ][1] != '\0' )
            {
              display.print(noteNames[seq.note_data [seq.current_pattern[track_count]][seq.step] % 12 ][1] );
            }
            display.print( (seq.note_data [seq.current_pattern[track_count]][seq.step] / 12) - 1);
          }
          else if ( seq.note_data [seq.current_pattern[track_count]][seq.step] == 130) //latch
            display.print(F("LAT"));
          else
            display.print(F("   "));
        } else //it is a drum pattern
          if ( seq.vel[seq.current_pattern[track_count]][seq.step] < 210 ) //is Drumtrack and not a pitched sample
          {
            bool found = false;
            for (uint8_t n = 0; n < NUM_DRUMSET_CONFIG - 1; n++)
            {
              if (seq.note_data[seq.current_pattern[track_count]][seq.step] == drum_config[n].midinote)
              {
                display.print( drum_config[n].shortname);
                found = true;
                break;
              }
            }
            if (found == false) display.print( "- ");
          }
          else if ( seq.vel[seq.current_pattern[track_count]][seq.step] > 209) //pitched sample
            display.print(F("PS"));
        track_count++;
      }
    }
  }
  else if (LCDML.FUNC_getID() == LCDML.OTHER_getIDFromFunction(UI_func_seq_pianoroll)) //is in UI of Pianoroll
  {
    update_pianoroll();
  }
}

int const qix_num = 20;

typedef struct qix_s
{
  int counthue;
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
    display.fillScreen(0);
    qix.counthue = random(358);
    // setup function
    LCDML.FUNC_setLoopInterval(20);  // starts a trigger event for the loop function every 20 milliseconds
  }
  if (LCDML.FUNC_loop())          // ****** LOOP *********
  {
    // if (LCDML.BT_checkAny() || touch.touched() == true   ) // check if any button is pressed (enter, up, down, left, right)
    if (LCDML.BT_checkAny()   ) // check if any button is pressed (enter, up, down, left, right)
    {
      LCDML.FUNC_goBackToMenu();  // leave this function
    }
    qix.counthue = qix.counthue + 1;
    if (qix.counthue > 359 - (qix_num * 3))
      qix.counthue = qix_num * 3;

    display.drawLine (qix.x0s[qix_num - 1], qix.y0s[qix_num - 1], qix.x1s[qix_num - 1], qix.y1s[qix_num - 1] , 0 );
    for (uint8_t j = 0; j < qix_num - 1; j++)
    {
      display.drawLine (qix.x0s[j], qix.y0s[j], qix.x1s[j], qix.y1s[j], ColorHSV(qix.counthue - (qix_num * 3) + j * 3 , 254, 254)    );
    }
    for (uint8_t j = qix_num - 1; j >= 1; j--)
    {
      qix.x0s[j] = qix.x0s[j - 1];
      qix.x1s[j] = qix.x1s[j - 1];
      qix.y0s[j] = qix.y0s[j - 1];
      qix.y1s[j] = qix.y1s[j - 1];
    }
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
  display.setRotation(3);
  display.setTextColor(COLOR_SYSTEXT, COLOR_BACKGROUND);
  display.setTextSize(2);
  display.fillScreen(COLOR_BACKGROUND);

  // LCDMenuLib Setup
  LCDML_setup(_LCDML_DISP_cnt);
  // Enable Menu Rollover
  //LCDML.MENU_enRollover();
  //Enable Screensaver (screensaver menu function, time to activate in ms)

  LCDML.SCREEN_enable(mFunc_screensaver, 200000);
  //LCDML.SCREEN_enable(mFunc_screensaver, 7000); //quick test time
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
    else if (LCDML.FUNC_getID() == LCDML.OTHER_getIDFromFunction(UI_func_microsynth))  //long press microsynth button R
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
        if ( LCDML.FUNC_getID() == LCDML.OTHER_getIDFromFunction(UI_func_song) && seq.tracktype_or_instrument_assign == 0 )
        {
          if (seq.cycle_touch_element < 5 || seq.cycle_touch_element > 9) //is not in song/chain edit
          {
            if (seq.loop_edit_step < 2)  //not in loop edit mode
            {
              seq.selected_track++;
              if (seq.loop_edit_step == 1)
              {
                seq.loop_edit_step = 0;
                seq.selected_track = 0;
              }
              if (seq.selected_track > NUM_SEQ_TRACKS - 1)
              {
                seq.loop_edit_step = 1;
                seq.selected_track = 0;
              }
            }
          }
          else  if (seq.cycle_touch_element == 5 ) //is in song edit
          {
            seq.cycle_touch_element = 6; //switch to chain edit
          }
          else  if (seq.cycle_touch_element == 6 ) //is in chain edit
          {
            seq.cycle_touch_element = 8; //switch to transpose edit
          }
          seq.help_text_needs_refresh = true;
        }
        else  if ( (LCDML.FUNC_getID() == LCDML.OTHER_getIDFromFunction(UI_func_song) && seq.tracktype_or_instrument_assign == 1) ||
                   (LCDML.FUNC_getID() == LCDML.OTHER_getIDFromFunction(UI_func_song) && seq.tracktype_or_instrument_assign == 5) ||
                   //select track for instrument or select track for tracktype change
                   (LCDML.FUNC_getID() == LCDML.OTHER_getIDFromFunction(UI_func_seq_tracker) && seq.edit_state == false) ||
                   (LCDML.FUNC_getID() == LCDML.OTHER_getIDFromFunction(UI_func_MultiSamplePlay) && seq.edit_state == false))

        {

          if (LCDML.FUNC_getID() == LCDML.OTHER_getIDFromFunction(UI_func_MultiSamplePlay) && seq.edit_state == false)
          {
            if (seq.selected_track == 6)
              seq.selected_track = 0;
            else
              seq.selected_track++;
          }
          else
          {
            if (seq.selected_track == NUM_SEQ_TRACKS - 1)
              seq.selected_track = 0;
            else
              seq.selected_track++;
          }
        }
        else if ( LCDML.FUNC_getID() == LCDML.OTHER_getIDFromFunction(UI_func_seq_tracker) && seq.edit_state  )
        {
          if (seq.vel[ seq.current_pattern[seq.selected_track]][seq.scrollpos] < 253)
            seq.vel[ seq.current_pattern[seq.selected_track]][seq.scrollpos]++;
        }
        else  if (LCDML.FUNC_getID() == LCDML.OTHER_getIDFromFunction(UI_func_song) && seq.tracktype_or_instrument_assign != 0)
        { //do nothing
          ;
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
        if (LCDML.FUNC_getID() == LCDML.OTHER_getIDFromFunction(UI_func_song)  && seq.tracktype_or_instrument_assign == 0 )
        {
          if (seq.cycle_touch_element < 5 || seq.cycle_touch_element > 9) //is not in song/chain edit
          {
            if (seq.loop_edit_step < 2)  //not in loop edit mode
            {
              if (seq.selected_track == 0)
              {
                seq.loop_edit_step = 1;
                seq.selected_track = NUM_SEQ_TRACKS - 1;
              }
              else if (seq.loop_edit_step == 1)
              {
                seq.loop_edit_step = 0;
                seq.selected_track = NUM_SEQ_TRACKS - 1;
              }
              else
                seq.selected_track--;
            }
          }
          else  if (seq.cycle_touch_element == 8 ) //is in transpose edit
          {
            seq.cycle_touch_element = 6; //switch to chain edit
          }
          else  if (seq.cycle_touch_element == 6 ) //is in chain edit
          {
            seq.cycle_touch_element = 5; //switch to song edit
          }
          seq.help_text_needs_refresh = true;
        }
        else  if ( (LCDML.FUNC_getID() == LCDML.OTHER_getIDFromFunction(UI_func_song) && seq.tracktype_or_instrument_assign == 1) ||
                   (LCDML.FUNC_getID() == LCDML.OTHER_getIDFromFunction(UI_func_song) && seq.tracktype_or_instrument_assign == 5) ||
                   //select track for instrument or select track for tracktype change
                   (LCDML.FUNC_getID() == LCDML.OTHER_getIDFromFunction(UI_func_seq_tracker) && seq.edit_state == false) ||
                   (LCDML.FUNC_getID() == LCDML.OTHER_getIDFromFunction(UI_func_MultiSamplePlay) && seq.edit_state == false))
        {

          if (seq.selected_track == 0)
          {
            if (LCDML.FUNC_getID() == LCDML.OTHER_getIDFromFunction(UI_func_MultiSamplePlay) && seq.edit_state == false)
              seq.selected_track = 6;
            else
              seq.selected_track = NUM_SEQ_TRACKS - 1;
          }
          else
            seq.selected_track--;
        }
        else if ( LCDML.FUNC_getID() == LCDML.OTHER_getIDFromFunction(UI_func_seq_tracker) && seq.edit_state  )
        {
          if (seq.vel[ seq.current_pattern[seq.selected_track]][seq.scrollpos] > 0)
            seq.vel[ seq.current_pattern[seq.selected_track]][seq.scrollpos]--;
        }
        else  if (LCDML.FUNC_getID() == LCDML.OTHER_getIDFromFunction(UI_func_song) && seq.tracktype_or_instrument_assign != 0)
        { //do nothing
          ;
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
            show(n + 1, 1, display_cols - 3, content_text);
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
          drawBitmap( (_LCDML_DISP_cols - 2) * CHAR_width , (n + 1) * CHAR_height, scroll_bar[0], 8, 17, COLOR_SYSTEXT, COLOR_BACKGROUND);
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
          drawBitmap( (_LCDML_DISP_cols - 2) * CHAR_width , CHAR_height, scroll_bar[1], 8, 17, COLOR_SYSTEXT, COLOR_BACKGROUND);
        } else if (scrollbar_cur_pos == (scrollbar_max - 1)) {
          // max pos
          drawBitmap( (_LCDML_DISP_cols - 2) * CHAR_width , (n_max ) * CHAR_height, scroll_bar[4], 8, 17, COLOR_SYSTEXT, COLOR_BACKGROUND);
        } else {
          // between
          drawBitmap( (_LCDML_DISP_cols - 2) * CHAR_width , (scroll_pos / n_max + 1) * CHAR_height, scroll_bar[(uint8_t)(scroll_pos % n_max) + 1], 8, 17, COLOR_SYSTEXT, COLOR_BACKGROUND);
        }
      }
      else
        display.fillRect((_LCDML_DISP_cols - 2) * CHAR_width , CHAR_height * 4, 11, 17  , COLOR_BACKGROUND); //Clear scrollbar bottom if menu options are lower than row count
    }
  }
  display.setTextColor(COLOR_SYSTEXT, COLOR_BACKGROUND);
  display.fillRect(CHAR_width * 18 - 1, 5, CHAR_width * 2 + 9, 9, COLOR_BACKGROUND); //clear upper right chars/icons
}

//####################################################################################################################################################################################################

/***********************************************************************
   MENU
 ***********************************************************************/


void colors_screen_update_text_preview()
{
  display.setTextSize(1);
  helptext_l("BACK");
  display.setTextColor(COLOR_SYSTEXT, COLOR_BACKGROUND);
  setCursor_textGrid(1, 1);
  display.print(F("COLORS"));
  display.setTextColor(COLOR_SYSTEXT, COLOR_BACKGROUND);
  setCursor_textGrid(1, 3);
  display.print(F("SYSTEM TEXT"));
  display.setTextColor(COLOR_SYSTEXT_ACCENT, COLOR_BACKGROUND);
  setCursor_textGrid(1, 4);
  display.print(F("SYSTEM ACCENT"));
  display.setTextColor(COLOR_BACKGROUND, COLOR_SYSTEXT);
  setCursor_textGrid(1, 5);
  display.print(F("SYS BACKGRND"));
  display.setTextColor(COLOR_DRUMS, COLOR_BACKGROUND);
  setCursor_textGrid(1, 6);
  display.print(F("DRUMS"));
  display.setTextColor(COLOR_BACKGROUND, COLOR_DRUMS);
  setCursor_textGrid(7, 6);
  display.print(F("DRUMS"));
  display.setTextColor(COLOR_PITCHSMP, COLOR_BACKGROUND);
  setCursor_textGrid(1, 7);
  display.print(F("PITCH SMP"));
  display.setTextColor(COLOR_BACKGROUND, COLOR_PITCHSMP);
  setCursor_textGrid(7, 7);
  display.print(F("PITCH SMP"));
  display.setTextColor(COLOR_CHORDS, COLOR_BACKGROUND);
  setCursor_textGrid(1, 8);
  display.print(F("CHORD/ARP"));
  display.setTextColor(COLOR_BACKGROUND, COLOR_CHORDS);
  setCursor_textGrid(7, 8);
  display.print(F("CHORD/ARP"));
  display.setTextColor(COLOR_INSTR, COLOR_BACKGROUND);
  setCursor_textGrid(1, 9);
  display.print(F("INSTR"));
  display.setTextColor(COLOR_BACKGROUND, COLOR_INSTR);
  setCursor_textGrid(7, 9);
  display.print(F("INSTR"));
}

void setModeColor(uint8_t selected_option)
{
  if (seq.temp_select_menu == selected_option)
  {
    display.setTextColor( COLOR_ARP, COLOR_BACKGROUND);
  } else
    display.setTextColor(COLOR_SYSTEXT, COLOR_BACKGROUND);
  if (seq.temp_select_menu == selected_option && seq_active_function == 1)
  {
    display.setTextColor(COLOR_PITCHSMP, COLOR_BACKGROUND);
  }
}

void print_small_intbar(uint8_t x, uint8_t y, uint8_t input_value, uint8_t selected_option, boolean show_bar, boolean show_zero)
{
  setCursor_textGrid_mini(x, y);
  setModeColor( selected_option);

  if (show_zero == false && input_value == 0)
    display.print(F("OFF"));
  else
    seq_print_formatted_number( input_value, 3);

  if (show_bar)
  {
    display.drawRect(CHAR_width_small * x + 4 * CHAR_width_small , 10 * y , 5 * CHAR_width_small, 7, COLOR_SYSTEXT );
    if (input_value >= 0 && input_value / 3.51 > 0)
    {
      display.fillRect(CHAR_width_small * x + 4 * CHAR_width_small + 1 + input_value / 3.51, 10 * y + 1, 6 * 5 - 1 - input_value / 3.51, 5, COLOR_BACKGROUND );
      if (input_value >= 1 && input_value / 3.51 > 1)
        display.fillRect(CHAR_width_small * x + 4 * CHAR_width_small + 1, 10 * y + 1, input_value / 3.51 , 5, COLOR_PITCHSMP );
    }
  }
}

void print_small_panbar(uint8_t x, uint8_t y, uint8_t input_value, uint8_t selected_option)
{
  setCursor_textGrid_mini(x, y);
  setModeColor( selected_option);
  if (input_value < 20)
  {
    display.print(F("L"));
    seq_print_formatted_number( 20 - input_value, 2);
  }
  else  if (input_value > 20)
  {
    display.print(F("R"));
    seq_print_formatted_number( input_value - 20, 2);
  }
  else
  {
    display.print(F("C"));
    seq_print_formatted_number( input_value - 20, 2);
  }
  display.drawRect(CHAR_width_small * x + 4 * CHAR_width_small , 10 * y, 5 * CHAR_width_small, 7, COLOR_SYSTEXT );
  display.fillRect(CHAR_width_small * x + 4 * CHAR_width_small + 1 , 10 * y + 1, 5 * CHAR_width_small - 2, 7 - 2, COLOR_BACKGROUND );
  display.fillRect(CHAR_width_small * x + 4 * CHAR_width_small + 1 + input_value / 1.60 , 10 * y + 1, 3 , 5, COLOR_PITCHSMP );
}

void colors_screen_update()
{
  int y = CHAR_height;

  if ( seq.temp_select_menu == 2)
  {
    display.fillRect(0, 0, 151, DISPLAY_HEIGHT, COLOR_BACKGROUND );
    colors_screen_update_text_preview();
  }
  do
  {
    display.drawLine ( 355, y , 395, y, ColorHSV( ts.temp_col_hue , (y - CHAR_height) * 0.95, 245)  );
    display.drawLine ( 440, y , DISPLAY_WIDTH, y, ColorHSV( ts.temp_col_hue , 245, (y - CHAR_height) * 0.95)   );
    y++;
  } while (y < TFT_WIDTH - CHAR_height * 2 );

  display.fillRect(CHAR_width * 9, CHAR_height, 30, CHAR_height, ColorHSV( ts.temp_col_hue, ts.temp_col_sat, ts.temp_col_bright) );
  display.setTextSize(1);

  if (seq.temp_select_menu == 0)
  {
    COLOR_SYSTEXT = ColorHSV( ts.temp_col_hue, ts.temp_col_sat, ts.temp_col_bright) ;
    display.setTextColor(COLOR_SYSTEXT, COLOR_BACKGROUND);
    setCursor_textGrid(1, 3);
    display.print(F("SYSTEM TEXT"));
  }
  else if (seq.temp_select_menu == 1)
  {
    COLOR_SYSTEXT_ACCENT = ColorHSV( ts.temp_col_hue, ts.temp_col_sat, ts.temp_col_bright) ;
    display.setTextColor(COLOR_SYSTEXT_ACCENT, COLOR_BACKGROUND);
    setCursor_textGrid(1, 4);
    display.print(F("SYSTEM ACCENT"));
    helptext_l("BACK");
  }
  else if (seq.temp_select_menu == 2)
  {
    COLOR_BACKGROUND = ColorHSV( ts.temp_col_hue, ts.temp_col_sat, ts.temp_col_bright) ;
    display.fillRect(CHAR_width * 9, CHAR_height, 30, CHAR_height, COLOR_BACKGROUND);
    display.setTextColor(COLOR_BACKGROUND, COLOR_SYSTEXT);
    setCursor_textGrid(1, 5);
    display.print(F("SYS BACKGRND"));
  }
  else if (seq.temp_select_menu == 3)
  {
    COLOR_DRUMS = ColorHSV( ts.temp_col_hue, ts.temp_col_sat, ts.temp_col_bright) ;
    display.setTextColor(COLOR_DRUMS, COLOR_BACKGROUND);
    setCursor_textGrid(1, 6);
    display.print(F("DRUMS"));
    display.setTextColor(COLOR_BACKGROUND, COLOR_DRUMS);
    setCursor_textGrid(7, 6);
    display.print(F("DRUMS"));
  }
  else if (seq.temp_select_menu == 4)
  {
    COLOR_PITCHSMP = ColorHSV( ts.temp_col_hue, ts.temp_col_sat, ts.temp_col_bright) ;
    display.setTextColor(COLOR_PITCHSMP, COLOR_BACKGROUND);
    setCursor_textGrid(1, 7);
    display.print(F("PITCH SMP"));
    display.setTextColor(COLOR_BACKGROUND, COLOR_PITCHSMP);
    setCursor_textGrid(7, 7);
    display.print(F("PITCH SMP"));
  }
  else if (seq.temp_select_menu == 5)
  {
    COLOR_CHORDS = ColorHSV( ts.temp_col_hue , ts.temp_col_sat, ts.temp_col_bright) ;
    display.setTextColor(COLOR_CHORDS, COLOR_BACKGROUND);
    setCursor_textGrid(1, 8);
    display.print(F("CHORD/ARP"));
    display.setTextColor(COLOR_BACKGROUND, COLOR_CHORDS);
    setCursor_textGrid(7, 8);
    display.print(F("CHORD/ARP"));
  }
  else if (seq.temp_select_menu == 6)
  {
    COLOR_INSTR = ColorHSV( ts.temp_col_hue, ts.temp_col_sat, ts.temp_col_bright) ;
    display.setTextColor(COLOR_INSTR, COLOR_BACKGROUND);
    setCursor_textGrid(1, 9);
    display.print(F("INSTR"));
    display.setTextColor(COLOR_BACKGROUND, COLOR_INSTR);
    setCursor_textGrid(7, 9);
    display.print(F("INSTR"));
  }
}

void UI_func_colors(uint8_t param)
{
  if (LCDML.FUNC_setup())         // ****** SETUP *********
  {
    encoderDir[ENC_R].reset();
    seq.temp_active_menu = 0;
    display.fillScreen(COLOR_BACKGROUND);
    display.setTextSize(1);
    display.fillRect(CHAR_width * 9, CHAR_height, 30, CHAR_height, COLOR_SYSTEXT );
    helptext_r("SELECT COLOR");
    colors_screen_update_text_preview();
    display.setTextSize(1);
    display.setTextColor(COLOR_SYSTEXT, GREY4);
    display.setCursor(170, TFT_WIDTH - CHAR_height * 2 + 3);
    display.print(F("HUE"));
    display.setCursor(226, TFT_WIDTH - CHAR_height * 2 + 3);
    display.print(F("SAT"));
    display.setCursor(283, TFT_WIDTH - CHAR_height * 2 + 3);
    display.print(F("BRIGHT"));
    display.setTextSize(2);
    int y = CHAR_height;
    do
    {
      display.drawLine ( 170, y , 206, y,  ColorHSV( (y - CHAR_height) * 1.44 , 235, 235)   );
      display.drawLine ( 226, y , 263, y,  ColorHSV( 1 , 235, (y - CHAR_height) * 0.95) );
      display.drawLine ( 283, y , DISPLAY_WIDTH, y,  ColorHSV( 1 , (y - CHAR_height) * 0.95, 235) );
      y++;
    } while (y < TFT_WIDTH - CHAR_height * 2 );

  }
  if (LCDML.FUNC_loop())          // ****** LOOP *********
  {
    if ((LCDML.BT_checkDown() && encoderDir[ENC_R].Down()) || (LCDML.BT_checkUp() && encoderDir[ENC_R].Up()) || LCDML.BT_checkEnter())
    {
      if (LCDML.BT_checkDown())
      {
        if (seq.temp_active_menu == 0)
        {
          if (seq.temp_select_menu == 6)
            seq.temp_select_menu = 0;
          else
            seq.temp_select_menu = constrain(seq.temp_select_menu + ENCODER[ENC_R].speed(), 0, 6);
        }
        else  if (seq.temp_active_menu == 1)
          ts.temp_col_hue = constrain(ts.temp_col_hue + ENCODER[ENC_R].speed(), 0, 359);
        else  if (seq.temp_active_menu == 2)
          ts.temp_col_sat = constrain(ts.temp_col_sat + ENCODER[ENC_R].speed(), 0, 254);
        else  if (seq.temp_active_menu == 3)
          ts.temp_col_bright = constrain(ts.temp_col_bright + ENCODER[ENC_R].speed(), 0, 254);
      }
      else if (LCDML.BT_checkUp())
      {
        if (seq.temp_active_menu == 0)
        {
          seq.temp_select_menu = constrain(seq.temp_select_menu - ENCODER[ENC_R].speed(), 0, 6);
        }
        else  if (seq.temp_active_menu == 1)
          ts.temp_col_hue = constrain(ts.temp_col_hue - ENCODER[ENC_R].speed(), 0, 359);
        else  if (seq.temp_active_menu == 2)
          ts.temp_col_sat = constrain(ts.temp_col_sat - ENCODER[ENC_R].speed(), 0, 254);
        else  if (seq.temp_active_menu == 3)
          ts.temp_col_bright = constrain(ts.temp_col_bright - ENCODER[ENC_R].speed(), 0, 254);
      }
      if (LCDML.BT_checkEnter())
      {
        seq.temp_active_menu++;
        if (seq.temp_active_menu > 3)
          seq.temp_active_menu = 0;
      }
      if (seq.temp_active_menu > 0)
      {
        colors_screen_update();
        if (seq.temp_active_menu == 1)
          helptext_r("ADJUST HUE");
        else if (seq.temp_active_menu == 2)
          helptext_r("ADJUST SATURATION");
        else  if (seq.temp_active_menu == 3)
          helptext_r("ADJUST BRIGHTNESS");
      }
      else if (seq.temp_active_menu == 0)
      {
        helptext_r("SELECT COLOR");
      }
    }
    uint8_t y = 0;
    if (seq.temp_active_menu == 0)
    {
      do
      {
        if (y == seq.temp_select_menu)
          display.drawRect ( 7, 45 + y * CHAR_height + 2, 137, CHAR_height - 1, COLOR_SYSTEXT_ACCENT );
        else
          display.drawRect ( 7, 45 + y * CHAR_height + 2, 137, CHAR_height - 1, COLOR_BACKGROUND  );
        y++;
      } while (y < 7 );
    }
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

    setCursor_textGrid(1, 1);
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

    setCursor_textGrid(1, 2);
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
    setCursor_textGrid(1, 1);
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
    setCursor_textGrid(1, 2);
    if (configuration.dexed[selected_instance_id].midi_channel == 0)
    {
      display.setTextColor(COLOR_SYSTEXT, COLOR_BACKGROUND);
      display.print(F("[OMNI]"));
    }
    else
    {
      display.setTextColor(COLOR_SYSTEXT, COLOR_BACKGROUND);
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
    sprintf(noteName, "L");
  else
  {
    noteNumber -= 21;
    if ( notes[noteNumber % 12][1] == '\0')
      sprintf(noteName, "%1s-%1d", notes[noteNumber % 12], oct_index / 12);
    else
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
    setCursor_textGrid(1, 1);
    display.print(F("Lowest Note"));
    setCursor_textGrid(1, 2);
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
    setCursor_textGrid(2, 2);
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
    setCursor_textGrid(1, 1);
    display.print(F("Highest Note"));
    setCursor_textGrid(1, 2);
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
    setCursor_textGrid(2, 2);
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
      setCursor_textGrid(1, 1);
      display.print(F("Panorama"));
      setCursor_textGrid(1, 2);
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
    setCursor_textGrid(1, 1);
    display.print(F("Favorites"));
    setCursor_textGrid(1, 2);
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

    setCursor_textGrid(1, 2);
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

    setCursor_textGrid(1, 1);
    display.print(F("Stereo/Mono"));
    setCursor_textGrid(1, 2);
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

    setCursor_textGrid(1, 2);
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

    setCursor_textGrid(1, 1);
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

  setCursor_textGrid(1, 2);
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

    setCursor_textGrid(1, 1);
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

    setCursor_textGrid(1, 2);
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

    setCursor_textGrid(1, 1);
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

    setCursor_textGrid(1, 2);
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

    setCursor_textGrid(1, 1);
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

    setCursor_textGrid(1, 2);
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

    setCursor_textGrid(1, 1);
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

    setCursor_textGrid(1, 2);
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

    setCursor_textGrid(1, 1);
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

    setCursor_textGrid(1, 2);
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

    setCursor_textGrid(1, 1);
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

    setCursor_textGrid(1, 2);
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

    setCursor_textGrid(1, 1);
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

    setCursor_textGrid(1, 2);
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

    setCursor_textGrid(1, 1);
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

    setCursor_textGrid(1, 2);
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

    setCursor_textGrid(1, 1);
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

    setCursor_textGrid(1, 2);
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

    setCursor_textGrid(1, 1);
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

    setCursor_textGrid(1, 2);
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

    setCursor_textGrid(1, 1);
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

    setCursor_textGrid(1, 2);
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

    setCursor_textGrid(1, 1);
    display.print(F("OP Enable"));
    setCursor_textGrid(1, 2);
    for (uint8_t i = 2; i < 8; i++)
      display.print(i);

    UI_update_instance_icons();

    setCursor_textGrid(op_selected, 1);
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
    setCursor_textGrid(op_selected, 1);
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
    setCursor_textGrid(1, 1);
    display.print(F("Drum Rev. Send"));
    setCursor_textGrid(2, 2);
    sprintf(displayname, "%02d", activesample);
    display.print(displayname);
    show(2, 5, 7, basename(drum_config[activesample].name));
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
      setCursor_textGrid(12, 2);
      display.print(" ");
      setCursor_textGrid(16, 2);
      display.print(" ");
      setCursor_textGrid(1, 2);
      display.print("[");
      setCursor_textGrid(4, 2);
      display.print("]");
      setCursor_textGrid(2, 2);
      sprintf(displayname, "%02d", activesample);
      display.print(displayname);
      show(2, 5, 7, basename(drum_config[activesample].name));
      sprintf(displayname, "%03d", (int)(drum_config[activesample].reverb_send * 100) );
      setCursor_textGrid(13, 2);
      display.print(displayname);
    } else {
      setCursor_textGrid(1, 2);
      display.print(" ");
      setCursor_textGrid(4, 2);
      display.print(" ");
      setCursor_textGrid(12, 2);
      display.print("[");
      setCursor_textGrid(16, 2);
      display.print("]");
      sprintf(displayname, "%03d", temp_int);
      setCursor_textGrid(13, 2);
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
    setCursor_textGrid(1, 1);
    display.print(F("MIDI Channel"));
  }
  if (LCDML.FUNC_loop())          // ****** LOOP *********
  {
    if (LCDML.BT_checkDown() && encoderDir[ENC_R].Down())
      drum_midi_channel = constrain(drum_midi_channel + ENCODER[ENC_R].speed(), MIDI_CHANNEL_MIN, MIDI_CHANNEL_MAX);
    else if (LCDML.BT_checkUp() && encoderDir[ENC_R].Up())
      drum_midi_channel = constrain(drum_midi_channel - ENCODER[ENC_R].speed(), MIDI_CHANNEL_MIN, MIDI_CHANNEL_MAX);

    setCursor_textGrid(1, 2);
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
  if (LCDML.FUNC_setup())         // ****** SETUP *********
  {
    encoderDir[ENC_R].reset();
    temp_int = mapfloat(seq.drums_volume, 0.0, VOL_MAX_FLOAT, 0, 100);
    display_bar_int("Drums Volume", temp_int, 1.0, SOUND_INTENSITY_MIN, SOUND_INTENSITY_MAX, 3, false, false, true);
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
    display_bar_int("Drums Volume", temp_int, 1.0, SOUND_INTENSITY_MIN, SOUND_INTENSITY_MAX, 3, false, false, false);
    master_mixer_r.gain (3, volume_transform(mapfloat(temp_int, 0, 100, 0.0, VOL_MAX_FLOAT)));
    master_mixer_l.gain (3, volume_transform(mapfloat(temp_int, 0, 100, 0.0, VOL_MAX_FLOAT)));
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
    setCursor_textGrid(1, 1);
    display.print(F("DrumSmp. Tune"));
    setCursor_textGrid(2, 2);
    sprintf(displayname, "%02d", activesample);
    display.print(displayname);
    show(2, 5, 8, basename(drum_config[activesample].name));
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
      setCursor_textGrid(13, 2);
      display.print(" ");
      setCursor_textGrid(17, 2);
      display.print(" ");
      setCursor_textGrid(1, 2);
      display.print("[");
      setCursor_textGrid(4, 2);
      display.print("]");
      setCursor_textGrid(2, 2);
      sprintf(displayname, "%02d", activesample);
      display.print(displayname);
      show(2, 5, 8, basename(drum_config[activesample].name));
      sprintf(displayname, "%03d", (int)(drum_config[activesample].p_offset * 200) );
      setCursor_textGrid(14, 2);
      display.print(displayname);
    } else {
      temp_float = mapfloat(temp_int, 0, 400, 0.0, 2.0);
      setCursor_textGrid(1, 2);
      display.print(" ");
      setCursor_textGrid(4, 2);
      display.print(" ");
      setCursor_textGrid(13, 2);
      display.print("[");
      setCursor_textGrid(17, 2);
      display.print("]");
      sprintf(displayname, "%03d", temp_int);
      setCursor_textGrid(14, 2);
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
    setCursor_textGrid(1, 1);
    display.print(F("DrumSmp. Pitch"));
    setCursor_textGrid(2, 2);
    sprintf(displayname, "%02d", activesample);
    display.print(displayname);
    show(2, 5, 8, basename(drum_config[activesample].name));
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
      setCursor_textGrid(13, 2);
      display.print(" ");
      setCursor_textGrid(17, 2);
      display.print(" ");
      setCursor_textGrid(1, 2);
      display.print("[");
      setCursor_textGrid(4, 2);
      display.print("]");
      setCursor_textGrid(2, 2);
      sprintf(displayname, "%02d", activesample);
      display.print(displayname);
      show(2, 5, 8, basename(drum_config[activesample].name));
      sprintf(displayname, "%03d", (int)(drum_config[activesample].pitch * 200) );
      setCursor_textGrid(14, 2);
      display.print(displayname);
    } else {
      temp_float = mapfloat(temp_int, 0, 400, 0.0, 2.0);
      setCursor_textGrid(1, 2);
      display.print(" ");
      setCursor_textGrid(4, 2);
      display.print(" ");
      setCursor_textGrid(13, 2);
      display.print("[");
      setCursor_textGrid(17, 2);
      display.print("]");
      sprintf(displayname, "%03d", temp_int);
      setCursor_textGrid(14, 2);
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
  display.setTextSize(1);
  display.setTextColor(COLOR_SYSTEXT, COLOR_BACKGROUND);
  for (uint8_t y = 0; y < 14; y++)
  {
    display.setCursor( 1 * CHAR_width_small, (y + 6) * 12);
    display.setTextColor(COLOR_SYSTEXT, COLOR_BACKGROUND);
    seq_print_formatted_number((y + 1), 2); //entry no.
    if (custom_midi_map[y].type == 0)
    {
      display.setTextColor(GREY2, COLOR_BACKGROUND);
      show_small_font((y + 6) * 12, 5 * CHAR_width_small, 5,  "NONE"  );
    }
    else if (custom_midi_map[y].type == 1)
    {
      display.setTextColor(COLOR_PITCHSMP, COLOR_BACKGROUND);
      show_small_font((y + 6) * 12, 5 * CHAR_width_small, 7, "KEY/PAD" );
    }
    else if (custom_midi_map[y].type == 2)
    {
      display.setTextColor(COLOR_PITCHSMP, COLOR_BACKGROUND);
      show_small_font((y + 6) * 12, 5 * CHAR_width_small, 7, "MIDI CC" );
    }
    display.setTextColor(COLOR_DRUMS, COLOR_BACKGROUND);
    show_small_font((y + 6) * 12, 14 * CHAR_width_small, 3,  custom_midi_map[y].in   );
    display.setTextColor(COLOR_CHORDS, COLOR_BACKGROUND);
    show_small_font((y + 6) * 12, 19 * CHAR_width_small, 3, custom_midi_map[y].out );
    display.setTextColor(COLOR_INSTR, COLOR_BACKGROUND);
    show_small_font((y + 6) * 12, 24 * CHAR_width_small, 3, custom_midi_map[y].channel );
    display.setTextColor(GREY2, COLOR_BACKGROUND);
    if (custom_midi_map[y].in == 0)
      show_small_font((y + 6) * 12, 29 * CHAR_width_small, 12, "EMPTY SLOT");
    else if (custom_midi_map[y].type == 1)
    {
      display.setTextColor(PINK, COLOR_BACKGROUND);
      show_small_font((y + 6) * 12, 29 * CHAR_width_small, 13, find_long_drum_name_from_note(custom_midi_map[y].out) );
    }
    else if (custom_midi_map[y].type == 2)
    {
      display.setTextColor(COLOR_INSTR, COLOR_BACKGROUND);
      for (uint8_t i = 0; i < sizeof(cc_dest_values); i++)
      {
        if (custom_midi_map[y].out == cc_dest_values[i])
          show_small_font((y + 6) * 12, 29 * CHAR_width_small, 13, cc_names[i] );
      }
    }
  }
}

void UI_func_custom_mappings(uint8_t param)
{
  char displayname[8] = {0, 0, 0, 0, 0, 0, 0};
  display.setTextSize(2);
  if (LCDML.FUNC_setup())         // ****** SETUP *********
  {
    encoderDir[ENC_R].reset();
    display.fillScreen(COLOR_BACKGROUND);
    display.setTextColor(COLOR_SYSTEXT, COLOR_BACKGROUND);
    setCursor_textGrid_mini(1, 1);
    display.print(F("CUSTOM MAPPINGS"));
    display.setTextSize(1);
    draw_button_on_grid(36, 1, "PREV.", "", 0);
    drawBitmap(CHAR_width_small * 38 + 4, CHAR_height * 1 + 8 , special_chars[19], 8, 8, GREEN);
    draw_button_on_grid(45, 1, "MIDI",  "LEARN", 0);
    setCursor_textGrid_mini(1, 5);
    display.setTextColor(COLOR_ARP, COLOR_BACKGROUND);
    display.print(F("SET YOUR MIDI DEVICE TO DRUM CH. "));
    display.setTextColor(GREY2, COLOR_BACKGROUND);
    display.print(F("["));
    display.setTextColor(COLOR_SYSTEXT, COLOR_BACKGROUND);
    display.print(DRUM_MIDI_CHANNEL);
    display.setTextColor(GREY2, COLOR_BACKGROUND);
    display.print(F("]"));
    display.setTextColor(COLOR_ARP, COLOR_BACKGROUND);
    display.print(F(" TO LEARN "));

    //scrollbar - not implemented, yet
    display.fillRect (DISPLAY_WIDTH - 4 - CHAR_width_small * 3, 9 * CHAR_height_small, CHAR_width_small * 2, 13 * 12 + 6, COLOR_SYSTEXT);
    display.fillRect (DISPLAY_WIDTH - 4 - CHAR_width_small * 3 + 1, 9 * CHAR_height_small + 1, CHAR_width_small * 2 - 2, 6 * 12, GREY2);

    display.setTextSize(1);
    display.setTextColor(COLOR_SYSTEXT, COLOR_BACKGROUND);
    setCursor_textGrid_mini(1, 6);
    display.print(F("NO  TYPE     IN   OUT  CH.  NAME"));
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
    setCursor_textGrid_mini(1, 3);
    display.print("[");
    setCursor_textGrid_mini(7, 3);
    display.print("]");
    setCursor_textGrid_mini(3, 3);

    sprintf(displayname, "%02d", activesample);
    display.print(displayname);
    display.setTextColor(COLOR_PITCHSMP, COLOR_BACKGROUND);
    show_small_font(4 * CHAR_height_small - 2, 9 * CHAR_width_small, 13, basename(drum_config[activesample].name));
  }
  if (LCDML.FUNC_close())     // ****** STABLE END *********
  {
    seq.midi_learn_active = false;
    display.fillScreen(COLOR_BACKGROUND);
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
    display.setTextSize(1);
    UI_update_instance_icons();
    setCursor_textGrid(1, 1);
    display.print("CUSTOM DEXED CC");
    display.setTextColor(COLOR_SYSTEXT, COLOR_PITCHSMP);
    draw_button_on_grid(45, 1, "MIDI",  "LEARN", 0);
    //scrollbar
    display.fillRect (DISPLAY_WIDTH - 4 - CHAR_width_small * 3, 9 * CHAR_height_small, CHAR_width_small * 2, 13 * 12 + 6, COLOR_SYSTEXT);
    display.fillRect (DISPLAY_WIDTH - 4 - CHAR_width_small * 3 + 1, 9 * CHAR_height_small + 1, CHAR_width_small * 2 - 2, 6 * 12, GREY2);
    display.setTextSize(1);
    display.setTextColor(COLOR_SYSTEXT, COLOR_BACKGROUND);
    setCursor_textGrid_mini(1, 6);
    display.print(F("NO  TYPE     IN   OUT  CH.  NAME"));
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
    show(2, 1, 13, cc_names[seq.temp_select_menu] );
  }
  if (LCDML.FUNC_close())     // ****** STABLE END *********
  {
    seq.midi_learn_active = false;
    display.fillScreen(COLOR_BACKGROUND);
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
    setCursor_textGrid(1, 1);
    display.print(F("DrumSmp. Volume"));

    setCursor_textGrid(2, 2);
    sprintf(displayname, "%02d", activesample);
    display.print(displayname);
    show(2, 4, 8, basename(drum_config[activesample].name));

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
      setCursor_textGrid(12, 2);
      display.print(" ");
      setCursor_textGrid(16, 2);
      display.print(" ");
      setCursor_textGrid(1, 2);
      display.print("[");
      setCursor_textGrid(4, 2);
      display.print("]");
      setCursor_textGrid(2, 2);
      sprintf(displayname, "%02d", activesample);
      display.print(displayname);
      show(2, 5, 7, basename(drum_config[activesample].name));
      sprintf(displayname, "%03d", (int)(drum_config[activesample].vol_max * 100) );
      setCursor_textGrid(13, 2);
      display.print(displayname);
    } else {
      temp_float = mapfloat(temp_int, 0, 100, 0.0, 1.0);
      setCursor_textGrid(1, 2);
      display.print(" ");
      setCursor_textGrid(4, 2);
      display.print(" ");
      setCursor_textGrid(12, 2);
      display.print("[");
      setCursor_textGrid(16, 2);
      display.print("]");
      sprintf(displayname, "%03d", temp_int);
      setCursor_textGrid(13, 2);
      display.print(displayname);
      drum_config[activesample].vol_max = temp_float;
    }
  }
  if (LCDML.FUNC_close())     // ****** STABLE END *********
  {
    encoderDir[ENC_R].reset();
  }
}

void UI_func_seq_settings(uint8_t param)
{
  char displayname[4] = {0, 0, 0, 0};
  if (LCDML.FUNC_setup())         // ****** SETUP *********
  {
    display.fillScreen(COLOR_BACKGROUND);
    temp_int = 0;
    menu_select_toggle = false;
    encoderDir[ENC_R].reset();
    display.setTextSize(1);
    display.setTextColor(RED);

    setCursor_textGrid_mini(1, 1);
    display.print("ADV. SEQUENCER SETTINGS");

    display.setTextColor(COLOR_SYSTEXT);
    setCursor_textGrid_mini(1, 4);
    display.setTextColor(GREY2);
    display.print("ARP/CHORD TRANSPOSE");

    display.setTextColor(COLOR_SYSTEXT);
    setCursor_textGrid_mini(1, 6);
    display.print("OCTAVE SHIFT -/+");
    setCursor_textGrid_mini(1, 7);
    display.print("NOTE (INTERVAL) SHIFT");
    setCursor_textGrid_mini(1, 8);
    display.print("MAX. CHORD NOTES");
    setCursor_textGrid_mini(26, 8);
    display.setTextColor(GREY2);
    display.print(F("KEYS"));
    display.setTextColor(COLOR_SYSTEXT);
    setCursor_textGrid_mini(1, 9);
    display.print("MAX. ARP NOTES/STEP");
    setCursor_textGrid_mini(26, 9);
    display.setTextColor(GREY2);
    display.print(F("STEPS"));
    display.setTextColor(COLOR_SYSTEXT);
    setCursor_textGrid_mini(1, 10);
    display.print(F( "CHRD/ARP VELOCITY"));
    setCursor_textGrid_mini(1, 11);
    display.print(F("CHORD INSTRUMENT"));

    display.setTextColor(GREY2);
    setCursor_textGrid_mini(1, 13);
    display.print("LIVE TRANSPOSE");
    display.setTextColor(COLOR_SYSTEXT);
    setCursor_textGrid_mini(1, 15);
    display.print("OCT. FOR LIVE TRANSP.");
    setCursor_textGrid_mini(27, 15);
    display.print("-");

    display.setTextColor(GREY2);
    setCursor_textGrid_mini(1, 17);
    display.print("SONG");
    display.setTextColor(COLOR_SYSTEXT);
    setCursor_textGrid_mini(1, 19);
    display.print("SONG TEMPO");
    setCursor_textGrid_mini(27, 19);
    display.print("BPM");
    display.setTextColor(GREY2);
    setCursor_textGrid_mini(12, 19);
    display.print("(");
    setCursor_textGrid_mini(16, 19);
    display.print("ms)");
  }
  if (LCDML.FUNC_loop())          // ****** LOOP *********
  {
    if (temp_int == 0 && menu_select_toggle)
    {
      if ((LCDML.BT_checkDown() && encoderDir[ENC_R].Down()) || (LCDML.BT_checkUp() && encoderDir[ENC_R].Up()) )
      {
        if (LCDML.BT_checkDown())
          seq.oct_shift = constrain(seq.oct_shift + ENCODER[ENC_R].speed(), -2, 2);
        else if (LCDML.BT_checkUp())
          seq.oct_shift = constrain(seq.oct_shift - ENCODER[ENC_R].speed(), -2, 2);
      }
    }
    else if (temp_int == 1 && menu_select_toggle)
    {
      if ((LCDML.BT_checkDown() && encoderDir[ENC_R].Down()) || (LCDML.BT_checkUp() && encoderDir[ENC_R].Up()) )
      {
        if (LCDML.BT_checkDown())
          seq.element_shift = constrain(seq.element_shift + ENCODER[ENC_R].speed(), 0, 6);
        else if (LCDML.BT_checkUp())
          seq.element_shift = constrain(seq.element_shift - ENCODER[ENC_R].speed(), 0, 6);
      }
    }
    else if (temp_int == 2 && menu_select_toggle)
    {
      if ((LCDML.BT_checkDown() && encoderDir[ENC_R].Down()) || (LCDML.BT_checkUp() && encoderDir[ENC_R].Up()) )
      {
        if (LCDML.BT_checkDown())
          seq.chord_key_ammount = constrain(seq.chord_key_ammount + ENCODER[ENC_R].speed(), 1, 7);
        else if (LCDML.BT_checkUp())
          seq.chord_key_ammount = constrain(seq.chord_key_ammount - ENCODER[ENC_R].speed(), 1, 7);
      }
    }
    else if (temp_int == 3 && menu_select_toggle) // edit max arp notes in 1/32 1/64 arps
    {
      if ((LCDML.BT_checkDown() && encoderDir[ENC_R].Down()) || (LCDML.BT_checkUp() && encoderDir[ENC_R].Up()) )
      {
        if (LCDML.BT_checkDown())
          seq.arp_num_notes_max = constrain(seq.arp_num_notes_max + ENCODER[ENC_R].speed(), 1, 32);
        else if (LCDML.BT_checkUp())
          seq.arp_num_notes_max = constrain(seq.arp_num_notes_max - ENCODER[ENC_R].speed(), 1, 32);
      }
    }
    else if (temp_int == 4 && menu_select_toggle) // Chord Velocity
    {
      if ((LCDML.BT_checkDown() && encoderDir[ENC_R].Down()) || (LCDML.BT_checkUp() && encoderDir[ENC_R].Up()) )
      {
        if (LCDML.BT_checkDown())
          seq.chord_vel = constrain(seq.chord_vel + 1, 10, 120);
        else if (LCDML.BT_checkUp())
          seq.chord_vel = constrain(seq.chord_vel - 1, 10, 120);
      }
    }
    else if (temp_int == 5 && menu_select_toggle) // edit default chord instrument
    {
      if ((LCDML.BT_checkDown() && encoderDir[ENC_R].Down()) || (LCDML.BT_checkUp() && encoderDir[ENC_R].Up()))
      {
        if (LCDML.BT_checkDown())
          seq.chord_dexed_inst = constrain(seq.chord_dexed_inst + 1, 0, 4);
        else if (LCDML.BT_checkUp())
          seq.chord_dexed_inst = constrain(seq.chord_dexed_inst - 1, 0, 4);
      }
    }
    else if (temp_int == 6 && menu_select_toggle)
    {
      if ((LCDML.BT_checkDown() && encoderDir[ENC_R].Down()) || (LCDML.BT_checkUp() && encoderDir[ENC_R].Up() ) )
      {
        if (LCDML.BT_checkDown())
          seq.transpose = constrain(seq.transpose + ENCODER[ENC_R].speed(), 24, 60);
        else if (LCDML.BT_checkUp())
          seq.transpose = constrain(seq.transpose - ENCODER[ENC_R].speed(), 24, 60);
      }
    }
    else if (temp_int == 7 && menu_select_toggle)
    {
      if ((LCDML.BT_checkDown() && encoderDir[ENC_R].Down()) || (LCDML.BT_checkUp() && encoderDir[ENC_R].Up()) )
      {
        if (LCDML.BT_checkDown())
          seq.bpm = constrain(seq.bpm + ENCODER[ENC_R].speed(), 40, 190);
        else if (LCDML.BT_checkUp())
          seq.bpm = constrain(seq.bpm - ENCODER[ENC_R].speed(), 40, 190);
      }
    }
    // -------------------------------------------------------------------------------------------------------------------------
    else if ( menu_select_toggle == false)
    {
      if ((LCDML.BT_checkDown() && encoderDir[ENC_R].Down()) || (LCDML.BT_checkUp() && encoderDir[ENC_R].Up()) )
      {
        if (LCDML.BT_checkDown())
          temp_int = constrain(temp_int + ENCODER[ENC_R].speed(), 0, 7);
        else if (LCDML.BT_checkUp())
          temp_int = constrain(temp_int - ENCODER[ENC_R].speed(), 0, 7);
      }
    }
    if (LCDML.BT_checkEnter())
    {
      menu_select_toggle = !menu_select_toggle;
    }
    if (temp_int == 0)
      display.setTextColor(COLOR_BACKGROUND, COLOR_SYSTEXT); else display.setTextColor(COLOR_SYSTEXT, COLOR_BACKGROUND);
    setCursor_textGrid_mini(23, 6);
    sprintf(displayname, "%02d", seq.oct_shift);
    display.print(displayname);
    if (temp_int == 1)
      display.setTextColor(COLOR_BACKGROUND, COLOR_SYSTEXT); else display.setTextColor(COLOR_SYSTEXT, COLOR_BACKGROUND);
    setCursor_textGrid_mini(23, 7);
    display.print(seq.element_shift);
    if (temp_int == 2)
      display.setTextColor(COLOR_BACKGROUND, COLOR_SYSTEXT); else display.setTextColor(COLOR_SYSTEXT, COLOR_BACKGROUND);
    setCursor_textGrid_mini(23, 8);
    sprintf(displayname, "%02d", seq.chord_key_ammount);
    display.print(displayname);
    if (temp_int == 3)
      display.setTextColor(COLOR_BACKGROUND, COLOR_SYSTEXT); else display.setTextColor(COLOR_SYSTEXT, COLOR_BACKGROUND);
    setCursor_textGrid_mini(23, 9);
    sprintf(displayname, "%02d", seq.arp_num_notes_max);
    display.print(displayname);
    if (temp_int == 4)
      display.setTextColor(COLOR_BACKGROUND, COLOR_SYSTEXT); else display.setTextColor(COLOR_SYSTEXT, COLOR_BACKGROUND);
    setCursor_textGrid_mini(23, 10);
    seq_print_formatted_number(seq.chord_vel, 3);
    if (temp_int == 5)
      display.setTextColor(COLOR_BACKGROUND, COLOR_SYSTEXT); else display.setTextColor(COLOR_SYSTEXT, COLOR_BACKGROUND);
    setCursor_textGrid_mini(23, 11);
    if (seq.chord_dexed_inst == 0)
      display.print(F( "DEXED1"));
    else if (seq.chord_dexed_inst == 1)
      display.print(F( "DEXED2"));
    else if (seq.chord_dexed_inst == 2)
      display.print(F( "EPIANO"));
    else if (seq.chord_dexed_inst == 3)
      display.print(F( "SYNTH1"));
    else if (seq.chord_dexed_inst == 4)
      display.print(F( "SYNTH2"));
    if (temp_int == 6)
      display.setTextColor(COLOR_BACKGROUND, COLOR_SYSTEXT); else display.setTextColor(COLOR_SYSTEXT, COLOR_BACKGROUND);
    setCursor_textGrid_mini(23, 15);
    if (seq.transpose != 0)
    {
      getNoteName(displayname, seq.transpose);
      display.print(displayname);
      setCursor_textGrid_mini(29, 15);
      getNoteName(displayname, seq.transpose + 12);
      display.print(displayname);
    }
    else
    {
      display.print(F("OFF"));
      setCursor_textGrid_mini(29, 15);
      display.print(F("OFF"));
    }


    display.setTextColor(GREY2, COLOR_BACKGROUND);
    setCursor_textGrid_mini(13, 19);
    sprintf(displayname, "%03d", seq.tempo_ms / 1000);
    display.print(displayname);
    if (temp_int == 7)
      display.setTextColor(COLOR_BACKGROUND, COLOR_SYSTEXT); else display.setTextColor(COLOR_SYSTEXT, COLOR_BACKGROUND);
    setCursor_textGrid_mini(23, 19);
    sprintf(displayname, "%03d", seq.bpm);
    display.print(displayname);
    seq.tempo_ms = 60000000 / seq.bpm / 4;

    if (temp_int == 7)
    {
      for (uint8_t i = 0; i < MAX_DEXED; i++)
      {
        if (configuration.fx.delay_sync[i] > 0)
        {
          uint16_t midi_sync_delay_time = uint16_t(60000.0 *  midi_ticks_factor[configuration.fx.delay_sync[i]] / seq.bpm);
          delay_fx[i]->delay(0, constrain(midi_sync_delay_time, DELAY_TIME_MIN, DELAY_TIME_MAX * 10));
        }
      }
      if (seq.running)
        sequencer_timer.begin(sequencer, seq.tempo_ms / 8);
      else
        sequencer_timer.begin(sequencer, seq.tempo_ms / 8, false);
    }
  }
  if (LCDML.FUNC_close())     // ****** STABLE END *********
  {
    encoderDir[ENC_R].reset();
    display.fillScreen(COLOR_BACKGROUND);
  }
}

void UI_func_drum_pan(uint8_t param)
{
  char displayname[8] = {0, 0, 0, 0, 0, 0, 0};
  if (LCDML.FUNC_setup())         // ****** SETUP *********
  {
    encoderDir[ENC_R].reset();
    temp_int = mapfloat(drum_config[activesample].pan, -1.0, 1.0, -99, 99);
    setCursor_textGrid(1, 1);
    display.print(F("DrmSmp. Panorama"));
    setCursor_textGrid(2, 2);
    sprintf(displayname, "%02d", activesample);
    display.print(displayname);
    show(2, 5, 6, basename(drum_config[activesample].name));
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
      setCursor_textGrid(12, 2);
      display.print(" ");
      setCursor_textGrid(16, 2);
      display.print(" ");
      setCursor_textGrid(1, 2);
      display.print("[");
      setCursor_textGrid(4, 2);
      display.print("]");
      setCursor_textGrid(2, 2);
      sprintf(displayname, "%02d", activesample);
      display.print(displayname);
      show(2, 5, 6, basename(drum_config[activesample].name));
    } else {
      setCursor_textGrid(1, 2);
      display.print(" ");
      setCursor_textGrid(4, 2);
      display.print(" ");
      setCursor_textGrid(12, 2);
      display.print("[");
      setCursor_textGrid(16, 2);
      display.print("]");
      drum_config[activesample].pan = mapfloat(temp_int, -99, 99, -1.0, 1.0);
    }
    temp_int = mapfloat(drum_config[activesample].pan, -1.0, 1.0, -99, 99);
    setCursor_textGrid(13, 2);
    if (temp_int > 1) {
      display.print("R");
    } else if (temp_int < 0) {
      display.print("L");
    }
    else {
      display.print("C");
    }
    sprintf(displayname, "%02d", abs(temp_int));
    setCursor_textGrid(14, 2);
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
    else if (seq.content_type[pattern] == 2 ) //  chord  pattern
      display.setTextColor(COLOR_CHORDS, DARKGREEN);
    else if ( seq.content_type[pattern] == 3) //   arp pattern
      display.setTextColor(COLOR_ARP, DARKGREEN);
  }
  else
  {
    if (seq.content_type[pattern] == 0) //Drumpattern
      display.setTextColor(COLOR_DRUMS, COLOR_BACKGROUND);
    else if (seq.content_type[pattern] == 1) //Instrument Pattern
      display.setTextColor(COLOR_INSTR, COLOR_BACKGROUND);
    else if (seq.content_type[pattern] == 2 ) //  chord  pattern
      display.setTextColor(COLOR_CHORDS, COLOR_BACKGROUND);
    else if ( seq.content_type[pattern] == 3) //   arp pattern
      display.setTextColor(COLOR_ARP, COLOR_BACKGROUND);
  }
}

void set_pattern_content_type_color_inverted(uint8_t pattern)
{
  if (seq.content_type[pattern] == 0) //Drumpattern
    display.setTextColor(  COLOR_BACKGROUND, COLOR_DRUMS);
  else if (seq.content_type[pattern] == 1) //Instrument Pattern
    display.setTextColor( COLOR_BACKGROUND, COLOR_INSTR);
  else if (seq.content_type[pattern] == 2 ) //  chord  pattern
    display.setTextColor( COLOR_BACKGROUND, COLOR_CHORDS);
  else if ( seq.content_type[pattern] == 3) //  arp pattern
    display.setTextColor( COLOR_BACKGROUND, COLOR_ARP);
}

void set_track_type_color(uint8_t track)
{
  if ( seq.track_type[track] == 0) //Drums
    display.setTextColor(COLOR_DRUMS, COLOR_BACKGROUND);
  else if ( seq.track_type[track] == 1) //Inst
    display.setTextColor(COLOR_INSTR, COLOR_BACKGROUND);
  else if ( seq.track_type[track] == 2 ) //Chord
    display.setTextColor(COLOR_CHORDS, COLOR_BACKGROUND);
  else if ( seq.track_type[track] == 3 ) // Arp
    display.setTextColor(COLOR_ARP, COLOR_BACKGROUND);
}

void set_track_type_color_inverted(uint8_t track)
{
  if ( seq.track_type[track] == 0) //Drums
    display.setTextColor(COLOR_BACKGROUND, COLOR_DRUMS);
  else if ( seq.track_type[track] == 1) //Inst
    display.setTextColor(COLOR_BACKGROUND, COLOR_INSTR);
  else if ( seq.track_type[track] == 2 ) //Chord
    display.setTextColor(COLOR_BACKGROUND, COLOR_CHORDS);
  else if ( seq.track_type[track] == 3) // Arp
    display.setTextColor(COLOR_BACKGROUND, COLOR_ARP);
}

void print_performance_name(int x, int y)
{
  display.setTextSize(1);
  display.setTextColor(COLOR_PITCHSMP, COLOR_BACKGROUND);
  display.setCursor(CHAR_width_small * 36,   11 * (CHAR_height_small + 2) + 10);
  seq_print_formatted_number(configuration.sys.performance_number, 2);
  display.print( ":");
  display.setTextColor(COLOR_PITCHSMP, COLOR_BACKGROUND);
  display.print( "[");
  display.setTextColor(COLOR_SYSTEXT, COLOR_BACKGROUND);
  display.print(seq.name);
  display.setTextColor(COLOR_PITCHSMP, COLOR_BACKGROUND);
  display.print( "]");
  display.setTextSize(2);
}

void seq_printAllSeqSteps()
{
  setCursor_textGrid(0, 1);
  for (uint8_t i = 0; i < 16; i++)
  {
    if ( seq.vel[seq.active_pattern][i] > 209 )
      display.setTextColor(COLOR_PITCHSMP, COLOR_BACKGROUND);
    else
      set_pattern_content_type_color(seq.active_pattern);
    display.print(seq_find_shortname(i)[0]);
  }
}

void seq_sub_print_track_assignments(int x, int y, bool init)
{
  if (seq.cycle_touch_element == 0)  // touch keyboard is off
  {
    display.setTextSize(1);
    display.setTextColor(GREY2, COLOR_BACKGROUND);
    for (uint8_t track = 0; track < NUM_SEQ_TRACKS; track++)
    {
      display.setCursor(CHAR_width_small * 36,  (track + 13) * (CHAR_height_small + 1) + 20  );
      if (seq.menu - 21 == track)
        set_track_type_color_inverted(track);
      else
        set_track_type_color(track);
      display.print(F("T"));
      display.print (track + 1);
      display.setCursor(CHAR_width_small * 36 + 3 * CHAR_width_small,   (track + 13) * (CHAR_height_small + 1) + 20  );
      if (seq.menu - 21 - NUM_SEQ_TRACKS == track)
        set_track_type_color_inverted(track);
      else
        set_track_type_color(track);
      if (seq.track_type[track] > 0 && seq.instrument[track] < 2)
      {
        display.print ("DEXED INST.");
        display.print (seq.instrument[track] + 1);
      }
      else if (seq.track_type[track] > 0 && seq.instrument[track] == 2) //epiano
      {
        display.print ("ELECTR.PIANO");
      }
      else if (seq.track_type[track] > 0 && seq.instrument[track] == 3)  //MicroSynth 0
      {
        display.print ("MICROSYNTH 1");
      }
      else if (seq.track_type[track] > 0 && seq.instrument[track] == 4)  //MicroSynth 1
      {
        display.print ("MICROSYNTH 2");
      }
      else if (seq.track_type[track] == 0 && seq.instrument[track] < 5) //drums/samples
      {
        display.print ("DRUMS/SAMPLE");
      }
      else if (seq.instrument[track] > 4 && seq.instrument[track] < 21) //external MIDI USB
      {
        display.print ("MIDI USB #");
        seq_print_formatted_number(seq.instrument[track] - 4, 2);
      }
      else if (seq.instrument[track] > 20 && seq.instrument[track] < 37) //external MIDI MINI JACK/DIN
      {
        display.print ("MIDI DIN #");
        seq_print_formatted_number(seq.instrument[track] - 20, 2);
      }
      else
      {
        display.print ("-----------");
      }
    }
    display.setCursor(CHAR_width_small * 36,  21 * (CHAR_height_small + 2) + 10  );
    display.setTextColor(GREY2, COLOR_BACKGROUND);
    display.print ("STEP RECORDING:");
    display.setCursor(CHAR_width_small * 36,  22 * (CHAR_height_small + 2) + 11  );
    display.setTextColor(RED, COLOR_BACKGROUND);
    if (seq.auto_advance_step == 1)
      display.print ("AUTO ADV. STEP");
    else if (seq.auto_advance_step == 2)
      display.print ("AUTO ADV.+STOP");
    else
      display.print ("KEEP CUR. STEP");
    if (init)
      print_performance_name(x , 17 * CHAR_height_small + 5);
    display.setTextSize(2);
  }
}

void seq_printVelGraphBar()
{
  display.fillRect(0, 2 * CHAR_height , 17 * CHAR_width, 17, COLOR_BACKGROUND);
  for (uint8_t i = 0; i < 16; i++)
  {
    if ( seq.vel[seq.active_pattern][i] < 130) {
      display.drawLine (0 + i * CHAR_width + 3 , CHAR_height * 3 - 3  , 0 + i * CHAR_width + 3 , CHAR_height * 3 - 3 - (seq.vel[seq.active_pattern][i] / 10)   , GREY1);
      display.drawLine (0 + i * CHAR_width + 4 , CHAR_height * 3 - 3  , 0 + i * CHAR_width + 4 , CHAR_height * 3 - 3 - (seq.vel[seq.active_pattern][i] / 10)   , GREY1);
      display.drawLine (0 + i * CHAR_width + 5 , CHAR_height * 3 - 3  , 0 + i * CHAR_width + 5 , CHAR_height * 3 - 3 - (seq.vel[seq.active_pattern][i] / 10)   , GREY1);
    }
    else
    {
      if ( seq.vel[seq.active_pattern][i] > 209)
      {
        drawBitmap((i - 1) * CHAR_width + 13, 2 * CHAR_height + 7, special_chars[22], 8, 8, COLOR_PITCHSMP);
      }
    }
  }
}

void seq_printVelGraphBar_single_step(uint8_t step, int color)
{
  display.fillRect(0 + step * CHAR_width + 1 , CHAR_height * 2 + 1  , 8, 14, COLOR_BACKGROUND);
  if ( seq.vel[seq.active_pattern][step] < 130)
  {
    display.drawLine (0 + step * CHAR_width + 3 , CHAR_height * 3 - 3  , 0 + step * CHAR_width + 3 , CHAR_height * 3 - 3 - (seq.vel[seq.active_pattern][step] / 10)   , color);
    display.drawLine (0 + step * CHAR_width + 4 , CHAR_height * 3 - 3  , 0 + step * CHAR_width + 4 , CHAR_height * 3 - 3 - (seq.vel[seq.active_pattern][step] / 10)   , color);
    display.drawLine (0 + step * CHAR_width + 5 , CHAR_height * 3 - 3  , 0 + step * CHAR_width + 5 , CHAR_height * 3 - 3 - (seq.vel[seq.active_pattern][step] / 10)   , color);
  }
  else
  {
    if ( seq.vel[seq.active_pattern][step] > 209)
    {
      drawBitmap((step - 1) * CHAR_width + 13, 2 * CHAR_height + 7, special_chars[22], 8, 8, COLOR_PITCHSMP);
    }
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
  //      show_smallfont_noGrid(5, 353 , 9, drum_config[samplenumber].name);
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
//      show_smallfont_noGrid(5, 419 , 9, drum_config[samplenumber].name);
//      display.setTextColor(COLOR_SYSTEXT, COLOR_BACKGROUND);
//    }
//    seq.last_drawn_sample = samplenumber;
//  }
//  display.setTextSize(2);
//}

#ifdef COMPILE_FOR_PROGMEM
void UI_draw_waveform_large()  // for progmem
{
  int xspace = 0;
  int i = 4;
  short samplevalue = 0;
  int oldx = 0, oldy = 180;
  display.setTextSize(1);
  setCursor_textGrid(21, 3);
  display.setTextColor(COLOR_SYSTEXT, COLOR_PITCHSMP );
  display.print("SCALE");
  setCursor_textGrid(21, 4);
  display.setTextColor(COLOR_SYSTEXT, COLOR_BACKGROUND );
  display.print(wave_spacing);
  display.print (" ");
  display.fillRect(0, 100, DISPLAY_WIDTH - 1, 100, COLOR_BACKGROUND);

  xspace = 0;
  i = 4;
  samplevalue = 0;
  oldx = 0;  oldy = 180;
  do {
    {
      samplevalue = (( (DRUM_SQBass[i * wave_spacing + 1]) * 256) + DRUM_SQBass[i * wave_spacing] )  / 710 + 100;

      if (DRUM_SQBass[i * wave_spacing + 1] < 128)
      {
        display.drawLine(oldx, oldy,  xspace, samplevalue + 60  , COLOR_SYSTEXT );
        oldx = xspace; oldy = samplevalue + 60;
      }
      else
      {
        display.drawLine(oldx, oldy,  xspace, samplevalue  - 40 , COLOR_SYSTEXT );
        oldx = xspace;  oldy = samplevalue - 40;
      }
      xspace = xspace + 1;  i++;
    }
  } while (oldx < DISPLAY_WIDTH);
  wave_spacing_old = wave_spacing;
}
#endif

#ifdef COMPILE_FOR_FLASH
void UI_draw_waveform_large()  // for flash
{
  display.setTextSize(1);
  setCursor_textGrid(21, 3);
  display.setTextColor(COLOR_SYSTEXT, COLOR_PITCHSMP );
  // display.print("SCALE");
  setCursor_textGrid(21, 4);
  display.setTextColor(COLOR_SYSTEXT, COLOR_BACKGROUND );
  display.print(wave_spacing);
  display.print (" ");
  display.fillRect(0, 80, DISPLAY_WIDTH - 1, 100, COLOR_BACKGROUND);
  int xspace = 0;
  int i = 4;
  short samplevalue = 0;
  int oldx = 0, oldy = 180;
  xspace = 0;
  samplevalue = 0;
  oldx = 0;  oldy = 180;
  int block = 0;
  unsigned long count = 0;
  SerialFlashFile f = SerialFlash.open("Casta.wav");
  //f.seek(44);
  unsigned long length = f.size();
  char buf[256];
  unsigned int n;
  while (count * 256 < length)
  {
    n = f.read(buf, 256);
    do {
      {
        samplevalue = (( buf[i + 1] * 256) + buf[i ] )  / 310 + 10;
        if (samplevalue < 128)
        {
          display.drawLine(block + oldx, oldy,  block + xspace, samplevalue + 60  , COLOR_SYSTEXT );
          oldx =  xspace; oldy = samplevalue + 60;
        }
        else
        {
          display.drawLine(block + oldx, oldy,  block + xspace, samplevalue  - 40 , COLOR_SYSTEXT );
          oldx = xspace;  oldy = samplevalue - 40;
        }
        xspace = xspace + 1;
        i = i + 1;
      }
    } while (i < 128);

    block = block + 128;
    count = count + n;
  }
  f.close();


}
#endif

void UI_func_sample_editor(uint8_t param)
{
  char tmp[5];
  if (LCDML.FUNC_setup())         // ****** SETUP *********
  {
    // setup function
    encoderDir[ENC_R].reset();
    display.fillScreen(COLOR_BACKGROUND);
    setCursor_textGrid(1, 1);
    display.setTextColor(COLOR_SYSTEXT, COLOR_PITCHSMP);
    display.print(F("SAMPLE:"));
    setCursor_textGrid(9, 1);
    display.setTextColor(COLOR_SYSTEXT, COLOR_BACKGROUND);
    display.print(F("--"));
    setCursor_textGrid(1, 2);
    display.print("V:");
    sprintf(tmp, "%03d", seq.vel[seq.active_pattern][seq.menu - 1]);
    setCursor_textGrid(3, 2);
    display.print(tmp);
    setCursor_textGrid(21, 0);
    display.print("[");
    setCursor_textGrid(22, 0);
    if (seq.active_pattern < 10)
      display.print("0");
    display.print(seq.active_pattern);
    setCursor_textGrid(24, 0);
    display.print("]");
  }
  if (LCDML.FUNC_loop())          // ****** LOOP *********
  {
    if ((LCDML.BT_checkDown() && encoderDir[ENC_R].Down()) || (LCDML.BT_checkUp() && encoderDir[ENC_R].Up()))
    {
      if (LCDML.BT_checkDown())
        wave_spacing = constrain(wave_spacing + 2, 0, 500);
      else if (LCDML.BT_checkUp())
        wave_spacing = constrain(wave_spacing - 2, 0, 500);
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
  display.setCursor(136 - 12, 71 - 17);

  if (LCDML.FUNC_getID() == LCDML.OTHER_getIDFromFunction(UI_func_seq_vel_editor))
  {

    if (seq.menu < 19)
    {
      display.setTextColor(COLOR_SYSTEXT, PINK);
      display.print(F("VEL./CHORDS"));
    }
    else if (seq.menu == 19)
    {
      display.setTextColor(COLOR_BACKGROUND, RED);
      display.print(F("CONT. TYPE "));
      //      display.setCursor(175 , 71 - 17);
      //      display.setTextColor(DARKGREEN, COLOR_SYSTEXT);
      //      display.print(seq.active_pattern);
      //      display.print(" ");
    }
    else if (seq.menu > 20 && seq.menu < 29)
    {
      display.setTextColor(COLOR_BACKGROUND, COLOR_SYSTEXT);
      display.print(F("TRACK TYPE "));
    }
    else if (seq.menu > 28 && seq.menu < 37)
    {
      display.setTextColor(COLOR_BACKGROUND, DX_DARKCYAN);
      display.print(F("DEXED/EP A."));
    }

  }
  else if (LCDML.FUNC_getID() == LCDML.OTHER_getIDFromFunction(UI_func_seq_pattern_editor))
  {
    display.setTextColor(COLOR_BACKGROUND, MIDDLEGREEN);
    display.print(F("NOTE EDITOR"));
  }
  display.setTextColor(COLOR_SYSTEXT, COLOR_BACKGROUND);
  display.setTextSize(2);
}

void print_track_steps_detailed(int xpos, int ypos, uint8_t currentstep, bool init, bool allsteps)
{
  if (seq.cycle_touch_element == 0)  // touch keyboard is off
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

    //  if (init)
    //  { //only needs to be drawn at first run
    //
    //    display.setTextColor(GREY2, COLOR_BACKGROUND);
    //    display.setCursor(xpos, ypos + 14);
    //    display.print(F("STEP  N   DATA  VEL   NAME / NOTE"));
    //  }

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
      y = ypos + 10 + i * (CHAR_height_small + 2);

      if (init)
      { //only needs to be drawn at first run

        if (i % 4 == 0)
          display.setTextColor(GREY1, COLOR_BACKGROUND);
        else
          display.setTextColor(MIDDLEGREEN, COLOR_BACKGROUND);
        display.setCursor(x, y);
        seq_print_formatted_number (i + 1 , 2);
      }
      // Short Name
      if (i == currentstep)
        display.setTextColor(COLOR_BACKGROUND, COLOR_SYSTEXT);
      else
        display.setTextColor(GREY2, COLOR_BACKGROUND);

      display.setCursor(CHAR_width_small * 4 , y);

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
      display.setCursor(CHAR_width_small * 7 , y);
      seq_print_formatted_number (seq.note_data[seq.active_pattern][i] , 3);

      // Velocity values
      if (i == currentstep)
        display.setTextColor(COLOR_BACKGROUND, COLOR_SYSTEXT);
      else
        display.setTextColor(GREY1, COLOR_BACKGROUND);
      display.setCursor(CHAR_width_small * 12 , y);
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
          show_smallfont_noGrid(y, CHAR_width_small * 17 , 10, basename(drum_config[seq.vel[seq.active_pattern][i] - 210].name) );
        }
        else // else it is a regular Drum Sample
          show_smallfont_noGrid(y, CHAR_width_small * 17 , 10, find_long_drum_name_from_note( seq.note_data[seq.active_pattern][i] ));
      }
      else if (seq.content_type[seq.active_pattern] > 0 ) //Inst Track or Chord or Arp
      {
        display.setCursor(x + CHAR_width_small * 17, y);
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
      while (display.getCursorX() < CHAR_width_small * 32)
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
      if (seq.content_type[seq.active_pattern] == 0)
        seq.note_editor_view = 0;
      else
        seq.note_editor_view = 1;
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
    else if (seq_active_function == 1 && seq.menu == 21 + i + NUM_SEQ_TRACKS) // edit dexed/instrument assign
    {
      if ((LCDML.BT_checkDown() && encoderDir[ENC_R].Down()) || (LCDML.BT_checkUp() && encoderDir[ENC_R].Up()))
      {
        if (LCDML.BT_checkDown())
          seq.instrument[i] = constrain(seq.instrument[i] + 1, 0, 36);
        else if (LCDML.BT_checkUp())
          seq.instrument[i] = constrain(seq.instrument[i] - 1, 0, 36);
      }
    }
    else if (seq_active_function == 1 && seq.menu == 37) // edit auto/manual advance in step recorder
    {
      if ((LCDML.BT_checkDown() && encoderDir[ENC_R].Down()) || (LCDML.BT_checkUp() && encoderDir[ENC_R].Up()))
      {
        if (LCDML.BT_checkDown())
          seq.auto_advance_step = constrain(seq.auto_advance_step + 1, 0, 2);
        else if (LCDML.BT_checkUp())
          seq.auto_advance_step = constrain(seq.auto_advance_step - 1, 0, 2);
      }
    }
  }
}

void UI_func_seq_vel_editor(uint8_t param)
{
  char tmp[5];
  if (LCDML.FUNC_setup())         // ****** SETUP *********
  {
    // setup function

    if (seq.menu_status != 1)
    {
      display.fillScreen(COLOR_BACKGROUND);
      seq_pattern_editor_update_dynamic_elements();
    }

    encoderDir[ENC_R].reset();
    print_edit_mode();
    seq.menu = 1;
    //clear "Insert xyz ?"  message
    display.fillRect(0, 3 * CHAR_height + 17, 212, 8, COLOR_BACKGROUND);
    setCursor_textGrid(14, 0);
    display.setTextColor(GREY2, COLOR_BACKGROUND);
    display.print("[");
    setCursor_textGrid(15, 0);
    seq_print_formatted_number(seq.active_pattern, 2);
    setCursor_textGrid(17, 0);
    display.print("]");
    if (seq.menu_status != 1)
    {
      display.setCursor(0, CHAR_height * 3 + 3);
      display.setTextSize(1);
      display.setTextColor(GREY1, COLOR_BACKGROUND);
      display.print(F("CONT.TYPE:"));
      display.setCursor(11 * CHAR_width_small, CHAR_height * 3 + 3);
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
            seq.menu = constrain(seq.menu + 1, 19, 37);
        }
        else if (LCDML.BT_checkUp())
        {
          if (seq.menu == 19)
          {
            seq.menu = 16;  //return to vel_editor
          }
          else
            seq.menu = constrain(seq.menu - 1, 19, 37);
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
        display.setCursor(0, 3 * CHAR_height + 17);
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

    // if ( seq.content_type[seq.active_pattern] > 1 && seq.vel[seq.active_pattern][seq.menu - 1] < 200)
    //   seq.vel[seq.active_pattern][seq.menu - 1] = 200;

    if (seq_active_function == 0)
    {
      display.setTextSize(1);
      display.setCursor(11 * CHAR_width_small, CHAR_height * 3 + 3);
      print_content_type();
      display.setTextSize(2);
    }
    if (seq.menu == 0)
    {
      display.setTextSize(2);
      display.setTextColor(COLOR_SYSTEXT, COLOR_BACKGROUND);
      setCursor_textGrid(14, 0);
      display.print("[");
      seq_print_formatted_number(seq.active_pattern, 2);
      display.print("]");
      setCursor_textGrid(0, 1);
      seq_printAllSeqSteps();
      seq_printVelGraphBar();
      if (seq.content_type[seq.active_pattern] == 0) //Drum Mode
      {
        if (seq.note_editor_view != 0)
        {
          seq.note_editor_view = 0;
          border3_clear();
          print_track_steps_detailed(0, CHAR_height * 4 + 3, 254, true, true);
        }
        else
          print_track_steps_detailed(0, CHAR_height * 4 + 3, 254, false, true);
      }
      else
      {
        if (seq.note_editor_view != 1)
        {
          seq.note_editor_view = 1;
          border3_clear();
          print_single_pattern_pianoroll_in_pattern_editor(0, DISPLAY_HEIGHT, seq.active_pattern, seq.menu - 3, true);
        }
        else
          print_single_pattern_pianoroll_in_pattern_editor(0, DISPLAY_HEIGHT, seq.active_pattern, seq.menu - 3, false);
      }
    }
    else if (seq.menu == 1)
    {
      display.setTextSize(2);
      setCursor_textGrid(14, 0);
      display.print(" ");
      setCursor_textGrid(17, 0);
      display.print(" ");

      setCursor_textGrid(0, 1);
      display.setTextColor(GREEN, GREY2);
      setCursor_textGrid(0, 1);
      display.print(seq_find_shortname(0)[0] );

      set_pattern_content_type_color(seq.active_pattern);
      setCursor_textGrid(1, 1);
      display.print(seq_find_shortname(1)[0] );
      display.setTextColor(COLOR_SYSTEXT, COLOR_BACKGROUND);

      if (seq.vel[seq.active_pattern][seq.menu - 1] < 210 && seq.content_type[seq.active_pattern] < 2) //it is a normal sample
      {
        sprintf(tmp, "%03d", seq.vel[seq.active_pattern][seq.menu - 1]);
        setCursor_textGrid(1, 0);
        display.print(tmp);
      }
    }
    else if (seq.menu > 1 && seq.menu < 17 )
    {
      display.setTextSize(2);
      set_pattern_content_type_color(seq.active_pattern);
      setCursor_textGrid(seq.menu - 2, 1);
      if (seq.vel[seq.active_pattern][seq.menu - 2] > 209) //if pitched sample, change color
        display.setTextColor(COLOR_PITCHSMP, COLOR_BACKGROUND);
      else
        set_pattern_content_type_color(seq.active_pattern);
      display.print(seq_find_shortname(seq.menu - 2)[0] );

      setCursor_textGrid(seq.menu - 1 , 1);
      display.setTextColor(GREEN, GREY2);
      display.print(seq_find_shortname(seq.menu - 1)[0] );
      set_pattern_content_type_color(seq.active_pattern);

      if (seq.menu < 16)
      {
        setCursor_textGrid(seq.menu + 0, 1);
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
          print_track_steps_detailed(0, CHAR_height * 4 + 3, seq.menu - 1, false, true);
        else
          print_track_steps_detailed(0, CHAR_height * 4 + 3, seq.menu - 1, false, false);
      }
      else
        print_single_pattern_pianoroll_in_pattern_editor(0, DISPLAY_HEIGHT, seq.active_pattern, seq.menu - 1, false);
      setCursor_textGrid(3, 0);
      if (seq.note_data[seq.active_pattern][seq.menu - 1] > 0)
      {
        if (seq.content_type[seq.active_pattern] == 0) //is Drumtrack
        {
          setCursor_textGrid(0, 0);
          if (seq.vel[seq.active_pattern][seq.menu - 1] < 210) //it is a normal sample
          {
            display.setTextColor(COLOR_SYSTEXT, COLOR_BACKGROUND);
            display.print("V:");
            sprintf(tmp, "%03d", seq.vel[seq.active_pattern][seq.menu - 1]);
            display.print(tmp);
            display.print(" ");
            display.setTextColor(COLOR_DRUMS, COLOR_BACKGROUND);
            show(0, 6, 7, seq_find_drum_name_from_note( seq.note_data[seq.active_pattern][seq.menu - 1]) );
            if (seq_active_function == 99)
            {
              display.setCursor(0, 3 * CHAR_height + 17);
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
            setCursor_textGrid(0, 0);
            display.print(F("Smp:["));
            setCursor_textGrid(13, 0);
            display.print("]");
            display.setTextColor(COLOR_PITCHSMP, COLOR_BACKGROUND);
            show(0, 5, 8, basename(drum_config[seq.vel[seq.active_pattern][seq.menu - 1] - 210].name));

            if (seq_active_function == 99)
            {
              display.setCursor(0, 3 * CHAR_height + 17);
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
              setCursor_textGrid(0, 0);
              display.print(F("Vel:"));
              sprintf(tmp, "%03d", seq.vel[seq.active_pattern][seq.menu - 1]);
              setCursor_textGrid(4, 0);
              display.print(tmp);
              display.print(" ");

            }
            set_pattern_content_type_color(seq.active_pattern);

            setCursor_textGrid(10, 0);
            display.print(noteNames[seq.note_data[seq.active_pattern][seq.menu - 1] % 12 ][0] );
            if (noteNames[seq.note_data[seq.active_pattern][seq.menu - 1] % 12 ][1] != '\0' ) {
              display.print(noteNames[seq.note_data[seq.active_pattern][seq.menu - 1] % 12 ][1] );
            }
            display.print( (seq.note_data[seq.active_pattern][seq.menu - 1] / 12) - 1);
            display.print("  ");
          }
          else
          { //note is latched
            setCursor_textGrid(0, 0);
            display.print(F("latched note "));
          }
          if (seq.content_type[seq.active_pattern] > 1) // is not drum or inst, print chord
          {
            setCursor_textGrid(0, 0);
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
        setCursor_textGrid(0, 0);
        display.print("              ");
        if (seq.menu - 1 > 0)
          seq_printVelGraphBar_single_step(seq.menu - 2, GREY1); //previous
        if (seq.menu - 1 < 15)
          seq_printVelGraphBar_single_step(seq.menu, GREY1 ); //next

        //clear "Insert xyz ?"  message
        display.fillRect(0, 3 * CHAR_height + 17, 212, 8, COLOR_BACKGROUND);
      }
    }
    if (seq.menu == 19 ) //edit content type of pattern
    {
      if (seq_active_function != 1)
      {
        //fix last step of sequence when switching to sub menus functions
        //print_track_steps_detailed(0, CHAR_height * 4 + 3, seq.menu - 1, false, true);
        seq_printVelGraphBar_single_step(15, GREY1);
        set_pattern_content_type_color(seq.active_pattern);
        display.setTextSize(2);
        setCursor_textGrid(15 , 1);
        display.print(seq_find_shortname(15)[0] );
        display.setCursor(0, 3 * CHAR_height + 17);
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
      display.setCursor(0, CHAR_height * 3 + 3);
      display.print("CONT.TYPE:");
      display.setCursor(11 * CHAR_width_small, CHAR_height * 3 + 3);

      print_content_type();
      display.setTextSize(2);
      seq_printAllSeqSteps();
      seq_printVelGraphBar();
      if (seq.cycle_touch_element == 0)  // touch keyboard is off
        border3_clear();
      if (seq.content_type[seq.active_pattern] == 0) //Drum Mode
      {
        print_track_steps_detailed(0, CHAR_height * 4 + 3, seq.menu - 1, true, true);
      }
      else
        print_single_pattern_pianoroll_in_pattern_editor(0, DISPLAY_HEIGHT, seq.active_pattern, seq.menu - 1, true);
      seq_sub_print_track_assignments(CHAR_width * 12, CHAR_height * 2, false);
    }
    else if (seq.menu > 20  && seq.menu < 29) // sub menus functions: track type
    {
      //disable menu 19
      display.setCursor(0, CHAR_height * 3 + 3);
      display.setTextSize(1);
      display.setTextColor(GREY1, COLOR_BACKGROUND);
      display.print("CONT.TYPE:");
      display.setCursor(11 * CHAR_width_small, CHAR_height * 3 + 3);

      print_content_type();
      display.setTextSize(2);
      // disable menu 19 end

      print_edit_mode();
      display.setTextSize(1);
      display.setCursor(0, 3 * CHAR_height + 17);
      if (seq_active_function != 1)
      {
        display.setTextColor(GREEN, COLOR_BACKGROUND);
        display.print(F("EDIT THE TRACK TYPE OF "));
        display.setTextColor( COLOR_SYSTEXT, COLOR_BACKGROUND);
        display.print(F("TRACK "));
        display.print( seq.menu - 20);
        display.setTextColor(GREEN, COLOR_BACKGROUND);
        display.print(F(" ?  "));

        display.setTextColor(COLOR_SYSTEXT, COLOR_BACKGROUND);
        display.setTextSize(2);
      } else
      {
        display.setTextColor(GREEN, COLOR_BACKGROUND);
        display.print("ASSIGN TRACK TYPE : ");

        set_track_type_color(seq.menu - 21);

        if (seq.track_type[seq.menu - 21] == 0 )   display.print(F("Drums"));
        else if (seq.track_type[seq.menu - 21] == 1 ) display.print(F("Instr."));
        else if (seq.track_type[seq.menu - 21] == 2 ) display.print(F("Chord"));
        else if (seq.track_type[seq.menu - 21] == 3 ) display.print(F("Arpeggio"));
        display.setTextColor(GREEN, COLOR_BACKGROUND);
        display.print(" ?    ");
        display.setTextSize(2);
      }
      seq_sub_print_track_assignments(CHAR_width * 12, CHAR_height * 2, false);
    }
    else if (seq.menu > 20 + NUM_SEQ_TRACKS  && seq.menu < 21 + 2 * NUM_SEQ_TRACKS) // sub menus functions: assign dexed/other synth instance to tracks
    {
      print_edit_mode();
      display.setTextSize(1);
      display.setCursor(0, 3 * CHAR_height + 17);
      if (seq_active_function != 1)
      {
        if (seq.track_type[seq.menu - 21 - NUM_SEQ_TRACKS] > 0 )
        {
          display.setTextColor(GREEN, COLOR_BACKGROUND);
          display.print(F("ASSIGN NEW INSTR. TO "));
          display.setTextColor( COLOR_SYSTEXT, COLOR_BACKGROUND);
          display.print(F("TRACK "));
          display.print( seq.menu - 20 - NUM_SEQ_TRACKS);
          display.setTextColor(GREEN, COLOR_BACKGROUND);
          display.print(F(" ? "));
          display.setTextColor(COLOR_SYSTEXT, COLOR_BACKGROUND);
          fill_up_with_spaces_left_window();
          display.setTextSize(2);
        }
        else
        {
          display.setTextColor( COLOR_BACKGROUND, COLOR_DRUMS);
          display.print(F("TRACK TYPE"));
          display.setTextColor(RED, COLOR_BACKGROUND);
          display.print(F(" T"));
          display.print( seq.menu - 20 - NUM_SEQ_TRACKS);
          display.setTextColor(COLOR_SYSTEXT, COLOR_BACKGROUND);
          display.print(F(" IS SET FOR "));
          display.setTextColor(COLOR_DRUMS, COLOR_BACKGROUND);
          display.print(F("DRUMS/SMP"));
        }
      }
      else
      {
        display.setTextColor(COLOR_SYSTEXT, COLOR_BACKGROUND);
        display.print(F("SET TRACK TO: "));
        display.setTextColor(GREEN, COLOR_BACKGROUND);
        if (seq.instrument[seq.menu - 21 - NUM_SEQ_TRACKS] < 2)
        {
          display.print(F("DEXED #"));
          display.print(seq.instrument[seq.menu - 21 - 8] + 1);
        }
        else if (seq.instrument[seq.menu - 21 - NUM_SEQ_TRACKS] == 2)
          display.print(F("ELEC.PIANO"));
        else if (seq.instrument[seq.menu - 21 - NUM_SEQ_TRACKS] == 3)
          display.print(F("MICROSYNTH #1"));
        else if (seq.instrument[seq.menu - 21 - NUM_SEQ_TRACKS] == 4)
          display.print(F("MICROSYNTH #2"));
        else if (seq.instrument[seq.menu - 21 - NUM_SEQ_TRACKS] > 4 && seq.instrument[seq.menu - 21 - NUM_SEQ_TRACKS] < 21)
        {
          display.print(F("MIDI USB #"));
          seq_print_formatted_number(seq.instrument[seq.menu - 21 - NUM_SEQ_TRACKS] - 4, 2);
        }
        else if (seq.instrument[seq.menu - 21 - NUM_SEQ_TRACKS] > 20 && seq.instrument[seq.menu - 21 - NUM_SEQ_TRACKS] < 37)
        {
          display.print(F("MIDI DIN #"));
          seq_print_formatted_number(seq.instrument[seq.menu - 21 - NUM_SEQ_TRACKS] - 20, 2);
        }
        display.setTextColor(COLOR_SYSTEXT, COLOR_BACKGROUND);
        display.print(F(" ?"));
        fill_up_with_spaces_left_window();
      }
      seq_sub_print_track_assignments(CHAR_width * 12, CHAR_height * 2, false);
    }
    else if (seq.menu == 16 )
    {
      display.setTextSize(1);
      display.setTextColor(GREY1, COLOR_BACKGROUND);
      display.setCursor(0, CHAR_height * 3 + 3);
      display.print(F("CONT.TYPE:"));
      display.setCursor(11 * CHAR_width_small, CHAR_height * 3 + 3);

      print_content_type();
      display.setTextSize(2);
      print_edit_mode();
      seq_sub_print_track_assignments(CHAR_width * 12, CHAR_height * 2, false);
    }
    if (seq.menu == 37 )
    {
      seq_sub_print_track_assignments(CHAR_width * 12, CHAR_height * 2, false);
      if (seq_active_function == 1)
        display.setTextColor(COLOR_SYSTEXT, RED);
      else
        display.setTextColor(COLOR_BACKGROUND, RED);
    }
    else
      display.setTextColor(RED, COLOR_BACKGROUND);
    display.setCursor(CHAR_width_small * 36,  22 * (CHAR_height_small + 2) + 11  );
    display.setTextSize(1);
    if (seq.auto_advance_step == 1)
      display.print ("AUTO ADV. STEP");
    else if (seq.auto_advance_step == 2)
      display.print ("AUTO ADV.+STOP");
    else
      display.print ("KEEP CUR. STEP");
  }
  if (LCDML.FUNC_close())     // ****** STABLE END *********
  {
    encoderDir[ENC_R].reset();

    if (seq.menu_status != 2) // don't clear screen when jumping (back) to pattern editor
    {
      display.fillScreen(COLOR_BACKGROUND);
      seq.menu_status = 0;
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
  display.fillRect(12 * CHAR_width, 0, 13, 16, COLOR_BACKGROUND);
  if (seq.running == false && seq.recording == false)
  {
    drawBitmap(12 * CHAR_width + 3, 0 + 3, special_chars[19], 8, 8, GREEN);
  } else if (seq.running == true && seq.recording == false)
  {
    seq.note_in = 0;
    drawBitmap(12 * CHAR_width + 3, 0 + 3, special_chars[20], 8, 8, RED);
  } else if (seq.running == true && seq.recording == true)
  {
    seq.note_in = 0;
    drawBitmap(12 * CHAR_width + 3, 0 + 3, special_chars[21], 8, 8, COLOR_PITCHSMP);
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
      setCursor_textGrid(15, 1);
      display.print(" ");
      setCursor_textGrid(18, 1);
      display.print(" ");
      setCursor_textGrid(2, 1);
      display.print(F("         "));
      setCursor_textGrid(17, 2);
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
      setCursor_textGrid(15, 1);
      display.print(" ");
      setCursor_textGrid(18, 1);
      display.print(" ");
      setCursor_textGrid(2, 1);
      display.print(F("         "));
      setCursor_textGrid(17, 2);
      display.print("  ");
      temp_int = seq.note_data[seq.active_pattern][0];
      seq_refresh_display_play_status();
      seq_printAllSeqSteps();
      print_track_steps_detailed(0, CHAR_height * 4 + 3, 254, false, true);
    }

    if ( seq.menu == 0 && seq_active_function == 0)
    {
      if ( (seq.content_type[seq.active_pattern] == 0 && activesample == NUM_DRUMSET_CONFIG + 3) || (seq.content_type[seq.active_pattern] > 0 && temp_int == 114) )
      { //swap patterns: Active pattern <-> destination pattern
        setCursor_textGrid(1, 1);
        display.print("SwapPattern:");
        temp_int = seq.active_pattern + 1;
        if (temp_int > NUM_SEQ_PATTERN - 1)temp_int = 0;
        seq.menu = 30;
      }
      else if ( (  seq.content_type[seq.active_pattern] == 0 && activesample == NUM_DRUMSET_CONFIG + 2) || ( seq.content_type[seq.active_pattern] > 0 && temp_int == 113) )
      { //copy pattern
        setCursor_textGrid(1, 1);
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
    setCursor_textGrid(15, 1);
    display.print("[");
    setCursor_textGrid(18, 1);
    display.print("]");
    setCursor_textGrid(1, 2);
    display.print(F("          to: [  ]"));
    setCursor_textGrid(16, 2);
    seq_print_formatted_number(temp_int, 2);
  }
  else if (seq.menu == 30)
  { //swap pattern
    setCursor_textGrid(15, 1);
    display.print("[");
    setCursor_textGrid(18, 1);
    display.print("]");
    setCursor_textGrid(1, 2);
    display.print(F("        with: [  ]"));
    setCursor_textGrid(16, 2);
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
      setCursor_textGrid(2, 1);
      display.print("         ");
      setCursor_textGrid(17, 2);
      display.print("  ");
      seq_refresh_display_play_status();
      seq_printAllSeqSteps();
      seq_printVelGraphBar();
      print_track_steps_detailed(0, CHAR_height * 4 + 3, 254, false, true);

    }
    else if ( seq.menu == 0 && seq_active_function == 0)
    {

      if ( (seq.content_type[seq.active_pattern] == 0 && activesample == NUM_DRUMSET_CONFIG + 4) || (seq.content_type[seq.active_pattern] > 0 && temp_int == 115) )
      { //fill patterns
        setCursor_textGrid(1, 1);
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
    setCursor_textGrid(6, 2);
    display.print(" ");
    setCursor_textGrid(13, 2);
    display.print(" ");
    setCursor_textGrid(13, 2);
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
      setCursor_textGrid(1, 2);
      display.print("with [");
      show(2, 7, 6, basename(drum_config[seq.temp_select_menu].name));
      setCursor_textGrid(13, 2);
      display.print("]");
      setCursor_textGrid(14, 2);
      if (seq.temp_active_menu == 0) display.print(F(" 1/4"));
      else if (seq.temp_active_menu == 1) display.print(F(" 1/8"));
      else if (seq.temp_active_menu == 2) display.print(F("1/16"));
    }
    else
    { //inst
      setCursor_textGrid(1, 2);
      display.print("with [");
      display.print(noteNames[seq.temp_select_menu % 12 ]);
      display.print( (seq.temp_select_menu / 12) - 1);
      display.print(" ");
      setCursor_textGrid(13, 2);
      display.print("]");
      setCursor_textGrid(14, 2);
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
      setCursor_textGrid(16, 1);
      seq_print_formatted_number(seq.active_pattern, 2);
      seq_refresh_display_play_status();
      seq_printAllSeqSteps();
      setCursor_textGrid(2, 1);
      display.print("         ");
      setCursor_textGrid(15, 1);
      display.print(" ");
      setCursor_textGrid(18, 1);
      display.print(" ");
    }
    if ( seq.menu == 0 && seq_active_function == 0)
    {
      if ( (seq.content_type[seq.active_pattern] == 0 && activesample == NUM_DRUMSET_CONFIG + 5) || (seq.content_type[seq.active_pattern] > 0 && temp_int == 116) )
      { //transpose pattern
        setCursor_textGrid(1, 1);
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
    setCursor_textGrid(14, 1);
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
    print_track_steps_detailed(0, CHAR_height * 4 + 3, 254, false, true);
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
        print_track_steps_detailed(0, CHAR_height * 4 + 3, 254, true, true);
      }
      else if ( (seq.content_type[seq.active_pattern] == 0 && activesample == NUM_DRUMSET_CONFIG) ||
                (seq.content_type[seq.active_pattern] > 0 && temp_int == 111) )
      { //clear pattern
        seq_clear_active_pattern();
        seq_printAllSeqSteps();
        print_track_steps_detailed(0, CHAR_height * 4 + 3, 254, true, true);
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
      setCursor_textGrid(11, 1);
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
    setCursor_textGrid(1, 0);
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
      setCursor_textGrid(10, 0);
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
    display.setCursor(0, 4 * CHAR_height);
    display.setTextSize(1);
    display.setTextColor(COLOR_SYSTEXT, COLOR_BACKGROUND);
    display.print(F("SELECT [ "));
    display.setTextColor(RED, COLOR_BACKGROUND);
    display.print("C0");
    display.setTextColor(COLOR_SYSTEXT, COLOR_BACKGROUND);
    display.print(F(" ] TO CLEAR THIS STEP"));
    fill_up_with_spaces_left_window();
    display.setTextSize(2);
  }
}

void print_current_sample_and_pitch_buffer()
{
  if (seq.cycle_touch_element == 0)  // touch keyboard is off
  {
    display.setTextSize(1);
    display.setCursor(36 * CHAR_width_small,  4 * (CHAR_height_small + 2) + 10  );
    display.setTextColor(GREY2, COLOR_BACKGROUND);
    display.print(F("SAMPLE BUFFER: "));
    display.setTextColor(COLOR_SYSTEXT, COLOR_BACKGROUND);
    display.setCursor(36 * CHAR_width_small,  5 * (CHAR_height_small + 2) + 10  );
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
    display.setCursor(36 * CHAR_width_small,  6 * (CHAR_height_small + 2) + 10  );
    display.setTextColor(GREY2);
    display.print(F("SAMPLE TYPE: "));
    display.setTextColor(COLOR_SYSTEXT, COLOR_BACKGROUND);
    display.setCursor(36 * CHAR_width_small,  7 * (CHAR_height_small + 2) + 10  );
    print_sample_type();
    display.setTextColor(GREY2, COLOR_BACKGROUND);
    display.setCursor(36 * CHAR_width_small,  8 * (CHAR_height_small + 2) + 10  );
    display.print(F("NOTE BUFFER: "));
    display.setTextColor(COLOR_SYSTEXT, COLOR_BACKGROUND);
    display.print(noteNames[temp_int % 12 ]);
    display.print( (temp_int / 12) - 1);
    display.print(" ");
    display.setTextColor(GREY2, COLOR_BACKGROUND);
    display.setCursor(36 * CHAR_width_small,  9 * (CHAR_height_small + 2) + 10  );
    display.print(F("USED IN TRK:"));
    display.setCursor(43 * CHAR_width_small,  10 * (CHAR_height_small + 2) + 10  );
    display.print(F("INST:"));
    display.setTextSize(2);
  }
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

//void seq_pattern_editor_update_disp_instr()
//{
//  if (seq.cycle_touch_element == 1)
//  {
//    display.setCursor(CHAR_width, 110);
//    display.setTextColor(COLOR_SYSTEXT, COLOR_BACKGROUND);
//    display.setTextSize(1);
//    display.print(F("PATTERN "));
//    display.setTextColor(GREEN, COLOR_BACKGROUND);
//    seq_print_formatted_number(seq.active_pattern, 2);
//    display.setTextColor(COLOR_SYSTEXT, COLOR_BACKGROUND);
//    display.print(F(" PLAYING:"));
//    if (seq.content_type[seq.active_pattern] == 1) //Inst Pattern
//    {
//      //UI_draw_waveform(activesample);
//
//
//      print_voice_settings_in_pattern_editor(12, 125);
//    }
//    else if (seq.content_type[seq.active_pattern] == 0)
//    {
//      display.setTextColor(COLOR_SYSTEXT, COLOR_BACKGROUND);
//      display.setCursor(CHAR_width, 125);
//      display.setTextSize(2);
//      display.print(F("DRUMS"));
//      display.fillRect(CHAR_width * 6, 124, 129, CHAR_height, COLOR_BACKGROUND);
//    }
//    else
//      display.fillRect(CHAR_width, 110, 70, 0, COLOR_PITCHSMP);
//
//  }
//
//  if (LCDML.FUNC_getID() == LCDML.OTHER_getIDFromFunction(UI_func_seq_pattern_editor))
//  {
//    display.setCursor ( 240, 110  );
//    virtual_keyboard_print_current_instrument();
//  }
//}



void virtual_keyboard_print_buttons()
{

  //oct +- buttons
  draw_button_on_grid(1, 16, "OCTAVE", "-", 0);
  draw_button_on_grid(45, 16, "OCTAVE", "+", 0);

  //instrument buttons
  draw_button_on_grid(9, 16,  "INSTR.", "-", 0);
  draw_button_on_grid(37, 16,  "INSTR.", "+", 0);

}
void seq_pattern_editor_update_dynamic_elements()
{
  if (seq.running == false)
  {
    if (seq.step_recording)
      draw_button_on_grid(36, 1, "RECORD", "ACTIVE", 1); //print step recorder icon
    else
      draw_button_on_grid(36, 1, "STEP", "RECORD", 2); //print step recorder icon
  }
  if (seq.cycle_touch_element == 0)
  {
    draw_button_on_grid(45, 1, "", "", 99); //print keyboard icon
    if (seq.content_type[seq.active_pattern] == 0) //Drum Mode
    {
      print_track_steps_detailed(0, CHAR_height * 4 + 3, 254, true, true);
      seq.note_editor_view = 0;
    }
    else
    {
      print_single_pattern_pianoroll_in_pattern_editor(0, DISPLAY_HEIGHT, seq.active_pattern, seq.menu - 3, true);
      seq.note_editor_view = 1;
    }
    seq_sub_print_track_assignments(CHAR_width * 12, CHAR_height * 2, true);
    print_current_sample_and_pitch_buffer();
  }
  else if (seq.cycle_touch_element == 1)
  {
    virtual_keyboard_print_buttons();
    draw_button_on_grid(45, 1, "BACK" , "TO SEQ", 0);
    virtual_keyboard();
  }
  display.setTextSize(2);
}

uint8_t find_track_in_song_where_pattern_is_used(uint8_t pattern)
{
  uint8_t result = 99;
  for (uint8_t s = 0; s < SONG_LENGTH; s++)
  {
    for (uint8_t t = 0; t < NUM_SEQ_TRACKS; t++)
    {
      for (uint8_t c = 0; c < 16; c++)
      {
        if (seq.chain[ seq.song[t][s] ][c] == pattern )
        {
          result = t;
          seq.current_track_type_of_active_pattern = seq.track_type[t];
          return result;
          break;
        }

      }
    }
  }
  return result;
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
      seq_pattern_editor_update_dynamic_elements();
      display.setCursor(0, CHAR_height * 3 + 3);
      display.setTextSize(1);
      display.setTextColor(GREY1, COLOR_BACKGROUND);
      display.print(F("CONT.TYPE:"));
      display.setCursor(11 * CHAR_width_small, CHAR_height * 3 + 3);
      print_content_type();
    }
    print_edit_mode();
    check_variable_samples_basespeed();
    temp_int = seq.note_data[seq.active_pattern][0];
    encoderDir[ENC_R].reset();
    seq.note_in = 0;
    display.setTextSize(2);
    seq_refresh_display_play_status();
    setCursor_textGrid(15, 0);
    display.setTextColor(GREY2, COLOR_BACKGROUND);
    seq_print_formatted_number(seq.active_pattern, 2);

    if (seq.menu_status != 2)
    {
      seq_printAllSeqSteps();
      seq_printVelGraphBar();
    }
    display.setTextSize(1);
    display.setTextColor(GREY2, COLOR_BACKGROUND);
    display.setCursor(48 * CHAR_width_small,  9 * (CHAR_height_small + 2) + 10  );
    if (find_track_in_song_where_pattern_is_used(seq.active_pattern) == 99)
      display.print( "NONE" );
    else
    {
      display.setTextColor(COLOR_SYSTEXT, COLOR_BACKGROUND);
      display.print( " " );
      display.print( find_track_in_song_where_pattern_is_used(seq.active_pattern) + 1 );
      display.print( "  " );
    }
    if (seq.instrument[find_track_in_song_where_pattern_is_used(seq.active_pattern)] < 40)
    {
      display.setCursor(49 * CHAR_width_small,  10 * (CHAR_height_small + 2) + 10  );
      seq_print_formatted_number(seq.instrument[find_track_in_song_where_pattern_is_used(seq.active_pattern)], 2);
    }
    display.setTextSize(2);
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
        display.setTextColor(GREY2, COLOR_BACKGROUND);
        display.setCursor(48 * CHAR_width_small,  9 * (CHAR_height_small + 2) + 10  );
        if (find_track_in_song_where_pattern_is_used(seq.active_pattern) == 99)
          display.print( "NONE" );
        else
        {
          display.setTextColor(COLOR_SYSTEXT, COLOR_BACKGROUND);
          display.print(" ");
          display.print( find_track_in_song_where_pattern_is_used(seq.active_pattern) + 1 );
          display.print("  ");
        }
        if (seq.instrument[find_track_in_song_where_pattern_is_used(seq.active_pattern)] < 40)
        {
          display.setCursor(49 * CHAR_width_small,  10 * (CHAR_height_small + 2) + 10  );
          seq_print_formatted_number(seq.instrument[find_track_in_song_where_pattern_is_used(seq.active_pattern)], 2);
        }
        display.setCursor(11 * CHAR_width_small, CHAR_height * 3 + 3);
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
            //show(0, 1, 9, basename(drum_config[activesample].name));
            ;
          }
          else
          {
            if (activesample < NUM_DRUMSET_CONFIG - 1 && seq.vel[seq.active_pattern][seq.menu - 3] > 209) //pitched sample
              // show(1, 2, 9, basename(drum_config[ seq.vel[seq.active_pattern][seq.menu - 3]-210  ].name));
              ;
          }
        }
        else
        {
          //          if (temp_int < 109)
          //          {
          //            setCursor_textGrid(3, 1);
          //            display.print("    ");
          //            setCursor_textGrid(1, 1);
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
      print_current_sample_and_pitch_buffer();
      display.setTextColor(COLOR_SYSTEXT, COLOR_BACKGROUND);
      setCursor_textGrid(11, 0);
      display.print(" ");
      setCursor_textGrid(13, 0);
      display.print(" ");
      setCursor_textGrid(0, 0);
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

          show(0, 1, 9, basename(drum_config[activesample].name));
          display.setTextColor(COLOR_SYSTEXT, COLOR_BACKGROUND);
        } else if (activesample == NUM_DRUMSET_CONFIG - 1) {
          setCursor_textGrid(1, 0);
          display.print(F("EMPTY    "));
        } else if (activesample == NUM_DRUMSET_CONFIG ) {
          setCursor_textGrid(1, 0);
          display.print(F("Clear Pat"));
        } else if (activesample == NUM_DRUMSET_CONFIG + 1) {
          setCursor_textGrid(1, 0);
          display.print(F("Clear All"));
        }
        else if (activesample == NUM_DRUMSET_CONFIG + 2) {
          setCursor_textGrid(1, 0);
          display.print(F("Copy Pat."));
        }
        else if (activesample == NUM_DRUMSET_CONFIG + 3) {
          setCursor_textGrid(1, 0);
          display.print(F("Swap Pat"));
        }
        else if (activesample == NUM_DRUMSET_CONFIG + 4) {
          setCursor_textGrid(1, 0);
          display.print(F("Fill Pat."));
        }
        else if (activesample == NUM_DRUMSET_CONFIG + 5) {
          setCursor_textGrid(1, 0);
          display.print(F("Transpose"));
        }
        setCursor_textGrid(10, 0);
        display.print("]");
      } else //Inst. Mode
      {
        if (temp_int < 109)
        {
          setCursor_textGrid(2, 0);
          display.print("    ");
          setCursor_textGrid(1, 0);
          if (seq.content_type[seq.active_pattern] == 1) //Inst
          {
            display.print(noteNames[temp_int % 12 ]);
            display.print( (temp_int / 12) - 1);
          }
          else if (seq.content_type[seq.active_pattern] == 2) //Chord
          {
            // print_chord_name(seq.menu - 3);
            setCursor_textGrid(1, 0);
            display.print(noteNames[temp_int % 12 ]);
            display.print( (temp_int / 12) - 1);
          }
        } else if (temp_int == 109)
        {
          setCursor_textGrid(1, 0);
          display.print(F("EMPTY    "));
        } else if (temp_int == 110)
        {
          setCursor_textGrid(1, 0);
          display.print(F("LATCH    "));
        } else if (temp_int == 111)
        {
          setCursor_textGrid(1, 0);
          display.print(F("ClearPat."));
        } else if (temp_int == 112)
        {
          setCursor_textGrid(1, 0);
          display.print(F("Clear All"));
        } else if (temp_int == 113)
        {
          setCursor_textGrid(1, 0);
          display.print(F("Copy Pat."));
        }  else if (temp_int == 114)
        {
          setCursor_textGrid(1, 0);
          display.print(F("Swap Pat."));
        } else if (temp_int == 115)
        {
          setCursor_textGrid(1, 0);
          display.print(F("Fill Pat."));
        } else if (temp_int == 116)
        {
          setCursor_textGrid(1, 0);
          display.print(F("Transpose"));
        }
        setCursor_textGrid(10, 0);
        display.print("]");
      }
    }
    else if (seq.menu == 1)
    {
      display.setTextColor(GREY2, COLOR_BACKGROUND);
      setCursor_textGrid(15, 0);
      seq_print_formatted_number(seq.active_pattern, 2);
      display.setTextColor(COLOR_SYSTEXT, COLOR_BACKGROUND);
      setCursor_textGrid(0, 0);
      display.print(" ");
      setCursor_textGrid(10, 0);
      display.print(" ");
      setCursor_textGrid(14, 0);
      display.print(" ");
      setCursor_textGrid(17, 0);
      display.print(" ");
      setCursor_textGrid(11, 0);
      display.print("[");
      setCursor_textGrid(13, 0);
      display.print("]");
      seq_refresh_display_play_status();
      if (activesample < NUM_DRUMSET_CONFIG - 1 && seq.content_type[seq.active_pattern] == 0)
      {
        if (drum_config[activesample].midinote < 210)
          display.setTextColor(COLOR_DRUMS, COLOR_BACKGROUND);
        else
          display.setTextColor(COLOR_PITCHSMP, COLOR_BACKGROUND);

        show(0, 1, 9, basename(drum_config[activesample].name));
        display.setTextColor(COLOR_SYSTEXT, COLOR_BACKGROUND);
      }
    }
    if (seq.menu == 2)
    {
      //seq_pattern_editor_update_disp_instr();
      display.fillRect(0, 3 * CHAR_height + 17, 212, 8, COLOR_BACKGROUND);
      display.setTextColor(COLOR_SYSTEXT, COLOR_BACKGROUND);
      display.setTextSize(2);
      setCursor_textGrid(11, 0);
      display.print(" ");
      setCursor_textGrid(13, 0);
      display.print(" ");
      setCursor_textGrid(14, 0);
      display.print("[");
      setCursor_textGrid(15, 0);
      seq_print_formatted_number(seq.active_pattern, 2);
      setCursor_textGrid(17, 0);
      display.print("]");
      if (seq.content_type[seq.active_pattern] == 0) //Drum Mode
      {
        setCursor_textGrid(0, 0);  // Print current sample name when switching track and track is drum track
        display.print(" ");
        if (drum_config[activesample].midinote < 210)
          display.setTextColor(COLOR_DRUMS, COLOR_BACKGROUND);
        else
          display.setTextColor(COLOR_PITCHSMP, COLOR_BACKGROUND);
        show(0, 1, 9, basename(drum_config[activesample].name));
        display.setTextColor(COLOR_SYSTEXT, COLOR_BACKGROUND);
        display.print("  ");
        if (seq.note_editor_view != 0)
        {
          seq.note_editor_view = 0;
          border3_clear();
          print_track_steps_detailed(0, CHAR_height * 4 + 3, 254, true, true);
        }
        else
          print_track_steps_detailed(0, CHAR_height * 4 + 3, 254, false, true);
      }
      else
      {
        if (seq.cycle_touch_element == 0)
        {
          // Print note buffer when switching track and track is an instrument track
          setCursor_textGrid(0, 0);
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
            print_single_pattern_pianoroll_in_pattern_editor(0, DISPLAY_HEIGHT, seq.active_pattern, seq.menu - 3, true);
          }
          else
            print_single_pattern_pianoroll_in_pattern_editor(0, DISPLAY_HEIGHT, seq.active_pattern, seq.menu - 3, false);

        }
      }
      display.setTextSize(2);
      seq_printAllSeqSteps();
      seq_printVelGraphBar();
      print_current_sample_and_pitch_buffer();
      seq_sub_print_track_assignments(CHAR_width * 12, CHAR_height * 2, false);
    }
    if (seq.menu == 3)
    {
      display.setTextColor(GREY2, COLOR_BACKGROUND);
      setCursor_textGrid(15, 0);
      seq_print_formatted_number(seq.active_pattern, 2);

      setCursor_textGrid(14, 0);
      display.print(" ");
      setCursor_textGrid(17, 0);
      display.print(" ");
    }
    if (seq.menu > 2 && seq.menu < 19 && seq_active_function != 40)
    {
      if (seq.content_type[seq.active_pattern] == 0) //Drum Mode
      {
        print_track_steps_detailed(0, CHAR_height * 4 + 3, seq.menu - 3, false, false);
      }
      else
      {
        print_single_pattern_pianoroll_in_pattern_editor(0, DISPLAY_HEIGHT, seq.active_pattern, seq.menu - 3, false);
      }
      if (seq.menu == 3)
        setCursor_textGrid(0, 1);
      else
        setCursor_textGrid(seq.menu - 3, 1);
      display.setTextColor(GREEN, GREY2);
      display.print(seq_find_shortname(seq.menu - 3)[0]);
      if (seq.menu > 3)
      {
        setCursor_textGrid(seq.menu - 4, 1);
        if (seq.vel[seq.active_pattern][seq.menu - 4] > 209) //if pitched sample, change color
          display.setTextColor(COLOR_PITCHSMP, COLOR_BACKGROUND);
        else
          set_pattern_content_type_color(seq.active_pattern);
        display.print(seq_find_shortname(seq.menu - 4)[0]);
      }
      if (seq.menu < 18)
      {
        setCursor_textGrid(seq.menu - 2, 1);
        if (seq.vel[seq.active_pattern][seq.menu - 2] > 209) //if pitched sample, change color
          display.setTextColor(COLOR_PITCHSMP, COLOR_BACKGROUND);
        else
          set_pattern_content_type_color(seq.active_pattern);
        display.print(seq_find_shortname(seq.menu - 2)[0]);
      }
      if (seq.content_type[seq.active_pattern] > 0 && seq.vel[seq.active_pattern][seq.menu - 3] < 210
          && seq.note_data[seq.active_pattern][seq.menu - 3] != 0) //is not Drum Mode and not empty, print note
      {
        setCursor_textGrid(0, 0);
        display.print(" ");
        seq_print_current_note_from_step( seq.menu - 3 );
      }
      else if (seq.content_type[seq.active_pattern] == 0)  // is drum pattern
      {
        //print current sample name on active step if not empty
        if (seq.note_data[seq.active_pattern][seq.menu - 3] > 0)
        {
          setCursor_textGrid(0, 0);
          display.print(" ");

          if (activesample < NUM_DRUMSET_CONFIG - 1 && seq.vel[seq.active_pattern][seq.menu - 3] < 210) //normal sample
          {
            show(0, 1, 9, seq_find_drum_name_from_note( seq.note_data[seq.active_pattern][seq.menu - 3]) );
            UI_draw_waveform(seq_find_drum_data_from_note  (seq.note_data[seq.active_pattern][seq.menu - 3])  );

            //check if the same note is already there, if so ask to clear it
            if (seq.note_data[seq.active_pattern][seq.menu - 3] == drum_config[activesample].midinote)
            {
              display.setTextColor(COLOR_SYSTEXT, COLOR_BACKGROUND);
              display.setCursor(0, 3 * CHAR_height + 17);
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
              display.setCursor(0, 3 * CHAR_height + 17);
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
              show(0, 1, 9, basename(drum_config[ seq.vel[seq.active_pattern][seq.menu - 3] - 210  ].name));
              UI_draw_waveform(seq.vel[seq.active_pattern][seq.menu - 3] - 210 );

              if (seq.vel[seq.active_pattern][seq.menu - 3] == drum_config[activesample].midinote)  // pitched sample is idential, ask to edit pitch/delete
              {
                display.setTextColor(COLOR_SYSTEXT, COLOR_BACKGROUND);
                display.setCursor(0, 3 * CHAR_height + 17);
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
                display.setCursor(0, 3 * CHAR_height + 17);
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
          setCursor_textGrid(0, 0);
          display.print(" ");
          display.setTextColor(GREY2, COLOR_BACKGROUND);
          show(0, 1, 9, "--EMPTY--");
          display.setTextColor(COLOR_SYSTEXT, COLOR_BACKGROUND);
          display.setCursor(0, 3 * CHAR_height + 17);
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
          setCursor_textGrid(0, 0);
          show(0, 1, 4, basename(drum_config[activesample].name));
          setCursor_textGrid(6, 0);
          seq_print_current_note_from_step(seq.menu - 3);
        }
      }
    }
    // instr. pitch edit/delete/exchange
    if ( seq.menu > 2 && seq.menu - 3 < 16 && seq.content_type[seq.active_pattern] > 0)
    {
      if (seq.note_data[seq.active_pattern][seq.menu - 3] == 0)  //insert note buffer if step is empty
      {
        display.setCursor(0, 3 * CHAR_height + 17);
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
          display.setCursor(0, 3 * CHAR_height + 17);
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
          display.setCursor(0, 3 * CHAR_height + 17);
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
    {
      display.fillScreen(COLOR_BACKGROUND);
      seq.menu_status = 0;
    }

  }
}

void UI_toplineInfoText( uint8_t s)
{
  if (s == 2)
    display.fillRect(0, 0, DISPLAY_WIDTH - 1, CHAR_height_small * 2 + 3, COLOR_PITCHSMP);
  else if (s == 1)
    display.fillRect(0, 0, DISPLAY_WIDTH - 1, CHAR_height_small * 1 + 3, COLOR_PITCHSMP);
  //s=size 1/2 lines
}


void update_microsynth_instance_icons()
{
  if (microsynth_selected_instance == 0)
  {
    setCursor_textGrid_mini(13, 1);
    display.setTextColor(COLOR_BACKGROUND, COLOR_SYSTEXT);
    display.print(F("1"));
    display.setTextColor(GREY1, COLOR_BACKGROUND);
    setCursor_textGrid_mini(15, 1);
    display.print(F("2"));
  }
  else
  {
    setCursor_textGrid_mini(13, 1);
    display.setTextColor(GREY1, COLOR_BACKGROUND);
    display.print(F("1"));

    display.setTextColor(COLOR_BACKGROUND, COLOR_SYSTEXT);
    setCursor_textGrid_mini(15, 1);
    display.print(F("2"));
  }
  display.setTextColor(GREY1);
}

void update_pwm_text()
{
  if (seq.cycle_touch_element != 1)
  {
    if (microsynth[microsynth_selected_instance].wave == 4 || microsynth[microsynth_selected_instance].wave == 7)
      display.setTextColor(GREY1);
    else
      display.setTextColor(GREY2);

    setCursor_textGrid_mini(1, 17);
    display.print(F("PWM"));
    setCursor_textGrid_mini(1, 18);
    display.print(F("SPEED"));
    display.setTextColor(GREY1);
  }
}

void microsynth_refresh_lower_screen_static_text()
{
  helptext_l("BACK");
  helptext_r("LONG PUSH:INST.SEL.  <>SEL.PARA.");

  display.setTextColor(GREY1, COLOR_BACKGROUND);
  setCursor_textGrid_mini(1, 13);
  display.print(F("FILTER"));
  setCursor_textGrid_mini(1, 14);
  display.print(F("FREQ"));
  setCursor_textGrid_mini(1, 15);
  display.print(F("RES"));
  setCursor_textGrid_mini(9, 15);
  display.print(F("SPEED"));
  setCursor_textGrid_mini(22, 12);
  display.print(F("REV. SEND"));
  setCursor_textGrid_mini(22, 13);
  display.print(F("CHR. SEND"));
  setCursor_textGrid_mini(22, 14);
  display.print(F("DLY. SEND"));
  setCursor_textGrid_mini(22, 16);
  display.print(F("PANORAMA"));
  setCursor_textGrid_mini(22, 17);
  display.print(F("MIDI CHN."));
  setCursor_textGrid_mini(13, 17);
  display.print(F(">"));
  setCursor_textGrid_mini(13, 14);
  display.print(F(">"));

}

void microsynth_refresh_lower_screen_dynamic_text()
{
  if (seq.temp_select_menu == 8)
    display.setTextColor(COLOR_BACKGROUND, COLOR_SYSTEXT); else display.setTextColor(RED, COLOR_BACKGROUND);
  setCursor_textGrid_mini(9, 13);
  if (microsynth[microsynth_selected_instance].filter_osc_mode == 0)
    display.print(F("OFF   "));
  else if (microsynth[microsynth_selected_instance].filter_osc_mode == 1)
    display.print(F("LP12dB"));
  else if (microsynth[microsynth_selected_instance].filter_osc_mode == 2)
    display.print(F("BP12dB"));
  else if (microsynth[microsynth_selected_instance].filter_osc_mode == 3)
    display.print(F("HI12dB"));

  print_small_intbar(9, 14, microsynth[microsynth_selected_instance].filter_osc_freq_from / 100, 9, 0, 1);
  print_small_intbar(15, 14, microsynth[microsynth_selected_instance].filter_osc_freq_to / 100, 10, 0, 1);
  print_small_intbar(5, 15, microsynth[microsynth_selected_instance].filter_osc_resonance, 11, 0, 1);
  print_small_intbar(15, 15, microsynth[microsynth_selected_instance].filter_osc_speed / 10, 12, 0, 1);

  print_small_intbar(9, 17, microsynth[microsynth_selected_instance].pwm_from, 13, 0, 1);
  print_small_intbar(15, 17,  microsynth[microsynth_selected_instance].pwm_to, 14, 0, 1);

  setCursor_textGrid_mini(10, 18);
  if (seq.temp_select_menu == 15)
    display.setTextColor(COLOR_BACKGROUND, COLOR_SYSTEXT); else display.setTextColor(COLOR_SYSTEXT, COLOR_BACKGROUND);
  seq_print_formatted_number( microsynth[microsynth_selected_instance].pwm_speed, 2);

  setCursor_textGrid_mini(33, 12);
  if (seq.temp_select_menu == 28)
  {
    display.setTextColor(COLOR_BACKGROUND, COLOR_SYSTEXT);
  }
  else display.setTextColor(COLOR_SYSTEXT, COLOR_BACKGROUND);
  seq_print_formatted_number( microsynth[microsynth_selected_instance].rev_send, 3);
  setCursor_textGrid_mini(33, 13);

  if (seq.temp_select_menu == 29)
    display.setTextColor(COLOR_BACKGROUND, COLOR_SYSTEXT); else display.setTextColor(COLOR_SYSTEXT, COLOR_BACKGROUND);
  seq_print_formatted_number( microsynth[microsynth_selected_instance].chorus_send, 3);
  setCursor_textGrid_mini(33, 14);

  if (seq.temp_select_menu == 30)
    display.setTextColor(COLOR_BACKGROUND, COLOR_SYSTEXT); else display.setTextColor(COLOR_SYSTEXT, COLOR_BACKGROUND);
  seq_print_formatted_number( microsynth[microsynth_selected_instance].delay_send, 3);

  if (seq.cycle_touch_element != 1)
    print_small_panbar(33, 16, microsynth[microsynth_selected_instance].pan, 31);


  if (seq.temp_select_menu == 32)
    display.setTextColor(COLOR_BACKGROUND, COLOR_SYSTEXT); else display.setTextColor(COLOR_SYSTEXT, COLOR_BACKGROUND);
  setCursor_textGrid_mini(34, 17);
  seq_print_formatted_number( microsynth[microsynth_selected_instance].midi_channel, 2);

  update_pwm_text();

}



void UI_func_epiano(uint8_t param)
{

  if (LCDML.FUNC_setup())         // ****** SETUP *********
  {
    // setup function
    encoderDir[ENC_R].reset();
    display.fillScreen(COLOR_BACKGROUND);
    display.setTextSize(1);
    helptext_l("BACK");
    helptext_r("< > SELECT PARAM");

    if (seq.cycle_touch_element != 1)
    {
      draw_button_on_grid(45, 1, "", "", 99); //print keyboard icon
    }
    else
    {
      draw_button_on_grid(45, 1, "MORE" , "PARAM.", 0);
      virtual_keyboard();
      virtual_keyboard_print_buttons();
      virtual_keyboard_print_current_instrument();
    }

    seq_active_function = 0;
    setCursor_textGrid_mini(1, 1);
    display.setTextColor(RED);
    display.print(F("ELECTRIC PIANO"));
    display.setTextColor(COLOR_SYSTEXT);
    display.setTextColor(GREY1);
    setCursor_textGrid_mini(1, 3);
    display.print(F("VOICE LEVEL"));
    setCursor_textGrid_mini(1, 4);
    display.print(F("PANORAMA"));
    setCursor_textGrid_mini(1, 5);
    display.print(F("TRANSPOSE"));
    display.setTextColor(GREY2);
    setCursor_textGrid_mini(1, 7);
    display.print(F("AUDIO"));

    display.setTextColor(GREY1);
    setCursor_textGrid_mini(1, 9);
    display.print(F("DECAY"));
    setCursor_textGrid_mini(1, 10);
    display.print(F("RELEASE"));
    setCursor_textGrid_mini(1, 11);
    display.print(F("HARDNESS"));
    setCursor_textGrid_mini(1, 12);
    display.print(F("TREBLE"));
    setCursor_textGrid_mini(1, 13);
    display.print(F("STEREO"));
    setCursor_textGrid_mini(1, 15);
    display.print(F("TUNE"));
    setCursor_textGrid_mini(1, 16);
    display.print(F("DETUNE"));

    display.setTextColor(GREY2);
    setCursor_textGrid_mini(1, 18);
    display.print(F("SYSTEM"));
    display.setTextColor(GREY1);
    setCursor_textGrid_mini(1, 20);
    display.print(F("POLYPHONY"));
    setCursor_textGrid_mini(1, 21);
    display.print(F("VELOCITY SENSE"));

    display.setTextColor(GREY2);
    setCursor_textGrid_mini(24, 1);
    display.print(F("EFFECTS"));
    display.setTextColor(GREY1);
    setCursor_textGrid_mini(24, 3);
    display.print(F("OVERDRIVE"));
    setCursor_textGrid_mini(24, 5);
    display.setTextColor(GREY2);
    display.print(F("TREMOLO"));
    display.setTextColor(GREY1);
    setCursor_textGrid_mini(24, 6);
    display.print(F("WIDTH"));
    setCursor_textGrid_mini(24, 7);
    display.print(F("LFO"));

    display.setTextColor(GREY2);
    setCursor_textGrid_mini(24, 9);
    display.print(F("CHORUS"));
    display.setTextColor(GREY1);
    setCursor_textGrid_mini(24, 10);
    display.print(F("FREQUENCY"));
    setCursor_textGrid_mini(24, 11);
    display.print(F("WAVEFORM"));
    setCursor_textGrid_mini(24, 12);
    display.print(F("DEPTH"));
    setCursor_textGrid_mini(24, 13);
    display.print(F("LEVEL"));
    setCursor_textGrid_mini(24, 15);
    display.print(F("REVERB S."));

    display.setTextColor(GREY2);
    setCursor_textGrid_mini(24, 17);
    display.print(F("MIDI"));
    display.setTextColor(GREY1);
    setCursor_textGrid_mini(24, 19);
    display.print(F("LOWEST NOTE"));
    setCursor_textGrid_mini(24, 20);
    display.print(F("HIGHEST NOTE"));
    setCursor_textGrid_mini(24, 21);
    display.print(F("MIDI CHANNEL"));

    display.setTextColor(GREY2, COLOR_BACKGROUND);
    setCursor_textGrid_mini(41, 19);
    display.print(F("["));
    setCursor_textGrid_mini(45, 19);
    display.print(F("]"));
    setCursor_textGrid_mini(41, 20);
    display.print(F("["));
    setCursor_textGrid_mini(45, 20);
    display.print(F("]"));

  }
  if (LCDML.FUNC_loop())          // ****** LOOP *********
  {
    char note_name[4];
    if ((LCDML.BT_checkDown() && encoderDir[ENC_R].Down()) || (LCDML.BT_checkUp() && encoderDir[ENC_R].Up())  )
    {
      if (LCDML.BT_checkDown())
      {
        if ( seq_active_function == 0 )
          seq.temp_select_menu = constrain(seq.temp_select_menu + 1, 0, 22);
        else if ( seq_active_function == 1 )
        {
          if ( seq.temp_select_menu == 0 )
          {
            configuration.epiano.sound_intensity = constrain(configuration.epiano.sound_intensity + ENCODER[ENC_R].speed(), EP_SOUND_INTENSITY_MIN, EP_SOUND_INTENSITY_MAX);
            MD_sendControlChange(configuration.epiano.midi_channel, 7, configuration.epiano.sound_intensity);
          }
          else if ( seq.temp_select_menu == 1 )
          {
            configuration.epiano.pan = constrain(configuration.epiano.pan + ENCODER[ENC_R].speed(), PANORAMA_MIN, PANORAMA_MAX);
            MD_sendControlChange(configuration.epiano.midi_channel, 10, map(configuration.epiano.pan, PANORAMA_MIN, PANORAMA_MAX, 0, 127));
          }
          else if ( seq.temp_select_menu == 2 )
            configuration.epiano.transpose = constrain(configuration.epiano.transpose + ENCODER[ENC_R].speed(), EP_TRANSPOSE_MIN, EP_TRANSPOSE_MAX);

          else if ( seq.temp_select_menu == 3 )
            configuration.epiano.decay = constrain(configuration.epiano.decay + ENCODER[ENC_R].speed(), EP_DECAY_MIN, EP_DECAY_MAX);
          else if ( seq.temp_select_menu == 4 )
            configuration.epiano.release = constrain(configuration.epiano.release + ENCODER[ENC_R].speed(), EP_RELEASE_MIN, EP_RELEASE_MAX);
          else if ( seq.temp_select_menu == 5 )
            configuration.epiano.hardness = constrain(configuration.epiano.hardness + ENCODER[ENC_R].speed(), EP_HARDNESS_MIN, EP_HARDNESS_MAX);
          else if ( seq.temp_select_menu == 6 )
            configuration.epiano.treble = constrain(configuration.epiano.treble + ENCODER[ENC_R].speed(), EP_TREBLE_MIN, EP_TREBLE_MAX);
          else if ( seq.temp_select_menu == 7 )
            configuration.epiano.stereo = constrain(configuration.epiano.stereo + ENCODER[ENC_R].speed(), EP_STEREO_MIN, EP_STEREO_MAX);
          else if ( seq.temp_select_menu == 8 )
          {
            configuration.epiano.tune = constrain(configuration.epiano.tune + ENCODER[ENC_R].speed(), EP_TUNE_MIN, EP_TUNE_MAX);
            MD_sendControlChange(configuration.epiano.midi_channel, 94, configuration.epiano.tune);
          }
          else if ( seq.temp_select_menu == 9 )
            configuration.epiano.detune = constrain(configuration.epiano.detune + ENCODER[ENC_R].speed(), EP_DETUNE_MIN, EP_DETUNE_MAX);
          else if ( seq.temp_select_menu == 10 )
            configuration.epiano.polyphony = constrain(configuration.epiano.polyphony + 1, EP_POLYPHONY_MIN, EP_POLYPHONY_MAX);

          else if ( seq.temp_select_menu == 11 )
            configuration.epiano.velocity_sense = constrain(configuration.epiano.velocity_sense + ENCODER[ENC_R].speed(), EP_VELOCITY_SENSE_MIN, EP_VELOCITY_SENSE_MAX);
          else if ( seq.temp_select_menu == 12 )
            configuration.epiano.overdrive = constrain(configuration.epiano.overdrive + ENCODER[ENC_R].speed(), EP_OVERDRIVE_MIN, EP_OVERDRIVE_MAX);
          else if ( seq.temp_select_menu == 13 )
            configuration.epiano.pan_tremolo = constrain(configuration.epiano.pan_tremolo + ENCODER[ENC_R].speed(), EP_PAN_TREMOLO_MIN, EP_PAN_TREMOLO_MAX);
          else if ( seq.temp_select_menu == 14 )
            configuration.epiano.pan_lfo = constrain(configuration.epiano.pan_lfo + ENCODER[ENC_R].speed(), EP_PAN_LFO_MIN, EP_PAN_LFO_MAX);
          else if ( seq.temp_select_menu == 15 )
            configuration.fx.ep_chorus_frequency = constrain(configuration.fx.ep_chorus_frequency + ENCODER[ENC_R].speed(), EP_CHORUS_FREQUENCY_MIN, EP_CHORUS_FREQUENCY_MAX);
          else if ( seq.temp_select_menu == 16 )
            configuration.fx.ep_chorus_waveform = constrain(configuration.fx.ep_chorus_waveform + 1, EP_CHORUS_WAVEFORM_MIN, EP_CHORUS_WAVEFORM_MAX);
          else if ( seq.temp_select_menu == 17 )
            configuration.fx.ep_chorus_depth = constrain(configuration.fx.ep_chorus_depth + ENCODER[ENC_R].speed(), EP_CHORUS_DEPTH_MIN, EP_CHORUS_DEPTH_MAX);
          else if ( seq.temp_select_menu == 18 )
          {
            configuration.fx.ep_chorus_level = constrain(configuration.fx.ep_chorus_level + ENCODER[ENC_R].speed(), EP_CHORUS_LEVEL_MIN, EP_CHORUS_LEVEL_MAX);
            MD_sendControlChange(configuration.epiano.midi_channel, 93, configuration.fx.ep_chorus_level);
          }
          else if ( seq.temp_select_menu == 19 )
          {
            configuration.fx.ep_reverb_send = constrain(configuration.fx.ep_reverb_send + ENCODER[ENC_R].speed(), EP_REVERB_SEND_MIN, EP_REVERB_SEND_MAX);
            MD_sendControlChange(configuration.epiano.midi_channel, 91, configuration.fx.ep_reverb_send);
          }
          if ( seq.temp_select_menu == 20 )
            configuration.epiano.lowest_note = constrain(configuration.epiano.lowest_note + ENCODER[ENC_R].speed(), EP_LOWEST_NOTE_MIN, EP_LOWEST_NOTE_MAX);
          else if ( seq.temp_select_menu == 21 )
            configuration.epiano.highest_note = constrain(configuration.epiano.highest_note + ENCODER[ENC_R].speed(), EP_HIGHEST_NOTE_MIN, EP_HIGHEST_NOTE_MAX);
          else if ( seq.temp_select_menu == 22 )
            configuration.epiano.midi_channel = constrain(configuration.epiano.midi_channel + ENCODER[ENC_R].speed(), EP_MIDI_CHANNEL_MIN, EP_MIDI_CHANNEL_MAX);
        }
      }
      else if (LCDML.BT_checkUp())
      {
        if ( seq_active_function == 0 )
          seq.temp_select_menu = constrain(seq.temp_select_menu - 1, 0, 22);
        else if ( seq_active_function == 1 )
        {
          if ( seq.temp_select_menu == 0 )
          {
            configuration.epiano.sound_intensity = constrain(configuration.epiano.sound_intensity - ENCODER[ENC_R].speed(), EP_SOUND_INTENSITY_MIN, EP_SOUND_INTENSITY_MAX);
            MD_sendControlChange(configuration.epiano.midi_channel, 7, configuration.epiano.sound_intensity);
          }
          else if ( seq.temp_select_menu == 1 )
          {
            configuration.epiano.pan = constrain(configuration.epiano.pan - ENCODER[ENC_R].speed(), PANORAMA_MIN, PANORAMA_MAX);
            MD_sendControlChange(configuration.epiano.midi_channel, 10, map(configuration.epiano.pan, PANORAMA_MIN, PANORAMA_MAX, 0, 127));
          }
          else if ( seq.temp_select_menu == 2 )
            configuration.epiano.transpose = constrain(configuration.epiano.transpose - ENCODER[ENC_R].speed(), EP_TRANSPOSE_MIN, EP_TRANSPOSE_MAX);
          else if ( seq.temp_select_menu == 3 )
            configuration.epiano.decay = constrain(configuration.epiano.decay - ENCODER[ENC_R].speed(), EP_DECAY_MIN, EP_DECAY_MAX);
          else if ( seq.temp_select_menu == 4 )
            configuration.epiano.release = constrain(configuration.epiano.release - ENCODER[ENC_R].speed(), EP_RELEASE_MIN, EP_RELEASE_MAX);
          else if ( seq.temp_select_menu == 5 )
            configuration.epiano.hardness = constrain(configuration.epiano.hardness - ENCODER[ENC_R].speed(), EP_HARDNESS_MIN, EP_HARDNESS_MAX);
          else if ( seq.temp_select_menu == 6 )
            configuration.epiano.treble = constrain(configuration.epiano.treble - ENCODER[ENC_R].speed(), EP_TREBLE_MIN, EP_TREBLE_MAX);
          else if ( seq.temp_select_menu == 7 )
            configuration.epiano.stereo = constrain(configuration.epiano.stereo - ENCODER[ENC_R].speed(), EP_STEREO_MIN, EP_STEREO_MAX);
          else if ( seq.temp_select_menu == 8 )
          {
            configuration.epiano.tune = constrain(configuration.epiano.tune - ENCODER[ENC_R].speed(), EP_TUNE_MIN, EP_TUNE_MAX);
            MD_sendControlChange(configuration.epiano.midi_channel, 94, configuration.epiano.tune);
          }
          else if ( seq.temp_select_menu == 9 )
            configuration.epiano.detune = constrain(configuration.epiano.detune - ENCODER[ENC_R].speed(), EP_DETUNE_MIN, EP_DETUNE_MAX);
          else if ( seq.temp_select_menu == 10 )
          {
            if (configuration.epiano.polyphony - 1 < 1)
              configuration.epiano.polyphony = 1;
            else
            {
              configuration.epiano.polyphony = constrain(configuration.epiano.polyphony - 1, EP_POLYPHONY_MIN, EP_POLYPHONY_MAX);
            }
          }
          else if ( seq.temp_select_menu == 11 )
            configuration.epiano.velocity_sense = constrain(configuration.epiano.velocity_sense - ENCODER[ENC_R].speed(), EP_VELOCITY_SENSE_MIN, EP_VELOCITY_SENSE_MAX);
          else if ( seq.temp_select_menu == 12 )
            configuration.epiano.overdrive = constrain(configuration.epiano.overdrive - ENCODER[ENC_R].speed(), EP_OVERDRIVE_MIN, EP_OVERDRIVE_MAX);

          else if ( seq.temp_select_menu == 13 )
            configuration.epiano.pan_tremolo = constrain(configuration.epiano.pan_tremolo - ENCODER[ENC_R].speed(), EP_PAN_TREMOLO_MIN, EP_PAN_TREMOLO_MAX);

          else if ( seq.temp_select_menu == 14 )
            configuration.epiano.pan_lfo = constrain(configuration.epiano.pan_lfo - ENCODER[ENC_R].speed(), EP_PAN_LFO_MIN, EP_PAN_LFO_MAX);

          else if ( seq.temp_select_menu == 15 )
            configuration.fx.ep_chorus_frequency = constrain(configuration.fx.ep_chorus_frequency - ENCODER[ENC_R].speed(), EP_CHORUS_FREQUENCY_MIN, EP_CHORUS_FREQUENCY_MAX);

          else if ( seq.temp_select_menu == 16 )
            configuration.fx.ep_chorus_waveform = constrain(configuration.fx.ep_chorus_waveform - 1, EP_CHORUS_WAVEFORM_MIN, EP_CHORUS_WAVEFORM_MAX);
          else if ( seq.temp_select_menu == 17 )
            configuration.fx.ep_chorus_depth = constrain(configuration.fx.ep_chorus_depth - ENCODER[ENC_R].speed(), EP_CHORUS_DEPTH_MIN, EP_CHORUS_DEPTH_MAX);
          else if ( seq.temp_select_menu == 18 )
          {
            configuration.fx.ep_chorus_level = constrain(configuration.fx.ep_chorus_level - ENCODER[ENC_R].speed(), EP_CHORUS_LEVEL_MIN, EP_CHORUS_LEVEL_MAX);
            MD_sendControlChange(configuration.epiano.midi_channel, 93, configuration.fx.ep_chorus_level);
          }
          else if ( seq.temp_select_menu == 19 )
          {
            configuration.fx.ep_reverb_send = constrain(configuration.fx.ep_reverb_send - ENCODER[ENC_R].speed(), EP_REVERB_SEND_MIN, EP_REVERB_SEND_MAX);
            MD_sendControlChange(configuration.epiano.midi_channel, 91, configuration.fx.ep_reverb_send);
          }
          if ( seq.temp_select_menu == 20 )
            configuration.epiano.lowest_note = constrain(configuration.epiano.lowest_note - ENCODER[ENC_R].speed(), EP_LOWEST_NOTE_MIN, EP_LOWEST_NOTE_MAX);
          else if ( seq.temp_select_menu == 21 )
            configuration.epiano.highest_note = constrain(configuration.epiano.highest_note - ENCODER[ENC_R].speed(), EP_HIGHEST_NOTE_MIN, EP_HIGHEST_NOTE_MAX);
          else if ( seq.temp_select_menu == 22 )
            configuration.epiano.midi_channel = constrain(configuration.epiano.midi_channel - ENCODER[ENC_R].speed(), EP_MIDI_CHANNEL_MIN, EP_MIDI_CHANNEL_MAX);
        }
      }
    }
    if (LCDML.BT_checkEnter()  && encoderDir[ENC_R].ButtonShort())  //handle button presses during menu
    {
      if ( seq_active_function == 0 )
        seq_active_function = 1;
      else
        seq_active_function = 0;
    }
    //button check end <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<

#if defined(USE_EPIANO)
    print_small_intbar(13, 3, configuration.epiano.sound_intensity, 0, 1, 0);
    ep.setVolume(mapfloat(configuration.epiano.sound_intensity, EP_SOUND_INTENSITY_MIN, EP_SOUND_INTENSITY_MAX, 0, 1.0));
#else
    {
      setCursor_textGrid_mini(13, 3);
      display.print(F("DISABLED"));
    }
#endif
    print_small_panbar(13, 4, configuration.epiano.pan, 1);
    ep_stereo_panorama.panorama(mapfloat(configuration.epiano.pan, PANORAMA_MIN, PANORAMA_MAX, -1.0, 1.0));
    setModeColor(2);
    setCursor_textGrid_mini(13, 5);
    seq_print_formatted_number_signed( configuration.epiano.transpose - 24, 2);
    print_small_intbar(13, 9, configuration.epiano.decay , 3, 1, 1);
    ep.setDecay(mapfloat(configuration.epiano.decay, EP_DECAY_MIN, EP_DECAY_MAX, 0, 1.0));
    print_small_intbar(13, 10, configuration.epiano.release , 4, 1, 1);
    ep.setRelease(mapfloat(configuration.epiano.release, EP_RELEASE_MIN, EP_RELEASE_MAX, 0, 1.0));
    print_small_intbar(13, 11, configuration.epiano.hardness , 5, 1, 1);
    ep.setHardness(mapfloat(configuration.epiano.hardness, EP_HARDNESS_MIN, EP_HARDNESS_MAX, 0, 1.0));
    print_small_intbar(13, 12, configuration.epiano.treble, 6, 1, 1);
    ep.setTreble(mapfloat(configuration.epiano.treble, EP_TREBLE_MIN, EP_TREBLE_MAX, 0, 1.0));
    print_small_intbar(13, 13, configuration.epiano.stereo , 7, 1, 1);
    ep.setStereo(mapfloat(configuration.epiano.stereo, EP_STEREO_MIN, EP_STEREO_MAX, 0, 1.0));
    setModeColor(8);
    setCursor_textGrid_mini(13, 15);
    seq_print_formatted_number_signed( configuration.epiano.tune - 100.0, 2);
#if defined(USE_EPIANO)
    ep.setTune((configuration.epiano.tune - 100) / 100.0);
#endif
    print_small_intbar(13, 16, configuration.epiano.detune , 9, 1, 1);
    ep.setDetune(mapfloat(configuration.epiano.detune, EP_DETUNE_MIN, EP_DETUNE_MAX, 0, 1.0));
    setModeColor(10);
    setCursor_textGrid_mini(17, 20);
    seq_print_formatted_number(configuration.epiano.polyphony, 2);
    ep.setPolyphony(configuration.epiano.polyphony);
    print_small_intbar(17, 21, configuration.epiano.velocity_sense , 11, 0, 1);
    ep.setVelocitySense(mapfloat(configuration.epiano.velocity_sense, EP_VELOCITY_SENSE_MIN, EP_VELOCITY_SENSE_MAX, 0, 1.0));
    print_small_intbar(34, 3, configuration.epiano.overdrive , 12, 1, 0);
    ep.setOverdrive(mapfloat(configuration.epiano.overdrive, EP_OVERDRIVE_MIN, EP_OVERDRIVE_MAX, 0, 1.0));
    print_small_intbar(34, 6, configuration.epiano.pan_tremolo , 13, 1, 0);
    if (configuration.epiano.pan_tremolo == 0)
      ep.setPanTremolo(0.0);
    else
      ep.setPanTremolo(mapfloat(configuration.epiano.pan_tremolo, EP_PAN_TREMOLO_MIN, EP_PAN_TREMOLO_MAX, 0.0, 1.0));
    print_small_intbar(34, 7, configuration.epiano.pan_lfo , 14, 1, 0);
    if (configuration.epiano.pan_lfo == 0)
      ep.setPanLFO(0.0);
    else
      ep.setPanLFO(mapfloat(configuration.epiano.pan_lfo, EP_PAN_LFO_MIN, EP_PAN_LFO_MAX, 0.0, 1.0));
    print_small_intbar(34, 10, configuration.fx.ep_chorus_frequency , 15, 1, 0);
#if defined(USE_EPIANO)
    ep_chorus_modulator.frequency(configuration.fx.ep_chorus_frequency / 10.0);
#endif
    setModeColor(16);
    setCursor_textGrid_mini(34, 11);
    switch (configuration.fx.ep_chorus_waveform)
    {
      case 0:
#if defined(USE_EPIANO)
        ep_chorus_modulator.begin(WAVEFORM_TRIANGLE);
#endif
        display.print(F("TRIANGLE"));
        break;
      case 1:
#if defined(USE_EPIANO)
        ep_chorus_modulator.begin(WAVEFORM_SINE);
#endif
        display.print(F("SINE    "));
        break;
      default:
#if defined(USE_EPIANO)
        ep_chorus_modulator.begin(WAVEFORM_TRIANGLE);
#endif
        display.print(F("TRIANGLE"));
        break;
    }
    print_small_intbar(34, 12, configuration.fx.ep_chorus_depth , 17, 1, 0);
    ep_chorus_modulator.amplitude(mapfloat(configuration.fx.ep_chorus_depth, EP_CHORUS_DEPTH_MIN, EP_CHORUS_DEPTH_MAX, 0.0, 1.0));
    print_small_intbar(34, 13, configuration.fx.ep_chorus_level , 18, 1, 0);
#if defined(USE_EPIANO)
    ep_chorus_mixer_r.gain(1, mapfloat(configuration.fx.ep_chorus_level, EP_CHORUS_LEVEL_MIN, EP_CHORUS_LEVEL_MAX, 0.0, 0.5));
    ep_chorus_mixer_l.gain(1, mapfloat(configuration.fx.ep_chorus_level, EP_CHORUS_LEVEL_MIN, EP_CHORUS_LEVEL_MAX, 0.0, 0.5));
#endif
    print_small_intbar(34, 15, configuration.fx.ep_reverb_send , 19, 1, 0);
    reverb_mixer_r.gain(REVERB_MIX_CH_EPIANO, volume_transform(mapfloat(configuration.fx.ep_reverb_send, EP_REVERB_SEND_MIN, EP_REVERB_SEND_MAX, 0.0, VOL_MAX_FLOAT)));
    reverb_mixer_l.gain(REVERB_MIX_CH_EPIANO, volume_transform(mapfloat(configuration.fx.ep_reverb_send, EP_REVERB_SEND_MIN, EP_REVERB_SEND_MAX, 0.0, VOL_MAX_FLOAT)));
    setModeColor(20);
    setCursor_textGrid_mini(37, 19);
    getNoteName(note_name, configuration.epiano.lowest_note);
    display.print(note_name);
    setCursor_textGrid_mini(42, 19);
    display.setTextColor(GREY2, COLOR_BACKGROUND);
    seq_print_formatted_number( configuration.epiano.lowest_note, 3);
    setModeColor(21);
    setCursor_textGrid_mini(37, 20);
    getNoteName(note_name, configuration.epiano.highest_note);
    display.print(note_name);
    setCursor_textGrid_mini(42, 20);
    display.setTextColor(GREY2, COLOR_BACKGROUND);
    seq_print_formatted_number( configuration.epiano.highest_note, 3);
    setModeColor(22);
    setCursor_textGrid_mini(37, 21);
    if (configuration.epiano.midi_channel == 0)
    {
      display.print(F("OMNI"));
    }
    else
    {
      seq_print_formatted_number( configuration.epiano.midi_channel, 2);
      display.print(F("  "));
    }
    //debug
    //    display.setTextColor(COLOR_BACKGROUND, COLOR_SYSTEXT);
    //    setCursor_textGrid_mini(37, 22);
    //    seq_print_formatted_number(seq.temp_select_menu, 2);
    //    setCursor_textGrid_mini(44, 22);
    //    seq_print_formatted_number(seq_active_function, 2);
  }
  if (LCDML.FUNC_close())     // ****** STABLE END *********
  {
    seq.menu = 0;
    seq_active_function = 99;
    encoderDir[ENC_R].reset();
    display.fillScreen(COLOR_BACKGROUND);
  }
}

void UI_func_microsynth(uint8_t param)
{

  if (LCDML.FUNC_setup())         // ****** SETUP *********
  {
    // setup function
    encoderDir[ENC_R].reset();
    display.fillScreen(COLOR_BACKGROUND);

    if (seq.cycle_touch_element != 1)
    {
      draw_button_on_grid(45, 1, "", "", 99); //print keyboard icon
      microsynth_refresh_lower_screen_static_text();
    }
    else
    {
      draw_button_on_grid(45, 1, "MORE" , "PARAM.", 0);
      virtual_keyboard();
      virtual_keyboard_print_buttons();
      virtual_keyboard_print_current_instrument();
    }

    display.setTextSize(1);

    seq_active_function = 0;
    setCursor_textGrid_mini(1, 1);
    display.setTextColor(RED);
    display.print(F("MICROSYNTH"));
    update_microsynth_instance_icons();

    setCursor_textGrid_mini(1, 3);
    display.print(F("VOLUME"));
    setCursor_textGrid_mini(1, 4);
    display.print(F("WAVE"));
    setCursor_textGrid_mini(1, 5);
    display.print(F("COARSE"));
    setCursor_textGrid_mini(1, 6);
    display.print(F("DETUNE"));

    setCursor_textGrid_mini(22, 1);
    display.print(F("NOISE VOL"));
    setCursor_textGrid_mini(22, 2);
    display.print(F("NOISE DCY"));
    setCursor_textGrid_mini(22, 3);
    display.print(F("TRIG.WITH"));

    setCursor_textGrid_mini(22, 4);
    display.print(F("FILTER"));
    setCursor_textGrid_mini(22, 5);
    display.print(F("FREQ"));
    setCursor_textGrid_mini(22, 6);
    display.print(F("RES"));
    setCursor_textGrid_mini(32, 6);
    display.print(F("SPEED"));

    setCursor_textGrid_mini(1, 8);
    display.print(F("ATTACK"));
    setCursor_textGrid_mini(1, 9);
    display.print(F("DECAY"));
    setCursor_textGrid_mini(1, 10);
    display.print(F("SUSTAIN"));
    setCursor_textGrid_mini(1, 11);
    display.print(F("RELEASE"));

    setCursor_textGrid_mini(22, 8);
    display.print(F("OSC LFO"));
    setCursor_textGrid_mini(22, 9);
    display.print(F("MODE"));
    setCursor_textGrid_mini(22, 10);
    display.print(F("DELAY"));
    setCursor_textGrid_mini(32, 10);
    display.print(F("SPEED"));

    update_pwm_text();

    setCursor_textGrid_mini(13, 5);
    display.print(F("STEPS"));
    setCursor_textGrid_mini(13, 6);
    display.print(F("CENTS"));
    setCursor_textGrid_mini(16, 8);
    display.print(F("MS"));
    setCursor_textGrid_mini(16, 9);
    display.print(F("MS"));
    setCursor_textGrid_mini(16, 10);
    display.print(F("LEVL"));
    setCursor_textGrid_mini(16, 11);
    display.print(F("MS"));

    // arrows
    display.setTextColor(GREY1, COLOR_BACKGROUND);
    setCursor_textGrid_mini(36, 5);
    display.print(F(">"));
  }
  if (LCDML.FUNC_loop())          // ****** LOOP *********
  {

    if ((LCDML.BT_checkDown() && encoderDir[ENC_R].Down()) || (LCDML.BT_checkUp() && encoderDir[ENC_R].Up()))
    {
      if (LCDML.BT_checkDown())
      {
        if ( seq_active_function == 0 )
          seq.temp_select_menu = constrain(seq.temp_select_menu + 1, 0, 32);
        else if ( seq.temp_select_menu == 0 )
          microsynth[microsynth_selected_instance].sound_intensity = constrain(microsynth[microsynth_selected_instance].sound_intensity + 1, 0, 100);
        else if ( seq.temp_select_menu == 1 )
        {
          microsynth[microsynth_selected_instance].wave = constrain(microsynth[microsynth_selected_instance].wave + 1, 0, 8);
          update_pwm_text();
        }
        else if ( seq.temp_select_menu == 2 )
          microsynth[microsynth_selected_instance].coarse = constrain(microsynth[microsynth_selected_instance].coarse + 1, -36, 36);
        else if ( seq.temp_select_menu == 3 )
          microsynth[microsynth_selected_instance].detune = constrain(microsynth[microsynth_selected_instance].detune + 1, -99, 99);
        else if ( seq.temp_select_menu == 4 )
          microsynth[microsynth_selected_instance].env_attack = constrain(microsynth[microsynth_selected_instance].env_attack + 1, 0, 254);
        else if ( seq.temp_select_menu == 5 )
          microsynth[microsynth_selected_instance].env_decay = constrain(microsynth[microsynth_selected_instance].env_decay + 1, 0, 254);
        else if ( seq.temp_select_menu == 6 )
          microsynth[microsynth_selected_instance].env_sustain = constrain(microsynth[microsynth_selected_instance].env_sustain + 1, 0, 50);
        else if ( seq.temp_select_menu == 7 )
          microsynth[microsynth_selected_instance].env_release = constrain(microsynth[microsynth_selected_instance].env_release + 1, 0, 99);
        else if ( seq.temp_select_menu == 8 )
          microsynth[microsynth_selected_instance].filter_osc_mode = constrain(microsynth[microsynth_selected_instance].filter_osc_mode + 1, 0, 3);
        else if ( seq.temp_select_menu == 9 )
          microsynth[microsynth_selected_instance].filter_osc_freq_from = constrain(microsynth[microsynth_selected_instance].filter_osc_freq_from + 80, 0, 15000);
        else if ( seq.temp_select_menu == 10 )
          microsynth[microsynth_selected_instance].filter_osc_freq_to = constrain(microsynth[microsynth_selected_instance].filter_osc_freq_to + 80, 0, 15000);
        else if ( seq.temp_select_menu == 11 )
          microsynth[microsynth_selected_instance].filter_osc_resonance = constrain(microsynth[microsynth_selected_instance].filter_osc_resonance + 1, 0, 99);
        else if ( seq.temp_select_menu == 12 )
          microsynth[microsynth_selected_instance].filter_osc_speed = constrain(microsynth[microsynth_selected_instance].filter_osc_speed + 5, 0, 999);
        else if ( seq.temp_select_menu == 13 )
          microsynth[microsynth_selected_instance].pwm_from = constrain(microsynth[microsynth_selected_instance].pwm_from + 5, 0, 999);
        else if ( seq.temp_select_menu == 14 )
          microsynth[microsynth_selected_instance].pwm_to = constrain(microsynth[microsynth_selected_instance].pwm_to + 5, 0, 999);
        else if ( seq.temp_select_menu == 15 )
          microsynth[microsynth_selected_instance].pwm_speed = constrain(microsynth[microsynth_selected_instance].pwm_speed + 1, 0, 99);
        else if ( seq.temp_select_menu == 16 )
          microsynth[microsynth_selected_instance].noise_vol = constrain(microsynth[microsynth_selected_instance].noise_vol + 1, 0, 100);
        else if ( seq.temp_select_menu == 17 )
          microsynth[microsynth_selected_instance].noise_decay = constrain(microsynth[microsynth_selected_instance].noise_decay + 1, 0, 100);
        else if ( seq.temp_select_menu == 18 )
          microsynth[microsynth_selected_instance].trigger_noise_with_osc = !microsynth[microsynth_selected_instance].trigger_noise_with_osc;
        else if ( seq.temp_select_menu == 19 )
          microsynth[microsynth_selected_instance].filter_noise_mode = constrain(microsynth[microsynth_selected_instance].filter_noise_mode + 1, 0, 3);
        else if ( seq.temp_select_menu == 20 )
          microsynth[microsynth_selected_instance].filter_noise_freq_from = constrain(microsynth[microsynth_selected_instance].filter_noise_freq_from + 100, 0, 15000);
        else if ( seq.temp_select_menu == 21 )
          microsynth[microsynth_selected_instance].filter_noise_freq_to = constrain(microsynth[microsynth_selected_instance].filter_noise_freq_to + 100, 0, 15000);
        else if ( seq.temp_select_menu == 22 )
          microsynth[microsynth_selected_instance].filter_noise_resonance = constrain(microsynth[microsynth_selected_instance].filter_noise_resonance + 1, 0, 99);
        else if ( seq.temp_select_menu == 23 )
          microsynth[microsynth_selected_instance].filter_noise_speed = constrain(microsynth[microsynth_selected_instance].filter_noise_speed + 6, 0, 999);
        else if ( seq.temp_select_menu == 24 )
          microsynth[microsynth_selected_instance].lfo_intensity = constrain(microsynth[microsynth_selected_instance].lfo_intensity + 1, 0, 254);
        else if ( seq.temp_select_menu == 25 )
          microsynth[microsynth_selected_instance].lfo_mode = constrain(microsynth[microsynth_selected_instance].lfo_mode + 1, 0, 2);
        else if ( seq.temp_select_menu == 26 )
          microsynth[microsynth_selected_instance].lfo_delay = constrain(microsynth[microsynth_selected_instance].lfo_delay + 1, 0, 254);
        else if ( seq.temp_select_menu == 27 )
          microsynth[microsynth_selected_instance].lfo_speed = constrain(microsynth[microsynth_selected_instance].lfo_speed + 1, 0, 254);
        else if ( seq.temp_select_menu == 28 )
          microsynth[microsynth_selected_instance].rev_send = constrain(microsynth[microsynth_selected_instance].rev_send + 1, 0, 127);
        else if ( seq.temp_select_menu == 29 )
          microsynth[microsynth_selected_instance].chorus_send = constrain(microsynth[microsynth_selected_instance].chorus_send + 1, 0, 127);
        else if ( seq.temp_select_menu == 30 )
          microsynth[microsynth_selected_instance].delay_send = constrain(microsynth[microsynth_selected_instance].delay_send + 1, 0, 127);
        else if ( seq.temp_select_menu == 31 )
          microsynth[microsynth_selected_instance].pan = constrain(microsynth[microsynth_selected_instance].pan + 1, PANORAMA_MIN, PANORAMA_MAX);
        else if ( seq.temp_select_menu == 32 )
          microsynth[microsynth_selected_instance].midi_channel = constrain(microsynth[microsynth_selected_instance].midi_channel + 1, 1, 15);
      }
      else if (LCDML.BT_checkUp())
      {
        if ( seq_active_function == 0 )
          seq.temp_select_menu = constrain(seq.temp_select_menu - 1, 0, 32);
        else if ( seq.temp_select_menu == 0 )
          microsynth[microsynth_selected_instance].sound_intensity = constrain(microsynth[microsynth_selected_instance].sound_intensity - 1, 0, 100);
        else if ( seq.temp_select_menu == 1 )
        {
          microsynth[microsynth_selected_instance].wave = constrain(microsynth[microsynth_selected_instance].wave - 1, 0, 8);
          update_pwm_text();
        }
        else if ( seq.temp_select_menu == 2 )
          microsynth[microsynth_selected_instance].coarse = constrain(microsynth[microsynth_selected_instance].coarse - 1, -36, 36);
        else if ( seq.temp_select_menu == 3 )
          microsynth[microsynth_selected_instance].detune = constrain(microsynth[microsynth_selected_instance].detune - 1, -127, 127);
        else if ( seq.temp_select_menu == 4 )
          microsynth[microsynth_selected_instance].env_attack = constrain(microsynth[microsynth_selected_instance].env_attack - 1, 0, 254);
        else if ( seq.temp_select_menu == 5 )
          microsynth[microsynth_selected_instance].env_decay = constrain(microsynth[microsynth_selected_instance].env_decay - 1, 0, 254);
        else if ( seq.temp_select_menu == 6 )
          microsynth[microsynth_selected_instance].env_sustain = constrain(microsynth[microsynth_selected_instance].env_sustain - 1, 0, 50);
        else if ( seq.temp_select_menu == 7 )
          microsynth[microsynth_selected_instance].env_release = constrain(microsynth[microsynth_selected_instance].env_release - 1, 0, 99);
        else if ( seq.temp_select_menu == 8 )
          microsynth[microsynth_selected_instance].filter_osc_mode = constrain(microsynth[microsynth_selected_instance].filter_osc_mode - 1, 0, 3);
        else if ( seq.temp_select_menu == 9 )
          microsynth[microsynth_selected_instance].filter_osc_freq_from = constrain(microsynth[microsynth_selected_instance].filter_osc_freq_from - 80, 0, 15000);
        else if ( seq.temp_select_menu == 10 )
          microsynth[microsynth_selected_instance].filter_osc_freq_to = constrain(microsynth[microsynth_selected_instance].filter_osc_freq_to - 80, 0, 15000);
        else if ( seq.temp_select_menu == 11 )
          microsynth[microsynth_selected_instance].filter_osc_resonance = constrain(microsynth[microsynth_selected_instance].filter_osc_resonance - 1, 0, 99);
        else if ( seq.temp_select_menu == 12 )
          microsynth[microsynth_selected_instance].filter_osc_speed = constrain(microsynth[microsynth_selected_instance].filter_osc_speed - 6, 0, 999);
        else if ( seq.temp_select_menu == 13 )
          microsynth[microsynth_selected_instance].pwm_from = constrain(microsynth[microsynth_selected_instance].pwm_from - 5, 0, 999);
        else if ( seq.temp_select_menu == 14 )
          microsynth[microsynth_selected_instance].pwm_to = constrain(microsynth[microsynth_selected_instance].pwm_to - 5, 0, 999);
        else if ( seq.temp_select_menu == 15 )
          microsynth[microsynth_selected_instance].pwm_speed = constrain(microsynth[microsynth_selected_instance].pwm_speed - 1, 0, 99);
        else if ( seq.temp_select_menu == 16 )
          microsynth[microsynth_selected_instance].noise_vol = constrain(microsynth[microsynth_selected_instance].noise_vol - 1, 0, 100);
        else if ( seq.temp_select_menu == 17 )
          microsynth[microsynth_selected_instance].noise_decay = constrain(microsynth[microsynth_selected_instance].noise_decay - 1, 0, 100);
        else if ( seq.temp_select_menu == 18 )
          microsynth[microsynth_selected_instance].trigger_noise_with_osc = !microsynth[microsynth_selected_instance].trigger_noise_with_osc;
        else if ( seq.temp_select_menu == 19 )
          microsynth[microsynth_selected_instance].filter_noise_mode = constrain(microsynth[microsynth_selected_instance].filter_noise_mode - 1, 0, 3);
        else if ( seq.temp_select_menu == 20 )
          microsynth[microsynth_selected_instance].filter_noise_freq_from = constrain(microsynth[microsynth_selected_instance].filter_noise_freq_from - 100, 0, 15000);
        else if ( seq.temp_select_menu == 21 )
          microsynth[microsynth_selected_instance].filter_noise_freq_to = constrain(microsynth[microsynth_selected_instance].filter_noise_freq_to - 100, 0, 15000);
        else if ( seq.temp_select_menu == 22 )
          microsynth[microsynth_selected_instance].filter_noise_resonance = constrain(microsynth[microsynth_selected_instance].filter_noise_resonance - 1, 0, 99);
        else if ( seq.temp_select_menu == 23 )
          microsynth[microsynth_selected_instance].filter_noise_speed = constrain(microsynth[microsynth_selected_instance].filter_noise_speed - 6, 0, 999);
        else if ( seq.temp_select_menu == 24 )
          microsynth[microsynth_selected_instance].lfo_intensity = constrain(microsynth[microsynth_selected_instance].lfo_intensity - 1, 0, 254);
        else if ( seq.temp_select_menu == 25 )
          microsynth[microsynth_selected_instance].lfo_mode = constrain(microsynth[microsynth_selected_instance].lfo_mode - 1, 0, 2);
        else if ( seq.temp_select_menu == 26 )
          microsynth[microsynth_selected_instance].lfo_delay = constrain(microsynth[microsynth_selected_instance].lfo_delay - 1, 0, 254);
        else if ( seq.temp_select_menu == 27 )
          microsynth[microsynth_selected_instance].lfo_speed = constrain(microsynth[microsynth_selected_instance].lfo_speed - 1, 0, 254);
        else if ( seq.temp_select_menu == 28 )
          microsynth[microsynth_selected_instance].rev_send = constrain(microsynth[microsynth_selected_instance].rev_send - 1, 0, 127);
        else if ( seq.temp_select_menu == 29 )
          microsynth[microsynth_selected_instance].chorus_send = constrain(microsynth[microsynth_selected_instance].chorus_send - 1, 0, 127);
        else if ( seq.temp_select_menu == 30 )
          microsynth[microsynth_selected_instance].delay_send = constrain(microsynth[microsynth_selected_instance].delay_send - 1, 0, 127);
        else if ( seq.temp_select_menu == 31 )
          microsynth[microsynth_selected_instance].pan = constrain(microsynth[microsynth_selected_instance].pan - 1, PANORAMA_MIN, PANORAMA_MAX);
        else if ( seq.temp_select_menu == 32 )
          microsynth[microsynth_selected_instance].midi_channel = constrain(microsynth[microsynth_selected_instance].midi_channel - 1, 1, 15);
      }
    }

    if (LCDML.BT_checkEnter()  && encoderDir[ENC_R].ButtonShort())  //handle button presses during menu >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
    {
      if ( seq_active_function == 0 )
        seq_active_function = 1;
      else
        seq_active_function = 0;
    }
    else if (LCDML.BT_checkEnter())
    {
      if (  microsynth_selected_instance == 0)
        microsynth_selected_instance = 1;
      else
        microsynth_selected_instance = 0;
      update_microsynth_instance_icons();
    }

    //button check end <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
    //display.setTextSize(1);
    if (seq_active_function == 1)
      microsynth_update_settings(microsynth_selected_instance);
    if (seq.temp_select_menu == 0)
      display.setTextColor(COLOR_BACKGROUND, COLOR_SYSTEXT); else display.setTextColor(COLOR_SYSTEXT, COLOR_BACKGROUND);
    print_small_intbar(9, 3,  microsynth[microsynth_selected_instance].sound_intensity, 0, 1, 0);
    if (seq.temp_select_menu == 1)
      display.setTextColor(COLOR_BACKGROUND, COLOR_SYSTEXT); else display.setTextColor(RED, COLOR_BACKGROUND);
    setCursor_textGrid_mini(9, 4);
    if (microsynth[microsynth_selected_instance].wave == 0)
      display.print(F("SINE    "));
    else if (microsynth[microsynth_selected_instance].wave == 1)
      display.print(F("TRIANGLE"));
    else if (microsynth[microsynth_selected_instance].wave == 2)
      display.print(F("SAWTOOTH"));
    else if (microsynth[microsynth_selected_instance].wave == 3)
      display.print(F("SQUARE  "));
    else if (microsynth[microsynth_selected_instance].wave == 4)
      display.print(F("PULSE   "));
    else if (microsynth[microsynth_selected_instance].wave == 5)
      display.print(F("LM_SAW  "));
    else if (microsynth[microsynth_selected_instance].wave == 6)
      display.print(F("LM_SQR  "));
    else if (microsynth[microsynth_selected_instance].wave == 7)
      display.print(F("LM_PULSE"));
    else if (microsynth[microsynth_selected_instance].wave == 8)
      display.print(F("SMP&HOLD"));
    if (seq.temp_select_menu == 2)
      display.setTextColor(COLOR_BACKGROUND, COLOR_SYSTEXT); else display.setTextColor(COLOR_SYSTEXT, COLOR_BACKGROUND);
    setCursor_textGrid_mini(9, 5);
    seq_print_formatted_number_signed( microsynth[microsynth_selected_instance].coarse, 2);
    if (seq.temp_select_menu == 3)
      display.setTextColor(COLOR_BACKGROUND, COLOR_SYSTEXT); else display.setTextColor(COLOR_SYSTEXT, COLOR_BACKGROUND);
    setCursor_textGrid_mini(9, 6);
    seq_print_formatted_number_signed( microsynth[microsynth_selected_instance].detune, 2);
    if (seq.temp_select_menu == 4)
    {
      display.setTextColor(COLOR_BACKGROUND, COLOR_SYSTEXT);
    } else
      display.setTextColor(COLOR_SYSTEXT, COLOR_BACKGROUND);
    setCursor_textGrid_mini(10, 8);
    seq_print_formatted_number( microsynth[microsynth_selected_instance].env_attack * 4, 4);
    if (seq.temp_select_menu == 5)
    {
      display.setTextColor(COLOR_BACKGROUND, COLOR_SYSTEXT);
    }
    else display.setTextColor(COLOR_SYSTEXT, COLOR_BACKGROUND);
    setCursor_textGrid_mini(10, 9);
    seq_print_formatted_number( microsynth[microsynth_selected_instance].env_decay * 4, 4);
    if (seq.temp_select_menu == 6)
    {
      display.setTextColor(COLOR_BACKGROUND, COLOR_SYSTEXT);
    } else  display.setTextColor(COLOR_SYSTEXT, COLOR_BACKGROUND);
    setCursor_textGrid_mini(11, 10);
    seq_print_formatted_number( microsynth[microsynth_selected_instance].env_sustain * 2, 3);
    if (seq.temp_select_menu == 7)
    {
      display.setTextColor(COLOR_BACKGROUND, COLOR_SYSTEXT);
    } else display.setTextColor(COLOR_SYSTEXT, COLOR_BACKGROUND);
    setCursor_textGrid_mini(10, 11);
    seq_print_formatted_number( microsynth[microsynth_selected_instance].env_release * microsynth[microsynth_selected_instance].env_release , 4);
    if (seq.cycle_touch_element != 1)
      microsynth_refresh_lower_screen_dynamic_text();
    print_small_intbar(32, 1,  microsynth[microsynth_selected_instance].noise_vol, 16, 1, 0);
    print_small_intbar(32, 2,  microsynth[microsynth_selected_instance].noise_decay, 17, 1, 1);
    if (seq.temp_select_menu == 18)
      display.setTextColor(COLOR_BACKGROUND, COLOR_SYSTEXT); else display.setTextColor(COLOR_SYSTEXT, COLOR_BACKGROUND);
    setCursor_textGrid_mini(32, 3);
    if (microsynth[microsynth_selected_instance].trigger_noise_with_osc)
      display.print (F("OSC."));
    else
      display.print (F("C-8 "));
    if (seq.temp_select_menu == 19)
      display.setTextColor(COLOR_BACKGROUND, COLOR_SYSTEXT); else display.setTextColor(RED, COLOR_BACKGROUND);
    setCursor_textGrid_mini(32, 4);
    if (microsynth[microsynth_selected_instance].filter_noise_mode == 0)
      display.print(F("OFF   "));
    else if (microsynth[microsynth_selected_instance].filter_noise_mode == 1)
      display.print(F("LP12dB"));
    else if (microsynth[microsynth_selected_instance].filter_noise_mode == 2)
      display.print(F("BP12dB"));
    else if (microsynth[microsynth_selected_instance].filter_noise_mode == 3)
      display.print(F("HI12dB"));
    print_small_intbar(32, 5, microsynth[microsynth_selected_instance].filter_noise_freq_from / 100, 20, 0, 1);
    print_small_intbar(38, 5, microsynth[microsynth_selected_instance].filter_noise_freq_to / 100, 21, 0, 1);
    print_small_intbar(27, 6, microsynth[microsynth_selected_instance].filter_noise_resonance, 22, 0, 1);
    print_small_intbar(38, 6, microsynth[microsynth_selected_instance].filter_noise_speed / 10, 23, 0, 1);
    print_small_intbar(32, 8, microsynth[microsynth_selected_instance].lfo_intensity, 24, 0, 1);

    if (seq.temp_select_menu == 25)
    {
      display.setTextColor(COLOR_BACKGROUND, COLOR_SYSTEXT);
    } else display.setTextColor(COLOR_SYSTEXT, COLOR_BACKGROUND);
    setCursor_textGrid_mini(29, 9);
    seq_print_formatted_number(microsynth[microsynth_selected_instance].lfo_mode , 2);

    if (microsynth[microsynth_selected_instance].lfo_mode == 0)
    {
      display.setTextColor(COLOR_SYSTEXT, RED);
    }
    else display.setTextColor(GREY1, GREY3);

    setCursor_textGrid_mini(32, 9);
    display.print(F("U&D"));
    if (microsynth[microsynth_selected_instance].lfo_mode == 1)
    {
      display.setTextColor(COLOR_SYSTEXT, RED);
    }
    else display.setTextColor(GREY1, GREY3);
    setCursor_textGrid_mini(36, 9);
    display.print(F("1U"));

    if (microsynth[microsynth_selected_instance].lfo_mode == 2)
    {
      display.setTextColor(COLOR_SYSTEXT, RED);
    }
    else display.setTextColor(GREY1, GREY3);
    setCursor_textGrid_mini(39, 9);
    display.print(F("1D"));
    print_small_intbar(28, 10, microsynth[microsynth_selected_instance].lfo_delay, 26, 0, 1);
    print_small_intbar(38, 10, microsynth[microsynth_selected_instance].lfo_speed, 27, 0, 1);
  }
  if (LCDML.FUNC_close())     // ****** STABLE END *********
  {
    seq.menu = 0;
    seq_active_function = 99;
    encoderDir[ENC_R].reset();
    display.fillScreen(COLOR_BACKGROUND);
  }
}

void tracker_print_pattern(int xpos, int ypos, uint8_t track_number)
{
  uint8_t yspacer = CHAR_height_small + 3;
  uint8_t ycount = 0;
  display.setTextSize(1);
  for (uint8_t y = 0; y < 16; y++)
  {
    // print data byte of current step
    if (track_number == seq.selected_track && y == seq.scrollpos)  //print velocity of active pattern-step
    {
      if (seq.edit_state)
        display.setTextColor( COLOR_SYSTEXT, RED);
      else
        display.setTextColor( COLOR_BACKGROUND, COLOR_SYSTEXT);
      display.setCursor(22 * CHAR_width_small, DISPLAY_HEIGHT - CHAR_height_small);
      seq_print_formatted_number(  seq.vel[ seq.current_pattern[track_number]][y]  , 3);
      display.setCursor(34 * CHAR_width_small, DISPLAY_HEIGHT - CHAR_height_small);
      if (seq.vel[ seq.current_pattern[track_number]][y] < 128)
        display.print( "VELOCITY");
      else if (seq.vel[ seq.current_pattern[track_number]][y] > 209)
        display.print( "PITCHSMP");
      else if (seq.vel[ seq.current_pattern[track_number]][y] > 199)
        display.print( "CHORD   ");
      else
        display.print( "OTHER   ");
    }

    /// -- update all other columns --

    display.setCursor(xpos, ypos + ycount * yspacer);
    if (seq.note_data[ seq.current_pattern[track_number]][y] != 99 && seq.note_data[ seq.current_pattern[track_number]][y] != 0)
    {
      if (seq.edit_state && track_number == seq.selected_track && y == seq.scrollpos)
        display.setTextColor( COLOR_SYSTEXT, RED);
      else if (track_number == seq.selected_track && y == seq.scrollpos)
        display.setTextColor( COLOR_BACKGROUND, COLOR_SYSTEXT);
      else
        set_pattern_content_type_color( seq.current_pattern[track_number] );
      display.print( tracker_find_shortname_from_pattern_step( track_number,   seq.current_pattern[track_number] , y )[0]);
    }
    else
    {
      if (seq.edit_state && track_number == seq.selected_track && y == seq.scrollpos)
        display.setTextColor( COLOR_SYSTEXT, RED);
      else if (track_number == seq.selected_track && y == seq.scrollpos)
        display.setTextColor( COLOR_BACKGROUND, COLOR_SYSTEXT);
      else
        display.setTextColor(GREY3, COLOR_BACKGROUND);
      display.print( "-");
    }
    display.setCursor(xpos + 2 * CHAR_width_small, ypos + ycount * yspacer);
    if (seq.note_data[ seq.current_pattern[track_number]][y] != 99 && seq.note_data[ seq.current_pattern[track_number]][y] != 0
        && seq.note_data[ seq.current_pattern[track_number]][y] != 130 )
    {
      if (seq.edit_state && track_number == seq.selected_track && y == seq.scrollpos)
        display.setTextColor( COLOR_SYSTEXT, RED);
      else if (track_number == seq.selected_track && y == seq.scrollpos)
        display.setTextColor( COLOR_BACKGROUND, COLOR_SYSTEXT);
      else
        set_pattern_content_type_color( seq.current_pattern[track_number] );
      seq_print_formatted_number(  seq.note_data[ seq.current_pattern[track_number]][y]  , 3);
    }
    else if (seq.note_data[ seq.current_pattern[track_number]][y] == 0 )  //empty
    {
      if (seq.edit_state && track_number == seq.selected_track && y == seq.scrollpos)
        display.setTextColor( COLOR_SYSTEXT, RED);
      else if (track_number == seq.selected_track && y == seq.scrollpos)
        display.setTextColor( COLOR_BACKGROUND, COLOR_SYSTEXT);
      else
        display.setTextColor(GREY2, COLOR_BACKGROUND);
      seq_print_formatted_number(  seq.note_data[ seq.current_pattern[track_number]][y]  , 3);
    }
    else if (seq.note_data[ seq.current_pattern[track_number]][y] == 130 )  //Latch
    {
      if (seq.edit_state && track_number == seq.selected_track && y == seq.scrollpos)
        display.setTextColor( COLOR_SYSTEXT, RED);
      else if (track_number == seq.selected_track && y == seq.scrollpos)
        display.setTextColor(COLOR_BACKGROUND, COLOR_SYSTEXT_ACCENT);
      else
        display.setTextColor(COLOR_SYSTEXT_ACCENT, COLOR_BACKGROUND);
      display.print( "LAT");
    }
    else
    {
      if (seq.edit_state && track_number == seq.selected_track && y == seq.scrollpos)
        display.setTextColor( COLOR_SYSTEXT, RED);
      else if (track_number == seq.selected_track && y == seq.scrollpos)
        display.setTextColor(COLOR_BACKGROUND, COLOR_SYSTEXT);
      else
        display.setTextColor(GREY2, COLOR_BACKGROUND);
      display.print( "---");
    }
    ycount++;
  }
}

void UI_func_seq_tracker(uint8_t param)
{
  if (LCDML.FUNC_setup())         // ****** SETUP *********
  {
    // setup function
    seq.menu = 0;
    seq.edit_state = false;
    display.setTextSize(1);
    display.fillScreen(COLOR_BACKGROUND);
    UI_toplineInfoText( 1);
    display.setCursor(1, 2);
    display.setTextColor(COLOR_SYSTEXT);
    display.print (F("TRACKER "));
    display.setCursor(CHAR_width_small * 10, 2);
    display.print (F("CHAIN"));
    display.setCursor(CHAR_width_small * 41, 2);
    display.print (F("SONG"));
    display.setCursor(CHAR_width_small * 49, 2);
    display.print (F("/"));
    display.setCursor(CHAR_width_small * 46, 2); //print song step at init
    display.setTextColor(COLOR_BACKGROUND, COLOR_PITCHSMP);
    seq_print_formatted_number(  seq.current_song_step , 2);
    display.setCursor(CHAR_width_small * 51, 2);
    seq_print_formatted_number(  get_song_length() , 2);
    display.setCursor(CHAR_width_small * 22, 2);

    for (uint8_t d = 0; d < NUM_SEQ_TRACKS; d++)  //print chain steps
    {
      display.setCursor(CHAR_width_small * 16 + (CHAR_width_small * 3)*d , 2);
      seq_print_formatted_number(  seq.chain_counter[d]  , 2);
      //  display.setCursor(CHAR_width_small * 16+ (CHAR_width_small*3)*d , 2);
      //  seq_print_formatted_number( get_chain_length_from_current_track(d)  , 2);
    }

    for (uint8_t x = 0; x < NUM_SEQ_TRACKS; x++)
    {
      seq.current_chain[x] = seq.song[x][seq.current_song_step];
      seq.current_pattern[x] = seq.chain[  seq.current_chain[x] ] [ seq.chain_counter[x] ];

      setCursor_textGrid_mini(6 + 6 * x, 2);
      display.setTextColor(COLOR_SYSTEXT, COLOR_BACKGROUND);
      display.print (F("TRK:"));
      display.print (x + 1);
      setCursor_textGrid_mini(6 + 6 * x, 3);
      display.print ("PAT");
      update_pattern_number_in_tracker(x);
    }
    display.setTextColor(DARKGREEN, COLOR_BACKGROUND);
    for (uint8_t y = 0; y < 16; y++) {
      display.setCursor(0, 45 + y * (CHAR_height_small + 3));
      seq_print_formatted_number(y, 2);
    }
    display.setCursor(CHAR_width_small * 11, DISPLAY_HEIGHT - CHAR_height_small);
    display.print ("DATA BYTE: ");
    display.setCursor(CHAR_width_small * 26, DISPLAY_HEIGHT - CHAR_height_small);
    display.print ("USED AS");

    display.setTextColor(GREEN, COLOR_BACKGROUND);
    display.setCursor(5 * CHAR_width_small, (5 + seq.step) * (CHAR_height_small + 3) - 7);
    display.print (F(">"));
    encoderDir[ENC_R].reset();
  }
  if (LCDML.FUNC_loop())          // ****** LOOP *********
  {

    if ((LCDML.BT_checkDown() && encoderDir[ENC_R].Down()) || (LCDML.BT_checkUp() && encoderDir[ENC_R].Up()) || (LCDML.BT_checkEnter() && encoderDir[ENC_R].ButtonShort()))
    {
      if (seq.edit_state == false)  // NOT in editor mode
      {
        if (LCDML.BT_checkDown())
        {
          seq.scrollpos++;
          if (seq.scrollpos > 15)
            seq.scrollpos = 15;
        }
        else if (LCDML.BT_checkUp())
        {
          if (seq.scrollpos > 0 )
          {
            seq.scrollpos--;
          }
        }
      }
      else  // IS in editor mode
      {
        if (LCDML.BT_checkDown())
        {

          seq.note_data[ seq.current_pattern[seq.selected_track]][seq.scrollpos] = constrain(seq.note_data[ seq.current_pattern[seq.selected_track]][seq.scrollpos] + ENCODER[ENC_R].speed(), 0, 254);
        }
        else if (LCDML.BT_checkUp())
        {
          seq.note_data[ seq.current_pattern[seq.selected_track]][seq.scrollpos] = constrain(seq.note_data[ seq.current_pattern[seq.selected_track]][seq.scrollpos] - ENCODER[ENC_R].speed(), 0, 254);
        }

      }
      if (LCDML.BT_checkEnter())  //handle button presses during menu >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
      {
        seq.edit_state = !seq.edit_state;
      }

    }
    for (uint8_t x = 0; x < NUM_SEQ_TRACKS; x++)
    {
      seq.current_chain[x] = seq.song[x][seq.current_song_step];
      seq.current_pattern[x] = seq.chain[  seq.current_chain[x] ] [ seq.chain_counter[x] ];
      // update_pattern_number_in_tracker(x);
    }

    display.setTextSize(1);
    setCursor_textGrid_mini(1, 2);
    if (seq.edit_state)
    {
      display.setTextColor(RED, COLOR_BACKGROUND);
      display.print ("EDIT");
      helptext_l("< > DATA");
      helptext_r("< > NOTE");
    }
    else
    {
      display.setTextColor(GREEN, COLOR_BACKGROUND);
      display.print ("PLAY");
      helptext_l("< > MOVE X");
      helptext_r("< > MOVE Y");
    }
    for (uint8_t d = 0; d < NUM_SEQ_TRACKS; d++)
    {
      tracker_print_pattern(  (6 + 6 * d) * CHAR_width_small , 48,  d);
    }

  }
  if (LCDML.FUNC_close())     // ****** STABLE END *********
  {
    encoderDir[ENC_R].reset();
    display.fillScreen(COLOR_BACKGROUND);
  }
}

void pattern_preview_in_song(uint8_t patternno)
{
  display.setTextSize(1);
  seq.active_pattern = patternno;
  display.setCursor (0, DISPLAY_HEIGHT - CHAR_height_small);
  display.setTextColor(COLOR_SYSTEXT, COLOR_CHORDS);
  if (patternno == 99)
  {
    display.print(F("EMPTY "));
  } else
  {
    display.print("PAT:");
    seq_print_formatted_number( patternno , 2);
  }
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
    if (patternno == 99)
      display.print(F(" "));
    else
      display.print(seq_find_shortname(i)[0]);
  }
  display.setTextColor(GREY1, GREY4);
  display.print("]");
}

void sub_song_print_tracknumbers()
{
  for (uint8_t x = 0; x < NUM_SEQ_TRACKS; x++)
  {
    display.setCursor(6 * CHAR_width_small + (4 * CHAR_width_small)*x ,  CHAR_height_small * 4 );
    if (seq.tracktype_or_instrument_assign == 0)
      set_track_type_color(x);
    else
      display.setTextColor(GREY3, COLOR_BACKGROUND);

    display.print (F("T"));
    display.print (x + 1);
  }
}

void sub_song_print_tracktypes()
{
  for (uint8_t x = 0; x < NUM_SEQ_TRACKS; x++)
  {
    display.setCursor(6 * CHAR_width_small + (4 * CHAR_width_small)*x ,  CHAR_height_small * 4 );
    if (seq.tracktype_or_instrument_assign == 0)
      set_track_type_color(x);
    else  if (seq.tracktype_or_instrument_assign == 5 && seq.selected_track == x )
      display.setTextColor( COLOR_BACKGROUND, GREEN);
    else  if (seq.tracktype_or_instrument_assign == 5 && seq.selected_track != x )
      display.setTextColor( MIDDLEGREEN, COLOR_BACKGROUND);
    else  if (seq.tracktype_or_instrument_assign == 6 && seq.selected_track == x )
      display.setTextColor( COLOR_SYSTEXT, MIDDLEGREEN);
    else
      display.setTextColor(GREY3, COLOR_BACKGROUND);
    display.setCursor(6 * CHAR_width_small + (4 * CHAR_width_small)*x ,  CHAR_height_small * 5 );
    if (seq.track_type[x] == 0 )   display.print(F("DRM"));
    else if (seq.track_type[x] == 1 ) display.print(F("INS"));
    else if (seq.track_type[x] == 2 ) display.print(F("CHD"));
    else if (seq.track_type[x] == 3 ) display.print(F("ARP"));
    else
    {
      display.setTextColor(RED, COLOR_BACKGROUND);
      display.print("???");
    }
  }
}

void sub_song_print_instruments(uint16_t front, uint16_t back)
{
  for (uint8_t x = 0; x < NUM_SEQ_TRACKS; x++)
  {
    if (seq.tracktype_or_instrument_assign == 2 && seq.selected_track == x && seq.track_type[x] != 0 )
      display.setTextColor( COLOR_SYSTEXT, MIDDLEGREEN);
    else if (seq.tracktype_or_instrument_assign == 1 && seq.selected_track == x && seq.track_type[x] != 0 )
      display.setTextColor( COLOR_BACKGROUND, GREEN);
    else if (seq.tracktype_or_instrument_assign == 1 && seq.selected_track != x && seq.track_type[x] != 0 )
      display.setTextColor(  MIDDLEGREEN, COLOR_BACKGROUND);
    else  if (seq.tracktype_or_instrument_assign == 1 && seq.selected_track == x && seq.track_type[x] == 0 )
      display.setTextColor( COLOR_BACKGROUND, RED);
    else if (seq.tracktype_or_instrument_assign == 0 || seq.tracktype_or_instrument_assign == 5 )
      display.setTextColor(front, back);
    else
      display.setTextColor(0x6000, COLOR_BACKGROUND);
    display.setCursor(6 * CHAR_width_small + (4 * CHAR_width_small)*x ,  CHAR_height_small * 6 );
    if (seq.track_type[x] != 0)
    {
      if (seq.instrument[x] == 0 )  display.print(F("DX1"));
      else if (seq.instrument[x] == 1 )  display.print(F("DX2"));
      else if (seq.instrument[x] == 2 )  display.print(F("EP "));
      else if (seq.instrument[x] == 3 )  display.print(F("MS1"));
      else if (seq.instrument[x] == 4 )  display.print(F("MS2"));
      else if (seq.instrument[x] > 4 && seq.instrument[x] < 21)
      {
        if (seq.tracktype_or_instrument_assign == 2)
        {
          display.setCursor(6 * CHAR_width_small + (4 * CHAR_width_small)*x ,  CHAR_height_small * 5 );
          display.print(F("USB"));
        }
        display.setCursor(6 * CHAR_width_small + (4 * CHAR_width_small)*x ,  CHAR_height_small * 6 );
        display.print(F("#"));
        seq_print_formatted_number(seq.instrument[x] - 4, 2);
      }
      else if (seq.instrument[x] > 20 && seq.instrument[x] < 37)
      {
        if (seq.tracktype_or_instrument_assign == 2)
        {
          display.setCursor(6 * CHAR_width_small + (4 * CHAR_width_small)*x ,  CHAR_height_small * 5 );
          display.print(F("DIN"));
        }
        display.setCursor(6 * CHAR_width_small + (4 * CHAR_width_small)*x ,  CHAR_height_small * 6 );
        display.print(F("#"));
        seq_print_formatted_number(seq.instrument[x] - 20, 2);
      }
      else display.print(F("???"));
    }
    else
    {
      if (seq.tracktype_or_instrument_assign == 0)
        display.print(F("   "));
      else
        display.print(F("DRM"));
    }
  }
}

void print_empty_spaces (uint8_t spaces)
{
  display.setTextColor(COLOR_SYSTEXT, COLOR_BACKGROUND);
  for (uint8_t x = 0; x < spaces; x++)
  {
    display.print(" ");
  }
}

void print_song_loop ()
{
  //print loop
  display.setCursor(CHAR_width_small * 11, 10);
  display.setTextColor(COLOR_BACKGROUND, COLOR_PITCHSMP);
  if (seq.loop_start != 99)
    seq_print_formatted_number( seq.loop_start + 1 , 2);
  else
    display.print(F("--"));

  display.setCursor(CHAR_width_small * 14, 10);
  display.print(F("-"));

  display.setCursor(CHAR_width_small * 16, 10);
  if (seq.loop_end != 99)
    seq_print_formatted_number( seq.loop_end + 1 , 2);
  else
    display.print(F("--"));
}

void print_song_length()
{
  //print song length
  display.setCursor(CHAR_width_small * 21, 10);
  display.setTextColor(COLOR_SYSTEXT, COLOR_PITCHSMP);
  seq_print_formatted_number( get_song_length(), 2);
}

void UI_func_song(uint8_t param)
{
  if (LCDML.FUNC_setup())         // ****** SETUP *********
  {
    // setup function
    encoderDir[ENC_R].reset();
    seq.edit_state = false;
    seq.help_text_needs_refresh = true;
    seq.loop_edit_step = 0;
    temp_int = 0;
    seq.cycle_touch_element = 0;
    seq.menu = 0;
    display.fillScreen(COLOR_BACKGROUND);

    display.setTextSize(1);
    UI_toplineInfoText( 2);

    display.setCursor(1, 1);
    display.setTextColor(COLOR_SYSTEXT);
    display.print (F("SONG"));

    show_small_font(10, 1, 11, seq.name);
    //print loop
    print_song_loop();
    //print song length
    print_song_length();
    //print currently playing chain steps
    print_playing_chains();
    sub_song_print_tracknumbers();
    sub_song_print_tracktypes();
    sub_song_print_instruments(GREY2, COLOR_BACKGROUND);

    display.setCursor(40 * CHAR_width_small  ,  CHAR_height_small * 4 );
    display.setTextColor(GREY1, COLOR_BACKGROUND);
    display.print (F("CHAIN:"));
    display.setCursor(50 * CHAR_width_small  ,  CHAR_height_small * 4 );
    display.setTextColor(GREY2, COLOR_BACKGROUND);
    display.print (F("L"));
    display.setTextColor(COLOR_SYSTEXT);
    display.setCursor(CHAR_width_small * 11, 1);
    display.print (F("LOOP"));
    display.setCursor(CHAR_width_small * 21, 1);
    display.print (F("SLEN LC"));

    display.setTextColor(GREY2, COLOR_BACKGROUND);
    display.setCursor(40 * CHAR_width_small  ,  CHAR_height_small * 6 );
    display.print (F("ST"));
    display.setCursor(43 * CHAR_width_small  ,  CHAR_height_small * 6 );
    display.print (F("PAT"));
    display.setCursor(48 * CHAR_width_small  ,  CHAR_height_small * 6 );
    display.print (F("TRANS"));
    display.setTextColor(COLOR_SYSTEXT);

    display.setTextColor(GREY2, COLOR_BACKGROUND);
    for (uint8_t y = 0; y < 16; y++)  // chain
    {
      display.setCursor(CHAR_width_small * 40, CHAR_height_small * 8 + y * 10);
      seq_print_formatted_number( y + 1 , 2);
      display.setCursor(CHAR_width_small * 43, CHAR_height_small * 8 + y * 10);
      display.print("P");
      display.setCursor(CHAR_width_small * 48, CHAR_height_small * 8 + y * 10);
      display.print("T");
    }
    display.setTextSize(1);
    if (CHAR_height_small * 8  + 10 *  ( seq.current_song_step - seq.scrollpos + 1)  > CHAR_height_small * 8)
    {
      display.setCursor(CHAR_width_small * 4 , CHAR_height_small * 8  + 10 *  ( seq.current_song_step - seq.scrollpos )  );
      display.setTextColor(GREEN, COLOR_BACKGROUND);
      display.print(">");
    }
  }
  if (LCDML.FUNC_loop())          // ****** LOOP *********
  {
    if ((LCDML.BT_checkDown() && encoderDir[ENC_R].Down()) || (LCDML.BT_checkUp() && encoderDir[ENC_R].Up()) || (LCDML.BT_checkEnter() && encoderDir[ENC_R].ButtonShort()))
    {
      if (LCDML.BT_checkDown())
      {
        if (seq.edit_state == false)
        {
          if (seq.cursor_scroll == 15)
          {
            seq.scrollpos++;
            if (seq.scrollpos > SONG_LENGTH - 16)
              seq.scrollpos = SONG_LENGTH - 16;
          }
          else if (seq.tracktype_or_instrument_assign == 1) //disable edit instruments for tracks
          {
            seq.tracktype_or_instrument_assign = 0;
            sub_song_print_tracknumbers();
            sub_song_print_tracktypes();
            sub_song_print_instruments(GREY2, COLOR_BACKGROUND);
            seq.help_text_needs_refresh = true;
          }
          else if (seq.tracktype_or_instrument_assign == 2) //select instruments for track
          {
            if (seq.instrument[seq.selected_track] < 36)
              seq.instrument[seq.selected_track]++;
          }
          else if (seq.tracktype_or_instrument_assign == 6) // tracktype change
          {
            if (seq.track_type[seq.selected_track] < 3)
              seq.track_type[seq.selected_track]++;
          }
          else if (seq.tracktype_or_instrument_assign == 5) //disable edit tracktype select
          {
            seq.help_text_needs_refresh = true;
            seq.tracktype_or_instrument_assign = 1;
            sub_song_print_instruments(GREY2, COLOR_BACKGROUND);
            sub_song_print_tracktypes();
          }
          else if (seq.tracktype_or_instrument_assign == 0)
            seq.cursor_scroll = constrain(seq.cursor_scroll + ENCODER[ENC_R].speed(), 0, 15);

          if (CHAR_height_small * 8  + 10 *  ( seq.current_song_step - seq.scrollpos + 1)  > CHAR_height_small * 8)
          {
            display.setCursor(CHAR_width_small * 4 , CHAR_height_small * 8  + 10 *  ( seq.current_song_step - seq.scrollpos )  );
            display.setTextColor(GREEN, COLOR_BACKGROUND);
            display.print(">");
          }
          if (CHAR_height_small * 8  + 10 *  ( seq.current_song_step - seq.scrollpos + 1)  > CHAR_height_small * 7)
          {
            display.setCursor(CHAR_width_small * 4 , CHAR_height_small * 8  + 10 *  ( seq.current_song_step - seq.scrollpos + 1)  );
            display.print(" ");
          }
        }
        else  if (seq.edit_state == true && seq.cycle_touch_element == 5)
        {
          if (temp_int == NUM_CHAINS)
            temp_int = 0;
          else
            temp_int = constrain( temp_int + ENCODER[ENC_R].speed(), 0, NUM_CHAINS );  // not -1: last element is for empty step
        }
        else  if ( (seq.edit_state == true && seq.cycle_touch_element == 6) || (seq.edit_state == true && seq.cycle_touch_element == 8) )
        {
          seq.menu = constrain(  seq.menu + ENCODER[ENC_R].speed(), 0, 15 );
        }
        else  if (seq.edit_state == true && seq.cycle_touch_element == 7  )
        {
          if (seq.sub_menu == 99)
            seq.sub_menu = 0;
          else
            seq.sub_menu = constrain(seq.sub_menu + ENCODER[ENC_R].speed(), 0, NUM_CHAINS);
        }
        else  if ( seq.edit_state == true && seq.cycle_touch_element == 9  )
        {
          if (seq.sub_menu > NUM_CHAINS)
            seq.sub_menu = seq.sub_menu - 1;
          else if (seq.sub_menu >= 0 && seq.sub_menu < NUM_CHAINS - 1)
            seq.sub_menu = seq.sub_menu + 1;
          if (seq.sub_menu == NUM_CHAINS )
            seq.sub_menu = 0;
        }
      }
      else if (LCDML.BT_checkUp())
      {
        if (seq.edit_state == false)
        {
          if (seq.cursor_scroll == 0 && seq.scrollpos > 0 && seq.tracktype_or_instrument_assign == 0 )
          {
            seq.scrollpos--;
          }
          else if (seq.cursor_scroll == 0 && seq.scrollpos == 0 && seq.tracktype_or_instrument_assign == 0) //edit instruments for tracks
          {
            seq.tracktype_or_instrument_assign = 1;
            seq.help_text_needs_refresh = true;
            sub_song_print_tracknumbers();
            sub_song_print_tracktypes();
            sub_song_print_instruments(GREY1, COLOR_BACKGROUND);
          }
          else if (seq.tracktype_or_instrument_assign == 2) //select instruments for track
          {
            if (seq.instrument[seq.selected_track] > 0)
              seq.instrument[seq.selected_track]--;
          }
          else if (seq.tracktype_or_instrument_assign == 1) //goto for tracktype change
          {
            seq.tracktype_or_instrument_assign = 5;
            seq.help_text_needs_refresh = true;
            sub_song_print_instruments(GREY3, COLOR_BACKGROUND);
          }
          else if (seq.tracktype_or_instrument_assign == 6) // tracktype change
          {
            if (seq.track_type[seq.selected_track] > 0)
              seq.track_type[seq.selected_track]--;
          }
          if (seq.tracktype_or_instrument_assign == 0)
            seq.cursor_scroll = constrain(seq.cursor_scroll - ENCODER[ENC_R].speed(), 0, 15);

          if (CHAR_height_small * 8  + 10 *  ( seq.current_song_step - seq.scrollpos - 1)  > CHAR_height_small * 8)
          {
            display.setCursor(CHAR_width_small * 4 , CHAR_height_small * 8  + 10 *  ( seq.current_song_step - seq.scrollpos )  );
            display.setTextColor(GREEN, COLOR_BACKGROUND);
            display.print(">");
          }
          if (CHAR_height_small * 8  + 10 *  ( seq.current_song_step - seq.scrollpos - 1)  > CHAR_height_small * 7)
          {
            display.setCursor(CHAR_width_small * 4 , CHAR_height_small * 8  + 10 *  ( seq.current_song_step - seq.scrollpos - 1)  );
            display.print(" ");
          }
        }
        else  if (seq.edit_state == true && seq.cycle_touch_element == 5)
        {
          if (temp_int == 0)
            temp_int = NUM_CHAINS;
          else
            temp_int = constrain(temp_int - ENCODER[ENC_R].speed(), 0, NUM_CHAINS ); // not -1:last element is for empty step
        }
        else  if ( (seq.edit_state == true && seq.cycle_touch_element == 6) || (seq.edit_state == true && seq.cycle_touch_element == 8) )
        {
          seq.menu = constrain(seq.menu - ENCODER[ENC_R].speed(), 0, 15 );
        }
        else  if ( (seq.edit_state == true && seq.cycle_touch_element == 7) )
        {
          if (seq.sub_menu == 0)
            seq.sub_menu = 99;
          else if (seq.sub_menu == 99)
            ;
          else
            seq.sub_menu = constrain(seq.sub_menu - ENCODER[ENC_R].speed(), 0, NUM_CHAINS);
        }
        else  if (seq.edit_state == true && seq.cycle_touch_element == 9)
        {
          if (seq.sub_menu > 0 && seq.sub_menu < NUM_CHAINS)
            seq.sub_menu--;
          else if (seq.sub_menu > NUM_CHAINS)
            seq.sub_menu = seq.sub_menu + 1;
          else if (seq.sub_menu == 0)
            seq.sub_menu = NUM_CHAINS + 1;
          if (seq.sub_menu > NUM_CHAINS * 2 - 1)
            seq.sub_menu = NUM_CHAINS * 2 - 1;
        }
      }
      else  if (LCDML.BT_checkEnter())  //handle button presses during menu >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
      {
        if (seq.tracktype_or_instrument_assign == 5) // tracktype change
        {
          seq.tracktype_or_instrument_assign = 6;
          seq.help_text_needs_refresh = true;
        }
        else if (seq.tracktype_or_instrument_assign == 6)  //exit track assign menu
        {
          seq.tracktype_or_instrument_assign = 5;
          seq.help_text_needs_refresh = true;
        }
        else if (seq.tracktype_or_instrument_assign == 1)  //go into instr. assign menu
        {
          if (  seq.track_type[seq.selected_track] != 0 )  //not set to a drum track
          {
            seq.tracktype_or_instrument_assign = 2;
            seq.help_text_needs_refresh = true;
          }
          else
          {
            helptext_l("BACK");
            helptext_r("TRACKTYPE [DRUMS] NOT VALID FOR INSTR");
          }
        }
        else if (seq.tracktype_or_instrument_assign == 2)  //exit instr. assign menu
        {
          seq.tracktype_or_instrument_assign = 1;
          sub_song_print_tracktypes();
          seq.help_text_needs_refresh = true;
        }

        if (seq.tracktype_or_instrument_assign == 0)
        {
          seq.help_text_needs_refresh = true;
          if (seq.loop_edit_step == 1  ) // edit loop step 1, set start
          {
            seq.loop_edit_step = 2;
            if (seq.loop_start != seq.cursor_scroll + seq.scrollpos)
              seq.loop_start = seq.cursor_scroll + seq.scrollpos;
            else
              seq.loop_start = 99;
          }
          else if (seq.loop_edit_step == 2  ) // edit loop step, set end
          {
            seq.loop_edit_step = 0;
            seq.selected_track = 0;

            display.setTextColor( GREEN, COLOR_BACKGROUND);  //play indicator song view


            if (CHAR_height_small * 8  + 10 *  (seq.current_song_step - seq.scrollpos) > CHAR_height_small * 7)
            {
              display.setCursor(CHAR_width_small * 4 , CHAR_height_small * 8  + 10 *  (seq.current_song_step - seq.scrollpos)  );
              display.print(" ");
            }

            if (seq.loop_end != seq.cursor_scroll + seq.scrollpos)
            {
              seq.loop_end = seq.cursor_scroll + seq.scrollpos;
            }
            else
            {
              seq.loop_end = 99;
            }
          }
          else if (seq.cycle_touch_element == 5 && seq.edit_state)
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
            seq.help_text_needs_refresh = true;
            seq.sub_menu = seq.chain[  seq.song[seq.selected_track][seq.cursor_scroll + seq.scrollpos] ][seq.menu];
          }
          else if (seq.edit_state && seq.cycle_touch_element == 7)
          {
            seq.cycle_touch_element = 6;  //go back from chain
          }
          else if (seq.edit_state && seq.cycle_touch_element == 8)
          {
            seq.cycle_touch_element = 9;  //edit transpose
            if (seq.chain_transpose[  seq.song[seq.selected_track][seq.cursor_scroll + seq.scrollpos] ][seq.menu] != 99)
              seq.sub_menu = seq.chain_transpose[  seq.song[seq.selected_track][seq.cursor_scroll + seq.scrollpos] ][seq.menu];
            else
              seq.sub_menu = 0;
          }
          else if (seq.edit_state && seq.cycle_touch_element == 9)
          {
            if (seq.sub_menu == 0)
              seq.chain_transpose[  seq.song[seq.selected_track][seq.cursor_scroll + seq.scrollpos] ][seq.menu] = 99;
            seq.cycle_touch_element = 8;  //go back from transpose
          }
          else if (seq.edit_state == false )
          {
            seq.cycle_touch_element = 0;
          }

          if (seq.cycle_touch_element < 7)
          {
            //print loop
            print_song_loop();
            //print song length
            print_song_length();
          }
          seq.help_text_needs_refresh = true;
        }
      }  // Button END
    }
    if (seq.help_text_needs_refresh)
    {
      if ( seq.tracktype_or_instrument_assign == 6 ) // is in tracktype select mode
      {
        helptext_l("BACK");
        helptext_r("SELECT WITH < > THEN PUSH TO CONFIRM TRACKTYPE");
      }
      else if ( seq.tracktype_or_instrument_assign == 5 ) // is in tracktype select mode
      {
        helptext_l("SELECT TRACK < > TO CHANGE TYPE");
        helptext_r("PUSH TO CONFIRM TRACK");
      }
      else if ( seq.tracktype_or_instrument_assign == 2 ) // is in inst. select mode
      {
        helptext_l("BACK");
        helptext_r("SELECT INSTRUMENT < > PUSH TO CONFIRM");
      }
      else  if ( seq.tracktype_or_instrument_assign == 1 )
      {
        display.fillRect(CHAR_width_small * 26 , DISPLAY_HEIGHT - CHAR_height_small, 6 * CHAR_width_small + 2, CHAR_height_small, COLOR_BACKGROUND );
        helptext_l("SELECT TRACK < > FOR INSTR");
        helptext_r("PUSH TO CONFIRM TRACK");
      }
      else if (seq.loop_edit_step == 1 )
      {
        helptext_r("SELECT LOOP START");
        display.fillRect(CHAR_width_small * 7, DISPLAY_HEIGHT - CHAR_height_small, 30 * CHAR_width_small, CHAR_height_small, COLOR_BACKGROUND );
      }
      else if (seq.tracktype_or_instrument_assign == 0)
      { // all messages below in standard song mode
        if (seq.loop_edit_step == 2 )
          helptext_r("SELECT LOOP END");
        else  if (seq.loop_edit_step == 0 && seq.edit_state == false && seq.cycle_touch_element != 0)
          helptext_r("MOVE Y");
        else if (seq.edit_state == false && seq.cycle_touch_element == 0)
        {
          helptext_l("MOVE X");
          helptext_r("MOVE Y");
          display.setTextSize(1);
          display.setCursor(7 * CHAR_width_small, DISPLAY_HEIGHT - CHAR_height_small * 1);
          display.setTextColor(COLOR_INSTR);
          display.print(F("INSTR "));
          display.setTextColor(COLOR_DRUMS);
          display.print(F("DRUM/SMP "));
          display.setTextColor(COLOR_PITCHSMP);
          display.print(F("PITCHED SAMPLE "));
          display.setTextColor(COLOR_CHORDS);
          display.print(F("CHORD "));
          display.setTextColor(COLOR_ARP);
          display.print(F("ARP"));
        }
        else if (seq.edit_state && seq.cycle_touch_element == 5)
        {
          display.setCursor(7 * CHAR_width_small, DISPLAY_HEIGHT - CHAR_height_small * 1);
          print_empty_spaces(31);
          helptext_l("> CHAIN");
          helptext_r("< > SELECT CHAIN");
        }
        else if (seq.edit_state && seq.cycle_touch_element == 6)
        {
          helptext_l("SONG < > TRANSPOSE");
          display.setCursor (CHAR_width_small * 18, DISPLAY_HEIGHT - CHAR_height_small);
          print_empty_spaces(7);
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
          helptext_l("< CHAIN");
          helptext_r ("< > SEL. STEP");
        }
      }
      seq.help_text_needs_refresh = false;
    }


    if (seq.tracktype_or_instrument_assign > 0  && seq.tracktype_or_instrument_assign < 3) //select track for instrument assignment
    {
      sub_song_print_instruments(DARKGREEN, COLOR_BACKGROUND);
    }
    else if (seq.tracktype_or_instrument_assign > 4  && seq.tracktype_or_instrument_assign < 7) //select track for tracktype assignment
    {
      sub_song_print_tracktypes();
      //helptext_l("<<<<yyyy");
    }


    else if (seq.tracktype_or_instrument_assign == 0) // normal mode: show song grid, chain..
    {
      for (uint8_t x = 0; x < NUM_SEQ_TRACKS; x++)
      {
        for (uint8_t y = 0; y < 16; y++) //song steps
        {
          uint8_t lineheight = 10;

          if ( (seq.loop_edit_step == 1 && y == seq.cursor_scroll)  ||  (seq.loop_edit_step == 2 && y == seq.cursor_scroll) )
            display.setTextColor(COLOR_BACKGROUND, GREEN);  //select loop start/end step
          else
            display.setTextColor(GREEN, COLOR_BACKGROUND);  // green step number

          if (seq.cycle_touch_element < 7) //test
          {
            display.setCursor(CHAR_width_small, y * lineheight + CHAR_height_small * 8 );
            seq_print_formatted_number( y + 1 + seq.scrollpos , 2);
          }
          if ( ( y + seq.scrollpos == seq.loop_start ) || (seq.loop_edit_step == 1 && y == seq.cursor_scroll))
          {
            if (seq.loop_edit_step == 1 &&  y == seq.cursor_scroll )
              display.setTextColor( RED, COLOR_BACKGROUND);
            else
              display.setTextColor(GREEN, COLOR_BACKGROUND);
            display.setCursor(0, y * lineheight + CHAR_height_small * 8 );
            display.print(">");
            display.setTextColor(GREEN, COLOR_BACKGROUND);
          }
          else
          {
            display.setCursor(0, y  * lineheight + CHAR_height_small * 8 );
            display.print(" ");
          }
          if ( ( y + seq.scrollpos == seq.loop_end) || (seq.loop_edit_step == 2 && y == seq.cursor_scroll))
          {
            if (seq.loop_edit_step == 2 &&  y == seq.cursor_scroll)
              display.setTextColor( RED, COLOR_BACKGROUND);
            else
              display.setTextColor(GREEN, COLOR_BACKGROUND);
            display.setCursor(3 * CHAR_width_small, y * lineheight + CHAR_height_small * 8 );
            display.print("<");
            display.setTextColor(GREEN, COLOR_BACKGROUND);
          }
          else
          {
            display.setCursor(3 * CHAR_width_small, y * lineheight + CHAR_height_small * 8 );
            display.print(" ");
          }
          if (seq.cycle_touch_element < 7)
          {
            display.setCursor(6 * CHAR_width_small + (2 * CHAR_width)*x , y * lineheight + CHAR_height_small * 8 );
            if (y == seq.cursor_scroll && x == seq.selected_track  && seq.edit_state == false && seq.loop_edit_step == 0
                && seq.tracktype_or_instrument_assign == 0)
              set_track_type_color_inverted(x);
            else if (y == seq.cursor_scroll && x == seq.selected_track  && seq.edit_state && seq.cycle_touch_element == 5 && seq.loop_edit_step == 0)
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
            }
          }
        }
      }
      uint8_t endline = 99;
      display.setTextSize(1);

      if (seq.edit_state && seq.cycle_touch_element == 9)
      {
        seq.chain_transpose[  seq.song[seq.selected_track][seq.cursor_scroll + seq.scrollpos]   ][seq.menu]  = seq.sub_menu;
      }

      if ( seq.cycle_touch_element < 10)
      {
        for (uint8_t y = 0; y < 16; y++)  // chain
        {
          if (seq.cycle_touch_element != 7)
          {
            display.setCursor(CHAR_width_small * 44, CHAR_height_small * 8 + y * 10);

            if (endline == 99 && seq.chain[  seq.song[seq.selected_track][seq.cursor_scroll + seq.scrollpos]   ][y] == 99 )
            {
              display.setTextColor(RED, COLOR_BACKGROUND);
              display.print(F("END"));
              display.setTextColor(GREY1, COLOR_BACKGROUND);
              endline = y;
            }
            if (seq.edit_state && seq.cycle_touch_element == 6)
              display.setTextColor(COLOR_SYSTEXT, COLOR_BACKGROUND);
            else
              display.setTextColor(GREY1, COLOR_BACKGROUND);
            display.setCursor(CHAR_width_small * 44, CHAR_height_small * 8 + y * 10);

            if ( seq.song[seq.selected_track][seq.cursor_scroll + seq.scrollpos] < 99 &&
                 seq.chain[  seq.song[seq.selected_track][seq.cursor_scroll + seq.scrollpos]    ][y]  < 99)
            {
              seq_print_formatted_number( seq.chain[ seq.song[seq.selected_track][seq.cursor_scroll + seq.scrollpos] ][y]  , 2);
              display.print(F(" "));
            }
            else if (endline != y)
              display.print(F("-- "));
          }
          display.setCursor(CHAR_width_small * 50, CHAR_height_small * 8 + y * 10);  // chain transpose
          if (  seq.edit_state && seq.cycle_touch_element == 9 && seq.menu == y)
            display.setTextColor(COLOR_SYSTEXT, RED);
          else if ( seq.edit_state && seq.cycle_touch_element == 8 && seq.menu == y)
            display.setTextColor( COLOR_BACKGROUND, COLOR_SYSTEXT);
          else
            display.setTextColor(GREY1, COLOR_BACKGROUND);
          if ( seq.song[seq.selected_track][seq.cursor_scroll + seq.scrollpos] != 99 &&
               seq.chain_transpose[  seq.song[seq.selected_track][seq.cursor_scroll + seq.scrollpos ]     ][y] < 99 )
          {
            if (seq.chain_transpose[ seq.song[seq.selected_track][seq.cursor_scroll + seq.scrollpos] ][y] < NUM_CHAINS)
              seq_print_formatted_number( seq.chain_transpose[ seq.song[seq.selected_track][seq.cursor_scroll + seq.scrollpos] ][y]   , 2);
            else
              seq_print_formatted_number( (seq.chain_transpose[ seq.song[seq.selected_track][seq.cursor_scroll + seq.scrollpos] ][y]) - NUM_CHAINS  , 2);

            display.setCursor(CHAR_width_small * 49, CHAR_height_small * 8 + y * 10);
            display.setTextColor(COLOR_SYSTEXT, COLOR_BACKGROUND);

            if ( seq.chain_transpose[  seq.song[seq.selected_track][seq.cursor_scroll + seq.scrollpos ]     ][y] > NUM_CHAINS &&
                 seq.chain_transpose[  seq.song[seq.selected_track][seq.cursor_scroll + seq.scrollpos ]     ][y] < NUM_CHAINS * 2)
              display.print(F("-"));
            else if ( seq.chain_transpose[  seq.song[seq.selected_track][seq.cursor_scroll + seq.scrollpos ]     ][y] != 0 &&
                      seq.chain_transpose[  seq.song[seq.selected_track][seq.cursor_scroll + seq.scrollpos ]     ][y] != 99)
              display.print(F("+"));
            else if ( seq.chain_transpose[  seq.song[seq.selected_track][seq.cursor_scroll + seq.scrollpos ]     ][y] == 0)
              display.print(F(" "));
          }
          else
          {
            display.print(F("--"));
            display.setCursor(CHAR_width_small * 49, CHAR_height_small * 8 + y * 10);
            display.setTextColor(COLOR_SYSTEXT, COLOR_BACKGROUND);
            display.print(F(" "));
          }
        }
      }
      if (seq.edit_state && seq.cycle_touch_element == 6)
      {
        display.setTextColor( COLOR_BACKGROUND, COLOR_SYSTEXT);

        display.setCursor(CHAR_width_small * 44, CHAR_height_small * 8 + seq.menu * 10);
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
        display.setTextColor(COLOR_SYSTEXT, RED);
        display.setCursor(CHAR_width_small * 44, CHAR_height_small * 8 + seq.menu * 10);
        if ( seq.chain[  seq.song[seq.selected_track][seq.cursor_scroll + seq.scrollpos]   ][seq.menu]   == 99 )
          display.print(F("--"));
        else
          seq_print_formatted_number( seq.chain[  seq.song[seq.selected_track][seq.cursor_scroll + seq.scrollpos]   ][seq.menu]   , 2);
        display.setTextColor(COLOR_SYSTEXT, COLOR_BACKGROUND);
        display.print(F(" "));
        pattern_preview_in_song(seq.chain[  seq.song[seq.selected_track][seq.cursor_scroll + seq.scrollpos]   ][seq.menu] );
      }

      //print current CHAIN Number
      display.setTextSize(1);
      display.setCursor(47 * CHAR_width_small  ,  CHAR_height_small * 4 );
      if (seq.edit_state && seq.cycle_touch_element > 5 && seq.cycle_touch_element < 9)
        display.setTextColor(COLOR_SYSTEXT, COLOR_BACKGROUND);
      else
        display.setTextColor(GREY1, COLOR_BACKGROUND);

      if ( seq.song[seq.selected_track][seq.cursor_scroll + seq.scrollpos] < 99 )
        seq_print_formatted_number(seq.song[seq.selected_track][seq.cursor_scroll + seq.scrollpos] , 2);
      else
        display.print("--");
      display.setCursor(51 * CHAR_width_small  ,  CHAR_height_small * 4 ); //print chain length of current track step
      seq_print_formatted_number(  get_chain_length_from_current_track(seq.selected_track), 2);

    }
  }
  if (LCDML.FUNC_close())     // ****** STABLE END *********
  {
    encoderDir[ENC_R].reset();
    seq.cycle_touch_element = 0;
    seq.tracktype_or_instrument_assign = 0;
    display.setTextColor(COLOR_SYSTEXT, COLOR_BACKGROUND);
    display.fillScreen(COLOR_BACKGROUND);
  }
}


//void UI_func_seq_tracker(uint8_t param)
//{
//  if (LCDML.FUNC_setup())         // ****** SETUP *********
//  {
//    // setup function
//    reset_tracker_edit_cache();
//    encoderDir[ENC_R].reset();
//    display.fillScreen(COLOR_BACKGROUND);
//    UI_toplineInfoText();
//    display.setTextSize(1);
//    setCursor_textGrid(1, 1);
//    display.setTextColor(COLOR_SYSTEXT);
//    display.print (F("TRACKER"));
//    display.setTextSize(1);
//    for (uint8_t x = 0; x < NUM_SEQ_TRACKS; x++)
//    {
//      setCursor_textGrid(4 + 6 * x, 3);
//      display.setCursor(display.getCursorX(), display.getCursorY() + 15);
//      display.setTextColor(COLOR_SYSTEXT, COLOR_BACKGROUND);
//      display.print (F("TRACK: "));
//      display.print (x + 1);
//      setCursor_textGrid(4 + 6 * x, 4);
//      display.setCursor(display.getCursorX(), display.getCursorY() + 9);
//      display.print (F("PAT:"));
//
//    }
//    display.setTextSize(2);
//
//  }
//  if (LCDML.FUNC_loop())          // ****** LOOP *********
//  {
//    if ((LCDML.BT_checkDown() && encoderDir[ENC_R].Down()) || (LCDML.BT_checkUp() && encoderDir[ENC_R].Up()) || (LCDML.BT_checkEnter() && encoderDir[ENC_R].ButtonShort()))
//    {
//      if (LCDML.BT_checkDown())
//      {
//        if (seq.scrollpos == 52)
//        {
//          seq.cursor_scroll++;
//          if (seq.cursor_scroll > 5)
//            seq.cursor_scroll = 5;
//          reset_tracker_edit_cache_single_track();
//        }
//        seq.scrollpos = constrain(seq.scrollpos + ENCODER[ENC_R].speed(), 0, 52);
//      }
//      else if (LCDML.BT_checkUp())
//      {
//        if (seq.scrollpos == 0)
//        {
//          seq.cursor_scroll--;
//          if (seq.cursor_scroll < -6)
//            seq.cursor_scroll = -6;
//          reset_tracker_edit_cache_single_track();
//        }
//        seq.scrollpos = constrain(seq.scrollpos - ENCODER[ENC_R].speed(), 0, 52);
//      }
//      else  if (LCDML.BT_checkEnter())  //handle button presses during menu >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
//      {
//        ;
//      }
//    }
//
//    print_merged_pattern_for_editor( 4 * CHAR_width_small , 95,  0);
//    print_merged_pattern_for_editor( 10 * CHAR_width_small , 95,  1);
//    print_merged_pattern_for_editor( 16 * CHAR_width_small , 95,  2);
//    print_merged_pattern_for_editor( 22 * CHAR_width_small , 95,  3);
//    print_merged_pattern_for_editor( 28 * CHAR_width_small , 95,  4);
//    print_merged_pattern_for_editor( 34 * CHAR_width_small , 95,  5);
//  }
//  if (LCDML.FUNC_close())     // ****** STABLE END *********
//  {
//    encoderDir[ENC_R].reset();
//    display.fillScreen(COLOR_BACKGROUND);
//  }
//}

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
    //display.fillRect(1, 72, DISPLAY_WIDTH, DISPLAY_HEIGHT - 72, COLOR_BACKGROUND);
    print_merged_pattern_pianoroll( 1 * CHAR_width , DISPLAY_HEIGHT - CHAR_height,  seq.active_track);
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
    print_merged_pattern_pianoroll( 1 * CHAR_width , DISPLAY_HEIGHT - CHAR_height,  seq.active_track);
  }
  if (LCDML.FUNC_close())     // ****** STABLE END *********
  {
    display.fillScreen(COLOR_BACKGROUND);
    encoderDir[ENC_R].reset();
  }
}

void arp_refresh_display_play_status()
{
  if (seq.running == false )
  {
    //play symbol
    drawBitmap(24 * CHAR_width - 4 , CHAR_height * 7 + 6, special_chars[19], 8, 8, GREEN);
  }  else if (seq.running == true )
  {
    //stop symbol
    drawBitmap(24 * CHAR_width - 4, CHAR_height * 7 + 6, special_chars[21], 8, 8, COLOR_SYSTEXT);
  }
}

void print_arp_start_stop_button()
{
  if (seq.running)
    draw_button_on_grid( 42, 15, "SEQ.", "STOP", 1 );
  else
    draw_button_on_grid( 42, 15, "SEQ.", "START", 0 );
  arp_refresh_display_play_status();
}

void UI_func_arpeggio(uint8_t param)
{
  if (LCDML.FUNC_setup())         // ****** SETUP *********
  {
    encoderDir[ENC_R].reset();
    seq.temp_select_menu = 0;
    seq.temp_active_menu = 0;
    display.fillScreen(COLOR_BACKGROUND);
    display.setTextSize(2);
    seq_active_function = 0;
    setCursor_textGrid_large(1, 1);
    display.setTextColor(RED);
    display.print(F("ARPEGGIO SETTINGS"));
    display.setTextColor(COLOR_SYSTEXT, COLOR_BACKGROUND);
    setCursor_textGrid_large( 1, 4);
    display.print(F("LENGTH"));
    setCursor_textGrid_large(14, 4);
    display.print(F("STEPS"));
    setCursor_textGrid_large( 1, 5);
    display.print(F("STYLE"));
    setCursor_textGrid_large( 1, 6);
    display.print(F("SPEED"));
    display.setTextColor(GREY2, COLOR_BACKGROUND);
    setCursor_textGrid_large(1, 8);
    display.print(F("PLAYING:"));
    setCursor_textGrid_large(10, 8);
    display.print(F("["));
    setCursor_textGrid_large(18, 8);
    display.print(F("]"));
    print_arp_start_stop_button();
    helptext_l("BACK");
    display.setTextSize(2);
  }
  if (LCDML.FUNC_loop())          // ****** LOOP *********
  {
    if ((LCDML.BT_checkDown() && encoderDir[ENC_R].Down()) || (LCDML.BT_checkUp() && encoderDir[ENC_R].Up()))
    {
      if (LCDML.BT_checkDown())
      {
        if ( seq_active_function == 0 )
          seq.temp_select_menu = constrain(seq.temp_select_menu + 1, 0, 2);
        else if ( seq.temp_select_menu == 0 )
          seq.arp_length = constrain(seq.arp_length + ENCODER[ENC_R].speed(), 0, 9);
        else if ( seq.temp_select_menu == 1 )
          seq.arp_style = constrain(seq.arp_style + ENCODER[ENC_R].speed(), 0, 3);
        else if ( seq.temp_select_menu == 2 )
          seq.arp_speed = constrain(seq.arp_speed + ENCODER[ENC_R].speed(), 0, 3);
      }
      else if (LCDML.BT_checkUp())
      {
        if ( seq_active_function == 0 )
          seq.temp_select_menu = constrain(seq.temp_select_menu - 1, 0, 2);
        else if ( seq.temp_select_menu == 0 )
          seq.arp_length = constrain(seq.arp_length - ENCODER[ENC_R].speed(), 0, 9);
        else if ( seq.temp_select_menu == 1 )
          seq.arp_style = constrain(seq.arp_style - ENCODER[ENC_R].speed(), 0, 3);
        else if ( seq.temp_select_menu == 2 )
          seq.arp_speed = constrain(seq.arp_speed - ENCODER[ENC_R].speed(), 0, 3);
      }
    }
    if (LCDML.BT_checkEnter()  && encoderDir[ENC_R].ButtonShort())  //handle button presses during menu >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
    {
      if ( seq_active_function == 0 )
        seq_active_function = 1;
      else
        seq_active_function = 0;

    }

    //button check end <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<

    if ( seq_active_function == 0 )
      helptext_r("< > SELECT OPTION TO EDIT");
    else
      helptext_r("< > EDIT VALUE");
    display.setTextSize(2);
    if (seq.temp_select_menu == 0)
      display.setTextColor(COLOR_BACKGROUND, COLOR_SYSTEXT); else display.setTextColor(COLOR_SYSTEXT, COLOR_BACKGROUND);
    setCursor_textGrid_large(10, 4);
    if (seq.arp_length == 0)
      display.print("ALL");
    else
      seq_print_formatted_number( seq.arp_length, 3);//play all elements or from 1-xx elements
    setCursor_textGrid_large(10, 5);
    for (uint8_t i = 0; i < 4; i++)
    {
      if (i == seq.arp_style && seq.temp_select_menu == 1)
        display.setTextColor(COLOR_SYSTEXT, GREY3);
      else if (i == seq.arp_style)
        display.setTextColor(RED, GREY3);
      else if (seq.temp_select_menu == 1)
        display.setTextColor(COLOR_BACKGROUND, GREY3); else display.setTextColor(GREY1, GREY3);
      display.print( seq.arp_style_names[i][0] );
      display.print( seq.arp_style_names[i][1] );
      display.print( seq.arp_style_names[i][2] );
      display.setTextColor(GREY1, COLOR_BACKGROUND);
      display.print(" ");
    }
    if (seq.temp_select_menu == 2)
      display.setTextColor(COLOR_BACKGROUND, COLOR_SYSTEXT); else display.setTextColor(COLOR_SYSTEXT, COLOR_BACKGROUND);
    setCursor_textGrid_large(10, 6);
    if (seq.arp_speed == 0)display.print("1/16 ");
    else if (seq.arp_speed == 1)display.print("1/8  ");
    else if (seq.arp_speed == 2)display.print("1/32 ");
    else if (seq.arp_speed == 3)display.print("1/64 ");
  }
  if (LCDML.FUNC_close())     // ****** STABLE END *********
  {
    seq.menu = 0;
    seq_active_function = 99;
    encoderDir[ENC_R].reset();
    display.setTextColor(COLOR_SYSTEXT, COLOR_BACKGROUND);
    display.fillScreen(COLOR_BACKGROUND);
  }
}

void UI_func_seq_mute_matrix(uint8_t param)
{
  if (LCDML.FUNC_setup())         // ****** SETUP *********
  {
    // setup function
    display.fillScreen(COLOR_BACKGROUND);
    UI_toplineInfoText( 1);
    display.setTextSize(1);
    display.setCursor(1, 2);
    display.setTextColor(COLOR_SYSTEXT);
    display.print ("MUTE MATRIX");
    helptext_l("BACK");
    helptext_r ("TOUCH SCREEN TO MUTE/UNMUTE");
  }
  if (LCDML.FUNC_loop())          // ****** LOOP *********
  {
    uint8_t button_count = 0;
    char buf[4];

    display.setTextColor(COLOR_SYSTEXT, COLOR_BACKGROUND);
    setCursor_textGrid_mini( 2, 4);
    display.print("MUTE/UNMUTE");
    setCursor_textGrid_mini( 2, 5);
    display.print("AT/IN:");

    for (uint8_t y = 0; y < 3; y++)
    {
      for (uint8_t x = 0; x < 4; x++)
      {
        if (y < 2)
        {
          if (!seq.track_mute[button_count])
            draw_button_on_grid( 2 + x * 14,  12 + y * 8, "TRACK:", itoa(button_count + 1, buf, 10), 1 );
          else
            draw_button_on_grid( 2 + x * 14,  12 + y * 8, "TRACK:", itoa(button_count + 1, buf, 10), 0 );
          button_count++;
        }
        else
        {
          if (x == 1)
          {
            if (seq.mute_mode == 0)
              draw_button_on_grid( 2 + x * 14,  4, "REAL", "TIME", 1 );
            else
              draw_button_on_grid( 2 + x * 14,  4, "REAL", "TIME", 0 );
          }
          else if (x == 2)
          {
            if (seq.mute_mode == 1)
              draw_button_on_grid( 2 + x * 14,  4, "NEXT", "PATTRN", 1 );
            else
              draw_button_on_grid( 2 + x * 14,  4, "NEXT", "PATTRN", 0 );
          }
          else if (x == 3)
          {
            if (seq.mute_mode == 2)
              draw_button_on_grid( 2 + x * 14,  4, "SONG", "STEP", 1 );
            else
              draw_button_on_grid( 2 + x * 14,  4, "SONG", "STEP", 0 );
          }
        }
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
    setCursor_textGrid(1, 1);
    display.print(F("Load Performance"));
    setCursor_textGrid(1, 2);
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
        setCursor_textGrid(1, 2);
        if (load_sd_performance_json(temp_int) == false)
          display.print(F("Does not exist."));
        else
        {
          load_sd_performance_json(temp_int);
          seq.state_last_loadsave = temp_int;
          setCursor_textGrid(1, 2);
          display.print(F("Done.           "));
        }
        delay(MESSAGE_WAIT_TIME);
        LCDML.FUNC_goBackToMenu();
      }
    }
    setCursor_textGrid(1, 2);
    char tmp[10];
    sprintf(tmp, "[%2d] ", temp_int);
    display.print(tmp);
    if (check_sd_performance_exists(temp_int))
    {
      get_sd_performance_name_json(temp_int);
      if ( seq.name_temp[0] != 0 )
        show(2, 6, 11, seq.name_temp);
      else
        display.print(F(" -- DATA --"));
    }
    else print_empty_spaces(11);
  }
  if (LCDML.FUNC_close())     // ****** STABLE END *********
  {
    if (mode < 0xff)
    {
      show(2, 1, 16, "Canceled.");
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
    setCursor_textGrid(1, 1);
    display.print(F("Save Performance"));
    setCursor_textGrid(1, 2);
    sprintf(tmp, "[%2d] ", temp_int);
    display.print(tmp);

    if (check_sd_performance_exists(temp_int))
    {
      overwrite = true;
      get_sd_performance_name_json(temp_int);
      if ( seq.name_temp[0] != 0 )
        show(2, 6, 11, seq.name_temp);
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
          setCursor_textGrid(1, 2);
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
            show(2, 1, 16, "Done.");
            seq.state_last_loadsave = temp_int;
            delay(MESSAGE_WAIT_TIME);
            LCDML.FUNC_goBackToMenu();
          }
          else if (overwrite == true && yesno == false)
          {
            char tmp[10];

            mode = 0;
            setCursor_textGrid(1, 2);
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
        setCursor_textGrid(1, 2);
        sprintf(tmp, "[%2d] ", temp_int);
        display.print(tmp);
        setCursor_textGrid(6, 2);
        if (overwrite == false)
        {
          display.print("-- EMPTY --");
        }
        else if (check_sd_performance_exists(temp_int))
        {
          get_sd_performance_name_json(temp_int);
          if ( seq.name_temp[0] != 0 )
            show(2, 6, 11, seq.name_temp);
          else
            display.print("-- DATA --");
        }
        else  print_empty_spaces(10);
      }
      else if (mode == 1)
      {
        setCursor_textGrid(13, 2);
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
      show(2, 1, 16, "Canceled.");
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
    setCursor_textGrid(1, 1);
    display.setTextSize(1);
    display.print(version_string);
    setCursor_textGrid(1, 2);
    display.setTextSize(1);
    display.print(sd_string);
    setCursor_textGrid(1, 3);
    display.setTextColor(GREY2);
    display.print(F("COMPILED FOR "));
    display.setTextColor(RED);
#ifdef COMPILE_FOR_PROGMEM
    display.print(F("PROGMEM"));
#endif
#ifdef COMPILE_FOR_FLASH
    display.print(F("FLASH CHIP"));
#endif
#ifdef COMPILE_FOR_SDCARD
    display.print(F("SD CARD"));
#endif
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

//void UI_func_speedtest(uint8_t param)
//{ // ILI9341 478 msecs
//  // ILI9486 1242 msecs
//  if (LCDML.FUNC_setup())         // ****** SETUP *********
//  {
//    display.fillScreen(COLOR_BACKGROUND);
//    encoderDir[ENC_R].reset();
//    display.setTextSize(1);
//    display.setTextColor(GREY3);
//  }
//  if (LCDML.FUNC_loop())          // ****** LOOP *********
//  {
//    elapsedMillis msecs;
//    for (uint8_t t = 0; t < 30; t++)
//    {
//      for (uint8_t y = 0; y < 14; y++)
//      {
//        for (uint8_t x = 0; x < 25; x++)
//        {
//          setCursor_textGrid(x, y);
//          display.write(x * y + t);
//        }
//      }
//    }
//    display.setTextColor(GREEN);
//    display.setTextSize(2);
//    setCursor_textGrid(5, 5);
//    display.print(F("RESULT"));
//    setCursor_textGrid(5, 6);
//    display.print(msecs);
//    display.print(F(" msecs"));
//  }
//  if (LCDML.FUNC_close())     // ****** STABLE END *********
//  {
//    encoderDir[ENC_R].reset();
//    display.setTextSize(1);
//  }
//}

void sd_printDirectory(File currentDirectory)
{
  char tmp[6];
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
    drawBitmap(CHAR_width_small,  5 * 11 - 1, special_chars[23], 8, 8, YELLOW);
    display.setCursor (CHAR_width_small * 3,   + 5 * 11  );
    display.print("..");
  }
  else
  {
    drawBitmap(CHAR_width_small,  5 * 11 - 1, special_chars[23], 8, 8, COLOR_BACKGROUND);
    display.setCursor (CHAR_width_small * 3,   + 5 * 11  );
    display.print("/ ");
  }
  for (uint8_t f = 0; f < 10; f++)
  {
    fm.sd_entry =  currentDirectory.openNextFile();
    if (! fm.sd_entry)
    {
      fm.sd_cap_rows = f - 1;
      display.fillRect(CHAR_width_small, f * 11 + 6 * 11 - 1 , CHAR_width_small * 27, (10 - f) * 11, COLOR_BACKGROUND);
      break;
    }
    if (fm.sd_entry.isDirectory() )
    {
      drawBitmap(CHAR_width_small, f * 11 - 1 + 6 * 11 , special_chars[23], 8, 8, YELLOW);
      if (f == fm.sd_selected_file && fm.sd_parent_folder == false && fm.active_window == 0)
        display.setTextColor(COLOR_BACKGROUND, COLOR_PITCHSMP);
      else
        display.setTextColor(COLOR_PITCHSMP, COLOR_BACKGROUND);
      show_smallfont_noGrid( f * 11  + 6 * 11, CHAR_width_small * 3 , 17, fm.sd_entry.name() );
      display.setCursor (CHAR_width_small * 22, f * 11  + 6 * 11  );
      display.setTextColor(DX_DARKCYAN, COLOR_BACKGROUND);
      display.print("DIR   ");
    } else
    {
      drawBitmap(CHAR_width_small, f * 11 - 1 + 6 * 11 , special_chars[23], 8, 8, COLOR_BACKGROUND);
      if (f == fm.sd_selected_file && fm.sd_parent_folder == false && fm.active_window == 0)
        display.setTextColor(COLOR_BACKGROUND, COLOR_SYSTEXT);
      else
        display.setTextColor(COLOR_SYSTEXT, COLOR_BACKGROUND);
      show_smallfont_noGrid(f * 11  + 6 * 11, CHAR_width_small * 3 , 17, fm.sd_entry.name() );
      display.setTextColor(COLOR_DRUMS, COLOR_BACKGROUND);
      display.setCursor (CHAR_width_small * 21, f * 11  + 6 * 11  );
      if (fm.sd_entry.size() / 1024 / 1024 > 0)
      {
        sprintf(tmp, "%4d", int(fm.sd_entry.size() / 1024 / 1024));
        display.print(tmp);
        display.print( " MB");
      }
      else if (int(fm.sd_entry.size() / 1024) > 0)
      {
        sprintf(tmp, "%4d", int(fm.sd_entry.size() / 1024));
        display.print(tmp);
        display.print( " KB");
      }
      else
      {
        sprintf(tmp, "%4d", int(fm.sd_entry.size()));
        display.print(tmp);
        display.print(" B ");
      }
    }
    if (f == fm.sd_selected_file && fm.sd_parent_folder == false) strcpy(fm.sd_temp_name, fm.sd_entry.name() );
    if (f == fm.sd_selected_file &&  fm.sd_entry.isDirectory() )
      fm.sd_is_folder = true;
    fm.sd_entry.close();
  }
}

void flash_printDirectory()
{
  char tmp[6];
  fm.flash_cap_rows = 9;
  uint8_t f = 0;
  char filename[26];
  uint32_t filesize;

  SerialFlash.opendir();

  if (fm.flash_skip_files > 0 )
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
    else if (SerialFlash.readdir(filename, sizeof(filename), filesize))
    {
      if (f == fm.flash_selected_file && fm.active_window == 1)
        display.setTextColor(COLOR_BACKGROUND, COLOR_PITCHSMP);
      else
        display.setTextColor(COLOR_PITCHSMP, COLOR_BACKGROUND);

      show_smallfont_noGrid( f * 11  + 6 * 11, CHAR_width_small * 30, 15, filename );

      display.setTextColor(COLOR_DRUMS, COLOR_BACKGROUND);
      display.setCursor (CHAR_width_small * 45, f * 11  + 6 * 11  );

      if (filesize / 1024 / 1024 > 0)
      {
        sprintf(tmp, "%4d", int(filesize / 1024 / 1024));
        display.print(tmp);
        display.print( " MB");
      }
      else if (int(filesize / 1024) > 0)
      {
        sprintf(tmp, "%4d", int(filesize / 1024));
        display.print(tmp);
        display.print( " KB");
      }
      else
      {
        sprintf(tmp, "%4d", int(filesize));
        display.print(tmp);
        display.print(" B ");
      }
    }
    //      else
    //      {
    //        fm.flash_cap_rows = f - 1;
    //        //      display.fillRect(10 + 240, f * 11   + 11 * 11 - 1 , CHAR_width * 15, (10 - f) * 11, COLOR_BACKGROUND);
    //        //  display.fillRect(CHAR_width_small*30, f * 11   + 6 * 11 - 1 , CHAR_width_small * 15, (10 - f) * 11, RED);
    //        break; // no more files
    //      }
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
  char tmp[6];
  unsigned char buf[256];
  unsigned long chipsize;
  uint32_t sum_used = 0;
  //uint16_t sum_files = 0;
  SerialFlash.readID(buf);
  SerialFlash.opendir();
  while (1)
  {
    char filename[25];
    uint32_t filesize;
    if (SerialFlash.readdir(filename, sizeof(filename), filesize))
    {
      sum_used = sum_used + filesize / 1024;
      fm.flash_sum_files++;
    } else {
      break; // no more files
    }
  }
  display.setTextSize(1);
  display.setCursor (CHAR_width_small * 38 , 4 * CHAR_height_small   );
  display.setTextColor(GREY2, COLOR_BACKGROUND);
  display.print("USED: ");
  display.setTextColor(COLOR_PITCHSMP, COLOR_BACKGROUND);
  sprintf(tmp, "%05d", int(sum_used));
  display.print(tmp);
  display.setTextColor(COLOR_CHORDS, COLOR_BACKGROUND);
  display.print(" KB");
  display.setCursor (CHAR_width_small * 37 , 3 * CHAR_height_small   );
  display.setTextColor(GREY2, COLOR_BACKGROUND);
  display.print("TOTAL: ");
  display.setTextColor(COLOR_PITCHSMP, COLOR_BACKGROUND);
  chipsize = SerialFlash.capacity(buf);
  sprintf(tmp, "%05d", int(chipsize / 1024));
  display.print(tmp);

  display.setTextColor(COLOR_CHORDS, COLOR_BACKGROUND);
  display.print(" KB");
  display.setCursor (CHAR_width_small * 42 , 1 * CHAR_height_small   );
  display.setTextColor(GREY2, COLOR_BACKGROUND);
  display.print("FILES: ");
  display.setTextColor(COLOR_PITCHSMP, COLOR_BACKGROUND);
  seq_print_formatted_number(fm.flash_sum_files, 3);
  display.setCursor (CHAR_width_small * 38 , 5 * CHAR_height_small   );
  display.setTextColor(GREY2, COLOR_BACKGROUND);
  display.print("FREE: ");
  display.setTextColor(COLOR_PITCHSMP, COLOR_BACKGROUND);

  sprintf(tmp, "%05d", int(chipsize / 1024 - sum_used) );
  display.print(tmp);
  display.setTextColor(COLOR_CHORDS, COLOR_BACKGROUND);
  display.print(" KB");
}

void print_sampler_keyboard(int x, int y)
{
  uint8_t offset[5] = {1, 2, 2, 4, 6 }; //+ is the offset to left
  int offcount = 0;

  //draw white keys
  for (uint8_t i = 0; i < 7; i++)
  {
    if (x * CHAR_width_small + 6 * i < DISPLAY_WIDTH - 8)
      display.fillRect( x * CHAR_width_small + 6 * i, y + 23, 5, 15, COLOR_SYSTEXT); // pianoroll white key
  }
  for (uint8_t i = 0; i < 11; i++)
  {
    if (seq.piano[i] == 1)
    {
      if ( x * CHAR_width_small + 4 * i - offset[offcount] < DISPLAY_WIDTH - 12)
        display.fillRect( x * CHAR_width_small + 4 * i - offset[offcount], y + 23, 4, 8, COLOR_BACKGROUND); // BLACK key
      offcount++;
      if (offcount == 5)offcount = 0;
    }
  }

}

void print_note_name_and_octave(uint8_t note)
{
  display.print( noteNames[note % 12][0]);
  if (noteNames[note % 12 ][1] != '\0' )
  {
    display.print(noteNames[note % 12 ][1] );
  }
  display.print( (note / 12) - 1);
  display.print(" ");
}

uint8_t get_distance(uint8_t a, uint8_t b)
{
  if (a == b)
    return 0;
  else if (a > b)
    return a - b;
  else  return b - a;
}

void calc_low_high(uint8_t preset)
{
  uint8_t result = 200;
  uint8_t result_zone = 99;

  for (uint8_t zone = 0; zone < NUM_MULTISAMPLE_ZONES; zone++)
  {
    msz[preset][zone].low = 0;
    msz[preset][zone].high = 0;
  }
  for (uint8_t key = 12; key < 109; key++)
  {
    for (uint8_t zone = 0; zone < NUM_MULTISAMPLE_ZONES; zone++)
    {
      if ( get_distance (msz[preset][zone].rootnote, key) < result)
      {
        result = get_distance (msz[preset][zone].rootnote, key);
        result_zone = zone;
      }
    }
    if (msz[preset][result_zone].rootnote > key && msz[preset][result_zone].rootnote != 0)
    {
      if (msz[preset][result_zone].low > msz[preset][result_zone].rootnote - result  || msz[preset][result_zone].low == 0 )
        msz[preset][result_zone].low = msz[preset][result_zone].rootnote - result;
    }
    else
    {
      if (msz[preset][result_zone].high < msz[preset][result_zone].rootnote + result && msz[preset][result_zone].rootnote != 0)
        msz[preset][result_zone].high = msz[preset][result_zone].rootnote + result;
    }

    result = 200;
    result_zone = 99;
  }
}

void sub_MultiSample_setColor(uint8_t row, uint8_t column)
{
  uint16_t temp_color = 0;
  uint16_t temp_background = 0;

  if ((seq.temp_select_menu == row + 1 && seq.selected_track == column ) || (seq.temp_select_menu == row + 1 && column == 99) )
  {
    temp_color = COLOR_BACKGROUND;
    temp_background = COLOR_SYSTEXT;
  }
  else
  {
    if (row == 0)
      temp_color = COLOR_PITCHSMP;
    else if (row == 1)
      temp_color = COLOR_INSTR;
    else if (row == 2)
      temp_color = COLOR_CHORDS;
    else if (row == 3)
      temp_color = COLOR_ARP;
    else if (row == 4)
      temp_color = COLOR_DRUMS;
    else if (row == 5)
      temp_color = GREEN;
    else if (row == 6)
      temp_color = YELLOW;
    else if (row == 7)
      temp_color = GREY1;

    temp_background = COLOR_BACKGROUND;
  }
  display.setTextColor(temp_color, temp_background);
}

uint16_t get_multisample_zone_color(uint8_t row)
{
  uint16_t temp_color = 0;
  if (seq.temp_select_menu == row + 1 )
    temp_color = RED;
  else if (row == 0)
    temp_color = COLOR_PITCHSMP;
  else if (row == 1)
    temp_color = COLOR_INSTR;
  else if (row == 2)
    temp_color = COLOR_CHORDS;
  else if (row == 3)
    temp_color = COLOR_ARP;
  else if (row == 4)
    temp_color = COLOR_DRUMS;
  else if (row == 5)
    temp_color = GREEN;
  else if (row == 6)
    temp_color = YELLOW;
  else if (row == 7)
    temp_color = GREY1;

  return temp_color;
}

void UI_func_MultiSamplePlay(uint8_t param)
{
  if (LCDML.FUNC_setup())         // ****** SETUP *********
  {
    seq.active_multisample = 0;
    seq_active_function = 99;
    seq.temp_select_menu = 0;
    seq.scrollpos = 0;
    calc_low_high(seq.active_multisample);
    display.fillScreen(COLOR_BACKGROUND);
    encoderDir[ENC_R].reset();
    display.setTextSize(1);
    border0();
    helptext_l ("MOVE X");
    helptext_r ("MOVE Y");
    display.setTextColor(COLOR_SYSTEXT, COLOR_BACKGROUND);
    for (uint8_t oct = 0; oct < 8; oct++)
    {
      print_sampler_keyboard(oct * 7 + 2, 135);
      display.setCursor(CHAR_width_small * 2 + oct * 42, 175);
      display.print(oct + 1);
    }
    display.setTextSize(2);
    setCursor_textGrid(1, 1);
    display.print(F("SAMPLER"));
    setCursor_textGrid(1, 2);
    display.print(F("["));
    setCursor_textGrid(13, 2);
    display.print(F("]"));
    display.setTextSize(1);
    print_flash_stats();
    display.setTextColor(COLOR_SYSTEXT, COLOR_BACKGROUND);

    display.setCursor(2 * CHAR_width_small , (6) * (CHAR_height_small + 2) - 2 );
    display.print(F("Z"));
    display.setCursor(4 * CHAR_width_small , (6) * (CHAR_height_small + 2) - 2 );
    display.print(F("ROOT"));
    display.setCursor(9 * CHAR_width_small , (6) * (CHAR_height_small + 2) - 2 );
    display.print(F("LOW"));
    display.setCursor(13 * CHAR_width_small , (6) * (CHAR_height_small + 2) - 2 );
    display.print(F("HIGH"));
    display.setCursor(18 * CHAR_width_small , (6) * (CHAR_height_small + 2) - 2 );
    display.print(F("VOL"));
    display.setCursor(22 * CHAR_width_small , (6) * (CHAR_height_small + 2) - 2 );
    display.print(F("PAN"));
    display.setCursor(30 * CHAR_width_small , (6) * (CHAR_height_small + 2) - 2 );
    display.print(F("REV"));
    display.setCursor(37 * CHAR_width_small , (6) * (CHAR_height_small + 2) - 2 );
    display.print(F("FILENAME"));
  }
  if (LCDML.FUNC_loop())          // ****** LOOP *********
  {
    if ((LCDML.BT_checkDown() && encoderDir[ENC_R].Down()) || (LCDML.BT_checkUp() && encoderDir[ENC_R].Up()))
    {
      if (seq_active_function == 0 && seq.temp_select_menu == 0)
      {
        if (LCDML.BT_checkDown())
        {
          seq.active_multisample = constrain(seq.active_multisample + 1, 0, NUM_MULTISAMPLES - 1);
        }
        else if (LCDML.BT_checkUp())
        {
          seq.active_multisample = constrain(seq.active_multisample - 1, 0, NUM_MULTISAMPLES - 1);
        }
        calc_low_high(seq.active_multisample);
      }
      if (seq_active_function == 0 && seq.temp_select_menu > 0 && seq.selected_track == 6) //file name selection
      {
        if (LCDML.BT_checkDown())
        {
          seq.scrollpos = constrain(seq.scrollpos + 1, 0, fm.flash_sum_files - 1);
        }
        else if (LCDML.BT_checkUp())
        {
          seq.scrollpos = constrain(seq.scrollpos - 1, 0, fm.flash_sum_files - 1);
        }
        stop_all_drum_slots();
        fill_msz_from_flash_filename(seq.scrollpos, seq.active_multisample, seq.temp_select_menu - 1);
      }
      if (seq_active_function == 0 && seq.temp_select_menu > 0 && seq.selected_track == 5) //reverb send selection
      {
        if (LCDML.BT_checkDown())
        {
          msz[seq.active_multisample][seq.temp_select_menu - 1].rev = constrain(msz[seq.active_multisample][seq.temp_select_menu - 1].rev + 1, 0, 99);
        }
        else if (LCDML.BT_checkUp())
        {
          msz[seq.active_multisample][seq.temp_select_menu - 1].rev = constrain(msz[seq.active_multisample][seq.temp_select_menu - 1].rev - 1, 0, 99);
        }
      }
      else if (seq_active_function == 0 && seq.temp_select_menu > 0 && seq.selected_track == 4) //pan selection
      {
        if (LCDML.BT_checkDown())
        {
          msz[seq.active_multisample][seq.temp_select_menu - 1].pan = constrain(msz[seq.active_multisample][seq.temp_select_menu - 1].pan + 1, PANORAMA_MIN, PANORAMA_MAX);
        }
        else if (LCDML.BT_checkUp())
        {
          msz[seq.active_multisample][seq.temp_select_menu - 1].pan = constrain(msz[seq.active_multisample][seq.temp_select_menu - 1].pan - 1, PANORAMA_MIN, PANORAMA_MAX);
        }
      }
      else if (seq_active_function == 0 && seq.temp_select_menu > 0 && seq.selected_track == 3) //volume selection
      {
        if (LCDML.BT_checkDown())
        {
          msz[seq.active_multisample][seq.temp_select_menu - 1].vol = constrain(msz[seq.active_multisample][seq.temp_select_menu - 1].vol + 1, 0, 100);
        }
        else if (LCDML.BT_checkUp())
        {
          msz[seq.active_multisample][seq.temp_select_menu - 1].vol = constrain(msz[seq.active_multisample][seq.temp_select_menu - 1].vol - 1, 0, 100);
        }
      }
      else if (seq_active_function == 0 && seq.temp_select_menu > 0 && seq.selected_track == 2) //high selection
      {
        if (LCDML.BT_checkDown())
        {
          msz[seq.active_multisample][seq.temp_select_menu - 1].high = constrain(msz[seq.active_multisample][seq.temp_select_menu - 1].high + 1, 12, 127);
        }
        else if (LCDML.BT_checkUp())
        {
          msz[seq.active_multisample][seq.temp_select_menu - 1].high = constrain(msz[seq.active_multisample][seq.temp_select_menu - 1].high - 1, 12, 127);
        }
      }
      else if (seq_active_function == 0 && seq.temp_select_menu > 0 && seq.selected_track == 1) //low selection
      {
        if (LCDML.BT_checkDown())
        {
          msz[seq.active_multisample][seq.temp_select_menu - 1].low = constrain(msz[seq.active_multisample][seq.temp_select_menu - 1].low + 1, 12, 127);
        }
        else if (LCDML.BT_checkUp())
        {
          msz[seq.active_multisample][seq.temp_select_menu - 1].low = constrain(msz[seq.active_multisample][seq.temp_select_menu - 1].low - 1, 12, 127);
        }
      }
      else if (seq_active_function == 0 && seq.temp_select_menu > 0 && seq.selected_track == 0) //root note selection
      {
        if (LCDML.BT_checkDown())
        {
          msz[seq.active_multisample][seq.temp_select_menu - 1].rootnote = constrain(msz[seq.active_multisample][seq.temp_select_menu - 1].rootnote + 1, 12, 127);
        }
        else if (LCDML.BT_checkUp())
        {
          msz[seq.active_multisample][seq.temp_select_menu - 1].rootnote = constrain(msz[seq.active_multisample][seq.temp_select_menu - 1].rootnote - 1, 12, 127);
        }
      }
      else if (seq_active_function == 99) // no option is selected, scroll parameter rows
      {
        if (LCDML.BT_checkDown())
        {
          seq.temp_select_menu = constrain( seq.temp_select_menu + 1, 0, 8);
        }
        else if (LCDML.BT_checkUp())
        {
          seq.temp_select_menu = constrain( seq.temp_select_menu - 1, 0, 8);
        }
      }
    }
    if (LCDML.BT_checkEnter())
    {
      if (seq_active_function == 99)
        seq_active_function = 0;
      else
        seq_active_function = 99;
    }
    display.setTextSize(2);
    if (seq.temp_select_menu == 0 )
      display.setTextColor(COLOR_SYSTEXT, COLOR_BACKGROUND);
    else
      display.setTextColor(GREY2, COLOR_BACKGROUND);
    setCursor_textGrid(1, 2);
    display.print(F("["));
    setCursor_textGrid(13, 2);
    display.print(F("]"));
    display.setTextColor(COLOR_PITCHSMP, COLOR_BACKGROUND);
    show(2, 2, 11, ms[seq.active_multisample].name);

    uint8_t yoffset = 7;
    display.setTextSize(1);
    for (uint8_t y = 0; y < NUM_MULTISAMPLE_ZONES; y++)
    {
      sub_MultiSample_setColor( y, 99);
      setCursor_textGrid_mini(2, y + yoffset);
      display.print(y + 1);
      sub_MultiSample_setColor( y, 0);
      setCursor_textGrid_mini(4, y + yoffset);
      print_note_name_and_octave(msz[seq.active_multisample][y].rootnote);
      display.setTextColor(COLOR_SYSTEXT, COLOR_BACKGROUND);
      display.print(" ");
      setCursor_textGrid_mini(9, y + yoffset);
      sub_MultiSample_setColor( y, 1);
      print_note_name_and_octave(msz[seq.active_multisample][y].low );
      display.setTextColor(COLOR_SYSTEXT, COLOR_BACKGROUND);
      display.print(" ");
      setCursor_textGrid_mini(13, y + yoffset);
      sub_MultiSample_setColor( y, 2);
      print_note_name_and_octave(msz[seq.active_multisample][y].high );
      display.setTextColor(COLOR_SYSTEXT, COLOR_BACKGROUND);
      display.print(" ");
      char tmp[4];
      
      sub_MultiSample_setColor( y, 3);
      show_smallfont_noGrid( (y + yoffset) * (CHAR_height_small + 2), 18 * CHAR_width_small, 3, itoa(msz[seq.active_multisample][y].vol, tmp, 10) );
      //setCursor_textGrid_mini(25, y + yoffset);
      //sub_MultiSample_setColor( y, 4);
      // show_smallfont_noGrid( (y + yoffset) * (CHAR_height_small + 2), 25 * CHAR_width_small, 3, itoa(msz[seq.active_multisample][y].pan, tmp, 10) );

      print_small_panbar(22, yoffset+y , msz[seq.active_multisample][y].pan, y+1);


      sub_MultiSample_setColor( y, 5);
      show_smallfont_noGrid( (y + yoffset) * (CHAR_height_small + 2), 31 * CHAR_width_small, 3, itoa(msz[seq.active_multisample][y].rev, tmp, 10) );
      setCursor_textGrid_mini(33, y + yoffset);
      sub_MultiSample_setColor( y, 6);
      display.print("[");
      show_smallfont_noGrid( (y + yoffset) * (CHAR_height_small + 2), 34 * CHAR_width_small, 17, msz[seq.active_multisample][y].name );
      setCursor_textGrid_mini(51, y + yoffset);
      display.print("]");
      if (msz[seq.active_multisample][y].low == 0 && msz[seq.active_multisample][y].high == 0)
        display.fillRect (0,
                          185 + y * 5,
                          DISPLAY_WIDTH - 1 , 5, COLOR_BACKGROUND);
      else
      {
        display.fillRect (0, 185 + y * 5, 2 * CHAR_width_small + msz[seq.active_multisample][y].low * 3.5 - (24 * 3.5) - 1 , 5, COLOR_BACKGROUND);

        display.fillRect (2 * CHAR_width_small + msz[seq.active_multisample][y].low * 3.5 - (24 * 3.5), 185 + y * 5,
                          (msz[seq.active_multisample][y].high - msz[seq.active_multisample][y].low) * 3.5 + 2.5 , 5, get_multisample_zone_color(y));

        display.fillRect (2 * CHAR_width_small + msz[seq.active_multisample][y].high * 3.5 - (24 * 3.5) + 3.5 - 1,  185 + y * 5,
                          DISPLAY_WIDTH - (msz[seq.active_multisample][y].high  * 3.5  )  + (18 * 3.5) , 5, COLOR_BACKGROUND);

        display.fillRect (2 * CHAR_width_small + msz[seq.active_multisample][y].rootnote * 3.5 - (24 * 3.5) - 1 ,  185 + y * 5 + 1,
                          3.5 + 1 , 5 - 2, COLOR_SYSTEXT);
      }

    }
  }
  if (LCDML.FUNC_close())     // ****** STABLE END *********
  {
    encoderDir[ENC_R].reset();
    display.fillScreen(COLOR_BACKGROUND);
    display.setTextColor(COLOR_SYSTEXT, COLOR_BACKGROUND);
  }
}

void sd_card_count_files_from_directory(File dir)
{
  fm.sd_sum_files = 0;
  dir.rewindDirectory();
  while (true) {
    File entry = dir.openNextFile();
    if (! entry) {
      // no more files
      break;
    }
    //    if (entry.isDirectory()) {
    //      count++;
    //    } else {
    // }
    fm.sd_sum_files++;
    entry.close();
  }
}

void UI_func_file_manager(uint8_t param)
{
  uint32_t volumesize;

  if (LCDML.FUNC_setup())         // ****** SETUP *********
  {
    display.fillScreen(COLOR_BACKGROUND);
    encoderDir[ENC_R].reset();
    display.setTextSize(1);
    border0();
    display.setCursor (CHAR_width_small * 1 , 1 * CHAR_height_small  );
    display.print(F("SD CARD"));
    display.setTextColor(GREY2);
    display.setCursor (CHAR_width_small * 1 , 2 * CHAR_height_small  );
    display.print("FILES:");
    display.setCursor (CHAR_width_small * 12 , 1 * CHAR_height_small  );
    // print the type of card
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
    display.setCursor (CHAR_width_small * 12 , 2 * CHAR_height_small  );
    volumesize = volume.blocksPerCluster();    // clusters are collections of blocks
    volumesize *= volume.clusterCount();       // we'll have a lot of clusters
    display.setTextColor(GREY2);
    display.print("TOTAL: ");
    display.setTextColor(COLOR_PITCHSMP);
    volumesize /= 1024;
    display.print(volumesize);
    display.setTextColor(COLOR_CHORDS);
    display.print(" MB");

    display.setTextColor(COLOR_SYSTEXT);
    display.setCursor (CHAR_width_small * 30 , 1 * CHAR_height_small  );
    display.print(F("SPI FLASH"));
    print_flash_stats();
    display.setTextSize(1);
    display.setCursor (CHAR_width_small * 1 , 3 * CHAR_height_small  );
    display.setTextColor(COLOR_CHORDS, COLOR_BACKGROUND);
    display.print(F("PATH:"));
    display.setCursor (CHAR_width_small * 1 , 4 * CHAR_height_small  );
    display.setTextColor(GREY2);
    display.print(F("CURRENT FILE/FOLDER :"));
    print_file_manager_buttons();
    flash_printDirectory();
  }
  if (LCDML.FUNC_loop())          // ****** LOOP *********
  {
    if ((LCDML.BT_checkDown() && encoderDir[ENC_R].Down()) || (LCDML.BT_checkUp() && encoderDir[ENC_R].Up()))
    {
      if (LCDML.BT_checkDown() && fm.active_window == 0) // left window, SDCARD
      {
        if (fm.sd_selected_file == fm.sd_cap_rows && fm.sd_cap_rows > 8 && fm.sd_skip_files < fm.sd_sum_files - fm.sd_cap_rows - 1)
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
        if (fm.flash_selected_file == fm.flash_cap_rows && fm.flash_cap_rows > 8 && fm.flash_skip_files < fm.flash_sum_files - fm.flash_cap_rows - 1)
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
        display.fillRect( CHAR_width_small * 1, CHAR_height_small * 6, DISPLAY_WIDTH / 2 - CHAR_width_small, CHAR_height_small * 16, COLOR_BACKGROUND);
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
          setCursor_textGrid_mini(1, 6 + screenline);

          display.setTextColor(COLOR_SYSTEXT, COLOR_BACKGROUND);
          //if (filename[0] != 46 && filename[1] != 95)
          if (filename[0] != 46)
          {
            display.print(filename);
            print_empty_spaces(7);
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
                    display.fillRect(CHAR_width_small * 38, CHAR_height_small * 7, count / (f.size() / (14 * CHAR_width_small)) , 8, RED);
                }
                ff.close();
                display.fillRect(CHAR_width_small * 38, CHAR_height_small * 7, (14 * CHAR_width_small) + 2, 8, COLOR_BACKGROUND);
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
        }
        rootdir.close();
        display.fillRect( CHAR_width_small * 1, CHAR_height_small * 6, DISPLAY_WIDTH / 2 - CHAR_width_small, CHAR_height_small * 16, COLOR_BACKGROUND);
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
              //display.fillRect(241, 80, 238, 8, COLOR_BACKGROUND);
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
      fm.sd_currentDirectory = SD.open(fm.sd_new_name);
      sd_printDirectory(fm.sd_currentDirectory);

      sd_card_count_files_from_directory(fm.sd_currentDirectory);
      display.setCursor (CHAR_width_small * 8 , 2 * CHAR_height_small  );
      display.setTextColor(COLOR_PITCHSMP, COLOR_BACKGROUND);
      seq_print_formatted_number(fm.sd_sum_files, 3);
      show_smallfont_noGrid(3 * CHAR_height_small, CHAR_width_small * 7 , 20, fm.sd_new_name);
      show_smallfont_noGrid(5 * CHAR_height_small , CHAR_width_small * 1, 20, fm.sd_temp_name);
    }
    else if (fm.active_window == 1)
    {
      flash_printDirectory();
    }
    //display.setTextSize(1);
    display.setTextColor(COLOR_DRUMS, COLOR_BACKGROUND);

    if (fm.sd_is_folder)
    {
      display.setCursor (CHAR_width_small * 14  , 4 * CHAR_height_small  );
      display.setTextColor(GREEN);
      display.print(F("FOLDER"));
      display.setCursor (CHAR_width_small * 9, 4 * CHAR_height_small );
      display.setTextColor(GREY2);
      display.print(F("FILE"));
    }
    else
    {
      display.setCursor (CHAR_width_small * 9, 4 * CHAR_height_small  );
      display.setTextColor(GREEN);
      display.print(F("FILE"));
      display.setCursor (CHAR_width_small * 14 , 4 * CHAR_height_small  );
      display.setTextColor(GREY2);
      display.print(F("FOLDER"));
    }
    if (fm.sd_is_folder)
      display.setTextColor(COLOR_PITCHSMP, COLOR_BACKGROUND);
    else
      display.setTextColor(COLOR_SYSTEXT, COLOR_BACKGROUND);
  }
  if (LCDML.FUNC_close())     // ****** STABLE END *********
  {
    encoderDir[ENC_R].reset();
    display.fillScreen(COLOR_BACKGROUND);
    display.setTextColor(COLOR_SYSTEXT, COLOR_BACKGROUND);
  }
}

void UI_func_midi_soft_thru(uint8_t param)
{
  static uint8_t old_soft_midi_thru;
  if (LCDML.FUNC_setup())         // ****** SETUP *********
  {
    encoderDir[ENC_R].reset();
    setCursor_textGrid(1, 1);
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
    setCursor_textGrid(1, 2);
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

void print_mixer_text()
{
  if (seq.temp_active_menu == 0)
    display.setTextColor(RED, COLOR_BACKGROUND); else display.setTextColor(COLOR_SYSTEXT, COLOR_BACKGROUND);
  setCursor_textGrid_mini(0, 20);
  display.print(F("DXD"));
  setCursor_textGrid_mini(0, 21);
  display.print(F("#1"));
  if (seq.temp_active_menu == 1)
    display.setTextColor(RED, COLOR_BACKGROUND); else display.setTextColor(COLOR_SYSTEXT, COLOR_BACKGROUND);
  setCursor_textGrid_mini(4, 20);
  display.print(F("DXD"));
  setCursor_textGrid_mini(4, 21);
  display.print(F("#2"));
  if (seq.temp_active_menu == 2)
    display.setTextColor(RED, COLOR_BACKGROUND); else display.setTextColor(COLOR_SYSTEXT, COLOR_BACKGROUND);
  setCursor_textGrid_mini(8, 20);
  display.print(F("MSY"));
  setCursor_textGrid_mini(8, 21);
  display.print(F("#1"));
  if (seq.temp_active_menu == 3)
    display.setTextColor(RED, COLOR_BACKGROUND); else display.setTextColor(COLOR_SYSTEXT, COLOR_BACKGROUND);
  setCursor_textGrid_mini(12, 20);
  display.print(F("MSY"));
  setCursor_textGrid_mini(12, 21);
  display.print(F("#2"));
  if (seq.temp_active_menu == 4)
    display.setTextColor(RED, COLOR_BACKGROUND); else display.setTextColor(COLOR_SYSTEXT, COLOR_BACKGROUND);
  setCursor_textGrid_mini(16, 20);
  display.print(F("DRM"));
  setCursor_textGrid_mini(16, 21);
  display.print(F("L"));
  setCursor_textGrid_mini(20, 20);
  display.print(F("DRM"));
  setCursor_textGrid_mini(20, 21);
  display.print(F("R"));
  if (seq.temp_active_menu == 5)
    display.setTextColor(RED, COLOR_BACKGROUND); else display.setTextColor(COLOR_SYSTEXT, COLOR_BACKGROUND);
  setCursor_textGrid_mini(27, 20);
  display.print(F("REVB"));
  setCursor_textGrid_mini(32, 20);
  display.print(F("REVB"));
  setCursor_textGrid_mini(27, 21);
  display.print(F("L"));
  setCursor_textGrid_mini(32, 21);
  display.print(F("R"));
  if (seq.temp_active_menu == 6)
    display.setTextColor(RED, COLOR_BACKGROUND); else display.setTextColor(COLOR_SYSTEXT, COLOR_BACKGROUND);
  setCursor_textGrid_mini(40, 20);
  display.print(F("MASTER"));
  setCursor_textGrid_mini(47, 20);
  display.print(F("MASTER"));
  setCursor_textGrid_mini(40, 21);
  display.print(F("L"));
  setCursor_textGrid_mini(47, 21);
  display.print(F("R"));
  display.setTextColor(GREY2, COLOR_BACKGROUND);
  setCursor_textGrid_mini(0, 19);
  seq_print_formatted_number(configuration.dexed[0].sound_intensity, 3);
  setCursor_textGrid_mini(4, 19);
  seq_print_formatted_number(configuration.dexed[1].sound_intensity, 3);
  setCursor_textGrid_mini(8, 19);
  seq_print_formatted_number(microsynth[0].sound_intensity, 3);
  setCursor_textGrid_mini(12, 19);
  seq_print_formatted_number(microsynth[1].sound_intensity, 3);
  temp_int = mapfloat(seq.drums_volume, 0.0, VOL_MAX_FLOAT, 0, 100);
  setCursor_textGrid_mini(16, 19);
  seq_print_formatted_number(temp_int, 3);
  setCursor_textGrid_mini(20, 19);
  seq_print_formatted_number(temp_int, 3);
  setCursor_textGrid_mini(27, 19);
  seq_print_formatted_number(configuration.fx.reverb_level, 3);
  setCursor_textGrid_mini(32, 19);
  seq_print_formatted_number(configuration.fx.reverb_level, 3);
  setCursor_textGrid_mini(40, 19);
  seq_print_formatted_number(configuration.sys.vol, 3);
  setCursor_textGrid_mini(47, 19);
  seq_print_formatted_number(configuration.sys.vol, 3);
}
void UI_func_mixer(uint8_t param)
{
  if (LCDML.FUNC_setup())         // ****** SETUP *********
  {
    encoderDir[ENC_R].reset();
    seq.temp_active_menu = 0;
    display.fillScreen(COLOR_BACKGROUND);
    for (uint8_t j = 0; j < uint8_t(sizeof(ts.displayed_peak)); j++)
      ts.displayed_peak[j] = 0;
    setCursor_textGrid(1, 1);
    display.print(F("MIXER"));
    helptext_l("BACK");
    helptext_r ("< > SELECT CH");
    display.setTextColor(COLOR_SYSTEXT, COLOR_BACKGROUND);
    print_mixer_text();
  }
  if (LCDML.FUNC_loop())          // ****** LOOP *********
  {
    if ((LCDML.BT_checkDown() && encoderDir[ENC_R].Down()) || (LCDML.BT_checkUp() && encoderDir[ENC_R].Up()))
    {
      if (!seq.edit_state)  //select channel
      {
        if (LCDML.BT_checkDown())
          seq.temp_active_menu = constrain(seq.temp_active_menu + ENCODER[ENC_R].speed(), 0, 6);
        else if (LCDML.BT_checkUp())
          seq.temp_active_menu = constrain(seq.temp_active_menu - ENCODER[ENC_R].speed(), 0, 6);
      }
      else
      {
        if (seq.temp_active_menu < 2)  //dexed instance #0 or #1
        {
          if (LCDML.BT_checkDown())
            configuration.dexed[seq.temp_active_menu].sound_intensity = constrain(configuration.dexed[seq.temp_active_menu].sound_intensity + ENCODER[ENC_R].speed(), SOUND_INTENSITY_MIN, SOUND_INTENSITY_MAX);
          else if (LCDML.BT_checkUp())
            configuration.dexed[seq.temp_active_menu].sound_intensity = constrain(configuration.dexed[seq.temp_active_menu].sound_intensity - ENCODER[ENC_R].speed(), SOUND_INTENSITY_MIN, SOUND_INTENSITY_MAX);
        }
        else if (seq.temp_active_menu > 1 && seq.temp_active_menu < 4) //microsynth
        {
          if (LCDML.BT_checkDown())
            microsynth[seq.temp_active_menu - 2].sound_intensity = constrain(microsynth[seq.temp_active_menu - 2].sound_intensity + 1, 0, 100);
          else if (LCDML.BT_checkUp())
            microsynth[seq.temp_active_menu - 2].sound_intensity = constrain(microsynth[seq.temp_active_menu - 2].sound_intensity - 1, 0, 100);
        }
        else if (seq.temp_active_menu == 4) //drums
        {
          if (LCDML.BT_checkDown())
            temp_int = constrain(temp_int + ENCODER[ENC_R].speed(), 0, 100);
          else if (LCDML.BT_checkUp())
            temp_int = constrain(temp_int - ENCODER[ENC_R].speed(), 0, 100);
          seq.drums_volume = mapfloat(temp_int, 0, 100, 0.0, VOL_MAX_FLOAT);
        }
        else if (seq.temp_active_menu == 5) //reverb level
        {
          if (LCDML.BT_checkDown())
            configuration.fx.reverb_level = constrain(configuration.fx.reverb_level + ENCODER[ENC_R].speed(), REVERB_LEVEL_MIN, REVERB_LEVEL_MAX);
          else if (LCDML.BT_checkUp())
            configuration.fx.reverb_level = constrain(configuration.fx.reverb_level - ENCODER[ENC_R].speed(), REVERB_LEVEL_MIN, REVERB_LEVEL_MAX);
        }
        else if (seq.temp_active_menu == 6) //master level
        {
          if (LCDML.BT_checkDown() )
          {
            configuration.sys.vol = constrain(configuration.sys.vol + ENCODER[ENC_L].speed(), VOLUME_MIN, VOLUME_MAX);
          }
          else if (LCDML.BT_checkUp() )
          {
            configuration.sys.vol = constrain(configuration.sys.vol - ENCODER[ENC_L].speed(), VOLUME_MIN, VOLUME_MAX);
          }
        }
      }
    }
    else  if (LCDML.BT_checkEnter())
    {
      seq.edit_state = !seq.edit_state;
      border1_clear();
      if (!seq.edit_state)
      {
        display.setTextSize(2);
        setCursor_textGrid(1, 1);
        display.print(F("MIXER"));
      }
    }
    if (seq.edit_state)
      helptext_r ("CHANGE VOLUME");
    else
      helptext_r ("< > SELECT CHANNEL");
    display.setTextSize(2);
    if (seq.temp_active_menu < 2 && seq.edit_state) // dexed 0 or dexed 1 instance selected
    {
      display.setTextColor(COLOR_SYSTEXT, COLOR_BACKGROUND);
      display_bar_int("", configuration.dexed[seq.temp_active_menu].sound_intensity, 1.0, SOUND_INTENSITY_MIN, SOUND_INTENSITY_MAX, 3, false, false, false);
      setCursor_textGrid(1, 1);
      display.print("DEXED #");
      display.print(seq.temp_active_menu + 1);
      MD_sendControlChange(configuration.dexed[seq.temp_active_menu].midi_channel, 7, configuration.dexed[seq.temp_active_menu].sound_intensity);
      MicroDexed[seq.temp_active_menu]->setGain(midi_volume_transform(map(configuration.dexed[seq.temp_active_menu].sound_intensity, SOUND_INTENSITY_MIN, SOUND_INTENSITY_MAX, 0, 127)));
    }
    if (seq.temp_active_menu > 1 && seq.temp_active_menu < 4 && seq.edit_state) // //microsynth
    {
      display.setTextColor(COLOR_SYSTEXT, COLOR_BACKGROUND);
      display_bar_int("", microsynth[seq.temp_active_menu - 2].sound_intensity, 1.0, SOUND_INTENSITY_MIN, SOUND_INTENSITY_MAX, 3, false, false, false);
      setCursor_textGrid(1, 1);
      display.print("MICROSYNTH #");
      display.print(seq.temp_active_menu - 2 + 1);
    }
    else if (seq.temp_active_menu == 4 && seq.edit_state) // drums
    {
      display.setTextColor(COLOR_SYSTEXT, COLOR_BACKGROUND);
      temp_int = mapfloat(seq.drums_volume, 0.0, VOL_MAX_FLOAT, 0, 100);
      display_bar_int("DRUMS VOLUME", temp_int, 1.0, SOUND_INTENSITY_MIN, SOUND_INTENSITY_MAX, 3, false, false, true);
      master_mixer_r.gain (3, volume_transform(mapfloat(temp_int, 0, 100, 0.0, VOL_MAX_FLOAT)));
      master_mixer_l.gain (3, volume_transform(mapfloat(temp_int, 0, 100, 0.0, VOL_MAX_FLOAT)));
    }
    else if (seq.temp_active_menu == 5 && seq.edit_state) // reverb level
    {
      display.setTextColor(COLOR_SYSTEXT, COLOR_BACKGROUND);
      setCursor_textGrid(1, 1);
      display.print(F("REVERB LEVEL"));
      display_bar_int("", configuration.fx.reverb_level, 1.0, REVERB_LEVEL_MIN, REVERB_LEVEL_MAX, 3, false, false, false);
      master_mixer_r.gain(MASTER_MIX_CH_REVERB, volume_transform(mapfloat(configuration.fx.reverb_level, REVERB_LEVEL_MIN, REVERB_LEVEL_MAX, 0.0, VOL_MAX_FLOAT)));
      master_mixer_l.gain(MASTER_MIX_CH_REVERB, volume_transform(mapfloat(configuration.fx.reverb_level, REVERB_LEVEL_MIN, REVERB_LEVEL_MAX, 0.0, VOL_MAX_FLOAT)));
    }
    else if (seq.temp_active_menu == 6 && seq.edit_state) // master volume
    {
      display.setTextColor(COLOR_SYSTEXT, COLOR_BACKGROUND);
      setCursor_textGrid(1, 1);
      display.print(F("MASTER VOLUME"));
      display_bar_int("", configuration.sys.vol, 1.0, VOLUME_MIN, VOLUME_MAX, 3, false, false, false);
      set_volume(configuration.sys.vol, configuration.sys.mono);
    }
    display.setTextSize(1);
    print_mixer_text();
  }
  if (LCDML.FUNC_close())     // ****** STABLE END *********
  {
    encoderDir[ENC_R].reset();
    display.fillScreen(COLOR_BACKGROUND);
  }
}

void UI_func_smart_filter(uint8_t param)
{
  if (LCDML.FUNC_setup())         // ****** SETUP *********
  {
    encoderDir[ENC_R].reset();

    setCursor_textGrid(1, 1);
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
    setCursor_textGrid(1, 2);
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
    display.fillRect(14 * CHAR_width + 11, 5, 9, 9, COLOR_SYSTEXT );
    display.setCursor(14 * CHAR_width + 13, 6);
    if (configuration.dexed[0].polyphony == 0)
      display.setTextColor(RED);
    else
      display.setTextColor(COLOR_BACKGROUND);
    display.print(F("1"));

    display.fillRect(15 * CHAR_width + 11, 5, 9, 9, COLOR_BACKGROUND );
    display.setCursor(15 * CHAR_width + 13, 6);
    if (configuration.dexed[1].polyphony == 0)
      display.setTextColor(RED);
    else
      display.setTextColor(COLOR_SYSTEXT);
    display.print(F("2"));
  }
  else
  {
    display.fillRect(14 * CHAR_width + 11, 5, 9, 9, COLOR_BACKGROUND );
    display.setCursor(14 * CHAR_width + 13, 6);
    if (configuration.dexed[0].polyphony == 0)
      display.setTextColor(RED);
    else
      display.setTextColor(COLOR_SYSTEXT);
    display.print(F("1"));

    display.fillRect(15 * CHAR_width + 11, 5, 9, 9, COLOR_SYSTEXT );
    display.setCursor(15 * CHAR_width + 13, 6);
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

  display.setTextSize(1);
  display.setCursor(x, y);
  if (selected_instance_id == instance_id)
    display.setTextColor(COLOR_SYSTEXT, COLOR_BACKGROUND);
  else
    display.setTextColor(GREY2, COLOR_BACKGROUND);
  display.print("INST "); display.setTextColor(COLOR_PITCHSMP, COLOR_BACKGROUND);
  if (selected_instance_id == instance_id)
    display.setTextColor(GREEN, COLOR_BACKGROUND);
  else
    display.setTextColor(GREY2, COLOR_BACKGROUND);
  display.print(instance_id + 1);
  if (selected_instance_id == instance_id)
    display.setTextColor(COLOR_PITCHSMP, COLOR_BACKGROUND);
  else
    display.setTextColor(GREY2, COLOR_BACKGROUND);
  display.setCursor(x + 54, y - 1);
  seq_print_formatted_number(configuration.dexed[instance_id].bank, 2);
  display.setCursor(x + 54, y + 7);
  seq_print_formatted_number(configuration.dexed[instance_id].voice + 1, 2);
  if (selected_instance_id == instance_id)
    display.setTextColor(COLOR_SYSTEXT, COLOR_BACKGROUND);
  else
    display.setTextColor(GREY2, COLOR_BACKGROUND);
  show_smallfont_noGrid(y - 1, x + 81, 12, g_bank_name[instance_id]);
  display.setCursor(x + 81 , y + 7);
  display.print(g_voice_name[instance_id]);

  // static content
  if (fullrefresh)
  {
    display.setTextSize(1);
    display.setTextColor(GREY2, COLOR_BACKGROUND);
    //yspacer = yspacer + 9;
    display.setCursor(x, y + yspacer);
    display.setTextColor(GREY2, COLOR_BACKGROUND);
    display.print(F("VOL"));
    display.setCursor(x + 80, y + yspacer);
    display.print(F("CHRS"));
    yspacer = yspacer + 9; display.setCursor(x, y + yspacer);
    display.setCursor(x, y + yspacer);
    display.print(F("TRANSP"));
    display.setCursor(x + 80, y + yspacer);
    display.print(F("DELAY"));
    yspacer = yspacer + 9; display.setCursor(x, y + yspacer);
    display.setCursor(x , y + yspacer);
    display.print(F("MIDI CH"));
    display.setCursor(x + 80, y + yspacer);
    display.print(F("REVERB"));
    yspacer = yspacer + 9; display.setCursor(x, y + yspacer);
    display.setTextSize(1);
    display.print(F("PAN "));

    yspacer = yspacer + 9; display.setCursor(x, y + yspacer);
    display.print(F("LOW NOTE"));
    display.setCursor(x + 80, y + yspacer);
    display.print(F("HI NOTE"));
  }
  display.setTextSize(1);
  if (selected_instance_id == instance_id)
    display.setTextColor(COLOR_PITCHSMP, COLOR_BACKGROUND);
  else
    display.setTextColor(GREY2, COLOR_BACKGROUND);
  yspacer = 16;

  display.setCursor(x + 9 * CHAR_width_small,  y + yspacer);
  display.print(configuration.dexed[instance_id].sound_intensity);
  display.setCursor(x + 21 * CHAR_width_small, y + yspacer);
  display.print(configuration.fx.chorus_level[instance_id]);
  yspacer = yspacer + 9;
  display.setCursor(x + 21 * CHAR_width_small, y + yspacer);
  display.print(configuration.fx.delay_level[instance_id]);
  display.setCursor(x + 9 * CHAR_width_small, y + yspacer);
  display.print(configuration.dexed[instance_id].transpose);
  yspacer = yspacer + 9;
  display.setCursor(x + 9 * CHAR_width_small, y + yspacer);
  seq_print_formatted_number( configuration.dexed[instance_id].midi_channel, 2 );
  display.setCursor(x + 21 * CHAR_width_small, y + yspacer);
  display.print(configuration.fx.reverb_send[instance_id]);
  yspacer = yspacer + 9;
  display.setCursor(x + 9 * CHAR_width_small, y + yspacer);
  display.print(configuration.dexed[instance_id].pan);

  yspacer = yspacer + 9;
  display.setCursor(x + 9 * CHAR_width_small, y + yspacer);
  display.print(configuration.dexed[instance_id].lowest_note);
  display.setCursor(x + 21 * CHAR_width_small, y + yspacer);
  display.print(configuration.dexed[instance_id].highest_note);
}

void print_perfmod_lables()
{
  char tmp[5];
  display.setTextSize(1);
  if (dexed_live_mod.active_button == 1)
  {
    display.setTextColor(COLOR_SYSTEXT, DX_DARKCYAN);
    display.setCursor( 4 * CHAR_width_small + 3, 194);
    sprintf(tmp, "%03d", dexed_live_mod.attack_mod[0]);
    display.print(tmp);
  }
  else
  {
    display.setTextColor(GREY1, GREY2);
    display.setCursor( 4 * CHAR_width_small + 3, 194);
    sprintf(tmp, "%03d", dexed_live_mod.attack_mod[0]);
    display.print(tmp);
  }
  if (dexed_live_mod.active_button == 2)
  {
    display.setTextColor(COLOR_SYSTEXT, DX_DARKCYAN);
    display.setCursor( 17 * CHAR_width_small + 3, 194);
    sprintf(tmp, "%03d", dexed_live_mod.release_mod[0]);
    display.print(tmp);
  }
  else
  {
    display.setTextColor(GREY1, GREY2);
    display.setCursor( 17 * CHAR_width_small + 3, 194);
    sprintf(tmp, "%03d", dexed_live_mod.release_mod[0]);
    display.print(tmp);
  }
  if (dexed_live_mod.active_button == 3)
  {
    display.setTextColor(COLOR_SYSTEXT, DX_DARKCYAN);
    display.setCursor( (4 + 27) * CHAR_width_small + 3, 194);
    sprintf(tmp, "%03d", dexed_live_mod.attack_mod[1]);
    display.print(tmp);
  }
  else
  {
    display.setTextColor(GREY1, GREY2);
    display.setCursor( (4 + 27) * CHAR_width_small + 3, 194);
    sprintf(tmp, "%03d", dexed_live_mod.attack_mod[1]);
    display.print(tmp);
  }
  if (dexed_live_mod.active_button == 4)
  {
    display.setTextColor(COLOR_SYSTEXT, DX_DARKCYAN);
    display.setCursor( (17 + 27) * CHAR_width_small + 3, 194);
    sprintf(tmp, "%03d", dexed_live_mod.release_mod[1]);
    display.print(tmp);
  }
  else
  {
    display.setTextColor(GREY1, GREY2);
    display.setCursor( (17 + 27) * CHAR_width_small + 3, 194);
    sprintf(tmp, "%03d", dexed_live_mod.release_mod[1]);
    display.print(tmp);
  }
  display.setTextColor(COLOR_SYSTEXT, COLOR_BACKGROUND);
}

void print_perfmod_buttons()
{
  // Performance Modifier Buttons
  if (dexed_live_mod.active_button == 1)
    draw_button_on_grid(1, 22, "ATTACK", "MD", 1);
  else
    draw_button_on_grid(1, 22, "ATTACK", "MD", 0);
  if (dexed_live_mod.active_button == 2)
    draw_button_on_grid(14, 22, "REL.", "MD", 1);
  else
    draw_button_on_grid(14, 22, "REL.", "MD", 0);
  if (dexed_live_mod.active_button == 3)
    draw_button_on_grid(28, 22, "ATTACK", "MD", 1);
  else
    draw_button_on_grid(28, 22, "ATTACK", "MD", 0);
  if (dexed_live_mod.active_button == 4)
    draw_button_on_grid(41, 22, "REL.", "MD", 1);
  else
    draw_button_on_grid(41, 22, "REL.", "MD", 0);
}

void print_voice_select_default_help()
{
  helptext_l ("BACK");
  helptext_r ("< > SELECT SOUND/BANK");
  display.setTextColor(GREY2, COLOR_BACKGROUND);
  display.setCursor(0, DISPLAY_HEIGHT - (CHAR_height_small * 2) - 2  );
  display.print(F("LONG PUSH:"));
  display.setTextColor(COLOR_ARP, COLOR_BACKGROUND);
  display.print(F("SET/DELETE"));
  display.setCursor(CHAR_width_small * 10, DISPLAY_HEIGHT - CHAR_height_small );
  display.setTextColor(COLOR_BACKGROUND, GREEN );
  display.print(F("F"));
  display.setTextColor(COLOR_ARP, COLOR_BACKGROUND);
  display.setCursor(CHAR_width_small * 11, DISPLAY_HEIGHT - CHAR_height_small  );
  display.print(F("AVORITE"));
  display.setTextColor(GREY2, COLOR_BACKGROUND);
  display.setCursor(CHAR_width_small * 23 + 2, DISPLAY_HEIGHT - (CHAR_height_small * 2) - 2 );
  display.print(F("SHORT:"));
  display.setTextColor(COLOR_ARP, COLOR_BACKGROUND);
  display.print(F("INSTANCE "));
  display.setTextColor(GREY2, COLOR_BACKGROUND);
  display.print(F("LONG:"));
  display.setTextColor(COLOR_ARP, COLOR_BACKGROUND);
  display.print(F("SOUND/BANK"));
}
void UI_func_voice_select(uint8_t param)
{
  static uint8_t menu_voice_select = MENU_VOICE_SOUND;

  if (LCDML.FUNC_setup())         // ****** SETUP *********
  {
    display.fillScreen(COLOR_BACKGROUND);
    border1();
    border2();
    display.drawRect(0, CHAR_height * 6 - 4, DISPLAY_WIDTH, DISPLAY_HEIGHT - CHAR_height * 6 - CHAR_height_small * 3,  GREY4);
    if (seq.cycle_touch_element != 1)
    {
      display.drawLine(DISPLAY_WIDTH / 2, CHAR_height * 6 - 4 , DISPLAY_WIDTH / 2, DISPLAY_HEIGHT - CHAR_height_small * 4 + 2 ,  GREY4);
      draw_button_on_grid(45, 1, "", "", 99); //print keyboard icon

      print_voice_settings(CHAR_width_small, 104, 0, true);
      print_voice_settings(CHAR_width_small + 160, 104, 1, true);
      print_voice_select_default_help();
      print_perfmod_buttons();
      print_perfmod_lables();
    }
    else
    {
      draw_button_on_grid(45, 1, "DEXED" , "DETAIL", 0);
      virtual_keyboard();
      virtual_keyboard_print_buttons();
      virtual_keyboard_print_current_instrument();
    }
    display.setTextColor(GREY2, COLOR_BACKGROUND);
    show_smallfont_noGrid( 4 * CHAR_height - 10, CHAR_width, 13, "PERFORMANCE #");
    display.setCursor(CHAR_width, 4 * CHAR_height  );
    display.setTextColor(COLOR_PITCHSMP, COLOR_BACKGROUND);
    display.setTextSize(1);
    seq_print_formatted_number(configuration.sys.performance_number, 2);
    display.setTextColor(COLOR_SYSTEXT, GREY3);
    show_smallfont_noGrid( 4 * CHAR_height, CHAR_width + 17, 11, seq.name);
    display.setTextColor(COLOR_SYSTEXT, COLOR_BACKGROUND);
    UI_draw_waveform(activesample);
    seq.last_drawn_sample = 254;
    encoderDir[ENC_R].reset();
    // char bank_name[BANK_NAME_LEN];
    // char voice_name[VOICE_NAME_LEN];
    // get_bank_name(configuration.dexed[selected_instance_id].bank, bank_name, sizeof(bank_name));
    // get_voice_name(configuration.dexed[selected_instance_id].bank, configuration.dexed[selected_instance_id].voice, voice_name, sizeof(voice_name));
    UI_update_instance_icons();
  }

  if (LCDML.FUNC_loop())          // ****** LOOP *********
  {
    if (dexed_live_mod.active_button != 0)  //touch button pressed for live modify
    {
      if ((LCDML.BT_checkDown() && encoderDir[ENC_R].Down()) || (LCDML.BT_checkUp() && encoderDir[ENC_R].Up()))
      {
        if ( (LCDML.BT_checkDown() && dexed_live_mod.active_button == 1) || (LCDML.BT_checkDown() && dexed_live_mod.active_button == 3))
        {
          if (dexed_live_mod.attack_mod[selected_instance_id] == 0)
            for (uint8_t i = 0; i < 6; i++)
            {
              dexed_live_mod.orig_attack_values[selected_instance_id][i] = MicroDexed[selected_instance_id]->getOPRate(i, ATTACK);
            }
          dexed_live_mod.attack_mod[selected_instance_id] = constrain(dexed_live_mod.attack_mod[selected_instance_id] + ENCODER[ENC_L].speed(), -MAX_PERF_MOD, MAX_PERF_MOD);
          for (uint8_t i = 0; i < 6; i++)
            MicroDexed[selected_instance_id]->setOPRate(i, ATTACK, dexed_live_mod.orig_attack_values[selected_instance_id][i] - dexed_live_mod.attack_mod[selected_instance_id] );
        }
        else if ((LCDML.BT_checkUp() && dexed_live_mod.active_button == 1 ) || (LCDML.BT_checkUp() && dexed_live_mod.active_button == 3 ))
        {
          if (dexed_live_mod.attack_mod[selected_instance_id] == 0)  // Save initial Values
            for (uint8_t i = 0; i < 6; i++)
            {
              dexed_live_mod.orig_attack_values[selected_instance_id][i] = MicroDexed[selected_instance_id]->getOPRate(i, ATTACK);
            }

          dexed_live_mod.attack_mod[selected_instance_id] = constrain(dexed_live_mod.attack_mod[selected_instance_id] - ENCODER[ENC_L].speed(), -MAX_PERF_MOD, MAX_PERF_MOD);
          for (uint8_t i = 0; i < 6; i++)
            MicroDexed[selected_instance_id]->setOPRate(i, ATTACK, dexed_live_mod.orig_attack_values[selected_instance_id][i] - dexed_live_mod.attack_mod[selected_instance_id] );
        }
        if ((LCDML.BT_checkDown() && dexed_live_mod.active_button == 2 ) || (LCDML.BT_checkDown() && dexed_live_mod.active_button == 4))
        {
          if (dexed_live_mod.release_mod[selected_instance_id] == 0) // Save initial Values
            for (uint8_t i = 0; i < 6; i++)
            {
              dexed_live_mod.orig_release_values[selected_instance_id][i] = MicroDexed[selected_instance_id]->getOPRate(i, RELEASE);
            }
          dexed_live_mod.release_mod[selected_instance_id] = constrain(dexed_live_mod.release_mod[selected_instance_id] + ENCODER[ENC_L].speed(), -MAX_PERF_MOD, MAX_PERF_MOD);
          for (uint8_t i = 0; i < 6; i++)
            MicroDexed[selected_instance_id]->setOPRate(i, RELEASE, dexed_live_mod.orig_release_values[selected_instance_id][i] - dexed_live_mod.release_mod[selected_instance_id] );
        }
        else if ((LCDML.BT_checkUp() && dexed_live_mod.active_button == 2 ) || ( LCDML.BT_checkUp() && dexed_live_mod.active_button == 4))
        {
          if (dexed_live_mod.release_mod[selected_instance_id] == 0)
            for (uint8_t i = 0; i < 6; i++)
            {
              dexed_live_mod.orig_release_values[selected_instance_id][i] = MicroDexed[selected_instance_id]->getOPRate(i, RELEASE);
            }
          dexed_live_mod.release_mod[selected_instance_id] = constrain(dexed_live_mod.release_mod[selected_instance_id] - ENCODER[ENC_L].speed(), -MAX_PERF_MOD, MAX_PERF_MOD);
          for (uint8_t i = 0; i < 6; i++)
            MicroDexed[selected_instance_id]->setOPRate(i, RELEASE, dexed_live_mod.orig_release_values[selected_instance_id][i] - dexed_live_mod.release_mod[selected_instance_id] );
        }
      }
      else if (LCDML.BT_checkEnter())
      {
        dexed_live_mod.active_button = 0;
        print_perfmod_buttons();
        dexed_live_mod.active_button = 99;
        print_voice_select_default_help();
      }
      if (seq.cycle_touch_element != 1)
        print_perfmod_lables();
    }
    if (dexed_live_mod.active_button == 0)
    {
      if ((LCDML.BT_checkDown() && encoderDir[ENC_R].Down()) || (LCDML.BT_checkUp() && encoderDir[ENC_R].Up()) || (LCDML.BT_checkEnter() && (encoderDir[ENC_R].ButtonShort() || encoderDir[ENC_R].ButtonLong())))
      {
        uint8_t bank_tmp;
        int8_t voice_tmp;

        // Reset Performance Modifiers to 0 after every preset change

        dexed_live_mod.attack_mod[selected_instance_id] = 0;
        dexed_live_mod.release_mod[selected_instance_id] = 0;
        if (seq.cycle_touch_element != 1)
          print_perfmod_lables();

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
                break;
            }
          }
          else {
            switch (configuration.sys.favorites) {
              case 1: //only Favs
                locate_previous_favorite();
                break;
              case 2: //only non-Favs
                locate_previous_non_favorite();
                break;
              case 3: //random non-Favs
                locate_random_non_favorite();
                break;
            }
          }

          load_sd_voice(configuration.dexed[selected_instance_id].bank, configuration.dexed[selected_instance_id].voice, selected_instance_id);
        }  //end UP
        else if (LCDML.BT_checkDown())
        {
          //start : show all presets
          if (configuration.sys.favorites == 0)
          {
            switch (menu_voice_select)
            {
              case MENU_VOICE_BANK:
                bank_tmp = constrain(configuration.dexed[selected_instance_id].bank + ENCODER[ENC_R].speed(), 0, MAX_BANKS - 1);
                configuration.dexed[selected_instance_id].bank = bank_tmp;
                break;

              case MENU_VOICE_SOUND:
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
                break;
            }
          }
          else { // favs
            switch (configuration.sys.favorites) {
              case 1: //only Favs
                locate_next_favorite();
                break;
              case 2: //only non-Favs
                locate_next_non_favorite();
                break;
              case 3: //random non-Favs
                locate_random_non_favorite();
                break;
            }
          }

          load_sd_voice(configuration.dexed[selected_instance_id].bank, configuration.dexed[selected_instance_id].voice, selected_instance_id);
        }
        else if (LCDML.BT_checkEnter() && encoderDir[ENC_R].ButtonPressed() && dexed_live_mod.active_button != 99)
        {
          if (menu_voice_select == MENU_VOICE_BANK)
            menu_voice_select = MENU_VOICE_SOUND;
          else
            menu_voice_select = MENU_VOICE_BANK;
        }
#if NUM_DEXED > 1
        else if (LCDML.BT_checkEnter() && dexed_live_mod.active_button != 99)
        {
          selected_instance_id = !selected_instance_id;

          UI_update_instance_icons();
        }
#endif
      }

      display.setTextSize(2);
      display.setTextColor(COLOR_PITCHSMP, COLOR_BACKGROUND);
      setCursor_textGrid(1, 1);
      seq_print_formatted_number(configuration.dexed[selected_instance_id].bank, 2);
      setCursor_textGrid(1, 2);
      seq_print_formatted_number(configuration.dexed[selected_instance_id].voice + 1, 2);
      display.setTextColor(COLOR_SYSTEXT, COLOR_BACKGROUND);
      show(1, 5, 8, g_bank_name[selected_instance_id]);
      show(2, 5, 10, g_voice_name[selected_instance_id]);

      display.setTextColor(GREY2, COLOR_BACKGROUND);
      switch (menu_voice_select)
      {
        case MENU_VOICE_BANK:
          show(1, 4, 1, "[");
          show(1, 13, 1, "]");
          show(2, 4, 1, " ");
          show(2, 15, 1, " ");
          break;
        case MENU_VOICE_SOUND:
          show(1, 4, 1, " ");
          show(1, 13, 1, " ");
          show(2, 4, 1, "[");
          show(2, 15, 1, "]");
          break;
      }
      display.setTextColor(COLOR_SYSTEXT, COLOR_BACKGROUND);
      draw_favorite_icon(configuration.dexed[selected_instance_id].bank, configuration.dexed[selected_instance_id].voice, selected_instance_id);

      if (seq.cycle_touch_element != 1)
      {
        print_voice_settings(CHAR_width_small, 104, 0, false);
        print_voice_settings(CHAR_width_small + 160, 104, 1, false);
      }
    }
    if (dexed_live_mod.active_button == 99) // if button press had confirmed live mod settings and is now unselected,
      dexed_live_mod.active_button = 0;     // skip button push from voice_select once, then back to normal
  }
  if (LCDML.FUNC_close())     // ****** STABLE END *********
  {
    display.fillScreen(COLOR_BACKGROUND);
    encoderDir[ENC_R].reset();
    dexed_live_mod.active_button = 0;
  }
}

void UI_func_volume(uint8_t param)
{
  static uint8_t old_volume;
  if (LCDML.FUNC_setup())         // ****** SETUP *********
  {
    old_volume = configuration.sys.vol;
    display.setTextSize(2);
    display.setTextColor(COLOR_SYSTEXT, COLOR_BACKGROUND);
    encoderDir[ENC_L].reset();
    back_from_volume = 0;
  }
  if (LCDML.FUNC_loop())          // ****** LOOP *********
  {
    if ((LCDML.BT_checkDown() && encoderDir[ENC_L].Down() ) || (LCDML.BT_checkUp() && encoderDir[ENC_L].Up() ))
    {
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
    display.setTextSize(2);
    display.setTextColor(COLOR_SYSTEXT, COLOR_BACKGROUND);
    //Master Volume
    setCursor_textGrid(1, 1);
    display.print(F("Master Volume"));
    display_bar_int("Master Vol.", configuration.sys.vol, 1.0, VOLUME_MIN, VOLUME_MAX, 3, false, false, false);
    set_volume(configuration.sys.vol, configuration.sys.mono);
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
  }
}

void UI_func_save_voice(uint8_t param)
{
  static bool yesno;
  static uint8_t mode;

  if (LCDML.FUNC_setup())         // ****** SETUP *********
  {
    encoderDir[ENC_R].reset();
    display.setTextColor(COLOR_SYSTEXT, COLOR_BACKGROUND);
    yesno = false;
#if NUM_DEXED == 1
    mode = 1;
#else
    mode = 0;
#endif

#if NUM_DEXED == 1
    setCursor_textGrid(1, 1);
    display.print(F("Save to Bank"));
    show(2, 1, 2, configuration.dexed[selected_instance_id].bank);
    show(2, 5, 10, g_bank_name[selected_instance_id]);
    show(2, 3, 2, " [");
    show(2, 15, 1, "]");
#else
    setCursor_textGrid(1, 1);
    display.print(F("Save Instance"));
    if (selected_instance_id == 0)
      display.setTextColor(COLOR_SYSTEXT, COLOR_BACKGROUND);
    else
      display.setTextColor(GREY2, COLOR_BACKGROUND);
    setCursor_textGrid(5, 2);
    display.print("0");
    if (selected_instance_id == 1)
      display.setTextColor(COLOR_SYSTEXT, COLOR_BACKGROUND);
    else
      display.setTextColor(GREY2, COLOR_BACKGROUND);
    setCursor_textGrid(10, 2);
    display.print("1");
    helptext_l("BACK");
    helptext_r("< > SELECT INSTANCE");
    display.setTextSize(2);
    display.setTextColor(COLOR_SYSTEXT, COLOR_BACKGROUND);
#endif
  }
  if (LCDML.FUNC_loop())          // ****** LOOP *********
  {
    if ((LCDML.BT_checkDown() && encoderDir[ENC_R].Down()) || (LCDML.BT_checkUp() && encoderDir[ENC_R].Up()))
    {
      switch (mode)
      {
        case 0: // Instance selection
          if (LCDML.BT_checkDown() || LCDML.BT_checkUp())
            selected_instance_id = !selected_instance_id;
          if (selected_instance_id == 0)
            display.setTextColor(COLOR_SYSTEXT, COLOR_BACKGROUND);
          else
            display.setTextColor(GREY2, COLOR_BACKGROUND);
          setCursor_textGrid(5, 2);
          display.print("0");
          if (selected_instance_id == 1)
            display.setTextColor(COLOR_SYSTEXT, COLOR_BACKGROUND);
          else
            display.setTextColor(GREY2, COLOR_BACKGROUND);
          setCursor_textGrid(10, 2);
          display.print("1");
          break;
        case 1: // Bank selection
          if (LCDML.BT_checkDown())
            configuration.dexed[selected_instance_id].bank = constrain(configuration.dexed[selected_instance_id].bank + ENCODER[ENC_R].speed(), 0, MAX_BANKS - 1);
          else if (LCDML.BT_checkUp() && configuration.dexed[selected_instance_id].bank > 0)
            configuration.dexed[selected_instance_id].bank = constrain(configuration.dexed[selected_instance_id].bank - ENCODER[ENC_R].speed(), 0, MAX_BANKS - 1);

          // get bank name from sysex on SD
          get_bank_name(configuration.dexed[selected_instance_id].bank, g_bank_name[selected_instance_id]);

          show(2, 1, 2, configuration.dexed[selected_instance_id].bank);
          show(2, 5, 10, g_bank_name[selected_instance_id]);
          break;
        case 2: // Voice selection
          if (LCDML.BT_checkDown() && configuration.dexed[selected_instance_id].voice < MAX_VOICES - 1)
            configuration.dexed[selected_instance_id].voice = constrain(configuration.dexed[selected_instance_id].voice + ENCODER[ENC_R].speed(), 0, MAX_VOICES - 1);
          else if (LCDML.BT_checkUp() && configuration.dexed[selected_instance_id].voice > 0)
            configuration.dexed[selected_instance_id].voice = constrain(configuration.dexed[selected_instance_id].voice - ENCODER[ENC_R].speed(), 0, MAX_VOICES - 1);

          // get voice name from sysex on SD
          get_voice_name(configuration.dexed[selected_instance_id].bank, configuration.dexed[selected_instance_id].voice, g_voice_name[selected_instance_id]);
          MicroDexed[selected_instance_id]->getName(g_voice_name[selected_instance_id]);

          show(2, 1, 2, configuration.dexed[selected_instance_id].voice + 1);
          show(2, 5, 10, g_voice_name[selected_instance_id]);
          break;
        case 3: // Yes/No selection
          yesno = !yesno;
          if (yesno == true)
          {
            show(2, 2, 3, "YES");
          }
          else
          {
            show(2, 2, 3, "NO");
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
          setCursor_textGrid(1, 1);
          display.print(F("Save to Bank "));
          show(2, 1, 2, configuration.dexed[selected_instance_id].bank);
          show(2, 5, 10, g_bank_name[selected_instance_id]);
          show(2, 3, 2, " [");
          show(2, 15, 1, "]");
          helptext_r("< > SELECT DESTINATION BANK");
          display.setTextSize(2);
          display.setTextColor(COLOR_SYSTEXT, COLOR_BACKGROUND);
          break;
        case 2:
          show(1, 1, 14, "Save to Voice ");
          // show(1, 16, 2, configuration.dexed[selected_instance_id].bank);
          show(2, 1, 2, configuration.dexed[selected_instance_id].voice + 1);
          show(2, 5, 10, g_voice_name[selected_instance_id]);
          helptext_r("< > SELECT DESTINATION VOICE");
          display.setTextSize(2);
          display.setTextColor(COLOR_SYSTEXT, COLOR_BACKGROUND);
          break;
        case 3:
          show(1, 1, 16, "Overwrite?");
          show(2, 1, 15, "[NO");
          show(2, 5, 1, "]");
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
            show(2, 1, 16, "Done.");
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
      show(2, 1, 16, "Canceled.");
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

    setCursor_textGrid(1, 1);
    display.print(F("MIDI Recv Bank"));
    setCursor_textGrid(3, 2);
    display.print(F("["));
    setCursor_textGrid(15, 2);
    display.print(F("]"));
    strcpy(receive_bank_filename, g_bank_name[selected_instance_id]);

    show(2, 1, 2, bank_number);
    show(2, 4, 10, receive_bank_filename);
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
            strcpy(receive_bank_filename, g_bank_name[selected_instance_id]);
            show(2, 1, 2, bank_number);
            show(2, 4, 10, receive_bank_filename);
            break;
          case 1:
            yesno = !yesno;
            if (yesno)
              show(2, 13, 3, "YES");
            else
              show(2, 13, 3, "NO");
            break;
          case 2:
            ui_select_name_state = UI_select_name(2, 2, receive_bank_filename, BANK_NAME_LEN - 1, false);
            break;
        }
      }
      else if (LCDML.BT_checkUp())
      {
        switch (mode)
        {
          case 0:
            bank_number = constrain(bank_number - ENCODER[ENC_R].speed(), 0, MAX_BANKS - 1);
            strcpy(receive_bank_filename, g_bank_name[selected_instance_id]);
            show(2, 1, 2, bank_number);
            show(2, 4, 10, receive_bank_filename);
            break;
          case 1:
            yesno = !yesno;
            if (yesno)
              show(2, 13, 3, "YES");
            else
              show(2, 13, 3, "NO");
            break;
          case 2:
            ui_select_name_state = UI_select_name(2, 2, receive_bank_filename, BANK_NAME_LEN - 1, false);
            break;
        }
      }
    }
    else if (LCDML.BT_checkEnter() && encoderDir[ENC_R].ButtonShort())
    {
      if (mode == 0)
      {
        if (strcmp(receive_bank_filename, "*ERROR*") != 0)
        {
          yesno = true;
          strcpy(receive_bank_filename, "NONAME");
          mode = 2;
          setCursor_textGrid(1, 2);
          display.setTextColor(COLOR_SYSTEXT, COLOR_BACKGROUND);
          display.print(F("[          ]    "));
          ui_select_name_state = UI_select_name(2, 2, receive_bank_filename, BANK_NAME_LEN - 1, true);
          // fix_later   lcd.blink();
        }
        else
        {
          mode = 1;
          setCursor_textGrid(1, 2);
          display.print(F("Overwrite: [NO ]"));
        }
      }
      else if (mode == 1 && yesno == true)
      {
        mode = 2;
        setCursor_textGrid(1, 2);
        display.print(F("[          ]    "));
        ui_select_name_state = UI_select_name(2, 2, receive_bank_filename, BANK_NAME_LEN - 1, true);
        // fix_later   lcd.blink();
      }
      else if (mode == 2)
      {
        ui_select_name_state = UI_select_name(2, 2, receive_bank_filename, BANK_NAME_LEN - 1, false);
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
            setCursor_textGrid(1, 2);
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
        setCursor_textGrid(1, 2);
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
      setCursor_textGrid(1, 2);
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
    setCursor_textGrid(1, 1);
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
          setCursor_textGrid(1, 2);
          display.print(F("OK.              "));
          delay(MESSAGE_WAIT_TIME);
          LCDML.FUNC_goBackToMenu();
        }
      }
    }
    if (mode == 0 )
    {
      mode = 1;
      strcpy(seq.name_temp, seq.name);
      setCursor_textGrid(1, 2);
      display.print(F("[          ]    "));
      ui_select_name_state = UI_select_name(2, 2, seq.name_temp, BANK_NAME_LEN - 1, true);
    }
  }
  if (LCDML.FUNC_close())     // ****** STABLE END *********
  {
    encoderDir[ENC_R].reset();
  }
}

void UI_func_sysex_send_bank(uint8_t param)
{
  static uint8_t bank_number;

  if (LCDML.FUNC_setup())         // ****** SETUP *********
  {
    encoderDir[ENC_R].reset();
    bank_number = configuration.dexed[selected_instance_id].bank;
    strcpy(tmp_bank_name, g_bank_name[selected_instance_id]);
    setCursor_textGrid(1, 1);
    display.print(F("MIDI Send Bank"));
    show(2, 3, 1, "[");
    show(2, 15, 1, "]");
    show(2, 1, 2, configuration.dexed[selected_instance_id].bank);
    show(2, 4, 10, tmp_bank_name);
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

      get_bank_name(bank_number, tmp_bank_name);
#ifdef DEBUG
      Serial.printf("send bank sysex %d - bank:[%s]\n", bank_number, tmp_bank_name);
#endif
      show(2, 1, 2, bank_number);
      show(2, 4, 10, tmp_bank_name);
    }
    else if (LCDML.BT_checkEnter() && encoderDir[ENC_R].ButtonShort())
    {
      if (strcmp("*ERROR*", tmp_bank_name) != 0)
      {
        char filename[FILENAME_LEN];
        sprintf(filename, "/%d/%s.syx", bank_number, tmp_bank_name);
#ifdef DEBUG
        Serial.print(F("Send bank "));
        Serial.print(filename);
        Serial.println(F(" from SD."));
#endif
        File sysex = SD.open(filename);
        if (!sysex)
        {
#ifdef DEBUG
          Serial.println(F("Cannot read from SD."));
#endif
          show(2, 1, 16, "Read error.");
          bank_number = 0xff;
        }
        else
        {
          uint8_t bank_data[4104];
          sysex.read(bank_data, 4104);
          sysex.close();

          show(2, 1, 16, "Sending Ch");
          if (configuration.dexed[selected_instance_id].midi_channel == MIDI_CHANNEL_OMNI)
          {
            show(2, 12, 2, "01");
            send_sysex_bank(1, bank_data);
          }
          else
          {
            show(2, 12, 2, configuration.dexed[selected_instance_id].midi_channel);
            send_sysex_bank(configuration.dexed[selected_instance_id].midi_channel, bank_data);
          }
          show(2, 1, 16, "Done.");
          bank_number = 0xff;
        }
      }
      else
      {
        show(2, 1, 16, "No bank.");
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
      setCursor_textGrid(1, 2);
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
    display.setTextColor(COLOR_SYSTEXT, COLOR_BACKGROUND);
    encoderDir[ENC_R].reset();
    mode = 0;
    bank_number = configuration.dexed[selected_instance_id].bank;
    voice_number = configuration.dexed[selected_instance_id].voice;

    strcpy(tmp_bank_name, g_bank_name[selected_instance_id]);
    strcpy(tmp_voice_name, g_voice_name[selected_instance_id]);
    MicroDexed[selected_instance_id]->getName(tmp_voice_name);

    setCursor_textGrid(1, 1);
    display.print(F("MIDI Send Voice"));
    show(2, 1, 2, bank_number);
    show(2, 5, 10, g_bank_name[selected_instance_id]);
    show(2, 4, 1, "[");
    show(2, 15, 1, "]");
    helptext_l("BACK");
    helptext_r("< > SELECT BANK");
    display.setTextSize(2);
    display.setTextColor(COLOR_SYSTEXT, COLOR_BACKGROUND);
  }
  if (LCDML.FUNC_loop())          // ****** LOOP *********
  {
    if ((LCDML.BT_checkDown() && encoderDir[ENC_R].Down()) || (LCDML.BT_checkUp() && encoderDir[ENC_R].Up()))
    {
      switch (mode)
      {
        case 0: // Bank selection
          if (LCDML.BT_checkDown())
            bank_number = constrain(bank_number + ENCODER[ENC_R].speed(), 0, MAX_BANKS - 1);
          else if (LCDML.BT_checkUp() && bank_number > 0)
            bank_number = constrain(bank_number - ENCODER[ENC_R].speed(), 0, MAX_BANKS - 1);

          get_bank_name(bank_number, tmp_bank_name);
          show(2, 1, 2, bank_number);
          show(2, 5, 10, tmp_bank_name);
          break;
        case 1: // Voice selection
          if (LCDML.BT_checkDown() && voice_number < MAX_VOICES - 1)
            voice_number = constrain(voice_number + ENCODER[ENC_R].speed(), 0, MAX_VOICES - 1);
          else if (LCDML.BT_checkUp() && voice_number > 0)
            voice_number = constrain(voice_number - ENCODER[ENC_R].speed(), 0, MAX_VOICES - 1);

          get_voice_name(bank_number, voice_number, tmp_voice_name);
          MicroDexed[selected_instance_id]->getName(tmp_voice_name);

          show(2, 1, 2, voice_number + 1);
          show(2, 5, 10, tmp_voice_name);
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
          show(2, 1, 2, voice_number + 1);
          show(2, 5, 10, tmp_voice_name);
          helptext_r("< > SELECT VOICE");
          display.setTextSize(2);
          display.setTextColor(COLOR_SYSTEXT, COLOR_BACKGROUND);
          break;
        case 2:
          if (strcmp("*ERROR*", tmp_bank_name) != 0)
          {
            char filename[FILENAME_LEN];
            sprintf(filename, "/%d/%s.syx", bank_number, tmp_bank_name);
#ifdef DEBUG
            Serial.print(F("Send voice "));
            Serial.print(voice_number);
            Serial.print(F(" of "));
            Serial.print(filename);
            Serial.println(F(" from SD."));
#endif
            File sysex = SD.open(filename);
            if (!sysex)
            {
#ifdef DEBUG
              Serial.println(F("Connot read from SD."));
#endif
              show(2, 1, 16, "Read error.");
              bank_number = 0xff;
            }
            else
            {
              uint8_t voice_data[155];
              uint8_t encoded_voice_data[128];

              sysex.seek(6 + (voice_number * 128));
              sysex.read(encoded_voice_data, 128);

              MicroDexed[selected_instance_id]->decodeVoice(voice_data, encoded_voice_data);

              show(2, 1, 16, "Sending Ch");
              if (configuration.dexed[selected_instance_id].midi_channel == MIDI_CHANNEL_OMNI)
              {
                show(2, 12, 2, "01");
                send_sysex_voice(1, voice_data);
              }
              else
              {
                show(2, 12, 2, configuration.dexed[selected_instance_id].midi_channel);
                send_sysex_voice(configuration.dexed[selected_instance_id].midi_channel, voice_data);
              }
              delay(MESSAGE_WAIT_TIME);
              show(2, 1, 16, "Done.");
              sysex.close();

              bank_number = 0xff;
            }
          }
          else
          {
            show(2, 1, 16, "No voice.");
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
      show(2, 1, 16, "Canceled.");
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
    setCursor_textGrid(1, 1);
    display.print(F("EQ Low-Cut"));
    setCursor_textGrid(1, 2);
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
    setCursor_textGrid(1, 1);
    display.print(F("EQ 120Hz"));
    setCursor_textGrid(1, 2);
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
    setCursor_textGrid(1, 1);
    display.print(F("EQ 220Hz"));
    setCursor_textGrid(1, 2);
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
    setCursor_textGrid(1, 1);
    display.print(F("EQ 1000Hz"));
    setCursor_textGrid(1, 2);
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
    setCursor_textGrid(1, 1);
    display.print(F("EQ 2000Hz"));
    setCursor_textGrid(1, 2);
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
    setCursor_textGrid(1, 1);
    display.print(F("EQ 7000Hz"));
    setCursor_textGrid(1, 2);
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
    setCursor_textGrid(1, 1);
    display.print(F("EQ High-Cut"));
    setCursor_textGrid(1, 2);
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

void UI_func_startup_performance(uint8_t param)
{
  bool stored = false;
  static uint8_t old_load_at_startup_performance;

  if (LCDML.FUNC_setup())         // ****** SETUP *********
  {
    helptext_l("BACK");
    helptext_r ("< > SELECT PERFORMANCE");
    display.setTextSize(2);
    display.setTextColor(COLOR_SYSTEXT, COLOR_BACKGROUND);
    old_load_at_startup_performance = configuration.sys.load_at_startup_performance;

    encoderDir[ENC_R].reset();
    show(1, 1, 16, "Load at startup");
    if (configuration.sys.load_at_startup_performance == 255)
      show(2, 1, 16, "Last Performance");
    else if (configuration.sys.load_at_startup_performance <= PERFORMANCE_NUM_MAX)
    {
      show(2, 1, 16, "Fixed Perf. [");
      setCursor_textGrid(14, 2);
      seq_print_formatted_number(configuration.sys.load_at_startup_performance, 2);
      show(2, 16, 1, "]");
    }
  }

  if (LCDML.FUNC_loop())          // ****** LOOP *********
  {
    if ((LCDML.BT_checkDown() && encoderDir[ENC_R].Down()) || (LCDML.BT_checkUp() && encoderDir[ENC_R].Up()) || (LCDML.BT_checkEnter() && encoderDir[ENC_R].ButtonShort()))
    {
      if (LCDML.BT_checkDown())
      {
        if (configuration.sys.load_at_startup_performance == 255)
          configuration.sys.load_at_startup_performance = PERFORMANCE_NUM_MIN;
        else if (configuration.sys.load_at_startup_performance >= 0 && configuration.sys.load_at_startup_performance <= PERFORMANCE_NUM_MAX)
          configuration.sys.load_at_startup_performance++;
        if (configuration.sys.load_at_startup_performance > PERFORMANCE_NUM_MAX)
          configuration.sys.load_at_startup_performance = 255;
      }
      else if (LCDML.BT_checkUp())
      {
        if (configuration.sys.load_at_startup_performance == 255)
          configuration.sys.load_at_startup_performance = PERFORMANCE_NUM_MAX;
        else if (configuration.sys.load_at_startup_performance >= PERFORMANCE_NUM_MIN && configuration.sys.load_at_startup_performance <= PERFORMANCE_NUM_MAX)
          configuration.sys.load_at_startup_performance--;
      }
      else if (LCDML.BT_checkEnter())
      {
        stored = true;
        show(2, 1, 16, "Done.");
        save_sd_sys_json();
        if (configuration.sys.load_at_startup_performance <= PERFORMANCE_NUM_MAX && configuration.sys.load_at_startup_performance != configuration.sys.performance_number)
          load_sd_performance_json(configuration.sys.load_at_startup_performance);
        delay(MESSAGE_WAIT_TIME);
        LCDML.FUNC_goBackToMenu();
      }

      display.setCursor(1, 2);
      if (configuration.sys.load_at_startup_performance == 255)
        show(2, 1, 16, "Last Performance");
      else if (configuration.sys.load_at_startup_performance <= PERFORMANCE_NUM_MAX)
      {
        show(2, 1, 16, "Fixed Perf. [");
        setCursor_textGrid(14, 2);
        seq_print_formatted_number(configuration.sys.load_at_startup_performance, 2);
        show(2, 16, 1, "]");
        display.setTextColor(COLOR_PITCHSMP, COLOR_BACKGROUND);
        if (check_sd_performance_exists(configuration.sys.load_at_startup_performance))
        {
          get_sd_performance_name_json(configuration.sys.load_at_startup_performance);
          if ( seq.name_temp[0] != 0 )
            show(3, 1, 16, seq.name_temp);
          else
            show(3, 1, 16, "----  DATA  ----");
        }
        else
          show(3, 1, 16,   "-- EMPTY SLOT --");
      }
      display.setTextColor(COLOR_SYSTEXT, COLOR_BACKGROUND);
    }
  }

  if (LCDML.FUNC_close())     // ****** STABLE END *********
  {
    if (stored == false)
    {
      show(2, 1, 16, "Canceled.");
      configuration.sys.load_at_startup_performance = old_load_at_startup_performance;
      delay(MESSAGE_WAIT_TIME);
    }
    encoderDir[ENC_R].reset();
  }
}

void UI_func_startup_page(uint8_t param)
{
  if (LCDML.FUNC_setup())         // ****** SETUP *********
  {
    encoderDir[ENC_R].reset();
    helptext_l("BACK");
    helptext_r ("< > SELECT PAGE");
    display.setTextSize(2);
    display.setTextColor(COLOR_SYSTEXT, COLOR_BACKGROUND);
    display.setCursor(0, 0);
    show(1, 1, 16, "Startup Page");
  }
  if (LCDML.FUNC_loop())  // ****** LOOP *********
  {
    if ((LCDML.BT_checkDown() && encoderDir[ENC_R].Down()) || (LCDML.BT_checkUp() && encoderDir[ENC_R].Up()) || (LCDML.BT_checkEnter() && encoderDir[ENC_R].ButtonShort()))
    {
      if (LCDML.BT_checkDown())
      {
        configuration.sys.load_at_startup_page++;
        if (configuration.sys.load_at_startup_page > 5)
          configuration.sys.load_at_startup_page = 5;
      }
      else if (LCDML.BT_checkUp())
      {
        if (configuration.sys.load_at_startup_page > 0)
          configuration.sys.load_at_startup_page--;
      }
      else if (LCDML.BT_checkEnter())
      {
        display.setTextColor(COLOR_SYSTEXT, COLOR_BACKGROUND);
        save_sd_sys_json();
        show(2, 1, 18, "Done.");
        delay(MESSAGE_WAIT_TIME);
        LCDML.FUNC_goBackToMenu();
      }
    }
    display.setTextColor(COLOR_PITCHSMP, COLOR_BACKGROUND);
    if (configuration.sys.load_at_startup_page == 0)
      show(2, 1, 17, "DX Voice Select");
    else if (configuration.sys.load_at_startup_page == 1)
      show(2, 1, 17, "Song");
    else if (configuration.sys.load_at_startup_page == 2)
      show(2, 1, 17, "Pattern Editor");
    else if (configuration.sys.load_at_startup_page == 3)
      show(2, 1, 17, "MicroSynth");
    else if (configuration.sys.load_at_startup_page == 4)
      show(2, 1, 17, "Tracker");
    else if (configuration.sys.load_at_startup_page == 5)
      show(2, 1, 17, "MultiSample");
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
    setCursor_textGrid(x, y);
    display.print(edit_string);
    //overwrite first char with inversed character
    setCursor_textGrid(x, y);
    display.setTextColor(COLOR_BACKGROUND, COLOR_PITCHSMP);
    display.print(edit_string[0]);
    display.setTextColor(GREY3, COLOR_BACKGROUND);
    setCursor_textGrid(x + len + 2, y);
    display.print(" OK ");
    display.setTextColor( COLOR_SYSTEXT, COLOR_BACKGROUND);
    setCursor_textGrid(x, y);
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
        display.setTextColor(COLOR_BACKGROUND, COLOR_DRUMS);
        setCursor_textGrid(x + edit_pos, y);
        display.print(edit_string[edit_pos]);
        display.setTextColor( COLOR_SYSTEXT, COLOR_BACKGROUND);
      }
      else
      {
        // if (edit_string[edit_pos] != 0 && edit_string[edit_pos] != 32)
        if (edit_string[edit_pos] != 0 )
        {
          edit_pos = constrain(edit_pos + 1, 0, len);
          if (edit_pos < len)
          {
            setCursor_textGrid(x + edit_pos, y);
            display.setTextColor(COLOR_BACKGROUND, COLOR_PITCHSMP);
            display.print(edit_string[edit_pos]); // highlight current char
            display.setTextColor( COLOR_SYSTEXT, COLOR_BACKGROUND);
          }
          if (edit_pos > 0)
          {
            display.setTextColor( COLOR_SYSTEXT, COLOR_BACKGROUND);
            setCursor_textGrid(x + edit_pos - 1, y);
            display.print(edit_string[edit_pos - 1]); // normal char to the left
          }
        }
        else
        {
          if (edit_pos + 1 > last_char_pos)
            edit_pos = len;
        }
        if (edit_pos == len)
        {
          display.setTextColor(COLOR_BACKGROUND, GREEN);
          setCursor_textGrid(x + len + 2, y);
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
          edit_value = 0;
        edit_string[edit_pos] = accepted_chars[edit_value];
        display.setTextColor(COLOR_BACKGROUND, COLOR_DRUMS);
        setCursor_textGrid(x + edit_pos, y);
        display.print(edit_string[edit_pos]);
        display.setTextColor( COLOR_SYSTEXT, COLOR_BACKGROUND);
      }
      else
      {
        if (edit_pos == len)
        {
          display.setTextColor(GREY3, COLOR_BACKGROUND);
          setCursor_textGrid(x + len + 2, y);
          display.print("[OK]");
          display.setTextColor(COLOR_SYSTEXT, COLOR_BACKGROUND);
        }
        if (edit_pos <= len  && edit_pos > 0)
        {
          setCursor_textGrid(x + edit_pos - 1, y);
          display.setTextColor(COLOR_BACKGROUND, COLOR_PITCHSMP);
          display.print(edit_string[edit_pos - 1]); // highlight current char
          if (edit_pos < len)
          {
            setCursor_textGrid(x + edit_pos, y);
            display.setTextColor( COLOR_SYSTEXT, COLOR_BACKGROUND);
            display.print(edit_string[edit_pos]); // unhighlight char to the right
          }
        }
        if (edit_pos - 1 > last_char_pos)
          edit_pos = last_char_pos;
        else
          edit_pos = constrain(edit_pos - 1, 0, len - 1);

        if (edit_pos == last_char_pos)
        {
          display.setTextColor( COLOR_SYSTEXT, COLOR_BACKGROUND);
          setCursor_textGrid(x - 1, y);
          display.print("[");
          setCursor_textGrid(x + len, y);
          display.print("]");
          setCursor_textGrid(x + len + 1, y);
          display.print("    ");
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
    // if (edit_mode == false && edit_pos < len && edit_string[edit_pos] != 0 && edit_string[edit_pos] != 32)
    if (edit_mode == false && edit_pos < len && edit_string[edit_pos] != 0 )
    {
      display.setTextColor( COLOR_SYSTEXT, COLOR_BACKGROUND);
      setCursor_textGrid(x + edit_pos, y);
      display.print(edit_string[edit_pos]); // unhighlight char
      edit_pos++;
      if (edit_pos < len)
      {
        display.setTextColor(COLOR_BACKGROUND, COLOR_PITCHSMP);
        setCursor_textGrid(x + edit_pos, y);
        display.print(edit_string[edit_pos]); // highlight next char
        display.setTextColor( COLOR_SYSTEXT, COLOR_BACKGROUND);
      }
      else
      {
        display.setTextColor(COLOR_BACKGROUND, GREEN);
        setCursor_textGrid(x + len + 2, y);
        display.print("[OK]");
        display.setTextColor(COLOR_SYSTEXT, COLOR_BACKGROUND);
      }
    }
    if (edit_mode == true)
    {
      setCursor_textGrid(x + len + 1, y);
      display.print("*");
      display.setTextColor(COLOR_BACKGROUND, COLOR_DRUMS);
      setCursor_textGrid(x + edit_pos, y);
      display.print(edit_string[edit_pos]); // highlight char
      display.setTextColor( COLOR_SYSTEXT, COLOR_BACKGROUND);
    }
    else
    {
      display.setTextColor( COLOR_SYSTEXT, COLOR_BACKGROUND);
      setCursor_textGrid(x + len + 1, y);
      display.print(" ");
    }
  }
  display.setTextColor( COLOR_SYSTEXT, COLOR_BACKGROUND);
  encoderDir[ENC_R].reset();
  return (false);
}

uint8_t search_accepted_char(uint8_t c)
{
  // if (c == 0)
  //   c = 32;

  for (uint8_t i = 0; i < sizeof(accepted_chars) - 1; i++)
  {
#ifdef DEBUG
    Serial.print(i, DEC);
    Serial.print(":");
    Serial.print(c);
    Serial.print("==");
    Serial.println(accepted_chars[i], DEC);
#endif
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

  v = float((value - min_value) * (display_cols - size)) / (max_value - min_value);
  vf = uint8_t(modff(v, &_vi) * 10.0 + 0.5);
  vi = uint8_t(_vi);

  if (sign == true)
    size += 1;

  // Title
  if (init == true)
    show(1, 1, display_cols - 2, title);

  // Value
  display.setCursor( CHAR_width * (display_cols - size - 3), CHAR_height * 2);
  display_float(value * factor, size_number, size_fraction, zeros, false, sign); // TBD

  // Bar

  if (vi == 0)
  {
    drawBitmap(  CHAR_width , 2 * CHAR_height, block_bar[ (uint8_t)(vf / 1.25 - 0.5)  ], 8, 15, COLOR_SYSTEXT, COLOR_BACKGROUND);
    for (uint8_t i = vi + 1; i < display_cols - size ; i++)
      display.fillRect( CHAR_width + i * 8, 2 * CHAR_height, 8, 15, COLOR_BACKGROUND ); //empty block
  }
  else
  {
    for (uint8_t i = 0; i < vi; i++)
      drawBitmap(  CHAR_width + i * 8 , 2 * CHAR_height, block_bar[7], 8, 15, COLOR_SYSTEXT, COLOR_BACKGROUND); // full block
    if (vi < display_cols - size)
      drawBitmap(  CHAR_width + vi * 8 , 2 * CHAR_height, block_bar[ (uint8_t)(vf / 1.25 - 0.5)  ], 8, 15, COLOR_SYSTEXT, COLOR_BACKGROUND);
    for (uint8_t i = vi + 1; i < display_cols + 1 - size; i++)
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

  v = float((value - min_value) * (display_cols - size)) / (max_value - min_value);
  vf = uint8_t(modff(v, &_vi) * 10.0 + 0.5);
  vi = uint8_t(_vi);

  if (init == true)
  {
    // Title
    display.setCursor(CHAR_width, CHAR_height);
    display.print(title);
  }

  // Value
  display.setCursor( (display_cols - size - 3)*CHAR_width, CHAR_height * 2);
  display_float((value + offset) * factor, size_number, size_fraction, zeros, false, sign);

  // Bar

  if (vi == 0)
  {
    drawBitmap(  CHAR_width , 2 * CHAR_height + 4, meter_bar[ (uint8_t)(vf / 1.25 - 0.5)  ], 8, 8, COLOR_SYSTEXT, COLOR_BACKGROUND);
    for (uint8_t i = 1; i < display_cols - size; i++)
      display.fillRect( CHAR_width + i * 8, 2 * CHAR_height + 4, 8, 8, COLOR_BACKGROUND ); //empty block
  }
  else if (vi == display_cols - size)
  {
    for (uint8_t i = 0; i < display_cols - size - 1; i++)
      display.fillRect( CHAR_width + i * 8, 2 * CHAR_height + 4, 8, 8, COLOR_BACKGROUND ); //empty block
    drawBitmap(  CHAR_width + (display_cols - size - 1) * 8 , 2 * CHAR_height + 4, meter_bar[7], 8, 8, COLOR_SYSTEXT, COLOR_BACKGROUND);
  }
  else
  {
    for (uint8_t i = 0; i < display_cols - size + 2; i++)
      display.fillRect( CHAR_width + i * 8, 2 * CHAR_height + 4, 8, 8, COLOR_BACKGROUND ); //empty block
    drawBitmap(  CHAR_width + vi * 8 , 2 * CHAR_height + 4, meter_bar[ (uint8_t)(vf / 1.25 - 0.5)  ], 8, 8, COLOR_SYSTEXT, COLOR_BACKGROUND);
    for (uint8_t i = vi + 1; i < display_cols - size + 2; i++)
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
  show(1, 1, display_cols - 2, "Delay Sync");
  if (seq.running == false)show(2, 1, 10, "MIDI Sync   "); else show(2, 0, 10, "Seq. Sync   ");
  switch (sync)
  {
    case 1:
      show(2, 13, 6, "1/16");
      break;
    case 2:
      show(2, 13, 6, "1/16T");
      break;
    case 3:
      show(2, 13, 6, "1/8");
      break;
    case 4:
      show(2, 13, 6, "1/8T");
      break;
    case 5:
      show(2, 13, 6, "1/4");
      break;
    case 6:
      show(2, 13, 6, "1/4T");
      break;
    case 7:
      show(2, 13, 6, "1/2");
      break;
    case 8:
      show(2, 13, 6, "1/2T");
      break;
    case 9:
      show(2, 13, 6, "1/1");
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
  show(2, 18, 1, "!");
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
  display.setTextSize(2);
  setCursor_textGrid(4, 1);
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
    display.setTextSize(2);
    setCursor_textGrid(4, 1);
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

    setCursor_textGrid(4, 1);
    display.setTextSize(2);
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
  setCursor_textGrid(4, 1);
  display.setTextSize(2);
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
    helptext_r ("FORMAT");
    helptext_l ("BACK");
    display.setTextColor(COLOR_SYSTEXT, COLOR_BACKGROUND);
    display.setTextSize(2);
    setCursor_textGrid(1, 1);
    display.print(F("FORMAT FLASH?  "));
    setCursor_textGrid(1, 2);
    display.print(F("PUSH TO CONFIRM"));
  }
  if (LCDML.FUNC_loop())          // ****** LOOP *********
  {
    if (LCDML.BT_checkEnter())
    {
      uint8_t screenline = 0;
      setCursor_textGrid(1, 1);
      display.print(F("Formatting     "));
      setCursor_textGrid(1, 2);
      print_empty_spaces(16);
      unsigned char id[5];
      SerialFlash.readID(id);
      unsigned long size = SerialFlash.capacity(id);
      if (size > 0) {
        setCursor_textGrid(1, 4);
        display.setTextColor(COLOR_SYSTEXT, COLOR_BACKGROUND);
        display.print(F("FLASH MEMORY HAS"));
        setCursor_textGrid(1, 5);
        display.print(size);
        display.print(F(" BYTES."));
        setCursor_textGrid(1, 7);
        display.setTextColor(RED, COLOR_BACKGROUND);
        display.print(F("Erasing ALL Flash Memory"));
        setCursor_textGrid(1, 8);
        display.setTextColor(GREY1, COLOR_BACKGROUND);
        display.print(F("Estimated time: "));
        int seconds = (float)size / eraseBytesPerSecond(id) + 0.5;
        display.setTextColor(COLOR_SYSTEXT, COLOR_BACKGROUND);
        display.print(seconds);
        display.setTextColor(GREY1, COLOR_BACKGROUND);
        display.print(F(" sec."));
        SerialFlash.eraseAll();
        unsigned long dotMillis = millis();
        unsigned char dotcount = 0;
        display.setTextColor(RED, COLOR_BACKGROUND);
        while (SerialFlash.ready() == false)
        {
          if (millis() - dotMillis > 1000)
          {
            dotMillis = dotMillis + 1000;
            setCursor_textGrid(1 + dotcount, 10 + screenline);
            display.print(".");
            dotcount = dotcount + 1;
            if (dotcount >= 24)
            {
              screenline++;
              dotcount = 0;
            }
          }
        }
        if (dotcount > 0)
        {
          display.fillRect(0, 7 * CHAR_height, DISPLAY_WIDTH, DISPLAY_HEIGHT, COLOR_BACKGROUND);
          setCursor_textGrid(1, 2);
          display.setTextColor(GREEN, COLOR_BACKGROUND);
          display.print(F("done!           "));
          helptext_l ("BACK");
          display.setTextColor(COLOR_SYSTEXT, COLOR_BACKGROUND);
        }
      }
    }
  }
  if (LCDML.FUNC_close())     // ****** STABLE END *********
  {
    encoderDir[ENC_R].reset();
    display.setTextColor(COLOR_SYSTEXT, COLOR_BACKGROUND);
    display.fillScreen(COLOR_BACKGROUND);
  }
}

void UI_func_clear_song(uint8_t param)
{
  if (LCDML.FUNC_setup())         // ****** SETUP *********
  {
    border3_large_clear();
    encoderDir[ENC_R].reset();
    helptext_r ("CLEAR SONG");
    helptext_l ("BACK");
    display.setTextColor(COLOR_SYSTEXT, COLOR_BACKGROUND);
    display.setTextSize(2);
    setCursor_textGrid(1, 1);
    display.print(F("CLEAR SONG?  "));
    setCursor_textGrid(1, 2);
    display.setTextColor(RED, COLOR_BACKGROUND);
    display.print(F("PUSH "));
    display.setTextColor(COLOR_SYSTEXT, DX_DARKCYAN);
    display.print(F("[ENC R]"));
    display.setTextColor(RED, COLOR_BACKGROUND);
    display.print(F(" TO CONFIRM"));
    display.setTextSize(1);
    setCursor_textGrid(1, 6);
    display.print(F("ALL SONG DATA WILL BE DELETED !"));
    display.setTextColor(COLOR_SYSTEXT, COLOR_BACKGROUND);
    setCursor_textGrid(1, 8);
    display.print(F("CHAINS, CHAIN TRANSPOSES AND PATTERNS"));
    setCursor_textGrid(1, 9);
    display.print(F("WILL NOT BE TOUCHED."));
    display.setTextSize(2);
  }
  if (LCDML.FUNC_loop())          // ****** LOOP *********
  {
    if (LCDML.BT_checkEnter())
    {
      for (uint8_t i = 0; i < NUM_SEQ_TRACKS ; i++)
      {
        memset(seq.song[i], 99, sizeof(seq.song[i]));
      }
      setCursor_textGrid(1, 1);
      display.print(F("Done."));
      print_empty_spaces(10);
      setCursor_textGrid(1, 2);
      print_empty_spaces(23);
      helptext_l ("");
      border3_large_clear();
      delay(MESSAGE_WAIT_TIME);
      LCDML.FUNC_goBackToMenu();
    }
  }
  if (LCDML.FUNC_close())     // ****** STABLE END *********
  {
    encoderDir[ENC_R].reset();
    display.setTextColor(COLOR_SYSTEXT, COLOR_BACKGROUND);
    display.fillScreen(COLOR_BACKGROUND);
  }
}

void UI_func_clear_song_chains(uint8_t param)
{
  if (LCDML.FUNC_setup())         // ****** SETUP *********
  {
    border3_large_clear();
    encoderDir[ENC_R].reset();
    helptext_r ("CLEAR ALL CHAINS");
    helptext_l ("BACK");
    display.setTextColor(COLOR_SYSTEXT, COLOR_BACKGROUND);
    display.setTextSize(2);
    setCursor_textGrid(1, 1);
    display.print(F("CLEAR CHAINS?"));
    setCursor_textGrid(1, 2);
    display.setTextColor(RED, COLOR_BACKGROUND);
    display.print(F("PUSH "));
    display.setTextColor(COLOR_SYSTEXT, DX_DARKCYAN);
    display.print(F("[ENC R]"));
    display.setTextColor(RED, COLOR_BACKGROUND);
    display.print(F(" TO CONFIRM"));
    display.setTextSize(1);
    setCursor_textGrid(1, 6);
    display.print(F("ALL CHAINS + CHAIN TRANSPOSES"));
    setCursor_textGrid(1, 7);
    display.print(F("WILL BE DELETED !"));
    display.setTextColor(COLOR_SYSTEXT, COLOR_BACKGROUND);
    setCursor_textGrid(1, 8);
    display.print(F("SONG DATA AND PATTERNS WILL NOT BE TOUCHED."));

    display.setTextSize(2);
  }
  if (LCDML.FUNC_loop())          // ****** LOOP *********
  {
    if (LCDML.BT_checkEnter())
    {
      for (uint8_t i = 0; i < NUM_CHAINS ; i++)
      {
        memset(seq.chain[i], 99, sizeof(seq.chain[i]));
        memset(seq.chain_transpose[i], 99, sizeof(seq.chain_transpose[i]));
      }
      setCursor_textGrid(1, 1);
      display.print(F("Done."));
      print_empty_spaces(10);
      setCursor_textGrid(1, 2);
      print_empty_spaces(23);
      helptext_l ("");
      border3_large_clear();
      delay(MESSAGE_WAIT_TIME);
      LCDML.FUNC_goBackToMenu();
    }
  }
  if (LCDML.FUNC_close())     // ****** STABLE END *********
  {
    encoderDir[ENC_R].reset();
    display.setTextColor(COLOR_SYSTEXT, COLOR_BACKGROUND);
    display.fillScreen(COLOR_BACKGROUND);
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
      //fav symbol
      display.fillRect(13 * CHAR_width + 11, 5, 9, 9, GREEN);
      display.setCursor(13 * CHAR_width + 13, 6);
      display.setTextColor(COLOR_BACKGROUND);
      display.print(F("F"));

    }
    else
    { // it is not a favorite
      //fav symbol
      display.fillRect(13 * CHAR_width + 11, 5, 9, 9, GREY2);
      display.setCursor(13 * CHAR_width + 13, 6);
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
      //fav symbol
      display.setTextSize(1);
      display.fillRect(13 * CHAR_width + 11, 5, 9, 9, GREEN);
      display.setCursor(13 * CHAR_width + 13, 6);
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

      ////remove fav symbol
      display.setTextSize(1);
      display.fillRect(13 * CHAR_width + 11, 5, 9, 9, GREY2);
      display.setCursor(13 * CHAR_width + 13, 6);
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

void fill_msz_from_flash_filename(const uint16_t entry_number, const uint8_t preset_number, const uint8_t zone_number)
{
  char filename[MAX_FLASH_FILENAME_LEN];
  uint32_t filesize;
  uint16_t f;

  SerialFlash.opendir();

  // search for the right file from its entry_number in the directory
  for (f = 0; f <= entry_number; f++) {
    if (SerialFlash.readdir(filename, sizeof(filename), filesize)) {
#ifdef DEBUG
      Serial.print(F("entry #"));
      Serial.print(f);
      Serial.print(F(": "));
      Serial.println(filename);
#endif
    }
    else
      break;
  }

  if (f == entry_number + 1) {
#ifdef DEBUG
    Serial.print(F("Flash file found for entry #"));
    Serial.print(entry_number);
    Serial.print(F(": "));
    Serial.println(filename);
#endif

    // Search root note from filename
    char root_note[4];
    memset(root_note, 0, sizeof(root_note));

    MatchState ms;
    ms.Target(filename);

    char result = ms.Match ("[-_ ][A-G]#?[0-9]");
    if (result > 0) {
      memcpy(root_note, filename + ms.MatchStart + 1, ms.MatchLength - 1);
#ifdef DEBUG
      Serial.print("Found match at: ");
      Serial.println(ms.MatchStart + 1);
      Serial.print("Match length: ");
      Serial.println(ms.MatchLength - 1);
      Serial.print("Match root note: ");
      Serial.println(root_note);
#endif

      // get midi note from the root note string
      uint8_t offset = 0;
      switch (root_note[0]) {
        case 'A':
          offset = 9;
          break;
        case 'B':
          offset = 11;
          break;
        case 'C':
          offset = 0;
          break;
        case 'D':
          offset = 2;
          break;
        case 'E':
          offset = 4;
          break;
        case 'F':
          offset = 5;
          break;
        case 'G':
          offset = 7;
          break;
      }

      if (root_note[ms.MatchLength - 2 - 1] == '#') {
        offset++;
      }
      uint8_t midi_root = (root_note[ms.MatchLength - 1 - 1] - '0' + 1) * 12 + offset;
#ifdef DEBUG
      Serial.printf("root note found: %s\n", root_note);
      Serial.printf("midi root note found: %d\n", midi_root);
#endif
      msz[preset_number][zone_number].rootnote = midi_root;

      // recalculate low and high notes for all zones
      calc_low_high(preset_number);
    }
    else {
#ifdef DEBUG
      Serial.println ("No match.");
#endif
    }
  } else {
#ifdef DEBUG
    Serial.print(F("Flash file not found for entry #"));
    Serial.println(entry_number);
#endif
    strcpy(filename, "*ERROR*");
  }

  // fill the multisample zone informations
  strcpy(msz[preset_number][zone_number].name, filename);
#ifdef DEBUG
  Serial.print(F("MSZ preset #"));
  Serial.print(preset_number);
  Serial.print(F(" - zone #"));
  Serial.print(zone_number);
  Serial.print(F(": "));
  Serial.print(msz[preset_number][zone_number].name);
  Serial.print(F(" root: "));
  Serial.println(msz[preset_number][zone_number].rootnote);
#endif
}


#endif
