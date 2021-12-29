
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
extern void border3_white();
extern void border4_white();
extern void flash_printDirectory();
extern void sd_printDirectory(File currentDirectory);
extern uint8_t find_longest_chain();
extern void seq_print_formatted_number(uint8_t v, uint8_t l);

extern void seq_pattern_editor_update_dynamic_elements();

extern void playWAVFile(const char *filename);

ts_t ts; //touch screen
fm_t fm; //file manager

void helptext_l (const char *str)
{
  uint8_t l = strlen(str);
  display.setCursor (0, 320 - CHAR_height);
  display.setTextColor(WHITE, DX_MAGENTA);
  display.print(str);

  if (l < ts.old_helptext_lenght[0])
  {
    display.setTextColor(WHITE, BLACK);
    for (uint8_t x = 0; x < ts.old_helptext_lenght[0] - l; x++)
      display.print(" ");
  }

  ts.old_helptext_lenght[0] = l;
}

void helptext_r (const char *str)
{
  uint8_t l = strlen(str);
  display.setCursor(480 - CHAR_width * l, 320 - CHAR_height);
  display.setTextColor(WHITE, DX_MAGENTA);
  display.print(str);
  if (l < ts.old_helptext_lenght[1])
  {
    display.setCursor(480 - CHAR_width * (ts.old_helptext_lenght[1]), 320 - CHAR_height);
    display.setTextColor(WHITE, BLACK);
    for (uint8_t x = 0; x < ts.old_helptext_lenght[1] - l; x++)
      display.print(" ");
  }
  ts.old_helptext_lenght[1] = l;
}

uint16_t RGB24toRGB565(uint8_t r, uint8_t g, uint8_t b)
{
  return ((r / 8) << 11) | ((g / 4) << 5) | (b / 8);
}

uint32_t ColorHSV(uint16_t hue, uint8_t sat, uint8_t val) {

  uint8_t r, g, b;

  // Remap 0-65535 to 0-1529. Pure red is CENTERED on the 64K rollover;
  // 0 is not the start of pure red, but the midpoint...a few values above
  // zero and a few below 65536 all yield pure red (similarly, 32768 is the
  // midpoint, not start, of pure cyan). The 8-bit RGB hexcone (256 values
  // each for red, green, blue) really only allows for 1530 distinct hues
  // (not 1536, more on that below), but the full unsigned 16-bit type was
  // chosen for hue so that one's code can easily handle a contiguous color
  // wheel by allowing hue to roll over in either direction.
  hue = (hue * 1530L + 32768) / 65536;
  // Because red is centered on the rollover point (the +32768 above,
  // essentially a fixed-point +0.5), the above actually yields 0 to 1530,
  // where 0 and 1530 would yield the same thing. Rather than apply a
  // costly modulo operator, 1530 is handled as a special case below.
  // So you'd think that the color "hexcone" (the thing that ramps from
  // pure red, to pure yellow, to pure green and so forth back to red,
  // yielding six slices), and with each color component having 256
  // possible values (0-255), might have 1536 possible items (6*256),
  // but in reality there's 1530. This is because the last element in
  // each 256-element slice is equal to the first element of the next
  // slice, and keeping those in there this would create small
  // discontinuities in the color wheel. So the last element of each
  // slice is dropped...we regard only elements 0-254, with item 255
  // being picked up as element 0 of the next slice. Like this:
  // Red to not-quite-pure-yellow is:        255,   0, 0 to 255, 254,   0
  // Pure yellow to not-quite-pure-green is: 255, 255, 0 to   1, 255,   0
  // Pure green to not-quite-pure-cyan is:     0, 255, 0 to   0, 255, 254
  // and so forth. Hence, 1530 distinct hues (0 to 1529), and hence why
  // the constants below are not the multiples of 256 you might expect.

  // Convert hue to R,G,B (nested ifs faster than divide+mod+switch):
  if (hue < 510) {        // Red to Green-1
    b = 0;
    if (hue < 255) {      //   Red to Yellow-1
      r = 255;
      g = hue;            //     g = 0 to 254
    } else {              //   Yellow to Green-1
      r = 510 - hue;      //     r = 255 to 1
      g = 255;
    }
  } else if (hue < 1020) { // Green to Blue-1
    r = 0;
    if (hue <  765) {     //   Green to Cyan-1
      g = 255;
      b = hue - 510;      //     b = 0 to 254
    } else {              //   Cyan to Blue-1
      g = 1020 - hue;     //     g = 255 to 1
      b = 255;
    }
  } else if (hue < 1530) { // Blue to Red-1
    g = 0;
    if (hue < 1275) {     //   Blue to Magenta-1
      r = hue - 1020;     //     r = 0 to 254
      b = 255;
    } else {              //   Magenta to Red-1
      r = 255;
      b = 1530 - hue;     //     b = 255 to 1
    }
  } else {                // Last 0.5 Red (quicker than % operator)
    r = 255;
    g = b = 0;
  }
  // Apply saturation and value to R,G,B, pack into 32-bit result:
  uint32_t v1 =   1 + val; // 1 to 256; allows >>8 instead of /255
  uint16_t s1 =   1 + sat; // 1 to 256; same reason
  uint8_t  s2 = 255 - sat; // 255 to 0

  //  return ((((((r * s1) >> 8) + s2) * v1) & 0xff00) << 8) |
  //          (((((g * s1) >> 8) + s2) * v1) & 0xff00)       |
  //         ( ((((b * s1) >> 8) + s2) * v1)           >> 8);

  return RGB24toRGB565( ((((r * s1) >> 8) + s2) * v1),  ((((g * s1) >> 8) + s2) * v1),  ( (((b * s1) >> 8) + s2) * v1)    );
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
  display.setTextColor(WHITE);
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
  display.setTextColor(WHITE, BLACK);
}

void virtual_keyboard_key_off_white ( int ypos, uint8_t note)
{
  display.setTextColor(BLACK, WHITE);
  display.setTextSize(1);
  //draw white keys

  for (uint8_t x = 0; x < 10; x++)
  {
    if (x == note )
    {
      handleNoteOff_MIDI_DEVICE_DIN(configuration.dexed[selected_instance_id].midi_channel, ts.virtual_keyboard_octave * 12 + note, 120);

      display.fillRect( x * 48.34, ypos + 50 , 44, 48, WHITE   ); // white key
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
  display.setTextColor(WHITE, BLACK);
}

void virtual_keyboard_key_off_black ( int ypos, uint8_t note)
{
  display.setTextColor(BLACK, WHITE);
  display.setTextSize(1);

  for (uint8_t x = 0; x < 16; x++)
  {
    if (x == note )
    {
      handleNoteOff_MIDI_DEVICE_DIN(configuration.dexed[selected_instance_id].midi_channel, ts.virtual_keyboard_octave * 12 + note, 120);
      display.fillRect(x * 27.7 , ypos   , 32, 45,  BLACK   ); // BLACK key
    }
  }
  display.setTextSize(2);
  display.setTextColor(WHITE, BLACK);
}

void virtual_keyboard (int ypos)
{
  uint8_t piano[12 * 4] = {0, 1, 0, 1, 0, 0, 1, 0, 1, 0, 1, 0,  0, 1, 0, 1, 0, 0, 1, 0, 1, 0, 1, 0, 0, 1, 0, 1, 0, 0, 1, 0, 1, 0, 1, 0, 0, 1, 0, 1, 0, 0, 1, 0, 1, 0, 1, 0, };

  int offcount = 0;
  uint8_t oct_count = 0;
  display.setTextColor(BLACK, WHITE);
  display.setTextSize(1);
  display.drawLine(1, ypos - 4, 480 - 2, ypos - 4, GREY1);

  display.drawLine(0, ypos - 3, 0 , ypos , BLACK);
  display.drawLine(239, ypos - 3, 239 , ypos , BLACK);
  display.drawLine(479, ypos - 3, 479 , ypos , BLACK);
  //draw white keys
  for (uint8_t x = 0; x < 10; x++)
  {
    display.fillRect(  x * 48.34, ypos , 44, 98, WHITE); // white key
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
      display.fillRect( x * 27.7 , ypos   , 32, 46, BLACK);  // BLACK key
      offcount++;
      if (offcount == 5)offcount = 0;
    }
  }
  display.setTextSize(2);
}

void print_virtual_keyboard_octave (  int ypos)
{
  uint8_t oct_count = 0;
  display.setTextColor(BLACK, WHITE);
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
  uint8_t ar[NUM_SEQ_TRACKS][4]={99,99,99,99, 99,99,99,99, 99,99,99,99, 99,99,99,99, 99,99,99,99, 99,99,99,99,  };
  uint8_t chain[NUM_SEQ_TRACKS]{99,99,99,99,99,99};
  uint8_t pattern[NUM_SEQ_TRACKS]{99,99,99,99,99,99};
  uint8_t chain_counter[NUM_SEQ_TRACKS]={0,0,0,0,0,0};

  for (uint8_t y = 0; y < 4; y++)
  {
    for (uint8_t x = 0; x < NUM_SEQ_TRACKS; x++)
    {
      chain[x] = seq.song[x][seq.current_song_step];
      pattern[x] = seq.chain[  chain[x] ] [ chain_counter[x] ];
      if ( pattern[x] != 99 )
      {
        ar[x][y] = pattern[x];
        if (seq.chain[  chain[x] ] [ chain_counter[x]+1 ] !=99)
         chain_counter[x]++;
      }
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
  //              display.fillRect( CHAR_width + x * (480 / 6 - 3)  , 2 * CHAR_height + y * (320 / 4 - 7),  68, 62, GREY4);
  //            }
  //            else
  //            {
  //              if (ar[x][y] > NUM_SEQ_PATTERN  && ar[x][y] != 99 )
  //              {
  //                ar[x][y] = ar[x][y] - (NUM_SEQ_PATTERN + 10);
  //                display.fillRect( CHAR_width + x * (480 / 6 - 3)  , 2 * CHAR_height + y * (320 / 4 - 7),  68, 62, GREY1);
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

      if (ar[x][y] < NUM_SEQ_PATTERN)
        display.setTextColor(WHITE, GREY1);
      else
        display.setTextColor(GREY4, GREY1);
      display.print ("P");
      if (ar[x][y] < NUM_SEQ_PATTERN || ar[x][y] == 99 )
         seq_print_formatted_number( ar[x][y], 2 );
      else if (ar[x][y] != 99)
     seq_print_formatted_number(  ar[x][y] - (NUM_SEQ_PATTERN + 10), 2 );
      display.setTextSize(1);
      if (ar[x][y] < NUM_SEQ_PATTERN  )
      {
        display.setCursor(  CHAR_width + x * (480 / 6 - 3) + 3  , 2 * CHAR_height + y * (320 / 4 - 7) + 51  );
        if (seq.content_type[ar[x][y]] == 0) //Drumpattern
          display.setTextColor(DX_ORANGE, GREY1);
        else if (seq.content_type[ar[x][y]] == 1) //Instrument Pattern
          display.setTextColor(LIGHTBLUE, GREY1);
        else if (seq.content_type[ar[x][y]] == 2 || seq.content_type[ar[x][y]] == 3) //  chord or arp pattern
          display.setTextColor(DX_MAGENTA, GREY1);
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
  //          display.drawRect( 11, 2 * CHAR_height + y * 73 - 1, 455, 64, WHITE  ); else
  //          display.drawRect( 11, 2 * CHAR_height + y * 73 - 1, 455, 64, BLACK  );
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
    display.setTextColor(WHITE, BLUE);
  else
    display.setTextColor(GREY1, BLUE);
  display.setCursor(CHAR_width + 16,       240 + 8);
  display.print("DELETE FILE");
  if (fm.sd_mode == 2)
    display.setTextColor(WHITE, BLUE);
  else
    display.setTextColor(GREY1, BLUE);
  display.setCursor(CHAR_width + 114 + 28, 240 + 8);
  display.print("PREVIEW");
  if (fm.sd_mode == 3)
    display.setTextColor(WHITE, BLUE);
  else
    display.setTextColor(GREY1, BLUE);
  display.setCursor(CHAR_width + 11,       280 + 8);
  display.print("COPY TO FLASH");
  if (fm.sd_mode == 4)
    display.setTextColor(WHITE, BLUE);
  else
    display.setTextColor(GREY1, BLUE);
  display.setCursor(CHAR_width + 127, 280 + 8);
  display.print("COPY PRESETS");

  // active_window   0 = left window (SDCARD) , 1 = FLASH

  if ( fm.active_window == 0)
  {
    border4();
    border3_white();

  }

  else
  {
    border3();
    border4_white();
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
  if (ts.midi_learn_active == true)
  {
    display.setTextColor(WHITE, RED);
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
    display.setTextColor(WHITE, BLUE);
    display.fillRect (240 + CHAR_width + CHAR_width * 10 - 2, CHAR_height, 8 * CHAR_width, 4 * CHAR_height, BLUE);
    display.setCursor(240 + CHAR_width * 12 + 4, 2 * CHAR_height);
    display.setTextSize(2);
    display.print("TOUCH");
    display.setCursor(240 + CHAR_width * 12 + 3, CHAR_height * 4 - 12 );
    display.setTextSize(1);
    display.print("MIDI LEARN");
    // ts.midi_learn_active = false;
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
      ts.midi_learn_active = !ts.midi_learn_active;
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
      ts.midi_learn_active = !ts.midi_learn_active;
      ts.block_screen_update = true;
      ts.slowdown_UI_input = 0;

      update_midi_learn_button();

    }
  }
  ts.slowdown_UI_input++;
  if (ts.slowdown_UI_input > 7115)
    ts.block_screen_update = false;
}
