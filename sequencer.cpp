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

#include "config.h"
#include "sequencer.h"
#include <LCDMenuLib2.h>
#include "synth_dexed.h"

extern ILI9486_Teensy display;
extern LCDMenuLib2 LCDML;
extern config_t configuration;
extern uint8_t drum_midi_channel;
extern uint8_t activesample;
extern uint8_t get_sample_note(uint8_t sample);
extern void handleNoteOn(byte , byte , byte );
extern void handleNoteOff(byte , byte , byte );
extern void UI_func_seq_pattern_editor(uint8_t);
extern void UI_func_arpeggio(uint8_t);
extern const char* seq_find_shortname(uint8_t);

extern const char* seq_find_shortname_in_track(uint8_t sstep, uint8_t track);

extern void set_sample_pitch (uint8_t, float);  //float32_t not working
extern float get_sample_vol_max(uint8_t);
extern float get_sample_p_offset(uint8_t);
boolean interrupt_swapper = false;
extern void helptext_l (const char *str);
extern void helptext_r (const char *str);
extern AudioSynthDexed*  MicroDexed[NUM_DEXED];

const char noteNames[12][3] = { "C", "C#", "D", "D#", "E", "F", "F#", "G", "G#", "A", "A#", "B" };
sequencer_t seq;

void seq_live_recording(void)
{
  //record to sequencer if sequencer menu is active and recording is active
  if (seq.note_in > 0 && seq.recording == true  && LCDML.FUNC_getID() == LCDML.OTHER_getIDFromFunction(UI_func_seq_pattern_editor))
  {
    seq.note_data[seq.active_track][seq.step] = seq.note_in;
    if ( get_sample_note(activesample) > 209 ) // pitched sample
    {
      seq.vel[seq.active_track][seq.step] = get_sample_note(activesample);
    }
    else
      seq.vel[seq.active_track][seq.step] = seq.note_in_velocity;

    seq.note_in = 0;
    seq.note_in_velocity = 0;
  }
}

uint8_t get_song_length()
{
  uint8_t best = 254;
  uint8_t stepcounter = 0;

  for (uint8_t t = 0; t < NUM_SEQ_TRACKS; t++)  //loop tracks
  {
    stepcounter = 0;
    for (uint8_t s = SONG_LENGHT - 1; s > 0; s--) //search song lenght from back to front
    {
      if (seq.song[t][s] == 99)
        stepcounter++;
    }
    if (stepcounter < best)
      best = stepcounter;
  }
  return SONG_LENGHT - best;
}

uint8_t get_chain_length_from_track(uint8_t tr)
{
  uint8_t stepcounter = 0;

  for (uint8_t s = 0; s < 16; s++)
  {
    if ( seq.current_chain[tr] == 99)
      break;
    if ( seq.chain[  seq.current_chain[tr] ] [ s ] == 99)
      break;
    else
      stepcounter++;
  }

  return stepcounter;
}

uint8_t find_longest_chain()
{
  uint8_t longest = 0;
  uint8_t stepcounter = 0;

  for (uint8_t t = 0; t < NUM_SEQ_TRACKS; t++)  //loop tracks
  {
    stepcounter = 0;
    for (uint8_t c = 0; c < 16; c++)
    {
      if ( seq.current_chain[t] == 99)
        break;

      if ( seq.chain[  seq.current_chain[t] ] [ c ] == 99)
        break;
      else
        stepcounter++;
    }
    if (stepcounter > longest)
      longest = stepcounter;
  }
  return longest;
}

void sequencer_part1(void)
{

  for (uint8_t d = 0; d < NUM_SEQ_TRACKS; d++)
  {
    uint8_t tr[NUM_SEQ_TRACKS] = {0, 0, 0, 0, 0, 0};
    seq.current_chain[d] = seq.song[d][seq.current_song_step];
    seq.current_pattern[d] = seq.chain[  seq.current_chain[d] ] [ seq.chain_counter[d] ];

    if (seq.chain_transpose[ seq.current_chain[d]][seq.chain_counter[d]] < 32 )
      tr[d] = seq.chain_transpose[ seq.current_chain[d]][seq.chain_counter[d]];

    if ( seq.current_pattern[d] < NUM_SEQ_PATTERN  && seq.current_chain[d] != 99)  // sequence not empty or muted
    {
      if ( seq.track_type[d] == 0  && seq.ticks == 0 )
      { // drum track (drum samples and pitched one-shot samples)
        if (seq.note_data[  seq.current_pattern[d] ][seq.step] > 0 )
        {
          if (seq.vel[  seq.current_pattern[d] ][seq.step] > 209)  // it is a pitched sample
          {
            // Drum[slot]->setPlaybackRate( pow (2, (inNote - 72) / 12.00) * drum_config[sample].pitch ); get_sample_vol_max(sample)
            set_sample_pitch(seq.vel[  seq.current_pattern[d] ][seq.step] - 210 , (float)pow (2, (seq.note_data[  seq.current_pattern[d] ][seq.step] - 72) / 12.00) * get_sample_p_offset( seq.vel[  seq.current_pattern[d] ][seq.step] - 210 ) );
            handleNoteOn(drum_midi_channel, seq.vel[  seq.current_pattern[d] ][seq.step] , 90 );
          }
          else // else play normal drum sample
            handleNoteOn(drum_midi_channel, seq.note_data[  seq.current_pattern[d] ][seq.step] , seq.vel[  seq.current_pattern[d] ][seq.step]);
        }
      }
      else {
        if (seq.note_data[ seq.current_pattern[d]][seq.step] > 0 )  // instrument track
        {
          if (seq.track_type[d] == 1 || (seq.track_type[d] == 3 && seq.arp_play_basenote) )
          {
            if (seq.note_data[ seq.current_pattern[d]][seq.step] != 130 && seq.ticks == 0)
            {
              if (seq.inst_dexed[d] < 2) // track is assigned for dexed
                handleNoteOn(configuration.dexed[seq.inst_dexed[d]].midi_channel, seq.note_data[  seq.current_pattern[d] ][seq.step] + tr[d]  , seq.vel[  seq.current_pattern[d] ][seq.step]);
              else if (seq.inst_dexed[d] == 2) // track is assigned for epiano
                handleNoteOn(configuration.epiano.midi_channel, seq.note_data[  seq.current_pattern[d] ][seq.step] + tr[d]  , seq.vel[  seq.current_pattern[d] ][seq.step]);
              seq.prev_note[d] = seq.note_data[  seq.current_pattern[d] ][seq.step] + tr[d]  ;
              seq.prev_vel[d] = seq.vel[  seq.current_pattern[d] ][seq.step];
            }
          }
          else if (seq.track_type[d] == 2 && seq.ticks == 0 ) //Chords
          {
            if (seq.vel[  seq.current_pattern[d]][seq.step] > 199)
            {
              for (uint8_t x = seq.element_shift; x < seq.element_shift + seq.chord_key_ammount; x++) //play chord notes
              {
                if (seq.inst_dexed[d] < 2) // track is assigned for dexed
                  handleNoteOn(configuration.dexed[seq.chord_dexed_inst].midi_channel, seq.note_data[  seq.current_pattern[d] ][seq.step] + tr[d]   + (seq.oct_shift * 12) + seq.arps[seq.vel[  seq.current_pattern[d] ][seq.step] - 200][x], seq.chord_velocity);
                else if (seq.inst_dexed[d] == 2) // track is assigned for epiano
                  handleNoteOn(configuration.epiano.midi_channel,  seq.note_data[  seq.current_pattern[d] ][seq.step] + tr[d]   + (seq.oct_shift * 12) + seq.arps[seq.vel[  seq.current_pattern[d] ][seq.step] - 200][x], seq.chord_velocity);
              }
              seq.prev_note[d] = seq.note_data[ seq.current_pattern[d]][seq.step] + tr[d]   + (seq.oct_shift * 12);
              seq.prev_vel[d] = seq.vel[ seq.current_pattern[d]][seq.step];
            }
          }
          else if (seq.track_type[d] == 3 && seq.ticks == 0 ) //Arp
          { 
            seq.arp_step = 0;
            seq.arp_num_notes_count = 0;
            seq.arp_counter = 0;
            seq.arp_volume_fade = seq.chord_velocity;
            seq.arp_note = seq.note_data[  seq.current_pattern[d] ][seq.step] + tr[d]   + (seq.oct_shift * 12);
            seq.arp_chord = seq.vel[ seq.current_pattern[d] ][seq.step] - 200;
          }
        }
        // after here not triggered by a key input -  arp only
        if (seq.track_type[d] == 3 && seq.arp_num_notes_count < seq.arp_num_notes_max)
        { //Arp
          if (seq.arp_speed == 0 ||  seq.arp_speed == 2  || seq.arp_speed == 3  || seq.arp_speed == 1 && seq.arp_counter == 0)
          {
            if (seq.arp_style == 0)
            { //arp up
              if (seq.arp_speed == 2 || seq.arp_speed == 3)  // 1/32 + 1/64 arps, SID style
              {
                seq.arp_volume_fade = seq.chord_velocity -  ( (seq.chord_velocity / seq.arp_num_notes_max)  * seq.arp_num_notes_count );
                set_sample_pitch(6, (float)pow (2, (seq.arp_note + seq.arps[seq.arp_chord][seq.arp_step + seq.element_shift] - 72) / 12.00) * get_sample_p_offset( 6 ) );
                handleNoteOn(drum_midi_channel, 216 , seq.arp_volume_fade );
              }
              else  if (seq.inst_dexed[d] < 2) // track is assigned for dexed
              {
                //configuration.dexed[seq.chord_dexed_inst].sound_intensity=seq.arp_volume_fade;
                MicroDexed[seq.chord_dexed_inst]->setGain(seq.arp_volume_fade);
                handleNoteOn(configuration.dexed[seq.chord_dexed_inst].midi_channel, seq.arp_note + seq.arps[seq.arp_chord][seq.arp_step + seq.element_shift], seq.chord_velocity);
              }
              else if (seq.inst_dexed[d] == 2) // track is assigned for epiano
                handleNoteOn(configuration.epiano.midi_channel, seq.arp_note + seq.arps[seq.arp_chord][seq.arp_step + seq.element_shift], seq.chord_velocity );
              seq.arp_note_prev = seq.arp_note + seq.arps[seq.arp_chord][seq.arp_step + seq.element_shift] ;
            }
            else if (seq.arp_style == 1)
            { //arp down
              if (seq.inst_dexed[d] < 2) // track is assigned for dexed
              {
                //configuration.dexed[seq.chord_dexed_inst].sound_intensity=seq.arp_volume_fade;
                MicroDexed[seq.chord_dexed_inst]->setGain(seq.arp_volume_fade);
                handleNoteOn(configuration.dexed[seq.chord_dexed_inst].midi_channel, seq.arp_note + seq.arps[seq.arp_chord][seq.arp_lenght - seq.arp_step + seq.element_shift], seq.chord_velocity);
              }
              else if (seq.inst_dexed[d] == 2) // track is assigned for epiano
                handleNoteOn(configuration.epiano.midi_channel, seq.arp_note + seq.arps[seq.arp_chord][seq.arp_lenght - seq.arp_step + seq.element_shift], seq.chord_velocity);
              seq.arp_note_prev = seq.arp_note + seq.arps[seq.arp_chord][seq.arp_lenght - seq.arp_step + seq.element_shift] ;
            }
            else if (seq.arp_style == 2)
            { //arp up & down
              if (seq.arp_step <= seq.arp_lenght)
              {
                if (seq.inst_dexed[d] < 2) // track is assigned for dexed
                  handleNoteOn(configuration.dexed[seq.chord_dexed_inst].midi_channel, seq.arp_note + seq.arps[seq.arp_chord][seq.arp_step ], seq.chord_velocity);
                else if (seq.inst_dexed[d] == 2) // track is assigned for epiano
                  handleNoteOn(configuration.epiano.midi_channel, seq.arp_note + seq.arps[seq.arp_chord][seq.arp_step ], seq.chord_velocity);
                seq.arp_note_prev = seq.arp_note + seq.arps[seq.arp_chord][seq.arp_step ] ;
              }
              else
              {
                if (seq.inst_dexed[d] < 2) // track is assigned for dexed
                  handleNoteOn(configuration.dexed[seq.chord_dexed_inst].midi_channel, seq.arp_note + seq.arps[seq.arp_chord][seq.arp_lenght * 2 - seq.arp_step ], seq.chord_velocity);
                else if (seq.inst_dexed[d] == 2) // track is assigned for epiano
                  handleNoteOn(configuration.epiano.midi_channel, seq.arp_note + seq.arps[seq.arp_chord][seq.arp_lenght * 2 - seq.arp_step ], seq.chord_velocity);
                seq.arp_note_prev = seq.arp_note + seq.arps[seq.arp_chord][seq.arp_lenght * 2 - seq.arp_step ] ;
              }
            }
            else if (seq.arp_style == 3)
            { //arp random

              uint8_t rnd1 = random(seq.arp_lenght);
              if (seq.inst_dexed[d] < 2) // track is assigned for dexed
                handleNoteOn(configuration.dexed[seq.chord_dexed_inst].midi_channel, seq.arp_note + seq.arps[seq.arp_chord][rnd1 + seq.element_shift] + (seq.oct_shift * 12), seq.chord_velocity );
              else if (seq.inst_dexed[d] == 2) // track is assigned for epiano
                handleNoteOn(configuration.epiano.midi_channel, seq.arp_note + seq.arps[seq.arp_chord][rnd1 + seq.element_shift] + (seq.oct_shift * 12), seq.chord_velocity);
              seq.arp_note_prev = seq.arp_note + seq.arps[seq.arp_chord][rnd1 + seq.element_shift] + (seq.oct_shift * 12);
            }
            seq.arp_num_notes_count++;
          }

          // seq.arp_volume_fade = seq.chord_velocity -  ( (seq.chord_velocity / seq.arp_num_notes_max)  * seq.arp_num_notes_count );
        }
      }
    }
    seq.noteoffsent[d] = false;
  }

  seq.arp_counter++;

  if (seq.ticks == 0)
    seq.step++;

  if (seq.arp_speed == 0)  // Arp Speed 1/16
  {
    seq.arp_step++;
  }
  else if (seq.arp_speed == 1) // Arp Speed 1/8
  {
    if (seq.arp_counter > 1)  {
      seq.arp_counter = 0;
      seq.arp_step++;
    }
  }
  else if (seq.arp_speed == 2) // Arp Speed 1/32
  {
    ;
  }

  else if (seq.arp_speed == 3) // Arp Speed 1/64
  {
    ;
  }


  if (seq.arp_style != 2) {

    if ( (seq.arp_step > 1 && seq.arps[seq.arp_chord][seq.arp_step] == 0) || seq.arp_step == seq.arp_lenght)
    {
      seq.arp_step = 0;
    }
  }
  if (seq.arp_style == 1 || seq.arp_style == 2  )
  {
    if (seq.arp_lenght == 0)seq.arp_lenght = 9;
  }
  if ( seq.arp_style == 2  )  //only for up&down
  {
    if ( (seq.arp_step > 1 && seq.arps[seq.arp_chord][seq.arp_step] == 0) || seq.arp_step == seq.arp_lenght * 2)
    {
      seq.arp_step = 0;
    }
  }
  if (seq.step > 15)
  {
    seq.step = 0;

    bool songstep_increased = false;

    for (uint8_t d = 0; d < NUM_SEQ_TRACKS; d++)
    {
      if (  get_chain_length_from_track(d) >  seq.chain_counter[d]  )
        seq.chain_counter[d]++;

      if (get_chain_length_from_track(d) > 0 && get_chain_length_from_track(d) == seq.chain_counter[d] && seq.chain_counter[d] < find_longest_chain())
        seq.chain_counter[d] = 0;

      if (seq.current_song_step == get_song_length() )
      {
        seq.current_song_step = 0;
        seq.chain_counter[d] = 0;
      }
      if ( seq.chain_counter[d]  == find_longest_chain() && songstep_increased == false )
      {
        seq.current_song_step++;
        seq.chain_counter[d] = 0;
        songstep_increased = true;
      }
      if (songstep_increased == true )
        seq.chain_counter[d] = 0;
    }
  }
}

void sequencer_part2(void)
{

  seq_live_recording();

  for (uint8_t d = 0; d < NUM_SEQ_TRACKS; d++)
  {
    if (seq.noteoffsent[d] == false)
    {
      if ( seq.prev_note[d] > 0 && seq.track_type[d] > 0)
      {
        if (seq.note_data[  seq.current_pattern[d] ][seq.step] != 130)
        {
          if (seq.inst_dexed[d] < 2)
            handleNoteOff(configuration.dexed[seq.inst_dexed[d]].midi_channel, seq.prev_note[d] , 0);
          else if (seq.inst_dexed[d] == 2)
            handleNoteOff(configuration.epiano.midi_channel, seq.prev_note[d] , 0);
          seq.noteoffsent[d] = true;
        }
        if (seq.track_type[d] == 2) //Chords
        {
          if ( seq.prev_vel[d] > 199)
          {
            for (uint8_t x = seq.element_shift; x < seq.element_shift + seq.chord_key_ammount; x++) //play chord notes
            {
              if (seq.inst_dexed[d] < 2) //dexed
                handleNoteOff(configuration.dexed[seq.chord_dexed_inst].midi_channel, seq.prev_note[d] + seq.arps[seq.prev_vel[d] - 200][x], 0);
              else if (seq.inst_dexed[d] == 2)
                handleNoteOff(configuration.epiano.midi_channel, seq.prev_note[d] + seq.arps[seq.prev_vel[d] - 200][x], 0);
              seq.noteoffsent[d] = true;
            }
          }
        }
        else if (seq.track_type[d] == 3)
        { //Arp
          if (seq.inst_dexed[d] < 2) //dexed
            handleNoteOff(configuration.dexed[seq.chord_dexed_inst].midi_channel, seq.arp_note_prev, 0);
          else if (seq.inst_dexed[d] == 2)  //epiano
            handleNoteOff(configuration.epiano.midi_channel, seq.arp_note_prev, 0);
          seq.noteoffsent[d] = true;
        }
      }
    }
  }
}


void sequencer(void)
{ // Runs in Interrupt Timer. Switches between the Noteon and Noteoff Task, each cycle

  interrupt_swapper = !interrupt_swapper;

  if (interrupt_swapper) sequencer_part1();
  else sequencer_part2();

  seq.ticks++;
  if (seq.ticks > 7)
    seq.ticks = 0;
}

void reset_tracker_edit_cache_single_track()
{
  memset(seq.tracker_data_cache[seq.selected_track], 254, 16);
  memset(seq.tracker_names_cache[seq.selected_track], 254, 16);
}

void reset_tracker_edit_cache()
{
  memset(seq.tracker_data_cache, 254, sizeof(seq.tracker_data_cache));
  memset(seq.tracker_names_cache, 254, sizeof(seq.tracker_names_cache));
}

void reset_tracker_edit_cache_current_step()
{
  for (uint8_t t = 0; t < NUM_SEQ_TRACKS; t++)
  {
    seq.tracker_data_cache[t][seq.tracker_active_step] = 254;
    seq.tracker_names_cache[t][seq.tracker_active_step] = 254;
  }
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
  uint8_t yspacer = 18;
  uint8_t count = 0;
  while ( count < 12 )
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

  // SEQUENCER REWRITE

  //  if (seq.patternchain[0][track_number] < NUM_SEQ_PATTERN && seq.patternchain[1][track_number] < NUM_SEQ_PATTERN
  //      && seq.patternchain[2][track_number] < NUM_SEQ_PATTERN && seq.patternchain[3][track_number] < NUM_SEQ_PATTERN)
  //  {
  //    for (uint8_t f = 0; f < 16; f++)  // Fill array with complete data from all chain parts of track
  //    {
  //      notes[f] = seq.note_data[ seq.patternchain[0][track_number] ][f];
  //      notes[f + 16] = seq.note_data[ seq.patternchain[1][track_number] ][f];
  //      notes[f + 32] = seq.note_data[ seq.patternchain[2][track_number] ][f];
  //      notes[f + 48] = seq.note_data[ seq.patternchain[3][track_number] ][f];
  //    }
  //
  //
  //    //find lowest note
  //    for (uint8_t f = 0; f < 64; f++)
  //    {
  //      if (notes[f] < lowest_note && notes[f] > 0)
  //      {
  //        lowest_note = notes[f];
  //      }
  //    }
  //    if (lowest_note == 127)
  //      lowest_note = 24;
  //    notes_display_shift = lowest_note % 12;
  //    update_keyboard_current_step(ypos, lowest_note / 12 , cur_step);
  //    if (cur_step - 1 >= 0)
  //    {
  //      if (notes[cur_step - 1] > 0)
  //      {
  //        if (notes[cur_step - 1] == 130)
  //        {
  //          display.fillRect ( 34 + (cur_step - 1) * 7,  ypos - 10 - (8.15 * notes_display_shift )  - (8.15 * (seq.pianoroll_last_valid_note - lowest_note) ) , 5, 5, DX_CYAN  );
  //        }
  //        else
  //        {
  //          display.fillRect  ( 34 + (cur_step - 1) * 7,  ypos - 10 - (8.15 * notes_display_shift )  - (8.15 * (notes[cur_step - 1] - lowest_note) ) , 5, 5, WHITE  );
  //          seq.pianoroll_last_valid_note = notes[cur_step - 1];
  //        }
  //      }
  //    }
  //    else
  //    {
  //      if (notes[63] > 0)
  //      {
  //        if (notes[63] == 130)
  //        {
  //          display.fillRect ( 34 + (63) * 7,  ypos - 10 - (8.15 * notes_display_shift )  - (8.15 * (seq.pianoroll_last_valid_note - lowest_note) ) , 5, 5, DX_CYAN  );
  //        }
  //        else
  //        {
  //          display.fillRect  ( 34 + (63) * 7,  ypos - 10 - (8.15 * notes_display_shift )  - (8.15 * (notes[63] - lowest_note) ) , 5, 5, WHITE  );
  //          seq.pianoroll_last_valid_note = notes[63];
  //        }
  //      }
  //    }
  //  }
}

void print_merged_pattern_pianoroll (int xpos, int ypos, uint8_t track_number)
{
  uint8_t notes[64];
  uint8_t lowest_note = 127;
  int notes_display_shift = 0;
  uint8_t last_valid_note = 254;

  //int8_t current_song_step=0;
  // int8_t chain_counter[NUM_SEQ_TRACKS] =  { 0,0,0,0,0,0 };
  int8_t current_chain = 99;
  int8_t pattern[4] =  { 99, 99, 99, 99 };

  // SEQUENCER REWRITE

  current_chain = seq.song[track_number][0];  //so far only step 0 of chain is displayed

  for (uint8_t d = 0; d < 4; d++)
  {
    pattern[d] = seq.chain[  current_chain ] [ d ];
  }

  display.setTextSize(2);
  helptext_l("MOVE Y");
  helptext_r("MOVE X");

  display.setTextColor(WHITE, DX_MAGENTA);
  display.setCursor (CHAR_width * 2, 0);

  display.print("[");
  display.print(0);
  display.print("]");

  display.setTextColor(WHITE, BLUE);

  display.print(" TRK:[");
  display.print(track_number + 1);
  display.print("] ");
  display.setTextColor(WHITE, BLACK);
  display.print(" ");
  display.setTextColor(WHITE, BLUE);
  display.print(" CHAIN: ");
  display.print(current_chain);
  display.print("  ");

  seq_print_formatted_number( pattern[0] , 2);
  display.write (0xf8);
  seq_print_formatted_number( pattern[1] , 2);
  display.write (0xf8);
  seq_print_formatted_number( pattern[2] , 2);
  display.write (0xf8);
  seq_print_formatted_number( pattern[3]  , 2);
  display.print(" ");

  //    if (pattern[0] < NUM_SEQ_PATTERN && pattern[1] < NUM_SEQ_PATTERN
  //        && pattern[2] < NUM_SEQ_PATTERN && pattern[3] < NUM_SEQ_PATTERN)
  //    {

  for (uint8_t f = 0; f < 16; f++)  // Fill array with complete data from all chain parts of track
  {
    notes[f] = seq.note_data[ pattern[0] ][f];
    notes[f + 16] = seq.note_data[ pattern[1]][f];
    notes[f + 32] = seq.note_data[ pattern[2] ][f];
    notes[f + 48] = seq.note_data[ pattern[3] ][f];
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

  //  }

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

void print_merged_pattern_for_editor(int xpos, int ypos, uint8_t track_number)
{

  // SEQUENCER REWRITE

  //  uint8_t yspacer = 19;
  //  uint8_t ycount = 0;
  //  uint8_t yoffset = 0;
  //  uint8_t cstep = 0;
  //
  //  for (uint8_t y = seq.scrollpos; y < seq.scrollpos + 12; y++) {
  //
  //    if (y >= 0 && y < 16)
  //    {
  //      yoffset = 0;
  //      cstep = 0;
  //    }
  //    else if (y >= 16 && y < 32)
  //    {
  //      yoffset = 16;
  //      cstep = 1;
  //    }
  //    else if (y >= 32 && y < 48)
  //    {
  //      yoffset = 32;
  //      cstep = 2;
  //    }
  //    else if (y >= 48 && y < 64)
  //    {
  //      yoffset = 48;
  //      cstep = 3;
  //    }
  //
  //    display.setCursor(0, ypos + ycount * yspacer);
  //    if (y % 16 == 0)
  //      display.setTextColor(GREEN, BLACK);
  //    else
  //      display.setTextColor(DARKGREEN, BLACK);
  //
  //    if (seq.selected_track == 6 && y == seq.scrollpos + 6 + seq.cursor_scroll )
  //      display.setTextColor(WHITE, BLACK);
  //
  //
  //    seq_print_formatted_number(y, 2);
  //    set_pattern_content_type_color( seq.patternchain[cstep][track_number] );
  //
  //    if (y == seq.scrollpos)
  //    {
  //      display.setTextSize(1);
  //      display.setCursor_textGrid(7 + 6 * track_number, 4);
  //      display.setCursor(display.getCursorX(), display.getCursorY() + 9);
  //      set_pattern_content_type_color( seq.patternchain[cstep][track_number] );
  //      seq_print_formatted_number( seq.patternchain[cstep][track_number], 2);
  //      display.setTextSize(2);
  //    }
  //
  //    if (seq.selected_track == track_number && y == seq.scrollpos + 6 + seq.cursor_scroll)
  //    {
  //      display.setTextColor(WHITE, BLACK);
  //      seq.tracker_active_step = y - yoffset;
  //    }
  //    else
  //      set_pattern_content_type_color( seq.patternchain[cstep][track_number] );
  //
  //    if (seq.tracker_names_cache[track_number][ycount] != seq_find_shortname_in_track( y - yoffset , seq.patternchain[cstep][track_number] )[0])
  //    {
  //      display.setCursor(xpos, ypos + ycount * yspacer);
  //      display.print( seq_find_shortname_in_track( y - yoffset , seq.patternchain[cstep][track_number] )[0]);
  //      seq.tracker_names_cache[track_number][ycount] = seq_find_shortname_in_track( y - yoffset , seq.patternchain[cstep][track_number] )[0];
  //    }
  //
  //    if ( seq.tracker_data_cache[track_number][ycount] != seq.note_data[ seq.patternchain[cstep][track_number] ][y - yoffset] )
  //    {
  //      display.setCursor(xpos + 24, ypos + ycount * yspacer);
  //      seq_print_formatted_number(  seq.note_data[ seq.patternchain[cstep][track_number] ][y - yoffset] , 3);
  //      seq.tracker_data_cache[track_number][ycount] = seq.note_data[ seq.patternchain[cstep][track_number] ][y - yoffset];
  //    }
  //
  //    ycount++;
  //  }
  //

}

void print_merged_pattern_fast_play_only(int xpos, int ypos, uint8_t track_number)
{

  // SEQUENCER REWRITE

  //  char names[48];
  //  uint8_t data[48];
  //  uint8_t step = seq.step;
  //  uint8_t yspacer = 18;
  //  uint8_t next_chain_step = 0;
  //  uint8_t last_chain_step = 0;
  //  if (seq.chain_active_step == 0)
  //  {
  //    next_chain_step = 1;
  //    last_chain_step = 3;
  //  }
  //  else if (seq.chain_active_step == 1)
  //  {
  //    next_chain_step = 2;
  //    last_chain_step = 0;
  //  }
  //  else if (seq.chain_active_step == 2)
  //  {
  //    next_chain_step = 3;
  //    last_chain_step = 1;
  //  }
  //  else if (seq.chain_active_step == 3) {
  //    next_chain_step = 0;
  //    last_chain_step = 2;
  //  }
  //  display.setTextSize(1);
  //  for (uint8_t x = 0; x < NUM_SEQ_TRACKS; x++)
  //  {
  //    display.setCursor_textGrid(7 + 6 * x, 4);
  //    display.setCursor(display.getCursorX(), display.getCursorY() + 9);
  //    set_pattern_content_type_color( seq.patternchain[seq.chain_active_step][x] );
  //    seq_print_formatted_number( seq.patternchain[seq.chain_active_step][x], 2);
  //  }
  //  display.setTextSize(2);
  //  for (uint8_t f = 0; f < 16; f++)
  //  {
  //    names[f] = seq_find_shortname_in_track( f , seq.patternchain[last_chain_step][track_number] )[0];
  //    names[f + 16] = seq_find_shortname_in_track( f , seq.patternchain[seq.chain_active_step][track_number] )[0];
  //    names[f + 32] = seq_find_shortname_in_track( f , seq.patternchain[next_chain_step][track_number] )[0];
  //    data[f] = seq.note_data[ seq.patternchain[last_chain_step][track_number] ][f];
  //    data[f + 16] = seq.note_data[ seq.patternchain[seq.chain_active_step][track_number] ][f];
  //    data[f + 32] = seq.note_data[ seq.patternchain[next_chain_step][track_number] ][f];
  //  }
  //  for (uint8_t ycount = 0; ycount < 12; ycount++)
  //  {
  //    if ( ycount == 6)
  //      display.setTextColor(WHITE, BLACK);
  //    else
  //      set_pattern_content_type_color( seq.patternchain[seq.chain_active_step][track_number] );
  //    {
  //      if (seq.tracker_names_cache[ track_number ][ycount  ] != names[ycount + step + 10 ])
  //      {
  //        display.setCursor(xpos, ypos + ycount * yspacer);
  //        display.print( names[ycount + step + 10] );
  //        seq.tracker_names_cache[ track_number ][ycount  ] = names[ycount + step + 10 ];
  //      }
  //      if  (seq.tracker_data_cache[ track_number ][ycount  ] != data[ycount + step + 10] )
  //      {
  //        display.setCursor(xpos + 24, ypos + ycount * yspacer);
  //        seq_print_formatted_number( data[ycount + step + 10], 3);
  //        seq.tracker_data_cache[ track_number ][ycount  ] = data[ycount + step + 10 ];
  //      }
  //    }
  //  }
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
