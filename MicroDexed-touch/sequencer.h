#include <sys/_stdint.h>
/*
   MicroDexed

   MicroDexed is a port of the Dexed sound engine
   (https://github.com/asb2m10/dexed) for the Teensy-3.5/3.6/4.x with audio shield.
   Dexed ist heavily based on https://github.com/google/music-synthesizer-for-android

   (c)2018-2021 M. Koslowski <positionhigh@gmx.de>
                H. Wirtz <wirtz@parasitstudio.de>

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program; if not, write to the Free Software Foundation,
   Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301  USA
*/

#ifndef _SEQUENCER_H
#define _SEQUENCER_H

#if defined GENERIC_DISPLAY
#include "XPT2046_Touchscreen.h"
#endif

#ifdef CAPACITIVE_TOUCH_DISPLAY
#include "Adafruit_FT6206.h"
#endif

#include <avr/pgmspace.h>
#include <SD.h>
#include "param.h"

extern Sd2Card card;

extern char noteNames[12][3];

static const float tune_frequencies2_PGM[128] = {
    8.1758, 8.6620, 9.1770, 9.7227, 10.3009, 10.9134, 11.5623, 12.2499,
    12.9783, 13.7500, 14.5676, 15.4339, 16.3516, 17.3239, 18.3540, 19.4454,
    20.6017, 21.8268, 23.1247, 24.4997, 25.9565, 27.5000, 29.1352, 30.8677,
    32.7032, 34.6478, 36.7081, 38.8909, 41.2034, 43.6535, 46.2493, 48.9994,
    51.9131, 55.0000, 58.2705, 61.7354, 65.4064, 69.2957, 73.4162, 77.7817,
    82.4069, 87.3071, 92.4986, 97.9989, 103.8262, 110.0000, 116.5409, 123.4708,
    130.8128, 138.5913, 146.8324, 155.5635, 164.8138, 174.6141, 184.9972, 195.9977,
    207.6523, 220.0000, 233.0819, 246.9417, 261.6256, 277.1826, 293.6648, 311.1270,
    329.6276, 349.2282, 369.9944, 391.9954, 415.3047, 440.0000, 466.1638, 493.8833,
    523.2511, 554.3653, 587.3295, 622.2540, 659.2551, 698.4565, 739.9888, 783.9909,
    830.6094, 880.0000, 932.3275, 987.7666, 1046.5023, 1108.7305, 1174.6591, 1244.5079,
    1318.5102, 1396.9129, 1479.9777, 1567.9817, 1661.2188, 1760.0000, 1864.6550, 1975.5332,
    2093.0045, 2217.4610, 2349.3181, 2489.0159, 2637.0205, 2793.8259, 2959.9554, 3135.9635,
    3322.4376, 3520.0000, 3729.3101, 3951.0664, 4186.0090, 4434.9221, 4698.6363, 4978.0317,
    5274.0409, 5587.6517, 5919.9108, 6271.9270, 6644.8752, 7040.0000, 7458.6202, 7902.1328,
    8372.0181, 8869.8442, 9397.2726, 9956.0635, 10548.0818, 11175.3034, 11839.8215, 12543.8540 };

typedef struct sequencer_s : public Params
{
    Param tempo_ms_desc{P_INT32_T, 1, "seq_tempo_ms", 16000,360000,180000}; int32_t tempo_ms;
    Param pattern_len_dec_desc{P_UINT8_T, 1, "pattern_len_dec", 0,8,0}; uint8_t pattern_len_dec;
    Param swing_steps_desc{P_UINT8_T, 1, "swing_steps", 0,2,0}; uint8_t swing_steps;
    Param bpm_desc{P_UINT8_T, 1, "seq_bpm", 40,190,102}; uint8_t bpm;
    Param arp_speed_desc{P_UINT8_T, 1, "arp_speed", 0,3,0}; uint8_t arp_speed;
    Param arp_length_desc{P_UINT8_T, 1, "arp_length", 0,16,8}; uint8_t arp_length;
    Param arp_volume_fade_desc{P_FLOAT, 1, "arp_volume_fade", 0,1.,0.01}; float arp_volume_fade;
    Param arp_style_desc{P_UINT8_T, 1, "arp_style", 0,3,0}; uint8_t arp_style; // up, down, up&down, random
    Param chord_vel_desc{P_UINT8_T, 1, "seq_chord_vel", 10,120,60}; uint8_t chord_vel;
    Param transpose_desc{P_UINT8_T, 1, "seq_transpose", 0,60,0}; uint8_t transpose;
    Param chord_key_ammount_desc{P_UINT8_T, 1, "chord_key_ammount", 1,7,4}; uint8_t chord_key_ammount;
    Param oct_shift_desc{P_INT8_T, 1, "seq_oct_shift", -2,2,0}; int8_t oct_shift;
    Param element_shift_desc{P_UINT8_T, 1, "seq_element_shift", 0,6,0}; uint8_t element_shift;
    Param euclidean_active_desc{P_UINT8_T, 1, "euclidean_active", 0,1,0}; bool euclidean_active;
    Param euclidean_offset_desc{P_UINT8_T, 1, "euclidean_offset", 0,15,5}; uint8_t euclidean_offset;
    Param track_type_desc{P_UINT8_T, NUM_SEQ_TRACKS, "track_type", 0,3,0}; uint8_t track_type[NUM_SEQ_TRACKS] = { 0, 0, 1, 1, 1, 1, 0, 0 }; // 0 = track is Drumtrack, 1 = Instrumenttrack, 2 = Chord, 3 = Arp
    Param content_type_desc{P_UINT8_T, NUM_SEQ_PATTERN, "content_type", 0,2,0}; uint8_t content_type[NUM_SEQ_PATTERN];
    // 0 = Drum pattern, 1 = Instrument pattern, 2 = Chord or Arpeggio
    Param instrument_desc{P_UINT8_T, NUM_SEQ_TRACKS, "seq_inst_dexed", 0,47,0}; uint8_t instrument[NUM_SEQ_TRACKS] = { 0, 0, 1, 1, 1, 1, 0, 0 }; // dexed instance 0+1,  2 = epiano , 3+4 = MicroSynth, 5 = Braids, 6-15 MultiSample 16-31 = MIDI OUT USB, 32-47 MIDI OUT DIN
    Param name_desc{P_UINT8_T, FILENAME_LEN, "seq_name", 0,255,'\0'}; char name[FILENAME_LEN];
    Param pat_chance_desc{P_UINT8_T, NUM_SEQ_PATTERN, "chance", 0,100,100}; uint8_t pat_chance[NUM_SEQ_PATTERN];
    Param pat_vel_variation_desc{P_UINT8_T, NUM_SEQ_PATTERN, "vel_variation", 0,100,0}; uint8_t pat_vel_variation[NUM_SEQ_PATTERN];
    Param p_end{P_END};
   //    uint8_t drum_midi_channel

    bool stop_screensaver = false;
    bool quicknav_song_to_pattern_jump = false;
    bool quicknav_pattern_to_song_jump = false;
    bool euclidean_state[16];
    bool DAC_mute_state = false;
    bool play_mode; // false = song, true = current pattern only
    bool hunt_pattern = false;
    uint8_t active_function = 99;
    uint16_t wave_spacing = 2;
    uint8_t active_multisample;    // multisample currently selected in editor
    uint8_t auto_advance_step = 0; // 0 = single step, 1 = auto advance, 2 = auto advance and auto stop
    bool step_recording = false;
    uint8_t current_track_type_of_active_pattern;
    bool track_mute[NUM_SEQ_TRACKS];
    uint8_t mute_mode = 0;
    const uint8_t piano[48] = { 0,1,0,1,0,0,1,0,1,0,1,0,0,1,0,1,0,0,1,0,1,0,1,0,0,1,0,1,0,0,1,0,1,0,1,0,0,1,0,1,0,0,1,0,1,0,1,0 };
    const uint8_t piano2[13] = { 1, 1, 0, 1, 1, 1, 0, 1, 1, 0, 1, 1, 1 };
    bool midi_learn_active = false;
    bool help_text_needs_refresh = false;
    uint8_t tracktype_or_instrument_assign;
    uint8_t loop_edit_step;
    uint8_t loop_start = 99;
    uint8_t loop_end = 99;
    uint8_t ticks;
    uint8_t cycle_touch_element = 0; // 0 = editor, 1 = touch keyboard, 5-9 = song/chain/transpose-functions
    uint8_t scrollpos;
    int cursor_scroll;
    int cursor_scroll_previous;
    uint8_t selected_track;
    uint8_t previous_track;
    bool edit_state;
    float drums_volume;
    uint8_t menu_status; // 0= normal jump
    // 1= jump from pattern edit to vel edit
    // 2= jump from vel edit to pattern edit
    // 3= ...
    uint8_t active_pattern = 0;
    uint8_t active_track = 0;
    uint8_t pianoroll_octave = 0;
    uint8_t pianoroll_last_valid_note = 254;
    uint8_t menu;
    uint8_t sub_menu;
    uint8_t UI_last_seq_step;
    uint8_t note_editor_view = 99; // 0 = list/tracker view, 1 = pianoroll
    bool noteoffsent[NUM_SEQ_TRACKS] = { false, false, false, false, false, false, false, false };
    uint8_t step = 0;
    //uint8_t total_played_patterns=0;//MIDI SLAVE SYNC TEST
    bool running = false;
    bool recording = false;
    bool smartfilter = false;
    uint8_t state_last_loadsave = 200;
    char name_temp[FILENAME_LEN];
    uint8_t note_in;
    uint8_t note_in_velocity;

    const uint8_t arps[6][23] = {
        {0, 4, 7, 12, 16, 19, 24, 28, 31, 36, 40, 43, 48, 52, 55, 60, 64, 67, 72, 76, 79, 84, 0}, // major
        {0, 3, 7, 12, 15, 19, 24, 27, 31, 36, 39, 43, 48, 51, 55, 60, 63, 67, 72, 75, 79, 84, 0}, // minor
        {0, 4, 7, 10, 12, 16, 19, 22, 24, 28, 31, 34, 36, 40, 43, 46, 48, 52, 55, 58, 60, 64, 0}, // seventh
        {0, 4, 8, 12, 16, 20, 24, 28, 32, 36, 40, 44, 48, 52, 56, 60, 64, 68, 72, 76, 80, 84, 0}, // augmented
        {0, 3, 6, 12, 15, 18, 24, 27, 30, 36, 39, 42, 48, 51, 54, 60, 63, 66, 72, 75, 78, 84, 0}, // dim
        {0, 4, 7, 11, 12, 16, 19, 23, 24, 28, 31, 35, 36, 40, 43, 47, 48, 52, 55, 59, 60, 64, 0}  // maj7
    };

    const char chord_names[7][7] = {
         "Major  ",
         "Minor  ",
         "seven  ",
         "aug    ",
         "dim    ",
         "Maj7   ",
         "NoChord" };

    const char arp_style_names[4][3] = {
        "UP ",
        "DWN",
        "U&D",
        "RND" };
    uint8_t temp_select_menu;
    uint8_t temp_active_menu = 99;

    int8_t current_song_step = 0;
    int8_t current_chain[NUM_SEQ_TRACKS] = { 99, 99, 99, 99, 99, 99, 99, 99 };
    int8_t current_pattern[NUM_SEQ_TRACKS] = { 99, 99, 99, 99, 99, 99, 99, 99 };
    int8_t chain_counter[NUM_SEQ_TRACKS] = { 0, 0, 0, 0, 0, 0, 0, 0 };

    uint8_t prev_note[NUM_SEQ_TRACKS]; // note_offs for every (instr.) track
    uint8_t prev_vel[NUM_SEQ_TRACKS];
    uint8_t arp_step;
    uint8_t arp_note;
    uint8_t arp_chord = 6;
    uint8_t arp_note_prev;
    uint8_t arp_octave;
    uint8_t arp_prev_oct;
    uint8_t arp_counter = 0;
    uint8_t arp_num_notes_max = 22;
    uint8_t arp_num_notes_count = 0;
    float arp_volume_base = 50;
    uint8_t data_buffer[16] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };

    Param note_data_desc{P_UINT8_T, NUM_SEQ_PATTERN * 16, "seq_data", 0, 255, 0};
    uint8_t note_data[NUM_SEQ_PATTERN][16];

    Param vel_desc{P_UINT8_T, NUM_SEQ_PATTERN * 16, "seq_velocity", 0, 255, 0};
    uint8_t vel[NUM_SEQ_PATTERN][16];

    Param song_desc{P_UINT8_T, NUM_SEQ_TRACKS * SONG_LENGTH, "s", 0, 99, 99};
    uint8_t song[NUM_SEQ_TRACKS][SONG_LENGTH];

    Param chain_desc{P_UINT8_T, NUM_CHAINS * 16, "c", 0, 99, 99};
    uint8_t chain[NUM_CHAINS][16];

    Param chain_transpose_desc{P_UINT8_T, NUM_CHAINS * 16, "t", 0, 99, 99};
    uint8_t chain_transpose[NUM_CHAINS][16];
} sequencer_t;

#endif
