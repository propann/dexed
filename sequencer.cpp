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
#include "ILI9341_t3n.h"

extern ILI9341_t3n display;
extern LCDMenuLib2 LCDML;

extern config_t configuration;
extern uint8_t drum_midi_channel;
extern uint8_t activesample;
extern uint8_t get_sample_note(uint8_t sample);
extern void handleNoteOn(byte , byte , byte, byte );
extern void handleNoteOff(byte , byte , byte, byte );
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
extern elapsedMillis midi_decay_timer_dexed;
extern elapsedMillis midi_decay_timer_microsynth;
extern int8_t midi_decay_dexed[NUM_DEXED];
#ifdef USE_MICROSYNTH
extern int8_t midi_decay_microsynth[NUM_MICROSYNTH];
#endif
extern void UI_func_microsynth(uint8_t param);

#ifdef USE_MICROSYNTH
extern AudioSynthWaveform       microsynth_waveform[NUM_MICROSYNTH];
extern AudioEffectEnvelope      microsynth_envelope_osc[NUM_MICROSYNTH];
extern AudioSynthNoisePink      microsynth_noise[NUM_MICROSYNTH];
extern AudioEffectEnvelope      microsynth_envelope_noise[NUM_MICROSYNTH];
extern AudioFilterStateVariable microsynth_filter_osc[NUM_MICROSYNTH];
extern AudioFilterStateVariable microsynth_filter_noise[NUM_MICROSYNTH];
extern elapsedMillis microsynth_lfo_delay_timer[2];
#endif



#ifdef USE_SEQUENCER
sequencer_t seq;
#endif

#ifdef USE_MICROSYNTH
microsynth_t microsynth[2];
#endif

#ifdef USE_BRAIDS
extern braids_t braids_osc;
//extern braids_filter_state_t* braids_filter_state[NUM_BRAIDS];
extern uint16_t braids_filter_state[NUM_BRAIDS];
#endif

#ifdef USE_BRAIDS
extern AudioEffectEnvelope*            braids_envelope[NUM_BRAIDS];
extern AudioFilterStateVariable*       braids_filter[NUM_BRAIDS];
#endif

extern uint16_t COLOR_SYSTEXT;
extern uint16_t COLOR_SYSTEXT_ACCENT;
extern uint16_t COLOR_BACKGROUND;
extern uint16_t COLOR_INSTR;
extern uint16_t COLOR_CHORDS;
extern uint16_t COLOR_ARP;
extern uint16_t COLOR_DRUMS;
extern uint16_t COLOR_PITCHSMP;
extern void setCursor_textGrid_small(uint8_t pos_x, uint8_t pos_y);

multisample_zone_t msz[NUM_MULTISAMPLES][NUM_MULTISAMPLE_ZONES];
multisample_s ms[NUM_MULTISAMPLES];

void seq_live_recording(void)
{
  //record to sequencer if sequencer menu is active and recording is active
  if (seq.note_in > 0 && seq.recording == true  && LCDML.FUNC_getID() == LCDML.OTHER_getIDFromFunction(UI_func_seq_pattern_editor))
  {
    seq.note_data[seq.active_pattern][seq.step] = seq.note_in;
    if ( seq.content_type[seq.active_pattern] == 0 &&  get_sample_note(activesample) > 209 ) // pitched sample
    {
      seq.vel[seq.active_pattern][seq.step] = get_sample_note(activesample);
    }
    else
      seq.vel[seq.active_pattern][seq.step] = seq.note_in_velocity;

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
    for (uint8_t s = SONG_LENGTH - 1; s > 0; s--) //search song length from back to front
    {
      if (seq.song[t][s] == 99)
        stepcounter++;
      else
        break;
    }
    if (stepcounter < best)
      best = stepcounter;
  }
  return SONG_LENGTH - best;
}

uint8_t get_chain_length_from_current_track(uint8_t tr)
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
  seq_live_recording();
  for (uint8_t d = 0; d < NUM_SEQ_TRACKS; d++)
  {
    int tr[NUM_SEQ_TRACKS] = {0, 0, 0, 0, 0, 0};
    seq.current_chain[d] = seq.song[d][seq.current_song_step];
    seq.current_pattern[d] = seq.chain[  seq.current_chain[d] ] [ seq.chain_counter[d] ];

    if (seq.chain_transpose[ seq.current_chain[d]][seq.chain_counter[d]] < NUM_CHAINS )
      tr[d] = seq.chain_transpose[ seq.current_chain[d]][seq.chain_counter[d]];
    else if (seq.chain_transpose[ seq.current_chain[d]][seq.chain_counter[d]] < NUM_CHAINS * 2 )
      tr[d] = NUM_CHAINS - (seq.chain_transpose[ seq.current_chain[d]][seq.chain_counter[d]]);  // number above NUM_CHAINS is used as negative value to save space

    if ( seq.current_pattern[d] < NUM_SEQ_PATTERN  && seq.current_chain[d] != 99 && !seq.track_mute[d])  // sequence not empty or muted
    {
      if ( seq.track_type[d] == 0  && seq.ticks == 0 )
      { // drum track (drum samples and pitched one-shot samples)
#if NUM_DRUMS > 0
        if (seq.note_data[  seq.current_pattern[d] ][seq.step] > 0 )
        {
          if (seq.vel[  seq.current_pattern[d] ][seq.step] > 209)  // it is a pitched sample
          {
            // Drum[slot]->setPlaybackRate( pow (2, (inNote - 72) / 12.00) * drum_config[sample].pitch ); get_sample_vol_max(sample)
            set_sample_pitch(seq.vel[  seq.current_pattern[d] ][seq.step] - 210 , (float)pow (2, (seq.note_data[  seq.current_pattern[d] ][seq.step] - 72 + tr[d] ) / 12.00) * get_sample_p_offset( seq.vel[  seq.current_pattern[d] ][seq.step] - 210 ) );
            handleNoteOn(drum_midi_channel, seq.vel[  seq.current_pattern[d] ][seq.step] , 90, 0 );
          }
          else // else play normal drum sample
            handleNoteOn(drum_midi_channel, seq.note_data[  seq.current_pattern[d] ][seq.step] , seq.vel[  seq.current_pattern[d] ][seq.step], 0);
        }
#endif
      }
      else {
        if (seq.note_data[ seq.current_pattern[d]][seq.step] > 0 )
        {
          if (seq.track_type[d] == 1) // instrument track
          {
            if (seq.note_data[ seq.current_pattern[d]][seq.step] != 130 && seq.ticks == 0)
            {
              //Braids Instrument
              if (seq.instrument[d] ==5) 
              {
                handleNoteOn(seq.instrument[d], seq.note_data[ seq.current_pattern[d]][seq.step] + tr[d], seq.vel[  seq.current_pattern[d] ][seq.step], 4);
              }

              //Multisampler Instrument
              if (seq.instrument[d] > 5 && seq.instrument[d] < 16) // track is for internal MultiSampler
              {
                handleNoteOn(seq.instrument[d] - 6, seq.note_data[ seq.current_pattern[d]][seq.step] + tr[d], seq.vel[  seq.current_pattern[d] ][seq.step], 3);
              }
              else if (seq.instrument[d] > 15 && seq.instrument[d] < 32) // track is for external USB MIDI
              {
#ifdef MIDI_DEVICE_USB_HOST
                handleNoteOn(seq.instrument[d] - 15, seq.note_data[ seq.current_pattern[d]][seq.step] + tr[d], seq.vel[  seq.current_pattern[d] ][seq.step], 1);
#endif
              }
              else if (seq.instrument[d] > 31 && seq.instrument[d] < 48) // track is for external DIN MIDI
              {
#ifdef MIDI_DEVICE_DIN
                handleNoteOn(seq.instrument[d] - 31, seq.note_data[ seq.current_pattern[d]][seq.step] + tr[d], seq.vel[  seq.current_pattern[d] ][seq.step], 2);
#endif
              }
              else if (seq.instrument[d] < 2) // track is assigned to dexed
                handleNoteOn(configuration.dexed[seq.instrument[d]].midi_channel, seq.note_data[  seq.current_pattern[d] ][seq.step] + tr[d]  , seq.vel[  seq.current_pattern[d] ][seq.step], 0);
              else if (seq.instrument[d] == 2) // track is assigned to epiano
                handleNoteOn(configuration.epiano.midi_channel, seq.note_data[  seq.current_pattern[d] ][seq.step] + tr[d]  , seq.vel[  seq.current_pattern[d] ][seq.step], 0);
#ifdef USE_MICROSYNTH
              else if (seq.instrument[d] == 3 || seq.instrument[d] == 4) // track is assigned for Microsynth
              {
                if (seq.note_data[seq.current_pattern[d] ][seq.step] == MIDI_C8)  // is noise only, do not transpose note
                  handleNoteOn( microsynth[ seq.instrument[d] - 3 ].midi_channel, seq.note_data[  seq.current_pattern[d] ][seq.step]   , microsynth[ seq.instrument[d] - 3 ].sound_intensity, 0 );
                else  //allow transpose
                  handleNoteOn( microsynth[ seq.instrument[d] - 3 ].midi_channel, seq.note_data[  seq.current_pattern[d] ][seq.step] + tr[d] , microsynth[ seq.instrument[d] - 3 ].sound_intensity, 0 );
              }
#endif

              if (seq.note_data[seq.current_pattern[d] ][seq.step] == MIDI_C8)  // is noise only, do not transpose note
                seq.prev_note[d] = seq.note_data[  seq.current_pattern[d] ][seq.step];
              else
                seq.prev_note[d] = seq.note_data[  seq.current_pattern[d] ][seq.step] + tr[d];

              seq.prev_vel[d] = seq.vel[  seq.current_pattern[d] ][seq.step];
            }
          }
          else if (seq.track_type[d] == 2 && seq.ticks == 0 ) //Chords
          {
            if (seq.vel[  seq.current_pattern[d]][seq.step] > 199)
            {
              for (uint8_t x = seq.element_shift; x < seq.element_shift + seq.chord_key_ammount; x++) //play chord notes
              {
                if (seq.instrument[d] < 2) // track is assigned to dexed
                  handleNoteOn(configuration.dexed[seq.chord_dexed_inst].midi_channel, seq.note_data[  seq.current_pattern[d] ][seq.step] + tr[d]   + (seq.oct_shift * 12) + seq.arps[seq.vel[  seq.current_pattern[d] ][seq.step] - 200][x], seq.chord_vel, 0);
                else if (seq.instrument[d] == 2) // track is assigned to epiano
                  handleNoteOn(configuration.epiano.midi_channel,  seq.note_data[  seq.current_pattern[d] ][seq.step] + tr[d] + (seq.oct_shift * 12) + seq.arps[seq.vel[  seq.current_pattern[d] ][seq.step] - 200][x], seq.chord_vel, 0);
#ifdef MIDI_DEVICE_USB_HOST
                else  if (seq.instrument[d] > 15 && seq.instrument[d] < 32) // track is for external USB MIDI
                {
                  handleNoteOn(seq.instrument[d] - 15, seq.note_data[  seq.current_pattern[d] ][seq.step] + tr[d] + (seq.oct_shift * 12) + seq.arps[seq.vel[  seq.current_pattern[d] ][seq.step] - 200][x], seq.chord_vel, 1);
                }
#endif
#ifdef MIDI_DEVICE_DIN
                else if (seq.instrument[d] > 31 && seq.instrument[d] < 48) // track is for external DIN MIDI
                {
                  handleNoteOn(seq.instrument[d] - 29, seq.note_data[  seq.current_pattern[d] ][seq.step] + tr[d] + (seq.oct_shift * 12) + seq.arps[seq.vel[  seq.current_pattern[d] ][seq.step] - 200][x], seq.chord_vel, 2);
                }
#endif
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
#ifdef USE_MICROSYNTH
            if (seq.instrument[d] == 3 || seq.instrument[d] == 4) // track is assigned to Microsynth
            {
              seq.arp_volume_base = microsynth[ seq.instrument[d] - 3 ].sound_intensity;
            }
#endif
            seq.arp_note = seq.note_data[  seq.current_pattern[d] ][seq.step] + tr[d]   + (seq.oct_shift * 12);
            seq.arp_chord = seq.vel[ seq.current_pattern[d] ][seq.step] - 200;
          }
        }
        // after here not triggered by a key input -  arp only
        if (seq.track_type[d] == 3 && seq.arp_num_notes_count < seq.arp_num_notes_max)
        { //Arp
          if (  (seq.arp_speed == 0 && seq.ticks == 0)  ||  (seq.arp_speed == 1 && seq.arp_counter == 0 && seq.ticks == 0) ||
                (seq.arp_speed == 2 && seq.ticks == 0) || (seq.arp_speed == 2 && seq.ticks == 2) || (seq.arp_speed == 2 && seq.ticks == 4) || (seq.arp_speed == 2 && seq.ticks == 6) ||
                seq.arp_speed == 3)
          {
            if (seq.arp_style == 0)
            { //arp up

              if (seq.instrument[d] == 3 || seq.instrument[d] == 4) // track is assigned to Microsynth
              {
#ifdef USE_MICROSYNTH
                handleNoteOn( microsynth[ seq.instrument[d] - 3 ].midi_channel,  seq.arp_note + seq.arps[seq.arp_chord][seq.arp_step + seq.element_shift] , seq.arp_volume_base - (seq.arp_num_notes_count * (seq.arp_volume_base / seq.arp_num_notes_max)) , 0);
                seq.arp_note_prev = seq.arp_note + seq.arps[seq.arp_chord][seq.arp_step + seq.element_shift];
                if (seq.arp_speed > 1)
                {
                  seq.arp_step++;
                  if (seq.arp_step >= seq.arp_length)
                    seq.arp_step = 0;
                }
#endif
              }
              else  if (seq.instrument[d] < 2) // track is assigned to dexed
              {
                handleNoteOn(configuration.dexed[seq.chord_dexed_inst].midi_channel, seq.arp_note + seq.arps[seq.arp_chord][seq.arp_step + seq.element_shift], seq.chord_vel, 0);
              }
              else if (seq.instrument[d] == 2) // track is assigned for epiano
                handleNoteOn(configuration.epiano.midi_channel, seq.arp_note + seq.arps[seq.arp_chord][seq.arp_step + seq.element_shift], seq.chord_vel , 0);
#ifdef MIDI_DEVICE_USB_HOST
              else  if (seq.instrument[d] > 15 && seq.instrument[d] < 32) // track is for external USB MIDI
              {
                handleNoteOn(seq.instrument[d] - 15, seq.arp_note + seq.arps[seq.arp_chord][seq.arp_step + seq.element_shift], seq.chord_vel, 1);
              }
#endif
#ifdef MIDI_DEVICE_DIN
              else if (seq.instrument[d] > 31 && seq.instrument[d] < 48) // track is for external DIN MIDI
              {
                handleNoteOn(seq.instrument[d] - 29, seq.arp_note + seq.arps[seq.arp_chord][seq.arp_step + seq.element_shift], seq.chord_vel, 2);

              }
#endif
              seq.arp_note_prev = seq.arp_note + seq.arps[seq.arp_chord][seq.arp_step + seq.element_shift] ;
            }
            else if (seq.arp_style == 1)
            { //arp down
              if (seq.instrument[d] == 3 || seq.instrument[d] == 4) // track is assigned to Microsynth
              {
#ifdef USE_MICROSYNTH
                handleNoteOn(microsynth[seq.instrument[d] - 3].midi_channel, seq.arp_note + seq.arps[seq.arp_chord][seq.arp_length - seq.arp_step + seq.element_shift], seq.arp_volume_base - (seq.arp_num_notes_count * (seq.arp_volume_base / seq.arp_num_notes_max)), 0 );

                if (seq.arp_speed > 1)
                {
                  seq.arp_step++;
                  if (seq.arp_step >= seq.arp_length)
                    seq.arp_step = 0;
                }
#endif
              }
              else if (seq.instrument[d] < 2) // track is assigned to dexed
              {
                handleNoteOn(configuration.dexed[seq.chord_dexed_inst].midi_channel, seq.arp_note + seq.arps[seq.arp_chord][seq.arp_length - seq.arp_step + seq.element_shift], seq.chord_vel, 0);
              }
              else if (seq.instrument[d] == 2) // track is assigned for epiano
                handleNoteOn(configuration.epiano.midi_channel, seq.arp_note + seq.arps[seq.arp_chord][seq.arp_length - seq.arp_step + seq.element_shift], seq.chord_vel, 0);
#ifdef MIDI_DEVICE_USB_HOST
              else  if (seq.instrument[d] > 15 && seq.instrument[d] < 32) // track is for external USB MIDI
              {

                handleNoteOn(seq.instrument[d] - 15, seq.arp_note + seq.arps[seq.arp_chord][seq.arp_length - seq.arp_step + seq.element_shift], seq.chord_vel, 1);
              }
#endif
#ifdef MIDI_DEVICE_DIN
              else if (seq.instrument[d] > 31 && seq.instrument[d] < 48) // track is for external DIN MIDI
              {
                handleNoteOn(seq.instrument[d] - 29, seq.arp_note + seq.arps[seq.arp_chord][seq.arp_length - seq.arp_step + seq.element_shift], seq.chord_vel, 2);
              }
#endif
              seq.arp_note_prev = seq.arp_note + seq.arps[seq.arp_chord][seq.arp_length - seq.arp_step + seq.element_shift] ;
            }
            else if (seq.arp_style == 2)
            { //arp up & down

              if (seq.arp_step <= seq.arp_length)
              {
                if (seq.instrument[d] == 3 || seq.instrument[d] == 4) // track is assigned to Microsynth
                {
#ifdef USE_MICROSYNTH
                  handleNoteOn( microsynth[ seq.instrument[d] - 3 ].midi_channel, seq.arp_note + seq.arps[seq.arp_chord][seq.arp_step ] , 90, 0);

                  if (seq.arp_speed > 1)
                  {
                    seq.arp_step++;
                    if (seq.arp_step >= seq.arp_length)
                      seq.arp_step = 0;
                  }
#endif
                }
                else if (seq.instrument[d] < 2) // track is assigned to dexed
                  handleNoteOn(configuration.dexed[seq.chord_dexed_inst].midi_channel, seq.arp_note + seq.arps[seq.arp_chord][seq.arp_step ], seq.chord_vel, 0);
                else if (seq.instrument[d] == 2) // track is assigned to epiano
                  handleNoteOn(configuration.epiano.midi_channel, seq.arp_note + seq.arps[seq.arp_chord][seq.arp_step ], seq.chord_vel, 0);
#ifdef MIDI_DEVICE_USB_HOST
                else  if (seq.instrument[d] > 15 && seq.instrument[d] < 32) // track is for external USB MIDI
                {
                  handleNoteOn(seq.instrument[d] - 15, seq.arp_note + seq.arps[seq.arp_chord][seq.arp_step ], seq.chord_vel, 1);
                }
#endif
#ifdef MIDI_DEVICE_DIN
                else if (seq.instrument[d] > 31 && seq.instrument[d] < 48) // track is for external DIN MIDI
                {
                  handleNoteOn(seq.instrument[d] - 29, seq.arp_note + seq.arps[seq.arp_chord][seq.arp_step ], seq.chord_vel, 2);
                }
#endif
                seq.arp_note_prev = seq.arp_note + seq.arps[seq.arp_chord][seq.arp_step ] ;
              }
              else
              {
                if (seq.instrument[d] == 3 || seq.instrument[d] == 4 ) // track is assigned to Microsynth
                {
#ifdef USE_MICROSYNTH
                  handleNoteOn( microsynth[ seq.instrument[d] - 3 ].midi_channel,   seq.arp_note + seq.arps[seq.arp_chord][seq.arp_length * 2 - seq.arp_step ] , 90, 0);

                  if (seq.arp_speed > 1)
                  {
                    seq.arp_step++;
                    if (seq.arp_step >= seq.arp_length)
                      seq.arp_step = 0;
                  }
#endif
                }
                else if (seq.instrument[d] < 2) // track is assigned to dexed
                  handleNoteOn(configuration.dexed[seq.chord_dexed_inst].midi_channel, seq.arp_note + seq.arps[seq.arp_chord][seq.arp_length * 2 - seq.arp_step ], seq.chord_vel, 0);
                else if (seq.instrument[d] == 2) // track is assigned to epiano
                  handleNoteOn(configuration.epiano.midi_channel, seq.arp_note + seq.arps[seq.arp_chord][seq.arp_length * 2 - seq.arp_step ], seq.chord_vel, 0);
#ifdef MIDI_DEVICE_USB_HOST
                else  if (seq.instrument[d] > 15 && seq.instrument[d] < 32) // track is for external USB MIDI
                {

                  handleNoteOn(seq.instrument[d] - 15, seq.arp_note + seq.arps[seq.arp_chord][seq.arp_length * 2 - seq.arp_step ], seq.chord_vel, 1);
                }
#endif
#ifdef MIDI_DEVICE_DIN
                else if (seq.instrument[d] > 31 && seq.instrument[d] < 48) // track is for external DIN MIDI
                {
                  handleNoteOn(seq.instrument[d] - 29, seq.arp_note + seq.arps[seq.arp_chord][seq.arp_length * 2 - seq.arp_step ], seq.chord_vel, 2);
                }
#endif
                seq.arp_note_prev = seq.arp_note + seq.arps[seq.arp_chord][seq.arp_length * 2 - seq.arp_step ] ;
              }
            }
            else if (seq.arp_style == 3)
            { //arp random
              uint8_t rnd1 = random(seq.arp_length);
              if (seq.instrument[d] == 3 || seq.instrument[d] == 4) // track is assigned to Microsynth
              {
#ifdef USE_MICROSYNTH
                handleNoteOn( microsynth[ seq.instrument[d] - 3 ].midi_channel,  seq.arp_note + seq.arps[seq.arp_chord][rnd1 + seq.element_shift] + (seq.oct_shift * 12) , 90, 0);

                if (seq.arp_speed > 1)
                {
                  seq.arp_step++;
                  if (seq.arp_step >= seq.arp_length)
                    seq.arp_step = 0;
                }
#endif
              }
              else if (seq.instrument[d] < 2) // track is assigned to dexed
                handleNoteOn(configuration.dexed[seq.chord_dexed_inst].midi_channel, seq.arp_note + seq.arps[seq.arp_chord][rnd1 + seq.element_shift] + (seq.oct_shift * 12), seq.chord_vel , 0 );
              else if (seq.instrument[d] == 2) // track is assigned to epiano
                handleNoteOn(configuration.epiano.midi_channel, seq.arp_note + seq.arps[seq.arp_chord][rnd1 + seq.element_shift] + (seq.oct_shift * 12), seq.chord_vel, 0);
#ifdef MIDI_DEVICE_USB_HOST
              else  if (seq.instrument[d] > 15 && seq.instrument[d] < 32) // track is for external USB MIDI
              {

                handleNoteOn(seq.instrument[d] - 15, seq.arp_note + seq.arps[seq.arp_chord][rnd1 + seq.element_shift] + (seq.oct_shift * 12), seq.chord_vel, 1);
              }
#endif
#ifdef MIDI_DEVICE_DIN
              else if (seq.instrument[d] > 31 && seq.instrument[d] < 48) // track is for external DIN MIDI
              {
                handleNoteOn(seq.instrument[d] - 29, seq.arp_note + seq.arps[seq.arp_chord][rnd1 + seq.element_shift] + (seq.oct_shift * 12), seq.chord_vel, 2);
              }
#endif
              seq.arp_note_prev = seq.arp_note + seq.arps[seq.arp_chord][rnd1 + seq.element_shift] + (seq.oct_shift * 12);
            }
            seq.arp_num_notes_count++;
          }
        }
      }
    }
    seq.noteoffsent[d] = false;
  }
  if (seq.ticks == 0)
  {
    seq.arp_counter++;
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

    if (seq.arp_style != 2) {

      if ( (seq.arp_step > 1 && seq.arps[seq.arp_chord][seq.arp_step] == 0) || seq.arp_step == seq.arp_length)
      {
        seq.arp_step = 0;
      }
    }
    if (seq.arp_style == 1 || seq.arp_style == 2  )
    {
      if (seq.arp_length == 0)seq.arp_length = 9;
    }
    if ( seq.arp_style == 2  )  //only for up&down
    {
      if ( (seq.arp_step > 1 && seq.arps[seq.arp_chord][seq.arp_step] == 0) || seq.arp_step == seq.arp_length * 2)
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
        if (  get_chain_length_from_current_track(d) >  seq.chain_counter[d]  )
          seq.chain_counter[d]++;

        if (get_chain_length_from_current_track(d) > 0 && get_chain_length_from_current_track(d) == seq.chain_counter[d] &&
            seq.chain_counter[d] < find_longest_chain())
          seq.chain_counter[d] = 0;

        if (seq.loop_end == 99) // no loop set
        {
          if (seq.current_song_step >= get_song_length() )
          {
            seq.current_song_step = 0;
            seq.chain_counter[d] = 0;
          }
        }
        else
        {
          if (seq.loop_start == seq.loop_end && seq.current_song_step > seq.loop_start) //loop only a single step
          {
            seq.current_song_step = seq.loop_start;
            seq.chain_counter[d] = 0;
          }

          // if loop is on and changed during playback and new loop values are lower than current playing range, get back into the new loop range
          else  if (seq.loop_start > seq.loop_end && seq.current_song_step == seq.loop_start + 1) //start is higher than end - > loop around
          {
            seq.current_song_step = 0;
            seq.chain_counter[d] = 0;
          }
          else if (seq.loop_start > seq.loop_end && seq.current_song_step == seq.loop_end + 1 && seq.loop_start != seq.loop_end + 1) //end is lower than start, jump to start at end
          {
            seq.current_song_step = seq.loop_start;
            seq.chain_counter[d] = 0;
          }
          else if (seq.loop_start < seq.loop_end && seq.current_song_step >= seq.loop_end + 1  ) //normal case, loop from end to start
          {
            seq.current_song_step = seq.loop_start;
            seq.chain_counter[d] = 0;
          }

          else if (seq.current_song_step >= get_song_length()  && seq.current_song_step > seq.loop_start && seq.current_song_step > seq.loop_end)
          {
            seq.current_song_step = 0;
            seq.chain_counter[d] = 0;
          }
        }
        if ( seq.chain_counter[d]  == find_longest_chain() && songstep_increased == false )
        {
          seq.current_song_step++;
          for (uint8_t z = 0; z < NUM_SEQ_TRACKS; z++)
          {
            seq.chain_counter[z] = 0;
          }
          songstep_increased = true;
        }
        if (songstep_increased == true )
          seq.chain_counter[d] = 0;
      }
    }
  }
}

void sequencer_part2(void)
{
  //seq_live_recording();

  for (uint8_t d = 0; d < NUM_SEQ_TRACKS; d++)
  {
    if (seq.noteoffsent[d] == false  )
    {
      if ( seq.prev_note[d] > 0 && seq.track_type[d] > 0  )
      {
        if (seq.note_data[  seq.current_pattern[d] ][seq.step] != 130 )
        {
          if (seq.instrument[d] < 2 && seq.ticks == 7)  //dexed
            handleNoteOff(configuration.dexed[seq.instrument[d]].midi_channel, seq.prev_note[d] , 0, 0);
          else if (seq.instrument[d] == 2 && seq.ticks == 7)  //epiano
            handleNoteOff(configuration.epiano.midi_channel, seq.prev_note[d] , 0, 0);
#ifdef USE_MICROSYNTH
          else if (seq.instrument[d] > 2 && seq.instrument[d] < 5)
            handleNoteOff( microsynth[ seq.instrument[d] - 3].midi_channel, seq.prev_note[d], 0, 0);
#endif
#ifdef USE_BRAIDS
          else if (seq.instrument[d] ==5 && seq.ticks == 7)
            handleNoteOff( seq.instrument[d], seq.prev_note[d] , 0, 4);
#endif
#ifdef MIDI_DEVICE_USB_HOST
          else if (seq.instrument[d] > 15 && seq.instrument[d] < 32 && seq.ticks == 7) // track is for external USB MIDI
          {
            handleNoteOff(seq.instrument[d] - 14, seq.prev_note[d] , 0, 1);
          }
#endif
#ifdef MIDI_DEVICE_DIN
          else if (seq.instrument[d] > 31 && seq.instrument[d] < 48 && seq.ticks == 7) // track is for external DIN MIDI
          {
            handleNoteOff(seq.instrument[d] - 31, seq.prev_note[d] , 0, 2);
          }
#endif
          seq.noteoffsent[d] = true;
        }
        if (seq.track_type[d] == 2 && seq.ticks == 7) //Chords
        {
          if ( seq.prev_vel[d] > 199)
          {
            for (uint8_t x = seq.element_shift; x < seq.element_shift + seq.chord_key_ammount; x++) //play chord notes
            {
              if (seq.instrument[d] < 2) //dexed
                handleNoteOff(configuration.dexed[seq.chord_dexed_inst].midi_channel, seq.prev_note[d] + seq.arps[seq.prev_vel[d] - 200][x], 0, 0);
              else if (seq.instrument[d] == 2)
                handleNoteOff(configuration.epiano.midi_channel, seq.prev_note[d] + seq.arps[seq.prev_vel[d] - 200][x], 0, 0);
#ifdef MIDI_DEVICE_USB_HOST
              else if (seq.instrument[d] > 15 && seq.instrument[d] < 32) // track is for external USB MIDI
              {

                handleNoteOff(seq.instrument[d] - 14, seq.prev_note[d] + seq.arps[seq.prev_vel[d] - 200][x], 0, 1);
              }
#endif
#ifdef MIDI_DEVICE_DIN
              else if (seq.instrument[d] > 31 && seq.instrument[d] < 48) // track is for external DIN MIDI
              {
                handleNoteOff(seq.instrument[d] - 31, seq.prev_note[d] + seq.arps[seq.prev_vel[d] - 200][x], 0, 2);
              }
#endif
              seq.noteoffsent[d] = true;
            }
          }
        }
        if (seq.track_type[d] == 3  )
        { //Arp
          if (seq.instrument[d] < 2 && seq.ticks == 7) //dexed
            handleNoteOff(configuration.dexed[seq.chord_dexed_inst].midi_channel, seq.arp_note_prev, 0, 0);
          else if (seq.instrument[d] == 2 && seq.ticks == 7)  //epiano
            handleNoteOff(configuration.epiano.midi_channel, seq.arp_note_prev, 0, 0);
#ifdef MIDI_DEVICE_USB_HOST
          else if (seq.instrument[d] > 15 && seq.instrument[d] < 32) // track is for external USB MIDI
          {

            handleNoteOff(seq.instrument[d] - 14, seq.arp_note_prev, 0, 1);
          }
#endif
#ifdef MIDI_DEVICE_DIN
          else if (seq.instrument[d] > 31 && seq.instrument[d] < 48) // track is for external DIN MIDI
          {
            handleNoteOff(seq.instrument[d] - 31, seq.arp_note_prev, 0, 2);
          }
#endif
#ifdef USE_MICROSYNTH
          else if (seq.instrument[d] > 2  ) // track is assigned to Microsynth
            handleNoteOff( microsynth[ seq.instrument[d] - 3].midi_channel, seq.arp_note_prev, 0, 0);
#endif
          seq.noteoffsent[d] = true;
        }
      }
    }
  }
}

void update_microsynth_params()
{
#ifdef USE_MICROSYNTH
  for (uint8_t d = 0; d < 2; d++)
  {
    if ( microsynth_envelope_osc[d].isActive() )  //pwm down
    {
      if  (microsynth[d].pwm_from > microsynth[d].pwm_to)
      {
        if (microsynth[d].pwm_current > microsynth[d].pwm_to)
        {
          if (microsynth[d].pwm_current - microsynth[d].pwm_speed >= 0)
            microsynth[d].pwm_current = microsynth[d].pwm_current - microsynth[d].pwm_speed;
          else
            microsynth[d].pwm_current = 0;
          microsynth_waveform[d].pulseWidth(microsynth[d].pwm_current / 2000.1);
        }
      } else
      {
        if (microsynth[d].pwm_current < microsynth[d].pwm_to)  //pwm up
        {
          if (microsynth[d].pwm_current + microsynth[d].pwm_speed <= 2000)
            microsynth[d].pwm_current = microsynth[d].pwm_current + microsynth[d].pwm_speed;
          else
            microsynth[d].pwm_current = 2000;
          microsynth_waveform[d].pulseWidth(microsynth[d].pwm_current / 2000.1);
        }
      }
      if  (microsynth[d].filter_osc_freq_from > microsynth[d].filter_osc_freq_to && microsynth[d].filter_osc_speed != 0)
      {
        if (microsynth[d].filter_osc_freq_current > microsynth[d].filter_osc_freq_to)  //osc filter down
        {
          if (int(microsynth[d].filter_osc_freq_current / float((1.01 + (microsynth[d].filter_osc_speed * 0.001)))) >= 0)
            microsynth[d].filter_osc_freq_current = int(microsynth[d].filter_osc_freq_current / float((1.01 + (microsynth[d].filter_osc_speed * 0.001))));
          else
            microsynth[d].filter_osc_freq_current = 0;

          microsynth_filter_osc[d].frequency(microsynth[d].filter_osc_freq_current);
        }
      } else
      {
        if (microsynth[d].filter_osc_freq_current < microsynth[d].filter_osc_freq_to && microsynth[d].filter_osc_speed != 0)
        { //osc filter up
          if (microsynth[d].filter_osc_freq_current + microsynth[d].filter_osc_speed <= 15000)
            microsynth[d].filter_osc_freq_current = microsynth[d].filter_osc_freq_current + microsynth[d].filter_osc_speed;
          else
            microsynth[d].filter_osc_freq_current = 15000;
          microsynth_filter_osc[d].frequency(microsynth[d].filter_osc_freq_current);
        }
      }

      if  (microsynth[d].filter_noise_freq_from > microsynth[d].filter_noise_freq_to && microsynth[d].filter_noise_speed != 0)
      {
        if (microsynth[d].filter_noise_freq_current > microsynth[d].filter_noise_freq_to)
        {
          if (int(microsynth[d].filter_noise_freq_current / float((1.01 + (microsynth[d].filter_noise_speed * 0.001)))) >= 0)
            microsynth[d].filter_noise_freq_current = int(microsynth[d].filter_noise_freq_current / float((1.01 + (microsynth[d].filter_noise_speed * 0.001))));
          else
            microsynth[d].filter_noise_freq_current = 0;
          microsynth_filter_noise[d].frequency(microsynth[d].filter_noise_freq_current);
        }
      } else
      {
        if (microsynth[d].filter_noise_freq_current < microsynth[d].filter_noise_freq_to && microsynth[d].filter_noise_speed != 0)
        {
          if (microsynth[d].filter_noise_freq_current + microsynth[d].filter_noise_speed <= 15000)
            microsynth[d].filter_noise_freq_current = microsynth[d].filter_noise_freq_current + microsynth[d].filter_noise_speed;
          else
            microsynth[d].filter_noise_freq_current = 15000;
          microsynth_filter_noise[d].frequency(microsynth[d].filter_noise_freq_current);
        }
      }  //  --------------------------------------------------------- OSC LFO ----------------------------------------------------------------------

      if (microsynth[d].lfo_speed > 0 && microsynth[d].lfo_mode == 0 )  // LFO U&D
      {
        if ( microsynth[d].lfo_direction == false && microsynth[d].lfo_value > microsynth[d].lfo_intensity * -1)
          microsynth[d].lfo_value = microsynth[d].lfo_value - microsynth[d].lfo_speed;

        else if ( microsynth[d].lfo_direction == true && microsynth[d].lfo_value < microsynth[d].lfo_intensity )
          microsynth[d].lfo_value = microsynth[d].lfo_value + microsynth[d].lfo_speed;

        if ( microsynth[d].lfo_value <= microsynth[d].lfo_intensity * -1)  //switch mode 0 LFO direction
          microsynth[d].lfo_direction = !microsynth[d].lfo_direction;
        else if (microsynth[d].lfo_mode == 0  && microsynth[d].lfo_value >= microsynth[d].lfo_intensity )
          microsynth[d].lfo_direction = !microsynth[d].lfo_direction;
      }
      else if (microsynth[d].lfo_speed > 0 && microsynth[d].lfo_mode == 1 )  // LFO Up
      {
        if (  microsynth[d].lfo_value < microsynth[d].lfo_intensity * 10 )
          microsynth[d].lfo_value = microsynth[d].lfo_value + microsynth[d].lfo_speed;
      }
      else if (microsynth[d].lfo_speed > 0 && microsynth[d].lfo_mode == 2 )  // LFO Down
      {
        if ( microsynth[d].lfo_value > microsynth[d].lfo_intensity * -10)
          microsynth[d].lfo_value = microsynth[d].lfo_value - microsynth[d].lfo_speed;
      }

      //--------------------------------------------------------------------------- LFO FADE/DELAY ---------------------------------------------------------

      if (microsynth[d].lfo_delay == 0 )  // no delay, instant lfo mod
      {
        microsynth_waveform[d].frequency(microsynth[d].osc_freq_current + microsynth[d].lfo_value / 10);
      }
      else if ( (int)microsynth_lfo_delay_timer[d] / 10 > microsynth[d].lfo_delay && microsynth[d].lfo_fade == 0 ) //init lfo fade in
      {
        microsynth[d].lfo_fade = microsynth[d].lfo_delay;
      }
      if (microsynth[d].lfo_fade > 0   ) //fade in to max lfo intensity
      {
        if (microsynth[d].osc_freq_current + ( (microsynth[d].lfo_value / 10) / (1 + float(microsynth[d].lfo_fade / 10) )  ) > 20 &&
            microsynth[d].osc_freq_current + ( (microsynth[d].lfo_value / 10) / (1 + float(microsynth[d].lfo_fade / 10) )  ) < 10000)
          microsynth_waveform[d].frequency(microsynth[d].osc_freq_current + ( (microsynth[d].lfo_value / 10) / (1 + float(microsynth[d].lfo_fade / 10) )  )  );
        if (microsynth[d].lfo_fade > 1) //only count down to multiplier of 1, not 0
          microsynth[d].lfo_fade = microsynth[d].lfo_fade - 1;
      }
    }
  }
#endif
}

void update_braids_params()
{
#ifdef USE_BRAIDS
  for (uint8_t d = 0; d < NUM_BRAIDS; d++)
  {
    if ( braids_envelope[d]->isActive() )
    {

      if  (braids_osc.filter_freq_from > braids_osc.filter_freq_to && braids_osc.filter_speed != 0)
      {
        if (braids_filter_state[d] > braids_osc.filter_freq_to)  //osc filter down
        {
          if (int(braids_filter_state[d] / float((1.01 + (braids_osc.filter_speed * 0.001)))) >= 0)
            braids_filter_state[d] = int(braids_filter_state[d] / float((1.01 + (braids_osc.filter_speed * 0.001))));
          else
            braids_filter_state[d] = 0;

          braids_filter[d]->frequency(braids_filter_state[d]);
        }
      } 
      else
      {
        if (braids_filter_state[d] < braids_osc.filter_freq_to && braids_osc.filter_speed != 0)
        { //osc filter up
          if (braids_filter_state[d] + braids_osc.filter_speed <= 15000)
            braids_filter_state[d] = braids_filter_state[d] + braids_osc.filter_speed;
          else
            braids_filter_state[d] = 15000;
          braids_filter[d]->frequency(braids_filter_state[d]);
        }
      }
    }
  }
#endif
}

void sequencer(void)
{ // Runs in Interrupt Timer. Switches between the Noteon and Noteoff Task, each cycle

  interrupt_swapper = !interrupt_swapper;

  if (seq.running)
  {
    if (seq.ticks < 4)
      seq_live_recording();

    if (interrupt_swapper)
      sequencer_part1();
    else sequencer_part2();

    seq.ticks++;
    if (seq.ticks > 7)
      seq.ticks = 0;
  }

}

void set_pattern_content_type_color(uint8_t pattern)
{
  if (seq.content_type[pattern] == 0) //Drumpattern
    display.setTextColor(COLOR_DRUMS, COLOR_BACKGROUND);
  else if (seq.content_type[pattern] == 1) //Instrument Pattern
    display.setTextColor(COLOR_INSTR, COLOR_BACKGROUND);
  else if (seq.content_type[pattern] == 2 ) //  chord  pattern
    display.setTextColor(COLOR_CHORDS, COLOR_BACKGROUND);
  else if ( seq.content_type[pattern] == 3) //   arp pattern
    display.setTextColor(COLOR_ARP, COLOR_BACKGROUND);
  else
    display.setTextColor(GREY2, COLOR_BACKGROUND);
}

int get_pattern_content_type_color(uint8_t pattern)
{
  int col = 0;

  if (seq.content_type[pattern] == 0) //Drumpattern
    col = COLOR_DRUMS;
  else if (seq.content_type[pattern] == 1) //Instrument Pattern
    col = COLOR_INSTR;
  else
    col = COLOR_CHORDS;
  return col;
}

void seq_print_formatted_number (uint16_t number, uint8_t length)
{
  if (length == 4)
  {
    if (number < 10)
      display.print("0");
    if (number < 100)
      display.print("0");
    if (number < 1000)
      display.print("0");
    display.print(number);
  }
  else if (length == 3)
  {
    if (number < 10)
      display.print("0");
    if (number < 100)
      display.print("0");
    display.print(number);
  }
  else
    //if not 3 then length defaults to 2
  {
    if (number < 10)
      display.print("0");
    display.print(number);
  }
}

void seq_print_formatted_number_signed (int number, uint8_t length)
{

  if (number > -1)
  {
    display.print("+");

    if (length == 3)
    {
      if (number < 10)
        display.print("0");
      if (number < 100)
        display.print("0");
      display.print(number);
    }
    else
      //if not 3 then length defaults to 2
    {
      if (number < 10)
        display.print("0");
      display.print(number);
    }
  }

  else
  {
    if (length == 3)
    {
      display.print("-");
      if (number > -10)
        display.print("0");
      if (number > -100)
        display.print("0");
      display.print(  abs( number));
    }
    else if (length == 2)
    {
      display.print("-");
      if (number > -10)
        display.print("0");
      display.print(  abs( number));
    }
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

//void seq_print_step_numbers(int xpos, int ypos)
//{
//  uint8_t buffer[35] = {10, 11, 12, 13, 14, 15, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12 };
//  uint8_t step = seq.step;
//  uint8_t yspacer = CHAR_height_small+2;
//  uint8_t count = 0;
//  while ( count < 16 )
//  {
//    display.setCursor(xpos, ypos + count * yspacer);
//    if (count == 6 )
//      display.setTextColor(COLOR_SYSTEXT, COLOR_BACKGROUND);
//    else
//      display.setTextColor(GREEN, COLOR_BACKGROUND);
//    //seq_print_formatted_number (buffer[step + count] , 2);
//    count++;
//  }
//}

void update_keyboard_current_step ( int ypos, uint8_t octave, uint8_t current_step)
{
  // draw grid
  for (uint8_t y = 0; y < 34; y++)
  {
    display.fillRect(34 + current_step * 7 , ypos + 6 - CHAR_height  - (y * 8.15 ), 5, 6, COLOR_SYSTEXT); // active step
    if (current_step > 0)
    {
      if (seq.piano[y] == 0 ) // is a white key
        display.fillRect(34 - 7 + current_step * 7 , ypos + 6 - CHAR_height  - (y * 8.15 ), 5, 6, GREY3); // GRID white key
      else
        display.fillRect(34 - 7 + current_step * 7 , ypos + 6 - CHAR_height  - (y * 8.15 ), 5, 6, GREY4); // GRID black key
    }
    else if (current_step == 0)
    {
      if (seq.piano[y] == 0 ) // is a white key
        display.fillRect(34  + 63 * 7 , ypos + 6 - CHAR_height  - (y * 8.15 ), 5, 6, GREY3); // GRID white key
      else
        display.fillRect(34  + 63 * 7 , ypos + 6 - CHAR_height  - (y * 8.15 ), 5, 6, GREY4); // GRID black key
    }
  }
}

void print_keyboard ( int ypos, uint8_t octave)
{
  uint8_t offset[5] = {12, 12, 14, 12, 11 }; //+ is up
  int offcount = 0;
  uint8_t oct_count = 0;
  uint8_t patternspacer = 0;
  uint8_t barspacer = 0;
  display.setTextColor(COLOR_BACKGROUND, COLOR_SYSTEXT);
  display.setTextSize(1);

  //draw white keys
  for (uint8_t y = 0; y < 14; y++)
  {
    display.fillRect(0, ypos - CHAR_height - (y * 14 ), 30, 13, COLOR_SYSTEXT); // pianoroll white key
    if ( y == 0 || y == 7 || y == 14) {
      display.setCursor (17, ypos - 14 - (y * 14 )   );
      display.print("C");
      display.print(octave - 1 + oct_count);
      oct_count++;
    }
  }
  for (uint8_t y = 0; y < 23; y++)
  {
    if (seq.piano[y] == 1)
    {
      display.fillRect(0, ypos - (y * 8.15 ) - offset[offcount] , 12, 8, COLOR_BACKGROUND);  // BLACK key
      offcount++;
      if (offcount == 5)offcount = 0;
    }
  }
  // draw grid

  for (uint8_t y = 0; y < 24; y++)
  {
    patternspacer = 0;
    barspacer = 0;
    for (uint8_t x = 0; x < 64; x++)
    {
      if (seq.piano[y] == 0 ) // is a white key
        display.fillRect(40 + patternspacer + barspacer + x * 4 , ypos + 6 - CHAR_height - (y * 8.15 ), 3, 6, GREY3); // GRID white key
      else
        display.fillRect(40 + patternspacer + barspacer + x * 4 , ypos + 6 - CHAR_height - (y * 8.15 ), 3, 6, GREY4); // GRID black key
      if ( (x + 1) % 16 == 0)
        patternspacer = patternspacer + 2;
      if ( (x + 1) % 4 == 0)
        barspacer = barspacer + 1;
    }
  }
}

void update_pianoroll ()
{
  uint8_t notes[64];
  uint8_t lowest_note = 127;
  int notes_display_shift = 0;
  uint8_t last_valid_note = 254;
  uint8_t patternspacer = 0;
  uint8_t barspacer = 0;
  int8_t current_chain = 99;
  int8_t pattern[4] =  { 99, 99, 99, 99 };

  current_chain = seq.song[seq.active_track][0];  //so far only step 0 of chain is displayed

  for (uint8_t d = 0; d < 4; d++)
  {
    pattern[d] = seq.chain[  current_chain ] [ d ];
  }

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

  for (uint8_t xcount = 0; xcount < 64; xcount++)
  {
    if (notes[xcount] > 0)
    {
      if (notes[xcount] == 130)
      {
        if (seq.step * seq.chain_counter[seq.active_track] != xcount)
          display.fillRect ( 40 + patternspacer + barspacer + xcount * 4, DISPLAY_HEIGHT - CHAR_height - 10 - (8.15 * notes_display_shift ) - (8.15 * (last_valid_note - lowest_note) ) , 3, 5, COLOR_PITCHSMP  );
        else
          display.fillRect ( 40 + patternspacer + barspacer + xcount * 4, DISPLAY_HEIGHT - CHAR_height - 10 - (8.15 * notes_display_shift ) - (8.15 * (last_valid_note - lowest_note) ) , 3, 5, RED  );
      }
      else
      {
        if (seq.step * seq.chain_counter[seq.active_track] != xcount)
          display.fillRect  ( 40 + patternspacer + barspacer + xcount * 4, DISPLAY_HEIGHT - CHAR_height - 10 - (8.15 * notes_display_shift ) - (8.15 * (notes[xcount] - lowest_note) ) , 3, 5, GREY1  );
        else
          display.fillRect  ( 40 + patternspacer + barspacer + xcount * 4, DISPLAY_HEIGHT - CHAR_height - 10 - (8.15 * notes_display_shift ) - (8.15 * (notes[xcount] - lowest_note) ) , 3, 5, GREEN  );
        last_valid_note = notes[xcount];
      }
    }
    if ( (xcount + 1) % 16 == 0)
      patternspacer = patternspacer + 2;
    if ( (xcount + 1) % 4 == 0)
      barspacer = barspacer + 1;
  }
}

void print_merged_pattern_pianoroll (int xpos, int ypos, uint8_t track_number)
{
  uint8_t notes[64];
  uint8_t lowest_note = 127;
  int notes_display_shift = 0;
  uint8_t last_valid_note = 254;
  uint8_t patternspacer = 0;
  uint8_t barspacer = 0;

  //int8_t current_song_step=0;
  // int8_t chain_counter[NUM_SEQ_TRACKS] =  { 0,0,0,0,0,0 };
  int8_t current_chain = 99;
  int8_t pattern[4] =  { 99, 99, 99, 99 };

  current_chain = seq.song[track_number][0];  //so far only step 0 of chain is displayed

  for (uint8_t d = 0; d < 4; d++)
  {
    pattern[d] = seq.chain[  current_chain ] [ d ];
  }

  helptext_l("MOVE Y");
  helptext_r("MOVE X");

  display.setTextColor(COLOR_SYSTEXT, COLOR_CHORDS);
  display.setCursor (CHAR_width * 2, 0);

  display.print("[");
  display.print(0);
  display.print("]");

  display.setTextColor(COLOR_SYSTEXT, COLOR_PITCHSMP);

  display.print(F(" TRK:["));
  display.print(track_number + 1);
  display.print("] ");
  display.setTextColor(COLOR_SYSTEXT, COLOR_BACKGROUND);
  display.print(" ");
  display.setTextColor(COLOR_SYSTEXT, COLOR_PITCHSMP);
  display.print(F(" CHAIN: "));
  display.print(current_chain);
  display.print(F("  "));

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
        display.fillRect ( 40 + patternspacer + barspacer + xcount * 4, ypos - 10 - (8.15 * notes_display_shift ) - (8.15 * (last_valid_note - lowest_note) ) , 3, 5, COLOR_PITCHSMP  );
      }
      else
      {
        display.fillRect  ( 40 + patternspacer + barspacer + xcount * 4, ypos - 10 - (8.15 * notes_display_shift ) - (8.15 * (notes[xcount] - lowest_note) ) , 3, 5, GREY1  );
        last_valid_note = notes[xcount];
      }
    }
    if ( (xcount + 1) % 16 == 0)
      patternspacer = patternspacer + 2;
    if ( (xcount + 1) % 4 == 0)
      barspacer = barspacer + 1;
  }
}

void seq_print_current_note_from_step(uint8_t step)
{
  if (seq.note_data[seq.active_pattern][step] == 130) //it is a latched note
  {
    display.setTextColor(GREEN, COLOR_BACKGROUND);
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

void print_keyboard_small (int xpos, int ypos, uint8_t octave, uint8_t actstep, bool fullredraw)
{
  uint8_t offset[5] = {12, 12, 14, 12, 11 }; //+ is up
  int offcount = 0;
  uint8_t oct_count = 0;
  uint8_t to_step = 16;
  if (fullredraw || seq.pianoroll_octave != octave)
  {
    seq.pianoroll_octave = octave;
    display.setTextColor(COLOR_BACKGROUND, COLOR_SYSTEXT);
    //draw white keys
    for (uint8_t y = 0; y < 11; y++)
    {
      display.fillRect(xpos, ypos - CHAR_height - (y * 14 ), 30, 13, COLOR_SYSTEXT); // pianoroll white key
      if ( y == 0 || y == 7 || y == 14) {
        display.setCursor (xpos + 17, ypos - 14 - (y * 14 )   );
        display.print("C"); display.print(octave - 1 + oct_count);
        oct_count++;
      }
    }
    for (uint8_t y = 0; y < 20; y++) // draw BLACK keys
    {
      if (seq.piano[y] == 1)
      {
        display.fillRect(xpos, ypos - (y * 8.15 ) - offset[offcount] , 12, 8, COLOR_BACKGROUND);  // BLACK key
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
  for (uint8_t y = 0; y < 19; y++)
  {
    for (uint8_t x = 0; x < to_step; x++)
    {
      if (seq.piano[y] == 0 ) // is a white key
        display.fillRect(xpos + 36 + x * 10 , ypos + 6 - CHAR_height  - (y * 8.15 ), 5, 6, GREY3); // GRID white key
      else
        display.fillRect(xpos + 36 + x * 10 , ypos + 6 - CHAR_height  - (y * 8.15 ), 5, 6, GREY4); // GRID black key
    }
  }
}

void print_single_pattern_pianoroll_in_pattern_editor (int xpos, int ypos, uint8_t pattern,  uint8_t actstep, bool fullredraw)
{
  uint8_t lowest_note = 128;
  int notes_display_shift = 0;
  uint8_t last_valid_note = 254;
  uint8_t from_step = 0;
  uint8_t to_step = 16;
  display.setTextSize(1);
  display.setTextColor(COLOR_SYSTEXT, COLOR_BACKGROUND);
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
  display.setTextColor(COLOR_SYSTEXT);
  for (from_step = 0; from_step < to_step; from_step++)
  {
    // if ( (ypos - 10 - (8.15 * notes_display_shift )  - (8.15 * (seq.note_data[pattern][from_step] - lowest_note))) > 5 * CHAR_height + 10 )
    // {

    if ( seq.note_data[pattern][from_step] != 0 && seq.note_data[pattern][from_step]  != 130)
    {
      if (from_step == actstep)
        display.fillRect ( xpos + 36 + from_step * 10,  ypos - 10 - (8.15 * notes_display_shift )  - (8.15 * (seq.note_data[pattern][from_step] - lowest_note) ) , 5, 5, COLOR_SYSTEXT  );
      else
        display.fillRect ( xpos + 36 + from_step * 10,  ypos - 10 - (8.15 * notes_display_shift )  - (8.15 * (seq.note_data[pattern][from_step] - lowest_note) ) , 5, 5, COLOR_INSTR  );
      last_valid_note = seq.note_data[pattern][from_step];
    }
    else if ( seq.note_data[pattern][from_step]  == 130) //last valid note was latch
    {
      if (from_step == actstep)
        display.fillRect ( xpos + 36 + from_step * 10,  ypos - 10 - (8.15 * notes_display_shift )  - (8.15 * ( last_valid_note  - lowest_note) ) , 5, 5, COLOR_SYSTEXT  );
      else
        display.fillRect ( xpos + 36 + from_step * 10,  ypos - 10 - (8.15 * notes_display_shift )  - (8.15 * ( last_valid_note  - lowest_note) ) , 5, 5, GREEN  );
    }
    else if (  from_step == actstep)
    {
      // if (last_valid_note != 0)
      display.fillRect  ( xpos + 36 + from_step * 10,  ypos - 10 - (8.15 * notes_display_shift )  - (8.15 * (last_valid_note - lowest_note) ) , 5, 5, COLOR_SYSTEXT );
    }
    if (from_step < 15 )
    {
      if ( seq.note_data[pattern][from_step + 1] == 0)
      {
        if (seq.piano[last_valid_note % 12 - lowest_note] == 0 ) // is a white key
          display.fillRect  ( xpos + 36 + (from_step + 1) * 10,  ypos - 10 - (8.15 * notes_display_shift )  - (8.15 * (last_valid_note - lowest_note) ) , 5, 5, GREY3); // GRID white key
        else
          display.fillRect  ( xpos + 36 + (from_step + 1) * 10,  ypos - 10 - (8.15 * notes_display_shift )  - (8.15 * (last_valid_note - lowest_note) ) , 5, 5, GREY4); // GRID black key
      }
    }
  }
  display.setTextSize(2);
  display.setTextColor(COLOR_SYSTEXT, COLOR_BACKGROUND);
}
