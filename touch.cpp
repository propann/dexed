#include "config.h"
#include "touch.h"
#include "sequencer.h"
#include <LCDMenuLib2.h>
#include "ILI9341_t3n.h"

#ifdef USE_MULTIBAND
#include "scope.h"
#include <Audio.h>
#include "template_mixer.hpp"
#endif

extern ILI9341_t3n display;
extern config_t configuration;
extern uint8_t selected_instance_id;
extern void handleNoteOn_MIDI_DEVICE_DIN(byte inChannel, byte inNumber, byte inVelocity);
extern void handleNoteOff_MIDI_DEVICE_DIN(byte inChannel, byte inNumber, byte inVelocity);
extern void print_voice_settings(bool fullrefresh);
extern void print_voice_settings_in_pattern_editor(int x, int y);
extern void UI_update_instance_icons();
extern LCDMenuLib2 LCDML;
extern void pattern_editor_menu_0();
extern void UI_func_test_touchscreen(uint8_t param);
extern void sub_touchscreen_test_page_init();
extern bool remote_touched;

#ifdef USE_SEQUENCER
extern sequencer_t seq;
#endif

extern void border3_large_clear();
extern void border3_large();
#ifdef COMPILE_FOR_FLASH
extern void flash_printDirectory();
#endif
#ifdef COMPILE_FOR_QSPI
extern void flash_printDirectory(File flash_currentDirectory);
#endif
extern void sd_printDirectory(File currentDirectory);
extern uint8_t find_longest_chain();
extern void print_formatted_number(uint16_t v, uint8_t l);
extern void virtual_keyboard_print_buttons();
extern void draw_button_on_grid(uint8_t x, uint8_t y, const char *t1, const char *t2, uint8_t color);
extern void seq_pattern_editor_update_dynamic_elements();
extern void colors_screen_update();
extern void microsynth_refresh_lower_screen_static_text();
extern void microsynth_refresh_lower_screen_dynamic_text();
extern float get_sample_p_offset(uint8_t sample);
extern void set_sample_pitch(uint8_t sample, float playbackspeed);
extern void show_small_font(int pos_y, int pos_x, uint8_t field_size, const char *str);
extern const char *find_long_drum_name_from_note(uint8_t note);
extern void print_perfmod_buttons();
extern void print_perfmod_lables();
extern void print_empty_spaces(uint8_t spaces);
extern void print_voice_select_default_help();
extern void playWAVFile(const char *filename);
extern void handleStop(void);
extern void handleStart(void);
extern void UI_func_load_performance(uint8_t param);
extern void UI_func_save_performance(uint8_t param);
extern void UI_func_seq_pattern_editor(uint8_t param);
extern void UI_func_seq_vel_editor(uint8_t param);
extern void UI_func_mixer(uint8_t param);
extern void UI_func_song(uint8_t param);
extern void UI_func_voice_select(uint8_t param);
extern void save_favorite(uint8_t b, uint8_t v, uint8_t instance_id);
extern uint8_t activesample;

#ifdef USE_MICROSYNTH
extern microsynth_t microsynth[NUM_MICROSYNTH];
#endif

#ifdef USE_BRAIDS
extern braids_t braids_osc;
#endif

#ifdef USE_MULTIBAND
extern void mb_set_mutes();
extern bool mb_solo_low;
extern bool mb_solo_mid;
extern bool mb_solo_upper_mid;
extern bool mb_solo_high;
extern Realtime_Scope scope;
extern AudioAnalyzePeak mb_before_l;
extern AudioAnalyzePeak mb_before_r;
extern AudioAnalyzePeak mb_after_l;
extern AudioAnalyzePeak mb_after_r;
extern void draw_volmeter(int x, int y, uint8_t arr, float value);
extern AudioMixer<4> mb_mixer_l;
extern AudioMixer<4> mb_mixer_r;
extern bool multiband_active;
extern uint8_t generic_active_function;
extern uint8_t generic_temp_select_menu;
extern void mb_set_mutes();
extern void mb_set_master();
extern void mb_set_compressor();
#endif

ts_t ts;                          //touch screen
fm_t fm;                          //file manager
dexed_live_mod_t dexed_live_mod;  // dexed quick live modifiers for attack and release
extern int temp_int;

extern uint16_t COLOR_BACKGROUND;
extern uint16_t COLOR_SYSTEXT;
extern uint16_t COLOR_SYSTEXT_ACCENT;
extern uint16_t COLOR_INSTR;
extern uint16_t COLOR_CHORDS;
extern uint16_t COLOR_ARP;
extern uint16_t COLOR_DRUMS;
extern uint16_t COLOR_PITCHSMP;

FLASHMEM void helptext_l(const char *str) {
  display.setTextSize(1);
  uint8_t l = strlen(str);

  display.setCursor(0, DISPLAY_HEIGHT - CHAR_height_small);
  display.setTextColor(COLOR_SYSTEXT, DX_DARKCYAN);
  display.print(str);

  if (l < ts.old_helptext_length[0]) {
    display.setTextColor(COLOR_SYSTEXT, COLOR_BACKGROUND);
    for (uint8_t x = 0; x < ts.old_helptext_length[0] - l; x++)
      display.print(" ");
  }

  ts.old_helptext_length[0] = l;
}

FLASHMEM void helptext_r(const char *str) {
  display.setTextSize(1);
  uint8_t l = strlen(str);
  display.setCursor(DISPLAY_WIDTH - CHAR_width_small * l, DISPLAY_HEIGHT - CHAR_height_small);
  display.setTextColor(COLOR_SYSTEXT, DX_DARKCYAN);
  display.print(str);
  if (l < ts.old_helptext_length[1]) {
    display.setCursor(DISPLAY_WIDTH - CHAR_width_small * (ts.old_helptext_length[1]), DISPLAY_HEIGHT - CHAR_height_small);
    display.setTextColor(COLOR_SYSTEXT, COLOR_BACKGROUND);
    for (uint8_t x = 0; x < ts.old_helptext_length[1] - l; x++)
      display.print(" ");
  }
  ts.old_helptext_length[1] = l;
}

FLASHMEM void helptext_c(const char *str) {
  display.setTextSize(1);
  uint8_t l = strlen(str);
  display.setCursor(DISPLAY_WIDTH / 2 - (l / 2) * CHAR_width_small, DISPLAY_HEIGHT - CHAR_height_small);
  display.setTextColor(COLOR_SYSTEXT, DX_DARKCYAN);
  display.print(str);
  if (l < ts.old_helptext_length[2]) {
    display.setCursor(DISPLAY_WIDTH / 2 - (ts.old_helptext_length[2] / 2) * CHAR_width_small, DISPLAY_HEIGHT - CHAR_height_small);
    display.setTextColor(COLOR_SYSTEXT, COLOR_BACKGROUND);
    for (uint8_t x = 0; x < ts.old_helptext_length[2] - l; x++)
      display.print(" ");
  }
  ts.old_helptext_length[2] = l;
}

uint16_t RGB24toRGB565(uint8_t r, uint8_t g, uint8_t b) {
  return ((r / 8) << 11) | ((g / 4) << 5) | (b / 8);
}

uint16_t ColorHSV(uint16_t hue, uint8_t sat, uint8_t val) {
  // hue: 0-359, sat: 0-255, val (lightness): 0-255
  int r = 0, g = 0, b = 0, base;

  base = ((255 - sat) * val) >> 8;
  switch (hue / 60) {
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
  return RGB24toRGB565(r, g, b);
  //  }
}

void print_current_chord() {
  for (uint8_t x = 0; x < 7; x++) {
    display.print(seq.chord_names[seq.arp_chord][x]);
  }
}

FLASHMEM void virtual_keyboard_print_current_instrument() {
  display.setTextColor(GREY2, COLOR_BACKGROUND);
  display.setTextSize(2);
  display.setCursor(17 * CHAR_width_small, 16 * CHAR_height_small);
  if (ts.virtual_keyboard_instrument < 8)
    display.print(F("PLAYING"));
  else {
    display.setTextSize(1);
    display.print(F("PITCHED SAMPLE "));
    display.console = 1;
    display.fillRect(17 * CHAR_width_small, 17 * CHAR_height_small, 14 * CHAR_width_small, 8, COLOR_BACKGROUND);
    display.console = 0;
  }
  display.setTextSize(2);
  display.setCursor(17 * CHAR_width_small, 18 * CHAR_height_small + 1);
  display.setTextColor(COLOR_SYSTEXT, COLOR_BACKGROUND);
  if (ts.virtual_keyboard_instrument == 1) {
    display.print(F("DEXED1  "));
    ts.virtual_keyboard_midi_channel = configuration.dexed[0].midi_channel;
  } else if (ts.virtual_keyboard_instrument == 2) {
    display.print(F("DEXED2  "));
    ts.virtual_keyboard_midi_channel = configuration.dexed[1].midi_channel;
  } else if (ts.virtual_keyboard_instrument == 3) {
    display.print(F("MSYNTH1 "));
#ifdef USE_MICROSYNTH
    ts.virtual_keyboard_midi_channel = microsynth[0].midi_channel;
#endif
  } else if (ts.virtual_keyboard_instrument == 4) {
    display.print(F("MSYNTH2 "));
#ifdef USE_MICROSYNTH
    ts.virtual_keyboard_midi_channel = microsynth[1].midi_channel;
#endif
  } else if (ts.virtual_keyboard_instrument == 5) {
    display.print(F("EPIANO  "));
    ts.virtual_keyboard_midi_channel = configuration.epiano.midi_channel;
  } else if (ts.virtual_keyboard_instrument == 6) {
    display.print(F("DRUMS   "));
    ts.virtual_keyboard_midi_channel = DRUM_MIDI_CHANNEL;
  } else if (ts.virtual_keyboard_instrument == 7) {
    display.print(F("BRAIDS   "));
#ifdef USE_BRAIDS
    ts.virtual_keyboard_midi_channel = braids_osc.midi_channel;
#endif
  } else if (ts.virtual_keyboard_instrument > 7) {
    display.print(ts.virtual_keyboard_instrument - 7);
    display.print(" ");

    show_small_font(18 * CHAR_height_small + 1, 20 * CHAR_width_small, 7, find_long_drum_name_from_note(ts.virtual_keyboard_instrument - 8 + 210));

    ts.virtual_keyboard_midi_channel = DRUM_MIDI_CHANNEL;
  }
}

FLASHMEM void virtual_keyboard_key_on() {
  int offcount = 0;
  uint8_t halftones = 0;
  display.setTextColor(COLOR_SYSTEXT);
  display.setTextSize(1);

  //draw white keys
  if (ts.p.y > VIRT_KEYB_YPOS + 36) {
    for (uint8_t x = 0; x < 10; x++) {
      if (ts.p.x > x * 32.22 && ts.p.x < x * 32.22 + 32) {
        if (ts.virtual_keyboard_state_white[x] == 0) {
          ts.virtual_keyboard_state_white[x] = 254;
          for (uint8_t z = 0; z < x; z++) {
            if (seq.piano2[z] == 1)
              halftones = halftones + 1;
          }
          // handleNoteOn_MIDI_DEVICE_DIN(configuration.dexed[selected_instance_id].midi_channel, ts.virtual_keyboard_octave * 12 + x + halftones, 120);
          if (ts.virtual_keyboard_instrument > 7)  //pitched samples
          {
            set_sample_pitch(ts.virtual_keyboard_instrument - 8, (float)pow(2, (ts.virtual_keyboard_octave * 12 + x + halftones - 72) / 12.00) * get_sample_p_offset(ts.virtual_keyboard_instrument - 7));
            handleNoteOn_MIDI_DEVICE_DIN(ts.virtual_keyboard_midi_channel, 210 + ts.virtual_keyboard_instrument - 8, 100);
          } else {
            handleNoteOn_MIDI_DEVICE_DIN(ts.virtual_keyboard_midi_channel, ts.virtual_keyboard_octave * 12 + x + halftones, 120);
            display.console = true;
            display.fillRect(1 + x * 32.22, VIRT_KEYB_YPOS + 34, 29.33, 39, RED);  // white key
            display.console = false;
          }
        }
      }
    }
  } else if (ts.p.y > VIRT_KEYB_YPOS && ts.p.y < VIRT_KEYB_YPOS + 34) {
    for (uint8_t x = 0; x < 16; x++) {
      if (seq.piano[x] == 1) {
        if (ts.p.x > x * 18.46 && ts.p.x < x * 18.46 + 24)

          if (ts.virtual_keyboard_state_black[x] == 0) {
            ts.virtual_keyboard_state_black[x] = 254;
            if (ts.virtual_keyboard_instrument > 7)  //pitched samples
            {
              set_sample_pitch(ts.virtual_keyboard_instrument - 8, (float)pow(2, (ts.virtual_keyboard_octave * 12 + x - 72) / 12.00) * get_sample_p_offset(ts.virtual_keyboard_instrument - 7));
              handleNoteOn_MIDI_DEVICE_DIN(ts.virtual_keyboard_midi_channel, 210 + ts.virtual_keyboard_instrument - 8, 100);
            } else {
              handleNoteOn_MIDI_DEVICE_DIN(ts.virtual_keyboard_midi_channel, ts.virtual_keyboard_octave * 12 + x, 120);
              display.console = true;
              display.fillRect(x * 18.56, VIRT_KEYB_YPOS, 21.33, 34.5, RED);  // BLACK key
              display.console = false;
            }
            offcount++;
            if (offcount == 5) offcount = 0;
          }
      }
    }
  }
  //display.fillRect(ts.p.x-1,ts.p.y-1,3,3,YELLOW);
  display.setTextSize(2);
  display.setTextColor(COLOR_SYSTEXT, COLOR_BACKGROUND);
  display.console = false;
}

FLASHMEM void virtual_keyboard_key_off_white(uint8_t note) {

  display.setTextColor(COLOR_BACKGROUND, COLOR_SYSTEXT);
  display.setTextSize(1);

  //draw white keys

  for (uint8_t x = 0; x < 10; x++) {
    if (x == note) {
      handleNoteOff_MIDI_DEVICE_DIN(ts.virtual_keyboard_midi_channel, ts.virtual_keyboard_octave * 12 + note, 120);
      display.console = true;
      display.fillRect(1 + x * 32.22, VIRT_KEYB_YPOS + 34, 29.33, 39, COLOR_SYSTEXT);  // white key
      display.console = false;

      if (x == 0 || x == 7) {
        display.setCursor(1 + x * 32.22 + 11.3, VIRT_KEYB_YPOS + 57.75);
        display.print("C");
        if (x == 0)
          display.print(ts.virtual_keyboard_octave);
        else if (x == 7)
          display.print(ts.virtual_keyboard_octave + 1);
      }
    }
  }
  display.setTextSize(2);
  display.setTextColor(COLOR_SYSTEXT, COLOR_BACKGROUND);
  display.console = false;
}

FLASHMEM void virtual_keyboard_key_off_black(uint8_t note) {
  display.setTextColor(COLOR_BACKGROUND, COLOR_SYSTEXT);
  display.setTextSize(1);

  for (uint8_t x = 0; x < 16; x++) {
    if (x == note) {
      handleNoteOff_MIDI_DEVICE_DIN(ts.virtual_keyboard_midi_channel, ts.virtual_keyboard_octave * 12 + note, 120);
      display.console = true;
      display.fillRect(x * 18.56, VIRT_KEYB_YPOS, 21.33, 34.5, COLOR_BACKGROUND);  // BLACK key
      display.console = false;
    }
  }
  display.setTextSize(2);
  display.setTextColor(COLOR_SYSTEXT, COLOR_BACKGROUND);
  display.console = false;
}

FLASHMEM void virtual_keyboard() {
  int offcount = 0;
  uint8_t oct_count = 0;
  display.setTextColor(COLOR_BACKGROUND, COLOR_SYSTEXT);
  display.setTextSize(1);
  display.console = true;
  if (LCDML.FUNC_getID() == LCDML.OTHER_getIDFromFunction(UI_func_voice_select))
    display.fillRect(0, DISPLAY_HEIGHT - 18, 1, 18, GREY3);  //clear 1 column of pixels from previous displayed help text

  //draw white keys
  for (uint8_t x = 0; x < 10; x++) {
    display.console = true;
    display.fillRect(1 + x * 32.22, VIRT_KEYB_YPOS, 29.33, 73.5, COLOR_SYSTEXT);  // WHITE key
    display.console = false;
    if (x == 0 || x == 7 || x == 14) {
      display.setCursor(1 + x * 32.22 + 11.3, VIRT_KEYB_YPOS + 57.75);
      display.print("C");
      display.print(ts.virtual_keyboard_octave + oct_count);
      oct_count++;
    }
  }
  for (uint8_t x = 0; x < 16; x++) {
    if (seq.piano[x] == 1) {
      display.console = true;
      display.fillRect(x * 18.56, VIRT_KEYB_YPOS, 21.33, 34.5, COLOR_BACKGROUND);  // BLACK key
      display.console = false;
      offcount++;
      if (offcount == 5) offcount = 0;
    }
  }
  display.setTextSize(2);
  display.console = false;
}

FLASHMEM void print_virtual_keyboard_octave() {
  uint8_t oct_count = 0;
  display.setTextColor(COLOR_BACKGROUND, COLOR_SYSTEXT);
  display.setTextSize(1);

  //draw white keys
  for (uint8_t x = 0; x < 10; x++) {
    if (x == 0 || x == 7 || x == 14) {
      display.setCursor(1 + x * 32.22 + 11.3, VIRT_KEYB_YPOS + 57.75);
      display.print("C");
      display.print(ts.virtual_keyboard_octave + oct_count);
      oct_count++;
    }
  }
  //display.setTextSize(2);
}

bool check_button_on_grid(uint8_t x, uint8_t y) {

  if (ts.p.x > x * CHAR_width_small && ts.p.x < (x + button_size_x) * CHAR_width_small && ts.p.y > y * CHAR_height_small && ts.p.y < (y + button_size_y) * CHAR_height_small)
    return true;
  else return false;
}

FLASHMEM void touch_button_oct_up() {
  ts.virtual_keyboard_octave++;
  if (ts.virtual_keyboard_octave > 8) ts.virtual_keyboard_octave = 8;
  ts.update_virtual_keyboard_octave = true;
}
FLASHMEM void touch_button_oct_down() {
  ts.virtual_keyboard_octave--;
  if (ts.virtual_keyboard_octave < 1) ts.virtual_keyboard_octave = 1;
  ts.update_virtual_keyboard_octave = true;
}
FLASHMEM void touch_button_inst_up() {
  ts.virtual_keyboard_instrument++;
  if (ts.virtual_keyboard_instrument > 12) ts.virtual_keyboard_instrument = 12;
  virtual_keyboard_print_current_instrument();
  ts.update_virtual_keyboard_octave = true;
}
FLASHMEM void touch_button_inst_down() {
  ts.virtual_keyboard_instrument--;
  if (ts.virtual_keyboard_instrument < 1) ts.virtual_keyboard_instrument = 1;
  virtual_keyboard_print_current_instrument();
  ts.update_virtual_keyboard_octave = true;
}

FLASHMEM void touch_check_all_keyboard_buttons() {
  if (check_button_on_grid(1, 16))
    touch_button_oct_down();
  else if (check_button_on_grid(45, 16))
    touch_button_oct_up();

  if ((check_button_on_grid(9, 16) && seq.cycle_touch_element == 1) || (check_button_on_grid(9, 16) && ts.keyb_in_menu_activated))
    touch_button_inst_down();
  else if ((check_button_on_grid(37, 16) && seq.cycle_touch_element == 1) || (check_button_on_grid(37, 16) && ts.keyb_in_menu_activated))
    touch_button_inst_up();
}

FLASHMEM void virtual_keyboard_update_all_key_states() {
  ts.slowdown_UI_input++;
  if (ts.slowdown_UI_input > 7) {
    for (uint8_t x = 0; x < 10; x++) {
      if (ts.virtual_keyboard_state_white[x] > 0)
        ts.virtual_keyboard_state_white[x]--;
      if (ts.virtual_keyboard_state_white[x] == 1)
        virtual_keyboard_key_off_white(x);
    }
    for (uint8_t x = 0; x < 16; x++) {
      if (ts.virtual_keyboard_state_black[x] > 0)
        ts.virtual_keyboard_state_black[x]--;

      if (ts.virtual_keyboard_state_black[x] == 1)
        virtual_keyboard_key_off_black(x);
    }
    ts.slowdown_UI_input = 0;
  }
}

void get_scaled_touch_point() {
  LCDML.SCREEN_resetTimer();
  if (remote_touched == false) {
    ts.p = touch.getPoint();
    // Scale from ~0->4000 to tft
    ts.p.x = map(ts.p.x, 205, 3860, 0, TFT_HEIGHT);
    ts.p.y = map(ts.p.y, 310, 3720, 0, TFT_WIDTH);
  }
}

void handle_touchscreen_voice_select() {
  if (touch.touched()) {
    get_scaled_touch_point();
    if (check_button_on_grid(45, 1) && seq.generic_ui_delay > 1000) {

      if (seq.cycle_touch_element == 1) {
        border3_large_clear();
        seq.cycle_touch_element = 0;

        //display.drawRect(DISPLAY_WIDTH / 2, CHAR_height * 6 - 4 , DISPLAY_WIDTH / 2, DISPLAY_HEIGHT - 1,  GREY4);
        draw_button_on_grid(45, 1, "", "", 99);  //print keyboard icon
        print_voice_settings(true);
        print_perfmod_buttons();
        print_perfmod_lables();
        print_voice_select_default_help();
      } else {
        border3_large_clear();
        seq.cycle_touch_element = 1;
        draw_button_on_grid(45, 1, "DEXED", "DETAIL", 0);
        virtual_keyboard();

        virtual_keyboard_print_buttons();
        virtual_keyboard_print_current_instrument();
      }
      seq.generic_ui_delay = 0;
    }

    if (seq.cycle_touch_element != 1 && seq.generic_ui_delay > 2000) {
      if (check_button_on_grid(37, 1)) {
        save_favorite(configuration.dexed[selected_instance_id].bank, configuration.dexed[selected_instance_id].voice, selected_instance_id);
      } else

        if (selected_instance_id == 0) {
        if (check_button_on_grid(2, 25)) {
          if (dexed_live_mod.active_button != 1)
            dexed_live_mod.active_button = 1;
          else
            dexed_live_mod.active_button = 0;
        } else if (check_button_on_grid(11, 25)) {
          if (dexed_live_mod.active_button != 2)
            dexed_live_mod.active_button = 2;
          else
            dexed_live_mod.active_button = 0;
        }
      } else if (selected_instance_id == 1) {
        if (check_button_on_grid(2, 25)) {
          if (dexed_live_mod.active_button != 3)
            dexed_live_mod.active_button = 3;
          else
            dexed_live_mod.active_button = 0;
        } else if (check_button_on_grid(11, 25)) {
          if (dexed_live_mod.active_button != 4)
            dexed_live_mod.active_button = 4;
          else
            dexed_live_mod.active_button = 0;
        }
      } else
        dexed_live_mod.active_button = 0;

      if (dexed_live_mod.active_button > 0 && dexed_live_mod.active_button < 5) {
        helptext_r("< > CHANGE MODIFIER VALUE");
        display.setCursor(0, DISPLAY_HEIGHT - (CHAR_height_small * 2) - 2);
        print_empty_spaces(38);
        display.setCursor(9 * CHAR_width_small, DISPLAY_HEIGHT - CHAR_height_small * 1);
        print_empty_spaces(9);
        display.setCursor(CHAR_width_small * 38 + 2, DISPLAY_HEIGHT - (CHAR_height_small * 2) - 2);
        display.print(F(" PUSH TO RETURN"));
      } else {
        print_voice_select_default_help();
      }
      print_voice_settings(false);

      print_perfmod_buttons();
      print_perfmod_lables();
      seq.generic_ui_delay = 0;
    }


    if (ts.update_virtual_keyboard_octave == false && seq.cycle_touch_element == 1) {
      touch_check_all_keyboard_buttons();
    }

    if (ts.p.x > 1 && ts.p.y > VIRT_KEYB_YPOS && seq.cycle_touch_element == 1) {
      virtual_keyboard_key_on();
    }
  }
  if (touch.touched() == false) {
    if (ts.update_virtual_keyboard_octave && seq.cycle_touch_element == 1) {
      print_virtual_keyboard_octave();
      ts.update_virtual_keyboard_octave = false;
    }
    //    if ( ts.switch_active_instance && seq.cycle_touch_element != 1)
    //    {
    //      UI_update_instance_icons();
    //      print_voice_settings(CHAR_width_small, 104, selected_instance_id, 0);
    //    }
  }
  seq.generic_ui_delay++;
  virtual_keyboard_update_all_key_states();
}

FLASHMEM void handle_touchscreen_pattern_editor() {
  if (touch.touched()) {
    get_scaled_touch_point();

    if (check_button_on_grid(36, 1) && seq.generic_ui_delay > 12000 && seq.running == false) {
      seq.note_in = 0;
      seq.step_recording = !seq.step_recording;
      if (seq.step_recording)
        draw_button_on_grid(36, 1, "RECORD", "ACTIVE", 1);  //print step recorder icon
      else
        draw_button_on_grid(36, 1, "STEP", "RECORD", 2);  //print step recorder icon
      seq.generic_ui_delay = 0;
    } else if (check_button_on_grid(45, 1) && seq.generic_ui_delay > 12000) {
      border3_large();
      border3_large_clear();
      if (seq.cycle_touch_element == 1) {
        seq.cycle_touch_element = 0;
        draw_button_on_grid(45, 1, "", "", 99);  //print keyboard icon
        display.fillRect(0, CHAR_height_small * 12 + 1, DISPLAY_WIDTH, 1, COLOR_BACKGROUND);
        seq_pattern_editor_update_dynamic_elements();
      } else {
        seq.cycle_touch_element = 1;
        draw_button_on_grid(45, 1, "BACK", "TO SEQ", 0);
        display.fillRect(216, CHAR_height_small * 6, 95, CHAR_height_small * 6 + 1, COLOR_BACKGROUND);
        display.fillRect(0, CHAR_height_small * 10 + 1, 195, CHAR_height_small * 2 + 1, COLOR_BACKGROUND);
        virtual_keyboard();
        virtual_keyboard_print_buttons();
        virtual_keyboard_print_current_instrument();
      }
      seq.generic_ui_delay = 0;
    } else if (check_button_on_grid(36, 16) && seq.generic_ui_delay > 12000)  // toggle seq. playmode song/pattern only
    {
      if (seq.play_mode)
        seq.play_mode = false;
      else
        seq.play_mode = true;

      if (seq.play_mode == false)  // is in full song more
      {
        draw_button_on_grid(36, 16, "PLAYNG", "SONG", 0);
        seq.hunt_pattern = false;
        draw_button_on_grid(45, 22, "HUNT", "PATT", 0);
      } else  // play only current pattern
        draw_button_on_grid(36, 16, "LOOP", "PATT", 2);
      seq.generic_ui_delay = 0;
    } else if (check_button_on_grid(45, 22) && seq.generic_ui_delay > 12000)  // hunt pattern
    {
      if (seq.hunt_pattern)
        seq.hunt_pattern = false;
      else
        seq.hunt_pattern = true;

      if (seq.hunt_pattern == false)
        draw_button_on_grid(45, 22, "HUNT", "PATT", 0);
      else  // play only current pattern
        draw_button_on_grid(45, 22, "HUNT", "PATT", 2);
      seq.generic_ui_delay = 0;
    } else if (check_button_on_grid(36, 22) && seq.generic_ui_delay > 12000)  // jump song editor
    {
      LCDML.OTHER_jumpToFunc(UI_func_song);
      seq.generic_ui_delay = 0;
    } else if (LCDML.FUNC_getID() == LCDML.OTHER_getIDFromFunction(UI_func_seq_pattern_editor))
      if (check_button_on_grid(45, 16) && seq.generic_ui_delay > 12000)  // jump pattern editor functions
      {

        if (seq.content_type[seq.active_pattern] == 0) {
          draw_button_on_grid(45, 16, "JUMP", "TOOLS", 2);
          activesample = NUM_DRUMSET_CONFIG;
          seq.menu = 0;
          seq.active_function = 0;
          pattern_editor_menu_0();
        } else {
          draw_button_on_grid(45, 16, "JUMP", "TOOLS", 2);
          temp_int = 111;
          seq.menu = 0;
          seq.active_function = 0;
          pattern_editor_menu_0();
        }
        seq.generic_ui_delay = 0;
      }

    if (ts.update_virtual_keyboard_octave == false && seq.cycle_touch_element == 1) {
      touch_check_all_keyboard_buttons();
    }
    if (ts.p.x > 1 && ts.p.y > VIRT_KEYB_YPOS && seq.cycle_touch_element == 1) {
      virtual_keyboard_key_on();
    }
  }
  if (touch.touched() == false) {
    if (ts.update_virtual_keyboard_octave && seq.cycle_touch_element == 1) {
      print_virtual_keyboard_octave();
      ts.update_virtual_keyboard_octave = false;
    }
  }
  seq.generic_ui_delay++;
  virtual_keyboard_update_all_key_states();
}

FLASHMEM void handle_touchscreen_microsynth() {
  if (touch.touched()) {
    get_scaled_touch_point();

    if (check_button_on_grid(45, 1) && seq.generic_ui_delay > 12000) {
      display.fillRect(0, VIRT_KEYB_YPOS - 6 * CHAR_height_small, DISPLAY_WIDTH, DISPLAY_HEIGHT, COLOR_BACKGROUND);
      if (seq.cycle_touch_element == 1) {
        seq.cycle_touch_element = 0;
        draw_button_on_grid(45, 1, "", "", 99);  //print keyboard icon
        microsynth_refresh_lower_screen_static_text();
        microsynth_refresh_lower_screen_dynamic_text();
      } else {
        seq.cycle_touch_element = 1;
        draw_button_on_grid(45, 1, "MORE", "PARAM.", 0);
        virtual_keyboard();
        virtual_keyboard_print_buttons();
        virtual_keyboard_print_current_instrument();
      }
      seq.generic_ui_delay = 0;
    }

    if (ts.update_virtual_keyboard_octave == false && seq.cycle_touch_element == 1) {
      touch_check_all_keyboard_buttons();
    }

    if (ts.p.x > 1 && ts.p.y > VIRT_KEYB_YPOS && seq.cycle_touch_element == 1) {
      virtual_keyboard_key_on();
    }
  }
  if (touch.touched() == false) {
    if (ts.update_virtual_keyboard_octave && seq.cycle_touch_element == 1) {
      print_virtual_keyboard_octave();
      ts.update_virtual_keyboard_octave = false;
    }
  }
  seq.generic_ui_delay++;
  virtual_keyboard_update_all_key_states();
}

FLASHMEM void print_file_manager_buttons() {
  if (fm.sd_mode == 0)
    draw_button_on_grid(1, 25, "BROWSE", "FILES", 1);
  else
    draw_button_on_grid(1, 25, "BROWSE", "FILES", 0);
  if (fm.sd_mode == 1)
    draw_button_on_grid(11, 25, "DELETE", "FILE", 1);
  else
    draw_button_on_grid(11, 25, "DELETE", "FILE", 0);
  if (fm.sd_mode == 2)
    draw_button_on_grid(41, 25, "PLAY", "SAMPLE", 1);
  else
    draw_button_on_grid(41, 25, "PLAY", "SAMPLE", 0);
  if (fm.sd_mode == 3)
    draw_button_on_grid(31, 25, "COPY >", "FLASH", 1);
  else
    draw_button_on_grid(31, 25, "COPY >", "FLASH", 0);
  if (fm.sd_mode == 4)
    draw_button_on_grid(21, 25, "COPY", "PRESET", 1);
  else
    draw_button_on_grid(21, 25, "COPY", "PRESET", 0);

  // active_window   0 = left window (SDCARD) , 1 = FLASH

  if (fm.active_window == 0) {
    display.drawRect(CHAR_width_small * 29 - 1, 0, CHAR_width_small * 24 + 3, CHAR_height_small * 23, GREY2);
    display.drawRect(0, 0, CHAR_width_small * 29, CHAR_height_small * 23, COLOR_SYSTEXT);
  } else {
    display.drawRect(0, 0, CHAR_width_small * 29, CHAR_height_small * 23, GREY2);
    display.drawRect(CHAR_width_small * 29 - 1, 0, CHAR_width_small * 24 + 3, CHAR_height_small * 23, COLOR_SYSTEXT);
  }
}
FLASHMEM void handle_touchscreen_file_manager() {
  if (touch.touched()) {
    get_scaled_touch_point();

    // check touch buttons

    if (ts.p.y > CHAR_height_small * 24) {
      if (check_button_on_grid(1, 25)) {
        fm.sd_mode = 0;  // browse files/directories
      }
      if (check_button_on_grid(11, 25)) {
        fm.sd_mode = 1;  // delete
      }
      if (check_button_on_grid(41, 25)) {
        fm.sd_mode = 2;  //play

        if (fm.sd_mode == 2) {
          if (fm.sd_is_folder == false) {
            if (fm.sd_mode == 2 && ts.block_screen_update == false)  //preview
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
      } else if (check_button_on_grid(31, 25)) {
        fm.sd_mode = 3;  //copy to flash
      } else if (check_button_on_grid(21, 25)) {
        fm.sd_mode = 4;  //copy preset samples to flash
      }
    }
    // active_window   0 = left window (SDCARD) , 1 = FLASH
    else if (ts.p.x > 1 && ts.p.y > 1 && ts.p.x < CHAR_width_small * 29 && ts.p.y < CHAR_height_small * 24) {
      fm.active_window = 0;
    } else if (ts.p.x > CHAR_width_small * 29 && ts.p.y > 1 && ts.p.y < CHAR_height_small * 24) {
      fm.active_window = 1;
    }
    print_file_manager_buttons();
    sd_printDirectory(fm.sd_currentDirectory);

#ifdef COMPILE_FOR_FLASH
    flash_printDirectory();
#endif
#ifdef COMPILE_FOR_QSPI
    flash_printDirectory(fm.flash_currentDirectory);
#endif
  }
  ts.slowdown_UI_input++;
  if (ts.slowdown_UI_input > 5)
    ts.block_screen_update = false;
}

FLASHMEM void update_midi_learn_button() {
  if (seq.midi_learn_active == true) {
    draw_button_on_grid(45, 1, "MIDI", "LEARN", 2);  //RED button
  } else {
    draw_button_on_grid(45, 1, "MIDI", "LEARN", 0);
    // seq.midi_learn_active = false;
  }
}

FLASHMEM void handle_touchscreen_custom_mappings() {
  if (touch.touched() && ts.block_screen_update == false) {
    get_scaled_touch_point();
    if (check_button_on_grid(45, 1)) {
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

FLASHMEM void handle_touchscreen_cc_mappings() {
  if (touch.touched() && ts.block_screen_update == false) {
    get_scaled_touch_point();
    if (check_button_on_grid(45, 1)) {
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

FLASHMEM void handle_touchscreen_color_edit() {
  if (touch.touched()) {
    get_scaled_touch_point();

    if (ts.p.x > 170 && ts.p.x < 170 + 36 && ts.p.y < DISPLAY_HEIGHT - CHAR_height)
      if (ts.p.y * 1.22 > 359)
        ts.temp_col_hue = 359;
      else
        ts.temp_col_hue = ts.p.y * 1.22;

    else if (ts.p.x > 226 && ts.p.x < 226 + 36 && ts.p.y < DISPLAY_HEIGHT - CHAR_height) {
      if (ts.p.y * 1.2 > 234)
        ts.temp_col_sat = 255;
      else
        ts.temp_col_sat = ts.p.y * 1.2;

    } else if (ts.p.x > 283 && ts.p.x < 283 + 36 && ts.p.y < DISPLAY_HEIGHT - CHAR_height) {
      if (ts.p.y * 1.2 > 234)
        ts.temp_col_bright = 255;
      else
        ts.temp_col_bright = ts.p.y * 1.2;
    }
    colors_screen_update();
  }
}

FLASHMEM void handle_touchscreen_mute_matrix() {
  if (touch.touched()) {
    get_scaled_touch_point();
    uint8_t button_count = 0;
    char buf[4];
    for (uint8_t y = 0; y < 3; y++) {
      for (uint8_t x = 0; x < 4; x++) {
        if (y < 2) {
          if (check_button_on_grid(2 + x * 14, 12 + y * 8) && seq.generic_ui_delay > 3000) {
            seq.track_mute[button_count] = !seq.track_mute[button_count];
            if (!seq.track_mute[button_count])
              draw_button_on_grid(2 + x * 14, 12 + y * 8, "TRACK:", itoa(button_count + 1, buf, 10), 1);
            else
              draw_button_on_grid(2 + x * 14, 12 + y * 8, "TRACK:", itoa(button_count + 1, buf, 10), 0);
            seq.generic_ui_delay = 0;
          }
          button_count++;
        } else {
          if (check_button_on_grid(2 + x * 14, 4) && seq.generic_ui_delay > 3000) {
            if (x == 1)
              seq.mute_mode = 0;
            else if (x == 2)
              seq.mute_mode = 1;
            else if (x == 3)
              seq.mute_mode = 2;

            seq.generic_ui_delay = 0;
          }
          if (x == 1) {
            if (seq.mute_mode == 0)
              draw_button_on_grid(2 + x * 14, 4, "REAL", "TIME", 1);
            else
              draw_button_on_grid(2 + x * 14, 4, "REAL", "TIME", 0);
          } else if (x == 2) {
            if (seq.mute_mode == 1)
              draw_button_on_grid(2 + x * 14, 4, "NEXT", "PATTRN", 1);
            else
              draw_button_on_grid(2 + x * 14, 4, "NEXT", "PATTRN", 0);
          } else if (x == 3) {
            if (seq.mute_mode == 2)
              draw_button_on_grid(2 + x * 14, 4, "SONG", "STEP", 1);
            else
              draw_button_on_grid(2 + x * 14, 4, "SONG", "STEP", 0);
          }
        }
      }
    }
  }
  seq.generic_ui_delay++;
}

FLASHMEM void handle_touchscreen_arpeggio() {
  if (touch.touched()) {
    get_scaled_touch_point();
    if (check_button_on_grid(2, 16) && seq.generic_ui_delay > 3000) {
      if (seq.running)
        handleStop();
      else
        handleStart();

      seq.generic_ui_delay = 0;
    }
  }
  seq.generic_ui_delay++;
}

FLASHMEM void handle_touchscreen_braids() {
  if (touch.touched()) {
    get_scaled_touch_point();
    seq.cycle_touch_element = 1;
    seq.generic_ui_delay = 0;
    if (ts.update_virtual_keyboard_octave == false && seq.cycle_touch_element == 1) {
      touch_check_all_keyboard_buttons();
    }
    if (ts.p.x > 1 && ts.p.y > VIRT_KEYB_YPOS && seq.cycle_touch_element == 1) {
      virtual_keyboard_key_on();
    }
  }
  if (touch.touched() == false) {
    if (ts.update_virtual_keyboard_octave && seq.cycle_touch_element == 1) {
      print_virtual_keyboard_octave();
      ts.update_virtual_keyboard_octave = false;
    }
  }
  seq.generic_ui_delay++;
  virtual_keyboard_update_all_key_states();
}

FLASHMEM void draw_menu_ui_icons() {
  draw_button_on_grid(2, 18, "DEXED", "VOICE", 0);
  draw_button_on_grid(13, 18, "SONG", "EDIT", 0);
  draw_button_on_grid(24, 18, "PATT", "EDIT", 0);
  if (seq.running)
    draw_button_on_grid(45, 18, "SEQ.", "STOP", 2);
  else
    draw_button_on_grid(45, 18, "SEQ.", "START", 1);
  draw_button_on_grid(13, 25, "LOAD", "PERF", 0);
  draw_button_on_grid(24, 25, "SAVE", "PERF", 0);
  if (multiband_active)
    draw_button_on_grid(35, 25, "MULTI", "BAND", 2);
  else
    draw_button_on_grid(35, 25, "MULTI", "BAND", 0);
  draw_button_on_grid(45, 25, "MAIN", "MIX", 0);
  draw_button_on_grid(45, 11, "", "", 99);  //print keyboard icon
}

FLASHMEM void handle_touchscreen_menu() {
  if (ts.touch_ui_drawn_in_menu == false) {
    if (ts.keyb_in_menu_activated) {
      display.console = true;
      display.fillRect(0, VIRT_KEYB_YPOS - 38, DISPLAY_WIDTH, DISPLAY_HEIGHT - VIRT_KEYB_YPOS + 38, COLOR_BACKGROUND);
      display.console = false;
      virtual_keyboard();
      virtual_keyboard_print_buttons();
      virtual_keyboard_print_current_instrument();
      draw_button_on_grid(45, 11, "KEYB", "OFF", 0);
      seq.generic_ui_delay = 0;
    } else {
      draw_menu_ui_icons();
      seq.generic_ui_delay = 0;
    }
    ts.touch_ui_drawn_in_menu = true;
  }
  if (touch.touched()) {
    get_scaled_touch_point();

    if (seq.generic_ui_delay > 1500) {
      if (check_button_on_grid(45, 11)) {
        display.console = true;
        display.fillRect(0, VIRT_KEYB_YPOS - 38, DISPLAY_WIDTH, DISPLAY_HEIGHT - VIRT_KEYB_YPOS + 38, COLOR_BACKGROUND);
        display.console = false;
        ts.keyb_in_menu_activated = !ts.keyb_in_menu_activated;
        if (ts.keyb_in_menu_activated) {
          virtual_keyboard();
          virtual_keyboard_print_buttons();
          virtual_keyboard_print_current_instrument();
          draw_button_on_grid(45, 11, "KEYB", "OFF", 0);
        } else {
          draw_menu_ui_icons();
        }
      }
      if (ts.keyb_in_menu_activated == false) {
        if (check_button_on_grid(2, 18)) {
          LCDML.OTHER_jumpToFunc(UI_func_voice_select);
        } else if (check_button_on_grid(13, 18)) {
          LCDML.OTHER_jumpToFunc(UI_func_song);
        } else if (check_button_on_grid(24, 18)) {
          LCDML.OTHER_jumpToFunc(UI_func_seq_pattern_editor);
        } else if (check_button_on_grid(45, 18)) {
          if (seq.running) {
            handleStop();
            draw_button_on_grid(45, 18, "SEQ.", "START", 1);
          } else {
            handleStart();
            draw_button_on_grid(45, 18, "SEQ.", "STOP", 2);
          }
        } else if (check_button_on_grid(13, 25)) {
          display.setTextSize(2);
          display.setTextColor(COLOR_SYSTEXT, COLOR_BACKGROUND);
          LCDML.OTHER_jumpToFunc(UI_func_load_performance);
        } else if (check_button_on_grid(24, 25)) {
          display.setTextSize(2);
          display.setTextColor(COLOR_SYSTEXT, COLOR_BACKGROUND);
          LCDML.OTHER_jumpToFunc(UI_func_save_performance);

        } else if (check_button_on_grid(45, 25)) {
          LCDML.OTHER_jumpToFunc(UI_func_mixer);
        } else if (check_button_on_grid(35, 25)) {
          multiband_active = !multiband_active;
          if (multiband_active)
            draw_button_on_grid(35, 25, "MULTI", "BAND", 2);
          else
            draw_button_on_grid(35, 25, "MULTI", "BAND", 0);
          mb_set_mutes();
          mb_set_master();
          mb_set_compressor();
        }
      }
      seq.generic_ui_delay = 0;
    }
    if (ts.update_virtual_keyboard_octave == false && ts.keyb_in_menu_activated) {
      touch_check_all_keyboard_buttons();
    }
    if (ts.p.x > 1 && ts.p.y > VIRT_KEYB_YPOS && ts.keyb_in_menu_activated) {
      virtual_keyboard_key_on();
    }
    ts.touch_ui_drawn_in_menu = true;
  }
  if (touch.touched() == false) {
    if (ts.update_virtual_keyboard_octave && ts.keyb_in_menu_activated) {
      print_virtual_keyboard_octave();
      ts.update_virtual_keyboard_octave = false;
    }
  }
  seq.generic_ui_delay++;
  if (ts.keyb_in_menu_activated)
    virtual_keyboard_update_all_key_states();
  display.setTextSize(2);
  display.setTextColor(COLOR_SYSTEXT, COLOR_BACKGROUND);
}

#ifdef USE_MULTIBAND
FLASHMEM void toggle_generic_active_function() {
  if (generic_active_function == 0)
    generic_active_function = 1;
  else
    generic_active_function = 0;
}
#endif

#ifdef USE_MULTIBAND
FLASHMEM void handle_touchscreen_multiband() {
  if (touch.touched()) {
    get_scaled_touch_point();

    if (seq.generic_ui_delay > 5000 && multiband_active) {
      if (check_button_on_grid(12, 8)) {
        mb_solo_high = !mb_solo_high;
        if (mb_solo_high) {
          draw_button_on_grid(9, 8, "SOLO", "ON", mb_solo_high + 1);
        } else
          draw_button_on_grid(9, 8, "SOLO", "  ", mb_solo_high);
      } else if (check_button_on_grid(12, 14)) {
        mb_solo_upper_mid = !mb_solo_upper_mid;
        if (mb_solo_upper_mid) {
          draw_button_on_grid(9, 14, "SOLO", "ON", mb_solo_upper_mid + 1);
        } else
          draw_button_on_grid(9, 14, "SOLO", "  ", mb_solo_upper_mid);
      } else if (check_button_on_grid(12, 20)) {
        mb_solo_mid = !mb_solo_mid;
        if (mb_solo_mid) {
          draw_button_on_grid(9, 20, "SOLO", "ON", mb_solo_mid + 1);
        } else
          draw_button_on_grid(9, 20, "SOLO", "  ", mb_solo_mid);
      } else if (check_button_on_grid(12, 26)) {
        mb_solo_low = !mb_solo_low;
        if (mb_solo_low) {
          draw_button_on_grid(9, 26, "SOLO", "ON", mb_solo_low + 1);
        } else
          draw_button_on_grid(9, 26, "SOLO", "  ", mb_solo_low);
      }
      if (mb_solo_low && mb_solo_upper_mid && mb_solo_mid && mb_solo_high) {
        mb_solo_low = false;
        mb_solo_mid = false;
        mb_solo_upper_mid = false;
        mb_solo_high = false;
        draw_button_on_grid(9, 8, "SOLO", "   ", mb_solo_high);
        draw_button_on_grid(9, 14, "SOLO", "   ", mb_solo_upper_mid);
        draw_button_on_grid(9, 20, "SOLO", "   ", mb_solo_mid);
        draw_button_on_grid(9, 26, "SOLO", "   ", mb_solo_low);
      }
      mb_set_mutes();

      if (check_button_on_grid(38, 8)) {
        toggle_generic_active_function();
        generic_temp_select_menu = 7;

      } else if (check_button_on_grid(38, 14)) {
        toggle_generic_active_function();
        generic_temp_select_menu = 12;

      } else if (check_button_on_grid(38, 20)) {
        toggle_generic_active_function();
        generic_temp_select_menu = 17;

      } else if (check_button_on_grid(38, 26)) {
        toggle_generic_active_function();
        generic_temp_select_menu = 22;
      }
      seq.generic_ui_delay = 0;
    }
  }
  if (touch.touched() == false) {
    ;
  }
  seq.generic_ui_delay++;
  display.setTextSize(1);
  display.setTextColor(COLOR_SYSTEXT, COLOR_BACKGROUND);
  if (scope.scope_delay % 60 == 0) {

    float l, r;
    l = mb_before_l.read();
    r = mb_before_r.read();
    draw_volmeter(CHAR_width_small * 1, 228, 0, l);
    draw_volmeter(CHAR_width_small * 5, 228, 1, r);

    if (multiband_active) {
      draw_volmeter(DISPLAY_WIDTH - CHAR_width_small * 8 + 2, 228, 2, mb_after_l.read());
      draw_volmeter(DISPLAY_WIDTH - CHAR_width_small * 4 + 2, 228, 3, mb_after_r.read());
    } else {
      draw_volmeter(DISPLAY_WIDTH - CHAR_width_small * 8 + 2, 228, 2, l);
      draw_volmeter(DISPLAY_WIDTH - CHAR_width_small * 4 + 2, 228, 3, r);
    }
  }
  display.setTextSize(2);
  display.setTextColor(COLOR_SYSTEXT, COLOR_BACKGROUND);
}
#else
FLASHMEM void handle_touchscreen_multiband() {
  ;
}
#endif

extern int temp_int;

FLASHMEM void handle_touchscreen_sample_editor() {
  if (touch.touched()) {
    get_scaled_touch_point();

    if (seq.generic_ui_delay > 10) {
      if (check_button_on_grid(45, 23) && fm.sample_preview_playing == false) {
        draw_button_on_grid(45, 23, "PLAY", "SAMPLE", 2);

        if (fm.sample_source == 1)  //FLASH
        {
          char filename[26];
          uint32_t filesize;
          SerialFlash.opendir();
          if (temp_int > 0) {
            for (int f = 0; f < temp_int; f++) {
              if (SerialFlash.readdir(filename, sizeof(filename), filesize))
                ;
              else
                break;
            }
          }

          fm.sample_screen_position_x = 0;
          fm.sample_preview_playing = true;
          playWAVFile(filename);
        }
      }
      seq.generic_ui_delay = 0;
    }
  }
  if (touch.touched() == false) {
    ;
  }
  seq.generic_ui_delay++;
}

FLASHMEM void handle_touchscreen_settings_button_test() {
  static bool button_state = false;

  if (touch.touched()) {
    get_scaled_touch_point();
    if (seq.generic_ui_delay > 50) {
      if (check_button_on_grid(42, 1)) {
        draw_button_on_grid(42, 1, "TOUCH", button_state ? "OK" : "TEST", button_state ? 2 : 0);
        button_state = !button_state;
      }
      seq.generic_ui_delay = 0;
    }
  }
  seq.generic_ui_delay++;
}

FLASHMEM void handle_touchscreen_test_touchscreen() {
  if (touch.touched()) {
    get_scaled_touch_point();
    if (seq.generic_ui_delay > 900) {
      if (check_button_on_grid(42, 1))
        sub_touchscreen_test_page_init();
      seq.generic_ui_delay = 0;
    }
display.console=true;
    display.fillRect(ts.p.x, ts.p.y,2,2, COLOR_SYSTEXT);
    display.console=false;

  }
  seq.generic_ui_delay++;
}