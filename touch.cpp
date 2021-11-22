
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
extern void display_active_instance_number(uint8_t instance_id);
extern void print_voice_settings(int x, int y, uint8_t instance_id, bool fullrefresh);
extern void UI_update_instance_icons();
extern LCDMenuLib2 LCDML;
extern sequencer_t seq;

extern void playWAVFile(const char *filename);

ts_t ts; //touch screen
fm_t fm; //file manager

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

void virtual_keyboard (  int ypos)
{
  uint8_t piano[12 * 4] = {0, 1, 0, 1, 0, 0, 1, 0, 1, 0, 1, 0,  0, 1, 0, 1, 0, 0, 1, 0, 1, 0, 1, 0, 0, 1, 0, 1, 0, 0, 1, 0, 1, 0, 1, 0, 0, 1, 0, 1, 0, 0, 1, 0, 1, 0, 1, 0, };

  int offcount = 0;
  uint8_t oct_count = 0;
  display.setTextColor(BLACK, WHITE);
  display.setTextSize(1);
  display.drawLine(1, ypos - 7, 480 - 2, ypos - 7, GREY1);
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
  uint8_t piano[12 * 4] = {0, 1, 0, 1, 0, 0, 1, 0, 1, 0, 1, 0,  0, 1, 0, 1, 0, 0, 1, 0, 1, 0, 1, 0, 0, 1, 0, 1, 0, 0, 1, 0, 1, 0, 1, 0, 0, 1, 0, 1, 0, 0, 1, 0, 1, 0, 1, 0, };

  int offcount = 0;
  uint8_t oct_count = 0;
  display.setTextColor(BLACK, WHITE);
  display.setTextSize(1);
  display.drawLine(1, ypos - 7, 480 - 2, ypos - 7, GREY1);
  //draw white keys
  for (uint8_t x = 0; x < 10; x++)
  {

    if ( x == 0 || x == 7 || x == 14) {
      display.fillRect(  x * 48.34, ypos , 44, 98, WHITE); // white key
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

void handle_touchscreen_mute_matrix()
{
  if (touch.touched())
  {
    LCDML.SCREEN_resetTimer();
    ts.p = touch.getPoint();

    // Scale from ~0->4000 to tft
    ts.p.x = map(ts.p.x, 205, 3860, 0, TFT_HEIGHT);
    ts.p.y = map(ts.p.y, 310, 3720 , 0, TFT_WIDTH);
    // seq.p.z = map(seq.p.z, 500, 2200 , 1, 127); //touch force == velocity

    for (uint8_t y = 0; y < 4; y++)
    {
      for (uint8_t x = 0; x < NUM_SEQ_TRACKS; x++)
      {
        if (ts.block_screen_update == false)
        {
          if ( ts.p.x > CHAR_width + x * (480 / 6 - 3) && ts.p.y > 2 * CHAR_height + y * (320 / 4 - 7) &&
               ts.p.x < CHAR_width + x * (480 / 6 - 3) + 68 && ts.p.y < 2 * CHAR_height + y * (320 / 4 - 7) + 62)
          {
            if (seq.patternchain[y][x] < NUM_SEQ_PATTERN)
            {
              seq.patternchain[y][x] = seq.patternchain[y][x] + (NUM_SEQ_PATTERN + 10);
              display.fillRect( CHAR_width + x * (480 / 6 - 3)  , 2 * CHAR_height + y * (320 / 4 - 7),  68, 62, GREY4);
            }
            else
            {
              if (seq.patternchain[y][x] > NUM_SEQ_PATTERN  && seq.patternchain[y][x] != 99 )
              {
                seq.patternchain[y][x] = seq.patternchain[y][x] - (NUM_SEQ_PATTERN + 10);
                display.fillRect( CHAR_width + x * (480 / 6 - 3)  , 2 * CHAR_height + y * (320 / 4 - 7),  68, 62, BLUE);
              }
            }
            display.setCursor(  CHAR_width + x * (480 / 6 - 3) + 3  , 2 * CHAR_height + y * (320 / 4 - 7) + 3  );
            display.setTextSize(2);

            if (seq.patternchain[y][x] < NUM_SEQ_PATTERN)
              display.setTextColor(WHITE, BLUE);
            else
              display.setTextColor(GREY1, GREY4);
            display.print ("P");
            if (seq.patternchain[y][x] < NUM_SEQ_PATTERN || seq.patternchain[y][x] == 99 )
              display.print (seq.patternchain[y][x]);
            else if (seq.patternchain[y][x] != 99)
              display.print ( seq.patternchain[y][x] - (NUM_SEQ_PATTERN + 10) );
            display.print (" ");
            display.setTextSize(1);
            if (seq.patternchain[y][x] < NUM_SEQ_PATTERN  )
            {
              display.setCursor(  CHAR_width + x * (480 / 6 - 3) + 3  , 2 * CHAR_height + y * (320 / 4 - 7) + 51  );
              if (seq.content_type[seq.patternchain[y][x]] == 0) //Drumpattern
                display.setTextColor(DX_ORANGE, BLUE);
              else if (seq.content_type[seq.patternchain[y][x]] == 1) //Instrument Pattern
                display.setTextColor(LIGHTBLUE, BLUE);
              else if (seq.content_type[seq.patternchain[y][x]] == 2 || seq.content_type[seq.patternchain[y][x]] == 3) //  chord or arp pattern
                display.setTextColor(DX_MAGENTA, BLUE);

              if (seq.content_type[seq.patternchain[y][x]] == 0)
                display.print("DRUM");
              else if (seq.content_type[seq.patternchain[y][x]] == 1)
                display.print("INSTR");
              else if (seq.content_type[seq.patternchain[y][x]] == 2  )
                display.print("CHORD/ARP");
            }
          }
        }
      }
    }
    ts.block_screen_update = true;
  }
  else
    ts.block_screen_update = false;

  if (seq.step == 1) {
    for (uint8_t y = 0; y < 4; y++)
    {
      if ( seq.chain_active_step == y)

        display.drawRect( 11, 2 * CHAR_height + y * 73 - 1, 455, 64, WHITE  ); else
        display.drawRect( 11, 2 * CHAR_height + y * 73 - 1, 455, 64, BLACK  );
    }
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
    else if (ts.p.y > 92  && ts.p.y < 190 && ts.switch_active_instance == false)
    {
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
      selected_instance_id = !selected_instance_id;
      display_active_instance_number(selected_instance_id);
      UI_update_instance_icons();
      print_voice_settings(12, 115, 0, 0);
      print_voice_settings(252, 115, 1, 0);
      ts.switch_active_instance = false;
    }
  }
  ts.slowdown_keyboard++;
  if (ts.slowdown_keyboard > 30)
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
    ts.slowdown_keyboard = 0;
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
      fm.mode = 1;
    }
    else if (   ts.p.x > CHAR_width + 114 && ts.p.y > 240   &&  ts.p.x < CHAR_width + 114 + 100 && ts.p.y < 240 + 25)
    {
      fm.mode = 2;

      if (fm.mode == 2)
      {
        if (fm.is_folder == false)
        {
          if (fm.mode == 2 && ts.block_screen_update==false) //preview
          {
            strcpy(fm.full_name, fm.new_name);
            strcat(fm.full_name, "/");
            strcat(fm.full_name, fm.temp_name);
            playWAVFile(fm.full_name);
            ts.slowdown_keyboard=0;
             ts.block_screen_update = true;
          }

        }
      }

    }
    else  if (  ts.p.x > 15               && ts.p.y > 280   &&  ts.p.x < 15 + 100              && ts.p.y < 280 + 25)
    {
      fm.mode = 3;
    }
    else if (   ts.p.x > CHAR_width + 114 && ts.p.y > 280   &&  ts.p.x < CHAR_width + 114 + 100 && ts.p.y < 280 + 25)
    {
      fm.mode = 4;
    }

  }
  if (fm.mode == 1)
    display.setTextColor(WHITE, BLUE);
  else
    display.setTextColor(GREY1, BLUE);
  display.setCursor(CHAR_width + 16,       240 + 8);
  display.print("DELETE FILE");
  if (fm.mode == 2)
    display.setTextColor(WHITE, BLUE);
  else
    display.setTextColor(GREY1, BLUE);
  display.setCursor(CHAR_width + 114 + 30, 240 + 8);
  display.print("PREVIEW");
  if (fm.mode == 3)
    display.setTextColor(WHITE, BLUE);
  else
    display.setTextColor(GREY1, BLUE);
  display.setCursor(CHAR_width + 11,       280 + 8);
  display.print("COPY TO FLASH");
  if (fm.mode == 4)
    display.setTextColor(WHITE, BLUE);
  else
    display.setTextColor(GREY1, BLUE);
  display.setCursor(CHAR_width + 114 + 32, 280 + 8);
  display.print("-----");
  
ts.slowdown_keyboard++;
  if (ts.slowdown_keyboard > 5)
ts.block_screen_update=false;


}
