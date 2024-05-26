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

boolean COND_hide() // hide a menu element
{
  return false;
}

// the id numbers MUST be kept in incrementing sequence.
// when menu options are added or removed, they must be renumbered!
// To do this automatically, paste the code below beginning with the second line
//  into the following command:
// cat -n | sed -E 's/([0-9]+)([^0-9]+)([0-9]+)(.*+)/\2\1\4/'

LCDML_add(0, LCDML_0, 1, "Dexed", NULL);
LCDML_add(1, LCDML_0_1, 1, "Select Voice", UI_func_voice_select);
LCDML_add(2, LCDML_0_1, 2, "Edit Voice", UI_func_voice_editor);
LCDML_add(3, LCDML_0_1, 3, "Audio", UI_func_dexed_audio);
LCDML_add(4, LCDML_0_1, 4, "Controller", UI_func_dexed_controllers);
LCDML_add(5, LCDML_0_1, 5, "Setup", UI_func_dexed_setup);
LCDML_add(6, LCDML_0_1, 6, "Operator", UI_handle_OP);
LCDML_add(7, LCDML_0_1, 7, "MIDI Send Voice", UI_func_sysex_send_voice);
LCDML_add(8, LCDML_0_1, 8, "Save Voice", UI_func_save_voice);
LCDML_add(9, LCDML_0, 2, "E-Piano", UI_func_epiano);
LCDML_add(10, LCDML_0, 3, "MicroSynth", UI_func_microsynth);
LCDML_add(11, LCDML_0, 4, "Braids", UI_func_braids);
LCDML_add(12, LCDML_0, 5, "MultiSample", UI_func_MultiSamplePlay);
LCDML_add(13, LCDML_0, 6, "Drums", UI_func_drums);
LCDML_add(14, LCDML_0, 7, "Master Effects", NULL);
LCDML_add(15, LCDML_0_7, 1, "Delay+Reverb", UI_func_master_effects);
LCDML_add(16, LCDML_0_7, 2, "SideChain", UI_func_sidechain);
LCDML_add(17, LCDML_0_7, 3, "Multiband", UI_func_multiband_dynamics);
LCDML_add(18, LCDML_0, 8, "Sequencer", NULL);
LCDML_add(19, LCDML_0_8, 1, "Song", UI_func_song);
LCDML_add(20, LCDML_0_8, 2, "Pattern Editor", UI_func_seq_pattern_editor);
LCDML_add(21, LCDML_0_8, 3, "Vel./Chrd Edit", UI_func_seq_vel_editor);
LCDML_add(22, LCDML_0_8, 4, "Live Sequencer", UI_func_livesequencer);
//LCDML_add(23, LCDML_0_8, 5, "Tracker", UI_func_seq_tracker);
LCDML_add(23, LCDML_0_8, 5, "Live Seq.Edit", UI_func_liveseq_editor);
LCDML_add(24, LCDML_0_8, 6, "Arpeggio", UI_func_arpeggio);
LCDML_add(25, LCDML_0_8, 7, "Mute Matrix", UI_func_seq_mute_matrix);
LCDML_add(26, LCDML_0_8, 8, "PianoRoll", UI_func_seq_pianoroll);
LCDML_add(27, LCDML_0_8, 9, "Probabilities", UI_func_seq_probabilities);
LCDML_add(28, LCDML_0_8, 10, "Smart Filter", UI_func_smart_filter);
LCDML_add(29, LCDML_0_8, 11, "Clear ALL", UI_func_clear_all);
LCDML_add(30, LCDML_0_8, 12, "Clear Song", UI_func_clear_song);
LCDML_add(31, LCDML_0_8, 13, "Clear Chains", UI_func_clear_song_chains);
LCDML_add(32, LCDML_0_8, 14, "Clear Patterns", UI_func_clear_patterns);
LCDML_add(33, LCDML_0_8, 15, "Settings", UI_func_seq_settings);
LCDML_add(34, LCDML_0, 9, "Mixer", UI_func_mixer);
LCDML_add(35, LCDML_0, 11, "Sample Editor", UI_func_sample_editor);
LCDML_add(36, LCDML_0, 12, "Audio/SMP Record", UI_func_recorder);
LCDML_add(37, LCDML_0, 13, "Load/Save", NULL);
LCDML_add(38, LCDML_0_13, 1, "Load Perf.", UI_func_load_performance);
LCDML_add(39, LCDML_0_13, 2, "Save Perf.", UI_func_save_performance);
LCDML_add(40, LCDML_0_13, 3, "Name Perf.", UI_func_set_performance_name);
LCDML_add(41, LCDML_0_13, 4, "Name Multisample", UI_func_set_multisample_name);
LCDML_add(42, LCDML_0_13, 5, "MIDI", NULL);
LCDML_add(43, LCDML_0_13_5, 1, "MIDI Recv Bank", UI_func_sysex_receive_bank);
LCDML_add(44, LCDML_0_13_5, 2, "MIDI Snd Bank", UI_func_sysex_send_bank);
LCDML_add(45, LCDML_0_13_5, 3, "MIDI Snd Voice", UI_func_sysex_send_voice);
LCDML_add(46, LCDML_0, 14, "File Manager", UI_func_file_manager);
LCDML_add(47, LCDML_0, 15, "System", NULL);
LCDML_add(48, LCDML_0_15, 1, "Stereo/Mono", UI_func_stereo_mono);
LCDML_add(49, LCDML_0_15, 2, "MIDI channels", UI_func_midi_channels);
LCDML_add(50, LCDML_0_15, 3, "MIDI Soft THRU", UI_func_midi_soft_thru);
LCDML_add(51, LCDML_0_15, 4, "MIDI Mapping", UI_func_custom_mappings);
LCDML_add(52, LCDML_0_15, 5, "Favorites", UI_func_favorites);
LCDML_add(53, LCDML_0_15, 6, "Startup Perform.", UI_func_startup_performance);
LCDML_add(54, LCDML_0_15, 7, "Startup Page", UI_func_startup_page);
LCDML_add(55, LCDML_0_15, 8, "Misc. Settings", UI_func_misc_settings);
LCDML_add(56, LCDML_0_15, 9, "Map Gamepad", UI_func_map_gamepad);
LCDML_add(57, LCDML_0_15, 10, "Calibrate Touch", UI_func_calibrate_touch);
LCDML_add(58, LCDML_0_15, 11, "TEST Touchscreen", UI_func_test_touchscreen);
LCDML_add(59, LCDML_0_15, 12, "TEST Audio Mute", UI_func_test_mute);
LCDML_add(60, LCDML_0_15, 13, "TEST PSRAM", UI_func_test_psram);
LCDML_add(61, LCDML_0_15, 14, "FORMAT FLASH", UI_func_format_flash);
LCDML_add(62, LCDML_0, 16, "Info", UI_func_information);
//LCDML_add(62, LCDML_0, 17, "MIDI SYNC", UI_func_midi_sync);
LCDML_addAdvanced(63, LCDML_0, 17, COND_hide, "Volume", UI_func_volume, 0, _LCDML_TYPE_default);
LCDML_addAdvanced(64, LCDML_0, 18, COND_hide, "Screensaver", mFunc_screensaver, 0, _LCDML_TYPE_default);
LCDML_addAdvanced(65, LCDML_0, 19, COND_hide, "SD_NOT_FOUND", UI_func_sd_content_not_found, 0, _LCDML_TYPE_default);
#define _LCDML_DISP_cnt 64
#endif

#define SCREENSAVER_INTERVAL_MS 40  // 25Hz refresh rate
#define SCREENSAVER_STAY_TIME 1500  // 60s @ 25Hz rate
#define SCREENSAVER_FADE_TIME 25    // 1s @ 25Hz rate
#define SCREENSAVER_BRIGHTNESS_STEP (255/SCREENSAVER_FADE_TIME)
#define SCREENSAVER_MAX_COUNTHUE 359
