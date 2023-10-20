#include "config.h"
#include "touch.h"
#include "sequencer.h"
#include <LCDMenuLib2.h>
#include "ILI9341_t3n.h"
#include "scope.h"
#include <Audio.h>
#include "template_mixer.hpp"

extern ILI9341_t3n display;
extern config_t configuration;
extern uint8_t selected_instance_id;
extern void handleNoteOn_MIDI_DEVICE_DIN(byte inChannel, byte inNumber, byte inVelocity);
extern void handleNoteOff_MIDI_DEVICE_DIN(byte inChannel, byte inNumber, byte inVelocity);
extern void print_voice_settings_in_dexed_voice_select(bool text, bool values);
extern void print_voice_settings_in_pattern_editor(int x, int y);
extern void UI_update_instance_icons();
extern LCDMenuLib2 LCDML;
extern void pattern_editor_menu_0();
extern void UI_func_test_touchscreen(uint8_t param);
extern void sub_touchscreen_test_page_init();
extern bool remote_touched;
extern sequencer_t seq;
extern void border3_large_clear();
extern void border3_large();
#ifdef COMPILE_FOR_FLASH
extern void flash_loadDirectory();
extern void flash_printDirectory();
#endif
extern void sd_printDirectory(bool forceReload);
extern uint8_t find_longest_chain();
extern void print_formatted_number(uint16_t v, uint8_t l);
extern void virtual_keyboard_print_buttons();
extern void draw_button_on_grid(uint8_t x, uint8_t y, const char* t1, const char* t2, uint8_t color);
extern void seq_pattern_editor_update_dynamic_elements();
extern void microsynth_refresh_lower_screen_static_text();
extern void microsynth_refresh_lower_screen_dynamic_text();
extern float get_sample_p_offset(uint8_t sample);
extern void set_sample_pitch(uint8_t sample, float playbackspeed);
extern void show_no_grid(int pos_y, int pos_x, uint8_t field_size, const char* str);
extern const char* find_long_drum_name_from_note(uint8_t note);
extern void print_perfmod_buttons();
extern void print_perfmod_lables();
extern void print_empty_spaces(uint8_t spaces);
extern void print_voice_select_default_help();
extern void handleStop(void);
extern void handleStart(void);
extern void UI_func_load_performance(uint8_t param);
extern void UI_func_save_performance(uint8_t param);
extern void UI_func_seq_pattern_editor(uint8_t param);
extern void UI_func_seq_vel_editor(uint8_t param);
extern void UI_func_mixer(uint8_t param);
extern void UI_func_song(uint8_t param);
extern void UI_func_voice_select(uint8_t param);
extern void UI_func_voice_editor(uint8_t param);
extern void save_favorite(uint8_t p, uint8_t b, uint8_t v, uint8_t instance_id);
extern void preview_sample(void);
extern void draw_volmeters_mixer();
extern void draw_volmeters_multiband_compressor();


extern uint8_t activesample;
extern microsynth_t microsynth[NUM_MICROSYNTH];

extern braids_t braids_osc;
extern bool generic_full_draw_required;

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
extern uint8_t last_menu_depth;

ts_t ts;                         // touch screen
fm_t fm;                         // file manager
dexed_live_mod_t dexed_live_mod; // dexed quick live modifiers for attack and release
extern int temp_int;

bool isButtonTouched = false;
static constexpr int TOUCH_MAX_REFRESH_RATE_MS = 10; // 100Hz
elapsedMillis touchReadTimer;
int numTouchPoints = 0;

int getNumTouchPoints() {
  if (touchReadTimer > TOUCH_MAX_REFRESH_RATE_MS) {
    touchReadTimer = 0;
      if (remote_touched) {
        numTouchPoints = 1;
      } else {
        // no remote touch, so update to check for real touch
        numTouchPoints = touch.touched();
    }
  }
  return numTouchPoints;
}

FLASHMEM void helptext_l(const char* str)
{
  display.setTextSize(1);
  uint8_t l = strlen(str);

  display.setCursor(0, DISPLAY_HEIGHT - CHAR_height_small);
  display.setTextColor(COLOR_SYSTEXT, DX_DARKCYAN);
  display.print(str);

  if (l < ts.old_helptext_length[0])
  {
    display.setTextColor(COLOR_SYSTEXT, COLOR_BACKGROUND);
    for (uint8_t x = 0; x < ts.old_helptext_length[0] - l; x++)
      display.print(" ");
  }

  ts.old_helptext_length[0] = l;
}

FLASHMEM void helptext_r(const char* str)
{
  display.setTextSize(1);
  uint8_t l = strlen(str);
  display.setCursor(DISPLAY_WIDTH - CHAR_width_small * l, DISPLAY_HEIGHT - CHAR_height_small);
  display.setTextColor(COLOR_SYSTEXT, DX_DARKCYAN);
  display.print(str);
  if (l < ts.old_helptext_length[1])
  {
    display.setCursor(DISPLAY_WIDTH - CHAR_width_small * (ts.old_helptext_length[1]), DISPLAY_HEIGHT - CHAR_height_small);
    display.setTextColor(COLOR_SYSTEXT, COLOR_BACKGROUND);
    for (uint8_t x = 0; x < ts.old_helptext_length[1] - l; x++)
      display.print(" ");
  }
  ts.old_helptext_length[1] = l;
}

FLASHMEM void helptext_c(const char* str)
{
  display.setTextSize(1);
  uint8_t l = strlen(str);
  display.setCursor(DISPLAY_WIDTH / 2 - (l / 2) * CHAR_width_small, DISPLAY_HEIGHT - CHAR_height_small);
  display.setTextColor(COLOR_SYSTEXT, DX_DARKCYAN);
  display.print(str);
  if (l < ts.old_helptext_length[2])
  {
    display.setCursor(DISPLAY_WIDTH / 2 - (ts.old_helptext_length[2] / 2) * CHAR_width_small, DISPLAY_HEIGHT - CHAR_height_small);
    display.setTextColor(COLOR_SYSTEXT, COLOR_BACKGROUND);
    for (uint8_t x = 0; x < ts.old_helptext_length[2] - l; x++)
      display.print(" ");
  }
  ts.old_helptext_length[2] = l;
}

FLASHMEM uint16_t RGB24toRGB565(uint8_t r, uint8_t g, uint8_t b)
{
  return ((r / 8) << 11) | ((g / 4) << 5) | (b / 8);
}

FLASHMEM uint16_t ColorHSV(uint16_t hue, uint8_t sat, uint8_t val)
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
  return RGB24toRGB565(r, g, b);
  //  }
}

FLASHMEM void print_current_chord()
{
  for (uint8_t x = 0; x < 7; x++)
  {
    display.print(seq.chord_names[seq.arp_chord][x]);
  }
}

FLASHMEM void virtual_keyboard_print_velocity_bar()
{
  // velocity bar disabled
  if ((LCDML.FUNC_getID() == LCDML.OTHER_getIDFromFunction(UI_func_seq_pattern_editor) && seq.step_recording == false && seq.cycle_touch_element == 1) || (LCDML.FUNC_getID() == LCDML.OTHER_getIDFromFunction(UI_func_seq_vel_editor) && seq.step_recording == false && seq.cycle_touch_element == 1))
  {
    display.console = 1;
    display.fillRect(1, 10 * CHAR_height_small, DISPLAY_WIDTH, 32, COLOR_BACKGROUND);
    display.console = 0;
  }
  else if (seq.cycle_touch_element == 1)
  {
    // velocity bar enabled
    display.setTextColor(COLOR_SYSTEXT, COLOR_BACKGROUND);
    display.setTextSize(1);
    display.setCursor(1 * CHAR_width_small, 11 * CHAR_height_small + 1);
    display.print(F("VELOCITY"));
    display.setCursor(1 * CHAR_width_small, 12 * CHAR_height_small + 3);
    print_formatted_number(ts.virtual_keyboard_velocity, 3);
    display.console = 1;
    display.drawRect(CHAR_width_small * 10, 11 * CHAR_height_small, CHAR_width * 17 - CHAR_width_small * 10, 20, GREY1);
    display.console = 1;
    display.fillRect(CHAR_width_small * 10 + 1, 11 * CHAR_height_small + 2, ts.virtual_keyboard_velocity * 1.12, 16, COLOR_PITCHSMP);
    display.console = 1;
    display.fillRect(CHAR_width_small * 10 + 2 + ts.virtual_keyboard_velocity * 1.13, 11 * CHAR_height_small + 2, CHAR_width * 17 - CHAR_width_small * 10 - 3 - ts.virtual_keyboard_velocity * 1.13, 16, COLOR_BACKGROUND);
    display.console = 0;
  }
}

FLASHMEM void virtual_keyboard_print_current_instrument()
{
  display.setTextColor(GREY2, COLOR_BACKGROUND);
  display.setTextSize(2);
  display.setCursor(17 * CHAR_width_small, 16 * CHAR_height_small);
  if (ts.virtual_keyboard_instrument < 8)
    display.print(F("PLAYING"));
  else
  {
    display.setTextSize(1);
    display.print(F("PITCHED SAMPLE "));
    display.console = 1;
    display.fillRect(17 * CHAR_width_small, 17 * CHAR_height_small, 14 * CHAR_width_small, 8, COLOR_BACKGROUND);
    display.console = 0;
  }
  display.setTextSize(2);
  display.setCursor(17 * CHAR_width_small, 18 * CHAR_height_small + 1);
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
    ts.virtual_keyboard_midi_channel = microsynth[1].midi_channel;
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
  else if (ts.virtual_keyboard_instrument == 7)
  {
    display.print(F("BRAIDS   "));
    ts.virtual_keyboard_midi_channel = braids_osc.midi_channel;
  }
  else if (ts.virtual_keyboard_instrument > 7)
  {
    display.print(ts.virtual_keyboard_instrument - 7);
    display.print(" ");

    show_no_grid(18 * CHAR_height_small + 1, 20 * CHAR_width_small, 7, find_long_drum_name_from_note(ts.virtual_keyboard_instrument - 8 + 210));

    ts.virtual_keyboard_midi_channel = DRUM_MIDI_CHANNEL;
  }
}

FLASHMEM void virtual_keyboard_key_off_white(uint8_t note)
{
  uint8_t halftones = 0;
  display.setTextColor(COLOR_BACKGROUND, COLOR_SYSTEXT);
  display.setTextSize(1);

  // draw white keys

  for (uint8_t x = 0; x < 10; x++)
  {
    if (x == note)
    {
      for (uint8_t z = 0; z < x; z++)
      {
        if (seq.piano2[z] == 1)
          halftones = halftones + 1;
      }
      // handleNoteOff_MIDI_DEVICE_DIN(ts.virtual_keyboard_midi_channel, ts.virtual_keyboard_octave * 12 + note, 120);
      handleNoteOff_MIDI_DEVICE_DIN(ts.virtual_keyboard_midi_channel, ts.virtual_keyboard_octave * 12 + note + halftones, 0);
      display.console = true;
      display.fillRect(1 + x * 32.22, VIRT_KEYB_YPOS + 34, 29.33, 39, COLOR_SYSTEXT); // white key
      display.console = false;

      if (x == 0 || x == 7)
      {
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

FLASHMEM void virtual_keyboard_key_off_black(uint8_t note)
{
  display.setTextColor(COLOR_BACKGROUND, COLOR_SYSTEXT);
  display.setTextSize(1);

  for (uint8_t x = 0; x < 16; x++)
  {
    if (x == note)
    {
      //  handleNoteOff_MIDI_DEVICE_DIN(ts.virtual_keyboard_midi_channel, ts.virtual_keyboard_octave * 12 + note, 120);
      handleNoteOff_MIDI_DEVICE_DIN(ts.virtual_keyboard_midi_channel, ts.virtual_keyboard_octave * 12 + note, 0);
      display.console = true;
      display.fillRect(x * 18.56, VIRT_KEYB_YPOS, 21.33, 34.5, COLOR_BACKGROUND); // BLACK key
      display.console = false;
    }
  }
  display.setTextSize(2);
  display.setTextColor(COLOR_SYSTEXT, COLOR_BACKGROUND);
  display.console = false;
}

FLASHMEM void virtual_keyboard_key_on()
{
  uint8_t halftones = 0;
  display.setTextColor(COLOR_SYSTEXT);
  display.setTextSize(1);

  const int sustain = 5; // rather workaround for ++/--

  // draw white keys
  if (ts.p.y > VIRT_KEYB_YPOS + 36) {
    for (uint8_t x = 0; x < 10; x++) {
      bool isKeyPressed = ts.p.x > x * 32.22 && ts.p.x < x * 32.22 + 32;

      if (isKeyPressed) {

        if (ts.virtual_keyboard_state_white[x] == 0) {
          ts.virtual_keyboard_state_white[x] = sustain;

          for (uint8_t z = 0; z < x; z++) {
            if (seq.piano2[z] == 1) {
              halftones = halftones + 1;
            }
          }
          // handleNoteOn_MIDI_DEVICE_DIN(configuration.dexed[selected_instance_id].midi_channel, ts.virtual_keyboard_octave * 12 + x + halftones, 120);
          // pitched samples
          if (ts.virtual_keyboard_instrument > 7) {
            set_sample_pitch(ts.virtual_keyboard_instrument - 8, (float)pow(2, (ts.virtual_keyboard_octave * 12 + x + halftones - 72) / 12.00) * get_sample_p_offset(ts.virtual_keyboard_instrument - 7));
            if ((LCDML.FUNC_getID() == LCDML.OTHER_getIDFromFunction(UI_func_seq_pattern_editor) && seq.cycle_touch_element == 1) || (LCDML.FUNC_getID() == LCDML.OTHER_getIDFromFunction(UI_func_seq_vel_editor) && seq.cycle_touch_element == 1)) {
              handleNoteOn_MIDI_DEVICE_DIN(ts.virtual_keyboard_midi_channel, 210 + ts.virtual_keyboard_instrument - 8, ts.virtual_keyboard_velocity);
            }
            else {
              handleNoteOn_MIDI_DEVICE_DIN(ts.virtual_keyboard_midi_channel, 210 + ts.virtual_keyboard_instrument - 8, 100);
            }
          }
          else {
            if ((LCDML.FUNC_getID() == LCDML.OTHER_getIDFromFunction(UI_func_seq_pattern_editor) && seq.cycle_touch_element == 1) || (LCDML.FUNC_getID() == LCDML.OTHER_getIDFromFunction(UI_func_seq_vel_editor) && seq.cycle_touch_element == 1)) {
              handleNoteOn_MIDI_DEVICE_DIN(ts.virtual_keyboard_midi_channel, ts.virtual_keyboard_octave * 12 + x + halftones, ts.virtual_keyboard_velocity);
            }
            else {
              handleNoteOn_MIDI_DEVICE_DIN(ts.virtual_keyboard_midi_channel, ts.virtual_keyboard_octave * 12 + x + halftones, 120);
            }
            display.console = true;
            display.fillRect(1 + x * 32.22, VIRT_KEYB_YPOS + 34, 29.33, 39, RED); // white key
            display.console = false;
          }
        }
        ts.virtual_keyboard_state_white[x]++;
      }
    }
  }
  else if (ts.p.y > VIRT_KEYB_YPOS && ts.p.y < VIRT_KEYB_YPOS + 34) {
    for (uint8_t x = 0; x < 16; x++) {
      if (seq.piano[x] == 1) {
        bool isKeyPressed = ts.p.x > x * 18.46 && ts.p.x < x * 18.46 + 24;
        if (isKeyPressed) {

          if (ts.virtual_keyboard_state_black[x] == 0) {
            ts.virtual_keyboard_state_black[x] = sustain;

            // pitched samples
            if (ts.virtual_keyboard_instrument > 7) {
              set_sample_pitch(ts.virtual_keyboard_instrument - 8, (float)pow(2, (ts.virtual_keyboard_octave * 12 + x - 72) / 12.00) * get_sample_p_offset(ts.virtual_keyboard_instrument - 7));
              if ((LCDML.FUNC_getID() == LCDML.OTHER_getIDFromFunction(UI_func_seq_pattern_editor) && seq.cycle_touch_element == 1) || (LCDML.FUNC_getID() == LCDML.OTHER_getIDFromFunction(UI_func_seq_vel_editor) && seq.cycle_touch_element == 1)) {
                handleNoteOn_MIDI_DEVICE_DIN(ts.virtual_keyboard_midi_channel, 210 + ts.virtual_keyboard_instrument - 8, ts.virtual_keyboard_velocity);
              }
              else {
                handleNoteOn_MIDI_DEVICE_DIN(ts.virtual_keyboard_midi_channel, 210 + ts.virtual_keyboard_instrument - 8, 100);
              }
            }
            else {
              if ((LCDML.FUNC_getID() == LCDML.OTHER_getIDFromFunction(UI_func_seq_pattern_editor) && seq.cycle_touch_element == 1) || (LCDML.FUNC_getID() == LCDML.OTHER_getIDFromFunction(UI_func_seq_vel_editor) && seq.cycle_touch_element == 1)) {
                handleNoteOn_MIDI_DEVICE_DIN(ts.virtual_keyboard_midi_channel, ts.virtual_keyboard_octave * 12 + x, ts.virtual_keyboard_velocity);
              }
              else {
                handleNoteOn_MIDI_DEVICE_DIN(ts.virtual_keyboard_midi_channel, ts.virtual_keyboard_octave * 12 + x, 120);
              }
              display.console = true;
              display.fillRect(x * 18.56, VIRT_KEYB_YPOS, 21.33, 34.5, RED); // BLACK key
              display.console = false;
            }
          }
          ts.virtual_keyboard_state_black[x]++;
        }
      }
    }
  }

  // display.fillRect(ts.p.x-1,ts.p.y-1,3,3,YELLOW);
  display.setTextSize(2);
  display.setTextColor(COLOR_SYSTEXT, COLOR_BACKGROUND);
  display.console = false;
}



FLASHMEM void virtual_keyboard()
{
  uint8_t oct_count = 0;
  display.setTextColor(COLOR_BACKGROUND, COLOR_SYSTEXT);
  display.setTextSize(1);
  display.console = true;
  if (LCDML.FUNC_getID() == LCDML.OTHER_getIDFromFunction(UI_func_voice_select))
    display.fillRect(0, DISPLAY_HEIGHT - 18, 1, 18, GREY3); // clear 1 column of pixels from previous displayed help text

  if (LCDML.FUNC_getID() == LCDML.OTHER_getIDFromFunction(UI_func_seq_pattern_editor) && seq.cycle_touch_element == 1 && seq.step_recording)
    virtual_keyboard_print_velocity_bar();

  // draw white keys
  for (uint8_t x = 0; x < 10; x++)
  {
    display.console = true;
    display.fillRect(1 + x * 32.22, VIRT_KEYB_YPOS, 29.33, 73.5, COLOR_SYSTEXT); // WHITE key
    display.console = false;
    if (x == 0 || x == 7 || x == 14)
    {
      display.setCursor(1 + x * 32.22 + 11.3, VIRT_KEYB_YPOS + 57.75);
      display.setTextColor(COLOR_BACKGROUND, COLOR_SYSTEXT);
      display.print("C");
      display.print(ts.virtual_keyboard_octave + oct_count);
      oct_count++;
    }
  }
  for (uint8_t x = 0; x < 16; x++)
  {
    if (seq.piano[x] == 1)
    {
      display.console = true;
      display.fillRect(x * 18.56, VIRT_KEYB_YPOS, 21.33, 34.5, COLOR_BACKGROUND); // BLACK key
      display.console = false;
    }
  }
  display.setTextSize(2);
  display.console = false;
}

FLASHMEM void print_virtual_keyboard_octave()
{
  uint8_t oct_count = 0;
  display.setTextColor(COLOR_BACKGROUND, COLOR_SYSTEXT);
  display.setTextSize(1);

  // draw white keys
  for (uint8_t x = 0; x < 10; x++)
  {
    if (x == 0 || x == 7 || x == 14)
    {
      display.setCursor(1 + x * 32.22 + 11.3, VIRT_KEYB_YPOS + 57.75);
      display.print("C");
      display.print(ts.virtual_keyboard_octave + oct_count);
      oct_count++;
    }
  }
  // display.setTextSize(2);
}

FLASHMEM bool check_button_on_grid(uint8_t x, uint8_t y)
{
  bool result = false;
  if (ts.p.x > x * CHAR_width_small && ts.p.x < (x + button_size_x) * CHAR_width_small && ts.p.y > y * CHAR_height_small && ts.p.y < (y + button_size_y) * CHAR_height_small) {
    if (isButtonTouched == false) {
      isButtonTouched = true;
      result = true;
    }
  }
  return result;
}

FLASHMEM void touch_button_oct_up()
{
  ts.virtual_keyboard_octave++;
  if (ts.virtual_keyboard_octave > 8)
    ts.virtual_keyboard_octave = 8;
  ts.update_virtual_keyboard_octave = true;
}
FLASHMEM void touch_button_oct_down()
{
  ts.virtual_keyboard_octave--;
  if (ts.virtual_keyboard_octave < 1)
    ts.virtual_keyboard_octave = 1;
  ts.update_virtual_keyboard_octave = true;
}
FLASHMEM void touch_button_inst_up()
{
  ts.virtual_keyboard_instrument++;
  if (ts.virtual_keyboard_instrument > 12)
    ts.virtual_keyboard_instrument = 12;
  virtual_keyboard_print_current_instrument();
  ts.update_virtual_keyboard_octave = true;
}
FLASHMEM void touch_button_inst_down()
{
  ts.virtual_keyboard_instrument--;
  if (ts.virtual_keyboard_instrument < 1)
    ts.virtual_keyboard_instrument = 1;
  virtual_keyboard_print_current_instrument();
  ts.update_virtual_keyboard_octave = true;
}

FLASHMEM void touch_check_all_keyboard_buttons()
{
  if (check_button_on_grid(1, 16))
    touch_button_oct_down();
  else if (check_button_on_grid(45, 16))
    touch_button_oct_up();

  if ((check_button_on_grid(9, 16) && seq.cycle_touch_element == 1) || (check_button_on_grid(9, 16) && ts.keyb_in_menu_activated))
    touch_button_inst_down();
  else if ((check_button_on_grid(37, 16) && seq.cycle_touch_element == 1) || (check_button_on_grid(37, 16) && ts.keyb_in_menu_activated))
    touch_button_inst_up();
}

FLASHMEM void virtual_keyboard_update_all_key_states()
{
  for (uint8_t x = 0; x < 10; x++)
  {
    if (ts.virtual_keyboard_state_white[x] > 0)
      ts.virtual_keyboard_state_white[x]--;
    if (ts.virtual_keyboard_state_white[x] == 1)
      virtual_keyboard_key_off_white(x);
  }
  for (uint8_t x = 0; x < 16; x++)
  {
    if (ts.virtual_keyboard_state_black[x] > 0)
      ts.virtual_keyboard_state_black[x]--;

    if (ts.virtual_keyboard_state_black[x] == 1)
      virtual_keyboard_key_off_black(x);
  }

}

FLASHMEM void get_scaled_touch_point()
{
  LCDML.SCREEN_resetTimer();
  if (remote_touched == false)
  {
#if defined GENERIC_DISPLAY
    if (ts.finished_calibration)
    {
      ts.p = touch.getPixel();
      // ts.p.x = map(ts.p.x, ts.calib_x_min, ts.calib_x_max, 0, TFT_HEIGHT);
      // ts.p.y = map(ts.p.y, ts.calib_y_min, ts.calib_y_max, 0, TFT_WIDTH);
    }
    else
    {
      // Scale from ~0->4000 to tft
      ts.p = touch.getPoint();
      ts.p.x = map(ts.p.x, 205, 3860, 0, TFT_HEIGHT);
      ts.p.y = map(ts.p.y, 310, 3720, 0, TFT_WIDTH);
    }
#endif

#ifdef CAPACITIVE_TOUCH_DISPLAY
    // Retrieve a point
    TS_Point p = touch.getPoint();

    //touch rotation seems to have changed, this is working for my current setup:
    ts.p.x = p.y;
    ts.p.y = DISPLAY_HEIGHT - p.x;
#endif
  }
}

extern uint8_t dexed_onscreen_algo;

FLASHMEM void handle_touchscreen_voice_select()
{

  if (getNumTouchPoints() > 0)
  {
    get_scaled_touch_point();
    if (check_button_on_grid(45, 1))
    {
      if (seq.cycle_touch_element == 1)
      {
        border3_large_clear();
        seq.cycle_touch_element = 0;
        // display.drawRect(DISPLAY_WIDTH / 2, CHAR_height * 6 - 4 , DISPLAY_WIDTH / 2, DISPLAY_HEIGHT - 1,  GREY4);
        draw_button_on_grid(45, 1, "", "", 99); // print keyboard icon
        dexed_onscreen_algo = 88; //dummy value to force draw on screen init
        print_voice_settings_in_dexed_voice_select(true, true);
        print_perfmod_buttons();
        print_perfmod_lables();
        print_voice_select_default_help();
      }
      else
      {
        border3_large_clear();
        seq.cycle_touch_element = 1;
        draw_button_on_grid(45, 1, "DEXED", "DETAIL", 0);
        virtual_keyboard();

        virtual_keyboard_print_buttons();
        virtual_keyboard_print_current_instrument();
      }
    }
    if (seq.cycle_touch_element != 1)
    {
      if (check_button_on_grid(37, 1))
      {
        save_favorite(configuration.dexed[selected_instance_id].pool, configuration.dexed[selected_instance_id].bank, configuration.dexed[selected_instance_id].voice, selected_instance_id);
      }
      else if (selected_instance_id == 0)
      {
        if (check_button_on_grid(2, 25))
        {
          if (dexed_live_mod.active_button != 1)
            dexed_live_mod.active_button = 1;
          else
            dexed_live_mod.active_button = 0;
        }
        else if (check_button_on_grid(11, 25))
        {
          if (dexed_live_mod.active_button != 2)
            dexed_live_mod.active_button = 2;
          else
            dexed_live_mod.active_button = 0;
        }
        else if (check_button_on_grid(45, 23))
          LCDML.OTHER_jumpToFunc(UI_func_voice_editor);
      }
      else if (selected_instance_id == 1)
      {
        if (check_button_on_grid(2, 25))
        {
          if (dexed_live_mod.active_button != 3)
            dexed_live_mod.active_button = 3;
          else
            dexed_live_mod.active_button = 0;
        }
        else if (check_button_on_grid(11, 25))
        {
          if (dexed_live_mod.active_button != 4)
            dexed_live_mod.active_button = 4;
          else
            dexed_live_mod.active_button = 0;
        }
        else if (check_button_on_grid(45, 23))
          LCDML.OTHER_jumpToFunc(UI_func_voice_editor);
      }
      else
        dexed_live_mod.active_button = 0;

      if (dexed_live_mod.active_button > 0 && dexed_live_mod.active_button < 5)
      {
        helptext_r("< > CHANGE MODIFIER VALUE");
        display.setCursor(0, DISPLAY_HEIGHT - (CHAR_height_small * 2) - 2);
        print_empty_spaces(38);
        display.setCursor(9 * CHAR_width_small, DISPLAY_HEIGHT - CHAR_height_small * 1);
        print_empty_spaces(9);
        display.setCursor(CHAR_width_small * 38 + 2, DISPLAY_HEIGHT - (CHAR_height_small * 2) - 2);
        display.print(F(" PUSH TO RETURN"));
      }
      else
      {
        print_voice_select_default_help();
      }

      print_voice_settings_in_dexed_voice_select(false, true);

      print_perfmod_buttons();
      print_perfmod_lables();
    }

    if (ts.update_virtual_keyboard_octave == false && seq.cycle_touch_element == 1)
    {
      touch_check_all_keyboard_buttons();
    }

    if (ts.p.x > 1 && ts.p.y > VIRT_KEYB_YPOS && seq.cycle_touch_element == 1)
    {
      virtual_keyboard_key_on();
    }
  }
  else {
    isButtonTouched = false;
    if (ts.update_virtual_keyboard_octave && seq.cycle_touch_element == 1)
    {
      print_virtual_keyboard_octave();
      ts.update_virtual_keyboard_octave = false;
    }
  }
  virtual_keyboard_update_all_key_states();
}

FLASHMEM void update_step_rec_buttons()
{
  // if (seq.step_recording && seq.cycle_touch_element == 1) {
  if (seq.cycle_touch_element == 1 && seq.step_recording)
  {
    if (seq.auto_advance_step == 1)
      draw_button_on_grid(45, 10, "AUTO", "ADV.", 1); // print step recorder icon
    else if (seq.auto_advance_step == 2)
      draw_button_on_grid(45, 10, "AUTO", "AD+ST", 1); // print step recorder icon
    else
      draw_button_on_grid(45, 10, "KEEP", "STEP", 1); // print step recorder icon
  }
  if (seq.step_recording)
  {
    draw_button_on_grid(36, 1, "RECORD", "ACTIVE", 2); // print step recorder icon
  }
  else
  {
    draw_button_on_grid(36, 1, "STEP", "RECORD", 1); // print step recorder icon
  }
}

FLASHMEM void handle_touchscreen_pattern_editor()
{
  if (getNumTouchPoints() > 0)
  {
    get_scaled_touch_point();
    if ((LCDML.FUNC_getID() == LCDML.OTHER_getIDFromFunction(UI_func_seq_pattern_editor) && seq.cycle_touch_element == 1) || (LCDML.FUNC_getID() == LCDML.OTHER_getIDFromFunction(UI_func_seq_vel_editor) && seq.cycle_touch_element == 1))
    {
      if (ts.p.y > 6 * CHAR_height_small && ts.p.y < 12 * CHAR_height_small + 20 && ts.p.x < 230)
      {
        ts.p.x = ts.p.x / 1.4;
        if (ts.p.x - 22 < 22)
          ts.p.x = 22;
        if (ts.p.x > 127 + 22)
          ts.p.x = 127 + 22;
        ts.virtual_keyboard_velocity = ts.p.x - 22;
        virtual_keyboard_print_velocity_bar();
        update_step_rec_buttons();
      }
    }
    if ((LCDML.FUNC_getID() == LCDML.OTHER_getIDFromFunction(UI_func_seq_pattern_editor) && seq.cycle_touch_element == 1) || (LCDML.FUNC_getID() == LCDML.OTHER_getIDFromFunction(UI_func_seq_vel_editor) && seq.cycle_touch_element == 1))
    {

      if (check_button_on_grid(45, 10) && seq.step_recording)
      {
        seq.auto_advance_step++;
        if (seq.auto_advance_step > 2)
          seq.auto_advance_step = 0;
      }
      update_step_rec_buttons();
    }
    if (check_button_on_grid(36, 1) && seq.running == false)
    {
      seq.note_in = 0;
      seq.step_recording = !seq.step_recording;
      update_step_rec_buttons();
      virtual_keyboard_print_velocity_bar();
    }
    else if (check_button_on_grid(45, 1))
    {
      border3_large();
      border3_large_clear();
      if (seq.cycle_touch_element == 1)
      {
        seq.cycle_touch_element = 0;
        draw_button_on_grid(45, 1, "", "", 99); // print keyboard icon
        display.fillRect(0, CHAR_height_small * 12 + 1, DISPLAY_WIDTH, 1, COLOR_BACKGROUND);
        seq_pattern_editor_update_dynamic_elements();
      }
      else
      {
        seq.cycle_touch_element = 1;
        draw_button_on_grid(45, 1, back_text, "TO SEQ", 0);
        display.fillRect(216, CHAR_height_small * 6, 95, CHAR_height_small * 6 + 1, COLOR_BACKGROUND);
        display.fillRect(0, CHAR_height_small * 10 + 1, 195, CHAR_height_small * 2 + 1, COLOR_BACKGROUND);
        virtual_keyboard();
        virtual_keyboard_print_buttons();
        virtual_keyboard_print_current_instrument();
      }
    }
    else if (check_button_on_grid(36, 16) && seq.cycle_touch_element != 1) // toggle seq. playmode song/pattern only
    {
      if (seq.play_mode)
        seq.play_mode = false;
      else
        seq.play_mode = true;

      if (seq.play_mode == false) // is in full song more
      {
        draw_button_on_grid(36, 16, "PLAYNG", "SONG", 0);
        seq.hunt_pattern = false;
        draw_button_on_grid(45, 22, "HUNT", "PATT", 0);
      }
      else // play only current pattern
        draw_button_on_grid(36, 16, "LOOP", "PATT", 2);
    }
    else if (check_button_on_grid(45, 22) && seq.cycle_touch_element != 1) // hunt pattern
    {
      if (seq.hunt_pattern)
        seq.hunt_pattern = false;
      else
        seq.hunt_pattern = true;

      if (seq.hunt_pattern == false)
        draw_button_on_grid(45, 22, "HUNT", "PATT", 0);
      else // play only current pattern
        draw_button_on_grid(45, 22, "HUNT", "PATT", 2);
    }
    else if (check_button_on_grid(36, 22) && seq.cycle_touch_element != 1) // jump song editor
    {
      LCDML.OTHER_jumpToFunc(UI_func_song);
    }
    else if (LCDML.FUNC_getID() == LCDML.OTHER_getIDFromFunction(UI_func_seq_pattern_editor) && seq.cycle_touch_element != 1)
      if (check_button_on_grid(45, 16)) // jump pattern editor functions
      {

        if (seq.content_type[seq.active_pattern] == 0)
        {
          draw_button_on_grid(45, 16, "JUMP", "TOOLS", 2);
          activesample = NUM_DRUMSET_CONFIG;
          seq.menu = 0;
          seq.active_function = 0;
          pattern_editor_menu_0();
        }
        else
        {
          draw_button_on_grid(45, 16, "JUMP", "TOOLS", 2);
          temp_int = 111;
          seq.menu = 0;
          seq.active_function = 0;
          pattern_editor_menu_0();
        }
      }

    if (ts.update_virtual_keyboard_octave == false && seq.cycle_touch_element == 1)
    {
      touch_check_all_keyboard_buttons();
    }
    if (ts.p.x > 1 && ts.p.y > VIRT_KEYB_YPOS && seq.cycle_touch_element == 1)
    {
      virtual_keyboard_key_on();
    }
  }
  else {
    isButtonTouched = false;
    if (ts.update_virtual_keyboard_octave && seq.cycle_touch_element == 1)
    {
      print_virtual_keyboard_octave();
      ts.update_virtual_keyboard_octave = false;
    }
  }
  virtual_keyboard_update_all_key_states();
}

FLASHMEM void handle_touchscreen_microsynth()
{
  if (getNumTouchPoints() > 0)
  {
    get_scaled_touch_point();

    if (check_button_on_grid(45, 1))
    {
      display.console = true;
      display.fillRect(0, VIRT_KEYB_YPOS - 6 * CHAR_height_small, DISPLAY_WIDTH - 1, DISPLAY_HEIGHT - VIRT_KEYB_YPOS + 6 * CHAR_height_small, COLOR_BACKGROUND);
      display.console = false;
      if (seq.cycle_touch_element == 1)
      {
        seq.cycle_touch_element = 0;
        draw_button_on_grid(45, 1, "", "", 99); // print keyboard icon
        generic_full_draw_required = true;
        microsynth_refresh_lower_screen_static_text();
        microsynth_refresh_lower_screen_dynamic_text();
        generic_full_draw_required = false;
      }
      else
      {
        seq.cycle_touch_element = 1;
        draw_button_on_grid(45, 1, "MORE", "PARAM.", 0);
        virtual_keyboard();
        virtual_keyboard_print_buttons();
        virtual_keyboard_print_current_instrument();
      }
    }

    if (ts.update_virtual_keyboard_octave == false && seq.cycle_touch_element == 1)
    {
      touch_check_all_keyboard_buttons();
    }

    if (ts.p.x > 1 && ts.p.y > VIRT_KEYB_YPOS && seq.cycle_touch_element == 1)
    {
      virtual_keyboard_key_on();
    }
  }
  else {
    isButtonTouched = false;
    if (ts.update_virtual_keyboard_octave && seq.cycle_touch_element == 1)
    {
      print_virtual_keyboard_octave();
      ts.update_virtual_keyboard_octave = false;
    }
  }
  virtual_keyboard_update_all_key_states();
}

FLASHMEM void print_file_manager_buttons()
{
  draw_button_on_grid(1, 25, "BROWSE", "FILES", fm.sd_mode == FM_BROWSE_FILES ? 1 : 0);
  draw_button_on_grid(10, 25, "DELETE", "FILE", fm.sd_mode == FM_DELETE_FILE ? 1 : 0);
#ifdef COMPILE_FOR_FLASH
  draw_button_on_grid(19, 25, "COPY", "PRESET", fm.sd_mode == FM_COPY_PRESETS ? 1 : 0);
  draw_button_on_grid(28, 25, "COPY >", "FLASH", fm.sd_mode == FM_COPY_TO_FLASH ? 1 : 0);
#endif
  draw_button_on_grid(46, 25, "PLAY", "SAMPLE", fm.sd_mode == FM_PLAY_SAMPLE ? 1 : 0);
}

FLASHMEM void print_file_manager_active_border()
{
  // active_window   0 = left window (SDCARD) , 1 = FLASH
  if (fm.active_window == 0)
  {
    display.console = true;
    display.drawRect(CHAR_width_small * 28 - 1, 0, CHAR_width_small * 25 + 3, CHAR_height_small * 23, GREY2);
    display.console = true;
    display.drawRect(0, 0, CHAR_width_small * 28, CHAR_height_small * 23, COLOR_SYSTEXT);
  }
  else
  {
    display.console = true;
    display.drawRect(0, 0, CHAR_width_small * 28, CHAR_height_small * 23, GREY2);
    display.console = true;
    display.drawRect(CHAR_width_small * 28 - 1, 0, CHAR_width_small * 25 + 3, CHAR_height_small * 23, COLOR_SYSTEXT);
  }
}

FLASHMEM void handle_touchscreen_file_manager()
{
  if (getNumTouchPoints() > 0)
  {
    get_scaled_touch_point();

    // check touch buttons
    if (ts.p.y > CHAR_height_small * 20)
    {
      if (check_button_on_grid(1, 25))
      {
        fm.sd_mode = FM_BROWSE_FILES;
        print_file_manager_buttons();
      }
      else if (check_button_on_grid(10, 25))
      {
        fm.sd_mode = FM_DELETE_FILE;
        print_file_manager_buttons();
      }
      else if (check_button_on_grid(19, 25))
      {
        fm.sd_mode = FM_COPY_PRESETS;
        print_file_manager_buttons();
      }
      else if (check_button_on_grid(28, 25))
      {
        fm.sd_mode = FM_COPY_TO_FLASH;
        print_file_manager_buttons();
      }
      else if (check_button_on_grid(46, 25))
      {
        fm.sd_mode = FM_PLAY_SAMPLE;
        preview_sample();
        print_file_manager_buttons();
      }
    }
    // active_window   0 = left window (SDCARD) , 1 = FLASH
    else if (ts.p.x > 1 && ts.p.x < CHAR_width_small * 29)
    {
      fm.active_window = 0;
      print_file_manager_active_border();
    }
    else if (ts.p.x > CHAR_width_small * 29)
    {
#ifdef COMPILE_FOR_FLASH
      fm.active_window = 1;
#endif
      print_file_manager_active_border();
    }
  }
  else {
    isButtonTouched = false;
  }
}

FLASHMEM void update_midi_learn_button()
{
  if (seq.midi_learn_active == true)
  {
    draw_button_on_grid(45, 1, "MIDI", "LEARN", 2); // RED button
  }
  else
  {
    draw_button_on_grid(45, 1, "MIDI", "LEARN", 0);
    // seq.midi_learn_active = false;
  }
}

FLASHMEM void handle_touchscreen_custom_mappings()
{
  if (getNumTouchPoints() > 0)
  {
    get_scaled_touch_point();
    if (check_button_on_grid(45, 1))
    {
      seq.midi_learn_active = !seq.midi_learn_active;
      update_midi_learn_button();
    }
  }
  else {
    isButtonTouched = false;
  }
}

FLASHMEM void handle_touchscreen_cc_mappings()
{
  if (getNumTouchPoints() > 0)
  {
    get_scaled_touch_point();
    if (check_button_on_grid(45, 1))
    {
      seq.midi_learn_active = !seq.midi_learn_active;
      update_midi_learn_button();
    }
  }
  else {
    isButtonTouched = false;
  }
}

FLASHMEM void handle_touchscreen_mute_matrix()
{
  if (getNumTouchPoints() > 0)
  {
    get_scaled_touch_point();
    uint8_t button_count = 0;
    char buf[4];
    for (uint8_t y = 0; y < 3; y++)
    {
      for (uint8_t x = 0; x < 4; x++)
      {
        if (y < 2)
        {
          if (check_button_on_grid(2 + x * 14, 12 + y * 8))
          {
            seq.track_mute[button_count] = !seq.track_mute[button_count];
            if (!seq.track_mute[button_count])
              draw_button_on_grid(2 + x * 14, 12 + y * 8, "TRACK:", itoa(button_count + 1, buf, 10), 1);
            else
              draw_button_on_grid(2 + x * 14, 12 + y * 8, "TRACK:", itoa(button_count + 1, buf, 10), 0);
          }
          button_count++;
        }
        else
        {
          if (check_button_on_grid(2 + x * 14, 4))
          {
            if (x == 1)
              seq.mute_mode = 0;
            else if (x == 2)
              seq.mute_mode = 1;
            else if (x == 3)
              seq.mute_mode = 2;
          }
          if (x == 1)
          {
            if (seq.mute_mode == 0)
              draw_button_on_grid(2 + x * 14, 4, "REAL", "TIME", 1);
            else
              draw_button_on_grid(2 + x * 14, 4, "REAL", "TIME", 0);
          }
          else if (x == 2)
          {
            if (seq.mute_mode == 1)
              draw_button_on_grid(2 + x * 14, 4, "NEXT", "PATTRN", 1);
            else
              draw_button_on_grid(2 + x * 14, 4, "NEXT", "PATTRN", 0);
          }
          else if (x == 3)
          {
            if (seq.mute_mode == 2)
              draw_button_on_grid(2 + x * 14, 4, "SONG", "STEP", 1);
            else
              draw_button_on_grid(2 + x * 14, 4, "SONG", "STEP", 0);
          }
        }
      }
    }
  }
  else {
    isButtonTouched = false;
  }
}

FLASHMEM void handle_touchscreen_arpeggio()
{
  if (getNumTouchPoints() > 0)
  {
    get_scaled_touch_point();
    if (check_button_on_grid(2, 23))
    {
      if (seq.running)
        handleStop();
      else
        handleStart();
    }
  }
  else {
    isButtonTouched = false;
  }
}

FLASHMEM void handle_touchscreen_braids()
{
  if (getNumTouchPoints() > 0)
  {
    get_scaled_touch_point();
    seq.cycle_touch_element = 1;
    if (ts.update_virtual_keyboard_octave == false && seq.cycle_touch_element == 1)
    {
      touch_check_all_keyboard_buttons();
    }
    if (ts.p.x > 1 && ts.p.y > VIRT_KEYB_YPOS && seq.cycle_touch_element == 1)
    {
      virtual_keyboard_key_on();
    }
  }
  else {
    isButtonTouched = false;

    if (ts.update_virtual_keyboard_octave && seq.cycle_touch_element == 1)
    {
      print_virtual_keyboard_octave();
      ts.update_virtual_keyboard_octave = false;
    }
  }
  virtual_keyboard_update_all_key_states();
}

FLASHMEM void draw_menu_ui_icons()
{
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
  draw_button_on_grid(45, 11, "", "", 99); // print keyboard icon
}

FLASHMEM void handle_touchscreen_menu()
{
  if (ts.touch_ui_drawn_in_menu == false)
  {
    if (ts.keyb_in_menu_activated)
    {
      display.console = true;
      display.fillRect(0, VIRT_KEYB_YPOS - 38, DISPLAY_WIDTH, DISPLAY_HEIGHT - VIRT_KEYB_YPOS + 38, COLOR_BACKGROUND);
      display.console = false;
      virtual_keyboard();
      virtual_keyboard_print_buttons();
      virtual_keyboard_print_current_instrument();
      draw_button_on_grid(45, 11, "KEYB", "OFF", 0);
    }
    else
    {
      draw_menu_ui_icons();
    }
    ts.touch_ui_drawn_in_menu = true;
  }
  if (getNumTouchPoints() > 0)
  {
    get_scaled_touch_point();

    if (check_button_on_grid(45, 11))
    {
      display.console = true;
      display.fillRect(0, VIRT_KEYB_YPOS - 38, DISPLAY_WIDTH, DISPLAY_HEIGHT - VIRT_KEYB_YPOS + 38, COLOR_BACKGROUND);
      display.console = false;
      ts.keyb_in_menu_activated = !ts.keyb_in_menu_activated;
      if (ts.keyb_in_menu_activated)
      {
        virtual_keyboard();
        virtual_keyboard_print_buttons();
        virtual_keyboard_print_current_instrument();
        draw_button_on_grid(45, 11, "KEYB", "OFF", 0);
      }
      else
      {
        draw_menu_ui_icons();
        if (LCDML.MENU_getLayer() > last_menu_depth)
        {
          if (ts.keyb_in_menu_activated == false)
          {
            helptext_l(back_text);
          }
        }
      }
    }
    if (ts.keyb_in_menu_activated == false)
    {
      if (check_button_on_grid(2, 18))
      {
        LCDML.OTHER_jumpToFunc(UI_func_voice_select);
      }
      else if (check_button_on_grid(13, 18))
      {
        LCDML.OTHER_jumpToFunc(UI_func_song);
      }
      else if (check_button_on_grid(24, 18))
      {
        LCDML.OTHER_jumpToFunc(UI_func_seq_pattern_editor);
      }
      else if (check_button_on_grid(45, 18))
      {
        if (seq.running)
        {
          handleStop();
          draw_button_on_grid(45, 18, "SEQ.", "START", 1);
        }
        else
        {
          handleStart();
          draw_button_on_grid(45, 18, "SEQ.", "STOP", 2);
        }
      }
      else if (check_button_on_grid(13, 25))
      {
        display.setTextSize(2);
        display.setTextColor(COLOR_SYSTEXT, COLOR_BACKGROUND);
        LCDML.OTHER_jumpToFunc(UI_func_load_performance);
      }
      else if (check_button_on_grid(24, 25))
      {
        display.setTextSize(2);
        display.setTextColor(COLOR_SYSTEXT, COLOR_BACKGROUND);
        LCDML.OTHER_jumpToFunc(UI_func_save_performance);
      }
      else if (check_button_on_grid(45, 25))
      {
        LCDML.OTHER_jumpToFunc(UI_func_mixer);
      }
      else if (check_button_on_grid(35, 25))
      {
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

    if (ts.update_virtual_keyboard_octave == false && ts.keyb_in_menu_activated)
    {
      touch_check_all_keyboard_buttons();
    }
    if (ts.p.x > 1 && ts.p.y > VIRT_KEYB_YPOS && ts.keyb_in_menu_activated)
    {
      virtual_keyboard_key_on();
    }
    ts.touch_ui_drawn_in_menu = true;
  }
  else {
    isButtonTouched = false;
    if (ts.update_virtual_keyboard_octave && ts.keyb_in_menu_activated)
    {
      print_virtual_keyboard_octave();
      ts.update_virtual_keyboard_octave = false;
    }
  }
  if (ts.keyb_in_menu_activated)
    virtual_keyboard_update_all_key_states();
  display.setTextSize(2);
  display.setTextColor(COLOR_SYSTEXT, COLOR_BACKGROUND);
}

FLASHMEM void toggle_generic_active_function()
{
  if (generic_active_function == 0)
    generic_active_function = 1;
  else
    generic_active_function = 0;
}

FLASHMEM void handle_touchscreen_mixer()
{
  if (scope.scope_delay % 60 == 0)
    draw_volmeters_mixer();
}

FLASHMEM void handle_touchscreen_multiband()
{
  if (getNumTouchPoints() > 0)
  {
    get_scaled_touch_point();

    if (multiband_active)
    {
      if (check_button_on_grid(12, 8))
      {
        mb_solo_high = !mb_solo_high;
        if (mb_solo_high)
        {
          draw_button_on_grid(9, 8, "SOLO", "ON", mb_solo_high + 1);
        }
        else
          draw_button_on_grid(9, 8, "SOLO", "  ", mb_solo_high);
      }
      else if (check_button_on_grid(12, 14))
      {
        mb_solo_upper_mid = !mb_solo_upper_mid;
        if (mb_solo_upper_mid)
        {
          draw_button_on_grid(9, 14, "SOLO", "ON", mb_solo_upper_mid + 1);
        }
        else
          draw_button_on_grid(9, 14, "SOLO", "  ", mb_solo_upper_mid);
      }
      else if (check_button_on_grid(12, 20))
      {
        mb_solo_mid = !mb_solo_mid;
        if (mb_solo_mid)
        {
          draw_button_on_grid(9, 20, "SOLO", "ON", mb_solo_mid + 1);
        }
        else
          draw_button_on_grid(9, 20, "SOLO", "  ", mb_solo_mid);
      }
      else if (check_button_on_grid(12, 26))
      {
        mb_solo_low = !mb_solo_low;
        if (mb_solo_low)
        {
          draw_button_on_grid(9, 26, "SOLO", "ON", mb_solo_low + 1);
        }
        else
          draw_button_on_grid(9, 26, "SOLO", "  ", mb_solo_low);
      }
      if (mb_solo_low && mb_solo_upper_mid && mb_solo_mid && mb_solo_high)
      {
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

      if (check_button_on_grid(38, 8))
      {
        toggle_generic_active_function();
        generic_temp_select_menu = 7;
      }
      else if (check_button_on_grid(38, 14))
      {
        toggle_generic_active_function();
        generic_temp_select_menu = 12;
      }
      else if (check_button_on_grid(38, 20))
      {
        toggle_generic_active_function();
        generic_temp_select_menu = 17;
      }
      else if (check_button_on_grid(38, 26))
      {
        toggle_generic_active_function();
        generic_temp_select_menu = 22;
      }
    }
  }
  else {
    isButtonTouched = false;
  }
  if (scope.scope_delay % 60 == 0)
  {
    draw_volmeters_multiband_compressor();
  }
}

extern int temp_int;

FLASHMEM void handle_touchscreen_sample_editor()
{
  if (getNumTouchPoints() > 0)
  {
    get_scaled_touch_point();

    if (check_button_on_grid(45, 23))
    {
      preview_sample();
    }
  }
  else {
    isButtonTouched = false;
  }
}

FLASHMEM void handle_touchscreen_settings_button_test()
{
  static bool button_state = false;

  if (getNumTouchPoints() > 0)
  {
    get_scaled_touch_point();
    if (check_button_on_grid(42, 1))
    {
      draw_button_on_grid(42, 1, "TOUCH", button_state ? "OK" : "TEST", button_state ? 2 : 0);
      button_state = !button_state;
    }
  }
  else {
    isButtonTouched = false;
  }
}

FLASHMEM void handle_touchscreen_test_touchscreen()
{
  if (getNumTouchPoints() > 0)
  {
    get_scaled_touch_point();
    if (check_button_on_grid(42, 1))
      sub_touchscreen_test_page_init();
    display.console = true;
    display.fillRect(ts.p.x, ts.p.y, 2, 2, COLOR_SYSTEXT);
    display.console = false;
  }
  else {
    isButtonTouched = false;
  }
}
