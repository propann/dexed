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
#include "splash_image.h"
#include "dexed_sd.h"
#include "screensaver.h"

extern qix_s qix;
extern bool touch_ic_found;

#include "synth_mda_epiano.h"
extern AudioSynthEPiano ep;
#include "effect_stereo_panorama.h"

#include <synth_braids.h>
#include "braids.h"
extern AudioSynthBraids* synthBraids[NUM_BRAIDS];

extern bool remote_active;
extern bool back_button_touch_page_check_and_init_done;
extern elapsedMillis sysinfo_millis;
extern uint8_t sysinfo_sound_state;
uint8_t sysinfo_chord_state = 0;
extern uint8_t sysinfo_logo_version;
uint8_t sysinfo_old_pool;
uint8_t sysinfo_old_bank;
uint8_t sysinfo_old_voice;
uint8_t sysinfo_old_transpose;
int sample_editor_scale_end;

// sidechains
extern uint8_t sidechain_a_sample_number;
extern uint8_t sidechain_b_sample_number;

extern uint8_t sidechain_a_speed;
extern uint8_t sidechain_b_speed;

extern bool sidechain_a_active;
extern bool sidechain_b_active;

elapsedMillis gamepad_millis;
int gamepad_accelerate;

uint8_t gamepad_last_dir;

uint8_t GAMEPAD_UP_0 = 127;
uint8_t GAMEPAD_UP_1 = 0;

uint8_t GAMEPAD_DOWN_0 = 127;
uint8_t GAMEPAD_DOWN_1 = 255;

uint8_t GAMEPAD_RIGHT_0 = 255;
uint8_t GAMEPAD_RIGHT_1 = 127;

uint8_t GAMEPAD_LEFT_0 = 0;
uint8_t GAMEPAD_LEFT_1 = 127;

uint32_t GAMEPAD_SELECT = 256;
uint32_t GAMEPAD_START = 512;
uint32_t GAMEPAD_BUTTON_A = 2;
uint32_t GAMEPAD_BUTTON_B = 1;

uint32_t gamepad_buttons_neutral;
int gamepad_0_neutral;
int gamepad_1_neutral;

bool remote_touched;
extern void draw_menu_ui_icons();

extern int (*calc_val[2])(int, int, int, int);
extern int menuhelper_previous_val;
extern bool menuhelper_redraw;
bool check_and_confirm_midi_channels;

extern AudioFilterStateVariable global_delay_filter[NUM_DEXED];
extern AudioMixer<4> global_delay_filter_mixer[NUM_DEXED];

uint16_t mb_cross_freq_low = 140;
uint16_t mb_cross_freq_mid = 2100;
uint16_t mb_cross_freq_upper_mid = 5200;
uint16_t mb_cross_freq_high = 8600;
float mb_global_gain = 2.0;
float mb_gain_low = 0.10;
float mb_gain_mid = 0.10;
float mb_gain_upper_mid = 0.10;
float mb_gain_high = 0.80;
bool multiband_active = false;
uint8_t mb_threshold_low = 5;
uint8_t mb_threshold_mid = 15;
uint8_t mb_threshold_upper_mid = 12;
uint8_t mb_threshold_high = 2;
float mb_q_low = 0.30;
float mb_q_mid = 0.10;
float mb_q_upper_mid = 0.10;
float mb_q_high = 0.50;
bool mb_solo_low;
bool mb_solo_mid;
bool mb_solo_upper_mid;
bool mb_solo_high;
uint8_t mb_global_ratio = 32;
extern AudioFilterBiquad mb_filter_l_0;
extern AudioFilterBiquad mb_filter_l_1;
extern AudioFilterBiquad mb_filter_l_2;
extern AudioFilterBiquad mb_filter_l_3;
extern AudioFilterBiquad mb_filter_r_0;
extern AudioFilterBiquad mb_filter_r_1;
extern AudioFilterBiquad mb_filter_r_2;
extern AudioFilterBiquad mb_filter_r_3;
extern AudioEffectDynamics mb_compressor_l_0;
extern AudioEffectDynamics mb_compressor_l_1;
extern AudioEffectDynamics mb_compressor_l_2;
extern AudioEffectDynamics mb_compressor_l_3;
extern AudioEffectDynamics mb_compressor_r_0;
extern AudioEffectDynamics mb_compressor_r_1;
extern AudioEffectDynamics mb_compressor_r_2;
extern AudioEffectDynamics mb_compressor_r_3;
extern AudioMixer<4> mb_mixer_l;
extern AudioMixer<4> mb_mixer_r;
extern AudioMixer<2> finalized_mixer_r;
extern AudioMixer<2> finalized_mixer_l;
extern AudioConnection patchCordUsbL;
extern AudioConnection patchCordUsbR;
extern AudioAnalyzePeak mb_before_l;
extern AudioAnalyzePeak mb_before_r;
extern AudioAnalyzePeak mb_after_l;
extern AudioAnalyzePeak mb_after_r;

extern void InitializeCube();
extern void cube_screensaver();
extern void qix_screensaver();
extern void terrain_screensaver();

#define _LCDML_DISP_cols display_cols
#define _LCDML_DISP_rows display_rows

#define _LCDML_DISP_cfg_cursor 0xda // cursor Symbol
#define _LCDML_DISP_cfg_scrollbar 1 // enable a scrollbar

extern PeriodicTimer sequencer_timer;
extern ILI9341_t3n display;

#if defined GENERIC_DISPLAY 
extern XPT2046_Touchscreen touch;
#endif

#ifdef CAPACITIVE_TOUCH_DISPLAY
extern Adafruit_FT6206 touch;
#endif

extern float sc_dexed1_current;
extern float sc_dexed2_current;

extern float sc_braids_current;

extern uint8_t sc_dexed1_target_a;
extern uint8_t sc_dexed2_target_a;

extern uint8_t sc_dexed1_target_b;
extern uint8_t sc_dexed2_target_b;

extern float sc_reverb_current;
extern uint8_t sc_reverb_target_a;
extern uint8_t sc_reverb_target_b;

extern uint8_t sc_braids_target_a;
extern uint8_t sc_braids_target_b;

extern float sc_delay_a_current;
extern float sc_delay_b_current;

extern uint8_t sc_delay_a_target_a;
extern uint8_t sc_delay_a_target_b;
extern uint8_t sc_delay_b_target_a;
extern uint8_t sc_delay_b_target_b;

extern void sequencer(void);
extern void preview_sample(void);
extern bool check_sd_performance_exists(uint8_t number);
extern SdVolume volume;
extern elapsedMillis record_timer;
uint16_t record_x_pos;

extern config_t configuration;
extern void set_volume(uint8_t v, uint8_t m);
extern void generate_version_string(char* buffer, uint8_t len);
extern float midi_volume_transform(uint8_t midi_amp);
extern float volume_transform(float amp);
extern uint8_t selected_instance_id;
extern char receive_bank_filename[FILENAME_LEN];
extern void print_current_chord(void);
extern void tracker_print_pattern(int xpos, int ypos, uint8_t track_number);
//extern void print_merged_pattern_pianoroll(int xpos, int ypos, uint8_t track_number);
extern void update_pianoroll();
extern void set_pattern_content_type_color(uint8_t pattern);
extern void print_formatted_number(uint16_t v, uint8_t l);
extern void print_formatted_number_trailing_space(uint16_t v, uint8_t l);
extern void print_formatted_number_signed(int v, uint8_t l);
extern void seq_print_current_note_from_step(uint8_t s);
extern void print_single_pattern_pianoroll_in_pattern_editor(int xpos, int ypos, uint8_t pattern, uint8_t actstep, bool fullredraw);
extern void print_chord_name(uint8_t currentstep);
extern void print_file_manager_buttons(void);
extern void print_file_manager_active_border(void);
extern uint16_t RGB24toRGB565(uint8_t r, uint8_t g, uint8_t b);
extern uint32_t ColorHSV(uint16_t hue, uint8_t sat, uint8_t val);
extern uint8_t get_chain_length_from_current_track(uint8_t tr);
extern uint8_t get_song_length(void);
extern void helptext_l(const char* str);
extern void helptext_r(const char* str);
extern void back_touchbutton();
extern void seq_pattern_editor_update_dynamic_elements();
extern uint8_t microsynth_selected_instance;
extern AudioMixer<2> microsynth_mixer_reverb;
extern void virtual_keyboard_print_current_instrument();
extern uint8_t find_longest_chain();
extern bool get_bank_name(uint8_t p, uint8_t b, char* bank_name);
extern bool get_voice_name(uint8_t p, uint8_t b, uint8_t v, char* voice_name);
extern void stop_all_drum_slots();
extern char noteNames[12][3];
extern void dac_mute(void);
extern void dac_unmute(void);
extern ts_t ts;
extern fm_t fm;
extern custom_midi_map_t custom_midi_map[NUM_CUSTOM_MIDI_MAPPINGS];
extern void virtual_keyboard();
extern void get_sd_performance_name_json(uint8_t number);
extern bool save_sd_performance_json(uint8_t p);
extern dexed_live_mod_t dexed_live_mod;
uint8_t activesample;
extern File frec;
extern AudioRecordQueue record_queue_l;
extern AudioRecordQueue record_queue_r;
extern char filename[CONFIG_FILENAME_LEN];
extern void psram_test();
extern void handle_touchscreen_settings_button_test();
extern uint8_t remote_MIDI_CC;
extern uint8_t remote_MIDI_CC_value;
void draw_euclidean_circle();
extern JoystickController joysticks[];
extern void microsynth_update_single_setting(uint8_t microsynth_selected_instance);
extern void update_step_rec_buttons();
extern void virtual_keyboard_print_current_instrument();
extern void virtual_keyboard_print_velocity_bar();
extern void update_latch_button();

#if defined(COMPILE_FOR_FLASH)
void UI_draw_waveform_preview_sample(int samplenumber);
#endif

#if NUM_DRUMS > 0
#include "drums.h"
extern uint8_t drum_midi_channel;
extern drum_config_t drum_config[NUM_DRUMSET_CONFIG];
#endif

extern sequencer_t seq;

extern multisample_t msp[NUM_MULTISAMPLES];
extern multisample_zone_t msz[NUM_MULTISAMPLES][NUM_MULTISAMPLE_ZONES];

uint8_t generic_active_function = 99;
uint8_t generic_temp_select_menu;
uint8_t generic_menu;
bool generic_full_draw_required = false;

// FX
extern AudioSynthWaveform* chorus_modulator[NUM_DEXED];
extern AudioMixer<8>* global_delay_in_mixer[NUM_DEXED];
extern AudioMixer<2>* delay_fb_mixer[NUM_DEXED];

#ifdef PSRAM
extern AudioEffectDelayExternal8* delay_fx[NUM_DEXED];
#else
extern AudioEffectDelay* delay_fx[NUM_DEXED];
#endif

extern AudioMixer<2>* delay_mixer[NUM_DEXED];
extern AudioEffectMonoStereo* dexed_mono2stereo[NUM_DEXED];
extern AudioEffectMonoStereo* dexed_dry_mono2stereo[NUM_DEXED];
extern AudioEffectMonoStereo* delay_mono2stereo[NUM_DEXED];

extern AudioAnalyzePeak microdexed_peak_0;
extern AudioAnalyzePeak microdexed_peak_1;
extern AudioAnalyzePeak ep_peak_l;
extern AudioAnalyzePeak ep_peak_r;
extern AudioAnalyzePeak microsynth_peak_osc_0;
extern AudioAnalyzePeak microsynth_peak_osc_1;
extern AudioAnalyzePeak braids_peak_l;
extern AudioAnalyzePeak braids_peak_r;
extern AudioAnalyzePeak drum_mixer_peak_l;
extern AudioAnalyzePeak drum_mixer_peak_r;
extern AudioAnalyzePeak reverb_return_peak_l;
extern AudioAnalyzePeak reverb_return_peak_r;

extern AudioMixer<8> reverb_mixer_r;
extern AudioMixer<8> reverb_mixer_l;

extern AudioMixer<4> microsynth_mixer_filter_osc[NUM_MICROSYNTH];
extern AudioMixer<4> microsynth_mixer_filter_noise[NUM_MICROSYNTH];
extern microsynth_t microsynth[NUM_MICROSYNTH];
extern AudioSynthWaveform microsynth_waveform[NUM_MICROSYNTH];
extern AudioEffectEnvelope microsynth_envelope_osc[NUM_MICROSYNTH];
extern AudioFilterStateVariable microsynth_filter_osc[NUM_MICROSYNTH];
extern AudioFilterStateVariable microsynth_filter_noise[NUM_MICROSYNTH];
extern AudioSynthNoisePink microsynth_noise[NUM_MICROSYNTH];
extern AudioEffectEnvelope microsynth_envelope_noise[NUM_MICROSYNTH];

extern braids_t braids_osc;
extern AudioSynthBraids* synthBraids[NUM_BRAIDS];
extern AudioMixer<NUM_BRAIDS> braids_mixer;
extern AudioMixer<4>* braids_mixer_filter[NUM_BRAIDS];
extern AudioMixer<2> braids_mixer_reverb;
extern AudioEffectEnvelope* braids_envelope[NUM_BRAIDS];
extern AudioFilterBiquad* braids_filter[NUM_BRAIDS];

extern AudioEffectStereoPanorama braids_stereo_panorama;

extern AudioEffectFlange braids_flanger_r;
extern AudioEffectFlange braids_flanger_l;

extern int braids_flanger_idx;
extern int braids_flanger_depth;
extern double braids_flanger_freq;

extern AudioEffectPlateReverb reverb;

extern AudioEffectStereoPanorama ep_stereo_panorama;
extern AudioSynthWaveform ep_chorus_modulator;
extern AudioFilterBiquad ep_modchorus_filter;
extern AudioEffectModulatedDelayStereo ep_modchorus;
extern AudioMixer<2> ep_chorus_mixer_r;
extern AudioMixer<2> ep_chorus_mixer_l;
extern AudioMixer<2> ep_delay_fb_mixer_r;
extern AudioMixer<2> ep_delay_fb_mixer_l;
extern AudioEffectDelay ep_delay_fx_r;
extern AudioEffectDelay ep_delay_fx_l;
extern AudioMixer<2> ep_delay_mixer_r;
extern AudioMixer<2> ep_delay_mixer_l;

extern AudioMixer<11> master_mixer_r;
extern AudioMixer<11> master_mixer_l;

extern AudioEffectStereoMono stereo2mono;
extern AudioAnalyzePeak master_peak_r;
extern AudioAnalyzePeak master_peak_l;

extern char sd_string[display_cols + 1];
extern char g_voice_name[NUM_DEXED][VOICE_NAME_LEN];
extern char g_bank_name[NUM_DEXED][BANK_NAME_LEN];
extern char tmp_voice_name[VOICE_NAME_LEN];
extern char tmp_bank_name[BANK_NAME_LEN];
extern const float midi_ticks_factor[10];
extern uint8_t midi_bpm;
extern elapsedMillis save_sys;
extern bool save_sys_flag;
extern void virtual_keyboard_smart_preselect_mode();

bool remote_console_keystate_select;
bool remote_console_keystate_a;
bool remote_console_keystate_b;

extern sdcard_t sdcard_infos;

#ifdef COMPILE_FOR_FLASH
extern flash_t flash_infos;
#endif

/***********************************************************************
   GLOBAL
************************************************************************/
elapsedMillis back_from_volume;
uint8_t instance_num[8][8];
PROGMEM const char accepted_chars[] = " _ABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789-abcdefghijklmnopqrstuvwxyz";

int temp_int;
bool menu_select_toggle;
float temp_float;

#define MAX_CC_DEST 15

PROGMEM const char cc_names[MAX_CC_DEST][13] = {
    "Volume      ",
    "Panorama    ",
    "Cursor RIGHT",
    "Cursor LEFT ",
    "Cursor UP   ",
    "Cursor DOWN ",
    "SELECT      ",
    //  "START",
    "BUTTON B    ",
    "BUTTON A    ",
    "Bank Select ",
    "Reverb Send ",
    "Seq. Start  ",
    "Seq. Stop   ",
    "Seq. RECORD ",
    "Panic Dexed ",
};

PROGMEM const uint8_t cc_dest_values[MAX_CC_DEST] = { 7, 10, 20, 21, 22, 23, 24,
// 25,
26, 27, 32, 91, 200, 201, 202, 203 };

PROGMEM const char cc_names_UI_mapping[8][13] = {

    "Cursor RIGHT",
    "Cursor LEFT ",
    "Cursor UP   ",
    "Cursor DOWN ",
    "SELECT      ",
    "START",
    "BUTTON B    ",
    "BUTTON A    ",
};

PROGMEM const uint8_t cc_dest_values_UI_mapping[8] = { 20, 21, 22, 23, 24, 25, 26, 27 };

PROGMEM const uint8_t special_chars[24][8] = {
    {B11111111, B11110111, B11100111, B11110111, B11110111, B11110111, B11110111, B11111111}, //  [0] 1 small invers
    {B11111111, B11110111, B11101011, B11111011, B11110111, B11101111, B11100011, B11111111}, //  [1] 2 small invers
    {B11111, B11011, B10011, B11011, B11011, B11011, B11011, B11111},                         //  [2] 1 OP invers
    {B11111, B11011, B10101, B11101, B11011, B10111, B10001, B11111},                         //  [3] 2 OP invers
    {B11111, B10001, B11101, B11011, B11101, B10101, B11011, B11111},                         //  [4] 3 OP invers
    {B11111, B10111, B10111, B10101, B10001, B11101, B11101, B11111},                         //  [5] 4 OP invers
    {B11111, B10001, B10111, B10011, B11101, B11101, B10011, B11111},                         //  [6] 5 OP invers
    {B11111, B11001, B10111, B10011, B10101, B10101, B11011, B11111},                         //  [7] 6 OP invers
    {B00000, B00000, B00000, B00000, B00000, B00000, B00000, B11111},                         //  [8] Level 1
    {B00000, B00000, B00000, B00000, B00000, B00000, B11111, B11111},                         //  [9] Level 2
    {B00000, B00000, B00000, B00000, B00000, B11111, B11111, B11111},                         // [10] Level 3
    {B00000, B00000, B00000, B00000, B11111, B11111, B11111, B11111},                         // [11] Level 4
    {B00000, B00000, B00000, B11111, B11111, B11111, B11111, B11111},                         // [12] Level 5
    {B00000, B00000, B11111, B11111, B11111, B11111, B11111, B11111},                         // [13] Level 6
    {B00000, B11111, B11111, B11111, B11111, B11111, B11111, B11111},                         // [14] Level 7
    {B11111, B11111, B11111, B11111, B11111, B11111, B11111, B11111},                         // [15] Level 8
    {B00100, B00110, B00101, B00101, B01101, B11101, B11100, B11000},                         // [16] Note
    {B01110, B10001, B10001, B01110, B00100, B00100, B00110, B00110},                         // [17] Disabled 2nd instance symbol
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
    WAVEFORM_SAMPLE_HOLD };

enum
{
  SCROLLBAR,
  BLOCKBAR,
  METERBAR
};
enum
{
  ENC_R,
  ENC_L
};
enum
{
  MENU_VOICE_POOL,
  MENU_VOICE_BANK,
  MENU_VOICE_SOUND,
  MENU_VOICE_NONE
};

void lcdml_menu_display(void);
void lcdml_menu_clear(void);
void lcdml_menu_control(void);
void mFunc_screensaver(uint8_t param);

void reverb_roomsize();
void reverb_lowpass();
void reverb_lodamp();
void reverb_hidamp();
void reverb_diffusion();
void reverb_level();
void set_delay_sync(uint8_t sync, uint8_t instance);
void UI_func_delay_level_global(uint8_t param);
void master_effects_set_delay_time(uint8_t instance);
void master_effects_set_delay_feedback(uint8_t instance);
void master_effects_set_delay_panorama(uint8_t instance);
void master_effects_set_reverb_send(uint8_t instance);
void UI_func_drum_reverb_send(uint8_t param);
void UI_func_stereo_mono(uint8_t param);
void UI_func_dexed_audio(uint8_t param);
void UI_func_dexed_controllers(uint8_t param);
void UI_func_dexed_setup(uint8_t param);
void UI_handle_OP(uint8_t param);
void UI_func_information(uint8_t param);
void UI_func_liveseq_listeditor(uint8_t param);
//void UI_func_midi_sync(uint8_t param);
void UI_func_master_effects(uint8_t param);
void UI_func_not_available(uint8_t param);
void UI_func_braids(uint8_t param);
void UI_func_multiband_dynamics(uint8_t param);
void UI_func_recorder(uint8_t param);
void UI_func_file_manager(uint8_t param);
void UI_func_custom_mappings(uint8_t param);
void UI_func_microsynth(uint8_t param);
void UI_func_seq_pattern_editor(uint8_t param);
void UI_func_seq_vel_editor(uint8_t param);
void UI_func_clear_song(uint8_t param);
void UI_func_clear_song_chains(uint8_t param);
void UI_func_clear_patterns(uint8_t param);
void UI_func_clear_all(uint8_t param);
void UI_func_seq_settings(uint8_t param);
void UI_func_seq_tracker(uint8_t param);
void UI_func_liveseq_pianoroll(uint8_t param);
void UI_func_arpeggio(uint8_t param);
void UI_func_seq_mute_matrix(uint8_t param);
void UI_func_dexed_assign(uint8_t param);
void UI_func_seq_display_style(uint8_t param);
void UI_func_set_performance_name(uint8_t param);
void UI_func_set_multisample_name(uint8_t param);
void UI_func_volume(uint8_t param);
void UI_func_smart_filter(uint8_t param);
void UI_func_seq_probabilities(uint8_t param);
void UI_func_mixer(uint8_t param);
void UI_func_sidechain(uint8_t param);
void UI_func_song(uint8_t param);
void UI_func_load_performance(uint8_t param);
void UI_func_save_performance(uint8_t param);
void UI_func_save_voice(uint8_t param);
void UI_func_midi_soft_thru(uint8_t param);
void UI_func_midi_channels(uint8_t param);
void UI_func_sd_content_not_found(uint8_t param);
void UI_func_misc_settings(uint8_t param);
void UI_func_velocity_level(uint8_t param);
void UI_func_voice_select(uint8_t param);
void UI_func_voice_editor(uint8_t param);
void UI_func_sysex_send_voice(uint8_t param);
void UI_func_sysex_receive_bank(uint8_t param);
void UI_func_sysex_send_bank(uint8_t param);
void UI_func_startup_performance(uint8_t param);
void UI_func_startup_page(uint8_t param);
void UI_func_map_gamepad(uint8_t param);
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
void string_trim(char* s);
void save_favorite(uint8_t p, uint8_t b, uint8_t v, uint8_t instance_id);
void draw_favorite_icon(uint8_t p, uint8_t b, uint8_t v, uint8_t instance_id);
bool check_favorite(uint8_t p, uint8_t b, uint8_t v, uint8_t instance_id);
bool quick_check_favorites_in_bank(uint8_t p, uint8_t b, uint8_t instance_id);
void locate_previous_non_favorite();
void locate_previous_favorite();
void locate_next_favorite();
void locate_next_non_favorite();
void locate_random_non_favorite();
void UI_func_drums(uint8_t param);
void UI_func_format_flash(uint8_t param);
void UI_func_test_mute(uint8_t param);
void UI_func_test_psram(uint8_t param);
void UI_func_test_touchscreen(uint8_t param);
void UI_func_MultiSamplePlay(uint8_t param);
void UI_func_sample_editor(uint8_t param);
void splash_screen1();
void splash_screen2();
void UI_draw_FM_algorithm(uint8_t algo, uint8_t x, uint8_t y);
void displayOp(char id, int x, int y, char link, char fb);
void fill_msz(char filename[], const uint8_t preset_number, const uint8_t zone_number);
void _setup_rotation_and_encoders(bool init);
void sd_go_parent_folder();
void sd_update_display();
void sd_card_count_files_from_directory(const char* path);
void print_voice_select_default_help();
char* basename(const char* filename);
bool load_performance_and_check_midi(uint8_t perf);
void clear_volmeter(int x, int y);
void draw_volmeters_mixer();
void draw_volmeters_multiband_compressor();

uint8_t x_pos_menu_header_layer[8];
uint8_t x_pos_previous_menu_header;
uint8_t last_menu_depth = 99;
uint8_t prev_menu_item = 0;    // avoid screen flicker at start / end of menu items

// normal menu
LCDMenuLib2_menu LCDML_0(255, 0, 0, NULL, NULL); // normal root menu element (do not change)
LCDMenuLib2 LCDML(LCDML_0, _LCDML_DISP_rows, _LCDML_DISP_cols, lcdml_menu_display, lcdml_menu_clear, lcdml_menu_control);

#include "UI.h"

uint8_t state_dir;
FLASHMEM void set_state_dir()
{
  if (seq.edit_state == 0)
    menuhelper_previous_val = generic_temp_select_menu;
  if (LCDML.BT_checkDown())
    state_dir = 0;
  else if (LCDML.BT_checkUp())
    state_dir = 1;
};

int favsearcher = 0;

void update_seq_speed() {
  seq.tempo_ms = 60000000 / seq.bpm / 4;

  for (uint8_t i = 0; i < MAX_DEXED; i++)
  {
    if (configuration.fx.delay_sync[i] > 0)
    {
      uint16_t midi_sync_delay_time = uint16_t(60000.0 * midi_ticks_factor[configuration.fx.delay_sync[i]] / seq.bpm);
      delay_fx[i]->delay(0, constrain(midi_sync_delay_time, DELAY_TIME_MIN, DELAY_TIME_MAX * 10));
    }
  }
  if (seq.running)
    sequencer_timer.begin(sequencer, seq.tempo_ms / 8);
  else
    sequencer_timer.begin(sequencer, seq.tempo_ms / 8, false);
}

FLASHMEM bool menu_item_check(uint8_t in)
{
  if (generic_temp_select_menu == in || generic_full_draw_required || (generic_temp_select_menu == in - 1 && seq.edit_state == false) || (generic_temp_select_menu == in + 1 && seq.edit_state == false))
    return true;
  else
    return false;
}

FLASHMEM void print_empty_spaces(uint8_t spaces)
{
  display.setTextColor(COLOR_SYSTEXT, COLOR_BACKGROUND);
  for (uint8_t x = 0; x < spaces; x++)
  {
    display.print(" ");
  }
}


FLASHMEM void print_shortcut_navigator()
{
  if (seq.cycle_touch_element != 1)
  {
    display.setTextSize(1);
    display.setCursor(CHAR_width_small * 36, 28 * (CHAR_height_small));
    if (LCDML.FUNC_getID() == LCDML.OTHER_getIDFromFunction(UI_func_song) && seq.cycle_touch_element < 6)
      display.setTextColor(COLOR_SYSTEXT, COLOR_BACKGROUND);
    else
      display.setTextColor(GREY2, COLOR_BACKGROUND);
    display.print("S");
    display.setCursor(CHAR_width_small * 37, 28 * (CHAR_height_small));
    if (LCDML.FUNC_getID() == LCDML.OTHER_getIDFromFunction(UI_func_song) && seq.cycle_touch_element > 5 && seq.cycle_touch_element < 7)
      display.setTextColor(COLOR_SYSTEXT, COLOR_BACKGROUND);
    else
      display.setTextColor(GREY2, COLOR_BACKGROUND);
    display.print("C");
    display.setCursor(CHAR_width_small * 38, 28 * (CHAR_height_small));
    if (LCDML.FUNC_getID() == LCDML.OTHER_getIDFromFunction(UI_func_song) && seq.cycle_touch_element > 7)
      display.setTextColor(COLOR_SYSTEXT, COLOR_BACKGROUND);
    else
      display.setTextColor(GREY2, COLOR_BACKGROUND);
    display.print("T");

    display.setCursor(CHAR_width_small * 39, 28 * (CHAR_height_small));
    if (LCDML.FUNC_getID() == LCDML.OTHER_getIDFromFunction(UI_func_seq_pattern_editor))
      display.setTextColor(COLOR_SYSTEXT, COLOR_BACKGROUND);
    else
      display.setTextColor(GREY2, COLOR_BACKGROUND);
    display.print("P");
    display.setCursor(CHAR_width_small * 40, 28 * (CHAR_height_small));
    if (LCDML.FUNC_getID() == LCDML.OTHER_getIDFromFunction(UI_func_seq_vel_editor))
      display.setTextColor(COLOR_SYSTEXT, COLOR_BACKGROUND);
    else
      display.setTextColor(GREY2, COLOR_BACKGROUND);
    display.print("V");
    display.setCursor(CHAR_width_small * 41, 28 * (CHAR_height_small));
    if (LCDML.FUNC_getID() == LCDML.OTHER_getIDFromFunction(UI_func_voice_select) || LCDML.FUNC_getID() == LCDML.OTHER_getIDFromFunction(UI_func_epiano) || LCDML.FUNC_getID() == LCDML.OTHER_getIDFromFunction(UI_func_microsynth) || LCDML.FUNC_getID() == LCDML.OTHER_getIDFromFunction(UI_func_braids) || LCDML.FUNC_getID() == LCDML.OTHER_getIDFromFunction(UI_func_MultiSamplePlay))
      display.setTextColor(COLOR_SYSTEXT, COLOR_BACKGROUND);
    else
      display.setTextColor(GREY2, COLOR_BACKGROUND);
    display.print("I");
  }
}

FLASHMEM void print_song_playhead()
{
  if (seq.running)
  {
    display.setTextColor(COLOR_SYSTEXT, COLOR_BACKGROUND); // play indicator song view

    if (CHAR_height_small * 8 + 10 * (seq.current_song_step - 1 - seq.scrollpos) > CHAR_height_small * 6)
    {
      display.setCursor(CHAR_width_small * 5, CHAR_height_small * 8 + 10 * (seq.current_song_step - 1 - seq.scrollpos));
      display.print(" ");
    }
    if (CHAR_height_small * 8 + 10 * (seq.current_song_step - seq.scrollpos) > CHAR_height_small * 7)
    {
      display.setCursor(CHAR_width_small * 5, CHAR_height_small * 8 + 10 * (seq.current_song_step - seq.scrollpos));
      display.print(">");
    }
    if (CHAR_height_small * 8 + 10 * (seq.current_song_step + 1 - seq.scrollpos) > CHAR_height_small * 6)
    {
      display.setCursor(CHAR_width_small * 5, CHAR_height_small * 8 + 10 * (seq.current_song_step + 1 - seq.scrollpos));
      display.print(" ");
    }

    if (seq.loop_start == 99) // no loop start set, start at step 0
    {
      if (seq.current_song_step == 0 && get_song_length() > 1) // clear last cursor step after first cycle when song longer than 1 Chainstep
      {
        display.setCursor(CHAR_width_small * 5, CHAR_height_small * 8 + 10 * (get_song_length() - seq.scrollpos - 1));
        display.print(" ");
      }
    }
    else
    {
      if (seq.current_song_step == seq.loop_start && seq.loop_start != seq.loop_end) // clear last cursor step after first cycle when song is in loop mode
      {
        display.setCursor(CHAR_width_small * 5, CHAR_height_small * 8 + 10 * (seq.loop_end - seq.scrollpos));
        display.print(" ");
      }
      // special case, loop start is higher than loop end
      if (seq.loop_start > seq.loop_end && seq.current_song_step == 0) // clear cursor on loop start since we are cycling to step 0
      {
        display.setCursor(CHAR_width_small * 5, CHAR_height_small * 8 + 10 * (seq.loop_start - seq.scrollpos));
        display.print(" ");
      }
    }
  }
}

FLASHMEM void clear_song_playhead()
{
  display.console = true;
  display.fillRect(CHAR_width_small * 5, CHAR_height_small * 8 - 2, 5, 10 * 16, COLOR_BACKGROUND);
}

FLASHMEM void print_song_mode_help()
{
  print_shortcut_navigator();

  if (seq.tracktype_or_instrument_assign == 8) // clear loop
  {
    helptext_l(back_text);
    helptext_r("CLEAR LOOP POINTS (NOT DATA) ?");
  }
  else if (seq.tracktype_or_instrument_assign == 10) // copy loop
  {
    helptext_l(back_text);
    helptext_r("COPY LOOP CONTENT TO NEW LOCATION ?");
  }
  else if (seq.tracktype_or_instrument_assign == 11) // copy loop
  {
    helptext_l(back_text);
    helptext_r("SELECT DESTINATION < >");
  }

  else if (seq.tracktype_or_instrument_assign == 6) // is in tracktype select mode
  {
    helptext_l(back_text);
    helptext_r("SELECT WITH < > THEN PUSH TO CONFIRM TRACKTYPE");
  }
  else if (seq.tracktype_or_instrument_assign == 5) // is in tracktype select mode
  {
    helptext_l("TRK TYPE CHANGE");
    helptext_r("CONFIRM TRK");
  }
  else if (seq.tracktype_or_instrument_assign == 2) // is in inst. select mode
  {
    helptext_l(back_text);
    helptext_r("SEL. INSTR. < > PUSH=CONFIRM");
  }
  else if (seq.tracktype_or_instrument_assign == 1)
  {
    display.fillRect(CHAR_width_small * 26, DISPLAY_HEIGHT - CHAR_height_small, 6 * CHAR_width_small + 2, CHAR_height_small, COLOR_BACKGROUND);
    helptext_l("SELECT TRACK< >FOR INSTR");
    helptext_r("CONFIRM TRK");
  }
  else if (seq.loop_edit_step == 1)
  {
    helptext_r("SEL LOOP START");
    //display.fillRect(CHAR_width_small * 7, DISPLAY_HEIGHT - CHAR_height_small, 30 * CHAR_width_small, CHAR_height_small, COLOR_BACKGROUND);
  }
  else if (seq.tracktype_or_instrument_assign == 0)
  { // all messages below in standard song mode
    if (seq.loop_edit_step == 2)
      helptext_r("SET LOOP END");
    else if (seq.loop_edit_step == 0 && seq.edit_state == false && seq.cycle_touch_element != 0)
      helptext_r("MOVE Y");
    else if (seq.edit_state == false && seq.cycle_touch_element == 0)
    {
      helptext_l("MOVE X");
      helptext_r("MOVE Y");
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
      display.setCursor(CHAR_width_small * 18, DISPLAY_HEIGHT - CHAR_height_small);
      print_empty_spaces(10);
      helptext_r("< > SEL. CH. STEP");
    }
    else if (seq.edit_state && seq.cycle_touch_element == 7)
    {
      helptext_r("< > SEL. PATTERN");
    }
    else if (seq.edit_state && seq.cycle_touch_element == 9)
    {
      helptext_l("");
      helptext_r("< > EDIT STEP");
    }
    else if (seq.edit_state && seq.cycle_touch_element == 8)
    {
      helptext_l("CHAIN < > PATTERN EDITOR");
      helptext_r("< > SEL. STEP");
    }
  }
  seq.help_text_needs_refresh = false;
}

FLASHMEM void print_fav_search_text(bool dir)
{
  display.setCursor(11 * CHAR_width_small, 2 * CHAR_height_small + 3);
  display.setTextSize(2);
  display.setTextColor(COLOR_PITCHSMP, COLOR_BACKGROUND);
  if (dir == LEFT)
    display.print(F("<SEARCHING"));
  else    if (dir == RIGHT)
    display.print(">SEARCHING");

  display.setTextColor(COLOR_SYSTEXT, COLOR_BACKGROUND);

}

FLASHMEM void sub_song_print_instruments(uint16_t front, uint16_t back)
{
  for (uint8_t x = 0; x < NUM_SEQ_TRACKS; x++)
  {
    if (seq.tracktype_or_instrument_assign == 2 && seq.selected_track == x && seq.track_type[x] != 0)
      display.setTextColor(COLOR_SYSTEXT, MIDDLEGREEN);
    else if (seq.tracktype_or_instrument_assign == 1 && seq.selected_track == x && seq.track_type[x] != 0)
      display.setTextColor(COLOR_BACKGROUND, GREEN);
    else if (seq.tracktype_or_instrument_assign == 1 && seq.selected_track != x && seq.track_type[x] != 0)
      display.setTextColor(MIDDLEGREEN, COLOR_BACKGROUND);
    else if (seq.tracktype_or_instrument_assign == 1 && seq.selected_track == x && seq.track_type[x] == 0)
      display.setTextColor(COLOR_BACKGROUND, RED);
    else if (seq.tracktype_or_instrument_assign == 0 || seq.tracktype_or_instrument_assign == 5)
      display.setTextColor(front, back);
    else
      display.setTextColor(0x6000, COLOR_BACKGROUND);
    display.setCursor(6 * CHAR_width_small + (4 * CHAR_width_small) * x, CHAR_height_small * 6);
    if (seq.track_type[x] != 0)
    {
      if (seq.instrument[x] == 0)
        display.print(F("DX1"));
      else if (seq.instrument[x] == 1)
        display.print(F("DX2"));
      else if (seq.instrument[x] == 2)
        display.print(F("EP "));
      else if (seq.instrument[x] == 3)
        display.print(F("MS1"));
      else if (seq.instrument[x] == 4)
        display.print(F("MS2"));
      else if (seq.instrument[x] == 5)
        display.print(F("BRD"));
      else if (seq.instrument[x] > 5 && seq.instrument[x] < 16)
      {
        display.setCursor(6 * CHAR_width_small + (4 * CHAR_width_small) * x, CHAR_height_small * 5);
        display.print(F("SMP"));
        display.setCursor(6 * CHAR_width_small + (4 * CHAR_width_small) * x, CHAR_height_small * 6);
        display.print(F("#"));
        print_formatted_number(seq.instrument[x] - 6, 2);
      }
      else if (seq.instrument[x] > 15 && seq.instrument[x] < 32)
      {
        // if (seq.tracktype_or_instrument_assign == 2) {
        display.setCursor(6 * CHAR_width_small + (4 * CHAR_width_small) * x, CHAR_height_small * 5);
        display.print(F("USB"));
        // }
        display.setCursor(6 * CHAR_width_small + (4 * CHAR_width_small) * x, CHAR_height_small * 6);
        display.print(F("#"));
        print_formatted_number(seq.instrument[x] - 15, 2);
      }
      else if (seq.instrument[x] > 31 && seq.instrument[x] < 48)
      {
        // if (seq.tracktype_or_instrument_assign == 2) {
        display.setCursor(6 * CHAR_width_small + (4 * CHAR_width_small) * x, CHAR_height_small * 5);
        display.print(F("DIN"));
        // }
        display.setCursor(6 * CHAR_width_small + (4 * CHAR_width_small) * x, CHAR_height_small * 6);
        display.print(F("#"));
        print_formatted_number(seq.instrument[x] - 31, 2);
      }
      else if (seq.instrument[x] > 47 && seq.instrument[x] < 64)
      {
        // if (seq.tracktype_or_instrument_assign == 2) {
        display.setCursor(6 * CHAR_width_small + (4 * CHAR_width_small) * x, CHAR_height_small * 5);
        display.print(F("INT"));
        // }
        display.setCursor(6 * CHAR_width_small + (4 * CHAR_width_small) * x, CHAR_height_small * 6);
        display.print(F("#"));
        print_formatted_number(seq.instrument[x] - 47, 2);
      }
      else
        display.print(F("???"));
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
FLASHMEM void draw_button_on_grid(uint8_t x, uint8_t y, const char* t1, const char* t2, uint8_t color)
{

  if (remote_active)
    display.console = true;

  if (color == 99) // special case, draw virtual keyboard button (icon)
  {
    display.fillRect(x * CHAR_width_small, y * CHAR_height_small, button_size_x * CHAR_width_small, CHAR_height_small * button_size_y / 2 - 2, GREY2);
    display.fillRect(x * CHAR_width_small, y * CHAR_height_small + CHAR_height_small * button_size_y / 2, button_size_x * CHAR_width_small, CHAR_height_small * button_size_y / 2 - 2, COLOR_BACKGROUND);
    uint8_t offset[5] = { 1, 2, 2, 4, 6 }; //+ is the offset to left
    int offcount = 0;
    display.setTextSize(1);
    display.setTextColor(GREY1, GREY2);
    display.setCursor(x * CHAR_width_small + CHAR_width_small / 2, y * CHAR_height_small + 4);
    display.print(F("V.KEYB"));
    // draw white keys
    for (uint8_t i = 0; i < 7; i++)
    {
      display.console = true;
      display.fillRect(x * CHAR_width_small + 6 * i, y * CHAR_height_small + 16, 5, 15, COLOR_SYSTEXT); // pianoroll white key
    }
    for (uint8_t i = 0; i < 11; i++)
    {
      if (seq.piano[i] == 1)
      {
        display.fillRect(x * CHAR_width_small + 4 * i - offset[offcount], y * CHAR_height_small + 16, 4, 8, COLOR_BACKGROUND); // BLACK key
        offcount++;
        if (offcount == 5)
          offcount = 0;
      }
    }
  }
  else if (color == 98) // special case, clear button to background color
  {
    display.fillRect(x * CHAR_width_small, y * CHAR_height_small, button_size_x * CHAR_width_small, CHAR_height_small * button_size_y, COLOR_BACKGROUND);
  }
  else if (color == 97) // special case, no touch button but text only, aligned / matching the button grid (used in live sequencer)
  {
    display.setTextColor(COLOR_SYSTEXT, GREY3);
    display.setCursor(x * CHAR_width_small + CHAR_width_small / 2, y * CHAR_height_small + 6);
    display.print(t1);
    display.setCursor(x * CHAR_width_small + CHAR_width_small / 2, y * CHAR_height_small + 10 + CHAR_height_small);
    display.print(t2);
    display.setTextSize(1);
  }
  else
  {
    display.setTextSize(1);
    if (color == 0) // standard grey
    {
      display.setTextColor(GREY1, GREY2);
      display.fillRect(x * CHAR_width_small, y * CHAR_height_small, button_size_x * CHAR_width_small, CHAR_height_small * button_size_y, GREY2);
    }
    else if (color == 1) // button has active color
    {
      display.setTextColor(COLOR_SYSTEXT, DX_DARKCYAN);
      display.fillRect(x * CHAR_width_small, y * CHAR_height_small, button_size_x * CHAR_width_small, CHAR_height_small * button_size_y, DX_DARKCYAN);
    }
    else if (color == 2) // button has RED color
    {
      display.setTextColor(COLOR_SYSTEXT, RED);
      display.fillRect(x * CHAR_width_small, y * CHAR_height_small, button_size_x * CHAR_width_small, CHAR_height_small * button_size_y, RED);
    }
    else if (color == 3) // button has highlighted color
    {
      display.setTextColor(COLOR_SYSTEXT, MIDDLEGREEN);
      display.fillRect(x * CHAR_width_small, y * CHAR_height_small, button_size_x * CHAR_width_small, CHAR_height_small * button_size_y, MIDDLEGREEN);
    }
    display.setCursor(x * CHAR_width_small + CHAR_width_small / 2, y * CHAR_height_small + 6);
    display.print(t1);
    if (t2[1] == '\0' && LCDML.FUNC_getID() == LCDML.OTHER_getIDFromFunction(UI_func_seq_mute_matrix))// big numbers for mute matrix
    {
      display.setCursor((x + 2) * CHAR_width_small + CHAR_width_small / 2, y * CHAR_height_small + 6 + CHAR_height_small);
      display.setTextSize(2);
    }
    else if (t2[0] >= '1' && t2[0] < '9' && LCDML.FUNC_getID() == LCDML.OTHER_getIDFromFunction(UI_func_seq_mute_matrix))  // big numbers for mute matrix
    {
      display.setCursor((x + 2) * CHAR_width_small - 4, y * CHAR_height_small + 6 + CHAR_height_small);
      display.setTextSize(2);
    }
    else
    {
      display.setCursor(x * CHAR_width_small + CHAR_width_small / 2, y * CHAR_height_small + 10 + CHAR_height_small);
    }
    display.print(t2);
    display.setTextSize(1);
    // display.setTextColor(COLOR_SYSTEXT,COLOR_BACKGROUND);
  }
}

FLASHMEM void show_smallfont_noGrid(int pos_y, int pos_x, uint8_t field_size, const char* str)
{
  display.setTextSize(1);
  char tmp[STRING_BUFFER_SIZE];
  char* s = tmp;
  uint8_t l = strlen(str);
  memset(tmp, 0, sizeof(tmp));
  memset(tmp, 0x20, field_size); // blank
  if (l > field_size)
    l = field_size;
  strncpy(s, str, l);
  display.setCursor(pos_x, pos_y);
  display.print(tmp);
}

FLASHMEM void setCursor_textGrid(uint8_t pos_x, uint8_t pos_y)
{
  display.setCursor(pos_x * CHAR_width, pos_y * CHAR_height);
}

FLASHMEM void setCursor_textGrid_small(uint8_t pos_x, uint8_t pos_y)
{
  display.setCursor(pos_x * 6, pos_y * 10);
}

FLASHMEM void setCursor_textGrid_large(uint8_t pos_x, uint8_t pos_y)
{
  display.setCursor(pos_x * CHAR_width, pos_y * (CHAR_height + 1));
}

FLASHMEM void show(uint8_t pos_y, uint8_t pos_x, uint8_t field_size, const char* str)
{
  char tmp[STRING_BUFFER_SIZE];
  char* s = tmp;
  uint8_t l = strlen(str);
  memset(tmp, 0, sizeof(tmp));
  memset(tmp, 0x20, field_size); // blank
  if (l > field_size)
    l = field_size;
  strncpy(s, str, l);
  display.setCursor(pos_x * CHAR_width, pos_y * CHAR_height);
  display.print(tmp);
}

FLASHMEM void show_uppercase_no_grid(uint8_t pos_y, uint8_t pos_x, uint8_t field_size, const char* str)
{
  char tmp[STRING_BUFFER_SIZE];
  char* s = tmp;
  uint8_t l = strlen(str);
  memset(tmp, 0, sizeof(tmp));
  memset(tmp, 0x20, field_size); // blank
  if (l > field_size)
    l = field_size;
  strncpy(s, str, l);
  for (uint8_t i = 0; i < l; ++i)
    tmp[i] = toupper(tmp[i]);
  display.setCursor(pos_x, pos_y);
  display.print(tmp);
}

FLASHMEM void show_no_grid(int pos_y, int pos_x, uint8_t field_size, const char* str)
{
  char tmp[STRING_BUFFER_SIZE];
  char* s = tmp;
  uint8_t l = strlen(str);
  memset(tmp, 0, sizeof(tmp));
  memset(tmp, 0x20, field_size); // blank
  if (l > field_size)
    l = field_size;
  strncpy(s, str, l);
  display.setCursor(pos_x, pos_y);
  display.print(tmp);
}

FLASHMEM void show(uint8_t pos_y, uint8_t pos_x, uint8_t field_size, long num)
{
  char _buf10[STRING_BUFFER_SIZE];
  show(pos_y, pos_x, field_size, itoa(num, _buf10, 10));
}
FLASHMEM void show_no_grid(uint8_t pos_y, uint8_t pos_x, uint8_t field_size, long num)
{
  char _buf10[STRING_BUFFER_SIZE];
  show_no_grid(pos_y, pos_x, field_size, itoa(num, _buf10, 10));
}

FLASHMEM void drawBitmap(int16_t x, int16_t y, const uint8_t bitmap[],
  int16_t w, int16_t h, uint16_t color)
{

  int16_t byteWidth = (w + 7) / 8; // Bitmap scanline pad = whole byte
  uint8_t byte = 0;

  display.console = true;
  for (int16_t j = 0; j < h; j++, y++)
  {
    for (int16_t i = 0; i < w; i++)
    {
      if (i & 7)
        byte <<= 1;
      else
        byte = pgm_read_byte(&bitmap[j * byteWidth + i / 8]);
      if (byte & 0x80)
        display.drawPixel(x + i, y, color);
    }
  }
}

FLASHMEM void drawBitmap(int16_t x, int16_t y, const uint8_t bitmap[],
  int16_t w, int16_t h, uint16_t color,
  uint16_t bg)
{
  int16_t byteWidth = (w + 7) / 8; // Bitmap scanline pad = whole byte
  uint8_t byte = 0;

  display.console = true;
  for (int16_t j = 0; j < h; j++, y++)
  {
    for (int16_t i = 0; i < w; i++)
    {
      if (i & 7)
        byte <<= 1;
      else
        byte = pgm_read_byte(&bitmap[j * byteWidth + i / 8]);
      display.drawPixel(x + i, y, (byte & 0x80) ? color : bg);
    }
  }
}

FLASHMEM void drawBitmap(int16_t x, int16_t y, uint8_t* bitmap, int16_t w,
  int16_t h, uint16_t color)
{

  int16_t byteWidth = (w + 7) / 8; // Bitmap scanline pad = whole byte
  uint8_t byte = 0;

  display.console = true;
  for (int16_t j = 0; j < h; j++, y++)
  {
    for (int16_t i = 0; i < w; i++)
    {
      if (i & 7)
        byte <<= 1;
      else
        byte = bitmap[j * byteWidth + i / 8];
      if (byte & 0x80)
        display.drawPixel(x + i, y, color);
    }
  }
}

FLASHMEM void drawBitmap(int16_t x, int16_t y, uint8_t* bitmap, int16_t w,
  int16_t h, uint16_t color, uint16_t bg)
{

  int16_t byteWidth = (w + 7) / 8; // Bitmap scanline pad = whole byte
  uint8_t byte = 0;

  display.console = true;
  for (int16_t j = 0; j < h; j++, y++)
  {
    for (int16_t i = 0; i < w; i++)
    {
      if (i & 7)
        byte <<= 1;
      else
        byte = bitmap[j * byteWidth + i / 8];
      display.drawPixel(x + i, y, (byte & 0x80) ? color : bg);
    }
  }
}

FLASHMEM void drawScrollbar(uint16_t x, uint16_t y, uint8_t sbNbLines, uint8_t nbTotalItems, uint8_t currentItem, uint8_t sbLineHeight)
{
  uint8_t n_max = (nbTotalItems >= sbNbLines) ? sbNbLines : nbTotalItems;
  float sbHeight = sbLineHeight * n_max;
  float sbItemSize = sbHeight / nbTotalItems;
  display.console = true;
  if (sbLineHeight > 10)
  {
    // big font
    display.fillRect(x, y, CHAR_width_small, sbHeight - 1, GREY2);
    display.fillRect(x, y + currentItem * sbItemSize, CHAR_width_small, sbItemSize, COLOR_SYSTEXT);
  }
  else
  {
    display.fillRect(x, y, CHAR_width_small, sbHeight - 2, GREY2);
    int8_t posOffset = 0;
    if (currentItem == nbTotalItems - 1)
      posOffset = -1;
    //    if(currentItem > 0 && currentItem < nbTotalItems -1)
    //      posOffset = -1;
    display.fillRect(x, y + currentItem * sbItemSize + posOffset, CHAR_width_small, sbItemSize, COLOR_SYSTEXT);
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

MD_REncoder ENCODER[NUM_ENCODER] = { MD_REncoder(ENC_R_PIN_B, ENC_R_PIN_A), MD_REncoder(ENC_L_PIN_B, ENC_L_PIN_A) };
EncoderDirection encoderDir[NUM_ENCODER];
bool button[NUM_ENCODER];

long g_LCDML_CONTROL_button_press_time[NUM_ENCODER] = { 0, 0 };
bool g_LCDML_CONTROL_button_prev[NUM_ENCODER] = { HIGH, HIGH };
uint8_t g_LCDML_CONTROL_prev[NUM_ENCODER] = { 0, 0 };
// bool menu_init = true; 

FLASHMEM void fill_up_with_spaces_right_window()
{
  do
  {
    display.print(" ");
  } while (display.getCursorX() < DISPLAY_WIDTH - 8);
}

FLASHMEM void fill_up_with_spaces_left_window()
{
  do
  {
    display.print(" ");
  } while (display.getCursorX() < 36 * CHAR_width_small);
}

FLASHMEM void fill_up_with_spaces_left_window_filemanager()
{
  do
  {
    display.print(" ");
  } while (display.getCursorX() < 27 * CHAR_width_small);
}

// uint8_t found_samples[20];

// FLASHMEM void init_found_samples_array()
// {
//   for (uint8_t i = 0; i < 20; i++)
//     found_samples[i] = 254; //set an empty value so sample #0 is unique and seachable, too.
// }

// FLASHMEM void print_used_samples()
// {

//   uint8_t count = 0;
//   uint8_t found_unique_sample_slot = 0;
//   bool found_previously = false;  //skip duplicates while searching
//   init_found_samples_array();
//   do
//   {
//     count++;
//     for (uint8_t d = 0; d < NUM_SEQ_PATTERN; d++)
//     {
//       if (seq.content_type[d] == 0)
//       {
//         for (uint8_t y = 0; y < 16; y++)
//         {
//           if ((drum_config[count].midinote == seq.note_data[d][y] && seq.vel[d][y] < 210) ||
//             (drum_config[count].midinote == seq.vel[d][y] && seq.vel[d][y] > 209))
//           {
//             found_previously = false;
//             for (uint8_t e = 0; e < 20; e++) {
//               if (found_samples[e] == drum_config[count].midinote)
//               {
//                 found_previously = true;
//               }
//             }
//             if (found_previously == false)
//             {
//               found_samples[found_unique_sample_slot] = drum_config[count].midinote;
//               found_unique_sample_slot++;
//             }

//           }
//         }
//       }
//     }
//   } while (count < NUM_DRUMSET_CONFIG - 2);

// #ifdef DEBUG
//   LOG.println(F("Found samples (MIDI Notes):"));
//   for (uint8_t e = 0; e < 20; e++)
//   {
//     if (found_samples[e] != 254)  //is not empty
//     {
//       LOG.print(found_samples[e]);
//       LOG.print(" ");
//     }
//   }
// #endif
// }

FLASHMEM bool legacy_touch_button_back_page() //legacy, 2 line text pages with touch back button
{
  if (LCDML.FUNC_getID() == LCDML.OTHER_getIDFromFunction(UI_handle_OP) ||
    LCDML.FUNC_getID() == LCDML.OTHER_getIDFromFunction(UI_func_sysex_send_voice) ||
    LCDML.FUNC_getID() == LCDML.OTHER_getIDFromFunction(UI_func_save_voice) ||
    LCDML.FUNC_getID() == LCDML.OTHER_getIDFromFunction(UI_func_smart_filter) ||
    LCDML.FUNC_getID() == LCDML.OTHER_getIDFromFunction(UI_func_load_performance) ||
    LCDML.FUNC_getID() == LCDML.OTHER_getIDFromFunction(UI_func_save_performance) ||
    LCDML.FUNC_getID() == LCDML.OTHER_getIDFromFunction(UI_func_set_performance_name) ||
    LCDML.FUNC_getID() == LCDML.OTHER_getIDFromFunction(UI_func_set_multisample_name) ||
    LCDML.FUNC_getID() == LCDML.OTHER_getIDFromFunction(UI_func_sysex_receive_bank) ||
    LCDML.FUNC_getID() == LCDML.OTHER_getIDFromFunction(UI_func_sysex_send_bank) ||
    LCDML.FUNC_getID() == LCDML.OTHER_getIDFromFunction(UI_func_stereo_mono) ||
    LCDML.FUNC_getID() == LCDML.OTHER_getIDFromFunction(UI_func_midi_soft_thru) ||
    LCDML.FUNC_getID() == LCDML.OTHER_getIDFromFunction(UI_func_favorites) ||
    LCDML.FUNC_getID() == LCDML.OTHER_getIDFromFunction(UI_func_startup_performance) ||
    LCDML.FUNC_getID() == LCDML.OTHER_getIDFromFunction(UI_func_startup_page))
    return true;
  else
    return false;
}

FLASHMEM bool touch_button_back_page() //modern pages with touch back button
{
  if (LCDML.FUNC_getID() == LCDML.OTHER_getIDFromFunction(UI_func_voice_editor) ||
    LCDML.FUNC_getID() == LCDML.OTHER_getIDFromFunction(UI_func_dexed_controllers) ||
    LCDML.FUNC_getID() == LCDML.OTHER_getIDFromFunction(UI_func_drums) ||
    LCDML.FUNC_getID() == LCDML.OTHER_getIDFromFunction(UI_func_misc_settings) ||
    LCDML.FUNC_getID() == LCDML.OTHER_getIDFromFunction(UI_func_microsynth) ||
    LCDML.FUNC_getID() == LCDML.OTHER_getIDFromFunction(UI_func_dexed_setup))

    return true;
  else
    return false;
}

FLASHMEM void smart_filter(uint8_t dir)
{
  bool found = false;
  // search backwards
  if (dir == 0)
  {
    if (seq.smartfilter)
    {
      do
      {
        if (found == false)
          activesample = constrain(activesample - 1, 0, NUM_DRUMSET_CONFIG - 2);
        for (uint8_t d = 0; d < NUM_SEQ_PATTERN; d++)
        {
          if (seq.content_type[d] == 0)
          {
            for (uint8_t y = 0; y < 16; y++)
            {
              if ((drum_config[activesample].midinote == seq.note_data[d][y] && seq.vel[d][y] < 210)
                || (drum_config[activesample].midinote == seq.vel[d][y] && seq.vel[d][y] > 209))
              {
                found = true;
                // #ifdef DEBUG
                //                 LOG.println(F("found used sample in pattern:"));
                //                 LOG.println(d);
                // #endif
                break;
              }
            }
          }
        }
      } while (found == false && activesample > 0);
    }
    else
      activesample = constrain(activesample - 1, 0, NUM_DRUMSET_CONFIG - 2);
  }
  else // search forwards
  {
    if (seq.smartfilter)
    {
      do
      {
        if (found == false)
          activesample = constrain(activesample + 1, 0, NUM_DRUMSET_CONFIG - 2);
        for (uint8_t d = 0; d < NUM_SEQ_PATTERN; d++)
        {
          if (seq.content_type[d] == 0)
          {

            for (uint8_t y = 0; y < 16; y++)
            {
              if ((drum_config[activesample].midinote == seq.note_data[d][y] && seq.vel[d][y] < 210) ||
                (drum_config[activesample].midinote == seq.vel[d][y] && seq.vel[d][y] > 209))
              {
                found = true;
                // #ifdef DEBUG
                //                 LOG.println(F("found used sample in pattern:"));
                //                 LOG.println(d);
                // #endif
                break;
              }
            }
          }
        }
      } while (found == false && activesample < NUM_DRUMSET_CONFIG - 2);
    }
    else
      activesample = constrain(activesample + 1, 0, NUM_DRUMSET_CONFIG - 2);
  }

}

FLASHMEM void border0() // main Border  default Colors = GREY2/4
{
  display.console = true;
  display.drawRect(0, 0, DISPLAY_WIDTH, DISPLAY_HEIGHT, GREY4); // Main Border
}

FLASHMEM void border1_clear() // upper left
{
  display.console = true;
  display.fillRect(1, CHAR_height, CHAR_width * 19 - 1, CHAR_height * display_rows, COLOR_BACKGROUND);
}

FLASHMEM void border3_clear() // lower left
{
  display.console = true;
  display.fillRect(0, CHAR_height * 5 - 4, CHAR_width * 17, DISPLAY_HEIGHT - CHAR_height * 5 + 3, COLOR_BACKGROUND);
}

FLASHMEM void border3_large() // lower left+right as one window
{
  display.console = true;
  display.drawRect(0, CHAR_height * 6 - 4, DISPLAY_WIDTH, DISPLAY_HEIGHT - CHAR_height * 6 + 4, GREY4);
}

FLASHMEM void border3_large_clear() // lower left+right as one window
{
  display.console = true;
  display.fillRect(1, CHAR_height * 5, DISPLAY_WIDTH, DISPLAY_HEIGHT - CHAR_height * 5, COLOR_BACKGROUND);
}

FLASHMEM const char* seq_find_shortname(uint8_t sstep)
{
  const char* shortname;
  bool found = false;
  if (seq.content_type[seq.active_pattern] == 0 && seq.vel[seq.active_pattern][sstep] < 210) // is Drumtrack and not a pitched sample
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
    if (found == false)
      shortname = "-";
  }
  else
  {
    if (seq.note_data[seq.active_pattern][sstep] > 0 && seq.note_data[seq.active_pattern][sstep] != 130)
    {
      shortname = noteNames[seq.note_data[seq.active_pattern][sstep] % 12];
    }
    else if (seq.note_data[seq.active_pattern][sstep] == 130)
      shortname = "~"; // note has tie/latch
    else
      shortname = "-";
  }
  return shortname;
}

FLASHMEM void set_track_type_color(uint8_t track)
{
  if (seq.track_type[track] == 0) // Drums
    display.setTextColor(COLOR_DRUMS, COLOR_BACKGROUND);
  else if (seq.track_type[track] == 1) // Inst
    display.setTextColor(COLOR_INSTR, COLOR_BACKGROUND);
  else if (seq.track_type[track] == 2) // Chord
    display.setTextColor(COLOR_CHORDS, COLOR_BACKGROUND);
  else if (seq.track_type[track] == 3) // Arp
    display.setTextColor(COLOR_ARP, COLOR_BACKGROUND);
}

FLASHMEM void sub_song_print_tracktypes()
{
  for (uint8_t x = 0; x < NUM_SEQ_TRACKS; x++)
  {
    display.setCursor(6 * CHAR_width_small + (4 * CHAR_width_small) * x, CHAR_height_small * 4);
    if (seq.tracktype_or_instrument_assign == 0)
      set_track_type_color(x);
    else if (seq.tracktype_or_instrument_assign == 5 && seq.selected_track == x)
      display.setTextColor(COLOR_BACKGROUND, GREEN);
    else if (seq.tracktype_or_instrument_assign == 5 && seq.selected_track != x)
      display.setTextColor(MIDDLEGREEN, COLOR_BACKGROUND);
    else if (seq.tracktype_or_instrument_assign == 6 && seq.selected_track == x)
      display.setTextColor(COLOR_SYSTEXT, MIDDLEGREEN);
    else
      display.setTextColor(GREY3, COLOR_BACKGROUND);
    display.setCursor(6 * CHAR_width_small + (4 * CHAR_width_small) * x, CHAR_height_small * 5);
    if (seq.track_type[x] == 0)
      display.print(F("DRM"));
    else if (seq.track_type[x] == 1)
      display.print(F("INS"));
    else if (seq.track_type[x] == 2)
      display.print(F("CHD"));
    else if (seq.track_type[x] == 3)
      display.print(F("ARP"));
    // else if (seq.track_type[x] == 1 && seq.instrument[seq.selected_track] < 16) display.print(F("INS"));
    // else if (seq.track_type[x] == 2 && seq.instrument[seq.selected_track] < 16) display.print(F("CHD"));
    // else if (seq.track_type[x] == 3 && seq.instrument[seq.selected_track] < 16) display.print(F("ARP"));
    // else if (seq.instrument[x] > 15 && seq.instrument[x] < 32) display.print("USB");
    // else if (seq.instrument[x] > 31 && seq.instrument[x] < 48) display.print("DIN");
    else
    {
      display.setTextColor(RED, COLOR_BACKGROUND);
      display.print("???");
    }
  }
}

FLASHMEM void set_track_type_color_inverted(uint8_t track)
{
  if (seq.track_type[track] == 0) // Drums
    display.setTextColor(COLOR_BACKGROUND, COLOR_DRUMS);
  else if (seq.track_type[track] == 1) // Inst
    display.setTextColor(COLOR_BACKGROUND, COLOR_INSTR);
  else if (seq.track_type[track] == 2) // Chord
    display.setTextColor(COLOR_BACKGROUND, COLOR_CHORDS);
  else if (seq.track_type[track] == 3) // Arp
    display.setTextColor(COLOR_BACKGROUND, COLOR_ARP);
}

bool song_page_full_draw_pattern_complete = false;
bool song_page_full_draw_chain_complete = false;
bool song_page_full_draw_transpose_complete = false;

FLASHMEM void print_song_loop_arrows()
{
  uint8_t lineheight = 10;

  display.console = true;
  display.fillRect(0, CHAR_height_small * 8 - 2, 5, lineheight * 16, COLOR_BACKGROUND);
  display.fillRect(3 * CHAR_width_small, CHAR_height_small * 8 - 2, 5, lineheight * 16, COLOR_BACKGROUND);
  for (uint8_t y = 0; y < 16; y++) // print loop arrows
  {
    // if (seq.loop_edit_step != 3)
    // {

    if (seq.tracktype_or_instrument_assign == 8 && y + seq.scrollpos >= seq.loop_start && y + seq.scrollpos <= seq.loop_end)//loop clear mode
      display.setTextColor(COLOR_BACKGROUND, GREEN);  //loop clear mode
    else if (seq.tracktype_or_instrument_assign == 10 && y + seq.scrollpos >= seq.loop_start && y + seq.scrollpos <= seq.loop_end)//loop copy mode
      display.setTextColor(COLOR_SYSTEXT, DARKGREEN);  //loop copy mode 
    else
      display.setTextColor(GREEN, COLOR_BACKGROUND);

    display.setCursor(CHAR_width_small, y * lineheight + CHAR_height_small * 8);
    print_formatted_number(y + 1 + seq.scrollpos, 2);
    // }

    display.setCursor(0, y * lineheight + CHAR_height_small * 8);

    if (seq.tracktype_or_instrument_assign == 8)
      display.setTextColor(RED, COLOR_BACKGROUND); //loop clear mode
    else if (seq.tracktype_or_instrument_assign == 10)
      display.setTextColor(COLOR_SYSTEXT, COLOR_BACKGROUND); //loop copy mode
    else
      display.setTextColor(GREEN, COLOR_BACKGROUND);

    if (y + seq.scrollpos == seq.loop_start)
      display.print(">");
    display.setCursor(3 * CHAR_width_small, y * lineheight + CHAR_height_small * 8);

    if (y + seq.scrollpos == seq.loop_end)
      display.print("<");
  }
}

FLASHMEM void edit_song_loop()
{
  uint8_t lineheight = 10;

  for (uint8_t y = 0; y < 16; y++) // print song step number for song loop edit mode
  {
    if ((seq.loop_edit_step == 1 && y == seq.cursor_scroll) || (seq.loop_edit_step == 2 && y == seq.cursor_scroll))
      display.setTextColor(COLOR_BACKGROUND, GREEN); // select loop start/end step
    else
      display.setTextColor(GREEN, COLOR_BACKGROUND); // green step number

    if (seq.cycle_touch_element == 0)
    {
      display.setCursor(CHAR_width_small, y * lineheight + CHAR_height_small * 8);
      print_formatted_number(y + 1 + seq.scrollpos, 2);
    }

    if ((seq.loop_edit_step == 1 && y == seq.cursor_scroll) || (y + seq.scrollpos == seq.loop_start))
    {

      if (y < 15 && y - 1 >= 0 && y + seq.scrollpos - 1 != seq.loop_start && y - 1 == seq.cursor_scroll - 1)
      {
        display.console = true;
        display.fillRect(0, y * lineheight + CHAR_height_small * 8 - lineheight, 5, 7, COLOR_BACKGROUND);
      }
      if (y < 15 && y + seq.scrollpos + 1 != seq.loop_start && y + 1 == seq.cursor_scroll + 1)
      {
        display.console = true;
        display.fillRect(0, (y + 1) * lineheight + CHAR_height_small * 8, 5, 7, COLOR_BACKGROUND);
      }

      display.setCursor(0, y * lineheight + CHAR_height_small * 8);
      if (y + seq.scrollpos == seq.loop_start)
        display.setTextColor(GREY2, COLOR_BACKGROUND);
      if (seq.loop_edit_step == 1 && y == seq.cursor_scroll)
        display.setTextColor(RED, COLOR_BACKGROUND);
      display.print(">");

    }


    if ((seq.loop_edit_step == 2 && y == seq.cursor_scroll) || (y + seq.scrollpos == seq.loop_end))
    {

      if (y < 15 && y - 1 >= 0 && y + seq.scrollpos - 1 != seq.loop_end && y - 1 == seq.cursor_scroll - 1)
      {
        display.console = true;
        display.fillRect(3 * CHAR_width_small, y * lineheight + CHAR_height_small * 8 - lineheight, 5, 7, COLOR_BACKGROUND);
      }
      if (y < 15 && y + seq.scrollpos + 1 != seq.loop_end && y + 1 == seq.cursor_scroll + 1)
      {
        display.console = true;
        display.fillRect(3 * CHAR_width_small, (y + 1) * lineheight + CHAR_height_small * 8, 5, 7, COLOR_BACKGROUND);
      }

      display.setCursor(3 * CHAR_width_small, y * lineheight + CHAR_height_small * 8);
      if (y + seq.scrollpos == seq.loop_end)
        display.setTextColor(GREY2, COLOR_BACKGROUND);
      if (seq.loop_edit_step == 2 && y == seq.cursor_scroll)
        display.setTextColor(RED, COLOR_BACKGROUND);
      display.print("<");

    }


  }
}

FLASHMEM void print_chain_matrix_in_song_page()
{


  if (seq.tracktype_or_instrument_assign > 0 && seq.tracktype_or_instrument_assign < 3) // select track for instrument assignment
  {
    sub_song_print_instruments(DARKGREEN, COLOR_BACKGROUND);
  }

  else if (seq.tracktype_or_instrument_assign > 4 && seq.tracktype_or_instrument_assign < 7) // select track for tracktype assignment
  {
    sub_song_print_tracktypes();
  }

  if (seq.tracktype_or_instrument_assign < 2 || seq.tracktype_or_instrument_assign == 6 || seq.tracktype_or_instrument_assign == 10 ||
    seq.tracktype_or_instrument_assign == 11 || seq.tracktype_or_instrument_assign == 8) // normal mode: show song grid, chain..
  {
    bool drawtrack[NUM_SEQ_TRACKS] = { false, false, false, false, false, false, false, false };
    if (seq.cursor_scroll == 0 || seq.cursor_scroll == 15 || seq.tracktype_or_instrument_assign == 6 ||
      seq.tracktype_or_instrument_assign == 11)
      song_page_full_draw_pattern_complete = false;

    if (seq.cursor_scroll < 16 && song_page_full_draw_pattern_complete && seq.tracktype_or_instrument_assign != 1)

    {

      if (seq.selected_track == 0)
      {
        drawtrack[7] = true;
        drawtrack[0] = true;
        drawtrack[1] = true;
      }
      else if (seq.selected_track == 7)
      {
        drawtrack[6] = true;
        drawtrack[7] = true;
        drawtrack[0] = true;
      }
      else
      {
        drawtrack[seq.selected_track] = true;
        drawtrack[seq.selected_track + 1] = true;
        drawtrack[seq.selected_track - 1] = true;
      }
    }
    else
    { // draw everything
      drawtrack[0] = true;
      drawtrack[1] = true;
      drawtrack[2] = true;
      drawtrack[3] = true;
      drawtrack[4] = true;
      drawtrack[5] = true;
      drawtrack[6] = true;
      drawtrack[7] = true;

    }

    for (uint8_t x = 0; x < NUM_SEQ_TRACKS; x++)
    {
      for (uint8_t y = 0; y < 16; y++) // visible song steps
      {
        uint8_t lineheight = 10;

        if (seq.cycle_touch_element < 7 || seq.tracktype_or_instrument_assign == 11)
        {
          if (drawtrack[x])
          {
            if ((y == seq.cursor_scroll && x == seq.selected_track) || seq.tracktype_or_instrument_assign == 11 ||
              (y == seq.cursor_scroll && x == 0 && seq.loop_edit_step == 1) || //is in loop edit, change trk0 to default state
              (y == seq.cursor_scroll && x == seq.previous_track) ||
              (y == seq.cursor_scroll_previous && x == seq.selected_track) ||
              seq.cursor_scroll == 15 || // not optimized when scrolling out of initial visible range (first 16 steps)
              (seq.cursor_scroll == 0 && song_page_full_draw_pattern_complete == false) //not optimized when scrolling screen back
              )
            {
              display.setCursor(6 * CHAR_width_small + (2 * CHAR_width) * x, y * lineheight + CHAR_height_small * 8);

              if (seq.tracktype_or_instrument_assign == 11 && y >= seq.cursor_scroll && y <= seq.cursor_scroll + (seq.loop_end - seq.loop_start))
                display.setTextColor(COLOR_SYSTEXT, RED); //copy mode
              else if ((seq.tracktype_or_instrument_assign == 10 && y + seq.scrollpos >= seq.loop_start && y + seq.scrollpos <= seq.loop_end) ||
                (seq.tracktype_or_instrument_assign == 11 && y + seq.scrollpos >= seq.loop_start && y + seq.scrollpos <= seq.loop_end))

                display.setTextColor(COLOR_SYSTEXT, DARKGREEN); //copy mode

              else if (seq.tracktype_or_instrument_assign == 10 || seq.tracktype_or_instrument_assign == 11) // dim down non copied lines to make more clear what is happening
                display.setTextColor(GREY3, COLOR_BACKGROUND);
              else if (y == seq.cursor_scroll && x == seq.selected_track && seq.edit_state == false && seq.loop_edit_step == 0 && seq.tracktype_or_instrument_assign == 0)
                set_track_type_color_inverted(x);
              else if (y == seq.cursor_scroll && x == seq.selected_track && seq.edit_state && seq.cycle_touch_element == 5 && seq.loop_edit_step == 0)
                display.setTextColor(COLOR_SYSTEXT, RED);
              else
                set_track_type_color(x);

              if (seq.edit_state && seq.cycle_touch_element == 5 && x == seq.selected_track && y == seq.cursor_scroll) // is in song edit mode
              {
                if (temp_int == NUM_CHAINS)
                {
                  temp_int = 99; // Select empty step/chain
                  seq.song[x][y + seq.scrollpos] = temp_int;
                  temp_int = NUM_CHAINS;
                }
                else
                  seq.song[x][y + seq.scrollpos] = temp_int;
              }

              if (seq.song[x][y + seq.scrollpos] < 99)
              {
                print_formatted_number(seq.song[x][y + seq.scrollpos], 2);

              }
              else {
                display.print(F("--"));

              }
            }
          }
        }
      }
    }
  }
  song_page_full_draw_pattern_complete = true;
}

FLASHMEM void pattern_preview_in_song(uint8_t patternno)
{
  display.setTextSize(1);
  seq.active_pattern = patternno;
  display.setCursor(0, DISPLAY_HEIGHT - CHAR_height_small);
  display.setTextColor(COLOR_SYSTEXT, COLOR_CHORDS);
  if (patternno == 99)
  {
    display.print(F("EMPTY "));
  }
  else
  {
    display.print("PAT:");
    print_formatted_number(patternno, 2);
  }
  display.setTextColor(GREY1, GREY4);
  display.print("[");
  for (uint8_t i = 0; i < 16; i++)
  {
    if (seq.vel[seq.active_pattern][i] > 209)
      display.setTextColor(COLOR_PITCHSMP, GREY4);
    else
    {
      if (seq.content_type[patternno] == 0) // Drumpattern
        display.setTextColor(COLOR_DRUMS, GREY4);
      else if (seq.content_type[patternno] == 1) // Instrument Pattern
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

FLASHMEM void print_transpose_in_song_page()
{
  uint8_t y_start;
  uint8_t y_end;

  if (song_page_full_draw_transpose_complete == false)
  {
    y_start = 0;
    y_end = 16;
    song_page_full_draw_transpose_complete = true;
  }
  else if (seq.menu == 0)
  {
    y_start = 0;
    y_end = 2;
  }
  else if (seq.menu == 15)
  {
    y_start = 14;
    y_end = 15;
  }
  else
  {
    y_start = seq.menu - 1;
    y_end = seq.menu + 2;
  }
  display.setTextSize(1);
  if (seq.cycle_touch_element < 10)
  {
    for (uint8_t y = y_start; y < y_end; y++) // transpose chain steps
    {
      display.setCursor(CHAR_width_small * 50, CHAR_height_small * 8 + y * 10); // chain transpose
      if (seq.edit_state && seq.cycle_touch_element == 9 && seq.menu == y)
        display.setTextColor(COLOR_SYSTEXT, RED);
      else if (seq.edit_state && seq.cycle_touch_element == 8 && seq.menu == y)
        display.setTextColor(COLOR_BACKGROUND, COLOR_SYSTEXT);

      else if (seq.cycle_touch_element == 8 && seq.edit_state == false)
        display.setTextColor(COLOR_SYSTEXT, COLOR_BACKGROUND);
      else
        display.setTextColor(GREY1, COLOR_BACKGROUND);
      if (seq.song[seq.selected_track][seq.cursor_scroll + seq.scrollpos] != 99 && seq.chain_transpose[seq.song[seq.selected_track][seq.cursor_scroll + seq.scrollpos]][y] < 99)
      {
        if (seq.chain_transpose[seq.song[seq.selected_track][seq.cursor_scroll + seq.scrollpos]][y] < NUM_CHAINS)
          print_formatted_number(seq.chain_transpose[seq.song[seq.selected_track][seq.cursor_scroll + seq.scrollpos]][y], 2);
        else
          print_formatted_number((seq.chain_transpose[seq.song[seq.selected_track][seq.cursor_scroll + seq.scrollpos]][y]) - NUM_CHAINS, 2);
        display.setCursor(CHAR_width_small * 49, CHAR_height_small * 8 + y * 10);
        if (seq.chain_transpose[seq.song[seq.selected_track][seq.cursor_scroll + seq.scrollpos]][y] > NUM_CHAINS && seq.chain_transpose[seq.song[seq.selected_track][seq.cursor_scroll + seq.scrollpos]][y] < NUM_CHAINS * 2)
          display.print(F("-"));
        else if (seq.chain_transpose[seq.song[seq.selected_track][seq.cursor_scroll + seq.scrollpos]][y] != 0 && seq.chain_transpose[seq.song[seq.selected_track][seq.cursor_scroll + seq.scrollpos]][y] != 99)
          display.print(F("+"));
        else if (seq.chain_transpose[seq.song[seq.selected_track][seq.cursor_scroll + seq.scrollpos]][y] == 0)
          display.print(F(" "));
      }
      else
      {
        display.print(F("--"));
        display.setCursor(CHAR_width_small * 49, CHAR_height_small * 8 + y * 10);
        display.setTextColor(GREY1, COLOR_BACKGROUND);
        display.print(F(" "));
      }
    }
  }
}

uint8_t song_previous_displayed_chain = 99;
uint8_t chain_endline = 99;

void calc_chain_endline()
{
  for (uint8_t y = 0; y < 16; y++) // chain
  {
    if (seq.chain[seq.song[seq.selected_track][seq.cursor_scroll + seq.scrollpos]][y] == 99)
    {
      chain_endline = y;
      break;
    }
  }
}

uint8_t chain_warning_number_buffer;

FLASHMEM void empty_chain_warning_text_in_song_page()
{
  if (chain_warning_number_buffer != seq.song[seq.selected_track][seq.cursor_scroll + seq.scrollpos])
  {
    if (seq.chain[seq.song[seq.selected_track][seq.cursor_scroll + seq.scrollpos]][0] == 99)
    {
      char tmptxt[5];
      display.setCursor(13 * CHAR_width_small, 29 * CHAR_height_small);
      display.setTextSize(1);
      display.setTextColor(RED, COLOR_BACKGROUND);
      display.print(F("CHAIN "));
      snprintf_P(tmptxt, sizeof(tmptxt), PSTR("[%02d]"), seq.song[seq.selected_track][seq.cursor_scroll + seq.scrollpos]);
      display.print(tmptxt);
      display.print(F(" IS EMPTY"));
    }
    else
    {
      display.console = true;
      display.fillRect(13 * CHAR_width_small, 29 * CHAR_height_small, 113, 7, COLOR_BACKGROUND);
    }
    chain_warning_number_buffer = seq.song[seq.selected_track][seq.cursor_scroll + seq.scrollpos];
  }
}

FLASHMEM void print_chain_steps_in_song_page()
{
  uint8_t y_start;
  uint8_t y_end;
  if (song_page_full_draw_chain_complete == false)
  {
    y_start = 0;
    y_end = 16;
  }
  else if (seq.menu == 0)
  {
    y_start = 0;
    y_end = 2;
  }
  else if (seq.menu == 15)
  {
    y_start = 14;
    y_end = 15;
  }
  else
  {
    y_start = seq.menu - 1;
    y_end = seq.menu + 2;
  }
  display.setTextSize(1);
  if (seq.edit_state && seq.cycle_touch_element == 9)
  {
    seq.chain_transpose[seq.song[seq.selected_track][seq.cursor_scroll + seq.scrollpos]][seq.menu] = seq.sub_menu;
  }
  if (seq.cycle_touch_element < 10)
  {
    calc_chain_endline();
    for (uint8_t y = y_start; y < y_end; y++) // chain
    {
      if (seq.cycle_touch_element != 7)
      {
        display.setCursor(CHAR_width_small * 44, CHAR_height_small * 8 + y * 10);
        if (y == chain_endline)
        {
          display.setTextColor(RED, COLOR_BACKGROUND);
          display.print(F("END"));
          display.setTextColor(GREY1, COLOR_BACKGROUND);
        }
        if (seq.edit_state && seq.cycle_touch_element == 6 && seq.menu == y)
          display.setTextColor(COLOR_SYSTEXT, COLOR_BACKGROUND);
        else
          display.setTextColor(GREY1, COLOR_BACKGROUND);
        display.setCursor(CHAR_width_small * 44, CHAR_height_small * 8 + y * 10);

        if (seq.song[seq.selected_track][seq.cursor_scroll + seq.scrollpos] < 99 && seq.chain[seq.song[seq.selected_track][seq.cursor_scroll + seq.scrollpos]][y] < 99)
        {
          print_formatted_number(seq.chain[seq.song[seq.selected_track][seq.cursor_scroll + seq.scrollpos]][y], 2);
          display.print(F(" ")); //clear previous END marking after END value changed
        }
        else if (chain_endline != y)
        {
          display.setTextColor(GREY1, COLOR_BACKGROUND);
          display.print(F("-- "));
        }
      }
    }
    song_page_full_draw_chain_complete = true;
  }
  if (seq.edit_state && seq.cycle_touch_element == 6)
  {
    display.setTextColor(COLOR_BACKGROUND, COLOR_SYSTEXT);

    display.setCursor(CHAR_width_small * 44, CHAR_height_small * 8 + seq.menu * 10);
    if (chain_endline == seq.menu)
      // if (seq.chain[seq.song[seq.selected_track][seq.cursor_scroll + seq.scrollpos]][seq.menu] == 99 && chain_endline == seq.menu)
      display.print(F("END"));
    else if (seq.chain[seq.song[seq.selected_track][seq.cursor_scroll + seq.scrollpos]][seq.menu] == 99)
      display.print(F("--"));
    else
      print_formatted_number(seq.chain[seq.song[seq.selected_track][seq.cursor_scroll + seq.scrollpos]][seq.menu], 2);
  }
  else if (seq.edit_state && seq.cycle_touch_element == 7)
  {
    if (seq.sub_menu == NUM_CHAINS)
    {
      seq.sub_menu = 99; // Select empty step/chain
      seq.chain[seq.song[seq.selected_track][seq.cursor_scroll + seq.scrollpos]][seq.menu] = seq.sub_menu;
      seq.sub_menu = NUM_CHAINS;
    }
    else
      seq.chain[seq.song[seq.selected_track][seq.cursor_scroll + seq.scrollpos]][seq.menu] = seq.sub_menu;
    display.setTextColor(COLOR_SYSTEXT, RED);
    display.setCursor(CHAR_width_small * 44, CHAR_height_small * 8 + seq.menu * 10);
    if (seq.chain[seq.song[seq.selected_track][seq.cursor_scroll + seq.scrollpos]][seq.menu] == 99)
      display.print(F("--"));
    else
      print_formatted_number(seq.chain[seq.song[seq.selected_track][seq.cursor_scroll + seq.scrollpos]][seq.menu], 2);
    display.setTextColor(COLOR_SYSTEXT, COLOR_BACKGROUND);
    display.print(F(" "));
    pattern_preview_in_song(seq.chain[seq.song[seq.selected_track][seq.cursor_scroll + seq.scrollpos]][seq.menu]);
  }
  // print current CHAIN Number
  display.setTextSize(1);
  display.setCursor(47 * CHAR_width_small, CHAR_height_small * 4);
  if (seq.edit_state && seq.cycle_touch_element > 5 && seq.cycle_touch_element < 10)
    display.setTextColor(COLOR_SYSTEXT, COLOR_BACKGROUND);
  else
    display.setTextColor(GREY1, COLOR_BACKGROUND);

  if (seq.song[seq.selected_track][seq.cursor_scroll + seq.scrollpos] < 99)
    print_formatted_number(seq.song[seq.selected_track][seq.cursor_scroll + seq.scrollpos], 2);
  else
    display.print(F("--"));

  display.setCursor(51 * CHAR_width_small, CHAR_height_small * 4); // print chain length of current track step
  print_formatted_number(get_chain_length_from_current_track(seq.selected_track), 2);
}

FLASHMEM uint8_t seq_find_drum_data_from_note(uint8_t note)
{
  bool found = false;
  uint8_t samplenumber;
  for (uint8_t d = 0; d < NUM_DRUMSET_CONFIG - 1; d++)
  {
    if (note == drum_config[d].midinote)
    {
      samplenumber = d;
      found = true;
      break;
    }
  }
  if (found == false)
    samplenumber = 0;

  return samplenumber;
}

FLASHMEM const char* seq_find_drum_name_from_note(uint8_t note)
{
  bool found = false;
  const char* shortname;
  for (uint8_t d = 0; d < NUM_DRUMSET_CONFIG - 1; d++)
  {
    if (note == drum_config[d].midinote)
    {
      shortname = basename(drum_config[d].name);
      found = true;
      break;
    }
  }
  if (found == false)
    shortname = " ";

  return shortname;
}

FLASHMEM const char* find_long_drum_name_from_note(uint8_t note)
{
  bool found = false;
  const char* name;
  for (uint8_t d = 0; d < NUM_DRUMSET_CONFIG - 1; d++)
  {
    if (note == drum_config[d].midinote)
    {
      name = basename(drum_config[d].name);
      found = true;
      break;
    }
  }
  if (found == false)
    name = " ";

  return name;
}

FLASHMEM const char* tracker_find_shortname_from_pattern_step(uint8_t track, uint8_t pattern, uint8_t sstep)
{
  const char* shortname;
  bool found = false;
  if (seq.content_type[seq.current_pattern[track]] == 0 && seq.vel[seq.current_pattern[track]][sstep] < 210) // is Drumtrack and not a pitched sample
  {
    for (uint8_t d = 0; d < NUM_DRUMSET_CONFIG - 1; d++)
    {
      if (seq.note_data[seq.current_pattern[track]][sstep] == drum_config[d].midinote)
      {
        shortname = drum_config[d].shortname;
        found = true;
        break;
      }
    }
    if (found == false)
      shortname = "-";
  }
  else
  {
    if (seq.vel[seq.current_pattern[track]][sstep] > 209)
      shortname = "P"; // pitched sample
    else if (seq.note_data[seq.current_pattern[track]][sstep] > 0 && seq.note_data[seq.current_pattern[track]][sstep] != 130)
      shortname = noteNames[seq.note_data[seq.current_pattern[track]][sstep] % 12];
    else if (seq.note_data[seq.current_pattern[track]][sstep] == 130)
      shortname = "~"; // note is a tie/latch
    else
      shortname = "-";
  }
  return shortname;
}

FLASHMEM const char* seq_find_shortname_in_track(uint8_t sstep, uint8_t track)
{
  const char* shortname;
  bool found = false;
  if (seq.content_type[track] == 0 && seq.vel[track][sstep] < 210) // is Drumtrack and not a pitched sample
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
    if (found == false)
      shortname = "-";
  }
  else
  {
    if (seq.vel[track][sstep] > 209)
      shortname = "P"; // pitched sample
    else if (seq.note_data[track][sstep] > 0 && seq.note_data[track][sstep] != 130)
      shortname = noteNames[seq.note_data[track][sstep] % 12];
    else if (seq.note_data[track][sstep] == 130)
      shortname = "~"; // note is a tie/latch
    else
      shortname = "-";
  }
  return shortname;
}

FLASHMEM void print_voice_settings_in_pattern_editor(int x, int y)
{
  display.setTextSize(1);
  display.setCursor(x, y);
  display.setTextColor(COLOR_SYSTEXT, COLOR_BACKGROUND);
  display.print(F("INST"));
  display.setTextColor(GREEN, COLOR_BACKGROUND);
  display.setCursor(x + 101, y);
  display.print(selected_instance_id + 1);
  display.setTextColor(COLOR_PITCHSMP, COLOR_BACKGROUND);
  display.setCursor(x + 118, y - 1);
  print_formatted_number(configuration.dexed[selected_instance_id].bank, 2);
  display.setCursor(x + 118, y + 7);
  print_formatted_number(configuration.dexed[selected_instance_id].voice + 1, 2);
  display.setTextColor(COLOR_SYSTEXT, COLOR_BACKGROUND);
  display.setCursor(x + 120 + 16, y - 1);
  display.print(g_bank_name[selected_instance_id]);
  display.setCursor(x + 120 + 16, y + 7);
  display.print(g_voice_name[selected_instance_id]);
  display.setTextSize(2);
}

FLASHMEM void update_pattern_number_in_tracker(uint8_t tracknumber)
{
  setCursor_textGrid_small(9 + 6 * tracknumber, 3);

  if (seq.current_pattern[tracknumber] < 99 && seq.current_chain[tracknumber] != 99)
  {
    set_pattern_content_type_color(seq.current_pattern[tracknumber]);
    print_formatted_number(seq.current_pattern[tracknumber], 2);
  }
  else
  {
    display.setTextColor(GREY2, COLOR_BACKGROUND);
    display.print(F("--"));
  }
}




FLASHMEM void print_live_probability_pattern_info()
{
  for (uint8_t d = 0; d < NUM_SEQ_TRACKS; d++) // print track numbers, patterns and currently playing notes/chords/drums
  {
    display.setTextColor(GREY2, COLOR_BACKGROUND);
    setCursor_textGrid_small(22 + (4 * d), 0);
    print_formatted_number(d + 1, 1);
    display.setTextColor(GREY1, COLOR_BACKGROUND);
    if (seq.current_chain[d] != 99)
    {
      setCursor_textGrid_small(22 + (4 * d), 1);
      print_formatted_number(seq.current_pattern[d], 2);
      setCursor_textGrid_small(22 + (4 * d), 2);
      set_pattern_content_type_color(seq.current_pattern[d]);
      if (seq.content_type[seq.current_pattern[d]] > 0) // it is a Inst. pattern
      {
        if (seq.note_data[seq.current_pattern[d]][seq.step] > 12 && seq.note_data[seq.current_pattern[d]][seq.step] != 130 && seq.note_data[seq.current_pattern[d]][seq.step] != 99)
        {
          display.print(noteNames[seq.note_data[seq.current_pattern[d]][seq.step] % 12][0]);
          if (noteNames[seq.note_data[seq.current_pattern[d]][seq.step] % 12][1] != '\0')
          {
            display.print(noteNames[seq.note_data[seq.current_pattern[d]][seq.step] % 12][1]);
          }
          display.print((seq.note_data[seq.current_pattern[d]][seq.step] / 12) - 1);
        }
        else if (seq.note_data[seq.current_pattern[d]][seq.step] == 130) // latch
          display.print(F("LAT"));
        else
          display.print(F("   "));
      }
      else // it is a drum pattern

        if (seq.vel[seq.current_pattern[d]][seq.step] < 210) // is Drumtrack and not a pitched sample
        {
          bool found = false;
          for (uint8_t n = 0; n < NUM_DRUMSET_CONFIG - 1; n++)
          {
            if (seq.note_data[seq.current_pattern[d]][seq.step] == drum_config[n].midinote)
            {
              display.print(drum_config[n].shortname);
              found = true;
              break;
            }
          }
          if (found == false)
            display.print(F("- "));
        }
        else if (seq.vel[seq.current_pattern[d]][seq.step] > 209) // pitched sample
          display.print(F("PS"));
    }
  }
}
FLASHMEM void print_track_steps_detailed_only_current_playing_note(int xpos, int ypos, uint8_t currentstep)
{
  if (seq.cycle_touch_element == 0) // touch keyboard is off
  {
    if (seq.content_type[seq.active_pattern] == 0)
    { // Drum Track
      uint8_t i = 0;
      int y = 0;
      uint8_t z = 0;
      uint8_t array[2] = { currentstep, 99 };
      display.setTextSize(1);
      display.setTextColor(GREY2, COLOR_BACKGROUND);
      display.setCursor(xpos, ypos);
      if (currentstep == 0)
        array[1] = 15 - seq.pattern_len_dec;
      else if (currentstep == 15 - seq.pattern_len_dec)
        array[1] = 14 - seq.pattern_len_dec;
      else
        array[1] = currentstep - 1;
      while (z < 2)
      {
        i = array[z];
        y = ypos + 10 + i * (CHAR_height_small + 2);
        // Short Name
        if ((array[1] == seq.menu - 3 && LCDML.FUNC_getID() == LCDML.OTHER_getIDFromFunction(UI_func_seq_pattern_editor)) || (array[1] == seq.menu - 1 && LCDML.FUNC_getID() == LCDML.OTHER_getIDFromFunction(UI_func_seq_vel_editor)))
          display.setTextColor(COLOR_BACKGROUND, COLOR_SYSTEXT);
        else if (i == currentstep)
          display.setTextColor(COLOR_SYSTEXT, COLOR_PITCHSMP);
        else
          display.setTextColor(GREY2, COLOR_BACKGROUND);
        display.setCursor(CHAR_width_small * 4, y);
        if (seq.vel[seq.active_pattern][i] > 209) // it is a pitched Drum Sample
        {
          seq_print_current_note_from_step(i);
        }
        else
        {
          display.print(seq_find_shortname_in_track(i, seq.active_pattern)[0]);
        }
        // Data values
        if ((array[1] == seq.menu - 3 && LCDML.FUNC_getID() == LCDML.OTHER_getIDFromFunction(UI_func_seq_pattern_editor)) || (array[1] == seq.menu - 1 && LCDML.FUNC_getID() == LCDML.OTHER_getIDFromFunction(UI_func_seq_vel_editor)))
          display.setTextColor(COLOR_BACKGROUND, COLOR_SYSTEXT);
        else if (i == currentstep)
          display.setTextColor(COLOR_SYSTEXT, COLOR_PITCHSMP);
        else
          display.setTextColor(GREY2, COLOR_BACKGROUND);
        display.setCursor(CHAR_width_small * 7, y);
        print_formatted_number(seq.note_data[seq.active_pattern][i], 3);
        // Velocity values
        if ((array[1] == seq.menu - 3 && LCDML.FUNC_getID() == LCDML.OTHER_getIDFromFunction(UI_func_seq_pattern_editor)) || (array[1] == seq.menu - 1 && LCDML.FUNC_getID() == LCDML.OTHER_getIDFromFunction(UI_func_seq_vel_editor)))
          display.setTextColor(COLOR_BACKGROUND, COLOR_SYSTEXT);
        else if (i == currentstep)
          display.setTextColor(COLOR_SYSTEXT, COLOR_PITCHSMP);
        else
          display.setTextColor(GREY1, COLOR_BACKGROUND);
        display.setCursor(CHAR_width_small * 12, y);
        print_formatted_number(seq.vel[seq.active_pattern][i], 3);
        // Long Name / Note

        if ((array[1] == seq.menu - 3 && LCDML.FUNC_getID() == LCDML.OTHER_getIDFromFunction(UI_func_seq_pattern_editor)) || (array[1] == seq.menu - 1 && LCDML.FUNC_getID() == LCDML.OTHER_getIDFromFunction(UI_func_seq_vel_editor)))
          display.setTextColor(COLOR_BACKGROUND, COLOR_SYSTEXT);
        else if (i == currentstep)
          display.setTextColor(COLOR_SYSTEXT, COLOR_PITCHSMP);
        else
          set_pattern_content_type_color(seq.active_pattern);

        if (seq.vel[seq.active_pattern][i] > 209) // it is a pitched Drum Sample
        {
          show_smallfont_noGrid(y, CHAR_width_small * 17, 10, basename(drum_config[seq.vel[seq.active_pattern][i] - 210].name));
        }
        else // else it is a regular Drum Sample
          show_smallfont_noGrid(y, CHAR_width_small * 17, 10, find_long_drum_name_from_note(seq.note_data[seq.active_pattern][i]));

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
    }
    else if (seq.content_type[seq.active_pattern] > 0)
    { // Inst Track or Chord or Arp
      if (seq.cycle_touch_element != 1)
        print_single_pattern_pianoroll_in_pattern_editor(0, DISPLAY_HEIGHT, seq.active_pattern, currentstep, false);
    }
    // {
    //   display.setCursor(x + CHAR_width_small * 17, y);
    //   if (seq.note_data[seq.active_pattern][i] != 0) {
    //     if (seq.note_data[seq.active_pattern][i] == 130)  //it is a latched note
    //     {
    //       if (i == currentstep)
    //         display.setTextColor(COLOR_SYSTEXT, COLOR_PITCHSMP);
    //       else
    //         display.setTextColor(GREEN, COLOR_BACKGROUND);
    //       display.write(0x7E);
    //       display.print(F("LATCH"));  //Tilde Symbol for latched note
    //     } else {
    //       display.print(noteNames[seq.note_data[seq.active_pattern][i] % 12][0]);
    //       if (noteNames[seq.note_data[seq.active_pattern][i] % 12][1] != '\0') {
    //         display.print(noteNames[seq.note_data[seq.active_pattern][i] % 12][1]);
    //       }
    //       if (seq.vel[seq.active_pattern][i] < 200)  //print octave when it is not a chord
    //       {
    //         display.print((seq.note_data[seq.active_pattern][i] / 12) - 1);
    //         display.print(" ");
    //       }
    //       if (seq.vel[seq.active_pattern][i] > 199)  //is a chord
    //       {
    //         display.print(" ");
    //         print_chord_name(i);
    //       }
    //     }
    //   }
  }
  display.setTextColor(COLOR_SYSTEXT, COLOR_BACKGROUND);
}

FLASHMEM void print_playing_chains()
{

  for (uint8_t x = 0; x < NUM_SEQ_TRACKS; x++)
  {
    display.setTextColor(COLOR_SYSTEXT, DX_DARKCYAN);
    display.setCursor(30 * CHAR_width_small + (x * 3 * CHAR_width_small), 1);
    if (seq.current_chain[x] < 99)
      print_formatted_number(seq.current_chain[x], 2);
    else
    {
      display.setTextColor(COLOR_BACKGROUND, DX_DARKCYAN);
      display.print(F("--"));
    }
    display.setTextColor(COLOR_BACKGROUND, DX_DARKCYAN);
    display.setCursor(30 * CHAR_width_small + (x * 3 * CHAR_width_small), 10);
    if (get_chain_length_from_current_track(x) > 0)
      print_formatted_number(get_chain_length_from_current_track(x), 2);
    else
    {
      display.setTextColor(COLOR_BACKGROUND, DX_DARKCYAN);
      display.print(F("--"));
    }
    // debug, show chain counter for all tracks
    display.setTextColor(GREY1, COLOR_BACKGROUND);
    display.setCursor(30 * CHAR_width_small + (x * 3 * CHAR_width_small), 20);
    print_formatted_number(seq.chain_counter[x], 2);
  }
  // show longest current chain
  display.setTextColor(COLOR_SYSTEXT, DX_DARKCYAN);
  display.setCursor(26 * CHAR_width_small, 10);
  print_formatted_number(find_longest_chain(), 2);
}

FLASHMEM void update_display_functions_while_seq_running()
{
  seq.UI_last_seq_step = seq.step;
  // is in UI_func_seq_pattern_editor or is in UI_func_seq_vel_editor
  if (LCDML.FUNC_getID() == LCDML.OTHER_getIDFromFunction(UI_func_seq_pattern_editor) || LCDML.FUNC_getID() == LCDML.OTHER_getIDFromFunction(UI_func_seq_vel_editor))
  {
    if (seq.menu != 33 && seq.menu != 32 && seq.active_function != 95 && seq.active_function != 97)
    {
      display.setTextSize(2);
      if (seq.step == 0)
      {
        set_pattern_content_type_color(seq.active_pattern);
        setCursor_textGrid(14 - seq.pattern_len_dec, 1);
        display.print(seq_find_shortname(14 - seq.pattern_len_dec)[0]);
        display.setTextColor(COLOR_BACKGROUND, COLOR_SYSTEXT);
        setCursor_textGrid(15 - seq.pattern_len_dec, 1);
        display.print(seq_find_shortname(15 - seq.pattern_len_dec)[0]);
        if (seq.note_editor_view == 0 && seq.cycle_touch_element == 0)
          print_track_steps_detailed_only_current_playing_note(0, CHAR_height * 4 + 3, 15 - seq.pattern_len_dec);
      }
      else if (seq.step == 1)
      {
        set_pattern_content_type_color(seq.active_pattern);
        // setCursor_textGrid(15, 1);
        setCursor_textGrid(15 - seq.pattern_len_dec, 1);
        // display.print(seq_find_shortname(15)[0]);
        display.print(seq_find_shortname(15 - seq.pattern_len_dec)[0]);
        display.setTextColor(COLOR_BACKGROUND, COLOR_SYSTEXT);
        setCursor_textGrid(0, 1);
        display.print(seq_find_shortname(0)[0]);
        if (seq.note_editor_view == 0 && seq.cycle_touch_element == 0)
          print_track_steps_detailed_only_current_playing_note(0, CHAR_height * 4 + 3, 0);
      }
      else
      {
        set_pattern_content_type_color(seq.active_pattern);
        setCursor_textGrid(seq.step - 2, 1);
        display.print(seq_find_shortname(seq.step - 2)[0]);
        display.setTextColor(COLOR_BACKGROUND, COLOR_SYSTEXT);
        setCursor_textGrid(seq.step - 1, 1);
        display.print(seq_find_shortname(seq.step - 1)[0]);
        if (seq.note_editor_view == 0 && seq.cycle_touch_element == 0)
          print_track_steps_detailed_only_current_playing_note(0, CHAR_height * 4 + 3, seq.step - 1);
      }
      if (seq.menu > 2 && seq.menu < 19 && LCDML.FUNC_getID() == LCDML.OTHER_getIDFromFunction(UI_func_seq_pattern_editor))
      { // update static cursor while moving cursor running in pattern editor
        setCursor_textGrid(seq.menu - 3, 1);
        display.setTextSize(2);
        display.setTextColor(COLOR_SYSTEXT, RED);
        display.print(seq_find_shortname(seq.menu - 3)[0]);
      }
      else if (seq.menu < 16 && LCDML.FUNC_getID() == LCDML.OTHER_getIDFromFunction(UI_func_seq_vel_editor))
      { // update static cursor while moving cursor running in vel editor
        setCursor_textGrid(seq.menu - 1, 1);
        display.setTextSize(2);
        display.setTextColor(COLOR_SYSTEXT, RED);
        display.print(seq_find_shortname(seq.menu - 1)[0]);
      }

      display.setTextColor(COLOR_SYSTEXT, COLOR_BACKGROUND);
    }
  }
  else if (LCDML.FUNC_getID() == LCDML.OTHER_getIDFromFunction(UI_func_arpeggio)) // is in UI of Arpeggiator
  {
    display.setTextSize(1);
    setCursor_textGrid_small(12, 17);
    display.setTextColor(COLOR_SYSTEXT, COLOR_BACKGROUND);
    print_current_chord();
    draw_euclidean_circle();
  }
  else if (LCDML.FUNC_getID() == LCDML.OTHER_getIDFromFunction(UI_func_seq_tracker)) // is in UI of Tracker
  {
    display.setTextColor(GREEN, COLOR_BACKGROUND);
    display.setCursor(5 * CHAR_width_small, (5 + seq.step) * (CHAR_height_small + 3) - 7);
    display.print(F(">"));
    if (seq.step == 1)
    {
      display.setCursor(CHAR_width_small * 46, 2); // print song step at each 1. pattern step
      display.setTextColor(COLOR_DRUMS, DX_DARKCYAN);
      print_formatted_number(seq.current_song_step, 2);
      display.setCursor(CHAR_width_small * 51, 2);
      print_formatted_number(get_song_length(), 2);

      for (uint8_t d = 0; d < NUM_SEQ_TRACKS; d++)
      {
        display.setTextColor(COLOR_BACKGROUND, DX_DARKCYAN);
        display.setCursor(CHAR_width_small * 16 + (CHAR_width_small * 3) * d, 2);
        print_formatted_number(seq.chain_counter[d], 2);
        tracker_print_pattern((6 + 6 * d) * CHAR_width_small, 48, d);
        update_pattern_number_in_tracker(d);
      }
    }

    if (seq.step > 0)
    {
      display.setCursor(5 * CHAR_width_small, (5 + seq.step - 1) * (CHAR_height_small + 3) - 7);
      display.print(F(" "));
    }
    else if (seq.step == 0)
    {
      display.setCursor(5 * CHAR_width_small, (5 + 15) * (CHAR_height_small + 3) - 7);
      display.print(F(" "));
    }
  }
  else if (LCDML.FUNC_getID() == LCDML.OTHER_getIDFromFunction(UI_func_song))
  {
    print_song_playhead();

    for (uint8_t d = 0; d < NUM_SEQ_TRACKS; d++) // print currently playing notes/chords/drums
    {
      if (seq.current_chain[d] != 99)
      {
        display.setCursor(CHAR_width_small * 6 + (4 * d) * CHAR_width_small, CHAR_height_small * 4);
        set_pattern_content_type_color(seq.current_pattern[d]);
        if (seq.content_type[seq.current_pattern[d]] > 0) // it is a Inst. pattern
        {
          if (seq.note_data[seq.current_pattern[d]][seq.step] > 12 && seq.note_data[seq.current_pattern[d]][seq.step] != 130 && seq.note_data[seq.current_pattern[d]][seq.step] != 99)
          {
            display.print(noteNames[seq.note_data[seq.current_pattern[d]][seq.step] % 12][0]);
            if (noteNames[seq.note_data[seq.current_pattern[d]][seq.step] % 12][1] != '\0')
            {
              display.print(noteNames[seq.note_data[seq.current_pattern[d]][seq.step] % 12][1]);
            }
            display.print((seq.note_data[seq.current_pattern[d]][seq.step] / 12) - 1);
          }
          else if (seq.note_data[seq.current_pattern[d]][seq.step] == 130) // latch
            display.print(F("LAT"));
          else
            display.print(F("   "));
        }
        else // it is a drum pattern

          if (seq.vel[seq.current_pattern[d]][seq.step] < 210) // is Drumtrack and not a pitched sample
          {
            bool found = false;
            for (uint8_t n = 0; n < NUM_DRUMSET_CONFIG - 1; n++)
            {
              if (seq.note_data[seq.current_pattern[d]][seq.step] == drum_config[n].midinote)
              {
                display.print(drum_config[n].shortname);
                found = true;
                break;
              }
            }
            if (found == false)
              display.print(F("- "));
          }
          else if (seq.vel[seq.current_pattern[d]][seq.step] > 209) // pitched sample
            display.print(F("PS"));
      }
    }
    // print currently playing chain steps
    print_playing_chains();
  }
  else if (LCDML.FUNC_getID() == LCDML.OTHER_getIDFromFunction(UI_func_seq_probabilities))
  {
    print_live_probability_pattern_info();
  }
  else if (LCDML.FUNC_getID() == LCDML.OTHER_getIDFromFunction(UI_func_seq_mute_matrix))
  {
    uint8_t track_count = 0;
    for (uint8_t y = 0; y < 2; y++)
    {
      for (uint8_t x = 0; x < 4; x++)
      {
        if (seq.current_chain[track_count] != 99)
        {
          display.setCursor((2 * CHAR_width_small) + x * (CHAR_width_small * 14), 4 + (10 * CHAR_height_small) + (y * (CHAR_height_small * 8)));
          set_pattern_content_type_color(seq.current_pattern[track_count]);
          if (seq.content_type[seq.current_pattern[track_count]] > 0) // it is a Inst. pattern
          {
            if (seq.note_data[seq.current_pattern[track_count]][seq.step] > 12 && seq.note_data[seq.current_pattern[track_count]][seq.step] != 130 && seq.note_data[seq.current_pattern[track_count]][seq.step] != 99)
            {
              display.print(noteNames[seq.note_data[seq.current_pattern[track_count]][seq.step] % 12][0]);
              if (noteNames[seq.note_data[seq.current_pattern[track_count]][seq.step] % 12][1] != '\0')
              {
                display.print(noteNames[seq.note_data[seq.current_pattern[track_count]][seq.step] % 12][1]);
              }
              display.print((seq.note_data[seq.current_pattern[track_count]][seq.step] / 12) - 1);
            }
            else if (seq.note_data[seq.current_pattern[track_count]][seq.step] == 130) // latch
              display.print(F("LAT"));
            else
              display.print(F("   "));
          }
          else                                                             // it is a drum pattern
            if (seq.vel[seq.current_pattern[track_count]][seq.step] < 210) // is Drumtrack and not a pitched sample
            {
              bool found = false;
              for (uint8_t n = 0; n < NUM_DRUMSET_CONFIG - 1; n++)
              {
                if (seq.note_data[seq.current_pattern[track_count]][seq.step] == drum_config[n].midinote)
                {
                  display.print(drum_config[n].shortname);
                  found = true;
                  break;
                }
              }
              if (found == false)
                display.print(F("- "));
            }
            else if (seq.vel[seq.current_pattern[track_count]][seq.step] > 209) // pitched sample
              display.print(F("PS"));
        }
        track_count++;
      }
    }
  }
  // else if (LCDML.FUNC_getID() == LCDML.OTHER_getIDFromFunction(UI_func_seq_pianoroll)) // is in UI of Pianoroll
  // {
  //   update_pianoroll();
  // }
}

uint8_t screensaver_mode_active;
int screensaver_switcher_timer;
uint8_t screensaver_brightness = 255;
uint16_t screensaver_counthue = 0;

bool flock_running = false;

extern void terrain_init();
extern void terrain_frame();

extern void flock_init();
extern void flock_frame();

extern bool wakeScreenFlag;

void resetScreenTimer() {
  LCDML.SCREEN_resetTimer();
  // exit screensaver if active
  if (LCDML.FUNC_getID() == LCDML.OTHER_getIDFromFunction(mFunc_screensaver)) {
    int id = LCDML.MENU_getLastActiveFunctionID();
    if (id != _LCDML_NO_FUNC) {
      LCDML.OTHER_jumpToID(id);
    }
    else {
      LCDML.FUNC_goBackToMenu();
    }
  }
}

// *********************************************************************
FLASHMEM void mFunc_screensaver(uint8_t param) // screensaver
// *********************************************************************
{
  if (LCDML.FUNC_setup()) // ****** SETUP *********
  {
    encoderDir[ENC_R].reset();
    // remove compiler warnings when the param variable is not used:
    LCDML_UNUSED(param);
    display.fillScreen(0);

    screensaver_counthue = random(SCREENSAVER_MAX_COUNTHUE);
    InitializeCube();
    if (configuration.sys.screen_saver_mode == ScreenSaver::RANDOM)
    {
      randomSeed(analogRead(0));
      screensaver_mode_active = random(ScreenSaver::NUM_SCREENSAVERS - 2) + 1; // skip 0, since that is for random
    }
    else {
      screensaver_mode_active = configuration.sys.screen_saver_mode;
    }

    if (screensaver_mode_active == ScreenSaver::TERRAIN) {
      terrain_init();
    }

    // setup function
    LCDML.FUNC_setLoopInterval(SCREENSAVER_INTERVAL_MS); // screensaver FUNC_loop() call interval
  }

  if (LCDML.FUNC_loop()) // ****** LOOP *********
  {
    if (remote_active) {
      display.console = true;
    }
    if (wakeScreenFlag || LCDML.BT_checkAny()) {
      // fast wakeup from MIDI noteOn event and touch presses
      resetScreenTimer();
    }

    if (++screensaver_counthue > SCREENSAVER_MAX_COUNTHUE) {
      screensaver_counthue = 0;
    }
    if (screensaver_mode_active == ScreenSaver::QIX)
      qix_screensaver();
    else if (screensaver_mode_active == ScreenSaver::CUBE)
      cube_screensaver();
    else if (screensaver_mode_active == ScreenSaver::SWARM)
    {
      if (flock_running == false)
      {
        flock_init();
        flock_running = true;
      }
      else
        flock_frame();
    }
    else if (screensaver_mode_active == ScreenSaver::TERRAIN)
    {
      terrain_frame();
    }

    if (configuration.sys.screen_saver_mode == ScreenSaver::RANDOM)
    {
      screensaver_switcher_timer++;
      if (screensaver_switcher_timer > SCREENSAVER_STAY_TIME)
      {
        screensaver_switcher_timer = 0;
        display.fillScreen(COLOR_BACKGROUND);

        randomSeed(analogRead(0));
        uint8_t oldScreenSaver = screensaver_mode_active;
        while (oldScreenSaver == screensaver_mode_active) {
          screensaver_mode_active = random(ScreenSaver::NUM_SCREENSAVERS - 2) + 1; // skip 0, since that is for random
        }
        if (screensaver_mode_active == ScreenSaver::CUBE) { // reinit because 3dterrain messes up some of it's vars in same functions
          InitializeCube();
        }
        if (screensaver_mode_active == ScreenSaver::TERRAIN) { // reinit because 3dterrain messes up some of it's vars in same functions
          terrain_init();
        }
      }

      if (screensaver_switcher_timer > (SCREENSAVER_STAY_TIME - SCREENSAVER_FADE_TIME)) {
        if (screensaver_brightness > SCREENSAVER_BRIGHTNESS_STEP) {
          screensaver_brightness -= SCREENSAVER_BRIGHTNESS_STEP;
        }
      }
      else if (screensaver_switcher_timer < SCREENSAVER_FADE_TIME) {
        if (screensaver_brightness < 255 - SCREENSAVER_BRIGHTNESS_STEP) {
          screensaver_brightness += SCREENSAVER_BRIGHTNESS_STEP;
        }
      }
      else {
        screensaver_brightness = 255;
      }
    }
  }
  if (LCDML.FUNC_close()) // ****** STABLE END *********
  {
    if (configuration.sys.screen_saver_mode != ScreenSaver::DISABLED)
    {
      encoderDir[ENC_L].reset();
      encoderDir[ENC_R].reset();
      display.fillScreen(COLOR_BACKGROUND);
    }
  }
}

FLASHMEM void setup_screensaver(void)
{
  configuration.sys.screen_saver_start = constrain(configuration.sys.screen_saver_start, 1, 59);
  if (configuration.sys.screen_saver_mode == ScreenSaver::DISABLED) // off
  {
    LCDML.SCREEN_disable();
  }
  else
  {
    // Enable Screensaver (screensaver menu function, time to activate in ms)
    LCDML.SCREEN_enable(mFunc_screensaver, configuration.sys.screen_saver_start * 60000); // from parameter in minutes
    // LCDML.SCREEN_enable(mFunc_screensaver, 3000); // quick screensaver test time
  }
}

FLASHMEM void setup_ui(void)
{
  _setup_rotation_and_encoders(true);

  if (LCDML.BT_setup())
  {
    ENCODER[ENC_R].begin();
    ENCODER[ENC_L].begin();

#ifdef PCM5102_MUTE_PIN
    pinMode(PCM5102_MUTE_PIN, OUTPUT);
    digitalWrite(PCM5102_MUTE_PIN, HIGH); // ENABLE/UNMUTE DAC
#endif
  }

  display.setTextColor(COLOR_SYSTEXT, COLOR_BACKGROUND);
  display.setTextSize(2);
  display.fillScreen(COLOR_BACKGROUND);

  // LCDMenuLib Setup
  LCDML_setup(_LCDML_DISP_cnt);

  // Enable Menu Rollover
  // LCDML.MENU_enRollover();
  setup_screensaver();
}

FLASHMEM void toggle_sequencer_play_status()
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

FLASHMEM boolean key_right()
{

  if (remote_MIDI_CC == 20)
  {
    remote_MIDI_CC = 0;
    return true;
  }

#ifdef ONBOARD_BUTTON_INTERFACE
  if (digitalRead(BI_RIGHT) == false)
    return true;
#endif

  if (joysticks[0].getAxis(0) == GAMEPAD_RIGHT_0 && joysticks[0].getAxis(1) == GAMEPAD_RIGHT_1)
    return true;

  return false;
}

FLASHMEM boolean key_left()
{
  if (remote_MIDI_CC == 21)
  {
    remote_MIDI_CC = 0;
    return true;
  }

#ifdef ONBOARD_BUTTON_INTERFACE
  if (digitalRead(BI_LEFT) == false)
    return true;
#endif

  if (joysticks[0].getAxis(0) == GAMEPAD_LEFT_0 && joysticks[0].getAxis(1) == GAMEPAD_LEFT_1)
    return true;

  return false;
}

FLASHMEM boolean key_up()
{

  if (remote_MIDI_CC == 22)
  {
    remote_MIDI_CC = 0;
    return true;
  }

#ifdef ONBOARD_BUTTON_INTERFACE
  if (digitalRead(BI_UP) == false)
    return true;
#endif

  if (joysticks[0].getAxis(0) == GAMEPAD_UP_0 && joysticks[0].getAxis(1) == GAMEPAD_UP_1)
    return true;

  return false;
}

FLASHMEM boolean key_down()
{

  if (remote_MIDI_CC == 23)
  {
    remote_MIDI_CC = 0;
    return true;
  }

#ifdef ONBOARD_BUTTON_INTERFACE
  if (digitalRead(BI_DOWN) == false)
    return true;
#endif

  if (joysticks[0].getAxis(0) == GAMEPAD_DOWN_0 && joysticks[0].getAxis(1) == GAMEPAD_DOWN_1)
    return true;

  return false;
}

FLASHMEM void gamepad_seq_navigation_func(uint32_t buttons)
{
  if (gamepad_millis > configuration.sys.gamepad_speed && seq.cycle_touch_element < 6 && buttons == GAMEPAD_SELECT && key_right())
  {
    seq.cycle_touch_element = 6; // goto chain edit
    seq.help_text_needs_refresh = true;
    seq.edit_state = true;
    print_chain_steps_in_song_page();
    print_transpose_in_song_page();
    print_chain_matrix_in_song_page();
    print_shortcut_navigator();
    print_song_mode_help();
    gamepad_millis = 0;
  }
  else if ((seq.cycle_touch_element == 6 && buttons == GAMEPAD_SELECT && key_left()) || (seq.cycle_touch_element == 7 && buttons == GAMEPAD_SELECT && key_left()))
  {
    seq.cycle_touch_element = 0; // goto main song mode
    seq.help_text_needs_refresh = true;
    seq.edit_state = false;
    print_chain_steps_in_song_page();
    print_transpose_in_song_page();
    print_chain_matrix_in_song_page();
    print_shortcut_navigator();
    print_song_mode_help();
    gamepad_millis = 0;
  }
  else if ((seq.cycle_touch_element == 8 && buttons == GAMEPAD_SELECT && key_left()) || (seq.cycle_touch_element == 9 && buttons == GAMEPAD_SELECT && key_left()))
  {
    seq.cycle_touch_element = 6; // go back from transpose to chain
    seq.help_text_needs_refresh = true;
    seq.edit_state = true;
    print_chain_steps_in_song_page();
    print_transpose_in_song_page();
    print_chain_matrix_in_song_page();
    print_shortcut_navigator();
    print_song_mode_help();
    gamepad_millis = 0;
  }
  else if ((seq.cycle_touch_element == 6 && buttons == GAMEPAD_SELECT && key_right()) || (seq.cycle_touch_element == 7 && buttons == GAMEPAD_SELECT && key_right()))
  {
    seq.cycle_touch_element = 8; // goto transpose from chain
    seq.help_text_needs_refresh = true;
    seq.edit_state = true;
    print_chain_steps_in_song_page();
    print_transpose_in_song_page();
    print_chain_matrix_in_song_page();
    print_shortcut_navigator();
    print_song_mode_help();
    gamepad_millis = 0;
  }
  else if (buttons == GAMEPAD_SELECT && key_up())
  {
    generic_temp_select_menu = 6; // preselect BPM
    LCDML.OTHER_jumpToFunc(UI_func_seq_settings);
    gamepad_millis = 0;
  }
  else if (seq.cycle_touch_element > 7 && buttons == GAMEPAD_SELECT && key_right())
  { // go to pattern editor
    gamepad_millis = 0;
    seq.quicknav_song_to_pattern_jump = true;
    LCDML.OTHER_jumpToFunc(UI_func_seq_pattern_editor);
  }
}

FLASHMEM void gamepad_learn_func(uint32_t buttons)
{
  if (gamepad_millis > 300)
  {
    if (temp_int > 7)
    {
      setCursor_textGrid_small(35, 3);
      display.setTextColor(GREEN, COLOR_BACKGROUND);
      display.print(F("READY ! "));
      setCursor_textGrid_small(1, 17);
      display.setTextColor(RED, COLOR_BACKGROUND);
      display.print(F("GO BACK WITH ENC[L]"));
      setCursor_textGrid_small(21, 17);
      display.setTextColor(GREEN, COLOR_BACKGROUND);
      display.print(F("AND TEST OUT YOUR GAMEPAD."));
      setCursor_textGrid_small(1, 18);
      display.print(F("ANY CHANGE TO GLOBALS, LIKE "));
      display.setTextColor(COLOR_SYSTEXT, COLOR_BACKGROUND);
      display.print(F("MASTER VOLUME "));
      setCursor_textGrid_small(1, 19);
      display.setTextColor(GREEN, COLOR_BACKGROUND);
      display.print(F("WILL STORE YOUR CURRENT GAMEPAD SETTINGS."));
    }
    else
    {
      display.setTextColor(COLOR_SYSTEXT, COLOR_BACKGROUND);
      setCursor_textGrid_small(35, 3);
      display.print(F("STEP "));
      display.print(temp_int + 1);
      display.print(F("/8  "));
    }
    if (temp_int == 0)
      display.setTextColor(RED, COLOR_BACKGROUND);
    else if (temp_int > 0)
      display.setTextColor(GREEN, COLOR_BACKGROUND);
    else
      display.setTextColor(GREY2, COLOR_BACKGROUND);
    setCursor_textGrid_small(16, 6);
    print_formatted_number(GAMEPAD_UP_0, 3);
    setCursor_textGrid_small(20, 6);
    print_formatted_number(GAMEPAD_UP_1, 3);

    if (temp_int == 1)
      display.setTextColor(RED, COLOR_BACKGROUND);
    else if (temp_int > 1)
      display.setTextColor(GREEN, COLOR_BACKGROUND);
    else
      display.setTextColor(GREY2, COLOR_BACKGROUND);
    setCursor_textGrid_small(16, 7);
    print_formatted_number(GAMEPAD_DOWN_0, 3);
    setCursor_textGrid_small(20, 7);
    print_formatted_number(GAMEPAD_DOWN_1, 3);

    if (temp_int == 2)
      display.setTextColor(RED, COLOR_BACKGROUND);
    else if (temp_int > 2)
      display.setTextColor(GREEN, COLOR_BACKGROUND);
    else
      display.setTextColor(GREY2, COLOR_BACKGROUND);
    setCursor_textGrid_small(16, 8);
    print_formatted_number(GAMEPAD_LEFT_0, 3);
    setCursor_textGrid_small(20, 8);
    print_formatted_number(GAMEPAD_LEFT_1, 3);

    if (temp_int == 3)
      display.setTextColor(RED, COLOR_BACKGROUND);
    else if (temp_int > 3)
      display.setTextColor(GREEN, COLOR_BACKGROUND);
    else
      display.setTextColor(GREY2, COLOR_BACKGROUND);
    setCursor_textGrid_small(16, 9);
    print_formatted_number(GAMEPAD_RIGHT_0, 3);
    setCursor_textGrid_small(20, 9);
    print_formatted_number(GAMEPAD_RIGHT_1, 3);

    if (temp_int == 4)
      display.setTextColor(RED, COLOR_BACKGROUND);
    else if (temp_int > 4)
      display.setTextColor(GREEN, COLOR_BACKGROUND);
    else
      display.setTextColor(GREY2, COLOR_BACKGROUND);
    setCursor_textGrid_small(16, 11);
    print_formatted_number(GAMEPAD_BUTTON_A, 3);
    if (temp_int == 5)
      display.setTextColor(RED, COLOR_BACKGROUND);
    else if (temp_int > 5)
      display.setTextColor(GREEN, COLOR_BACKGROUND);
    else
      display.setTextColor(GREY2, COLOR_BACKGROUND);
    setCursor_textGrid_small(16, 12);
    print_formatted_number(GAMEPAD_BUTTON_B, 3);
    if (temp_int == 6)
      display.setTextColor(RED, COLOR_BACKGROUND);
    else if (temp_int > 6)
      display.setTextColor(GREEN, COLOR_BACKGROUND);
    else
      display.setTextColor(GREY2, COLOR_BACKGROUND);
    setCursor_textGrid_small(16, 14);
    print_formatted_number(GAMEPAD_SELECT, 3);
    if (temp_int == 7)
      display.setTextColor(RED, COLOR_BACKGROUND);
    else if (temp_int > 7)
      display.setTextColor(GREEN, COLOR_BACKGROUND);
    else
      display.setTextColor(GREY2, COLOR_BACKGROUND);
    setCursor_textGrid_small(16, 15);
    print_formatted_number(GAMEPAD_START, 3);
    if (buttons != gamepad_buttons_neutral || joysticks[0].getAxis(0) != gamepad_0_neutral || joysticks[0].getAxis(1) != gamepad_1_neutral)
    {
      if (temp_int == 0)
      {
        GAMEPAD_UP_0 = joysticks[0].getAxis(0);
        GAMEPAD_UP_1 = joysticks[0].getAxis(1);
      }
      else if (temp_int == 1)
      {
        GAMEPAD_DOWN_0 = joysticks[0].getAxis(0);
        GAMEPAD_DOWN_1 = joysticks[0].getAxis(1);
      }
      else if (temp_int == 2)
      {
        GAMEPAD_LEFT_0 = joysticks[0].getAxis(0);
        GAMEPAD_LEFT_1 = joysticks[0].getAxis(1);
      }
      else if (temp_int == 3)
      {
        GAMEPAD_RIGHT_0 = joysticks[0].getAxis(0);
        GAMEPAD_RIGHT_1 = joysticks[0].getAxis(1);
      }
      else if (temp_int == 4)
      {
        GAMEPAD_BUTTON_A = buttons;
      }
      else if (temp_int == 5)
      {
        GAMEPAD_BUTTON_B = buttons;
      }
      else if (temp_int == 6)
      {
        GAMEPAD_SELECT = buttons;
      }
      else if (temp_int == 7)
      {
        GAMEPAD_START = buttons;
      }
      temp_int++;
      gamepad_millis = 0;
    }
  }
}
// #endif

// ####################################################################################################################################################################################################

/***********************************************************************
   GENERAL UI ELEMENTS
 ***********************************************************************/

FLASHMEM void setModeColor(uint8_t selected_option)
{
  if (generic_temp_select_menu == selected_option)
  {
    display.setTextColor(COLOR_BACKGROUND, COLOR_SYSTEXT);
  }
  else
    display.setTextColor(COLOR_SYSTEXT, COLOR_BACKGROUND);
  if ((generic_temp_select_menu == selected_option && generic_active_function == 1) || (generic_temp_select_menu == selected_option && seq.edit_state == 1))
  {
    display.setTextColor(COLOR_SYSTEXT, RED);
  }
}

// only used for displaying value as a bar-graph display without numeric value, borders or other extras
FLASHMEM void print_fast_level_indicator(uint8_t x, uint8_t y, int16_t input_value, int16_t limit_min, int16_t limit_max)
{
  if (limit_min >= 0)
  { // filled bar
    uint8_t split = (5 * CHAR_width_small - 2) * (input_value - limit_min) / (limit_max - limit_min);
    display.console = true;
    if (split < 5 * CHAR_width_small - 2)
      display.fillRect(CHAR_width_small * x + 4 * CHAR_width_small + split, 10 * y + 1, 5 * CHAR_width_small - 2 - split, 5, COLOR_BACKGROUND);
    display.console = true;
    if (split > 0)
      display.fillRect(CHAR_width_small * x + 4 * CHAR_width_small, 10 * y + 1, split, 5, RED);
  }
}

// a small scaled bar, that can show an abitrary range in always the same space, given the expected min and max limit values.
// also atomagically shows a pan bar (marker line instead of filled bar), if min is smaller than zero.
FLASHMEM void print_small_scaled_bar(uint8_t x, uint8_t y, int16_t input_value, int16_t limit_min, int16_t limit_max, int16_t selected_option, boolean show_bar, const char* zero_name = NULL)
{
  setCursor_textGrid_small(x, y);
  setModeColor(selected_option);
  // display.print(F("   ")); // make sure long numbers don't stuck vs. short zero-names
  // setCursor_textGrid_small(x, y);

  if (limit_min == 0 && limit_max == 1)
    display.print(input_value ? F("ON ") : F("OFF"));
  else if (zero_name != NULL && input_value == 0)
    display.print(zero_name);
  else if (limit_min < 0)
    print_formatted_number_signed(input_value, 2);
  else if (limit_max <= 99)
  {
    setCursor_textGrid_small(x + 1, y);
    print_formatted_number(input_value, 2);
  }
  else
    print_formatted_number(input_value, 3);

  if (show_bar)
  {
    display.console = true;

    display.drawRect(CHAR_width_small * x + 4 * CHAR_width_small, 10 * y, 5 * CHAR_width_small, 7, input_value == 0 ? GREY2 : COLOR_SYSTEXT);

    if (limit_min >= 0)
    { // filled bar
      uint8_t split = (5 * CHAR_width_small - 2) * (input_value - limit_min) / (limit_max - limit_min);
      display.console = true;
      if (split < 5 * CHAR_width_small - 2)
        display.fillRect(CHAR_width_small * x + 4 * CHAR_width_small + 1 + split, 10 * y + 1, 5 * CHAR_width_small - 2 - split, 5, COLOR_BACKGROUND);
      display.console = true;
      if (split > 0)
        display.fillRect(CHAR_width_small * x + 4 * CHAR_width_small + 1, 10 * y + 1, split, 5, COLOR_PITCHSMP);
    }
    else
    { // pan bar
      uint8_t split = (5 * CHAR_width_small - 2 - 3) * (input_value - limit_min) / (limit_max - limit_min);
      display.console = true;
      display.fillRect(CHAR_width_small * x + 4 * CHAR_width_small + 1, 10 * y + 1, 5 * CHAR_width_small - 2, 7 - 2, COLOR_BACKGROUND);
      display.console = true;
      display.fillRect(CHAR_width_small * x + 4 * CHAR_width_small + 1 + split, 10 * y + 1, 3, 5, COLOR_PITCHSMP);
    }
  }
}

// FLASHMEM void check_glow(uint8_t x, uint8_t y, uint8_t input_value, uint8_t selected_option)
// {
// if (generic_temp_select_menu==selected_option )
//   {
//    glow_x=x;
//   glow_y=y;
//   glow_value=input_value;
// }
// else
//  {
//   glow_type=0;
// }
//}

FLASHMEM void print_small_intbar(uint8_t x, uint8_t y, uint8_t input_value, uint8_t selected_option, boolean show_bar, boolean show_zero)
{
  print_small_scaled_bar(x, y, input_value, 0, 100, selected_option, show_bar, show_zero ? NULL : (const char*)F("OFF"));
  // glow_type=1;
  // check_glow(x,y, input_value,selected_option);
}

FLASHMEM void print_small_panbar(uint8_t x, uint8_t y, uint8_t input_value, uint8_t selected_option)
{
  setCursor_textGrid_small(x, y);
  setModeColor(selected_option);
  if (input_value < 20)
  {
    display.print(F("L"));
    print_formatted_number(20 - input_value, 2);
  }
  else if (input_value > 20)
  {
    display.print(F("R"));
    print_formatted_number(input_value - 20, 2);
  }
  else
  {
    display.print(F("C"));
    print_formatted_number(input_value - 20, 2);
  }
  display.console = true;
  display.drawRect(CHAR_width_small * x + 4 * CHAR_width_small, 10 * y, 5 * CHAR_width_small, 7, COLOR_SYSTEXT);
  display.console = true;
  display.fillRect(CHAR_width_small * x + 4 * CHAR_width_small + 1, 10 * y + 1, 5 * CHAR_width_small - 2, 7 - 2, COLOR_BACKGROUND);
  display.fillRect(CHAR_width_small * x + 4 * CHAR_width_small + 1 + input_value / 1.60, 10 * y + 1, 3, 5, COLOR_PITCHSMP);
  // glow_type=2;
  // check_glow(x,y, input_value,selected_option);
}
FLASHMEM void print_small_panbar_mixer(uint8_t x, uint8_t y, uint8_t input_value, uint8_t selected_option)
{
  setCursor_textGrid_small(x, y);
  setModeColor(selected_option);
  display.console = true;
  display.drawRect(CHAR_width_small * x, 10 * y, 3 * CHAR_width_small, 7, COLOR_SYSTEXT);
  display.console = true;
  display.fillRect(CHAR_width_small * x + 1, 10 * y + 1, 3 * CHAR_width_small - 2, 5, COLOR_BACKGROUND);
  display.fillRect(CHAR_width_small * x + 1 + input_value / 2.30, 10 * y + 1, 3, 5, COLOR_PITCHSMP);
}

// read right encoder change, with optional acceleration.
FLASHMEM int16_t encoder_change(bool fast)
{
  int16_t dir = 0;
  if (LCDML.BT_checkDown())
    dir = 1;
  if (LCDML.BT_checkUp())
    dir = -1;

  // if(fast) return dir * (ENCODER[ENC_R].speed() + ENCODER[ENC_L].speed());
  if (fast)
    return dir * (ENCODER[ENC_R].speed());
  else
    return dir;
}

// check if certain encoder changed.
// as this resets internal state by the way, it should only be called once per menu loop cycle.
// TODO get rid of the side-effects.
bool encoder_changed(uint8_t id)
{
  return (LCDML.BT_checkDown() && encoderDir[id].Down()) || (LCDML.BT_checkUp() && encoderDir[id].Up()) || LCDML.BT_checkEnter();
}

// Store a generic parameter editor.
//
// The editor has
// - a name, as shown on screen
// - a position on screen (x,y),
// - a select_id defining its position in the selection order (usually assigned automatically)
// - limits for its value
// - a fast flag, to enable accelerated changes of large values (also set automatically for large ranges)
// - a pointer to the value to adjust, can be NULL if only custom setters / getters are used.
// - a pointer to a getter function (can be set automatically for simple editors, using the value pointer)
// - a pointer to a setter function (can be set automatically for simple editors, using the value pointer)
// - a pointer to a renderer (can be null to provide the default small-scaled-bar-with-number editor)
class Editor
{
  friend class UI;

private:
  const bool fast;                                     // if the editor should accelerate. Automatically set for large min-max ranges.
  int16_t(* const getter)(Editor* param);              // pointer to a function to read the value from somewhere
  void (* const setter)(Editor* param, int16_t value);  // pointer to a function to save the altered value somewhere
  void (* const renderer)(Editor* param, bool refresh); // pointer to a function to draw the editor on screen.
  // if refresh is true, it should redraw value-dependent parts.

  // constructor for creating empty array
  Editor()
    : fast(false), getter(NULL), setter(NULL), renderer(NULL), name(NULL), x(0), y(0), select_id(-1),
    limit_min(0), limit_max(0), value(NULL) {}

  // only valid constructor used by friend class UI
  Editor(const char* const _name, int16_t _limit_min, int16_t _limit_max,
    uint8_t _x, uint8_t _y, uint8_t _select_id,
    void* const _valuePtr,
    int16_t(* const _getter)(Editor* param),
    void (* const _setter)(Editor* param, int16_t value),
    void (* const _renderer)(Editor* param, bool refresh))
    : fast(limit_max - limit_min > 32), getter(_getter), setter(_setter), renderer(_renderer),
    name(_name), x(_x), y(_y), select_id(_select_id), limit_min(_limit_min), limit_max(_limit_max),
    value(_valuePtr) {}

  // draw this editor on screen. If refresh is set,
  // only parts that depend on changed value or selection are drawn.
  // otherwise everything is drawn (label, borders, etc.)
  //
  // by default if renderer==NULL, print_small_scaled_bar is used.
  // otherwise the definded renderer is used.
  //
  void draw_editor(bool refresh)
  {
    if (renderer != NULL)
    {
      // a custom renderer is defined. Draw using it and exit.
      renderer(this, refresh);
      return;
    }
    // No custom rederer was defined, just draw label and small_scaled_bar.
    display.setTextSize(1);
    if (!refresh)
    {
      setCursor_textGrid_small(this->x + 10, this->y);
      display.setTextColor(GREY2, COLOR_BACKGROUND);
      display.print(this->name);
    }
    print_small_scaled_bar(x, y, get(), limit_min, limit_max, select_id, 1, NULL);
  };

  // read encoders / other input sources and adjust the assigned value
  // by using the getter and setter functions. And redraw afterwards.
  void handle_parameter_editor()
  {
    int16_t change = encoder_change(fast);

    if (LCDML.FUNC_getID() == LCDML.OTHER_getIDFromFunction(UI_func_drums) && generic_temp_select_menu == 0)
    {
      if (change != 0)
      {
        if (change > 0)
          smart_filter(1);
        else
          smart_filter(0);
        draw_editor(true);
      }
    }
    else
    {
      if (change != 0)
      {
        set(constrain(get() + change, limit_min, limit_max));
        draw_editor(true);
      }
    }

  };

public:
  // these members are never set, but still public, as they are
  // used by custom getter, setter and render functions.
  const char* const name;             // short name of parameter.
  const uint8_t x, y;                 // position on screen in small-font grid
  const uint8_t select_id;            // position in input selection order - automatically set on definiton of UI elements.
  const int16_t limit_min, limit_max; // int limits of the parameter, used to limit input and scale the bar
  void* const value;                  // a pointer to the value to adjust. The value can be of any type (eg. uint8_t, int16_t, float32_t...)
  // The getter and setter functions must correctly handle the type.
  // get & set this editors value, propagating to the value storage or setter.
  int16_t get()
  {
    if (getter != NULL)
      return getter(this);
    return 0; // should never happen
  };
  void set(int16_t _value)
  {
    // should always be !=NULL, just prevent crashes.
    if (setter != NULL)
      setter(this, _value);
  };
};

// Global UI helper
//
// This helps conveniently build a single UI page,
// and repeatedly update all UI elements with a calls to handle_input() later.
//
// a lot of addEditor and addCustomEditor functions are provided to define UI elements
// for different value types, adding custom getters or setters to update the engines under control.
// And provide custom renderers for some elements (eg. the instance selector, name fields etc.)
//
// The usual UI definition goes this way:
//
// on LCDML.func_setup():
// -Remove the old UI by calling ui.reset(). This also clears the screen.
// -Add one or more editors by addEditor and addCustom editor. Create sections by setCursor() and printLn().
// -all coding work goes here, especially by providing custom getter, setter or renderer code to handle
//  the different parameter types and how engine parameters are modified.
// on LCDML.func_loop():
// -just call ui.handle_input().
//
#define UI_MAX_EDITORS 64
class UI
{
private:
  uint8_t x, y;                      // current cursor position, can be set and updates automatically
  uint8_t num_editors;               // the count of editors defined so far
  Editor editors[UI_MAX_EDITORS];    // the editors defined for the current page
  Editor* encoderLeftHandler = NULL; // a special editor assigned to the left encoder
  Editor* buttonLongHandler = NULL;  // a special editor assigned to a long button press

public:
  // clear, but dont reset this UI.
  // this can be called for massive refreshs without loosing the current selection.
  // All UI elements must be defined again afterwards.
  void clear()
  {
    display.fillScreen(COLOR_BACKGROUND);
    border0();


    if (touch_button_back_page() || legacy_touch_button_back_page())
    {
      back_touchbutton();
      // current_page_has_touch_back_button = true;
    }
    else
      helptext_l(back_text);

    num_editors = 0;
    buttonLongHandler = NULL;
    encoderLeftHandler = NULL;
  };

  // reset. this must be called to start a new UI page.
  // like clear(), but the selection is reset too.
  void reset()
  {
    clear();
    seq.edit_state = 0;
    generic_temp_select_menu = 0;
  };

  // set the cursor before drawing the next UI element.
  // if omitted, the next element is drawn just below the last one.
  void setCursor(uint8_t _x, uint8_t _y)
  {
    x = _x;
    y = _y;
  };

  // print a static label that can't be selected.
  void printLn(const char* text, uint32_t color = COLOR_SYSTEXT)
  {
    print(text, color);
    y += 1;
  }

  // print some text on cursor location for use in renderers
  void print(const char* text, uint32_t color = COLOR_SYSTEXT)
  {
    display.setTextSize(1);
    setCursor_textGrid_small(x, y);
    display.setTextColor(color);
    display.print(text);
  }

  // add a custom editor providing its own getter, setter and renderer function.
  void addCustomEditor(const char* const name, int16_t limit_min, int16_t limit_max,
    void* const valuePtr,
    int16_t(* const getter)(Editor* param),
    void (* const setter)(Editor* param, int16_t value),
    void (* const renderer)(Editor* param, bool refresh))
  {

    // construct editor in-place in editors array
    // there is no corresponding delete, as editors are just overwritten after ui.reset() !
    new (&editors[num_editors]) Editor(
      name, limit_min, limit_max, x, y, num_editors, valuePtr,
      getter, setter, renderer);

    editors[num_editors].draw_editor(false);
    y++;
    num_editors++;
  };

  // editor providing default float32_t getter + setters if missed out
  void addEditor(const char* const name, int16_t limit_min, int16_t limit_max, float32_t* valuePtr,
    int16_t(* const getter)(Editor* param) = NULL,
    void (* const setter)(Editor* param, int16_t value) = NULL,
    void (* const renderer)(Editor* param, bool refresh) = NULL)
  {
    addCustomEditor(
      name, limit_min, limit_max, valuePtr,
      getter != NULL ? getter : [](Editor* editor) -> int16_t
      {
        return *((float32_t*)editor->value) * 100;
      },
      setter != NULL ? setter : [](Editor* editor, int16_t value) -> void
      {
        *((float32_t*)editor->value) = value / 100.f;
      },
      renderer);
  };

  // editor providing default uint8_t getter + setters if missed out
  void addEditor(const char* const name, int16_t limit_min, int16_t limit_max, uint8_t* const valuePtr,
    int16_t(* const getter)(Editor* param) = NULL,
    void (* const setter)(Editor* param, int16_t value) = NULL,
    void (* const renderer)(Editor* param, bool refresh) = NULL)
  {
    addCustomEditor(
      name, limit_min, limit_max, valuePtr,
      getter != NULL ? getter : [](Editor* editor) -> int16_t
      {
        return *((uint8_t*)editor->value);
      },
      setter != NULL ? setter : [](Editor* editor, int16_t value) -> void
      {
        *((uint8_t*)editor->value) = value;
      },
      renderer);
  };

  // editor providing custom getter + setters not using the valuePtr feature
  void addEditor(const char* const name, int16_t limit_min, int16_t limit_max,
    int16_t(* const getter)(Editor* param),
    void (* const setter)(Editor* param, int16_t value),
    void (* const renderer)(Editor* param, bool refresh) = NULL)
  {
    addCustomEditor(
      name, limit_min, limit_max, NULL,
      getter != NULL ? getter : [](Editor* editor) -> int16_t
      {
        return *((uint8_t*)editor->value);
      },
      setter != NULL ? setter : [](Editor* editor, int16_t value) -> void
      {
        *((uint8_t*)editor->value) = value;
      },
      renderer);
  };

  // assign the editor just added to the long button press too (just a toggle action)
  void enableButtonLongEditor()
  {
    buttonLongHandler = &editors[num_editors - 1];
  }

  // check if the current UI snags on long button presses
  bool handlesButtonLong()
  {
    return buttonLongHandler != NULL;
  }

  // assign the editor just added to the left encoder (always, no selection needed)
  void enableLeftEncoderEditor()
  {
    encoderLeftHandler = &editors[num_editors - 1];
  }

  // check if the current UI snags on the left encoder
  bool handlesLeftEncoder()
  {
    return encoderLeftHandler != NULL;
  }

  // (re-)draw all editors, omit static parts if refresh is set.
  void draw_editors(bool refresh)
  {
    for (uint8_t i = 0; i < num_editors; i++)
      editors[i].draw_editor(refresh);
  };

  // Handle all input, updating editors and engine parameters, if changed.
  //
  // This need to be repeatedly called in the menu loop, usually by LCDML.func_loop() sections
  //
  void handle_input()
  {
    // toggle between navigate and value editing
    if (LCDML.BT_checkEnter() && encoderDir[ENC_R].ButtonShort())
    {
      seq.edit_state = 1 - seq.edit_state;
      editors[generic_temp_select_menu].draw_editor(true);
    }

    // handle right encoder
    if (encoder_changed(ENC_R))
    {
      if (seq.edit_state == 0)
      {
        // handle parameter selection
        uint8_t last = generic_temp_select_menu;
        generic_temp_select_menu = constrain(generic_temp_select_menu + encoder_change(false), 0, num_editors - 1);
        editors[last].draw_editor(true);
        editors[generic_temp_select_menu].draw_editor(true);
      }
      else
        // change currently selected editor's value by right encoder
        editors[generic_temp_select_menu].handle_parameter_editor();
    }

    // optionally set a specific editor's value by left encoder
    if (encoderLeftHandler && encoder_changed(ENC_L))
    {
      encoderLeftHandler->handle_parameter_editor();
    }
    // optionally toggle a specific editor by long button press
    if (buttonLongHandler && encoderDir[ENC_R].ButtonLong())
    {
      buttonLongHandler->set(1 - buttonLongHandler->get()); // toggle value between 0 and 1
      buttonLongHandler->draw_editor(true);
    }
  };
} ui;

/***********************************************************************
   MENU CONTROL
 ***********************************************************************/
FLASHMEM uint8_t get_current_cursor_id(void)
{
  LCDMenuLib2_menu* tmp;

  if ((tmp = LCDML.MENU_getCurrentObj()) != NULL)
    return (tmp->getChild(LCDML.MENU_getCursorPosAbs())->getID());
  else
    return (0);
}

uint8_t touchX = 0;
uint8_t touchY = 0;
extern void boot_animation();

FLASHMEM void lcdml_menu_control(void)
{
  // If something must init, put in in the setup condition

  if (back_from_volume > BACK_FROM_VOLUME_MS && LCDML.FUNC_getID() == LCDML.OTHER_getIDFromFunction(UI_func_volume))
  {
    encoderDir[ENC_L].reset();
    encoderDir[ENC_R].reset();

    if (LCDML.MENU_getLastActiveFunctionID() < 0xff)
      LCDML.OTHER_jumpToID(LCDML.MENU_getLastActiveFunctionID());
    else
      LCDML.OTHER_setCursorToID(LCDML.MENU_getLastCursorPositionID());
    // LCDML.FUNC_goBackToMenu();
  }

  // Volatile Variables
  long g_LCDML_CONTROL_Encoder_position[NUM_ENCODER] = { ENCODER[ENC_R].read(), ENCODER[ENC_L].read() };
  if (configuration.sys.ui_reverse)
  {
    button[0] = digitalRead(BUT_L_PIN);
    button[1] = digitalRead(BUT_R_PIN);
  }
  else
  {
    button[0] = digitalRead(BUT_R_PIN);
    button[1] = digitalRead(BUT_L_PIN);
  }

#ifdef USB_KEYPAD // USB KEYPAD CONTROL TEST
  if (USB_KEY != 0)
  {
    switch (USB_KEY)
    {
    case 211:
      g_LCDML_CONTROL_Encoder_position[ENC_L] = -4;
      break;
    case 218:
      g_LCDML_CONTROL_Encoder_position[ENC_L] = 4;
      break;
    case 43:
      button[ENC_L] = LOW;
      break;

    case 214:
      g_LCDML_CONTROL_Encoder_position[ENC_R] = -4;
      break;
    case 217:
      g_LCDML_CONTROL_Encoder_position[ENC_R] = 4;
      break;
    case 10:
      button[ENC_R] = LOW;
      break;
    }
  }
#endif

  uint32_t buttons = joysticks[0].getButtons();

  // MIDI remote
  if (remote_MIDI_CC > 0)
  {
    switch (remote_MIDI_CC)
    {
    case 24: // SELECT
      remote_MIDI_CC = 0;
      buttons = GAMEPAD_SELECT;
      remote_console_keystate_select = (remote_MIDI_CC_value == 127 ? true : false);
      break;
      // case 25: // START
      // buttons = buttons + GAMEPAD_START;
      // remote_MIDI_CC = 0;
      //   break;
    case 26: // BUTTON B
      remote_MIDI_CC = 0;
      buttons = buttons + GAMEPAD_BUTTON_B;
      remote_console_keystate_b = (remote_MIDI_CC_value == 127 ? true : false);
      break;
    case 27: // BUTTON A
      remote_MIDI_CC = 0;
      buttons = buttons + GAMEPAD_BUTTON_A;
      remote_console_keystate_a = (remote_MIDI_CC_value == 127 ? true : false);
      break;
    case 28: // init display at remote connection
      remote_MIDI_CC = 0;
      buttons = 0;
      remote_console_keystate_select = false;
      remote_console_keystate_a = false;
      remote_console_keystate_b = false;
      ts.touch_ui_drawn_in_menu = false;
      ts.keyb_in_menu_activated = false;

      // enable/disable remote display
      remote_active = (remote_MIDI_CC_value == 1 ? true : false);
      display.console = remote_active;

      // if sequencer is running, alert remote
      if (seq.running)
      {
        usbMIDI.sendRealTime(midi::Start);
      }

      // if remote active, dynamic patching for usb audio
      if (remote_active)
      {
        patchCordUsbL.connect();
        patchCordUsbR.connect();
      }
      else
      {
        patchCordUsbL.disconnect();
        patchCordUsbR.disconnect();
      }

      // LCDML.FUNC_goBackToMenu();
      LCDML.MENU_goRoot();
      break;
    case 29: // remote touch, receive x
      remote_MIDI_CC = 0;
      touchX = remote_MIDI_CC_value;
      break;
    case 30: // remote touch, receive y
      remote_MIDI_CC = 0;
      touchY = remote_MIDI_CC_value;

      // remote touch pressed
      ts.p.x = touchX * 2.5 + 1; // incoming x is divided by 2.5 minus 1
      ts.p.y = touchY * 2.5 + 1; // incoming y is divided by 2.5 minus 1
      buttons = 0;
      remote_touched = true;
      break;
    case 31:
      // remote touch released
      remote_MIDI_CC = 0;
      remote_touched = false;
      touchX = touchY = 0;
      break;
    default:
      break;
    }
  }

#ifdef ONBOARD_BUTTON_INTERFACE
  if (LCDML.FUNC_getID() != LCDML.OTHER_getIDFromFunction(UI_func_map_gamepad))
  {

    if (digitalRead(BI_SELECT) == false)
      buttons = GAMEPAD_SELECT;
    if (digitalRead(BI_START) == false)
    {
      if (buttons == GAMEPAD_SELECT)
        buttons = buttons + GAMEPAD_START;
      else
        buttons = GAMEPAD_START;
    }
    if (digitalRead(BI_BUTTON_A) == false)
      // buttons = buttons + GAMEPAD_BUTTON_A;
      buttons = GAMEPAD_BUTTON_A;
    if (digitalRead(BI_BUTTON_B) == false)
      // buttons = buttons + GAMEPAD_BUTTON_B;
      buttons = GAMEPAD_BUTTON_B;
    //}
  }
#endif

  if (remote_console_keystate_select)
  {
    buttons = GAMEPAD_SELECT;
    remote_console_keystate_select = true;
  }
  if (remote_console_keystate_a)
  {
    buttons = buttons + GAMEPAD_BUTTON_A;
    remote_console_keystate_a = true;
  }
  if (remote_console_keystate_b)
  {
    buttons = buttons + GAMEPAD_BUTTON_B;
    remote_console_keystate_b = true;
  }

  if (gamepad_millis + (gamepad_accelerate) >= configuration.sys.gamepad_speed)
  {

    // key-learn function
    if (LCDML.FUNC_getID() == LCDML.OTHER_getIDFromFunction(UI_func_map_gamepad) && temp_int < 9)
    {
      gamepad_learn_func(buttons);
    }
    // LSDJ Style Navigation:
    else if (buttons == GAMEPAD_SELECT && LCDML.FUNC_getID() == LCDML.OTHER_getIDFromFunction(UI_func_song))
    {
      gamepad_seq_navigation_func(buttons);
    }
    else if ((buttons != 0 && LCDML.FUNC_getID() == LCDML.OTHER_getIDFromFunction(UI_func_seq_pattern_editor)) || (buttons != 0 && LCDML.FUNC_getID() == LCDML.OTHER_getIDFromFunction(UI_func_seq_vel_editor)))
    {
      if (buttons == GAMEPAD_SELECT && key_left())
      { // go back to song-transpose
        seq.help_text_needs_refresh = true;
        seq.edit_state = true;
        seq.quicknav_pattern_to_song_jump = true;
        seq.quicknav_song_to_pattern_jump = false;
        gamepad_millis = 0;
        LCDML.OTHER_jumpToFunc(UI_func_song);
      }
    }
    else if (buttons == GAMEPAD_SELECT && key_down() && LCDML.FUNC_getID() == LCDML.OTHER_getIDFromFunction(UI_func_seq_settings))
    {
      LCDML.OTHER_jumpToFunc(UI_func_song); // go back from seq.settings to song
    }
    else if (LCDML.FUNC_getID() != LCDML.OTHER_getIDFromFunction(UI_func_map_gamepad))
    {
      bool reverse_y = false;
      bool xy_navigation = false;

      if (seq.edit_state || generic_active_function != 0) // is in edit state
        if (LCDML.FUNC_getID() != LCDML.OTHER_getIDFromFunction(UI_func_song))
          reverse_y = true;

      if (LCDML.FUNC_getID() == LCDML.OTHER_getIDFromFunction(UI_func_seq_vel_editor) || LCDML.FUNC_getID() == LCDML.OTHER_getIDFromFunction(UI_func_seq_pattern_editor))
      {
        if (seq.active_function != 99 || seq.quicknav_song_to_pattern_jump)
          reverse_y = true;
        if (seq.active_function == 99)
          reverse_y = false;
      }

      // if ((LCDML.FUNC_getID() > 5 && LCDML.FUNC_getID() < 9) || (LCDML.FUNC_getID() > 17 && LCDML.FUNC_getID() < 25) || (LCDML.FUNC_getID() > 33 && LCDML.FUNC_getID() < 41))  //"2-line menus", reverse y
      //   reverse_y = true;

      // some pages do x/y navigation using ENC[R]for y and ENC[L] for x movement - depending on that and the edit state of the field, this needs special handling for gamepad usage
      if (LCDML.FUNC_getID() == LCDML.OTHER_getIDFromFunction(UI_func_song) || LCDML.FUNC_getID() == LCDML.OTHER_getIDFromFunction(UI_func_MultiSamplePlay) || LCDML.FUNC_getID() == LCDML.OTHER_getIDFromFunction(UI_func_seq_tracker))
      {

        if (seq.edit_state)
        {
          ;
        }
        else
        {
          xy_navigation = true;
        }
      }

      if (LCDML.FUNC_getID() == LCDML.OTHER_getIDFromFunction(UI_func_song))
      {
        if (seq.edit_state == false && seq.cycle_touch_element == 0)
        {
          reverse_y = false;
        }
        else if (seq.edit_state && seq.cycle_touch_element == 5)
        {
          reverse_y = true;
        }
        else if (seq.edit_state && seq.cycle_touch_element == 6)
        {
          reverse_y = false;
        }
        else if (seq.edit_state && seq.cycle_touch_element == 7)
        {
          reverse_y = true;
        }
        else if (seq.edit_state && seq.cycle_touch_element == 8)
        {
          reverse_y = false;
        }
        else if (seq.edit_state && seq.cycle_touch_element == 9)
        {
          reverse_y = true;
        }
      }

      // start gamepad cases

      if (buttons == gamepad_buttons_neutral)
      {
        if (key_up())
        {
          if (reverse_y)
            g_LCDML_CONTROL_Encoder_position[ENC_R] = -4;
          else
            g_LCDML_CONTROL_Encoder_position[ENC_R] = 4;
          if (gamepad_last_dir == 1)
            gamepad_accelerate = 1 + gamepad_accelerate * 1.4;
          else
            gamepad_accelerate = 0;
          gamepad_millis = 0;
          gamepad_last_dir = 1;
        }
        else if (key_down())
        {
          if (reverse_y)
            g_LCDML_CONTROL_Encoder_position[ENC_R] = 4;
          else
            g_LCDML_CONTROL_Encoder_position[ENC_R] = -4;
          if (gamepad_last_dir == 2)
            gamepad_accelerate = 1 + gamepad_accelerate * 1.4;
          else
            gamepad_accelerate = 0;
          gamepad_millis = 0;
          gamepad_last_dir = 2;
        }
        else if (key_right())
        {
          if (xy_navigation)
            g_LCDML_CONTROL_Encoder_position[ENC_L] = -4;
          else
            g_LCDML_CONTROL_Encoder_position[ENC_R] = -4;
          gamepad_accelerate = 0;
          gamepad_millis = 0;
        }
        else if (key_left())
        {
          if (xy_navigation)
            g_LCDML_CONTROL_Encoder_position[ENC_L] = 4;
          else
            g_LCDML_CONTROL_Encoder_position[ENC_R] = 4;
          gamepad_millis = 0;
          gamepad_accelerate = 0;
        }
        else
          gamepad_accelerate = 0;
      }

      // end gamepad cases
    }
    if (gamepad_accelerate > configuration.sys.gamepad_speed / 1.1)
      gamepad_accelerate = configuration.sys.gamepad_speed / 1.1;

    // GAMEPAD BUTTON HANDLING

    if (LCDML.FUNC_getID() != LCDML.OTHER_getIDFromFunction(UI_func_map_gamepad))
    {
      if (buttons == GAMEPAD_BUTTON_B)
      {
        button[ENC_L] = 0;
        gamepad_accelerate = 0;
      }
      else if (buttons == GAMEPAD_BUTTON_A)
      {
        button[ENC_R] = 0;
        gamepad_accelerate = 0;
      }
      else if (buttons == GAMEPAD_START && gamepad_millis >= configuration.sys.gamepad_speed * 3)
      {
        gamepad_millis = 0;
        gamepad_accelerate = 0;

        if (!seq.running)
          handleStart();
        else
          handleStop();
      }
    }
  }

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
      LOG.println(F("ENC-R left"));
#endif
      encoderDir[ENC_R].Left(true);
      g_LCDML_CONTROL_button_prev[ENC_R] = LOW;
      g_LCDML_CONTROL_button_press_time[ENC_R] = -1;
    }
    else
    {
#ifdef DEBUG
      LOG.println(F("ENC-R down"));
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
      LOG.println(F("ENC-R right"));
#endif
      encoderDir[ENC_R].Right(true);
      LCDML.BT_right();
      g_LCDML_CONTROL_button_prev[ENC_R] = LOW;
      g_LCDML_CONTROL_button_press_time[ENC_R] = -1;
    }
    else
    {
#ifdef DEBUG
      LOG.println(F("ENC-R up"));
#endif
      encoderDir[ENC_R].Up(true);
      LCDML.BT_up();
    }
    ENCODER[ENC_R].write(g_LCDML_CONTROL_Encoder_position[ENC_R] - 4);
  }
  else
  {
    if (!button[ENC_R] && g_LCDML_CONTROL_button_prev[ENC_R]) // falling edge, button pressed
    {
      encoderDir[ENC_R].ButtonPressed(true);
      g_LCDML_CONTROL_button_prev[ENC_R] = LOW;
      g_LCDML_CONTROL_button_press_time[ENC_R] = millis();
    }
    else if (button[ENC_R] && !g_LCDML_CONTROL_button_prev[ENC_R]) // rising edge, button not active
    {
      encoderDir[ENC_R].ButtonPressed(false);
      g_LCDML_CONTROL_button_prev[ENC_R] = HIGH;

      if (g_LCDML_CONTROL_button_press_time[ENC_R] < 0)
      {
        g_LCDML_CONTROL_button_press_time[ENC_R] = millis();
        // Reset for left right action
      }
      else if ((millis() - g_LCDML_CONTROL_button_press_time[ENC_R]) >= LONG_BUTTON_PRESS)
      {
#ifdef DEBUG
        LOG.println(F("ENC-R long released"));
#endif
        // LCDML.BT_quit();
        encoderDir[ENC_R].ButtonLong(false);
      }
      else if ((millis() - g_LCDML_CONTROL_button_press_time[ENC_R]) >= BUT_DEBOUNCE_MS)
      {
#ifdef DEBUG
        LOG.println(F("ENC-R short"));
#endif
        encoderDir[ENC_R].ButtonShort(true);

        LCDML.BT_enter();
      }
    }
  }
  if (encoderDir[ENC_R].ButtonPressed() == true && (millis() - g_LCDML_CONTROL_button_press_time[ENC_R]) >= LONG_BUTTON_PRESS)
  {
#ifdef DEBUG
    LOG.println(F("ENC-R long recognized"));
#endif
    encoderDir[ENC_R].ButtonLong(true);
    if (ui.handlesButtonLong() || LCDML.FUNC_getID() == LCDML.OTHER_getIDFromFunction(UI_func_voice_select) || LCDML.FUNC_getID() == LCDML.OTHER_getIDFromFunction(UI_func_microsynth) || (LCDML.FUNC_getID() == LCDML.OTHER_getIDFromFunction(UI_func_custom_mappings) && generic_temp_select_menu == 1))
    { // handle long press ENC_R
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
      LOG.println(F("ENC-L left"));
#endif
      encoderDir[ENC_L].Left(true);
      LCDML.BT_left();
      g_LCDML_CONTROL_button_prev[ENC_L] = LOW;
      g_LCDML_CONTROL_button_press_time[ENC_L] = -1;
    }
    else
    {
#ifdef DEBUG
      LOG.println(F("ENC-L down"));
#endif
      encoderDir[ENC_L].Down(true);
      LCDML.BT_down();
      if (LCDML.FUNC_getID() != LCDML.OTHER_getIDFromFunction(UI_func_volume))
      {
        // special case : if is in tracker/song edit, left ENC controls x axis, right ENC controls y axis
        if (LCDML.FUNC_getID() == LCDML.OTHER_getIDFromFunction(UI_func_song) && seq.tracktype_or_instrument_assign == 0)
        {

          if (seq.cycle_touch_element == 8) // is in song transpose column - junp to pattern editor
          {
            seq.quicknav_song_to_pattern_jump = true;
            LCDML.OTHER_jumpToFunc(UI_func_seq_pattern_editor);
          }
          else
            if (seq.cycle_touch_element < 5 || seq.cycle_touch_element > 9) // is not in song/chain edit
            {
              if (seq.loop_edit_step < 2) // not in loop edit mode
              {
                seq.previous_track = seq.selected_track;
                seq.cursor_scroll_previous = seq.cursor_scroll;
                seq.selected_track++;
                if (seq.loop_edit_step == 1)
                {
                  seq.loop_edit_step = 0;
                  seq.selected_track = 0;
                  print_song_loop_arrows();
                }
                if (seq.selected_track > NUM_SEQ_TRACKS - 1)
                {
                  seq.loop_edit_step = 1;
                  seq.selected_track = 0;
                }
              }
            }
            else if (seq.cycle_touch_element == 5) // is in song edit
            {
              seq.cycle_touch_element = 6; // switch to chain edit
            }
            else if (seq.cycle_touch_element == 6) // is in chain edit
            {
              seq.cycle_touch_element = 8; // switch to transpose edit
            }
          seq.help_text_needs_refresh = true;


        }
        else if ((LCDML.FUNC_getID() == LCDML.OTHER_getIDFromFunction(UI_func_song) && seq.tracktype_or_instrument_assign == 1) || (LCDML.FUNC_getID() == LCDML.OTHER_getIDFromFunction(UI_func_song) && seq.tracktype_or_instrument_assign == 5) ||
          // select track for instrument or select track for tracktype change
          (LCDML.FUNC_getID() == LCDML.OTHER_getIDFromFunction(UI_func_seq_tracker) && seq.edit_state == false) || (LCDML.FUNC_getID() == LCDML.OTHER_getIDFromFunction(UI_func_MultiSamplePlay) && seq.edit_state == false))

        {

          if (LCDML.FUNC_getID() == LCDML.OTHER_getIDFromFunction(UI_func_MultiSamplePlay) && seq.edit_state == false)
          {
            if (seq.selected_track == 8)
              seq.selected_track = 0;
            else
            {

              seq.selected_track++;
            }
          }
          else
          {
            if (seq.selected_track == NUM_SEQ_TRACKS - 1)
            {

              seq.selected_track = 0;
            }
            else
            {

              seq.selected_track++;
            }
          }
        }
        else if (LCDML.FUNC_getID() == LCDML.OTHER_getIDFromFunction(UI_func_seq_tracker) && seq.edit_state)
        {
          if (seq.vel[seq.current_pattern[seq.selected_track]][seq.scrollpos] < 253)
            seq.vel[seq.current_pattern[seq.selected_track]][seq.scrollpos]++;
        }
        else if (LCDML.FUNC_getID() == LCDML.OTHER_getIDFromFunction(UI_func_song) && seq.tracktype_or_instrument_assign != 0)
        { // do nothing
          ;
        }
        else if (ui.handlesLeftEncoder())
        { // do nothing
          ;
        }

        else if (LCDML.FUNC_getID() == LCDML.OTHER_getIDFromFunction(UI_func_seq_pattern_editor))
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
        else if (LCDML.FUNC_getID() == LCDML.OTHER_getIDFromFunction(UI_func_seq_vel_editor))
        {
          prev_menu_item = seq.menu;
          seq.menu = constrain(seq.menu + 1, 0, 17);
        }
        else if (LCDML.FUNC_getID() != LCDML.OTHER_getIDFromFunction(mFunc_screensaver))
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
      LOG.println(F("ENC-L right"));
#endif
      encoderDir[ENC_L].Right(true);
      LCDML.BT_right();
      g_LCDML_CONTROL_button_prev[ENC_L] = LOW;
      g_LCDML_CONTROL_button_press_time[ENC_L] = -1;
    }
    else
    {
#ifdef DEBUG
      LOG.println(F("ENC-L up"));
#endif
      encoderDir[ENC_L].Up(true);
      LCDML.BT_up();
      if (LCDML.FUNC_getID() != LCDML.OTHER_getIDFromFunction(UI_func_volume))
      {
        // special case : if is in tracker edit, left ENC controls x axis, right ENC controls y axis
        if (LCDML.FUNC_getID() == LCDML.OTHER_getIDFromFunction(UI_func_song) && seq.tracktype_or_instrument_assign == 0)
        {
          if (seq.cycle_touch_element < 5 || seq.cycle_touch_element > 9) // is not in song/chain edit
          {
            if (seq.loop_edit_step < 2) // not in loop edit mode
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
                print_song_loop_arrows();
              }
              else
              {
                seq.previous_track = seq.selected_track;
                seq.cursor_scroll_previous = seq.cursor_scroll;
                seq.selected_track--;
              }
            }
          }
          else if (seq.cycle_touch_element == 8) // is in transpose edit
          {
            seq.cycle_touch_element = 6; // switch to chain edit
          }
          else if (seq.cycle_touch_element == 6) // is in chain edit
          {
            seq.cycle_touch_element = 5; // switch to song edit
          }
          seq.help_text_needs_refresh = true;
        }
        else if ((LCDML.FUNC_getID() == LCDML.OTHER_getIDFromFunction(UI_func_song) && seq.tracktype_or_instrument_assign == 1) || (LCDML.FUNC_getID() == LCDML.OTHER_getIDFromFunction(UI_func_song) && seq.tracktype_or_instrument_assign == 5) ||
          // select track for instrument or select track for tracktype change
          (LCDML.FUNC_getID() == LCDML.OTHER_getIDFromFunction(UI_func_seq_tracker) && seq.edit_state == false) || (LCDML.FUNC_getID() == LCDML.OTHER_getIDFromFunction(UI_func_MultiSamplePlay) && seq.edit_state == false))
        {

          if (seq.selected_track == 0)
          {
            if (LCDML.FUNC_getID() == LCDML.OTHER_getIDFromFunction(UI_func_MultiSamplePlay) && seq.edit_state == false)
              seq.selected_track = 8;
            else
              seq.selected_track = NUM_SEQ_TRACKS - 1;
          }
          else
            seq.selected_track--;
        }
        else if (LCDML.FUNC_getID() == LCDML.OTHER_getIDFromFunction(UI_func_seq_tracker) && seq.edit_state)
        {
          if (seq.vel[seq.current_pattern[seq.selected_track]][seq.scrollpos] > 0)
            seq.vel[seq.current_pattern[seq.selected_track]][seq.scrollpos]--;
        }
        else if (LCDML.FUNC_getID() == LCDML.OTHER_getIDFromFunction(UI_func_song) && seq.tracktype_or_instrument_assign != 0)
        { // do nothing
          ;
        }
        else if (ui.handlesLeftEncoder())
        { // do nothing
          ;
        }

        else if (LCDML.FUNC_getID() == LCDML.OTHER_getIDFromFunction(UI_func_seq_pattern_editor))
        {
          if (seq.menu == 0 && seq.quicknav_song_to_pattern_jump == true)
          {
            // go back to song-transpose when previously navigated in from song edit
            seq.help_text_needs_refresh = true;
            seq.edit_state = true;
            seq.quicknav_pattern_to_song_jump = true;
            seq.quicknav_song_to_pattern_jump = false;
            LCDML.OTHER_jumpToFunc(UI_func_song);
          }
          else
            seq.menu = constrain(seq.menu - 1, 0, 18);

        }
        else if (LCDML.FUNC_getID() == LCDML.OTHER_getIDFromFunction(UI_func_seq_vel_editor))
        {
          if (seq.menu == 0)
          {
            seq.menu_status = 2;
            LCDML.OTHER_jumpToFunc(UI_func_seq_pattern_editor);
          }
          else
          {
            seq.menu_status = 0;
            seq.menu = constrain(seq.menu - 1, 0, 17);
          }

        }
        else if (LCDML.FUNC_getID() != LCDML.OTHER_getIDFromFunction(mFunc_screensaver))
          LCDML.OTHER_jumpToFunc(UI_func_volume);
      }
    }
    ENCODER[ENC_L].write(g_LCDML_CONTROL_Encoder_position[ENC_L] - 4);
  }
  else
  {
    if (!button[ENC_L] && g_LCDML_CONTROL_button_prev[ENC_L]) // falling edge, button pressed
    {
      encoderDir[ENC_L].ButtonPressed(true);
      g_LCDML_CONTROL_button_prev[ENC_L] = LOW;
      g_LCDML_CONTROL_button_press_time[ENC_L] = millis();
    }
    else if (button[ENC_L] && !g_LCDML_CONTROL_button_prev[ENC_L]) // rising edge, button not active
    {
      encoderDir[ENC_L].ButtonPressed(false);
      g_LCDML_CONTROL_button_prev[ENC_L] = HIGH;

      if (g_LCDML_CONTROL_button_press_time[ENC_L] < 0)
      {
        g_LCDML_CONTROL_button_press_time[ENC_L] = millis();
        // Reset for left right action
      }
      else if ((millis() - g_LCDML_CONTROL_button_press_time[ENC_L]) >= LONG_BUTTON_PRESS)
      {
#ifdef DEBUG
        LOG.println(F("ENC-L long released"));
#endif
        // encoderDir[ENC_L].ButtonLong(true);
        // LCDML.BT_quit();
      }
      else if ((millis() - g_LCDML_CONTROL_button_press_time[ENC_L]) >= BUT_DEBOUNCE_MS)
      {
        // LCDML.BT_enter();
#ifdef DEBUG
        LOG.println(F("ENC-L short"));
#endif
        encoderDir[ENC_L].ButtonShort(true);

        // damster@09.03.2024: commented out as this bugs initial page back behavior 
        //        if ((LCDML.MENU_getLastActiveFunctionID() == 0xff && LCDML.MENU_getLastCursorPositionID() == 0) || menu_init == true)
        //        {
        //          LCDML.MENU_goRoot();
        //          menu_init = false;
        //        }
        //        else if (LCDML.FUNC_getID() == LCDML.OTHER_getIDFromFunction(UI_func_volume))
        if (LCDML.FUNC_getID() == LCDML.OTHER_getIDFromFunction(UI_func_volume))
        {
          encoderDir[ENC_L].reset();
          encoderDir[ENC_R].reset();

          if (LCDML.MENU_getLastActiveFunctionID() < 0xff)
            LCDML.OTHER_jumpToID(LCDML.MENU_getLastActiveFunctionID());
          else
            LCDML.OTHER_setCursorToID(LCDML.MENU_getLastCursorPositionID());
        }
        else {
          LCDML.BT_quit();
          LCDML.SCREEN_resetTimer(); // reset timer on exiting screensaver through back key
        }
      }
    }
  }

  if (encoderDir[ENC_L].ButtonPressed() == true && (millis() - g_LCDML_CONTROL_button_press_time[ENC_L]) >= LONG_BUTTON_PRESS)
  {
#ifdef DEBUG
    LOG.println(F("ENC-L long recognized"));
#endif

    //    // when in Voice select Menu, long left-press sets/unsets favorite
    //    if (LCDML.FUNC_getID() == LCDML.OTHER_getIDFromFunction(UI_func_voice_select))
    //    save_favorite(configuration.dexed[selected_instance_id].bank, configuration.dexed[selected_instance_id].voice, selected_instance_id);
    //    // when not in Voice select Menu, long left-press starts/stops sequencer
    //    else if (LCDML.FUNC_getID() != LCDML.OTHER_getIDFromFunction(UI_func_voice_select))
    //    toggle_sequencer_play_status();

    // long left-press starts/stops sequencer

    if (LCDML.FUNC_getID() == LCDML.OTHER_getIDFromFunction(UI_func_file_manager))
    { // when in filemanager, long push ENC-L goes up one directory
      sd_go_parent_folder();
      sd_update_display();
    }
    else
      toggle_sequencer_play_status();

    if (LCDML.FUNC_getID() == LCDML.OTHER_getIDFromFunction(UI_func_voice_select))
      print_voice_select_default_help();

    // for (uint8_t i = 0; i < NUM_DEXED; i++)
    //   MicroDexed[i]->panic();

    encoderDir[ENC_L].reset();
    encoderDir[ENC_R].reset();
  }
}

/***********************************************************************
   MENU DISPLAY
 ***********************************************************************/
FLASHMEM void lcdml_menu_clear(void)
{
  if (seq.menu_status == 0)
    border1_clear();
  ts.touch_ui_drawn_in_menu = false;
  back_button_touch_page_check_and_init_done = false;
}

FLASHMEM void lcdml_menu_display(void)
{
  // update content
  // ***************
  if (LCDML.DISP_checkMenuUpdate())
  {
    // declaration of some variables
    // ***************

    // content variable
    char content_text[_LCDML_DISP_cols]; // save the content text of current menu element
    // menu element object
    LCDMenuLib2_menu* tmp;
    // some limit values
    uint8_t i = LCDML.MENU_getScroll();
    uint8_t maxi = _LCDML_DISP_rows + i;
    uint8_t n = 0;

    // check if this element has children
    if ((tmp = LCDML.MENU_getDisplayedObj()) != NULL)
    {
      // Display a header with the parent element name
      display.setTextSize(1);
      display.setTextColor(GREY1, COLOR_BACKGROUND);

      if (LCDML.MENU_getLayer() == 0)
      {
        // this is displayed when no header is available
       // x_pos_menu_header_layer[LCDML.MENU_getLayer() + 1] = CHAR_width + CHAR_width_small*4;
        x_pos_menu_header_layer[LCDML.MENU_getLayer() + 1] = CHAR_width;
        last_menu_depth = LCDML.MENU_getLayer();
        display.console = true;
        display.fillRect(x_pos_menu_header_layer[LCDML.MENU_getLayer()] + CHAR_width, 7, 90, 7, COLOR_BACKGROUND); //clears longest text of submenus when going back
        display.console = false;
        //if (ts.keyb_in_menu_activated == false)
        //{
          //display.setCursor(0, DISPLAY_HEIGHT - CHAR_height_small);
          //display.setTextColor(COLOR_SYSTEXT, COLOR_BACKGROUND);
          //display.print(back_clear);
        //}
      }
      else if (LCDML.MENU_getLayer() > last_menu_depth)
      {
        display.setCursor(x_pos_menu_header_layer[LCDML.MENU_getLayer()], 7);
        display.setTextColor(RED, COLOR_BACKGROUND);
        display.print(">");
        display.setTextColor(GREY1, COLOR_BACKGROUND);
        LCDML_getContent(content_text, LCDML.MENU_getParentID());
        show_uppercase_no_grid(7, display.getCursorX(), strlen(content_text), content_text);
        // x_pos_menu_header_layer[LCDML.MENU_getLayer() + 1] = display.getCursorX();
       // if (ts.keyb_in_menu_activated == false)
        //  helptext_l(back_text);//xxxyyy
      }
      back_touchbutton();
      display.setTextSize(2);
      seq.edit_state = false;
      generic_active_function = 0;
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
              display.setTextColor(COLOR_SYSTEXT, DX_DARKCYAN);
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

      // clear menu lines if menu content < display lines
      if (i < _LCDML_DISP_rows)
      {
        display.console = true;
        display.fillRect(CHAR_width, CHAR_height * (i + 1) - 1, CHAR_width * (_LCDML_DISP_cols - 2), CHAR_height * (_LCDML_DISP_rows - i), COLOR_BACKGROUND);
      }
    }
  }
  if (LCDML.DISP_checkMenuCursorUpdate())
  {
    drawScrollbar((_LCDML_DISP_cols - 2) * CHAR_width + CHAR_width_small, CHAR_height, 6, LCDML.MENU_getChilds(), LCDML.MENU_getCursorPosAbs(), CHAR_height);
  }
}

FLASHMEM void draw_instance_editor(Editor* editor, bool refresh)
{
  display.setTextColor(COLOR_SYSTEXT, COLOR_BACKGROUND);
  display.setTextSize(1);

  if (!refresh)
  {
    display.setTextSize(1);
    display.setTextColor(COLOR_SYSTEXT, COLOR_BACKGROUND);
    display.setCursor(CHAR_width_small * 25, 6);
    display.print(F("DEXED"));
    display.setTextColor(GREY2, COLOR_BACKGROUND);
    display.setCursor(CHAR_width_small * 25 + 2, DISPLAY_HEIGHT - (CHAR_height_small * 2) - 2);
    display.print(F("SHORT:"));
    display.setTextColor(COLOR_ARP, COLOR_BACKGROUND);
    if (generic_active_function == 1)
      display.print(F("APPLY"));
    else if (generic_active_function == 0)
      display.print(F(" EDIT"));
    display.setTextColor(GREY2, COLOR_BACKGROUND);
    display.print(F(" LONG:"));
    display.setTextColor(COLOR_ARP, COLOR_BACKGROUND);
    display.print(F("TOGGLE INST"));

  }
  display.setCursor(CHAR_width_small * 10, 6);
  if (generic_temp_select_menu == editor->select_id)
  {
    setModeColor(editor->select_id);
    helptext_r("< > SELECT INSTANCE");
    display.setTextSize(1);
    if (seq.edit_state == 0)
      display.setTextColor(COLOR_BACKGROUND, COLOR_SYSTEXT);
    else
      display.setTextColor(COLOR_SYSTEXT, RED);
    display.setCursor(CHAR_width_small * 25, 6);
    display.print(F("DEXED"));
  }
  else
  {
    display.setTextSize(1);
    display.setTextColor(COLOR_SYSTEXT, COLOR_BACKGROUND);
    display.setCursor(CHAR_width_small * 25, 6);
    display.print(F("DEXED"));
    helptext_r("< > EDIT PARAM.");
  }
  UI_update_instance_icons();
}

FLASHMEM void addInstanceEditor(
  void (*renderer)(Editor* param, bool refresh) = &draw_instance_editor)
{
  ui.addEditor(
    "INSTANCE", 0, 1, &selected_instance_id, NULL,
    [](Editor* editor, int16_t value) -> void
    {
      selected_instance_id = value;
      ui.draw_editors(true);
    },
    renderer);
  ui.enableButtonLongEditor();
}

FLASHMEM void UI_func_map_gamepad(uint8_t param)
{
  if (LCDML.FUNC_setup()) // ****** SETUP *********
  {
    encoderDir[ENC_R].reset();
    seq.temp_active_menu = 0;
    temp_int = 0;
    display.fillScreen(COLOR_BACKGROUND);
    helptext_l(back_text);
    helptext_r("RESTART");
    display.setTextSize(1);
    setCursor_textGrid_small(1, 1);
    display.setTextColor(RED);
    display.print(F("AUTO-MAP GAMEPAD KEYS"));
    display.setTextColor(GREY2);
    setCursor_textGrid_small(1, 3);
    display.print(F("PLEASE PUSH EACH BUTTON ONCE"));
    setCursor_textGrid_small(1, 4);
    display.print(F("IN THE LISTED ORDER"));

    setCursor_textGrid_small(1, 6);
    display.print(F("UP"));
    setCursor_textGrid_small(1, 7);
    display.print(F("DOWN"));
    setCursor_textGrid_small(1, 8);
    display.print(F("LEFT"));
    setCursor_textGrid_small(1, 9);
    display.print(F("RIGHT"));
    setCursor_textGrid_small(1, 11);
    display.print(F("BUTTON A"));
    setCursor_textGrid_small(1, 12);
    display.print(F("BUTTON B"));
    setCursor_textGrid_small(1, 14);
    display.print(F("SELECT"));
    setCursor_textGrid_small(1, 15);
    display.print(F("START"));
    setCursor_textGrid_small(35, 5);
    display.print(F("NEUTRAL STATE:"));
    setCursor_textGrid_small(35, 6);
    display.print(F("BUTTONS"));
    setCursor_textGrid_small(35, 7);
    display.print(F("Value 0"));
    setCursor_textGrid_small(35, 8);
    display.print(F("Value 1"));
    setCursor_textGrid_small(46, 6);
    print_formatted_number(gamepad_buttons_neutral, 3);
    setCursor_textGrid_small(46, 7);
    print_formatted_number(gamepad_0_neutral, 3);
    setCursor_textGrid_small(46, 8);
    print_formatted_number(gamepad_1_neutral, 3);
    setCursor_textGrid_small(1, 21);
    display.print(F("YOU CAN RESTART KEY-MAPPING WITH "));
    display.setTextColor(RED);
    display.print(F("ENC[R] "));
    display.setTextColor(GREY2);
    display.print(F("AT ANY TIME."));
  }
  if (LCDML.FUNC_loop()) // ****** LOOP *********
  {
    if ((LCDML.BT_checkDown() && encoderDir[ENC_R].Down()) || (LCDML.BT_checkUp() && encoderDir[ENC_R].Up()) || LCDML.BT_checkEnter())
    {
      if (LCDML.BT_checkDown())
      {
        ;
      }
      else if (LCDML.BT_checkUp())
      {
        ;
      }
      if (LCDML.BT_checkEnter())
      {
        temp_int = 0;
        display.fillRect(0, 170, DISPLAY_WIDTH, 40, COLOR_BACKGROUND);
      }
    }
  }
  if (LCDML.FUNC_close()) // ****** STABLE END *********
  {
    encoderDir[ENC_R].reset();
    display.fillScreen(COLOR_BACKGROUND);
  }
}

FLASHMEM void reverb_roomsize()
{
  if (LCDML.BT_checkDown())
    configuration.fx.reverb_roomsize = constrain(configuration.fx.reverb_roomsize + ENCODER[ENC_R].speed(), REVERB_ROOMSIZE_MIN, REVERB_ROOMSIZE_MAX);
  else if (LCDML.BT_checkUp())
    configuration.fx.reverb_roomsize = constrain(configuration.fx.reverb_roomsize - ENCODER[ENC_R].speed(), REVERB_ROOMSIZE_MIN, REVERB_ROOMSIZE_MAX);
  // display_bar_int("Reverb Room", configuration.fx.reverb_roomsize, 1.0, REVERB_ROOMSIZE_MIN, REVERB_ROOMSIZE_MAX, 3, false, false, false);
  reverb.size(mapfloat(configuration.fx.reverb_roomsize, REVERB_ROOMSIZE_MIN, REVERB_ROOMSIZE_MAX, 0.0, 1.0));
}

FLASHMEM void reverb_lowpass()
{
  if (LCDML.BT_checkDown())
    configuration.fx.reverb_lowpass = constrain(configuration.fx.reverb_lowpass + ENCODER[ENC_R].speed(), REVERB_LOWPASS_MIN, REVERB_LOWPASS_MAX);
  else if (LCDML.BT_checkUp())
    configuration.fx.reverb_lowpass = constrain(configuration.fx.reverb_lowpass - ENCODER[ENC_R].speed(), REVERB_LOWPASS_MIN, REVERB_LOWPASS_MAX);
  // display_bar_int("Reverb Lowpass", configuration.fx.reverb_lowpass, 1.0, REVERB_LOWPASS_MIN, REVERB_LOWPASS_MAX, 3, false, false, false);
  reverb.lowpass(mapfloat(configuration.fx.reverb_lowpass, REVERB_LOWPASS_MIN, REVERB_LOWPASS_MAX, 0.0, 1.0));
}

FLASHMEM void reverb_lodamp()
{
  if (LCDML.BT_checkDown())
    configuration.fx.reverb_lodamp = constrain(configuration.fx.reverb_lodamp + ENCODER[ENC_R].speed(), REVERB_LODAMP_MIN, REVERB_LODAMP_MAX);
  else if (LCDML.BT_checkUp())
    configuration.fx.reverb_lodamp = constrain(configuration.fx.reverb_lodamp - ENCODER[ENC_R].speed(), REVERB_LODAMP_MIN, REVERB_LODAMP_MAX);
  // display_bar_int("Reverb Lodamp.", configuration.fx.reverb_lodamp, 1.0, REVERB_LODAMP_MIN, REVERB_LODAMP_MAX, 3, false, false, false);
  reverb.lodamp(mapfloat(configuration.fx.reverb_lodamp, REVERB_LODAMP_MIN, REVERB_LODAMP_MAX, 0.0, 1.0));
}

FLASHMEM void reverb_hidamp()
{
  if (LCDML.BT_checkDown())
    configuration.fx.reverb_hidamp = constrain(configuration.fx.reverb_hidamp + ENCODER[ENC_R].speed(), REVERB_HIDAMP_MIN, REVERB_HIDAMP_MAX);
  else if (LCDML.BT_checkUp())
    configuration.fx.reverb_hidamp = constrain(configuration.fx.reverb_hidamp - ENCODER[ENC_R].speed(), REVERB_HIDAMP_MIN, REVERB_HIDAMP_MAX);
  // display_bar_int("Reverb Hidamp.", configuration.fx.reverb_hidamp, 1.0, REVERB_HIDAMP_MIN, REVERB_HIDAMP_MAX, 3, false, false, false);
  reverb.hidamp(mapfloat(configuration.fx.reverb_hidamp, REVERB_HIDAMP_MIN, REVERB_HIDAMP_MAX, 0.0, 1.0));
}

FLASHMEM void reverb_diffusion()
{
  if (LCDML.BT_checkDown())
    configuration.fx.reverb_diffusion = constrain(configuration.fx.reverb_diffusion + ENCODER[ENC_R].speed(), REVERB_DIFFUSION_MIN, REVERB_DIFFUSION_MAX);
  else if (LCDML.BT_checkUp())
    configuration.fx.reverb_diffusion = constrain(configuration.fx.reverb_diffusion - ENCODER[ENC_R].speed(), REVERB_DIFFUSION_MIN, REVERB_DIFFUSION_MAX);
  // display_bar_int("Reverb Diff.", configuration.fx.reverb_diffusion, 1.0, REVERB_DIFFUSION_MIN, REVERB_DIFFUSION_MAX, 3, false, false, false);
  reverb.diffusion(mapfloat(configuration.fx.reverb_diffusion, REVERB_DIFFUSION_MIN, REVERB_DIFFUSION_MAX, 0.0, 1.0));
}

FLASHMEM void reverb_level()
{
  if (LCDML.BT_checkDown())
    configuration.fx.reverb_level = constrain(configuration.fx.reverb_level + ENCODER[ENC_R].speed(), REVERB_LEVEL_MIN, REVERB_LEVEL_MAX);
  else if (LCDML.BT_checkUp())
    configuration.fx.reverb_level = constrain(configuration.fx.reverb_level - ENCODER[ENC_R].speed(), REVERB_LEVEL_MIN, REVERB_LEVEL_MAX);
  // display_bar_int("Reverb Level", configuration.fx.reverb_level, 1.0, REVERB_LEVEL_MIN, REVERB_LEVEL_MAX, 3, false, false, true);
  master_mixer_r.gain(MASTER_MIX_CH_REVERB, volume_transform(mapfloat(configuration.fx.reverb_level, REVERB_LEVEL_MIN, REVERB_LEVEL_MAX, 0.0, VOL_MAX_FLOAT)));
  master_mixer_l.gain(MASTER_MIX_CH_REVERB, volume_transform(mapfloat(configuration.fx.reverb_level, REVERB_LEVEL_MIN, REVERB_LEVEL_MAX, 0.0, VOL_MAX_FLOAT)));
}

FLASHMEM void UI_func_chorus_frequency(uint8_t param)
{
  if (LCDML.FUNC_setup()) // ****** SETUP *********
  {
    encoderDir[ENC_R].reset();

    display_bar_float("Chorus Frq.", configuration.fx.chorus_frequency[selected_instance_id], 0.1, CHORUS_FREQUENCY_MIN, CHORUS_FREQUENCY_MAX, 2, 1, false, false, true);
    UI_update_instance_icons();
  }

  if (LCDML.FUNC_loop()) // ****** LOOP *********
  {
    if ((LCDML.BT_checkDown() && encoderDir[ENC_R].Down()) || (LCDML.BT_checkUp() && encoderDir[ENC_R].Up()) || (LCDML.BT_checkEnter() && encoderDir[ENC_R].ButtonShort()))
    {
      if (LCDML.BT_checkDown())
        configuration.fx.chorus_frequency[selected_instance_id] = constrain(configuration.fx.chorus_frequency[selected_instance_id] + ENCODER[ENC_R].speed(), CHORUS_FREQUENCY_MIN, CHORUS_FREQUENCY_MAX);
      else if (LCDML.BT_checkUp())
        configuration.fx.chorus_frequency[selected_instance_id] = constrain(configuration.fx.chorus_frequency[selected_instance_id] - ENCODER[ENC_R].speed(), CHORUS_FREQUENCY_MIN, CHORUS_FREQUENCY_MAX);
      else if (LCDML.BT_checkEnter())
      {
        selected_instance_id = !selected_instance_id;
        UI_update_instance_icons();
      }
    }
    display_bar_float("Chorus Frq.", configuration.fx.chorus_frequency[selected_instance_id], 0.1, CHORUS_FREQUENCY_MIN, CHORUS_FREQUENCY_MAX, 2, 1, false, false, false);

    chorus_modulator[selected_instance_id]->frequency(configuration.fx.chorus_frequency[selected_instance_id] / 10.0);
  }

  if (LCDML.FUNC_close()) // ****** STABLE END *********
  {
    encoderDir[ENC_R].reset();
  }
}

FLASHMEM void UI_func_chorus_waveform(uint8_t param)
{
  if (LCDML.FUNC_setup()) // ****** SETUP *********
  {
    encoderDir[ENC_R].reset();

    setCursor_textGrid(1, 1);
    display.print(F("Chorus Wavefrm"));
    UI_update_instance_icons();
  }

  if (LCDML.FUNC_loop()) // ****** LOOP *********
  {
    if (LCDML.BT_checkDown() && encoderDir[ENC_R].Down())
      configuration.fx.chorus_waveform[selected_instance_id] = constrain(configuration.fx.chorus_waveform[selected_instance_id] + 1, CHORUS_WAVEFORM_MIN, CHORUS_WAVEFORM_MAX);
    else if (LCDML.BT_checkUp() && encoderDir[ENC_R].Up())
      configuration.fx.chorus_waveform[selected_instance_id] = constrain(configuration.fx.chorus_waveform[selected_instance_id] - 1, CHORUS_WAVEFORM_MIN, CHORUS_WAVEFORM_MAX);
    else if (LCDML.BT_checkEnter() && encoderDir[ENC_R].ButtonShort())
    {
      selected_instance_id = !selected_instance_id;
      UI_update_instance_icons();
    }

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

  if (LCDML.FUNC_close()) // ****** STABLE END *********
  {
    encoderDir[ENC_R].reset();
  }
}

FLASHMEM void UI_func_chorus_depth(uint8_t param)
{
  if (LCDML.FUNC_setup()) // ****** SETUP *********
  {
    encoderDir[ENC_R].reset();

    display_bar_int("Chorus Dpt.", configuration.fx.chorus_depth[selected_instance_id], 1.0, CHORUS_DEPTH_MIN, CHORUS_DEPTH_MAX, 3, false, false, true);
    UI_update_instance_icons();
  }

  if (LCDML.FUNC_loop()) // ****** LOOP *********
  {
    if ((LCDML.BT_checkDown() && encoderDir[ENC_R].Down()) || (LCDML.BT_checkUp() && encoderDir[ENC_R].Up()) || (LCDML.BT_checkEnter() && encoderDir[ENC_R].ButtonShort()))
    {
      if (LCDML.BT_checkDown())
        configuration.fx.chorus_depth[selected_instance_id] = constrain(configuration.fx.chorus_depth[selected_instance_id] + ENCODER[ENC_R].speed(), CHORUS_DEPTH_MIN, CHORUS_DEPTH_MAX);
      else if (LCDML.BT_checkUp())
        configuration.fx.chorus_depth[selected_instance_id] = constrain(configuration.fx.chorus_depth[selected_instance_id] - ENCODER[ENC_R].speed(), CHORUS_DEPTH_MIN, CHORUS_DEPTH_MAX);
      else if (LCDML.BT_checkEnter())
      {
        selected_instance_id = !selected_instance_id;

        UI_update_instance_icons();
      }
    }

    display_bar_int("Chorus Dpt.", configuration.fx.chorus_depth[selected_instance_id], 1.0, CHORUS_DEPTH_MIN, CHORUS_DEPTH_MAX, 3, false, false, false);

    chorus_modulator[selected_instance_id]->amplitude(configuration.fx.chorus_depth[selected_instance_id] / 100.0);
  }

  if (LCDML.FUNC_close()) // ****** STABLE END *********
  {
    encoderDir[ENC_R].reset();
  }
}

FLASHMEM void UI_func_chorus_level(uint8_t param)
{
  if (LCDML.FUNC_setup()) // ****** SETUP *********
  {
    encoderDir[ENC_R].reset();

    display_bar_int("Chorus Lvl.", configuration.fx.chorus_level[selected_instance_id], 1.0, CHORUS_LEVEL_MIN, CHORUS_LEVEL_MAX, 3, false, false, true);

    UI_update_instance_icons();
  }

  if (LCDML.FUNC_loop()) // ****** LOOP *********
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
      else if (LCDML.BT_checkEnter())
      {
        selected_instance_id = !selected_instance_id;

        UI_update_instance_icons();
      }
    }

    display_bar_int("Chorus Lvl.", configuration.fx.chorus_level[selected_instance_id], 1.0, CHORUS_LEVEL_MIN, CHORUS_LEVEL_MAX, 3, false, false, false);
    /// global_delay_in_mixer[selected_instance_id]->gain(1, mapfloat(configuration.fx.chorus_level[selected_instance_id], CHORUS_LEVEL_MIN, CHORUS_LEVEL_MAX, 0.0, 0.9));
  }

  if (LCDML.FUNC_close()) // ****** STABLE END *********
  {
    encoderDir[ENC_R].reset();
  }
}

FLASHMEM void master_effects_set_delay_time(uint8_t instance)
{

  if (configuration.fx.delay_sync[instance] > 0)
  {
    set_delay_sync(configuration.fx.delay_sync[instance], instance);
  }

  if (LCDML.BT_checkDown())
  {
    if (configuration.fx.delay_time[instance] == DELAY_TIME_MIN && configuration.fx.delay_sync[instance] > DELAY_SYNC_MIN)
    {
      // MIDI-sync delay
      configuration.fx.delay_sync[instance] = constrain(configuration.fx.delay_sync[instance] - 1, DELAY_SYNC_MIN, DELAY_SYNC_MAX);
    }
    else
    {
      configuration.fx.delay_time[instance] = constrain(configuration.fx.delay_time[instance] + ENCODER[ENC_R].speed(), DELAY_TIME_MIN, DELAY_TIME_MAX);
      MD_sendControlChange(configuration.dexed[instance].midi_channel, 105, configuration.fx.delay_time[instance]);
    }
  }
  else if (LCDML.BT_checkUp())
  {
    if (configuration.fx.delay_time[instance] == DELAY_TIME_MIN && configuration.fx.delay_sync[instance] > DELAY_SYNC_MIN)
    {
      // MIDI-sync delay
      configuration.fx.delay_sync[instance] = constrain(configuration.fx.delay_sync[instance] + 1, DELAY_SYNC_MIN, DELAY_SYNC_MAX);
    }
    else
    {
      if (configuration.fx.delay_time[instance] == DELAY_TIME_MIN)
        configuration.fx.delay_sync[instance] = DELAY_SYNC_MIN + 1;
      else
      {
        configuration.fx.delay_time[instance] = constrain(configuration.fx.delay_time[instance] - ENCODER[ENC_R].speed(), DELAY_TIME_MIN, DELAY_TIME_MAX);
        MD_sendControlChange(configuration.dexed[instance].midi_channel, 105, configuration.fx.delay_time[instance]);
      }
    }
  }
  if (configuration.fx.delay_sync[instance] > 0)
  {
    set_delay_sync(configuration.fx.delay_sync[instance], instance); // MIDI Sync Delay
  }
  else
  {

    if (configuration.fx.delay_time[instance] <= DELAY_TIME_MIN)
      delay_fx[instance]->disable(0);
    else
      delay_fx[instance]->delay(0, constrain(configuration.fx.delay_time[instance], DELAY_TIME_MIN, DELAY_TIME_MAX) * 10);
  }
}

FLASHMEM void master_effects_set_delay_feedback(uint8_t instance)
{
  if (LCDML.BT_checkDown())
  {
    configuration.fx.delay_feedback[instance] = constrain(configuration.fx.delay_feedback[instance] + ENCODER[ENC_R].speed(), DELAY_FEEDBACK_MIN, DELAY_FEEDBACK_MAX);
    MD_sendControlChange(configuration.dexed[instance].midi_channel, 106, configuration.fx.delay_feedback[instance]);
  }
  else if (LCDML.BT_checkUp())
  {
    configuration.fx.delay_feedback[instance] = constrain(configuration.fx.delay_feedback[instance] - ENCODER[ENC_R].speed(), DELAY_FEEDBACK_MIN, DELAY_FEEDBACK_MAX);
    MD_sendControlChange(configuration.dexed[instance].midi_channel, 106, configuration.fx.delay_feedback[instance]);
  }
  delay_fb_mixer[instance]->gain(1, mapfloat(configuration.fx.delay_feedback[instance], DELAY_FEEDBACK_MIN, DELAY_FEEDBACK_MAX, 0.0, 0.8));
}

FLASHMEM void master_effects_delay_level_global(uint8_t instance)
{
  if (LCDML.BT_checkDown())
  {
    configuration.fx.delay_level_global[instance] = constrain(configuration.fx.delay_level_global[instance] + ENCODER[ENC_R].speed(), DELAY_LEVEL_MIN, DELAY_LEVEL_MAX);
  }
  else if (LCDML.BT_checkUp())
  {
    configuration.fx.delay_level_global[instance] = constrain(configuration.fx.delay_level_global[instance] - ENCODER[ENC_R].speed(), DELAY_LEVEL_MIN, DELAY_LEVEL_MAX);
  }
  delay_mixer[instance]->gain(1, mapfloat(configuration.fx.delay_level_global[instance], DELAY_LEVEL_MIN, DELAY_LEVEL_MAX, 0.0, 0.9));
}

FLASHMEM void master_effects_set_reverb_send(uint8_t instance)
{
  if (LCDML.BT_checkDown())
  {
    configuration.fx.reverb_send[instance] = constrain(configuration.fx.reverb_send[instance] + ENCODER[ENC_R].speed(), REVERB_SEND_MIN, REVERB_SEND_MAX);
    MD_sendControlChange(configuration.dexed[instance].midi_channel, 91, configuration.fx.reverb_send[instance]);
  }
  else if (LCDML.BT_checkUp())
  {
    configuration.fx.reverb_send[instance] = constrain(configuration.fx.reverb_send[instance] - ENCODER[ENC_R].speed(), REVERB_SEND_MIN, REVERB_SEND_MAX);
    MD_sendControlChange(configuration.dexed[instance].midi_channel, 91, configuration.fx.reverb_send[instance]);
  }
  // display_bar_int("Reverb Send", configuration.fx.reverb_send[instance], 1.0, REVERB_SEND_MIN, REVERB_SEND_MAX, 3, false, false, false);
  reverb_mixer_r.gain(instance, volume_transform(mapfloat(configuration.fx.reverb_send[instance], REVERB_SEND_MIN, REVERB_SEND_MAX, 0.0, VOL_MAX_FLOAT)));
  reverb_mixer_l.gain(instance, volume_transform(mapfloat(configuration.fx.reverb_send[instance], REVERB_SEND_MIN, REVERB_SEND_MAX, 0.0, VOL_MAX_FLOAT)));
}

FLASHMEM void UI_func_filter_cutoff(uint8_t param)
{
  if (LCDML.FUNC_setup()) // ****** SETUP *********
  {
    encoderDir[ENC_R].reset();

    display_bar_int("Filter Cutoff", configuration.fx.filter_cutoff[selected_instance_id], 1.0, FILTER_CUTOFF_MIN, FILTER_CUTOFF_MAX, 3, false, false, true);

    UI_update_instance_icons();
  }

  if (LCDML.FUNC_loop()) // ****** LOOP *********
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
      else if (LCDML.BT_checkEnter())
      {
        selected_instance_id = !selected_instance_id;

        UI_update_instance_icons();
      }
    }

    display_bar_int("Filter Cutoff", configuration.fx.filter_cutoff[selected_instance_id], 1.0, FILTER_CUTOFF_MIN, FILTER_CUTOFF_MAX, 3, false, false, false);

    MicroDexed[selected_instance_id]->setFilterCutoff(mapfloat(configuration.fx.filter_cutoff[selected_instance_id], FILTER_CUTOFF_MIN, FILTER_CUTOFF_MAX, 1.0, 0.0));
  }

  if (LCDML.FUNC_close()) // ****** STABLE END *********
  {
    encoderDir[ENC_R].reset();
  }
}

FLASHMEM void UI_func_filter_resonance(uint8_t param)
{
  if (LCDML.FUNC_setup()) // ****** SETUP *********
  {
    encoderDir[ENC_R].reset();

    display_bar_int("Filter Reso.", configuration.fx.filter_resonance[selected_instance_id], 1.0, FILTER_RESONANCE_MIN, FILTER_RESONANCE_MAX, 3, false, false, true);

    UI_update_instance_icons();
  }

  if (LCDML.FUNC_loop()) // ****** LOOP *********
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
      else if (LCDML.BT_checkEnter())
      {
        selected_instance_id = !selected_instance_id;

        UI_update_instance_icons();
      }
    }

    display_bar_int("Filter Reso.", configuration.fx.filter_resonance[selected_instance_id], 1.0, FILTER_RESONANCE_MIN, FILTER_RESONANCE_MAX, 3, false, false, false);

    MicroDexed[selected_instance_id]->setFilterResonance(mapfloat(configuration.fx.filter_resonance[selected_instance_id], FILTER_RESONANCE_MIN, FILTER_RESONANCE_MAX, 1.0, 0.0));
  }

  if (LCDML.FUNC_close()) // ****** STABLE END *********
  {
    encoderDir[ENC_R].reset();
  }
}

FLASHMEM void getNoteName(char* noteName, uint8_t noteNumber)
{
  char notes[12][3] = { "A", "A#", "B", "C", "C#", "D", "D#", "E", "F", "F#", "G", "G#" };
  uint8_t oct_index = noteNumber - 12;
  if (noteNumber == 130) // it is a latched note
  {
    noteName[0] = 'L';
    noteName[1] = '\0';
  }
  else
  {
    noteNumber -= 21;
    if (notes[noteNumber % 12][1] == '\0')
      snprintf_P(noteName, sizeof(noteName), PSTR("%1s-%1d"), notes[noteNumber % 12], oct_index / 12);
    else
      snprintf_P(noteName, sizeof(noteName), PSTR("%2s%1d"), notes[noteNumber % 12], oct_index / 12);
  }
}

FLASHMEM void UI_func_sound_intensity(uint8_t param)
{
  if (LCDML.FUNC_setup()) // ****** SETUP *********
  {
    encoderDir[ENC_R].reset();

    display_bar_int("Voice Level", configuration.dexed[selected_instance_id].sound_intensity, 1.0, SOUND_INTENSITY_MIN, SOUND_INTENSITY_MAX, 3, false, false, true);

    UI_update_instance_icons();
  }

  if (LCDML.FUNC_loop()) // ****** LOOP *********
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

      else if (LCDML.BT_checkEnter())
      {
        selected_instance_id = !selected_instance_id;

        UI_update_instance_icons();
      }
    }

    display_bar_int("Voice Level", configuration.dexed[selected_instance_id].sound_intensity, 1.0, SOUND_INTENSITY_MIN, SOUND_INTENSITY_MAX, 3, false, false, false);
    MicroDexed[selected_instance_id]->setGain(midi_volume_transform(map(configuration.dexed[selected_instance_id].sound_intensity, SOUND_INTENSITY_MIN, SOUND_INTENSITY_MAX, 0, 127)));
  }

  if (LCDML.FUNC_close()) // ****** STABLE END *********
  {
    encoderDir[ENC_R].reset();
  }
}

FLASHMEM void master_effects_set_delay_panorama(uint8_t instance)
{
  if (LCDML.BT_checkDown())
  {
    configuration.fx.delay_pan[instance] = constrain(configuration.fx.delay_pan[instance] + ENCODER[ENC_R].speed(), PANORAMA_MIN, PANORAMA_MAX);
  }
  else if (LCDML.BT_checkUp())
  {
    configuration.fx.delay_pan[instance] = constrain(configuration.fx.delay_pan[instance] - ENCODER[ENC_R].speed(), PANORAMA_MIN, PANORAMA_MAX);
  }
  delay_mono2stereo[instance]->panorama(mapfloat(configuration.fx.delay_pan[instance], PANORAMA_MIN, PANORAMA_MAX, -1.0, 1.0));
}

FLASHMEM void UI_func_favorites(uint8_t param)
{
  static uint8_t old_favorites;

  if (LCDML.FUNC_setup()) // ****** SETUP *********
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

  if (LCDML.FUNC_loop()) // ****** LOOP *********
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

  if (LCDML.FUNC_close()) // ****** STABLE END *********
  {
    encoderDir[ENC_L].reset();
    if (old_favorites != configuration.sys.favorites)
    {
      save_sys_flag = true;
      save_sys = 0;
    }
  }
}

FLASHMEM void UI_func_stereo_mono(uint8_t param)
{
  static uint8_t old_mono;

  if (LCDML.FUNC_setup()) // ****** SETUP *********
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

  if (LCDML.FUNC_loop()) // ****** LOOP *********
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

  if (LCDML.FUNC_close()) // ****** STABLE END *********
  {
    encoderDir[ENC_L].reset();

    if (old_mono != configuration.sys.mono)
    {
      save_sys_flag = true;
      save_sys = 0;
    }
  }
}

// get and set dexed configuration to our dexed configuration structure
int16_t dexed_current_instance_getter(Editor* editor)
{
  // the controller parameter may be from either instance, which may be
  // switched at any time. So recompute the value pointer in respect of the instance!
  uint8_t* ptr = (uint8_t*)((char*)editor->value - (char*)&configuration.dexed[0] + (char*)&configuration.dexed[selected_instance_id]);
  return *ptr;
}
void dexed_current_instance_setter(Editor* editor, int16_t value)
{
  // the controller parameter may be from either instance, which may be
  // switched at any time. So recompute the value pointer in respect of the instance!
  uint8_t* ptr = (uint8_t*)((char*)editor->value - (char*)&configuration.dexed[0] + (char*)&configuration.dexed[selected_instance_id]);
  *ptr = (uint8_t)value;
}

// prepare rendering for an editor field providing multiple options to select between
void prepare_multi_options(Editor* editor, bool refresh)
{
  display.setTextSize(1);
  if (!refresh)
  {
    setCursor_textGrid_small(editor->x + 10, editor->y);
    display.setTextColor(GREY2, COLOR_BACKGROUND);
    display.print(editor->name);
  }

  setModeColor(editor->select_id);
  setCursor_textGrid_small(editor->x, editor->y);
  ui.setCursor(editor->x, editor->y);
}

// set portamento setup to dexed engine and send SysEx for it.
void dexed_portamento_setter(Editor* editor, int16_t value)
{
  dexed_current_instance_setter(editor, value);
  dexed_t& dexed = configuration.dexed[selected_instance_id];
  MicroDexed[selected_instance_id]->setPortamento(dexed.portamento_mode, dexed.portamento_glissando, dexed.portamento_time);
  send_sysex_param(dexed.midi_channel, 67, dexed.portamento_mode, 2);
  send_sysex_param(dexed.midi_channel, 68, dexed.portamento_glissando, 2);
  send_sysex_param(dexed.midi_channel, 69, dexed.portamento_time, 2);
};

// display the voice name for the currently selected instance
// used by mutliple UI pages.
void dexed_voice_name_renderer(Editor* param, bool refresh)
{
  draw_instance_editor(param, refresh);
  display.setTextSize(2);
  show(1, 1, 10, g_voice_name[selected_instance_id]);
}

int16_t fx_current_instance_getter(struct Editor* editor)
{
  // the parameter may be from either instance, which may be
  // switched at any time. So recompute the value pointer in respect of the instance!
  uint8_t* ptr = &((uint8_t*)editor->value)[selected_instance_id];
  return *ptr;
}

void fx_current_instance_setter(struct Editor* editor, int16_t value)
{
  // the parameter may be from either instance, which may be
  // switched at any time. So recompute the value pointer in respect of the instance!
  uint8_t* ptr = &((uint8_t*)editor->value)[selected_instance_id];
  *ptr = (uint8_t)value;
}

FLASHMEM void UI_func_dexed_audio(uint8_t param)
{

  if (LCDML.FUNC_setup()) // ****** SETUP *********
  {
    ui.reset();
    ui.setCursor(1, 1);
    addInstanceEditor(&dexed_voice_name_renderer);

    ui.setCursor(1, 4);
    ui.printLn("DEXED AUDIO SETUP");
    ui.printLn("");
    ui.addEditor("VOLUME", SOUND_INTENSITY_MIN, SOUND_INTENSITY_MAX, &configuration.dexed[0].sound_intensity,
      &dexed_current_instance_getter, [](Editor* editor, int16_t value)
      {
        dexed_current_instance_setter(editor, value);
        MD_sendControlChange(configuration.dexed[selected_instance_id].midi_channel, 7, value);
        MicroDexed[selected_instance_id]->setGain(midi_volume_transform(map(value, SOUND_INTENSITY_MIN, SOUND_INTENSITY_MAX, 0, 127))); });

    // pan: custom getter and setter to center pan around 0 for a nice pan bar
    const int16_t pan_center = (PANORAMA_MAX + PANORAMA_MIN) / 2;
    ui.addEditor(
      "PAN", (int16_t)(PANORAMA_MIN - pan_center), (int16_t)(PANORAMA_MAX - pan_center), &configuration.dexed[0].pan,
      [](Editor* editor) -> int16_t
      {
        return dexed_current_instance_getter(editor) - pan_center; // center around 0
      },
      [](Editor* editor, int16_t value)
      {
        dexed_current_instance_setter(editor, value + pan_center);
        MD_sendControlChange(configuration.dexed[selected_instance_id].midi_channel, 10, map(value + pan_center, PANORAMA_MIN, PANORAMA_MAX, 0, 127));
        if (configuration.sys.mono == 0)
        {
          dexed_dry_mono2stereo[selected_instance_id]->panorama(mapfloat(value + pan_center, PANORAMA_MIN, PANORAMA_MAX, -1.0, 1.0));
        }
      });

    ui.printLn("");
    ui.printLn("CHORUS", GREY2);
    ui.addEditor("FREQUENCY", CHORUS_FREQUENCY_MIN, CHORUS_FREQUENCY_MAX, &configuration.fx.chorus_frequency[0],
      &fx_current_instance_getter, [](Editor* editor, int16_t value)
      {
        fx_current_instance_setter(editor, value);
        chorus_modulator[selected_instance_id]->frequency(value / 10.0); });
    ui.addEditor(
      "WAVEFORM", CHORUS_WAVEFORM_MIN, CHORUS_WAVEFORM_MAX, &configuration.fx.chorus_waveform[0],
      &fx_current_instance_getter, [](Editor* editor, int16_t value)
      {
        fx_current_instance_setter(editor, value);
        chorus_modulator[selected_instance_id]->begin(value == 1 ? WAVEFORM_SINE : WAVEFORM_TRIANGLE); },
      [](Editor* editor, bool refresh)
      {
        prepare_multi_options(editor, refresh);
        ui.print(editor->get() ? "[SINE    ]" : "[TRIANGLE]");
      });
    ui.addEditor("DEPTH", CHORUS_DEPTH_MIN, CHORUS_DEPTH_MAX, &configuration.fx.chorus_depth[0],
      &fx_current_instance_getter, [](Editor* editor, int16_t value)
      {
        fx_current_instance_setter(editor, value);
        chorus_modulator[selected_instance_id]->amplitude(value / 100.0); });
    ui.addEditor("LEVEL", CHORUS_LEVEL_MIN, CHORUS_LEVEL_MAX, &configuration.fx.chorus_level[0],
      &fx_current_instance_getter, [](Editor* editor, int16_t value)
      {
        fx_current_instance_setter(editor, value);
        MD_sendControlChange(configuration.dexed[selected_instance_id].midi_channel, 93, value);
        global_delay_in_mixer[selected_instance_id]->gain(1, mapfloat(value, CHORUS_LEVEL_MIN, CHORUS_LEVEL_MAX, 0.0, 0.9)); });

    ui.printLn("");
    ui.printLn("EFFECTS", GREY2);
    ui.addEditor("DELAY SEND", DELAY_LEVEL_MIN, DELAY_LEVEL_MAX, &configuration.fx.delay_level[0],
      &fx_current_instance_getter, [](Editor* editor, int16_t value)
      {
        fx_current_instance_setter(editor, value);
        MD_sendControlChange(configuration.dexed[selected_instance_id].midi_channel, 107, value);
        global_delay_in_mixer[selected_instance_id]->gain(0, midi_volume_transform(map(value, DELAY_LEVEL_MIN, DELAY_LEVEL_MAX, 0, 127))); });
    ui.addEditor("REVERB SEND", REVERB_SEND_MIN, REVERB_SEND_MAX, &configuration.fx.reverb_send[0],
      &fx_current_instance_getter, [](Editor* editor, int16_t value)
      {
        fx_current_instance_setter(editor, value);
        MD_sendControlChange(configuration.dexed[selected_instance_id].midi_channel, 91, value);
        reverb_mixer_l.gain(selected_instance_id, volume_transform(mapfloat(value, REVERB_SEND_MIN, REVERB_SEND_MAX, 0.0, VOL_MAX_FLOAT)));
        reverb_mixer_r.gain(selected_instance_id, volume_transform(mapfloat(value, REVERB_SEND_MIN, REVERB_SEND_MAX, 0.0, VOL_MAX_FLOAT))); });
    ui.addEditor("DELAY TO REVERB", REVERB_SEND_MIN, REVERB_SEND_MAX, &configuration.fx.delay_to_reverb[0],
      &fx_current_instance_getter, &fx_current_instance_setter);

    // filter
    ui.printLn("");
    ui.printLn("FILTER", GREY2);
    ui.addEditor("CUTOFF", FILTER_CUTOFF_MIN, FILTER_CUTOFF_MAX, &configuration.fx.filter_cutoff[0],
      &fx_current_instance_getter, [](Editor* editor, int16_t value)
      {
        fx_current_instance_setter(editor, value);
        MD_sendControlChange(configuration.dexed[selected_instance_id].midi_channel, 104, value);
        MicroDexed[selected_instance_id]->setFilterCutoff(mapfloat(value, FILTER_CUTOFF_MIN, FILTER_CUTOFF_MAX, 1.0, 0.0)); });
    ui.addEditor("RESONANCE", FILTER_RESONANCE_MIN, FILTER_RESONANCE_MAX, &configuration.fx.filter_resonance[0],
      &fx_current_instance_getter, [](Editor* editor, int16_t value)
      {
        fx_current_instance_setter(editor, value);
        MD_sendControlChange(configuration.dexed[selected_instance_id].midi_channel, 103, value);
        MicroDexed[selected_instance_id]->setFilterResonance(mapfloat(value, FILTER_RESONANCE_MIN, FILTER_RESONANCE_MAX, 1.0, 0.0)); });
  }
  if (LCDML.FUNC_loop()) // ****** LOOP *********
  {
    ui.handle_input();
  }
  if (LCDML.FUNC_close()) // ****** STABLE END *********
  {
    ui.clear();
  }
}

void note_name_renderer(struct Editor* editor, bool refresh)
{
  prepare_multi_options(editor, refresh);
  char note_name[4];
  getNoteName(note_name, editor->get());
  display.print("[");
  display.print(note_name);
  display.print("]");
}

// UI page to allow editing of all global dexed parameters
// this somehow resebles the "Function" edit plane on an DX7 instrument.
//
FLASHMEM void UI_func_dexed_setup(uint8_t param)
{

  if (LCDML.FUNC_setup()) // ****** SETUP *********
  {
    ui.reset();
    ui.setCursor(1, 1);
    // allow switching the currently displayed instance
    addInstanceEditor(&dexed_voice_name_renderer);

    ui.setCursor(1, 4);
    ui.printLn("DEXED INSTANCE SETUP");
    ui.printLn("");

    ui.printLn("MIDI");
    ui.addEditor("MIDI CHANNEL", MIDI_CHANNEL_MIN, MIDI_CHANNEL_MAX, &configuration.dexed[0].midi_channel, &dexed_current_instance_getter, dexed_current_instance_setter);
    ui.addEditor("LOWEST NOTE", INSTANCE_LOWEST_NOTE_MIN, INSTANCE_LOWEST_NOTE_MAX, &configuration.dexed[0].lowest_note,
      &dexed_current_instance_getter, &dexed_current_instance_setter, &note_name_renderer);
    ui.addEditor("HIGHEST NOTE", INSTANCE_HIGHEST_NOTE_MIN, INSTANCE_HIGHEST_NOTE_MAX, &configuration.dexed[0].highest_note,
      &dexed_current_instance_getter, &dexed_current_instance_setter, &note_name_renderer);
    ui.printLn("");

    ui.printLn("POLYPHONY");
    ui.addEditor(
      "MONO/POLY", MONOPOLY_MIN, MONOPOLY_MAX, &configuration.dexed[0].monopoly,
      &dexed_current_instance_getter, [](Editor* editor, int16_t value)
      {
        dexed_current_instance_setter(editor, value);
        MicroDexed[selected_instance_id]->setMonoMode(!value); },
      [](struct Editor* editor, bool refresh)
      {
        prepare_multi_options(editor, refresh);
        if (!editor->get())
          display.print(F("[MONO]"));
        else
          display.print(F("[POLY]"));
      });
    ui.addEditor("POLYPHONY", POLYPHONY_MIN, POLYPHONY_MAX, &configuration.dexed[0].polyphony,
      &dexed_current_instance_getter, [](Editor* editor, int16_t value)
      {
        dexed_current_instance_setter(editor, value);
        MicroDexed[selected_instance_id]->setMaxNotes(value); });
    ui.printLn("");

    ui.printLn("TUNING");
    ui.addEditor("TRANSPOSE", TRANSPOSE_MIN, TRANSPOSE_MAX, &configuration.dexed[0].transpose,
      &dexed_current_instance_getter, [](Editor* editor, int16_t value)
      {
        dexed_current_instance_setter(editor, value);
        MicroDexed[selected_instance_id]->setTranspose(value);
        MicroDexed[selected_instance_id]->notesOff();
        send_sysex_param(configuration.dexed[selected_instance_id].midi_channel, 144, value, 0); });
    ui.addEditor("FINE TUNE", TUNE_MIN, TUNE_MAX, &configuration.dexed[0].tune,
      &dexed_current_instance_getter, [](Editor* editor, int16_t value)
      {
        dexed_current_instance_setter(editor, value);
        MD_sendControlChange(configuration.dexed[selected_instance_id].midi_channel, 94, value); });
    ui.printLn("");

    ui.setCursor(27, 4);
    ui.printLn("PORTAMENTO", GREY2);
    ui.addEditor("MODE", PORTAMENTO_MODE_MIN, PORTAMENTO_MODE_MAX, &configuration.dexed[0].portamento_mode,
      &dexed_current_instance_getter, &dexed_portamento_setter, [](struct Editor* editor, bool refresh)
      {
        prepare_multi_options(editor, refresh);
        uint8_t mode = editor->get();
        uint8_t monopoly = configuration.dexed[selected_instance_id].monopoly;
        if (!mode && monopoly) display.print("[RETAIN]");
        if (!mode && !monopoly) display.print("[FINGER]");
        if (mode && monopoly) display.print("[FOLLOW]");
        if (mode && !monopoly) display.print("[FULL  ]"); });
    ui.addEditor("GLISSANDO", PORTAMENTO_GLISSANDO_MIN, PORTAMENTO_GLISSANDO_MAX, &configuration.dexed[0].portamento_glissando,
      &dexed_current_instance_getter, &dexed_portamento_setter);
    ui.addEditor("TIME", PORTAMENTO_TIME_MIN, PORTAMENTO_TIME_MAX, &configuration.dexed[0].portamento_time,
      &dexed_current_instance_getter, &dexed_portamento_setter);
    ui.printLn("");

    ui.printLn("INTERNALS", GREY2);
    ui.addEditor(
      "NOTE REFRESH", NOTE_REFRESH_MIN, NOTE_REFRESH_MAX, &configuration.dexed[0].note_refresh,
      &dexed_current_instance_getter, [](Editor* editor, int16_t value)
      {
        dexed_current_instance_setter(editor, value);
        MicroDexed[selected_instance_id]->setNoteRefreshMode(value); },
      [](struct Editor* editor, bool refresh)
      {
        prepare_multi_options(editor, refresh);
        if (!editor->get())
          display.print(F("[NORMAL ]"));
        else
          display.print(F("[RETRIG.]"));
      });
    ui.addEditor("VELOCITY LEVEL", VELOCITY_LEVEL_MIN, VELOCITY_LEVEL_MAX, &configuration.dexed[0].velocity_level,
      &dexed_current_instance_getter, &dexed_current_instance_setter);
  }
  if (LCDML.FUNC_loop()) // ****** LOOP *********
  {
    ui.handle_input();
  }
  if (LCDML.FUNC_close()) // ****** STABLE END *********
  {
    ui.clear();
  }
}

FLASHMEM void UI_handle_OP(uint8_t param)
{
  static uint8_t op_selected;

  display_OP_active_instance_number(selected_instance_id, configuration.dexed[selected_instance_id].op_enabled);

  if (LCDML.FUNC_setup()) // ****** SETUP *********
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
  if (LCDML.FUNC_loop()) // ****** LOOP *********
  {
    if (LCDML.BT_checkUp() && encoderDir[ENC_R].Up())
    {
      if (op_selected == 0)
      {
        selected_instance_id = !selected_instance_id;
        op_selected = 5;
        display_OP_active_instance_number(selected_instance_id, configuration.dexed[selected_instance_id].op_enabled);
      }
      else
        op_selected = constrain(op_selected - 1, 0, 5);
    }
    else if (LCDML.BT_checkDown() && encoderDir[ENC_R].Down())
    {
      if (op_selected == 5)
      {
        selected_instance_id = !selected_instance_id;
        op_selected = 0;
        display_OP_active_instance_number(selected_instance_id, configuration.dexed[selected_instance_id].op_enabled);
      }
      else
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

  if (LCDML.FUNC_close()) // ****** STABLE END *********
  {
    encoderDir[ENC_R].reset();
    display.setTextColor(COLOR_SYSTEXT, COLOR_BACKGROUND);
    display.fillScreen(COLOR_BACKGROUND);
  }
}

void pattern_preview_in_probability_editor(uint8_t line, uint8_t patternno)
{
  display.setTextSize(1);
  seq.active_pattern = patternno;
  display.setTextColor(COLOR_SYSTEXT, COLOR_CHORDS);
  if (patternno == 99)
  {
    display.print(F("EMPTY "));
  }
  display.setTextColor(GREY1, GREY4);
  display.print("[");
  for (uint8_t i = 0; i < 16; i++)
  {
    if (seq.vel[patternno][i] > 209)
      display.setTextColor(COLOR_PITCHSMP, GREY4);
    else
    {
      if (seq.content_type[patternno] == 0) // Drumpattern
        display.setTextColor(COLOR_DRUMS, GREY4);
      else if (seq.content_type[patternno] == 1) // Instrument Pattern
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

FLASHMEM void print_probabilities()
{
  for (uint8_t y = 0; y < 14; y++)
  {
    display.setTextSize(1);
    if (y == temp_int - generic_temp_select_menu && generic_menu == 0)
      display.setTextColor(COLOR_BACKGROUND, COLOR_SYSTEXT);
    else
      display.setTextColor(COLOR_SYSTEXT, COLOR_BACKGROUND);
    setCursor_textGrid_small(1, y + 6);
    print_formatted_number(y + generic_temp_select_menu, 2);
    display.print(F("        "));
    setCursor_textGrid_small(11, y + 6);
    if (y == temp_int - generic_temp_select_menu && generic_menu == 1)
      display.setTextColor(RED, GREY4);
    print_formatted_number(seq.pat_chance[y + generic_temp_select_menu], 3);
    display.print(F(" %"));
    if (y == temp_int - generic_temp_select_menu && generic_menu == 1)
      display.setTextColor(COLOR_SYSTEXT, COLOR_BACKGROUND);
    display.print(F("    "));
    display.setTextColor(COLOR_SYSTEXT, COLOR_BACKGROUND);
    setCursor_textGrid_small(20, y + 6);
    if (y == temp_int - generic_temp_select_menu && generic_menu == 2)
      display.setTextColor(RED, GREY4);
    else if (y == temp_int - generic_temp_select_menu && generic_menu == 0)
      display.setTextColor(COLOR_BACKGROUND, COLOR_SYSTEXT);
    else
      display.setTextColor(COLOR_SYSTEXT, COLOR_BACKGROUND);
    print_formatted_number(seq.pat_vel_variation[y + generic_temp_select_menu], 3);
    display.setTextColor(GREY2, COLOR_BACKGROUND);
    if (y == temp_int - generic_temp_select_menu && generic_menu == 2)
      display.setTextColor(RED, COLOR_BACKGROUND);
    display.print(F(" STP"));
    setCursor_textGrid_small(29, y + 6);
    pattern_preview_in_probability_editor(y + 6, y + generic_temp_select_menu);
  }

  // scrollbar
  drawScrollbar(DISPLAY_WIDTH - 6 - CHAR_width_small, 6 * (CHAR_height_small + 2), 14, NUM_SEQ_PATTERN, temp_int, CHAR_height_small + 2);
}

FLASHMEM void UI_func_seq_probabilities(uint8_t param)
{
  display.setTextSize(1);

  if (LCDML.FUNC_setup()) // ****** SETUP *********
  {
    temp_int = 0;
    generic_temp_select_menu = 0;
    generic_menu = 0;
    encoderDir[ENC_R].reset();
    display.fillScreen(COLOR_BACKGROUND);
    helptext_l(back_text);
    helptext_r("SELECT PATTERN");
    display.setCursor(1 * CHAR_width_small, DISPLAY_HEIGHT - CHAR_height_small * 3);
    display.setTextColor(COLOR_INSTR);
    display.print(F("INSTR  "));
    display.setTextColor(COLOR_DRUMS);
    display.print(F("DRUM/SMP  "));
    display.setTextColor(COLOR_PITCHSMP);
    display.print(F("PITCHED SAMPLE  "));
    display.setTextColor(COLOR_CHORDS);
    display.print(F("CHORD  "));
    display.setTextColor(COLOR_ARP);
    display.print(F("ARP"));
    display.setTextColor(RED, COLOR_BACKGROUND);
    display.setTextSize(1);
    setCursor_textGrid_small(1, 1);
    display.print(F("SEQUENCER TRIGGER"));
    setCursor_textGrid_small(1, 2);
    display.print(F("PROBABILITIES"));
    display.setTextColor(GREY2, COLOR_BACKGROUND);
    setCursor_textGrid_small(20, 0);
    display.print(F("T"));
    setCursor_textGrid_small(20, 1);
    display.print(F("P"));
    setCursor_textGrid_small(20, 2);
    display.print(F("N"));
    print_live_probability_pattern_info();
    display.setTextColor(COLOR_SYSTEXT, COLOR_BACKGROUND);
    setCursor_textGrid_small(1, 4);
    display.print(F("PATTERN   CHANCE   VEL.+-   PREVIEW"));
  }
  if (LCDML.FUNC_loop()) // ****** LOOP *********
  {
    if ((LCDML.BT_checkDown() && encoderDir[ENC_R].Down()) || (LCDML.BT_checkUp() && encoderDir[ENC_R].Up()) || (LCDML.BT_checkEnter() && encoderDir[ENC_R].ButtonShort()))
    {
      if (LCDML.BT_checkDown())
      {
        if (generic_menu == 0)
        {
          temp_int = constrain(temp_int + 1, 0, NUM_SEQ_PATTERN - 1);
          if (generic_temp_select_menu < NUM_SEQ_PATTERN - 1 - 13 && temp_int > 13)
            generic_temp_select_menu++;
        }
        else if (generic_menu == 1)
        {
          seq.pat_chance[temp_int] = constrain(seq.pat_chance[temp_int] + 1, 0, 100);
        }
        else if (generic_menu == 2)
        {
          seq.pat_vel_variation[temp_int] = constrain(seq.pat_vel_variation[temp_int] + 1, 0, 100);
        }
      }
      else if (LCDML.BT_checkUp())
      {
        if (generic_menu == 0)
        {
          temp_int = constrain(temp_int - 1, 0, NUM_SEQ_PATTERN - 1);
          if (generic_temp_select_menu > 0)
            generic_temp_select_menu--;
        }
        else if (generic_menu == 1)
        {
          seq.pat_chance[temp_int] = constrain(seq.pat_chance[temp_int] - 1, 0, 100);
        }
        else if (generic_menu == 2)
        {
          seq.pat_vel_variation[temp_int] = constrain(seq.pat_vel_variation[temp_int] - 1, 0, 100);
        }
      }
    }
    if (LCDML.BT_checkEnter())
    {
      generic_menu++;
      if (generic_menu > 2)
        generic_menu = 0;

      if (generic_menu == 0)
        helptext_r("SELECT PATTERN");
      else if (generic_menu == 1)
        helptext_r("SET PROBABILITY");
      else if (generic_menu == 2)
        helptext_r("SET VEL. VARIATION");
    }
    print_probabilities();
  }
  if (LCDML.FUNC_close()) // ****** STABLE END *********
  {
    display.fillScreen(COLOR_BACKGROUND);
    encoderDir[ENC_R].reset();
  }
}

FLASHMEM void print_custom_mappings()
{
  display.setTextSize(1);
  uint8_t line = 9;
  int offset = generic_temp_select_menu - 3;
  if (offset < 10 || offset >= 99)
    offset = 0;
  if (offset > 9)
    offset = generic_temp_select_menu - 3 - 9;
  display.setTextColor(COLOR_SYSTEXT, COLOR_BACKGROUND);
  for (uint8_t y = 0 + offset; y < 10 + offset; y++)
  {
    display.setCursor(1 * CHAR_width_small, line * 12);

    if (generic_temp_select_menu - 3 == y)
      display.setTextColor(COLOR_SYSTEXT, RED);
    else
      display.setTextColor(COLOR_SYSTEXT, COLOR_BACKGROUND);

    print_formatted_number((y + 1), 2); // entry no.
    if (custom_midi_map[y].type == 0)
    {
      display.setTextColor(GREY2, COLOR_BACKGROUND);
      show_no_grid(line * 12, 5 * CHAR_width_small, 7, "NONE");
    }
    else if (custom_midi_map[y].type == 1)
    {
      display.setTextColor(COLOR_PITCHSMP, COLOR_BACKGROUND);
      show_no_grid(line * 12, 5 * CHAR_width_small, 7, "KEY/PAD");
    }
    else if (custom_midi_map[y].type == 2)
    {
      display.setTextColor(COLOR_PITCHSMP, COLOR_BACKGROUND);
      show_no_grid(line * 12, 5 * CHAR_width_small, 7, "MIDI CC");
    }
    else if (custom_midi_map[y].type == 3)
    {
      display.setTextColor(PINK, COLOR_BACKGROUND);
      show_no_grid(line * 12, 5 * CHAR_width_small, 7, "UI KEY ");
    }
    display.setTextColor(COLOR_DRUMS, COLOR_BACKGROUND);
    show_no_grid(line * 12, 14 * CHAR_width_small, 3, custom_midi_map[y].in);
    display.setTextColor(COLOR_CHORDS, COLOR_BACKGROUND);
    show_no_grid(line * 12, 19 * CHAR_width_small, 3, custom_midi_map[y].out);
    display.setTextColor(COLOR_INSTR, COLOR_BACKGROUND);
    show_no_grid(line * 12, 24 * CHAR_width_small, 3, custom_midi_map[y].channel);

    if (generic_temp_select_menu - 3 == y)
      display.setTextColor(COLOR_SYSTEXT, RED);
    // else
    //   display.setTextColor(COLOR_SYSTEXT, COLOR_BACKGROUND);
    else if (custom_midi_map[y].in == 0)
      display.setTextColor(GREY2, COLOR_BACKGROUND);
    else if (custom_midi_map[y].type == 1)
      display.setTextColor(COLOR_DRUMS, COLOR_BACKGROUND);
    else if (custom_midi_map[y].type == 2)
      display.setTextColor(COLOR_INSTR, COLOR_BACKGROUND);
    else if (custom_midi_map[y].type == 3)
      display.setTextColor(PINK, COLOR_BACKGROUND);

    if (custom_midi_map[y].in == 0)
      show_no_grid(line * 12, 34 * CHAR_width_small, 14, "EMPTY SLOT");
    else if (custom_midi_map[y].type == 1)
    {

      show_no_grid(line * 12, 34 * CHAR_width_small, 14, find_long_drum_name_from_note(custom_midi_map[y].out));
    }
    else if (custom_midi_map[y].type == 2)
    {

      for (uint8_t i = 0; i < sizeof(cc_dest_values); i++)
      {
        if (custom_midi_map[y].out == cc_dest_values[i])
        {
          show_no_grid(line * 12, 34 * CHAR_width_small, 14, cc_names[i]);
        }
      }
    }
    else if (custom_midi_map[y].type == 3)
    {
      for (uint8_t i = 0; i < sizeof(cc_dest_values_UI_mapping); i++)
      {
        if (custom_midi_map[y].out == cc_dest_values_UI_mapping[i])
        {
          show_no_grid(line * 12, 34 * CHAR_width_small, 14, cc_names_UI_mapping[i]);
        }
      }
    }
    line++;
  }
}

FLASHMEM void print_custom_mapping_drums()
{
  display.fillRect(14 * CHAR_width + 10, 5, 20, 9, COLOR_BACKGROUND);
  display.setTextSize(1);
  setCursor_textGrid_small(1, 6);
  display.setTextColor(COLOR_ARP, COLOR_BACKGROUND);
  display.print(F("SET YOUR MIDI DEVICE TO DRUM CH. "));
  display.setTextColor(COLOR_DRUMS, COLOR_BACKGROUND);
  display.print(F("["));
  display.setTextColor(COLOR_SYSTEXT, COLOR_BACKGROUND);
  display.print(drum_midi_channel);
  display.setTextColor(COLOR_DRUMS, COLOR_BACKGROUND);
  display.print(F("]        "));
  display.setTextColor(COLOR_ARP, COLOR_BACKGROUND);
  setCursor_textGrid_small(1, 7);
  display.print(F("TOUCH "));
  display.setTextColor(COLOR_SYSTEXT, COLOR_BACKGROUND);
  display.print(F("LEARN "));
  display.setTextColor(COLOR_ARP, COLOR_BACKGROUND);
  display.print(F("TO REMAP YOUR FAVORITE DRUMS "));
}

FLASHMEM void print_custom_mapping_cc()
{
  UI_update_instance_icons();
  display.setTextSize(1);
  setCursor_textGrid_small(1, 6);
  display.setTextColor(COLOR_ARP, COLOR_BACKGROUND);
  display.print(F("MAP MIDI CC TO DEXED PARAMETERS, INSTANCE "));
  display.setTextColor(COLOR_DRUMS, COLOR_BACKGROUND);
  display.print(F("["));
  display.setTextColor(COLOR_SYSTEXT, COLOR_BACKGROUND);
  display.print(selected_instance_id + 1);
  display.setTextColor(COLOR_DRUMS, COLOR_BACKGROUND);
  display.print(F("] "));
  setCursor_textGrid_small(1, 7);
  display.setTextColor(COLOR_ARP, COLOR_BACKGROUND);
  display.print(F("SWITCH INSTANCES WITH "));
  display.setTextColor(COLOR_SYSTEXT, COLOR_BACKGROUND);
  display.print(F("LONG PUSH "));
  display.setTextColor(RED, COLOR_BACKGROUND);
  display.print(F("ENCODER_R   "));
}

FLASHMEM void print_custom_mapping_ui()
{
  display.fillRect(14 * CHAR_width + 10, 5, 20, 9, COLOR_BACKGROUND);
  display.setTextSize(1);
  setCursor_textGrid_small(1, 6);
  display.setTextColor(COLOR_ARP, COLOR_BACKGROUND);
  display.print(F("MAP/ADD CUSTOM MIDI KEYS FOR MIDI UI CONTROL "));
  setCursor_textGrid_small(1, 7);
  display.setTextColor(COLOR_ARP, COLOR_BACKGROUND);
  display.print(F("USEFUL WHEN YOU CAN'T SEND DEFAULT CC VALUES"));
}

FLASHMEM void print_mapping_help_text()
{
  if (seq.edit_state == false && generic_temp_select_menu == 0)
    print_custom_mapping_drums();
  else if (seq.edit_state == false && generic_temp_select_menu == 1)
    print_custom_mapping_cc();
  else if (seq.edit_state == false && generic_temp_select_menu == 2)
    print_custom_mapping_ui();
}

FLASHMEM void draw_scrollbar_custom_mappings()
{
  if (generic_temp_select_menu > 2)
    drawScrollbar(DISPLAY_WIDTH - CHAR_width_small * 2, 9 * 12, 10, NUM_CUSTOM_MIDI_MAPPINGS, generic_temp_select_menu - 3, 12);
  else
    drawScrollbar(DISPLAY_WIDTH - CHAR_width_small * 2, 9 * 12, 10, NUM_CUSTOM_MIDI_MAPPINGS, 0, 12);
  print_custom_mappings();
}

uint16_t get_overview_factor(unsigned long filelength)
{
  uint16_t overview_factor = 0;
  overview_factor = (filelength / 140);
  if (overview_factor % 2 != 0)
    overview_factor = overview_factor - 1;
  return overview_factor;
}


#if defined(COMPILE_FOR_FLASH)
void draw_waveform_overview_in_drums_page_step2(uint8_t overview_buffer[], int end_zoomed_x_position, unsigned long filelength)
{
  uint8_t xspace = 0;
  uint32_t current_pos = 0;
  int oldy = 50;
  uint8_t x_offset = 174;
  uint16_t overview_factor = 0;
  display.console = true;
  display.fillRect(x_offset, 14, DISPLAY_WIDTH - x_offset - 11, 38, GREY4);

  while (xspace < 133) // draw overview
  {
    display.drawLine(x_offset + 1 + (xspace - 1), oldy - 16, x_offset + 1 + xspace, overview_buffer[xspace] - 16, COLOR_SYSTEXT);
    oldy = overview_buffer[xspace];
    xspace = xspace + 1;
    current_pos = current_pos + overview_factor;
  }
}
#endif


#if defined(COMPILE_FOR_FLASH)
void draw_waveform_overview_in_drums_page_step1(int samplenumber)
{
  display.console = true;
  uint8_t overview_buffer[142];
  uint16_t xspace = 1;
  short samplevalue = 0;
  uint32_t current_pos = 0;
  int end_zoomed_x_position = 0;
  uint16_t overview_factor = 0;
  char filename[26];
  uint32_t filesize;

  SerialFlash.opendir();
  if (samplenumber > 0)
  {
    for (int f = 0; f < samplenumber; f++)
    {
      if (SerialFlash.readdir(filename, sizeof(filename), filesize))
        ;
      else
        break;
    }
  }
  SerialFlashFile f = SerialFlash.open(filename);
  unsigned long filelength = f.size();

  f.seek(44);
  char buf[256];
  overview_factor = get_overview_factor(filelength);

  end_zoomed_x_position = current_pos;
  //draw_end_marking(xspace);
  xspace = 0;
  current_pos = 0;
  while (xspace < 140) // generate overview
  {
    f.read(buf, 256);
    samplevalue = ((buf[1] * 256) + buf[0]);
    overview_buffer[xspace] = samplevalue / 1800 + 50;
    xspace = xspace + 1;
    current_pos = current_pos + overview_factor;
    f.seek(44 + current_pos);
  }
  draw_waveform_overview_in_drums_page_step2(overview_buffer, end_zoomed_x_position, filelength);

  f.close();

  // else if (fm.sample_source == 0) // SD
  // {
  //   File f;
  //   File myDir = SD.open("/DRUMS");
  //   for (uint16_t i = 0; i < samplenumber; i++)
  //   {
  //     f.close();
  //     f = myDir.openNextFile();
  //     // if (! f)  break;
  //   }
  //   unsigned long filelength = f.size();
  //   f.seek(44);
  //   char buf[256];
  //   overview_factor = get_overview_factor(filelength);
  //   end_zoomed_x_position = current_pos;
  //   //draw_end_marking(xspace);
  //   xspace = 0;
  //   current_pos = 0;
  //   while (xspace < 140) // generate overview
  //   {
  //     f.read(buf, 256);
  //     samplevalue = ((buf[1] * 256) + buf[0]);
  //     overview_buffer[xspace] = samplevalue / 1800 + 50;
  //     xspace = xspace + 1;
  //     current_pos = current_pos + overview_factor;
  //     f.seek(44 + current_pos);
  //   }
  //   draw_waveform_overview_in_drums_page_step2(overview_buffer, end_zoomed_x_position, filelength);
  //   f.close();
  //   myDir.close();
  // }

}
#endif

FLASHMEM void UI_func_custom_mappings(uint8_t param)
{
  char displayname[8] = { 0, 0, 0, 0, 0, 0, 0 };

  if (LCDML.FUNC_setup()) // ****** SETUP *********
  {
    encoderDir[ENC_R].reset();
    display.fillScreen(COLOR_BACKGROUND);
    generic_menu = 0;
    generic_temp_select_menu = 0;
    generic_active_function = 0;

    seq.edit_state = false;
    helptext_l(back_text);
    helptext_r("SELECT MAPPING TYPE");
    display.setTextColor(COLOR_SYSTEXT, COLOR_BACKGROUND);
    display.setTextSize(1);
    draw_button_on_grid(36, 1, "PREV.", "", 0);
    drawBitmap(CHAR_width_small * 38 + 4, CHAR_height * 1 + 8, special_chars[19], 8, 8, GREEN);
    draw_button_on_grid(45, 1, "MIDI", "LEARN", 0);
    display.setTextSize(1);
    display.setTextColor(COLOR_SYSTEXT, COLOR_BACKGROUND);
    setCursor_textGrid_small(1, 9);
    display.print(F("NO  TYPE     IN   OUT  CH.  NAME"));
    print_custom_mapping_drums();
    print_custom_mappings();
    draw_scrollbar_custom_mappings();
  }
  if (LCDML.FUNC_loop()) // ****** LOOP *********
  {
    if ((LCDML.BT_checkDown() && encoderDir[ENC_R].Down()) || (LCDML.BT_checkUp() && encoderDir[ENC_R].Up()) || (LCDML.BT_checkEnter() && encoderDir[ENC_R].ButtonShort()))
    {
      if (LCDML.BT_checkDown())
      {
        if (seq.edit_state == false)
        {
          generic_temp_select_menu = constrain(generic_temp_select_menu + 1, 0, 2 + NUM_CUSTOM_MIDI_MAPPINGS);
          print_mapping_help_text();
          draw_scrollbar_custom_mappings();
        }
        else if (seq.edit_state && generic_temp_select_menu == 0)
        {
          smart_filter(1);
        }
        else if (seq.edit_state && generic_temp_select_menu == 1)
        {
          generic_menu = constrain(generic_menu + 1, 0, MAX_CC_DEST - 1);
        }
        else if (seq.edit_state && generic_temp_select_menu == 2)
        {
          generic_menu = constrain(generic_menu + 1, 0, 7);
        }
      }
      else if (LCDML.BT_checkUp())
      {
        if (seq.edit_state == false)
        {
          generic_temp_select_menu = constrain(generic_temp_select_menu - 1, 0, 2 + NUM_CUSTOM_MIDI_MAPPINGS);
          print_mapping_help_text();
          draw_scrollbar_custom_mappings();
        }
        else if (seq.edit_state && generic_temp_select_menu == 0)
        {
          smart_filter(0);
        }
        else if (seq.edit_state && generic_temp_select_menu == 1)
        {
          generic_menu = constrain(generic_menu - 1, 0, MAX_CC_DEST - 1);
        }
        else if (seq.edit_state && generic_temp_select_menu == 2)
        {
          generic_menu = constrain(generic_menu - 1, 0, 7);
        }
      }
    }
    if (LCDML.BT_checkEnter() && encoderDir[ENC_R].ButtonPressed() && generic_temp_select_menu == 1)
    {
      selected_instance_id = !selected_instance_id;
      print_custom_mapping_cc();
    }
    else if (LCDML.BT_checkEnter())
    {

      if (generic_temp_select_menu < 3)
      {
        if (seq.edit_state == false)
          seq.edit_state = true;
        else
          seq.edit_state = false;

        if (seq.edit_state == false)
          helptext_r("SELECT MAPPING TYPE");
        else
        {
          generic_menu = 0;
          helptext_r("SELECT DESTINATION");
        }
        display.setTextSize(2);
      }
      if (generic_temp_select_menu > 2)
      { // is in data line

        custom_midi_map[generic_temp_select_menu - 3].type = 0;
        custom_midi_map[generic_temp_select_menu - 3].in = 0;
        custom_midi_map[generic_temp_select_menu - 3].out = 0;
        custom_midi_map[generic_temp_select_menu - 3].channel = 0;
        print_custom_mappings();
      }
    }

    display.setTextSize(2);

    setCursor_textGrid_small(3, 1);
    if (generic_temp_select_menu == 0)
    {
      setModeColor(0);
      display.print(F("NOTES/DRUMS"));
    }
    else if (generic_temp_select_menu == 1)
    {
      setModeColor(1);
      display.print(F("MIDI CC    "));
    }
    else if (generic_temp_select_menu == 2)
    {
      setModeColor(2);
      display.print(F("NOTES TO UI"));
      if (seq.edit_state == false)
        helptext_r("SELECT MAPPING TYPE");
      else
        helptext_r("SELECT DESTINATION");
    }
    display.setTextSize(2);
    if (generic_temp_select_menu < 3)
    {
      if (seq.edit_state)
      {
        display.setTextColor(GREY2, COLOR_BACKGROUND);
        setCursor_textGrid_small(1, 1);
        display.print(" ");
        setCursor_textGrid_small(25, 1);
        display.print(" ");

        display.setTextColor(COLOR_SYSTEXT, RED);
        setCursor_textGrid_small(1, 3);
        display.print("[");
        setCursor_textGrid_small(9, 3);
        display.print("]");
      }
      else
      {
        display.setTextColor(COLOR_BACKGROUND, COLOR_SYSTEXT);
        setCursor_textGrid_small(1, 1);
        display.print("[");
        setCursor_textGrid_small(25, 1);
        display.print("]");

        display.setTextColor(GREY2, COLOR_BACKGROUND);
        setCursor_textGrid_small(1, 3);
        display.print(" ");
        setCursor_textGrid_small(9, 3);
        display.print(" ");
      }
    }
    if (generic_temp_select_menu == 3)
    { // gone from menu to data lines

      display.setTextColor(GREY2, COLOR_BACKGROUND);
      setCursor_textGrid_small(1, 1);
      display.print(" ");
      setCursor_textGrid_small(25, 1);
      display.print(" ");
      helptext_r("DELETE MAPPING");
    }

    display.setTextSize(2);
    if (generic_temp_select_menu == 0)
    {
      setCursor_textGrid_small(3, 3);
      snprintf_P(displayname, sizeof(displayname), PSTR("%03d"), activesample);
      display.print(displayname);

      // display.setTextColor(COLOR_SYSTEXT, COLOR_BACKGROUND);
      show_no_grid(4 * CHAR_height_small - 2, 11 * CHAR_width_small, 12, basename(drum_config[activesample].name));
    }
    else if (generic_temp_select_menu == 1)
    {

      setCursor_textGrid_small(3, 3);
      snprintf_P(displayname, sizeof(displayname), PSTR("%03d"), cc_dest_values[generic_menu]);
      display.print(displayname);
      show_no_grid(4 * CHAR_height_small - 2, 11 * CHAR_width_small, 12, cc_names[generic_menu]);
    }
    else if (generic_temp_select_menu == 2)
    {

      setCursor_textGrid_small(3, 3);
      snprintf_P(displayname, sizeof(displayname), PSTR("%03d"), cc_dest_values_UI_mapping[generic_menu]);
      display.print(displayname);
      show_no_grid(4 * CHAR_height_small - 2, 11 * CHAR_width_small, 12, cc_names_UI_mapping[generic_menu]);
    }
  }
  if (LCDML.FUNC_close()) // ****** STABLE END *********
  {
    seq.midi_learn_active = false;
    display.fillScreen(COLOR_BACKGROUND);
    encoderDir[ENC_R].reset();
  }
}

// MIDI channel renderer
// just a "small bar" but showing "OMN" for zero value.
void midi_channel_renderer(Editor* editor, bool refresh)
{
  display.setTextSize(1);
  if (!refresh)
  {
    setCursor_textGrid_small(editor->x + 10, editor->y);
    display.setTextColor(GREY2, COLOR_BACKGROUND);
    display.print(editor->name);
  }
  print_small_scaled_bar(editor->x, editor->y, editor->get(), 0, 16, editor->select_id, 1, (const char*)F("OMN"));
}

// the drum parameter editors depend on the currently selected sample activesample.
// So we need custom getters and setters respecting activesample.
void addDrumParameterEditor(const char* name, int16_t limit_min, int16_t limit_max, float32_t* valuePtr)
{
  ui.addCustomEditor(
    name, limit_min, limit_max, valuePtr,
    [](Editor* editor) -> int16_t
    {
      // the parameter may be from either sample, which may be
      // switched at any time. So recompute the value pointer in respect of to activesample !
      float32_t* ptr = (float32_t*)((char*)editor->value - (char*)&drum_config[0] + (char*)&drum_config[activesample]);
      //return round(*ptr * 100.f); 
      return round(*ptr * 99.f);
    },
    [](Editor* editor, int16_t value)
    {
      // the parameter may be from either sample, which may be
      // switched at any time. So recompute the value pointer in respect of to activesample !
      float32_t* ptr = (float32_t*)((char*)editor->value - (char*)&drum_config[0] + (char*)&drum_config[activesample]);
      // *ptr = value / 100.f;
      *ptr = value / 99.f;
    },
    NULL);
}

void addDrumParameterEditor_int16_t(const char* name, int16_t limit_min, int16_t limit_max, int16_t* valuePtr)
{
  ui.addCustomEditor(
    name, limit_min, limit_max, valuePtr,
    [](Editor* editor) -> int16_t
    {
      int16_t* ptr = (int16_t*)((char*)editor->value - (char*)&drum_config[0] + (char*)&drum_config[activesample]);
      return *ptr;
    },
    [](Editor* editor, int16_t value)
    {
      int16_t* ptr = (int16_t*)((char*)editor->value - (char*)&drum_config[0] + (char*)&drum_config[activesample]);
      *ptr = value;
    },
    NULL);
}

bool drumScreenActive = false;
void drumChanged(uint8_t drumNote) {
  if (drumScreenActive) {
    activesample = constrain(drumNote, 0, NUM_DRUMSET_CONFIG - 1);

    ui.draw_editors(true);  // hack..not sure how to otherwise update all parameters when scrolling through samples

#if defined(COMPILE_FOR_FLASH)
    if (seq.running == false) {
      // this makes MDT crash often (press two keys fast one after another)
      //draw_waveform_overview_in_drums_page_step1(activesample);
    }
#endif
  }
}

// Create drum UI.
// The drum UI needs to be redrawn if activesample changes.
void UI_func_drums(uint8_t param)
{
  if (LCDML.FUNC_setup()) // ****** SETUP *********
  {
    drumScreenActive = true;
    ui.reset();
    ui.clear(); // just recreate UI without resetting selection / edit mode

    ui.setCursor(1, 1);
    // the drum sample slot selector.
    // changes activesample and redraws all other editors as they depend on.
    ui.addEditor((const char*)F(""), 0, NUM_DRUMSET_CONFIG - 2, &activesample,
      NULL, // default getter does it.
      [](Editor* editor, int16_t value)
      {
        // set the currently selected drum sample
        // and refresh all editors.
        activesample = value;
        ui.draw_editors(true);
      },
      [](Editor* editor, bool refresh)
      {
        // render the current selected drum sample slot number and name
        char number[] = "00:";
        snprintf_P(number, sizeof(number), PSTR("%02d:"), activesample);
        display.setTextSize(2);
        setModeColor(editor->select_id);
        show(editor->y, editor->x, 10, number);
        show(editor->y, editor->x + 3, 10, basename(drum_config[activesample].name));


      });

    ui.setCursor(2, 4);
    // the parameter editors depend on activesample. We define them by sample index 0,
    // but they will act on current slot activesample later.
    addDrumParameterEditor((const char*)F("VOLUME"), 0, 100, &drum_config[0].vol_max);
    addDrumParameterEditor((const char*)F("PAN"), -99, 99, &drum_config[0].pan);
    addDrumParameterEditor((const char*)F("REVERB"), 0, 100, &drum_config[0].reverb_send);
    addDrumParameterEditor((const char*)F("PITCH"), 0, 200, &drum_config[0].pitch);
    addDrumParameterEditor((const char*)F("TUNE"), 0, 200, &drum_config[0].p_offset);
    addDrumParameterEditor_int16_t((const char*)F("FILTER"), 0, 3, &drum_config[0].filter_mode);
    addDrumParameterEditor((const char*)F("FREQ"), 0, 220, &drum_config[0].filter_freq);
    addDrumParameterEditor((const char*)F("Q"), 0, 100, &drum_config[0].filter_q);

    ui.setCursor(2, 15);
    ui.addEditor((const char*)F("DRUMS MAIN VOLUME"), 0, 100,
      [](Editor* editor) -> int16_t
      {
        return round(mapfloat(seq.drums_volume, 0.0, VOL_MAX_FLOAT, 0., 100.));
      },
      [](Editor* editor, int16_t value)
      {
        seq.drums_volume = mapfloat(value, 0., 100., 0.0, VOL_MAX_FLOAT);
        master_mixer_r.gain(MASTER_MIX_CH_DRUMS, volume_transform(seq.drums_volume));
        master_mixer_l.gain(MASTER_MIX_CH_DRUMS, volume_transform(seq.drums_volume));
      });

    ui.addEditor((const char*)F("DRUMS MIDI CHANNEL"), 0, 16, &drum_midi_channel, NULL, NULL, midi_channel_renderer);

    ui.setCursor(2, 17);
    ui.addEditor((const char*)F("SMART FILTER"), 0, 1,
      [](Editor* editor) -> int16_t
      {
        return seq.smartfilter;
      },
      [](Editor* editor, int16_t value)
      {
        seq.smartfilter = !seq.smartfilter;
      });
  }
  if (LCDML.FUNC_loop()) // ****** LOOP *********
  {
    ui.handle_input();

    if (generic_temp_select_menu == 0 || generic_temp_select_menu == 6)
    {
      if (generic_temp_select_menu == 0) {
        drumChanged(activesample);
      }

      display.setTextSize(1);
      setCursor_textGrid_small(20, 9);
      if (seq.edit_state == 0) {
        display.setTextColor(COLOR_SYSTEXT, COLOR_BACKGROUND);
      }

      switch (drum_config[activesample].filter_mode) {
      case 0:
        display.print("OFF     ");
        break;
      case 1:
        display.print("LOWPASS ");
        break;
      case 2:
        display.print("BANDPASS");
        break;
      case 3:
        display.print("HIGHPASS");
        break;
      }
    }
  }
  if (LCDML.FUNC_close()) // ****** CLOSE *********
  {
    drumScreenActive = false;
    ui.clear();
  }
}

extern float get_sample_p_offset(uint8_t sample);
extern void set_sample_pitch(uint8_t sample, float playbackspeed);

FLASHMEM void pattern_editor_play_current_step(uint8_t step)
{
  if (seq.running == false && seq.note_data[seq.active_pattern][step] > 0 && step < 16)
  {
    if (seq.content_type[seq.active_pattern] == 0)
    { // Drumtrack

      if (seq.vel[seq.active_pattern][step] > 209) // it is a pitched sample
      {
        // Drum[slot]->setPlaybackRate( pow (2, (inNote - 72) / 12.00) * drum_config[sample].pitch ); get_sample_vol_max(sample)
        set_sample_pitch(seq.vel[seq.active_pattern][step] - 210, (float)pow(2, (seq.note_data[seq.active_pattern][step] - 72) / 12.00) * get_sample_p_offset(seq.vel[seq.active_pattern][step] - 210));
        handleNoteOn(drum_midi_channel, seq.vel[seq.active_pattern][step], 90, 0);
      }
      else // else play normal drum sample
        handleNoteOn(drum_midi_channel, seq.note_data[seq.active_pattern][step], seq.vel[seq.active_pattern][step], 0);
    }
    else  // is not a sample, preview note with epiano by default
    {
      handleNoteOn(configuration.epiano.midi_channel, seq.note_data[seq.active_pattern][step], seq.vel[seq.active_pattern][step], 0);

      handleNoteOff(configuration.epiano.midi_channel, seq.note_data[seq.active_pattern][step], 0, 0);
    }
  }
}

FLASHMEM void UI_func_seq_settings(uint8_t param)
{
  char displayname[4] = { 0, 0, 0, 0 };
  if (LCDML.FUNC_setup()) // ****** SETUP *********
  {
    display.fillScreen(COLOR_BACKGROUND);
    if (generic_temp_select_menu != 6) // preselect BPM when coming from song edit by quick navigation
      generic_temp_select_menu = 0;
    generic_active_function = false;
    encoderDir[ENC_R].reset();
    display.setTextSize(1);
    display.setTextColor(RED);

    setCursor_textGrid_small(1, 1);
    display.print("ADV. SEQUENCER SETTINGS");

    display.setTextColor(COLOR_SYSTEXT);
    setCursor_textGrid_small(1, 4);
    display.setTextColor(GREY2);
    display.print("ARP/CHORD TRANSPOSE");

    display.setTextColor(COLOR_SYSTEXT);
    setCursor_textGrid_small(1, 6);
    display.print("OCTAVE SHIFT -/+");
    setCursor_textGrid_small(1, 7);
    display.print("NOTE (INTERVAL) SHIFT");
    setCursor_textGrid_small(1, 8);
    display.print("MAX. CHORD NOTES");
    setCursor_textGrid_small(26, 8);
    display.setTextColor(GREY2);
    display.print(F("KEYS"));
    display.setTextColor(COLOR_SYSTEXT);
    setCursor_textGrid_small(1, 9);
    display.print("MAX. ARP NOTES/STEP");
    setCursor_textGrid_small(26, 9);
    display.setTextColor(GREY2);
    display.print(F("STEPS"));
    display.setTextColor(COLOR_SYSTEXT);
    setCursor_textGrid_small(1, 10);
    display.print(F("CHRD/ARP VELOCITY"));
    display.setTextColor(GREY2);
    setCursor_textGrid_small(1, 12);
    display.print(F("LIVE TRANSPOSE"));
    display.setTextColor(COLOR_SYSTEXT);
    setCursor_textGrid_small(1, 14);
    display.print(F("OCT. FOR LIVE TRANSP."));
    setCursor_textGrid_small(27, 14);
    display.print("-");

    display.setTextColor(GREY2);
    setCursor_textGrid_small(1, 16);
    display.print(F("SONG"));

    display.setTextColor(COLOR_SYSTEXT);
    setCursor_textGrid_small(1, 18);
    display.print(F("SONG TEMPO"));
    setCursor_textGrid_small(27, 18);
    display.print("BPM");
    display.setTextColor(GREY2);
    setCursor_textGrid_small(12, 18);
    display.print("(");
    setCursor_textGrid_small(16, 18);
    display.print("ms)");
    setCursor_textGrid_small(1, 19);
    display.print(F("PATTERN LENGTH"));
    setCursor_textGrid_small(1, 20);
    display.print(F("SWING/SHUFFLE"));
    setCursor_textGrid_small(1, 21);
    display.print(F("STEP RECORDING:"));
  }
  if (LCDML.FUNC_loop()) // ****** LOOP *********
  {
    if (generic_temp_select_menu == 0 && generic_active_function == 1)
    {
      if ((LCDML.BT_checkDown() && encoderDir[ENC_R].Down()) || (LCDML.BT_checkUp() && encoderDir[ENC_R].Up()))
      {
        if (LCDML.BT_checkDown())
          seq.oct_shift = constrain(seq.oct_shift + 1, -2, 2);
        else if (LCDML.BT_checkUp())
          seq.oct_shift = constrain(seq.oct_shift - 1, -2, 2);
      }
    }
    else if (generic_temp_select_menu == 1 && generic_active_function == 1)
    {
      if ((LCDML.BT_checkDown() && encoderDir[ENC_R].Down()) || (LCDML.BT_checkUp() && encoderDir[ENC_R].Up()))
      {
        if (LCDML.BT_checkDown())
          seq.element_shift = constrain(seq.element_shift + 1, 0, 6);
        else if (LCDML.BT_checkUp())
          seq.element_shift = constrain(seq.element_shift - 1, 0, 6);
      }
    }
    else if (generic_temp_select_menu == 2 && generic_active_function == 1)
    {
      if ((LCDML.BT_checkDown() && encoderDir[ENC_R].Down()) || (LCDML.BT_checkUp() && encoderDir[ENC_R].Up()))
      {
        if (LCDML.BT_checkDown())
          seq.chord_key_ammount = constrain(seq.chord_key_ammount + 1, 1, 7);
        else if (LCDML.BT_checkUp())
          seq.chord_key_ammount = constrain(seq.chord_key_ammount - 1, 1, 7);
      }
    }
    else if (generic_temp_select_menu == 3 && generic_active_function == 1) // edit max arp notes in 1/32 1/64 arps
    {
      if ((LCDML.BT_checkDown() && encoderDir[ENC_R].Down()) || (LCDML.BT_checkUp() && encoderDir[ENC_R].Up()))
      {
        if (LCDML.BT_checkDown())
          seq.arp_num_notes_max = constrain(seq.arp_num_notes_max + 1, 1, 64);
        else if (LCDML.BT_checkUp())
          seq.arp_num_notes_max = constrain(seq.arp_num_notes_max - 1, 1, 64);
      }
    }
    else if (generic_temp_select_menu == 4 && generic_active_function == 1) // Chord Velocity
    {
      if ((LCDML.BT_checkDown() && encoderDir[ENC_R].Down()) || (LCDML.BT_checkUp() && encoderDir[ENC_R].Up()))
      {
        if (LCDML.BT_checkDown())
          seq.chord_vel = constrain(seq.chord_vel + 1, 10, 120);
        else if (LCDML.BT_checkUp())
          seq.chord_vel = constrain(seq.chord_vel - 1, 10, 120);
      }
    }

    else if (generic_temp_select_menu == 5 && generic_active_function == 1)
    {
      if ((LCDML.BT_checkDown() && encoderDir[ENC_R].Down()) || (LCDML.BT_checkUp() && encoderDir[ENC_R].Up()))
      {
        if (LCDML.BT_checkDown())
          seq.transpose = constrain(seq.transpose + 1, 0, 60);
        else if (LCDML.BT_checkUp())
          seq.transpose = constrain(seq.transpose - 1, 0, 60);
      }
    }
    else if (generic_temp_select_menu == 6 && generic_active_function == 1)
    {
      if ((LCDML.BT_checkDown() && encoderDir[ENC_R].Down()) || (LCDML.BT_checkUp() && encoderDir[ENC_R].Up()))
      {
        if (LCDML.BT_checkDown())
          seq.bpm = constrain(seq.bpm + 1, 40, 190);
        else if (LCDML.BT_checkUp())
          seq.bpm = constrain(seq.bpm - 1, 40, 190);
      }
    }
    else if (generic_temp_select_menu == 7 && generic_active_function == 1)
    {
      if ((LCDML.BT_checkDown() && encoderDir[ENC_R].Down()) || (LCDML.BT_checkUp() && encoderDir[ENC_R].Up()))
      {
        if (LCDML.BT_checkDown())
          seq.pattern_len_dec = constrain(seq.pattern_len_dec - 1, 0, 8);  // dir reversed as this is decreased from 16
        else if (LCDML.BT_checkUp())
          seq.pattern_len_dec = constrain(seq.pattern_len_dec + 1, 0, 8);// dir reversed as this is decreased from 16
      }
    }
    else if (generic_temp_select_menu == 8 && generic_active_function == 1)
    {
      if ((LCDML.BT_checkDown() && encoderDir[ENC_R].Down()) || (LCDML.BT_checkUp() && encoderDir[ENC_R].Up()))
      {
        if (LCDML.BT_checkDown())
          seq.swing_steps = constrain(seq.swing_steps + 1, 0, 2);
        else if (LCDML.BT_checkUp())
          seq.swing_steps = constrain(seq.swing_steps - 1, 0, 2);
      }
    }
    else if (generic_temp_select_menu == 9 && generic_active_function == 1)
    {
      if ((LCDML.BT_checkDown() && encoderDir[ENC_R].Down()) || (LCDML.BT_checkUp() && encoderDir[ENC_R].Up()))
      {
        if (LCDML.BT_checkDown())
          seq.auto_advance_step = constrain(seq.auto_advance_step + 1, 0, 2);
        else if (LCDML.BT_checkUp())
          seq.auto_advance_step = constrain(seq.auto_advance_step - 1, 0, 2);
      }
    }
    // -------------------------------------------------------------------------------------------------------------------------
    else if (generic_active_function == 0)
    {
      if ((LCDML.BT_checkDown() && encoderDir[ENC_R].Down()) || (LCDML.BT_checkUp() && encoderDir[ENC_R].Up()))
      {
        if (LCDML.BT_checkDown())
          generic_temp_select_menu = constrain(generic_temp_select_menu + 1, 0, 9);
        else if (LCDML.BT_checkUp())
          generic_temp_select_menu = constrain(generic_temp_select_menu - 1, 0, 9);
      }
    }
    if (LCDML.BT_checkEnter())
    {
      if (generic_active_function == 0)
        generic_active_function = 1;
      else
        generic_active_function = 0;
    }
    setModeColor(0);
    setCursor_textGrid_small(23, 6);
    snprintf_P(displayname, sizeof(displayname), PSTR("%02d"), seq.oct_shift);
    display.print(displayname);
    setModeColor(1);
    setCursor_textGrid_small(23, 7);
    display.print(seq.element_shift);
    setModeColor(2);
    setCursor_textGrid_small(23, 8);
    snprintf_P(displayname, sizeof(displayname), PSTR("%02d"), seq.chord_key_ammount);
    display.print(displayname);
    setModeColor(3);
    setCursor_textGrid_small(23, 9);
    snprintf_P(displayname, sizeof(displayname), PSTR("%02d"), seq.arp_num_notes_max);
    display.print(displayname);
    setModeColor(4);
    setCursor_textGrid_small(23, 10);
    print_formatted_number(seq.chord_vel, 3);
    setModeColor(5);
    setCursor_textGrid_small(23, 14);
    if (seq.transpose != 0)
    {
      getNoteName(displayname, seq.transpose);
      display.print(displayname);
      setCursor_textGrid_small(29, 14);
      getNoteName(displayname, seq.transpose + 12);
      display.print(displayname);
    }
    else
    {
      display.print(F("OFF"));
      setCursor_textGrid_small(29, 14);
      display.print(F("OFF"));
    }
    display.setTextColor(GREY2, COLOR_BACKGROUND);
    setCursor_textGrid_small(13, 18);
    snprintf_P(displayname, sizeof(displayname), PSTR("%03d"), seq.tempo_ms / 1000);
    display.print(displayname);
    setModeColor(6);
    setCursor_textGrid_small(23, 18);
    snprintf_P(displayname, sizeof(displayname), PSTR("%03d"), seq.bpm);
    display.print(displayname);

    if (generic_temp_select_menu == 6 && generic_active_function)
      update_seq_speed();

    setModeColor(7);
    setCursor_textGrid_small(23, 19);
    print_formatted_number(16 - seq.pattern_len_dec, 2);

    setModeColor(8);
    // setCursor_textGrid_small(23, 20);
    //print_formatted_number( seq.swing_steps, 1);
    setCursor_textGrid_small(23, 20);
    if (seq.swing_steps == 0)
      display.print(F("NONE    "));
    else if (seq.swing_steps == 1)
      display.print(F("SHUFFLE "));
    else if (seq.swing_steps == 2)
      display.print(F("TRIPLETS"));
    //  else   
    //   display.print(F("OTHER   "));

    setModeColor(9);
    setCursor_textGrid_small(23, 21);
    if (seq.auto_advance_step == 1)
      display.print(F("AUTO ADVANCE STEP"));
    else if (seq.auto_advance_step == 2)
      display.print(F("AUTO ADVANCE+STOP"));
    else
      display.print(F("KEEP CURRENT STEP"));
  }
  if (LCDML.FUNC_close()) // ****** STABLE END *********
  {
    encoderDir[ENC_R].reset();
    display.fillScreen(COLOR_BACKGROUND);
  }
}

void UI_func_drum_pan(uint8_t param)
{
  char displayname[8] = { 0, 0, 0, 0, 0, 0, 0 };
  if (LCDML.FUNC_setup()) // ****** SETUP *********
  {
    encoderDir[ENC_R].reset();
    temp_int = mapfloat(drum_config[activesample].pan, -1.0, 1.0, -99, 99);
    setCursor_textGrid(1, 1);
    display.print(F("DrmSmp. Panorama"));
    setCursor_textGrid(2, 2);
    snprintf_P(displayname, sizeof(displayname), PSTR("%02d"), activesample);
    display.print(displayname);
    show(2, 5, 6, basename(drum_config[activesample].name));
  }
  if (LCDML.FUNC_loop()) // ****** LOOP *********
  {
    if (menu_select_toggle == false)
    {
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
    }
    else
    {
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
      if (menu_select_toggle)
      {
        menu_select_toggle = false;
      }
      else
      {
        menu_select_toggle = true;
      }
    }
    if (menu_select_toggle == false)
    {
      setCursor_textGrid(12, 2);
      display.print(" ");
      setCursor_textGrid(16, 2);
      display.print(" ");
      setCursor_textGrid(1, 2);
      display.print("[");
      setCursor_textGrid(4, 2);
      display.print("]");
      setCursor_textGrid(2, 2);
      snprintf_P(displayname, sizeof(displayname), PSTR("%02d"), activesample);
      display.print(displayname);
      show(2, 5, 6, basename(drum_config[activesample].name));
    }
    else
    {
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
    if (temp_int > 1)
    {
      display.print("R");
    }
    else if (temp_int < 0)
    {
      display.print("L");
    }
    else
    {
      display.print("C");
    }
    snprintf_P(displayname, sizeof(displayname), PSTR("%02d"), abs(temp_int));
    setCursor_textGrid(14, 2);
    display.print(displayname);
  }
  if (LCDML.FUNC_close()) // ****** STABLE END *********
  {
    encoderDir[ENC_R].reset();
  }
}

void set_pattern_content_type_color_with_green_highlight(uint8_t pattern)
{
  if (pattern == seq.active_pattern)
  {
    if (seq.content_type[pattern] == 0) // Drumpattern
      display.setTextColor(COLOR_DRUMS, DARKGREEN);
    else if (seq.content_type[pattern] == 1) // Instrument Pattern
      display.setTextColor(COLOR_INSTR, DARKGREEN);
    else if (seq.content_type[pattern] == 2) //  chord  pattern
      display.setTextColor(COLOR_CHORDS, DARKGREEN);
    else if (seq.content_type[pattern] == 3) //   arp pattern
      display.setTextColor(COLOR_ARP, DARKGREEN);
  }
  else
  {
    if (seq.content_type[pattern] == 0) // Drumpattern
      display.setTextColor(COLOR_DRUMS, COLOR_BACKGROUND);
    else if (seq.content_type[pattern] == 1) // Instrument Pattern
      display.setTextColor(COLOR_INSTR, COLOR_BACKGROUND);
    else if (seq.content_type[pattern] == 2) //  chord  pattern
      display.setTextColor(COLOR_CHORDS, COLOR_BACKGROUND);
    else if (seq.content_type[pattern] == 3) //   arp pattern
      display.setTextColor(COLOR_ARP, COLOR_BACKGROUND);
  }
}

void set_pattern_content_type_color_inverted(uint8_t pattern)
{
  if (seq.content_type[pattern] == 0) // Drumpattern
    display.setTextColor(COLOR_BACKGROUND, COLOR_DRUMS);
  else if (seq.content_type[pattern] == 1) // Instrument Pattern
    display.setTextColor(COLOR_BACKGROUND, COLOR_INSTR);
  else if (seq.content_type[pattern] == 2) //  chord  pattern
    display.setTextColor(COLOR_BACKGROUND, COLOR_CHORDS);
  else if (seq.content_type[pattern] == 3) //  arp pattern
    display.setTextColor(COLOR_BACKGROUND, COLOR_ARP);
}

void print_performance_name(int x, int y)
{
  display.setTextSize(1);
  display.setTextColor(COLOR_PITCHSMP, COLOR_BACKGROUND);
  // display.setCursor(CHAR_width_small * 36,   11 * (CHAR_height_small + 2) + 10);
  display.setCursor(x, y);
  print_formatted_number(configuration.sys.performance_number, 2);
  display.print(":");
  display.setTextColor(COLOR_PITCHSMP, COLOR_BACKGROUND);
  display.print("[");
  display.setTextColor(COLOR_SYSTEXT, COLOR_BACKGROUND);
  display.print(seq.name);
  display.setTextColor(COLOR_PITCHSMP, COLOR_BACKGROUND);
  display.print("]");
  display.setTextSize(2);
}

void seq_printAllSeqSteps()
{
  setCursor_textGrid(0, 1);
  for (uint8_t i = 0; i < 16; i++)
  {
    if (i > 15 - seq.pattern_len_dec) // pattern is shorter than default 16 steps
      display.setTextColor(GREY3, COLOR_BACKGROUND);
    else if (seq.vel[seq.active_pattern][i] > 209)
      display.setTextColor(COLOR_PITCHSMP, COLOR_BACKGROUND);
    else
      set_pattern_content_type_color(seq.active_pattern);
    display.print(seq_find_shortname(i)[0]);
  }
}

// void seq_sub_print_track_assignments(int x, int y, bool init)
//{
//   if (seq.cycle_touch_element == 0)  // touch keyboard is off
//   {
//     display.setTextSize(1);
//     display.setTextColor(GREY2, COLOR_BACKGROUND);
//     for (uint8_t track = 0; track < NUM_SEQ_TRACKS; track++)
//     {
//       display.setCursor(CHAR_width_small * 36,  (track + 13) * (CHAR_height_small + 1) + 20  );
//       if (seq.menu - 21 == track)
//         set_track_type_color_inverted(track);
//       else
//         set_track_type_color(track);
//       display.print(F("T"));
//       display.print (track + 1);
//       display.setCursor(CHAR_width_small * 36 + 3 * CHAR_width_small,   (track + 13) * (CHAR_height_small + 1) + 20  );
//       if (seq.menu - 21 - NUM_SEQ_TRACKS == track)
//         set_track_type_color_inverted(track);
//       else
//         set_track_type_color(track);
//       if (seq.track_type[track] > 0 && seq.instrument[track] < 2)
//       {
//         display.print ("DEXED INST.");
//         display.print (seq.instrument[track] + 1);
//       }
//       else if (seq.track_type[track] > 0 && seq.instrument[track] == 2) //epiano
//       {
//         display.print ("ELECTR.PIANO");
//       }
//       else if (seq.track_type[track] > 0 && seq.instrument[track] == 3)  //MicroSynth 0
//       {
//         display.print ("MICROSYNTH 1");
//       }
//       else if (seq.track_type[track] > 0 && seq.instrument[track] == 4)  //MicroSynth 1
//       {
//         display.print ("MICROSYNTH 2");
//       }
//       else if (seq.instrument[track] == 5) //Braids
//       {
//         display.print ("BRAIDS OSC");
//       }
//       else if ( seq.instrument[track] > 5 && seq.instrument[track] < 16) //multisamples
//       {
//         display.print ("MULTISAMPLE");
//       }
//       else if (seq.instrument[track] > 15 && seq.instrument[track] < 32) //external MIDI USB
//       {
//         display.print ("MIDI USB #");
//         print_formatted_number(seq.instrument[track] - 15, 2);
//       }
//       else if (seq.instrument[track] > 31 && seq.instrument[track] < 48) //external MIDI MINI JACK/DIN
//       {
//         display.print ("MIDI DIN #");
//         print_formatted_number(seq.instrument[track] - 31, 2);
//       }
//       else
//       {
//         display.print ("-----------");
//       }
//     }
//   }
// }

void seq_printVelGraphBar()
{
  if (remote_active)
    display.console = true;
  display.fillRect(0, 2 * CHAR_height, 17 * CHAR_width, 17, COLOR_BACKGROUND);
  for (uint8_t i = 0; i < 16; i++)
  {
    if (seq.vel[seq.active_pattern][i] < 130)
    {
      display.drawLine(0 + i * CHAR_width + 3, CHAR_height * 3 - 3, 0 + i * CHAR_width + 3, CHAR_height * 3 - 3 - (seq.vel[seq.active_pattern][i] / 10), GREY1);
      display.drawLine(0 + i * CHAR_width + 4, CHAR_height * 3 - 3, 0 + i * CHAR_width + 4, CHAR_height * 3 - 3 - (seq.vel[seq.active_pattern][i] / 10), GREY1);
      display.drawLine(0 + i * CHAR_width + 5, CHAR_height * 3 - 3, 0 + i * CHAR_width + 5, CHAR_height * 3 - 3 - (seq.vel[seq.active_pattern][i] / 10), GREY1);
    }
    else
    {
      if (seq.vel[seq.active_pattern][i] > 209)
      {
        drawBitmap((i - 1) * CHAR_width + 13, 2 * CHAR_height + 7, special_chars[22], 8, 8, COLOR_PITCHSMP);
      }
    }
  }
}

void seq_printVelGraphBar_single_step(uint8_t step, int color)
{
  if (remote_active)
    display.console = true;
  display.fillRect(0 + step * CHAR_width + 1, CHAR_height * 2 + 1, 8, 14, COLOR_BACKGROUND);
  if (seq.vel[seq.active_pattern][step] < 130)
  {
    display.drawLine(0 + step * CHAR_width + 3, CHAR_height * 3 - 3, 0 + step * CHAR_width + 3, CHAR_height * 3 - 3 - (seq.vel[seq.active_pattern][step] / 10), color);
    display.drawLine(0 + step * CHAR_width + 4, CHAR_height * 3 - 3, 0 + step * CHAR_width + 4, CHAR_height * 3 - 3 - (seq.vel[seq.active_pattern][step] / 10), color);
    display.drawLine(0 + step * CHAR_width + 5, CHAR_height * 3 - 3, 0 + step * CHAR_width + 5, CHAR_height * 3 - 3 - (seq.vel[seq.active_pattern][step] / 10), color);
  }
  else
  {
    if (seq.vel[seq.active_pattern][step] > 209)
    {
      drawBitmap((step - 1) * CHAR_width + 13, 2 * CHAR_height + 7, special_chars[22], 8, 8, COLOR_PITCHSMP);
    }
  }
}


// #if defined(COMPILE_FOR_PROGMEM)
// void draw_waveform_overview_in_sample_editor_step1(int samplenumber) // for progmem
// {
//   int xspace = 0;
//   int i = 4;
//   short samplevalue = 0;
//   int oldx = 0, oldy = 180;
//   display.setTextSize(1);
//   setCursor_textGrid(21, 3);
//   display.setTextColor(COLOR_SYSTEXT, COLOR_PITCHSMP);
//   display.print("SCALE");
//   setCursor_textGrid(21, 4);
//   display.setTextColor(COLOR_SYSTEXT, COLOR_BACKGROUND);
//   display.print(seq.wave_spacing);
//   display.print(" ");
//   display.fillRect(0, 100, DISPLAY_WIDTH - 1, 100, COLOR_BACKGROUND);
//   xspace = 0;
//   i = 4;
//   samplevalue = 0;
//   oldx = 0;
//   oldy = 180;
//   do
//   {
//     {
//       samplevalue = (((DRUM_SQBass[i * seq.wave_spacing + 1]) * 256) + DRUM_SQBass[i * seq.wave_spacing]) / 710 + 100;

//       if (DRUM_SQBass[i * seq.wave_spacing + 1] < 128)
//       {
//         display.drawLine(oldx, oldy, xspace, samplevalue + 60, COLOR_SYSTEXT);
//         oldx = xspace;
//         oldy = samplevalue + 60;
//       }
//       else
//       {
//         display.drawLine(oldx, oldy, xspace, samplevalue - 40, COLOR_SYSTEXT);
//         oldx = xspace;
//         oldy = samplevalue - 40;
//       }
//       xspace = xspace + 1;
//       i++;
//     }
//   } while (oldx < DISPLAY_WIDTH);
// }
// #endif

bool wave_editor_skip_redraw_overview = false;

//#if defined(COMPILE_FOR_FLASH)


void draw_end_marking(uint16_t xspace)
{
  if (xspace < DISPLAY_WIDTH - 1)
  {
    display.console = true;
    display.fillRect(xspace + 1, 72, DISPLAY_WIDTH - xspace - 2, 101, COLOR_BACKGROUND);
    display.drawLine(xspace, 74, xspace, 170, RED);
    display.drawLine(xspace, 120, DISPLAY_WIDTH - 1, 120, GREY2);
  }
  if (xspace < DISPLAY_WIDTH - CHAR_width_small * 3 - 3)
  {
    display.setCursor(xspace + 3, 173 - CHAR_height_small);
    display.print("END");
    sample_editor_scale_end = xspace + 3;
  }
}

FLASHMEM void draw_waveform_overview_in_sample_editor_step2(uint8_t overview_buffer[], int end_zoomed_x_position, unsigned long filelength)
{
  if (wave_editor_skip_redraw_overview == false)
  {
    uint8_t xspace = 0;
    uint32_t current_pos = 0;
    int oldy = 50;

    uint16_t overview_factor = 0;

    overview_factor = (filelength / 140);
    if (overview_factor % 2 != 0)
      overview_factor = overview_factor - 1;
    display.console = true;
    if ((end_zoomed_x_position / overview_factor) + 169 < DISPLAY_WIDTH - 12 && end_zoomed_x_position / overview_factor > 169)
      display.fillRect(169, 31, end_zoomed_x_position / overview_factor, 38, GREY2);
    else
      display.fillRect(169, 31, DISPLAY_WIDTH - 169 - 11, 38, GREY2);

    if (169 + end_zoomed_x_position / overview_factor < DISPLAY_WIDTH - 30)
      display.fillRect(169 + end_zoomed_x_position / overview_factor, 31, 140 - end_zoomed_x_position / overview_factor, 38, GREY4);

    while (xspace < 140) // draw overview
    {
      display.drawLine(169 + (xspace - 1), oldy, 169 + xspace, overview_buffer[xspace], COLOR_SYSTEXT);
      oldy = overview_buffer[xspace];
      xspace = xspace + 1;
      current_pos = current_pos + overview_factor;
    }
  }
}

FLASHMEM void sample_editor_update_file_counts()
{
  if (fm.sample_source == 1) // source = FLASH
  {
    SerialFlash.opendir();
    while (1)
    {
      char filename[25];
      uint32_t filesize;

      if (SerialFlash.readdir(filename, sizeof(filename), filesize))
      {
        fm.flash_sum_files++;
      }
      else
      {
        break; // no more files
      }
    }
  }
  else if (fm.sample_source == 0) // source = SD CARD
  {
    sd_card_count_files_from_directory("/DRUMS");
  }
}

FLASHMEM void print_available_loop_options()
{
  if (fm.sample_source == 2 || fm.sample_source == 3)
    display.setTextColor(COLOR_SYSTEXT, COLOR_BACKGROUND);
  else
    display.setTextColor(GREY2, COLOR_BACKGROUND);
  setCursor_textGrid_small(1, 4);
  display.print(F("LOOP START:"));
  setCursor_textGrid_small(1, 5);
  display.print(F("LOOP END:"));
  setCursor_textGrid_small(1, 6);
  display.print(F("LOOP TYPE:"));
}


FLASHMEM void draw_waveform_overview_in_sample_editor_step1(int samplenumber)
{
  display.console = true;
  uint8_t overview_buffer[142];
  uint16_t xspace = 1;
  short samplevalue = 0;
  uint32_t current_pos = 0;
  int oldy = 122;
  int end_zoomed_x_position = 0;
  uint16_t overview_factor = 0;
  char filename[26];
  uint32_t filesize;

  if ((seq.edit_state == 1 && generic_temp_select_menu == 3) ||
    (seq.edit_state == 1 && generic_temp_select_menu == 4) || generic_temp_select_menu == 5)
    wave_editor_skip_redraw_overview = true;
  else
    wave_editor_skip_redraw_overview = false;


  if (fm.sample_source == 1) // FLASH
  {
    SerialFlash.opendir();
    if (samplenumber > 0)
    {
      for (int f = 0; f < samplenumber; f++)
      {
        if (SerialFlash.readdir(filename, sizeof(filename), filesize))
          ;
        else
          break;
      }
    }
    SerialFlashFile f = SerialFlash.open(filename);

    show_smallfont_noGrid(CHAR_height_small + 2, CHAR_width_small * 14, 16, filename);
    unsigned long filelength = f.size();
    setCursor_textGrid_small(32, 1);
    display.setTextColor(GREY1, COLOR_BACKGROUND);
    display.print(filelength / 1024);
    display.setTextColor(GREY2, COLOR_BACKGROUND);
    display.print(" KB  ");
    f.seek(44);
    char buf[256];
    overview_factor = get_overview_factor(filelength);

    oldy = 120;
    while (xspace < DISPLAY_WIDTH - 1 && current_pos + 44 + 256 < filelength)
    {
      f.read(buf, 256);
      samplevalue = ((buf[1] * 256) + buf[0]);

      display.drawLine(xspace, 72, xspace, 172, COLOR_BACKGROUND);

      display.drawLine(xspace - 1, oldy, xspace, samplevalue / 700 + 120, GREEN);

      oldy = samplevalue / 700 + 120;
      xspace = xspace + 1;

      current_pos = current_pos + seq.wave_spacing * seq.wave_spacing;
      f.seek(44 + current_pos);
    }
    end_zoomed_x_position = current_pos;
    draw_end_marking(xspace);
    xspace = 0;
    current_pos = 0;
    while (xspace < 140) // generate overview
    {
      f.read(buf, 256);
      samplevalue = ((buf[1] * 256) + buf[0]);
      overview_buffer[xspace] = samplevalue / 1800 + 50;
      xspace = xspace + 1;
      current_pos = current_pos + overview_factor;
      f.seek(44 + current_pos);
    }
    draw_waveform_overview_in_sample_editor_step2(overview_buffer, end_zoomed_x_position, filelength);

    f.close();
  }
  else if (fm.sample_source == 0) // SD
  {
    File f;
    File myDir = SD.open("/DRUMS");
    for (uint16_t i = 0; i < samplenumber; i++)
    {
      f.close();
      f = myDir.openNextFile();
      // if (! f)  break;
    }

    show_smallfont_noGrid(CHAR_height_small + 2, CHAR_width_small * 14, 16, f.name());
    unsigned long filelength = f.size();
    setCursor_textGrid_small(32, 1);
    display.setTextColor(GREY1, COLOR_BACKGROUND);
    display.print(filelength / 1024);
    display.setTextColor(GREY2, COLOR_BACKGROUND);
    display.print(" KB  ");
    f.seek(44);
    char buf[256];
    overview_factor = get_overview_factor(filelength);

    oldy = 120;
    while (xspace < DISPLAY_WIDTH - 1 && current_pos + 44 + 256 < filelength)
    {
      f.read(buf, 256);
      samplevalue = ((buf[1] * 256) + buf[0]);

      display.drawLine(xspace, 72, xspace, 172, COLOR_BACKGROUND);
      display.drawLine(xspace - 1, oldy, xspace, samplevalue / 700 + 120, GREEN);

      oldy = samplevalue / 700 + 120;
      xspace = xspace + 1;

      current_pos = current_pos + seq.wave_spacing * seq.wave_spacing;
      f.seek(44 + current_pos);
    }
    end_zoomed_x_position = current_pos;
    draw_end_marking(xspace);
    xspace = 0;
    current_pos = 0;
    while (xspace < 140) // generate overview
    {
      f.read(buf, 256);
      samplevalue = ((buf[1] * 256) + buf[0]);
      overview_buffer[xspace] = samplevalue / 1800 + 50;
      xspace = xspace + 1;
      current_pos = current_pos + overview_factor;
      f.seek(44 + current_pos);
    }
    draw_waveform_overview_in_sample_editor_step2(overview_buffer, end_zoomed_x_position, filelength);

    f.close();
    myDir.close();
  }

  if (fm.sample_source > 1 && fm.sample_source < 4) // MSP
  {
    SerialFlashFile f = SerialFlash.open(msz[fm.sample_source - 2][samplenumber].filename);

    show_smallfont_noGrid(CHAR_height_small + 2, CHAR_width_small * 14, 16, msz[fm.sample_source - 2][samplenumber].filename);
    unsigned long filelength = f.size();
    msz[fm.sample_source - 2][samplenumber].filelength = filelength;
    setCursor_textGrid_small(32, 1);
    display.setTextColor(GREY1, COLOR_BACKGROUND);
    display.print(filelength / 1024);
    display.setTextColor(GREY2, COLOR_BACKGROUND);
    display.print(" KB  ");
    f.seek(44);
    char buf[256];
    oldy = 120;
    while (xspace < DISPLAY_WIDTH - 1 && current_pos + 44 + 256 < filelength)
    {
      f.read(buf, 256);
      samplevalue = ((buf[1] * 256) + buf[0]);

      if (current_pos >= msz[fm.sample_source - 2][samplenumber].loop_start && current_pos <= msz[fm.sample_source - 2][samplenumber].loop_end)
      {
        if (msz[fm.sample_source - 2][samplenumber].loop_start != 0 || msz[fm.sample_source - 2][samplenumber].loop_end != 0)
        {
          display.drawLine(xspace, 72, xspace, 172, DARKGREEN);
        }
      }
      else
      {
        display.drawLine(xspace, 72, xspace, 172, COLOR_BACKGROUND);
      }
      display.drawLine(xspace - 1, oldy, xspace, samplevalue / 700 + 120, GREEN);

      oldy = samplevalue / 700 + 120;
      xspace = xspace + 1;

      current_pos = current_pos + seq.wave_spacing * seq.wave_spacing;
      f.seek(44 + current_pos);
    }
    end_zoomed_x_position = current_pos;
    draw_end_marking(xspace);

    if (generic_temp_select_menu != 5 && generic_temp_select_menu != 4 && generic_temp_select_menu != 3)
    {
      overview_factor = get_overview_factor(filelength);
      xspace = 0;
      current_pos = 0;
      while (xspace < 140) // generate overview
      {
        f.read(buf, 256);
        samplevalue = ((buf[1] * 256) + buf[0]);
        overview_buffer[xspace] = samplevalue / 1800 + 50;
        xspace = xspace + 1;
        current_pos = current_pos + overview_factor;
        f.seek(44 + current_pos);
      }
      draw_waveform_overview_in_sample_editor_step2(overview_buffer, end_zoomed_x_position, filelength);
    }

    f.close();
  }
}


FLASHMEM void UI_func_sample_editor(uint8_t param)
{
  if (LCDML.FUNC_setup()) // ****** SETUP *********
  {
    // setup function

#ifdef COMPILE_FOR_PROGMEM
    fm.sample_source = 0;
#endif

#ifdef COMPILE_FOR_SDCARD
    fm.sample_source = 0;
#endif

#ifdef COMPILE_FOR_FLASH
    fm.sample_source = 2;//default to MSP0
#endif

    menuhelper_redraw = true;
    temp_int = 0;
    fm.flash_sum_files = 0;
    fm.sd_sum_files = 0;
    seq.edit_state = 0;
    generic_temp_select_menu = 0;
    encoderDir[ENC_R].reset();
    display.fillScreen(COLOR_BACKGROUND);
    border0();
    helptext_l(back_text);
    helptext_r("SELECT SAMPLE");
    display.setTextSize(1);
    setCursor_textGrid_small(1, 1);
    display.setTextColor(COLOR_SYSTEXT, COLOR_BACKGROUND);
    display.print(F("SAMPLE"));
    display.setTextColor(COLOR_SYSTEXT, COLOR_BACKGROUND);
    setCursor_textGrid_small(42, 1);
    display.print("SCALE:");

    setCursor_textGrid_small(1, 3);
    display.setTextColor(COLOR_SYSTEXT, COLOR_BACKGROUND);
    display.print(F("SOURCE:"));

    display.console = true;
    display.drawRect(168, 30, 142, 40, GREY2); //overview

    draw_button_on_grid(1, 23, "SAMPLE", "START", 0);
    draw_button_on_grid(9, 23, "SAMPLE", "END", 0);

    draw_button_on_grid(17, 23, "LOOP", "START", 0);
    draw_button_on_grid(25, 23, "LOOP", "END", 0);
    draw_button_on_grid(45, 23, "PLAY", "SAMPLE", 0);

    sample_editor_update_file_counts();
    print_available_loop_options();
  }
  if (LCDML.FUNC_loop()) // ****** LOOP *********
  {
    if ((LCDML.BT_checkDown() && encoderDir[ENC_R].Down()) || (LCDML.BT_checkUp() && encoderDir[ENC_R].Up()))
    {
      set_state_dir();

      if (seq.edit_state == 0)
      {
        if (fm.sample_source == 2 || fm.sample_source == 3)
          generic_temp_select_menu = calc_val[state_dir](generic_temp_select_menu, 1, 0, 5);
        else
          generic_temp_select_menu = calc_val[state_dir](generic_temp_select_menu, 1, 0, 2);

        menuhelper_redraw = false;
      }

      else if (seq.edit_state == 1 && generic_temp_select_menu == 0)
      {
        stop_all_drum_slots();
        if (fm.sample_source == 0)
          temp_int = calc_val[state_dir](temp_int, 1, 0, fm.sd_sum_files - 1);
        else if (fm.sample_source == 1)
          temp_int = calc_val[state_dir](temp_int, 1, 0, fm.flash_sum_files - 1);
        else if (fm.sample_source == 2 || fm.sample_source == 3)
          temp_int = calc_val[state_dir](temp_int, 1, 0, 7);
      }
      else if (seq.edit_state == 1 && generic_temp_select_menu == 1)
      {
        seq.wave_spacing = calc_val[state_dir](seq.wave_spacing, 2, 2, 96);
      }
      else if (seq.edit_state == 1 && generic_temp_select_menu == 2)
      {
        stop_all_drum_slots();
#ifdef COMPILE_FOR_PROGMEM
        fm.sample_source = 0;
#endif

#ifdef COMPILE_FOR_SDCARD
        fm.sample_source = 0;
#endif

#ifdef COMPILE_FOR_FLASH
        fm.sample_source = calc_val[state_dir](fm.sample_source, 1, 0, 3);
#endif
        sample_editor_update_file_counts();
        print_available_loop_options();
        if (fm.sample_source != menuhelper_previous_val)
        {
          menuhelper_previous_val = fm.sample_source;
          menuhelper_redraw = true;
        }
      }
      else if (seq.edit_state == 1 && generic_temp_select_menu == 3 && fm.sample_source > 1)
      {
        stop_all_drum_slots();
        msz[fm.sample_source - 2][temp_int].loop_start = calc_val[state_dir](msz[fm.sample_source - 2][temp_int].loop_start, ENCODER[ENC_R].speed()* seq.wave_spacing * 10, 0, msz[fm.sample_source - 2][temp_int].filelength);
        if (msz[fm.sample_source - 2][temp_int].loop_end <= msz[fm.sample_source - 2][temp_int].loop_start)
          msz[fm.sample_source - 2][temp_int].loop_end = msz[fm.sample_source - 2][temp_int].loop_start + 1;
      }
      else if (seq.edit_state == 1 && generic_temp_select_menu == 4 && fm.sample_source > 1)
      {
        stop_all_drum_slots();
        msz[fm.sample_source - 2][temp_int].loop_end = calc_val[state_dir](msz[fm.sample_source - 2][temp_int].loop_end, ENCODER[ENC_R].speed()* seq.wave_spacing * 10, 0, msz[fm.sample_source - 2][temp_int].filelength);
        if (msz[fm.sample_source - 2][temp_int].loop_end < msz[fm.sample_source - 2][temp_int].loop_start)
          msz[fm.sample_source - 2][temp_int].loop_start = msz[fm.sample_source - 2][temp_int].loop_end - 1;
        if (msz[fm.sample_source - 2][temp_int].loop_start < 0 || msz[fm.sample_source - 2][temp_int].loop_start>msz[fm.sample_source - 2][temp_int].loop_end)
          msz[fm.sample_source - 2][temp_int].loop_start = 0;
      }
      else if (seq.edit_state == 1 && generic_temp_select_menu == 5 && fm.sample_source > 1)
      {
        stop_all_drum_slots();
        msz[fm.sample_source - 2][temp_int].loop_type = calc_val[state_dir](msz[fm.sample_source - 2][temp_int].loop_type, 1, 0, 2);
      }

    }

    if (LCDML.BT_checkEnter() && encoderDir[ENC_R].ButtonShort())// handle button presses during menu >>>>>>>>>>>>>>>>
    {
      stop_all_drum_slots();
      if (seq.edit_state == 0)
        seq.edit_state = 1;
      else
        seq.edit_state = 0;

    }
    // button check end <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<

    setCursor_textGrid_small(9, 1);
    setModeColor(0);
    print_formatted_number(temp_int + 1, 3);
    setModeColor(1);
    setCursor_textGrid_small(49, 1);
    print_formatted_number(seq.wave_spacing / 2, 2);
    setModeColor(2);
    setCursor_textGrid_small(10, 3);
    if (fm.sample_source == 0)
      display.print(F("SD - CARD"));
    else if (fm.sample_source == 1)
      display.print(F("EXT.FLASH"));
    else if (fm.sample_source == 2)
      display.print(F("MSP #0   "));
    else if (fm.sample_source == 3)
      display.print(F("MSP #1   "));

    if (fm.sample_source == 2 || fm.sample_source == 3)  //loop available
    {
      setModeColor(3);
      show_no_grid(4 * 11 - 5, CHAR_width_small * 13, 7, msz[fm.sample_source - 2][temp_int].loop_start);
      setModeColor(4);
      show_no_grid(5 * 11 - 5, CHAR_width_small * 13, 7, msz[fm.sample_source - 2][temp_int].loop_end);
      setModeColor(5);
      setCursor_textGrid_small(13, 6);
      if (msz[fm.sample_source - 2][temp_int].loop_type == 0)
        display.print(F("NO LOOP "));
      else    if (msz[fm.sample_source - 2][temp_int].loop_type == 1)
        display.print(F("REPEAT  "));
      else    if (msz[fm.sample_source - 2][temp_int].loop_type == 2)
        display.print(F("PINGPONG"));
    }
    else
    { //no loop available
      display.setTextColor(GREY2, COLOR_BACKGROUND);
      show_no_grid(4 * 11 - 5, CHAR_width_small * 13, 7, "0");
      show_no_grid(5 * 11 - 5, CHAR_width_small * 13, 7, "0");
      setCursor_textGrid_small(13, 6);
      display.print(F("NO LOOP "));
    }
    display.setTextColor(COLOR_SYSTEXT, COLOR_BACKGROUND);
#if defined(COMPILE_FOR_FLASH) || defined(COMPILE_FOR_PROGMEM)
    if (menuhelper_redraw)
    {
      if (fm.sample_source > 1 && fm.sample_source < 4) // MSP
        draw_waveform_overview_in_sample_editor_step1(temp_int); //array starts with 0, not 1
      else
        draw_waveform_overview_in_sample_editor_step1(temp_int + 1);

      menuhelper_redraw = false;
    }
#endif
  }
  if (LCDML.FUNC_close()) // ****** STABLE END *********
  {
    encoderDir[ENC_R].reset();
    display.fillScreen(COLOR_BACKGROUND);
  }
}

void print_content_type()
{
  set_pattern_content_type_color(seq.active_pattern);
  display.setTextSize(1);
  if (seq.content_type[seq.active_pattern] == 0)
    display.print(F("Drum/Smp."));
  else if (seq.content_type[seq.active_pattern] == 1)
    display.print(F("Instr.   "));
  else if (seq.content_type[seq.active_pattern] == 2)
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
  }
  else if (LCDML.FUNC_getID() == LCDML.OTHER_getIDFromFunction(UI_func_seq_pattern_editor))
  {
    display.setTextColor(COLOR_BACKGROUND, MIDDLEGREEN);
    display.print(F("NOTE EDITOR"));
  }
  display.setTextColor(COLOR_SYSTEXT, COLOR_BACKGROUND);
  display.setTextSize(2);
}

void sub_set_color_print_track_step_detailed(uint8_t i, uint8_t currentstep)
{
  if (i > 15 - seq.pattern_len_dec && i != currentstep)  //above current pattern length setting
    display.setTextColor(GREY3, COLOR_BACKGROUND);
  else if (i == currentstep && i > 15 - seq.pattern_len_dec)
    display.setTextColor(COLOR_BACKGROUND, GREY3);
  else if (i == currentstep)
    display.setTextColor(COLOR_BACKGROUND, COLOR_SYSTEXT);
  else
    display.setTextColor(GREY1, COLOR_BACKGROUND);

}
void print_track_steps_detailed(int xpos, int ypos, uint8_t currentstep, bool init, bool allsteps)
{
  if (seq.cycle_touch_element == 0) // touch keyboard is off
  {
    // bool init = only print static content one time. if true, print the static content
    // allsteps ==  print all lines , allsteps == false print just the current step +-1 steps
    uint8_t i = 0;
    uint8_t laststep = 16;
    int y = 0;
    int x = 0;

    display.setTextSize(1);
    display.setTextColor(GREY2, COLOR_BACKGROUND);
    display.setCursor(xpos, ypos);

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

    while (i < laststep)
    {
      x = xpos;
      y = ypos + 10 + i * (CHAR_height_small + 2);

      if (init)
      { // only needs to be drawn at first run

        if (i % 4 == 0)
          display.setTextColor(GREY1, COLOR_BACKGROUND);
        else
          display.setTextColor(MIDDLEGREEN, COLOR_BACKGROUND);
        display.setCursor(x, y);
        print_formatted_number(i + 1, 2);
      }
      // Short Name
      sub_set_color_print_track_step_detailed(i, currentstep);

      display.setCursor(CHAR_width_small * 4, y);

      if (seq.vel[seq.active_pattern][i] > 209) // it is a pitched Drum Sample
      {
        seq_print_current_note_from_step(i);
      }
      else
      {
        display.print(seq_find_shortname_in_track(i, seq.active_pattern)[0]);
      }
      // Data values
      sub_set_color_print_track_step_detailed(i, currentstep);
      display.setCursor(CHAR_width_small * 7, y);
      print_formatted_number(seq.note_data[seq.active_pattern][i], 3);

      // Velocity values
      sub_set_color_print_track_step_detailed(i, currentstep);
      display.setCursor(CHAR_width_small * 12, y);
      print_formatted_number(seq.vel[seq.active_pattern][i], 3);
      // Long Name / Note
      if (i > 15 - seq.pattern_len_dec && i != currentstep)  //above current pattern length setting
        display.setTextColor(GREY3, COLOR_BACKGROUND);
      else if (i == currentstep && i > 15 - seq.pattern_len_dec)
        display.setTextColor(COLOR_BACKGROUND, GREY3);
      else if (i == currentstep)
        display.setTextColor(COLOR_BACKGROUND, COLOR_SYSTEXT);
      else
        set_pattern_content_type_color(seq.active_pattern);
      if (seq.content_type[seq.active_pattern] == 0) // Drum Track
      {
        if (seq.vel[seq.active_pattern][i] > 209) // it is a pitched Drum Sample
        {
          show_smallfont_noGrid(y, CHAR_width_small * 17, 10, basename(drum_config[seq.vel[seq.active_pattern][i] - 210].name));
        }
        else // else it is a regular Drum Sample
          show_smallfont_noGrid(y, CHAR_width_small * 17, 10, find_long_drum_name_from_note(seq.note_data[seq.active_pattern][i]));
      }
      else if (seq.content_type[seq.active_pattern] > 0) // Inst Track or Chord or Arp
      {
        display.setCursor(x + CHAR_width_small * 17, y);
        if (seq.note_data[seq.active_pattern][i] != 0)
        {
          if (seq.note_data[seq.active_pattern][i] == 130) // it is a latched note
          {
            if (i == currentstep)
              display.setTextColor(COLOR_BACKGROUND, COLOR_SYSTEXT);
            else
              display.setTextColor(GREEN, COLOR_BACKGROUND);
            display.write(0x7E);
            display.print("LATCH"); // Tilde Symbol for latched note
          }
          else
          {
            display.print(noteNames[seq.note_data[seq.active_pattern][i] % 12][0]);
            if (noteNames[seq.note_data[seq.active_pattern][i] % 12][1] != '\0')
            {
              display.print(noteNames[seq.note_data[seq.active_pattern][i] % 12][1]);
            }
            if (seq.vel[seq.active_pattern][i] < 200) // print octave is not a chord
            {
              display.print((seq.note_data[seq.active_pattern][i] / 12) - 1);
            }
            if (seq.vel[seq.active_pattern][i] > 199) // is a chord
            {
              display.print(" ");
              print_chord_name(i);
            }
          }
        }
      }
      while (display.getCursorX() < CHAR_width_small * 32)
      {

        sub_set_color_print_track_step_detailed(i, currentstep);
        display.print(" ");
      }
      i++;
    }
    display.setTextColor(COLOR_SYSTEXT, COLOR_BACKGROUND);
    display.setTextSize(2);
  }
}

void UI_func_seq_vel_editor(uint8_t param)
{
  char tmp[5];
  uint8_t prev_option = 99; // avoid screen flicker at start / end of menu options
  if (LCDML.FUNC_setup())   // ****** SETUP *********
  {
    // setup function
    if (seq.cycle_touch_element != 1)
      draw_button_on_grid(45, 16, "", "", 0); // clear button
    if (seq.menu_status != 1)
    {
      display.fillScreen(COLOR_BACKGROUND);
      seq_pattern_editor_update_dynamic_elements();
    }
    encoderDir[ENC_R].reset();
    print_edit_mode();
    seq.menu = 1;
    print_shortcut_navigator();
    display.setTextSize(2);
    display.fillRect(0, 3 * CHAR_height + 17, 212, 8, COLOR_BACKGROUND);
    setCursor_textGrid(14, 0);
    display.setTextColor(GREY2, COLOR_BACKGROUND);
    display.print("[");
    setCursor_textGrid(15, 0);
    print_formatted_number(seq.active_pattern, 2);
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
  if (LCDML.FUNC_loop()) // ****** LOOP *********
  {
    if (seq.active_function == 99)
    {
      if ((LCDML.BT_checkDown() && encoderDir[ENC_R].Down()) || (LCDML.BT_checkUp() && encoderDir[ENC_R].Up()))
      {
        if (LCDML.BT_checkDown())
        {
          prev_menu_item = seq.menu;
          seq.menu = constrain(seq.menu + 1, 0, 17);
          pattern_editor_play_current_step(seq.menu - 1);
        }
        else if (LCDML.BT_checkUp())
        {
          if (seq.menu == 0)
          {
            seq.menu_status = 2;
            LCDML.OTHER_jumpToFunc(UI_func_seq_pattern_editor);
          }
          else
          {
            seq.menu_status = 0;
            seq.menu = constrain(seq.menu - 1, 0, 17);
            pattern_editor_play_current_step(seq.menu - 1);
          }
        }
      }
    }
    else if (seq.active_function == 0 && seq.menu == 0)
    {
      if ((LCDML.BT_checkDown() && encoderDir[ENC_R].Down()) || (LCDML.BT_checkUp() && encoderDir[ENC_R].Up()))
      {
        if (LCDML.BT_checkDown())
          seq.active_pattern = constrain(seq.active_pattern + 1, 0, NUM_SEQ_PATTERN - 1);
        else if (LCDML.BT_checkUp())
          seq.active_pattern = constrain(seq.active_pattern - 1, 0, NUM_SEQ_PATTERN - 1);
        display.setCursor(11 * CHAR_width_small, CHAR_height * 3 + 3);
        print_content_type();
        virtual_keyboard_smart_preselect_mode();
        update_latch_button();
      }
    }
    else if (seq.active_function == 0 && seq.menu > 0 && seq.menu < 17)
    {
      if (seq.note_data[seq.active_pattern][seq.menu - 1] > 0)
      {
        if (seq.vel[seq.active_pattern][seq.menu - 1] < 210) // it is a normal sample
        {
          if (seq.active_function == 0 && seq.content_type[seq.active_pattern] < 2)
          { // if is Drum or normal Instrument Track
            if ((LCDML.BT_checkDown() && encoderDir[ENC_R].Down()) || (LCDML.BT_checkUp() && encoderDir[ENC_R].Up()))
            {
              if (LCDML.BT_checkDown())
                seq.vel[seq.active_pattern][seq.menu - 1] = constrain(seq.vel[seq.active_pattern][seq.menu - 1] + ENCODER[ENC_R].speed(), 0, 127);
              else if (LCDML.BT_checkUp())
                seq.vel[seq.active_pattern][seq.menu - 1] = constrain(seq.vel[seq.active_pattern][seq.menu - 1] - ENCODER[ENC_R].speed(), 0, 127);
            }
          }
          else if (seq.active_function == 0 && seq.content_type[seq.active_pattern] > 1)
          { // is in Chord or Arp Mode
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
          // is in pitched Sample Mode
          if ((LCDML.BT_checkDown() && encoderDir[ENC_R].Down()) || (LCDML.BT_checkUp() && encoderDir[ENC_R].Up()))
          {
            if (LCDML.BT_checkDown())
              seq.vel[seq.active_pattern][seq.menu - 1] = constrain(seq.vel[seq.active_pattern][seq.menu - 1] + 1, 210, 217);
            else if (LCDML.BT_checkUp())
              seq.vel[seq.active_pattern][seq.menu - 1] = constrain(seq.vel[seq.active_pattern][seq.menu - 1] - 1, 210, 217);
          }
        }
      }
    }
    else if (seq.active_function == 0 && seq.menu == 17) // edit content type of current pattern
    {

      if ((LCDML.BT_checkDown() && encoderDir[ENC_R].Down()) || (LCDML.BT_checkUp() && encoderDir[ENC_R].Up()))
      {
        prev_option = seq.note_editor_view;
        if (LCDML.BT_checkDown())
          seq.content_type[seq.active_pattern] = constrain(seq.content_type[seq.active_pattern] + 1, 0, 2);
        else if (LCDML.BT_checkUp())
          seq.content_type[seq.active_pattern] = constrain(seq.content_type[seq.active_pattern] - 1, 0, 2);

        seq_printAllSeqSteps();
        if (seq.content_type[seq.active_pattern] == 0)
          seq.note_editor_view = 0;
        else
          seq.note_editor_view = 1;
      }
    }

    if (LCDML.BT_checkEnter()) // handle button presses during menu >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
    {
      if (seq.active_function == 99)
      {
        seq.active_function = 0;
      }
      else if (seq.active_function == 0)
      {
        seq.active_function = 99;
      }
    }
    // button check end <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<

    // if ( seq.content_type[seq.active_pattern] > 1 && seq.vel[seq.active_pattern][seq.menu - 1] < 200)
    //   seq.vel[seq.active_pattern][seq.menu - 1] = 200;

    if (seq.active_function == 0)
    {
      display.setTextSize(1);
      display.setCursor(11 * CHAR_width_small, CHAR_height * 3 + 3);
      print_content_type();
      display.setTextSize(2);
    }
    if (seq.menu == 0 && seq.active_function == 99)
    {
      display.setTextSize(1);
      display.setCursor(0, 3 * CHAR_height + 17);
      display.setTextColor(GREEN, COLOR_BACKGROUND);
      display.print(F("SELECT PATTERN TO EDIT"));
      print_empty_spaces(8);
      display.setTextSize(2);
      display.setCursor(0, 0);
      display.setTextColor(GREY1, COLOR_BACKGROUND);
      print_empty_spaces(14);
    }
    if (seq.menu == 0)
    {
      display.setTextSize(2);
      if (seq.active_function != 99)
        display.setTextColor(RED, COLOR_BACKGROUND);
      else
        display.setTextColor(COLOR_SYSTEXT, COLOR_BACKGROUND);
      setCursor_textGrid(14, 0);
      display.print("[");
      print_formatted_number(seq.active_pattern, 2);
      display.print("]");
      setCursor_textGrid(0, 1);
      seq_printAllSeqSteps();
      seq_printVelGraphBar();
      if (seq.content_type[seq.active_pattern] == 0) // Drum Mode
      {
        if (seq.note_editor_view != 0)
        {
          seq.note_editor_view = 0;
          if (seq.cycle_touch_element == 0) // touch keyboard is off
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
          if (seq.cycle_touch_element == 0) // touch keyboard is off
            border3_clear();
          if (seq.cycle_touch_element != 1)
            print_single_pattern_pianoroll_in_pattern_editor(0, DISPLAY_HEIGHT, seq.active_pattern, seq.menu - 3, true);
        }
        else if (seq.active_function == 99)
        {
          if (seq.cycle_touch_element != 1)
            print_single_pattern_pianoroll_in_pattern_editor(0, DISPLAY_HEIGHT, seq.active_pattern, seq.menu - 3, false);
        }
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
      if (seq.active_function == 99)
        display.setTextColor(COLOR_BACKGROUND, COLOR_SYSTEXT);
      else
        display.setTextColor(COLOR_SYSTEXT, RED);
      setCursor_textGrid(0, 1);
      display.print(seq_find_shortname(0)[0]);

      set_pattern_content_type_color(seq.active_pattern);
      setCursor_textGrid(1, 1);
      display.print(seq_find_shortname(1)[0]);
      display.setTextColor(COLOR_SYSTEXT, COLOR_BACKGROUND);
    }
    else if (seq.menu > 1 && seq.menu < 17)
    {
      display.setTextSize(2);
      //set_pattern_content_type_color(seq.active_pattern);
      setCursor_textGrid(seq.menu - 2, 1);

      if (seq.menu - 2 > 15 - seq.pattern_len_dec)
        display.setTextColor(GREY3, COLOR_BACKGROUND);

      else if (seq.vel[seq.active_pattern][seq.menu - 2] > 209) // if pitched sample, change color
        display.setTextColor(COLOR_PITCHSMP, COLOR_BACKGROUND);
      else
        set_pattern_content_type_color(seq.active_pattern);
      display.print(seq_find_shortname(seq.menu - 2)[0]);

      setCursor_textGrid(seq.menu - 1, 1);

      if (seq.menu - 1 > 15 - seq.pattern_len_dec && seq.active_function == 99) ////////
        display.setTextColor(COLOR_BACKGROUND, GREY2);

      else   if (seq.active_function == 99)
        display.setTextColor(COLOR_BACKGROUND, COLOR_SYSTEXT);
      else if (seq.menu - 1 > 15 - seq.pattern_len_dec)
        display.setTextColor(COLOR_BACKGROUND, RED);
      else
        display.setTextColor(COLOR_SYSTEXT, RED);

      display.print(seq_find_shortname(seq.menu - 1)[0]);
      set_pattern_content_type_color(seq.active_pattern);

      if (seq.menu < 16)
      {
        setCursor_textGrid(seq.menu + 0, 1);

        if (seq.menu > 15 - seq.pattern_len_dec)
          display.setTextColor(GREY3, COLOR_BACKGROUND);

        else  if (seq.vel[seq.active_pattern][seq.menu] > 209) // if pitched sample, change color
          display.setTextColor(COLOR_PITCHSMP, COLOR_BACKGROUND);
        else
          set_pattern_content_type_color(seq.active_pattern);
        display.print(seq_find_shortname(seq.menu)[0]);
      }
    }
    if (seq.menu > 0 && seq.menu < 17)
    {
      display.setTextSize(2);
      if (seq.content_type[seq.active_pattern] == 0) // Drum Mode
      {
        if (seq.menu - 1 == 0)
          print_track_steps_detailed(0, CHAR_height * 4 + 3, seq.menu - 1, false, true);
        else
          print_track_steps_detailed(0, CHAR_height * 4 + 3, seq.menu - 1, false, false);
      }
      else if (seq.active_function == 99)
      {
        if (seq.cycle_touch_element != 1)
          print_single_pattern_pianoroll_in_pattern_editor(0, DISPLAY_HEIGHT, seq.active_pattern, seq.menu - 1, false);
      }
      setCursor_textGrid(3, 0);
      if (seq.note_data[seq.active_pattern][seq.menu - 1] > 0)
      {
        if (seq.content_type[seq.active_pattern] == 0) // is Drumtrack
        {
          setCursor_textGrid(0, 0);
          if (seq.vel[seq.active_pattern][seq.menu - 1] < 210) // it is a normal sample
          {
            display.setTextColor(COLOR_SYSTEXT, COLOR_BACKGROUND);
            display.print("V:");
            snprintf_P(tmp, sizeof(tmp), PSTR("%03d"), seq.vel[seq.active_pattern][seq.menu - 1]);
            display.print(tmp);
            display.print(" ");
            display.setTextColor(COLOR_DRUMS, COLOR_BACKGROUND);
            show(0, 6, 8, seq_find_drum_name_from_note(seq.note_data[seq.active_pattern][seq.menu - 1]));
            if (seq.active_function == 99)
            {
              display.setCursor(0, 3 * CHAR_height + 17);
              display.setTextSize(1);
              display.setTextColor(GREEN, COLOR_BACKGROUND);
              display.print(F("EDIT VELOCITY OF STEP "));
              display.setTextColor(COLOR_SYSTEXT, COLOR_BACKGROUND);
              display.print(seq.menu);
              display.setTextColor(GREEN, COLOR_BACKGROUND);
              display.print(F(" ?    "));
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

            if (seq.active_function == 99)
            {
              display.setCursor(0, 3 * CHAR_height + 17);
              display.setTextSize(1);
              display.setTextColor(GREEN, COLOR_BACKGROUND);
              display.print(F("REPLACE "));
              display.setTextColor(COLOR_BACKGROUND, GREEN);
              display.print(basename(drum_config[seq.vel[seq.active_pattern][seq.menu - 1] - 210].name));
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
          if (seq.note_data[seq.active_pattern][seq.menu - 1] != 130) // note not latched
          {
            if (seq.content_type[seq.active_pattern] < 2)
            {
              setCursor_textGrid(0, 0);
              display.print(F("Vel:"));
              snprintf_P(tmp, sizeof(tmp), PSTR("%03d"), seq.vel[seq.active_pattern][seq.menu - 1]);
              setCursor_textGrid(4, 0);
              display.print(tmp);
              setCursor_textGrid(7, 0);
              print_empty_spaces(3);
            }
            set_pattern_content_type_color(seq.active_pattern);

            setCursor_textGrid(10, 0);
            display.print(noteNames[seq.note_data[seq.active_pattern][seq.menu - 1] % 12][0]);
            if (noteNames[seq.note_data[seq.active_pattern][seq.menu - 1] % 12][1] != '\0')
            {
              display.print(noteNames[seq.note_data[seq.active_pattern][seq.menu - 1] % 12][1]);
            }
            display.print((seq.note_data[seq.active_pattern][seq.menu - 1] / 12) - 1);
            display.print("  ");
          }
          else
          { // note is latched
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
          seq_printVelGraphBar_single_step(seq.menu - 2, GREY1); // previous
        if (seq.menu - 1 < 15)
          seq_printVelGraphBar_single_step(seq.menu, GREY1); // next
      }
      else
      {
        setCursor_textGrid(0, 0);
        display.print(F("              "));
        if (seq.menu - 1 > 0)
          seq_printVelGraphBar_single_step(seq.menu - 2, GREY1); // previous
        if (seq.menu - 1 < 15)
          seq_printVelGraphBar_single_step(seq.menu, GREY1); // next

        // clear "Insert xyz ?"  message
        display.fillRect(0, 3 * CHAR_height + 17, 212, 8, COLOR_BACKGROUND);
      }
    }

    if (seq.menu == 17 && prev_menu_item != 17 && prev_option != seq.note_editor_view) // edit content type of pattern
    {
      if (seq.active_function != 1)
      {
        display.setCursor(0, 0);
        display.setTextSize(2);
        display.setTextColor(GREY1, COLOR_BACKGROUND);
        print_empty_spaces(15);
        display.setTextSize(1);
        display.setCursor(0, 3 * CHAR_height + 17);
        display.setTextColor(GREEN, COLOR_BACKGROUND);
        display.print(F("EDIT CONTENT TYPE OF PAT. "));
        display.setTextColor(COLOR_SYSTEXT, COLOR_BACKGROUND);
        display.print(seq.active_pattern);
        display.setTextColor(GREEN, COLOR_BACKGROUND);
        display.print(" ? ");
      }
      print_edit_mode();
      display.setTextSize(1);
      if (seq.active_function == 99)
        display.setTextColor(COLOR_BACKGROUND, COLOR_SYSTEXT);
      else
        display.setTextColor(COLOR_SYSTEXT, RED);
      display.setCursor(0, CHAR_height * 3 + 3);
      display.print("CONT.TYPE:");
      display.setCursor(11 * CHAR_width_small, CHAR_height * 3 + 3);
      print_content_type();
      display.setTextSize(2);
      seq_printAllSeqSteps();
      seq_printVelGraphBar();
      if (seq.cycle_touch_element == 0) // touch keyboard is off
        border3_clear();
      if (seq.content_type[seq.active_pattern] == 0) // Drum Mode
      {
        print_track_steps_detailed(0, CHAR_height * 4 + 3, seq.menu - 1, true, true);
      }
      else
      {
        if (seq.cycle_touch_element != 1)
          print_single_pattern_pianoroll_in_pattern_editor(0, DISPLAY_HEIGHT, seq.active_pattern, seq.menu - 1, true);
      }
      prev_option = 99;
    }

    else if (seq.menu == 16)
    {
      display.setTextSize(1);
      display.setTextColor(GREY1, COLOR_BACKGROUND);
      display.setCursor(0, CHAR_height * 3 + 3);
      display.print(F("CONT.TYPE:"));
      display.setCursor(11 * CHAR_width_small, CHAR_height * 3 + 3);
      print_content_type();
      display.setTextSize(2);
      print_edit_mode();
    }
    virtual_keyboard_smart_preselect_mode();
  }
  if (LCDML.FUNC_close()) // ****** STABLE END *********
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
    seq.content_type[i] = 0;
  }
  for (uint8_t i = 0; i < NUM_SEQ_TRACKS - 1; i++)
  {
    seq.track_type[i] = 0;
  }
}

FLASHMEM void seq_clear_song_data()
{
  for (uint8_t i = 0; i < NUM_SEQ_TRACKS; i++)
  {
    memset(seq.song[i], 99, sizeof(seq.song[i]));
  }
}

FLASHMEM void seq_clear_chain_data()
{
  for (uint8_t i = 0; i < NUM_CHAINS; i++)
  {
    memset(seq.chain[i], 99, sizeof(seq.chain[i]));
    memset(seq.chain_transpose[i], 99, sizeof(seq.chain_transpose[i]));
  }
}

void seq_refresh_display_play_status()
{
  display.fillRect(12 * CHAR_width, 0, 13, 16, COLOR_BACKGROUND);
  if (seq.running == false && seq.recording == false)
  {
    drawBitmap(12 * CHAR_width + 3, 0 + 3, special_chars[19], 8, 8, GREEN);
  }
  else if (seq.running == true && seq.recording == false)
  {
    seq.note_in = 0;
    drawBitmap(12 * CHAR_width + 3, 0 + 3, special_chars[20], 8, 8, RED);
  }
  else if (seq.running == true && seq.recording == true)
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

void print_sample_type()
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
    print_empty_spaces(6);
  }
}

void seq_sub_copy_swap()
{
  if (seq.menu == 30 || seq.menu == 31)
  { // is in sub-function - swap pattern or copy pattern
    seq.active_function = 98;
    if ((LCDML.BT_checkDown() && encoderDir[ENC_R].Down()) || (LCDML.BT_checkUp() && encoderDir[ENC_R].Up()))
    {
      if (LCDML.BT_checkDown())
      {

        temp_int = constrain(temp_int + 1, 0, NUM_SEQ_PATTERN - 1);
        if (temp_int == seq.active_pattern)
          temp_int++;
        if (temp_int > NUM_SEQ_PATTERN - 1)
          temp_int = 0;
      }
      else if (LCDML.BT_checkUp())
      {
        temp_int = constrain(temp_int - 1, 0, NUM_SEQ_PATTERN - 1);
        if (temp_int == seq.active_pattern)
          temp_int--;
        if (temp_int < 0)
          temp_int = NUM_SEQ_PATTERN - 1;
      }
    }
  }
  if (LCDML.BT_checkEnter()) // handle button presses during menu >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
  {
    if (seq.menu == 31 && seq.active_function != 40) // copy patterns
    {
      memcpy(seq.note_data[temp_int], seq.note_data[seq.active_pattern], sizeof(seq.note_data[0]));
      memcpy(seq.vel[temp_int], seq.vel[seq.active_pattern], sizeof(seq.vel[0]));
      seq.content_type[temp_int] = seq.content_type[seq.active_pattern];
      seq.menu = 0;
      seq.active_function = 0;
      activesample = 0;
      temp_int = seq.note_data[seq.active_pattern][0];
      display.setTextSize(2);
      setCursor_textGrid(14, 0);
      display.print(" ");
      setCursor_textGrid(17, 0);
      display.print(" ");
      setCursor_textGrid(1, 0);
      display.print(F("         "));
      setCursor_textGrid(16, 1);
      display.print(F("  "));
      seq_refresh_display_play_status();
      seq_printAllSeqSteps();
    }
    else if (seq.menu == 30 && seq.active_function != 40) // swap patterns
    {
      uint8_t data_temp[1][16];
      uint8_t vel_temp[1][16];
      uint8_t content_type_temp;
      memcpy(data_temp[0], seq.note_data[seq.active_pattern], sizeof(data_temp[0]));
      memcpy(vel_temp[0], seq.vel[seq.active_pattern], sizeof(vel_temp[0]));
      content_type_temp = seq.content_type[seq.active_pattern];
      memcpy(seq.note_data[seq.active_pattern], seq.note_data[temp_int], sizeof(data_temp[0]));
      memcpy(seq.vel[seq.active_pattern], seq.vel[temp_int], sizeof(vel_temp[0]));
      seq.content_type[seq.active_pattern] = seq.content_type[temp_int];
      memcpy(seq.note_data[temp_int], data_temp[0], sizeof(data_temp[0]));
      memcpy(seq.vel[temp_int], vel_temp[0], sizeof(vel_temp[0]));
      seq.content_type[temp_int] = content_type_temp;
      seq.menu = 0;
      seq.active_function = 0;
      activesample = 0;
      border3_clear();
      display.setTextSize(2);
      display.setCursor(11 * CHAR_width_small, CHAR_height * 3 + 3);
      print_content_type();
      display.setTextSize(2);
      setCursor_textGrid(14, 0);
      display.print(" ");
      setCursor_textGrid(17, 0);
      display.print(" ");
      setCursor_textGrid(1, 0);
      display.print(F("         "));
      setCursor_textGrid(16, 1);
      display.print("  ");
      temp_int = seq.note_data[seq.active_pattern][0];
      seq_refresh_display_play_status();
      seq_printAllSeqSteps();
      if (seq.content_type[seq.active_pattern] == 0)
        print_track_steps_detailed(0, CHAR_height * 4 + 3, 254, true, true);
      else
        print_single_pattern_pianoroll_in_pattern_editor(0, DISPLAY_HEIGHT, seq.active_pattern, seq.menu - 1, true);
    }

    if (seq.menu == 0 && seq.active_function == 0)
    {
      if ((seq.content_type[seq.active_pattern] == 0 && activesample == NUM_DRUMSET_CONFIG + 3) || (seq.content_type[seq.active_pattern] > 0 && temp_int == 114))
      { // swap patterns: Active pattern <-> destination pattern
        setCursor_textGrid(0, 0);
        display.setTextSize(2);
        display.print(F("SwapPattern: "));
        temp_int = seq.active_pattern + 1;
        if (temp_int > NUM_SEQ_PATTERN - 1)
          temp_int = 0;
        seq.menu = 30;
      }
      else if ((seq.content_type[seq.active_pattern] == 0 && activesample == NUM_DRUMSET_CONFIG + 2) || (seq.content_type[seq.active_pattern] > 0 && temp_int == 113))
      { // copy pattern
        setCursor_textGrid(0, 0);
        display.setTextSize(2);
        display.print(F("Copy Pattern:"));
        temp_int = seq.active_pattern + 1;
        if (temp_int > NUM_SEQ_PATTERN - 1)
          temp_int = 0;
        seq.menu = 31;
      }
    }
  }
  // button check end <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<

  if (seq.menu == 31)
  { // copy pattern
    display.setTextSize(2);
    setCursor_textGrid(14, 0);
    display.print("[");
    setCursor_textGrid(17, 0);
    display.print("]");
    setCursor_textGrid(0, 1);
    display.print(F("          to: [  ]"));
    setCursor_textGrid(15, 1);
    print_formatted_number(temp_int, 2);
  }
  else if (seq.menu == 30)
  { // swap pattern
    display.setTextSize(2);
    setCursor_textGrid(14, 0);
    display.print("[");
    setCursor_textGrid(17, 0);
    display.print("]");
    setCursor_textGrid(0, 1);
    display.print(F("       with:  [  ]"));
    setCursor_textGrid(15, 1);
    print_formatted_number(temp_int, 2);
  }
}

void seq_sub_pattern_fill()
{
  if (seq.menu == 33)
  { // is in sub-function - fill pattern
    seq.active_function = 95;
    if ((LCDML.BT_checkDown() && encoderDir[ENC_R].Down()) || (LCDML.BT_checkUp() && encoderDir[ENC_R].Up()))
    {
      if (LCDML.BT_checkDown())
      {
        seq.temp_active_menu = constrain(seq.temp_active_menu + 1, 0, 2); // fill step 1/4 , 1/8, 1/16
      }
      else if (LCDML.BT_checkUp())
      {
        seq.temp_active_menu = constrain(seq.temp_active_menu - 1, 0, 2); // fill step 1/4 , 1/8, 1/16
      }
    }
  }
  else if (seq.menu == 32)
  { // is in sub-function - fill pattern
    seq.active_function = 97;
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
          seq.temp_select_menu = constrain(seq.temp_select_menu + 1, 0, 108);
        else if (LCDML.BT_checkUp())
          seq.temp_select_menu = constrain(seq.temp_select_menu - 1, 0, 108);
      }
    }
  }

  if (LCDML.BT_checkEnter()) // handle button presses during menu >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
  {
    if (seq.menu == 32 && seq.active_function == 97) // fill pattern every 1/4, 1/8, 1/16 step with active sample/note  step 1
    {
      seq.active_function = 96;
      seq.menu = 33;

    }
    else if (seq.menu == 33 && seq.active_function == 95) // fill pattern every 1/4, 1/8, 1/16 step with active sample/note  step 2
    {

      if (seq.content_type[seq.active_pattern] == 0)
      { // Drumtrack
        for (uint8_t i = 0; i < 16; i++)
        {
          seq.note_data[seq.active_pattern][i] = drum_config[seq.temp_select_menu].midinote;
          seq.vel[seq.active_pattern][i] = 120;
          if (seq.temp_active_menu == 0)
            i = i + 3;
          else if (seq.temp_active_menu == 1)
            i = i + 1;
        }
      }
      else
      { // Inst. Track
        for (uint8_t i = 0; i < 16; i++)
        {
          seq.note_data[seq.active_pattern][i] = seq.temp_select_menu;
          seq.vel[seq.active_pattern][i] = 120;
          if (seq.temp_active_menu == 0)
            i = i + 3;
          else if (seq.temp_active_menu == 1)
            i = i + 1;
        }
      }
      seq.menu = 0;
      seq.active_function = 0;
      activesample = 0;
      temp_int = seq.note_data[seq.active_pattern][0];
      setCursor_textGrid(1, 0);
      display.setTextSize(2);
      display.print("         ");
      setCursor_textGrid(16, 1);
      display.print("  ");
      seq_refresh_display_play_status();
      seq_printAllSeqSteps();
      seq_printVelGraphBar();
      if (seq.content_type[seq.active_pattern] == 0)
        print_track_steps_detailed(0, CHAR_height * 4 + 3, 254, true, true);
      else
        print_single_pattern_pianoroll_in_pattern_editor(0, DISPLAY_HEIGHT, seq.active_pattern, seq.menu - 1, true);
    }
    else if (seq.menu == 0 && seq.active_function == 0)
    {
      if ((seq.content_type[seq.active_pattern] == 0 && activesample == NUM_DRUMSET_CONFIG + 4) || (seq.content_type[seq.active_pattern] > 0 && temp_int == 115))
      { // fill patterns
        display.setTextColor(COLOR_SYSTEXT, COLOR_BACKGROUND);
        setCursor_textGrid(0, 0);
        display.setTextSize(2);
        display.print(F("Fill Pattern:"));
        seq.menu = 32;
        seq.temp_select_menu = 0;
        seq.temp_active_menu = 0;
      }
    }
  }
  // button check end <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<

  if (seq.menu == 33) // editor step 2
  {                   // fill pattern 2nd parameter
    setCursor_textGrid(5, 1);
    display.setTextSize(2);
    display.print(" ");
    setCursor_textGrid(12, 1);
    display.setTextColor(RED, COLOR_BACKGROUND);
    display.print("[");
    display.setTextColor(COLOR_SYSTEXT, COLOR_BACKGROUND);
    if (seq.temp_active_menu == 0)
      display.print(F(" 1/4"));
    else if (seq.temp_active_menu == 1)
      display.print(F(" 1/8"));
    else if (seq.temp_active_menu == 2)
      display.print(F("1/16"));
    display.setTextColor(RED, COLOR_BACKGROUND);
    display.print("]");
    display.setTextColor(COLOR_SYSTEXT, COLOR_BACKGROUND);
  }
  else if (seq.menu == 32) // editor step 1
  {                        // fill pattern
    if (seq.content_type[seq.active_pattern] == 0)
    { // drum
      setCursor_textGrid(0, 1);
      display.setTextSize(2);

      display.setTextColor(COLOR_SYSTEXT, COLOR_BACKGROUND);
      display.print("with ");
      display.setTextColor(RED, COLOR_BACKGROUND);
      display.print("[");
      display.setTextColor(COLOR_SYSTEXT, COLOR_BACKGROUND);
      show(1, 6, 6, basename(drum_config[seq.temp_select_menu].name));
      setCursor_textGrid(12, 1);
      display.setTextColor(RED, COLOR_BACKGROUND);
      display.print("]");
      display.setTextColor(COLOR_SYSTEXT, COLOR_BACKGROUND);
      setCursor_textGrid(13, 1);
      if (seq.temp_active_menu == 0)
        display.print(F(" 1/4"));
      else if (seq.temp_active_menu == 1)
        display.print(F(" 1/8"));
      else if (seq.temp_active_menu == 2)
        display.print(F("1/16"));
    }
    else
    { // inst
      setCursor_textGrid(0, 1);
      display.setTextSize(2);
      display.setTextColor(COLOR_SYSTEXT, COLOR_BACKGROUND);
      display.print("with ");
      display.setTextColor(RED, COLOR_BACKGROUND);
      display.print("[");
      display.setTextColor(COLOR_SYSTEXT, COLOR_BACKGROUND);
      display.print(noteNames[seq.temp_select_menu % 12]);
      display.print((seq.temp_select_menu / 12) - 1);
      display.print(F("   "));
      setCursor_textGrid(12, 1);
      display.setTextColor(RED, COLOR_BACKGROUND);
      display.print("]");
      display.setTextColor(COLOR_SYSTEXT, COLOR_BACKGROUND);
      setCursor_textGrid(13, 1);
      if (seq.temp_active_menu == 0)
        display.print(F(" 1/4"));
      else if (seq.temp_active_menu == 1)
        display.print(F(" 1/8"));
      else if (seq.temp_active_menu == 2)
        display.print(F("1/16"));
    }
  }
}

void seq_sub_pattern_transpose()
{
  if (seq.menu == 34)
  { // is in transpose edit
    seq.active_function = 94;
    if ((LCDML.BT_checkDown() && encoderDir[ENC_R].Down()) || (LCDML.BT_checkUp() && encoderDir[ENC_R].Up()))
    {
      if (LCDML.BT_checkDown())
      {
        temp_int = constrain(temp_int + 1, -36, 36);
        if (temp_int > 36)
          temp_int = 36;
      }
      else if (LCDML.BT_checkUp())
      {
        temp_int = constrain(temp_int - 1, -36, 36);
        if (temp_int < -36)
          temp_int = -36;
      }
    }
  }
  if (LCDML.BT_checkEnter()) // handle button presses during menu >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
  {
    if (seq.menu == 34) // transpose pattern
    {
      seq.menu = 0;
      seq.active_function = 0;
      activesample = 0;
      temp_int = seq.note_data[seq.active_pattern][0];
      setCursor_textGrid(14, 1);
      print_formatted_number(seq.active_pattern, 2);
      seq_refresh_display_play_status();
      seq_printAllSeqSteps();
      display.setTextSize(2);
      setCursor_textGrid(2, 0);
      display.print("         ");
      setCursor_textGrid(13, 0);
      display.print(" ");
      setCursor_textGrid(16, 0);
      display.print(" ");
    }
    if (seq.menu == 0 && seq.active_function == 0)
    {
      if ((seq.content_type[seq.active_pattern] == 0 && activesample == NUM_DRUMSET_CONFIG + 5) || (seq.content_type[seq.active_pattern] > 0 && temp_int == 116))
      { // transpose pattern
        setCursor_textGrid(1, 0);
        display.setTextSize(2);
        display.print(F("Transpose: [ 00] "));
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
  // button check end <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<

  if (seq.menu == 34 && seq.active_function == 94)
  { // transpose
    char displayname[4] = { 0, 0, 0, 0 };
    setCursor_textGrid(12, 0);
    display.setTextSize(2);
    display.print("[");
    if (temp_int > 0)
    {
      display.print("+");
    }
    else if (temp_int < 0)
    {
      display.print("-");
    }
    else
    {
      display.print(" ");
    }
    snprintf_P(displayname, sizeof(displayname), PSTR("%02d"), abs(temp_int));
    display.print(displayname);
    display.print("]");
    for (uint8_t i = 0; i < 16; i++)
    {
      if (seq.content_type[seq.active_pattern] == 0)
      {                                                                                                                                                    // drums
        if (seq.data_buffer[i] != 0 && seq.data_buffer[i] + temp_int >= 0 && seq.data_buffer[i] + temp_int < 254 && seq.vel[seq.active_pattern][i] >= 210) // pitched drums only
          seq.note_data[seq.active_pattern][i] = seq.data_buffer[i] + temp_int;
      }
      else
        // instruments
        if (seq.content_type[seq.active_pattern] > 0)
        {
          if (seq.data_buffer[i] != 0 && seq.data_buffer[i] != 130 && seq.data_buffer[i] + temp_int > 0 && seq.data_buffer[i] + temp_int < 254)
            seq.note_data[seq.active_pattern][i] = seq.data_buffer[i] + temp_int;
        }
    }
    seq_printAllSeqSteps();
    if (seq.content_type[seq.active_pattern] == 0)
      print_track_steps_detailed(0, CHAR_height * 4 + 3, 254, true, true);
    else
      print_single_pattern_pianoroll_in_pattern_editor(0, DISPLAY_HEIGHT, seq.active_pattern, seq.menu - 1, true);
  }
}

void seq_sub_clear_pattern_or_clear_all()
{
  if (LCDML.BT_checkEnter()) // handle button presses during menu >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
  {
    if (seq.menu == 0 && seq.active_function == 0)
    {
      if ((seq.content_type[seq.active_pattern] == 0 && activesample == NUM_DRUMSET_CONFIG + 1) || (seq.content_type[seq.active_pattern] > 0 && temp_int == 112))
      { // clear all patterns
        seq_clear_all_patterns();
        seq_printAllSeqSteps();
        print_track_steps_detailed(0, CHAR_height * 4 + 3, 254, true, true);
      }
      else if ((seq.content_type[seq.active_pattern] == 0 && activesample == NUM_DRUMSET_CONFIG) || (seq.content_type[seq.active_pattern] > 0 && temp_int == 111))
      { // clear pattern
        seq_clear_active_pattern();
        seq_printAllSeqSteps();
        print_track_steps_detailed(0, CHAR_height * 4 + 3, 254, true, true);
      }
      seq.active_function = 99;
    }
  }
  // button check end <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
}

void seq_sub_pitch_edit_pitched_sample()
{
  if (seq.active_function == 40)
  { // pitch edit sample
    if ((LCDML.BT_checkDown() && encoderDir[ENC_R].Down()) || (LCDML.BT_checkUp() && encoderDir[ENC_R].Up()))
    {
      if (LCDML.BT_checkDown())
      {
        seq.note_data[seq.active_pattern][seq.menu - 3] = constrain(seq.note_data[seq.active_pattern][seq.menu - 3] + 1, 12, 108);
      }
      else if (LCDML.BT_checkUp())
      {
        seq.note_data[seq.active_pattern][seq.menu - 3] = constrain(seq.note_data[seq.active_pattern][seq.menu - 3] - 1, 12, 108);
      }
    }
  }
  if (LCDML.BT_checkEnter()) // handle button presses during menu >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
  {
    if (seq.menu > 2 && seq.menu < 19 && seq.vel[seq.active_pattern][seq.menu - 3] > 209 && seq.active_function != 40 && activesample != NUM_DRUMSET_CONFIG - 1) // enter edit pitch of sample mode
    {
      seq.active_function = 40;
    }
    else if (seq.active_function == 40) // get out of pitch edit for samples
    {
      if (seq.note_data[seq.active_pattern][seq.menu - 3] == 12)
      {
        seq.note_data[seq.active_pattern][seq.menu - 3] = 0;
        seq.vel[seq.active_pattern][seq.menu - 3] = 0;
      }
      setCursor_textGrid(11, 1);
      display.print(" ");
      seq.active_function = 0;
      activesample = 0;
      seq_refresh_display_play_status();
      seq_printAllSeqSteps();
    }
  }
  // button check end <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<

  if (seq.vel[seq.active_pattern][seq.menu - 3] > 209 && activesample != NUM_DRUMSET_CONFIG - 1 && seq.active_function == 40) // is pitched sample and selected item is not set to EMPTY
  {
    display.setTextSize(2);
    setCursor_textGrid(1, 0);
    if (seq.note_data[seq.active_pattern][seq.menu - 3] != 12)
    {
      display.setTextColor(COLOR_SYSTEXT, COLOR_BACKGROUND);
      display.print(F("[EDIT "));
      // seq_print_current_note_from_step(seq.menu - 3);
      display.print(noteNames[seq.note_data[seq.active_pattern][seq.menu - 3] % 12][0]);
      if (noteNames[seq.note_data[seq.active_pattern][seq.menu - 3] % 12][1] != '\0')
      {
        display.print(noteNames[seq.note_data[seq.active_pattern][seq.menu - 3] % 12][1]);
      }
      display.print((seq.note_data[seq.active_pattern][seq.menu - 3] / 12) - 1);
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
  if (seq.cycle_touch_element == 0) // touch keyboard is off
  {
    display.setTextSize(1);
    display.setCursor(36 * CHAR_width_small, 4 * (CHAR_height_small + 2) + 10);
    display.setTextColor(GREY2, COLOR_BACKGROUND);
    display.print(F("SAMPLE BUFFER: "));
    display.setTextColor(COLOR_SYSTEXT, COLOR_BACKGROUND);
    display.setCursor(36 * CHAR_width_small, 5 * (CHAR_height_small + 2) + 10);
    if (activesample < NUM_DRUMSET_CONFIG - 1 && seq.vel[seq.active_pattern][seq.menu - 3] < 210) // normal sample
    {
      display.print(basename(drum_config[activesample].name));
      fill_up_with_spaces_right_window();
    }
    else if (activesample < NUM_DRUMSET_CONFIG - 1 && seq.vel[seq.active_pattern][seq.menu - 3] > 209) // pitched sample
    {
      display.print(basename(drum_config[seq.vel[seq.active_pattern][seq.menu - 3] - 210].name));
      fill_up_with_spaces_right_window();
    }
    display.setCursor(36 * CHAR_width_small, 6 * (CHAR_height_small + 2) + 10);
    display.setTextColor(GREY2);
    display.print(F("SAMPLE TYPE: "));
    display.setTextColor(COLOR_SYSTEXT, COLOR_BACKGROUND);
    display.setCursor(36 * CHAR_width_small, 7 * (CHAR_height_small + 2) + 10);
    print_sample_type();
    display.setTextColor(GREY2, COLOR_BACKGROUND);
    display.setCursor(36 * CHAR_width_small, 8 * (CHAR_height_small + 2) + 10);
    display.print(F("NOTE BUFFER: "));
    display.setTextColor(COLOR_SYSTEXT, COLOR_BACKGROUND);
    display.print(noteNames[temp_int % 12]);
    display.print((temp_int / 12) - 1);
    display.print(" ");

    display.setTextSize(1);
    print_performance_name(CHAR_width_small * 36, 10 * (CHAR_height_small + 2) + 10);
    display.setTextSize(2);
  }
}

void set_sample_type_color()
{
  if (drum_config[activesample].midinote > 209)
    display.setTextColor(COLOR_BACKGROUND, COLOR_PITCHSMP);
  else
    display.setTextColor(COLOR_BACKGROUND, COLOR_DRUMS);
}

void set_sample_type_color_of(uint8_t samplekey)
{
  if (samplekey > 209)
    display.setTextColor(COLOR_BACKGROUND, COLOR_PITCHSMP);
  else
    display.setTextColor(COLOR_BACKGROUND, COLOR_DRUMS);
}

// void seq_pattern_editor_update_disp_instr()
//{
//   if (seq.cycle_touch_element == 1)
//   {
//     display.setCursor(CHAR_width, 110);
//     display.setTextColor(COLOR_SYSTEXT, COLOR_BACKGROUND);
//     display.setTextSize(1);
//     display.print(F("PATTERN "));
//     display.setTextColor(GREEN, COLOR_BACKGROUND);
//     print_formatted_number(seq.active_pattern, 2);
//     display.setTextColor(COLOR_SYSTEXT, COLOR_BACKGROUND);
//     display.print(F(" PLAYING:"));
//     if (seq.content_type[seq.active_pattern] == 1) //Inst Pattern
//     {
//       //UI_draw_waveform(activesample);
//
//
//       print_voice_settings_in_pattern_editor(12, 125);
//     }
//     else if (seq.content_type[seq.active_pattern] == 0)
//     {
//       display.setTextColor(COLOR_SYSTEXT, COLOR_BACKGROUND);
//       display.setCursor(CHAR_width, 125);
//       display.setTextSize(2);
//       display.print(F("DRUMS"));
//       display.fillRect(CHAR_width * 6, 124, 129, CHAR_height, COLOR_BACKGROUND);
//     }
//     else
//       display.fillRect(CHAR_width, 110, 70, 0, COLOR_PITCHSMP);
//
//   }
//
//   if (LCDML.FUNC_getID() == LCDML.OTHER_getIDFromFunction(UI_func_seq_pattern_editor))
//   {
//     display.setCursor ( 240, 110  );
//     virtual_keyboard_print_current_instrument();
//   }
// }

void virtual_keyboard_print_buttons()
{
  // oct +- buttons
  draw_button_on_grid(1, 16, "OCTAVE", "-", 0);
  draw_button_on_grid(45, 16, "OCTAVE", "+", 0);

  // instrument buttons
  draw_button_on_grid(9, 16, "INSTR.", "-", 0);
  draw_button_on_grid(37, 16, "INSTR.", "+", 0);
}

void seq_pattern_editor_update_dynamic_elements()
{
  if (seq.running == false)
  {
    if (seq.step_recording)
      draw_button_on_grid(36, 1, "RECORD", "ACTIVE", 2); // print step recorder icon
    else
      draw_button_on_grid(36, 1, "STEP", "RECORD", 1); // print step recorder icon
  }
  if (seq.cycle_touch_element == 0)
  {
    draw_button_on_grid(45, 1, "", "", 99); // print keyboard icon

    if (seq.play_mode == false) // is in full song more
      draw_button_on_grid(36, 16, "PLAYNG", "SONG", 0);
    else // play only current pattern
      draw_button_on_grid(36, 16, "LOOP", "PATT", 2);
    if (LCDML.FUNC_getID() == LCDML.OTHER_getIDFromFunction(UI_func_seq_pattern_editor))
      draw_button_on_grid(45, 16, "JUMP", "TOOLS", 0);
    else
      draw_button_on_grid(45, 16, "-", "-", 0);

    draw_button_on_grid(36, 22, "SONG", "EDITOR", 0);
    draw_button_on_grid(45, 22, "HUNT", "PATT", 0);

    if (seq.content_type[seq.active_pattern] == 0) // Drum Mode
    {
      print_track_steps_detailed(0, CHAR_height * 4 + 3, 254, true, true);
      seq.note_editor_view = 0;
    }
    else
    {
      print_single_pattern_pianoroll_in_pattern_editor(0, DISPLAY_HEIGHT, seq.active_pattern, seq.menu - 3, true);
      seq.note_editor_view = 1;
    }
    // seq_sub_print_track_assignments(CHAR_width * 12, CHAR_height * 2, true);
    print_current_sample_and_pitch_buffer();
  }
  else if (seq.cycle_touch_element == 1)
  {
    virtual_keyboard_print_buttons();
    draw_button_on_grid(45, 1, "BACK", "TO SEQ", 0);
    virtual_keyboard();
    update_step_rec_buttons();
    virtual_keyboard_print_current_instrument();
    virtual_keyboard_print_velocity_bar();
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
        if (seq.chain[seq.song[t][s]][c] == pattern)
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

uint8_t find_first_song_step_with_pattern(uint8_t pattern)
{
  uint8_t result = 99;
  for (uint8_t s = 0; s < SONG_LENGTH; s++)
  {
    for (uint8_t t = 0; t < NUM_SEQ_TRACKS; t++)
    {
      for (uint8_t c = 0; c < 16; c++)
      {
        if (seq.chain[seq.song[t][s]][c] == pattern)
        {
          result = s;
          return result;
          break;
        }
      }
    }
  }
  return result;
}

uint8_t find_first_chain_step_with_pattern(uint8_t pattern)
{
  uint8_t result = 99;
  for (uint8_t s = 0; s < SONG_LENGTH; s++)
  {
    for (uint8_t t = 0; t < NUM_SEQ_TRACKS; t++)
    {
      for (uint8_t c = 0; c < 16; c++)
      {
        if (seq.chain[seq.song[t][s]][c] == pattern)
        {
          result = c;
          return result;
          break;
        }
      }
    }
  }
  return result;
}

void pattern_editor_menu_0()
{
  display.setTextSize(2);
  if (seq.menu == 0) // sound select menu
  {
    print_current_sample_and_pitch_buffer();
    if (seq.active_function != 99)
      display.setTextColor(RED, COLOR_BACKGROUND);
    else
      display.setTextColor(COLOR_SYSTEXT, COLOR_BACKGROUND);
    setCursor_textGrid(11, 0);
    display.print(" ");
    setCursor_textGrid(13, 0);
    display.print(" ");
    setCursor_textGrid(0, 0);
    display.print("[");
    if (seq.content_type[seq.active_pattern] == 0) // Drum Mode
    {
      // UI_draw_waveform(activesample);
      if (activesample < NUM_DRUMSET_CONFIG - 1)
      {
        if (drum_config[activesample].midinote < 210)
          display.setTextColor(COLOR_DRUMS, COLOR_BACKGROUND);
        else
          display.setTextColor(COLOR_PITCHSMP, COLOR_BACKGROUND);

        show(0, 1, 9, basename(drum_config[activesample].name));
        if (seq.active_function != 99)
          display.setTextColor(RED, COLOR_BACKGROUND);
        else
          display.setTextColor(COLOR_SYSTEXT, COLOR_BACKGROUND);
      }
      else if (activesample == NUM_DRUMSET_CONFIG - 1)
      {
        setCursor_textGrid(1, 0);
        display.print(F("EMPTY    "));
      }
      else if (activesample == NUM_DRUMSET_CONFIG)
      {
        setCursor_textGrid(1, 0);
        display.print(F("Clear Pat"));
      }
      else if (activesample == NUM_DRUMSET_CONFIG + 1)
      {
        setCursor_textGrid(1, 0);
        display.print(F("Clear All"));
      }
      else if (activesample == NUM_DRUMSET_CONFIG + 2)
      {
        setCursor_textGrid(1, 0);
        display.print(F("Copy Pat."));
      }
      else if (activesample == NUM_DRUMSET_CONFIG + 3)
      {
        setCursor_textGrid(1, 0);
        display.print(F("Swap Pat"));
      }
      else if (activesample == NUM_DRUMSET_CONFIG + 4)
      {
        setCursor_textGrid(1, 0);
        display.print(F("Fill Pat."));
      }
      else if (activesample == NUM_DRUMSET_CONFIG + 5)
      {
        setCursor_textGrid(1, 0);
        display.print(F("Transpose"));
      }
      setCursor_textGrid(10, 0);
      display.print("]");
    }
    else // Inst. Mode
    {
      if (temp_int < 109)
      {
        setCursor_textGrid(1, 0);
        if (seq.content_type[seq.active_pattern] == 1) // Inst
        {
          display.print(noteNames[temp_int % 12]);
          display.print((temp_int / 12) - 1);
          display.print(F("  "));
        }
        else if (seq.content_type[seq.active_pattern] == 2) // Chord
        {
          // print_chord_name(seq.menu - 3);
          setCursor_textGrid(1, 0);
          display.print(noteNames[temp_int % 12]);
          display.print((temp_int / 12) - 1);
          display.print(F("  "));
        }
      }
      else if (temp_int == 109)
      {
        setCursor_textGrid(1, 0);
        display.print(F("EMPTY    "));
      }
      else if (temp_int == 110)
      {
        setCursor_textGrid(1, 0);
        display.print(F("LATCH    "));
      }
      else if (temp_int == 111)
      {
        setCursor_textGrid(1, 0);
        display.print(F("ClearPat."));
      }
      else if (temp_int == 112)
      {
        setCursor_textGrid(1, 0);
        display.print(F("Clear All"));
      }
      else if (temp_int == 113)
      {
        setCursor_textGrid(1, 0);
        display.print(F("Copy Pat."));
      }
      else if (temp_int == 114)
      {
        setCursor_textGrid(1, 0);
        display.print(F("Swap Pat."));
      }
      else if (temp_int == 115)
      {
        setCursor_textGrid(1, 0);
        display.print(F("Fill Pat."));
      }
      else if (temp_int == 116)
      {
        setCursor_textGrid(1, 0);
        display.print(F("Transpose"));
      }
      setCursor_textGrid(10, 0);
      display.print("]");
      if (temp_int == 108)
      {
        setCursor_textGrid(5, 0);
        display.print(F("  "));
      }
    }
  }
}

FLASHMEM void play_sample_on_virtual_drumpads(uint8_t note)
{
  handleNoteOn_MIDI_DEVICE_DIN(ts.virtual_keyboard_midi_channel, drum_config[note - 14].midinote, ts.virtual_keyboard_velocity);
}

void UI_func_seq_pattern_editor(uint8_t param)
{
  if (LCDML.FUNC_setup()) // ****** SETUP *********
  {
    // setup function
    seq.menu = 3;

    if (seq.cycle_touch_element != 1)
      draw_button_on_grid(45, 16, "JUMP", "TOOLS", 0);

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
    print_formatted_number(seq.active_pattern, 2);

    if (seq.menu_status != 2)
    {
      seq_printAllSeqSteps();
      seq_printVelGraphBar();
    }

    display.setTextSize(2);
  }
  if (LCDML.FUNC_loop()) // ****** LOOP *********
  {
    seq_sub_pitch_edit_pitched_sample();
    seq_sub_pattern_fill();
    seq_sub_copy_swap();
    seq_sub_pattern_transpose();

    if (seq.active_function == 99)
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
            pattern_editor_play_current_step(seq.menu - 3);
          }
        }
        else if (LCDML.BT_checkUp())
        {
          if (seq.menu == 0 && seq.quicknav_song_to_pattern_jump == true)
          {
            // go back to song-transpose when previously navigated in from song edit
            seq.help_text_needs_refresh = true;
            seq.edit_state = true;
            seq.quicknav_pattern_to_song_jump = true;
            seq.quicknav_song_to_pattern_jump = false;
            LCDML.OTHER_jumpToFunc(UI_func_song);
          }
          else
            seq.menu = constrain(seq.menu - 1, 0, 18);
          pattern_editor_play_current_step(seq.menu - 3);
        }
      }
    }
    else if (seq.active_function == 0)
    {
      if (seq.content_type[seq.active_pattern] == 0) // is in Drumedit mode
      {
        if ((LCDML.BT_checkDown() && encoderDir[ENC_R].Down()) || (LCDML.BT_checkUp() && encoderDir[ENC_R].Up()))
        {
          if (LCDML.BT_checkDown())
            activesample = constrain(activesample + 1, 0, NUM_DRUMSET_CONFIG + 5);
          else if (LCDML.BT_checkUp())
            activesample = constrain(activesample - 1, 0, NUM_DRUMSET_CONFIG + 5);

          if (seq.running == false && activesample < NUM_DRUMSET_CONFIG - 1) {
            handleNoteOn(drum_midi_channel, drum_config[activesample].midinote, 90, 0);
          }
        }
      }
      else // is in Instrument Mode
      {
        if ((LCDML.BT_checkDown() && encoderDir[ENC_R].Down()) || (LCDML.BT_checkUp() && encoderDir[ENC_R].Up()))
        {
          if (LCDML.BT_checkDown())
            temp_int = constrain(temp_int + 1, 0, 116);
          else if (LCDML.BT_checkUp())
            temp_int = constrain(temp_int - 1, 0, 116);
        }
      }
    }
    else if (seq.active_function == 2)
    {
      if ((LCDML.BT_checkDown() && encoderDir[ENC_R].Down()) || (LCDML.BT_checkUp() && encoderDir[ENC_R].Up()))
      {
        if (LCDML.BT_checkDown())
          seq.active_pattern = constrain(seq.active_pattern + 1, 0, NUM_SEQ_PATTERN - 1);
        else if (LCDML.BT_checkUp())
          seq.active_pattern = constrain(seq.active_pattern - 1, 0, NUM_SEQ_PATTERN - 1);

        display.setCursor(11 * CHAR_width_small, CHAR_height * 3 + 3);
        print_content_type();
        virtual_keyboard_smart_preselect_mode();
        update_latch_button();
      }
    }
    if (LCDML.BT_checkEnter()) // handle button presses during menu >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
    {

      if (seq.menu == 0 && seq.active_function == 0 && seq.cycle_touch_element != 1)
      {
        if (LCDML.FUNC_getID() == LCDML.OTHER_getIDFromFunction(UI_func_seq_pattern_editor))
          draw_button_on_grid(45, 16, "JUMP", "TOOLS", 0);
        display.setTextSize(2);
      }
      if (seq.menu == 0 && seq.active_function == 99)
      {
        seq.active_function = 0;
      }
      else if (seq.menu == 0 && seq.active_function == 0)
      {
        seq_sub_clear_pattern_or_clear_all();
      }
      if (seq.menu == 1 && seq.active_function != 40)
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
      else if (seq.menu == 2 && seq.active_function != 40)
      {
        if (seq.active_function != 2)
          seq.active_function = 2;
        else
          seq.active_function = 99;

        if (seq.content_type[seq.active_pattern] == 0) // drumtrack
        {
          if (activesample < NUM_DRUMSET_CONFIG - 1 && seq.vel[seq.active_pattern][seq.menu - 3] < 210) // normal sample
          {
            // show(0, 1, 9, basename(drum_config[activesample].name));
            ;
          }
          else
          {
            if (activesample < NUM_DRUMSET_CONFIG - 1 && seq.vel[seq.active_pattern][seq.menu - 3] > 209) // pitched sample
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
      else if (seq.menu > 2 && seq.menu < 30 && seq.active_function != 40)
      {
        if (seq.active_function == 99)
        {
          if (seq.content_type[seq.active_pattern] == 0)
          { // Drumtrack

            if (drum_config[activesample].midinote > 209) // it selected sample is a pitched sample
            {
              // Step is empty - put in selected pitched sample with selected note value
              if (seq.note_data[seq.active_pattern][seq.menu - 3] == 0)
              {
                seq.vel[seq.active_pattern][seq.menu - 3] = drum_config[activesample].midinote;
                if (temp_int > 12)
                  seq.note_data[seq.active_pattern][seq.menu - 3] = temp_int;
                else
                  seq.note_data[seq.active_pattern][seq.menu - 3] = 12; // else insert C2
                pattern_editor_play_current_step(seq.menu - 3);
              }
              else
              { // step is an other sample, replace with selected pitched sample
                seq.vel[seq.active_pattern][seq.menu - 3] = drum_config[activesample].midinote;
              }
            }
            else

              if (drum_config[activesample].midinote < 210)
              {
                // normal sample: check if note is already there, if not -> insert it, else remove it from grid.
                if (seq.note_data[seq.active_pattern][seq.menu - 3] == drum_config[activesample].midinote)
                {
                  seq.note_data[seq.active_pattern][seq.menu - 3] = 0;
                  seq.vel[seq.active_pattern][seq.menu - 3] = 0;
                }
                else
                {
                  seq.note_data[seq.active_pattern][seq.menu - 3] = drum_config[activesample].midinote;
                  seq.vel[seq.active_pattern][seq.menu - 3] = 120;
                  pattern_editor_play_current_step(seq.menu - 3);
                }
              }
          }
          else
          { // Inst. Track
            if (temp_int == 109 || seq.note_data[seq.active_pattern][seq.menu - 3] == temp_int)
            { // clear note
              seq.note_data[seq.active_pattern][seq.menu - 3] = 0;
              seq.vel[seq.active_pattern][seq.menu - 3] = 0;
            }
            else if (temp_int == 110)
            { // latch note
              seq.note_data[seq.active_pattern][seq.menu - 3] = 130;
              // seq.vel[seq.active_pattern][seq.menu - 3] = 0;
            }
            else
            {
              if (seq.note_data[seq.active_pattern][seq.menu - 3] != temp_int)
              {

                seq.note_data[seq.active_pattern][seq.menu - 3] = temp_int;
                if (seq.content_type[seq.active_pattern] == 2) // Chords
                  seq.vel[seq.active_pattern][seq.menu - 3] = 200;
                else // normal note
                  seq.vel[seq.active_pattern][seq.menu - 3] = 120;
                pattern_editor_play_current_step(seq.menu - 3);
              }
            }
          }
        }
        else
          seq.active_function = 99;
        display.setTextSize(2);
        seq_printAllSeqSteps();
        seq_printVelGraphBar();
      }
    }
    // button check end <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<

    print_shortcut_navigator();
    display.setTextSize(2);
    if (seq.menu == 0) // sound select menu
      pattern_editor_menu_0();

    else if (seq.menu == 1)
    {
      display.setTextColor(GREY2, COLOR_BACKGROUND);
      setCursor_textGrid(15, 0);
      print_formatted_number(seq.active_pattern, 2);
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
      // seq_pattern_editor_update_disp_instr();
      if (seq.cycle_touch_element != 1)
        display.fillRect(0, 3 * CHAR_height + 17, 212, 8, COLOR_BACKGROUND);
      if (seq.active_function != 99)
        display.setTextColor(RED, COLOR_BACKGROUND);
      else
        display.setTextColor(COLOR_SYSTEXT, COLOR_BACKGROUND);
      display.setTextSize(2);
      setCursor_textGrid(11, 0);
      display.print(" ");
      setCursor_textGrid(13, 0);
      display.print(" ");
      setCursor_textGrid(14, 0);
      display.print("[");
      setCursor_textGrid(15, 0);
      print_formatted_number(seq.active_pattern, 2);
      setCursor_textGrid(17, 0);
      display.print("]");
      display.setTextColor(COLOR_SYSTEXT, COLOR_BACKGROUND);
      if (seq.content_type[seq.active_pattern] == 0) // Drum Mode
      {
        setCursor_textGrid(0, 0); // Print current sample name when switching track and track is drum track
        display.print(" ");
        if (drum_config[activesample].midinote < 210)
          display.setTextColor(COLOR_DRUMS, COLOR_BACKGROUND);
        else
          display.setTextColor(COLOR_PITCHSMP, COLOR_BACKGROUND);
        show(0, 1, 9, basename(drum_config[activesample].name));
        display.setTextColor(COLOR_SYSTEXT, COLOR_BACKGROUND);
        display.print("  ");
        if (seq.note_editor_view != 0 && seq.cycle_touch_element != 1)
        {
          seq.note_editor_view = 0;
          border3_clear();
          print_track_steps_detailed(0, CHAR_height * 4 + 3, 254, true, true);
        }
        else
        {
          if (seq.cycle_touch_element != 1)
            print_track_steps_detailed(0, CHAR_height * 4 + 3, 254, false, true);
        }
      }
      else
      {
        // Print note buffer when switching track and track is an instrument track
        setCursor_textGrid(0, 0);
        display.print(" ");
        display.setTextColor(COLOR_INSTR, COLOR_BACKGROUND);
        display.print(noteNames[temp_int % 12][0]);
        if (noteNames[temp_int % 12][1] != '\0')
        {
          display.print(noteNames[temp_int % 12][1]);
        }
        display.print((temp_int / 12) - 1);
        display.print(F("       "));
        display.setTextColor(COLOR_SYSTEXT, COLOR_BACKGROUND);

        if (seq.note_editor_view != 1 && seq.cycle_touch_element != 1)
        {
          seq.note_editor_view = 1;
          border3_clear();
          print_single_pattern_pianoroll_in_pattern_editor(0, DISPLAY_HEIGHT, seq.active_pattern, seq.menu - 3, true);

        }
        else
        {
          if (seq.cycle_touch_element != 1)
            print_single_pattern_pianoroll_in_pattern_editor(0, DISPLAY_HEIGHT, seq.active_pattern, seq.menu - 3, false);
        }
      }
      display.setTextSize(2);
      seq_printAllSeqSteps();
      seq_printVelGraphBar();
      if (seq.cycle_touch_element != 1)
        print_current_sample_and_pitch_buffer();
      // seq_sub_print_track_assignments(CHAR_width * 12, CHAR_height * 2, false);
    }
    if (seq.menu == 3)
    {
      display.setTextColor(GREY2, COLOR_BACKGROUND);
      setCursor_textGrid(15, 0);
      print_formatted_number(seq.active_pattern, 2);

      setCursor_textGrid(14, 0);
      display.print(" ");
      setCursor_textGrid(17, 0);
      display.print(" ");
    }
    if (seq.menu > 2 && seq.menu < 19 && seq.active_function != 40)
    {
      if (seq.content_type[seq.active_pattern] == 0) // Drum Mode
      {
        if (seq.cycle_touch_element != 1)
          print_track_steps_detailed(0, CHAR_height * 4 + 3, seq.menu - 3, false, false);
      }
      else
      {
        if (seq.cycle_touch_element != 1)
          print_single_pattern_pianoroll_in_pattern_editor(0, DISPLAY_HEIGHT, seq.active_pattern, seq.menu - 3, false);
      }
      if (seq.menu == 3)
        setCursor_textGrid(0, 1);
      else
        setCursor_textGrid(seq.menu - 3, 1);

      if (seq.menu - 3 > 15 - seq.pattern_len_dec)
        display.setTextColor(COLOR_BACKGROUND, RED);
      else
        display.setTextColor(COLOR_SYSTEXT, RED);
      display.print(seq_find_shortname(seq.menu - 3)[0]);
      if (seq.menu > 3)
      {
        setCursor_textGrid(seq.menu - 4, 1);

        if (seq.menu - 4 > 15 - seq.pattern_len_dec)
          display.setTextColor(GREY3, COLOR_BACKGROUND);

        else   if (seq.vel[seq.active_pattern][seq.menu - 4] > 209) // if pitched sample, change color
          display.setTextColor(COLOR_PITCHSMP, COLOR_BACKGROUND);
        else
          set_pattern_content_type_color(seq.active_pattern);
        display.print(seq_find_shortname(seq.menu - 4)[0]);
      }
      if (seq.menu < 18)
      {
        setCursor_textGrid(seq.menu - 2, 1);

        if (seq.menu - 2 > 15 - seq.pattern_len_dec)
          display.setTextColor(GREY3, COLOR_BACKGROUND);
        else  if (seq.vel[seq.active_pattern][seq.menu - 2] > 209) // if pitched sample, change color
          display.setTextColor(COLOR_PITCHSMP, COLOR_BACKGROUND);
        else
          set_pattern_content_type_color(seq.active_pattern);


        display.print(seq_find_shortname(seq.menu - 2)[0]);
      }
      if (seq.content_type[seq.active_pattern] > 0 && seq.vel[seq.active_pattern][seq.menu - 3] < 210 && seq.note_data[seq.active_pattern][seq.menu - 3] != 0) // is not Drum Mode and not empty, print note
      {
        setCursor_textGrid(0, 0);
        display.print(" ");
        seq_print_current_note_from_step(seq.menu - 3);
      }
      else if (seq.content_type[seq.active_pattern] == 0) // is drum pattern
      {
        // print current sample name on active step if not empty
        if (seq.note_data[seq.active_pattern][seq.menu - 3] > 0)
        {
          setCursor_textGrid(0, 0);
          display.print(" ");

          if (activesample < NUM_DRUMSET_CONFIG - 1 && seq.vel[seq.active_pattern][seq.menu - 3] < 210) // normal sample
          {
            show(0, 1, 9, seq_find_drum_name_from_note(seq.note_data[seq.active_pattern][seq.menu - 3]));
            // UI_draw_waveform(seq_find_drum_data_from_note  (seq.note_data[seq.active_pattern][seq.menu - 3])  );

            // check if the same note is already there, if so ask to clear it
            if (seq.note_data[seq.active_pattern][seq.menu - 3] == drum_config[activesample].midinote)
            {
              display.setTextColor(COLOR_SYSTEXT, COLOR_BACKGROUND);
              display.setCursor(0, 3 * CHAR_height + 17);
              display.setTextSize(1);
              display.setTextColor(GREEN, COLOR_BACKGROUND);
              display.print("CLEAR ");
              set_sample_type_color();
              display.print(basename(drum_config[activesample].name));
              display.setTextColor(GREEN, COLOR_BACKGROUND);
              display.print(" FROM STEP ");
              display.setTextColor(COLOR_SYSTEXT, COLOR_BACKGROUND);
              display.print(seq.menu - 2);
              display.setTextColor(GREEN, COLOR_BACKGROUND);
              display.print(" ?");
              fill_up_with_spaces_left_window();
              display.setTextColor(COLOR_SYSTEXT, COLOR_BACKGROUND);
            }
            // else if there is an other note, ask to replace it
            else if (seq.note_data[seq.active_pattern][seq.menu - 3] != drum_config[activesample].midinote && seq.note_data[seq.active_pattern][seq.menu - 3] > 0)
            {
              display.setTextColor(COLOR_SYSTEXT, COLOR_BACKGROUND);
              display.setCursor(0, 3 * CHAR_height + 17);
              display.setTextSize(1);
              display.setTextColor(GREEN, COLOR_BACKGROUND);
              display.print(F("REPLACE "));
              set_sample_type_color_of(seq.note_data[seq.active_pattern][seq.menu - 3]);
              display.print(find_long_drum_name_from_note(seq.note_data[seq.active_pattern][seq.menu - 3]));
              display.setTextColor(GREEN, COLOR_BACKGROUND);
              display.print(F(" WITH "));
              set_sample_type_color();
              display.print(basename(drum_config[activesample].name));
              display.setTextColor(GREEN, COLOR_BACKGROUND);
              display.print(F(" ?"));
              fill_up_with_spaces_left_window();
              display.setTextColor(COLOR_SYSTEXT, COLOR_BACKGROUND);
            }
          }
          else
          {
            if (activesample < NUM_DRUMSET_CONFIG - 1 && seq.vel[seq.active_pattern][seq.menu - 3] > 209) // pitched sample
            {
              show(0, 1, 9, basename(drum_config[seq.vel[seq.active_pattern][seq.menu - 3] - 210].name));
              // UI_draw_waveform(seq.vel[seq.active_pattern][seq.menu - 3] - 210 );

              if (seq.vel[seq.active_pattern][seq.menu - 3] == drum_config[activesample].midinote) // pitched sample is idential, ask to edit pitch/delete
              {
                display.setTextColor(COLOR_SYSTEXT, COLOR_BACKGROUND);
                display.setCursor(0, 3 * CHAR_height + 17);
                display.setTextSize(1);
                display.setTextColor(GREEN, COLOR_BACKGROUND);
                display.print(F("EDIT/"));
                display.setTextColor(RED, COLOR_BACKGROUND);
                display.print(F("DELETE "));
                set_sample_type_color();
                display.print(basename(drum_config[activesample].name));
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
                display.print(basename(drum_config[seq.vel[seq.active_pattern][seq.menu - 3] - 210].name));
                display.setTextColor(GREEN, COLOR_BACKGROUND);
                display.print(F(" WITH "));
                set_sample_type_color();
                display.print(basename(drum_config[activesample].name));

                display.setTextColor(GREEN, COLOR_BACKGROUND);
                display.print(" ?");
                display.setTextColor(COLOR_SYSTEXT, COLOR_BACKGROUND);
              }
            }
          }
        }
        else if (seq.note_data[seq.active_pattern][seq.menu - 3] == 0 && seq.vel[seq.active_pattern][seq.menu - 3] < 210) // if step empty, print selected, active sample and ask if should be inserted
        {
          setCursor_textGrid(0, 0);
          display.print(" ");
          display.setTextColor(GREY2, COLOR_BACKGROUND);
          show(0, 1, 11, "--EMPTY--");
          display.setTextColor(COLOR_SYSTEXT, COLOR_BACKGROUND);
          display.setCursor(0, 3 * CHAR_height + 17);
          display.setTextSize(1);
          display.setTextColor(GREEN, COLOR_BACKGROUND);
          display.print(F("INSERT "));
          set_sample_type_color();
          display.print(basename(drum_config[activesample].name));
          display.setTextColor(GREEN, COLOR_BACKGROUND);
          display.print(F(" IN STEP "));
          display.setTextColor(COLOR_SYSTEXT, COLOR_BACKGROUND);
          display.print(seq.menu - 2);
          display.setTextColor(GREEN, COLOR_BACKGROUND);
          display.print(F(" ?"));
          fill_up_with_spaces_left_window();
          display.setTextColor(COLOR_SYSTEXT, COLOR_BACKGROUND);
        }
        else if (seq.active_function == 40 && activesample != NUM_DRUMSET_CONFIG - 1)
        { // is in pitch edit function 40
          setCursor_textGrid(0, 0);
          show(0, 1, 4, basename(drum_config[activesample].name));
          setCursor_textGrid(6, 0);
          seq_print_current_note_from_step(seq.menu - 3);
        }
      }
    }
    // instr. pitch edit/delete/exchange
    if (seq.menu > 2 && seq.menu - 3 < 16 && seq.content_type[seq.active_pattern] > 0)
    {
      if (seq.note_data[seq.active_pattern][seq.menu - 3] == 0) // insert note buffer if step is empty
      {
        display.setCursor(0, 3 * CHAR_height + 17);
        display.setTextSize(1);

        display.setTextColor(GREEN, COLOR_BACKGROUND);
        display.print(F("INSERT NOTE "));
        display.setTextColor(COLOR_INSTR, COLOR_BACKGROUND);
        display.print(noteNames[temp_int % 12][0]);
        if (noteNames[temp_int % 12][1] != '\0')
        {
          display.print(noteNames[temp_int % 12][1]);
        }
        display.print(temp_int / 12 - 1);
        display.setTextColor(GREEN, COLOR_BACKGROUND);
        display.print(F(" IN STEP "));
        display.setTextColor(COLOR_SYSTEXT, COLOR_BACKGROUND);
        display.print(seq.menu - 2);
        display.setTextColor(GREEN, COLOR_BACKGROUND);
        display.print(F(" ?   "));
        display.setTextSize(2);
      }
      else // change note if different
        if (seq.note_data[seq.active_pattern][seq.menu - 3] > 0 && seq.note_data[seq.active_pattern][seq.menu - 3] != temp_int)
        {
          display.setCursor(0, 3 * CHAR_height + 17);
          display.setTextSize(1);
          display.setTextColor(GREEN, COLOR_BACKGROUND);
          display.print(F("CHANGE "));
          display.setTextColor(COLOR_INSTR, COLOR_BACKGROUND);
          display.print(noteNames[seq.note_data[seq.active_pattern][seq.menu - 3] % 12][0]);
          if (noteNames[seq.note_data[seq.active_pattern][seq.menu - 3] % 12][1] != '\0')
          {
            display.print(noteNames[seq.note_data[seq.active_pattern][seq.menu - 3] % 12][1]);
          }
          display.print((seq.note_data[seq.active_pattern][seq.menu - 3] / 12) - 1);
          display.setTextColor(GREEN, COLOR_BACKGROUND);
          display.print(" > ");
          display.setTextColor(COLOR_INSTR, COLOR_BACKGROUND);
          display.print(noteNames[temp_int % 12][0]);
          if (noteNames[temp_int % 12][1] != '\0')
          {
            display.print(noteNames[temp_int % 12][1]);
          }
          display.print(temp_int / 12 - 1);
          display.setTextColor(GREEN, COLOR_BACKGROUND);
          display.print(F(" IN STEP "));
          display.setTextColor(COLOR_SYSTEXT, COLOR_BACKGROUND);
          display.print(seq.menu - 2);
          display.setTextColor(GREEN, COLOR_BACKGROUND);
          display.print(" ? ");
          display.setTextSize(2);
        }
        else if (seq.note_data[seq.active_pattern][seq.menu - 3] == temp_int) // ask to delete note if it is the same
        {
          display.setCursor(0, 3 * CHAR_height + 17);
          display.setTextSize(1);
          display.setTextColor(RED, COLOR_BACKGROUND);
          display.print(F("DELETE"));
          display.setTextColor(GREEN, COLOR_BACKGROUND);
          display.print(F(" NOTE "));
          display.setTextColor(COLOR_INSTR, COLOR_BACKGROUND);
          display.print(noteNames[seq.note_data[seq.active_pattern][seq.menu - 3] % 12][0]);
          if (noteNames[seq.note_data[seq.active_pattern][seq.menu - 3] % 12][1] != '\0')
          {
            display.print(noteNames[seq.note_data[seq.active_pattern][seq.menu - 3] % 12][1]);
          }
          display.print((seq.note_data[seq.active_pattern][seq.menu - 3] / 12) - 1);
          display.setTextColor(GREEN, COLOR_BACKGROUND);
          display.print(" IN STEP ");
          display.print(seq.menu - 2);
          display.setTextColor(GREEN, COLOR_BACKGROUND);
          display.print(" ?   ");
          display.setTextSize(2);
        }
    }
    virtual_keyboard_smart_preselect_mode();
  }
  if (LCDML.FUNC_close()) // ****** STABLE END *********
  {
    encoderDir[ENC_R].reset();
    seq.menu = 0;
    seq.active_function = 99;
    display.setTextSize(2);
    if (seq.menu_status != 1) // do not clear screen, jumping to velocity edit
    {
      display.fillScreen(COLOR_BACKGROUND);
      seq.menu_status = 0;
    }
  }
}

void UI_toplineInfoText(uint8_t s)
{
  // s=size 1/2 lines
  if (remote_active)
    display.console = true;
  if (s == 2)
    display.fillRect(0, 0, DISPLAY_WIDTH - 1, CHAR_height_small * 2 + 3, DX_DARKCYAN);
  else if (s == 1)
    display.fillRect(0, 0, DISPLAY_WIDTH - 1, CHAR_height_small * 1 + 3, DX_DARKCYAN);
  if (remote_active)
    display.console = false;
}

void update_microsynth_instance_icons()
{
  display.setTextSize(1);
  if (microsynth_selected_instance == 0)
  {
    setCursor_textGrid_small(13, 1);
    display.setTextColor(COLOR_BACKGROUND, COLOR_SYSTEXT);
    display.print(F("1"));
    display.setTextColor(GREY1, COLOR_BACKGROUND);
    setCursor_textGrid_small(15, 1);
    display.print(F("2"));
  }
  else
  {
    setCursor_textGrid_small(13, 1);
    display.setTextColor(GREY1, COLOR_BACKGROUND);
    display.print(F("1"));

    display.setTextColor(COLOR_BACKGROUND, COLOR_SYSTEXT);
    setCursor_textGrid_small(15, 1);
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

    setCursor_textGrid_small(1, 17);
    display.print(F("PWM"));
    setCursor_textGrid_small(1, 18);
    display.print(F("SPEED"));
    display.setTextColor(GREY1);
  }
}

void microsynth_refresh_lower_screen_static_text()
{
  helptext_r("LONG PUSH:INST.SEL.  <>SEL.PARA.");

  display.setTextColor(GREY1, COLOR_BACKGROUND);
  setCursor_textGrid_small(1, 13);
  display.print(F("FILTER"));
  setCursor_textGrid_small(1, 14);
  display.print(F("FREQ"));
  setCursor_textGrid_small(1, 15);
  display.print(F("RES"));
  setCursor_textGrid_small(9, 15);
  display.print(F("SPEED"));
  setCursor_textGrid_small(22, 12);
  display.print(F("REV. SEND"));
  setCursor_textGrid_small(22, 13);
  display.print(F("CHR. SEND"));
  setCursor_textGrid_small(22, 14);
  display.print(F("DLY. SENDS"));
  setCursor_textGrid_small(22, 16);
  display.print(F("PANORAMA"));
  setCursor_textGrid_small(22, 17);
  display.print(F("MIDI CHN."));
  setCursor_textGrid_small(22, 19);
  display.print(F("MAP VELOCITY TO FILTER FREQ"));
  setCursor_textGrid_small(22, 20);
  display.print(F("OSC"));
  setCursor_textGrid_small(22, 21);
  display.print(F("NOISE"));
  setCursor_textGrid_small(13, 17);
  display.print(F(">"));
  setCursor_textGrid_small(13, 14);
  display.print(F(">"));
}

void microsynth_refresh_lower_screen_dynamic_text()
{
  if (menu_item_check(8))
  {
    setModeColor(8);
    setCursor_textGrid_small(9, 13);
    if (microsynth[microsynth_selected_instance].filter_osc_mode == 0)
      display.print(F("OFF   "));
    else if (microsynth[microsynth_selected_instance].filter_osc_mode == 1)
      display.print(F("LP12dB"));
    else if (microsynth[microsynth_selected_instance].filter_osc_mode == 2)
      display.print(F("BP12dB"));
    else if (microsynth[microsynth_selected_instance].filter_osc_mode == 3)
      display.print(F("HI12dB"));
  }
  if (menu_item_check(9))
    print_small_intbar(9, 14, microsynth[microsynth_selected_instance].filter_osc_freq_from / 100, 9, 0, 1);
  if (menu_item_check(10))
    print_small_intbar(15, 14, microsynth[microsynth_selected_instance].filter_osc_freq_to / 100, 10, 0, 1);
  if (menu_item_check(11))
    print_small_intbar(5, 15, microsynth[microsynth_selected_instance].filter_osc_resonance, 11, 0, 1);
  if (menu_item_check(12))
    print_small_intbar(15, 15, microsynth[microsynth_selected_instance].filter_osc_speed / 10, 12, 0, 1);
  if (menu_item_check(13))
    print_small_intbar(9, 17, microsynth[microsynth_selected_instance].pwm_from / 10, 13, 0, 1);
  if (menu_item_check(14))
    print_small_intbar(15, 17, microsynth[microsynth_selected_instance].pwm_to / 10, 14, 0, 1);

  if (menu_item_check(15))
  {
    setCursor_textGrid_small(10, 18);
    setModeColor(15);
    print_formatted_number(microsynth[microsynth_selected_instance].pwm_speed, 2);
  }
  if (menu_item_check(28))
  {
    setCursor_textGrid_small(33, 12);
    setModeColor(28);
    print_formatted_number(microsynth[microsynth_selected_instance].rev_send, 3);
  }
  if (menu_item_check(29))
  {
    setCursor_textGrid_small(33, 13);
    setModeColor(29);
    print_formatted_number(microsynth[microsynth_selected_instance].chorus_send, 3);
  }
  if (menu_item_check(30))
  {
    setCursor_textGrid_small(33, 14);
    setModeColor(30);
    print_formatted_number(microsynth[microsynth_selected_instance].delay_send[0], 3);
  }
  if (menu_item_check(31))
  {
    setModeColor(31);
    setCursor_textGrid_small(37, 14);
    print_formatted_number(microsynth[microsynth_selected_instance].delay_send[1], 3);
  }
  if (seq.cycle_touch_element != 1)
    if (menu_item_check(32))
      print_small_panbar(33, 16, microsynth[microsynth_selected_instance].pan, 32);
  if (menu_item_check(33))
  {
    setModeColor(33);
    setCursor_textGrid_small(33, 17);
    print_formatted_number(microsynth[microsynth_selected_instance].midi_channel, 3);
  }
  if (menu_item_check(34))
  {
    setModeColor(34);
    setCursor_textGrid_small(33, 20);

    print_formatted_number(microsynth[microsynth_selected_instance].vel_mod_filter_osc, 3);
  }
  if (menu_item_check(35))
  {
    setModeColor(35);
    setCursor_textGrid_small(33, 21);
    print_formatted_number(microsynth[microsynth_selected_instance].vel_mod_filter_noise, 3);
  }
  update_pwm_text();
}

void print_epiano_static_texts()
{
  display.setTextSize(1);
  helptext_l(back_text);
  helptext_r("< > SELECT PARAM");
  display.setTextSize(1);
  generic_active_function = 0;
  setCursor_textGrid_small(1, 1);
  display.setTextColor(RED);
  display.print(F("ELECTRIC PIANO"));
  display.setTextColor(COLOR_SYSTEXT);
  display.setTextColor(GREY1);
  setCursor_textGrid_small(1, 3);
  display.print(F("VOICE LEVEL"));
  setCursor_textGrid_small(1, 4);
  display.print(F("PANORAMA"));
  setCursor_textGrid_small(1, 5);
  display.print(F("TRANSPOSE"));
  display.setTextColor(GREY2);
  setCursor_textGrid_small(1, 7);
  display.print(F("AUDIO"));
  display.setTextColor(GREY1);
  setCursor_textGrid_small(1, 9);
  display.print(F("DECAY"));
  setCursor_textGrid_small(1, 10);
  display.print(F("RELEASE"));
  setCursor_textGrid_small(1, 11);
  display.print(F("HARDNESS"));
  setCursor_textGrid_small(1, 12);
  display.print(F("TREBLE"));
  setCursor_textGrid_small(1, 13);
  display.print(F("STEREO"));
  setCursor_textGrid_small(1, 15);
  display.print(F("TUNE"));
  setCursor_textGrid_small(1, 16);
  display.print(F("DETUNE"));

  display.setTextColor(GREY2);
  setCursor_textGrid_small(1, 18);
  display.print(F("SYSTEM"));
  display.setTextColor(GREY1);
  setCursor_textGrid_small(1, 20);
  display.print(F("POLYPHONY"));
  setCursor_textGrid_small(1, 21);
  display.print(F("VELOCITY SENSE"));

  display.setTextColor(GREY2);
  setCursor_textGrid_small(24, 1);
  display.print(F("EFFECTS"));
  display.setTextColor(GREY1);
  setCursor_textGrid_small(24, 3);
  display.print(F("OVERDRIVE"));
  setCursor_textGrid_small(24, 5);
  display.setTextColor(GREY2);
  display.print(F("TREMOLO"));
  display.setTextColor(GREY1);
  setCursor_textGrid_small(24, 6);
  display.print(F("WIDTH"));
  setCursor_textGrid_small(24, 7);
  display.print(F("LFO"));
  display.setTextColor(GREY2);
  setCursor_textGrid_small(24, 9);
  display.print(F("CHORUS"));
  display.setTextColor(GREY1);
  setCursor_textGrid_small(24, 10);
  display.print(F("FREQUENCY"));
  setCursor_textGrid_small(24, 11);
  display.print(F("WAVEFORM"));
  setCursor_textGrid_small(24, 12);
  display.print(F("DEPTH"));
  setCursor_textGrid_small(24, 13);
  display.print(F("LEVEL"));
  setCursor_textGrid_small(24, 15);
  display.print(F("DELAYS"));
  setCursor_textGrid_small(24, 17);
  display.print(F("REVERB S."));

  display.setTextColor(GREY1);
  setCursor_textGrid_small(24, 19);
  display.print(F("LOWEST NOTE"));
  setCursor_textGrid_small(24, 20);
  display.print(F("HIGHEST NOTE"));
  setCursor_textGrid_small(24, 21);
  display.print(F("MIDI CHANNEL"));

  display.setTextColor(GREY2, COLOR_BACKGROUND);
  setCursor_textGrid_small(41, 19);
  display.print(F("["));
  setCursor_textGrid_small(45, 19);
  display.print(F("]"));
  setCursor_textGrid_small(41, 20);
  display.print(F("["));
  setCursor_textGrid_small(45, 20);
  display.print(F("]"));
}
void update_selective_values_epiano()
{
  char note_name[4];
  if (menu_item_check(0))
    print_small_intbar(13, 3, configuration.epiano.sound_intensity, 0, 1, 0);
  if (generic_temp_select_menu == 0 && seq.edit_state)
    ep.setVolume(mapfloat(configuration.epiano.sound_intensity, SOUND_INTENSITY_MIN, SOUND_INTENSITY_MAX, 0, 1.0));
  if (menu_item_check(1))
    print_small_panbar(13, 4, configuration.epiano.pan, 1);
  if (generic_temp_select_menu == 1 && seq.edit_state)
    ep_stereo_panorama.panorama(mapfloat(configuration.epiano.pan, PANORAMA_MIN, PANORAMA_MAX, -1.0, 1.0));
  if (menu_item_check(2))
  {
    setModeColor(2);
    setCursor_textGrid_small(13, 5);
    print_formatted_number_signed(configuration.epiano.transpose - 24, 2);
  }
  if (menu_item_check(3))
    print_small_intbar(13, 9, configuration.epiano.decay, 3, 1, 1);
  if (generic_temp_select_menu == 3 && seq.edit_state)
    ep.setDecay(mapfloat(configuration.epiano.decay, EP_DECAY_MIN, EP_DECAY_MAX, 0, 1.0));
  if (menu_item_check(4))
    print_small_intbar(13, 10, configuration.epiano.release, 4, 1, 1);
  if (generic_temp_select_menu == 4 && seq.edit_state)
    ep.setRelease(mapfloat(configuration.epiano.release, EP_RELEASE_MIN, EP_RELEASE_MAX, 0, 1.0));
  if (menu_item_check(5))
    print_small_intbar(13, 11, configuration.epiano.hardness, 5, 1, 1);
  if (generic_temp_select_menu == 5 && seq.edit_state)
    ep.setHardness(mapfloat(configuration.epiano.hardness, EP_HARDNESS_MIN, EP_HARDNESS_MAX, 0, 1.0));
  if (menu_item_check(6))
    print_small_intbar(13, 12, configuration.epiano.treble, 6, 1, 1);
  if (generic_temp_select_menu == 6 && seq.edit_state)
    ep.setTreble(mapfloat(configuration.epiano.treble, EP_TREBLE_MIN, EP_TREBLE_MAX, 0, 1.0));
  if (menu_item_check(7))
    print_small_intbar(13, 13, configuration.epiano.stereo, 7, 1, 1);
  if (generic_temp_select_menu == 7 && seq.edit_state)
    ep.setStereo(mapfloat(configuration.epiano.stereo, EP_STEREO_MIN, EP_STEREO_MAX, 0, 1.0));
  if (menu_item_check(8))
  {
    setModeColor(8);
    setCursor_textGrid_small(13, 15);
    print_formatted_number_signed(configuration.epiano.tune - 100, 3);
  }
  if (generic_temp_select_menu == 8 && seq.edit_state)
    ep.setTune(mapfloat(configuration.epiano.tune, EP_TUNE_MIN, EP_TUNE_MAX, 0.0, 1.0));
  if (menu_item_check(9))
    print_small_intbar(13, 16, configuration.epiano.detune, 9, 1, 1);
  if (generic_temp_select_menu == 9 && seq.edit_state)
    ep.setDetune(mapfloat(configuration.epiano.detune, EP_DETUNE_MIN, EP_DETUNE_MAX, 0, 1.0));
  if (menu_item_check(10))
  {
    setModeColor(10);
    setCursor_textGrid_small(17, 20);
    print_formatted_number(configuration.epiano.polyphony, 2);
  }
  if (generic_temp_select_menu == 10 && seq.edit_state)
    ep.setPolyphony(configuration.epiano.polyphony);
  if (menu_item_check(11))
    print_small_intbar(17, 21, configuration.epiano.velocity_sense, 11, 0, 1);
  if (generic_temp_select_menu == 11 && seq.edit_state)
    ep.setVelocitySense(mapfloat(configuration.epiano.velocity_sense, EP_VELOCITY_SENSE_MIN, EP_VELOCITY_SENSE_MAX, 0, 1.0));
  if (menu_item_check(12))
    print_small_intbar(34, 3, configuration.epiano.overdrive, 12, 1, 0);
  if (generic_temp_select_menu == 12 && seq.edit_state)
    ep.setOverdrive(mapfloat(configuration.epiano.overdrive, EP_OVERDRIVE_MIN, EP_OVERDRIVE_MAX, 0, 1.0));
  if (menu_item_check(13))
    print_small_intbar(34, 6, configuration.epiano.pan_tremolo, 13, 1, 0);
  if (generic_temp_select_menu == 13 && seq.edit_state)
  {
    if (configuration.epiano.pan_tremolo == 0)
      ep.setPanTremolo(0.0);
    else
      ep.setPanTremolo(mapfloat(configuration.epiano.pan_tremolo, EP_PAN_TREMOLO_MIN, EP_PAN_TREMOLO_MAX, 0.0, 1.0));
  }
  if (menu_item_check(14))
    print_small_intbar(34, 7, configuration.epiano.pan_lfo, 14, 1, 0);
  if (generic_temp_select_menu == 14 && seq.edit_state)
  {
    if (configuration.epiano.pan_lfo == 0)
      ep.setPanLFO(0.0);
    else
      ep.setPanLFO(mapfloat(configuration.epiano.pan_lfo, EP_PAN_LFO_MIN, EP_PAN_LFO_MAX, 0.0, 1.0));
  }
  if (menu_item_check(15))
    print_small_intbar(34, 10, configuration.fx.ep_chorus_frequency, 15, 1, 0);
  if (generic_temp_select_menu == 15 && seq.edit_state)
    ep_chorus_modulator.frequency(configuration.fx.ep_chorus_frequency / 10.0);
  if (menu_item_check(16))
  {
    setModeColor(16);
    setCursor_textGrid_small(34, 11);
    switch (configuration.fx.ep_chorus_waveform)
    {
    case 0:
      if (generic_temp_select_menu == 16 && seq.edit_state)
        ep_chorus_modulator.begin(WAVEFORM_TRIANGLE);
      display.print(F("TRIANGLE"));
      break;
    case 1:
      if (generic_temp_select_menu == 16 && seq.edit_state)
        ep_chorus_modulator.begin(WAVEFORM_SINE);
      display.print(F("SINE    "));
      break;
    default:
      if (generic_temp_select_menu == 16 && seq.edit_state)
        ep_chorus_modulator.begin(WAVEFORM_TRIANGLE);
      display.print(F("TRIANGLE"));
      break;
    }
  }
  if (menu_item_check(17))
    print_small_intbar(34, 12, configuration.fx.ep_chorus_depth, 17, 1, 0);
  if (generic_temp_select_menu == 17 && seq.edit_state)
    ep_chorus_modulator.amplitude(mapfloat(configuration.fx.ep_chorus_depth, EP_CHORUS_DEPTH_MIN, EP_CHORUS_DEPTH_MAX, 0.0, 1.0));
  if (menu_item_check(18))
    print_small_intbar(34, 13, configuration.fx.ep_chorus_level, 18, 1, 0);
  if (generic_temp_select_menu == 18 && seq.edit_state)
  {
    ep_chorus_mixer_r.gain(1, mapfloat(configuration.fx.ep_chorus_level, EP_CHORUS_LEVEL_MIN, EP_CHORUS_LEVEL_MAX, 0.0, 0.5));
    ep_chorus_mixer_l.gain(1, mapfloat(configuration.fx.ep_chorus_level, EP_CHORUS_LEVEL_MIN, EP_CHORUS_LEVEL_MAX, 0.0, 0.5));
  }
  if (menu_item_check(19))
  {
    setModeColor(19);
    setCursor_textGrid_small(34, 15);
    print_formatted_number(configuration.fx.ep_delay_send_1, 3);
  }
  if (generic_temp_select_menu == 19 && seq.edit_state)
  {
    global_delay_in_mixer[0]->gain(5, mapfloat(configuration.fx.ep_delay_send_1, DELAY_LEVEL_MIN, DELAY_LEVEL_MAX, 0.0, 1.0));
    global_delay_in_mixer[0]->gain(6, mapfloat(configuration.fx.ep_delay_send_1, DELAY_LEVEL_MIN, DELAY_LEVEL_MAX, 0.0, 1.0));
  }
  if (menu_item_check(20))
  {
    setCursor_textGrid_small(38, 15);
    setModeColor(20);
    print_formatted_number(configuration.fx.ep_delay_send_2, 3);
  }
  if (generic_temp_select_menu == 20 && seq.edit_state)
  {
    global_delay_in_mixer[1]->gain(5, mapfloat(configuration.fx.ep_delay_send_2, DELAY_LEVEL_MIN, DELAY_LEVEL_MAX, 0.0, 1.0));
    global_delay_in_mixer[1]->gain(6, mapfloat(configuration.fx.ep_delay_send_2, DELAY_LEVEL_MIN, DELAY_LEVEL_MAX, 0.0, 1.0));
  }
  if (menu_item_check(21))
    print_small_intbar(34, 17, configuration.fx.ep_reverb_send, 21, 1, 0);
  if (generic_temp_select_menu == 21 && seq.edit_state)
  {
    reverb_mixer_r.gain(REVERB_MIX_CH_EPIANO, volume_transform(mapfloat(configuration.fx.ep_reverb_send, REVERB_SEND_MIN, REVERB_SEND_MAX, 0.0, VOL_MAX_FLOAT)));
    reverb_mixer_l.gain(REVERB_MIX_CH_EPIANO, volume_transform(mapfloat(configuration.fx.ep_reverb_send, REVERB_SEND_MIN, REVERB_SEND_MAX, 0.0, VOL_MAX_FLOAT)));
  }
  if (menu_item_check(22))
  {
    setModeColor(22);
    setCursor_textGrid_small(37, 19);
    getNoteName(note_name, configuration.epiano.lowest_note);
    display.print(note_name);
    setCursor_textGrid_small(42, 19);
    display.setTextColor(GREY2, COLOR_BACKGROUND);
    print_formatted_number(configuration.epiano.lowest_note, 3);
  }
  if (menu_item_check(23))
  {
    setModeColor(23);
    setCursor_textGrid_small(37, 20);
    getNoteName(note_name, configuration.epiano.highest_note);
    display.print(note_name);
    setCursor_textGrid_small(42, 20);
    display.setTextColor(GREY2, COLOR_BACKGROUND);
    print_formatted_number(configuration.epiano.highest_note, 3);
  }
  if (menu_item_check(24))
  {
    setModeColor(24);
    setCursor_textGrid_small(37, 21);
    if (configuration.epiano.midi_channel == MIDI_CHANNEL_OMNI)
    {
      display.print(F("OMNI"));
    }
    else
    {
      print_formatted_number(configuration.epiano.midi_channel, 2);
      display.print(F("  "));
    }
  }
}

void UI_func_epiano(uint8_t param)
{
  if (LCDML.FUNC_setup()) // ****** SETUP *********
  {
    // setup function
    encoderDir[ENC_R].reset();
    display.fillScreen(COLOR_BACKGROUND);
    print_epiano_static_texts();
    if (LCDML.MENU_getLastActiveFunctionID() != LCDML.OTHER_getIDFromFunction(UI_func_volume) && LCDML.MENU_getLastActiveFunctionID() != LCDML.OTHER_getIDFromFunction(mFunc_screensaver))
      generic_temp_select_menu = 0;

    generic_full_draw_required = true;
    update_selective_values_epiano(); // print all at init
    generic_full_draw_required = false;

    display.setTextSize(1);
  }
  if (LCDML.FUNC_loop()) // ****** LOOP *********
  {
    if ((LCDML.BT_checkDown() && encoderDir[ENC_R].Down()) || (LCDML.BT_checkUp() && encoderDir[ENC_R].Up()))
    {
      set_state_dir();
      if (seq.edit_state == 0)
      {
        generic_temp_select_menu = calc_val[state_dir](generic_temp_select_menu, 1, 0, 24);
        if (generic_temp_select_menu != menuhelper_previous_val)
          menuhelper_redraw = true;
      }
      else if (seq.edit_state == 1)
      {
        if (generic_temp_select_menu == 0)
        {
          configuration.epiano.sound_intensity = calc_val[state_dir](configuration.epiano.sound_intensity, ENCODER[ENC_R].speed(), SOUND_INTENSITY_MIN, SOUND_INTENSITY_MAX);
          MD_sendControlChange(configuration.epiano.midi_channel, 7, configuration.epiano.sound_intensity);
        }
        else if (generic_temp_select_menu == 1)
        {
          configuration.epiano.pan = calc_val[state_dir](configuration.epiano.pan, ENCODER[ENC_R].speed(), PANORAMA_MIN, PANORAMA_MAX);
          MD_sendControlChange(configuration.epiano.midi_channel, 10, map(configuration.epiano.pan, PANORAMA_MIN, PANORAMA_MAX, 0, 127));
        }
        else if (generic_temp_select_menu == 2)
          configuration.epiano.transpose = calc_val[state_dir](configuration.epiano.transpose, ENCODER[ENC_R].speed(), EP_TRANSPOSE_MIN, EP_TRANSPOSE_MAX);
        else if (generic_temp_select_menu == 3)
          configuration.epiano.decay = calc_val[state_dir](configuration.epiano.decay, ENCODER[ENC_R].speed(), EP_DECAY_MIN, EP_DECAY_MAX);
        else if (generic_temp_select_menu == 4)
          configuration.epiano.release = calc_val[state_dir](configuration.epiano.release, ENCODER[ENC_R].speed(), EP_RELEASE_MIN, EP_RELEASE_MAX);
        else if (generic_temp_select_menu == 5)
          configuration.epiano.hardness = calc_val[state_dir](configuration.epiano.hardness, ENCODER[ENC_R].speed(), EP_HARDNESS_MIN, EP_HARDNESS_MAX);
        else if (generic_temp_select_menu == 6)
          configuration.epiano.treble = calc_val[state_dir](configuration.epiano.treble, ENCODER[ENC_R].speed(), EP_TREBLE_MIN, EP_TREBLE_MAX);
        else if (generic_temp_select_menu == 7)
          configuration.epiano.stereo = calc_val[state_dir](configuration.epiano.stereo, ENCODER[ENC_R].speed(), EP_STEREO_MIN, EP_STEREO_MAX);
        else if (generic_temp_select_menu == 8)
        {
          configuration.epiano.tune = calc_val[state_dir](configuration.epiano.tune, ENCODER[ENC_R].speed(), EP_TUNE_MIN, EP_TUNE_MAX);
          MD_sendControlChange(configuration.epiano.midi_channel, 94, configuration.epiano.tune);
        }
        else if (generic_temp_select_menu == 9)
          configuration.epiano.detune = calc_val[state_dir](configuration.epiano.detune, ENCODER[ENC_R].speed(), EP_DETUNE_MIN, EP_DETUNE_MAX);
        else if (generic_temp_select_menu == 10)
          configuration.epiano.polyphony = calc_val[state_dir](configuration.epiano.polyphony, 1, EP_POLYPHONY_MIN, EP_POLYPHONY_MAX);
        else if (generic_temp_select_menu == 11)
          configuration.epiano.velocity_sense = calc_val[state_dir](configuration.epiano.velocity_sense, ENCODER[ENC_R].speed(), EP_VELOCITY_SENSE_MIN, EP_VELOCITY_SENSE_MAX);
        else if (generic_temp_select_menu == 12)
          configuration.epiano.overdrive = calc_val[state_dir](configuration.epiano.overdrive, ENCODER[ENC_R].speed(), EP_OVERDRIVE_MIN, EP_OVERDRIVE_MAX);
        else if (generic_temp_select_menu == 13)
          configuration.epiano.pan_tremolo = calc_val[state_dir](configuration.epiano.pan_tremolo, ENCODER[ENC_R].speed(), EP_PAN_TREMOLO_MIN, EP_PAN_TREMOLO_MAX);
        else if (generic_temp_select_menu == 14)
          configuration.epiano.pan_lfo = calc_val[state_dir](configuration.epiano.pan_lfo, ENCODER[ENC_R].speed(), EP_PAN_LFO_MIN, EP_PAN_LFO_MAX);
        else if (generic_temp_select_menu == 15)
          configuration.fx.ep_chorus_frequency = calc_val[state_dir](configuration.fx.ep_chorus_frequency, ENCODER[ENC_R].speed(), EP_CHORUS_FREQUENCY_MIN, EP_CHORUS_FREQUENCY_MAX);
        else if (generic_temp_select_menu == 16)
          configuration.fx.ep_chorus_waveform = calc_val[state_dir](configuration.fx.ep_chorus_waveform, 1, EP_CHORUS_WAVEFORM_MIN, EP_CHORUS_WAVEFORM_MAX);
        else if (generic_temp_select_menu == 17)
          configuration.fx.ep_chorus_depth = calc_val[state_dir](configuration.fx.ep_chorus_depth, ENCODER[ENC_R].speed(), EP_CHORUS_DEPTH_MIN, EP_CHORUS_DEPTH_MAX);
        else if (generic_temp_select_menu == 18)
        {
          configuration.fx.ep_chorus_level = calc_val[state_dir](configuration.fx.ep_chorus_level, ENCODER[ENC_R].speed(), EP_CHORUS_LEVEL_MIN, EP_CHORUS_LEVEL_MAX);
          MD_sendControlChange(configuration.epiano.midi_channel, 93, configuration.fx.ep_chorus_level);
        }
        else if (generic_temp_select_menu == 19)
        {
          configuration.fx.ep_delay_send_1 = calc_val[state_dir](configuration.fx.ep_delay_send_1, ENCODER[ENC_R].speed(), DELAY_LEVEL_MIN, DELAY_LEVEL_MAX);
        }
        else if (generic_temp_select_menu == 20)
        {
          configuration.fx.ep_delay_send_2 = calc_val[state_dir](configuration.fx.ep_delay_send_2, ENCODER[ENC_R].speed(), DELAY_LEVEL_MIN, DELAY_LEVEL_MAX);
        }
        else if (generic_temp_select_menu == 21)
        {
          configuration.fx.ep_reverb_send = calc_val[state_dir](configuration.fx.ep_reverb_send, ENCODER[ENC_R].speed(), REVERB_SEND_MIN, REVERB_SEND_MAX);
          MD_sendControlChange(configuration.epiano.midi_channel, 91, configuration.fx.ep_reverb_send);
        }
        else if (generic_temp_select_menu == 22)
          configuration.epiano.lowest_note = calc_val[state_dir](configuration.epiano.lowest_note, ENCODER[ENC_R].speed(), EP_LOWEST_NOTE_MIN, EP_LOWEST_NOTE_MAX);
        else if (generic_temp_select_menu == 23)
          configuration.epiano.highest_note = calc_val[state_dir](configuration.epiano.highest_note, ENCODER[ENC_R].speed(), EP_HIGHEST_NOTE_MIN, EP_HIGHEST_NOTE_MAX);
        else if (generic_temp_select_menu == 24)
          configuration.epiano.midi_channel = calc_val[state_dir](configuration.epiano.midi_channel, 1, EP_MIDI_CHANNEL_MIN, EP_MIDI_CHANNEL_MAX);
      }
    }
    if (LCDML.BT_checkEnter() && encoderDir[ENC_R].ButtonShort()) // handle button presses during menu
    {
      seq.edit_state = !seq.edit_state;
      menuhelper_redraw = true;
    }
    // button check end <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<

    if (menuhelper_redraw)
    {
      update_selective_values_epiano();
      menuhelper_redraw = false;
    }
  }
  if (LCDML.FUNC_close()) // ****** STABLE END *********
  {
    generic_menu = 0;
    // generic_active_function = 99;
    encoderDir[ENC_R].reset();
    display.fillScreen(COLOR_BACKGROUND);
  }
}

void print_static_texts_microsynth()
{
  display.setTextSize(1);
  generic_active_function = 0;
  setCursor_textGrid_small(1, 1);
  display.setTextColor(RED);
  display.print(F("MICROSYNTH"));
  update_microsynth_instance_icons();

  setCursor_textGrid_small(1, 3);
  display.print(F("VOLUME"));
  setCursor_textGrid_small(1, 4);
  display.print(F("WAVE"));
  setCursor_textGrid_small(1, 5);
  display.print(F("COARSE"));
  setCursor_textGrid_small(1, 6);
  display.print(F("DETUNE"));

  setCursor_textGrid_small(22, 1);
  display.print(F("NOISE VOL"));
  setCursor_textGrid_small(22, 2);
  display.print(F("NOISE DCY"));
  setCursor_textGrid_small(22, 3);
  display.print(F("TRIG.WITH"));

  setCursor_textGrid_small(22, 4);
  display.print(F("FILTER"));
  setCursor_textGrid_small(22, 5);
  display.print(F("FREQ"));
  setCursor_textGrid_small(22, 6);
  display.print(F("RES"));
  setCursor_textGrid_small(32, 6);
  display.print(F("SPEED"));

  setCursor_textGrid_small(1, 8);
  display.print(F("ATTACK"));
  setCursor_textGrid_small(1, 9);
  display.print(F("DECAY"));
  setCursor_textGrid_small(1, 10);
  display.print(F("SUSTAIN"));
  setCursor_textGrid_small(1, 11);
  display.print(F("RELEASE"));

  setCursor_textGrid_small(22, 8);
  display.print(F("OSC LFO"));
  setCursor_textGrid_small(22, 9);
  display.print(F("MODE"));
  setCursor_textGrid_small(22, 10);
  display.print(F("DELAY"));
  setCursor_textGrid_small(32, 10);
  display.print(F("SPEED"));

  update_pwm_text();

  setCursor_textGrid_small(13, 5);
  display.print(F("STEPS"));
  setCursor_textGrid_small(13, 6);
  display.print(F("CENTS"));
  setCursor_textGrid_small(16, 8);
  display.print(F("MS"));
  setCursor_textGrid_small(16, 9);
  display.print(F("MS"));
  setCursor_textGrid_small(16, 10);
  display.print(F("LEVL"));
  setCursor_textGrid_small(16, 11);
  display.print(F("MS"));

  // arrows
  display.setTextColor(GREY1, COLOR_BACKGROUND);
  setCursor_textGrid_small(36, 5);
  display.print(F(">"));
}

void update_selective_values_microsynth()
{
  display.setTextSize(1);
  if (seq.edit_state == 1)
    microsynth_update_single_setting(microsynth_selected_instance);
  if (generic_temp_select_menu == 0)
    display.setTextColor(COLOR_BACKGROUND, COLOR_SYSTEXT);
  else
    display.setTextColor(COLOR_SYSTEXT, COLOR_BACKGROUND);
  display.setTextSize(1);
  if (menu_item_check(0))
    print_small_intbar(9, 3, microsynth[microsynth_selected_instance].sound_intensity, 0, 1, 0);
  if (menu_item_check(1))
  {
    setModeColor(1);
    setCursor_textGrid_small(9, 4);
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
  }
  if (menu_item_check(2))
  {
    setModeColor(2);
    setCursor_textGrid_small(9, 5);
    print_formatted_number_signed(microsynth[microsynth_selected_instance].coarse, 2);
  }
  if (menu_item_check(3))
  {
    setModeColor(3);
    setCursor_textGrid_small(9, 6);
    print_formatted_number_signed(microsynth[microsynth_selected_instance].detune, 2);
  }
  if (menu_item_check(4))
  {
    setModeColor(4);
    setCursor_textGrid_small(10, 8);
    print_formatted_number(microsynth[microsynth_selected_instance].env_attack * 4, 4);
  }
  if (menu_item_check(5))
  {
    setModeColor(5);
    setCursor_textGrid_small(10, 9);
    print_formatted_number(microsynth[microsynth_selected_instance].env_decay * 4, 4);
  }
  if (menu_item_check(6))
  {
    setModeColor(6);
    setCursor_textGrid_small(11, 10);
    print_formatted_number(microsynth[microsynth_selected_instance].env_sustain * 2, 3);
  }
  if (menu_item_check(7))
  {
    setModeColor(7);
    setCursor_textGrid_small(10, 11);
    print_formatted_number(microsynth[microsynth_selected_instance].env_release * microsynth[microsynth_selected_instance].env_release, 4);
  }
  if (seq.cycle_touch_element != 1)
    microsynth_refresh_lower_screen_dynamic_text();
  if (menu_item_check(16))
    print_small_intbar(32, 1, microsynth[microsynth_selected_instance].noise_vol, 16, 1, 0);
  if (menu_item_check(17))
    print_small_intbar(32, 2, microsynth[microsynth_selected_instance].noise_decay, 17, 1, 1);
  if (menu_item_check(18))
  {
    setModeColor(18);
    setCursor_textGrid_small(32, 3);
    if (microsynth[microsynth_selected_instance].trigger_noise_with_osc)
      display.print(F("OSC."));
    else
      display.print(F("C-8 "));
  }
  if (menu_item_check(19))
  {
    setModeColor(19);
    setCursor_textGrid_small(32, 4);
    if (microsynth[microsynth_selected_instance].filter_noise_mode == 0)
      display.print(F("OFF   "));
    else if (microsynth[microsynth_selected_instance].filter_noise_mode == 1)
      display.print(F("LP12dB"));
    else if (microsynth[microsynth_selected_instance].filter_noise_mode == 2)
      display.print(F("BP12dB"));
    else if (microsynth[microsynth_selected_instance].filter_noise_mode == 3)
      display.print(F("HI12dB"));
  }
  if (menu_item_check(20))
    print_small_intbar(32, 5, microsynth[microsynth_selected_instance].filter_noise_freq_from / 100, 20, 0, 1);
  if (menu_item_check(21))
    print_small_intbar(38, 5, microsynth[microsynth_selected_instance].filter_noise_freq_to / 100, 21, 0, 1);
  if (menu_item_check(22))
    print_small_intbar(27, 6, microsynth[microsynth_selected_instance].filter_noise_resonance, 22, 0, 1);
  if (menu_item_check(23))
    print_small_intbar(38, 6, microsynth[microsynth_selected_instance].filter_noise_speed / 10, 23, 0, 1);
  if (menu_item_check(24))
    print_small_intbar(32, 8, microsynth[microsynth_selected_instance].lfo_intensity, 24, 0, 1);
  if (menu_item_check(25))
  {
    setModeColor(25);
    setCursor_textGrid_small(29, 9);
    print_formatted_number(microsynth[microsynth_selected_instance].lfo_mode, 2);

    if (microsynth[microsynth_selected_instance].lfo_mode == 0)
    {
      display.setTextColor(COLOR_SYSTEXT, RED);
    }
    else
      display.setTextColor(GREY1, GREY3);

    setCursor_textGrid_small(32, 9);
    display.print(F("U&D"));
    if (microsynth[microsynth_selected_instance].lfo_mode == 1)
    {
      display.setTextColor(COLOR_SYSTEXT, RED);
    }
    else
      display.setTextColor(GREY1, GREY3);
    setCursor_textGrid_small(36, 9);
    display.print(F("1U"));

    if (microsynth[microsynth_selected_instance].lfo_mode == 2)
    {
      display.setTextColor(COLOR_SYSTEXT, RED);
    }
    else
      display.setTextColor(GREY1, GREY3);
    setCursor_textGrid_small(39, 9);
    display.print(F("1D"));
  }
  if (menu_item_check(26))
    print_small_intbar(28, 10, microsynth[microsynth_selected_instance].lfo_delay, 26, 0, 1);
  if (menu_item_check(27))
    print_small_intbar(38, 10, microsynth[microsynth_selected_instance].lfo_speed, 27, 0, 1);
}

void UI_func_microsynth(uint8_t param)
{
  if (LCDML.FUNC_setup()) // ****** SETUP *********
  {
    // setup function
    encoderDir[ENC_R].reset();
    display.fillScreen(COLOR_BACKGROUND);
    virtual_keyboard_smart_preselect_mode();
    if (seq.cycle_touch_element != 1)
    {
      draw_button_on_grid(45, 1, "", "", 99); // print keyboard icon
      microsynth_refresh_lower_screen_static_text();
    }
    else
    {
      draw_button_on_grid(45, 1, "MORE", "PARAM.", 0);
      virtual_keyboard();
      virtual_keyboard_print_buttons();
      virtual_keyboard_print_current_instrument();
    }
    print_static_texts_microsynth();
    if (LCDML.MENU_getLastActiveFunctionID() != LCDML.OTHER_getIDFromFunction(UI_func_volume) && LCDML.MENU_getLastActiveFunctionID() != LCDML.OTHER_getIDFromFunction(mFunc_screensaver))
      generic_temp_select_menu = 0;

    generic_full_draw_required = true;
    update_selective_values_microsynth();
    generic_full_draw_required = false;
  }
  if (LCDML.FUNC_loop()) // ****** LOOP *********
  {

    if ((LCDML.BT_checkDown() && encoderDir[ENC_R].Down()) || (LCDML.BT_checkUp() && encoderDir[ENC_R].Up()))
    {
      set_state_dir();
      if (seq.edit_state == 0)
      {
        generic_temp_select_menu = calc_val[state_dir](generic_temp_select_menu, 1, 0, 35);
        if (generic_temp_select_menu != menuhelper_previous_val)
          menuhelper_redraw = true;
      }
      else if (seq.edit_state == 1)
      {
        microsynth_t* ms = &(microsynth[microsynth_selected_instance]);

        if (generic_temp_select_menu == 0)
          ms->sound_intensity = calc_val[state_dir](ms->sound_intensity, ENCODER[ENC_R].speed(), SOUND_INTENSITY_MIN, SOUND_INTENSITY_MAX);
        else if (generic_temp_select_menu == 1)
        {
          ms->wave = calc_val[state_dir](ms->wave, 1, 0, 8);
          update_pwm_text();
        }
        else if (generic_temp_select_menu == 2)
          ms->coarse = calc_val[state_dir](ms->coarse, 1, -36, 36);
        else if (generic_temp_select_menu == 3)
          ms->detune = calc_val[state_dir](ms->detune, ENCODER[ENC_R].speed(), -99, 99);
        else if (generic_temp_select_menu == 4)
          ms->env_attack = calc_val[state_dir](ms->env_attack, ENCODER[ENC_R].speed(), 0, 254);
        else if (generic_temp_select_menu == 5)
          ms->env_decay = calc_val[state_dir](ms->env_decay, ENCODER[ENC_R].speed(), 0, 254);
        else if (generic_temp_select_menu == 6)
          ms->env_sustain = calc_val[state_dir](ms->env_sustain, ENCODER[ENC_R].speed(), 0, 50);
        else if (generic_temp_select_menu == 7)
          ms->env_release = calc_val[state_dir](ms->env_release, ENCODER[ENC_R].speed(), 0, 99);
        else if (generic_temp_select_menu == 8)
          ms->filter_osc_mode = calc_val[state_dir](ms->filter_osc_mode, 1, 0, 3);
        else if (generic_temp_select_menu == 9)
          ms->filter_osc_freq_from = calc_val[state_dir](ms->filter_osc_freq_from, ENCODER[ENC_R].speed() * 40, 0, 15000);
        else if (generic_temp_select_menu == 10)
          ms->filter_osc_freq_to = calc_val[state_dir](ms->filter_osc_freq_to, ENCODER[ENC_R].speed() * 40, 0, 15000);
        else if (generic_temp_select_menu == 11)
          ms->filter_osc_resonance = calc_val[state_dir](ms->filter_osc_resonance, ENCODER[ENC_R].speed(), 0, 99);
        else if (generic_temp_select_menu == 12)
          ms->filter_osc_speed = calc_val[state_dir](ms->filter_osc_speed, 5 + ENCODER[ENC_R].speed(), 0, 999);
        else if (generic_temp_select_menu == 13)
          ms->pwm_from = calc_val[state_dir](ms->pwm_from, ENCODER[ENC_R].speed() * 4, 0, 999);
        else if (generic_temp_select_menu == 14)
          ms->pwm_to = calc_val[state_dir](ms->pwm_to, ENCODER[ENC_R].speed() * 4, 0, 999);
        else if (generic_temp_select_menu == 15)
          ms->pwm_speed = calc_val[state_dir](ms->pwm_speed, ENCODER[ENC_R].speed(), 0, 99);
        else if (generic_temp_select_menu == 16)
          ms->noise_vol = calc_val[state_dir](ms->noise_vol, ENCODER[ENC_R].speed(), 0, 100);
        else if (generic_temp_select_menu == 17)
          ms->noise_decay = calc_val[state_dir](ms->noise_decay, ENCODER[ENC_R].speed(), 0, 100);
        else if (generic_temp_select_menu == 18)
        {

          ms->trigger_noise_with_osc = !ms->trigger_noise_with_osc;
          menuhelper_redraw = true;
        }
        else if (generic_temp_select_menu == 19)
          ms->filter_noise_mode = calc_val[state_dir](ms->filter_noise_mode, 1, 0, 3);
        else if (generic_temp_select_menu == 20)
          ms->filter_noise_freq_from = calc_val[state_dir](ms->filter_noise_freq_from, ENCODER[ENC_R].speed() * 40, 0, 15000);
        else if (generic_temp_select_menu == 21)
          ms->filter_noise_freq_to = calc_val[state_dir](ms->filter_noise_freq_to, ENCODER[ENC_R].speed() * 40, 0, 15000);
        else if (generic_temp_select_menu == 22)
          ms->filter_noise_resonance = calc_val[state_dir](ms->filter_noise_resonance, ENCODER[ENC_R].speed(), 0, 99);
        else if (generic_temp_select_menu == 23)
          ms->filter_noise_speed = calc_val[state_dir](ms->filter_noise_speed, ENCODER[ENC_R].speed() * 2, 0, 999);
        else if (generic_temp_select_menu == 24)
          ms->lfo_intensity = calc_val[state_dir](ms->lfo_intensity, ENCODER[ENC_R].speed(), 0, 254);
        else if (generic_temp_select_menu == 25)
          ms->lfo_mode = calc_val[state_dir](ms->lfo_mode, 1, 0, 2);
        else if (generic_temp_select_menu == 26)
          ms->lfo_delay = calc_val[state_dir](ms->lfo_delay, ENCODER[ENC_R].speed(), 0, 254);
        else if (generic_temp_select_menu == 27)
          ms->lfo_speed = calc_val[state_dir](ms->lfo_speed, ENCODER[ENC_R].speed(), 0, 254);
        else if (generic_temp_select_menu == 28)
          ms->rev_send = calc_val[state_dir](ms->rev_send, ENCODER[ENC_R].speed(), 0, 100);
        else if (generic_temp_select_menu == 29)
          ms->chorus_send = calc_val[state_dir](ms->chorus_send, ENCODER[ENC_R].speed(), 0, 100);
        else if (generic_temp_select_menu == 30)
          ms->delay_send[0] = calc_val[state_dir](ms->delay_send[0], ENCODER[ENC_R].speed(), 0, 100);
        else if (generic_temp_select_menu == 31)
          ms->delay_send[1] = calc_val[state_dir](ms->delay_send[1], ENCODER[ENC_R].speed(), 0, 100);
        else if (generic_temp_select_menu == 32)
          ms->pan = calc_val[state_dir](ms->pan, 1, PANORAMA_MIN, PANORAMA_MAX);
        else if (generic_temp_select_menu == 33)
          ms->midi_channel = calc_val[state_dir](ms->midi_channel, 1, 1, 16);
        else if (generic_temp_select_menu == 34)
          ms->vel_mod_filter_osc = calc_val[state_dir](ms->vel_mod_filter_osc, ENCODER[ENC_R].speed(), 0, 254);
        else if (generic_temp_select_menu == 35)
          ms->vel_mod_filter_noise = calc_val[state_dir](ms->vel_mod_filter_noise, ENCODER[ENC_R].speed(), 0, 254);
      }
    }

    if (LCDML.BT_checkEnter() && encoderDir[ENC_R].ButtonShort()) // handle button presses during menu >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
    {
      seq.edit_state = !seq.edit_state;
      menuhelper_redraw = true;
      generic_full_draw_required = true;
    }
    else if (LCDML.BT_checkEnter())
    {
      if (microsynth_selected_instance == 0)
        microsynth_selected_instance = 1;
      else
        microsynth_selected_instance = 0;
      update_microsynth_instance_icons();
      virtual_keyboard_smart_preselect_mode();
      menuhelper_redraw = true;
      generic_full_draw_required = true;
    }

    // button check end <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
    if (menuhelper_redraw)
    {
      update_selective_values_microsynth();
      generic_full_draw_required = false;
      menuhelper_redraw = false;
    }
  }
  if (LCDML.FUNC_close()) // ****** STABLE END *********
  {
    generic_active_function = 99;
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
    if (track_number == seq.selected_track && y == seq.scrollpos && seq.current_chain[track_number] != 99) // print velocity of active pattern-step
    {
      if (seq.edit_state)
        display.setTextColor(COLOR_SYSTEXT, RED);
      else
        display.setTextColor(COLOR_BACKGROUND, COLOR_SYSTEXT);
      display.setCursor(22 * CHAR_width_small, DISPLAY_HEIGHT - CHAR_height_small);
      print_formatted_number(seq.vel[seq.current_pattern[track_number]][y], 3);
      display.setCursor(34 * CHAR_width_small, DISPLAY_HEIGHT - CHAR_height_small);
      if (seq.vel[seq.current_pattern[track_number]][y] < 128)
        display.print("VELOCITY");
      else if (seq.vel[seq.current_pattern[track_number]][y] > 209)
        display.print("PITCHSMP");
      else if (seq.vel[seq.current_pattern[track_number]][y] > 199)
        display.print("CHORD   ");
      else
        display.print("OTHER   ");
    }

    /// -- update all other columns --

    display.setCursor(xpos, ypos + ycount * yspacer);
    if (seq.note_data[seq.current_pattern[track_number]][y] != 99 && seq.note_data[seq.current_pattern[track_number]][y] != 0 && seq.current_chain[track_number] != 99)
    {
      if (seq.edit_state && track_number == seq.selected_track && y == seq.scrollpos)
        display.setTextColor(COLOR_SYSTEXT, RED);
      else if (track_number == seq.selected_track && y == seq.scrollpos)
        display.setTextColor(COLOR_BACKGROUND, COLOR_SYSTEXT);
      else
        set_pattern_content_type_color(seq.current_pattern[track_number]);
      display.print(tracker_find_shortname_from_pattern_step(track_number, seq.current_pattern[track_number], y)[0]);
    }
    else
    {
      if (seq.edit_state && track_number == seq.selected_track && y == seq.scrollpos && seq.current_chain[track_number] != 99)
        display.setTextColor(COLOR_SYSTEXT, RED);
      else if (track_number == seq.selected_track && y == seq.scrollpos && seq.current_chain[track_number] != 99)
        display.setTextColor(COLOR_BACKGROUND, COLOR_SYSTEXT);
      else
        display.setTextColor(GREY3, COLOR_BACKGROUND);
      display.print("-");
    }
    display.setCursor(xpos + 2 * CHAR_width_small, ypos + ycount * yspacer);
    if (seq.note_data[seq.current_pattern[track_number]][y] != 99 && seq.note_data[seq.current_pattern[track_number]][y] != 0 && seq.note_data[seq.current_pattern[track_number]][y] != 130 && seq.current_chain[track_number] != 99)
    {
      if (seq.edit_state && track_number == seq.selected_track && y == seq.scrollpos)
        display.setTextColor(COLOR_SYSTEXT, RED);
      else if (track_number == seq.selected_track && y == seq.scrollpos)
        display.setTextColor(COLOR_BACKGROUND, COLOR_SYSTEXT);
      else
        set_pattern_content_type_color(seq.current_pattern[track_number]);
      print_formatted_number(seq.note_data[seq.current_pattern[track_number]][y], 3);
    }
    else if (seq.note_data[seq.current_pattern[track_number]][y] == 0 && seq.current_chain[track_number] != 99) // empty
    {
      if (seq.edit_state && track_number == seq.selected_track && y == seq.scrollpos)
        display.setTextColor(COLOR_SYSTEXT, RED);
      else if (track_number == seq.selected_track && y == seq.scrollpos)
        display.setTextColor(COLOR_BACKGROUND, COLOR_SYSTEXT);
      else
        display.setTextColor(GREY2, COLOR_BACKGROUND);
      print_formatted_number(seq.note_data[seq.current_pattern[track_number]][y], 3);
    }
    else if (seq.note_data[seq.current_pattern[track_number]][y] == 130 && seq.current_chain[track_number] != 99) // Latch
    {
      if (seq.edit_state && track_number == seq.selected_track && y == seq.scrollpos)
        display.setTextColor(COLOR_SYSTEXT, RED);
      else if (track_number == seq.selected_track && y == seq.scrollpos)
        display.setTextColor(COLOR_BACKGROUND, DX_DARKCYAN);
      else
        display.setTextColor(DX_DARKCYAN, COLOR_BACKGROUND);
      display.print("LAT");
    }
    else
    {
      if (seq.edit_state && track_number == seq.selected_track && y == seq.scrollpos && seq.current_chain[track_number] != 99)
        display.setTextColor(COLOR_SYSTEXT, RED);
      else if (track_number == seq.selected_track && y == seq.scrollpos && seq.current_chain[track_number] != 99)
        display.setTextColor(COLOR_BACKGROUND, COLOR_SYSTEXT);
      else
        display.setTextColor(GREY2, COLOR_BACKGROUND);
      display.print("---");
    }
    ycount++;
  }
}

FLASHMEM void UI_func_seq_tracker(uint8_t param)
{
  if (LCDML.FUNC_setup()) // ****** SETUP *********
  {
    // setup function
    seq.menu = 0;
    seq.edit_state = false;
    display.setTextSize(1);
    display.fillScreen(COLOR_BACKGROUND);
    UI_toplineInfoText(1);
    display.setCursor(1, 2);
    display.setTextColor(COLOR_SYSTEXT, DX_DARKCYAN);
    display.print(F("TRACKER"));
    display.setCursor(CHAR_width_small * 10, 2);
    display.print(F("CHAIN"));
    display.setCursor(CHAR_width_small * 41, 2);
    display.print(F("SONG"));
    display.setCursor(CHAR_width_small * 49, 2);
    display.print(F("/"));
    display.setCursor(CHAR_width_small * 46, 2); // print song step at init
    display.setTextColor(COLOR_BACKGROUND, DX_DARKCYAN);
    print_formatted_number(seq.current_song_step, 2);
    display.setCursor(CHAR_width_small * 51, 2);
    print_formatted_number(get_song_length(), 2);
    display.setCursor(CHAR_width_small * 22, 2);

    for (uint8_t d = 0; d < NUM_SEQ_TRACKS; d++) // print chain steps
    {
      if (seq.current_chain[d] != 99)
      {
        display.setCursor(CHAR_width_small * 16 + (CHAR_width_small * 3) * d, 2);
        print_formatted_number(seq.chain_counter[d], 2);
      }
      //  display.setCursor(CHAR_width_small * 16+ (CHAR_width_small*3)*d , 2);
      //  print_formatted_number( get_chain_length_from_current_track(d)  , 2);
    }

    for (uint8_t x = 0; x < NUM_SEQ_TRACKS; x++)
    {
      seq.current_chain[x] = seq.song[x][seq.current_song_step];
      seq.current_pattern[x] = seq.chain[seq.current_chain[x]][seq.chain_counter[x]];

      setCursor_textGrid_small(6 + 6 * x, 2);
      display.setTextColor(COLOR_SYSTEXT, COLOR_BACKGROUND);
      display.print(F("TRK:"));
      display.print(x + 1);
      setCursor_textGrid_small(6 + 6 * x, 3);
      display.print("PAT");
      if (seq.current_chain[x] != 99)
        update_pattern_number_in_tracker(x);
    }
    display.setTextColor(DARKGREEN, COLOR_BACKGROUND);
    for (uint8_t y = 0; y < 16; y++)
    {
      display.setCursor(0, 45 + y * (CHAR_height_small + 3));
      print_formatted_number(y, 2);
    }
    display.setCursor(CHAR_width_small * 11, DISPLAY_HEIGHT - CHAR_height_small);
    display.print("DATA BYTE: ");
    display.setCursor(CHAR_width_small * 26, DISPLAY_HEIGHT - CHAR_height_small);
    display.print("USED AS");

    display.setTextColor(GREEN, COLOR_BACKGROUND);
    display.setCursor(5 * CHAR_width_small, (5 + seq.step) * (CHAR_height_small + 3) - 7);
    display.print(F(">"));
    encoderDir[ENC_R].reset();
  }
  if (LCDML.FUNC_loop()) // ****** LOOP *********
  {

    if ((LCDML.BT_checkDown() && encoderDir[ENC_R].Down()) || (LCDML.BT_checkUp() && encoderDir[ENC_R].Up()) || (LCDML.BT_checkEnter() && encoderDir[ENC_R].ButtonShort()))
    {
      if (seq.edit_state == false) // NOT in editor mode
      {
        if (LCDML.BT_checkDown())
        {
          seq.scrollpos++;
          if (seq.scrollpos > 15)
            seq.scrollpos = 15;
        }
        else if (LCDML.BT_checkUp())
        {
          if (seq.scrollpos > 0)
          {
            seq.scrollpos--;
          }
        }
      }
      else // IS in editor mode
      {
        if (LCDML.BT_checkDown())
        {

          seq.note_data[seq.current_pattern[seq.selected_track]][seq.scrollpos] = constrain(seq.note_data[seq.current_pattern[seq.selected_track]][seq.scrollpos] + ENCODER[ENC_R].speed(), 0, 254);
        }
        else if (LCDML.BT_checkUp())
        {
          seq.note_data[seq.current_pattern[seq.selected_track]][seq.scrollpos] = constrain(seq.note_data[seq.current_pattern[seq.selected_track]][seq.scrollpos] - ENCODER[ENC_R].speed(), 0, 254);
        }
      }
      if (LCDML.BT_checkEnter()) // handle button presses during menu >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
      {
        seq.edit_state = !seq.edit_state;
      }
    }
    for (uint8_t x = 0; x < NUM_SEQ_TRACKS; x++)
    {
      seq.current_chain[x] = seq.song[x][seq.current_song_step];
      seq.current_pattern[x] = seq.chain[seq.current_chain[x]][seq.chain_counter[x]];
      // update_pattern_number_in_tracker(x);
    }

    display.setTextSize(1);
    setCursor_textGrid_small(1, 2);
    if (seq.edit_state)
    {
      display.setTextColor(RED, COLOR_BACKGROUND);
      display.print("EDIT");
      helptext_l("< > DATA");
      helptext_r("< > NOTE");
    }
    else
    {
      display.setTextColor(GREEN, COLOR_BACKGROUND);
      display.print("PLAY");
      helptext_l("< > MOVE X");
      helptext_r("< > MOVE Y");
    }
    for (uint8_t d = 0; d < NUM_SEQ_TRACKS; d++)
    {
      tracker_print_pattern((6 + 6 * d) * CHAR_width_small, 48, d);
    }
  }
  if (LCDML.FUNC_close()) // ****** STABLE END *********
  {
    encoderDir[ENC_R].reset();
    display.fillScreen(COLOR_BACKGROUND);
  }
}

FLASHMEM void sub_song_print_tracknumbers()
{
  char str[3];

  for (uint8_t x = 0; x < NUM_SEQ_TRACKS; x++)
  {
    display.setCursor(6 * CHAR_width_small + (4 * CHAR_width_small) * x, CHAR_height_small * 4);
    if (seq.tracktype_or_instrument_assign == 0)
      set_track_type_color(x);
    else
      display.setTextColor(GREY3, COLOR_BACKGROUND);

    sprintf(str, "T%d", x + 1);
    display.print(str);
  }
}

FLASHMEM void print_song_loop_text()
{
  // print loop
  if (seq.tracktype_or_instrument_assign < 8) // not in loop clear
  {
    display.setCursor(CHAR_width_small * 11, 10);
    display.setTextColor(COLOR_BACKGROUND, DX_DARKCYAN);
    if (seq.loop_start != 99)
      print_formatted_number(seq.loop_start + 1, 2);
    else
      display.print(F("--"));

    display.setCursor(CHAR_width_small * 13, 10);
    display.print(F(" - "));

    display.setCursor(CHAR_width_small * 16, 10);
    if (seq.loop_end != 99)
      print_formatted_number(seq.loop_end + 1, 2);
    else
      display.print(F("--"));
  }
  else if (seq.tracktype_or_instrument_assign == 8) //clear loop mode
  {
    display.setCursor(CHAR_width_small * 11, 10);
    display.setTextColor(COLOR_SYSTEXT, RED);
    display.print(F("CLEAR ?"));
    print_song_loop_arrows();
    print_chain_matrix_in_song_page();
  }
  else if (seq.tracktype_or_instrument_assign == 10) //copy loop mode
  {
    display.setCursor(CHAR_width_small * 11, 10);
    display.setTextColor(COLOR_SYSTEXT, RED);
    display.print(F("COPY  ?"));
    print_song_loop_arrows();
    print_chain_matrix_in_song_page();
  }
  else
  {
    display.setCursor(CHAR_width_small * 11, 10);
    display.setTextColor(COLOR_SYSTEXT, RED);
    display.print(F("???????")); //error
  }

}

FLASHMEM void print_song_length()
{
  // print song length
  display.setCursor(CHAR_width_small * 21, 10);
  display.setTextColor(COLOR_SYSTEXT, DX_DARKCYAN);
  print_formatted_number(get_song_length(), 2);
}

FLASHMEM void print_chain_header()
{
  if (seq.cycle_touch_element < 6)
  {
    display.setTextColor(GREY2, COLOR_BACKGROUND);
  }
  else
  {
    display.setTextColor(COLOR_SYSTEXT, COLOR_BACKGROUND);
  }
  display.setCursor(40 * CHAR_width_small, CHAR_height_small * 4);
  display.print(F("CHAIN:"));
  display.setCursor(50 * CHAR_width_small, CHAR_height_small * 4);
  display.print(F("L"));
  display.setTextColor(GREY2, COLOR_BACKGROUND);
  display.setCursor(40 * CHAR_width_small, CHAR_height_small * 6);
  display.print(F("ST"));
  display.setCursor(43 * CHAR_width_small, CHAR_height_small * 6);
  if (seq.cycle_touch_element == 6 || seq.cycle_touch_element == 7)
    display.setTextColor(COLOR_SYSTEXT, COLOR_BACKGROUND);
  display.print(F("PAT"));
  display.setTextColor(GREY2, COLOR_BACKGROUND);
  if (seq.cycle_touch_element == 8 || seq.cycle_touch_element == 9)
    display.setTextColor(COLOR_SYSTEXT, COLOR_BACKGROUND);
  display.setCursor(48 * CHAR_width_small, CHAR_height_small * 6);
  display.print(F("TRANS"));
  display.setTextColor(GREY2, COLOR_BACKGROUND);
  for (uint8_t y = 0; y < 16; y++) // chain
  {
    display.setCursor(CHAR_width_small * 40, CHAR_height_small * 8 + y * 10);
    print_formatted_number(y + 1, 2);
    display.setCursor(CHAR_width_small * 43, CHAR_height_small * 8 + y * 10);
    display.print("P");
    display.setCursor(CHAR_width_small * 48, CHAR_height_small * 8 + y * 10);
    display.print("T");
  }
}

FLASHMEM void adjust_scroll_pos_for_loop_visible_on_screen()
{
  if (seq.loop_start > 7) // loop start is below half of screen
  {
    if (seq.loop_start > seq.scrollpos)
      seq.scrollpos = seq.loop_start;
  }
}

FLASHMEM void UI_func_song(uint8_t param)
{
  if (LCDML.FUNC_setup()) // ****** SETUP *********
  {
    // setup function
    encoderDir[ENC_R].reset();
    song_page_full_draw_pattern_complete = false;
    seq.help_text_needs_refresh = true;
    seq.loop_edit_step = 0;
    temp_int = 0;
    seq.cursor_scroll = 0;
    seq.scrollpos = 0;
    seq.cycle_touch_element = 0;
    display.fillScreen(COLOR_BACKGROUND);
    display.setTextSize(1);
    if (seq.quicknav_pattern_to_song_jump == false)
    {
      seq.edit_state = false;
    }
    else
    {
      seq.quicknav_pattern_to_song_jump = false;
      print_chain_matrix_in_song_page();
      seq.cycle_touch_element = 8;
    }
    seq.menu = 0;
    UI_toplineInfoText(2);
    display.setCursor(1, 1);
    display.setTextColor(COLOR_SYSTEXT, DX_DARKCYAN);
    display.print(F("SONG"));
    display.setCursor(CHAR_width_small * 11, 1);
    display.print(F("LOOP"));
    display.setCursor(CHAR_width_small * 21, 1);
    display.print(F("SLEN"));
    display.setCursor(CHAR_width_small * 26, 1);
    display.print(F("LC"));
    print_chain_header();

    display.setTextColor(COLOR_SYSTEXT, DX_DARKCYAN);
    show_no_grid(10, 1, 10, seq.name);
    // print loop text
    print_song_loop_text();
    print_song_loop_arrows();
    // print song length
    print_song_length();
    // print currently playing chain steps
    print_playing_chains();
    sub_song_print_tracknumbers();
    sub_song_print_tracktypes();
    sub_song_print_instruments(GREY2, COLOR_BACKGROUND);

    display.setTextSize(1);
  }
  if (LCDML.FUNC_loop()) // ****** LOOP *********
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
            print_song_loop_arrows();
          }

          else if (seq.tracktype_or_instrument_assign == 10) // go back from loop copy to tracktype
          {
            seq.tracktype_or_instrument_assign = 8;
            print_song_loop_text();
            seq.help_text_needs_refresh = true;
          }

          else if (seq.tracktype_or_instrument_assign == 8 && seq.loop_edit_step != 3) // go back from loop clear to tracktype
          {
            seq.tracktype_or_instrument_assign = 5;
            print_song_loop_text();


            seq.help_text_needs_refresh = true;
          }
          else if (seq.tracktype_or_instrument_assign == 8 && seq.loop_edit_step == 3) // exit clear song loop
          {
            seq.tracktype_or_instrument_assign = 0;
            seq.loop_edit_step = 0;
            seq.selected_track = 0;
            print_song_loop_text();
            print_song_loop_arrows();
            seq.help_text_needs_refresh = true;
          }
          else if (seq.tracktype_or_instrument_assign == 1 && seq.loop_edit_step == 0) // disable edit instruments for tracks
          {
            seq.tracktype_or_instrument_assign = 0;
            sub_song_print_tracknumbers();
            sub_song_print_tracktypes();
            sub_song_print_instruments(GREY2, COLOR_BACKGROUND);
            seq.help_text_needs_refresh = true;
          }
          else if (seq.tracktype_or_instrument_assign == 2 && seq.loop_edit_step == 0) // select instruments for track
          {
            if (seq.instrument[seq.selected_track] < 63)
              seq.instrument[seq.selected_track]++;

            if (seq.instrument[seq.selected_track] > 5 && seq.instrument[seq.selected_track] < 16) // skip currently unused msp slots
            {
              if (seq.instrument[seq.selected_track] > 5 + NUM_MULTISAMPLES)
                seq.instrument[seq.selected_track] = 16;
            }
          }
          else if (seq.tracktype_or_instrument_assign == 6 && seq.loop_edit_step == 0) // tracktype change
          {
            if (seq.track_type[seq.selected_track] < 3)
              seq.track_type[seq.selected_track]++;
          }
          else if (seq.tracktype_or_instrument_assign == 5 && seq.loop_edit_step == 0) // disable edit tracktype select
          {
            seq.help_text_needs_refresh = true;
            seq.tracktype_or_instrument_assign = 1;
            sub_song_print_instruments(GREY2, COLOR_BACKGROUND);
            sub_song_print_tracktypes();
          }
          else if (seq.tracktype_or_instrument_assign == 0 || seq.tracktype_or_instrument_assign == 11)
          {
            seq.cursor_scroll_previous = seq.cursor_scroll;
            seq.previous_track = seq.selected_track;
            seq.cursor_scroll = constrain(seq.cursor_scroll + 1, 0, 15);
          }
        }
        else if (seq.edit_state == true && seq.cycle_touch_element == 5)
        {
          if (temp_int == NUM_CHAINS)
            temp_int = 0;
          else
            temp_int = constrain(temp_int + 1, 0, NUM_CHAINS); // not -1: last element is for empty step
        }
        else if ((seq.edit_state == true && seq.cycle_touch_element == 6) || (seq.edit_state == true && seq.cycle_touch_element == 8))
        {
          // if ( seq.loop_edit_step == 0)
          seq.menu = constrain(seq.menu + 1, 0, 15);
        }
        else if (seq.edit_state == true && seq.cycle_touch_element == 7)
        {
          if (seq.sub_menu == 99)
            seq.sub_menu = 0;
          else
            seq.sub_menu = constrain(seq.sub_menu + 1, 0, NUM_CHAINS);
          song_page_full_draw_chain_complete = false;

        }
        else if (seq.edit_state == true && seq.cycle_touch_element == 9)
        {
          if (seq.sub_menu > NUM_CHAINS)
            seq.sub_menu = seq.sub_menu - 1;
          else if (seq.sub_menu >= 0 && seq.sub_menu < NUM_CHAINS - 1)
            seq.sub_menu = seq.sub_menu + 1;
          if (seq.sub_menu == NUM_CHAINS)
            seq.sub_menu = 0;

        }
      }
      else if (LCDML.BT_checkUp())
      {
        if (seq.edit_state == false)
        {
          if ((seq.cursor_scroll == 0 && seq.scrollpos > 0 && seq.tracktype_or_instrument_assign == 0)
            || (seq.cursor_scroll == 0 && seq.scrollpos > 0 && seq.tracktype_or_instrument_assign == 11))
          {
            seq.scrollpos--;
            print_song_loop_arrows();
          }

          else if (seq.cursor_scroll == 0 && seq.scrollpos == 0 && seq.tracktype_or_instrument_assign == 0 && seq.loop_edit_step == 0) // edit instruments for tracks
          {
            seq.tracktype_or_instrument_assign = 1;
            seq.help_text_needs_refresh = true;
            sub_song_print_tracknumbers();
            sub_song_print_tracktypes();
            sub_song_print_instruments(GREY1, COLOR_BACKGROUND);
          }

          else if (seq.cursor_scroll == 0 && seq.scrollpos == 0 && seq.tracktype_or_instrument_assign == 0 && seq.loop_edit_step != 0) // go to clear loop 
          {
            if (seq.loop_start != 99 || seq.loop_end != 99) //do not go in if no loop is set
            {
              seq.tracktype_or_instrument_assign = 8;
              seq.loop_edit_step = 3;
              print_song_loop_arrows();
              sub_song_print_tracktypes();
              print_song_loop_text();
              seq.help_text_needs_refresh = true;
              clear_song_playhead();
            }
          }
          else if (seq.tracktype_or_instrument_assign == 2 && seq.loop_edit_step == 0) // select instruments for track
          {
            if (seq.instrument[seq.selected_track] > 0)
            {
              seq.instrument[seq.selected_track]--;
              if (seq.instrument[seq.selected_track] == 5)
                sub_song_print_tracktypes();
            }
            if (seq.instrument[seq.selected_track] > 5 && seq.instrument[seq.selected_track] < 16) // skip currently unused msp slots
            {
              if (seq.instrument[seq.selected_track] > 5 + NUM_MULTISAMPLES)
                seq.instrument[seq.selected_track] = 5 + NUM_MULTISAMPLES;
            }
          }
          else if (seq.tracktype_or_instrument_assign == 1 && seq.loop_edit_step == 0) // goto for tracktype change
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

          else if (seq.tracktype_or_instrument_assign == 5) // goto loop clear
          {
            if (seq.loop_start != 99 || seq.loop_end != 99) //do not go if no loop is set
            {
              seq.tracktype_or_instrument_assign = 8;
              seq.loop_edit_step = 3;
              sub_song_print_tracktypes();
              adjust_scroll_pos_for_loop_visible_on_screen();
              print_song_loop_text();
              seq.help_text_needs_refresh = true;
              clear_song_playhead();
            }
          }

          else if (seq.tracktype_or_instrument_assign == 8) // goto loop copy mode
          {
            if (seq.loop_start != 99 || seq.loop_end != 99) //do not go if no loop is set
            {

              seq.tracktype_or_instrument_assign = 10;

              //scroll display if selected loop is out of screen bounds

              adjust_scroll_pos_for_loop_visible_on_screen();

              print_song_loop_text();
              seq.help_text_needs_refresh = true;
              clear_song_playhead();
            }
          }

          if (seq.tracktype_or_instrument_assign == 0 || seq.tracktype_or_instrument_assign == 11)
          {
            seq.cursor_scroll_previous = seq.cursor_scroll;
            seq.previous_track = seq.selected_track;
            seq.cursor_scroll = constrain(seq.cursor_scroll - 1, 0, 15);
          }
        }

        else if (seq.edit_state == true && seq.cycle_touch_element == 5)
        {
          if (temp_int == 0)
            temp_int = NUM_CHAINS;
          else
            temp_int = constrain(temp_int - 1, 0, NUM_CHAINS); // not -1:last element is for empty step
        }
        else if ((seq.edit_state == true && seq.cycle_touch_element == 6) || (seq.edit_state == true && seq.cycle_touch_element == 8))
        {
          seq.menu = constrain(seq.menu - 1, 0, 15);

        }
        else if ((seq.edit_state == true && seq.cycle_touch_element == 7))
        {
          if (seq.sub_menu == 0)
            seq.sub_menu = 99;
          else if (seq.sub_menu == 99)
            ;
          else
            seq.sub_menu = constrain(seq.sub_menu - 1, 0, NUM_CHAINS);
          song_page_full_draw_chain_complete = false;
        }

        else if (seq.edit_state == true && seq.cycle_touch_element == 9)
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
      else if (LCDML.BT_checkEnter()) // handle button presses during menu >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
      {

        if (seq.tracktype_or_instrument_assign == 5) // tracktype change
        {
          seq.tracktype_or_instrument_assign = 6;
          seq.help_text_needs_refresh = true;
        }
        else if (seq.tracktype_or_instrument_assign == 6) // exit track assign menu
        {
          seq.tracktype_or_instrument_assign = 5;
          seq.help_text_needs_refresh = true;
        }
        else if (seq.tracktype_or_instrument_assign == 1) // go into instr. assign menu
        {
          if (seq.track_type[seq.selected_track] != 0) // not set to a drum track
          {
            seq.tracktype_or_instrument_assign = 2;
            seq.help_text_needs_refresh = true;
          }
          else
          {
            helptext_l(back_text);
            helptext_r("TRACKTYPE [DRUMS] NOT VALID FOR INSTR");
          }
        }
        else if (seq.tracktype_or_instrument_assign == 2) // exit instr. assign menu
        {
          seq.tracktype_or_instrument_assign = 1;
          sub_song_print_tracktypes();
          seq.help_text_needs_refresh = true;
        }

        else if (seq.tracktype_or_instrument_assign == 8) // clear song loop
        {
          seq.tracktype_or_instrument_assign = 0;
          seq.loop_start = 99;
          seq.loop_end = 99;
          seq.scrollpos = 0;
          seq.loop_edit_step = 0;
          seq.selected_track = 0;
          print_song_loop_arrows();
          print_song_loop_text();
          clear_song_playhead();
          seq.help_text_needs_refresh = true;
        }

        else if (seq.tracktype_or_instrument_assign == 10) // copy mode - not enabled
        {
          seq.tracktype_or_instrument_assign = 11;
          if (seq.scrollpos == 0)
            seq.cursor_scroll = seq.loop_end + 1;

          if (seq.loop_end > 15) {
            seq.cursor_scroll = seq.loop_end - seq.loop_start + 1;
            seq.scrollpos = seq.loop_start;
          }

          seq.help_text_needs_refresh = true;
        }
        else if (seq.tracktype_or_instrument_assign == 11) // confirmed copy mode 
        {
          ////copy loop to destination
          for (uint8_t x = 0; x < NUM_SEQ_TRACKS; x++)
          {
            for (uint8_t y = 0; y < seq.loop_end - seq.loop_start + 1; y++)
            {
              if (seq.cursor_scroll + seq.scrollpos + y < SONG_LENGTH - 1) // do not copy out of song bounds
                seq.song[x][seq.cursor_scroll + seq.scrollpos + y] = seq.song[x][seq.loop_start + y];
            }
          }

          seq.tracktype_or_instrument_assign = 0;
          seq.scrollpos = 0;
          seq.selected_track = 0;
          seq.loop_edit_step = 0;
          print_song_loop_arrows();
          print_song_loop_text();
          clear_song_playhead();
          seq.cursor_scroll = 0;
          seq.help_text_needs_refresh = true;

        }

        else  if (seq.tracktype_or_instrument_assign == 0)
        {
          seq.help_text_needs_refresh = true;
          if (seq.loop_edit_step == 1) // edit loop step 1, set start
          {
            seq.loop_edit_step = 2;
            seq.loop_start = seq.cursor_scroll + seq.scrollpos;
          }
          else if (seq.loop_edit_step == 2) // edit loop step, set end
          {
            seq.loop_edit_step = 0;
            seq.selected_track = 0;
            seq.loop_end = seq.cursor_scroll + seq.scrollpos;
            print_song_loop_arrows();
            clear_song_playhead();

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
            seq.cycle_touch_element = 7; // edit chain
            seq.help_text_needs_refresh = true;
            seq.sub_menu = seq.chain[seq.song[seq.selected_track][seq.cursor_scroll + seq.scrollpos]][seq.menu];
          }
          else if (seq.edit_state && seq.cycle_touch_element == 7)
          {
            seq.cycle_touch_element = 6; // go back from chain
          }
          else if (seq.edit_state && seq.cycle_touch_element == 8)
          {
            seq.cycle_touch_element = 9; // edit transpose
            if (seq.chain_transpose[seq.song[seq.selected_track][seq.cursor_scroll + seq.scrollpos]][seq.menu] != 99)
              seq.sub_menu = seq.chain_transpose[seq.song[seq.selected_track][seq.cursor_scroll + seq.scrollpos]][seq.menu];
            else
              seq.sub_menu = 0;
            song_page_full_draw_transpose_complete = false;
          }
          else if (seq.edit_state && seq.cycle_touch_element == 9)
          {
            if (seq.sub_menu == 0)
              seq.chain_transpose[seq.song[seq.selected_track][seq.cursor_scroll + seq.scrollpos]][seq.menu] = 99;
            seq.cycle_touch_element = 8; // go back from transpose 
          }
          else if (seq.edit_state == false)
          {
            seq.cycle_touch_element = 0;
          }

          if (seq.cycle_touch_element < 7)
          {
            print_song_loop_text();
            print_song_length();
          }
          seq.help_text_needs_refresh = true;
        }
      } // Button END
    }

    if (seq.help_text_needs_refresh)
    {
      print_chain_header();
      print_song_mode_help();
      seq.help_text_needs_refresh = false;
    }

    if (seq.song[seq.selected_track][seq.cursor_scroll + seq.scrollpos] != song_previous_displayed_chain)
    {
      //current displayed chain has changed
#ifdef DEBUG
      // LOG.println(F(" "));
      // LOG.println(F("chain changed from "));
      // LOG.print(song_previous_displayed_chain);
      // LOG.print(F(" to "));
      // LOG.print(seq.song[seq.selected_track][seq.cursor_scroll + seq.scrollpos]);
#endif

      song_page_full_draw_chain_complete = false;
      song_page_full_draw_transpose_complete = false;
      song_previous_displayed_chain = seq.song[seq.selected_track][seq.cursor_scroll + seq.scrollpos];
      chain_endline = 99;
    }

    if (seq.loop_edit_step == 1 || seq.loop_edit_step == 2)
      edit_song_loop();

    print_song_playhead();

    print_chain_matrix_in_song_page();
    print_chain_steps_in_song_page();
    print_transpose_in_song_page();
    empty_chain_warning_text_in_song_page();

    //DEBUG
        // display.setTextColor(COLOR_SYSTEXT, DARKGREEN);
        // display.setTextSize(1);
        // display.setCursor(0, 0);
        // display.print(seq.cursor_scroll);
        // display.print("  ");
        // display.setCursor(1, CHAR_height);
        // display.print(seq.scrollpos);
        // display.print("  ");

  }

  if (LCDML.FUNC_close()) // ****** STABLE END *********
  {
    encoderDir[ENC_R].reset();
    seq.cycle_touch_element = 0;
    seq.tracktype_or_instrument_assign = 0;
    song_page_full_draw_pattern_complete = false;
    song_page_full_draw_chain_complete = false;
    song_page_full_draw_transpose_complete = false;

    chain_endline = 99;
    display.setTextColor(COLOR_SYSTEXT, COLOR_BACKGROUND);
    display.fillScreen(COLOR_BACKGROUND);
  }
}

// void UI_func_seq_pianoroll(uint8_t param)
// {

  //for old sequencer

//   if (LCDML.FUNC_setup()) // ****** SETUP *********
//   {
//     // setup function
//     display.fillScreen(COLOR_BACKGROUND);
//     encoderDir[ENC_R].reset();

//     display.setTextColor(COLOR_SYSTEXT, COLOR_CHORDS);
//     display.setTextSize(1);
//     display.setCursor(0, 0);
//     display.print("SONG");
//     display.setCursor(0, CHAR_height / 2);
//     display.print("STEP");
//     // display.fillRect(1, 72, DISPLAY_WIDTH, DISPLAY_HEIGHT - 72, COLOR_BACKGROUND);
//     print_merged_pattern_pianoroll(1 * CHAR_width, DISPLAY_HEIGHT - CHAR_height, seq.active_track);
//   }
//   if (LCDML.FUNC_loop()) // ****** LOOP *********
//   {
//     if (LCDML.BT_checkDown())
//     {
//       seq.active_track = constrain(seq.active_track + ENCODER[ENC_R].speed(), 0, NUM_SEQ_TRACKS - 1);
//     }
//     else if (LCDML.BT_checkUp())
//     {
//       seq.active_track = constrain(seq.active_track - ENCODER[ENC_R].speed(), 0, NUM_SEQ_TRACKS - 1);
//     }
//     print_merged_pattern_pianoroll(1 * CHAR_width, DISPLAY_HEIGHT - CHAR_height, seq.active_track);
//   }
//   if (LCDML.FUNC_close()) // ****** STABLE END *********
//   {
//     display.fillScreen(COLOR_BACKGROUND);
//     encoderDir[ENC_R].reset();
//   }
// }

void print_merged_pattern_pianoroll(int xpos, int ypos, uint8_t track_number)
{  //for old sequencer

  // uint8_t notes[64];
  // uint8_t lowest_note = 127;
  // int notes_display_shift = 0;
  // uint8_t last_valid_note = 254;
  // uint8_t patternspacer = 0;
  // uint8_t barspacer = 0;

  // int8_t current_chain = 99;
  // int8_t pattern[4] = { 99, 99, 99, 99 };

  // current_chain = seq.song[track_number][0]; // so far only step 0 of chain is displayed

  // for (uint8_t d = 0; d < 4; d++)
  // {
  //   pattern[d] = seq.chain[current_chain][d];
  // }

  // helptext_l("MOVE Y");
  // helptext_r("MOVE X");

  // display.setTextColor(COLOR_SYSTEXT, COLOR_CHORDS);
  // display.setCursor(CHAR_width * 2, 0);

  // display.print("[");
  // display.print(0);
  // display.print("]");

  // display.setTextColor(COLOR_SYSTEXT, COLOR_PITCHSMP);

  // display.print(F(" TRK:["));
  // display.print(track_number + 1);
  // display.print("] ");
  // display.setTextColor(COLOR_SYSTEXT, COLOR_BACKGROUND);
  // display.print(" ");
  // display.setTextColor(COLOR_SYSTEXT, COLOR_PITCHSMP);
  // display.print(F(" CHAIN: "));
  // display.print(current_chain);
  // display.print(F("  "));

  // print_formatted_number(pattern[0], 2);
  // display.write(25);
  // print_formatted_number(pattern[1], 2);
  // display.write(25);
  // print_formatted_number(pattern[2], 2);
  // display.write(25);
  // print_formatted_number(pattern[3], 2);
  // display.print(" ");

  //    if (pattern[0] < NUM_SEQ_PATTERN && pattern[1] < NUM_SEQ_PATTERN
  //        && pattern[2] < NUM_SEQ_PATTERN && pattern[3] < NUM_SEQ_PATTERN)
  //    {

  // for (uint8_t f = 0; f < 16; f++) // Fill array with complete data from all chain parts of track
  // {
  //   notes[f] = seq.note_data[pattern[0]][f];
  //   notes[f + 16] = seq.note_data[pattern[1]][f];
  //   notes[f + 32] = seq.note_data[pattern[2]][f];
  //   notes[f + 48] = seq.note_data[pattern[3]][f];
  // }
  // // find lowest note
  // for (uint8_t f = 0; f < 64; f++)
  // {
  //   if (notes[f] < lowest_note && notes[f] > 0)
  //   {
  //     lowest_note = notes[f];
  //   }
  // }
  // if (lowest_note > 120)
  //   lowest_note = 24;
  // notes_display_shift = lowest_note % 12;
  // print_keyboard(ypos, lowest_note / 12);
  // for (uint8_t xcount = 0; xcount < 64; xcount++)
  // {
  //   if (notes[xcount] > 0)
  //   {
  //     if (notes[xcount] == 130)
  //     {
  //       display.fillRect(40 + patternspacer + barspacer + xcount * 4, ypos - 10 - (8.15 * notes_display_shift) - (8.15 * (last_valid_note - lowest_note)), 3, 5, COLOR_PITCHSMP);
  //     }
  //     else
  //     {
  //       display.fillRect(40 + patternspacer + barspacer + xcount * 4, ypos - 10 - (8.15 * notes_display_shift) - (8.15 * (notes[xcount] - lowest_note)), 3, 5, GREY1);
  //       last_valid_note = notes[xcount];
  //     }
  //   }
  //   if ((xcount + 1) % 16 == 0)
  //     patternspacer = patternspacer + 2;
  //   if ((xcount + 1) % 4 == 0)
  //     barspacer = barspacer + 1;
  // }
}

uint8_t temp_uint;
int temp_int16;
uint16_t liveseq_edit_noteoff_step = 0;

FLASHMEM void print_keyboard_livesequencer(int ypos, uint8_t octave)
{
  uint8_t offset[5] = { 12, 12, 14, 12, 11 }; //+ is up
  int offcount = 0;
  uint8_t oct_count = 0;

  display.setTextColor(COLOR_BACKGROUND, COLOR_SYSTEXT);
  display.setTextSize(1);

  // draw white keys
  for (uint8_t y = 0; y < 14; y++)
  {
    display.console = true;
    if (ypos - CHAR_height - (y * 14) > CHAR_height_small && ypos - CHAR_height - (y * 14) + 13 < DISPLAY_HEIGHT - 4 * CHAR_height - 4)
      display.fillRect(0, ypos - CHAR_height - (y * 14), 30, 13, COLOR_SYSTEXT); // pianoroll white key
    if (y == 0 || y == 7 || y == 14)
    {
      if (ypos - 14 - (y * 14) > CHAR_height_small && ypos - 14 - (y * 14) < DISPLAY_HEIGHT - 5 * CHAR_height)
      {
        display.setCursor(17, ypos - 14 - (y * 14));
        display.print("C");
        display.print(octave - 1 + oct_count);
      }
      oct_count++;
    }
  }
  for (uint8_t y = 0; y < 23; y++)
  {
    if (seq.piano[y] == 1)
    {
      display.console = true;
      if (ypos - (y * 8.15) - offset[offcount] > CHAR_height_small && ypos - (y * 8.15) - offset[offcount] + 8 < DISPLAY_HEIGHT - 4 * CHAR_height - 4)
        display.fillRect(0, ypos - (y * 8.15) - offset[offcount], 12, 8, COLOR_BACKGROUND); // BLACK key
      offcount++;
      if (offcount == 5)
        offcount = 0;
    }
  }
}

#include "livesequencer.h"
extern LiveSequencer liveSeq;

bool get_current = false;

bool fullrefresh_values_liveseq_pianoroll = false;
int liveseq_pianoroll_y_scroll = 0;

void buttons_liveseq_pianoroll();

FLASHMEM  void liveseq_pianoroll_draw_graphics()
{
  std::vector<std::vector<LiveSequencer::NotePair>> notePairs = liveSeq.getNotePairsFromTrack(temp_int);

  uint8_t xoff = 33;
  float pat_len = (DISPLAY_WIDTH - xoff) / 4;
  float xscaler = 33;
  display.setTextColor(COLOR_SYSTEXT, COLOR_BACKGROUND);
  display.setCursor(CHAR_width_small * 35, 0);
  print_formatted_number(generic_temp_select_menu + 1, 3);//event number
  display.setCursor(CHAR_width_small * 52, 0);
  display.print(temp_int + 1); //track number

  if ((generic_menu == 0 && menuhelper_redraw) || generic_menu == 2 || generic_menu == 4 || generic_menu == 5 || generic_menu == 20 || generic_menu == 21)
  {
    menuhelper_redraw = false;
    display.console = true;
    display.fillRect(xoff, CHAR_height, DISPLAY_WIDTH - xoff, DISPLAY_HEIGHT - 5 * CHAR_height - 10, GREY4);

    for (uint8_t j = 0; j < 4; j++)
    {
      display.drawLine(xoff + j * pat_len, CHAR_height, xoff + j * pat_len, DISPLAY_HEIGHT - 5 * CHAR_height + 5, GREY2);

      for (uint8_t k = 1; k < 16; k++)
        display.drawLine(xoff + j * pat_len + pat_len / 16 * k, CHAR_height, xoff + j * pat_len + pat_len / 16 * k, DISPLAY_HEIGHT - 5 * CHAR_height + 5, GREY3);
    }
    display.drawLine(xoff + 4 * pat_len, CHAR_height, xoff + 4 * pat_len, DISPLAY_HEIGHT - 5 * CHAR_height + 5, GREY2);
  }

  LiveSequencer::LiveSeqData* data = liveSeq.getData();

  // find lowest note
  uint8_t lowest_note = 127;
  int notes_display_shift = 0;

  int i = 0;
  for (auto& e : data->eventsList)
  {
    if (e.track == temp_int && e.event == midi::NoteOn)
      if (e.note_in < lowest_note && e.note_in> 0)
      {
        lowest_note = e.note_in;
      }
    i++;
  }

  uint16_t eventcount = 0;

  if (lowest_note > 120)
    lowest_note = 24;
  notes_display_shift = (lowest_note % 12) + liveseq_pianoroll_y_scroll+7;

 buttons_liveseq_pianoroll();

  i = 0;

  if (generic_menu == 2)
  {
    //get current value when starting editing so it does not start at zero
    if (get_current)
    {
      temp_uint = notePairs[0][generic_temp_select_menu].noteOn.note_in;
      get_current = false;
    }
    // note_value[generic_temp_select_menu] = temp_uint;
    notePairs[0][generic_temp_select_menu].noteOn.note_in = temp_uint;
    notePairs[0][generic_temp_select_menu].noteOff.note_in = temp_uint;
  }
  else if (generic_menu == 3) // edit velocity
  {
    if (get_current)
    {
      temp_uint = notePairs[0][generic_temp_select_menu].noteOn.note_in_velocity;
      get_current = false;
    }
    //nothing to do visually
    notePairs[0][generic_temp_select_menu].noteOn.note_in_velocity = temp_uint;
  }
  else if (generic_menu == 4) // edit start time
  {
    if (get_current)
    {
      temp_int16 = notePairs[0][generic_temp_select_menu].noteOn.patternMs;
      get_current = false;
    }
    notePairs[0][generic_temp_select_menu].noteOn.patternMs = temp_int16;
  }
  else if (generic_menu == 5) // edit end time
  {
    if (get_current)
    {
      temp_int16 = notePairs[0][generic_temp_select_menu].noteOff.patternMs;
      get_current = false;
    }
    notePairs[0][generic_temp_select_menu].noteOff.patternMs = temp_int16;
  }

  for (auto& e : data->eventsList)
  {

    i++;
    if (e.track == temp_int)
      eventcount++;
  }

  eventcount = eventcount / 2;
  if (eventcount < 3)
    eventcount = 0;

  display.setCursor(CHAR_width_small * 41, 0);
  print_formatted_number(eventcount, 3);

  //  display.setCursor(CHAR_width_small * 18, 0);
  //   print_formatted_number(data->patternLengthMs, 4);

  uint8_t from = 0;
  uint8_t to = eventcount;

  // if in edit more, redraw just the current note, +-1 note

  //  if (generic_menu >1)
  // {
  // if (generic_temp_select_menu>0)
  // from=generic_temp_select_menu-1;
  // else
  // from =0;

  // to =generic_temp_select_menu+1;

  //}

  for (uint8_t j = from; j < to; j++)
  {
    uint16_t col;
    if ((generic_menu > 0 && generic_menu != 99 && j == generic_temp_select_menu))
    {
      col = RED;
    }
    else if ( j == generic_temp_select_menu)
    {
      col = GREEN;
    }
else
      col = COLOR_SYSTEXT;

    if (DISPLAY_HEIGHT - 28 - (8.15 * notes_display_shift) - (8.15 * (notePairs[0][j].noteOn.note_in - lowest_note)) >= CHAR_height &&
      DISPLAY_HEIGHT - 28 - (8.15 * notes_display_shift) - (8.15 * (notePairs[0][j].noteOn.note_in - lowest_note)) < DISPLAY_HEIGHT - 5 * CHAR_height)
    {
      if (notePairs[0][j].noteOff.note_in > 0 && notePairs[0][j].noteOn.note_in > 0)
      {
        display.console = true;
        display.fillRect(notePairs[0][j].noteOn.patternNumber * pat_len + xoff + notePairs[0][j].noteOn.patternMs / xscaler,
          (DISPLAY_HEIGHT - 28 - (8.15 * notes_display_shift) - (8.15 * (notePairs[0][j].noteOn.note_in - lowest_note))),
          (notePairs[0][j].noteOff.patternNumber - notePairs[0][j].noteOn.patternNumber) * pat_len + (notePairs[0][j].noteOff.patternMs / xscaler - notePairs[0][j].noteOn.patternMs / xscaler), 5, col);
      }
      else //no note-off for note-on found, draw just note start
      {
        if ((generic_menu == 1 && j == generic_temp_select_menu))
          col = RED;
        else
          col = GREEN;
        display.console = true;
        display.fillRect(notePairs[0][j].noteOn.patternNumber * pat_len + xoff + notePairs[0][j].noteOn.note_in / xscaler,
          (DISPLAY_HEIGHT - 28 - (8.15 * notes_display_shift) - (8.15 * (notePairs[0][j].noteOn.note_in - lowest_note))), 5, 5, col);
      }
    }
  }
}


FLASHMEM  void buttons_liveseq_pianoroll()
{
  std::vector<std::vector<LiveSequencer::NotePair>> notePairs = liveSeq.getNotePairsFromTrack(temp_int);

  //layer
  // display.setCursor((CHAR_width_small * 9), CHAR_height_small * 29);
  // display.print(e.layer + 1);

    //patternNumber
  // display.setTextColor(GREY2, COLOR_BACKGROUND);
  // display.setCursor((CHAR_width_small * 10), CHAR_height_small * 29);
  // display.print("P");
  // display.print(e.patternNumber + 1);

 //display.setTextSize(1);
  char buf[5] = { 0, 0, 0, 0,0 };
  //display.setTextColor(GREY2, COLOR_BACKGROUND);

  if (generic_menu == 0) // select track
  {
    draw_button_on_grid(0, 21, "TRACK", itoa(temp_int + 1, buf, 10), 2);
  }

  else
    if (fullrefresh_values_liveseq_pianoroll)
      draw_button_on_grid(0, 21, "SELECT", "TRACK", 1);

  if (generic_menu == 20) // select scroll up & down
  {
    draw_button_on_grid(8, 21, "SCROLL", itoa(liveseq_pianoroll_y_scroll, buf, 10), 2);
    display.console = true;
    display.fillRect(0, CHAR_height, 30, DISPLAY_HEIGHT - 5 * CHAR_height - 5, COLOR_BACKGROUND);
    print_keyboard_livesequencer(DISPLAY_HEIGHT - CHAR_height - (8.15 * (liveseq_pianoroll_y_scroll+7)) - 392, 8);
    print_keyboard_livesequencer(DISPLAY_HEIGHT - CHAR_height - (8.15 * (liveseq_pianoroll_y_scroll+7)) - 196, 6);
    print_keyboard_livesequencer(DISPLAY_HEIGHT - CHAR_height - (8.15 * (liveseq_pianoroll_y_scroll+7)), 4);
    print_keyboard_livesequencer(DISPLAY_HEIGHT - CHAR_height - (8.15 * (liveseq_pianoroll_y_scroll+7)) + 196, 2);
    print_keyboard_livesequencer(DISPLAY_HEIGHT - CHAR_height - (8.15 * (liveseq_pianoroll_y_scroll+7)) + 392, 0);

    display.console = true;
    display.fillRect(0, CHAR_height_small, 30, CHAR_height_small + 1, COLOR_BACKGROUND);
  }
  else if (fullrefresh_values_liveseq_pianoroll)
    draw_button_on_grid(8, 21, "SCROLL", "UP/DWN", 1);

  if (fullrefresh_values_liveseq_pianoroll) // 
    draw_button_on_grid(16, 21, "ZOOM", "X", 0);
  if (fullrefresh_values_liveseq_pianoroll) // 
    draw_button_on_grid(24, 21, "LAYER", "1-4", 0);
  if (fullrefresh_values_liveseq_pianoroll) // 
    draw_button_on_grid(32, 21, "PAT", "1-4", 0);
  if (fullrefresh_values_liveseq_pianoroll) // 
    draw_button_on_grid(40, 21, "", "", 0);
  if (fullrefresh_values_liveseq_pianoroll) // 
    draw_button_on_grid(48, 21, "ADD", "NOTE", 0);
  if (fullrefresh_values_liveseq_pianoroll) // 
    draw_button_on_grid(0, 26, "GO", "BACK", 1);

  if (generic_menu == 1) // select note
    draw_button_on_grid(8, 26, "SELECT", "NOTE", 2);
  else
    if (fullrefresh_values_liveseq_pianoroll)
      draw_button_on_grid(8, 26, "SELECT", "NOTE", 1);

  if (generic_menu == 2) // edit note
  {
    getNoteName(buf, notePairs[0][generic_temp_select_menu].noteOn.note_in);
    draw_button_on_grid(16, 26, "NOTE", buf, 2);
  }
  else
    if (fullrefresh_values_liveseq_pianoroll)
      draw_button_on_grid(16, 26, "EDIT", "NOTE", 1);

  if (generic_menu == 3) // edit vel
  {
    if (notePairs[0][generic_temp_select_menu].noteOn.note_in_velocity > 0)
    {
      draw_button_on_grid(24, 26, "VEL", itoa(notePairs[0][generic_temp_select_menu].noteOn.note_in_velocity, buf, 10), 2);
    }
    else
      draw_button_on_grid(24, 26, "VEL", "OFF", 2);
  }
  else if (fullrefresh_values_liveseq_pianoroll)
    draw_button_on_grid(24, 26, "EDIT", "VEL", 1);


  if (generic_menu == 4) // edit start
  {
    draw_button_on_grid(32, 26, "START", itoa(notePairs[0][generic_temp_select_menu].noteOn.patternMs, buf, 10), 2);
  }
  else
    if (fullrefresh_values_liveseq_pianoroll)
      draw_button_on_grid(32, 26, "EDIT", "START", 1);


  if (generic_menu == 5) // edit end
  {
    draw_button_on_grid(40, 26, "END", itoa(notePairs[0][generic_temp_select_menu].noteOff.patternMs, buf, 10), 2);
  }
  else
    if (fullrefresh_values_liveseq_pianoroll)
      draw_button_on_grid(40, 26, "EDIT", "END", 1);

if (generic_menu == 21) // delete note
  {
    draw_button_on_grid(48, 26, "DEL", "NOTE", 2);
notePairs[0][generic_temp_select_menu].noteOn.note_in=0;
notePairs[0][generic_temp_select_menu].noteOff.note_in=0;
notePairs[0][generic_temp_select_menu].noteOn.event = midi::InvalidType;
notePairs[0][generic_temp_select_menu].noteOff.event = midi::InvalidType;
delay(100);

  liveSeq.cleanEvents();
   generic_menu =1;
 
   //liveseq_pianoroll_draw_graphics();
  
   draw_button_on_grid(48, 26, "DEL", "NOTE", 1);
  }else
  {
   if (generic_menu == 1 || generic_menu == 2|| generic_menu == 3 || generic_menu == 4 || generic_menu == 5)
    draw_button_on_grid(48, 26, "DEL", "NOTE", 1);
    else
  if (fullrefresh_values_liveseq_pianoroll)  
    draw_button_on_grid(48, 26, "DEL", "NOTE", 0);
  }


  fullrefresh_values_liveseq_pianoroll = false;
}

FLASHMEM void UI_func_liveseq_pianoroll(uint8_t param)
{  // for Livesequencer

  if (LCDML.FUNC_setup()) // ****** SETUP *********
  {
    fullrefresh_values_liveseq_pianoroll = true;
    menuhelper_previous_val = 99;
    menuhelper_redraw = true;
    temp_int = 5;
    menuhelper_redraw = true;
    generic_temp_select_menu = 0;
    generic_menu = 0;
    // setup function
    display.fillScreen(COLOR_BACKGROUND);

    encoderDir[ENC_R].reset();

    display.setTextColor(COLOR_SYSTEXT, COLOR_CHORDS);
    display.setTextSize(1);

    print_keyboard_livesequencer(DISPLAY_HEIGHT - CHAR_height - (8.15 * (liveseq_pianoroll_y_scroll+7)), 4);
    display.setCursor(0, 0);
    display.setTextColor(RED, COLOR_BACKGROUND);
    display.print("LIVESEQ EDITOR");
    display.setTextColor(GREY1, COLOR_BACKGROUND);
    display.setCursor(CHAR_width_small * 29, 0);
    display.print(F("EVENT"));
    display.setCursor(CHAR_width_small * 39, 0);
    display.print(F("/"));
    display.setCursor(CHAR_width_small * 46, 0);
    display.print(F("TRACK"));
  }
  if (LCDML.FUNC_loop()) // ****** LOOP *********
  {
    if (generic_menu == 0) // select track
    {
      if (LCDML.BT_checkDown())
      {
        menuhelper_previous_val = temp_int;
        temp_int = constrain(temp_int + 1, 0, 5);
        if (menuhelper_previous_val != temp_int)
         { menuhelper_redraw = true;
          liveseq_pianoroll_y_scroll=0;
         }
      }
      else if (LCDML.BT_checkUp())
      {
        menuhelper_previous_val = temp_int;
        temp_int = constrain(temp_int - 1, 0, 5);
        if (menuhelper_previous_val != temp_int)
          { menuhelper_redraw = true;
          liveseq_pianoroll_y_scroll=0;
         }
      }
    }
    else if (generic_menu == 1) // select note
    {
      if (LCDML.BT_checkDown())
      {
        generic_temp_select_menu = constrain(generic_temp_select_menu + 1, 0, 50);
      }
      else if (LCDML.BT_checkUp())
      {
        generic_temp_select_menu = constrain(generic_temp_select_menu - 1, 0, 50);
      }
    }
    else if (generic_menu == 2) // edit note
    {
      if (LCDML.BT_checkDown())
      {
        temp_uint = constrain(temp_uint + 1, 1, 127);
      }
      else if (LCDML.BT_checkUp())
      {
        temp_uint = constrain(temp_uint - 1, 1, 127);
      }
    }
    else if (generic_menu == 3) // edit vel
    {
      if (LCDML.BT_checkDown())
      {
        temp_uint = constrain(temp_uint + 1, 0, 127);
      }
      else if (LCDML.BT_checkUp())
      {
        temp_uint = constrain(temp_uint - 1, 0, 127);
      }
    }
    else if (generic_menu == 4 || generic_menu == 5) // edit start/end time
    {
      if (LCDML.BT_checkDown())
      {
        temp_int16 = constrain(temp_int16 + ENCODER[ENC_R].speed() * 8, 0, 7000);
      }
      else if (LCDML.BT_checkUp())
      {
        temp_int16 = constrain(temp_int16 - ENCODER[ENC_R].speed() * 8, 0, 7000);
      }
    }

    else if (generic_menu == 20) // scroll up&down
    {
      if (LCDML.BT_checkDown())
      {
        liveseq_pianoroll_y_scroll = constrain(liveseq_pianoroll_y_scroll + 1, -99, 99);
      }
      else if (LCDML.BT_checkUp())
      {
        liveseq_pianoroll_y_scroll = constrain(liveseq_pianoroll_y_scroll - 1, -99, 99);
      }
    }

    if (LCDML.BT_checkEnter() && encoderDir[ENC_R].ButtonShort()) // handle button presses during menu >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
    {
      if (generic_menu == 0)
      {
        generic_menu = 1;
      }
      else  if (generic_menu == 1)
      {
        generic_menu = 2;
        get_current = true;
      }
      else  if (generic_menu == 2)
      {
        generic_menu = 3;
        get_current = true;
      }
      else  if (generic_menu == 3)
      {
        generic_menu = 4;
        get_current = true;
      }
      else  if (generic_menu == 4)
      {
        generic_menu = 5;
        get_current = true;
      }
      else  if (generic_menu == 5)
      {
        display.console = true;
        //  display.fillRect(CHAR_width_small * 6, CHAR_height_small * 29, 204, 8, COLOR_BACKGROUND);
        generic_menu = 99;
      }

      fullrefresh_values_liveseq_pianoroll = true;
    }

    liveseq_pianoroll_draw_graphics();

  }
  if (LCDML.FUNC_close()) // ****** STABLE END *********
  {
    display.fillScreen(COLOR_BACKGROUND);
    encoderDir[ENC_R].reset();
  }
}

void arp_refresh_display_play_status()
{
  if (seq.running == false)
  {
    // play symbol
    drawBitmap(4 * CHAR_width - 4, CHAR_height * 11 + 3, special_chars[19], 8, 8, GREEN);
  }
  else if (seq.running == true)
  {
    // stop symbol
    drawBitmap(4 * CHAR_width - 4, CHAR_height * 11 + 3, special_chars[21], 8, 8, COLOR_SYSTEXT);
  }
}

FLASHMEM void print_arp_start_stop_button()
{
  if (seq.running)
    draw_button_on_grid(2, 23, "SEQ.", "STOP", 1);
  else
    draw_button_on_grid(2, 23, "SEQ.", "START", 0);
  arp_refresh_display_play_status();
}

FLASHMEM void draw_euclidean_circle()
{
  uint8_t r = 61;
  int a = 300;
  int b = 210;
  for (int i = 0; i < 16; i++)
  {
    double t = 2 * PI * i / 16;
    int x = (int)a + r * cos(t);
    int y = (int)b + r * sin(t);
    if (i > 3)
    {
      if (seq.euclidean_state[i - 4])
        display.fillCircle(x - r, y - r, 10, RED);
      else
        display.fillCircle(x - r, y - r, 10, GREY3);
    }
    else
    {
      if (seq.euclidean_state[i + 12])
        display.fillCircle(x - r, y - r, 10, RED);
      else
        display.fillCircle(x - r, y - r, 10, GREY3);
    }
  }
  r = 41;
  a = 300 - 20;
  b = 210 - 20;
  for (int i = 0; i < 16; i++)
  {
    double t = 2 * PI * i / 16;
    int x = (int)a + r * cos(t);
    int y = (int)b + r * sin(t);
    if (seq.step > 4)
    {
      if (i == seq.step - 5)
        display.fillCircle(x - r, y - r, 6, RED);
      else
        display.fillCircle(x - r, y - r, 6, GREY3);
    }
    else
    {
      if (i == seq.step + 11)
        display.fillCircle(x - r, y - r, 6, RED);
      else
        display.fillCircle(x - r, y - r, 6, GREY3);
    }
  }
}

//----------------------------- Euclid calculation functions ---------------------------------------//
//--- the three functions below are taken directly from http://clsound.com/euclideansequenc.html ---//
//--- acknowledgment to Craig Lee ------------------------------------------------------------------//

//------------Function to right rotate n by d bits---------------------------------//
uint16_t rightRotate(int shift, uint16_t value, uint8_t pattern_length)
{
  uint16_t mask = ((1 << pattern_length) - 1);
  value &= mask;
  return ((value >> shift) | (value << (pattern_length - shift))) & mask;
}

//----1---------Function to find the binary length of a number by counting bitwise-------//
int findlength(unsigned int bnry)
{
  boolean lengthfound = false;
  int length = 1; // no number can have a length of zero - single 0 has a length of one, but no 1s for the sytem to count
  for (int q = 32; q >= 0; q--)
  {
    int r = bitRead(bnry, q);
    if (r == 1 && lengthfound == false)
    {
      length = q + 1;
      lengthfound = true;
    }
  }
  return length;
}

//-----2--------Function to concatenate two binary numbers bitwise----------------------//
unsigned int ConcatBin(unsigned int bina, unsigned int binb)
{
  int binb_len = findlength(binb);
  unsigned int sum = (bina << binb_len);
  sum = sum | binb;
  return sum;
}

//------3-------------------Euclidean bit sorting funciton-------------------------------//
unsigned int euclid(int n, int k, int o)
{ // inputs: n=total, k=beats, o = offset
  int pauses = n - k;
  int pulses = k;
  int offset = o;
  int steps = n;
  int per_pulse = pauses / k;
  int remainder = pauses % pulses;
  unsigned int workbeat[n];
  unsigned int outbeat;
  uint16_t outbeat2;
  int workbeat_count = n;
  int a;
  int b;
  int trim_count;

  for (a = 0; a < n; a++)
  { // Populate workbeat with unsorted pulses and pauses
    if (a < pulses)
    {
      workbeat[a] = 1;
    }
    else
    {
      workbeat[a] = 0;
    }
  }

  if (per_pulse > 0 && remainder < 2)
  { // Handle easy cases where there is no or only one remainer
    for (a = 0; a < pulses; a++)
    {
      for (b = workbeat_count - 1; b > workbeat_count - per_pulse - 1; b--)
      {
        workbeat[a] = ConcatBin(workbeat[a], workbeat[b]);
      }
      workbeat_count = workbeat_count - per_pulse;
    }

    outbeat = 0; // Concatenate workbeat into outbeat - according to workbeat_count
    for (a = 0; a < workbeat_count; a++)
    {
      outbeat = ConcatBin(outbeat, workbeat[a]);
    }

    if (offset > 0)
    {
      outbeat2 = rightRotate(offset, outbeat, steps); // Add offset to the step pattern
    }
    else
    {
      outbeat2 = outbeat;
    }

    return outbeat2;
  }

  else
  {
    if (pulses == 0)
    {
      pulses = 1; // Prevent crashes when k=0 and n goes from 0 to 1
    }
    int groupa = pulses;
    int groupb = pauses;
    int iteration = 0;
    if (groupb <= 1)
    {
    }

    while (groupb > 1)
    { // main recursive loop

      if (groupa > groupb)
      {                                    // more Group A than Group B
        int a_remainder = groupa - groupb; // what will be left of groupa once groupB is interleaved
        trim_count = 0;
        for (a = 0; a < groupa - a_remainder; a++)
        { // count through the matching sets of A, ignoring remaindered
          workbeat[a] = ConcatBin(workbeat[a], workbeat[workbeat_count - 1 - a]);
          trim_count++;
        }
        workbeat_count = workbeat_count - trim_count;

        groupa = groupb;
        groupb = a_remainder;
      }

      else if (groupb > groupa)
      {                                    // More Group B than Group A
        int b_remainder = groupb - groupa; // what will be left of group once group A is interleaved
        trim_count = 0;
        for (a = workbeat_count - 1; a >= groupa + b_remainder; a--)
        { // count from right back through the Bs
          workbeat[workbeat_count - a - 1] = ConcatBin(workbeat[workbeat_count - a - 1], workbeat[a]);

          trim_count++;
        }
        workbeat_count = workbeat_count - trim_count;
        groupb = b_remainder;
      }

      else if (groupa == groupb)
      { // groupa = groupb
        trim_count = 0;
        for (a = 0; a < groupa; a++)
        {
          workbeat[a] = ConcatBin(workbeat[a], workbeat[workbeat_count - 1 - a]);
          trim_count++;
        }
        workbeat_count = workbeat_count - trim_count;
        groupb = 0;
      }

      else
      {
        // LOG.println(F("ERROR"));
      }
      iteration++;
    }

    outbeat = 0; // Concatenate workbeat into outbeat - according to workbeat_count
    for (a = 0; a < workbeat_count; a++)
    {
      outbeat = ConcatBin(outbeat, workbeat[a]);
    }

    if (offset > 0)
    {
      outbeat2 = rightRotate(offset, outbeat, steps); // Add offset to the step pattern
    }
    else
    {
      outbeat2 = outbeat;
    }

    return outbeat2;
  }
}
//------------------end euclidian math-------------------------------------//

void update_euclidean()
{
  for (uint8_t i = 0; i < 16; i++)
  {
    if (bitRead(euclid(16, seq.arp_length, 16 - seq.euclidean_offset), i))
      seq.euclidean_state[i] = true;
    else
      seq.euclidean_state[i] = false;
  }
}

void show_euclidean()
{
  update_euclidean();
  draw_euclidean_circle();
}

void UI_func_arpeggio(uint8_t param)
{
  char displayname[8] = { 0, 0, 0, 0, 0, 0, 0 };
  if (LCDML.FUNC_setup()) // ****** SETUP *********
  {

    encoderDir[ENC_R].reset();
    generic_temp_select_menu = 0;
    seq.temp_active_menu = 0;
    display.fillScreen(COLOR_BACKGROUND);
    display.setTextSize(1);
    seq.edit_state = false;
    setCursor_textGrid_large(1, 1);
    display.setTextColor(RED);
    display.print(F("ARPEGGIO SETTINGS"));
    display.setTextColor(COLOR_SYSTEXT, COLOR_BACKGROUND);
    setCursor_textGrid_small(2, 4);
    display.print(F("LENGTH"));
    setCursor_textGrid_small(15, 4);
    display.print(F("STEPS"));
    setCursor_textGrid_small(2, 5);
    display.print(F("STYLE"));
    setCursor_textGrid_small(2, 6);
    display.print(F("SPEED"));
    setCursor_textGrid_small(17, 6);
    display.setTextColor(GREY2, COLOR_BACKGROUND);
    display.print(F("32/64 MICROSYNTH ONLY [SID STYLE]"));
    display.setTextColor(COLOR_SYSTEXT, COLOR_BACKGROUND);
    setCursor_textGrid_small(2, 7);
    display.print(F("OFFSET"));
    setCursor_textGrid_small(2, 8);
    display.print(F("MODE"));
    setCursor_textGrid_small(2, 9);
    display.print(F("MX NOTES"));
    display.setTextColor(GREY2, COLOR_BACKGROUND);
    setCursor_textGrid_small(14, 9);
    display.print(F("[SID STYLE]"));

    setCursor_textGrid_small(2, 11);
    display.print(F("FROM ADV. SETTINGS:"));

    setCursor_textGrid_small(2, 13);
    display.print(F("VELOCITY"));

    setCursor_textGrid_small(17, 13);
    print_formatted_number(seq.chord_vel, 3);

    setCursor_textGrid_small(2, 14);
    display.print(F("OCTAVE SHIFT"));

    setCursor_textGrid_small(17, 14);
    snprintf_P(displayname, sizeof(displayname), PSTR("%02d"), seq.oct_shift);
    display.print(displayname);

    setCursor_textGrid_small(2, 15);
    display.print(F("NOTE SHIFT"));

    setCursor_textGrid_small(17, 15);
    display.print(seq.element_shift);

    setCursor_textGrid_small(2, 17);
    display.print(F("PLAYING:"));
    setCursor_textGrid_small(11, 17);
    display.print(F("["));
    setCursor_textGrid_small(19, 17);
    display.print(F("]"));
    print_arp_start_stop_button();

    helptext_l(back_text);
    display.setTextSize(2);
    show_euclidean();
  }
  if (LCDML.FUNC_loop()) // ****** LOOP *********
  {
    if ((LCDML.BT_checkDown() && encoderDir[ENC_R].Down()) || (LCDML.BT_checkUp() && encoderDir[ENC_R].Up()))
    {
      if (LCDML.BT_checkDown())
      {
        if (seq.edit_state == false)
          generic_temp_select_menu = constrain(generic_temp_select_menu + 1, 0, 5);
        else if (generic_temp_select_menu == 0)
          seq.arp_length = constrain(seq.arp_length + ENCODER[ENC_R].speed(), 0, 16);
        else if (generic_temp_select_menu == 1)
          seq.arp_style = constrain(seq.arp_style + ENCODER[ENC_R].speed(), 0, 3);
        else if (generic_temp_select_menu == 2)
          seq.arp_speed = constrain(seq.arp_speed + ENCODER[ENC_R].speed(), 0, 3);
        else if (generic_temp_select_menu == 3)
          seq.euclidean_offset = constrain(seq.euclidean_offset + ENCODER[ENC_R].speed(), 0, 15);
        else if (generic_temp_select_menu == 4)
          seq.euclidean_active = !seq.euclidean_active;
        else if (generic_temp_select_menu == 5)
          seq.arp_num_notes_max = constrain(seq.arp_num_notes_max + 1, 1, 64);

      }
      else if (LCDML.BT_checkUp())
      {
        if (seq.edit_state == false)
          generic_temp_select_menu = constrain(generic_temp_select_menu - 1, 0, 5);
        else if (generic_temp_select_menu == 0)
          seq.arp_length = constrain(seq.arp_length - ENCODER[ENC_R].speed(), 0, 16);
        else if (generic_temp_select_menu == 1)
          seq.arp_style = constrain(seq.arp_style - ENCODER[ENC_R].speed(), 0, 3);
        else if (generic_temp_select_menu == 2)
          seq.arp_speed = constrain(seq.arp_speed - ENCODER[ENC_R].speed(), 0, 3);
        else if (generic_temp_select_menu == 3)
          seq.euclidean_offset = constrain(seq.euclidean_offset - ENCODER[ENC_R].speed(), 0, 15);
        else if (generic_temp_select_menu == 4)
          seq.euclidean_active = !seq.euclidean_active;
        else if (generic_temp_select_menu == 5)
          seq.arp_num_notes_max = constrain(seq.arp_num_notes_max - 1, 1, 64);

      }
    }
    if (LCDML.BT_checkEnter() && encoderDir[ENC_R].ButtonShort()) // handle button presses during menu >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
    {
      if (seq.edit_state == 0)
        seq.edit_state = 1;
      else
        seq.edit_state = 0;
    }
    // button check end <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
    show_euclidean();

    if (seq.edit_state == false)
      helptext_r("< > SELECT OPTION TO EDIT");
    else
      helptext_r("< > EDIT VALUE");
    display.setTextSize(1);

    setModeColor(0);
    setCursor_textGrid_small(11, 4);
    if (seq.arp_length == 0)
      display.print("ALL");
    else
      print_formatted_number(seq.arp_length, 3); // play all elements or from 1-xx elements

    setModeColor(1);
    setCursor_textGrid_small(11, 5);
    for (uint8_t i = 0; i < 4; i++)
    {
      if (i == seq.arp_style && generic_temp_select_menu == 1)
        display.setTextColor(COLOR_SYSTEXT, GREY3);
      else if (i == seq.arp_style)
        display.setTextColor(RED, GREY3);
      else if (generic_temp_select_menu == 1)
        display.setTextColor(COLOR_BACKGROUND, GREY3);
      else
        display.setTextColor(GREY2, GREY3);
      display.print(seq.arp_style_names[i][0]);
      display.print(seq.arp_style_names[i][1]);
      display.print(seq.arp_style_names[i][2]);
      display.setTextColor(GREY1, COLOR_BACKGROUND);
      display.print(" ");
    }
    setModeColor(2);
    setCursor_textGrid_small(11, 6);
    if (seq.arp_speed == 0)
      display.print("1/16 ");
    else if (seq.arp_speed == 1)
      display.print("1/8  ");
    else if (seq.arp_speed == 2)
      display.print("1/32 ");
    else if (seq.arp_speed == 3)
      display.print("1/64 ");

    setModeColor(3);
    setCursor_textGrid_small(11, 7);
    print_formatted_number(seq.euclidean_offset, 2);

    if (generic_temp_select_menu != 4)
    {
      setCursor_textGrid_small(11, 8);
      if (seq.euclidean_active)
        display.setTextColor(GREY2, GREY3);
      else
        display.setTextColor(RED, GREY3);
      display.print("LINEAR");
      setCursor_textGrid_small(18, 8);
      if (seq.euclidean_active)
        display.setTextColor(RED, GREY3);
      else
        display.setTextColor(GREY2, GREY3);
      display.print("EUCLIDEAN");
    }
    else
    {
      setCursor_textGrid_small(11, 8);
      if (seq.euclidean_active)
        display.setTextColor(GREY2, GREY3);
      else
        display.setTextColor(COLOR_SYSTEXT, COLOR_BACKGROUND);
      display.print("LINEAR");
      setCursor_textGrid_small(18, 8);
      if (seq.euclidean_active)
        display.setTextColor(COLOR_SYSTEXT, COLOR_BACKGROUND);
      else
        display.setTextColor(GREY2, GREY3);
      display.print("EUCLIDEAN");
    }

    setModeColor(5);

    setCursor_textGrid_small(11, 9);
    snprintf_P(displayname, sizeof(displayname), PSTR("%02d"), seq.arp_num_notes_max);
    display.print(displayname);

  }
  if (LCDML.FUNC_close()) // ****** STABLE END *********
  {
    seq.menu = 0;
    seq.edit_state = false;
    encoderDir[ENC_R].reset();

    display.setTextColor(COLOR_SYSTEXT, COLOR_BACKGROUND);
    display.fillScreen(COLOR_BACKGROUND);
  }
}

void UI_func_seq_mute_matrix(uint8_t param)
{
  if (LCDML.FUNC_setup()) // ****** SETUP *********
  {
    // setup function
    display.fillScreen(COLOR_BACKGROUND);
    UI_toplineInfoText(1);
    display.setTextSize(1);
    display.setCursor(1, 2);
    display.setTextColor(COLOR_SYSTEXT, DX_DARKCYAN);
    display.print("MUTE");
    display.setCursor(1 + 5 * CHAR_width_small, 2);
    display.print("MATRIX");
    helptext_l(back_text);
    helptext_r("TOUCH SCREEN TO MUTE/UNMUTE");
  }
  if (LCDML.FUNC_loop()) // ****** LOOP *********
  {
    uint8_t button_count = 0;
    char buf[4];

    display.setTextColor(COLOR_SYSTEXT, COLOR_BACKGROUND);
    setCursor_textGrid_small(2, 4);
    display.print("MUTE/UNMUTE");
    setCursor_textGrid_small(2, 5);
    display.print("AT/IN:");

    for (uint8_t y = 0; y < 3; y++)
    {
      for (uint8_t x = 0; x < 4; x++)
      {
        if (y < 2)
        {
          if (!seq.track_mute[button_count])
            draw_button_on_grid(2 + x * 14, 12 + y * 8, "TRACK:", itoa(button_count + 1, buf, 10), 1);
          else
            draw_button_on_grid(2 + x * 14, 12 + y * 8, "TRACK:", itoa(button_count + 1, buf, 10), 0);
          button_count++;
        }
        else
        {
          if (x == 1)
          {
            if (seq.mute_mode == 0)
              draw_button_on_grid(2 + x * 14, 4, "REAL", "TIME", 1);
            else
              draw_button_on_grid(2 + x * 14, 4, "REAL", "TIME", 0);
          }
          else if (x == 2)
          {
            if (seq.mute_mode == 1)
              draw_button_on_grid(2 + x * 14, 4, "NEXT", "PATTRN", 1);
            else
              draw_button_on_grid(2 + x * 14, 4, "NEXT", "PATTRN", 0);
          }
          else if (x == 3)
          {
            if (seq.mute_mode == 2)
              draw_button_on_grid(2 + x * 14, 4, "SONG", "STEP", 1);
            else
              draw_button_on_grid(2 + x * 14, 4, "SONG", "STEP", 0);
          }
        }
      }
    }
  }
  if (LCDML.FUNC_close()) // ****** STABLE END *********
  {
    encoderDir[ENC_R].reset();
    display.fillScreen(COLOR_BACKGROUND);
    display.setTextColor(COLOR_SYSTEXT, COLOR_BACKGROUND);
    display.setTextSize(2);
  }
}

FLASHMEM uint8_t count_omni()
{
  uint8_t count = 0;

  if (configuration.dexed[0].midi_channel == MIDI_CHANNEL_OMNI)
    count++;
  if (configuration.dexed[1].midi_channel == MIDI_CHANNEL_OMNI)
    count++;
  if (configuration.epiano.midi_channel == MIDI_CHANNEL_OMNI)
    count++;
  if (microsynth[0].midi_channel == MIDI_CHANNEL_OMNI)
    count++;
  if (microsynth[1].midi_channel == MIDI_CHANNEL_OMNI)
    count++;
  if (braids_osc.midi_channel == MIDI_CHANNEL_OMNI)
    count++;
  if (msp[0].midi_channel == MIDI_CHANNEL_OMNI)
    count++;
  if (msp[1].midi_channel == MIDI_CHANNEL_OMNI)
    count++;
  if (drum_midi_channel == MIDI_CHANNEL_OMNI)
    count++;

  if (count != 0)
    check_and_confirm_midi_channels = true;
  return count;
}

FLASHMEM uint8_t count_midi_channel_duplicates(bool find_first)
{
  uint8_t count = 0;
  uint8_t midi_ch[17] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
  uint8_t ins[9] = { 0, 0, 0, 0, 0, 0, 0, 0, 0 };

  ins[0] = configuration.dexed[0].midi_channel;
  ins[1] = configuration.dexed[1].midi_channel;
  ins[2] = configuration.epiano.midi_channel;
  ins[3] = microsynth[0].midi_channel;
  ins[4] = microsynth[1].midi_channel;
  ins[5] = braids_osc.midi_channel;
  ins[6] = msp[0].midi_channel;
  ins[7] = msp[1].midi_channel;
  ins[8] = drum_midi_channel;

  for (uint8_t i = 1; i < 17; i++)
  {

    for (uint8_t j = 0; j < 9; j++)
    {
      if (ins[j] == i)
        midi_ch[i]++;
    }
  }

  for (uint8_t i = 1; i < 17; i++)
  {
    if (midi_ch[i] > 1)
    {
      count++;
      if (find_first)
      {
        count = i;
        break;
      }
    }
  }
  if (count != 0)
    check_and_confirm_midi_channels = true;
  return count;
}

FLASHMEM bool load_performance_and_check_midi(uint8_t perf)
{
  bool ret = load_sd_performance_json(perf);

  // check MIDI channels
  if (count_omni() != 0 || count_midi_channel_duplicates(false) != 0) // startup with midi channel setup page
    LCDML.OTHER_jumpToFunc(UI_func_midi_channels);

  return ret;
}

void UI_func_load_performance(uint8_t param)
{
  static uint8_t mode;
  if (LCDML.FUNC_setup()) // ****** SETUP *********
  {
    char tmp[10];
    if (seq.state_last_loadsave != 200)
    {
      temp_int = seq.state_last_loadsave;
    }
    else
    {
      temp_int = param;
    }

    mode = 0;
    encoderDir[ENC_R].reset();
    border1_clear();
    setCursor_textGrid(1, 1);
    display.print(F("Load Performance"));
    setCursor_textGrid(1, 2);
    snprintf_P(tmp, sizeof(tmp), PSTR("[%2d]"), param);
    display.print(tmp);
  }

  if (LCDML.FUNC_loop()) // ****** LOOP *********
  {
    if ((LCDML.BT_checkDown() && encoderDir[ENC_R].Down()) || (LCDML.BT_checkUp() && encoderDir[ENC_R].Up()))
    {
      set_state_dir();
      if (seq.edit_state == 0)
      {
        temp_int = calc_val[state_dir](temp_int, 1, 0, 99);
      }
    }

    if (LCDML.BT_checkEnter())
    {
      display.fillRect(0, CHAR_height * 3, CHAR_width * 19, CHAR_height * 4, COLOR_BACKGROUND);
      mode = 0xff;
      setCursor_textGrid(1, 2);

      bool perfLoaded = load_performance_and_check_midi(temp_int);
      if (!perfLoaded)
      {
        display.setTextColor(RED, COLOR_BACKGROUND);
        display.print(F("Does not exist."));
      }
      else
      {
        seq.state_last_loadsave = temp_int;
        setCursor_textGrid(1, 2);
        seq.play_mode = false;
        display.setTextColor(GREEN, COLOR_BACKGROUND);
        display.print(F("Done.           "));
      }
      delay(MESSAGE_WAIT_TIME);
      LCDML.FUNC_goBackToMenu();
    }

    if (mode != 0xff)
    {
      display.setTextColor(COLOR_SYSTEXT, COLOR_BACKGROUND);
      setCursor_textGrid(1, 2);
      char tmp[10];
      snprintf_P(tmp, sizeof(tmp), PSTR("[%2d] "), temp_int);
      display.print(tmp);
      if (check_sd_performance_exists(temp_int))
      {
        get_sd_performance_name_json(temp_int);
        if (seq.name_temp[0] != 0)
          show(2, 6, 11, seq.name_temp);
        else
          display.print(F(" -- DATA --"));
      }
      else
        print_empty_spaces(11);

      //      for (uint8_t nextslot = 1; nextslot < 5; nextslot++)
      //      {
      //        display.setTextColor(GREY3, COLOR_BACKGROUND);
      //        setCursor_textGrid(1, 2 + nextslot);
      //        if (temp_int + nextslot < 100)
      //        {
      //          snprintf_P(tmp, sizeof(tmp), PSTR("[%2d] "), temp_int + nextslot);
      //          display.print(tmp);
      //          if (check_sd_performance_exists(temp_int + nextslot))
      //          {
      //            get_sd_performance_name_json(temp_int + nextslot);
      //            if ( seq.name_temp[0] != 0 )
      //              show(2 + nextslot, 6, 11, seq.name_temp);
      //            else
      //              display.print(F(" -- DATA --"));
      //          }
      //          else print_empty_spaces(11);
      //        }
      //        else
      //        {
      //          setCursor_textGrid(1, 2 + nextslot);
      //          print_empty_spaces(11);
      //        }
      //      }
    }
  }

  if (LCDML.FUNC_close()) // ****** STABLE END *********
  {
    if (mode < 0xff)
    {
      display.fillRect(0, CHAR_height * 3, CHAR_width * 19, CHAR_height * 4, COLOR_BACKGROUND);
      display.setTextColor(RED, COLOR_BACKGROUND);
      show(2, 1, 16, "Canceled.");
      delay(MESSAGE_WAIT_TIME);
    }
    else
    {
      display.setTextColor(COLOR_SYSTEXT, COLOR_BACKGROUND);
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
  if (LCDML.FUNC_setup()) // ****** SETUP *********
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
    snprintf_P(tmp, sizeof(tmp), PSTR("[%2d] "), temp_int);
    display.print(tmp);

    if (check_sd_performance_exists(temp_int))
    {
      overwrite = true;
      get_sd_performance_name_json(temp_int);
      if (seq.name_temp[0] != 0)
        show(2, 6, 11, seq.name_temp);
      else
        display.print(F(" -- DATA --"));
    }
    else
      overwrite = false;
  }
  if (LCDML.FUNC_loop()) // ****** LOOP *********
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
            save_sd_performance_json(temp_int);
            display.setTextColor(GREEN, COLOR_BACKGROUND);
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
            snprintf_P(tmp, sizeof(tmp), PSTR("[%2d]   "), temp_int);
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
        snprintf_P(tmp, sizeof(tmp), PSTR("[%2d] "), temp_int);
        display.print(tmp);
        setCursor_textGrid(6, 2);
        if (overwrite == false)
        {
          display.print("-- EMPTY --");
        }
        else if (check_sd_performance_exists(temp_int))
        {
          get_sd_performance_name_json(temp_int);
          if (seq.name_temp[0] != 0)
            show(2, 6, 11, seq.name_temp);
          else
            display.print("-- DATA --");
        }
        else
          print_empty_spaces(10);
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
  if (LCDML.FUNC_close()) // ****** STABLE END *********
  {
    if (mode < 0xff)
    {
      show(2, 1, 16, "Canceled.");
      delay(MESSAGE_WAIT_TIME);
    }
    encoderDir[ENC_R].reset();
  }
}

FLASHMEM void set_delay_sync(uint8_t sync, uint8_t instance)
{

  if (seq.running == false)
  {
    // uint16_t midi_sync_delay_time = uint16_t(60000.0 * midi_ticks_factor[sync] / midi_bpm + 0.5);
    uint16_t midi_sync_delay_time = uint16_t(60000.0 * midi_ticks_factor[sync] / seq.bpm);
    delay_fx[instance]->delay(0, constrain(midi_sync_delay_time * configuration.fx.delay_multiplier[instance], DELAY_TIME_MIN, DELAY_TIME_MAX * 10));
    if (midi_sync_delay_time > DELAY_MAX_TIME)
    {
#ifdef DEBUG
      LOG.println(F("Calculated MIDI-Sync delay: "));
      LOG.print(round(60000.0 * midi_ticks_factor[sync] / midi_bpm), DEC);
      LOG.println(F("ms"));
      LOG.println(F("MIDI-Sync delay: midi_sync_delay_time"));
      LOG.print(midi_sync_delay_time, DEC);
      LOG.println(F("ms"));
#endif
    }
  }
  else
  {
    uint16_t midi_sync_delay_time = uint16_t(60000.0 * midi_ticks_factor[sync] / seq.bpm);
    delay_fx[instance]->delay(0, constrain(midi_sync_delay_time * configuration.fx.delay_multiplier[instance], DELAY_TIME_MIN, DELAY_TIME_MAX * 10));
    }
  }

FLASHMEM void print_sync_timing(uint8_t sync)
{
  switch (sync)
  {
  case 1:
    display.print(F("1/16 "));
    break;
  case 2:
    display.print(F("1/16T"));
    break;
  case 3:
    display.print(F("1/8  "));
    break;
  case 4:
    display.print(F("1/8T "));
    break;
  case 5:
    display.print(F("1/4  "));
    break;
  case 6:
    display.print(F("1/4T "));
    break;
  case 7:
    display.print(F("1/2  "));
    break;
  case 8:
    display.print(F("1/2T "));
    break;
  case 9:
    display.print(F("1/1  "));
    break;
  }
}

FLASHMEM void print_delay_sync_status(uint8_t instance)
{

  if (configuration.fx.delay_sync[instance] > 0)
  {
    if (instance == 0)
    {
      setCursor_textGrid_small(6, 6);
    }
    else
    {
      setCursor_textGrid_small(22, 6);
    }
    display.setTextColor(GREY1, GREY2);
    display.print(F("ON "));
    if (instance == 0)
      setCursor_textGrid_small(10, 6);
    else
      setCursor_textGrid_small(26, 6);
    display.setTextColor(COLOR_SYSTEXT, COLOR_BACKGROUND);
    print_sync_timing(configuration.fx.delay_sync[instance]);
  }
  else
  {
    if (instance == 0)
    {
      setCursor_textGrid_small(6, 6);
    }
    else
    {
      setCursor_textGrid_small(22, 6);
    }
    display.setTextColor(GREY2, GREY3);
    display.print(F("OFF"));
    print_empty_spaces(6);
  }
  display.setTextColor(COLOR_SYSTEXT, COLOR_BACKGROUND);
}

void print_static_text_master_effects()
{
  if (configuration.fx.delay_multiplier[0] == 0)
    configuration.fx.delay_multiplier[0] = 1;
  if (configuration.fx.delay_multiplier[1] == 0)
    configuration.fx.delay_multiplier[1] = 1;

  display.setTextSize(1);
  display.setTextColor(RED, COLOR_BACKGROUND);
  setCursor_textGrid_small(1, 1);
  display.print(F("MASTER EFFECTS"));
  display.setTextColor(COLOR_SYSTEXT, GREY4);
  setCursor_textGrid_small(1, 3);
  display.print(F("DELAY A"));

  display.setTextColor(GREY1, GREY4);
  setCursor_textGrid_small(1, 4);
  display.print(F("MULTIPLIER"));
  setCursor_textGrid_small(1, 5);
  display.print(F("TIME"));
  setCursor_textGrid_small(13, 5);
  display.print(F("MS"));
  setCursor_textGrid_small(1, 6);
  display.print(F("SYNC"));

  display.setTextColor(GREY1, GREY4);
  setCursor_textGrid_small(1, 8);
  display.print(F("FDBK"));
  setCursor_textGrid_small(1, 9);
  display.print(F("PAN"));
  setCursor_textGrid_small(1, 10);
  display.print(F("LEVEL"));
  setCursor_textGrid_small(1, 11);
  display.print(F("FILTER"));
  setCursor_textGrid_small(1, 12);
  display.print(F("FREQ."));

  display.setTextColor(COLOR_SYSTEXT, GREY3);
  setCursor_textGrid_small(17, 3);
  display.print(F("DELAY B"));
  display.setTextColor(GREY1, GREY3);
  setCursor_textGrid_small(17, 4);
  display.print(F("MULTIPLIER"));
  setCursor_textGrid_small(17, 5);
  display.print(F("TIME"));
  setCursor_textGrid_small(29, 5);
  display.print(F("MS"));
  setCursor_textGrid_small(17, 6);
  display.print(F("SYNC"));

  display.setTextColor(GREY1, GREY3);
  setCursor_textGrid_small(17, 8);
  display.print(F("FDBK"));
  setCursor_textGrid_small(17, 9);
  display.print(F("PAN"));
  setCursor_textGrid_small(17, 10);
  display.print(F("LEVEL"));
  setCursor_textGrid_small(17, 11);
  display.print(F("FILTER"));
  setCursor_textGrid_small(17, 12);
  display.print(F("FREQ."));

  display.setTextColor(COLOR_SYSTEXT, GREY4);
  setCursor_textGrid_small(33, 3);
  display.print(F("REVERB"));
  display.setTextColor(GREY1, GREY4);
  setCursor_textGrid_small(33, 5);
  display.print(F("ROOMSIZE"));
  setCursor_textGrid_small(33, 6);
  display.print(F("LOWPASS"));
  setCursor_textGrid_small(33, 7);
  display.print(F("LODAMP"));
  setCursor_textGrid_small(33, 8);
  display.print(F("HIDAMP"));
  setCursor_textGrid_small(33, 9);
  display.print(F("DIFFUSION"));
  setCursor_textGrid_small(33, 10);
  display.print(F("LEVEL"));

  display.setTextColor(GREY2, GREY4);
  setCursor_textGrid_small(1, 13);
  display.print(F("SEND LEVELS A"));

  display.setTextColor(GREY2, GREY3);
  setCursor_textGrid_small(17, 13);
  display.print(F("SEND LEVELS B"));

  display.setTextColor(GREY1, GREY4);
  setCursor_textGrid_small(1, 14);
  display.print(F("DX1"));
  setCursor_textGrid_small(1, 15);
  display.print(F("EP"));
  setCursor_textGrid_small(1, 16);
  display.print(F("MS1"));
  setCursor_textGrid_small(1, 17);
  display.print(F("MS2"));
  setCursor_textGrid_small(1, 18);
  display.print(F("BRD"));
  setCursor_textGrid_small(1, 19);
  display.print(F("DL2"));
  setCursor_textGrid_small(1, 20);
  display.print(F("REV"));

  display.setTextColor(GREY1, GREY3);
  setCursor_textGrid_small(17, 14);
  display.print(F("DX2"));
  setCursor_textGrid_small(17, 15);
  display.print(F("EP"));
  setCursor_textGrid_small(17, 16);
  display.print(F("MS1"));
  setCursor_textGrid_small(17, 17);
  display.print(F("MS2"));
  setCursor_textGrid_small(17, 18);
  display.print(F("BRD"));
  setCursor_textGrid_small(17, 19);
  display.print(F("DL1"));
  setCursor_textGrid_small(17, 20);
  display.print(F("REV"));

  display.setTextColor(GREY1, GREY4);
  setCursor_textGrid_small(33, 14);
  display.print(F("DX1"));
  setCursor_textGrid_small(33, 15);
  display.print(F("DX2"));
  setCursor_textGrid_small(33, 16);
  display.print(F("EP"));
  setCursor_textGrid_small(33, 17);
  display.print(F("MS1"));
  setCursor_textGrid_small(33, 18);
  display.print(F("MS2"));
  setCursor_textGrid_small(33, 19);
  display.print(F("BRD"));
}

void print_delay_time(uint8_t instance, uint8_t param)
{
  setModeColor(param);
  if (instance == 0)
    setCursor_textGrid_small(6, 5);
  else
    setCursor_textGrid_small(22, 5);

  if (configuration.fx.delay_sync[instance] > 0)
  {
    uint16_t synced_delay_time = uint16_t(60000.0 * midi_ticks_factor[configuration.fx.delay_sync[instance]] / seq.bpm) * configuration.fx.delay_multiplier[instance];
    if (synced_delay_time <= DELAY_MAX_TIME)
    {
      print_formatted_number(synced_delay_time, 4);
      if (seq.edit_state == 1)
        helptext_r("SCROLL > FOR TIME IN MS");
    }
    else
    {
      display.print(F("---"));
      if (seq.edit_state == 1)
        helptext_r("NOT ENOUGH MEMORY");
    }
  }
  else
  {
    print_formatted_number(configuration.fx.delay_time[instance] * 10, 4);
    if (seq.edit_state == 1)
      helptext_r("SCROLL < FOR SYNCED TIME");
  }
}

void set_global_delay_filter(uint8_t instance)
{
  global_delay_filter[instance].resonance(3);
  if (configuration.fx.delay_filter_mode[instance] == 0)
  {

    global_delay_filter_mixer[instance].gain(0, 1.0);
    global_delay_filter_mixer[instance].gain(1, 0.0);
    global_delay_filter_mixer[instance].gain(2, 0.0);
    global_delay_filter_mixer[instance].gain(3, 0.0);
  }
  else if (configuration.fx.delay_filter_mode[instance] == 1)
  {
    global_delay_filter[instance].frequency(configuration.fx.delay_filter_freq[instance] / 1.7);
    global_delay_filter_mixer[instance].gain(0, 0.0);
    global_delay_filter_mixer[instance].gain(1, 1.0);
  }
  else if (configuration.fx.delay_filter_mode[instance] == 2)
  {
    global_delay_filter[instance].frequency(configuration.fx.delay_filter_freq[instance] / 1.6);
    global_delay_filter_mixer[instance].gain(0, 0.0);
    global_delay_filter_mixer[instance].gain(2, 1.0);
  }
  else if (configuration.fx.delay_filter_mode[instance] == 3)
  {
    global_delay_filter[instance].frequency(configuration.fx.delay_filter_freq[instance] * 2.6);
    global_delay_filter_mixer[instance].gain(0, 0.0);
    global_delay_filter_mixer[instance].gain(3, 1.0);
  }
}

void print_delay_filter_mode(uint8_t instance, uint8_t param)
{
  uint8_t x = 0;
  if (instance == 0)
    x = 10;
  else if (instance == 1)
    x = 26;
  setModeColor(param);
  setCursor_textGrid_small(x, 11);
  if (configuration.fx.delay_filter_mode[instance] == 0)
    display.print(F("OFF "));

  else if (configuration.fx.delay_filter_mode[instance] == 1)
    display.print(F("LOW "));
  else if (configuration.fx.delay_filter_mode[instance] == 2)
    display.print(F("BAND"));
  else if (configuration.fx.delay_filter_mode[instance] == 3)
    display.print(F("HIGH"));
  set_global_delay_filter(instance);
}

void print_delay_filter_freq(uint8_t instance, uint8_t param)
{
  uint8_t x = 0;
  if (instance == 0)
    x = 10;
  else if (instance == 1)
    x = 26;
  setModeColor(param);
  print_small_intbar(x - 4, 12, configuration.fx.delay_filter_freq[instance] / 100, param, 1, 1);
  set_global_delay_filter(instance);
}

void print_delay_multiplier(uint8_t instance, uint8_t param)
{
  setModeColor(param);

  if (instance == 0)
    setCursor_textGrid_small(6 + 7, 4);
  else
    setCursor_textGrid_small(22 + 7, 4);
  display.print(configuration.fx.delay_multiplier[instance]);
  display.print(F("x"));
  print_delay_time(instance, 99);
  set_global_delay_filter(instance);
}

void update_selective_values_master_effects()
{
  print_delay_sync_status(0);
  print_delay_sync_status(1);
  if (menu_item_check(1))
    print_delay_multiplier(0, 1);

  if (menu_item_check(2))
    print_delay_time(0, 2);

  if (menu_item_check(3))
    print_small_intbar(6, 8, configuration.fx.delay_feedback[0], 3, 1, 0);

  if (menu_item_check(4))
    print_small_panbar(6, 9, configuration.fx.delay_pan[0], 4);

  if (menu_item_check(5))
    print_small_intbar(6, 10, configuration.fx.delay_level_global[0], 5, 1, 0);

  if (menu_item_check(6))
    print_delay_filter_mode(0, 6);

  if (menu_item_check(7))
    print_delay_filter_freq(0, 7);

  if (menu_item_check(8))
    print_small_intbar(6, 14, configuration.fx.delay_level[0], 8, 1, 0);
  if (menu_item_check(9))
    print_small_intbar(6, 15, configuration.fx.ep_delay_send_1, 9, 1, 0);
  if (menu_item_check(10))
    print_small_intbar(6, 16, microsynth[0].delay_send[0], 10, 1, 0);
  if (menu_item_check(11))
    print_small_intbar(6, 17, microsynth[1].delay_send[0], 11, 1, 0);
  if (menu_item_check(12))
    print_small_intbar(6, 18, braids_osc.delay_send_1, 12, 1, 0);
  if (menu_item_check(13))
    print_small_intbar(6, 19, configuration.fx.delay1_to_delay2, 13, 1, 0);
  if (menu_item_check(14))
    print_small_intbar(6, 20, configuration.fx.delay_to_reverb[0], 14, 1, 0);

  if (menu_item_check(15))
    print_delay_multiplier(1, 15);

  if (menu_item_check(16))
    print_delay_time(1, 16);

  if (menu_item_check(17))
    print_small_intbar(22, 8, configuration.fx.delay_feedback[1], 17, 1, 0);

  if (menu_item_check(18))
    print_small_panbar(22, 9, configuration.fx.delay_pan[1], 18);

  if (menu_item_check(19))
    print_small_intbar(22, 10, configuration.fx.delay_level_global[1], 19, 1, 0);

  if (menu_item_check(20))
    print_delay_filter_mode(1, 20);

  if (menu_item_check(21))
    print_delay_filter_freq(1, 21);

  if (menu_item_check(22))
    print_small_intbar(22, 14, configuration.fx.delay_level[1], 22, 1, 0);
  if (menu_item_check(23))
    print_small_intbar(22, 15, configuration.fx.ep_delay_send_2, 23, 1, 0);
  if (menu_item_check(24))
    print_small_intbar(22, 16, microsynth[0].delay_send[1], 24, 1, 0);
  if (menu_item_check(25))
    print_small_intbar(22, 17, microsynth[1].delay_send[1], 25, 1, 0);
  if (menu_item_check(26))
    print_small_intbar(22, 18, braids_osc.delay_send_2, 26, 1, 0);
  if (menu_item_check(27))
    print_small_intbar(22, 19, configuration.fx.delay2_to_delay1, 27, 1, 0);
  if (menu_item_check(28))
    print_small_intbar(22, 20, configuration.fx.delay_to_reverb[1], 28, 1, 0);

  if (menu_item_check(29))
    print_small_intbar(43, 5, configuration.fx.reverb_roomsize, 29, 1, 0);
  if (menu_item_check(30))
    print_small_intbar(43, 6, configuration.fx.reverb_lowpass, 30, 1, 0);
  if (menu_item_check(31))
    print_small_intbar(43, 7, configuration.fx.reverb_lodamp, 31, 1, 0);
  if (menu_item_check(32))
    print_small_intbar(43, 8, configuration.fx.reverb_hidamp, 32, 1, 0);
  if (menu_item_check(33))
    print_small_intbar(43, 9, configuration.fx.reverb_diffusion, 33, 1, 0);
  if (menu_item_check(34))
    print_small_intbar(43, 10, configuration.fx.reverb_level, 34, 1, 0);

  if (menu_item_check(35))
    print_small_intbar(43, 14, configuration.fx.reverb_send[0], 35, 1, 0);
  if (menu_item_check(36))
    print_small_intbar(43, 15, configuration.fx.reverb_send[1], 36, 1, 0);
  if (menu_item_check(37))
    print_small_intbar(43, 16, configuration.fx.ep_reverb_send, 37, 1, 0);
  if (menu_item_check(38))
    print_small_intbar(43, 17, microsynth[0].rev_send, 38, 1, 0);
  if (menu_item_check(39))
    print_small_intbar(43, 18, microsynth[1].rev_send, 39, 1, 0);
  if (menu_item_check(40))
    print_small_intbar(43, 19, braids_osc.rev_send, 40, 1, 0);
}

// #ifdef PSRAM
extern "C" uint8_t external_psram_size;
// #endif

void UI_func_master_effects(uint8_t param)
{
  if (LCDML.FUNC_setup()) // ****** SETUP *********
  {
    encoderDir[ENC_R].reset();
    display.fillScreen(COLOR_BACKGROUND);

#ifdef PSRAM
    char text1[30];
    uint8_t size = external_psram_size;
#endif

    setCursor_textGrid_small(1, 22);
    display.setTextColor(GREY2);
    display.setTextSize(1);


#ifdef PSRAM
    if (size != 0)
    {
      sprintf(text1, "%d MB PSRAM CHIP FOUND, MAX DELAY: 2x %d MS", size, DELAY_MAX_TIME);
      display.print(text1);
    }
    else
      display.print(F("NO VALID PSRAM CHIP FOUND"));
#else
    display.print(F("FOR LONGER DELAY TIMES YOU CAN ADD A PSRAM CHIP"));
#endif
    display.fillRect(0, 24, 95, DISPLAY_HEIGHT - 52, GREY4);
    display.fillRect(98, 24, 92, DISPLAY_HEIGHT - 52, GREY3);
    display.fillRect(193, 24, 130, DISPLAY_HEIGHT - 52, GREY4);

    print_static_text_master_effects();
    if (LCDML.MENU_getLastActiveFunctionID() != LCDML.OTHER_getIDFromFunction(UI_func_volume) && LCDML.MENU_getLastActiveFunctionID() != LCDML.OTHER_getIDFromFunction(mFunc_screensaver))

      generic_temp_select_menu = 1;
    generic_full_draw_required = true;
    update_selective_values_master_effects();
    generic_full_draw_required = false;

    helptext_l(back_text);
    helptext_r("SELECT PARAM.");
  }
  if (LCDML.FUNC_loop()) // ****** LOOP *********
  {

    if ((LCDML.BT_checkDown() && encoderDir[ENC_R].Down()) || (LCDML.BT_checkUp() && encoderDir[ENC_R].Up()) || (LCDML.BT_checkEnter() && encoderDir[ENC_R].ButtonShort()))
    {

      if (seq.edit_state == 0)
      {
        if (LCDML.BT_checkDown() && generic_temp_select_menu < 40)
        {
          generic_temp_select_menu++;
        }
        else if (LCDML.BT_checkUp() && generic_temp_select_menu > 1)
        {
          generic_temp_select_menu--;
        }
        update_selective_values_master_effects();
      }
      if (LCDML.BT_checkEnter())
      {
        seq.edit_state = !seq.edit_state;
        if (seq.edit_state == 0 && generic_temp_select_menu == 0)
          helptext_r("SELECT PARAM");
        else if (seq.edit_state == 1 && generic_temp_select_menu != 0)
          helptext_r("EDIT VALUE");
      }
      if (seq.edit_state == 1)
      {

        if (generic_temp_select_menu == 1)
        {
          if (LCDML.BT_checkDown())
          {
            configuration.fx.delay_multiplier[0] = constrain(configuration.fx.delay_multiplier[0] + 1, 1, 4);
          }
          else if (LCDML.BT_checkUp())
          {
            configuration.fx.delay_multiplier[0] = constrain(configuration.fx.delay_multiplier[0] - 1, 1, 4);
          }
          // multiplier 1
          print_delay_multiplier(0, 1);
          if (configuration.fx.delay_sync[0] > 0)
          {
            set_delay_sync(configuration.fx.delay_sync[0], 0); // go to MIDI Sync
          }
        }

        if (generic_temp_select_menu == 2)
        { // delay time
          master_effects_set_delay_time(0);
          print_delay_time(0, 2);
          print_delay_sync_status(0);
        }
        if (generic_temp_select_menu == 3)
        {
          master_effects_set_delay_feedback(0); // feedback instance 0
          print_small_intbar(6, 8, configuration.fx.delay_feedback[0], 3, 1, 0);
        }
        else if (generic_temp_select_menu == 4)
        { // pan
          master_effects_set_delay_panorama(0);
          print_small_panbar(6, 9, configuration.fx.delay_pan[0], 4);
        }
        else if (generic_temp_select_menu == 5)
        { // level
          master_effects_delay_level_global(0);
          print_small_intbar(6, 10, configuration.fx.delay_level_global[0], 5, 1, 0);
        }

        else if (generic_temp_select_menu == 6)
        {
          if (LCDML.BT_checkDown())
          {
            configuration.fx.delay_filter_mode[0] = constrain(configuration.fx.delay_filter_mode[0] + 1, 0, 3);
          }
          else if (LCDML.BT_checkUp())
          {
            configuration.fx.delay_filter_mode[0] = constrain(configuration.fx.delay_filter_mode[0] - 1, 0, 3);
          }
          print_delay_filter_mode(0, 6);
        }

        else if (generic_temp_select_menu == 7)
        {
          if (LCDML.BT_checkDown())
          {
            configuration.fx.delay_filter_freq[0] = constrain(configuration.fx.delay_filter_freq[0] + 50, 0, 9999);
          }
          else if (LCDML.BT_checkUp())
          {
            configuration.fx.delay_filter_freq[0] = constrain(configuration.fx.delay_filter_freq[0] - 50, 0, 9999);
          }
          print_delay_filter_freq(0, 7);
        }

        else if (generic_temp_select_menu == 8)
        {
          if (LCDML.BT_checkDown())
          {
            configuration.fx.delay_level[0] = constrain(configuration.fx.delay_level[0] + ENCODER[ENC_R].speed(), DELAY_LEVEL_MIN, DELAY_LEVEL_MAX);
            MD_sendControlChange(configuration.dexed[0].midi_channel, 107, configuration.fx.delay_level[0]);
          }
          else if (LCDML.BT_checkUp())
          {
            configuration.fx.delay_level[0] = constrain(configuration.fx.delay_level[0] - ENCODER[ENC_R].speed(), DELAY_LEVEL_MIN, DELAY_LEVEL_MAX);
            MD_sendControlChange(configuration.dexed[0].midi_channel, 107, configuration.fx.delay_level[0]);
          }
          global_delay_in_mixer[0]->gain(0, mapfloat(configuration.fx.delay_level[0], DELAY_LEVEL_MIN, DELAY_LEVEL_MAX, 0.0, 0.9));
          print_small_intbar(6, 14, configuration.fx.delay_level[0], 8, 1, 0);
        }
        else if (generic_temp_select_menu == 9)
        { // epiano delay level
          if (LCDML.BT_checkDown())
          {
            configuration.fx.ep_delay_send_1 = constrain(configuration.fx.ep_delay_send_1 + ENCODER[ENC_R].speed(), DELAY_LEVEL_MIN, DELAY_LEVEL_MAX);
          }
          else if (LCDML.BT_checkUp())
          {
            configuration.fx.ep_delay_send_1 = constrain(configuration.fx.ep_delay_send_1 - ENCODER[ENC_R].speed(), DELAY_LEVEL_MIN, DELAY_LEVEL_MAX);
          }
          global_delay_in_mixer[0]->gain(5, mapfloat(configuration.fx.ep_delay_send_1, DELAY_LEVEL_MIN, DELAY_LEVEL_MAX, 0.0, 1.0));
          global_delay_in_mixer[0]->gain(6, mapfloat(configuration.fx.ep_delay_send_1, DELAY_LEVEL_MIN, DELAY_LEVEL_MAX, 0.0, 1.0));
          print_small_intbar(6, 15, configuration.fx.ep_delay_send_1, 9, 1, 0);
        }
        else if (generic_temp_select_menu == 10)
        { // microsynth 1 delay1 level
          if (LCDML.BT_checkDown())
          {
            microsynth[0].delay_send[0] = constrain(microsynth[0].delay_send[0] + ENCODER[ENC_R].speed(), 0, DELAY_LEVEL_MAX);
          }
          else if (LCDML.BT_checkUp())
          {
            microsynth[0].delay_send[0] = constrain(microsynth[0].delay_send[0] - ENCODER[ENC_R].speed(), 0, DELAY_LEVEL_MAX);
          }
          global_delay_in_mixer[0]->gain(2, mapfloat(microsynth[0].delay_send[0], DELAY_LEVEL_MIN, DELAY_LEVEL_MAX, 0.0, 1.0));
          print_small_intbar(6, 16, microsynth[0].delay_send[0], 10, 1, 0);
        }
        else if (generic_temp_select_menu == 11)
        { // microsynth 2 delay1 level
          if (LCDML.BT_checkDown())
          {
            microsynth[1].delay_send[0] = constrain(microsynth[1].delay_send[0] + ENCODER[ENC_R].speed(), 0, DELAY_LEVEL_MAX);
          }
          else if (LCDML.BT_checkUp())
          {
            microsynth[1].delay_send[0] = constrain(microsynth[1].delay_send[0] - ENCODER[ENC_R].speed(), 0, DELAY_LEVEL_MAX);
          }
          global_delay_in_mixer[0]->gain(3, mapfloat(microsynth[1].delay_send[0], DELAY_LEVEL_MIN, DELAY_LEVEL_MAX, 0.0, 1.0));
          print_small_intbar(6, 17, microsynth[1].delay_send[0], 11, 1, 0);
        }
        else if (generic_temp_select_menu == 12)
        { // braids delay level
          if (LCDML.BT_checkDown())
          {
            braids_osc.delay_send_1 = constrain(braids_osc.delay_send_1 + ENCODER[ENC_R].speed(), 0, DELAY_LEVEL_MAX);
          }
          else if (LCDML.BT_checkUp())
          {
            braids_osc.delay_send_1 = constrain(braids_osc.delay_send_1 - ENCODER[ENC_R].speed(), 0, DELAY_LEVEL_MAX);
          }
          global_delay_in_mixer[0]->gain(4, mapfloat(braids_osc.delay_send_1, DELAY_LEVEL_MIN, DELAY_LEVEL_MAX, 0.0, 1.0));
          print_small_intbar(6, 18, braids_osc.delay_send_1, 12, 1, 0);
        }
        else if (generic_temp_select_menu == 13)
        { // delay1 to delay2
          if (LCDML.BT_checkDown())
          {
            configuration.fx.delay1_to_delay2 = constrain(configuration.fx.delay1_to_delay2 + ENCODER[ENC_R].speed(), 0, DELAY_LEVEL_MAX);
          }
          else if (LCDML.BT_checkUp())
          {
            configuration.fx.delay1_to_delay2 = constrain(configuration.fx.delay1_to_delay2 - ENCODER[ENC_R].speed(), 0, DELAY_LEVEL_MAX);
          }
          global_delay_in_mixer[1]->gain(7, mapfloat(configuration.fx.delay1_to_delay2, DELAY_LEVEL_MIN, DELAY_LEVEL_MAX, 0.0, 0.9));
          print_small_intbar(6, 19, configuration.fx.delay1_to_delay2, 13, 1, 0);
        }
        else if (generic_temp_select_menu == 14)
        { // delay1 to reverb send
          if (LCDML.BT_checkDown())
          {
            configuration.fx.delay_to_reverb[0] = constrain(configuration.fx.delay_to_reverb[0] + ENCODER[ENC_R].speed(), REVERB_LEVEL_MIN, REVERB_LEVEL_MAX);
          }
          else if (LCDML.BT_checkUp())
          {
            configuration.fx.delay_to_reverb[0] = constrain(configuration.fx.delay_to_reverb[0] - ENCODER[ENC_R].speed(), REVERB_LEVEL_MIN, REVERB_LEVEL_MAX);
          }
          reverb_mixer_l.gain(REVERB_MIX_CH_AUX_DELAY1, mapfloat(configuration.fx.delay_to_reverb[0], REVERB_LEVEL_MIN, REVERB_LEVEL_MAX, 0.0, 1.0));
          reverb_mixer_r.gain(REVERB_MIX_CH_AUX_DELAY1, mapfloat(configuration.fx.delay_to_reverb[0], REVERB_LEVEL_MIN, REVERB_LEVEL_MAX, 0.0, 1.0));
          print_small_intbar(6, 20, configuration.fx.delay_to_reverb[0], 14, 1, 0);
        }
        else if (generic_temp_select_menu == 15)
        {
          if (LCDML.BT_checkDown())
          {
            configuration.fx.delay_multiplier[1] = constrain(configuration.fx.delay_multiplier[1] + 1, 1, 4);
          }
          else if (LCDML.BT_checkUp())
          {
            configuration.fx.delay_multiplier[1] = constrain(configuration.fx.delay_multiplier[1] - 1, 1, 4);
          }
          if (configuration.fx.delay_sync[1] > 0)
          {
            set_delay_sync(configuration.fx.delay_sync[1], 1); // go to MIDI Sync
          }
          print_delay_multiplier(1, 15);
        }
        else if (generic_temp_select_menu == 16)
        { // delay time
          master_effects_set_delay_time(1);
          print_delay_time(1, 16);
          print_delay_sync_status(1);
        }
        else if (generic_temp_select_menu == 17)
        {
          master_effects_set_delay_feedback(1); // feedback
          print_small_intbar(22, 8, configuration.fx.delay_feedback[1], 17, 1, 0);
        }
        else if (generic_temp_select_menu == 18)
        { // pan
          master_effects_set_delay_panorama(1);
          print_small_panbar(22, 9, configuration.fx.delay_pan[1], 18);
        }
        else if (generic_temp_select_menu == 19)
        { // level
          master_effects_delay_level_global(1);
          print_small_intbar(22, 10, configuration.fx.delay_level_global[1], 19, 1, 0);
        }

        else if (generic_temp_select_menu == 20)
        {
          if (LCDML.BT_checkDown())
          {
            configuration.fx.delay_filter_mode[1] = constrain(configuration.fx.delay_filter_mode[1] + 1, 0, 3);
          }
          else if (LCDML.BT_checkUp())
          {
            configuration.fx.delay_filter_mode[1] = constrain(configuration.fx.delay_filter_mode[1] - 1, 0, 3);
          }
          print_delay_filter_mode(1, 20);
        }

        else if (generic_temp_select_menu == 21)
        {
          if (LCDML.BT_checkDown())
          {
            configuration.fx.delay_filter_freq[1] = constrain(configuration.fx.delay_filter_freq[1] + 50, 0, 9999);
          }
          else if (LCDML.BT_checkUp())
          {
            configuration.fx.delay_filter_freq[1] = constrain(configuration.fx.delay_filter_freq[1] - 50, 0, 9999);
          }
          print_delay_filter_freq(1, 21);
        }

        else if (generic_temp_select_menu == 22)
        { // dx2 delay send
          if (LCDML.BT_checkDown())
          {
            configuration.fx.delay_level[1] = constrain(configuration.fx.delay_level[1] + ENCODER[ENC_R].speed(), DELAY_LEVEL_MIN, DELAY_LEVEL_MAX);
            MD_sendControlChange(configuration.dexed[1].midi_channel, 107, configuration.fx.delay_level[1]);
          }
          else if (LCDML.BT_checkUp())
          {
            configuration.fx.delay_level[1] = constrain(configuration.fx.delay_level[1] - ENCODER[ENC_R].speed(), DELAY_LEVEL_MIN, DELAY_LEVEL_MAX);
            MD_sendControlChange(configuration.dexed[1].midi_channel, 107, configuration.fx.delay_level[1]);
          }
          global_delay_in_mixer[1]->gain(0, mapfloat(configuration.fx.delay_level[1], DELAY_LEVEL_MIN, DELAY_LEVEL_MAX, 0.0, 0.9));
          print_small_intbar(22, 14, configuration.fx.delay_level[1], 22, 1, 0);
        }
        else if (generic_temp_select_menu == 23)
        { // ep send
          if (LCDML.BT_checkDown())
          {
            configuration.fx.ep_delay_send_2 = constrain(configuration.fx.ep_delay_send_2 + ENCODER[ENC_R].speed(), DELAY_LEVEL_MIN, DELAY_LEVEL_MAX);
          }
          else if (LCDML.BT_checkUp())
          {
            configuration.fx.ep_delay_send_2 = constrain(configuration.fx.ep_delay_send_2 - ENCODER[ENC_R].speed(), DELAY_LEVEL_MIN, DELAY_LEVEL_MAX);
          }
          global_delay_in_mixer[1]->gain(5, mapfloat(configuration.fx.ep_delay_send_2, DELAY_LEVEL_MIN, DELAY_LEVEL_MAX, 0.0, 1.0));
          global_delay_in_mixer[1]->gain(6, mapfloat(configuration.fx.ep_delay_send_2, DELAY_LEVEL_MIN, DELAY_LEVEL_MAX, 0.0, 1.0));
          print_small_intbar(22, 15, configuration.fx.ep_delay_send_2, 23, 1, 0);
        }
        else if (generic_temp_select_menu == 24)
        { // microsynth 1 delay2 level
          if (LCDML.BT_checkDown())
          {
            microsynth[0].delay_send[1] = constrain(microsynth[0].delay_send[1] + ENCODER[ENC_R].speed(), DELAY_TIME_MIN, DELAY_LEVEL_MAX);
          }
          else if (LCDML.BT_checkUp())
          {
            microsynth[0].delay_send[1] = constrain(microsynth[0].delay_send[1] - ENCODER[ENC_R].speed(), DELAY_TIME_MIN, DELAY_LEVEL_MAX);
          }
          global_delay_in_mixer[1]->gain(2, mapfloat(microsynth[0].delay_send[1], DELAY_LEVEL_MIN, DELAY_LEVEL_MAX, 0.0, 1.0));
          print_small_intbar(22, 16, microsynth[0].delay_send[1], 24, 1, 0);
        }
        else if (generic_temp_select_menu == 25)
        { // microsynth 2 delay2 level
          if (LCDML.BT_checkDown())
          {
            microsynth[1].delay_send[1] = constrain(microsynth[1].delay_send[1] + ENCODER[ENC_R].speed(), DELAY_TIME_MIN, DELAY_LEVEL_MAX);
          }
          else if (LCDML.BT_checkUp())
          {
            microsynth[1].delay_send[1] = constrain(microsynth[1].delay_send[1] - ENCODER[ENC_R].speed(), DELAY_TIME_MIN, DELAY_LEVEL_MAX);
          }
          global_delay_in_mixer[1]->gain(3, mapfloat(microsynth[1].delay_send[1], DELAY_LEVEL_MIN, DELAY_LEVEL_MAX, 0.0, 1.0));
          print_small_intbar(22, 17, microsynth[1].delay_send[1], 25, 1, 0);
        }
        else if (generic_temp_select_menu == 26)
        { // braids delay2 level
          if (LCDML.BT_checkDown())
          {
            braids_osc.delay_send_2 = constrain(braids_osc.delay_send_2 + ENCODER[ENC_R].speed(), 0, DELAY_LEVEL_MAX);
          }
          else if (LCDML.BT_checkUp())
          {
            braids_osc.delay_send_2 = constrain(braids_osc.delay_send_2 - ENCODER[ENC_R].speed(), 0, DELAY_LEVEL_MAX);
          }
          global_delay_in_mixer[1]->gain(4, mapfloat(braids_osc.delay_send_2, DELAY_LEVEL_MIN, DELAY_LEVEL_MAX, 0.0, 1.0));
          print_small_intbar(22, 18, braids_osc.delay_send_2, 26, 1, 0);
        }
        else if (generic_temp_select_menu == 27)
        { // delay2 to delay1
          if (LCDML.BT_checkDown())
          {
            configuration.fx.delay2_to_delay1 = constrain(configuration.fx.delay2_to_delay1 + ENCODER[ENC_R].speed(), 0, DELAY_LEVEL_MAX);
          }
          else if (LCDML.BT_checkUp())
          {
            configuration.fx.delay2_to_delay1 = constrain(configuration.fx.delay2_to_delay1 - ENCODER[ENC_R].speed(), 0, DELAY_LEVEL_MAX);
          }
          global_delay_in_mixer[0]->gain(7, mapfloat(configuration.fx.delay2_to_delay1, DELAY_LEVEL_MIN, DELAY_LEVEL_MAX, 0.0, 0.9));
          print_small_intbar(22, 19, configuration.fx.delay2_to_delay1, 27, 1, 0);
        }
        else if (generic_temp_select_menu == 28)
        { // delay2 to reverb
          if (LCDML.BT_checkDown())
          {
            configuration.fx.delay_to_reverb[1] = constrain(configuration.fx.delay_to_reverb[1] + ENCODER[ENC_R].speed(), REVERB_LEVEL_MIN, REVERB_LEVEL_MAX);
          }
          else if (LCDML.BT_checkUp())
          {
            configuration.fx.delay_to_reverb[1] = constrain(configuration.fx.delay_to_reverb[1] - ENCODER[ENC_R].speed(), REVERB_LEVEL_MIN, REVERB_LEVEL_MAX);
          }
          reverb_mixer_l.gain(REVERB_MIX_CH_AUX_DELAY2, mapfloat(configuration.fx.delay_to_reverb[1], REVERB_LEVEL_MIN, REVERB_LEVEL_MAX, 0.0, 1.0));
          reverb_mixer_r.gain(REVERB_MIX_CH_AUX_DELAY2, mapfloat(configuration.fx.delay_to_reverb[1], REVERB_LEVEL_MIN, REVERB_LEVEL_MAX, 0.0, 1.0));
          print_small_intbar(22, 20, configuration.fx.delay_to_reverb[1], 28, 1, 0);
        }

        else if (generic_temp_select_menu == 29)
        { // reverb room size
          reverb_roomsize();
          print_small_intbar(43, 5, configuration.fx.reverb_roomsize, 29, 1, 0);
        }
        else if (generic_temp_select_menu == 30)
        {
          reverb_lowpass();
          print_small_intbar(43, 6, configuration.fx.reverb_lowpass, 30, 1, 0);
        }
        else if (generic_temp_select_menu == 31)
        {
          reverb_lodamp();
          print_small_intbar(43, 7, configuration.fx.reverb_lodamp, 31, 1, 0);
        }
        else if (generic_temp_select_menu == 32)
        {
          reverb_hidamp();
          print_small_intbar(43, 8, configuration.fx.reverb_hidamp, 32, 1, 0);
        }
        else if (generic_temp_select_menu == 33)
        {
          reverb_diffusion();
          print_small_intbar(43, 9, configuration.fx.reverb_diffusion, 33, 1, 0);
        }
        else if (generic_temp_select_menu == 34)
        {
          reverb_level();
          print_small_intbar(43, 10, configuration.fx.reverb_level, 34, 1, 0);
        }
        else if (generic_temp_select_menu == 35)
        {
          master_effects_set_reverb_send(0);
          print_small_intbar(43, 14, configuration.fx.reverb_send[0], 35, 1, 0);
        }
        else if (generic_temp_select_menu == 36)
        {
          master_effects_set_reverb_send(1);
          print_small_intbar(43, 15, configuration.fx.reverb_send[1], 36, 1, 0);
        }
        else if (generic_temp_select_menu == 37)
        { // epiano reverb send
          if (LCDML.BT_checkDown())
          {
            configuration.fx.ep_reverb_send = constrain(configuration.fx.ep_reverb_send + ENCODER[ENC_R].speed(), REVERB_SEND_MIN, REVERB_SEND_MAX);
            MD_sendControlChange(configuration.epiano.midi_channel, 91, configuration.fx.ep_reverb_send);
          }
          else if (LCDML.BT_checkUp())
          {
            configuration.fx.ep_reverb_send = constrain(configuration.fx.ep_reverb_send - ENCODER[ENC_R].speed(), REVERB_SEND_MIN, REVERB_SEND_MAX);
            MD_sendControlChange(configuration.epiano.midi_channel, 91, configuration.fx.ep_reverb_send);
          }
          reverb_mixer_r.gain(REVERB_MIX_CH_EPIANO, mapfloat(configuration.fx.ep_reverb_send, REVERB_SEND_MIN, REVERB_SEND_MAX, 0.0, 1.0)); // EPiano Reverb-Send
          reverb_mixer_l.gain(REVERB_MIX_CH_EPIANO, mapfloat(configuration.fx.ep_reverb_send, REVERB_SEND_MIN, REVERB_SEND_MAX, 0.0, 1.0)); // EPiano Reverb-Send
          print_small_intbar(43, 16, configuration.fx.ep_reverb_send, 37, 1, 0);
        }
        else if (generic_temp_select_menu == 38)
        {
          if (LCDML.BT_checkDown())
          {
            microsynth[0].rev_send = constrain(microsynth[0].rev_send + ENCODER[ENC_R].speed(), REVERB_SEND_MIN, REVERB_SEND_MAX);
          }
          else if (LCDML.BT_checkUp())
          {
            microsynth[0].rev_send = constrain(microsynth[0].rev_send - ENCODER[ENC_R].speed(), REVERB_SEND_MIN, REVERB_SEND_MAX);
          }
          microsynth_mixer_reverb.gain(0, volume_transform(mapfloat(microsynth[0].rev_send, REVERB_SEND_MIN, REVERB_SEND_MAX, 0.0, VOL_MAX_FLOAT)));
          print_small_intbar(43, 17, microsynth[0].rev_send, 38, 1, 0);
        }
        else if (generic_temp_select_menu == 39)
        {
          if (LCDML.BT_checkDown())
          {
            microsynth[1].rev_send = constrain(microsynth[1].rev_send + ENCODER[ENC_R].speed(), REVERB_SEND_MIN, REVERB_SEND_MAX);
          }
          else if (LCDML.BT_checkUp())
          {
            microsynth[1].rev_send = constrain(microsynth[1].rev_send - ENCODER[ENC_R].speed(), REVERB_SEND_MIN, REVERB_SEND_MAX);
          }
          microsynth_mixer_reverb.gain(1, volume_transform(mapfloat(microsynth[1].rev_send, REVERB_SEND_MIN, REVERB_SEND_MAX, 0.0, VOL_MAX_FLOAT)));
          print_small_intbar(43, 18, microsynth[1].rev_send, 39, 1, 0);
        }
        else if (generic_temp_select_menu == 40)
        {
          if (LCDML.BT_checkDown())
          {
            braids_osc.rev_send = constrain(braids_osc.rev_send + ENCODER[ENC_R].speed(), REVERB_SEND_MIN, REVERB_SEND_MAX);
          }
          else if (LCDML.BT_checkUp())
          {
            braids_osc.rev_send = constrain(braids_osc.rev_send - ENCODER[ENC_R].speed(), REVERB_SEND_MIN, REVERB_SEND_MAX);
          }
          braids_mixer_reverb.gain(0, volume_transform(mapfloat(braids_osc.rev_send, REVERB_SEND_MIN, REVERB_SEND_MAX, 0.0, VOL_MAX_FLOAT)));
          braids_mixer_reverb.gain(1, volume_transform(mapfloat(braids_osc.rev_send, REVERB_SEND_MIN, REVERB_SEND_MAX, 0.0, VOL_MAX_FLOAT)));
          print_small_intbar(43, 19, braids_osc.rev_send, 40, 1, 0);
        }
      }
    }
  }
  if (LCDML.FUNC_close()) // ****** STABLE END *********
  {
    encoderDir[ENC_R].reset();
    display.fillScreen(COLOR_BACKGROUND);
  }
}

void sysinfo_reload_prev_voice()
{
  if (sysinfo_sound_state > 0)
  {
    if (seq.running == false)
    {
      MicroDexed[0]->keyup(MIDI_E4);
      MicroDexed[0]->keyup(MIDI_G3);
      MicroDexed[0]->keyup(MIDI_AIS5);
      MicroDexed[0]->keyup(MIDI_D5);
      MicroDexed[0]->keyup(MIDI_D4);
      MicroDexed[0]->keyup(MIDI_F4);

      // reload current(previous active) dexed0 patch
      MicroDexed[0]->setGain(midi_volume_transform(map(configuration.dexed[0].sound_intensity, SOUND_INTENSITY_MIN, SOUND_INTENSITY_MAX, 0, 127)));
      load_sd_voice(sysinfo_old_pool, sysinfo_old_bank, sysinfo_old_voice, 0);
      configuration.dexed[0].transpose = sysinfo_old_transpose;
      MicroDexed[0]->setTranspose(configuration.dexed[0].transpose);
    }
    sysinfo_sound_state = 0;
  }
}

int liveseq_listeditor_steps = 0;
uint8_t liveseq_listeditor_filter = 0;
int liveseq_listeditor_pattern_start[5];

FLASHMEM void get_liveseq_listeditor_pattern_starts() {
  LiveSequencer::LiveSeqData* data = liveSeq.getData();
  int p = 1;
  int i = 0;

  for (auto& e : data->eventsList)
  {
    if (e.patternNumber == p)
    {
      liveseq_listeditor_pattern_start[p] = i + 1;
      p++;
    }
    i++;
  }
}

FLASHMEM void print_liveseq_listeditor_update_steps() {
  LiveSequencer::LiveSeqData* data = liveSeq.getData();
  if (int(data->eventsList.size()) - 1 >= 0)
    liveseq_listeditor_steps = int(data->eventsList.size()) - 1;
  display.setTextSize(1);

  display.setTextColor(GREY2, COLOR_BACKGROUND);
  display.setCursor(CHAR_width_small * 20, 0);
  display.print("P1:");
  display.print(liveseq_listeditor_pattern_start[0]);
  display.print(" P2:");
  display.print(liveseq_listeditor_pattern_start[1]);
  display.print(" P3:");
  display.print(liveseq_listeditor_pattern_start[2]);
  display.print(" P4:");
  display.print(liveseq_listeditor_pattern_start[3]);

  display.setCursor(CHAR_width_small * 46, 0);
  display.setTextColor(GREEN, COLOR_BACKGROUND);
  print_formatted_number(temp_int + generic_temp_select_menu + 1, 3);

  display.setTextColor(GREEN, COLOR_BACKGROUND);
  display.setCursor(CHAR_width_small * 50, 0);
  print_formatted_number(liveseq_listeditor_steps + 1, 3);


}

FLASHMEM void liveseq_listeditor_printEventGridLine(int i, LiveSequencer::MidiEvent e) {

  char displayname[4] = { 0, 0, 0, 0 };

  display.setCursor((CHAR_width_small + e.track * (CHAR_width_small * 9)), CHAR_height_small * 4 + ((CHAR_height_small + 1) * i));

  getNoteName(displayname, e.note_in);

  if (i + temp_int == liveseq_edit_noteoff_step && e.event != midi::InvalidType && e.event == midi::NoteOff)
    display.setTextColor(COLOR_BACKGROUND, COLOR_PITCHSMP);

  else  if (i == generic_temp_select_menu && e.event != midi::InvalidType && e.event == midi::NoteOn)
    display.setTextColor(COLOR_SYSTEXT, RED);

  else if (i == generic_temp_select_menu && e.event != midi::InvalidType && e.event == midi::NoteOff)
    display.setTextColor(DARKGREEN, RED);
  else if (e.event != midi::InvalidType && e.event == midi::NoteOff)
    display.setTextColor(DARKGREEN, GREY3);
  else
    display.setTextColor(COLOR_SYSTEXT, GREY3);

  display.print(displayname);

  //Velocity
  display.setCursor(CHAR_width_small * 5 + (e.track * (CHAR_width_small * 9)), CHAR_height_small * 4 + ((CHAR_height_small + 1) * i));

  if (i == generic_temp_select_menu && e.event != midi::InvalidType && e.event == midi::NoteOn)
    display.setTextColor(COLOR_SYSTEXT, RED);

  else if (i != generic_temp_select_menu && e.event != midi::InvalidType && e.event == midi::NoteOn)
    display.setTextColor(COLOR_SYSTEXT, GREY3);

  else  if (i + temp_int == liveseq_edit_noteoff_step && e.event != midi::InvalidType && e.event == midi::NoteOff)
    display.setTextColor(COLOR_BACKGROUND, COLOR_PITCHSMP);

  else if (i == generic_temp_select_menu && e.event != midi::InvalidType && e.event == midi::NoteOff)
    display.setTextColor(DARKGREEN, RED);

  else
    display.setTextColor(DARKGREEN, GREY3);

  if (e.note_in_velocity > 0)
    print_formatted_number(e.note_in_velocity, 3);
  else
    display.print(F("OFF"));

}

uint8_t liveseq_listeditor_state = 0;

FLASHMEM void liveseq_listeditor_printDetailedEvent(int i, LiveSequencer::MidiEvent e) {

  display.setTextSize(2);
  char displayname[4] = { 0, 0, 0, 0 };

  display.setTextColor(COLOR_SYSTEXT, COLOR_BACKGROUND);

  //track
  display.setCursor((CHAR_width_small * 1), CHAR_height_small * 23);
  display.print(e.track + 1);

  //layer
  display.setCursor((CHAR_width_small * 8), CHAR_height_small * 23);
  display.print(e.layer + 1);

  //patternNumber
  display.setTextColor(GREY1, COLOR_BACKGROUND);
  display.setCursor((CHAR_width_small * 15), CHAR_height_small * 23);
  display.print(e.patternNumber + 1);

  //note

  display.setCursor(CHAR_width_small * 24, CHAR_height_small * 23);
  getNoteName(displayname, e.note_in);
  if (liveseq_listeditor_state == 1)
    display.setTextColor(COLOR_SYSTEXT, RED);
  else
    display.setTextColor(COLOR_SYSTEXT, COLOR_BACKGROUND);

  display.print(displayname);

  //velocity
  display.setCursor(CHAR_width_small * 34, CHAR_height_small * 23);

  if (liveseq_listeditor_state == 2)
    display.setTextColor(COLOR_SYSTEXT, RED);
  else
    display.setTextColor(GREY1, COLOR_BACKGROUND);
  if (e.note_in_velocity > 0) {
    print_formatted_number(e.note_in_velocity, 3);
  }
  else {
    display.print("OFF");
  }
  //patternMs

  if (liveseq_listeditor_state == 3)
    display.setTextColor(COLOR_SYSTEXT, RED);
  else
    display.setTextColor(COLOR_SYSTEXT, COLOR_BACKGROUND);
  display.setCursor((CHAR_width_small * 44), CHAR_height_small * 23);
  print_formatted_number(e.patternMs, 4);

  display.setTextColor(COLOR_SYSTEXT, COLOR_BACKGROUND);
  display.setTextSize(1);
}

void liveseq_listeditor_printEventGrid();

FLASHMEM void liveseq_listeditor_edit_element()
{
  liveseq_listeditor_state++;
  if (liveseq_listeditor_state > 3)
    liveseq_listeditor_state = 0;

  if (liveseq_listeditor_state == 0)
  {
    helptext_r("MOVE Y");
    draw_button_on_grid(13, 26, "EDIT", "STEP", 1);
  }
  else if (liveseq_listeditor_state == 1)
  {
    helptext_r("EDIT NOTE");
    draw_button_on_grid(13, 26, "EDIT", "NEXT->", 2);
  }
  else if (liveseq_listeditor_state == 2)
  {
    helptext_r("EDIT VEL");
  }
  else if (liveseq_listeditor_state == 3)
  {
    helptext_r("EDIT MS");
    draw_button_on_grid(13, 26, "EXIT", "EDIT", 2);
  }
  liveseq_listeditor_printEventGrid();
}

int scrollbuffer_liveseq_listeditor = 999;

FLASHMEM void liveseq_listeditor_printEventGrid()
{
  uint8_t search_step = 0;

  uint8_t search_note = 0;
  uint8_t search_track = 0;
  LiveSequencer::LiveSeqData* data = liveSeq.getData();

  if (generic_temp_select_menu == 0 || generic_temp_select_menu == 13) {
    display.console = true;
    display.fillRect(CHAR_width_small, 30, 319 - CHAR_width_small, 130, GREY3);
  }
  int i = 0;

  for (auto& e : data->eventsList) //find matching note off to current selected note
  {
    if (temp_int <= i && i < 14 + temp_int)
    {
      if (temp_int + generic_temp_select_menu == i)
      {
        if (search_step == 0)
        {
          search_note = e.note_in;
          search_track = e.track;
          search_step = 1;
          if (e.event == midi::NoteOn)
          {
            draw_button_on_grid(24, 26, "DEL", "STEP", 1);
            seq.edit_state = true;
          }
          else
          {
            draw_button_on_grid(24, 26, "DEL", "STEP", 0);
            seq.edit_state = false;
          }
        }
      }
    }

    if (search_step == 1 && e.event == midi::NoteOff && e.note_in == search_note && e.track == search_track)
    {
      liveseq_edit_noteoff_step = i;
      search_step = 2;
    }
    i++;
  }

  i = 0;
  for (auto& e : data->eventsList)  //display all rows with current on + off note
  {
    if (temp_int <= i && i < 14 + temp_int)
    {
      liveseq_listeditor_printEventGridLine(i - temp_int, e);
      if (temp_int + generic_temp_select_menu == i)
      {
        liveseq_listeditor_printDetailedEvent(temp_int, e);
      }
    }
    i++;
  }
}

FLASHMEM void liveseq_listeditor_delete_element()
{
  LiveSequencer::LiveSeqData* data = liveSeq.getData();
  int i = 0;
  bool note_valid = false;
  for (auto& e : data->eventsList)
  {
    if (i == temp_int + generic_temp_select_menu && e.event == midi::NoteOn) {
      //mark for deleting note on event
      e.note_in = 0;
      e.note_in_velocity = 0;
      e.event = midi::InvalidType;
      note_valid = true;
    }

    if (i == liveseq_edit_noteoff_step && e.event == midi::NoteOff && note_valid) {
      //mark for deleting note off event
      e.note_in = 0;
      e.note_in_velocity = 0;
      e.event = midi::InvalidType;
    }
    i++;
  }
  liveSeq.cleanEvents();
  display.console = true;
  display.fillRect(CHAR_width_small, 30, 319 - CHAR_width_small, 130, GREY3);
  get_liveseq_listeditor_pattern_starts();
  print_liveseq_listeditor_update_steps();
  liveseq_listeditor_printEventGrid();

}

FLASHMEM void liveseq_listeditor_get_current_values()
{
  LiveSequencer::LiveSeqData* data = liveSeq.getData();
  int i = 0;
  for (auto& e : data->eventsList)
  {
    if (i == temp_int + generic_temp_select_menu) {

      if (liveseq_listeditor_state == 1) //note
        temp_uint = e.note_in;
      else if (liveseq_listeditor_state == 2) //vel
        temp_uint = e.note_in_velocity;
      else if (liveseq_listeditor_state == 3) //ms
        temp_int16 = e.patternMs;
    }
    i++;
  }
}

FLASHMEM void liveseq_listeditor_save_changed_element()
{
  LiveSequencer::LiveSeqData* data = liveSeq.getData();
  int i = 0;
  for (auto& e : data->eventsList)
  {
    if (i == temp_int + generic_temp_select_menu && e.event == midi::NoteOn) {

      if (liveseq_listeditor_state == 1) //note
        e.note_in = temp_uint;
      else if (liveseq_listeditor_state == 2) //vel
        e.note_in_velocity = temp_uint;
      else if (liveseq_listeditor_state == 3) //ms
        e.patternMs = temp_int16;
    }
    if (i == liveseq_edit_noteoff_step && e.event == midi::NoteOff) {
      if (liveseq_listeditor_state == 1) //note 
        e.note_in = temp_uint; //change note off according to modfied note on
    }
    i++;
  }
  liveseq_listeditor_printEventGrid();
}


FLASHMEM void print_liveseq_listeditor_filter() {
  char buf[4];
  if (liveseq_listeditor_filter == 3)
    draw_button_on_grid(36, 26, "JUMP", "1", 1);
  else
    draw_button_on_grid(36, 26, "JUMP", itoa(liveseq_listeditor_filter + 2, buf, 10), 1);
  display.console = true;
  display.fillRect(CHAR_width_small, 30, 319 - CHAR_width_small, 130, GREY3);
}

FLASHMEM void UI_func_liveseq_listeditor(uint8_t param)
{

  if (LCDML.FUNC_setup()) // ****** SETUP *********
  {
    temp_int = 0;
    get_liveseq_listeditor_pattern_starts();
    helptext_r("");  //required
    generic_temp_select_menu = 6;
    scrollbuffer_liveseq_listeditor = 999;

    encoderDir[ENC_R].reset();
    display.fillScreen(COLOR_BACKGROUND);
    display.console = true;
    display.fillRect(CHAR_width_small, 30, 319 - CHAR_width_small, 130, GREY3);
    display.setTextColor(RED, COLOR_BACKGROUND);
    display.setCursor(CHAR_width_small, CHAR_height_small * 0);

    display.setTextSize(1);
    display.print("LIVESEQUENCER EDIT");

    display.setCursor(CHAR_width_small, CHAR_height_small * 21);
    display.setTextColor(GREY1, COLOR_BACKGROUND);
    display.print(F("TRACK  LAYER  PAT.NO.  NOTE      VEL.      PAT.MS"));

    draw_button_on_grid(1, 26, "GO", back_text, 1);
    if (liveseq_listeditor_state == 0)
      draw_button_on_grid(13, 26, "EDIT", "STEP", 1);
    else
      draw_button_on_grid(13, 26, "EDIT", "STEP", 2);

    draw_button_on_grid(24, 26, "DEL", "STEP", 1);
    print_liveseq_listeditor_filter();

    helptext_r("MOVE Y");

    display.setTextColor(GREEN, COLOR_BACKGROUND);
    display.setCursor(CHAR_width_small * 49, 0);
    display.print("/");

    display.setCursor(CHAR_width_small * 4 - 1, CHAR_height_small * 19);

    for (int x = 0; x < 6; x++)
    {
      display.setCursor((CHAR_width_small + x * (CHAR_width_small * 9)), CHAR_height_small * 2);
      display.setTextColor(GREY1, COLOR_BACKGROUND);
      display.print("TRACK ");

      display.setTextColor(COLOR_SYSTEXT, COLOR_BACKGROUND);
      display.print(x + 1);
    }
  }

  if (LCDML.FUNC_loop()) // ****** LOOP *********
  {
    if ((LCDML.BT_checkDown() && encoderDir[ENC_R].Down()) || (LCDML.BT_checkUp() && encoderDir[ENC_R].Up()) || (LCDML.BT_checkEnter() && encoderDir[ENC_R].ButtonShort()))
    {
      if (liveseq_listeditor_state == 0)
      {
        if (LCDML.BT_checkDown())
        {
          if (generic_temp_select_menu < 13)
            generic_temp_select_menu = generic_temp_select_menu + 1;
          else
            temp_int = constrain(temp_int + 1, 0, liveseq_listeditor_steps - 13);
        }
        else if (LCDML.BT_checkUp())
        {

          if (generic_temp_select_menu > 0)
            generic_temp_select_menu = generic_temp_select_menu - 1;

          else
            temp_int = constrain(temp_int - 1, 0, liveseq_listeditor_steps);
        }
      }
      else
      {  //edit mode is on

        liveseq_listeditor_get_current_values();

        if (liveseq_listeditor_state == 1 || liveseq_listeditor_state == 2)
        {

          if (LCDML.BT_checkDown())
          {
            temp_uint = constrain(temp_uint + 1, 0, 127);
          }
          else if (LCDML.BT_checkUp())
          {
            temp_uint = constrain(temp_uint - 1, 0, 127);
          }
        }
        else if (liveseq_listeditor_state == 3)  //16 bit
        {
          if (LCDML.BT_checkDown())
          {
            temp_int16 = constrain(temp_int16 + ENCODER[ENC_R].speed(), 0, 6000);
          }
          else if (LCDML.BT_checkUp())
          {
            temp_int16 = constrain(temp_int16 - ENCODER[ENC_R].speed(), 0, 6000);
          }
        }

      }
    }
    if (LCDML.BT_checkEnter()) // handle button presses during menu >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
    {

      liveseq_listeditor_edit_element();
      // liveseq_listeditor_save_changed_element();
      // liveseq_listeditor_state = 0;
      // draw_button_on_grid(13, 26, "EDIT", "STEP", 1);
      // liveseq_listeditor_printEventGrid();
      // helptext_r("MOVE Y");
    }

    if (liveseq_listeditor_state != 0)
    {
      liveseq_listeditor_save_changed_element();
    }
    print_liveseq_listeditor_update_steps();

    if (scrollbuffer_liveseq_listeditor != temp_int + generic_temp_select_menu)
    {
      liveseq_listeditor_printEventGrid();
      scrollbuffer_liveseq_listeditor = temp_int + generic_temp_select_menu;
    }
  }
  if (LCDML.FUNC_close()) // ****** STABLE END *********
  {
    encoderDir[ENC_R].reset();
    display.fillScreen(COLOR_BACKGROUND);
  }
}

void UI_func_information(uint8_t param)
{

  if (LCDML.FUNC_setup()) // ****** SETUP *********
  {
    sysinfo_chord_state = 0;
    char version_string[display_cols + 10 + 1];
    encoderDir[ENC_R].reset();
    display.fillScreen(COLOR_BACKGROUND);
    generate_version_string(version_string, sizeof(version_string));
    display.setCursor(CHAR_width_small * 4 - 1, CHAR_height_small * 19);
    display.setTextSize(1);
    display.print(version_string);
    display.setCursor(CHAR_width_small * 4 - 1, CHAR_height_small * 21);
    display.print(sd_string);
    display.setCursor(CHAR_width_small * 4 - 1, CHAR_height_small * 23);
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
    display.setTextColor(GREY1);
    display.setCursor(CHAR_width_small * 34 - 2, CHAR_height_small * 25);
    display.print(F("CPU"));
    display.setCursor(CHAR_width_small * 41 - 2, CHAR_height_small * 25);
    display.print(F("%"));
    display.setCursor(CHAR_width_small * 43 - 2, CHAR_height_small * 25);
    display.print(F("TEMP"));
    display.setCursor(CHAR_width_small * 51 - 2, CHAR_height_small * 25);
    display.print(F("C"));
    display.setCursor(CHAR_width_small * 4 - 2, CHAR_height_small * 25);
    display.setTextColor(COLOR_BACKGROUND, GREY2);
#ifdef DEBUG
    display.setTextColor(COLOR_SYSTEXT, RED);
#endif
    display.print(F("DEBUG"));
    display.setTextColor(GREY1, COLOR_BACKGROUND);
    display.print(F(" "));
    display.setTextColor(COLOR_BACKGROUND, GREY2);
    if (remote_active)
      display.setTextColor(COLOR_SYSTEXT, RED);
    display.print(F("REMOTE_CON"));
    display.setTextColor(GREY1, COLOR_BACKGROUND);
    display.print(F(" "));

    display.setTextColor(COLOR_BACKGROUND, GREY2);
    if (external_psram_size != 0)
      display.setTextColor(COLOR_BACKGROUND, COLOR_SYSTEXT);
    else
      display.setTextColor(COLOR_BACKGROUND, GREY2);
    display.print(F("PSRAM:"));
    display.print(F(" "));
    if (external_psram_size != 0)
    {
      display.print(external_psram_size);
      display.print(" MB");
    }
    else
    {
      display.print("NONE");
    }

    display.setCursor(CHAR_width_small * 4 - 2, CHAR_height_small * 27);
    display.setTextColor(COLOR_BACKGROUND, GREY2);
#ifdef I2S_AUDIO_ONLY
    display.setTextColor(COLOR_SYSTEXT, GREY2);
#endif
    display.print(F("I2S"));
    display.setTextColor(GREY1, COLOR_BACKGROUND);
    display.print(F(" "));
    display.setTextColor(COLOR_BACKGROUND, GREY2);

#ifdef TEENSY_AUDIO_BOARD
    display.setTextColor(COLOR_SYSTEXT, GREY2);
#endif
    display.print(F("T_AUDIO"));
    display.setTextColor(GREY1, COLOR_BACKGROUND);
    display.print(F(" "));
    display.setTextColor(COLOR_BACKGROUND, GREY2);

#ifdef MIDI_DEVICE_DIN
    display.setTextColor(COLOR_SYSTEXT, GREY2);
#endif
    display.print(F("MIDI DIN"));
    display.setTextColor(GREY1, COLOR_BACKGROUND);
    display.print(F(" "));
    display.setTextColor(COLOR_BACKGROUND, GREY2);

#ifdef MIDI_DEVICE_USB
    display.setTextColor(COLOR_SYSTEXT, GREY2);
#endif
    display.print(F("MIDI USB"));
    display.setTextColor(GREY1, COLOR_BACKGROUND);
    display.print(F(" "));
    display.setTextColor(COLOR_BACKGROUND, GREY2);

#ifdef MIDI_DEVICE_USB_HOST
    display.setTextColor(COLOR_SYSTEXT, GREY2);
#endif
    display.print(F("USB HOST"));
    display.setTextColor(GREY1, COLOR_BACKGROUND);
    display.print(F(" "));
    display.setTextColor(COLOR_BACKGROUND, GREY2);

#ifdef AUDIO_DEVICE_USB
    display.setTextColor(COLOR_SYSTEXT, GREY2);
#endif
    display.print(F("AUDIO USB"));
    display.setTextColor(GREY1, COLOR_BACKGROUND);
    display.print(F(" "));
    display.setTextColor(COLOR_BACKGROUND, GREY2);

    /////////////

    /////SPDIF
    display.setTextColor(GREY1);
    display.setCursor(CHAR_width_small * 8 - 2, CHAR_height_small * 29);
    display.print(F("SPDIF IN:"));
    display.setCursor(CHAR_width_small * 21 - 2, CHAR_height_small * 29);
    display.print(F("FREQ:"));


    /// CAPACITIVE TOUCH STATUS
#ifdef CAPACITIVE_TOUCH_DISPLAY
    display.setCursor(CHAR_width_small * 34 - 2, CHAR_height_small * 29);
    if (touch_ic_found)
      display.setTextColor(COLOR_BACKGROUND, GREEN);
    else
      display.setTextColor(COLOR_BACKGROUND, RED);
    display.print(F("TOUCH"));
    display.setTextColor(GREY1);
#endif

    ///////////////
    if (sysinfo_sound_state == 0)
    {
      if (seq.running == false)
      {
        sysinfo_old_pool = configuration.dexed[0].pool;
        sysinfo_old_bank = configuration.dexed[0].bank;
        sysinfo_old_voice = configuration.dexed[0].voice;
        sysinfo_old_transpose = configuration.dexed[0].transpose;
        load_sd_voice(0, 1, 21, 0);
        MicroDexed[0]->setGain(0.9);
        MicroDexed[0]->keydown(MIDI_G3, 40);
        sysinfo_chord_state = 1;
      }
      randomSeed(analogRead(0));
      if (random(2) == 0)
      {
        sysinfo_logo_version = 2;
        sysinfo_sound_state = 10;
        splash_screen2();
      }
      else
      {
        sysinfo_logo_version = 1;
        sysinfo_sound_state = 10;
        splash_screen1();
      }

      sysinfo_millis = 0;
    }
  }
  if (LCDML.FUNC_loop()) // ****** LOOP *********
  {
    if (LCDML.BT_checkEnter()) // handle button presses during menu >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
    {
      ;
    }
  }
  if (LCDML.FUNC_close()) // ****** STABLE END *********
  {
    sysinfo_reload_prev_voice();
    encoderDir[ENC_R].reset();
    display.fillScreen(COLOR_BACKGROUND);
  }
}

// void UI_func_midi_sync(uint8_t param)
// {

//   if (LCDML.FUNC_setup()) // ****** SETUP *********
//   {

//     encoderDir[ENC_R].reset();
//     display.fillScreen(COLOR_BACKGROUND);

//     display.setCursor(CHAR_width_small * 2, CHAR_height_small * 2);
//     display.setTextSize(1);
//     display.setTextColor(RED);
//     display.print(F("MIDI SYNC"));
//     display.setTextColor(COLOR_SYSTEXT, COLOR_BACKGROUND);

//     display.setCursor(CHAR_width_small * 2, CHAR_height_small * 4);
//     display.print(F("MIDI CLOCK (in 16th steps)"));
//     display.setTextColor(GREY1, COLOR_BACKGROUND);

//     for (int x = 0; x < 16; x++)
//     {
//       display.setCursor(CHAR_width_small * 2 + (x * 3) * CHAR_width_small, CHAR_height_small * 5);
//       print_formatted_number(x + 1, 2);
//     }

//     display.setCursor(CHAR_width_small * 2, CHAR_height_small * 8);
//     display.print(F("MDT SEQUENCER (in 16th steps)"));
//     display.setTextColor(GREY1, COLOR_BACKGROUND);

//     for (int x = 0; x < 16; x++)
//     {
//       display.setCursor(CHAR_width_small * 2 + (x * 3) * CHAR_width_small, CHAR_height_small * 9);
//       print_formatted_number(x + 1, 2);
//     }

//   }
//   if (LCDML.FUNC_loop()) // ****** LOOP *********
//   {
//     if (LCDML.BT_checkEnter()) // handle button presses during menu >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
//     {
//       ;
//     }
//   }
//   if (LCDML.FUNC_close()) // ****** STABLE END *********
//   {
//     encoderDir[ENC_R].reset();
//     display.fillScreen(COLOR_BACKGROUND);
//   }
// }

FLASHMEM void not_available_message()
{
  display.fillScreen(COLOR_BACKGROUND);
  display.setTextSize(2);
  display.setTextColor(RED);
  setCursor_textGrid(1, 1);
  display.print(F("NOT SELECTED TO"));
  setCursor_textGrid(1, 2);
  display.print(F("COMPILE INTO THE CODE"));
  setCursor_textGrid(1, 4);
  display.setTextColor(GREY2);
  display.print(F("OR IS NOT YET AVAILABLE"));
  setCursor_textGrid(1, 6);
  display.setTextColor(COLOR_SYSTEXT);
  display.print(F("ENC-L TO GO BACK"));
  helptext_l(back_text);
}

FLASHMEM void print_mb_params()
{
  display.setTextSize(1);
  setCursor_textGrid_small(12, 1);
  if (generic_temp_select_menu == 0)
  {
    display.setTextColor(COLOR_BACKGROUND, GREEN);
    if (multiband_active)
      display.print(F("ACTIVE  "));
    else
    {
      display.setTextColor(COLOR_BACKGROUND, RED);
      display.print(F("INACTIVE"));
    }
  }
  else
  {
    display.setTextColor(GREEN, COLOR_BACKGROUND);
    if (multiband_active)
      display.print(F("ACTIVE  "));
    else
    {
      display.setTextColor(RED, COLOR_BACKGROUND);
      display.print(F("INACTIVE"));
    }
  }
  setCursor_textGrid_small(14, 2);
  if (generic_temp_select_menu == 1)
    display.setTextColor(COLOR_BACKGROUND, COLOR_SYSTEXT);
  else
    display.setTextColor(COLOR_SYSTEXT, COLOR_BACKGROUND);
  display.print(mb_global_gain);
  display.print("dB");
  display.setTextColor(COLOR_SYSTEXT, COLOR_BACKGROUND);
  display.print("  ");

  setCursor_textGrid_small(20, 3);
  if (generic_temp_select_menu == 2)
    display.setTextColor(COLOR_BACKGROUND, COLOR_SYSTEXT);
  else
    display.setTextColor(COLOR_SYSTEXT, COLOR_BACKGROUND);
  display.print("1:");
  display.print(mb_global_ratio);
  display.setTextColor(COLOR_SYSTEXT, COLOR_BACKGROUND);
  display.print(" ");
}

FLASHMEM void mb_set_mutes()
{
  if (mb_solo_low)
  {
    mb_mixer_l.gain(0, VOL_MAX_FLOAT + mb_global_gain + mb_gain_low);
    mb_mixer_r.gain(0, VOL_MAX_FLOAT + mb_global_gain + mb_gain_low);
  }
  else
  {
    mb_mixer_l.gain(0, 0);
    mb_mixer_r.gain(0, 0);
  }
  if (mb_solo_mid)
  {
    mb_mixer_l.gain(1, VOL_MAX_FLOAT + mb_global_gain + mb_gain_mid);
    mb_mixer_r.gain(1, VOL_MAX_FLOAT + mb_global_gain + mb_gain_mid);
  }
  else
  {
    mb_mixer_l.gain(1, 0);
    mb_mixer_r.gain(1, 0);
  }
  if (mb_solo_upper_mid)
  {
    mb_mixer_l.gain(2, VOL_MAX_FLOAT + mb_global_gain + mb_gain_upper_mid);
    mb_mixer_r.gain(2, VOL_MAX_FLOAT + mb_global_gain + mb_gain_upper_mid);
  }
  else
  {
    mb_mixer_l.gain(2, 0);
    mb_mixer_r.gain(2, 0);
  }
  if (mb_solo_high)
  {
    mb_mixer_l.gain(3, VOL_MAX_FLOAT + mb_global_gain + mb_gain_high);
    mb_mixer_r.gain(3, VOL_MAX_FLOAT + mb_global_gain + mb_gain_high);
  }
  else
  {
    mb_mixer_l.gain(3, 0);
    mb_mixer_r.gain(3, 0);
  }
  if (mb_solo_low == false && mb_solo_upper_mid == false && mb_solo_mid == false && mb_solo_high == false)
  {
    mb_mixer_l.gain(0, 1.0 + mb_global_gain + mb_gain_low);
    mb_mixer_r.gain(0, 1.0 + mb_global_gain + mb_gain_low);
    mb_mixer_l.gain(1, 1.0 + mb_global_gain + mb_gain_mid);
    mb_mixer_r.gain(1, 1.0 + mb_global_gain + mb_gain_mid);
    mb_mixer_l.gain(2, 1.0 + mb_global_gain + mb_gain_upper_mid);
    mb_mixer_r.gain(2, 1.0 + mb_global_gain + mb_gain_upper_mid);
    mb_mixer_l.gain(3, 1.0 + mb_global_gain + mb_gain_high);
    mb_mixer_r.gain(3, 1.0 + mb_global_gain + mb_gain_high);
  }
}

FLASHMEM void mb_set_master()
{
  if (multiband_active)
  {
    finalized_mixer_l.gain(0, 0); // mute normal output
    finalized_mixer_r.gain(0, 0);
    finalized_mixer_l.gain(1, VOL_MAX_FLOAT);
    finalized_mixer_r.gain(1, VOL_MAX_FLOAT);
  }
  else // disable multiband output on finalized mixer
  {
    finalized_mixer_l.gain(0, VOL_MAX_FLOAT); // normal output, mute multiband
    finalized_mixer_r.gain(0, VOL_MAX_FLOAT);
    finalized_mixer_l.gain(1, 0);
    finalized_mixer_r.gain(1, 0);
  }
}

FLASHMEM void mb_set_compressor()
{
  //  mb_compressor_l_0.compression(mb_threshold_low * -1, 0.03f , 0.2f , mb_global_ratio, 0.0f , mb_gain_low );
  //  mb_compressor_r_0.compression(mb_threshold_low * -1, 0.03f , 0.2f , mb_global_ratio, 0.0f , mb_gain_low );
  //
  //  mb_compressor_l_1.compression(mb_threshold_mid * -1, 0.03f , 0.2f , mb_global_ratio, 0.0f , mb_gain_mid);
  //  mb_compressor_r_1.compression(mb_threshold_mid * -1, 0.03f , 0.2f , mb_global_ratio, 0.0f , mb_gain_mid);
  //
  //  mb_compressor_l_2.compression(mb_threshold_upper_mid * -1, 0.03f , 0.2f , mb_global_ratio, 0.0f , mb_gain_upper_mid);
  //  mb_compressor_r_2.compression(mb_threshold_upper_mid * -1, 0.03f , 0.2f , mb_global_ratio, 0.0f , mb_gain_upper_mid);
  //
  //  mb_compressor_l_3.compression(mb_threshold_high * -1, 0.03f , 0.2f , mb_global_ratio, 0.0f , mb_gain_high);
  //  mb_compressor_r_3.compression(mb_threshold_high * -1, 0.03f , 0.2f , mb_global_ratio, 0.0f , mb_gain_high);

  mb_compressor_l_0.limit(mb_threshold_low * -1, 0.03f, 0.2f);
  mb_compressor_r_0.limit(mb_threshold_low * -1, 0.03f, 0.2f);

  mb_compressor_l_1.limit(mb_threshold_mid * -1, 0.03f, 0.2f);
  mb_compressor_r_1.limit(mb_threshold_mid * -1, 0.03f, 0.2f);

  mb_compressor_l_2.limit(mb_threshold_upper_mid * -1, 0.03f, 0.2f);
  mb_compressor_r_2.limit(mb_threshold_upper_mid * -1, 0.03f, 0.2f);

  mb_compressor_l_3.limit(mb_threshold_high * -1, 0.03f, 0.2f);
  mb_compressor_r_3.limit(mb_threshold_high * -1, 0.03f, 0.2f);

  mb_filter_l_0.setLowpass(0, mb_cross_freq_low, mb_q_low);
  mb_filter_r_0.setLowpass(0, mb_cross_freq_low, mb_q_low);

  mb_filter_l_1.setBandpass(0, mb_cross_freq_mid, mb_q_mid);
  mb_filter_r_1.setBandpass(0, mb_cross_freq_mid, mb_q_mid);

  mb_filter_l_2.setBandpass(0, mb_cross_freq_upper_mid, mb_q_upper_mid);
  mb_filter_r_2.setBandpass(0, mb_cross_freq_upper_mid, mb_q_upper_mid);

  mb_filter_l_3.setHighpass(0, mb_cross_freq_high, mb_q_high);
  mb_filter_r_3.setHighpass(0, mb_cross_freq_high, mb_q_high);

  //  mb_filter_l_0.setLowpass(0, mb_cross_freq_low, mb_q_low);
  //  mb_filter_r_0.setLowpass(0, mb_cross_freq_low, mb_q_low);
  //  mb_filter_l_0.setLowpass(1, mb_cross_freq_low, mb_q_low * 2);
  //  mb_filter_r_0.setLowpass(1, mb_cross_freq_low, mb_q_low * 2);
  //  mb_filter_l_0.setLowpass(2, mb_cross_freq_low, mb_q_low);
  //  mb_filter_r_0.setLowpass(2, mb_cross_freq_low, mb_q_low);
  //  mb_filter_l_0.setLowpass(3, mb_cross_freq_low, mb_q_low * 2);
  //  mb_filter_r_0.setLowpass(3, mb_cross_freq_low, mb_q_low * 2);
  //
  //  mb_filter_l_1.setBandpass(0, mb_cross_freq_mid, mb_q_mid);
  //  mb_filter_r_1.setBandpass(0, mb_cross_freq_mid, mb_q_mid);
  //  mb_filter_l_1.setBandpass(1, mb_cross_freq_mid, mb_q_mid * 2);
  //  mb_filter_r_1.setBandpass(1, mb_cross_freq_mid, mb_q_mid * 2);
  //  mb_filter_l_1.setBandpass(2, mb_cross_freq_mid, mb_q_mid);
  //  mb_filter_r_1.setBandpass(2, mb_cross_freq_mid, mb_q_mid);
  //  mb_filter_l_1.setBandpass(3, mb_cross_freq_mid, mb_q_mid * 2);
  //  mb_filter_r_1.setBandpass(3, mb_cross_freq_mid, mb_q_mid * 2);
  //
  //  mb_filter_l_2.setBandpass(0, mb_cross_freq_upper_mid, mb_q_upper_mid);
  //  mb_filter_r_2.setBandpass(0, mb_cross_freq_upper_mid, mb_q_upper_mid);
  //  mb_filter_l_2.setBandpass(1, mb_cross_freq_upper_mid, mb_q_upper_mid * 2);
  //  mb_filter_r_2.setBandpass(1, mb_cross_freq_upper_mid, mb_q_upper_mid * 2);
  //  mb_filter_l_2.setBandpass(2, mb_cross_freq_upper_mid, mb_q_upper_mid);
  //  mb_filter_r_2.setBandpass(2, mb_cross_freq_upper_mid, mb_q_upper_mid);
  //  mb_filter_l_2.setBandpass(3, mb_cross_freq_upper_mid, mb_q_upper_mid * 2);
  //  mb_filter_r_2.setBandpass(3, mb_cross_freq_upper_mid, mb_q_upper_mid * 2);
  //
  //  mb_filter_l_3.setHighpass(0, mb_cross_freq_high, mb_q_high);
  //  mb_filter_r_3.setHighpass(0, mb_cross_freq_high, mb_q_high);
  //  mb_filter_l_3.setHighpass(1, mb_cross_freq_high, mb_q_high * 2);
  //  mb_filter_r_3.setHighpass(1, mb_cross_freq_high, mb_q_high * 2);
  //  mb_filter_l_3.setHighpass(2, mb_cross_freq_high, mb_q_high);
  //  mb_filter_r_3.setHighpass(2, mb_cross_freq_high, mb_q_high);
  //  mb_filter_l_3.setHighpass(3, mb_cross_freq_high, mb_q_high * 2);
  //  mb_filter_r_3.setHighpass(3, mb_cross_freq_high, mb_q_high * 2);
}

FLASHMEM void mb_print_solo_buttons()
{
  if (generic_temp_select_menu == 3 && generic_active_function == 0)
  {
    if (mb_solo_high)
      draw_button_on_grid(9, 8, "SOLO", "ON ", 1);
    else
      draw_button_on_grid(9, 8, "SOLO", "  ", 1);
  }
  else if (generic_temp_select_menu > 1 && generic_temp_select_menu < 5)
  {
    if (mb_solo_high)
      draw_button_on_grid(9, 8, "SOLO", "ON ", 2);
    else
      draw_button_on_grid(9, 8, "SOLO", "  ", mb_solo_high);
  }

  if (generic_temp_select_menu == 8 && generic_active_function == 0)
  {
    if (mb_solo_upper_mid)
      draw_button_on_grid(9, 14, "SOLO", "ON ", 1);
    else
      draw_button_on_grid(9, 14, "SOLO", "  ", 1);
  }
  else if (generic_temp_select_menu > 6 && generic_temp_select_menu < 10)
  {
    if (mb_solo_upper_mid)
      draw_button_on_grid(9, 14, "SOLO", "ON ", 2);
    else
      draw_button_on_grid(9, 14, "SOLO", "  ", mb_solo_upper_mid);
  }

  if (generic_temp_select_menu == 13 && generic_active_function == 0)
  {
    if (mb_solo_mid)
      draw_button_on_grid(9, 20, "SOLO", "ON ", 1);
    else
      draw_button_on_grid(9, 20, "SOLO", "  ", 1);
  }
  else if (generic_temp_select_menu > 11 && generic_temp_select_menu < 15)
  {
    if (mb_solo_mid)
      draw_button_on_grid(9, 20, "SOLO", "ON ", 2);
    else
      draw_button_on_grid(9, 20, "SOLO", "  ", mb_solo_mid);
  }

  if (generic_temp_select_menu == 18 && generic_active_function == 0)
  {
    if (mb_solo_low)
      draw_button_on_grid(9, 26, "SOLO", "ON ", 1);
    else
      draw_button_on_grid(9, 26, "SOLO", "  ", 1);
  }
  else if (generic_temp_select_menu > 16 && generic_temp_select_menu < 20)
  {
    if (mb_solo_low)
      draw_button_on_grid(9, 26, "SOLO", "ON ", 2);
    else
      draw_button_on_grid(9, 26, "SOLO", "  ", mb_solo_low);
  }
}

FLASHMEM void mb_print_threshold_buttons()
{
  char temp_char[4];
  if (generic_temp_select_menu == 7 && generic_active_function == 0)
    draw_button_on_grid(38, 8, "THRLD", itoa(mb_threshold_high, temp_char, 10), 1);
  else if (generic_temp_select_menu == 7 && generic_active_function == 1)
    draw_button_on_grid(38, 8, "THRLD", itoa(mb_threshold_high, temp_char, 10), 2);
  else if (generic_temp_select_menu == 6 || generic_temp_select_menu == 8)
    draw_button_on_grid(38, 8, "THRLD", itoa(mb_threshold_high, temp_char, 10), 0);

  if (generic_temp_select_menu == 12 && generic_active_function == 0)
    draw_button_on_grid(38, 14, "THRLD", itoa(mb_threshold_upper_mid, temp_char, 10), 1);
  else if (generic_temp_select_menu == 12 && generic_active_function == 1)
    draw_button_on_grid(38, 14, "THRLD", itoa(mb_threshold_upper_mid, temp_char, 10), 2);
  else if (generic_temp_select_menu == 11 || generic_temp_select_menu == 13)
    draw_button_on_grid(38, 14, "THRLD", itoa(mb_threshold_upper_mid, temp_char, 10), 0);

  if (generic_temp_select_menu == 17 && generic_active_function == 0)
    draw_button_on_grid(38, 20, "THRLD", itoa(mb_threshold_mid, temp_char, 10), 1);
  else if (generic_temp_select_menu == 17 && generic_active_function == 1)
    draw_button_on_grid(38, 20, "THRLD", itoa(mb_threshold_mid, temp_char, 10), 2);
  else if (generic_temp_select_menu == 16 || generic_temp_select_menu == 18)
    draw_button_on_grid(38, 20, "THRLD", itoa(mb_threshold_mid, temp_char, 10), 0);

  if (generic_temp_select_menu == 22 && generic_active_function == 0)
    draw_button_on_grid(38, 26, "THRLD", itoa(mb_threshold_low, temp_char, 10), 1);
  else if (generic_temp_select_menu == 22 && generic_active_function == 1)
    draw_button_on_grid(38, 26, "THRLD", itoa(mb_threshold_low, temp_char, 10), 2);
  else if (generic_temp_select_menu == 21)
    draw_button_on_grid(38, 26, "THRLD", itoa(mb_threshold_low, temp_char, 10), 0);
}

FLASHMEM void mb_print_freq_and_q()
{
  for (int y = 0; y < 4; y++)
  {
    display.setTextColor(COLOR_SYSTEXT, COLOR_BACKGROUND);
    display.setCursor(CHAR_width_small * 17 - 2, CHAR_height_small * 10 + y * (CHAR_height_small * 6) + 2);
    if (y == 0)
    {
      if (generic_temp_select_menu == 4 && generic_active_function == 0)
        display.setTextColor(COLOR_SYSTEXT, DX_DARKCYAN);
      display.print(mb_cross_freq_high);
      display.print(F(" "));
    }
    else if (y == 1)
    {
      if (generic_temp_select_menu == 9 && generic_active_function == 0)
        display.setTextColor(COLOR_SYSTEXT, DX_DARKCYAN);
      display.print(mb_cross_freq_upper_mid);
      display.print(F(" "));
    }
    else if (y == 2)
    {
      if (generic_temp_select_menu == 14 && generic_active_function == 0)
        display.setTextColor(COLOR_SYSTEXT, DX_DARKCYAN);
      display.print(mb_cross_freq_mid);
      display.print(F(" "));
    }
    else if (y == 3)
    {
      if (generic_temp_select_menu == 19 && generic_active_function == 0)
        display.setTextColor(COLOR_SYSTEXT, DX_DARKCYAN);
      display.print(mb_cross_freq_low);
      display.print(F(" "));
    }

    display.setTextColor(GREY1, COLOR_BACKGROUND);
    display.setCursor(CHAR_width_small * 23 - 1, CHAR_height_small * 10 + y * (CHAR_height_small * 6) + 2);
    if (y == 0)
    {
      if (generic_temp_select_menu == 5 && generic_active_function == 0)
        display.setTextColor(COLOR_SYSTEXT, DX_DARKCYAN);
      display.print(mb_q_high);
    }
    if (y == 1)
    {
      if (generic_temp_select_menu == 10 && generic_active_function == 0)
        display.setTextColor(COLOR_SYSTEXT, DX_DARKCYAN);
      display.print(mb_q_upper_mid);
    }
    if (y == 2)
    {
      if (generic_temp_select_menu == 15 && generic_active_function == 0)
        display.setTextColor(COLOR_SYSTEXT, DX_DARKCYAN);
      display.print(mb_q_mid);
    }
    if (y == 3)
    {
      if (generic_temp_select_menu == 20 && generic_active_function == 0)
        display.setTextColor(COLOR_SYSTEXT, DX_DARKCYAN);
      display.print(mb_q_low);
    }

    display.setTextColor(COLOR_SYSTEXT, COLOR_BACKGROUND);
    display.setCursor(CHAR_width_small * 28 - 1, CHAR_height_small * 10 + y * (CHAR_height_small * 6) + 2);
    if (y == 0)
    {
      if (generic_temp_select_menu == 6 && generic_active_function == 0)
        display.setTextColor(COLOR_SYSTEXT, DX_DARKCYAN);
      display.print(mb_gain_high);
    }
    if (y == 1)
    {
      if (generic_temp_select_menu == 11 && generic_active_function == 0)
        display.setTextColor(COLOR_SYSTEXT, DX_DARKCYAN);
      display.print(mb_gain_upper_mid);
    }
    if (y == 2)
    {
      if (generic_temp_select_menu == 16 && generic_active_function == 0)
        display.setTextColor(COLOR_SYSTEXT, DX_DARKCYAN);
      display.print(mb_gain_mid);
    }
    if (y == 3)
    {
      if (generic_temp_select_menu == 21 && generic_active_function == 0)
        display.setTextColor(COLOR_SYSTEXT, DX_DARKCYAN);
      display.print(mb_gain_low);
    }
  }
}

FLASHMEM void mb_clear_caches()
{
  memset(ts.displayed_peak, 0, sizeof(ts.displayed_peak));
  clear_volmeter(CHAR_width_small * 1, 228);
  clear_volmeter(CHAR_width_small * 5, 228);
  clear_volmeter(DISPLAY_WIDTH - CHAR_width_small * 8 + 2, 228);
  clear_volmeter(DISPLAY_WIDTH - CHAR_width_small * 4 + 2, 228);
}

FLASHMEM void UI_func_multiband_dynamics(uint8_t param)
{
  char temp_char[4];
  if (LCDML.FUNC_setup()) // ****** SETUP *********
  {
    generic_active_function = 0;
    display.fillScreen(COLOR_BACKGROUND);
    display.setTextColor(COLOR_SYSTEXT);
    setCursor_textGrid(1, 1);
    display.setTextSize(1);
    setCursor_textGrid_small(1, 1);
    display.print(F("MULTIBAND:"));
    setCursor_textGrid_small(1, 2);
    display.print(F("GLOBAL GAIN:"));
    setCursor_textGrid_small(1, 3);
    display.print(F("GLOBAL COMP. RATIO"));
    display.setCursor(CHAR_width_small * 1, CHAR_height_small * 12 - 2);
    display.print(F("IN  IN"));
    display.setCursor(CHAR_width_small * 1, CHAR_height_small * 13 - 2);
    display.print(F("L   R"));
    display.setCursor(CHAR_width_small * 46, CHAR_height_small * 12 - 2);
    display.print(F("OUT OUT"));
    display.setCursor(CHAR_width_small * 46, CHAR_height_small * 13 - 2);
    display.print(F("L   R"));
    display.setCursor(CHAR_width_small * 9, CHAR_height_small * 7 - 2);
    display.print(F("HIGH"));
    display.setCursor(CHAR_width_small * 9, CHAR_height_small * 13 - 2);
    display.print(F("UPPER MID"));
    display.setCursor(CHAR_width_small * 9, CHAR_height_small * 19 - 2);
    display.print(F("MID"));
    display.setCursor(CHAR_width_small * 9, CHAR_height_small * 25 - 2);
    display.print(F("LOW"));

    draw_button_on_grid(9, 8, "SOLO", "  ", 0);
    draw_button_on_grid(38, 8, "THRLD", itoa(mb_threshold_high, temp_char, 10), 0);

    draw_button_on_grid(9, 14, "SOLO", "  ", 0);
    draw_button_on_grid(38, 14, "THRLD", itoa(mb_threshold_upper_mid, temp_char, 10), 0);

    draw_button_on_grid(9, 20, "SOLO", "  ", 0);
    draw_button_on_grid(38, 20, "THRLD", itoa(mb_threshold_mid, temp_char, 10), 0);

    draw_button_on_grid(9, 26, "SOLO", "  ", 0);
    draw_button_on_grid(38, 26, "THRLD", itoa(mb_threshold_low, temp_char, 10), 0);

    print_mb_params();

    for (int y = 0; y < 4; y++)
    {
      display.setCursor(CHAR_width_small * 17 - 1, CHAR_height_small * 9 + y * (CHAR_height_small * 6) - 2);
      display.print(F("FREQ:"));
      display.setCursor(CHAR_width_small * 23 - 1, CHAR_height_small * 9 + y * (CHAR_height_small * 6) - 2);
      display.setTextColor(GREY1);
      display.print(F("Q:"));
      display.setCursor(CHAR_width_small * 28 - 1, CHAR_height_small * 9 + y * (CHAR_height_small * 6) - 2);
      display.setTextColor(COLOR_SYSTEXT);
      display.print(F("GAIN:"));
      display.console = true;
      display.drawLine(190 + 7, CHAR_height_small * 8 + y * (CHAR_height_small * 6), 190 + 7, CHAR_height_small * 12 + y * (CHAR_height_small * 6) - 1, GREY2);
      display.console = true;
      display.drawRect(104 - 7, 64 + (y * (CHAR_height_small * 6)), 116 + 14, 32, GREY2);
      display.setTextColor(COLOR_SYSTEXT, COLOR_BACKGROUND);
    }
    mb_print_freq_and_q();
    mb_set_mutes();
    mb_set_compressor();
    mb_set_master();
  }
  if (LCDML.FUNC_loop()) // ****** LOOP *********
  {
    if ((LCDML.BT_checkDown() && encoderDir[ENC_R].Down()) || (LCDML.BT_checkUp() && encoderDir[ENC_R].Up()) || (LCDML.BT_checkEnter() && encoderDir[ENC_R].ButtonShort()))
    {
      if (LCDML.BT_checkDown())
      {
        if (generic_active_function == 0)
          generic_temp_select_menu = constrain(generic_temp_select_menu + 1, 0, 22);
        else if (generic_temp_select_menu == 0)
        {
          multiband_active = !multiband_active;
          mb_clear_caches();
        }
        else if (generic_temp_select_menu == 1)
          mb_global_gain = constrain(mb_global_gain + 0.2, -2, 12);
        else if (generic_temp_select_menu == 2)
          mb_global_ratio = constrain(mb_global_ratio + ENCODER[ENC_R].speed(), 1, 60);
        else if (generic_temp_select_menu == 3)
          mb_solo_high = !mb_solo_high;
        else if (generic_temp_select_menu == 4)
          mb_cross_freq_high = constrain(mb_cross_freq_high + ENCODER[ENC_R].speed() * 10, 2000, 12000);
        else if (generic_temp_select_menu == 5)
          mb_q_high = constrain(mb_q_high + 0.1, 0, 2);
        else if (generic_temp_select_menu == 6)
          mb_gain_high = constrain(mb_gain_high + 0.2, -9, 9);
        else if (generic_temp_select_menu == 7)
          mb_threshold_high = constrain(mb_threshold_high + ENCODER[ENC_R].speed(), 0, 40);
        else if (generic_temp_select_menu == 8)
          mb_solo_upper_mid = !mb_solo_upper_mid;
        else if (generic_temp_select_menu == 9)
          mb_cross_freq_upper_mid = constrain(mb_cross_freq_upper_mid + ENCODER[ENC_R].speed() * 10, 1000, 9999);
        else if (generic_temp_select_menu == 10)
          mb_q_upper_mid = constrain(mb_q_upper_mid + 0.1, 0, 2);
        else if (generic_temp_select_menu == 11)
          mb_gain_upper_mid = constrain(mb_gain_upper_mid + 0.2, -9, 9);
        else if (generic_temp_select_menu == 12)
          mb_threshold_upper_mid = constrain(mb_threshold_upper_mid + ENCODER[ENC_R].speed(), 0, 40);
        else if (generic_temp_select_menu == 13)
          mb_solo_mid = !mb_solo_mid;
        else if (generic_temp_select_menu == 14)
          mb_cross_freq_mid = constrain(mb_cross_freq_mid + ENCODER[ENC_R].speed() * 10, 400, 7000);
        else if (generic_temp_select_menu == 15)
          mb_q_mid = constrain(mb_q_mid + 0.1, 0, 2);
        else if (generic_temp_select_menu == 16)
          mb_gain_mid = constrain(mb_gain_mid + 0.2, -9, 9);
        else if (generic_temp_select_menu == 17)
          mb_threshold_mid = constrain(mb_threshold_mid + ENCODER[ENC_R].speed(), 0, 40);
        else if (generic_temp_select_menu == 18)
          mb_solo_low = !mb_solo_low;
        else if (generic_temp_select_menu == 19)
          mb_cross_freq_low = constrain(mb_cross_freq_low + ENCODER[ENC_R].speed() * 10, 10, 2000);
        else if (generic_temp_select_menu == 20)
          mb_q_low = constrain(mb_q_low + 0.1, 0, 2);
        else if (generic_temp_select_menu == 21)
          mb_gain_low = constrain(mb_gain_low + 0.2, -9, 9);
        else if (generic_temp_select_menu == 22)
          mb_threshold_low = constrain(mb_threshold_low + ENCODER[ENC_R].speed(), 0, 40);
      }
      else if (LCDML.BT_checkUp())
      {
        if (generic_active_function == 0)
          generic_temp_select_menu = constrain(generic_temp_select_menu - 1, 0, 22);
        else if (generic_temp_select_menu == 0)
        {
          multiband_active = !multiband_active;
          mb_clear_caches();
        }
        else if (generic_temp_select_menu == 1)
          mb_global_gain = constrain(mb_global_gain - 0.2, -2, 12);
        else if (generic_temp_select_menu == 2)
          mb_global_ratio = constrain(mb_global_ratio - ENCODER[ENC_R].speed(), 1, 60);
        else if (generic_temp_select_menu == 3)
          mb_solo_high = !mb_solo_high;
        else if (generic_temp_select_menu == 4)
          mb_cross_freq_high = constrain(mb_cross_freq_high - ENCODER[ENC_R].speed() * 10, 2000, 12000);
        else if (generic_temp_select_menu == 5)
          mb_q_high = constrain(mb_q_high - 0.1, 0, 2);
        else if (generic_temp_select_menu == 6)
          mb_gain_high = constrain(mb_gain_high - 0.2, -9, 9);
        else if (generic_temp_select_menu == 7)
          mb_threshold_high = constrain(mb_threshold_high - ENCODER[ENC_R].speed(), 0, 40);
        else if (generic_temp_select_menu == 8)
          mb_solo_upper_mid = !mb_solo_upper_mid;
        else if (generic_temp_select_menu == 9)
          mb_cross_freq_upper_mid = constrain(mb_cross_freq_upper_mid - ENCODER[ENC_R].speed() * 10, 1000, 9999);
        else if (generic_temp_select_menu == 10)
          mb_q_upper_mid = constrain(mb_q_upper_mid - 0.1, 0, 2);
        else if (generic_temp_select_menu == 11)
          mb_gain_upper_mid = constrain(mb_gain_upper_mid - 0.2, -9, 9);
        else if (generic_temp_select_menu == 12)
          mb_threshold_upper_mid = constrain(mb_threshold_upper_mid - ENCODER[ENC_R].speed(), 0, 40);
        else if (generic_temp_select_menu == 13)
          mb_solo_mid = !mb_solo_mid;
        else if (generic_temp_select_menu == 14)
          mb_cross_freq_mid = constrain(mb_cross_freq_mid - ENCODER[ENC_R].speed() * 10, 400, 7000);
        else if (generic_temp_select_menu == 15)
          mb_q_mid = constrain(mb_q_mid - 0.1, 0, 2);
        else if (generic_temp_select_menu == 16)
          mb_gain_mid = constrain(mb_gain_mid - 0.2, -9, 9);
        else if (generic_temp_select_menu == 17)
          mb_threshold_mid = constrain(mb_threshold_mid - ENCODER[ENC_R].speed(), 0, 40);
        else if (generic_temp_select_menu == 18)
          mb_solo_low = !mb_solo_low;
        else if (generic_temp_select_menu == 19)
          mb_cross_freq_low = constrain(mb_cross_freq_low - ENCODER[ENC_R].speed() * 10, 10, 2000);
        else if (generic_temp_select_menu == 20)
          mb_q_low = constrain(mb_q_low - 0.1, 0, 2);
        else if (generic_temp_select_menu == 21)
          mb_gain_low = constrain(mb_gain_low - 0.2, -9, 9);
        else if (generic_temp_select_menu == 22)
          mb_threshold_low = constrain(mb_threshold_low - ENCODER[ENC_R].speed(), 0, 40);
      }

      if (LCDML.BT_checkEnter()) // handle button presses during menu >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
      {
        if (generic_active_function == 0)
          generic_active_function = 1;
        else
          generic_active_function = 0;
      }
    }
    display.setTextColor(COLOR_SYSTEXT, COLOR_BACKGROUND);
    display.setTextSize(1);

    print_mb_params();
    mb_set_mutes();
    mb_set_compressor();
    display.console = true;
    for (int y = 0; y < 4; y++)
    {
      display.fillRect(192 + 7, CHAR_height_small * 8 + y * (CHAR_height_small * 6) + 1, 27, 27, COLOR_BACKGROUND);
      display.console = true;
      if (y == 0)
      {
        display.drawLine(191 + 7, CHAR_height_small * 11 + 5 + y * (CHAR_height_small * 6), 191 + 27 + 7 - mb_threshold_high / 2.5, CHAR_height_small * 11 + 5 + y * (CHAR_height_small * 6) - 27 + mb_threshold_high / 2.6, GREEN);
        display.drawLine(191 + 7 + 27 - mb_threshold_high / 2.5, CHAR_height_small * 11 + 5 + y * (CHAR_height_small * 6) - 27 + mb_threshold_high / 2.6, 191 + 27 + 7, CHAR_height_small * 11 + 5 + y * (CHAR_height_small * 6) - 27 + mb_threshold_high / 2.9, GREEN);
      }
      else if (y == 1)
      {
        display.drawLine(191 + 7, CHAR_height_small * 11 + 5 + y * (CHAR_height_small * 6), 191 + 27 + 7 - mb_threshold_upper_mid / 2.5, CHAR_height_small * 11 + 5 + y * (CHAR_height_small * 6) - 27 + mb_threshold_upper_mid / 2.6, GREEN);
        display.drawLine(191 + 7 + 27 - mb_threshold_upper_mid / 2.5, CHAR_height_small * 11 + 5 + y * (CHAR_height_small * 6) - 27 + mb_threshold_upper_mid / 2.6, 191 + 27 + 7, CHAR_height_small * 11 + 5 + y * (CHAR_height_small * 6) - 27 + mb_threshold_upper_mid / 2.9, GREEN);
      }
      else if (y == 2)
      {
        display.drawLine(191 + 7, CHAR_height_small * 11 + 5 + y * (CHAR_height_small * 6), 191 + 27 + 7 - mb_threshold_mid / 2.5, CHAR_height_small * 11 + 5 + y * (CHAR_height_small * 6) - 27 + mb_threshold_mid / 2.6, GREEN);
        display.drawLine(191 + 7 + 27 - mb_threshold_mid / 2.5, CHAR_height_small * 11 + 5 + y * (CHAR_height_small * 6) - 27 + mb_threshold_mid / 2.6, 191 + 27 + 7, CHAR_height_small * 11 + 5 + y * (CHAR_height_small * 6) - 27 + mb_threshold_mid / 2.9, GREEN);
      }
      else if (y == 3)
      {
        display.drawLine(191 + 7, CHAR_height_small * 11 + 5 + y * (CHAR_height_small * 6), 191 + 27 + 7 - mb_threshold_low / 2.5, CHAR_height_small * 11 + 5 + y * (CHAR_height_small * 6) - 27 + mb_threshold_low / 2.6, GREEN);
        display.drawLine(191 + 7 + 27 - mb_threshold_low / 2.5, CHAR_height_small * 11 + 5 + y * (CHAR_height_small * 6) - 27 + mb_threshold_low / 2.6, 191 + 27 + 7, CHAR_height_small * 11 + 5 + y * (CHAR_height_small * 6) - 27 + mb_threshold_low / 2.9, GREEN);
      }
    }
    if (generic_temp_select_menu == 0)
    {
      mb_set_master();
    }

    // mute/solo buttons
    mb_print_solo_buttons();

    // freq + q
    mb_print_freq_and_q();

    // threshold buttons
    mb_print_threshold_buttons();
  }
  if (LCDML.FUNC_close()) // ****** STABLE END *********
  {
    encoderDir[ENC_R].reset();

    display.setTextSize(2);
    display.setTextColor(COLOR_SYSTEXT, COLOR_BACKGROUND);
    display.fillScreen(COLOR_BACKGROUND);
  }
}

void startRecording()
{
  display.setTextSize(2);
  record_x_pos = 0;
  record_timer = 0;
  display.setTextColor(GREY2, COLOR_BACKGROUND);
  setCursor_textGrid(11, 6);
  display.print(":");
  setCursor_textGrid(14, 6);
  display.print(":");
  setCursor_textGrid(17, 6);
  display.print("    ");
  if (SD.exists(filename))
  {
    // The SD library writes new data to the end of the
    // file, so to start a new recording, the old file
    // must be deleted before new data is written.
    SD.remove(filename);
  }
  frec = SD.open(filename, FILE_WRITE);
  helptext_r("PUSH TO STOP");

  if (frec)
  {
    record_queue_l.begin();
    record_queue_r.begin();
    fm.wav_recorder_mode = 1;
  }
}

// write all 512 bytes to the SD card
void continueRecording()
{
  if (record_queue_l.available() >= 2 && record_queue_r.available() >= 2)
  {
    byte buffer[512];
    byte bufferL[256];
    byte bufferR[256];
    memcpy(bufferL, record_queue_l.readBuffer(), 256);
    memcpy(bufferR, record_queue_r.readBuffer(), 256);
    record_queue_l.freeBuffer();
    record_queue_r.freeBuffer();
    int b = 0;
    for (int i = 0; i < 512; i += 4)
    {
      buffer[i] = bufferL[b];
      buffer[i + 1] = bufferL[b + 1];
      buffer[i + 2] = bufferR[b];
      buffer[i + 3] = bufferR[b + 1];
      b = b + 2;
    }
    frec.write(buffer, 512);

    if (record_timer % 6 == 0)
    {
      record_x_pos++;
      if (record_x_pos > DISPLAY_WIDTH - 1)
        record_x_pos = 0;

      short samplevalue = 0;
      samplevalue = ((bufferL[1] * 256) + bufferL[0]);
      display.drawLine(record_x_pos, 175 - 50, record_x_pos, 175 + 50, GREY4);
      display.drawLine(record_x_pos, 175, record_x_pos, samplevalue / 700 + 175, GREEN);
    }
  }

  uint32_t seconds = record_timer / 1000, minutes, hours;
  minutes = seconds / 60;
  seconds %= 60;
  hours = minutes / 60;
  minutes %= 60;

  display.setTextColor(GREY2, COLOR_BACKGROUND);
  setCursor_textGrid(9, 6);
  print_formatted_number(hours, 2);
  setCursor_textGrid(12, 6);
  print_formatted_number(minutes, 2);
  setCursor_textGrid(15, 6);
  print_formatted_number(seconds, 2);
}

void stopRecording()
{
  char tmp[6];

  record_queue_l.end();
  record_queue_r.end();
  if (fm.wav_recorder_mode == 1)
  {
    while (record_queue_l.available() > 0 && record_queue_r.available() > 0)
    {
      record_queue_l.readBuffer();
      record_queue_l.freeBuffer();
      record_queue_r.readBuffer();
      record_queue_r.freeBuffer();
    }
    setCursor_textGrid(9, 6);
    display.setTextColor(GREY2, COLOR_BACKGROUND);
    display.print("SAVED ");
    display.setTextColor(RED, COLOR_BACKGROUND);
    if (frec.size() / 1024 / 1024 > 0)
    {
      snprintf_P(tmp, sizeof(tmp), PSTR("%03d"), int(frec.size() / 1024 / 1024));
      display.print(tmp);
      display.setTextColor(GREY2, COLOR_BACKGROUND);
      display.print(" MB  ");
    }
    else if (int(frec.size() / 1024) > 0)
    {
      snprintf_P(tmp, sizeof(tmp), PSTR("%03d"), int(frec.size() / 1024));
      display.print(tmp);
      display.setTextColor(GREY2, COLOR_BACKGROUND);
      display.print(" KB  ");
    }

    frec.close(); // close file
    fm.wav_recorder_mode = 0;
    // helptext_r ("PUSH TO RECORD");
    display.setTextColor(COLOR_SYSTEXT);
  }
}

FLASHMEM void UI_func_recorder(uint8_t param)
{
  if (LCDML.FUNC_setup()) // ****** SETUP *********
  {
    generic_active_function = 0;
    display.fillScreen(COLOR_BACKGROUND);
    display.setTextColor(COLOR_SYSTEXT);
    display.setTextSize(2);
    setCursor_textGrid(1, 1);
    display.print(F("AUDIO RECORDER"));
    setCursor_textGrid(1, 4);
    display.print(F("STATUS:"));
    setCursor_textGrid(1, 5);
    display.print(F("FILE:"));

    display.setTextColor(GREY2, COLOR_BACKGROUND);
    setCursor_textGrid(9, 6);
    display.print("00:00:00");
  }
  if (LCDML.FUNC_loop()) // ****** LOOP *********
  {
    if ((LCDML.BT_checkDown() && encoderDir[ENC_R].Down()) || (LCDML.BT_checkUp() && encoderDir[ENC_R].Up()) || (LCDML.BT_checkEnter() && encoderDir[ENC_R].ButtonShort()))
    {
      if (LCDML.BT_checkDown())
      {
        temp_int = constrain(temp_int + ENCODER[ENC_R].speed(), 0, 254);
      }
      else if (LCDML.BT_checkUp())
      {
        temp_int = constrain(temp_int - ENCODER[ENC_R].speed(), 0, 254);
      }

      if (LCDML.BT_checkEnter()) // handle button presses during menu >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
      {
        if (fm.wav_recorder_mode == 0)
          startRecording();
        else if (fm.wav_recorder_mode == 1)
          stopRecording();
      }
    }
    helptext_r("+/- FILE, PUSH TO RECORD");
    helptext_l(back_text);
    display.setTextColor(GREEN, COLOR_BACKGROUND);
    display.setTextSize(2);

    setCursor_textGrid(9, 4);
    if (fm.wav_recorder_mode == 0)
      display.print(F("READY TO RECORD"));
    else
    {
      display.setTextColor(RED, COLOR_BACKGROUND);
      display.print(F("NOW RECORDING     "));
    }

    display.setTextColor(COLOR_SYSTEXT, COLOR_BACKGROUND);
    snprintf_P(filename, sizeof(filename), PSTR("REC_%03d.RAW"), temp_int);
    setCursor_textGrid(9, 5);
    display.print(filename);
  }
  if (LCDML.FUNC_close()) // ****** STABLE END *********
  {
    encoderDir[ENC_R].reset();
    display.setTextSize(2);
    display.setTextColor(COLOR_SYSTEXT, COLOR_BACKGROUND);
    display.fillScreen(COLOR_BACKGROUND);
    temp_int = 0;
  }
}

void print_braids_static_texts()
{
  display.setTextSize(1);
  seq.edit_state = 0;

  setCursor_textGrid_small(1, 1);
  display.setTextColor(RED);
  display.print(F("BRAIDS"));
  display.setTextColor(GREY1);
  setCursor_textGrid_small(1, 3);
  display.print(F("VOLUME"));
  setCursor_textGrid_small(1, 4);
  display.print(F("SHAPE"));
  setCursor_textGrid_small(1, 5);
  display.print(F("COLOR"));
  setCursor_textGrid_small(1, 6);
  display.print(F("TIMBRE"));
  setCursor_textGrid_small(1, 7);
  display.print(F("COARSE"));
  setCursor_textGrid_small(1, 8);
  display.print(F("ATTACK"));
  setCursor_textGrid_small(1, 9);
  display.print(F("DECAY"));
  setCursor_textGrid_small(1, 10);
  display.print(F("SUSTAIN"));
  setCursor_textGrid_small(1, 11);
  display.print(F("RELEASE"));
  setCursor_textGrid_small(22, 3);
  display.print(F("FILTER"));
  setCursor_textGrid_small(22, 4);
  display.print(F("FREQ"));
  setCursor_textGrid_small(22, 5);
  display.print(F("RES"));
  setCursor_textGrid_small(32, 5);
  display.print(F("SPEED"));
  setCursor_textGrid_small(22, 6);
  display.print(F("LFO"));
  setCursor_textGrid_small(32, 6);
  display.print(F("L.SPD"));
  setCursor_textGrid_small(22, 7);
  display.print(F("REV. SEND"));
  setCursor_textGrid_small(22, 8);
  display.print(F("FLANGER"));
  setCursor_textGrid_small(39, 7);
  display.setTextColor(GREY2);
  display.print(F("<LR>"));
  display.setTextColor(GREY1);
  setCursor_textGrid_small(22, 9);
  display.print(F("DLY. SENDS"));
  setCursor_textGrid_small(22, 10);
  display.print(F("PANORAMA"));
  setCursor_textGrid_small(22, 11);
  display.print(F("MIDI CHN."));
  setCursor_textGrid_small(13, 7);
  display.print(F("STEPS"));
  setCursor_textGrid_small(16, 8);
  display.print(F("MS"));
  setCursor_textGrid_small(16, 9);
  display.print(F("MS"));
  setCursor_textGrid_small(16, 10);
  display.print(F("LEVL"));
  setCursor_textGrid_small(16, 11);
  display.print(F("MS"));
  setCursor_textGrid_small(45, 5);
  display.print(F("FILTERS"));

  // arrows
  display.setTextColor(GREY1, COLOR_BACKGROUND);
  setCursor_textGrid_small(36, 4);
  display.print(F(">"));
}

void update_selective_values_braids()
{
  display.setTextSize(1);
  if (menu_item_check(0))
  {
    setModeColor(0);
    print_small_intbar(9, 3, braids_osc.sound_intensity, 0, 1, 0);
  }
  if (menu_item_check(1))
  {
    setModeColor(1);
    setCursor_textGrid_small(9, 4);
    print_formatted_number(braids_osc.algo, 2);
    setCursor_textGrid_small(13, 4);
    display.setTextColor(RED, COLOR_BACKGROUND);
    display.print(synthBraids[0]->get_name(braids_osc.algo));
    // braids_print (synthBraids[0]->get_name(braids_osc.algo)[i],i);
  }
  if (menu_item_check(2))
  {
    setModeColor(2);
    setCursor_textGrid_small(9, 5);
    print_formatted_number(braids_osc.color, 3);
  }
  if (menu_item_check(3))
  {
    setModeColor(3);
    setCursor_textGrid_small(9, 6);
    print_formatted_number(braids_osc.timbre, 3);
  }
  if (menu_item_check(4))
  {
    setModeColor(4);
    setCursor_textGrid_small(9, 7);
    print_formatted_number_signed(braids_osc.coarse, 2);
  }
  if (menu_item_check(5))
  {
    setModeColor(5);
    setCursor_textGrid_small(10, 8);
    print_formatted_number(braids_osc.env_attack * 4, 4);
  }
  if (menu_item_check(6))
  {
    setModeColor(6);
    setCursor_textGrid_small(10, 9);
    print_formatted_number(braids_osc.env_decay * 4, 4);
  }
  if (menu_item_check(7))
  {
    setModeColor(7);
    setCursor_textGrid_small(11, 10);
    print_formatted_number(braids_osc.env_sustain * 2, 3);
  }
  if (menu_item_check(8))
  {
    setModeColor(8);
    setCursor_textGrid_small(10, 11);
    print_formatted_number(braids_osc.env_release * braids_osc.env_release, 4);
  }
  if (menu_item_check(9))
  {
    setModeColor(9);
    setCursor_textGrid_small(32, 3);
    if (braids_osc.filter_mode == 0)
      display.print(F("OFF   "));
    else if (braids_osc.filter_mode == 1)
      display.print(F("LP12dB"));
    else if (braids_osc.filter_mode == 2)
      display.print(F("BP12dB"));
    else if (braids_osc.filter_mode == 3)
      display.print(F("HI12dB"));
  }
  if (menu_item_check(10))
    print_small_intbar(32, 4, braids_osc.filter_freq_from / 100, 10, 0, 1);
  if (menu_item_check(11))
    print_small_intbar(38, 4, braids_osc.filter_freq_to / 100, 11, 0, 1);
  if (menu_item_check(12))
    print_small_intbar(27, 5, braids_osc.filter_resonance, 12, 0, 1);
  if (menu_item_check(13))
    print_small_intbar(38, 5, braids_osc.filter_speed / 10, 13, 0, 1);
  if (menu_item_check(14))
    print_small_intbar(27, 6, braids_osc.filter_lfo_intensity / 100, 14, 0, 1);
  if (menu_item_check(15))
    print_small_intbar(38, 6, braids_osc.filter_lfo_speed, 15, 0, 1);
  if (menu_item_check(16))
  {
    setCursor_textGrid_small(33, 7);
    setModeColor(16);
    print_formatted_number(braids_osc.rev_send, 3);
  }
  if (menu_item_check(17))
  {
    setCursor_textGrid_small(33, 8);
    setModeColor(17);
    print_formatted_number(braids_osc.flanger, 3);
  }
  if (menu_item_check(18))
  {
    setModeColor(18);
    setCursor_textGrid_small(38, 8);
    print_formatted_number(braids_osc.flanger_spread, 3);
  }
  if (menu_item_check(19))
  {
    setModeColor(19);
    setCursor_textGrid_small(33, 9);
    print_formatted_number(braids_osc.delay_send_1, 3);
  }
  if (menu_item_check(20))
  {
    setCursor_textGrid_small(38, 9);
    setModeColor(20);
    print_formatted_number(braids_osc.delay_send_2, 3);
  }
  if (menu_item_check(21))
  {
    setModeColor(21);
    print_small_panbar(33, 10, braids_osc.pan, 21);
  }
  if (menu_item_check(22))
  {
    setModeColor(22);
    setCursor_textGrid_small(34, 11);
    print_formatted_number(braids_osc.midi_channel, 2);
  }
}

FLASHMEM void UI_func_braids(uint8_t param)
{
  if (LCDML.FUNC_setup()) // ****** SETUP *********
  {

    // setup function
    encoderDir[ENC_R].reset();
    display.fillScreen(COLOR_BACKGROUND);
    virtual_keyboard_smart_preselect_mode();
    seq.cycle_touch_element = 1;
    virtual_keyboard();
    virtual_keyboard_print_buttons();
    virtual_keyboard_print_current_instrument();
    print_braids_static_texts();
    if (LCDML.MENU_getLastActiveFunctionID() != LCDML.OTHER_getIDFromFunction(UI_func_volume) && LCDML.MENU_getLastActiveFunctionID() != LCDML.OTHER_getIDFromFunction(mFunc_screensaver))

      generic_temp_select_menu = 0;
    generic_full_draw_required = true;
    update_selective_values_braids();
    generic_full_draw_required = false;

  }
  if (LCDML.FUNC_loop()) // ****** LOOP *********
  {
    if ((LCDML.BT_checkDown() && encoderDir[ENC_R].Down()) || (LCDML.BT_checkUp() && encoderDir[ENC_R].Up()))
    {
      set_state_dir();
      if (seq.edit_state == 0)
      {
        generic_temp_select_menu = calc_val[state_dir](generic_temp_select_menu, 1, 0, 22);
        if (generic_temp_select_menu != menuhelper_previous_val)
          menuhelper_redraw = true;
      }
      else if (seq.edit_state != 0)
      {
        if (generic_temp_select_menu == 0)
          braids_osc.sound_intensity = calc_val[state_dir](braids_osc.sound_intensity, 1, 0, 100);
        else if (generic_temp_select_menu == 1)
          braids_osc.algo = calc_val[state_dir](braids_osc.algo, 1, 0, 42);
        else if (generic_temp_select_menu == 2)
          braids_osc.color = calc_val[state_dir](braids_osc.color, 1, 0, 255);
        else if (generic_temp_select_menu == 3)
          braids_osc.timbre = calc_val[state_dir](braids_osc.timbre, 1, 0, 255);
        else if (generic_temp_select_menu == 4)
          braids_osc.coarse = calc_val[state_dir](braids_osc.coarse, 1, -36, 36);
        else if (generic_temp_select_menu == 5)
          braids_osc.env_attack = calc_val[state_dir](braids_osc.env_attack, 1, 0, 255);
        else if (generic_temp_select_menu == 6)
          braids_osc.env_decay = calc_val[state_dir](braids_osc.env_decay, 1, 0, 255);
        else if (generic_temp_select_menu == 7)
          braids_osc.env_sustain = calc_val[state_dir](braids_osc.env_sustain, 1, 0, 50);
        else if (generic_temp_select_menu == 8)
          braids_osc.env_release = calc_val[state_dir](braids_osc.env_release, 1, 0, 99);
        else if (generic_temp_select_menu == 9)
          braids_osc.filter_mode = calc_val[state_dir](braids_osc.filter_mode, 1, 0, 3);
        else if (generic_temp_select_menu == 10)
          braids_osc.filter_freq_from = calc_val[state_dir](braids_osc.filter_freq_from, 80, 0, 15000);
        else if (generic_temp_select_menu == 11)
          braids_osc.filter_freq_to = calc_val[state_dir](braids_osc.filter_freq_to, 80, 0, 15000);
        else if (generic_temp_select_menu == 12)
          braids_osc.filter_resonance = calc_val[state_dir](braids_osc.filter_resonance, 1, 0, 99);
        else if (generic_temp_select_menu == 13)
          braids_osc.filter_speed = calc_val[state_dir](braids_osc.filter_speed, 5, 0, 999);
        else if (generic_temp_select_menu == 14)
          braids_osc.filter_lfo_intensity = calc_val[state_dir](braids_osc.filter_lfo_intensity, 80, 0, 15000);
        else if (generic_temp_select_menu == 15)
          braids_osc.filter_lfo_speed = calc_val[state_dir](braids_osc.filter_lfo_speed, 1, 0, 255);
        else if (generic_temp_select_menu == 16)
          braids_osc.rev_send = calc_val[state_dir](braids_osc.rev_send, 1, 0, REVERB_SEND_MAX);
        else if (generic_temp_select_menu == 17)
          braids_osc.flanger = calc_val[state_dir](braids_osc.flanger, 1, 0, REVERB_SEND_MAX);
        else if (generic_temp_select_menu == 18)
          braids_osc.flanger_spread = calc_val[state_dir](braids_osc.flanger_spread, 1, 0, REVERB_SEND_MAX);
        else if (generic_temp_select_menu == 19)
          braids_osc.delay_send_1 = calc_val[state_dir](braids_osc.delay_send_1, 1, 0, DELAY_LEVEL_MAX);
        else if (generic_temp_select_menu == 20)
          braids_osc.delay_send_2 = calc_val[state_dir](braids_osc.delay_send_2, 1, 0, DELAY_LEVEL_MAX);
        else if (generic_temp_select_menu == 21)
          braids_osc.pan = calc_val[state_dir](braids_osc.pan, 1, PANORAMA_MIN, PANORAMA_MAX);
        else if (generic_temp_select_menu == 22)
          braids_osc.midi_channel = calc_val[state_dir](braids_osc.midi_channel, 1, 1, 16);
      }
    }

    if (LCDML.BT_checkEnter() && encoderDir[ENC_R].ButtonShort()) // handle button presses during menu >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
    {
      seq.edit_state = !seq.edit_state;
      menuhelper_redraw = true;
    }
    else if (LCDML.BT_checkEnter())
    {
      ;
    }

    // button check end <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
    if (menuhelper_redraw)
    {
      braids_update_single_setting();
      update_selective_values_braids();
      menuhelper_redraw = false;
    }
  }
  if (LCDML.FUNC_close()) // ****** STABLE END *********
  {
    generic_active_function = 99;
    seq.cycle_touch_element = 0;
    encoderDir[ENC_R].reset();
    display.fillScreen(COLOR_BACKGROUND);
  }
}

// void UI_func_speedtest(uint8_t param)
//{ // ILI9341 478 msecs
//   // ILI9486 1242 msecs
//   if (LCDML.FUNC_setup())         // ****** SETUP *********
//   {
//     display.fillScreen(COLOR_BACKGROUND);
//     encoderDir[ENC_R].reset();
//     display.setTextSize(1);
//     display.setTextColor(GREY3);
//   }
//   if (LCDML.FUNC_loop())          // ****** LOOP *********
//   {
//     elapsedMillis msecs;
//     for (uint8_t t = 0; t < 30; t++)
//     {
//       for (uint8_t y = 0; y < 14; y++)
//       {
//         for (uint8_t x = 0; x < 25; x++)
//         {
//           setCursor_textGrid(x, y);
//           display.write(x * y + t);
//         }
//       }
//     }
//     display.setTextColor(GREEN);
//     display.setTextSize(2);
//     setCursor_textGrid(5, 5);
//     display.print(F("RESULT"));
//     setCursor_textGrid(5, 6);
//     display.print(msecs);
//     display.print(F(" msecs"));
//   }
//   if (LCDML.FUNC_close())     // ****** STABLE END *********
//   {
//     encoderDir[ENC_R].reset();
//     display.setTextSize(1);
//   }
// }

FLASHMEM void sd_printDirectory(bool forceReload)
{

  if (forceReload || strcmp(fm.sd_new_name, fm.sd_prev_dir))
  {
    load_sd_directory();
  }

  char tmp[6];
  fm.sd_is_folder = false;
  fm.sd_cap_rows = 9;
  if (fm.sd_parent_folder && fm.sd_folder_depth > 0)
  {
    fm.sd_is_folder = true;
    strcpy(fm.sd_temp_name, "..");
    display.setTextColor(COLOR_SYSTEXT, COLOR_PITCHSMP);
  }
  else
    display.setTextColor(COLOR_PITCHSMP, COLOR_BACKGROUND);

  if (fm.sd_folder_depth > 0)
  {
    drawBitmap(CHAR_width_small, 5 * 11 - 1, special_chars[23], 8, 8, YELLOW);
    display.setCursor(CHAR_width_small * 3, +5 * 11);
    display.print("..");
  }
  else
  {
    drawBitmap(CHAR_width_small, 5 * 11 - 1, special_chars[23], 8, 8, COLOR_BACKGROUND);
    display.setCursor(CHAR_width_small * 3, +5 * 11);
    display.print("/ ");
  }
  for (uint8_t f = 0; f < 10; f++)
  {
    if (f >= fm.sd_sum_files || f >= (fm.sd_sum_files - fm.sd_skip_files))
    {
      fm.sd_cap_rows = f - 1;
      display.console = true;
      display.fillRect(CHAR_width_small, f * 11 + 6 * 11 - 1, CHAR_width_small * 26, (10 - f) * 11, COLOR_BACKGROUND);
      break;
    }

    storage_file_t sd_entry = sdcard_infos.files[fm.sd_skip_files + f];
    if (sd_entry.isDirectory)
    {
      drawBitmap(CHAR_width_small, f * 11 - 1 + 6 * 11, special_chars[23], 8, 8, YELLOW);
      if (f == fm.sd_selected_file && fm.sd_parent_folder == false && fm.active_window == 0)
        display.setTextColor(COLOR_BACKGROUND, COLOR_PITCHSMP);
      else
        display.setTextColor(COLOR_PITCHSMP, COLOR_BACKGROUND);
      show_smallfont_noGrid(f * 11 + 6 * 11, CHAR_width_small * 3, 17, sd_entry.name);
      display.setCursor(CHAR_width_small * 21, f * 11 + 6 * 11);
      display.setTextColor(DX_DARKCYAN, COLOR_BACKGROUND);
      display.print("DIR   ");
    }
    else
    {
      drawBitmap(CHAR_width_small, f * 11 - 1 + 6 * 11, special_chars[23], 8, 8, COLOR_BACKGROUND);
      if (f == fm.sd_selected_file && fm.sd_parent_folder == false && fm.active_window == 0)
        display.setTextColor(COLOR_BACKGROUND, COLOR_SYSTEXT);
      else
        display.setTextColor(COLOR_SYSTEXT, COLOR_BACKGROUND);
      show_smallfont_noGrid(f * 11 + 6 * 11, CHAR_width_small * 3, 17, sd_entry.name);
      display.setTextColor(COLOR_DRUMS, COLOR_BACKGROUND);
      display.setCursor(CHAR_width_small * 20, f * 11 + 6 * 11);
      if (sd_entry.size / 1024 / 1024 > 0)
      {
        snprintf_P(tmp, sizeof(tmp), PSTR("%4d"), int(sd_entry.size / 1024 / 1024));
        display.print(tmp);
        display.print(" MB");
      }
      else if (int(sd_entry.size / 1024) > 0)
      {
        snprintf_P(tmp, sizeof(tmp), PSTR("%4d"), int(sd_entry.size / 1024));
        display.print(tmp);
        display.print(" KB");
      }
      else
      {
        snprintf_P(tmp, sizeof(tmp), PSTR("%4d"), int(sd_entry.size));
        display.print(tmp);
        display.print(" B ");
      }
    }
    if (f == fm.sd_selected_file && fm.sd_parent_folder == false)
      strcpy(fm.sd_temp_name, sd_entry.name);
    if (f == fm.sd_selected_file && sd_entry.isDirectory)
      fm.sd_is_folder = true;
  }
}

#ifdef COMPILE_FOR_FLASH
FLASHMEM void flash_printDirectory() // SPI FLASH
{
  if (seq.running == false)
  {
    char tmp[6];
    fm.flash_cap_rows = 9;

    for (uint8_t f = 0; f < 10; f++)
    {

      // if (f >= fm.flash_sum_files) {
      //   fm.flash_cap_rows = f - 1;
      //   display.console = true;
      //   display.fillRect(CHAR_width_small, f * 11 + 6 * 11 - 1, CHAR_width_small * 27 - 1, (10 - f) * 11, COLOR_BACKGROUND);
      //   break;
      // }

      storage_file_t flash_entry = flash_infos.files[fm.flash_skip_files + f];
      if (f == fm.flash_selected_file && fm.active_window == 1)
        display.setTextColor(COLOR_BACKGROUND, COLOR_PITCHSMP);
      else
        display.setTextColor(COLOR_PITCHSMP, COLOR_BACKGROUND);

      show_smallfont_noGrid(f * 11 + 6 * 11, CHAR_width_small * 29, 15, flash_entry.name);

      display.setTextColor(COLOR_DRUMS, COLOR_BACKGROUND);
      display.setCursor(CHAR_width_small * 45, f * 11 + 6 * 11);

      if (flash_entry.size / 1024 / 1024 > 0)
      {
        snprintf_P(tmp, sizeof(tmp), PSTR("%4d"), int(flash_entry.size / 1024 / 1024));
        display.print(tmp);
        display.print(" MB");
      }
      else if (int(flash_entry.size / 1024) > 0)
      {
        snprintf_P(tmp, sizeof(tmp), PSTR("%4d"), int(flash_entry.size / 1024));
        display.print(tmp);
        display.print(" KB");
      }
      else
      {
        snprintf_P(tmp, sizeof(tmp), PSTR("%4d"), int(flash_entry.size));
        display.print(tmp);
        display.print(" B ");
      }

      if (f == fm.flash_selected_file)
        strcpy(fm.flash_temp_name, flash_entry.name);
    }
  }
  else
  {
    display.setTextColor(RED, COLOR_BACKGROUND);
    display.setCursor(CHAR_width_small * 31, 6 * 11);
    display.print(F("NOT AVAILABLE"));
    display.setCursor(CHAR_width_small * 31, 7 * 11);
    display.print(F("WHILE SEQUENCER"));
    display.setCursor(CHAR_width_small * 31, 8 * 11);
    display.print(F("IS PLAYING FROM FLASH"));
  }
}

FLASHMEM void flash_loadDirectory() // SPI FLASH
{
  uint32_t filesize;
  uint16_t filepos = 0;
  unsigned char buf[256];

  SerialFlash.readID(buf);
  flash_infos.capacity = SerialFlash.capacity(buf);

  SerialFlash.opendir();
  flash_infos.used = 0;

  while (1)
  {
    if (SerialFlash.readdir(flash_infos.files[filepos].name, MAX_FLASH_FILENAME_LEN, filesize))
    {
      flash_infos.files[filepos].size = filesize;
      flash_infos.used = flash_infos.used + filesize / 1024;

#ifdef DEBUG
      LOG.print(filepos);
      LOG.print(F("  "));
      LOG.print(flash_infos.files[filepos].name);
      LOG.print(F("  "));
      LOG.print(filesize);
      LOG.print(F(" bytes"));
      LOG.println();
#endif
      filepos++;
    }
    else
    {
      break; // no more files
    }
  }

  fm.flash_sum_files = filepos;
#ifdef DEBUG
  LOG.print(F("Total flash files: "));
  LOG.println(fm.flash_sum_files);
#endif

  if (fm.flash_sum_files)
  {
    qsort(flash_infos.files, fm.flash_sum_files, sizeof(storage_file_t), compare_files_by_name);

    // Update MSP zones entry number from filename
    for (uint8_t i = 0; i < NUM_MULTISAMPLES; i++)
    {
      for (uint8_t j = 0; j < NUM_MULTISAMPLE_ZONES; j++)
      {
        bool found = false;
        msz[i][j].entry_number = 0;
        for (uint8_t k = 0; k < filepos && !found; k++)
        {
          if (strcmp(flash_infos.files[k].name, msz[i][j].filename) == 0)
          {
            msz[i][j].entry_number = k + 1;
            found = true;
          }
        }
      }
    }
}
}

FLASHMEM bool compareFiles(File& file, SerialFlashFile& ffile)
{
  file.seek(0);
  ffile.seek(0);
  unsigned long count = file.size();
  while (count > 0)
  {
    char buf1[128], buf2[128];
    unsigned long n = count;
    if (n > 128)
      n = 128;
    file.read(buf1, n);
    ffile.read(buf2, n);
    if (memcmp(buf1, buf2, n) != 0)
      return false; // differ
    count = count - n;
  }
  return true; // all data identical
}

FLASHMEM void print_flash_stats()
{
  char tmp[6];

  display.setTextSize(1);
  display.setCursor(CHAR_width_small * 42, 1 * CHAR_height_small);
  display.setTextColor(GREY2, COLOR_BACKGROUND);
  display.print("FILES: ");
  display.setTextColor(COLOR_PITCHSMP, COLOR_BACKGROUND);
  print_formatted_number(fm.flash_sum_files, 3);
  display.setCursor(CHAR_width_small * 37, 2 * CHAR_height_small);
  display.setTextColor(GREY2, COLOR_BACKGROUND);
  display.print("TOTAL: ");
  display.setTextColor(COLOR_PITCHSMP, COLOR_BACKGROUND);
  snprintf_P(tmp, sizeof(tmp), PSTR("%05d"), int(flash_infos.capacity / 1024));
  display.print(tmp);
  display.setTextColor(COLOR_CHORDS, COLOR_BACKGROUND);
  display.print(" KB");
  display.setCursor(CHAR_width_small * 38, 3 * CHAR_height_small);
  display.setTextColor(GREY2, COLOR_BACKGROUND);
  display.print("USED: ");
  display.setTextColor(COLOR_PITCHSMP, COLOR_BACKGROUND);
  snprintf_P(tmp, sizeof(tmp), PSTR("%05d"), int(flash_infos.used));
  display.print(tmp);
  display.setTextColor(COLOR_CHORDS, COLOR_BACKGROUND);
  display.print(" KB");
  display.setCursor(CHAR_width_small * 38, 4 * CHAR_height_small);
  display.setTextColor(GREY2, COLOR_BACKGROUND);
  display.print("FREE: ");
  display.setTextColor(COLOR_PITCHSMP, COLOR_BACKGROUND);
  snprintf_P(tmp, sizeof(tmp), PSTR("%05d"), int(flash_infos.capacity / 1024 - flash_infos.used));
  display.print(tmp);
  display.setTextColor(COLOR_CHORDS, COLOR_BACKGROUND);
  display.print(" KB");
}
#endif

FLASHMEM void print_sampler_keyboard(int x, int y)
{
  uint8_t offset[5] = { 1, 2, 2, 4, 6 }; //+ is the offset to left
  int offcount = 0;
  display.console = true;
  // draw white keys
  for (uint8_t i = 0; i < 7; i++)
  {
    if (x * CHAR_width_small + 6 * i < DISPLAY_WIDTH - 8)
      display.fillRect(x * CHAR_width_small + 6 * i, y + 23, 5, 15, COLOR_SYSTEXT); // pianoroll white key
  }
  for (uint8_t i = 0; i < 12; i++)
  {
    if (seq.piano[i] == 1)
    {
      if (x * CHAR_width_small + 4 * i - offset[offcount] < DISPLAY_WIDTH - 10)
        display.fillRect(x * CHAR_width_small + 4 * i - offset[offcount], y + 23, 4, 8, COLOR_BACKGROUND); // BLACK key
      offcount++;
      if (offcount == 5)
        offcount = 0;
    }
  }
}

FLASHMEM void print_note_name_and_octave(uint8_t note)
{
  display.print(noteNames[note % 12][0]);
  if (noteNames[note % 12][1] != '\0')
  {
    display.print(noteNames[note % 12][1]);
  }
  display.print((note / 12) - 1);
  display.print(" ");
}

FLASHMEM uint8_t get_distance(uint8_t a, uint8_t b)
{
  if (a == b)
    return 0;
  else if (a > b)
    return a - b;
  else
    return b - a;
}

FLASHMEM void calc_low_high(uint8_t preset)
{
  uint8_t result = 200;
  uint8_t result_zone = 99;

  for (uint8_t zone = 0; zone < NUM_MULTISAMPLE_ZONES; zone++)
  {
    msz[preset][zone].low = 0;
    msz[preset][zone].high = 0;
  }
  for (uint8_t key = 24; key < 110; key++)
  {
    for (uint8_t zone = 0; zone < NUM_MULTISAMPLE_ZONES; zone++)
    {
      if (get_distance(msz[preset][zone].rootnote, key) < result)
      {
        result = get_distance(msz[preset][zone].rootnote, key);
        result_zone = zone;
      }
    }
    if (msz[preset][result_zone].rootnote > key && msz[preset][result_zone].rootnote != 0)
    {
      if (msz[preset][result_zone].low > msz[preset][result_zone].rootnote - result || msz[preset][result_zone].low == 0)
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

FLASHMEM void sub_MultiSample_setColor(uint8_t row, uint8_t column)
{
  uint16_t temp_color = 0;
  uint16_t temp_background = 0;

  if ((generic_temp_select_menu == row + 3 && seq.selected_track == column && seq.edit_state == false) || (generic_temp_select_menu == row + 3 && column == 99 && seq.edit_state == false))
  {
    temp_color = COLOR_BACKGROUND;
    temp_background = COLOR_SYSTEXT;
  }
  else if ((generic_temp_select_menu == row + 3 && seq.selected_track == column && seq.edit_state))
  {
    temp_background = RED;
    temp_color = COLOR_SYSTEXT;
  }
  else
  {
    temp_background = COLOR_BACKGROUND;
    if (row == 0)
      temp_color = COLOR_PITCHSMP;
    else if (row == 1)
      temp_color = DX_DARKCYAN;
    else if (row == 2)
      temp_color = COLOR_CHORDS;
    else if (row == 3)
      temp_color = COLOR_ARP;
    else if (row == 4)
      temp_color = COLOR_DRUMS;
    else if (row == 5)
      temp_color = GREEN;
    else if (row == 6)
      temp_color = MIDDLEGREEN;
    else if (row == 7)
      temp_color = YELLOW;
  }
  display.setTextColor(temp_color, temp_background);
}

FLASHMEM uint16_t get_multisample_zone_color(uint8_t row)
{
  uint16_t temp_color = 0;
  if (generic_temp_select_menu == row + 3)
    temp_color = RED;
  else if (row == 0)
    temp_color = COLOR_PITCHSMP;
  else if (row == 1)
    temp_color = DX_DARKCYAN;
  else if (row == 2)
    temp_color = COLOR_CHORDS;
  else if (row == 3)
    temp_color = COLOR_ARP;
  else if (row == 4)
    temp_color = COLOR_DRUMS;
  else if (row == 5)
    temp_color = GREEN;
  else if (row == 6)
    temp_color = MIDDLEGREEN;
  else if (row == 7)
    temp_color = YELLOW;
  return temp_color;
}

FLASHMEM void print_multisampler_panbar(uint8_t x, uint8_t y, uint8_t input_value, uint8_t selected_option)
{
  display.console = true;

  if (selected_option == generic_temp_select_menu - 3 && seq.selected_track == 5 && seq.edit_state)
    display.drawRect(CHAR_width_small * x + 4 * CHAR_width_small, 10 * y, 3 * CHAR_width_small + 1, 7, RED);
  else if (selected_option == generic_temp_select_menu - 3 && seq.selected_track == 5)
    display.drawRect(CHAR_width_small * x + 4 * CHAR_width_small, 10 * y, 3 * CHAR_width_small + 1, 7, COLOR_SYSTEXT);
  else
    display.drawRect(CHAR_width_small * x + 4 * CHAR_width_small, 10 * y, 3 * CHAR_width_small + 1, 7, GREY2);

  display.console = true;
  display.fillRect(CHAR_width_small * x + 4 * CHAR_width_small + 1, 10 * y + 1, 3 * CHAR_width_small - 1, 7 - 2, COLOR_BACKGROUND);
  display.console = true;
  if (input_value == 20)
    display.fillRect(CHAR_width_small * x + 4 * CHAR_width_small + 1 + input_value / 2.83, 10 * y + 1, 3, 5, COLOR_SYSTEXT);
  else
    display.fillRect(CHAR_width_small * x + 4 * CHAR_width_small + 1 + input_value / 2.83, 10 * y + 1, 3, 5, COLOR_PITCHSMP);
  display.console = false;
}

FLASHMEM void print_multisampler_tunebar(uint8_t x, uint8_t y, uint8_t input_value, uint8_t selected_option)
{
  display.console = true;

  if (selected_option == generic_temp_select_menu - 3 && seq.selected_track == 7 && seq.edit_state)
    display.drawRect(CHAR_width_small * x + 4 * CHAR_width_small, 10 * y, 3 * CHAR_width_small + 1, 7, RED);
  else if (selected_option == generic_temp_select_menu - 3 && seq.selected_track == 7)
    display.drawRect(CHAR_width_small * x + 4 * CHAR_width_small, 10 * y, 3 * CHAR_width_small + 1, 7, COLOR_SYSTEXT);
  else
    display.drawRect(CHAR_width_small * x + 4 * CHAR_width_small, 10 * y, 3 * CHAR_width_small + 1, 7, GREY2);

  display.console = true;
  display.fillRect(CHAR_width_small * x + 4 * CHAR_width_small + 1, 10 * y + 1, 3 * CHAR_width_small - 1, 7 - 2, COLOR_BACKGROUND);
  display.console = true;
  if (input_value == 100)
    display.fillRect(CHAR_width_small * x + 4 * CHAR_width_small + 1 + input_value / 14, 10 * y + 1, 3, 5, COLOR_SYSTEXT);
  else
    display.fillRect(CHAR_width_small * x + 4 * CHAR_width_small + 1 + input_value / 14, 10 * y + 1, 3, 5, COLOR_PITCHSMP);
  display.console = false;
}

#if defined COMPILE_FOR_FLASH

FLASHMEM void print_msp_zone(uint8_t zone, bool fullrefresh_values)
{
  uint8_t yoffset = 7;
  display.setTextSize(1);

  sub_MultiSample_setColor(zone, 0);
  setCursor_textGrid_small(2, zone + yoffset);
  print_note_name_and_octave(msz[seq.active_multisample][zone].rootnote);
  display.setTextColor(COLOR_SYSTEXT, COLOR_BACKGROUND);
  display.print(" ");

  setCursor_textGrid_small(7, zone + yoffset);
  sub_MultiSample_setColor(zone, 1);
  print_note_name_and_octave(msz[seq.active_multisample][zone].low);
  display.setTextColor(COLOR_SYSTEXT, COLOR_BACKGROUND);
  display.print(" ");

  setCursor_textGrid_small(11, zone + yoffset);
  sub_MultiSample_setColor(zone, 2);
  print_note_name_and_octave(msz[seq.active_multisample][zone].high);
  display.setTextColor(COLOR_SYSTEXT, COLOR_BACKGROUND);
  display.print(" ");
  char tmp[4];
  sub_MultiSample_setColor(zone, 3);

  setCursor_textGrid_small(16, zone + yoffset);
  if (msz[seq.active_multisample][zone].playmode != 0)
  {

    uint8_t offset_x = 0;
    if (msz[seq.active_multisample][zone].playmode == 1)
      offset_x = 1;
    else if (msz[seq.active_multisample][zone].playmode == 3)
      offset_x = 2;
    else if (msz[seq.active_multisample][zone].playmode == 5)
      offset_x = 3;
    else if (msz[seq.active_multisample][zone].playmode == 7)
      offset_x = 4;

    display.write(23);

    display.setCursor(display.getCursorX() - 5 + offset_x, display.getCursorY());
    display.write(24);
    if (offset_x < 4)
    {
      display.console = 1;
      if (seq.edit_state && seq.selected_track == 3)
        display.fillRect(display.getCursorX(), display.getCursorY(), 4 - offset_x, 8, RED);
      else
        display.fillRect(display.getCursorX(), display.getCursorY(), 4 - offset_x, 8, COLOR_BACKGROUND);
    }
  }
  else
  {
    display.write(21);
    display.setCursor(display.getCursorX() - 1, display.getCursorY());
    display.write(22);
  }
  sub_MultiSample_setColor(zone, 4);
  show_smallfont_noGrid((zone + yoffset) * (CHAR_height_small + 2), 19 * CHAR_width_small, 3, itoa(msz[seq.active_multisample][zone].vol, tmp, 10));
  sub_MultiSample_setColor(zone, 5);
  print_multisampler_panbar(19, yoffset + zone, msz[seq.active_multisample][zone].pan, zone);

  sub_MultiSample_setColor(zone, 6);
  show_smallfont_noGrid((zone + yoffset) * (CHAR_height_small + 2), 27 * CHAR_width_small, 3, itoa(msz[seq.active_multisample][zone].rev, tmp, 10));

  sub_MultiSample_setColor(zone, 7);
  print_multisampler_tunebar(27, yoffset + zone, msz[seq.active_multisample][zone].tune, zone);

  setCursor_textGrid_small(36, zone + yoffset);
  sub_MultiSample_setColor(zone, 8);
  display.print("[");
  if (msz[seq.active_multisample][generic_temp_select_menu - 3].entry_number == 0 && seq.edit_state && generic_temp_select_menu > 2 && seq.selected_track == 8)
    show_smallfont_noGrid((zone + yoffset) * (CHAR_height_small + 2), 37 * CHAR_width_small, 15, "CLEAR ZONE ?");
  else
    show_smallfont_noGrid((zone + yoffset) * (CHAR_height_small + 2), 37 * CHAR_width_small, 15, msz[seq.active_multisample][zone].filename);
  setCursor_textGrid_small(51, zone + yoffset);
  display.print("]");
  display.console = true;
  if (msz[seq.active_multisample][zone].low == 0 && msz[seq.active_multisample][zone].high == 0)
    display.fillRect(1,
      185 + zone * 5,
      DISPLAY_WIDTH - 2, 5, COLOR_BACKGROUND);
  else
  {
    if (msz[seq.active_multisample][zone].low < 25)
      msz[seq.active_multisample][zone].low = 24;
    if (msz[seq.active_multisample][zone].high > 109)
      msz[seq.active_multisample][zone].high = 109;

    display.console = true;
    display.fillRect(1, 185 + zone * 5, 2 * CHAR_width_small + msz[seq.active_multisample][zone].low * 3.5 - (24 * 3.5) - 1, 5, COLOR_BACKGROUND);
    display.console = true;
    display.fillRect(2 * CHAR_width_small + msz[seq.active_multisample][zone].low * 3.5 - (24 * 3.5), 185 + zone * 5,
      (msz[seq.active_multisample][zone].high - msz[seq.active_multisample][zone].low) * 3.5 + 2.5 + 1, 5, get_multisample_zone_color(zone));
    display.console = true;
    display.fillRect(2 * CHAR_width_small + msz[seq.active_multisample][zone].high * 3.5 - (24 * 3.5) + 3.5 + 1, 185 + zone * 5,
      DISPLAY_WIDTH - (msz[seq.active_multisample][zone].high * 3.5) + (18 * 3.5), 5, COLOR_BACKGROUND);
    display.console = true;
    display.fillRect(2 * CHAR_width_small + msz[seq.active_multisample][zone].rootnote * 3.5 - (24 * 3.5) + 1, 185 + zone * 5 + 1,
      3.5 + 1, 5 - 2, COLOR_SYSTEXT);
  }
}
FLASHMEM void print_msp_all_zones()
{
  for (uint8_t zone = 0; zone < NUM_MULTISAMPLE_ZONES; zone++)
  {
    print_msp_zone(zone, true);
  }
}

FLASHMEM void UI_func_MultiSamplePlay(uint8_t param)
{
  if (LCDML.FUNC_setup()) // ****** SETUP *********
  {
    seq.active_multisample = 0;
    seq.edit_state = false;
    generic_temp_select_menu = 0;
    // calc_low_high(seq.active_multisample);
    display.fillScreen(COLOR_BACKGROUND);
    encoderDir[ENC_R].reset();
    display.setTextSize(1);
    border0();

    display.setTextColor(COLOR_SYSTEXT, COLOR_BACKGROUND);
    for (uint8_t oct = 0; oct < 8; oct++)
    {
      print_sampler_keyboard(oct * 7 + 2, 135);
      display.setCursor(CHAR_width_small * 2 + oct * 42, 175);
      display.print(oct + 1);
    }
    display.setTextSize(1);
    display.setTextColor(GREY2, COLOR_BACKGROUND);
    setCursor_textGrid_small(2, 4);
    display.print(F("Volume"));
    setCursor_textGrid_small(22, 4);
    display.print(F("MIDI Channel"));

    flash_loadDirectory();
    print_flash_stats();
    display.setTextColor(COLOR_SYSTEXT, COLOR_BACKGROUND);

    display.setCursor(2 * CHAR_width_small, (6) * (CHAR_height_small + 2) - 2);
    display.print(F("ROOT"));
    display.setCursor(7 * CHAR_width_small, (6) * (CHAR_height_small + 2) - 2);
    display.print(F("LOW"));
    display.setCursor(11 * CHAR_width_small, (6) * (CHAR_height_small + 2) - 2);
    display.print(F("HIGH"));
    display.setCursor(16 * CHAR_width_small, (6) * (CHAR_height_small + 2) - 2);
    display.print(F("PM"));
    display.setCursor(19 * CHAR_width_small, (6) * (CHAR_height_small + 2) - 2);
    display.print(F("VOL"));
    display.setCursor(23 * CHAR_width_small, (6) * (CHAR_height_small + 2) - 2);
    display.print(F("PAN"));
    display.setCursor(27 * CHAR_width_small, (6) * (CHAR_height_small + 2) - 2);
    display.print(F("REV"));
    display.setCursor(31 * CHAR_width_small, (6) * (CHAR_height_small + 2) - 2);
    display.print(F("TUNE"));
    display.setCursor(36 * CHAR_width_small + 2, (6) * (CHAR_height_small + 2) - 2);
    display.print(F("FILENAME"));

    print_msp_all_zones();
  }
  if (LCDML.FUNC_loop()) // ****** LOOP *********
  {
    multisample_zone_t* zoneEdited = &msz[seq.active_multisample][generic_temp_select_menu - 3];
    if ((LCDML.BT_checkDown() && encoderDir[ENC_R].Down()) || (LCDML.BT_checkUp() && encoderDir[ENC_R].Up()))
    {
      set_state_dir();
      if (seq.edit_state && generic_temp_select_menu == 0)
      {
        seq.active_multisample = calc_val[state_dir](seq.active_multisample, ENCODER[ENC_R].speed(), 0, NUM_MULTISAMPLES);
        if (seq.active_multisample < NUM_MULTISAMPLES)
        {
          calc_low_high(seq.active_multisample);
          print_msp_all_zones();
        }
        else
        {
          if (seq.active_multisample == NUM_MULTISAMPLES)
          {
            helptext_r("PUSH JUMPS TO RENAME MENU");
            display.setTextSize(2);
            show(1, 1, 15, "JUMP TO RENAME?");
          }
        }
      }
      if (seq.edit_state && generic_temp_select_menu == 1)
      {
        msp[seq.active_multisample].sound_intensity = calc_val[state_dir](msp[seq.active_multisample].sound_intensity, ENCODER[ENC_R].speed(), 0, 100);
      }
      if (seq.edit_state && generic_temp_select_menu == 2)
      {
        msp[seq.active_multisample].midi_channel = calc_val[state_dir](msp[seq.active_multisample].midi_channel, 1, 0, 16);
      }

      if (seq.edit_state && generic_temp_select_menu > 2 && seq.selected_track == 8) // file name selection
      {
        zoneEdited->entry_number = calc_val[state_dir](zoneEdited->entry_number, ENCODER[ENC_R].speed(), 0, fm.flash_sum_files - 1);

        if (seq.running == true)
          stop_all_drum_slots();

        fill_msz(flash_infos.files[zoneEdited->entry_number - 1].name, seq.active_multisample, generic_temp_select_menu - 3);
      }
      else if (seq.edit_state && generic_temp_select_menu > 2 && seq.selected_track == 7) // tune/pitch
      {
        zoneEdited->tune = calc_val[state_dir](zoneEdited->tune, ENCODER[ENC_R].speed(), 1, 200);
      }
      if (seq.edit_state && generic_temp_select_menu > 2 && seq.selected_track == 6) // reverb send selection
      {
        zoneEdited->rev = calc_val[state_dir](zoneEdited->rev, ENCODER[ENC_R].speed(), 0, 100);
      }
      else if (seq.edit_state && generic_temp_select_menu > 2 && seq.selected_track == 5) // pan selection
      {
        zoneEdited->pan = calc_val[state_dir](zoneEdited->pan, ENCODER[ENC_R].speed(), PANORAMA_MIN, PANORAMA_MAX);
      }
      else if (seq.edit_state && generic_temp_select_menu > 2 && seq.selected_track == 4) // volume selection
      {
        zoneEdited->vol = calc_val[state_dir](zoneEdited->vol, ENCODER[ENC_R].speed(), 0, 100);
      }
      else if (seq.edit_state && generic_temp_select_menu > 2 && seq.selected_track == 3) // playmode selection
      {
        // zoneEdited->playmode = !zoneEdited->playmode;
        zoneEdited->playmode = calc_val[state_dir](zoneEdited->playmode, 1, 0, 6);
        if (zoneEdited->playmode > 1 && zoneEdited->playmode % 2 == 0)
        {
          if (state_dir == 0)
            zoneEdited->playmode++;
          else if (state_dir == 1)
            zoneEdited->playmode--;
        }
      }
      else if (seq.edit_state && generic_temp_select_menu > 2 && seq.selected_track == 2) // high selection
      {
        zoneEdited->high = calc_val[state_dir](zoneEdited->high, ENCODER[ENC_R].speed(), 24, 109);
      }
      else if (seq.edit_state && generic_temp_select_menu > 2 && seq.selected_track == 1) // low selection
      {
        zoneEdited->low = calc_val[state_dir](zoneEdited->low, ENCODER[ENC_R].speed(), 24, 109);
      }
      else if (seq.edit_state && generic_temp_select_menu > 2 && seq.selected_track == 0) // root note selection
      {
        zoneEdited->rootnote = calc_val[state_dir](zoneEdited->rootnote, ENCODER[ENC_R].speed(), 24, 109);
      }
      else if (seq.edit_state == false) // no option is selected, scroll parameter rows
      {
        generic_temp_select_menu = calc_val[state_dir](generic_temp_select_menu, 1, 0, 11);
      }
    }

    if (LCDML.BT_checkEnter())
    {
      if (seq.edit_state && generic_temp_select_menu == 0 && seq.active_multisample == NUM_MULTISAMPLES)
      {
        seq.active_multisample = 0;
        LCDML.OTHER_jumpToFunc(UI_func_set_multisample_name);
      }
      else if (zoneEdited->entry_number == 0 && seq.edit_state && generic_temp_select_menu > 2 && seq.selected_track == 8)
      {
        // clear zone
        msz[seq.active_multisample][generic_temp_select_menu - 3].rootnote = 0;
        msz[seq.active_multisample][generic_temp_select_menu - 3].rev = 0;
        msz[seq.active_multisample][generic_temp_select_menu - 3].high = 0;
        msz[seq.active_multisample][generic_temp_select_menu - 3].low = 0;
        msz[seq.active_multisample][generic_temp_select_menu - 3].vol = 100;
        msz[seq.active_multisample][generic_temp_select_menu - 3].tune = 100;
      }
      if (seq.edit_state == false)
        seq.edit_state = true;
      else
        seq.edit_state = false;
    }
    if (generic_temp_select_menu > 2)
      helptext_l("MOVE X");
    else
      helptext_l(back_text);

    if (generic_temp_select_menu > 2)
      helptext_r("MOVE Y");
    else if (generic_temp_select_menu == 0 && seq.active_multisample != NUM_MULTISAMPLES)
      helptext_r("SEL. MULTISAMPLE");
    else if (generic_temp_select_menu == 1)
      helptext_r("CHANGE VOLUME");
    else if (generic_temp_select_menu == 2)
      helptext_r("SEL. MIDI CHANNEL");
    display.setTextSize(2);

    if (seq.active_multisample < NUM_MULTISAMPLES)
    {
      display.setTextColor(GREY2, COLOR_BACKGROUND);
      setCursor_textGrid(1, 1);
      if (generic_temp_select_menu == 0)
        setModeColor(0);
      print_formatted_number(seq.active_multisample, 2);
      setCursor_textGrid(3, 1);
      display.setTextColor(GREY2, COLOR_BACKGROUND);
      display.print(F("["));
      setCursor_textGrid(15, 1);
      display.print(F("]"));
      display.setTextColor(COLOR_SYSTEXT, COLOR_BACKGROUND);

      show(1, 4, 11, msp[seq.active_multisample].name);

      setModeColor(1);
      display.setTextSize(1);
      setCursor_textGrid_small(9, 4);
      print_small_intbar(9, 4, msp[seq.active_multisample].sound_intensity, 1, 1, 0);

      setModeColor(2);
      setCursor_textGrid_small(35, 4);
      if (msp[seq.active_multisample].midi_channel != 0)
      {
        print_formatted_number(msp[seq.active_multisample].midi_channel, 2);
        display.print(F("  "));
      }
      else
        display.print(F("OMNI"));

      if (seq.edit_state == false)
      {
        if (generic_temp_select_menu == 2)
        {
          print_msp_zone(0, false);
        }
        else if (generic_temp_select_menu == 3)
        {
          print_msp_zone(generic_temp_select_menu - 3, false);
          print_msp_zone(generic_temp_select_menu - 2, false);
        }
        else if (generic_temp_select_menu > 3 && generic_temp_select_menu < 10)
        {
          print_msp_zone(generic_temp_select_menu - 4, false);
          print_msp_zone(generic_temp_select_menu - 3, false);
          print_msp_zone(generic_temp_select_menu - 2, false);
        }
        else if (generic_temp_select_menu == 10)
        {
          print_msp_zone(generic_temp_select_menu - 4, false);
          print_msp_zone(generic_temp_select_menu - 3, false);
        }
      }
      else
      {
        if (generic_temp_select_menu > 2)
          print_msp_zone(generic_temp_select_menu - 3, false);
      }
    }
  }
  if (LCDML.FUNC_close()) // ****** STABLE END *********
  {
    encoderDir[ENC_R].reset();
    // seq.scrollpos = 0;
    display.fillScreen(COLOR_BACKGROUND);
    display.setTextColor(COLOR_SYSTEXT, COLOR_BACKGROUND);
  }
}
#else
FLASHMEM void UI_func_MultiSamplePlay(uint8_t param)
{
  if (LCDML.FUNC_setup()) // ****** SETUP *********
  {
    display.fillScreen(COLOR_BACKGROUND);
    encoderDir[ENC_R].reset();
    helptext_l(back_text);
    setCursor_textGrid(1, 1);
    display.setTextSize(1);
    display.setTextColor(RED, COLOR_BACKGROUND);
    display.print(F("ONLY WORKING WITH SERIAL FLASH CHIP"));
    setCursor_textGrid(1, 2);
    display.print(F("AND COMPILED FOR FLASH CONFIG"));
    display.setTextColor(COLOR_SYSTEXT, COLOR_BACKGROUND);
  }

  if (LCDML.FUNC_loop()) // ****** LOOP *********
  {
    ;
  }

  if (LCDML.FUNC_close()) // ****** STABLE END *********
  {
    encoderDir[ENC_R].reset();
    display.fillScreen(COLOR_BACKGROUND);
  }
}
#endif

FLASHMEM void sd_card_count_files_from_directory(const char* dir_name)
{
  fm.sd_sum_files = 0;
  File dir = SD.open(dir_name);

  while (true)
  {
    File entry = dir.openNextFile();
    if (!entry)
    {
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

  dir.close();
  }

FLASHMEM void sd_go_parent_folder()
{
  if (fm.sd_folder_depth < 2)
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

  fm.sd_selected_file = 0;
}
FLASHMEM void sd_update_display()
{
  sd_printDirectory(true);

  display.setCursor(CHAR_width_small * 24, 1 * CHAR_height_small);
  display.setTextColor(COLOR_PITCHSMP, COLOR_BACKGROUND);
  print_formatted_number(fm.sd_sum_files, 3);
  show_smallfont_noGrid(3 * CHAR_height_small, CHAR_width_small * 7, 20, fm.sd_new_name);
  show_smallfont_noGrid(5 * CHAR_height_small, CHAR_width_small * 1, 20, fm.sd_temp_name);

}

FLASHMEM void UI_func_file_manager(uint8_t param)
{
  uint32_t volumesize;

  if (LCDML.FUNC_setup()) // ****** SETUP *********
  {
    // handleStop(); //test 20/03/2023 for remote console crash when opening file manager while sequencer is playing
    fm.sd_mode = FM_BROWSE_FILES;
    fm.active_window = 0;
    // fm.sd_skip_files = 0;
    // fm.sd_selected_file = 0;
    strcpy(fm.sd_prev_dir, "");
    // strcpy(fm.sd_new_name, "/");

    display.fillScreen(COLOR_BACKGROUND);
    encoderDir[ENC_R].reset();
    display.setTextSize(1);
    border0();
    display.setCursor(CHAR_width_small * 1, 1 * CHAR_height_small);
    display.print(F("SD CARD"));
    display.setTextColor(GREY2);
    display.setCursor(CHAR_width_small * 17, 1 * CHAR_height_small);
    display.print("FILES:");
    display.setCursor(CHAR_width_small * 1, 2 * CHAR_height_small);
    display.print(F("TYPE: "));
    display.setTextColor(COLOR_PITCHSMP);
    display.print(sdcard_infos.type);
    display.setCursor(CHAR_width_small * 12, 2 * CHAR_height_small);
    volumesize = volume.blocksPerCluster(); // clusters are collections of blocks
    volumesize *= volume.clusterCount();    // we'll have a lot of clusters
    display.setTextColor(GREY2);
    display.print("TOTAL: ");
    display.setTextColor(COLOR_PITCHSMP);
    volumesize /= 1024;
    display.print(volumesize);
    display.setTextColor(COLOR_CHORDS);
    display.print(" MB");
    display.setTextSize(1);
    display.setCursor(CHAR_width_small * 1, 3 * CHAR_height_small);
    display.setTextColor(COLOR_CHORDS, COLOR_BACKGROUND);
    display.print(F("PATH:"));
    // display.setCursor(CHAR_width_small * 1, 4 * CHAR_height_small);
    // display.setTextColor(GREY2);
    // display.print(F("CURRENT FILE/FOLDER :"));

#ifdef COMPILE_FOR_FLASH
    // fm.flash_skip_files = 0;
    // fm.flash_selected_file = 0;
    flash_loadDirectory();
    display.setTextColor(COLOR_SYSTEXT);
    display.setCursor(CHAR_width_small * 29, 1 * CHAR_height_small);
    display.print(F("FLASH"));
    print_flash_stats();
    flash_printDirectory();
#endif
    print_file_manager_buttons();
    print_file_manager_active_border();
  }
  if (LCDML.FUNC_loop()) // ****** LOOP *********
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
        }
        else
          fm.sd_selected_file = constrain(fm.sd_selected_file + 1, 0, fm.sd_cap_rows);
      }
      else if (LCDML.BT_checkUp() && fm.active_window == 0) // left window, SDCARD
      {
        if (fm.sd_selected_file == 0 && fm.sd_skip_files > 0)
          fm.sd_skip_files--;
        else if (fm.sd_selected_file == 0 && fm.sd_skip_files == 0)
        {
          if (fm.sd_folder_depth > 0)
            fm.sd_parent_folder = true;
        }
        fm.sd_selected_file = constrain(fm.sd_selected_file - 1, 0, fm.sd_cap_rows);
      }
#ifdef COMPILE_FOR_FLASH
      if (LCDML.BT_checkDown() && fm.active_window == 1) // right window, FLASH
      {
        if (fm.flash_selected_file == fm.flash_cap_rows && fm.flash_cap_rows > 8 && fm.flash_skip_files < fm.flash_sum_files - fm.flash_cap_rows - 1)
          fm.flash_skip_files++;

        fm.flash_selected_file = constrain(fm.flash_selected_file + 1, 0, fm.flash_cap_rows);
      }
      else if (LCDML.BT_checkUp() && fm.active_window == 1) // // right window, FLASH
      {
        if (fm.flash_selected_file == 0 && fm.flash_skip_files > 0)
          fm.flash_skip_files--;

        fm.flash_selected_file = constrain(fm.flash_selected_file - 1, 0, fm.flash_cap_rows);
      }
#endif
    }
    if (LCDML.BT_checkEnter() && fm.active_window == 0) // left window, SDCARD
    {
#ifdef COMPILE_FOR_FLASH
      if (fm.sd_mode == FM_COPY_PRESETS) // copy presets dir from SD to flash
      {
        display.console = true;
        display.fillRect(CHAR_width_small * 1, CHAR_height_small * 6, DISPLAY_WIDTH / 2 - CHAR_width_small, CHAR_height_small * 16, COLOR_BACKGROUND);
        encoderDir[ENC_R].reset();
        uint8_t screenline = 0;

        File rootdir = SD.open("/DRUMS");
        while (1)
        {
          // open a file from the SD card
          File f = rootdir.openNextFile();
          if (!f)
            break;
          const char* filename = f.name();

          // filter out the ._ mac files
          if (strstr(filename, "._") != NULL)
            continue;

          if (screenline > 10)
            screenline = 0;
          setCursor_textGrid_small(1, 6 + screenline);

          display.setTextColor(COLOR_SYSTEXT, COLOR_BACKGROUND);
          // if (filename[0] != 46 && filename[1] != 95)
          if (filename[0] != 46)
          {
            display.print(filename);
            fill_up_with_spaces_left_window_filemanager();
            screenline++;
          }
          unsigned long length = f.size();
          // check if this file is already on the Flash chip
          if (SerialFlash.exists(filename))
          {
#ifdef DEBUG
            LOG.println(F("  already exists on the Flash chip"));
#endif
            SerialFlashFile ff = SerialFlash.open(filename);
            if (ff && ff.size() == f.size())
            {
#ifdef DEBUG
              LOG.println(F("  size is the same, comparing data..."));
#endif
              if (compareFiles(f, ff) == true)
              {
#ifdef DEBUG
                LOG.println(F("  files are identical :)"));
#endif
                f.close();
                ff.close();
                continue; // advance to next file
              }
              else
              {
#ifdef DEBUG
                LOG.println(F("  files are different"));
#endif
              }
              }
            else
            {
#ifdef DEBUG
              LOG.print(F("  size is different, "));
              LOG.print(ff.size());
              LOG.println(F(" bytes"));
#endif
            }
            // delete the copy on the Flash chip, if different
#ifdef DEBUG
            LOG.println(F("  delete file from Flash chip"));
#endif
            SerialFlash.remove(filename);
          }
          // if (filename[0] != 46 && filename[1] != 95)
          if (filename[0] != 46)
          {
            // create the file on the Flash chip and copy data
            if (SerialFlash.create(filename, length))
            {
              SerialFlashFile ff = SerialFlash.open(filename);
              if (ff)
              {
#ifdef DEBUG
                LOG.print(F("  copying"));
#endif
                // copy data loop
                unsigned long count = 0;
                display.console = true;
                while (count < length)
                {
                  char buf[256];
                  unsigned int n;
                  n = f.read(buf, 256);
                  ff.write(buf, n);
                  count = count + n;
                  if (count % 5120 == 0)
                    display.fillRect(CHAR_width_small * 38, CHAR_height_small * 7, count / (f.size() / (14 * CHAR_width_small)), 8, RED);
                }
                ff.close();
                display.fillRect(CHAR_width_small * 38 - 2, CHAR_height_small * 7, (14 * CHAR_width_small) + 4, 8, COLOR_BACKGROUND);
                flash_loadDirectory();
                print_flash_stats();
                flash_printDirectory();
              }
              else
              {
#ifdef DEBUG
                LOG.println(F("  error opening freshly created file!"));
#endif
              }
            }
            else
            {
#ifdef DEBUG
              LOG.println(F("  unable to create file"));
#endif
          }
        }
          f.close();
        }
        rootdir.close();
        display.console = true;
        display.fillRect(CHAR_width_small * 1, CHAR_height_small * 6, DISPLAY_WIDTH / 2 - CHAR_width_small, CHAR_height_small * 16, COLOR_BACKGROUND);
        flash_loadDirectory();
        print_flash_stats();
        flash_printDirectory();
#ifdef DEBUG
        LOG.println(F("Finished All Files"));
#endif
      }
      else
#endif
        if (fm.sd_is_folder)
        {
          if (fm.sd_temp_name[0] == 0x2E && fm.sd_temp_name[1] == 0x2E) // return to parent folder
          {
            if (fm.sd_folder_depth < 2)
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
          // is a file
        {
          if (fm.sd_mode == FM_DELETE_FILE)
          {
            strcpy(fm.sd_full_name, fm.sd_new_name);
            strcat(fm.sd_full_name, "/");
            strcat(fm.sd_full_name, fm.sd_temp_name);
            SD.remove(fm.sd_full_name);
            load_sd_directory();
          }
#ifdef COMPILE_FOR_FLASH
          else if (fm.sd_mode == FM_COPY_TO_FLASH)
          {
            strcpy(fm.sd_full_name, fm.sd_new_name);
            strcat(fm.sd_full_name, "/");
            strcat(fm.sd_full_name, fm.flash_temp_name);
            File f = SD.open(fm.sd_full_name);
            const char* filename = f.name();
            unsigned long length = f.size();
            // check if this file is already on the Flash chip
            if (SerialFlash.exists(filename))
            {
#ifdef DEBUG
              LOG.println(F("  already exists on the Flash chip"));
#endif
              SerialFlashFile ff = SerialFlash.open(filename);
              if (ff && ff.size() == f.size())
              {
#ifdef DEBUG
                LOG.println(F("  size is the same, comparing data..."));
#endif
                if (compareFiles(f, ff) == true)
                {
#ifdef DEBUG
                  LOG.println(F("  files are identical :)"));
#endif
                  f.close();
                  ff.close();
                }
                else
                {
#ifdef DEBUG
                  LOG.println(F("  files are different"));
#endif
                }
              }
              else
              {
#ifdef DEBUG
                LOG.print(F("  size is different, "));
                LOG.print(ff.size());
                LOG.println(F(" bytes"));
#endif
            }
              // delete the copy on the Flash chip, if different
#ifdef DEBUG
              LOG.println(F("  delete file from Flash chip"));
#endif
              SerialFlash.remove(filename);
          }
            else
            {
              // create the file on the Flash chip and copy data
              if (SerialFlash.create(filename, length))
              {
                SerialFlashFile ff = SerialFlash.open(filename);
                if (ff)
                {
#ifdef DEBUG
                  LOG.print(F("  copying"));
#endif
                  // copy data loop
                  unsigned long count = 0;
                  display.console = true;
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
                }
                else
                {
#ifdef DEBUG
                  LOG.println(F("  error opening freshly created file!"));
#endif
                }
                f.close();
                // display.fillRect(241, 80, 238, 8, COLOR_BACKGROUND);
                flash_loadDirectory();
                print_flash_stats();
                flash_printDirectory();
              }
        }
    }
#endif
  }
}
    if (LCDML.BT_checkEnter() && fm.sd_mode == FM_PLAY_SAMPLE) // preview - compiled for flash
    {
      preview_sample();
    }
    // show files
    if (fm.active_window == 0)
    {
      if (fm.sd_new_name[0] != 0x2f)
        fm.sd_new_name[0] = 0x2f;
      sd_printDirectory(false);

      display.setCursor(CHAR_width_small * 24, 1 * CHAR_height_small);
      display.setTextColor(COLOR_PITCHSMP, COLOR_BACKGROUND);
      print_formatted_number(fm.sd_sum_files, 3);
      show_smallfont_noGrid(3 * CHAR_height_small, CHAR_width_small * 7, 20, fm.sd_new_name);
      // show_smallfont_noGrid(5 * CHAR_height_small, CHAR_width_small * 1, 20, fm.sd_temp_name);
    }
#ifdef COMPILE_FOR_FLASH
    else if (fm.active_window == 1)
    {
      flash_printDirectory();
    }
#endif
    // display.setCursor(CHAR_width_small * 14, 4 * CHAR_height_small);
    // display.setTextColor(fm.sd_is_folder ? GREEN : GREY2);
    // display.print(F("FOLDER"));
    // display.setCursor(CHAR_width_small * 9, 4 * CHAR_height_small);
    // display.setTextColor(fm.sd_is_folder ? GREY2 : GREEN);
    // display.print(F("FILE"));
    // display.setTextColor(fm.sd_is_folder ? COLOR_PITCHSMP : COLOR_SYSTEXT, COLOR_BACKGROUND);
}

  if (LCDML.FUNC_close()) // ****** STABLE END *********
  {
    encoderDir[ENC_R].reset();
    display.fillScreen(COLOR_BACKGROUND);
    display.setTextColor(COLOR_SYSTEXT, COLOR_BACKGROUND);
  }
}

FLASHMEM void UI_func_midi_soft_thru(uint8_t param)
{
  static uint8_t old_soft_midi_thru;
  if (LCDML.FUNC_setup()) // ****** SETUP *********
  {
    encoderDir[ENC_R].reset();
    setCursor_textGrid(1, 1);
    display.print(F("MIDI Soft THRU"));
  }
  if (LCDML.FUNC_loop()) // ****** LOOP *********
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
  if (LCDML.FUNC_close()) // ****** STABLE END *********
  {
    encoderDir[ENC_R].reset();
    if (old_soft_midi_thru != configuration.sys.soft_midi_thru)
    {
      save_sys_flag = true;
      save_sys = 0;
    }
  }
}

FLASHMEM void _print_midi_channel(uint8_t midi_channel)
{
  if (midi_channel == MIDI_CHANNEL_OMNI)
  {
    display.print(F("OMNI"));
  }
  else
  {
    print_formatted_number(midi_channel, 2);
    display.print(F("  "));
  }
}
FLASHMEM void _show_midi_channel(char* text, uint8_t line, uint8_t midi_channel)
{
  setCursor_textGrid(1, line);
  display.setTextColor(GREY2);
  display.print(text);
  display.setTextColor(COLOR_SYSTEXT);
  setCursor_textGrid(20, line);
  _print_midi_channel(midi_channel);
}

FLASHMEM void UI_func_sd_content_not_found(uint8_t param)
{
  if (LCDML.FUNC_setup())
  {
    display.fillScreen(COLOR_BACKGROUND);
    encoderDir[ENC_R].reset();
    generic_temp_select_menu = 0;
    configuration.sys.screen_saver_mode = 4; // no screensaver
    LCDML.SCREEN_disable();
    display.setTextColor(COLOR_PITCHSMP, COLOR_BACKGROUND);
    display.setTextSize(1);
    setCursor_textGrid_small(2, 1);
    display.print(F("WELCOME TO MDT! "));
    display.setTextColor(GREY2, COLOR_BACKGROUND);
    display.print(F("UNFORTUNATELY THERE IS AN ISSUE:"));
    display.setTextSize(2);
    setCursor_textGrid_small(2, 3);
    display.setTextColor(RED, COLOR_BACKGROUND);
    display.print(F("*ERROR*"));
    display.setTextColor(COLOR_SYSTEXT, COLOR_BACKGROUND);
    display.setTextSize(1);
    setCursor_textGrid_small(2, 5);
    display.print(F("DEFAULT DATA FROM THE SD CARD "));
    display.setTextColor(COLOR_SYSTEXT, RED);
    display.print(F("NOT FOUND"));
    setCursor_textGrid_small(2, 6);
    display.setTextColor(COLOR_SYSTEXT, COLOR_BACKGROUND);
    display.print(F("MDT "));
    display.setTextColor(COLOR_SYSTEXT, RED);
    display.print(F("CAN NOT"));
    display.setTextColor(COLOR_SYSTEXT, COLOR_BACKGROUND);
    display.print(F(" LOAD ANY PRESET OR PERFORMANCE "));
    display.setTextColor(COLOR_PITCHSMP, COLOR_BACKGROUND);
    display.print(F(":_("));
    display.setTextColor(COLOR_SYSTEXT, COLOR_BACKGROUND);
    setCursor_textGrid_small(2, 9);
    display.print(F("HAVE YOU COPIED THE SD CONTENT TO YOUR SD CARD ? "));
    display.setTextColor(COLOR_PITCHSMP, COLOR_BACKGROUND);
    setCursor_textGrid_small(2, 11);
    display.print(F("Please copy all Files/Directories from:"));
    setCursor_textGrid_small(2, 12);
    display.print(F("/addon/SD/"));
    setCursor_textGrid_small(2, 13);
    display.print(F("to the root of your SD Card (FAT32) and"));
    setCursor_textGrid_small(2, 14);
    display.print(F("insert the card in SD Slot of the Teensy 4.1"));
    setCursor_textGrid_small(2, 16);
    display.setTextColor(COLOR_SYSTEXT, COLOR_BACKGROUND);
    display.print(F("PLEASE ALSO CHECK:"));
    display.setTextColor(COLOR_PITCHSMP, COLOR_BACKGROUND);
    setCursor_textGrid_small(2, 18);
    display.print(F("codeberg.org/positionhigh/MicroDexed-touch/releases"));
    setCursor_textGrid_small(2, 19);
    display.setTextColor(GREY1, COLOR_BACKGROUND);
    display.print(F("and the FAQ for further information"));
    setCursor_textGrid_small(2, 21);
    display.setTextColor(GREY1, COLOR_BACKGROUND);
    display.print(F("DISCORD CHAT: "));
    display.setTextColor(GREY2, COLOR_BACKGROUND);
    display.print(F("https://discord.gg/XCYk5P8GzF"));
    helptext_l(back_text);
  }
  if (LCDML.FUNC_loop())
  {
    if ((LCDML.BT_checkDown() && encoderDir[ENC_R].Down()) || (LCDML.BT_checkUp() && encoderDir[ENC_R].Up()))
    {
    }
    // handle button presses during menu
    if (LCDML.BT_checkEnter() && encoderDir[ENC_R].ButtonShort())
    {
      ;
    }
    else if (LCDML.BT_checkEnter())
    {
      ;
    }
    // button check end
  }
  // ****** STABLE END *********
  if (LCDML.FUNC_close())
  {
    encoderDir[ENC_R].reset();
    display.fillScreen(COLOR_BACKGROUND);
  }
}

FLASHMEM void UI_func_midi_channels(uint8_t param)
{
  if (LCDML.FUNC_setup())
  {
    display.fillScreen(COLOR_BACKGROUND);
    encoderDir[ENC_R].reset();
    generic_temp_select_menu = 0;
    setCursor_textGrid(1, 1);
    display.setTextSize(2);
    display.print(F("MIDI channels"));
    helptext_l(back_text);
    helptext_r("EDIT");
    display.setTextSize(2);

    char buf[16];
    for (uint8_t i = 0; i < 2; i++)
    {
      snprintf_P(buf, sizeof(buf), PSTR("%s%d"), F("Dexed #"), i + 1);
      _show_midi_channel(buf, 3 + i, configuration.dexed[i].midi_channel);
    }

    snprintf_P(buf, sizeof(buf), PSTR("%s"), F("E-Piano"));
    _show_midi_channel(buf, 5, configuration.epiano.midi_channel);

    for (uint8_t i = 0; i < 2; i++)
    {
      snprintf_P(buf, sizeof(buf), PSTR("%s%d"), F("MicroSynth #"), i + 1);
      _show_midi_channel(buf, 6 + i, microsynth[i].midi_channel);
    }

    snprintf_P(buf, sizeof(buf), PSTR("%s"), F("Braids"));
    _show_midi_channel(buf, 8, braids_osc.midi_channel);

    for (uint8_t i = 0; i < 2; i++)
    {
      snprintf_P(buf, sizeof(buf), PSTR("%s%d"), F("MultiSample #"), i + 1);
      _show_midi_channel(buf, 9 + i, msp[i].midi_channel);
    }

    snprintf_P(buf, sizeof(buf), PSTR("%s"), F("Drums"));
    _show_midi_channel(buf, 11, drum_midi_channel);
  }
  if (LCDML.FUNC_loop())
  {
    if ((LCDML.BT_checkDown() && encoderDir[ENC_R].Down()) || (LCDML.BT_checkUp() && encoderDir[ENC_R].Up()))
    {
      set_state_dir();
      if (seq.edit_state == 0)
        generic_temp_select_menu = calc_val[state_dir](generic_temp_select_menu, 1, 0, 8);
      else if (generic_temp_select_menu == 0)
        configuration.dexed[0].midi_channel = calc_val[state_dir](configuration.dexed[0].midi_channel, 1, 0, 16);
      else if (generic_temp_select_menu == 1)
        configuration.dexed[1].midi_channel = calc_val[state_dir](configuration.dexed[1].midi_channel, 1, 0, 16);
      else if (generic_temp_select_menu == 2)
        configuration.epiano.midi_channel = calc_val[state_dir](configuration.epiano.midi_channel, 1, 0, 16);
      else if (generic_temp_select_menu == 3)
        microsynth[0].midi_channel = calc_val[state_dir](microsynth[0].midi_channel, 1, 0, 16);
      else if (generic_temp_select_menu == 4)
        microsynth[1].midi_channel = calc_val[state_dir](microsynth[1].midi_channel, 1, 0, 16);
      else if (generic_temp_select_menu == 5)
        braids_osc.midi_channel = calc_val[state_dir](braids_osc.midi_channel, 1, 0, 16);
      else if (generic_temp_select_menu == 6)
        msp[0].midi_channel = calc_val[state_dir](msp[0].midi_channel, 1, 0, 16);
      else if (generic_temp_select_menu == 7)
        msp[1].midi_channel = calc_val[state_dir](msp[1].midi_channel, 1, 0, 16);
      else if (generic_temp_select_menu == 8)
        drum_midi_channel = calc_val[state_dir](drum_midi_channel, 1, 0, 16);
    }
    // handle button presses during menu
    if (LCDML.BT_checkEnter() && encoderDir[ENC_R].ButtonShort())
    {
      if (seq.edit_state == 0)
        seq.edit_state = 1;
      else
        seq.edit_state = 0;
    }
    else if (LCDML.BT_checkEnter())
    {
      ;
    }
    // button check end
    display.setTextSize(2);
    if (menu_item_check(0))
    {
      setModeColor(0);
      setCursor_textGrid(20, 3);
      _print_midi_channel(configuration.dexed[0].midi_channel);
    }
    if (menu_item_check(1))
    {
      setModeColor(1);
      setCursor_textGrid(20, 4);
      _print_midi_channel(configuration.dexed[1].midi_channel);
    }
    if (menu_item_check(2))
    {
      setModeColor(2);
      setCursor_textGrid(20, 5);
      _print_midi_channel(configuration.epiano.midi_channel);
    }
    if (menu_item_check(3))
    {
      setModeColor(3);
      setCursor_textGrid(20, 6);
      _print_midi_channel(microsynth[0].midi_channel);
    }
    if (menu_item_check(4))
    {
      setModeColor(4);
      setCursor_textGrid(20, 7);
      _print_midi_channel(microsynth[1].midi_channel);
    }
    if (menu_item_check(5))
    {
      setModeColor(5);
      setCursor_textGrid(20, 8);
      _print_midi_channel(braids_osc.midi_channel);
    }
    if (menu_item_check(6))
    {
      setModeColor(6);
      setCursor_textGrid(20, 9);
      _print_midi_channel(msp[0].midi_channel);
    }
    if (menu_item_check(7))
    {
      setModeColor(7);
      setCursor_textGrid(20, 10);
      _print_midi_channel(msp[1].midi_channel);
    }
    if (menu_item_check(8))
    {
      setModeColor(8);
      setCursor_textGrid(20, 11);
      _print_midi_channel(drum_midi_channel);
    }

    if (count_omni() != 0)
    {
      display.setTextSize(1);
      setCursor_textGrid_small(8, 21);
      display.setTextColor(COLOR_SYSTEXT, COLOR_BACKGROUND);
      display.print(F("WARNING: "));
      display.setTextColor(RED, COLOR_BACKGROUND);

      display.print(count_omni());
      if (count_omni() == 1)
        display.print(F(" INSTRUMENT IS SET TO OMNI   "));
      else
        display.print(F(" INSTRUMENTS ARE SET TO OMNI "));
      display.setTextColor(GREY2, COLOR_BACKGROUND);
      setCursor_textGrid_small(8, 22);
      display.print(F("ASSIGN EACH INSTRUMENT TO AN UNIQUE"));
      setCursor_textGrid_small(8, 23);
      display.print(F("MIDI CHANNEL AND "));
      display.setTextColor(GREY1, COLOR_BACKGROUND);
      display.print(F("SAVE YOUR PERFORMANCE"));
      display.setTextColor(COLOR_SYSTEXT, COLOR_BACKGROUND);
    }
    else if (count_midi_channel_duplicates(false) != 0)
    {
      display.setTextSize(1);
      setCursor_textGrid_small(8, 21);
      display.setTextColor(COLOR_SYSTEXT, COLOR_BACKGROUND);
      display.print(F("NOTICE: "));
      display.setTextColor(RED, COLOR_BACKGROUND);

      if (count_midi_channel_duplicates(false) != 1)
      {
        display.print(count_midi_channel_duplicates(false));
        display.print(F(" "));
      }
      display.print(F("MIDI CHANNEL"));
      if (count_midi_channel_duplicates(false) != 1)
        display.print(F("S"));
      if (count_midi_channel_duplicates(false) == 1)
      {
        display.print(F(" #"));
        display.print(count_midi_channel_duplicates(true));
      }
      display.setTextColor(GREY2, COLOR_BACKGROUND);

      if (count_midi_channel_duplicates(false) == 1)
        display.print(F(" IS"));
      else
        display.print(F(" ARE"));
      display.print(F(" ASSIGNED"));
      if (count_midi_channel_duplicates(false) == 1)
        display.print(F("  "));

      setCursor_textGrid_small(8, 22);
      display.print(F("TO MULTIPLE INSTRUMENTS - YOU CAN  "));
      setCursor_textGrid_small(8, 23);
      display.print(F("IGNORE THIS WHEN IT IS DONE"));
      display.setTextColor(GREY1, COLOR_BACKGROUND);
      display.print(F(" ON PURPOSE"));
      display.setTextColor(COLOR_SYSTEXT, COLOR_BACKGROUND);
    }

    else if (check_and_confirm_midi_channels == true)
    {
      display.setTextSize(1);
      setCursor_textGrid_small(8, 21);
      display.setTextColor(COLOR_SYSTEXT, COLOR_BACKGROUND);
      display.print(F("ALL MIDI CHANNEL SETTINGS"));
      display.setTextColor(GREEN, COLOR_BACKGROUND);
      display.print(F(" CLEAR !   "));

      display.setTextColor(GREY2, COLOR_BACKGROUND);

      setCursor_textGrid_small(8, 22);
      display.print(F("YOU NOW SHOULD SAVE YOUR CHANGES   "));
      setCursor_textGrid_small(8, 23);
      display.print(F("TO THE CURRENTLY LOADED PERFORMANCE   "));
      display.setTextColor(COLOR_SYSTEXT, COLOR_BACKGROUND);
      check_and_confirm_midi_channels = false;
    }
    else
    {
      if (remote_active)
        display.console = true;
      display.fillRect(7 * CHAR_width_small + 3, 25 * CHAR_height_small + 4, 231, 34, COLOR_BACKGROUND);
      if (remote_active)
        display.console = false;

    }
  }
  // ****** STABLE END *********
  if (LCDML.FUNC_close())
  {
    // generic_active_function = 99;
    check_and_confirm_midi_channels = false;
    encoderDir[ENC_R].reset();
    display.fillScreen(COLOR_BACKGROUND);
  }
}

FLASHMEM void print_screensaver_mode()
{
  if (configuration.sys.screen_saver_mode == ScreenSaver::RANDOM)
    display.print(F("RANDOM "));
  else if (configuration.sys.screen_saver_mode == ScreenSaver::QIX)
    display.print(F("QIX    "));
  else if (configuration.sys.screen_saver_mode == ScreenSaver::CUBE)
    display.print(F("CUBE   "));
  else if (configuration.sys.screen_saver_mode == ScreenSaver::SWARM)
    display.print(F("SWARM  "));
  else if (configuration.sys.screen_saver_mode == ScreenSaver::TERRAIN)
    display.print(F("TERRAIN"));
  else if (configuration.sys.screen_saver_mode == ScreenSaver::DISABLED)
    display.print(F("OFF    "));

  else
    display.print(F("ERROR  "));
}

FLASHMEM void _render_misc_settings()
{
  display.fillScreen(COLOR_BACKGROUND);
  display.setTextSize(2);
  setCursor_textGrid(1, 1);
  display.setTextColor(RED, COLOR_BACKGROUND);
  display.print(F("MISC. SETTINGS"));
  helptext_r("SELECT PARAMETER");

  draw_button_on_grid(42, 1, "TOUCH", "TEST", 0);
  display.setTextColor(COLOR_SYSTEXT, COLOR_BACKGROUND);

  setCursor_textGrid_small(2, 7);
  display.print(F("GAMEPAD/NATIVE KEY SPEED"));
  setCursor_textGrid_small(2, 8);
  display.print(F("SCREENSAVER START"));
  setCursor_textGrid_small(2, 9);
  display.print(F("SCREENSAVER MODE"));
  setCursor_textGrid_small(2, 10);
  display.print(F("DISPLAY ROTATION"));
  setCursor_textGrid_small(2, 11);
  display.print(F("TOUCH SCREEN ROTATION"));
  setCursor_textGrid_small(2, 12);
  display.print(F("REVERSE UI (ENCODERS ON TOP)"));
  setCursor_textGrid_small(42, 7);
  print_formatted_number(configuration.sys.gamepad_speed, 3);
  setCursor_textGrid_small(46, 7);
  display.print(F("ms"));

  setCursor_textGrid_small(2, 13);
  display.print(F("SKIP BOOT ANIMATION"));
  setCursor_textGrid_small(2, 14);
  display.print(F("INVERT COLORS (EXPERIMENTAL)"));

  setCursor_textGrid_small(42, 8);
  display.print(configuration.sys.screen_saver_start);
  setCursor_textGrid_small(42, 9);
  print_screensaver_mode();
  setCursor_textGrid_small(42, 10);
  display.print(configuration.sys.display_rotation);
  setCursor_textGrid_small(42, 11);
  display.print(configuration.sys.touch_rotation);
  setCursor_textGrid_small(42, 12);
  display.print(configuration.sys.ui_reverse ? F("ON ") : F("OFF"));
  setCursor_textGrid_small(42, 13);
  display.print(configuration.sys.boot_anim_skip ? F("YES") : F("NO "));
  setCursor_textGrid_small(42, 14);
  display.print(configuration.sys.invert_colors ? F("YES") : F("NO "));
}

FLASHMEM void UI_func_misc_settings(uint8_t param)
{
  uint8_t settings_modified = 0;

  if (LCDML.FUNC_setup())
  {
    encoderDir[ENC_R].reset();
    generic_active_function = 0;
    generic_temp_select_menu = 0;
    _render_misc_settings();
  }
  if (LCDML.FUNC_loop())
  {

    // handle encoders
    if ((LCDML.BT_checkDown() && encoderDir[ENC_R].Down()) || (LCDML.BT_checkUp() && encoderDir[ENC_R].Up()))
    {
      if (LCDML.BT_checkDown())
      {
        uint8_t menu = 0;
        if (generic_active_function == 0)
          generic_temp_select_menu = constrain(generic_temp_select_menu + 1, 0, 7);
        else if (generic_temp_select_menu == menu++)
        {
          configuration.sys.gamepad_speed = constrain(configuration.sys.gamepad_speed + 10, GAMEPAD_SPEED_MIN, GAMEPAD_SPEED_MAX);
          settings_modified = 1;
        }
        else if (generic_temp_select_menu == menu++)
        {
          configuration.sys.screen_saver_start = constrain(configuration.sys.screen_saver_start + 1, SCREEN_SAVER_START_MIN, SCREEN_SAVER_START_MAX);
          settings_modified = 2;
        }
        else if (generic_temp_select_menu == menu++)
        {
          configuration.sys.screen_saver_mode = constrain(configuration.sys.screen_saver_mode + 1, SCREEN_SAVER_MODE_MIN, SCREEN_SAVER_MODE_MAX);
          settings_modified = 3;
        }
        else if (generic_temp_select_menu == menu++)
        {
          configuration.sys.display_rotation = constrain(configuration.sys.display_rotation + 1, DISPLAY_ROTATION_MIN, DISPLAY_ROTATION_MAX);
          settings_modified = 4;
        }
        else if (generic_temp_select_menu == menu++)
        {
          configuration.sys.touch_rotation = constrain(configuration.sys.touch_rotation + 1, TOUCH_ROTATION_MIN, TOUCH_ROTATION_MAX);
          settings_modified = 5;
        }
        else if (generic_temp_select_menu == menu++)
        {
          configuration.sys.ui_reverse = !configuration.sys.ui_reverse;
          settings_modified = 6;
        }
        else if (generic_temp_select_menu == menu++)
        {
          configuration.sys.boot_anim_skip = !configuration.sys.boot_anim_skip;
          settings_modified = 7;
        }
        else if (generic_temp_select_menu == menu++)
        {
          configuration.sys.invert_colors = !configuration.sys.invert_colors;
          settings_modified = 8;
        }

      }
      else if (LCDML.BT_checkUp())
      {
        uint8_t menu = 0;
        if (generic_active_function == 0)
          generic_temp_select_menu = constrain(generic_temp_select_menu - 1, 0, 7);
        else if (generic_temp_select_menu == menu++)
        {
          configuration.sys.gamepad_speed = constrain(configuration.sys.gamepad_speed - 10, GAMEPAD_SPEED_MIN, GAMEPAD_SPEED_MAX);
          settings_modified = 1;
        }
        else if (generic_temp_select_menu == menu++)
        {
          configuration.sys.screen_saver_start = constrain(configuration.sys.screen_saver_start - 1, SCREEN_SAVER_START_MIN, SCREEN_SAVER_START_MAX);
          settings_modified = 2;
        }
        else if (generic_temp_select_menu == menu++)
        {
          configuration.sys.screen_saver_mode = constrain(configuration.sys.screen_saver_mode - 1, SCREEN_SAVER_MODE_MIN, SCREEN_SAVER_MODE_MAX);
          settings_modified = 3;
        }
        else if (generic_temp_select_menu == menu++)
        {
          configuration.sys.display_rotation = constrain(configuration.sys.display_rotation - 1, DISPLAY_ROTATION_MIN, DISPLAY_ROTATION_MAX);
          settings_modified = 4;
        }
        else if (generic_temp_select_menu == menu++)
        {
          configuration.sys.touch_rotation = constrain(configuration.sys.touch_rotation - 1, TOUCH_ROTATION_MIN, TOUCH_ROTATION_MAX);
          settings_modified = 5;
        }
        else if (generic_temp_select_menu == menu++)
        {
          configuration.sys.ui_reverse = !configuration.sys.ui_reverse;
          settings_modified = 6;
        }
        else if (generic_temp_select_menu == menu++)
        {
          configuration.sys.boot_anim_skip = !configuration.sys.boot_anim_skip;
          settings_modified = 7;
        }
        else if (generic_temp_select_menu == menu++)
        {
          configuration.sys.invert_colors = !configuration.sys.invert_colors;
          settings_modified = 8;
        }
      }
    }

    // handle button presses during menu
    if (LCDML.BT_checkEnter() && encoderDir[ENC_R].ButtonShort())
    {
      if (generic_active_function == 0)
        generic_active_function = 1;
      else
        generic_active_function = 0;
    }
    else if (LCDML.BT_checkEnter())
    {
      ;
    }
    // button check end

    // Gamepad settings
    setModeColor(0);
    setCursor_textGrid_small(42, 7);
    print_formatted_number(configuration.sys.gamepad_speed, 3);
    setCursor_textGrid_small(46, 7);
    display.print(F("ms"));
    display.setTextColor(COLOR_SYSTEXT, COLOR_BACKGROUND);

    // Screen saver starts after xx seconds
    setModeColor(1);
    setCursor_textGrid_small(42, 8);
    print_formatted_number(configuration.sys.screen_saver_start, 2);
    setCursor_textGrid_small(45, 8);
    display.print(configuration.sys.screen_saver_start > 1 ? F("MINS") : F("MIN "));
    if (settings_modified == 2)
    {
      setup_screensaver();
    }
    if (settings_modified == 3)
    {
      setup_screensaver();
    }
    setModeColor(2);
    setCursor_textGrid_small(42, 9);
    print_screensaver_mode();

    setModeColor(3);
    setCursor_textGrid_small(42, 10);
    display.print(configuration.sys.display_rotation);
    if (settings_modified == 4)
    {
      display.setRotation(configuration.sys.display_rotation); // rotation 180°
      _render_misc_settings();
    }

    // Touch rotation
    setModeColor(4);
    setCursor_textGrid_small(42, 11);
    display.print(configuration.sys.touch_rotation);
#if defined GENERIC_DISPLAY
    if (settings_modified == 5)
    {
      touch.setRotation(configuration.sys.touch_rotation); // rotation 180°
    }
#endif

    // UI reverse
    setModeColor(5);
    setCursor_textGrid_small(42, 12);
    display.print(configuration.sys.ui_reverse ? F("ON ") : F("OFF"));

    setModeColor(6);
    setCursor_textGrid_small(42, 13);
    display.print(configuration.sys.boot_anim_skip ? F("YES") : F("NO "));
    setModeColor(7);
    setCursor_textGrid_small(42, 14);
    display.print(configuration.sys.invert_colors ? F("YES") : F("NO "));

    if (settings_modified == 8)
      display.invertDisplay(!configuration.sys.invert_colors);

    if (settings_modified == 6)
    {
      if (configuration.sys.display_rotation == DISPLAY_ROTATION_DEFAULT)
      {
        configuration.sys.display_rotation = DISPLAY_ROTATION_INVERTED;
        configuration.sys.touch_rotation = TOUCH_ROTATION_INVERTED;
      }
      else
      {
        configuration.sys.display_rotation = DISPLAY_ROTATION_DEFAULT;
        configuration.sys.touch_rotation = TOUCH_ROTATION_DEFAULT;
      }

      // set hardware rotations for display/touch and pins for encoders
      _setup_rotation_and_encoders(false);

      // Re render the page
      generic_active_function = 0;
      generic_temp_select_menu = 0;
      _render_misc_settings();
    }

    else if (settings_modified > 6)
    {
      set_sys_params();
    }
    if (settings_modified > 0)
    {
      save_sys_flag = true;
      save_sys = SAVE_SYS_MS / 2;
      settings_modified = 0;
    }

  }
  // ****** STABLE END *********
  if (LCDML.FUNC_close())
  {
    generic_active_function = 99;
    encoderDir[ENC_R].reset();
    display.fillScreen(COLOR_BACKGROUND);
  }
}

FLASHMEM void _setup_rotation_and_encoders(bool init)
{
  display.setRotation(configuration.sys.display_rotation);
#if defined GENERIC_DISPLAY
  touch.setRotation(configuration.sys.touch_rotation);
#endif
  if (configuration.sys.ui_reverse == true)
  {
    MD_REncoder encoder_tmp = ENCODER[ENC_L];
    ENCODER[ENC_L] = ENCODER[ENC_R];
    ENCODER[ENC_R] = encoder_tmp;
  }
  else
  {
    if (!init)
    {
      MD_REncoder encoder_tmp = ENCODER[ENC_R];
      ENCODER[ENC_R] = ENCODER[ENC_L];
      ENCODER[ENC_L] = encoder_tmp;
    }
  }
}

FLASHMEM void print_mixer_text()
{
  // Dexed
  if (seq.temp_active_menu == 0 || seq.temp_active_menu == 1)
    display.setTextColor(RED, COLOR_BACKGROUND);
  else
    display.setTextColor(COLOR_SYSTEXT, COLOR_BACKGROUND);
  setCursor_textGrid_small(0, 21);
  display.print(F("DEXED"));

  if (seq.temp_active_menu == 0)
    display.setTextColor(RED, COLOR_BACKGROUND);
  else
    display.setTextColor(COLOR_SYSTEXT, COLOR_BACKGROUND);
  setCursor_textGrid_small(0, 20);
  display.print(F("#1"));
  if (seq.temp_active_menu == 1)
    display.setTextColor(RED, COLOR_BACKGROUND);
  else
    display.setTextColor(COLOR_SYSTEXT, COLOR_BACKGROUND);
  setCursor_textGrid_small(4, 20);
  display.print(F("#2"));

  // Epiano
  if (seq.temp_active_menu == 2)
    display.setTextColor(RED, COLOR_BACKGROUND);
  else
    display.setTextColor(COLOR_SYSTEXT, COLOR_BACKGROUND);
  // setCursor_textGrid_small(8, 20);
  // display.print(F("L"));
  // setCursor_textGrid_small(12, 20);
  // display.print(F("R"));
  setCursor_textGrid_small(8, 21);
  display.print(F("EP"));

  // MicroSynth
  if (seq.temp_active_menu == 3 || seq.temp_active_menu == 4)
    display.setTextColor(RED, COLOR_BACKGROUND);
  else
    display.setTextColor(COLOR_SYSTEXT, COLOR_BACKGROUND);
  setCursor_textGrid_small(12, 21);
  display.print(F("MSYNTH"));

  if (seq.temp_active_menu == 3)
    display.setTextColor(RED, COLOR_BACKGROUND);
  else
    display.setTextColor(COLOR_SYSTEXT, COLOR_BACKGROUND);
  setCursor_textGrid_small(12, 20);
  display.print(F("#1"));
  if (seq.temp_active_menu == 4)
    display.setTextColor(RED, COLOR_BACKGROUND);
  else
    display.setTextColor(COLOR_SYSTEXT, COLOR_BACKGROUND);
  setCursor_textGrid_small(16, 20);
  display.print(F("#2"));

  // Braids
  if (seq.temp_active_menu == 5)
    display.setTextColor(RED, COLOR_BACKGROUND);
  else
    display.setTextColor(COLOR_SYSTEXT, COLOR_BACKGROUND);
  // setCursor_textGrid_small(28, 20);
  // display.print(F("BRA"));
  setCursor_textGrid_small(20, 21);
  display.print(F("BRD"));

  // MSP
  if (seq.temp_active_menu == 6 || seq.temp_active_menu == 7)
    display.setTextColor(RED, COLOR_BACKGROUND);
  else
    display.setTextColor(COLOR_SYSTEXT, COLOR_BACKGROUND);
  setCursor_textGrid_small(24, 21);
  display.print(F("MULTSMP"));

  if (seq.temp_active_menu == 6)
    display.setTextColor(RED, COLOR_BACKGROUND);
  else
    display.setTextColor(COLOR_SYSTEXT, COLOR_BACKGROUND);
  setCursor_textGrid_small(24, 20);
  display.print(F("#1"));
  if (seq.temp_active_menu == 7)
    display.setTextColor(RED, COLOR_BACKGROUND);
  else
    display.setTextColor(COLOR_SYSTEXT, COLOR_BACKGROUND);
  setCursor_textGrid_small(28, 20);
  display.print(F("#2"));

  // Drums
  if (seq.temp_active_menu == 8)
    display.setTextColor(RED, COLOR_BACKGROUND);
  else
    display.setTextColor(COLOR_SYSTEXT, COLOR_BACKGROUND);
  // setCursor_textGrid_small(28, 20);
  // display.print(F("L"));
  // setCursor_textGrid_small(32, 20);
  // display.print(F("R"));
  setCursor_textGrid_small(32, 21);
  display.print(F("DRM"));

  // Reverb
  if (seq.temp_active_menu == 9)
    display.setTextColor(RED, COLOR_BACKGROUND);
  else
    display.setTextColor(COLOR_SYSTEXT, COLOR_BACKGROUND);
  setCursor_textGrid_small(38, 20);
  display.print(F("L"));
  setCursor_textGrid_small(42, 20);
  display.print(F("R"));
  setCursor_textGrid_small(38, 21);
  display.print(F("REVERB"));

  // Master
  if (seq.temp_active_menu == 10)
    display.setTextColor(RED, COLOR_BACKGROUND);
  else
    display.setTextColor(COLOR_SYSTEXT, COLOR_BACKGROUND);
  setCursor_textGrid_small(46, 20);
  display.print(F("L"));
  setCursor_textGrid_small(50, 20);
  display.print(F("R"));
  setCursor_textGrid_small(46, 21);
  display.print(F("MASTER"));

  /// Values
  display.setTextColor(GREY2, COLOR_BACKGROUND);
  // print_small_panbar_mixer(0, 17, configuration.dexed[0].pan, 31);
  setCursor_textGrid_small(0, 19);
  print_formatted_number(configuration.dexed[0].sound_intensity, 3);
  // print_small_panbar_mixer(4, 17, configuration.dexed[1].pan, 31);
  setCursor_textGrid_small(4, 19);
  print_formatted_number(configuration.dexed[1].sound_intensity, 3);

  // print_small_panbar_mixer(8, 17, configuration.epiano.pan, 31);
  setCursor_textGrid_small(8, 19);
  print_formatted_number(configuration.epiano.sound_intensity, 3);

  // print_small_panbar_mixer(12, 17, microsynth[0].pan, 31);
  setCursor_textGrid_small(12, 19);
  print_formatted_number(microsynth[0].sound_intensity, 3);
  // print_small_panbar_mixer(16, 17, microsynth[1].pan, 31);
  setCursor_textGrid_small(16, 19);
  print_formatted_number(microsynth[1].sound_intensity, 3);

  // print_small_panbar_mixer(20, 17, braids_osc.pan, 31);
  setCursor_textGrid_small(20, 19);
  print_formatted_number(braids_osc.sound_intensity, 3);

  // msp
  //  print_small_panbar_mixer(20, 17, braids_osc.pan, 31); // pan of the msp #1 zone played
  setCursor_textGrid_small(24, 19);
  print_formatted_number(msp[0].sound_intensity, 3);
  //  print_small_panbar_mixer(20, 17, braids_osc.pan, 31); // pan of the msp #2 zone played
  setCursor_textGrid_small(28, 19);
  print_formatted_number(msp[0].sound_intensity, 3);

  // drums
  temp_int = mapfloat(seq.drums_volume, 0.0, VOL_MAX_FLOAT, 0, 100);
  // setCursor_textGrid_small(28, 19);
  // print_formatted_number(temp_int, 3);
  setCursor_textGrid_small(32, 19);
  print_formatted_number(temp_int, 3);

  // reverb
  setCursor_textGrid_small(38, 19);
  print_formatted_number(configuration.fx.reverb_level, 3);
  setCursor_textGrid_small(42, 19);
  print_formatted_number(configuration.fx.reverb_level, 3);

  // Master
  setCursor_textGrid_small(46, 19);
  print_formatted_number(configuration.sys.vol, 3);
  setCursor_textGrid_small(50, 19);
  print_formatted_number(configuration.sys.vol, 3);
}

FLASHMEM void update_braids_volume()
{
  for (uint8_t instance_id = 0; instance_id < NUM_BRAIDS; instance_id++)
    braids_mixer.gain(instance_id, volume_transform(mapfloat(braids_osc.sound_intensity, REVERB_SEND_MIN, REVERB_SEND_MAX, 0.0, 0.7)));
}

FLASHMEM void UI_func_mixer(uint8_t param)
{
  if (LCDML.FUNC_setup()) // ****** SETUP *********
  {
    encoderDir[ENC_R].reset();
    seq.temp_active_menu = 0;
    display.fillScreen(COLOR_BACKGROUND);
    for (uint8_t j = 0; j < uint8_t(sizeof(ts.displayed_peak)); j++)
      ts.displayed_peak[j] = 0;
    setCursor_textGrid(1, 1);
    display.print(F("MIXER"));
    helptext_l(back_text);
    helptext_r("< > SELECT CH");
    display.setTextColor(COLOR_SYSTEXT, COLOR_BACKGROUND);
    print_mixer_text();
  }
  if (LCDML.FUNC_loop()) // ****** LOOP *********
  {
    if ((LCDML.BT_checkDown() && encoderDir[ENC_R].Down()) || (LCDML.BT_checkUp() && encoderDir[ENC_R].Up()))
    {
      if (!seq.edit_state) // select channel
      {
        if (LCDML.BT_checkDown())
          seq.temp_active_menu = constrain(seq.temp_active_menu + 1, 0, 10);
        else if (LCDML.BT_checkUp())
          seq.temp_active_menu = constrain(seq.temp_active_menu - 1, 0, 10);
      }
      else
      {
        if (seq.temp_active_menu < 2) // dexed instance #0 or #1
        {
          if (LCDML.BT_checkDown())
            configuration.dexed[seq.temp_active_menu].sound_intensity = constrain(configuration.dexed[seq.temp_active_menu].sound_intensity + ENCODER[ENC_R].speed(), SOUND_INTENSITY_MIN, SOUND_INTENSITY_MAX);
          else if (LCDML.BT_checkUp())
            configuration.dexed[seq.temp_active_menu].sound_intensity = constrain(configuration.dexed[seq.temp_active_menu].sound_intensity - ENCODER[ENC_R].speed(), SOUND_INTENSITY_MIN, SOUND_INTENSITY_MAX);
        }
        else if (seq.temp_active_menu == 2) // epiano
        {
          if (LCDML.BT_checkDown())
          {
            configuration.epiano.sound_intensity = constrain(configuration.epiano.sound_intensity + ENCODER[ENC_R].speed(), SOUND_INTENSITY_MIN, SOUND_INTENSITY_MAX);
            ep.setVolume(mapfloat(configuration.epiano.sound_intensity, SOUND_INTENSITY_MIN, SOUND_INTENSITY_MAX, 0, 1.0));
          }
          else if (LCDML.BT_checkUp())
          {
            configuration.epiano.sound_intensity = constrain(configuration.epiano.sound_intensity - ENCODER[ENC_R].speed(), SOUND_INTENSITY_MIN, SOUND_INTENSITY_MAX);
            ep.setVolume(mapfloat(configuration.epiano.sound_intensity, SOUND_INTENSITY_MIN, SOUND_INTENSITY_MAX, 0, 1.0));
          }
        }
        else if (seq.temp_active_menu > 2 && seq.temp_active_menu < 5) // microsynth
        {
          if (LCDML.BT_checkDown())
            microsynth[seq.temp_active_menu - 3].sound_intensity = constrain(microsynth[seq.temp_active_menu - 3].sound_intensity + ENCODER[ENC_R].speed(), SOUND_INTENSITY_MIN, SOUND_INTENSITY_MAX);
          else if (LCDML.BT_checkUp())
            microsynth[seq.temp_active_menu - 3].sound_intensity = constrain(microsynth[seq.temp_active_menu - 3].sound_intensity - ENCODER[ENC_R].speed(), SOUND_INTENSITY_MIN, SOUND_INTENSITY_MAX);
        }
        else if (seq.temp_active_menu == 5) // braids
        {
          if (LCDML.BT_checkDown())
          {
            braids_osc.sound_intensity = constrain(braids_osc.sound_intensity + ENCODER[ENC_R].speed(), SOUND_INTENSITY_MIN, SOUND_INTENSITY_MAX);
            update_braids_volume();
          }
          else if (LCDML.BT_checkUp())
          {
            braids_osc.sound_intensity = constrain(braids_osc.sound_intensity - ENCODER[ENC_R].speed(), SOUND_INTENSITY_MIN, SOUND_INTENSITY_MAX);
            update_braids_volume();
          }
        }
        else if (seq.temp_active_menu == 6) // msp1
        {
          if (LCDML.BT_checkDown())
            msp[0].sound_intensity = constrain(msp[0].sound_intensity + ENCODER[ENC_R].speed(), SOUND_INTENSITY_MIN, SOUND_INTENSITY_MAX);
          else if (LCDML.BT_checkUp())
            msp[0].sound_intensity = constrain(msp[0].sound_intensity - ENCODER[ENC_R].speed(), SOUND_INTENSITY_MIN, SOUND_INTENSITY_MAX);
        }
        else if (seq.temp_active_menu == 7) // msp2
        {
          if (LCDML.BT_checkDown())
            msp[1].sound_intensity = constrain(msp[1].sound_intensity + ENCODER[ENC_R].speed(), SOUND_INTENSITY_MIN, SOUND_INTENSITY_MAX);
          else if (LCDML.BT_checkUp())
            msp[1].sound_intensity = constrain(msp[1].sound_intensity - ENCODER[ENC_R].speed(), SOUND_INTENSITY_MIN, SOUND_INTENSITY_MAX);
        }
        else if (seq.temp_active_menu == 8) // drums/samples
        {
          if (LCDML.BT_checkDown())
            temp_int = constrain(temp_int + ENCODER[ENC_R].speed(), SOUND_INTENSITY_MIN, SOUND_INTENSITY_MAX);
          else if (LCDML.BT_checkUp())
            temp_int = constrain(temp_int - ENCODER[ENC_R].speed(), SOUND_INTENSITY_MIN, SOUND_INTENSITY_MAX);
          seq.drums_volume = mapfloat(temp_int, SOUND_INTENSITY_MIN, SOUND_INTENSITY_MAX, 0.0, VOL_MAX_FLOAT);
        }
        else if (seq.temp_active_menu == 9) // reverb level
        {
          if (LCDML.BT_checkDown())
            configuration.fx.reverb_level = constrain(configuration.fx.reverb_level + ENCODER[ENC_R].speed(), REVERB_LEVEL_MIN, REVERB_LEVEL_MAX);
          else if (LCDML.BT_checkUp())
            configuration.fx.reverb_level = constrain(configuration.fx.reverb_level - ENCODER[ENC_R].speed(), REVERB_LEVEL_MIN, REVERB_LEVEL_MAX);
        }
        else if (seq.temp_active_menu == 10) // master level
        {
          if (LCDML.BT_checkDown())
          {
            configuration.sys.vol = constrain(configuration.sys.vol + ENCODER[ENC_R].speed(), VOLUME_MIN, VOLUME_MAX);
          }
          else if (LCDML.BT_checkUp())
          {
            configuration.sys.vol = constrain(configuration.sys.vol - ENCODER[ENC_R].speed(), VOLUME_MIN, VOLUME_MAX);
          }
        }
      }
    }
    else if (LCDML.BT_checkEnter())
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
      helptext_r("CHANGE VOLUME");
    else
      helptext_r("< > SELECT CHANNEL");
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
    else if (seq.temp_active_menu == 2 && seq.edit_state) // epiano
    {
      display.setTextColor(COLOR_SYSTEXT, COLOR_BACKGROUND);
      display_bar_int("", configuration.epiano.sound_intensity, 1.0, SOUND_INTENSITY_MIN, SOUND_INTENSITY_MAX, 3, false, false, false);
      setCursor_textGrid(1, 1);
      display.print("EP");
    }
    else if (seq.temp_active_menu > 2 && seq.temp_active_menu < 5 && seq.edit_state) // microsynth
    {
      display.setTextColor(COLOR_SYSTEXT, COLOR_BACKGROUND);
      display_bar_int("", microsynth[seq.temp_active_menu - 3].sound_intensity, 1.0, SOUND_INTENSITY_MIN, SOUND_INTENSITY_MAX, 3, false, false, false);
      setCursor_textGrid(1, 1);
      display.print("MICROSYNTH #");
      display.print(seq.temp_active_menu - 2);
    }
    else if (seq.temp_active_menu == 5 && seq.edit_state) // braids
    {
      display.setTextColor(COLOR_SYSTEXT, COLOR_BACKGROUND);
      display_bar_int("", braids_osc.sound_intensity, 1.0, SOUND_INTENSITY_MIN, SOUND_INTENSITY_MAX, 3, false, false, false);
      setCursor_textGrid(1, 1);
      display.print("BRAIDS");
    }
    else if (seq.temp_active_menu == 6 && seq.edit_state) // msp0
    {
      display.setTextColor(COLOR_SYSTEXT, COLOR_BACKGROUND);
      display_bar_int("", msp[0].sound_intensity, 1.0, SOUND_INTENSITY_MIN, SOUND_INTENSITY_MAX, 3, false, false, false);
      setCursor_textGrid(1, 1);
      display.print("MULTISAMPLE #");
      display.print(seq.temp_active_menu - 5);
    }
    else if (seq.temp_active_menu == 7 && seq.edit_state) // msp1
    {
      display.setTextColor(COLOR_SYSTEXT, COLOR_BACKGROUND);
      display_bar_int("", msp[1].sound_intensity, 1.0, SOUND_INTENSITY_MIN, SOUND_INTENSITY_MAX, 3, false, false, false);
      setCursor_textGrid(1, 1);
      display.print("MULTISAMPLE #");
      display.print(seq.temp_active_menu - 5);
    }
    else if (seq.temp_active_menu == 8 && seq.edit_state) // drums
    {
      display.setTextColor(COLOR_SYSTEXT, COLOR_BACKGROUND);
      temp_int = mapfloat(seq.drums_volume, 0.0, VOL_MAX_FLOAT, 0, 100);
      display_bar_int("DRUMS VOLUME", temp_int, 1.0, SOUND_INTENSITY_MIN, SOUND_INTENSITY_MAX, 3, false, false, true);
      master_mixer_r.gain(3, volume_transform(mapfloat(temp_int, 0, 100, 0.0, VOL_MAX_FLOAT)));
      master_mixer_l.gain(3, volume_transform(mapfloat(temp_int, 0, 100, 0.0, VOL_MAX_FLOAT)));
    }
    else if (seq.temp_active_menu == 9 && seq.edit_state) // reverb level
    {
      display.setTextColor(COLOR_SYSTEXT, COLOR_BACKGROUND);
      setCursor_textGrid(1, 1);
      display.print(F("REVERB LEVEL"));
      display_bar_int("", configuration.fx.reverb_level, 1.0, REVERB_LEVEL_MIN, REVERB_LEVEL_MAX, 3, false, false, false);
      master_mixer_r.gain(MASTER_MIX_CH_REVERB, volume_transform(mapfloat(configuration.fx.reverb_level, REVERB_LEVEL_MIN, REVERB_LEVEL_MAX, 0.0, VOL_MAX_FLOAT)));
      master_mixer_l.gain(MASTER_MIX_CH_REVERB, volume_transform(mapfloat(configuration.fx.reverb_level, REVERB_LEVEL_MIN, REVERB_LEVEL_MAX, 0.0, VOL_MAX_FLOAT)));
    }
    else if (seq.temp_active_menu == 10 && seq.edit_state) // master volume
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
  if (LCDML.FUNC_close()) // ****** STABLE END *********
  {
    encoderDir[ENC_R].reset();
    display.fillScreen(COLOR_BACKGROUND);
  }
}

void print_sidechain_static_texts()
{
  setCursor_textGrid_small(1, 1);
  display.setTextSize(2);
  display.setTextColor(COLOR_SYSTEXT, COLOR_BACKGROUND);
  display.print(F("DUAL SIDECHAIN"));
  helptext_l(back_text);
  helptext_r("< > SELECT PARAM");
  display.setTextColor(COLOR_SYSTEXT, COLOR_BACKGROUND);

  setCursor_textGrid_small(1, 4);
  display.print(F("TRIGGER"));
  setCursor_textGrid_small(11, 4);
  display.setTextColor(RED, COLOR_BACKGROUND);
  display.print(F("A"));
  display.setTextColor(COLOR_SYSTEXT, COLOR_BACKGROUND);
  setCursor_textGrid_small(33, 4);
  display.setTextColor(RED, COLOR_BACKGROUND);
  display.print(F("B"));
  display.setTextColor(COLOR_SYSTEXT, COLOR_BACKGROUND);

  setCursor_textGrid_small(1, 5);
  display.print(F("SOURCE"));

  setCursor_textGrid_small(1, 6);
  display.print(F("SPEED"));

  display.setTextColor(COLOR_SYSTEXT, COLOR_BACKGROUND);
  setCursor_textGrid_small(1, 8);
  display.print(F("TARGETS"));

  display.setTextColor(GREY2, COLOR_BACKGROUND);
  setCursor_textGrid_small(11, 8);
  display.print(F("STRENGTH"));
  setCursor_textGrid_small(21, 8);
  display.print(F("REDUCTION"));

  setCursor_textGrid_small(33, 8);
  display.print(F("STRENGTH"));
  setCursor_textGrid_small(43, 8);
  display.print(F("REDUCTION"));

  setCursor_textGrid_small(1, 10);
  display.print(F("DEXED1"));
  setCursor_textGrid_small(1, 11);
  display.print(F("DEXED2"));
  setCursor_textGrid_small(1, 12);
  display.print(F("EPIANO"));
  setCursor_textGrid_small(1, 13);
  display.print(F("MSYNTH1"));
  setCursor_textGrid_small(1, 14);
  display.print(F("MSYNTH2"));
  setCursor_textGrid_small(1, 15);
  display.print(F("BRAIDS"));
  setCursor_textGrid_small(1, 16);
  display.print(F("MSP1"));
  setCursor_textGrid_small(1, 17);
  display.print(F("MSP1"));
  setCursor_textGrid_small(1, 19);
  display.print(F("DELAY A"));
  setCursor_textGrid_small(1, 20);
  display.print(F("DELAY B"));
  setCursor_textGrid_small(1, 21);
  display.print(F("REVERB"));

  print_small_intbar(33, 10, sc_dexed1_target_b, 17, 1, 1);
  print_small_intbar(33, 11, sc_dexed2_target_b, 18, 1, 1);

  print_small_intbar(33, 12, 0, 19, 1, 1);
  print_small_intbar(33, 13, 0, 20, 1, 1);
  print_small_intbar(33, 14, 0, 21, 1, 1);
  print_small_intbar(33, 15, sc_braids_target_b, 22, 1, 1);
  print_small_intbar(33, 16, 0, 23, 1, 1);
  print_small_intbar(33, 17, 0, 24, 1, 1);

  print_small_intbar(33, 19, sc_delay_a_target_b, 25, 1, 1);
  print_small_intbar(33, 20, sc_delay_b_target_b, 26, 1, 1);
  print_small_intbar(33, 21, sc_reverb_target_b, 27, 1, 1);

  // display.setTextSize(2);

  setModeColor(0);
  setCursor_textGrid_small(15, 4);
  if (sidechain_a_active)
  {
    display.print(F("ON "));
  }
  else
  {
    display.print(F("OFF"));
  }
  setModeColor(14);
  setCursor_textGrid_small(37, 4);
  if (sidechain_b_active)
  {
    display.print(F("ON "));
  }
  else
  {
    display.print(F("OFF"));
  }

  setModeColor(1);
  setCursor_textGrid_small(11, 5);
  print_formatted_number(sidechain_a_sample_number, 3);
  show_no_grid(6 * CHAR_height_small + 2, 15 * CHAR_width_small, 14, basename(drum_config[sidechain_a_sample_number].name));

  setModeColor(15);
  setCursor_textGrid_small(33, 5);
  print_formatted_number(sidechain_b_sample_number, 3);
  show_no_grid(6 * CHAR_height_small + 2, 37 * CHAR_width_small, 14, basename(drum_config[sidechain_b_sample_number].name));

  print_small_intbar(11, 6, sidechain_a_speed, 2, 1, 1); // sidechain a speed

  print_small_intbar(11, 10, sc_dexed1_target_a, 3, 1, 1);
  print_small_intbar(11, 11, sc_dexed2_target_a, 4, 1, 1);

  print_small_intbar(11, 12, 0, 5, 1, 1);
  print_small_intbar(11, 13, 0, 6, 1, 1);
  print_small_intbar(11, 14, 0, 7, 1, 1);
  print_small_intbar(11, 15, sc_braids_target_a, 8, 1, 1);
  print_small_intbar(11, 16, 0, 9, 1, 1);
  print_small_intbar(11, 17, 0, 10, 1, 1);

  print_small_intbar(11, 19, sc_delay_a_target_a, 11, 1, 1);
  print_small_intbar(11, 20, sc_delay_b_target_a, 12, 1, 1);
  print_small_intbar(11, 21, sc_reverb_target_a, 13, 1, 1);

  // b
  display.setTextSize(1);
  print_small_intbar(33, 6, sidechain_b_speed, 16, 1, 1); // sidechain b speed

  char displayname[8] = { 0, 0, 0, 0, 0, 0, 0 };
  setCursor_textGrid_small(21, 6);
  //snprintf_P(displayname, sizeof(displayname), PSTR("%04d"), (seq.tempo_ms / 1000) * (sidechain_a_speed / 10));
  snprintf_P(displayname, sizeof(displayname), PSTR("%04d"), 1000 - sidechain_a_speed * 10);

  display.setTextColor(COLOR_SYSTEXT, COLOR_BACKGROUND);
  display.print(displayname);
  setCursor_textGrid_small(26, 6);
  display.setTextColor(GREY2, COLOR_BACKGROUND);
  display.print("MS");

  setCursor_textGrid_small(43, 6);
  // snprintf_P(displayname, sizeof(displayname), PSTR("%04d"), (seq.tempo_ms / 1000) * (sidechain_b_speed / 10));
  snprintf_P(displayname, sizeof(displayname), PSTR("%04d"), 1000 - sidechain_b_speed * 10);
  display.setTextColor(COLOR_SYSTEXT, COLOR_BACKGROUND);
  display.print(displayname);
  setCursor_textGrid_small(48, 6);
  display.setTextColor(GREY2, COLOR_BACKGROUND);
  display.print("MS");

  display.setTextColor(COLOR_SYSTEXT, COLOR_BACKGROUND);
}

void print_sidechain_editor_values()
{
  char displayname[8] = { 0, 0, 0, 0, 0, 0, 0 };
  if (menu_item_check(0))
  {
    setModeColor(0);
    setCursor_textGrid_small(15, 4);
    if (sidechain_a_active)
    {
      display.print(F("ON "));
    }
    else
    {
      display.print(F("OFF"));
    }
  }

  if (menu_item_check(1))
  {
    setModeColor(1);
    setCursor_textGrid_small(11, 5);
    print_formatted_number(sidechain_a_sample_number, 3);
    show_no_grid(6 * CHAR_height_small + 2, 15 * CHAR_width_small, 14, basename(drum_config[sidechain_a_sample_number].name));
  }
  if (menu_item_check(2))
  {

    print_small_intbar(11, 6, sidechain_a_speed, 2, 1, 1); // sidechain a speed
    setCursor_textGrid_small(21, 6);
    //  snprintf_P(displayname, sizeof(displayname), PSTR("%04d"), (seq.tempo_ms / 1000) * (sidechain_a_speed / 10));
    snprintf_P(displayname, sizeof(displayname), PSTR("%04d"), 1000 - sidechain_a_speed * 10);
    display.print(displayname);
  }
  if (menu_item_check(3))
  {
    print_small_intbar(11, 10, sc_dexed1_target_a, 3, 1, 1);
  }
  if (menu_item_check(4))
  {
    print_small_intbar(11, 11, sc_dexed2_target_a, 4, 1, 1);
  }

  if (menu_item_check(5))
  {
    print_small_intbar(11, 12, 0, 5, 1, 1);
  }
  if (menu_item_check(6))
  {
    print_small_intbar(11, 13, 0, 6, 1, 1);
  }
  if (menu_item_check(7))
  {
    print_small_intbar(11, 14, 0, 7, 1, 1);
  }
  if (menu_item_check(8))
  {
    print_small_intbar(11, 15, sc_braids_target_a, 8, 1, 1);
  }
  if (menu_item_check(9))
  {
    print_small_intbar(11, 16, 0, 9, 1, 1);
  }
  if (menu_item_check(10))
  {
    print_small_intbar(11, 17, 0, 10, 1, 1);
  }
  if (menu_item_check(11))
  {
    print_small_intbar(11, 19, sc_delay_a_target_a, 11, 1, 1);
  }
  if (menu_item_check(12))
  {
    print_small_intbar(11, 20, sc_delay_b_target_a, 12, 1, 1);
  }
  if (menu_item_check(13))
  {
    print_small_intbar(11, 21, sc_reverb_target_a, 13, 1, 1);
  }

  /// COMPRESSOR B

  if (menu_item_check(14))
  {
    setModeColor(14);
    setCursor_textGrid_small(37, 4);
    if (sidechain_b_active)
    {
      display.print(F("ON "));
    }
    else
    {
      display.print(F("OFF"));
    }
  }
  if (menu_item_check(15))
  {
    setModeColor(15);
    setCursor_textGrid_small(33, 5);
    print_formatted_number(sidechain_b_sample_number, 3);
    show_no_grid(6 * CHAR_height_small + 2, 37 * CHAR_width_small, 14, basename(drum_config[sidechain_b_sample_number].name));
  }
  if (menu_item_check(16))
  {
    print_small_intbar(33, 6, sidechain_b_speed, 16, 1, 1); // sidechain b speed
    setCursor_textGrid_small(43, 6);
    // snprintf_P(displayname, sizeof(displayname), PSTR("%04d"), (seq.tempo_ms / 1000) * (sidechain_b_speed / 10));
    snprintf_P(displayname, sizeof(displayname), PSTR("%04d"), 1000 - sidechain_b_speed * 10);
    display.print(displayname);
  }

  if (menu_item_check(17))
  {
    print_small_intbar(33, 10, sc_dexed1_target_b, 17, 1, 1);
  }
  if (menu_item_check(18))
  {
    print_small_intbar(33, 11, sc_dexed2_target_b, 18, 1, 1);
  }

  if (menu_item_check(19))
  {
    print_small_intbar(33, 12, 0, 19, 1, 1);
  }
  if (menu_item_check(20))
  {
    print_small_intbar(33, 13, 0, 20, 1, 1);
  }
  if (menu_item_check(21))
  {
    print_small_intbar(33, 14, 0, 21, 1, 1);
  }
  if (menu_item_check(22))
  {
    print_small_intbar(33, 15, sc_braids_target_b, 22, 1, 1);
  }
  if (menu_item_check(23))
  {
    print_small_intbar(33, 16, 0, 23, 1, 1);
  }
  if (menu_item_check(24))
  {
    print_small_intbar(33, 17, 0, 24, 1, 1);
  }
  if (menu_item_check(25))
  {
    print_small_intbar(33, 19, 0, 25, 1, 1);
  }
  if (menu_item_check(26))
  {
    print_small_intbar(33, 20, 0, 26, 1, 1);
  }
  if (menu_item_check(27))
  {
    print_small_intbar(33, 21, sc_reverb_target_b, 27, 1, 1);
  }
}

void print_sidechain_level_indicators()
{
  if (sidechain_a_active && seq.running)
  {
    print_fast_level_indicator(17, 10, 100 - (float)sc_dexed1_current * 100, 0, 100);
    print_fast_level_indicator(17, 11, 100 - (float)sc_dexed2_current * 100, 0, 100);
    print_fast_level_indicator(17, 15, 100 - (float)sc_braids_current * 100, 0, 100);

    print_fast_level_indicator(17, 19, 100 - (float)sc_delay_a_current * 100, 0, 100);
    print_fast_level_indicator(17, 20, 100 - (float)sc_delay_b_current * 100, 0, 100);

    print_fast_level_indicator(17, 21, 100 - (float)sc_reverb_current * 100, 0, 100);
  }

  // print_fast_level_indicator(17, 12, random(100), 0, 100);
  // print_fast_level_indicator(17, 13, random(100), 0, 100);
  // print_fast_level_indicator(17, 14, random(100), 0, 100);

  // print_fast_level_indicator(17, 16, random(100), 0, 100);
  // print_fast_level_indicator(17, 17, random(100), 0, 100);

  //  print_fast_level_indicator(39, 10, random(100), 0, 100);
  //  print_fast_level_indicator(39, 11, random(100), 0, 100);
  // print_fast_level_indicator(39, 12, random(100), 0, 100);
  // print_fast_level_indicator(39, 13, random(100), 0, 100);
  // print_fast_level_indicator(39, 14, random(100), 0, 100);
  // print_fast_level_indicator(39, 15, random(100), 0, 100);
  // print_fast_level_indicator(39, 16, random(100), 0, 100);
  // print_fast_level_indicator(39, 17, random(100), 0, 100);

  // print_fast_level_indicator(39, 19, random(100), 0, 100);
  // print_fast_level_indicator(39, 20, random(100), 0, 100);
  // print_fast_level_indicator(39, 21, random(100), 0, 100);
}

FLASHMEM void UI_func_sidechain(uint8_t param)
{
  if (LCDML.FUNC_setup()) // ****** SETUP *********
  {
    encoderDir[ENC_R].reset();

    seq.temp_active_menu = 0;
    generic_temp_select_menu = 0;
    // seq.edit_state=false;
    display.fillScreen(COLOR_BACKGROUND);

    print_sidechain_static_texts();
    print_sidechain_editor_values();
    print_sidechain_level_indicators();
  }
  if (LCDML.FUNC_loop()) // ****** LOOP *********
  {
    if ((LCDML.BT_checkDown() && encoderDir[ENC_R].Down()) || (LCDML.BT_checkUp() && encoderDir[ENC_R].Up()))
    {

      if (!seq.edit_state)
      {
        if (LCDML.BT_checkDown())
          generic_temp_select_menu = constrain(generic_temp_select_menu + 1, 0, 27);
        else if (LCDML.BT_checkUp())
          generic_temp_select_menu = constrain(generic_temp_select_menu - 1, 0, 27);
      }

      else if (seq.edit_state)
      {

        if (generic_temp_select_menu == 0) // sidechain a on/off
        {
          if (LCDML.BT_checkDown())
            sidechain_a_active = !sidechain_a_active;
          else if (LCDML.BT_checkUp())
            sidechain_a_active = !sidechain_a_active;
        }
        else if (generic_temp_select_menu == 14) // sidechain b on/off
        {
          if (LCDML.BT_checkDown())
            sidechain_b_active = !sidechain_b_active;
          else if (LCDML.BT_checkUp())
            sidechain_b_active = !sidechain_b_active;
        }
        else

          if (generic_temp_select_menu == 1) // sidechain a sample
          {
            if (LCDML.BT_checkDown())
              sidechain_a_sample_number = constrain(sidechain_a_sample_number + 1, 0, NUM_DRUMSET_CONFIG - 1);
            else if (LCDML.BT_checkUp())
              sidechain_a_sample_number = constrain(sidechain_a_sample_number - 1, 0, NUM_DRUMSET_CONFIG - 1);
          }
          else if (generic_temp_select_menu == 15) // sidechain b sample
          {
            if (LCDML.BT_checkDown())
              sidechain_b_sample_number = constrain(sidechain_b_sample_number + 1, 0, NUM_DRUMSET_CONFIG - 1);
            else if (LCDML.BT_checkUp())
              sidechain_b_sample_number = constrain(sidechain_b_sample_number - 1, 0, NUM_DRUMSET_CONFIG - 1);
          }

        if (generic_temp_select_menu == 2) // sidechain a speed
        {
          if (LCDML.BT_checkDown())
            sidechain_a_speed = constrain(sidechain_a_speed + ENCODER[ENC_R].speed(), 0, 100);
          else if (LCDML.BT_checkUp())
            sidechain_a_speed = constrain(sidechain_a_speed - ENCODER[ENC_R].speed(), 0, 100);
        }
        else if (generic_temp_select_menu == 16) // sidechain b speed
        {
          if (LCDML.BT_checkDown())
            sidechain_b_speed = constrain(sidechain_b_speed + ENCODER[ENC_R].speed(), 0, 100);
          else if (LCDML.BT_checkUp())
            sidechain_b_speed = constrain(sidechain_b_speed - ENCODER[ENC_R].speed(), 0, 100);
        }
        else if (generic_temp_select_menu == 3) // dexed 0 target A
        {
          if (LCDML.BT_checkDown())
            sc_dexed1_target_a = constrain(sc_dexed1_target_a + ENCODER[ENC_R].speed(), 0, 100);
          else if (LCDML.BT_checkUp())
            sc_dexed1_target_a = constrain(sc_dexed1_target_a - ENCODER[ENC_R].speed(), 0, 100);
        }
        else if (generic_temp_select_menu == 4) // dexed 1 target A
        {
          if (LCDML.BT_checkDown())
            sc_dexed2_target_a = constrain(sc_dexed2_target_a + ENCODER[ENC_R].speed(), 0, 100);
          else if (LCDML.BT_checkUp())
            sc_dexed2_target_a = constrain(sc_dexed2_target_a - ENCODER[ENC_R].speed(), 0, 100);
        }

        else if (generic_temp_select_menu == 8) // braids target A
        {
          if (LCDML.BT_checkDown())
            sc_braids_target_a = constrain(sc_braids_target_a + ENCODER[ENC_R].speed(), 0, 100);
          else if (LCDML.BT_checkUp())
            sc_braids_target_a = constrain(sc_braids_target_a - ENCODER[ENC_R].speed(), 0, 100);
        }
        else if (generic_temp_select_menu == 22) // braids target B
        {
          if (LCDML.BT_checkDown())
            sc_braids_target_b = constrain(sc_braids_target_b + ENCODER[ENC_R].speed(), 0, 100);
          else if (LCDML.BT_checkUp())
            sc_braids_target_b = constrain(sc_braids_target_b - ENCODER[ENC_R].speed(), 0, 100);
        }

        else if (generic_temp_select_menu == 11) // delay A target A
        {
          if (LCDML.BT_checkDown())
            sc_delay_a_target_a = constrain(sc_delay_a_target_a + ENCODER[ENC_R].speed(), 0, 100);
          else if (LCDML.BT_checkUp())
            sc_delay_a_target_a = constrain(sc_delay_a_target_a - ENCODER[ENC_R].speed(), 0, 100);
        }
        else if (generic_temp_select_menu == 12) // delay B target A
        {
          if (LCDML.BT_checkDown())
            sc_delay_b_target_a = constrain(sc_delay_b_target_a + ENCODER[ENC_R].speed(), 0, 100);
          else if (LCDML.BT_checkUp())
            sc_delay_b_target_a = constrain(sc_delay_b_target_a - ENCODER[ENC_R].speed(), 0, 100);
        }

        else if (generic_temp_select_menu == 25) // delay A target B
        {
          if (LCDML.BT_checkDown())
            sc_delay_a_target_b = constrain(sc_delay_a_target_b + ENCODER[ENC_R].speed(), 0, 100);
          else if (LCDML.BT_checkUp())
            sc_delay_a_target_b = constrain(sc_delay_a_target_b - ENCODER[ENC_R].speed(), 0, 100);
        }
        else if (generic_temp_select_menu == 26) // delay B target B
        {
          if (LCDML.BT_checkDown())
            sc_delay_b_target_b = constrain(sc_delay_b_target_b + ENCODER[ENC_R].speed(), 0, 100);
          else if (LCDML.BT_checkUp())
            sc_delay_b_target_b = constrain(sc_delay_b_target_b - ENCODER[ENC_R].speed(), 0, 100);
        }

        else if (generic_temp_select_menu == 13) // reverb target A
        {
          if (LCDML.BT_checkDown())
            sc_reverb_target_a = constrain(sc_reverb_target_a + ENCODER[ENC_R].speed(), 0, 100);
          else if (LCDML.BT_checkUp())
            sc_reverb_target_a = constrain(sc_reverb_target_a - ENCODER[ENC_R].speed(), 0, 100);
        }
        else if (generic_temp_select_menu == 27) // reverb target B
        {
          if (LCDML.BT_checkDown())
            sc_reverb_target_b = constrain(sc_reverb_target_b + ENCODER[ENC_R].speed(), 0, 100);
          else if (LCDML.BT_checkUp())
            sc_reverb_target_b = constrain(sc_reverb_target_b - ENCODER[ENC_R].speed(), 0, 100);
        }

        else if (generic_temp_select_menu == 17) // dexed 0 target B
        {
          if (LCDML.BT_checkDown())
            sc_dexed1_target_b = constrain(sc_dexed1_target_b + ENCODER[ENC_R].speed(), 0, 100);
          else if (LCDML.BT_checkUp())
            sc_dexed1_target_b = constrain(sc_dexed1_target_b - ENCODER[ENC_R].speed(), 0, 100);
        }
        else if (generic_temp_select_menu == 18) // dexed 1 target B
        {
          if (LCDML.BT_checkDown())
            sc_dexed2_target_b = constrain(sc_dexed2_target_b + ENCODER[ENC_R].speed(), 0, 100);
          else if (LCDML.BT_checkUp())
            sc_dexed2_target_b = constrain(sc_dexed2_target_b - ENCODER[ENC_R].speed(), 0, 100);
        }
      }
    }

    if (LCDML.BT_checkEnter())
    {
      seq.edit_state = !seq.edit_state;
      //     if (!seq.edit_state) {
      //       display.setTextSize(2);
      //       setCursor_textGrid(1, 1);
      //       display.print(F("SIDECHAIN"));
      //     }
    } // buttons END

    // else if (seq.temp_active_menu == 2)  //epiano
    //       {
    //         if (LCDML.BT_checkDown()) {
    //           configuration.epiano.sidechain_time = constrain(configuration.epiano.sidechain_time + ENCODER[ENC_R].speed(), SOUND_INTENSITY_MIN, SOUND_INTENSITY_MAX);
    //           // ep.setVolume(mapfloat(configuration.epiano.sidechain_time, SOUND_INTENSITY_MIN, SOUND_INTENSITY_MAX, 0, 1.0));
    //         } else if (LCDML.BT_checkUp()) {
    //           configuration.epiano.sidechain_time = constrain(configuration.epiano.sidechain_time - ENCODER[ENC_R].speed(), SOUND_INTENSITY_MIN, SOUND_INTENSITY_MAX);
    //           // ep.setVolume(mapfloat(configuration.epiano.sidechain_time, SOUND_INTENSITY_MIN, SOUND_INTENSITY_MAX, 0, 1.0));
    //         }

    //       } else if (seq.temp_active_menu > 2 && seq.temp_active_menu < 5)  //microsynth
    //       {
    //         if (LCDML.BT_checkDown())
    //           microsynth[seq.temp_active_menu - 3].sidechain_time = constrain(microsynth[seq.temp_active_menu - 3].sidechain_time + ENCODER[ENC_R].speed(), SOUND_INTENSITY_MIN, SOUND_INTENSITY_MAX);
    //         else if (LCDML.BT_checkUp())
    //           microsynth[seq.temp_active_menu - 3].sidechain_time = constrain(microsynth[seq.temp_active_menu - 3].sidechain_time - ENCODER[ENC_R].speed(), SOUND_INTENSITY_MIN, SOUND_INTENSITY_MAX);
    //       } else if (seq.temp_active_menu == 5)  // braids
    //       {
    //         if (LCDML.BT_checkDown()) {
    //           braids_osc.sidechain_time = constrain(braids_osc.sidechain_time + ENCODER[ENC_R].speed(), SOUND_INTENSITY_MIN, SOUND_INTENSITY_MAX);
    //           // update_braids_volume();
    //         } else if (LCDML.BT_checkUp()) {
    //           braids_osc.sidechain_time = constrain(braids_osc.sidechain_time - ENCODER[ENC_R].speed(), SOUND_INTENSITY_MIN, SOUND_INTENSITY_MAX);
    //           // update_braids_volume();
    //         }
    //       } else if (seq.temp_active_menu == 6)  // msp1
    //       {
    //         if (LCDML.BT_checkDown())
    //           msp[0].sidechain_time = constrain(msp[0].sidechain_time + ENCODER[ENC_R].speed(), SOUND_INTENSITY_MIN, SOUND_INTENSITY_MAX);
    //         else if (LCDML.BT_checkUp())
    //           msp[0].sidechain_time = constrain(msp[0].sidechain_time - ENCODER[ENC_R].speed(), SOUND_INTENSITY_MIN, SOUND_INTENSITY_MAX);
    //       } else if (seq.temp_active_menu == 7)  // msp2
    //       {
    //         if (LCDML.BT_checkDown())
    //           msp[1].sidechain_time = constrain(msp[1].sidechain_time + ENCODER[ENC_R].speed(), SOUND_INTENSITY_MIN, SOUND_INTENSITY_MAX);
    //         else if (LCDML.BT_checkUp())
    //           msp[1].sidechain_time = constrain(msp[1].sidechain_time - ENCODER[ENC_R].speed(), SOUND_INTENSITY_MIN, SOUND_INTENSITY_MAX);
    //       }
    //       // else if (seq.temp_active_menu == 8)  //drums/samples
    //       // {
    //       //   if (LCDML.BT_checkDown())
    //       //     temp_int = constrain(temp_int + ENCODER[ENC_R].speed(), SOUND_INTENSITY_MIN, SOUND_INTENSITY_MAX);
    //       //   else if (LCDML.BT_checkUp())
    //       //     temp_int = constrain(temp_int - ENCODER[ENC_R].speed(), SOUND_INTENSITY_MIN, SOUND_INTENSITY_MAX);
    //       //   seq.drums_volume = mapfloat(temp_int, SOUND_INTENSITY_MIN, SOUND_INTENSITY_MAX, 0.0, VOL_MAX_FLOAT);
    //       // }
    //       else if (seq.temp_active_menu == 9)  //reverb level
    //       {
    //         if (LCDML.BT_checkDown())
    //           configuration.fx.reverb_sidechain_time = constrain(configuration.fx.reverb_sidechain_time + ENCODER[ENC_R].speed(), SOUND_INTENSITY_MIN, SOUND_INTENSITY_MAX);
    //         else if (LCDML.BT_checkUp())
    //           configuration.fx.reverb_sidechain_time = constrain(configuration.fx.reverb_sidechain_time - ENCODER[ENC_R].speed(), SOUND_INTENSITY_MIN, SOUND_INTENSITY_MAX);
    //       }
    //     }

    print_sidechain_editor_values();
    print_sidechain_level_indicators();

    //   if (seq.edit_state)
    //     helptext_r("SIDECHAIN TIME");
    //   else
    //     helptext_r("< > SELECT INST");
    //   display.setTextSize(2);
    //   if (seq.temp_active_menu < 2 && seq.edit_state)  // dexed 0 or dexed 1 instance selected
    //   {
    //     display.setTextColor(COLOR_SYSTEXT, COLOR_BACKGROUND);
    //     display_bar_int("", configuration.dexed[seq.temp_active_menu].sidechain_time, 1.0, SOUND_INTENSITY_MIN, SOUND_INTENSITY_MAX, 3, false, false, false);
    //     setCursor_textGrid(1, 1);
    //     display.print("DEXED #");
    //     display.print(seq.temp_active_menu + 1);
    //     //MD_sendControlChange(configuration.dexed[seq.temp_active_menu].midi_channel, 7, configuration.dexed[seq.temp_active_menu].sidechain_time);
    //     //MicroDexed[seq.temp_active_menu]->setGain(midi_volume_transform(map(configuration.dexed[seq.temp_active_menu].sidechain_time, SOUND_INTENSITY_MIN, SOUND_INTENSITY_MAX, 0, 127)));
    //   } else if (seq.temp_active_menu == 2 && seq.edit_state)  // epiano
    //   {
    //     display.setTextColor(COLOR_SYSTEXT, COLOR_BACKGROUND);
    //     display_bar_int("", configuration.epiano.sidechain_time, 1.0, SOUND_INTENSITY_MIN, SOUND_INTENSITY_MAX, 3, false, false, false);
    //     setCursor_textGrid(1, 1);
    //     display.print("EP");
    //   } else if (seq.temp_active_menu > 2 && seq.temp_active_menu < 5 && seq.edit_state)  //microsynth
    //   {
    //     display.setTextColor(COLOR_SYSTEXT, COLOR_BACKGROUND);
    //     display_bar_int("", microsynth[seq.temp_active_menu - 3].sidechain_time, 1.0, SOUND_INTENSITY_MIN, SOUND_INTENSITY_MAX, 3, false, false, false);

    //     setCursor_textGrid(1, 1);
    //     display.print("MICROSYNTH #");
    //     display.print(seq.temp_active_menu - 2);
    //   } else if (seq.temp_active_menu == 5 && seq.edit_state)  // braids
    //   {
    //     display.setTextColor(COLOR_SYSTEXT, COLOR_BACKGROUND);
    //     display_bar_int("", braids_osc.sidechain_time, 1.0, SOUND_INTENSITY_MIN, SOUND_INTENSITY_MAX, 3, false, false, false);
    //     setCursor_textGrid(1, 1);
    //     display.print("BRAIDS");
    //   } else if (seq.temp_active_menu == 6 && seq.edit_state)  // msp0
    //   {
    //     display.setTextColor(COLOR_SYSTEXT, COLOR_BACKGROUND);
    //     display_bar_int("", msp[0].sidechain_time, 1.0, SOUND_INTENSITY_MIN, SOUND_INTENSITY_MAX, 3, false, false, false);
    //     setCursor_textGrid(1, 1);
    //     display.print("MULTISAMPLE #");
    //     display.print(seq.temp_active_menu - 5);
    //   } else if (seq.temp_active_menu == 7 && seq.edit_state)  // msp1
    //   {
    //     display.setTextColor(COLOR_SYSTEXT, COLOR_BACKGROUND);
    //     display_bar_int("", msp[1].sidechain_time, 1.0, SOUND_INTENSITY_MIN, SOUND_INTENSITY_MAX, 3, false, false, false);
    //     setCursor_textGrid(1, 1);
    //     display.print("MULTISAMPLE #");
    //     display.print(seq.temp_active_menu - 5);
    //   }
    //   //  else if (seq.temp_active_menu == 8 && seq.edit_state)  // drums
    //   // {
    //   //   display.setTextColor(COLOR_SYSTEXT, COLOR_BACKGROUND);
    //   //   temp_int = mapfloat(seq.drums_volume, 0.0, VOL_MAX_FLOAT, 0, 100);
    //   //   display_bar_int("DRUMS VOLUME", temp_int, 1.0, SOUND_INTENSITY_MIN, SOUND_INTENSITY_MAX, 3, false, false, true);
    //   //   master_mixer_r.gain(3, volume_transform(mapfloat(temp_int, 0, 100, 0.0, VOL_MAX_FLOAT)));
    //   //   master_mixer_l.gain(3, volume_transform(mapfloat(temp_int, 0, 100, 0.0, VOL_MAX_FLOAT)));
    //   // }
    //   else if (seq.temp_active_menu == 9 && seq.edit_state)  // reverb level
    //   {
    //     display.setTextColor(COLOR_SYSTEXT, COLOR_BACKGROUND);
    //     setCursor_textGrid(1, 1);
    //     display.print(F("REVERB SC TIME"));
    //     display_bar_int("", configuration.fx.reverb_sidechain_time, 1.0, SOUND_INTENSITY_MIN, SOUND_INTENSITY_MAX, 3, false, false, false);
    //     // master_mixer_r.gain(MASTER_MIX_CH_REVERB, volume_transform(mapfloat(configuration.fx.reverb_level, REVERB_LEVEL_MIN, REVERB_LEVEL_MAX, 0.0, VOL_MAX_FLOAT)));
    //     // master_mixer_l.gain(MASTER_MIX_CH_REVERB, volume_transform(mapfloat(configuration.fx.reverb_level, REVERB_LEVEL_MIN, REVERB_LEVEL_MAX, 0.0, VOL_MAX_FLOAT)));
    //   } else if (seq.temp_active_menu == 10 && seq.edit_state)  // master volume
    //   {
    //     display.setTextColor(COLOR_SYSTEXT, COLOR_BACKGROUND);
    //     setCursor_textGrid(1, 1);
    //     display.print(F("MASTER VOLUME"));
    //     display_bar_int("", configuration.sys.vol, 1.0, VOLUME_MIN, VOLUME_MAX, 3, false, false, false);
    //     set_volume(configuration.sys.vol, configuration.sys.mono);
    //   }
    //   display.setTextSize(1);
  }

  if (LCDML.FUNC_close()) // ****** STABLE END *********
  {
    encoderDir[ENC_R].reset();
    display.fillScreen(COLOR_BACKGROUND);
  }
}

FLASHMEM void UI_func_smart_filter(uint8_t param)
{
  if (LCDML.FUNC_setup()) // ****** SETUP *********
  {
    encoderDir[ENC_R].reset();

    setCursor_textGrid(1, 1);
    display.print(F("Drm Smart Filter"));
  }
  if (LCDML.FUNC_loop()) // ****** LOOP *********
  {
    if ((LCDML.BT_checkDown() && encoderDir[ENC_R].Down()) || (LCDML.BT_checkUp() && encoderDir[ENC_R].Up()))
    {
      if (LCDML.BT_checkDown())
        seq.smartfilter = !seq.smartfilter;
      else if (LCDML.BT_checkUp())
        seq.smartfilter = !seq.smartfilter;
    }
    setCursor_textGrid(1, 2);
    if (seq.smartfilter)
      display.print(F("[ON ]"));
    else
      display.print(F("[OFF]"));
  }

  // #ifdef DEBUG
  //   print_used_samples();
  // #endif

  if (LCDML.FUNC_close()) // ****** STABLE END *********
  {
    encoderDir[ENC_R].reset();
  }
}

uint8_t dexed_onscreen_algo = 0;

FLASHMEM void  print_voice_select_fav_search_button()
{
  switch (configuration.sys.favorites)
  {
  case 0:
    draw_button_on_grid(45, 11, "ALL", "PRESET", 0);
    break;
  case 1:
    draw_button_on_grid(45, 11, "FAVs.", "ONLY", 0);
    break;
  case 2:
    draw_button_on_grid(45, 11, "NON", "FAVs.", 0);
    break;
  case 3:
    draw_button_on_grid(45, 11, "RND", "NONFAV", 0);
    break;
  }

}

FLASHMEM void print_voice_settings_in_dexed_voice_select(bool fullrefresh_text, bool fullrefresh_values)
{
  char note_name[4];
  display.setTextSize(1);

  // static content
  if (fullrefresh_text)
  {
    display.setTextSize(2);
    display.setTextColor(GREY2, COLOR_BACKGROUND);
    display.setCursor(2 * CHAR_width_small, 2 * CHAR_height_small + 3);
    display.print(F("B"));

    display.setCursor(2 * CHAR_width_small, 4 * CHAR_height_small + 5);
    display.print(F("V"));

    display.setTextSize(1);

    display.setTextColor(COLOR_SYSTEXT, COLOR_BACKGROUND);
    display.setCursor(2 * CHAR_width_small + 1, 6);
    display.print(F("POOL "));
    setCursor_textGrid_small(2, 6);
    display.setCursor(CHAR_width_small * 25, 6);
    display.print(F("DEXED"));
    setCursor_textGrid_small(2, 6);
    display.setTextColor(GREY1, COLOR_BACKGROUND);
    display.print(F("VOLUME"));
    setCursor_textGrid_small(2, 7);
    display.print(F("TRANSP."));
    if (seq.cycle_touch_element != 1)
    {
      setCursor_textGrid_small(2, 9);
      display.print(F("PAN"));
      setCursor_textGrid_small(2, 10);
      display.print(F("LOW NOTE"));
      setCursor_textGrid_small(2, 11);
      display.print(F("HI NOTE"));
      setCursor_textGrid_small(2, 12);
      display.print(F("MIDI CH"));
      setCursor_textGrid_small(2, 14);
      display.print(F("CHORUS"));
      setCursor_textGrid_small(2, 15);
      display.print(F("DELAY"));
      setCursor_textGrid_small(2, 16);
      display.print(F("REVERB"));
      display.console = true;
      display.drawLine(1, CHAR_height * 5 - 2, DISPLAY_WIDTH - 2, CHAR_height * 5 - 2, GREY4);
      display.drawLine(CHAR_width * 18, 1, CHAR_width * 18, CHAR_height * 5 - 2, GREY4);
    }
    setCursor_textGrid_small(26, 9);
    display.setTextSize(1);
    display.setTextColor(GREY1);
    if (seq.cycle_touch_element != 1)
    {
      display.print(F("ALGORITHM"));
      print_voice_select_fav_search_button();
      draw_button_on_grid(20, 23, "CLEAR", "MODS", 0);
      draw_button_on_grid(37, 23, "EDIT", "ALGO", 0);
      draw_button_on_grid(45, 23, "EDIT", "VOICE", 0);

      if ((dexed_onscreen_algo != MicroDexed[selected_instance_id]->getAlgorithm()))
      {
        UI_draw_FM_algorithm(MicroDexed[selected_instance_id]->getAlgorithm(), 147, 86);
        dexed_onscreen_algo = MicroDexed[selected_instance_id]->getAlgorithm();
      }
    }
  }
  display.setTextSize(1);
  if (menu_item_check(0) || fullrefresh_values)
  {
    setModeColor(0);
    display.setCursor(CHAR_width_small * 7, 6);
    print_formatted_number(configuration.dexed[selected_instance_id].pool, 2);
  }
  if (menu_item_check(4) || fullrefresh_values)
  {
    print_small_intbar(11, 6, configuration.dexed[selected_instance_id].sound_intensity, 4, 1, 1);
  }
  if (menu_item_check(5) || menu_item_check(0) || menu_item_check(2) || menu_item_check(3) || fullrefresh_values)
  {
    setModeColor(5);
    setCursor_textGrid_small(11, 7);
    print_formatted_number_signed(configuration.dexed[selected_instance_id].transpose - 24, 2);
  }
  if (seq.cycle_touch_element != 1)
  {

    if (menu_item_check(6) || fullrefresh_values)
      print_small_panbar(11, 9, configuration.dexed[selected_instance_id].pan, 6);
    if (menu_item_check(7) || fullrefresh_values)
    {
      setModeColor(7);
      setCursor_textGrid_small(11, 10);
      print_formatted_number(configuration.dexed[selected_instance_id].lowest_note, 3);
      display.setTextColor(GREY1, COLOR_BACKGROUND);
      getNoteName(note_name, configuration.dexed[selected_instance_id].lowest_note);
      setCursor_textGrid_small(15, 10);
      display.print(note_name);
    }
    if (menu_item_check(8) || fullrefresh_values)
    {
      setModeColor(8);
      setCursor_textGrid_small(11, 11);
      print_formatted_number(configuration.dexed[selected_instance_id].highest_note, 3);
      getNoteName(note_name, configuration.dexed[selected_instance_id].highest_note);
      setCursor_textGrid_small(15, 11);
      display.setTextColor(GREY1, COLOR_BACKGROUND);
      display.print(note_name);
    }
    if (menu_item_check(9) || fullrefresh_values)
    {
      setCursor_textGrid_small(11, 12);
      setModeColor(9);
      if (configuration.dexed[selected_instance_id].midi_channel == MIDI_CHANNEL_OMNI)
      {
        display.print(F("OMNI"));
      }
      else
      {
        print_formatted_number_trailing_space(configuration.dexed[selected_instance_id].midi_channel, 4);
      }
    }
    if (menu_item_check(10) || fullrefresh_values)
      print_small_intbar(11, 14, configuration.fx.chorus_level[selected_instance_id], 10, 1, 1);
    if (menu_item_check(11) || fullrefresh_values)
      print_small_intbar(11, 15, configuration.fx.delay_level[selected_instance_id], 11, 1, 1);
    if (menu_item_check(12) || fullrefresh_values)
      print_small_intbar(11, 16, configuration.fx.reverb_send[selected_instance_id], 12, 1, 1);
    if (menu_item_check(13) || fullrefresh_values)
    {
      setModeColor(13);
      setCursor_textGrid_small(28, 6);
      print_formatted_number(seq.bpm, 3);

    }
  }
}

void toggle_dexed_instance_in_voice_select()
{
  if (selected_instance_id == 0)
    selected_instance_id = 1;
  else
    selected_instance_id = 0;
  print_voice_settings_in_dexed_voice_select(false, true);
  UI_update_instance_icons();
};

FLASHMEM void UI_func_velocity_level(uint8_t param)
{
  if (LCDML.FUNC_setup()) // ****** SETUP *********
  {
    encoderDir[ENC_R].reset();
    display_bar_int("Velocity Lvl", configuration.dexed[selected_instance_id].velocity_level, 1.0, VELOCITY_LEVEL_MIN, VELOCITY_LEVEL_MAX, 3, false, false, true);

    UI_update_instance_icons();
  }

  if (LCDML.FUNC_loop()) // ****** LOOP *********
  {
    if ((LCDML.BT_checkDown() && encoderDir[ENC_R].Down()) || (LCDML.BT_checkUp() && encoderDir[ENC_R].Up()))
    {
      if (LCDML.BT_checkDown())
        configuration.dexed[selected_instance_id].velocity_level = constrain(configuration.dexed[selected_instance_id].velocity_level + ENCODER[ENC_R].speed(), VELOCITY_LEVEL_MIN, VELOCITY_LEVEL_MAX);
      else if (LCDML.BT_checkUp())
        configuration.dexed[selected_instance_id].velocity_level = constrain(configuration.dexed[selected_instance_id].velocity_level - ENCODER[ENC_R].speed(), VELOCITY_LEVEL_MIN, VELOCITY_LEVEL_MAX);
    }
    else if (LCDML.BT_checkEnter())
    {
      selected_instance_id = !selected_instance_id;

      UI_update_instance_icons();
    }
    display_bar_int("Velocity Lvl", configuration.dexed[selected_instance_id].velocity_level, 1.0, VELOCITY_LEVEL_MIN, VELOCITY_LEVEL_MAX, 3, false, false, false);
  }
  if (LCDML.FUNC_close()) // ****** STABLE END *********
  {
    encoderDir[ENC_R].reset();
  }
}

FLASHMEM void UI_update_instance_icons()
{
  // display.fillRect(14 * CHAR_width + 16, 5, 20, 9, COLOR_BACKGROUND); //should not be required, testing
  display.setTextSize(1);
  display.console = true;
  if (selected_instance_id == 0)
  {
    display.fillRect(14 * CHAR_width + 17, 5, 9, 9, COLOR_SYSTEXT);
    display.setCursor(14 * CHAR_width + 19, 6);
    if (configuration.dexed[0].polyphony == 0)
      display.setTextColor(RED, COLOR_SYSTEXT);
    else
      display.setTextColor(COLOR_BACKGROUND, COLOR_SYSTEXT);
    display.print(F("1"));
    display.console = true;
    display.fillRect(15 * CHAR_width + 17, 5, 9, 9, COLOR_BACKGROUND);
    display.setCursor(15 * CHAR_width + 19, 6);
    if (configuration.dexed[1].polyphony == 0)
      display.setTextColor(RED, COLOR_BACKGROUND);
    else
      display.setTextColor(COLOR_SYSTEXT, COLOR_BACKGROUND);
    display.print(F("2"));
  }
  else
  {
    display.console = true;
    display.fillRect(14 * CHAR_width + 17, 5, 9, 9, COLOR_BACKGROUND);
    display.setCursor(14 * CHAR_width + 19, 6);
    if (configuration.dexed[0].polyphony == 0)
      display.setTextColor(RED, COLOR_BACKGROUND);
    else
      display.setTextColor(COLOR_SYSTEXT, COLOR_BACKGROUND);
    display.print(F("1"));
    display.console = true;
    display.fillRect(15 * CHAR_width + 17, 5, 9, 9, COLOR_SYSTEXT);
    display.setCursor(15 * CHAR_width + 19, 6);
    if (configuration.dexed[1].polyphony == 0)
      display.setTextColor(RED, COLOR_SYSTEXT);
    else
      display.setTextColor(COLOR_BACKGROUND, COLOR_SYSTEXT);
    display.print(F("2"));
  }
  display.setTextSize(2);
  display.setTextColor(COLOR_SYSTEXT, COLOR_BACKGROUND);
}

FLASHMEM void print_perfmod_lables()
{
  if (seq.cycle_touch_element != 1) {
    char tmp[5];
    display.setTextSize(1);
    if (dexed_live_mod.active_button == 1 || dexed_live_mod.active_button == 3)
      display.setTextColor(COLOR_SYSTEXT, DX_DARKCYAN);
    else
      display.setTextColor(GREY1, GREY2);
    display.setCursor(5 * CHAR_width_small + 3, 25 * CHAR_height_small + 2);
    snprintf_P(tmp, sizeof(tmp), PSTR("%03d"), dexed_live_mod.attack_mod[selected_instance_id]);
    display.print(tmp);
    if (dexed_live_mod.active_button == 2 || dexed_live_mod.active_button == 4)
      display.setTextColor(COLOR_SYSTEXT, DX_DARKCYAN);
    else
      display.setTextColor(GREY1, GREY2);
    display.setCursor(14 * CHAR_width_small + 3, 25 * CHAR_height_small + 2);
    snprintf_P(tmp, sizeof(tmp), PSTR("%03d"), dexed_live_mod.release_mod[selected_instance_id]);
    display.print(tmp);
    display.setTextColor(COLOR_SYSTEXT, COLOR_BACKGROUND);
  }
}

FLASHMEM void print_perfmod_buttons()
{
  // Performance Modifier Buttons
  if (seq.cycle_touch_element != 1) {
    if (selected_instance_id == 0)
    {
      if (dexed_live_mod.active_button == 1)
        draw_button_on_grid(2, 23, "ATTACK", "MD", 1);
      else
        draw_button_on_grid(2, 23, "ATTACK", "MD", 0);
      if (dexed_live_mod.active_button == 2)
        draw_button_on_grid(11, 23, "REL.", "MD", 1);
      else
        draw_button_on_grid(11, 23, "REL.", "MD", 0);
    }
    else if (selected_instance_id == 1)
    {
      if (dexed_live_mod.active_button == 3)
        draw_button_on_grid(2, 23, "ATTACK", "MD", 1);
      else
        draw_button_on_grid(2, 23, "ATTACK", "MD", 0);
      if (dexed_live_mod.active_button == 4)
        draw_button_on_grid(11, 23, "REL.", "MD", 1);
      else
        draw_button_on_grid(11, 23, "REL.", "MD", 0);
    }
  }
}

FLASHMEM void print_drumpads()
{
  uint8_t offset = 14;
  if (seq.cycle_touch_element == 1) {
    char tmp[14];
    char tmp2[14];
    for (uint8_t x = 0; x < 6; x++)
    {
      snprintf_P(tmp, sizeof(tmp), PSTR("%.6s"), drum_config[x + ts.virtual_keyboard_octave * 12 - offset].name);
      snprintf_P(tmp2, sizeof(tmp2), PSTR("%.6s"), &drum_config[x + ts.virtual_keyboard_octave * 12 - offset].name[6]);
      if (x + ts.virtual_keyboard_octave * 12 - offset < NUM_DRUMSET_CONFIG && x + ts.virtual_keyboard_octave * 12 - offset >= 0)
        draw_button_on_grid(x * 9 + 1, 21, tmp, tmp2, 1);
      else
        draw_button_on_grid(x * 9 + 1, 21, "", "", 1);

      snprintf_P(tmp, sizeof(tmp), PSTR("%.6s"), drum_config[x + 6 + ts.virtual_keyboard_octave * 12 - offset].name);
      snprintf_P(tmp2, sizeof(tmp2), PSTR("%.6s"), &drum_config[x + 6 + ts.virtual_keyboard_octave * 12 - offset].name[6]);
      if (x + ts.virtual_keyboard_octave * 12 - offset < NUM_DRUMSET_CONFIG && x + ts.virtual_keyboard_octave * 12 - offset >= 0)
        draw_button_on_grid(x * 9 + 1, 26, tmp, tmp2, 1);
      else
        draw_button_on_grid(x * 9 + 1, 26, "", "", 1);
    }
  }
}

FLASHMEM void print_voice_select_default_help()
{
  if (seq.cycle_touch_element != 1)
  {
    helptext_l(back_text);
    helptext_r("< > SELECT VOICE");
    display.setTextColor(GREY2, COLOR_BACKGROUND);
    display.setCursor(0, DISPLAY_HEIGHT - (CHAR_height_small * 2) - 2);
    display.print(F("LONG:"));
    display.setTextColor(COLOR_ARP, COLOR_BACKGROUND);
    if (seq.running)
      display.print(F("STOP SEQUENCER "));
    else
      display.print(F("START SEQUENCER"));
    display.setTextColor(GREY2, COLOR_BACKGROUND);
    display.setCursor(CHAR_width_small * 25 + 2, DISPLAY_HEIGHT - (CHAR_height_small * 2) - 2);
    display.print(F("SHORT:"));
    display.setTextColor(COLOR_ARP, COLOR_BACKGROUND);
    if (generic_active_function == 1)
      display.print(F("APPLY"));
    else if (generic_active_function == 0)
      display.print(F(" EDIT"));
    display.setTextColor(GREY2, COLOR_BACKGROUND);
    display.print(F(" LONG:"));
    display.setTextColor(COLOR_ARP, COLOR_BACKGROUND);
    display.print(F("TOGGLE INST"));
  }
}

FLASHMEM void UI_func_voice_select_touch_loop()
{
  if ((LCDML.BT_checkDown() && encoderDir[ENC_R].Down()) || (LCDML.BT_checkUp() && encoderDir[ENC_R].Up()))
  {
    if ((LCDML.BT_checkDown() && dexed_live_mod.active_button == 1) || (LCDML.BT_checkDown() && dexed_live_mod.active_button == 3))
    {
      if (dexed_live_mod.attack_mod[selected_instance_id] == 0)
        for (uint8_t i = 0; i < 6; i++)
        {
          dexed_live_mod.orig_attack_values[selected_instance_id][i] = MicroDexed[selected_instance_id]->getOPRate(i, ATTACK);
        }
      dexed_live_mod.attack_mod[selected_instance_id] = constrain(dexed_live_mod.attack_mod[selected_instance_id] + ENCODER[ENC_L].speed(), -MAX_PERF_MOD, MAX_PERF_MOD);
      for (uint8_t i = 0; i < 6; i++)
        MicroDexed[selected_instance_id]->setOPRate(i, ATTACK, dexed_live_mod.orig_attack_values[selected_instance_id][i] - dexed_live_mod.attack_mod[selected_instance_id]);
    }
    else if ((LCDML.BT_checkUp() && dexed_live_mod.active_button == 1) || (LCDML.BT_checkUp() && dexed_live_mod.active_button == 3))
    {
      if (dexed_live_mod.attack_mod[selected_instance_id] == 0) // Save initial Values
        for (uint8_t i = 0; i < 6; i++)
        {
          dexed_live_mod.orig_attack_values[selected_instance_id][i] = MicroDexed[selected_instance_id]->getOPRate(i, ATTACK);
        }

      dexed_live_mod.attack_mod[selected_instance_id] = constrain(dexed_live_mod.attack_mod[selected_instance_id] - ENCODER[ENC_L].speed(), -MAX_PERF_MOD, MAX_PERF_MOD);
      for (uint8_t i = 0; i < 6; i++)
        MicroDexed[selected_instance_id]->setOPRate(i, ATTACK, dexed_live_mod.orig_attack_values[selected_instance_id][i] - dexed_live_mod.attack_mod[selected_instance_id]);
    }
    if ((LCDML.BT_checkDown() && dexed_live_mod.active_button == 2) || (LCDML.BT_checkDown() && dexed_live_mod.active_button == 4))
    {
      if (dexed_live_mod.release_mod[selected_instance_id] == 0) // Save initial Values
        for (uint8_t i = 0; i < 6; i++)
        {
          dexed_live_mod.orig_release_values[selected_instance_id][i] = MicroDexed[selected_instance_id]->getOPRate(i, RELEASE);
        }
      dexed_live_mod.release_mod[selected_instance_id] = constrain(dexed_live_mod.release_mod[selected_instance_id] + ENCODER[ENC_L].speed(), -MAX_PERF_MOD, MAX_PERF_MOD);
      for (uint8_t i = 0; i < 6; i++)
        MicroDexed[selected_instance_id]->setOPRate(i, RELEASE, dexed_live_mod.orig_release_values[selected_instance_id][i] - dexed_live_mod.release_mod[selected_instance_id]);
    }
    else if ((LCDML.BT_checkUp() && dexed_live_mod.active_button == 2) || (LCDML.BT_checkUp() && dexed_live_mod.active_button == 4))
    {
      if (dexed_live_mod.release_mod[selected_instance_id] == 0)
        for (uint8_t i = 0; i < 6; i++)
        {
          dexed_live_mod.orig_release_values[selected_instance_id][i] = MicroDexed[selected_instance_id]->getOPRate(i, RELEASE);
        }
      dexed_live_mod.release_mod[selected_instance_id] = constrain(dexed_live_mod.release_mod[selected_instance_id] - ENCODER[ENC_L].speed(), -MAX_PERF_MOD, MAX_PERF_MOD);
      for (uint8_t i = 0; i < 6; i++)
        MicroDexed[selected_instance_id]->setOPRate(i, RELEASE, dexed_live_mod.orig_release_values[selected_instance_id][i] - dexed_live_mod.release_mod[selected_instance_id]);
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

FLASHMEM void reset_live_modifiers()
{
  dexed_live_mod.attack_mod[selected_instance_id] = 0;
  dexed_live_mod.release_mod[selected_instance_id] = 0;

}

FLASHMEM void UI_func_voice_select_loop()
{
  static uint8_t menu_voice_select = MENU_VOICE_SOUND;

  if (generic_temp_select_menu == 2)
    menu_voice_select = MENU_VOICE_BANK;
  if (generic_temp_select_menu == 3)
    menu_voice_select = MENU_VOICE_SOUND;

  if ((LCDML.BT_checkDown() && encoderDir[ENC_R].Down()) || (LCDML.BT_checkUp() && encoderDir[ENC_R].Up()) || (LCDML.BT_checkEnter() && (encoderDir[ENC_R].ButtonShort() || encoderDir[ENC_R].ButtonLong())))
  {
    uint8_t bank_tmp;
    int8_t voice_tmp;

    // Reset Performance Modifiers to 0 after every preset change
    reset_live_modifiers();
    if (seq.cycle_touch_element != 1)
      print_perfmod_lables();

    if (LCDML.BT_checkUp())
    {
      // start : show all presets
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
          voice_tmp = configuration.dexed[selected_instance_id].voice - 1;
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
      else
      {
        switch (configuration.sys.favorites)
        {
        case 1: // only Favs
          locate_previous_favorite();
          break;
        case 2: // only non-Favs
          locate_previous_non_favorite();
          break;
        case 3: // random non-Favs
          locate_random_non_favorite();
          break;
        }
      }

      load_sd_voice(configuration.dexed[selected_instance_id].pool, configuration.dexed[selected_instance_id].bank, configuration.dexed[selected_instance_id].voice, selected_instance_id);
      menuhelper_redraw = true;
    } // end UP
    else if (LCDML.BT_checkDown())
    {
      // start : show all presets
      if (configuration.sys.favorites == 0)
      {
        switch (menu_voice_select)
        {
        case MENU_VOICE_BANK:
          bank_tmp = constrain(configuration.dexed[selected_instance_id].bank + ENCODER[ENC_R].speed(), 0, MAX_BANKS - 1);
          configuration.dexed[selected_instance_id].bank = bank_tmp;
          break;

        case MENU_VOICE_SOUND:
          voice_tmp = configuration.dexed[selected_instance_id].voice + 1;
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
          configuration.dexed[selected_instance_id].voice = constrain(voice_tmp, 0, MAX_VOICES - 1);
          break;
        }
      }
      else
      { // favs
        switch (configuration.sys.favorites)
        {
        case 1: // only Favs
          locate_next_favorite();
          break;
        case 2: // only non-Favs
          locate_next_non_favorite();
          break;
        case 3: // random non-Favs
          locate_random_non_favorite();
          break;
        }
      }

      load_sd_voice(configuration.dexed[selected_instance_id].pool, configuration.dexed[selected_instance_id].bank, configuration.dexed[selected_instance_id].voice, selected_instance_id);
      menuhelper_redraw = true;
    }
    else if (LCDML.BT_checkEnter() && encoderDir[ENC_R].ButtonPressed() && dexed_live_mod.active_button != 99)
    {
      if (generic_active_function == 0)
        generic_active_function = 1;
      else
        generic_active_function = 0;
      menuhelper_redraw = true;
    }
  }
}

FLASHMEM void UI_func_voice_select(uint8_t param)
{
  if (generic_active_function > 98)
    generic_active_function = 0;

  if (LCDML.FUNC_setup()) // ****** SETUP *********
  {
    dexed_onscreen_algo = 88; // dummy value to force draw on screen init
    display.fillScreen(COLOR_BACKGROUND);
    border0();
    virtual_keyboard_smart_preselect_mode();
    ts.fav_buttton_state = 0; //clear touch button state when starting page
    seq.cycle_touch_element = 0;
    if (LCDML.MENU_getLastActiveFunctionID() != LCDML.OTHER_getIDFromFunction(UI_func_volume) && LCDML.MENU_getLastActiveFunctionID() != LCDML.OTHER_getIDFromFunction(mFunc_screensaver))
      generic_temp_select_menu = 3;
    draw_button_on_grid(45, 1, "", "", 99); // print keyboard icon
    print_voice_settings_in_dexed_voice_select(true, true);
    print_voice_select_default_help();
    print_perfmod_buttons();
    print_perfmod_lables();
    UI_update_instance_icons();
    draw_favorite_icon(configuration.dexed[selected_instance_id].pool, configuration.dexed[selected_instance_id].bank, configuration.dexed[selected_instance_id].voice, selected_instance_id);
    display.setTextColor(COLOR_PITCHSMP, COLOR_BACKGROUND);
    display.setTextSize(1);
    setCursor_textGrid_small(22, 6);
    display.print(F("P#"));
    display.setTextColor(GREY1, COLOR_BACKGROUND);
    setCursor_textGrid_small(32, 6);
    display.print(F("BPM"));
    setCursor_textGrid_small(22, 7);
    display.setTextColor(COLOR_PITCHSMP, COLOR_BACKGROUND);
    print_formatted_number(configuration.sys.performance_number, 2);
    display.setTextColor(COLOR_BACKGROUND, COLOR_PITCHSMP);
    show_smallfont_noGrid(9 * CHAR_height_small - 2, CHAR_width_small * 25, 10, seq.name);
    display.setTextColor(COLOR_SYSTEXT, COLOR_BACKGROUND);

    encoderDir[ENC_R].reset();
  }

  if (LCDML.FUNC_loop()) // ****** LOOP *********
  {
    if (dexed_live_mod.active_button != 0) // touch button pressed for live modify
      UI_func_voice_select_touch_loop();

    if (dexed_live_mod.active_button == 0)
    {
      if ((seq.edit_state == 1 && generic_temp_select_menu == 2) || (seq.edit_state == 1 && generic_temp_select_menu == 3))

        UI_func_voice_select_loop();
    }
    // generic_temp_select_menu
    // 0 = bank select
    // 1 = instance select
    // 2 = bank select
    // 3 = voice select
    // 4 = volume
    //

    if ((LCDML.BT_checkDown() && encoderDir[ENC_R].Down()) || (LCDML.BT_checkUp() && encoderDir[ENC_R].Up()))
    {
      set_state_dir();

      if (seq.edit_state == 0)
      {
        generic_temp_select_menu = calc_val[state_dir](generic_temp_select_menu, 1, 0, 13);
        if (generic_temp_select_menu != menuhelper_previous_val)
          menuhelper_redraw = true;
      }
      else if (seq.edit_state == 1)
      {
        if (generic_temp_select_menu == 0)
        {
          if (LCDML.BT_checkDown() || LCDML.BT_checkUp())
          {
            configuration.dexed[selected_instance_id].pool = calc_val[state_dir](configuration.dexed[selected_instance_id].pool, 1, 0, DEXED_POOLS - 1);
            if (load_sd_voice(configuration.dexed[selected_instance_id].pool, configuration.dexed[selected_instance_id].bank, configuration.dexed[selected_instance_id].voice, selected_instance_id) == false)
              load_sd_voice(configuration.dexed[selected_instance_id].pool, 0, 0, selected_instance_id);
          }
        }
        if (generic_temp_select_menu == 1)
        {
          if (LCDML.BT_checkDown() || LCDML.BT_checkUp())
          {
            toggle_dexed_instance_in_voice_select();
            draw_favorite_icon(configuration.dexed[selected_instance_id].pool, configuration.dexed[selected_instance_id].bank, configuration.dexed[selected_instance_id].voice, selected_instance_id);
            load_sd_voice(configuration.dexed[selected_instance_id].pool, configuration.dexed[selected_instance_id].bank, configuration.dexed[selected_instance_id].voice, selected_instance_id);
          }
        }

        if (generic_temp_select_menu == 4)
        {
          configuration.dexed[selected_instance_id].sound_intensity = calc_val[state_dir](configuration.dexed[selected_instance_id].sound_intensity, 1, SOUND_INTENSITY_MIN, SOUND_INTENSITY_MAX);
          MD_sendControlChange(configuration.dexed[selected_instance_id].midi_channel, 7, configuration.dexed[selected_instance_id].sound_intensity);
          MicroDexed[selected_instance_id]->setGain(midi_volume_transform(map(configuration.dexed[selected_instance_id].sound_intensity, SOUND_INTENSITY_MIN, SOUND_INTENSITY_MAX, 0, 127)));
        }
        if (generic_temp_select_menu == 5)
        {
          configuration.dexed[selected_instance_id].transpose = calc_val[state_dir](configuration.dexed[selected_instance_id].transpose, 1, TRANSPOSE_MIN, TRANSPOSE_MAX);
          MicroDexed[selected_instance_id]->setTranspose(configuration.dexed[selected_instance_id].transpose);
          MicroDexed[selected_instance_id]->notesOff();
          send_sysex_param(configuration.dexed[selected_instance_id].midi_channel, 144, configuration.dexed[selected_instance_id].transpose, 0);
        }
        if (generic_temp_select_menu == 6)
        {
          configuration.dexed[selected_instance_id].pan = calc_val[state_dir](configuration.dexed[selected_instance_id].pan, ENCODER[ENC_R].speed(), PANORAMA_MIN, PANORAMA_MAX);
          // MD_sendControlChange(configuration.dexed[selected_instance_id].midi_channel, 10, map(configuration.dexed[selected_instance_id].pan, PANORAMA_MIN, PANORAMA_MAX, 0, 127));
          dexed_dry_mono2stereo[selected_instance_id]->panorama(mapfloat(configuration.dexed[selected_instance_id].pan, PANORAMA_MIN, PANORAMA_MAX, -1.0, 1.0));
        }
        if (generic_temp_select_menu == 7) // lowest note
        {
          configuration.dexed[selected_instance_id].lowest_note = calc_val[state_dir](configuration.dexed[selected_instance_id].lowest_note, ENCODER[ENC_R].speed(), INSTANCE_LOWEST_NOTE_MIN, INSTANCE_LOWEST_NOTE_MAX);
        }
        if (generic_temp_select_menu == 8)
        {
          configuration.dexed[selected_instance_id].highest_note = calc_val[state_dir](configuration.dexed[selected_instance_id].highest_note, ENCODER[ENC_R].speed(), INSTANCE_HIGHEST_NOTE_MIN, INSTANCE_HIGHEST_NOTE_MAX);
        }
        if (generic_temp_select_menu == 9) // MIDI CHANNEL
        {
          configuration.dexed[selected_instance_id].midi_channel = calc_val[state_dir](configuration.dexed[selected_instance_id].midi_channel, 1, MIDI_CHANNEL_MIN, MIDI_CHANNEL_MAX);
        }
        if (generic_temp_select_menu == 10)
        {
          configuration.fx.chorus_level[selected_instance_id] = calc_val[state_dir](configuration.fx.chorus_level[selected_instance_id], ENCODER[ENC_R].speed(), CHORUS_LEVEL_MIN, CHORUS_LEVEL_MAX);
          MD_sendControlChange(configuration.dexed[selected_instance_id].midi_channel, 93, configuration.fx.chorus_level[selected_instance_id]);
          // global_delay_in_mixer[selected_instance_id]->gain(1, mapfloat(configuration.fx.chorus_level[selected_instance_id], CHORUS_LEVEL_MIN, CHORUS_LEVEL_MAX, 0.0, 0.9));
        }
        if (generic_temp_select_menu == 11) // DELAY
        {
          configuration.fx.delay_level[selected_instance_id] = calc_val[state_dir](configuration.fx.delay_level[selected_instance_id], ENCODER[ENC_R].speed(), DELAY_LEVEL_MIN, DELAY_LEVEL_MAX);
          MD_sendControlChange(configuration.dexed[selected_instance_id].midi_channel, 107, configuration.fx.delay_level[selected_instance_id]);
          global_delay_in_mixer[selected_instance_id]->gain(0, mapfloat(configuration.fx.delay_level[selected_instance_id], DELAY_LEVEL_MIN, DELAY_LEVEL_MAX, 0.0, 1.0));
        }
        if (generic_temp_select_menu == 12) // REVERB SEND
        {
          configuration.fx.reverb_send[selected_instance_id] = calc_val[state_dir](configuration.fx.reverb_send[selected_instance_id], ENCODER[ENC_R].speed(), REVERB_SEND_MIN, REVERB_SEND_MAX);
          MD_sendControlChange(configuration.dexed[selected_instance_id].midi_channel, 91, configuration.fx.reverb_send[selected_instance_id]);
          reverb_mixer_r.gain(selected_instance_id, volume_transform(mapfloat(configuration.fx.reverb_send[selected_instance_id], REVERB_SEND_MIN, REVERB_SEND_MAX, 0.0, VOL_MAX_FLOAT)));
          reverb_mixer_l.gain(selected_instance_id, volume_transform(mapfloat(configuration.fx.reverb_send[selected_instance_id], REVERB_SEND_MIN, REVERB_SEND_MAX, 0.0, VOL_MAX_FLOAT)));
        }
        if (generic_temp_select_menu == 13) // BPM
        {
          seq.bpm = calc_val[state_dir](seq.bpm, 1, 40, 190);
          update_seq_speed();
        }
      }
    }
    if (LCDML.BT_checkEnter() && encoderDir[ENC_R].ButtonPressed() && dexed_live_mod.active_button != 99)
    {
      toggle_dexed_instance_in_voice_select();
      virtual_keyboard_smart_preselect_mode();
      draw_favorite_icon(configuration.dexed[selected_instance_id].pool, configuration.dexed[selected_instance_id].bank, configuration.dexed[selected_instance_id].voice, selected_instance_id);
    }
    else if (LCDML.BT_checkEnter()) // handle button presses during menu >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
    {
      if (seq.edit_state == 0)
        seq.edit_state = 1;
      else
        seq.edit_state = 0;
      menuhelper_redraw = true;
      print_voice_select_default_help();
    }

    if (seq.cycle_touch_element == 1 && generic_temp_select_menu > 5)
      generic_temp_select_menu = 5;

    if (menu_item_check(0))
    {
      setModeColor(0);
      display.setTextSize(1);
      display.setCursor(CHAR_width_small * 2, 6);
      display.print(F("POOL "));
    }

    if (menu_item_check(1))
    {
      setModeColor(1);
      display.setTextSize(1);
      display.setCursor(CHAR_width_small * 25, 6);
      display.print(F("DEXED"));
    }

    if (generic_temp_select_menu != 1)
    {
      draw_favorite_icon(configuration.dexed[selected_instance_id].pool, configuration.dexed[selected_instance_id].bank, configuration.dexed[selected_instance_id].voice, selected_instance_id);
    }
    display.setTextSize(2);
    display.setTextColor(GREY2, COLOR_BACKGROUND);
    display.setCursor(5 * CHAR_width_small, 2 * CHAR_height_small + 3);
    print_formatted_number(configuration.dexed[selected_instance_id].bank, 2);
    display.setCursor(5 * CHAR_width_small, 4 * CHAR_height_small + 5);
    print_formatted_number(configuration.dexed[selected_instance_id].voice + 1, 2);
    setModeColor(2);
    show_no_grid(2 * CHAR_height_small + 3, 11 * CHAR_width_small, 10, g_bank_name[selected_instance_id]);//2 extra chars for FAV searching text
    setModeColor(3);
    show_no_grid(4 * CHAR_height_small + 5, 11 * CHAR_width_small, 10, g_voice_name[selected_instance_id]);

    display.setTextColor(COLOR_SYSTEXT, COLOR_BACKGROUND);

    if (seq.cycle_touch_element != 1)
    {
      if ((dexed_live_mod.active_button == 0 && dexed_onscreen_algo != MicroDexed[selected_instance_id]->getAlgorithm()))
        UI_draw_FM_algorithm(MicroDexed[selected_instance_id]->getAlgorithm(), 147, 86);
      dexed_onscreen_algo = MicroDexed[selected_instance_id]->getAlgorithm();
    }

    if (menuhelper_redraw)
      print_voice_settings_in_dexed_voice_select(false, false);
    menuhelper_redraw = false;

    if (dexed_live_mod.active_button == 99) // if button press had confirmed live mod settings and is now unselected,
      dexed_live_mod.active_button = 0;     // skip button push from voice_select once, then back to normal
    // debug
    //     display.setCursor(CHAR_width, 11 * CHAR_height  );
    //     display.setTextColor(COLOR_PITCHSMP, COLOR_BACKGROUND);
    //     display.setTextSize(1);
    //     print_formatted_number(seq.edit_state, 2);
    //     display.setCursor(CHAR_width * 5, 11 * CHAR_height  );
    //     print_formatted_number(generic_temp_select_menu, 2);
    //     display.setCursor(1, 1);
    //     display.print(MicroDexed[selected_instance_id]->getAlgorithm());
    //     display.print(" ");
  }
  if (LCDML.FUNC_close()) // ****** STABLE END *********
  {
    display.fillScreen(COLOR_BACKGROUND);
    encoderDir[ENC_R].reset();

    dexed_live_mod.active_button = 0;
  }
}

// ==================
// DEXED voice editor
// ==================

// a single dexed voice parameter definition
struct voice_param
{
  const char* name;
  uint8_t max;
};

// list of all dexed voice operator parameter definitions.
// actual parameters are repeating this six times plus the global voice parameters.
const struct voice_param voice_op_params[] = {
    {"R1", 99},
    {"R2", 99},
    {"R3", 99},
    {"R4", 99},
    {"L1", 99},
    {"L2", 99},
    {"L3", 99},
    {"L4", 99},
    {"LEV SCL BRK PT", 99},
    {"SCL LEFT DEPTH", 99},
    {"SCL RGHT DEPTH", 99},
    {"SCL LEFT CURVE", 3},
    {"SCL RGHT CURVE", 3},
    {"OSC RATE SCALE", 7},
    {"AMP MOD SENS", 3},
    {"KEY VEL SENS", 7},
    {"OUTPUT LEV", 99},
    {"OSC MODE", 1},
    {"FREQ COARSE", 31},
    {"FREQ FINE", 99},
    {"OSC DETUNE", 14} };
const uint8_t num_voice_op_params = 21;

// list of all dexed global voice parameter definitions.
const struct voice_param voice_params[] = {
    {"R1", 99},
    {"R2", 99},
    {"R3", 99},
    {"R4", 99},
    {"L1", 99},
    {"L2", 99},
    {"L3", 99},
    {"L4", 99},
    {"ALGORITHM", 31},
    {"FEEDBACK", 7},
    {"OSC KEY SYNC", 1},
    {"LFO SPEED", 99},
    {"LFO DELAY", 99},
    {"LFO PITCH MOD DEP", 99},
    {"LFO AMP MOD DEP", 99},
    {"LFO SYNC", 1},
    {"LFO WAVE", 4},
    {"LFO PITCH MOD SENS", 7},
    {"TRANSPOSE", 48},
    {"NAME", 127} };
const uint8_t num_voice_params = 19; // omit name for now

uint8_t current_voice_op = 0; // currently selected operator for edits

// the dexed engine global voice parameter getter and setters.
// all parameter values are uint8_t values, starting with 0.
// They are defined in the dexed VoiceData array:
// -Operator parameters are stored six times repeated for all six operators.
//  They are load and stored to the engine depending on the current selected_instance_id.
// -Global voice parameters go after the operator values, starting with DEXED_VOICE_OFFSET.
//  They are load and stored to the engine depending on the current selected_instance_id and current_voice_op operator.
//
int16_t dexed_getter(Editor* param)
{
  int8_t addr = param->select_id - 1 < num_voice_params ? param->select_id - 1 + DEXED_VOICE_OFFSET : param->select_id - 2 - num_voice_params + current_voice_op * num_voice_op_params;
  return MicroDexed[selected_instance_id]->getVoiceDataElement(addr);
};
void dexed_setter(Editor* param, int16_t value)
{
  uint8_t addr = param->select_id - 1 < num_voice_params ? param->select_id - 1 + DEXED_VOICE_OFFSET : param->select_id - 2 - num_voice_params + current_voice_op * num_voice_op_params;
  MicroDexed[selected_instance_id]->setVoiceDataElement(addr, value);
};

// allow switching the currently edited operator.
int16_t dexed_op_getter(Editor* param)
{
  return current_voice_op;
};
void dexed_op_setter(Editor* param, int16_t value)
{
  current_voice_op = value;
  ui.draw_editors(true); // as half of the editors have changed when a different operator is selected.
};

// the dexed voice edior, showing an UI for all 144 parameters of the current voice
// as not all editors fit on screen, only the global parameters and one set of operator parameters is shown.
// The current operator can be selected to access all parameters.
FLASHMEM void UI_func_voice_editor(uint8_t param)
{

  if (LCDML.FUNC_setup()) // ****** SETUP *********
  {
    ui.reset();

    ui.setCursor(0, 1);

    // allow switching between mutliple instances (also by long button press)
    addInstanceEditor(&dexed_voice_name_renderer);

    // voice global parameters
    display.setTextSize(1);
    display.setTextColor(GREY2, COLOR_BACKGROUND);
    setCursor_textGrid_small(0, 4);

    display.print(F("PITCH EG"));
    // display PITCH EG editor in two columns to save space
    ui.setCursor(0, 5);
    for (uint8_t i = 0; i < 4; i++)
      ui.addEditor(voice_params[i].name, 0, voice_params[i].max, &dexed_getter, &dexed_setter);
    ui.setCursor(14, 5);
    for (uint8_t i = 4; i < 8; i++)
      ui.addEditor(voice_params[i].name, 0, voice_params[i].max, &dexed_getter, &dexed_setter);
    ui.setCursor(0, 9);
    // display the remaining global editors
    for (uint8_t i = 8; i < num_voice_params; i++)
      ui.addEditor(voice_params[i].name, 0, voice_params[i].max, &dexed_getter, &dexed_setter);

    // operator parameter set selector
    ui.setCursor(27, 3);
    ui.addEditor((const char*)F("EDIT OPERATOR"), 0, 5, dexed_op_getter, dexed_op_setter);
    ui.enableLeftEncoderEditor(); // also select operator by left encoder

    setCursor_textGrid_small(29, 4);
    display.print(F("OPERATOR EG"));
    // display OPERATOR EG editor in two columns to save space
    ui.setCursor(27, 5);
    for (uint8_t i = 0; i < 4; i++)
      ui.addEditor(voice_op_params[i].name, 0, voice_op_params[i].max, &dexed_getter, &dexed_setter);
    ui.setCursor(41, 5);
    for (uint8_t i = 4; i < 8; i++)
      ui.addEditor(voice_op_params[i].name, 0, voice_op_params[i].max, &dexed_getter, &dexed_setter);
    ui.setCursor(27, 9);
    // display the remaining operator editors
    for (uint8_t i = 8; i < num_voice_op_params; i++)
      ui.addEditor(voice_op_params[i].name, 0, voice_op_params[i].max, &dexed_getter, &dexed_setter);
  }
  if (LCDML.FUNC_loop()) // ****** LOOP *********
  {
    ui.handle_input();
  }
  if (LCDML.FUNC_close()) // ****** STABLE END *********
  {
    ui.clear();
    encoderDir[ENC_R].reset();
    dexed_live_mod.active_button = 0;
  }
}

// ======================
// Dexed controller setup
// ======================

// display modes a controller can interact
void dexed_mode_renderer(Editor* editor, bool refresh)
{
  prepare_multi_options(editor, refresh);
  display.print("          ");
  setCursor_textGrid_small(editor->x, editor->y);
  uint8_t mode = editor->get();
  if (mode == 0)
    display.print("LINEAR");
  if (mode == 1)
    display.print("REV.LINEAR");
  if (mode == 2)
    display.print("DIRECT");
}

// display the targets a controller can be assigned to (multiple choice bit field)
void dexed_assign_renderer(Editor* editor, bool refresh)
{
  prepare_multi_options(editor, refresh);
  display.print("          ");
  setCursor_textGrid_small(editor->x, editor->y);
  uint8_t mode = editor->get();
  if (mode & 1)
    display.print("PTH ");
  if (mode & 2)
    display.print("AMP ");
  if (mode & 4)
    display.print("EG");
}

// compare edited parameter location to a given one and send SysEx message if they match
void send_sysex_if_changed(uint8_t id, uint8_t* valuePtr, uint8_t* changedValuePtr)
{
  if (valuePtr == changedValuePtr)
    send_sysex_param(configuration.dexed[selected_instance_id].midi_channel, id, *((uint8_t*)valuePtr), 2);
}

// apply changed controller values (all at once)
// SysEx messages are only send for the actual chaged parameter.
//

void dexed_controller_setter(Editor* editor, int16_t value)
{

  // call base setter to store editor value into our dexed parameter storage.
  dexed_current_instance_setter(editor, value);

  // send all editor changes to dexed engine.
  MicroDexed[selected_instance_id]->setPBController(configuration.dexed[selected_instance_id].pb_range, configuration.dexed[selected_instance_id].pb_step);
  MicroDexed[selected_instance_id]->setMWController(configuration.dexed[selected_instance_id].mw_range, configuration.dexed[selected_instance_id].mw_assign, configuration.dexed[selected_instance_id].mw_mode);
  MicroDexed[selected_instance_id]->setFCController(configuration.dexed[selected_instance_id].fc_range, configuration.dexed[selected_instance_id].fc_assign, configuration.dexed[selected_instance_id].fc_mode);
  MicroDexed[selected_instance_id]->setBCController(configuration.dexed[selected_instance_id].bc_range, configuration.dexed[selected_instance_id].bc_assign, configuration.dexed[selected_instance_id].bc_mode);
  MicroDexed[selected_instance_id]->setATController(configuration.dexed[selected_instance_id].at_range, configuration.dexed[selected_instance_id].at_assign, configuration.dexed[selected_instance_id].at_mode);
  MicroDexed[selected_instance_id]->ControllersRefresh();

  // send SysEx only for the value actually named by editor value pointer
  // to make sure we don't spam around SysEx messages for unchanged values!
  send_sysex_if_changed(65, &configuration.dexed[selected_instance_id].pb_range, (uint8_t*)editor->value);
  send_sysex_if_changed(66, &configuration.dexed[selected_instance_id].pb_step, (uint8_t*)editor->value);
  send_sysex_if_changed(70, &configuration.dexed[selected_instance_id].mw_range, (uint8_t*)editor->value);
  send_sysex_if_changed(71, &configuration.dexed[selected_instance_id].mw_assign, (uint8_t*)editor->value);
  send_sysex_if_changed(72, &configuration.dexed[selected_instance_id].fc_range, (uint8_t*)editor->value);
  send_sysex_if_changed(73, &configuration.dexed[selected_instance_id].fc_assign, (uint8_t*)editor->value);
  send_sysex_if_changed(74, &configuration.dexed[selected_instance_id].bc_range, (uint8_t*)editor->value);
  send_sysex_if_changed(75, &configuration.dexed[selected_instance_id].bc_assign, (uint8_t*)editor->value);
  send_sysex_if_changed(76, &configuration.dexed[selected_instance_id].at_range, (uint8_t*)editor->value);
  send_sysex_if_changed(77, &configuration.dexed[selected_instance_id].at_assign, (uint8_t*)editor->value);
}

// UI page to configure and assign the plentyful controllers a dexed engine can get input from:
//   Pitch Bend wheel, Modulation wheel, Foot pedal controller, Breath Controller, After Touch Pressure.
// Each of them (except pitch bend) can be assigned to zero or more of the controller channels:
//   Pitch modulation, Amplitude modulation, EG bias (a static offset to the operator EG values)
// The range and mapping can be altered for every controller.
//
FLASHMEM void UI_func_dexed_controllers(uint8_t param)
{

  if (LCDML.FUNC_setup()) // ****** SETUP *********
  {
    ui.reset();
    ui.setCursor(1, 1);
    // allow switching which dexed instance to edit
    addInstanceEditor(&dexed_voice_name_renderer);

    ui.setCursor(1, 5);
    ui.printLn("PITCH BEND WHEEL");
    ui.addEditor("PB RANGE", PB_RANGE_MIN, PB_RANGE_MAX, &configuration.dexed[0].pb_range,
      &dexed_current_instance_getter, &dexed_controller_setter);
    ui.addEditor("PB STEP", PB_STEP_MIN, PB_STEP_MAX, &configuration.dexed[0].pb_step,
      &dexed_current_instance_getter, &dexed_controller_setter);

    ui.printLn("");
    ui.printLn("MODULATION WHEEL");
    ui.addEditor("MW RANGE", MW_RANGE_MIN, MW_RANGE_MAX, &configuration.dexed[0].mw_range, &dexed_current_instance_getter, &dexed_controller_setter);
    ui.addEditor("MW ASSIGN", MW_ASSIGN_MIN, MW_ASSIGN_MAX, &configuration.dexed[0].mw_assign, &dexed_current_instance_getter, &dexed_controller_setter, &dexed_assign_renderer);
    ui.addEditor("MW MODE", MW_MODE_MIN, MW_MODE_MAX, &configuration.dexed[0].mw_mode, &dexed_current_instance_getter, &dexed_controller_setter, &dexed_mode_renderer);

    ui.printLn("");
    ui.printLn("FOOT CONTROLLER");
    ui.addEditor("FC RANGE", FC_RANGE_MIN, FC_RANGE_MAX, &configuration.dexed[0].fc_range, &dexed_current_instance_getter, &dexed_controller_setter);
    ui.addEditor("FC ASSIGN", FC_ASSIGN_MIN, FC_ASSIGN_MAX, &configuration.dexed[0].fc_assign, &dexed_current_instance_getter, &dexed_controller_setter, &dexed_assign_renderer);
    ui.addEditor("FC MODE", FC_MODE_MIN, FC_MODE_MAX, &configuration.dexed[0].fc_mode, &dexed_current_instance_getter, &dexed_controller_setter, &dexed_mode_renderer);

    ui.setCursor(29, 5 + 4);
    ui.printLn("BREATH CONTROLLER");
    ui.addEditor("BC RANGE", BC_RANGE_MIN, BC_RANGE_MAX, &configuration.dexed[0].bc_range, &dexed_current_instance_getter, &dexed_controller_setter);
    ui.addEditor("BC ASSIGN", BC_ASSIGN_MIN, BC_ASSIGN_MAX, &configuration.dexed[0].bc_assign, &dexed_current_instance_getter, &dexed_controller_setter, &dexed_assign_renderer);
    ui.addEditor("BC MODE", BC_MODE_MIN, BC_MODE_MAX, &configuration.dexed[0].bc_mode, &dexed_current_instance_getter, &dexed_controller_setter, &dexed_mode_renderer);

    ui.printLn("");
    ui.printLn("AFTERTOUCH");
    ui.addEditor("AT RANGE", AT_RANGE_MIN, AT_RANGE_MAX, &configuration.dexed[0].at_range, &dexed_current_instance_getter, &dexed_controller_setter);
    ui.addEditor("AT ASSIGN", AT_ASSIGN_MIN, AT_ASSIGN_MAX, &configuration.dexed[0].at_assign, &dexed_current_instance_getter, &dexed_controller_setter, &dexed_assign_renderer);
    ui.addEditor("AT MODE", AT_MODE_MIN, AT_MODE_MAX, &configuration.dexed[0].at_mode, &dexed_current_instance_getter, &dexed_controller_setter, &dexed_mode_renderer);
  }
  if (LCDML.FUNC_loop()) // ****** LOOP *********
  {
    ui.handle_input();
  }
  if (LCDML.FUNC_close()) // ****** STABLE END *********
  {
    ui.clear();
  }
}



FLASHMEM void UI_func_volume(uint8_t param)
{
  static uint8_t old_volume;
  if (LCDML.FUNC_setup()) // ****** SETUP *********
  {
    old_volume = configuration.sys.vol;
    display.setTextSize(2);
    display.setTextColor(COLOR_SYSTEXT, COLOR_BACKGROUND);
    encoderDir[ENC_L].reset();
    back_from_volume = 0;
  }
  if (LCDML.FUNC_loop()) // ****** LOOP *********
  {
    if ((LCDML.BT_checkDown() && encoderDir[ENC_L].Down()) || (LCDML.BT_checkUp() && encoderDir[ENC_L].Up()))
    {
      back_from_volume = 0;
      if (LCDML.BT_checkDown())
      {
        if (ENCODER[ENC_L].speed() > 5)
          configuration.sys.vol = constrain(configuration.sys.vol + ENCODER[ENC_L].speed() / 2, VOLUME_MIN, VOLUME_MAX);
        else
          configuration.sys.vol = constrain(configuration.sys.vol + 1, VOLUME_MIN, VOLUME_MAX);
      }
      else if (LCDML.BT_checkUp())
      {
        if (ENCODER[ENC_L].speed() > 5)
          configuration.sys.vol = constrain(configuration.sys.vol - ENCODER[ENC_L].speed() / 2, VOLUME_MIN, VOLUME_MAX);
        else
          configuration.sys.vol = constrain(configuration.sys.vol - 1, VOLUME_MIN, VOLUME_MAX);
      }
    }

    // Master Volume

    if (multiband_active)
    {
      multiband_active = false;
      display.setTextSize(2);
      setCursor_textGrid(1, 4);
      display.setTextColor(RED, COLOR_BACKGROUND);
      display.print(F("MULTIBAND"));
      setCursor_textGrid(1, 5);
      display.print(F("DEACTIVATED"));
      display.setTextSize(2);
      mb_set_master();
      mb_set_mutes();
      mb_set_compressor();
      if (LCDML.FUNC_getID() > _LCDML_DISP_cnt || LCDML.FUNC_getID() == LCDML.OTHER_getIDFromFunction(UI_func_volume))
        draw_button_on_grid(35, 25, "MULTI", "BAND", 0);
    }
    display.setTextSize(2);
    display.setTextColor(COLOR_SYSTEXT, COLOR_BACKGROUND);
    setCursor_textGrid(1, 1);
    display.print(F("Master Volume"));
    display_bar_int("Master Vol.", configuration.sys.vol, 1.0, VOLUME_MIN, VOLUME_MAX, 3, false, false, false);
    set_volume(configuration.sys.vol, configuration.sys.mono);
  }
  if (LCDML.FUNC_close()) // ****** STABLE END *********
  {
    encoderDir[ENC_L].reset();
    if (old_volume != configuration.sys.vol)
    {
      save_sys_flag = true;
      save_sys = SAVE_SYS_MS / 2;
    }
  }
}

FLASHMEM void UI_func_save_voice(uint8_t param)
{
  static bool yesno;
  static uint8_t mode;

  if (LCDML.FUNC_setup()) // ****** SETUP *********
  {
    encoderDir[ENC_R].reset();
    display.setTextColor(COLOR_SYSTEXT, COLOR_BACKGROUND);
    yesno = false;
    mode = 0;

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
    helptext_r("< > SELECT INSTANCE");
    display.setTextSize(2);
    display.setTextColor(COLOR_SYSTEXT, COLOR_BACKGROUND);
  }
  if (LCDML.FUNC_loop()) // ****** LOOP *********
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
        get_bank_name(configuration.dexed[selected_instance_id].pool, configuration.dexed[selected_instance_id].bank, g_bank_name[selected_instance_id]);

        show(2, 1, 2, configuration.dexed[selected_instance_id].bank);
        show(2, 5, 10, g_bank_name[selected_instance_id]);
        break;
      case 2: // Voice selection
        if (LCDML.BT_checkDown() && configuration.dexed[selected_instance_id].voice < MAX_VOICES - 1)
          configuration.dexed[selected_instance_id].voice = constrain(configuration.dexed[selected_instance_id].voice + ENCODER[ENC_R].speed(), 0, MAX_VOICES - 1);
        else if (LCDML.BT_checkUp() && configuration.dexed[selected_instance_id].voice > 0)
          configuration.dexed[selected_instance_id].voice = constrain(configuration.dexed[selected_instance_id].voice - ENCODER[ENC_R].speed(), 0, MAX_VOICES - 1);

        // get voice name from sysex on SD
        get_voice_name(configuration.dexed[selected_instance_id].pool, configuration.dexed[selected_instance_id].bank, configuration.dexed[selected_instance_id].voice, g_voice_name[selected_instance_id]);
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
          bool ret = save_sd_voice(configuration.dexed[selected_instance_id].pool, configuration.dexed[selected_instance_id].bank, configuration.dexed[selected_instance_id].voice, selected_instance_id);
          if (ret == true)
            LOG.println(F("Saving voice OK."));
          else
            LOG.println(F("Error while saving voice."));
#else
          save_sd_voice(configuration.dexed[selected_instance_id].pool, configuration.dexed[selected_instance_id].bank, configuration.dexed[selected_instance_id].voice, selected_instance_id);
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
  if (LCDML.FUNC_close()) // ****** STABLE END *********
  {
    if (mode < 0xff)
    {
      show(2, 1, 16, "Canceled.");
      delay(MESSAGE_WAIT_TIME);
    }
    encoderDir[ENC_R].reset();
    display.setTextColor(COLOR_SYSTEXT, COLOR_BACKGROUND);
    display.fillScreen(COLOR_BACKGROUND);
  }
}

FLASHMEM void UI_func_sysex_receive_bank(uint8_t param)
{
  static bool yesno;
  static uint8_t mode;
  static uint8_t bank_number;
  static uint8_t ui_select_name_state;

  if (LCDML.FUNC_setup()) // ****** SETUP *********
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
  if (LCDML.FUNC_loop()) // ****** LOOP *********
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
            LOG.print(F("Bank name: ["));
            LOG.print(receive_bank_filename);
            LOG.println(F("]"));
#endif
            char tmp[FILENAME_LEN];
            strcpy(tmp, receive_bank_filename);
            snprintf_P(receive_bank_filename, sizeof(receive_bank_filename), PSTR("/%s/%d/%s.syx"), DEXED_CONFIG_PATH, bank_number, tmp);
#ifdef DEBUG
            LOG.print(F("Receiving into bank "));
            LOG.print(bank_number);
            LOG.print(F(" as filename "));
            LOG.print(receive_bank_filename);
            LOG.println(F("."));
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
        LOG.println(mode, DEC);
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

  if (LCDML.FUNC_close()) // ****** STABLE END *********
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

FLASHMEM void UI_func_set_performance_name(uint8_t param)
{
  static uint8_t mode;
  static uint8_t ui_select_name_state;
  if (LCDML.FUNC_setup()) // ****** SETUP *********
  {
    encoderDir[ENC_R].reset();
    mode = 0;
    setCursor_textGrid(1, 1);
    display.print(F("Perform. Name"));
  }
  if (LCDML.FUNC_loop()) // ****** LOOP *********
  {
    if ((LCDML.BT_checkDown() && encoderDir[ENC_R].Down()) || (LCDML.BT_checkUp() && encoderDir[ENC_R].Up()))
    {
      if (LCDML.BT_checkDown())
      {
        if (mode == 1)
          ui_select_name_state = UI_select_name(2, 2, seq.name_temp, BANK_NAME_LEN - 1, false);
      }
      else if (LCDML.BT_checkUp())
      {
        if (mode == 1)
          ui_select_name_state = UI_select_name(2, 2, seq.name_temp, BANK_NAME_LEN - 1, false);
      }
    }
    else if (LCDML.BT_checkEnter() && encoderDir[ENC_R].ButtonShort())
    {
      if (mode == 1)
      {
        ui_select_name_state = UI_select_name(2, 2, seq.name_temp, BANK_NAME_LEN - 1, false);
        if (ui_select_name_state == true)
        {
          strcpy(seq.name, seq.name_temp);
          mode = 0xff;
          setCursor_textGrid(1, 2);
          display.print(F("OK.              "));
          delay(MESSAGE_WAIT_TIME);
          LCDML.FUNC_goBackToMenu();
        }
      }
    }
    if (mode == 0)
    {
      mode = 1;
      strcpy(seq.name_temp, seq.name);
      setCursor_textGrid(1, 2);
      display.print(F("[          ]    "));
      ui_select_name_state = UI_select_name(2, 2, seq.name_temp, BANK_NAME_LEN - 1, true);
    }
  }
  if (LCDML.FUNC_close()) // ****** STABLE END *********
  {
    encoderDir[ENC_R].reset();
  }
}

FLASHMEM void UI_func_set_multisample_name(uint8_t param)
{
  static uint8_t mode;
  static uint8_t ui_select_name_state;
  if (LCDML.FUNC_setup()) // ****** SETUP *********
  {
    encoderDir[ENC_R].reset();
    display.setTextSize(2);
    mode = 0;
    setCursor_textGrid(1, 1);
    display.print(F("Multisample Name"));
  }
  if (LCDML.FUNC_loop()) // ****** LOOP *********
  {
    if ((LCDML.BT_checkDown() && encoderDir[ENC_R].Down()) || (LCDML.BT_checkUp() && encoderDir[ENC_R].Up()))
    {
      if (LCDML.BT_checkDown())
      {
        if (mode == 0)
          seq.active_multisample = constrain(seq.active_multisample + ENCODER[ENC_L].speed(), 0, NUM_MULTISAMPLES - 1);
        else if (mode == 2)
          ui_select_name_state = UI_select_name(2, 4, seq.name_temp, BANK_NAME_LEN - 1, false);
      }
      else if (LCDML.BT_checkUp())
      {
        if (mode == 0)
          seq.active_multisample = constrain(seq.active_multisample - ENCODER[ENC_L].speed(), 0, NUM_MULTISAMPLES - 1);
        else if (mode == 2)
          ui_select_name_state = UI_select_name(2, 4, seq.name_temp, BANK_NAME_LEN - 1, false);
      }
    }
    else if (LCDML.BT_checkEnter() && encoderDir[ENC_R].ButtonShort())
    {
      if (mode == 2)
      {
        ui_select_name_state = UI_select_name(2, 4, seq.name_temp, BANK_NAME_LEN - 1, false);
        if (ui_select_name_state == true)
        {
          strcpy(msp[seq.active_multisample].name, seq.name_temp);
          mode = 0xff;
          setCursor_textGrid(1, 2);
          display.print(F("OK.                 "));
          delay(MESSAGE_WAIT_TIME);
          //          if (LCDML.MENU_getLastActiveFunctionID() == LCDML.OTHER_getIDFromFunction(UI_func_set_multisample_name) )
          //            LCDML.OTHER_jumpToID(LCDML.MENU_getLastActiveFunctionID());
          //          else
          LCDML.FUNC_goBackToMenu();
        }
      }
      if (mode == 0)
        mode = 1;
    }
    if (mode == 0)
    {
      setCursor_textGrid(1, 2);
      display.setTextColor(COLOR_PITCHSMP, COLOR_BACKGROUND);
      print_formatted_number(seq.active_multisample, 2);
      setCursor_textGrid(3, 2);
      display.print(F("["));
      setCursor_textGrid(15, 2);
      display.print(F("]"));
      display.setTextColor(COLOR_SYSTEXT, COLOR_BACKGROUND);
      show(2, 4, 11, msp[seq.active_multisample].name);
    }
    if (mode == 1)
    {
      mode = 2;
      strcpy(seq.name_temp, msp[seq.active_multisample].name);
      setCursor_textGrid(3, 2);
      display.print(F("[          ]      "));
      ui_select_name_state = UI_select_name(2, 4, seq.name_temp, BANK_NAME_LEN - 1, true);
    }
  }
  if (LCDML.FUNC_close()) // ****** STABLE END *********
  {
    encoderDir[ENC_R].reset();
  }
}

FLASHMEM void UI_func_sysex_send_bank(uint8_t param)
{
  static uint8_t pool_number;
  static uint8_t bank_number;

  if (LCDML.FUNC_setup()) // ****** SETUP *********
  {
    encoderDir[ENC_R].reset();
    pool_number = configuration.dexed[selected_instance_id].pool;
    bank_number = configuration.dexed[selected_instance_id].bank;
    strcpy(tmp_bank_name, g_bank_name[selected_instance_id]);
    setCursor_textGrid(1, 1);
    display.print(F("MIDI Send Bank"));
    show(2, 3, 1, "[");
    show(2, 15, 1, "]");
    show(2, 1, 2, configuration.dexed[selected_instance_id].bank);
    show(2, 4, 10, tmp_bank_name);
  }

  if (LCDML.FUNC_loop()) // ****** LOOP *********
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

      get_bank_name(configuration.dexed[selected_instance_id].pool, bank_number, tmp_bank_name);
#ifdef DEBUG
      LOG.printf_P(PSTR("send bank sysex %d - bank:[%s]\n"), bank_number, tmp_bank_name);
#endif
      show(2, 1, 2, bank_number);
      show(2, 4, 10, tmp_bank_name);
    }
    else if (LCDML.BT_checkEnter() && encoderDir[ENC_R].ButtonShort())
    {
      if (strcmp("*ERROR*", tmp_bank_name) != 0)
      {
        char filename[FILENAME_LEN + 4];
        snprintf_P(filename, sizeof(filename), PSTR("/%s/%d/%d/%s.syx"), DEXED_CONFIG_PATH, pool_number, bank_number, tmp_bank_name);
#ifdef DEBUG
        LOG.print(F("Send bank "));
        LOG.print(filename);
        LOG.println(F(" from SD."));
#endif
        File sysex = SD.open(filename);
        if (!sysex)
        {
#ifdef DEBUG
          LOG.println(F("Cannot read from SD."));
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
  if (LCDML.FUNC_close()) // ****** STABLE END *********
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

FLASHMEM void UI_func_sysex_send_voice(uint8_t param)
{
  static uint8_t mode;
  static uint8_t pool_number;
  static uint8_t bank_number;
  static uint8_t voice_number;

  if (LCDML.FUNC_setup()) // ****** SETUP *********
  {
    display.setTextColor(COLOR_SYSTEXT, COLOR_BACKGROUND);
    encoderDir[ENC_R].reset();
    mode = 0;
    pool_number = configuration.dexed[selected_instance_id].pool;
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
    helptext_r("< > SELECT BANK");
    display.setTextSize(2);
    display.setTextColor(COLOR_SYSTEXT, COLOR_BACKGROUND);
  }
  if (LCDML.FUNC_loop()) // ****** LOOP *********
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

        get_bank_name(pool_number, bank_number, tmp_bank_name);
        show(2, 1, 2, bank_number);
        show(2, 5, 10, tmp_bank_name);
        get_voice_name(pool_number, bank_number, voice_number, tmp_voice_name);
        break;
      case 1: // Voice selection
        if (LCDML.BT_checkDown() && voice_number < MAX_VOICES - 1)
          voice_number = constrain(voice_number + ENCODER[ENC_R].speed(), 0, MAX_VOICES - 1);
        else if (LCDML.BT_checkUp() && voice_number > 0)
          voice_number = constrain(voice_number - ENCODER[ENC_R].speed(), 0, MAX_VOICES - 1);

        get_voice_name(pool_number, bank_number, voice_number, tmp_voice_name);
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
          char filename[FILENAME_LEN + 4];
          snprintf_P(filename, sizeof(filename), PSTR("/%s/%d/%d/%s.syx"), DEXED_CONFIG_PATH, pool_number, bank_number, tmp_bank_name);
#ifdef DEBUG
          LOG.print(F("Send voice "));
          LOG.print(voice_number);
          LOG.print(F(" of "));
          LOG.print(filename);
          LOG.println(F(" from SD."));
#endif
          File sysex = SD.open(filename);
          if (!sysex)
          {
#ifdef DEBUG
            LOG.println(F("Connot read from SD."));
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

  if (LCDML.FUNC_close()) // ****** STABLE END *********
  {
    if (mode < 0xff)
    {
      show(2, 1, 16, "Canceled.");
      delay(MESSAGE_WAIT_TIME);
    }
    encoderDir[ENC_R].reset();
    display.setTextColor(COLOR_SYSTEXT, COLOR_BACKGROUND);
    display.fillScreen(COLOR_BACKGROUND);
  }
}

FLASHMEM void UI_func_startup_performance(uint8_t param)
{
  bool stored = false;
  static uint8_t old_load_at_startup_performance;

  if (LCDML.FUNC_setup()) // ****** SETUP *********
  {
    helptext_r("< > SELECT PERFORMANCE");
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
      print_formatted_number(configuration.sys.load_at_startup_performance, 2);
      show(2, 16, 1, "]");
    }
  }

  if (LCDML.FUNC_loop()) // ****** LOOP *********
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
        print_formatted_number(configuration.sys.load_at_startup_performance, 2);
        show(2, 16, 1, "]");
        display.setTextColor(COLOR_PITCHSMP, COLOR_BACKGROUND);
        if (check_sd_performance_exists(configuration.sys.load_at_startup_performance))
        {
          get_sd_performance_name_json(configuration.sys.load_at_startup_performance);
          if (seq.name_temp[0] != 0)
            show(3, 1, 16, seq.name_temp);
          else
            show(3, 1, 16, "----  DATA  ----");
        }
        else
          show(3, 1, 16, "-- EMPTY SLOT --");
      }
      display.setTextColor(COLOR_SYSTEXT, COLOR_BACKGROUND);
    }
  }

  if (LCDML.FUNC_close()) // ****** STABLE END *********
  {
    if (stored == false)
    {
      show(2, 1, 16, "Canceled.");
      configuration.sys.load_at_startup_performance = old_load_at_startup_performance;
      delay(MESSAGE_WAIT_TIME);
    }
    encoderDir[ENC_R].reset();
    helptext_r("");
  }
}

FLASHMEM void UI_func_startup_page(uint8_t param)
{
  if (LCDML.FUNC_setup()) // ****** SETUP *********
  {
    encoderDir[ENC_R].reset();
    helptext_r("< > SELECT PAGE");
    display.setTextSize(2);
    display.setTextColor(COLOR_SYSTEXT, COLOR_BACKGROUND);
    display.setCursor(0, 0);
    show(1, 1, 16, "Startup Page");
  }
  if (LCDML.FUNC_loop()) // ****** LOOP *********
  {
    if ((LCDML.BT_checkDown() && encoderDir[ENC_R].Down()) || (LCDML.BT_checkUp() && encoderDir[ENC_R].Up()) || (LCDML.BT_checkEnter() && encoderDir[ENC_R].ButtonShort()))
    {
      if (LCDML.BT_checkDown())
      {
        configuration.sys.load_at_startup_page++;
        if (configuration.sys.load_at_startup_page > 9)
          configuration.sys.load_at_startup_page = 50; // System Info Page
      }
      else if (LCDML.BT_checkUp())
      {
        if (configuration.sys.load_at_startup_page > 0)
        {
          if (configuration.sys.load_at_startup_page == 50) // System Info Page
            configuration.sys.load_at_startup_page = 9;
          else
            configuration.sys.load_at_startup_page--;
        }
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
    else if (configuration.sys.load_at_startup_page == 6)
      show(2, 1, 17, "EPiano");
    else if (configuration.sys.load_at_startup_page == 7)
      show(2, 1, 17, "Braids");
    else if (configuration.sys.load_at_startup_page == 8)
      show(2, 1, 17, "Master Mixer");
    else if (configuration.sys.load_at_startup_page == 9)
      show(2, 1, 17, "Live Sequencer");
    else if (configuration.sys.load_at_startup_page == 50)
      show(2, 1, 17, "System Info");
  }
  if (LCDML.FUNC_close()) // ****** STABLE END *********
  {
    encoderDir[ENC_R].reset();
    helptext_r("");
    display.setTextSize(2);
  }
}

FLASHMEM bool UI_select_name(uint8_t y, uint8_t x, char* edit_string, uint8_t len, bool init)
{
  static int8_t edit_pos;
  static bool edit_mode;
  static uint8_t edit_value;
  static uint8_t last_char_pos;
  display.setTextColor(COLOR_SYSTEXT, COLOR_BACKGROUND);
  if (init == true)
  {
    edit_mode = false;
    edit_pos = 0;
    edit_value = search_accepted_char(edit_string[edit_pos]);
    last_char_pos = strlen(edit_string);
    string_trim(edit_string); // just to be sure
    // draw full string
    setCursor_textGrid(x, y);
    display.print(edit_string);
    // overwrite first char with inversed character
    setCursor_textGrid(x, y);
    display.setTextColor(COLOR_BACKGROUND, COLOR_PITCHSMP);
    display.print(edit_string[0]);
    display.setTextColor(GREY3, COLOR_BACKGROUND);
    setCursor_textGrid(x + len + 2, y);
    display.print(" OK ");
    display.setTextColor(COLOR_SYSTEXT, COLOR_BACKGROUND);
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
        display.setTextColor(COLOR_SYSTEXT, COLOR_BACKGROUND);
      }
      else
      {
        // if (edit_string[edit_pos] != 0 && edit_string[edit_pos] != 32)
        if (edit_string[edit_pos] != 0)
        {
          edit_pos = constrain(edit_pos + 1, 0, len);
          if (edit_pos < len)
          {
            setCursor_textGrid(x + edit_pos, y);
            display.setTextColor(COLOR_BACKGROUND, COLOR_PITCHSMP);
            display.print(edit_string[edit_pos]); // highlight current char
            display.setTextColor(COLOR_SYSTEXT, COLOR_BACKGROUND);
          }
          if (edit_pos > 0)
          {
            display.setTextColor(COLOR_SYSTEXT, COLOR_BACKGROUND);
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
        display.setTextColor(COLOR_SYSTEXT, COLOR_BACKGROUND);
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
        if (edit_pos <= len && edit_pos > 0)
        {
          setCursor_textGrid(x + edit_pos - 1, y);
          display.setTextColor(COLOR_BACKGROUND, COLOR_PITCHSMP);
          display.print(edit_string[edit_pos - 1]); // highlight current char
          if (edit_pos < len)
          {
            setCursor_textGrid(x + edit_pos, y);
            display.setTextColor(COLOR_SYSTEXT, COLOR_BACKGROUND);
            display.print(edit_string[edit_pos]); // unhighlight char to the right
          }
        }
        if (edit_pos - 1 > last_char_pos)
          edit_pos = last_char_pos;
        else
          edit_pos = constrain(edit_pos - 1, 0, len - 1);

        if (edit_pos == last_char_pos)
        {
          display.setTextColor(COLOR_SYSTEXT, COLOR_BACKGROUND);
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
    if (edit_mode == false && edit_pos < len && edit_string[edit_pos] != 0)
    {
      display.setTextColor(COLOR_SYSTEXT, COLOR_BACKGROUND);
      setCursor_textGrid(x + edit_pos, y);
      display.print(edit_string[edit_pos]); // unhighlight char
      edit_pos++;
      if (edit_pos < len)
      {
        display.setTextColor(COLOR_BACKGROUND, COLOR_PITCHSMP);
        setCursor_textGrid(x + edit_pos, y);
        display.print(edit_string[edit_pos]); // highlight next char
        display.setTextColor(COLOR_SYSTEXT, COLOR_BACKGROUND);
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
      display.setTextColor(COLOR_SYSTEXT, COLOR_BACKGROUND);
    }
    else
    {
      display.setTextColor(COLOR_SYSTEXT, COLOR_BACKGROUND);
      setCursor_textGrid(x + len + 1, y);
      display.print(" ");
    }
  }
  display.setTextColor(COLOR_SYSTEXT, COLOR_BACKGROUND);
  encoderDir[ENC_R].reset();
  return (false);
}

FLASHMEM uint8_t search_accepted_char(uint8_t c)
{
  // if (c == 0)
  //   c = 32;

  for (uint8_t i = 0; i < sizeof(accepted_chars) - 1; i++)
  {
#ifdef DEBUG
    LOG.print(i, DEC);
    LOG.print(F(":"));
    LOG.print(c);
    LOG.print(F("=="));
    LOG.println(accepted_chars[i], DEC);
#endif
    if (c == accepted_chars[i])
      return (i);
  }
  return (0);
  }

FLASHMEM void display_int(int16_t var, uint8_t size, bool zeros, bool brackets, bool sign)
{
  display_float(float(var), size, 0, zeros, brackets, sign);
}

FLASHMEM void display_float(float var, uint8_t size_number, uint8_t size_fraction, bool zeros, bool brackets, bool sign)
{
  char s[display_cols + 1];

  if (size_fraction > 0)
  {
    if (zeros == true && sign == true)
      snprintf_P(s, sizeof(s), PSTR("%+0*.*f"), size_number + size_fraction + 2, size_fraction, var);
    else if (zeros == true && sign == false)
      snprintf_P(s, sizeof(s), PSTR("%0*.*f"), size_number + size_fraction + 1, size_fraction, var);
    else if (zeros == false && sign == true)
      snprintf_P(s, sizeof(s), PSTR("%+*.*f"), size_number + size_fraction + 2, size_fraction, var);
    else if (zeros == false && sign == false)
      snprintf_P(s, sizeof(s), PSTR("%*.*f"), size_number + size_fraction + 1, size_fraction, var);
  }
  else
  {
    if (zeros == true && sign == true)
      snprintf_P(s, sizeof(s), PSTR("%+0*d"), size_number + 1, int(var));
    else if (zeros == true && sign == false)
      snprintf_P(s, sizeof(s), PSTR("%0*d"), size_number, int(var));
    else if (zeros == false && sign == true)
      snprintf_P(s, sizeof(s), PSTR("%+*d"), size_number + 1, int(var));
    else if (zeros == false && sign == false)
      snprintf_P(s, sizeof(s), PSTR("%*d"), size_number, int(var));
  }

  if (brackets == true)
  {
    char tmp[display_cols + 1];

    strcpy(tmp, s);
    snprintf_P(s, sizeof(s), PSTR("[%s]"), tmp);
  }

  display.setTextColor(COLOR_SYSTEXT, COLOR_BACKGROUND);
  display.print(s);
  display.setTextColor(COLOR_SYSTEXT);
}

FLASHMEM void display_bar_int(const char* title, uint32_t value, float factor, int32_t min_value, int32_t max_value, uint8_t size, bool zeros, bool sign, bool init)
{
  display_bar_float(title, float(value), factor, min_value, max_value, size, 0, zeros, sign, init);
}

FLASHMEM void display_bar_float(const char* title, float value, float factor, int32_t min_value, int32_t max_value, uint8_t size_number, uint8_t size_fraction, bool zeros, bool sign, bool init)
{
  uint8_t size;
  // float v;
  // float _vi = 0.0;
  // uint8_t vf;
  // uint8_t vi;

  if (size_fraction == 0)
    size = size_number;
  else
    size = size_number + size_fraction + 1;
  if (sign == true)
    size++;

  // v = float((value - min_value) * (display_cols - size)) / (max_value - min_value);
  // vf = uint8_t(modff(v, &_vi) * 10.0 + 0.5);
  // vi = uint8_t(_vi);

  if (sign == true)
    size += 1;

  // Title
  if (init == true)
    show(1, 1, display_cols - 2, title);

  // Value
  display.setCursor(CHAR_width * (display_cols - size - 3), CHAR_height * 2);
  display_float(value * factor, size_number, size_fraction, zeros, false, sign); // TBD

  // Bar
  // if (vi == 0)
  display.console = true;
  if (value == 0)
  {
    display.fillRect(CHAR_width, 2 * CHAR_height, 12 * CHAR_width, CHAR_height - 2, COLOR_BACKGROUND);
  }
  else
  {
    display.fillRect(CHAR_width, 2 * CHAR_height, value * 1.43, CHAR_height - 2, COLOR_SYSTEXT);
    display.fillRect(CHAR_width + value * 1.43, 2 * CHAR_height, (max_value - value) * 1.43 + 1, CHAR_height - 2, COLOR_BACKGROUND);
  }
}

FLASHMEM inline void display_meter_int(const char* title, uint32_t value, float factor, float offset, int32_t min_value, int32_t max_value, uint8_t size, bool zeros, bool sign, bool init)
{
  display_meter_float(title, float(value), factor, offset, min_value, max_value, size, 0, zeros, sign, init);
}

FLASHMEM void display_meter_float(const char* title, float value, float factor, float offset, int32_t min_value, int32_t max_value, uint8_t size_number, uint8_t size_fraction, bool zeros, bool sign, bool init)
{
  uint8_t size = 0;
  float v;
  // float _vi = 0.0;
  // uint8_t vf;
  // uint8_t vi;

  if (size_fraction == 0)
    size = size_number;
  else
    size = size_number + size_fraction + 1;
  if (sign == true)
    size++;

  // v = float((value - min_value) * (display_cols - size)) / (max_value - min_value);
  v = float((value - min_value) * (16 * 8 + 2 - size)) / (max_value - min_value);
  // vf = uint8_t(modff(v, &_vi) * 10.0 + 0.5);
  // vi = uint8_t(_vi);

  if (init == true)
  {
    // Title
    display.setCursor(CHAR_width, CHAR_height);
    display.print(title);
    display.console = true;
    display.drawRect(CHAR_width, 2 * CHAR_height + 2, CHAR_width * 11 + 1, CHAR_height - 5, GREY1);
  }

  // Value

  display.setCursor((display_cols - size - 3) * CHAR_width, CHAR_height * 2);

  display.setTextColor(COLOR_SYSTEXT, COLOR_BACKGROUND);
  display.print((value + offset) * factor);

  // Bar
  // if (vi == 0) {
  //   drawBitmap(CHAR_width, 2 * CHAR_height + 4, meter_bar[(uint8_t)(vf / 1.25 - 0.5)], 8, 8, COLOR_SYSTEXT, COLOR_BACKGROUND);
  //   for (uint8_t i = 1; i < display_cols - size; i++)
  //     display.fillRect(CHAR_width + i * 8, 2 * CHAR_height + 4, 8, 8, COLOR_BACKGROUND);  //empty block
  // } else if (vi == display_cols - size) {
  //   for (uint8_t i = 0; i < display_cols - size - 1; i++)
  //     display.fillRect(CHAR_width + i * 8, 2 * CHAR_height + 4, 8, 8, COLOR_BACKGROUND);  //empty block
  //   drawBitmap(CHAR_width + (display_cols - size - 1) * 8, 2 * CHAR_height + 4, meter_bar[7], 8, 8, COLOR_SYSTEXT, COLOR_BACKGROUND);
  // } else {
  //   for (uint8_t i = 0; i < display_cols - size + 2; i++)
  //     display.fillRect(CHAR_width + i * 8, 2 * CHAR_height + 4, 8, 8, COLOR_BACKGROUND);  //empty block
  //   drawBitmap(CHAR_width + vi * 8, 2 * CHAR_height + 4, meter_bar[(uint8_t)(vf / 1.25 - 0.5)], 8, 8, COLOR_SYSTEXT, COLOR_BACKGROUND);
  //  for (uint8_t i = vi + 1; i < display_cols - size + 2; i++)
  //    display.fillRect(CHAR_width + i * 8, 2 * CHAR_height + 4, 8, 8, COLOR_BACKGROUND);  //empty block
  // }

  // new gauge
  display.console = true;
  display.fillRect(CHAR_width + 1, 2 * CHAR_height + 4, v + 2, 8, COLOR_BACKGROUND);
  display.fillRect(CHAR_width + v + 5, 2 * CHAR_height + 4, CHAR_width * 10 + 7 - v, 8, COLOR_BACKGROUND);
  display.fillRect(CHAR_width + v + 1, 2 * CHAR_height + 4, 4, 8, COLOR_SYSTEXT);
}

FLASHMEM uint8_t bit_reverse8(uint8_t v)
{
  uint8_t result = 0;
  for (; v > 0; v >>= 1)
    (result <<= 1) |= (v & 1);
  return (result);
}

FLASHMEM void display_OP_active_instance_number(uint8_t instance_id, uint8_t op)
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

FLASHMEM void string_trim(char* s)
{
  int i;

  while (isspace(*s))
    s++; // skip left side white spaces
  for (i = strlen(s) - 1; (isspace(s[i])); i--)
    ; // skip right side white spaces
  s[i + 1] = '\0';
}

FLASHMEM void locate_previous_non_favorite()
{
  // find prev. non fav in current bank
  print_fav_search_text(LEFT);
  do
  {
    if (configuration.dexed[selected_instance_id].voice == 0)
    {
      configuration.dexed[selected_instance_id].voice = 32; //+1
      if (configuration.dexed[selected_instance_id].bank < 1)
        configuration.dexed[selected_instance_id].bank = MAX_BANKS - 1;

      do
      { // seek for previous bank
        configuration.dexed[selected_instance_id].bank--;
        if (configuration.dexed[selected_instance_id].bank < 1)
          configuration.dexed[selected_instance_id].bank = MAX_BANKS - 1;
        favsearcher++;
      } while (quick_check_favorites_in_bank(configuration.dexed[selected_instance_id].pool, configuration.dexed[selected_instance_id].bank, selected_instance_id) == true && favsearcher < 132);
    }
    configuration.dexed[selected_instance_id].voice--;
    favsearcher++;
  } while (check_favorite(configuration.dexed[selected_instance_id].pool, configuration.dexed[selected_instance_id].bank, configuration.dexed[selected_instance_id].voice,
    selected_instance_id) == true &&
    favsearcher < 170);
  favsearcher = 0;
}

FLASHMEM void locate_previous_favorite()
{
  // worst case, nothing found below voice 0 /  bank 0 - start loop at last bank
  if (configuration.dexed[selected_instance_id].voice < 2 && configuration.dexed[selected_instance_id].bank == 0 && favsearcher < 170)
  {
    configuration.dexed[selected_instance_id].bank = MAX_BANKS - 1;
    configuration.dexed[selected_instance_id].voice = 32;
  }
  else

    if (configuration.dexed[selected_instance_id].voice == 0 && configuration.dexed[selected_instance_id].bank < MAX_BANKS - 1)
    { // if at begin of any other bank
      configuration.dexed[selected_instance_id].bank--;
      configuration.dexed[selected_instance_id].voice = 32;
    }

  if (configuration.dexed[selected_instance_id].voice >= 0 && configuration.dexed[selected_instance_id].bank >= 0)
  {
    print_fav_search_text(LEFT);

    do
    { // first find previous fav in current bank

      if (configuration.dexed[selected_instance_id].voice == 0)
      {

        if (configuration.dexed[selected_instance_id].bank == 0)
        {
          configuration.dexed[selected_instance_id].bank = MAX_BANKS - 1;
          configuration.dexed[selected_instance_id].voice = 32;
        }
        else
          configuration.dexed[selected_instance_id].bank--;
        configuration.dexed[selected_instance_id].voice = 32;
      }
      else

        configuration.dexed[selected_instance_id].voice--;
      favsearcher++;

    } while (check_favorite(configuration.dexed[selected_instance_id].pool, configuration.dexed[selected_instance_id].bank, configuration.dexed[selected_instance_id].voice,
      selected_instance_id) == false &&
      configuration.dexed[selected_instance_id].voice >= 1 && favsearcher < 36);

    // if found, we are done. else quick check in previous banks

    if (check_favorite(configuration.dexed[selected_instance_id].pool, configuration.dexed[selected_instance_id].bank, configuration.dexed[selected_instance_id].voice,
      selected_instance_id) == false &&
      configuration.dexed[selected_instance_id].voice >= 0 && configuration.dexed[selected_instance_id].bank >= 0 && favsearcher < 170)
    {
      configuration.dexed[selected_instance_id].voice = 32;

      do
      { // seek for previous bank
        configuration.dexed[selected_instance_id].bank--;
        favsearcher++;
      } while (quick_check_favorites_in_bank(configuration.dexed[selected_instance_id].pool, configuration.dexed[selected_instance_id].bank, selected_instance_id) == false && favsearcher < 132 && configuration.dexed[selected_instance_id].bank >= 0);

      do
      { // last try to search if a bank with fav was found

        configuration.dexed[selected_instance_id].voice--;
        favsearcher++;
      } while (check_favorite(configuration.dexed[selected_instance_id].pool, configuration.dexed[selected_instance_id].bank, configuration.dexed[selected_instance_id].voice,
        selected_instance_id) == false &&
        configuration.dexed[selected_instance_id].voice >= 1 && favsearcher < 170);
    }
  }
  favsearcher = 0;
}

FLASHMEM void locate_next_favorite()
{
  bool RollOver = false;
  if (configuration.dexed[selected_instance_id].voice > 30 && configuration.dexed[selected_instance_id].bank >= MAX_BANKS - 1)
  { // if at end of all banks
    configuration.dexed[selected_instance_id].bank = 0;
    configuration.dexed[selected_instance_id].voice = 0;
    RollOver = true;
  }
  else if (configuration.dexed[selected_instance_id].voice > 30 && configuration.dexed[selected_instance_id].bank < MAX_BANKS - 1)
  { // if at end of any other bank
    configuration.dexed[selected_instance_id].bank++;
    configuration.dexed[selected_instance_id].voice = 0;
  }

  if (configuration.dexed[selected_instance_id].voice <= 30 && configuration.dexed[selected_instance_id].bank <= MAX_BANKS)
  {
    print_fav_search_text(RIGHT);

    do
    { // first find next fav in current bank

      if (RollOver == false)
        configuration.dexed[selected_instance_id].voice++;
      else
        RollOver = true;
      favsearcher++;

    } while (check_favorite(configuration.dexed[selected_instance_id].pool, configuration.dexed[selected_instance_id].bank, configuration.dexed[selected_instance_id].voice,
      selected_instance_id) == false &&
      configuration.dexed[selected_instance_id].voice <= 32 && favsearcher < 36);

    // if found, we are done. else quick check in next banks

    if (check_favorite(configuration.dexed[selected_instance_id].pool, configuration.dexed[selected_instance_id].bank, configuration.dexed[selected_instance_id].voice,
      selected_instance_id) == false &&
      configuration.dexed[selected_instance_id].bank < MAX_BANKS && favsearcher < 170)
    {
      configuration.dexed[selected_instance_id].voice = 0;

      do
      { // seek in next bank

        configuration.dexed[selected_instance_id].bank++;
        if (configuration.dexed[selected_instance_id].bank > MAX_BANKS - 1 && favsearcher < 190)
        {
          configuration.dexed[selected_instance_id].bank = 0;
          configuration.dexed[selected_instance_id].voice = 0;
        }
        favsearcher++;
      } while (quick_check_favorites_in_bank(configuration.dexed[selected_instance_id].pool, configuration.dexed[selected_instance_id].bank, selected_instance_id) == false && favsearcher < 132);

      if (check_favorite(configuration.dexed[selected_instance_id].pool, configuration.dexed[selected_instance_id].bank, configuration.dexed[selected_instance_id].voice,
        selected_instance_id) == false &&
        configuration.dexed[selected_instance_id].voice <= 32 && favsearcher < 190)
      {
        do
        { // last bank to search if a fav can be found

          configuration.dexed[selected_instance_id].voice++;
          favsearcher++;
        } while (check_favorite(configuration.dexed[selected_instance_id].pool, configuration.dexed[selected_instance_id].bank, configuration.dexed[selected_instance_id].voice,
          selected_instance_id) == false &&
          favsearcher < 170);
      }
    }
  }
  favsearcher = 0;
}

FLASHMEM void locate_next_non_favorite()
{
  // find next non-fav in current bank
  print_fav_search_text(RIGHT);
  do
  {
    configuration.dexed[selected_instance_id].voice++;
    if (configuration.dexed[selected_instance_id].voice > 31)
    {
      configuration.dexed[selected_instance_id].voice = 0;
      // configuration.dexed[selected_instance_id].bank++;
      if (configuration.dexed[selected_instance_id].bank > MAX_BANKS - 1)
        configuration.dexed[selected_instance_id].bank = 0;
      do
      { // seek for next bank
        configuration.dexed[selected_instance_id].bank++;
        if (configuration.dexed[selected_instance_id].bank > MAX_BANKS - 1)
          configuration.dexed[selected_instance_id].bank = 0;
        favsearcher++;
      } while (quick_check_favorites_in_bank(configuration.dexed[selected_instance_id].pool, configuration.dexed[selected_instance_id].bank, selected_instance_id) == true && favsearcher < 132);
    }
    favsearcher++;
  } while (check_favorite(configuration.dexed[selected_instance_id].pool, configuration.dexed[selected_instance_id].bank, configuration.dexed[selected_instance_id].voice,
    selected_instance_id) == true &&
    favsearcher < 170);
  favsearcher = 0;
}

FLASHMEM void locate_random_non_favorite()
{
  // find random non-fav
  do
  {
    configuration.dexed[selected_instance_id].voice = random(32);
    configuration.dexed[selected_instance_id].bank = random(MAX_BANKS - 1);
    favsearcher++;
  } while (check_favorite(configuration.dexed[selected_instance_id].pool, configuration.dexed[selected_instance_id].bank, configuration.dexed[selected_instance_id].voice,
    selected_instance_id) == true &&
    favsearcher < 100);
  favsearcher = 0;
}

FLASHMEM bool check_favorite(uint8_t p, uint8_t b, uint8_t v, uint8_t instance_id)
{
  p = constrain(p, 0, DEXED_POOLS - 1);
  b = constrain(b, 0, MAX_BANKS - 1);
  v = constrain(v, 0, MAX_VOICES - 1);
  char tmp[CONFIG_FILENAME_LEN];
  File myFav;
  if (sd_card > 0)
  {
    snprintf_P(tmp, sizeof(tmp), PSTR("/%s/%d/%s/%d/%d.fav"), DEXED_CONFIG_PATH, p, FAV_CONFIG_PATH, b, v);
#ifdef DEBUG
    LOG.print(F("check if Voice is a Favorite: "));
    LOG.print(tmp);
    LOG.println();
#endif
    if (SD.exists(tmp))
    { // is Favorite
#ifdef DEBUG
      LOG.println(F(" - It is in Favorites."));
#endif
      return true;
  }
    else
    { // it was not a favorite

#ifdef DEBUG
      LOG.println(F(" - It is not in Favorites."));
#endif
      return false;
}
}
  else
    return false;
}

FLASHMEM float eraseBytesPerSecond(const unsigned char* id)
{
  if (id[0] == 0x20)
    return 152000.0; // Micron
  if (id[0] == 0x01)
    return 500000.0; // Spansion
  if (id[0] == 0xEF)
    return 419430.0; // Winbond
  if (id[0] == 0xC2)
    return 279620.0; // Macronix
  return 320000.0;   // guess?
}

FLASHMEM void UI_func_format_flash(uint8_t param)
{

  if (LCDML.FUNC_setup()) // ****** SETUP *********
  {
    generic_menu = 0;
    display.fillScreen(COLOR_BACKGROUND);
    encoderDir[ENC_R].reset();
    helptext_r("FORMAT");
    helptext_l(back_text);
    display.setTextColor(COLOR_SYSTEXT, COLOR_BACKGROUND);
    display.setTextSize(2);
    setCursor_textGrid(1, 1);
    display.print(F("FORMAT FLASH?  "));
    setCursor_textGrid(1, 2);
    display.print(F("PUSH TO CONFIRM"));
#if (defined COMPILE_FOR_FLASH)
    print_flash_stats();
#endif
    display.setTextSize(2);
  }
#if (defined COMPILE_FOR_FLASH)
  if (LCDML.FUNC_loop()) // ****** LOOP *********
  {
    if (LCDML.BT_checkEnter())
    {
      generic_menu++;
    }

    if (generic_menu == 1) //Format FLASH
    {

      helptext_r("FORMATTING");
      display.setTextSize(2);
      uint8_t screenline = 0;
      setCursor_textGrid(1, 1);
      display.setTextColor(COLOR_BACKGROUND, RED);
      display.print(F("FORMATTING...  "));
      setCursor_textGrid(1, 2);
      print_empty_spaces(16);
      unsigned char id[5];
      SerialFlash.readID(id);
      unsigned long size = SerialFlash.capacity(id);
      if (size > 0)
      {
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
          display.print(F("Done!           "));
          helptext_l(back_text);
          display.setTextColor(COLOR_SYSTEXT, COLOR_BACKGROUND);
        }
      }
      generic_menu++;
    }

    if (generic_menu == 2)
    {
      display.console = true;
      display.fillRect(CHAR_width_small * 1, CHAR_height_small * 6, DISPLAY_WIDTH - CHAR_width_small * 2, CHAR_height_small * 16, COLOR_BACKGROUND);

      display.setTextColor(COLOR_SYSTEXT, COLOR_BACKGROUND);
      display.setTextSize(2);
      setCursor_textGrid(1, 1);
      display.print(F("COPY PRESETS?  "));
      setCursor_textGrid(1, 2);
      display.print(F("PUSH TO CONFIRM"));
      helptext_r("COPY PRESETS TO FLASH");
      setCursor_textGrid_small(1, 10);
      display.setTextColor(COLOR_SYSTEXT, COLOR_BACKGROUND);
      setCursor_textGrid_small(2, 11);
      display.print(F("IF YOU HAVE COPIED THE PRESET CONTENT"));
      setCursor_textGrid_small(2, 12);
      display.print(F("TO THE SDCARD, YOU CAN NOW COPY THE"));
      setCursor_textGrid_small(2, 13);
      display.print(F("SAMPLES FROM /DRUMS TO THE FLASH CHIP"));
      setCursor_textGrid_small(2, 15);
      display.setTextColor(RED, COLOR_BACKGROUND);
      display.print(F("PUSH ENCODER RIGHT TO CONFIRM"));
      display.setTextColor(COLOR_SYSTEXT, COLOR_BACKGROUND);
    }

    if (generic_menu == 3) //COPY presets
    {
      display.console = true;
      display.fillRect(CHAR_width_small * 1, CHAR_height_small * 6, DISPLAY_WIDTH - CHAR_width_small * 2, CHAR_height_small * 16, COLOR_BACKGROUND);

      display.setTextColor(COLOR_SYSTEXT, COLOR_BACKGROUND);
      display.setTextSize(2);
      setCursor_textGrid(1, 1);
      display.print(F("COPY IN PROGRESS"));
      setCursor_textGrid(1, 2);
      display.print(F("PLEASE WAIT    "));
      helptext_r("COPYING");
      encoderDir[ENC_R].reset();
      uint8_t screenline = 0;

      File rootdir = SD.open("/DRUMS");
      while (1)
      {
        // open a file from the SD card
        File f = rootdir.openNextFile();
        if (!f)
          break;
        const char* filename = f.name();

        // filter out the ._ mac files
        if (strstr(filename, "._") != NULL)
          continue;

        if (screenline > 10)
          screenline = 0;
        setCursor_textGrid_small(2, 6 + screenline);

        display.setTextColor(COLOR_SYSTEXT, COLOR_BACKGROUND);
        // if (filename[0] != 46 && filename[1] != 95)
        if (filename[0] != 46)
        {
          display.print(filename);
          fill_up_with_spaces_left_window_filemanager();
          screenline++;
        }
        unsigned long length = f.size();
        // check if this file is already on the Flash chip
        if (SerialFlash.exists(filename))
        {
          SerialFlashFile ff = SerialFlash.open(filename);
          if (ff && ff.size() == f.size())
          {
            if (compareFiles(f, ff) == true)
            {

              f.close();
              ff.close();
              continue; // advance to next file
            }
          }
          // delete the copy on the Flash chip, if different

          SerialFlash.remove(filename);
        }
        // if (filename[0] != 46 && filename[1] != 95)
        if (filename[0] != 46)
        {
          // create the file on the Flash chip and copy data
          if (SerialFlash.create(filename, length))
          {
            SerialFlashFile ff = SerialFlash.open(filename);
            if (ff)
            {
              // copy data loop
              unsigned long count = 0;
              display.console = true;
              while (count < length)
              {
                char buf[256];
                unsigned int n;
                n = f.read(buf, 256);
                ff.write(buf, n);
                count = count + n;
                if (count % 5120 == 0)
                  display.fillRect(CHAR_width_small * 38, CHAR_height_small * 7, count / (f.size() / (14 * CHAR_width_small)), 8, RED);
              }
              ff.close();
              display.fillRect(CHAR_width_small * 38 - 2, CHAR_height_small * 7, (14 * CHAR_width_small) + 4, 8, COLOR_BACKGROUND);
              flash_loadDirectory();
              print_flash_stats();
              flash_printDirectory();
            }
          }
        }
        f.close();
      }
      rootdir.close();
      // flash_loadDirectory();
      print_flash_stats();
      display.console = true;
      display.fillRect(CHAR_width_small * 1, CHAR_height_small * 6, DISPLAY_WIDTH - CHAR_width_small * 2, CHAR_height_small * 16, COLOR_BACKGROUND);
      display.setTextSize(2);
      helptext_r("FINISHED");
      helptext_l(back_text);
      display.setTextColor(COLOR_SYSTEXT, COLOR_BACKGROUND);
      display.setTextSize(2);
      setCursor_textGrid(1, 1);
      display.print(F("FINISHED!       "));
      setCursor_textGrid(1, 2);
      display.print(F("                "));

    }
  }
#endif
  if (LCDML.FUNC_close()) // ****** STABLE END *********
  {
    encoderDir[ENC_R].reset();
    display.setTextColor(COLOR_SYSTEXT, COLOR_BACKGROUND);
    display.fillScreen(COLOR_BACKGROUND);
  }
}

FLASHMEM void UI_func_test_mute(uint8_t param)
{
  if (LCDML.FUNC_setup()) // ****** SETUP *********
  {
    display.fillScreen(COLOR_BACKGROUND);
    encoderDir[ENC_R].reset();
    helptext_r("CONFIRM");
    helptext_l(back_text);
    display.setTextColor(COLOR_SYSTEXT, COLOR_BACKGROUND);
    display.setTextSize(2);
    setCursor_textGrid(1, 1);
    display.print(F("TEST MENU"));
  }
  if (LCDML.FUNC_loop()) // ****** LOOP *********
  {
    if (LCDML.BT_checkEnter())
    {
      if (seq.DAC_mute_state)
        dac_unmute();
      else
        dac_mute();
    }
    setCursor_textGrid(1, 2);
    if (seq.DAC_mute_state)
      display.print(F("UNMUTE DAC"));
    else
      display.print(F("MUTE DAC  "));
  }
  if (LCDML.FUNC_close()) // ****** STABLE END *********
  {
    encoderDir[ENC_R].reset();
    display.setTextColor(COLOR_SYSTEXT, COLOR_BACKGROUND);
    display.fillScreen(COLOR_BACKGROUND);
  }
}

FLASHMEM void UI_func_test_psram(uint8_t param)
{
  if (LCDML.FUNC_setup()) // ****** SETUP *********
  {
    display.fillScreen(COLOR_BACKGROUND);
    encoderDir[ENC_R].reset();
#ifdef PSRAM
    ;
#else
    helptext_r("START");
#endif
    helptext_l(back_text);
    display.setTextColor(COLOR_SYSTEXT, COLOR_BACKGROUND);
    display.setTextSize(2);
    setCursor_textGrid(1, 1);
    display.print(F("TEST PSRAM CHIP"));
    setCursor_textGrid_small(2, 4);
    display.setTextSize(1);
#ifdef PSRAM
    display.setTextColor(RED, COLOR_BACKGROUND);
    display.print(F("PSRAM TEST NOT POSSIBLE WHILE IT IS IN USE"));
    display.setTextColor(COLOR_SYSTEXT, COLOR_BACKGROUND);
    setCursor_textGrid_small(2, 6);
    display.print(F("FLASH A GENERIC MDT VERSION OR WHEN COMPILING"));
    setCursor_textGrid_small(2, 7);
    display.print(F("FROM SOURCE, DO NOT INCLUDE PSRAM OPTION"));
    display.setTextColor(GREY1, COLOR_BACKGROUND);
    setCursor_textGrid_small(2, 10);
    display.print(F("FAQ AND OTHER HELP IS AVAILABLE AT:"));
    setCursor_textGrid_small(2, 12);
    display.setTextColor(GREY1, COLOR_BACKGROUND);
    display.print(F("https://"));
    setCursor_textGrid_small(2, 13);
    display.setTextColor(COLOR_SYSTEXT, COLOR_BACKGROUND);
    display.print(F("codeberg.org/positionhigh/MicroDexed-touch/wiki"));
    display.setTextColor(COLOR_SYSTEXT, COLOR_BACKGROUND);
#else
    display.setTextColor(GREY1, COLOR_BACKGROUND);
    display.print(F("TEST SHOULD RUN AROUND 52 SECONDS FOR 8 MB CHIP"));
    display.setTextColor(COLOR_SYSTEXT, COLOR_BACKGROUND);
    setCursor_textGrid_small(2, 5);
    display.setTextColor(RED, COLOR_BACKGROUND);
    display.print(F("PUSH ENC_R TO START"));
    display.setTextColor(COLOR_SYSTEXT, COLOR_BACKGROUND);
#endif
  }
  if (LCDML.FUNC_loop()) // ****** LOOP *********
  {
    if (LCDML.BT_checkEnter())
    {
#ifdef PSRAM
      ;
#else
      display.setTextColor(COLOR_SYSTEXT, COLOR_BACKGROUND);
      psram_test();
#endif
    }
    // setCursor_textGrid(1, 2);
    }
  if (LCDML.FUNC_close()) // ****** STABLE END *********
  {
    encoderDir[ENC_R].reset();
    display.setTextColor(COLOR_SYSTEXT, COLOR_BACKGROUND);
    display.fillScreen(COLOR_BACKGROUND);
  }
  }

void sub_touchscreen_test_page_init()
{
  display.fillScreen(COLOR_BACKGROUND);
  encoderDir[ENC_R].reset();
  helptext_r("CLEAR");
  helptext_l(back_text);
  display.setTextColor(COLOR_SYSTEXT, COLOR_BACKGROUND);
  display.setTextSize(2);
  setCursor_textGrid(1, 1);
  display.print(F("TEST TOUCHSCREEN"));
  setCursor_textGrid_small(2, 4);
  display.setTextSize(1);
  display.setTextColor(GREY1, COLOR_BACKGROUND);
  display.print(F("TEST TOUCH INPUT ON THE SCREEN"));
  display.setTextColor(COLOR_SYSTEXT, COLOR_BACKGROUND);
  setCursor_textGrid_small(2, 5);
  display.setTextColor(RED, COLOR_BACKGROUND);
  display.print(F("PUSH ENC_R TO CLEAR SCREEN"));
  display.setTextColor(COLOR_SYSTEXT, COLOR_BACKGROUND);
  draw_button_on_grid(42, 1, "CLEAR", "SCRN", 0);
};

FLASHMEM void UI_func_test_touchscreen(uint8_t param)
{
  if (LCDML.FUNC_setup()) // ****** SETUP *********
  {
    sub_touchscreen_test_page_init();
  }
  if (LCDML.FUNC_loop()) // ****** LOOP *********
  {
    if (LCDML.BT_checkEnter())
    {
      sub_touchscreen_test_page_init();
    }
  }
  if (LCDML.FUNC_close()) // ****** STABLE END *********
  {
    encoderDir[ENC_R].reset();
    display.setTextColor(COLOR_SYSTEXT, COLOR_BACKGROUND);
    display.fillScreen(COLOR_BACKGROUND);
  }
}

FLASHMEM void UI_func_clear_song(uint8_t param)
{
  if (LCDML.FUNC_setup()) // ****** SETUP *********
  {
    display.fillScreen(COLOR_BACKGROUND);
    encoderDir[ENC_R].reset();
    helptext_r("CLEAR SONG");
    helptext_l(back_text);
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
    setCursor_textGrid(1, 7);
    display.print(F("CHAINS, CHAIN TRANSPOSES AND PATTERNS"));
    setCursor_textGrid(1, 8);
    display.print(F("WILL NOT BE TOUCHED."));
    display.setTextSize(2);
  }
  if (LCDML.FUNC_loop()) // ****** LOOP *********
  {
    if (LCDML.BT_checkEnter())
    {
      seq_clear_song_data();
      setCursor_textGrid(1, 1);
      display.print(F("Done."));
      print_empty_spaces(10);
      setCursor_textGrid(1, 2);
      print_empty_spaces(23);
      helptext_l("");
      border3_large_clear();
      delay(MESSAGE_WAIT_TIME);
      LCDML.FUNC_goBackToMenu();
    }
  }
  if (LCDML.FUNC_close()) // ****** STABLE END *********
  {
    encoderDir[ENC_R].reset();
    display.setTextColor(COLOR_SYSTEXT, COLOR_BACKGROUND);
    display.fillScreen(COLOR_BACKGROUND);
  }
}

FLASHMEM void UI_func_clear_song_chains(uint8_t param)
{
  if (LCDML.FUNC_setup()) // ****** SETUP *********
  {
    display.fillScreen(COLOR_BACKGROUND);
    encoderDir[ENC_R].reset();
    helptext_r("CLEAR ALL CHAINS");
    helptext_l(back_text);
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
  if (LCDML.FUNC_loop()) // ****** LOOP *********
  {
    if (LCDML.BT_checkEnter())
    {
      seq_clear_chain_data();
      setCursor_textGrid(1, 1);
      display.print(F("Done."));
      print_empty_spaces(10);
      setCursor_textGrid(1, 2);
      print_empty_spaces(23);
      helptext_l("");
      border3_large_clear();
      delay(MESSAGE_WAIT_TIME);
      LCDML.FUNC_goBackToMenu();
    }
  }
  if (LCDML.FUNC_close()) // ****** STABLE END *********
  {
    encoderDir[ENC_R].reset();
    display.setTextColor(COLOR_SYSTEXT, COLOR_BACKGROUND);
    display.fillScreen(COLOR_BACKGROUND);
  }
}

FLASHMEM void UI_func_clear_patterns(uint8_t param)
{
  if (LCDML.FUNC_setup()) // ****** SETUP *********
  {
    display.fillScreen(COLOR_BACKGROUND);
    encoderDir[ENC_R].reset();
    helptext_r("CLEAR ALL PATTERNS");
    helptext_l(back_text);
    display.setTextColor(COLOR_SYSTEXT, COLOR_BACKGROUND);
    display.setTextSize(2);
    setCursor_textGrid(1, 1);
    display.print(F("CLEAR PATTERNS? "));
    setCursor_textGrid(1, 2);
    display.setTextColor(RED, COLOR_BACKGROUND);
    display.print(F("PUSH "));
    display.setTextColor(COLOR_SYSTEXT, DX_DARKCYAN);
    display.print(F("[ENC R]"));
    display.setTextColor(RED, COLOR_BACKGROUND);
    display.print(F(" TO CONFIRM"));
    display.setTextSize(1);
    setCursor_textGrid(1, 6);
    display.print(F("ALL PATTERNS WILL BE DELETED !"));
    display.setTextColor(COLOR_SYSTEXT, COLOR_BACKGROUND);
    setCursor_textGrid(1, 8);
    display.print(F("SONG, CHAINS AND CHAIN TRANSPOSES"));
    setCursor_textGrid(1, 9);
    display.print(F("WILL NOT BE TOUCHED."));
    display.setTextSize(2);
  }
  if (LCDML.FUNC_loop()) // ****** LOOP *********
  {
    if (LCDML.BT_checkEnter())
    {
      seq_clear_all_patterns();
      setCursor_textGrid(1, 1);
      display.print(F("Done."));
      print_empty_spaces(10);
      setCursor_textGrid(1, 2);
      print_empty_spaces(23);
      helptext_l("");
      border3_large_clear();
      delay(MESSAGE_WAIT_TIME);
      LCDML.FUNC_goBackToMenu();
    }
  }
  if (LCDML.FUNC_close()) // ****** STABLE END *********
  {
    encoderDir[ENC_R].reset();
    display.setTextColor(COLOR_SYSTEXT, COLOR_BACKGROUND);
    display.fillScreen(COLOR_BACKGROUND);
  }
}

FLASHMEM void UI_func_clear_all(uint8_t param)
{
  if (LCDML.FUNC_setup()) // ****** SETUP *********
  {
    display.fillScreen(COLOR_BACKGROUND);
    encoderDir[ENC_R].reset();
    helptext_r("CLEAR ALL");
    helptext_l(back_text);
    display.setTextColor(COLOR_SYSTEXT, COLOR_BACKGROUND);
    display.setTextSize(2);
    setCursor_textGrid(1, 1);
    display.print(F("CLEAR EVERYTHING?"));
    setCursor_textGrid(1, 2);
    display.setTextColor(RED, COLOR_BACKGROUND);
    display.print(F("PUSH "));
    display.setTextColor(COLOR_SYSTEXT, DX_DARKCYAN);
    display.print(F("[ENC R]"));
    display.setTextColor(RED, COLOR_BACKGROUND);
    display.print(F(" TO CONFIRM"));
    display.setTextSize(1);
    setCursor_textGrid(1, 6);
    display.print(F("ALL SONG, CHAIN, CHAIN TRANSPOSES, "));
    setCursor_textGrid(1, 7);
    display.print(F("AND PATTERN DATA WILL BE CLEARED!"));
    display.setTextSize(2);
    display.setTextColor(COLOR_SYSTEXT, COLOR_BACKGROUND);
  }
  if (LCDML.FUNC_loop()) // ****** LOOP *********
  {
    if (LCDML.BT_checkEnter())
    {
      seq_clear_song_data();
      seq_clear_chain_data();
      seq_clear_all_patterns();
      setCursor_textGrid(1, 1);
      display.print(F("Done.  "));
      print_empty_spaces(10);
      setCursor_textGrid(1, 2);
      print_empty_spaces(23);
      helptext_l("");
      border3_large_clear();
      delay(MESSAGE_WAIT_TIME);
      LCDML.FUNC_goBackToMenu();
    }
  }
  if (LCDML.FUNC_close()) // ****** STABLE END *********
  {
    encoderDir[ENC_R].reset();
    display.setTextColor(COLOR_SYSTEXT, COLOR_BACKGROUND);
    display.fillScreen(COLOR_BACKGROUND);
  }
}

FLASHMEM void draw_favorite_icon(uint8_t p, uint8_t b, uint8_t v, uint8_t instance_id)
{
  p = constrain(p, 0, DEXED_POOLS - 1);
  b = constrain(b, 0, MAX_BANKS - 1);
  v = constrain(v, 0, MAX_VOICES - 1);
  char tmp[CONFIG_FILENAME_LEN];
  display.console = true;
  File myFav;
  if (sd_card > 0)
  {
    display.setTextSize(1);
    snprintf_P(tmp, sizeof(tmp), PSTR("/%s/%d/%s/%d/%d.fav"), DEXED_CONFIG_PATH, p, FAV_CONFIG_PATH, b, v);
    if (SD.exists(tmp))
    { // is Favorite
      display.fillRect(4 * CHAR_width + 18, 5, 9, 9, GREEN);
      display.setCursor(4 * CHAR_width + 20, 6);
      display.setTextColor(COLOR_BACKGROUND, GREEN);
      display.print(F("F"));
      if (ts.fav_buttton_state != 2)
      {
        draw_button_on_grid(37, 1, "REMOVE", "FAV.", 2);
        ts.fav_buttton_state = 2;
      }
    }
    else
    {
      // it is not a favorite
      display.fillRect(4 * CHAR_width + 18, 5, 9, 9, GREY2);
      display.setCursor(4 * CHAR_width + 20, 6);
      display.setTextColor(COLOR_BACKGROUND, GREY2);
      display.print(F("F"));
      if (ts.fav_buttton_state != 1)
      {
        draw_button_on_grid(37, 1, "SET AS", "FAV.", 1);
        ts.fav_buttton_state = 1;
      }
    }
  }
}

FLASHMEM bool quick_check_favorites_in_bank(uint8_t p, uint8_t b, uint8_t instance_id)
{
  p = constrain(p, 0, DEXED_POOLS - 1);
  b = constrain(b, 0, MAX_BANKS - 1);
  char tmp[CONFIG_FILENAME_LEN];

  if (sd_card > 0)
  {
    snprintf_P(tmp, sizeof(tmp), PSTR("/%s/%d/%s/%d"), DEXED_CONFIG_PATH, p, FAV_CONFIG_PATH, b);
#ifdef DEBUG
    LOG.print(F("check if there is a Favorite in Bank: "));
    LOG.print(tmp);
    LOG.println();
#endif
    if (SD.exists(tmp))
    { // this bank HAS at least 1 favorite(s)
#ifdef DEBUG
      LOG.println(F("quickcheck found a FAV in bank!"));
#endif
      return (true);
  }
    else
    { // no favorites in bank stored
      return (false);
#ifdef DEBUG
      LOG.println(F(" - It is no Favorite in current Bank."));
#endif
}
}
  else
    return false;
}

FLASHMEM void save_favorite(uint8_t p, uint8_t b, uint8_t v, uint8_t instance_id)
{
#ifdef DEBUG
  LOG.println(F("Starting saving Favorite."));
#endif
  p = constrain(p, 0, DEXED_POOLS - 1);
  b = constrain(b, 0, MAX_BANKS - 1);
  v = constrain(v, 0, MAX_VOICES - 1);
  char tmp[CONFIG_FILENAME_LEN];
  char tmpfolder[CONFIG_FILENAME_LEN];
  File myFav;
  uint8_t i = 0, countfavs = 0;
  if (sd_card > 0)
  {
    snprintf_P(tmp, sizeof(tmp), PSTR("/%s/%d/%s/%d/%d.fav"), DEXED_CONFIG_PATH, p, FAV_CONFIG_PATH, b, v);
    snprintf_P(tmpfolder, sizeof(tmpfolder), PSTR("/%s/%d/%s/%d"), DEXED_CONFIG_PATH, p, FAV_CONFIG_PATH, b);
#ifdef DEBUG
    LOG.println(F("Save Favorite to SD card..."));
    LOG.println(tmp);
#endif
    if (!SD.exists(tmp))
    { // create Favorite Semaphore
      if (!SD.exists(tmpfolder))
      {
        SD.mkdir(tmpfolder);
    }
      myFav = SD.open(tmp, FILE_WRITE);
      myFav.close();
#ifdef DEBUG
      LOG.println(F("Favorite saved..."));
#endif
      // fav symbol
      draw_favorite_icon(p, b, v, instance_id);

#ifdef DEBUG
      LOG.println(F("Added to Favorites..."));
#endif
  }
    else
    { // delete the file, is no longer a favorite
      SD.remove(tmp);
      draw_favorite_icon(p, b, v, instance_id);
#ifdef DEBUG
      LOG.println(F("Removed from Favorites..."));
#endif
      for (i = 0; i < 32; i++)
      { // if no other favs exist in current bank, remove folder
        snprintf_P(tmp, sizeof(tmp), PSTR("/%s/%d/%s/%d/%d.fav"), DEXED_CONFIG_PATH, p, FAV_CONFIG_PATH, b, i);
        if (SD.exists(tmp))
          countfavs++;
      }
      if (countfavs == 0)
      {
        snprintf_P(tmp, sizeof(tmp), PSTR("/%s/%d/%s/%d"), DEXED_CONFIG_PATH, p, FAV_CONFIG_PATH, b);
        SD.rmdir(tmp);
#ifdef DEBUG
        LOG.println(F("Fav count in bank:"));
        LOG.print(countfavs);
        LOG.println(F("Removed folder since no voice in bank flagged as favorite any more"));
#endif
      }

      ////remove fav symbol
      draw_favorite_icon(p, b, v, instance_id);

#ifdef DEBUG
      LOG.println(F("Removed from Favorites..."));
#endif
    }
}
}

FLASHMEM char* basename(const char* filename)
{
  char* p = strrchr(filename, '/');
  return p ? p + 1 : (char*)filename;
}

#ifdef COMPILE_FOR_FLASH
FLASHMEM void fill_msz(char filename[], const uint8_t preset_number, const uint8_t zone_number)
{
  // fill the multisample zone informations
  strcpy(msz[preset_number][zone_number].filename, filename);

  // Search root note from filename
  char root_note[4];
  memset(root_note, 0, sizeof(root_note));

  MatchState ms;
  ms.Target(filename);

  char result = ms.Match("[-_ ][A-G]#?[0-9]");
  if (result > 0)
  {
    memcpy(root_note, filename + ms.MatchStart + 1, ms.MatchLength - 1);
#ifdef DEBUG
    LOG.print(F("Found match at: "));
    LOG.println(ms.MatchStart + 1);
    LOG.print(F("Match length: "));
    LOG.println(ms.MatchLength - 1);
    LOG.print(F("Match root note: "));
    LOG.println(root_note);
#endif

    // get midi note from the root note string
    uint8_t offset = 0;
    switch (root_note[0])
    {
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

    if (root_note[ms.MatchLength - 2 - 1] == '#')
    {
      offset++;
    }
    uint8_t midi_root = (root_note[ms.MatchLength - 1 - 1] - '0' + 1) * 12 + offset;
#ifdef DEBUG
    LOG.printf_P(PSTR("root note found: %s\n"), root_note);
    LOG.printf_P(PSTR("midi root note found: %d\n"), midi_root);
#endif
    msz[preset_number][zone_number].rootnote = midi_root;

    // recalculate low and high notes for all zones
    calc_low_high(preset_number);
  }
  else
  {
#ifdef DEBUG
    LOG.println("No match.");
#endif
  }

#ifdef DEBUG
  LOG.print(F("MSZ preset #"));
  LOG.print(preset_number);
  LOG.print(F(" - zone #"));
  LOG.print(zone_number);
  LOG.print(F(": "));
  LOG.print(msz[preset_number][zone_number].filename);
  LOG.print(F(" root: "));
  LOG.println(msz[preset_number][zone_number].rootnote);
#endif
}
#endif

/*************************************************************************
  RLE_Uncompress() - Uncompress a block of data using an RLE decoder.
   in      - Input (compressed) buffer.
   out     - Output (uncompressed) buffer. This buffer must be large
             enough to hold the uncompressed data.
   insize  - Number of input bytes.
*************************************************************************/

FLASHMEM void RLE_Uncompress(const unsigned char* in, unsigned char* out,
  unsigned int insize)
{
  unsigned char marker, symbol;
  unsigned int i, inpos, outpos, count;

  /* Do we have anything to uncompress? */
  if (insize < 1)
  {
    return;
  }
  /* Get marker symbol from input stream */
  inpos = 0;
  marker = in[inpos++];

  /* Main decompression loop */
  outpos = 0;
  do
  {
    symbol = in[inpos++];
    if (symbol == marker)
    {
      /* We had a marker byte */
      count = in[inpos++];
      if (count <= 2)
      {
        /* Counts 0, 1 and 2 are used for marker byte repetition
           only */
        for (i = 0; i <= count; ++i)
        {
          out[outpos++] = marker;
        }
      }
      else
      {
        if (count & 0x80)
        {
          count = ((count & 0x7f) << 8) + in[inpos++];
        }
        symbol = in[inpos++];
        for (i = 0; i <= count; ++i)
        {
          out[outpos++] = symbol;
        }
      }
    }
    else
    {
      /* No marker, plain copy */
      out[outpos++] = symbol;
    }
  }

  while (inpos < insize);
}

FLASHMEM void splash_draw_header()
{
  display.setCursor(57, 7);
  display.setTextSize(2);
  display.setTextColor(COLOR_SYSTEXT, COLOR_BACKGROUND);
  display.print(F("m   i   c   r   o"));
  display.fillRect(3, 25, DISPLAY_WIDTH - 7, 2, GREY2);
  display.setTextSize(1);
}
FLASHMEM void splash_draw_D()
{
  if (remote_active)
    display.console = true;

  display.fillRect(3, 34, 89, 2, COLOR_PITCHSMP);
  display.fillRect(3, 39, 97, 2, COLOR_PITCHSMP);
  display.fillRect(3, 44, 102, 2, COLOR_PITCHSMP);
  display.fillRect(3, 49, 25, 2, COLOR_PITCHSMP);
  display.fillRect(83, 49, 27, 2, COLOR_PITCHSMP);
  display.fillRect(3, 54, 25, 2, COLOR_PITCHSMP);
  display.fillRect(87, 54, 26, 2, COLOR_PITCHSMP);
  display.fillRect(3, 59, 25, 2, COLOR_PITCHSMP);
  display.fillRect(90, 59, 25, 2, COLOR_PITCHSMP);
  display.fillRect(3, 64, 25, 2, COLOR_PITCHSMP);
  display.fillRect(92, 64, 25, 2, COLOR_PITCHSMP);
  display.fillRect(3, 69, 25, 2, COLOR_PITCHSMP);
  display.fillRect(94, 69, 25, 2, COLOR_PITCHSMP);
  display.fillRect(3, 74, 25, 2, COLOR_PITCHSMP);
  display.fillRect(95, 74, 24, 2, COLOR_PITCHSMP);
  display.fillRect(3, 79, 25, 2, COLOR_PITCHSMP);
  display.fillRect(94, 79, 25, 2, COLOR_PITCHSMP);
  display.fillRect(3, 84, 25, 2, COLOR_PITCHSMP);
  display.fillRect(92, 84, 25, 2, COLOR_PITCHSMP);
  display.fillRect(3, 89, 25, 2, COLOR_PITCHSMP);
  display.fillRect(90, 89, 25, 2, COLOR_PITCHSMP);
  display.fillRect(3, 94, 25, 2, COLOR_PITCHSMP);
  display.fillRect(87, 94, 26, 2, COLOR_PITCHSMP);
  display.fillRect(3, 99, 25, 2, COLOR_PITCHSMP);
  display.fillRect(83, 99, 27, 2, COLOR_PITCHSMP);
  display.fillRect(3, 104, 102, 2, COLOR_PITCHSMP);
  display.fillRect(3, 109, 97, 2, COLOR_PITCHSMP);
  display.fillRect(3, 114, 89, 2, COLOR_PITCHSMP);
  if (remote_active)
    display.console = false;
}

FLASHMEM void splash_draw_X(uint8_t c)
{
  uint16_t colors[2] = { COLOR_PITCHSMP, COLOR_SYSTEXT };

  if (remote_active)
    display.console = true;
  display.fillRect(107, 34, 27, 2, colors[c]);
  display.fillRect(186, 34, 27, 2, colors[c]);
  display.fillRect(112, 39, 27, 2, colors[c]);
  display.fillRect(181, 39, 27, 2, colors[c]);
  display.fillRect(117, 44, 27, 2, colors[c]);
  display.fillRect(176, 44, 27, 2, colors[c]);
  display.fillRect(122, 49, 27, 2, colors[c]);
  display.fillRect(172, 49, 27, 2, colors[c]);
  display.fillRect(127, 54, 27, 2, colors[c]);
  display.fillRect(167, 54, 27, 2, colors[c]);
  display.fillRect(132, 59, 27, 2, colors[c]);
  display.fillRect(162, 59, 27, 2, colors[c]);
  display.fillRect(137, 64, 46, 2, colors[c]);
  display.fillRect(142, 69, 36, 2, colors[c]);
  display.fillRect(147, 74, 26, 2, colors[c]);
  display.fillRect(142, 79, 36, 2, colors[c]);
  display.fillRect(137, 84, 46, 2, colors[c]);
  display.fillRect(132, 89, 27, 2, colors[c]);
  display.fillRect(162, 89, 28, 2, colors[c]);
  display.fillRect(127, 94, 27, 2, colors[c]);
  display.fillRect(167, 94, 28, 2, colors[c]);
  display.fillRect(122, 99, 27, 2, colors[c]);
  display.fillRect(172, 99, 27, 2, colors[c]);
  display.fillRect(117, 104, 27, 2, colors[c]);
  display.fillRect(176, 104, 27, 2, colors[c]);
  display.fillRect(112, 109, 27, 2, colors[c]);
  display.fillRect(181, 109, 28, 2, colors[c]);
  display.fillRect(107, 114, 27, 2, colors[c]);
  display.fillRect(186, 114, 27, 2, colors[c]);
  if (remote_active)
    display.console = false;
}

FLASHMEM void splash_draw_reverseD()
{
  if (remote_active)
    display.console = true;
  display.fillRect(227, 34, 89, 2, COLOR_PITCHSMP);
  display.fillRect(220, 39, 96, 2, COLOR_PITCHSMP);
  display.fillRect(214, 44, 102, 2, COLOR_PITCHSMP);
  display.fillRect(291, 49, 25, 2, COLOR_PITCHSMP);
  display.fillRect(210, 49, 27, 2, COLOR_PITCHSMP);
  display.fillRect(291, 54, 25, 2, COLOR_PITCHSMP);
  display.fillRect(207, 54, 26, 2, COLOR_PITCHSMP);
  display.fillRect(291, 59, 25, 2, COLOR_PITCHSMP);
  display.fillRect(204, 59, 25, 2, COLOR_PITCHSMP);
  display.fillRect(291, 64, 25, 2, COLOR_PITCHSMP);
  display.fillRect(202, 64, 25, 2, COLOR_PITCHSMP);
  display.fillRect(291, 69, 25, 2, COLOR_PITCHSMP);
  display.fillRect(200, 69, 25, 2, COLOR_PITCHSMP);
  display.fillRect(291, 74, 25, 2, COLOR_PITCHSMP);
  display.fillRect(199, 74, 24, 2, COLOR_PITCHSMP);
  display.fillRect(291, 79, 25, 2, COLOR_PITCHSMP);
  display.fillRect(200, 79, 25, 2, COLOR_PITCHSMP);
  display.fillRect(291, 84, 25, 2, COLOR_PITCHSMP);
  display.fillRect(202, 84, 25, 2, COLOR_PITCHSMP);
  display.fillRect(291, 89, 25, 2, COLOR_PITCHSMP);
  display.fillRect(204, 89, 25, 2, COLOR_PITCHSMP);
  display.fillRect(291, 94, 25, 2, COLOR_PITCHSMP);
  display.fillRect(207, 94, 26, 2, COLOR_PITCHSMP);
  display.fillRect(291, 99, 25, 2, COLOR_PITCHSMP);
  display.fillRect(210, 99, 27, 2, COLOR_PITCHSMP);
  display.fillRect(214, 104, 102, 2, COLOR_PITCHSMP);
  display.fillRect(220, 109, 96, 2, COLOR_PITCHSMP);
  display.fillRect(227, 114, 89, 2, COLOR_PITCHSMP);
  if (remote_active)
    display.console = false;
}

FLASHMEM void splash_screen1()
{ // orig, first mdt logo
  splash_draw_header();
  splash_draw_D();
  splash_draw_reverseD();
  display.setTextColor(COLOR_SYSTEXT, COLOR_BACKGROUND);
  display.setTextSize(1);
  display.setCursor(0, 124);
  display.print(F("(c) 2018-2021 H. WIRTZ"));
  display.setCursor(0, 133);
  display.print(F("(c) 2021-2023 H. WIRTZ, M. KOSLOWSKI, D. PERBAL"));
  display.setCursor(0, 142);
  display.print(F("(c) 2024 H.WIRTZ, M.KOSLOWSKI, D.PERBAL, D.WEBER"));
}

FLASHMEM void splash_screen2_anim()
{
  if (sysinfo_sound_state - 10 < 8)
  {
    unsigned char splash[23360];
    RLE_Uncompress(splash_image, splash, 3033);
    uint16_t c;
    uint16_t color;

    if (remote_active)
      display.console = true;

    for (uint8_t y = 0 + sysinfo_sound_state - 10; y < 73; y = y + 8)
    {
      for (uint16_t x = 0; x < DISPLAY_WIDTH; x++)
      {
        if (splash[x + y * DISPLAY_WIDTH] != 0)
        {
          if (splash[x + y * DISPLAY_WIDTH] > 130 || y < 26 || (x < 163 && y < 46) || x > 241 || x < 80 || (x > 189 && y > 64))
          {
            color = RGB24toRGB565(splash[x + y * DISPLAY_WIDTH], splash[x + y * DISPLAY_WIDTH], splash[x + y * DISPLAY_WIDTH]);
          }
          else
            color = RGB24toRGB565(0, splash[x + y * DISPLAY_WIDTH] * 1.5, splash[x + y * DISPLAY_WIDTH]);

          for (uint16_t s = 3; s < 200; s++)
          {
            if (splash[x + y * DISPLAY_WIDTH] == splash[(x + s) + y * DISPLAY_WIDTH] && x + s < DISPLAY_WIDTH)
              c++;
            else
              break;
          }
          if (c > 0 && color > 0)
          {
            display.fillRect(x, y, c + 1, 1, color);
            x = x + c;
          }
          else if (color > 0)
          {
            display.drawPixel(x, y, color);
          }
        }

        c = 0;
        if (y < 26 && x > 132)
          break;
      }
    }
  }

  if (remote_active)
    display.console = false;
}

FLASHMEM void draw_logo_instant(uint8_t yoffset)
{
  unsigned char splash[23360];
  RLE_Uncompress(splash_image, splash, 3033);
  uint16_t c;
  uint16_t color;

  if (remote_active)
    display.console = true;

  for (uint8_t y = 0; y < 73; y++)
  {
    for (uint16_t x = 0; x < DISPLAY_WIDTH; x++)
    {
      if (splash[x + y * DISPLAY_WIDTH] != 0)
      {
        if (splash[x + y * DISPLAY_WIDTH] > 130 || y < 26 || (x < 163 && y < 46) || x > 241 || x < 80 || (x > 189 && y > 64))
        {

          color = RGB24toRGB565(splash[x + y * DISPLAY_WIDTH], splash[x + y * DISPLAY_WIDTH], splash[x + y * DISPLAY_WIDTH]);
        }
        else
          color = RGB24toRGB565(0, splash[x + y * DISPLAY_WIDTH] * 1.5, splash[x + y * DISPLAY_WIDTH]);
        for (uint16_t s = 3; s < 200; s++)
        {
          if (splash[x + y * DISPLAY_WIDTH] == splash[(x + s) + y * DISPLAY_WIDTH] && x + s < DISPLAY_WIDTH)
            c++;
          else
            break;
        }
        if (c > 0 && color > 0)
        {
          display.fillRect(x, y + yoffset, c + 1, 1, color);
          x = x + c;
        }
        else if (color > 0)
        {
          display.drawPixel(x, y + yoffset, color);
        }
      }
      c = 0;
      if (y < 26 && x > 132)
        break;
    }
  }

  if (remote_active)
    display.console = false;
}

FLASHMEM void splash_screen2()
{  // modern mdt logo
  display.setTextColor(COLOR_SYSTEXT);
  display.setTextSize(1);
  display.setCursor(1, 90);
  display.print(F("(c) 2018-2021 H. WIRTZ"));
  display.setCursor(1, 100);
  display.print(F("(c) 2021-2023 H. WIRTZ, M. KOSLOWSKI, D. PERBAL"));
  display.setCursor(1, 110);
  display.print(F("(c) 2024 H.WIRTZ, M.KOSLOWSKI, D.PERBAL, D.WEBER"));
  display.setCursor(1 + CHAR_width_small * 4, 128);
  display.setTextColor(GREY2);
  display.print(F("https://codeberg.org/positionhigh/"));
}

float scalex = 1;
float scaley = 1;
int UI_FM_offset_x = 0;
int UI_FM_offset_y = 0;
#define LINE_SZ 2

FLASHMEM void displayOp(char id, int _gridX, int _gridY, char link, char fb)
{
  //  bool opOn = opStatus[6-id] == '1';
  bool opOn = true;
  int x = _gridX * 24;
  x += 3 + UI_FM_offset_x;
  int y = _gridY * 21;
  y += 5 + UI_FM_offset_y;

  // Draw OP
  display.console = true;
  display.fillRect(x, y, 13, 11, _gridY == 3 ? MIDDLEGREEN : DX_DARKCYAN);
  display.setTextSize(1);
  if (opOn)
  {
    display.setTextColor(COLOR_SYSTEXT, _gridY == 3 ? MIDDLEGREEN : DX_DARKCYAN);
  }
  else
  {
    display.setTextColor(RED, GREY4);
  }
  display.setCursor(x + 4, y + 2);
  display.print(id + 0);

  // Draw lines
  uint16_t color;
  if (opOn)
  {
    color = GREY2;
  }
  else
  {
    color = RED;
  }
  display.console = true;
  switch (link)
  {
  case 0: // LINE DOWN
    display.fillRect(x + 6, y + 11, LINE_SZ, 10, color);
    break;
  case 1: // ARROW TO RIGHT
    display.fillRect(x + 6, y + 11, LINE_SZ, 6, color);
    display.fillRect(x + 6, y + 15, 25, LINE_SZ, color);
    break;
  case 2: // ARROW TO RIGHT JOIN
    display.fillRect(x + 6, y + 11, LINE_SZ, 6, color);
    break;
  case 3: // ARROW TO RIGHT AND DOWN
    display.fillRect(x + 6, y + 11, LINE_SZ, 10, color);
    display.fillRect(x + 6, y + 16, 25, LINE_SZ, color);
    display.fillRect(x + 31, y + 16, LINE_SZ, 5, color);
    break;
  case 4: // ARROW TO RIGHT+LEFT AND DOWN
    display.fillRect(x + 6, y + 11, LINE_SZ, 10, color);
    display.fillRect(x + 6, y + 16, 25, LINE_SZ, color);
    display.fillRect(x + 30, y + 16, LINE_SZ, 5, color);
    display.fillRect(x - 17, y + 16, 25, LINE_SZ, color);
    display.fillRect(x - 18, y + 16, LINE_SZ, 5, color);
    break;
  case 6:
    display.fillRect(x + 6, y + 11, LINE_SZ, 6, color);
    display.fillRect(x + 6, y + 15, 50, LINE_SZ, color);
    break;
  case 7: // ARROW TO LEFT
    display.fillRect(x + 6, y + 11, LINE_SZ, 6, color);
    display.fillRect(x - 17, y + 15, 25, LINE_SZ, color);
    break;
  case 8: // ARROW TO LEFT JOIN
    display.fillRect(x + 6, y + 11, LINE_SZ, 6, color);
    break;
  }

  switch (fb)
  {
  case 0:
    break;
  case 1: // single OP feedback
    display.fillRect(x + 6, y - 4, LINE_SZ, 4, color);
    display.fillRect(x + 6, y - 4, 10, LINE_SZ, color);
    display.fillRect(x + 15, y - 4, LINE_SZ, 19, color);
    display.fillRect(x + 6, y + 13, 10, LINE_SZ, color);
    break;
  case 2: // ALGO 4: 3 OPs feedback
    display.fillRect(x + 6, y - 4, LINE_SZ, 5, color);
    display.fillRect(x + 6, y - 4, 10, LINE_SZ, color);
    display.fillRect(x + 15, y - 4, LINE_SZ, 62, color);
    display.fillRect(x + 6, y + 56, 10, LINE_SZ, color);
    break;
  case 3: // ALGO 6: 2 OPs feedback
    display.fillRect(x + 6, y - 4, LINE_SZ, 5, color);
    display.fillRect(x + 6, y - 4, 10, LINE_SZ, color);
    display.fillRect(x + 15, y - 4, LINE_SZ, 45, color);
    display.fillRect(x + 6, y + 36, 10, LINE_SZ, color);
    break;
  case 4: // single OP feedback to the left
    display.fillRect(x + 6, y - 4, LINE_SZ, 4, color);
    display.fillRect(x - 4, y - 4, 10, LINE_SZ, color);
    display.fillRect(x - 4, y - 4, LINE_SZ, 19, color);
    display.fillRect(x - 4, y + 13, 10, LINE_SZ, color);
    break;
  }
}

FLASHMEM void UI_draw_FM_algorithm(uint8_t algo, uint8_t x, uint8_t y)
{
  UI_FM_offset_x = x;
  UI_FM_offset_y = y + 3;
  display.console = true;
  display.fillRect(x + 75, y + 4, 37, 22, COLOR_BACKGROUND);
  display.fillRect(x + 23, y + 25, 93, 41, COLOR_BACKGROUND);
  display.fillRect(x + 2, y + 66, 138, 27, COLOR_BACKGROUND);

  setCursor_textGrid_small(33, 10);
  display.setTextSize(1);
  display.setTextColor(COLOR_SYSTEXT, COLOR_BACKGROUND);
  print_formatted_number(algo + 1, 2);

  switch (algo)
  {
  case 0:
    displayOp(6, 3, 0, 0, 1);
    displayOp(5, 3, 1, 0, 0);
    displayOp(4, 3, 2, 0, 0);
    displayOp(3, 3, 3, 2, 0);
    displayOp(2, 2, 2, 0, 0);
    displayOp(1, 2, 3, 1, 0);
    break;
  case 1:
    displayOp(6, 3, 0, 0, 0);
    displayOp(5, 3, 1, 0, 0);
    displayOp(4, 3, 2, 0, 0);
    displayOp(3, 3, 3, 2, 0);
    displayOp(2, 2, 2, 0, 1);
    displayOp(1, 2, 3, 1, 0);
    break;
  case 2:
    displayOp(6, 3, 1, 0, 1);
    displayOp(5, 3, 2, 0, 0);
    displayOp(4, 3, 3, 2, 0);
    displayOp(3, 2, 1, 0, 0);
    displayOp(2, 2, 2, 0, 0);
    displayOp(1, 2, 3, 1, 0);
    break;
  case 3:
    displayOp(6, 3, 1, 0, 2);
    displayOp(5, 3, 2, 0, 0);
    displayOp(4, 3, 3, 2, 0);
    displayOp(3, 2, 1, 0, 0);
    displayOp(2, 2, 2, 0, 0);
    displayOp(1, 2, 3, 1, 0);
    break;
  case 4:
    displayOp(6, 4, 2, 0, 1);
    displayOp(5, 4, 3, 2, 0);
    displayOp(4, 3, 2, 0, 0);
    displayOp(3, 3, 3, 1, 0);
    displayOp(2, 2, 2, 0, 0);
    displayOp(1, 2, 3, 1, 0);
    break;
  case 5:
    displayOp(6, 4, 2, 0, 3);
    displayOp(5, 4, 3, 2, 0);
    displayOp(4, 3, 2, 0, 0);
    displayOp(3, 3, 3, 1, 0);
    displayOp(2, 2, 2, 0, 0);
    displayOp(1, 2, 3, 1, 0);
    break;
  case 6:
    displayOp(6, 4, 1, 0, 1);
    displayOp(5, 4, 2, 7, 0);
    displayOp(4, 3, 2, 0, 0);
    displayOp(3, 3, 3, 2, 0);
    displayOp(2, 2, 2, 0, 0);
    displayOp(1, 2, 3, 1, 0);
    break;
  case 7:
    displayOp(6, 4, 1, 0, 0);
    displayOp(5, 4, 2, 7, 0);
    displayOp(4, 3, 2, 0, 4);
    displayOp(3, 3, 3, 2, 0);
    displayOp(2, 2, 2, 0, 0);
    displayOp(1, 2, 3, 1, 0);
    break;
  case 8:
    displayOp(6, 4, 1, 0, 0);
    displayOp(5, 4, 2, 7, 0);
    displayOp(4, 3, 2, 0, 0);
    displayOp(3, 3, 3, 2, 0);
    displayOp(2, 2, 2, 0, 1);
    displayOp(1, 2, 3, 1, 0);
    break;
  case 9:
    displayOp(6, 2, 2, 0, 0);
    displayOp(5, 1, 2, 1, 0);
    displayOp(4, 2, 3, 1, 0);
    displayOp(3, 3, 1, 0, 1);
    displayOp(2, 3, 2, 0, 0);
    displayOp(1, 3, 3, 2, 0);
    break;
  case 10:
    displayOp(6, 2, 2, 0, 1);
    displayOp(5, 1, 2, 1, 0);
    displayOp(4, 2, 3, 1, 0);
    displayOp(3, 3, 1, 0, 0);
    displayOp(2, 3, 2, 0, 0);
    displayOp(1, 3, 3, 2, 0);
    break;
  case 11:
    displayOp(6, 3, 2, 7, 0);
    displayOp(5, 2, 2, 0, 0);
    displayOp(4, 1, 2, 1, 0);
    displayOp(3, 2, 3, 6, 0);
    displayOp(2, 4, 2, 0, 1);
    displayOp(1, 4, 3, 2, 0);
    break;
  case 12:
    displayOp(6, 3, 2, 7, 1);
    displayOp(5, 2, 2, 0, 0);
    displayOp(4, 1, 2, 1, 0);
    displayOp(3, 2, 3, 6, 0);
    displayOp(2, 4, 2, 0, 0);
    displayOp(1, 4, 3, 2, 0);
    break;
  case 13:
    displayOp(6, 3, 1, 0, 1);
    displayOp(5, 2, 1, 1, 0);
    displayOp(4, 3, 2, 0, 0);
    displayOp(3, 3, 3, 2, 0);
    displayOp(2, 2, 2, 0, 0);
    displayOp(1, 2, 3, 1, 0);
    break;
  case 14:
    displayOp(6, 4, 1, 7, 0);
    displayOp(5, 3, 1, 0, 0);
    displayOp(4, 3, 2, 0, 0);
    displayOp(3, 3, 3, 2, 0);
    displayOp(2, 2, 2, 0, 4);
    displayOp(1, 2, 3, 1, 0);
    break;
  case 15:
    displayOp(6, 4, 1, 0, 1);
    displayOp(5, 4, 2, 7, 0);
    displayOp(4, 3, 1, 0, 0);
    displayOp(3, 3, 2, 0, 0);
    displayOp(2, 2, 2, 1, 0);
    displayOp(1, 3, 3, 0, 0);
    break;
  case 16:
    displayOp(6, 4, 1, 0, 0);
    displayOp(5, 4, 2, 7, 0);
    displayOp(4, 3, 1, 0, 0);
    displayOp(3, 3, 2, 0, 0);
    displayOp(2, 2, 2, 1, 4);
    displayOp(1, 3, 3, 0, 0);
    break;
  case 17:
    displayOp(6, 4, 0, 0, 0);
    displayOp(5, 4, 1, 0, 0);
    displayOp(4, 4, 2, 7, 0);
    displayOp(3, 3, 2, 0, 4);
    displayOp(2, 2, 2, 1, 0);
    displayOp(1, 3, 3, 0, 0);
    break;
  case 18:
    displayOp(6, 3, 2, 3, 4);
    displayOp(5, 4, 3, 2, 0);
    displayOp(4, 3, 3, 1, 0);
    displayOp(3, 2, 1, 0, 0);
    displayOp(2, 2, 2, 0, 0);
    displayOp(1, 2, 3, 1, 0);
    break;
  case 19:
    displayOp(6, 4, 2, 0, 0);
    displayOp(5, 3, 2, 1, 0);
    displayOp(4, 4, 3, 2, 0);
    displayOp(3, 1, 2, 3, 4);
    displayOp(2, 2, 3, 6, 0);
    displayOp(1, 1, 3, 1, 0);
    break;
  case 20:
    displayOp(6, 3, 2, 3, 0);
    displayOp(5, 4, 3, 2, 0);
    displayOp(4, 3, 3, 1, 0);
    displayOp(3, 1, 2, 3, 1);
    displayOp(2, 2, 3, 1, 0);
    displayOp(1, 1, 3, 1, 0);
    break;
  case 21:
    displayOp(6, 3, 2, 4, 1);
    displayOp(5, 4, 3, 2, 0);
    displayOp(4, 3, 3, 1, 0);
    displayOp(3, 2, 3, 1, 0);
    displayOp(2, 1, 2, 0, 0);
    displayOp(1, 1, 3, 1, 0);
    break;
  case 22: // CC
    displayOp(6, 3, 2, 3, 1);
    displayOp(5, 4, 3, 2, 0);
    displayOp(4, 3, 3, 1, 0);
    displayOp(3, 2, 2, 0, 0);
    displayOp(2, 2, 3, 1, 0);
    displayOp(1, 1, 3, 1, 0);
    break;
  case 23: // CC
    displayOp(6, 3, 2, 4, 1);
    displayOp(5, 4, 3, 2, 0);
    displayOp(4, 3, 3, 1, 0);
    displayOp(3, 2, 3, 1, 0);
    displayOp(2, 1, 3, 1, 0);
    displayOp(1, 0, 3, 1, 0);
    break;
  case 24: // CC
    displayOp(6, 3, 2, 3, 1);
    displayOp(5, 4, 3, 2, 0);
    displayOp(4, 3, 3, 1, 0);
    displayOp(3, 2, 3, 1, 0);
    displayOp(2, 1, 3, 1, 0);
    displayOp(1, 0, 3, 1, 0);
    break;
  case 25:
    displayOp(6, 4, 2, 0, 1);
    displayOp(5, 3, 2, 1, 0);
    displayOp(4, 4, 3, 2, 0);
    displayOp(3, 2, 2, 0, 0);
    displayOp(2, 2, 3, 6, 0);
    displayOp(1, 1, 3, 1, 0);
    break;
  case 26:
    displayOp(6, 4, 2, 0, 0);
    displayOp(5, 3, 2, 1, 0);
    displayOp(4, 4, 3, 2, 0);
    displayOp(3, 2, 2, 0, 1);
    displayOp(2, 2, 3, 6, 0);
    displayOp(1, 1, 3, 1, 0);
    break;
  case 27:
    displayOp(6, 4, 3, 2, 0);
    displayOp(5, 3, 1, 0, 1);
    displayOp(4, 3, 2, 0, 0);
    displayOp(3, 3, 3, 1, 0);
    displayOp(2, 2, 2, 0, 0);
    displayOp(1, 2, 3, 1, 0);
    break;
  case 28:
    displayOp(6, 4, 2, 0, 1);
    displayOp(5, 4, 3, 2, 0);
    displayOp(4, 3, 2, 0, 0);
    displayOp(3, 3, 3, 1, 0);
    displayOp(2, 2, 3, 1, 0);
    displayOp(1, 1, 3, 1, 0);
    break;
  case 29:
    displayOp(6, 4, 3, 2, 0);
    displayOp(5, 3, 1, 0, 1);
    displayOp(4, 3, 2, 0, 0);
    displayOp(3, 3, 3, 1, 0);
    displayOp(2, 2, 3, 1, 0);
    displayOp(1, 1, 3, 1, 0);
    break;
  case 30:
    displayOp(6, 4, 2, 0, 1);
    displayOp(5, 4, 3, 2, 0);
    displayOp(4, 3, 3, 1, 0);
    displayOp(3, 2, 3, 1, 0);
    displayOp(2, 1, 3, 1, 0);
    displayOp(1, 0, 3, 1, 0);
    break;
  case 31:
    displayOp(6, 5, 3, 2, 1);
    displayOp(5, 4, 3, 1, 0);
    displayOp(4, 3, 3, 1, 0);
    displayOp(3, 2, 3, 1, 0);
    displayOp(2, 1, 3, 1, 0);
    displayOp(1, 0, 3, 1, 0);
    break;
  default:
    break;
  }
}

FLASHMEM void _draw_volmeter(int x, int y, uint8_t meter, float height)
{
  // draw bar
  if (height > ts.displayed_peak[meter])
  {
    // draw a rainbow gradient rectangle, but only up to the old displayed_peak
    display.fillRectRainbow(x, y, 17, height, ts.displayed_peak[meter]);
    ts.displayed_peak[meter] = height;
  }
  else
  {
    if (ts.displayed_peak[meter] > 1)
    {
      display.fillRect(x, y - (ts.displayed_peak[meter]), 17, 2, COLOR_BACKGROUND);
      ts.displayed_peak[meter] = ts.displayed_peak[meter] - 2;
    }
    else if (ts.displayed_peak[meter] > 0)
    {
      display.fillRect(x, y - (ts.displayed_peak[meter]), 17, 1, COLOR_BACKGROUND);
      ts.displayed_peak[meter] = ts.displayed_peak[meter] - 1;
    }
  }
}

FLASHMEM void draw_volmeters_mixer()
{
  float meters[13] = {
    microdexed_peak_0.read(),
    microdexed_peak_1.read(),
    ep_peak_l.available() && ep_peak_r.available() ? (ep_peak_l.read() + ep_peak_r.read()) / 2 : 0,
    microsynth_peak_osc_0.available() ? microsynth_peak_osc_0.read() : 0,
    microsynth_peak_osc_1.available() ? microsynth_peak_osc_1.read() : 0,
    braids_peak_l.available() && braids_peak_r.available() ? (braids_peak_l.read() + braids_peak_r.read()) / 2 : 0,
    mapfloat(ts.msp_peak[0], 0, 127, 0.0, 1.0),
    mapfloat(ts.msp_peak[1], 0, 127, 0.0, 1.0),
    drum_mixer_peak_l.available() && drum_mixer_peak_r.available() ? (drum_mixer_peak_l.read() + drum_mixer_peak_r.read()) / 2 : 0,
    reverb_return_peak_l.read(),
    reverb_return_peak_r.read(),
    master_peak_l.read(),
    master_peak_r.read()
  };
  int y = 170;

  display.setTextSize(1);
  display.setTextColor(COLOR_SYSTEXT, COLOR_BACKGROUND);

  for (uint8_t i = 0; i < 13; i++) {
    int x = CHAR_width_small * i * 4 + (i >= 9 ? 2 * CHAR_width_small : 0);

    // draw text
    display.setCursor(x, y + 4);

    int height = mapfloat(meters[i], 0.0, 1.0, 0, 99);

    // draw bar
    _draw_volmeter(x, y, i, height);
  }
}

FLASHMEM void draw_volmeters_multiband_compressor()
{
  float l, r;
  l = mb_before_l.read();
  r = mb_before_r.read();

  float meters[4] = {
    l,
    r,
    multiband_active ? mb_after_l.read() : l,
    multiband_active ? mb_after_r.read() : r,
  };
  int y = 228;

  display.setTextSize(1);
  display.setTextColor(COLOR_SYSTEXT, COLOR_BACKGROUND);

  for (uint8_t i = 0; i < 4; i++) {
    int x = CHAR_width_small * i * 4 + (i >= 2 ? 38 * CHAR_width_small : 1 * CHAR_width_small);

    // draw text
    display.setCursor(x, y + 4);

    int height = mapfloat(meters[i], 0.0, 1.0, 0, 99);

    // draw bar
    _draw_volmeter(x, y, i, height);
  }
}

FLASHMEM void clear_volmeter(int x, int y)
{
  display.console = false;
  display.fillRect(x, y - 100, 17, 100, COLOR_BACKGROUND);
}

#endif
