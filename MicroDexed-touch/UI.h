/*
   MicroDexed-touch

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

#ifndef _UI_H_
#define _UI_H_

#define _LCDML_DISP_cnt 64
#define SCREENSAVER_INTERVAL_MS 40  // 25Hz refresh rate
#define SCREENSAVER_STAY_TIME 1500  // 60s @ 25Hz rate
#define SCREENSAVER_FADE_TIME 25    // 1s @ 25Hz rate
#define SCREENSAVER_BRIGHTNESS_STEP (255/SCREENSAVER_FADE_TIME)
#define SCREENSAVER_MAX_COUNTHUE 359

void splash_draw_header();
void splash_draw_D();
void splash_draw_reverseD();
void splash_draw_X(uint8_t c);
void splash_screen2_anim(uint8_t sysinfo_sound_state);
void draw_logo_instant(uint8_t yoffset);
void setCursor_textGrid(uint8_t pos_x, uint8_t pos_y);
void setCursor_textGrid_small(uint8_t pos_x, uint8_t pos_y);

void UI_func_stereo_mono(uint8_t param);
void UI_func_dexed_audio(uint8_t param);
void UI_func_dexed_controllers(uint8_t param);
void UI_func_dexed_setup(uint8_t param);
void UI_handle_OP(uint8_t param);
void UI_func_information(uint8_t param);
void UI_func_liveseq_listeditor(uint8_t param);
void UI_func_master_effects(uint8_t param);
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
void UI_func_drums(uint8_t param);
void UI_func_MultiSamplePlay(uint8_t param);
void UI_func_liveseq_pianoroll(uint8_t param);
void UI_func_arpeggio(uint8_t param);
void UI_func_seq_mute_matrix(uint8_t param);
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
void UI_func_system_settings(uint8_t param);
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

#endif //_UI_H_