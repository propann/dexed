
#include "config.h"
#include "touch.h"
#include "sequencer.h"
#include <LCDMenuLib2.h>
#include "ILI9341_t3n.h"


extern ILI9341_t3n display;
extern config_t configuration;
extern uint8_t selected_instance_id;
extern void handleNoteOn_MIDI_DEVICE_DIN(byte inChannel, byte inNumber, byte inVelocity);
extern void handleNoteOff_MIDI_DEVICE_DIN(byte inChannel, byte inNumber, byte inVelocity);
extern void print_voice_settings(int x, int y, uint8_t instance_id, bool fullrefresh);
extern void print_voice_settings_in_pattern_editor(int x, int y);
extern void UI_update_instance_icons();
extern LCDMenuLib2 LCDML;
extern sequencer_t seq;
extern void border3_large_clear();
extern void border3_large();
extern void flash_printDirectory();
extern void sd_printDirectory(File currentDirectory);
extern uint8_t find_longest_chain();
extern void seq_print_formatted_number(uint16_t v, uint8_t l);
extern void virtual_keyboard_print_buttons();
extern void draw_button_on_grid(uint8_t x, uint8_t y, const char *t1, const char *t2, uint8_t color);
extern void seq_pattern_editor_update_dynamic_elements();
extern void colors_screen_update();
extern void microsynth_refresh_lower_screen_static_text();
extern void microsynth_refresh_lower_screen_dynamic_text();
extern float get_sample_p_offset(uint8_t sample);
extern void set_sample_pitch(uint8_t sample, float playbackspeed);
extern void show_small_font(int pos_y, int pos_x, uint8_t field_size, const char *str);
extern const char* find_long_drum_name_from_note(uint8_t note);
extern void print_perfmod_buttons();
extern void print_perfmod_lables();
extern microsynth_t  microsynth[NUM_MICROSYNTH];
extern void print_empty_spaces (uint8_t spaces);
extern void print_voice_select_default_help();
extern void playWAVFile(const char *filename);

ts_t ts; //touch screen
fm_t fm; //file manager
dexed_live_mod_t dexed_live_mod; // dexed quick live modifiers for attack and release

extern uint16_t COLOR_BACKGROUND;
extern uint16_t COLOR_SYSTEXT;
extern uint16_t COLOR_SYSTEXT_ACCENT;
extern uint16_t COLOR_INSTR;
extern uint16_t COLOR_CHORDS;
extern uint16_t COLOR_ARP;
extern uint16_t COLOR_DRUMS;
extern uint16_t COLOR_PITCHSMP;

void helptext_l (const char *str)
{
  display.setTextSize(1);
  uint8_t l = strlen(str);
  display.setCursor (0, DISPLAY_HEIGHT - CHAR_height_small);
  display.setTextColor(COLOR_SYSTEXT, DX_DARKCYAN);
  display.print(str);

  if (l < ts.old_helptext_lenght[0])
  {
    display.setTextColor(COLOR_SYSTEXT, COLOR_BACKGROUND);
    for (uint8_t x = 0; x < ts.old_helptext_lenght[0] - l; x++)
      display.print(" ");
  }

  ts.old_helptext_lenght[0] = l;
}

void helptext_r (const char *str)
{
  display.setTextSize(1);
  uint8_t l = strlen(str);
  display.setCursor(DISPLAY_WIDTH - CHAR_width_small * l, DISPLAY_HEIGHT - CHAR_height_small);
  display.setTextColor(COLOR_SYSTEXT, DX_DARKCYAN);
  display.print(str);
  if (l < ts.old_helptext_lenght[1])
  {
    display.setCursor(DISPLAY_WIDTH - CHAR_width_small * (ts.old_helptext_lenght[1]), DISPLAY_HEIGHT - CHAR_height_small);
    display.setTextColor(COLOR_SYSTEXT, COLOR_BACKGROUND);
    for (uint8_t x = 0; x < ts.old_helptext_lenght[1] - l; x++)
      display.print(" ");
  }
  ts.old_helptext_lenght[1] = l;
}

void helptext_c (const char *str)
{
  display.setTextSize(1);
  uint8_t l = strlen(str);
  display.setCursor(DISPLAY_WIDTH / 2 - (l / 2)*CHAR_width_small,  DISPLAY_HEIGHT - CHAR_height_small  );
  display.setTextColor(COLOR_SYSTEXT, DX_DARKCYAN);
  display.print(str);
  if (l < ts.old_helptext_lenght[2])
  {
    display.setCursor(DISPLAY_WIDTH / 2 - (ts.old_helptext_lenght[2] / 2)*CHAR_width_small,  DISPLAY_HEIGHT - CHAR_height_small  );
    display.setTextColor(COLOR_SYSTEXT, COLOR_BACKGROUND);
    for (uint8_t x = 0; x < ts.old_helptext_lenght[2] - l; x++)
      display.print(" ");
  }
  ts.old_helptext_lenght[2] = l;
}

uint16_t RGB24toRGB565(uint8_t r, uint8_t g, uint8_t b)
{
  return ((r / 8) << 11) | ((g / 4) << 5) | (b / 8);
}

uint16_t ColorHSV(uint16_t hue, uint8_t sat, uint8_t val )
{
  // hue: 0-359, sat: 0-255, val (lightness): 0-255
  int r = 0, g = 0, b = 0, base;

  base = ((255 - sat) * val) >> 8;
  switch (hue / 60)
  {
    case 0:
      r = val;
      g = (((val - base) * hue) / 60) + base;
      b = base;
      break;
    case 1:
      r = (((val - base) * (60 - (hue % 60))) / 60) + base;
      g = val;
      b = base;
      break;
    case 2:
      r = base;
      g = val;
      b = (((val - base) * (hue % 60)) / 60) + base;
      break;
    case 3:
      r = base;
      g = (((val - base) * (60 - (hue % 60))) / 60) + base;
      b = val;
      break;
    case 4:
      r = (((val - base) * (hue % 60)) / 60) + base;
      g = base;
      b = val;
      break;
    case 5:
      r = val;
      g = base;
      b = (((val - base) * (60 - (hue % 60))) / 60) + base;
      break;
  }
  return  RGB24toRGB565( r, g,  b);
  //  }
}

void print_current_chord()
{
  for (uint8_t x = 0; x < 7; x++)
  {
    display.print( seq.chord_names[seq.arp_chord][x]);
  }
}

void virtual_keyboard_print_current_instrument()
{

  display.setTextColor(GREY2, COLOR_BACKGROUND);
  display.setTextSize(2);
  display.setCursor ( 17 * CHAR_width_small, 16 * CHAR_height_small );
  if (ts.virtual_keyboard_instrument < 7)
    display.print(F("PLAYING"));
  else
  {
    display.setTextSize(1);
    display.print(F("PITCHED SAMPLE "));
    display.fillRect ( 17 * CHAR_width_small, 17 * CHAR_height_small , 14 * CHAR_width_small, 8, COLOR_BACKGROUND );
  }
  display.setTextSize(2);
  display.setCursor ( 17 * CHAR_width_small, 18 * CHAR_height_small + 1);
  display.setTextColor(COLOR_SYSTEXT, COLOR_BACKGROUND);
  if (ts.virtual_keyboard_instrument == 1)
  {
    display.print(F("DEXED1  "));
    ts.virtual_keyboard_midi_channel = configuration.dexed[0].midi_channel;
  }
  else if (ts.virtual_keyboard_instrument == 2)
  {
    display.print(F("DEXED2  "));
    ts.virtual_keyboard_midi_channel = configuration.dexed[1].midi_channel;
  }
  else if (ts.virtual_keyboard_instrument == 3)
  {
    display.print(F("MSYNTH1 "));
    ts.virtual_keyboard_midi_channel = microsynth[0].midi_channel;
  }
  else if (ts.virtual_keyboard_instrument == 4)
  {
    display.print(F("MSYNTH2 "));
    ts.virtual_keyboard_midi_channel = microsynth[1].midi_channel;;
  }
  else if (ts.virtual_keyboard_instrument == 5)
  {
    display.print(F("EPIANO  "));
    ts.virtual_keyboard_midi_channel = configuration.epiano.midi_channel;
  }
  else if (ts.virtual_keyboard_instrument == 6)
  {
    display.print(F("DRUMS   "));
    ts.virtual_keyboard_midi_channel = DRUM_MIDI_CHANNEL;
  }
  else if (ts.virtual_keyboard_instrument > 6)
  {
    display.print(ts.virtual_keyboard_instrument - 7);
    display.print(" ");

    show_small_font( 18 * CHAR_height_small + 1, 20 * CHAR_width_small, 7,  find_long_drum_name_from_note( ts.virtual_keyboard_instrument - 7 + 210) );

    ts.virtual_keyboard_midi_channel = DRUM_MIDI_CHANNEL;
  }

}
void virtual_keyboard_key_on ()
{
  int offcount = 0;
  uint8_t halftones = 0;
  display.setTextColor(COLOR_SYSTEXT);
  display.setTextSize(1);
  //draw white keys
  if ( ts.p.y > VIRT_KEYB_YPOS + 42)
  {
    for (uint8_t x = 0; x < 10; x++)
    {
      if (ts.p.x > x * 32.22 + 10 && ts.p.x < x * 32.22 + 32 + 10 )
      {
        if (ts.virtual_keyboard_state_white[x] == 0)
        {
          ts.virtual_keyboard_state_white[x] = 254;
          for (uint8_t z = 0; z < x; z++)
          {
            if (seq.piano2[z] == 1)
              halftones = halftones + 1;
          }
          // handleNoteOn_MIDI_DEVICE_DIN(configuration.dexed[selected_instance_id].midi_channel, ts.virtual_keyboard_octave * 12 + x + halftones, 120);
          if (ts.virtual_keyboard_instrument > 6) //pitched samples
          {
            set_sample_pitch(ts.virtual_keyboard_instrument - 7, (float)pow(2, (ts.virtual_keyboard_octave * 12 + x + halftones - 72) / 12.00)*get_sample_p_offset(ts.virtual_keyboard_instrument - 7));
            handleNoteOn_MIDI_DEVICE_DIN(ts.virtual_keyboard_midi_channel, 210 + ts.virtual_keyboard_instrument - 7 , 100 );
          }
          else
            handleNoteOn_MIDI_DEVICE_DIN(ts.virtual_keyboard_midi_channel, ts.virtual_keyboard_octave * 12 + x + halftones, 120);
          display.fillRect( 1 + x * 32.22, VIRT_KEYB_YPOS + 34 , 29.33, 39, RED   ); // white key
        }
      }
    }
  }
  else if (ts.p.y > VIRT_KEYB_YPOS + 6 && ts.p.y < VIRT_KEYB_YPOS + 40)
  {
    for (uint8_t x = 0; x < 16; x++)
    {
      if (seq.piano[x] == 1)
      {
        if (ts.p.x > x * 18.46 + 12 && ts.p.x < x * 18.46 + 18 + 11)

          if (ts.virtual_keyboard_state_black[x] == 0)
          {
            ts.virtual_keyboard_state_black[x] = 254;
            if (ts.virtual_keyboard_instrument > 6) //pitched samples
            {
              set_sample_pitch(ts.virtual_keyboard_instrument - 7, (float)pow(2, (ts.virtual_keyboard_octave * 12 + x - 72) / 12.00)*get_sample_p_offset(ts.virtual_keyboard_instrument - 7));
              handleNoteOn_MIDI_DEVICE_DIN(ts.virtual_keyboard_midi_channel, 210 + ts.virtual_keyboard_instrument - 7 , 100 );
            }
            else
              handleNoteOn_MIDI_DEVICE_DIN(ts.virtual_keyboard_midi_channel, ts.virtual_keyboard_octave * 12 + x , 120);
            display.fillRect(x * 18.56 , VIRT_KEYB_YPOS , 21.33, 34.5, RED);  // BLACK key
            offcount++;
            if (offcount == 5)offcount = 0;
          }
      }
    }
  }
  display.setTextSize(2);
  display.setTextColor(COLOR_SYSTEXT, COLOR_BACKGROUND);
}

void virtual_keyboard_key_off_white ( uint8_t note)
{

  display.setTextColor(COLOR_BACKGROUND, COLOR_SYSTEXT);
  display.setTextSize(1);
  //draw white keys

  for (uint8_t x = 0; x < 10; x++)
  {
    if (x == note )
    {
      handleNoteOff_MIDI_DEVICE_DIN(ts.virtual_keyboard_midi_channel, ts.virtual_keyboard_octave * 12 + note, 120);
      display.fillRect( 1 + x * 32.22, VIRT_KEYB_YPOS + 34 , 29.33, 39, COLOR_SYSTEXT    ); // white key

      if ( x == 0 || x == 7 ) {
        display.setCursor (1 + x * 32.22 + 11.3 , VIRT_KEYB_YPOS + 57.75   );
        display.print("C");
        if ( x == 0)
          display.print(ts.virtual_keyboard_octave );
        else if (  x == 7 )
          display.print(ts.virtual_keyboard_octave + 1);
      }
    }
  }
  display.setTextSize(2);
  display.setTextColor(COLOR_SYSTEXT, COLOR_BACKGROUND);
}

void virtual_keyboard_key_off_black ( uint8_t note)
{
  display.setTextColor(COLOR_BACKGROUND, COLOR_SYSTEXT);
  display.setTextSize(1);

  for (uint8_t x = 0; x < 16; x++)
  {
    if (x == note )
    {
      handleNoteOff_MIDI_DEVICE_DIN(ts.virtual_keyboard_midi_channel, ts.virtual_keyboard_octave * 12 + note, 120);
      display.fillRect(x * 18.56 , VIRT_KEYB_YPOS , 21.33, 34.5, COLOR_BACKGROUND);  // BLACK key
    }
  }
  display.setTextSize(2);
  display.setTextColor(COLOR_SYSTEXT, COLOR_BACKGROUND);
}

void virtual_keyboard ()
{
  int offcount = 0;
  uint8_t oct_count = 0;
  display.setTextColor(COLOR_BACKGROUND, COLOR_SYSTEXT);
  display.setTextSize(1);

  //display.drawLine(1, VIRT_KEYB_YPOS - 4, DISPLAY_WIDTH - 2, VIRT_KEYB_YPOS - 4, GREY2);

  //display.drawLine(0, VIRT_KEYB_YPOS - 3, 0 , VIRT_KEYB_YPOS , COLOR_BACKGROUND);
  //display.drawLine(239, VIRT_KEYB_YPOS - 3, 239 , VIRT_KEYB_YPOS , COLOR_BACKGROUND);
  //display.drawLine(479, VIRT_KEYB_YPOS - 3, 479 , VIRT_KEYB_YPOS , COLOR_BACKGROUND);

  //draw white keys
  for (uint8_t x = 0; x < 10; x++)
  {
    display.fillRect( 1 + x * 32.22, VIRT_KEYB_YPOS , 29.33, 73.5, COLOR_SYSTEXT); // white key
    if ( x == 0 || x == 7 || x == 14) {
      display.setCursor (1 + x * 32.22 + 11.3 , VIRT_KEYB_YPOS + 57.75   );
      display.print("C");
      display.print(ts.virtual_keyboard_octave + oct_count);
      oct_count++;
    }
  }
  for (uint8_t x = 0; x < 16; x++)
  {
    if (seq.piano[x] == 1)
    {
      display.fillRect( x * 18.56 , VIRT_KEYB_YPOS   , 21.33, 34.5, COLOR_BACKGROUND);  // BLACK key
      offcount++;
      if (offcount == 5)offcount = 0;
    }
  }
  display.setTextSize(2);
}

void print_virtual_keyboard_octave ()
{
  uint8_t oct_count = 0;
  display.setTextColor(COLOR_BACKGROUND, COLOR_SYSTEXT);
  display.setTextSize(1);

  //draw white keys
  for (uint8_t x = 0; x < 10; x++)
  {
    if ( x == 0 || x == 7 || x == 14) {
      display.setCursor ( 1 + x * 32.22 + 11.3 , VIRT_KEYB_YPOS + 57.75   );
      display.print("C");
      display.print(ts.virtual_keyboard_octave + oct_count);
      oct_count++;
    }
  }
  //display.setTextSize(2);
}

void handle_touchscreen_mute_matrix()
{

  // SEQUENCER REWRITE
  uint8_t ar[NUM_SEQ_TRACKS][4] = {99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99,  };
  uint8_t chain[NUM_SEQ_TRACKS] {99, 99, 99, 99, 99, 99};
  uint8_t pattern[NUM_SEQ_TRACKS] {99, 99, 99, 99, 99, 99};
  uint8_t chain_counter[NUM_SEQ_TRACKS] = {0, 0, 0, 0, 0, 0};

  for (uint8_t y = 0; y < 4; y++)
  {
    for (uint8_t x = 0; x < NUM_SEQ_TRACKS; x++)
    {
      chain[x] = seq.song[x][seq.current_song_step];
      pattern[x] = seq.chain[  chain[x] ] [ chain_counter[x] ];
      // if ( pattern[x] != 99 )
      // {
      ar[x][y] = pattern[x];
      if (seq.chain[  chain[x] ] [ chain_counter[x] + 1 ] != 99)
        chain_counter[x]++;
      // }
      // else
      // ar[x][y] = 99;

      if ( chain_counter[x]  == find_longest_chain()  )
        chain_counter[x] = 0;

    }
  }

  //  if (touch.touched())
  //  {
  //    LCDML.SCREEN_resetTimer();
  //    ts.p = touch.getPoint();
  //
  //    // Scale from ~0->4000 to tft
  //    ts.p.x = map(ts.p.x, 205, 3860, 0, TFT_HEIGHT);
  //    ts.p.y = map(ts.p.y, 310, 3720 , 0, TFT_WIDTH);
  //    // seq.p.z = map(seq.p.z, 500, 2200 , 1, 127); //touch force == velocity
  //
  //    for (uint8_t y = 0; y < 4; y++)
  //    {
  //
  //      for (uint8_t x = 0; x < NUM_SEQ_TRACKS; x++)
  //      {
  //        if (ts.block_screen_update == false)
  //        {
  //          if ( ts.p.x > CHAR_width + x * (480 / 6 - 3) && ts.p.y > 2 * CHAR_height + y * (320 / 4 - 7) &&
  //               ts.p.x < CHAR_width + x * (480 / 6 - 3) + 68 && ts.p.y < 2 * CHAR_height + y * (320 / 4 - 7) + 62)
  //          {
  //            if (ar[x][y] < NUM_SEQ_PATTERN)
  //            {
  //              ar[x][y] = ar[x][y] + (NUM_SEQ_PATTERN + 10);
  //  seq.chain[  chain[x] ] [ chain_counter[x] ]  = ar[x][y] + (NUM_SEQ_PATTERN + 10);
  //              display.fillRect( CHAR_width + x * (480 / 6 - 3)  , 2 * CHAR_height + y * (320 / 4 - 7),  68, 62, GREY4);
  //            }
  //            else
  //            {
  //              if (ar[x][y] > NUM_SEQ_PATTERN  && ar[x][y] != 99 )
  //              {
  //                ar[x][y] = ar[x][y] - (NUM_SEQ_PATTERN + 10);
  //                display.fillRect( CHAR_width + x * (480 / 6 - 3)  , 2 * CHAR_height + y * (320 / 4 - 7),  68, 62, GREY2);
  //              }
  //            }
  //
  //          }
  //        }
  //      }
  //    }
  //    ts.block_screen_update = true;
  //  }
  //  else
  //    ts.block_screen_update = false;


  for (uint8_t y = 0; y < 4; y++)
  {
    for (uint8_t x = 0; x < NUM_SEQ_TRACKS; x++)
    {
      display.setCursor(  CHAR_width + x * (480 / 6 - 3) + 3  , 2 * CHAR_height + y * (320 / 4 - 7) + 3  );
      display.setTextSize(2);

      if (ar[x][y] < NUM_SEQ_PATTERN && ar[x][y] != 99)
      {
        display.setTextColor(COLOR_SYSTEXT, GREY2);
        display.print ("P");
      }
      //      else
      //      {
      //        display.setTextColor(GREY4, GREY2);
      //      display.print (" ");
      //      }
      if (ar[x][y] < NUM_SEQ_PATTERN  )
        //  if (ar[x][y] < NUM_SEQ_PATTERN || ar[x][y] == 99 )
        seq_print_formatted_number( ar[x][y], 2 );
      //else if (ar[x][y] != 99)
      else
        display.print("   ");
      //seq_print_formatted_number(  ar[x][y] - (NUM_SEQ_PATTERN + 10), 2 );
      display.setTextSize(1);
      if (ar[x][y] < NUM_SEQ_PATTERN  )
      {
        display.setCursor(  CHAR_width + x * (480 / 6 - 3) + 3  , 2 * CHAR_height + y * (320 / 4 - 7) + 51  );
        if (seq.content_type[ar[x][y]] == 0) //Drumpattern
          display.setTextColor(COLOR_DRUMS, GREY2);
        else if (seq.content_type[ar[x][y]] == 1) //Instrument Pattern
          display.setTextColor(COLOR_INSTR, GREY2);
        else if (seq.content_type[ar[x][y]] == 2 || seq.content_type[ar[x][y]] == 3) //  chord or arp pattern
          display.setTextColor(COLOR_CHORDS, GREY2);
        if (seq.content_type[ar[x][y]] == 0)
          display.print("DRUM ");
        else if (seq.content_type[ar[x][y]] == 1)
          display.print("INSTR");
        else if (seq.content_type[ar[x][y]] == 2  )
          display.print("CHORD");
      }
    }
  }


  //    if (seq.step == 1) {
  //      for (uint8_t y = 0; y < 4; y++)
  //      {
  //        if ( seq.chain_active_step == y)
  //
  //         else
  //          display.drawRect( 11, 2 * CHAR_height + y * 73 - 1, 455, 64, COLOR_BACKGROUND  );
  //      }
  //    }
}

bool check_button_on_grid(uint8_t x, uint8_t y)
{

  if (ts.p.x > x * CHAR_width_small && ts.p.x < (x + button_size_x) * CHAR_width_small &&
      ts.p.y > y * CHAR_height_small && ts.p.y < (y + button_size_y) * CHAR_height_small)
    return true;
  else return false;
}

void touch_button_oct_up()
{
  ts.virtual_keyboard_octave++;
  if (ts.virtual_keyboard_octave > 8)ts.virtual_keyboard_octave = 8;
  ts.update_virtual_keyboard_octave = true;
}
void touch_button_oct_down()
{
  ts.virtual_keyboard_octave--;
  if (ts.virtual_keyboard_octave < 1)ts.virtual_keyboard_octave = 1;
  ts.update_virtual_keyboard_octave = true;
}
void touch_button_inst_up()
{
  ts.virtual_keyboard_instrument++;
  if (ts.virtual_keyboard_instrument > 10)ts.virtual_keyboard_instrument = 10;
  virtual_keyboard_print_current_instrument();
  ts.update_virtual_keyboard_octave = true;
}
void touch_button_inst_down()
{
  ts.virtual_keyboard_instrument--;
  if (ts.virtual_keyboard_instrument < 1)ts.virtual_keyboard_instrument = 1;
  virtual_keyboard_print_current_instrument();
  ts.update_virtual_keyboard_octave = true;
}

void touch_check_all_keyboard_buttons()
{
  if (check_button_on_grid(1, 16) )
    touch_button_oct_down();
  else if (check_button_on_grid(45, 16) )
    touch_button_oct_up();

  if (check_button_on_grid(9, 16) && seq.cycle_touch_element == 1)
    touch_button_inst_down();
  else if (check_button_on_grid(37, 16) && seq.cycle_touch_element == 1)
    touch_button_inst_up();
}

void virtual_keyboard_update_all_key_states()
{
  ts.slowdown_UI_input++;
  if (ts.slowdown_UI_input > 27 )
  {
    for (uint8_t x = 0; x < 10; x++)
    {
      if (ts.virtual_keyboard_state_white[x] > 0)
        ts.virtual_keyboard_state_white[x] --;
      if (ts.virtual_keyboard_state_white[x] == 1)
        virtual_keyboard_key_off_white( x);
    }
    for (uint8_t x = 0; x < 16; x++)
    {
      if (ts.virtual_keyboard_state_black[x] > 0)
        ts.virtual_keyboard_state_black[x] --;

      if (ts.virtual_keyboard_state_black[x] == 1)
        virtual_keyboard_key_off_black( x);
    }
    ts.slowdown_UI_input = 0;
  }
}

void handle_touchscreen_voice_select()
{
  if (touch.touched())
  {
    LCDML.SCREEN_resetTimer();
    ts.p = touch.getPoint();
    // Scale from ~0->4000 to tft
    ts.p.x = map(ts.p.x, 205, 3860, 0, TFT_HEIGHT);
    ts.p.y = map(ts.p.y, 310, 3720 , 0, TFT_WIDTH);
    //display.drawPixel(ts.p.x, ts.p.y, RED); //DEBUG

    if (seq.cycle_touch_element != 1 && seq.generic_ui_delay > 31000 )
    {
      if (check_button_on_grid(1, 22))
      {
        if (dexed_live_mod.active_button != 1)
          dexed_live_mod.active_button = 1;
        else
          dexed_live_mod.active_button = 0;
      }
      else if (check_button_on_grid(14, 22))
      {
        if (dexed_live_mod.active_button != 2)
          dexed_live_mod.active_button = 2;
        else
          dexed_live_mod.active_button = 0;
      }
      else if (check_button_on_grid(28, 22))
      {
        if (dexed_live_mod.active_button != 3)
          dexed_live_mod.active_button = 3;
        else
          dexed_live_mod.active_button = 0;
      }
      else if (check_button_on_grid(41, 22))
      {
        if (dexed_live_mod.active_button != 4)
          dexed_live_mod.active_button = 4;
        else
          dexed_live_mod.active_button = 0;
      }
      else
        dexed_live_mod.active_button = 0;

      if (dexed_live_mod.active_button > 0 && dexed_live_mod.active_button < 3)
        selected_instance_id = 0;
      else if (dexed_live_mod.active_button > 2 && dexed_live_mod.active_button < 5)
        selected_instance_id = 1;

      if (dexed_live_mod.active_button > 0 && dexed_live_mod.active_button < 5)
      {
        helptext_r ("< > CHANGE MODIFIER VALUE");
        display.setCursor(0, DISPLAY_HEIGHT - (CHAR_height_small * 2) - 2  );
        print_empty_spaces(38);
        display.setCursor(9 * CHAR_width_small, DISPLAY_HEIGHT - CHAR_height_small * 1  );
        print_empty_spaces(9);
        display.setCursor(CHAR_width_small * 38 + 2, DISPLAY_HEIGHT - (CHAR_height_small * 2) - 2 );
        display.print(F(" PUSH TO RETURN"));
      }
      else
      {
        print_voice_select_default_help();
      }
      print_voice_settings(CHAR_width_small, 104, 0, false);
      print_voice_settings(CHAR_width_small + 160, 104, 1, false);
      print_perfmod_buttons();
      print_perfmod_lables();
      seq.generic_ui_delay = 0;
    }
    if (check_button_on_grid(45, 1) && seq.generic_ui_delay > 12000 )
    {
      border3_large_clear();
      if (seq.cycle_touch_element == 1)
      {
        seq.cycle_touch_element = 0;

        display.drawRect(DISPLAY_WIDTH / 2, CHAR_height * 6 - 4 , DISPLAY_WIDTH / 2, DISPLAY_HEIGHT - 1,  GREY4);
        draw_button_on_grid(45, 1, "TOUCH", "KEYBRD", 0);
        print_voice_settings(CHAR_width_small, 104, 0, true);
        print_voice_settings(CHAR_width_small + 160, 104, 1, true);
        print_perfmod_buttons();
        print_perfmod_lables();
        print_voice_select_default_help();
      }
      else
      {
        seq.cycle_touch_element = 1;
        draw_button_on_grid(45, 1, "DEXED" , "DETAIL", 0);
        virtual_keyboard();

        virtual_keyboard_print_buttons();
        virtual_keyboard_print_current_instrument();
      }
      seq.generic_ui_delay = 0;
    }

    if ( ts.update_virtual_keyboard_octave == false && seq.cycle_touch_element == 1)
    {
      touch_check_all_keyboard_buttons();
    }
    else if (ts.p.y > 92  && ts.p.y < 165 && ts.p.x < 320 / 2 && ts.switch_active_instance == false && seq.cycle_touch_element != 1)
    {
      selected_instance_id = 0;
      ts.switch_active_instance = true;
    }
    else if (ts.p.y > 92  && ts.p.y < 165 && ts.p.x > 320 / 2 && ts.switch_active_instance == false && seq.cycle_touch_element != 1)
    {
      selected_instance_id = 1;
      ts.switch_active_instance = true;
    }
    if (ts.p.x > 1 && ts.p.y > VIRT_KEYB_YPOS && seq.cycle_touch_element == 1)
    {
      virtual_keyboard_key_on();
    }
  }
  if (touch.touched() == false )
  {
    if ( ts.update_virtual_keyboard_octave && seq.cycle_touch_element == 1)
    {
      print_virtual_keyboard_octave();
      ts.update_virtual_keyboard_octave = false;
    }
    if ( ts.switch_active_instance && seq.cycle_touch_element != 1)
    {
      UI_update_instance_icons();
      print_voice_settings(CHAR_width_small, 104, 0, 0);
      print_voice_settings(CHAR_width_small + 160, 104, 1, 0);
      ts.switch_active_instance = false;
    }

  }
  seq.generic_ui_delay++;
  virtual_keyboard_update_all_key_states();
}

void handle_touchscreen_pattern_editor()
{
  if (touch.touched())
  {
    LCDML.SCREEN_resetTimer();
    ts.p = touch.getPoint();
    // Scale from ~0->4000 to tft
    ts.p.x = map(ts.p.x, 205, 3860, 0, TFT_HEIGHT);
    ts.p.y = map(ts.p.y, 310, 3720 , 0, TFT_WIDTH);
    if ( ts.p.y > 168  && ts.p.y < 168 + 42  &&  ts.update_virtual_keyboard_octave == false)
    {
      if (seq.cycle_touch_element == 1)
        touch_check_all_keyboard_buttons();

    }
    else if (ts.p.x > 430 && ts.p.y < CHAR_height * 3  && ts.update_virtual_keyboard_octave == false)
    {
      if (seq.cycle_touch_element == 0)
      {
        seq.cycle_touch_element = 1;
      }
      else if (seq.cycle_touch_element == 1)
      {
        seq.cycle_touch_element = 0;
      }
      ts.update_virtual_keyboard_octave = true;
      seq_pattern_editor_update_dynamic_elements();
    }
    else if (ts.p.x > 1 && ts.p.y > VIRT_KEYB_YPOS && seq.cycle_touch_element == 1)
    {
      virtual_keyboard_key_on();
    }
  }
  if (touch.touched() == false)
  {
    if ( ts.update_virtual_keyboard_octave)
    {
      //print_virtual_keyboard_octave();
      ts.update_virtual_keyboard_octave = false;
    }
  }
  virtual_keyboard_update_all_key_states();
}


void handle_touchscreen_microsynth()
{
  if (touch.touched())
  {
    LCDML.SCREEN_resetTimer();
    ts.p = touch.getPoint();
    // Scale from ~0->4000 to tft
    ts.p.x = map(ts.p.x, 205, 3860, 0, TFT_HEIGHT);
    ts.p.y = map(ts.p.y, 310, 3720 , 0, TFT_WIDTH);

    if (check_button_on_grid(45, 1) && seq.generic_ui_delay > 12000 )
    {
      display.fillRect(0, VIRT_KEYB_YPOS - 6 * CHAR_height_small,  DISPLAY_WIDTH, DISPLAY_HEIGHT , COLOR_BACKGROUND);
      if (seq.cycle_touch_element == 1)
      {
        seq.cycle_touch_element = 0;
        draw_button_on_grid(45, 1, "TOUCH", "KEYBRD", 0);
        microsynth_refresh_lower_screen_static_text();
        microsynth_refresh_lower_screen_dynamic_text();
      }
      else
      {
        seq.cycle_touch_element = 1;
        draw_button_on_grid(45, 1, "MORE" , "PARAM.", 0);
        virtual_keyboard();
        virtual_keyboard_print_buttons();
        virtual_keyboard_print_current_instrument();
      }
      seq.generic_ui_delay = 0;
    }

    if ( ts.update_virtual_keyboard_octave == false && seq.cycle_touch_element == 1)
    {
      touch_check_all_keyboard_buttons();
    }

    if (ts.p.x > 1 && ts.p.y > VIRT_KEYB_YPOS && seq.cycle_touch_element == 1)
    {
      virtual_keyboard_key_on();
    }
  }
  if (touch.touched() == false )
  {
    if ( ts.update_virtual_keyboard_octave && seq.cycle_touch_element == 1)
    {
      print_virtual_keyboard_octave();
      ts.update_virtual_keyboard_octave = false;
    }

  }
  seq.generic_ui_delay++;
  virtual_keyboard_update_all_key_states();
}

void print_file_manager_buttons()
{
  if (fm.sd_mode == 1)
    draw_button_on_grid(1, 25, "DELETE", "FILE", 1);
  else
    draw_button_on_grid(1, 25, "DELETE", "FILE", 0);
  if (fm.sd_mode == 2)
    draw_button_on_grid(28, 25, "PLAY", "SAMPLE", 1);
  else
    draw_button_on_grid(28, 25, "PLAY", "SAMPLE", 0);
  if (fm.sd_mode == 3)
    draw_button_on_grid(19, 25, "COPY >", "FLASH", 1);
  else
    draw_button_on_grid(19, 25, "COPY >", "FLASH", 0);
  if (fm.sd_mode == 4)
    draw_button_on_grid(10, 25, "COPY", "PRESET", 1);
  else
    draw_button_on_grid(10, 25, "COPY", "PRESET", 0);

  // active_window   0 = left window (SDCARD) , 1 = FLASH

  if ( fm.active_window == 0)
  {
    display.drawRect( CHAR_width_small * 29 - 1, 0, CHAR_width_small * 24 + 3 , CHAR_height_small * 23, GREY2);
    display.drawRect( 0, 0, CHAR_width_small * 29 , CHAR_height_small * 23, COLOR_SYSTEXT);

  }
  else
  {

    display.drawRect( 0, 0, CHAR_width_small * 29 , CHAR_height_small * 23, GREY2);
    display.drawRect( CHAR_width_small * 29 - 1, 0, CHAR_width_small * 24 + 3 , CHAR_height_small * 23, COLOR_SYSTEXT);
  }

}
void handle_touchscreen_file_manager()
{
  if (touch.touched())
  {
    LCDML.SCREEN_resetTimer();
    ts.p = touch.getPoint();
    // Scale from ~0->4000 to tft
    ts.p.x = map(ts.p.x, 205, 3860, 0, TFT_HEIGHT);
    ts.p.y = map(ts.p.y, 310, 3720 , 0, TFT_WIDTH);

    // check touch buttons

    if (    ts.p.y > CHAR_height_small * 25 )
    {
      if (  ts.p.x > 1 * CHAR_width_small && ts.p.x < CHAR_width_small * 9   )
      {
        fm.sd_mode = 1;
      }
      else if (   ts.p.x > 28 * CHAR_width_small && ts.p.x < CHAR_width_small * 35 )
      {
        fm.sd_mode = 2;

        if (fm.sd_mode == 2)
        {
          if (fm.sd_is_folder == false)
          {
            if (fm.sd_mode == 2 && ts.block_screen_update == false) //preview
            {
              strcpy(fm.sd_full_name, fm.sd_new_name);
              strcat(fm.sd_full_name, "/");
              strcat(fm.sd_full_name, fm.sd_temp_name);
              playWAVFile(fm.sd_full_name);
              ts.slowdown_UI_input = 0;
              ts.block_screen_update = true;
            }
          }
        }
      }
      else  if (  ts.p.x > 19 * CHAR_width_small && ts.p.x < CHAR_width_small * 26  )
      {
        fm.sd_mode = 3;
      }
      else if (  ts.p.x > 10 * CHAR_width_small && ts.p.x < CHAR_width_small * 17 )
      {
        fm.sd_mode = 4;
      }
    }
    // active_window   0 = left window (SDCARD) , 1 = FLASH
    else if (   ts.p.x > 1  && ts.p.y > 1    &&  ts.p.x < CHAR_width_small * 29 && ts.p.y < CHAR_height_small * 24 )
    {
      fm.active_window = 0;
    }
    else if (   ts.p.x > CHAR_width_small * 29  && ts.p.y > 1 && ts.p.y < CHAR_height_small * 24 )
    {
      fm.active_window = 1;
    }
    print_file_manager_buttons();
    sd_printDirectory(fm.sd_currentDirectory);
    flash_printDirectory();
  }
  ts.slowdown_UI_input++;
  if (ts.slowdown_UI_input > 5)
    ts.block_screen_update = false;
}

void update_midi_learn_button()
{
  if (seq.midi_learn_active == true)
  {
    display.setTextColor(COLOR_SYSTEXT, RED);
    display.fillRect (240 + CHAR_width + CHAR_width * 10 - 2, CHAR_height, 8 * CHAR_width, 4 * CHAR_height, RED);
    display.setCursor(240 + CHAR_width * 12 + 4, 2 * CHAR_height);
    display.setTextSize(2);
    display.print("TOUCH");
    display.setCursor(240 + CHAR_width * 12 + 3, CHAR_height * 4 - 12 );
    display.setTextSize(1);
    display.print("MIDI LEARN");
  }
  else
  {
    display.setTextColor(COLOR_SYSTEXT, COLOR_PITCHSMP);
    display.fillRect (240 + CHAR_width + CHAR_width * 10 - 2, CHAR_height, 8 * CHAR_width, 4 * CHAR_height, COLOR_PITCHSMP);
    display.setCursor(240 + CHAR_width * 12 + 4, 2 * CHAR_height);
    display.setTextSize(2);
    display.print("TOUCH");
    display.setCursor(240 + CHAR_width * 12 + 3, CHAR_height * 4 - 12 );
    display.setTextSize(1);
    display.print("MIDI LEARN");
    // seq.midi_learn_active = false;
  }
}

//void handle_touchscreen_custom_mappings()
//{
//  if (touch.touched() && ts.block_screen_update == false)
//  {
//    LCDML.SCREEN_resetTimer();
//    ts.p = touch.getPoint();
//    // Scale from ~0->4000 to tft
//    ts.p.x = map(ts.p.x, 205, 3860, 0, TFT_HEIGHT);
//    ts.p.y = map(ts.p.y, 310, 3720 , 0, TFT_WIDTH);
//
//    if (  ts.p.x > 240 + CHAR_width + CHAR_width * 10 - 2   && ts.p.y > CHAR_height  &&
//          ts.p.x < 240 + CHAR_width + CHAR_width * 18       && ts.p.y < 5 * CHAR_height )
//    {
//      seq.midi_learn_active = !seq.midi_learn_active;
//      ts.block_screen_update = true;
//      ts.slowdown_UI_input = 0;
//
//      update_midi_learn_button();
//
//    }
//  }
//  ts.slowdown_UI_input++;
//  if (ts.slowdown_UI_input > 7115)
//    ts.block_screen_update = false;
//}

//void handle_touchscreen_cc_mappings()
//{
//  if (touch.touched() && ts.block_screen_update == false)
//  {
//    LCDML.SCREEN_resetTimer();
//    ts.p = touch.getPoint();
//    // Scale from ~0->4000 to tft
//    ts.p.x = map(ts.p.x, 205, 3860, 0, TFT_HEIGHT);
//    ts.p.y = map(ts.p.y, 310, 3720 , 0, TFT_WIDTH);
//
//    if (  ts.p.x > 240 + CHAR_width + CHAR_width * 10 - 2   && ts.p.y > CHAR_height  &&
//          ts.p.x < 240 + CHAR_width + CHAR_width * 18       && ts.p.y < 5 * CHAR_height )
//    {
//      seq.midi_learn_active = !seq.midi_learn_active;
//      ts.block_screen_update = true;
//      ts.slowdown_UI_input = 0;
//
//      update_midi_learn_button();
//
//    }
//  }
//  ts.slowdown_UI_input++;
//  if (ts.slowdown_UI_input > 7115)
//    ts.block_screen_update = false;
//}

//void handle_touchscreen_color_edit()
//{
//  if (touch.touched())
//  {
//    LCDML.SCREEN_resetTimer();
//    ts.p = touch.getPoint();
//    ts.p.x = map(ts.p.x, 205, 3860, 0, TFT_HEIGHT);
//    ts.p.y = map(ts.p.y, 310, 3720 , 0, TFT_WIDTH);
//
//    if ( ts.p.x > 270  && ts.p.x < 310  && ts.p.y < TFT_WIDTH - CHAR_height )
//      if (ts.p.y * 1.22 > 359)
//        ts.temp_col_hue = 359;
//      else
//        ts.temp_col_hue = ts.p.y * 1.22;
//
//    else if ( ts.p.x > 355  && ts.p.x < 395 && ts.p.y < TFT_WIDTH - CHAR_height)
//    {
//      if (ts.p.y > 254)
//        ts.temp_col_sat = 255;
//      else
//        ts.temp_col_sat = ts.p.y / 1.20;
//
//    }
//    else if ( ts.p.x > 440  && ts.p.x < 480 && ts.p.y < TFT_WIDTH - CHAR_height )
//    {
//      if (ts.p.y > 254)
//        ts.temp_col_bright = 255;
//      else
//        ts.temp_col_bright = ts.p.y / 1.20;
//    }
//
//    colors_screen_update();
//  }
//}
