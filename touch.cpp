
#include "config.h"
#include "touch.h"
#include "sequencer.h"
#include "ILI9486_Teensy.h"
#include <LCDMenuLib2.h>

extern ILI9486_Teensy display;
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
extern void border3();
extern void border4();
extern void border3_systext();
extern void border4_systext();
extern void flash_printDirectory();
extern void sd_printDirectory(File currentDirectory);
extern uint8_t find_longest_chain();
extern void seq_print_formatted_number(uint16_t v, uint8_t l);

extern void seq_pattern_editor_update_dynamic_elements();
extern void colors_screen_update();

extern void playWAVFile(const char *filename);

ts_t ts; //touch screen
fm_t fm; //file manager

extern uint16_t COLOR_BACKGROUND;
extern uint16_t COLOR_SYSTEXT;
extern uint16_t COLOR_SYSTEXT_ACCENT;
extern uint16_t COLOR_INSTR;
extern uint16_t COLOR_CHORDS;
extern uint16_t COLOR_DRUMS;
extern uint16_t COLOR_PITCHSMP;

void helptext_l (const char *str)
{
  uint8_t l = strlen(str);
  display.setCursor (0, 320 - CHAR_height);
  display.setTextColor(COLOR_SYSTEXT, COLOR_SYSTEXT_ACCENT);
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
  uint8_t l = strlen(str);
  display.setCursor(480 - CHAR_width * l, 320 - CHAR_height);
  display.setTextColor(COLOR_SYSTEXT, COLOR_SYSTEXT_ACCENT);
  display.print(str);
  if (l < ts.old_helptext_lenght[1])
  {
    display.setCursor(480 - CHAR_width * (ts.old_helptext_lenght[1]), 320 - CHAR_height);
    display.setTextColor(COLOR_SYSTEXT, COLOR_BACKGROUND);
    for (uint8_t x = 0; x < ts.old_helptext_lenght[1] - l; x++)
      display.print(" ");
  }
  ts.old_helptext_lenght[1] = l;
}


uint16_t RGB24toRGB565(uint8_t r, uint8_t g, uint8_t b)
{
  return ((r / 8) << 11) | ((g / 4) << 5) | (b / 8);
}

uint16_t ColorHSV(uint16_t hue, uint8_t sat, uint8_t val )
{
  // hue: 0-359, sat: 0-255, val (lightness): 0-255
  int r = 0, g = 0, b = 0, base;
  //  if (sat == 0)
  //  { // Achromatic color (gray).
  //    colors[0] = val;
  //    colors[1] = val;
  //    colors[2] = val;
  //  }
  //  else
  //  {
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
  //    colors[0] = r;
  //    colors[1] = g;
  //    colors[2] = b;
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

void virtual_keyboard_key_on (int ypos)
{
  uint8_t piano[12 * 4] = {0, 1, 0, 1, 0, 0, 1, 0, 1, 0, 1, 0,  0, 1, 0, 1, 0, 0, 1, 0, 1, 0, 1, 0, 0, 1, 0, 1, 0, 0, 1, 0, 1, 0, 1, 0, 0, 1, 0, 1, 0, 0, 1, 0, 1, 0, 1, 0, };
  uint8_t piano2[13] = {1, 1, 0, 1, 1, 1, 0, 1, 1, 0, 1, 1, 1 };

  int offcount = 0;
  uint8_t halftones = 0;
  display.setTextColor(COLOR_SYSTEXT);
  display.setTextSize(1);
  //draw white keys
  if ( ts.p.y > ypos + 42)
  {
    for (uint8_t x = 0; x < 10; x++)
    {
      if (ts.p.x > x * 48.34 - 20 && ts.p.x < x * 48.34 + 33 - 20 )
      {
        if (ts.virtual_keyboard_state_white[x] == 0)
        {
          ts.virtual_keyboard_state_white[x] = 254;
          for (uint8_t z = 0; z < x; z++)
          {
            if (piano2[z] == 1)
              halftones = halftones + 1;
          }
          handleNoteOn_MIDI_DEVICE_DIN(configuration.dexed[selected_instance_id].midi_channel, ts.virtual_keyboard_octave * 12 + x + halftones, 120);
          display.fillRect( x * 48.34, ypos + 50 , 44, 48, RED   ); // white key
        }
      }
    }
  }
  else if (ts.p.y > ypos && ts.p.y < ypos + 25)
  {
    for (uint8_t x = 0; x < 16; x++)
    {
      if (piano[x] == 1)
      {
        if (ts.p.x > x * 27.7 - 25 && ts.p.x < x * 27.7 + 31 - 25 )

          if (ts.virtual_keyboard_state_black[x] == 0)
          {
            ts.virtual_keyboard_state_black[x] = 254;
            handleNoteOn_MIDI_DEVICE_DIN(configuration.dexed[selected_instance_id].midi_channel, ts.virtual_keyboard_octave * 12 + x , 120);
            display.fillRect(x * 27.7 , ypos   , 32, 45,  RED   ); // BLACK key
            offcount++;
            if (offcount == 5)offcount = 0;
          }
      }
    }
  }
  display.setTextSize(2);
  display.setTextColor(COLOR_SYSTEXT, COLOR_BACKGROUND);
}

void virtual_keyboard_key_off_white ( int ypos, uint8_t note)
{
  display.setTextColor(COLOR_BACKGROUND, COLOR_SYSTEXT);
  display.setTextSize(1);
  //draw white keys

  for (uint8_t x = 0; x < 10; x++)
  {
    if (x == note )
    {
      handleNoteOff_MIDI_DEVICE_DIN(configuration.dexed[selected_instance_id].midi_channel, ts.virtual_keyboard_octave * 12 + note, 120);

      display.fillRect( x * 48.34, ypos + 50 , 44, 48, COLOR_SYSTEXT   ); // white key
      if ( x == 0 || x == 7 ) {
        display.setCursor (x * 48.34 + 17 , ypos + 77   );
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

void virtual_keyboard_key_off_black ( int ypos, uint8_t note)
{
  display.setTextColor(COLOR_BACKGROUND, COLOR_SYSTEXT);
  display.setTextSize(1);

  for (uint8_t x = 0; x < 16; x++)
  {
    if (x == note )
    {
      handleNoteOff_MIDI_DEVICE_DIN(configuration.dexed[selected_instance_id].midi_channel, ts.virtual_keyboard_octave * 12 + note, 120);
      display.fillRect(x * 27.7 , ypos , 32, 45,  COLOR_BACKGROUND  ); // BLACK key
    }
  }
  display.setTextSize(2);
  display.setTextColor(COLOR_SYSTEXT, COLOR_BACKGROUND);
}

void virtual_keyboard (int ypos)
{
  uint8_t piano[12 * 4] = {0, 1, 0, 1, 0, 0, 1, 0, 1, 0, 1, 0,  0, 1, 0, 1, 0, 0, 1, 0, 1, 0, 1, 0, 0, 1, 0, 1, 0, 0, 1, 0, 1, 0, 1, 0, 0, 1, 0, 1, 0, 0, 1, 0, 1, 0, 1, 0, };

  int offcount = 0;
  uint8_t oct_count = 0;
  display.setTextColor(COLOR_BACKGROUND, COLOR_SYSTEXT);
  display.setTextSize(1);
  display.drawLine(1, ypos - 4, 480 - 2, ypos - 4, GREY2);

  display.drawLine(0, ypos - 3, 0 , ypos , COLOR_BACKGROUND);
  display.drawLine(239, ypos - 3, 239 , ypos , COLOR_BACKGROUND);
  display.drawLine(479, ypos - 3, 479 , ypos , COLOR_BACKGROUND);
  //draw white keys
  for (uint8_t x = 0; x < 10; x++)
  {
    display.fillRect(  x * 48.34, ypos , 44, 98, COLOR_SYSTEXT); // white key
    if ( x == 0 || x == 7 || x == 14) {
      display.setCursor ( x * 48.34 + 17 , ypos + 77   );
      display.print("C");
      display.print(ts.virtual_keyboard_octave + oct_count);
      oct_count++;
    }
  }
  for (uint8_t x = 0; x < 16; x++)
  {
    if (piano[x] == 1)
    {
      display.fillRect( x * 27.7 , ypos   , 32, 46, COLOR_BACKGROUND);  // BLACK key
      offcount++;
      if (offcount == 5)offcount = 0;
    }
  }
  display.setTextSize(2);
}

void print_virtual_keyboard_octave (  int ypos)
{
  uint8_t oct_count = 0;
  display.setTextColor(COLOR_BACKGROUND, COLOR_SYSTEXT);
  display.setTextSize(1);
  //draw white keys
  for (uint8_t x = 0; x < 10; x++)
  {
    if ( x == 0 || x == 7 || x == 14) {
      display.setCursor ( x * 48.34 + 17 , ypos + 77   );
      display.print("C");
      display.print(ts.virtual_keyboard_octave + oct_count);
      oct_count++;
    }
  }
  display.setTextSize(2);
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

void handle_touchscreen_voice_select()

{
  if (touch.touched())
  {
    LCDML.SCREEN_resetTimer();
    ts.p = touch.getPoint();
    // Scale from ~0->4000 to tft
    ts.p.x = map(ts.p.x, 205, 3860, 0, TFT_HEIGHT);
    ts.p.y = map(ts.p.y, 310, 3720 , 0, TFT_WIDTH);
    // seq.p.z = map(seq.p.z, 500, 2200 , 1, 127); //touch force == velocity
    if (ts.p.x > 1 && ts.p.y < 92  && ts.update_virtual_keyboard_octave == false)
    {
      if (ts.p.x < TFT_WIDTH / 2)
      {
        ts.virtual_keyboard_octave--;
        if (ts.virtual_keyboard_octave < 1)ts.virtual_keyboard_octave = 1;
        ts.update_virtual_keyboard_octave = true;
      }
      else
      {
        ts.virtual_keyboard_octave++;
        if (ts.virtual_keyboard_octave > 8)ts.virtual_keyboard_octave = 8;
        ts.update_virtual_keyboard_octave = true;
      }
    }
    else if (ts.p.y > 92  && ts.p.y < 190 && ts.p.x < 480 / 2 && ts.switch_active_instance == false)
    {
      selected_instance_id = 0;
      ts.switch_active_instance = true;
    }
    else if (ts.p.y > 92  && ts.p.y < 190 && ts.p.x > 480 / 2 && ts.switch_active_instance == false)
    {
      selected_instance_id = 1;
      ts.switch_active_instance = true;
    }
    else if (ts.p.x > 1 && ts.p.y > 190)
    {
      virtual_keyboard_key_on(220 );
    }
  }
  if (touch.touched() == false)
  {
    if ( ts.update_virtual_keyboard_octave)
    {
      print_virtual_keyboard_octave(220);
      ts.update_virtual_keyboard_octave = false;
    }
    if ( ts.switch_active_instance)
    {
      UI_update_instance_icons();
      print_voice_settings(12, 115, 0, 0);
      print_voice_settings(252, 115, 1, 0);
      ts.switch_active_instance = false;
    }
  }
  ts.slowdown_UI_input++;
  if (ts.slowdown_UI_input > 30)
  {
    for (uint8_t x = 0; x < 10; x++)
    {
      if (ts.virtual_keyboard_state_white[x] > 0)
        ts.virtual_keyboard_state_white[x] --;
      if (ts.virtual_keyboard_state_white[x] == 1)
        virtual_keyboard_key_off_white(220 , x);
    }
    for (uint8_t x = 0; x < 16; x++)
    {
      if (ts.virtual_keyboard_state_black[x] > 0)
        ts.virtual_keyboard_state_black[x] --;

      if (ts.virtual_keyboard_state_black[x] == 1)
        virtual_keyboard_key_off_black(220 , x);
    }
    ts.slowdown_UI_input = 0;
  }
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
      if (ts.p.x < 60)
      {
        ts.virtual_keyboard_octave--;
        if (ts.virtual_keyboard_octave < 1)ts.virtual_keyboard_octave = 1;
        ts.update_virtual_keyboard_octave = true;
        print_virtual_keyboard_octave(220);
      }
      else if (ts.p.x > 480 - 60)
      {
        ts.virtual_keyboard_octave++;
        if (ts.virtual_keyboard_octave > 8)ts.virtual_keyboard_octave = 8;
        ts.update_virtual_keyboard_octave = true;
        print_virtual_keyboard_octave(220);
      }
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
    else if (ts.p.x > 1 && ts.p.y > 190 && seq.cycle_touch_element == 1)
    {
      virtual_keyboard_key_on(220 );
    }
  }
  if (touch.touched() == false)
  {
    if ( ts.update_virtual_keyboard_octave)
    {
      //print_virtual_keyboard_octave(220);
      ts.update_virtual_keyboard_octave = false;
    }
  }
  ts.slowdown_UI_input++;
  if (ts.slowdown_UI_input > 30 && seq.cycle_touch_element == 1)
  {
    for (uint8_t x = 0; x < 10; x++)
    {
      if (ts.virtual_keyboard_state_white[x] > 0)
        ts.virtual_keyboard_state_white[x] --;
      if (ts.virtual_keyboard_state_white[x] == 1)
        virtual_keyboard_key_off_white(220 , x);
    }
    for (uint8_t x = 0; x < 16; x++)
    {
      if (ts.virtual_keyboard_state_black[x] > 0)
        ts.virtual_keyboard_state_black[x] --;

      if (ts.virtual_keyboard_state_black[x] == 1)
        virtual_keyboard_key_off_black(220 , x);
    }
    ts.slowdown_UI_input = 0;
  }
}

void print_file_manager_buttons()
{
  if (fm.sd_mode == 1)
    display.setTextColor(COLOR_SYSTEXT, COLOR_PITCHSMP);
  else
    display.setTextColor(GREY2, COLOR_PITCHSMP);
  display.setCursor(CHAR_width + 16,       240 + 8);
  display.print("DELETE FILE");
  if (fm.sd_mode == 2)
    display.setTextColor(COLOR_SYSTEXT, COLOR_PITCHSMP);
  else
    display.setTextColor(GREY2, COLOR_PITCHSMP);
  display.setCursor(CHAR_width + 114 + 28, 240 + 8);
  display.print("PREVIEW");
  if (fm.sd_mode == 3)
    display.setTextColor(COLOR_SYSTEXT, COLOR_PITCHSMP);
  else
    display.setTextColor(GREY2, COLOR_PITCHSMP);
  display.setCursor(CHAR_width + 11,       280 + 8);
  display.print("COPY TO FLASH");
  if (fm.sd_mode == 4)
    display.setTextColor(COLOR_SYSTEXT, COLOR_PITCHSMP);
  else
    display.setTextColor(GREY2, COLOR_PITCHSMP);
  display.setCursor(CHAR_width + 127, 280 + 8);
  display.print("COPY PRESETS");

  // active_window   0 = left window (SDCARD) , 1 = FLASH

  if ( fm.active_window == 0)
  {
    border4();
    border3_systext();

  }

  else
  {
    border3();
    border4_systext();
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

    if (        ts.p.x > 15               && ts.p.y > 240   &&  ts.p.x < 15 + 100              && ts.p.y < 240 + 25)
    {
      fm.sd_mode = 1;
    }
    else if (   ts.p.x > CHAR_width + 114 && ts.p.y > 240   &&  ts.p.x < CHAR_width + 114 + 100 && ts.p.y < 240 + 25)
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
    else  if (  ts.p.x > 15               && ts.p.y > 280   &&  ts.p.x < 15 + 100              && ts.p.y < 280 + 25)
    {
      fm.sd_mode = 3;
    }
    else if (   ts.p.x > CHAR_width + 114 && ts.p.y > 280   &&  ts.p.x < CHAR_width + 114 + 100 && ts.p.y < 280 + 25)
    {
      fm.sd_mode = 4;
    }
    // active_window   0 = left window (SDCARD) , 1 = FLASH
    else if (   ts.p.x > 1  && ts.p.y > CHAR_height * 5   &&  ts.p.x < 480 / 2 && ts.p.y < 200 )
    {
      fm.active_window = 0;
    }
    else if (   ts.p.x > 480 / 2  && ts.p.y > CHAR_height * 5   &&  ts.p.x < 480 && ts.p.y < 200 )
    {
      fm.active_window = 1;
    }
    print_file_manager_buttons();
    sd_printDirectory(fm.sd_currentDirectoy);
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

void handle_touchscreen_custom_mappings()

{
  if (touch.touched() && ts.block_screen_update == false)
  {
    LCDML.SCREEN_resetTimer();
    ts.p = touch.getPoint();
    // Scale from ~0->4000 to tft
    ts.p.x = map(ts.p.x, 205, 3860, 0, TFT_HEIGHT);
    ts.p.y = map(ts.p.y, 310, 3720 , 0, TFT_WIDTH);

    if (  ts.p.x > 240 + CHAR_width + CHAR_width * 10 - 2   && ts.p.y > CHAR_height  &&
          ts.p.x < 240 + CHAR_width + CHAR_width * 18       && ts.p.y < 5 * CHAR_height )
    {
      seq.midi_learn_active = !seq.midi_learn_active;
      ts.block_screen_update = true;
      ts.slowdown_UI_input = 0;

      update_midi_learn_button();

    }
  }
  ts.slowdown_UI_input++;
  if (ts.slowdown_UI_input > 7115)
    ts.block_screen_update = false;
}

void handle_touchscreen_cc_mappings()
{
  if (touch.touched() && ts.block_screen_update == false)
  {
    LCDML.SCREEN_resetTimer();
    ts.p = touch.getPoint();
    // Scale from ~0->4000 to tft
    ts.p.x = map(ts.p.x, 205, 3860, 0, TFT_HEIGHT);
    ts.p.y = map(ts.p.y, 310, 3720 , 0, TFT_WIDTH);

    if (  ts.p.x > 240 + CHAR_width + CHAR_width * 10 - 2   && ts.p.y > CHAR_height  &&
          ts.p.x < 240 + CHAR_width + CHAR_width * 18       && ts.p.y < 5 * CHAR_height )
    {
      seq.midi_learn_active = !seq.midi_learn_active;
      ts.block_screen_update = true;
      ts.slowdown_UI_input = 0;

      update_midi_learn_button();

    }
  }
  ts.slowdown_UI_input++;
  if (ts.slowdown_UI_input > 7115)
    ts.block_screen_update = false;
}

void handle_touchscreen_color_edit()
{
  if (touch.touched())
  {
    LCDML.SCREEN_resetTimer();
    ts.p = touch.getPoint();
    ts.p.x = map(ts.p.x, 205, 3860, 0, TFT_HEIGHT);
    ts.p.y = map(ts.p.y, 310, 3720 , 0, TFT_WIDTH);

    if ( ts.p.x > 270  && ts.p.x < 310  && ts.p.y < TFT_WIDTH - CHAR_height )
    if (ts.p.y*1.22>359)
    ts.temp_col_hue=359;
    else
      ts.temp_col_hue = ts.p.y*1.22;

    else if ( ts.p.x > 355  && ts.p.x < 395 && ts.p.y < TFT_WIDTH - CHAR_height)
    {
      if (ts.p.y>254)
      ts.temp_col_sat=255;
      else
      ts.temp_col_sat = ts.p.y / 1.20;
      
    }
    else if ( ts.p.x > 440  && ts.p.x < 480 && ts.p.y < TFT_WIDTH - CHAR_height )
    {
       if (ts.p.y>254)
      ts.temp_col_bright=255;
      else
      ts.temp_col_bright = ts.p.y / 1.20;
    }
    
    colors_screen_update();
  }
}
