/*
   MicroDexed

   MicroDexed is a port of the Dexed sound engine
   (https://github.com/asb2m10/dexed) for the Teensy-3.5/3.6/4.x with audio shield.
   Dexed ist heavily based on https://github.com/google/music-synthesizer-for-android

   (c)2018-2021 H. Wirtz <wirtz@parasitstudio.de>
   (c)2021      H. Wirtz <wirtz@parasitstudio.de>, M. Koslowski <positionhigh@gmx.de>

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

#include <Arduino.h>
#include "config.h"
#include <Wire.h>
#include <SD.h>
#include <ArduinoJson.h>
#include <TeensyTimerTool.h>
using namespace TeensyTimerTool;
#include "dexed_sd.h"
#include "synth_dexed.h"
#if NUM_DRUMS > 0
#include "drums.h"
extern void set_drums_volume(float vol);
extern drum_config_t drum_config[];
extern custom_midi_map_t custom_midi_map[NUM_CUSTOM_MIDI_MAPPINGS];
#endif

extern char g_voice_name[NUM_DEXED][VOICE_NAME_LEN];
extern char g_bank_name[NUM_DEXED][BANK_NAME_LEN];
extern void init_MIDI_send_CC(void);
extern void check_configuration_dexed(uint8_t instance_id);
extern void check_configuration_performance(void);
extern void check_configuration_fx(void);
extern void check_configuration_epiano(void);
extern void update_euclidean(void);

#ifdef USE_MICROSYNTH
extern microsynth_t microsynth[NUM_MICROSYNTH];
extern void microsynth_update_all_settings(uint8_t instance_id);
#endif

#ifdef USE_BRAIDS
extern braids_t braids_osc;
extern void braids_update_all_settings();
#endif

extern uint8_t GAMEPAD_UP_0;
extern uint8_t GAMEPAD_UP_1;
extern uint8_t GAMEPAD_UP_BUTTONS;

extern uint8_t GAMEPAD_DOWN_0;
extern uint8_t GAMEPAD_DOWN_1;
extern uint8_t GAMEPAD_DOWN_BUTTONS;

extern uint8_t GAMEPAD_RIGHT_0;
extern uint8_t GAMEPAD_RIGHT_1;
extern uint8_t GAMEPAD_RIGHT_BUTTONS;

extern uint8_t GAMEPAD_LEFT_0;
extern uint8_t GAMEPAD_LEFT_1;
extern uint8_t GAMEPAD_LEFT_BUTTONS;

extern uint32_t GAMEPAD_SELECT;
extern uint32_t GAMEPAD_START;
extern uint32_t GAMEPAD_BUTTON_A;
extern uint32_t GAMEPAD_BUTTON_B;

extern uint8_t gamepad_0_neutral;
extern uint8_t gamepad_1_neutral;

#ifdef USE_SEQUENCER
#include "sequencer.h"
extern PeriodicTimer sequencer_timer;
extern void sequencer();
extern sequencer_t seq;
#endif

#ifdef USE_MULTIBAND
extern uint16_t mb_cross_freq_low;
extern uint16_t mb_cross_freq_mid;
extern uint16_t mb_cross_freq_upper_mid;
extern uint16_t mb_cross_freq_high;
extern float mb_global_gain;
extern float mb_gain_low;
extern float mb_gain_mid;
extern float mb_gain_upper_mid;
extern float mb_gain_high;
extern bool multiband_active;
extern uint8_t mb_threshold_low;
extern uint8_t mb_threshold_mid;
extern uint8_t mb_threshold_upper_mid;
extern uint8_t mb_threshold_high;
extern float mb_q_low;
extern float mb_q_mid;
extern float mb_q_upper_mid;
extern float mb_q_high;
extern bool mb_solo_low;
extern bool mb_solo_mid;
extern bool mb_solo_upper_mid;
extern bool mb_solo_high;
extern uint8_t mb_global_ratio;
#endif

extern float midi_volume_transform(uint8_t midi_amp);
extern void set_sample_note(uint8_t sample, uint8_t note);
extern void set_sample_pitch(uint8_t sample, float playbackspeed);
extern void set_sample_p_offset(uint8_t sample, float s_offset);
extern void set_sample_pan(uint8_t sample, float s_pan);
extern void set_sample_vol_max(uint8_t sample, float s_max);
extern void set_sample_vol_min(uint8_t sample, float s_min);
extern void set_sample_reverb_send(uint8_t sample, float s_reverb);
extern void handleStop(void);
extern void handleStart(void);
extern void dac_mute(void);
extern void dac_unmute(void);
extern void check_configuration_sys(void);
extern uint8_t get_sample_note(uint8_t sample);
extern float get_sample_pitch(uint8_t sample);
extern float get_sample_p_offset(uint8_t sample);
extern float get_sample_pan(uint8_t sample);
extern float get_sample_vol_max(uint8_t sample);
extern float get_sample_vol_min(uint8_t sample);
extern float get_sample_reverb_send(uint8_t sample);
extern uint8_t find_drum_number_from_note(uint8_t note);
extern uint16_t COLOR_SYSTEXT;
extern uint16_t COLOR_SYSTEXT_ACCENT;
extern uint16_t COLOR_BACKGROUND;
extern uint16_t COLOR_INSTR;
extern uint16_t COLOR_CHORDS;
extern uint16_t COLOR_ARP;
extern uint16_t COLOR_DRUMS;
extern uint16_t COLOR_PITCHSMP;
extern multisample_t msp[NUM_MULTISAMPLES];
extern multisample_zone_t msz[NUM_MULTISAMPLES][NUM_MULTISAMPLE_ZONES];

File json;
char filename[CONFIG_FILENAME_LEN];
const char* sError = "*ERROR*";

/******************************************************************************
   SD BANK/VOICE LOADING
 ******************************************************************************/

FLASHMEM bool load_sd_voice(uint8_t b, uint8_t v, uint8_t instance_id) {
#ifdef DEBUG
  Serial.printf_P(PSTR("load voice, bank [%d] - voice [%d]\n"), b, v + 1);
#endif
  v = constrain(v, 0, MAX_VOICES - 1);
  b = constrain(b, 0, MAX_BANKS - 1);

  if (sd_card > 0) {
    File sysex_dir;
    char bankdir[4];
    char bank_name[BANK_NAME_LEN];
    char voice_name[VOICE_NAME_LEN];
    uint8_t data[128];

    snprintf_P(bankdir, sizeof(bankdir), PSTR("/%d"), b);

    AudioNoInterrupts();
    sysex_dir = SD.open(bankdir);
    AudioInterrupts();
    if (!sysex_dir) {
      strcpy(g_bank_name[instance_id], sError);
      strcpy(g_voice_name[instance_id], sError);

#ifdef DEBUG
      Serial.print(F("E : Cannot open "));
      Serial.print(bankdir);
      Serial.println(F(" on SD."));
#endif
      return (false);
    }

    File entry;
    do {
      entry = sysex_dir.openNextFile();
    } while (entry.isDirectory());

    if (entry.isDirectory()) {
      AudioNoInterrupts();
      entry.close();
      sysex_dir.close();
      AudioInterrupts();
      strcpy(g_bank_name[instance_id], sError);
      strcpy(g_voice_name[instance_id], sError);
      return (false);
    }

    strip_extension(entry.name(), bank_name, BANK_NAME_LEN);
    string_toupper(bank_name);
    strcpy(g_bank_name[instance_id], bank_name);
#ifdef DEBUG
    char filename[FILENAME_LEN];
    snprintf_P(filename, sizeof(filename), PSTR("/%d/%s.syx"), b, bank_name);
    Serial.print(F("Loading voice from "));
    Serial.print(filename);
    Serial.print(F(" bank:["));
    Serial.print(g_bank_name[instance_id]);
    Serial.println(F("]"));
#endif
    // search voice name
    memset(voice_name, '\0', VOICE_NAME_LEN);
    entry.seek(124 + (v * 128));
    entry.read(voice_name, min(VOICE_NAME_LEN, 10));
    string_toupper(voice_name);
    strcpy(g_voice_name[instance_id], voice_name);

    if (get_sd_voice(entry, v, data)) {
#ifdef DEBUG
      Serial.print(F("get_sd_voice:["));
      Serial.print(g_voice_name[instance_id]);
      Serial.println(F("]"));
#endif
      uint8_t tmp_data[156];
      bool ret = MicroDexed[instance_id]->decodeVoice(tmp_data, data);
      MicroDexed[instance_id]->loadVoiceParameters(tmp_data);
#ifdef DEBUG
      show_patch(instance_id);
#endif

      AudioNoInterrupts();
      entry.close();
      sysex_dir.close();
      AudioInterrupts();
      configuration.dexed[instance_id].transpose = MicroDexed[instance_id]->getTranspose();
      configuration.dexed[instance_id].bank = b;
      configuration.dexed[instance_id].voice = v;

      uint8_t data_copy[155];
      MicroDexed[instance_id]->getVoiceData(data_copy);
      send_sysex_voice(configuration.dexed[instance_id].midi_channel, data_copy);
      init_MIDI_send_CC();
      return (ret);
    } else {
      strcpy(g_voice_name[instance_id], sError);
#ifdef DEBUG
      Serial.println(F("E : Cannot load voice data"));
#endif
    }
    AudioNoInterrupts();
    entry.close();
    sysex_dir.close();
    AudioInterrupts();

    return true;
  }

  strcpy(g_bank_name[instance_id], sError);
  strcpy(g_voice_name[instance_id], sError);
  return false;
}

FLASHMEM bool save_sd_voice(uint8_t b, uint8_t v, uint8_t instance_id) {
#ifdef DEBUG
  Serial.printf_P(PSTR("save_sd_voice, b:%d - d:%d\n"), b, v);
#endif
  v = constrain(v, 0, MAX_VOICES - 1);
  b = constrain(b, 0, MAX_BANKS - 1);

  if (sd_card > 0) {
    File sysex;
    char filename[FILENAME_LEN];
    uint8_t data[128];

    snprintf_P(filename, sizeof(filename), PSTR("/%d/%s.syx"), b, g_bank_name[instance_id]);

    AudioNoInterrupts();
    sysex = SD.open(filename, FILE_WRITE);
    AudioInterrupts();
    if (!sysex) {
#ifdef DEBUG
      Serial.print(F("E : Cannot open "));
      Serial.print(filename);
      Serial.println(F(" on SD."));
#endif
      return (false);
    }

    MicroDexed[instance_id]->encodeVoice(data);

    if (put_sd_voice(sysex, v, data)) {
#ifdef DEBUG
      Serial.print(F("Saving voice to "));
      Serial.print(filename);
      Serial.print(F(" ["));
      Serial.print(g_voice_name[instance_id]);
      Serial.println(F("]"));
#endif
      AudioNoInterrupts();
      sysex.close();
      AudioInterrupts();

      return (true);
    }
#ifdef DEBUG
    else
      Serial.println(F("E : Cannot load voice data"));
#endif
    AudioNoInterrupts();
    sysex.close();
    AudioInterrupts();
  }

  return (false);
}

FLASHMEM bool get_sd_voice(File sysex, uint8_t voice_number, uint8_t* data) {
  uint16_t n;
  int32_t bulk_checksum_calc = 0;
  int8_t bulk_checksum;

  AudioNoInterrupts();
  if (sysex.size() != 4104)  // check sysex size
  {
#ifdef DEBUG
    Serial.println(F("E : SysEx file size wrong."));
#endif
    return (false);
  }

  sysex.seek(0);
  if (sysex.read() != 0xf0)  // check sysex start-byte
  {
#ifdef DEBUG
    Serial.println(F("E : SysEx start byte not found."));
#endif
    return (false);
  }
  if (sysex.read() != 0x43)  // check sysex vendor is Yamaha
  {
#ifdef DEBUG
    Serial.println(F("E : SysEx vendor not Yamaha."));
#endif
    return (false);
  }
  sysex.seek(4103);
  if (sysex.read() != 0xf7)  // check sysex end-byte
  {
#ifdef DEBUG
    Serial.println(F("E : SysEx end byte not found."));
#endif
    return (false);
  }
  sysex.seek(3);
  if (sysex.read() != 0x09)  // check for sysex type (0x09=32 voices)
  {
#ifdef DEBUG
    Serial.println(F("E : SysEx type not 32 voices."));
#endif
    return (false);
  }
  sysex.seek(4102);  // Bulk checksum
  bulk_checksum = sysex.read();

  sysex.seek(6);  // start of bulk data
  for (n = 0; n < 4096; n++) {
    uint8_t d = sysex.read();
    if (n >= voice_number * 128 && n < (voice_number + 1) * 128)
      data[n - (voice_number * 128)] = d;
    bulk_checksum_calc -= d;
  }
  bulk_checksum_calc &= 0x7f;
  AudioInterrupts();

#ifdef DEBUG
  Serial.print(F("Bulk checksum : 0x"));
  Serial.print(bulk_checksum_calc, HEX);
  Serial.print(F(" [0x"));
  Serial.print(bulk_checksum, HEX);
  Serial.println(F("]"));
#endif

  if (bulk_checksum_calc != bulk_checksum) {
#ifdef DEBUG
    Serial.print(F("E : Bulk checksum mismatch : 0x"));

    Serial.print(bulk_checksum_calc, HEX);
    Serial.print(F(" != 0x"));
    Serial.println(bulk_checksum, HEX);
#endif
    return (false);
  }

  // MicroDexed[0]->resetRenderTimeMax(); // necessary?

  return (true);
}

FLASHMEM bool put_sd_voice(File sysex, uint8_t voice_number, uint8_t* data) {
  uint16_t n;
  int32_t bulk_checksum_calc = 0;

  AudioNoInterrupts();
  sysex.seek(0);

  if (sysex.size() != 4104)  // check sysex size
  {
#ifdef DEBUG
    Serial.println(F("E : SysEx file size wrong."));
#endif
    return (false);
  }
  if (sysex.read() != 0xf0)  // check sysex start-byte
  {
#ifdef DEBUG
    Serial.println(F("E : SysEx start byte not found."));
#endif
    return (false);
  }
  if (sysex.read() != 0x43)  // check sysex vendor is Yamaha
  {
#ifdef DEBUG
    Serial.println(F("E : SysEx vendor not Yamaha."));
#endif
    return (false);
  }
  sysex.seek(4103);
  if (sysex.read() != 0xf7)  // check sysex end-byte
  {
#ifdef DEBUG
    Serial.println(F("E : SysEx end byte not found."));
#endif
    return (false);
  }
  sysex.seek(3);
  if (sysex.read() != 0x09)  // check for sysex type (0x09=32 voices)
  {
#ifdef DEBUG
    Serial.println(F("E : SysEx type not 32 voices."));
#endif
    return (false);
  }

  sysex.seek(6 + (voice_number * 128));
  sysex.write(data, 128);

  // checksum calculation
  sysex.seek(6);  // start of bulk data
  for (n = 0; n < 4096; n++) {
    uint8_t d = sysex.read();
    bulk_checksum_calc -= d;
  }
  sysex.seek(4102);  // Bulk checksum
  sysex.write(bulk_checksum_calc & 0x7f);
  AudioInterrupts();

#ifdef DEBUG
  Serial.print(F("Bulk checksum : 0x"));
  Serial.println(bulk_checksum_calc & 0x7f, HEX);
#endif

  return (true);
}

FLASHMEM bool save_sd_bank(const char* bank_filename, uint8_t* data) {
  char tmp[FILENAME_LEN];
  char tmp2[FILENAME_LEN];
  int bank_number;
  File root, entry;

  if (sd_card > 0) {
#ifdef DEBUG
    Serial.print(F("Trying so store "));
    Serial.print(bank_filename);
    Serial.println(F("."));
#endif

    // first remove old bank
    sscanf(bank_filename, "/%d/%s", &bank_number, tmp);
    snprintf_P(tmp, sizeof(tmp), PSTR("/%d"), bank_number);
    AudioNoInterrupts();
    root = SD.open(tmp);
    while (42 == 42) {
      entry = root.openNextFile();
      if (entry) {
        if (!entry.isDirectory()) {
#ifdef DEBUG
          Serial.print(F("Removing "));
          Serial.print(tmp);
          Serial.print(F("/"));
          Serial.println(entry.name());
#endif
          snprintf_P(tmp2, sizeof(tmp2), PSTR("%s/%s"), tmp, entry.name());
          entry.close();
#ifndef DEBUG
          SD.remove(tmp2);
#else
          bool r = SD.remove(tmp2);
          if (r == false) {
            Serial.print(F("E: cannot remove "));
            Serial.print(tmp2);
            Serial.println(F("."));
          }
#endif
          break;
        }
      } else {
        break;
      }
    }
    root.close();

    // store new bank at /<b>/<bank_name>.syx
#ifdef DEBUG
    Serial.print(F("Storing bank as "));
    Serial.print(bank_filename);
    Serial.print(F("..."));
#endif
    root = SD.open(bank_filename, FILE_WRITE);
    root.write(data, 4104);
    root.close();
    AudioInterrupts();
#ifdef DEBUG
    Serial.println(F(" done."));
#endif
  } else
    return (false);

  return (true);
}

/******************************************************************************
   SD DRUM CUSTOM MAPPINGS
 ******************************************************************************/

FLASHMEM bool load_sd_drummappings_json(uint8_t number) {
#if NUM_DRUMS > 0
  if (number < 0)
    return (false);

  if (sd_card > 0) {
    number = constrain(number, PERFORMANCE_NUM_MIN, PERFORMANCE_NUM_MAX);
    snprintf_P(filename, sizeof(filename), PSTR("/%s/%d/%s.json"), PERFORMANCE_CONFIG_PATH, number, DRUMS_MAPPING_NAME);

    // first check if file exists...
    AudioNoInterrupts();
    if (SD.exists(filename)) {
      // ... and if: load
#ifdef DEBUG
      Serial.print(F("Found drum mapping ["));
      Serial.print(filename);
      Serial.println(F("]... loading..."));
#endif
      json = SD.open(filename);
      if (json) {
        StaticJsonDocument<JSON_BUFFER_SIZE> data_json;
        deserializeJson(data_json, json);
        json.close();
        AudioInterrupts();
#ifdef DEBUG
        Serial.println(F("Read JSON data:"));
        serializeJsonPretty(data_json, Serial);
        Serial.println();
#endif

        for (uint8_t i = 0; i < NUM_CUSTOM_MIDI_MAPPINGS; i++) {
          custom_midi_map[i].type = data_json["type"][i];
          custom_midi_map[i].in = data_json["in"][i];
          custom_midi_map[i].out = data_json["out"][i];
          custom_midi_map[i].channel = data_json["channel"][i];
        }
        return (true);
      }
#ifdef DEBUG
      else {
        Serial.print(F("E : Cannot open "));
        Serial.print(filename);
        Serial.println(F(" on SD."));
      }
    } else {
      Serial.print(F("No "));
      Serial.print(filename);
      Serial.println(F(" available."));
#endif
    }
  }
#endif
  return (false);
}

FLASHMEM bool save_sd_drummappings_json(uint8_t number) {
#if NUM_DRUMS > 0
  if (sd_card > 0) {
    number = constrain(number, 0, 99);
    if (check_performance_directory(number)) {
      snprintf_P(filename, sizeof(filename), PSTR("/%s/%d/%s.json"), PERFORMANCE_CONFIG_PATH, number, DRUMS_MAPPING_NAME);

#ifdef DEBUG
      Serial.print(F("Saving drum mapping "));
      Serial.print(number);
      Serial.print(F(" to "));
      Serial.println(filename);
#endif
      AudioNoInterrupts();
      if (SD.exists(filename)) {
#ifdef DEBUG
        Serial.println(F("remove old drum mapping file"));
#endif
        SD.remove(filename);
      }
      json = SD.open(filename, FILE_WRITE);
      if (json) {
        StaticJsonDocument<JSON_BUFFER_SIZE> data_json;
        for (uint8_t i = 0; i < NUM_CUSTOM_MIDI_MAPPINGS; i++) {
          data_json["type"][i] = custom_midi_map[i].type;
          data_json["in"][i] = custom_midi_map[i].in;
          data_json["out"][i] = custom_midi_map[i].out;
          data_json["channel"][i] = custom_midi_map[i].channel;
        }
#ifdef DEBUG
        Serial.println(F("Write JSON data:"));
        serializeJsonPretty(data_json, Serial);
        Serial.println();
#endif
        serializeJsonPretty(data_json, json);
        json.close();
        AudioInterrupts();
        return (true);
      } else {
#ifdef DEBUG
        Serial.print(F("E : Cannot open "));
        Serial.print(filename);
        Serial.println(F(" on SD."));
#endif
        AudioInterrupts();
        return (false);
      }
    } else {
      AudioInterrupts();
      return (false);
    }
  }
#ifdef DEBUG
  else {
    Serial.println(F("E: SD card not available"));
  }
#endif
#endif
  return (false);
}


/******************************************************************************
   SD DRUMSETTINGS
 ******************************************************************************/
FLASHMEM bool load_sd_drumsettings_json(uint8_t number) {
#if NUM_DRUMS > 0
  if (number < 0)
    return (false);

  if (sd_card > 0) {
    number = constrain(number, PERFORMANCE_NUM_MIN, PERFORMANCE_NUM_MAX);
    snprintf_P(filename, sizeof(filename), PSTR("/%s/%d/%s.json"), PERFORMANCE_CONFIG_PATH, number, DRUMS_CONFIG_NAME);

    // first check if file exists...
    AudioNoInterrupts();
    if (SD.exists(filename)) {
      // ... and if: load
#ifdef DEBUG
      Serial.print(F("Found drums configuration ["));
      Serial.print(filename);
      Serial.println(F("]... loading..."));
#endif
      json = SD.open(filename);
      if (json) {
        StaticJsonDocument<JSON_BUFFER_SIZE> data_json;
        deserializeJson(data_json, json);
        json.close();
        AudioInterrupts();
#if defined(DEBUG) && defined(DEBUG_SHOW_JSON)
        Serial.println(F("Read JSON data:"));
        serializeJsonPretty(data_json, Serial);
        Serial.println();
#endif
        seq.drums_volume = data_json["drums_volume"];
        set_drums_volume(seq.drums_volume);
        for (uint8_t i = 0; i < NUM_DRUMSET_CONFIG - 1; i++) {
          uint8_t drumnumber = 0;
          drumnumber = find_drum_number_from_note(data_json["note"][i]);
          if (((int)data_json["note"][i] > 0 && find_drum_number_from_note(data_json["note"][i]) > 0) || (i == 0 && (int)data_json["note"][i] == 210)) {
            set_sample_pitch(drumnumber, data_json["pitch"][i]);
            set_sample_p_offset(drumnumber, data_json["p_offset"][i]);
            set_sample_pan(drumnumber, data_json["pan"][i]);
            set_sample_vol_max(drumnumber, data_json["vol_max"][i]);
            set_sample_vol_min(drumnumber, data_json["vol_min"][i]);
            set_sample_reverb_send(drumnumber, data_json["reverb_send"][i]);
          }
        }
        return (true);
      }
#ifdef DEBUG
      else {
        Serial.print(F("E : Cannot open "));
        Serial.print(filename);
        Serial.println(F(" on SD."));
      }
    } else {
      Serial.print(F("No "));
      Serial.print(filename);
      Serial.println(F(" available."));
#endif
    }
  }
#endif
  return (false);
}

FLASHMEM bool save_sd_drumsettings_json(uint8_t number) {
#if NUM_DRUMS > 0
  if (sd_card > 0) {
    number = constrain(number, PERFORMANCE_NUM_MIN, PERFORMANCE_NUM_MAX);
    if (check_performance_directory(number)) {
      snprintf_P(filename, sizeof(filename), PSTR("/%s/%d/%s.json"), PERFORMANCE_CONFIG_PATH, number, DRUMS_CONFIG_NAME);

#ifdef DEBUG
      Serial.print(F("Saving drums config "));
      Serial.print(number);
      Serial.print(F(" to "));
      Serial.println(filename);
#endif
      AudioNoInterrupts();
      if (SD.exists(filename)) {
#ifdef DEBUG
        Serial.println(F("remove old drumsettings file"));
#endif
        SD.remove(filename);
      }
      json = SD.open(filename, FILE_WRITE);
      if (json) {
        StaticJsonDocument<JSON_BUFFER_SIZE> data_json;
        data_json["drums_volume"] = seq.drums_volume;
        for (uint8_t i = 0; i < NUM_DRUMSET_CONFIG - 1; i++) {
          data_json["note"][i] = get_sample_note(i);
          data_json["pitch"][i] = get_sample_pitch(i);
          data_json["p_offset"][i] = get_sample_p_offset(i);
          data_json["pan"][i] = get_sample_pan(i);
          data_json["vol_max"][i] = get_sample_vol_max(i);
          data_json["vol_min"][i] = get_sample_vol_min(i);
          data_json["reverb_send"][i] = get_sample_reverb_send(i);
        }
#if defined(DEBUG) && defined(DEBUG_SHOW_JSON)
        Serial.println(F("Write JSON data:"));
        serializeJsonPretty(data_json, Serial);
        Serial.println();
#endif
        serializeJsonPretty(data_json, json);
        json.close();
        AudioInterrupts();
        return (true);
      } else {
#ifdef DEBUG
        Serial.print(F("E : Cannot open "));
        Serial.print(filename);
        Serial.println(F(" on SD."));
#endif
        AudioInterrupts();
        return (false);
      }
    } else {
      AudioInterrupts();
      return (false);
    }
  }
#ifdef DEBUG
  else {
    Serial.println(F("E: SD card not available"));
  }
#endif
#endif
  return (false);
}

/******************************************************************************
   SD VOICECONFIG
 ******************************************************************************/
FLASHMEM bool load_sd_voiceconfig_json(uint8_t vc, uint8_t instance_id) {
  if (sd_card > 0) {
    vc = constrain(vc, PERFORMANCE_NUM_MIN, PERFORMANCE_NUM_MAX);
    snprintf_P(filename, sizeof(filename), PSTR("/%s/%d/%s%d.json"), PERFORMANCE_CONFIG_PATH, vc, VOICE_CONFIG_NAME, instance_id + 1);

    // first check if file exists...
    AudioNoInterrupts();
    if (SD.exists(filename)) {
      // ... and if: load
#ifdef DEBUG
      Serial.print(F("Found voice configuration ["));
      Serial.print(filename);
      Serial.println(F("]... loading..."));
#endif
      json = SD.open(filename);
      if (json) {
        StaticJsonDocument<JSON_BUFFER_SIZE> data_json;
        deserializeJson(data_json, json);

        json.close();
        AudioInterrupts();

#if defined(DEBUG) && defined(DEBUG_SHOW_JSON)
        Serial.println(F("Read JSON data:"));
        serializeJsonPretty(data_json, Serial);
        Serial.println();
#endif
        configuration.dexed[instance_id].bank = data_json["bank"];
        configuration.dexed[instance_id].voice = data_json["voice"];
        configuration.dexed[instance_id].lowest_note = data_json["lowest_note"];
        configuration.dexed[instance_id].highest_note = data_json["highest_note"];
        configuration.dexed[instance_id].transpose = data_json["transpose"];
        configuration.dexed[instance_id].tune = data_json["tune"];
        configuration.dexed[instance_id].sound_intensity = data_json["sound_intensity"];
        configuration.dexed[instance_id].pan = data_json["pan"];
        configuration.dexed[instance_id].polyphony = data_json["polyphony"];
        configuration.dexed[instance_id].velocity_level = data_json["velocity_level"];
        configuration.dexed[instance_id].monopoly = data_json["monopoly"];
        configuration.dexed[instance_id].note_refresh = data_json["note_refresh"];
        configuration.dexed[instance_id].pb_range = data_json["pb_range"];
        configuration.dexed[instance_id].pb_step = data_json["pb_step"];
        configuration.dexed[instance_id].mw_range = data_json["mw_range"];
        configuration.dexed[instance_id].mw_assign = data_json["mw_assign"];
        configuration.dexed[instance_id].mw_mode = data_json["mw_mode"];
        configuration.dexed[instance_id].fc_range = data_json["fc_range"];
        configuration.dexed[instance_id].fc_assign = data_json["fc_assign"];
        configuration.dexed[instance_id].fc_mode = data_json["fc_mode"];
        configuration.dexed[instance_id].bc_range = data_json["bc_range"];
        configuration.dexed[instance_id].bc_assign = data_json["bc_assign"];
        configuration.dexed[instance_id].bc_mode = data_json["bc_mode"];
        configuration.dexed[instance_id].at_range = data_json["at_range"];
        configuration.dexed[instance_id].at_assign = data_json["at_assign"];
        configuration.dexed[instance_id].at_mode = data_json["at_mode"];
        configuration.dexed[instance_id].portamento_mode = data_json["portamento_mode"];
        configuration.dexed[instance_id].portamento_glissando = data_json["portamento_glissando"];
        configuration.dexed[instance_id].portamento_time = data_json["portamento_time"];
        configuration.dexed[instance_id].op_enabled = data_json["op_enabled"];
        configuration.dexed[instance_id].midi_channel = data_json["midi_channel"];

        check_configuration_dexed(instance_id);
        set_voiceconfig_params(instance_id);

        return (true);
      }
#ifdef DEBUG
      else {
        Serial.print(F("E : Cannot open "));
        Serial.print(filename);
        Serial.println(F(" on SD."));
      }
    } else {
      Serial.print(F("No "));
      Serial.print(filename);
      Serial.println(F(" available."));
#endif
    }
  }

  return (false);
}

FLASHMEM bool save_sd_voiceconfig_json(uint8_t vc, uint8_t instance_id) {
  if (sd_card > 0) {
    vc = constrain(vc, PERFORMANCE_NUM_MIN, PERFORMANCE_NUM_MAX);
    snprintf_P(filename, sizeof(filename), PSTR("/%s/%d/%s%d.json"), PERFORMANCE_CONFIG_PATH, vc, VOICE_CONFIG_NAME, instance_id + 1);

#ifdef DEBUG
    Serial.print(F("Saving voice config "));
    Serial.print(vc);
    Serial.print(F("["));
    Serial.print(instance_id);
    Serial.print(F("]"));
    Serial.print(F(" to "));
    Serial.println(filename);
#endif

    SD.remove(filename);
    json = SD.open(filename, FILE_WRITE);
    if (json) {
      StaticJsonDocument<JSON_BUFFER_SIZE> data_json;
      data_json["bank"] = configuration.dexed[instance_id].bank;
      data_json["voice"] = configuration.dexed[instance_id].voice;
      data_json["lowest_note"] = configuration.dexed[instance_id].lowest_note;
      data_json["highest_note"] = configuration.dexed[instance_id].highest_note;
      data_json["transpose"] = configuration.dexed[instance_id].transpose;
      data_json["tune"] = configuration.dexed[instance_id].tune;
      data_json["sound_intensity"] = configuration.dexed[instance_id].sound_intensity;
      data_json["pan"] = configuration.dexed[instance_id].pan;
      data_json["polyphony"] = configuration.dexed[instance_id].polyphony;
      data_json["velocity_level"] = configuration.dexed[instance_id].velocity_level;
      data_json["monopoly"] = configuration.dexed[instance_id].monopoly;
      data_json["monopoly"] = configuration.dexed[instance_id].monopoly;
      data_json["note_refresh"] = configuration.dexed[instance_id].note_refresh;
      data_json["pb_range"] = configuration.dexed[instance_id].pb_range;
      data_json["pb_step"] = configuration.dexed[instance_id].pb_step;
      data_json["mw_range"] = configuration.dexed[instance_id].mw_range;
      data_json["mw_assign"] = configuration.dexed[instance_id].mw_assign;
      data_json["mw_mode"] = configuration.dexed[instance_id].mw_mode;
      data_json["fc_range"] = configuration.dexed[instance_id].fc_range;
      data_json["fc_assign"] = configuration.dexed[instance_id].fc_assign;
      data_json["fc_mode"] = configuration.dexed[instance_id].fc_mode;
      data_json["bc_range"] = configuration.dexed[instance_id].bc_range;
      data_json["bc_assign"] = configuration.dexed[instance_id].bc_assign;
      data_json["bc_mode"] = configuration.dexed[instance_id].bc_mode;
      data_json["at_range"] = configuration.dexed[instance_id].at_range;
      data_json["at_assign"] = configuration.dexed[instance_id].at_assign;
      data_json["at_mode"] = configuration.dexed[instance_id].at_mode;
      data_json["portamento_mode"] = configuration.dexed[instance_id].portamento_mode;
      data_json["portamento_glissando"] = configuration.dexed[instance_id].portamento_glissando;
      data_json["portamento_time"] = configuration.dexed[instance_id].portamento_time;
      data_json["op_enabled"] = configuration.dexed[instance_id].op_enabled;
      data_json["midi_channel"] = configuration.dexed[instance_id].midi_channel;

#if defined(DEBUG) && defined(DEBUG_SHOW_JSON)
      Serial.println(F("Write JSON data:"));
      serializeJsonPretty(data_json, Serial);
      Serial.println();
#endif
      serializeJsonPretty(data_json, json);
      json.close();
      AudioInterrupts();

      return (true);
    }
    json.close();
  } else {
#ifdef DEBUG
    Serial.print(F("E : Cannot open "));
    Serial.print(filename);
    Serial.println(F(" on SD."));
#endif
  }

  return (false);
}

/******************************************************************************
   SD MICROSYNTH
 ******************************************************************************/
FLASHMEM bool load_sd_microsynth_json(uint8_t ms, uint8_t instance_id) {
#ifdef USE_MICROSYNTH
  if (sd_card > 0) {
    ms = constrain(ms, PERFORMANCE_NUM_MIN, PERFORMANCE_NUM_MAX);
    snprintf_P(filename, sizeof(filename), PSTR("/%s/%d/%s%d.json"), PERFORMANCE_CONFIG_PATH, ms, MICROSYNTH_CONFIG_NAME, instance_id + 1);

    // first check if file exists...
    AudioNoInterrupts();
    if (SD.exists(filename)) {
      // ... and if: load
#ifdef DEBUG
      Serial.print(F("Found micro synth configuration ["));
      Serial.print(filename);
      Serial.println(F("]... loading..."));
#endif
      json = SD.open(filename);
      if (json) {
        StaticJsonDocument<JSON_BUFFER_SIZE> data_json;
        deserializeJson(data_json, json);

        json.close();
        AudioInterrupts();

#if defined(DEBUG) && defined(DEBUG_SHOW_JSON)
        Serial.println(F("Read JSON data:"));
        serializeJsonPretty(data_json, Serial);
        Serial.println();
#endif
        microsynth[instance_id].sound_intensity = data_json["sound_intensity"];
        microsynth[instance_id].wave = data_json["wave"];
        microsynth[instance_id].coarse = data_json["coarse"];
        microsynth[instance_id].detune = data_json["detune"];
        microsynth[instance_id].trigger_noise_with_osc = data_json["trigger_noise_with_osc"];
        microsynth[instance_id].env_attack = data_json["env_attack"];
        microsynth[instance_id].env_decay = data_json["env_decay"];
        microsynth[instance_id].env_sustain = data_json["env_sustain"];
        microsynth[instance_id].env_release = data_json["env_release"];
        microsynth[instance_id].filter_osc_mode = data_json["filter_osc_mode"];
        microsynth[instance_id].filter_osc_freq_from = data_json["filter_osc_freq_from"];
        microsynth[instance_id].filter_osc_freq_to = data_json["filter_osc_freq_to"];
        microsynth[instance_id].filter_osc_speed = data_json["filter_osc_speed"];
        microsynth[instance_id].filter_osc_resonance = data_json["filter_osc_resonance"];
        microsynth[instance_id].noise_vol = data_json["noise_vol"];
        microsynth[instance_id].noise_decay = data_json["noise_decay"];
        microsynth[instance_id].filter_noise_freq_from = data_json["filter_noise_freq_from"];
        microsynth[instance_id].filter_noise_freq_to = data_json["filter_noise_freq_to"];
        microsynth[instance_id].filter_noise_speed = data_json["filter_noise_speed"];
        microsynth[instance_id].filter_noise_mode = data_json["filter_noise_mode"];
        microsynth[instance_id].pwm_from = data_json["pwm_from"];
        microsynth[instance_id].pwm_to = data_json["pwm_to"];
        microsynth[instance_id].pwm_speed = data_json["pwm_speed"];
        microsynth[instance_id].rev_send = data_json["rev_send"];
        microsynth[instance_id].chorus_send = data_json["chorus_send"];
        microsynth[instance_id].delay_send = data_json["delay_send"];
        microsynth[instance_id].midi_channel = data_json["midi_channel"];
        microsynth[instance_id].pan = data_json["pan"];
        microsynth[instance_id].vel_mod_filter_osc = data_json["vel_mod_filter_osc"];
        microsynth[instance_id].vel_mod_filter_noise = data_json["vel_mod_filter_noise"];

        microsynth_update_all_settings(instance_id);

        return (true);
      }
#ifdef DEBUG
      else {
        Serial.print(F("E : Cannot open "));
        Serial.print(filename);
        Serial.println(F(" on SD."));
      }
    } else {
      Serial.print(F("No "));
      Serial.print(filename);
      Serial.println(F(" available."));
#endif
    }
  }
#endif
  return (false);
}

FLASHMEM bool save_sd_microsynth_json(uint8_t ms, uint8_t instance_id) {
#ifdef USE_MICROSYNTH
  if (sd_card > 0) {
    ms = constrain(ms, PERFORMANCE_NUM_MIN, PERFORMANCE_NUM_MAX);
    snprintf_P(filename, sizeof(filename), PSTR("/%s/%d/%s%d.json"), PERFORMANCE_CONFIG_PATH, ms, MICROSYNTH_CONFIG_NAME, instance_id + 1);

#ifdef DEBUG
    Serial.print(F("Saving microsynth config "));
    Serial.print(ms);
    Serial.print(F("["));
    Serial.print(instance_id);
    Serial.print(F("]"));
    Serial.print(F(" to "));
    Serial.println(filename);
#endif

    AudioNoInterrupts();
    SD.remove(filename);
    json = SD.open(filename, FILE_WRITE);
    if (json) {
      StaticJsonDocument<JSON_BUFFER_SIZE> data_json;
      data_json["sound_intensity"] = microsynth[instance_id].sound_intensity;
      data_json["wave"] = microsynth[instance_id].wave;
      data_json["coarse"] = microsynth[instance_id].coarse;
      data_json["detune"] = microsynth[instance_id].detune;
      data_json["trigger_noise_with_osc"] = microsynth[instance_id].trigger_noise_with_osc;
      data_json["env_attack"] = microsynth[instance_id].env_attack;
      data_json["env_decay"] = microsynth[instance_id].env_decay;
      data_json["env_sustain"] = microsynth[instance_id].env_sustain;
      data_json["env_release"] = microsynth[instance_id].env_release;
      data_json["filter_osc_mode"] = microsynth[instance_id].filter_osc_mode;
      data_json["filter_osc_freq_from"] = microsynth[instance_id].filter_osc_freq_from;
      data_json["filter_osc_freq_to"] = microsynth[instance_id].filter_osc_freq_to;
      data_json["filter_osc_speed"] = microsynth[instance_id].filter_osc_speed;
      data_json["filter_osc_resonance"] = microsynth[instance_id].filter_osc_resonance;
      data_json["noise_vol"] = microsynth[instance_id].noise_vol;
      data_json["noise_decay"] = microsynth[instance_id].noise_decay;
      data_json["filter_noise_freq_from"] = microsynth[instance_id].filter_noise_freq_from;
      data_json["filter_noise_freq_to"] = microsynth[instance_id].filter_noise_freq_to;
      data_json["filter_noise_mode"] = microsynth[instance_id].filter_noise_mode;
      data_json["filter_noise_speed"] = microsynth[instance_id].filter_noise_speed;
      data_json["pwm_from"] = microsynth[instance_id].pwm_from;
      data_json["pwm_to"] = microsynth[instance_id].pwm_to;
      data_json["pwm_speed"] = microsynth[instance_id].pwm_speed;
      data_json["rev_send"] = microsynth[instance_id].rev_send;
      data_json["chorus_send"] = microsynth[instance_id].chorus_send;
      data_json["delay_send"] = microsynth[instance_id].delay_send;
      data_json["midi_channel"] = microsynth[instance_id].midi_channel;
      data_json["pan"] = microsynth[instance_id].pan;
      data_json["vel_mod_filter_osc"] = microsynth[instance_id].vel_mod_filter_osc;
      data_json["vel_mod_filter_noise"] = microsynth[instance_id].vel_mod_filter_noise;

#if defined(DEBUG) && defined(DEBUG_SHOW_JSON)
      Serial.println(F("Write JSON data:"));
      serializeJsonPretty(data_json, Serial);
      Serial.println();
#endif
      serializeJsonPretty(data_json, json);
      json.close();
      AudioInterrupts();

      return (true);
    }
    json.close();
  } else {
#ifdef DEBUG
    Serial.print(F("E : Cannot open "));
    Serial.print(filename);
    Serial.println(F(" on SD."));
#endif
  }
#endif
  return (false);
}

/******************************************************************************
   SD FX
 ******************************************************************************/
FLASHMEM bool load_sd_fx_json(uint8_t number) {
  number = constrain(number, PERFORMANCE_NUM_MIN, PERFORMANCE_NUM_MAX);

  load_sd_drumsettings_json(number);

  if (sd_card > 0) {
    snprintf_P(filename, sizeof(filename), PSTR("/%s/%d/%s.json"), PERFORMANCE_CONFIG_PATH, number, FX_CONFIG_NAME);

    // first check if file exists...
    AudioNoInterrupts();
    if (SD.exists(filename)) {
      // ... and if: load
#ifdef DEBUG
      Serial.print(F("Found fx configuration ["));
      Serial.print(filename);
      Serial.println(F("]... loading..."));
#endif
      json = SD.open(filename);
      if (json) {
        StaticJsonDocument<JSON_BUFFER_SIZE> data_json;
        deserializeJson(data_json, json);

        json.close();
        AudioInterrupts();

#if defined(DEBUG) && defined(DEBUG_SHOW_JSON)
        Serial.println(F("Read JSON data:"));
        serializeJsonPretty(data_json, Serial);
        Serial.println();
#endif
        for (uint8_t i = 0; i < MAX_DEXED; i++) {
          configuration.fx.filter_cutoff[i] = data_json["filter_cutoff"][i];
          configuration.fx.filter_resonance[i] = data_json["filter_resonance"][i];
          configuration.fx.chorus_frequency[i] = data_json["chorus_frequency"][i];
          configuration.fx.chorus_waveform[i] = data_json["chorus_waveform"][i];
          configuration.fx.chorus_depth[i] = data_json["chorus_depth"][i];
          configuration.fx.chorus_level[i] = data_json["chorus_level"][i];
          configuration.fx.delay_time[i] = data_json["delay_time"][i];
          configuration.fx.delay_feedback[i] = data_json["delay_feedback"][i];
          configuration.fx.delay_level[i] = data_json["delay_level"][i];
          configuration.fx.delay_sync[i] = data_json["delay_sync"][i];
          configuration.fx.reverb_send[i] = data_json["reverb_send"][i];
          if (configuration.fx.delay_sync[i] > 0) {
            configuration.fx.delay_time[i] = 0;
          }
        }
        configuration.fx.reverb_roomsize = data_json["reverb_roomsize"];
        configuration.fx.reverb_damping = data_json["reverb_damping"];
        configuration.fx.reverb_lowpass = data_json["reverb_lowpass"];
        configuration.fx.reverb_lodamp = data_json["reverb_lodamp"];
        configuration.fx.reverb_hidamp = data_json["reverb_hidamp"];
        configuration.fx.reverb_diffusion = data_json["reverb_diffusion"];
        configuration.fx.reverb_level = data_json["reverb_level"];
        configuration.fx.eq_1 = data_json["eq_1"];
        configuration.fx.eq_2 = data_json["eq_2"];
        configuration.fx.eq_3 = data_json["eq_3"];
        configuration.fx.eq_4 = data_json["eq_4"];
        configuration.fx.eq_5 = data_json["eq_5"];
        configuration.fx.eq_6 = data_json["eq_6"];
        configuration.fx.eq_7 = data_json["eq_7"];
        configuration.fx.ep_chorus_frequency = data_json["ep_chorus_frequency"];
        configuration.fx.ep_chorus_waveform = data_json["ep_chorus_waveform"];
        configuration.fx.ep_chorus_depth = data_json["ep_chorus_depth"];
        configuration.fx.ep_chorus_level = data_json["ep_chorus_level"];
        configuration.fx.ep_reverb_send = data_json["ep_reverb_send"];

        check_configuration_fx();
        set_fx_params();

        return (true);
      }
#ifdef DEBUG
      else {
        Serial.print(F("E : Cannot open "));
        Serial.print(filename);
        Serial.println(F(" on SD."));
      }
    } else {
      Serial.print(F("No "));
      Serial.print(filename);
      Serial.println(F(" available."));
#endif
    }
  }
  return (false);
}

FLASHMEM bool save_sd_fx_json(uint8_t number) {
  number = constrain(number, PERFORMANCE_NUM_MIN, PERFORMANCE_NUM_MAX);

  save_sd_drumsettings_json(number);

  if (sd_card > 0) {
    snprintf_P(filename, sizeof(filename), PSTR("/%s/%d/%s.json"), PERFORMANCE_CONFIG_PATH, number, FX_CONFIG_NAME);

#ifdef DEBUG
    Serial.print(F("Saving fx config "));
    Serial.print(number);
    Serial.print(F(" to "));
    Serial.println(filename);
#endif

    AudioNoInterrupts();
    SD.remove(filename);
    json = SD.open(filename, FILE_WRITE);
    if (json) {
      StaticJsonDocument<JSON_BUFFER_SIZE> data_json;
      for (uint8_t i = 0; i < MAX_DEXED; i++) {
        data_json["filter_cutoff"][i] = configuration.fx.filter_cutoff[i];
        data_json["filter_resonance"][i] = configuration.fx.filter_resonance[i];
        data_json["chorus_frequency"][i] = configuration.fx.chorus_frequency[i];
        data_json["chorus_waveform"][i] = configuration.fx.chorus_waveform[i];
        data_json["chorus_depth"][i] = configuration.fx.chorus_depth[i];
        data_json["chorus_level"][i] = configuration.fx.chorus_level[i];
        data_json["delay_time"][i] = configuration.fx.delay_time[i];
        data_json["delay_feedback"][i] = configuration.fx.delay_feedback[i];
        data_json["delay_level"][i] = configuration.fx.delay_level[i];
        data_json["delay_sync"][i] = configuration.fx.delay_sync[i];
        data_json["reverb_send"][i] = configuration.fx.reverb_send[i];
      }
      data_json["reverb_roomsize"] = configuration.fx.reverb_roomsize;
      data_json["reverb_damping"] = configuration.fx.reverb_damping;
      data_json["reverb_lowpass"] = configuration.fx.reverb_lowpass;
      data_json["reverb_lodamp"] = configuration.fx.reverb_lodamp;
      data_json["reverb_hidamp"] = configuration.fx.reverb_hidamp;
      data_json["reverb_diffusion"] = configuration.fx.reverb_diffusion;
      data_json["reverb_level"] = configuration.fx.reverb_level;
      data_json["eq_1"] = configuration.fx.eq_1;
      data_json["eq_2"] = configuration.fx.eq_2;
      data_json["eq_3"] = configuration.fx.eq_3;
      data_json["eq_4"] = configuration.fx.eq_4;
      data_json["eq_5"] = configuration.fx.eq_5;
      data_json["eq_6"] = configuration.fx.eq_6;
      data_json["eq_7"] = configuration.fx.eq_7;
      data_json["ep_chorus_frequency"] = configuration.fx.ep_chorus_frequency;
      data_json["ep_chorus_waveform"] = configuration.fx.ep_chorus_waveform;
      data_json["ep_chorus_depth"] = configuration.fx.ep_chorus_depth;
      data_json["ep_chorus_level"] = configuration.fx.ep_chorus_level;
      data_json["ep_reverb_send"] = configuration.fx.ep_reverb_send;

#if defined(DEBUG) && defined(DEBUG_SHOW_JSON)
      Serial.println(F("Write JSON data:"));
      serializeJsonPretty(data_json, Serial);
      Serial.println();
#endif
      serializeJsonPretty(data_json, json);
      json.close();
      AudioInterrupts();
      return (true);
    }
    json.close();
  } else {
#ifdef DEBUG
    Serial.print(F("E : Cannot open "));
    Serial.print(filename);
    Serial.println(F(" on SD."));
#endif
  }
  return (false);
}

/******************************************************************************
   SD EPIANO
 ******************************************************************************/
FLASHMEM bool load_sd_epiano_json(uint8_t number) {
  if (sd_card > 0) {
    number = constrain(number, PERFORMANCE_NUM_MIN, PERFORMANCE_NUM_MAX);
    snprintf_P(filename, sizeof(filename), PSTR("/%s/%d/%s.json"), PERFORMANCE_CONFIG_PATH, number, EPIANO_CONFIG_NAME);

    // first check if file exists...
    AudioNoInterrupts();
    if (SD.exists(filename)) {
      // ... and if: load
#ifdef DEBUG
      Serial.print(F("Found epiano configuration ["));
      Serial.print(filename);
      Serial.println(F("]... loading..."));
#endif
      json = SD.open(filename);
      if (json) {
        StaticJsonDocument<JSON_BUFFER_SIZE> data_json;
        deserializeJson(data_json, json);

        json.close();
        AudioInterrupts();

#if defined(DEBUG) && defined(DEBUG_SHOW_JSON)
        Serial.println(F("Read JSON data:"));
        serializeJsonPretty(data_json, Serial);
        Serial.println();
#endif
        configuration.epiano.decay = data_json["decay"];
        configuration.epiano.release = data_json["release"];
        configuration.epiano.hardness = data_json["hardness"];
        configuration.epiano.treble = data_json["treble"];
        configuration.epiano.pan_tremolo = data_json["pan_tremolo"];
        configuration.epiano.pan_lfo = data_json["pan_lf"];
        configuration.epiano.velocity_sense = data_json["velocity"];
        configuration.epiano.stereo = data_json["stereo"];
        configuration.epiano.polyphony = data_json["polyphony"];
        configuration.epiano.tune = data_json["tune"];
        configuration.epiano.detune = data_json["detune"];
        configuration.epiano.overdrive = data_json["overdrive"];
        configuration.epiano.lowest_note = data_json["lowest_note"];
        configuration.epiano.highest_note = data_json["highest_note"];
        configuration.epiano.transpose = data_json["transpose"];
        configuration.epiano.sound_intensity = data_json["sound_intensity"];
        configuration.epiano.pan = data_json["pan"];
        configuration.epiano.midi_channel = data_json["midi_channel"];

        check_configuration_epiano();
        set_epiano_params();

        return (true);
      }
#ifdef DEBUG
      else {
        Serial.print(F("E : Cannot open "));
        Serial.print(filename);
        Serial.println(F(" on SD."));
      }
    } else {
      Serial.print(F("No "));
      Serial.print(filename);
      Serial.println(F(" available."));
#endif
    }
  }
  return (false);
}

FLASHMEM bool save_sd_epiano_json(uint8_t number) {
  if (sd_card > 0) {
    number = constrain(number, PERFORMANCE_NUM_MIN, PERFORMANCE_NUM_MAX);
    snprintf_P(filename, sizeof(filename), PSTR("/%s/%d/%s.json"), PERFORMANCE_CONFIG_PATH, number, EPIANO_CONFIG_NAME);

#ifdef DEBUG
    Serial.print(F("Saving epiano config "));
    Serial.print(number);
    Serial.print(F(" to "));
    Serial.println(filename);
#endif

    AudioNoInterrupts();
    SD.remove(filename);
    json = SD.open(filename, FILE_WRITE);
    if (json) {
      StaticJsonDocument<JSON_BUFFER_SIZE> data_json;
      data_json["decay"] = configuration.epiano.decay;
      data_json["release"] = configuration.epiano.release;
      data_json["hardness"] = configuration.epiano.hardness;
      data_json["treble"] = configuration.epiano.treble;
      data_json["pan_tremolo"] = configuration.epiano.pan_tremolo;
      data_json["pan_lfo"] = configuration.epiano.pan_lfo;
      data_json["velocity_sense"] = configuration.epiano.velocity_sense;
      data_json["stereo"] = configuration.epiano.stereo;
      data_json["polyphony"] = configuration.epiano.polyphony;
      data_json["tune"] = configuration.epiano.tune;
      data_json["detune"] = configuration.epiano.detune;
      data_json["overdrive"] = configuration.epiano.overdrive;
      data_json["lowest_note"] = configuration.epiano.lowest_note;
      data_json["highest_note"] = configuration.epiano.highest_note;
      data_json["transpose"] = configuration.epiano.transpose;
      data_json["sound_intensity"] = configuration.epiano.sound_intensity;
      data_json["pan"] = configuration.epiano.pan;
      data_json["midi_channel"] = configuration.epiano.midi_channel;

#if defined(DEBUG) && defined(DEBUG_SHOW_JSON)
      Serial.println(F("Write JSON data:"));
      serializeJsonPretty(data_json, Serial);
      Serial.println();
#endif
      serializeJsonPretty(data_json, json);
      json.close();
      AudioInterrupts();
      return (true);
    }
    json.close();
  } else {
#ifdef DEBUG
    Serial.print(F("E : Cannot open "));
    Serial.print(filename);
    Serial.println(F(" on SD."));
#endif
  }

  AudioInterrupts();
  return (false);
}

/******************************************************************************
   SD SYS
 ******************************************************************************/
FLASHMEM bool load_sd_sys_json(void) {
  if (sd_card > 0) {
    snprintf_P(filename, sizeof(filename), PSTR("/%s.json"), SYS_CONFIG_NAME);

    // first check if file exists...
    AudioNoInterrupts();
    if (SD.exists(filename)) {
      // ... and if: load
#ifdef DEBUG
      Serial.println(F("Found sys configuration"));
#endif
      json = SD.open(filename);
      if (json) {
        StaticJsonDocument<JSON_BUFFER_SIZE> data_json;
        deserializeJson(data_json, json);

        json.close();
        AudioInterrupts();

#if defined(DEBUG) && defined(DEBUG_SHOW_JSON)
        Serial.println(F("Read JSON data:"));
        serializeJsonPretty(data_json, Serial);
        Serial.println();
#endif

        configuration.sys.vol = data_json["vol"];
        configuration.sys.mono = data_json["mono"];
        configuration.sys.soft_midi_thru = data_json["soft_midi_thru"];
        configuration.sys.performance_number = data_json["performance_number"];
        configuration.sys.favorites = data_json["favorites"];
        configuration.sys.load_at_startup_performance = data_json["load_at_startup_performance"];
        configuration.sys.load_at_startup_page = data_json["load_at_startup_page"];
        if (data_json.containsKey("display_rotation")) {
          configuration.sys.display_rotation = data_json["display_rotation"];
          configuration.sys.touch_rotation = data_json["touch_rotation"];
          configuration.sys.ui_reverse = data_json["ui_reverse"];
          configuration.sys.screen_saver_start = data_json["screen_saver_start"];
        }
        if (data_json.containsKey("gp_speed"))
          configuration.sys.gamepad_speed = data_json["gp_speed"];
        if (data_json["gp_a"] != data_json["gp_b"]) {
          GAMEPAD_UP_0 = data_json["gp_up_0"];
          GAMEPAD_UP_1 = data_json["gp_up_1"];
          GAMEPAD_DOWN_0 = data_json["gp_down_0"];
          GAMEPAD_DOWN_1 = data_json["gp_down_1"];
          GAMEPAD_LEFT_0 = data_json["gp_left_0"];
          GAMEPAD_LEFT_1 = data_json["gp_left_1"];
          GAMEPAD_RIGHT_0 = data_json["gp_right_0"];
          GAMEPAD_RIGHT_1 = data_json["gp_right_1"];
          GAMEPAD_SELECT = data_json["gp_select"];
          GAMEPAD_START = data_json["gp_start"];
          GAMEPAD_BUTTON_A = data_json["gp_a"];
          GAMEPAD_BUTTON_B = data_json["gp_b"];
        }
        check_configuration_sys();
        set_sys_params();

        return (true);
      }
#ifdef DEBUG
      else {
        Serial.print(F("E : Cannot open "));
        Serial.print(filename);
        Serial.println(F(" on SD."));
      }
    } else {
      Serial.print(F("No "));
      Serial.print(filename);
      Serial.println(F(" available."));
#endif
    }
  }

  AudioInterrupts();
  return (false);
}

FLASHMEM bool save_sd_sys_json(void) {
  if (sd_card > 0) {
    snprintf_P(filename, sizeof(filename), PSTR("/%s.json"), SYS_CONFIG_NAME);

#ifdef DEBUG
    Serial.print(F("Saving sys config to "));
    Serial.println(filename);
#endif

    SD.remove(filename);
    json = SD.open(filename, FILE_WRITE);
    if (json) {
      StaticJsonDocument<JSON_BUFFER_SIZE> data_json;
      data_json["vol"] = configuration.sys.vol;
      data_json["mono"] = configuration.sys.mono;
      data_json["soft_midi_thru"] = configuration.sys.soft_midi_thru;
      data_json["performance_number"] = configuration.sys.performance_number;
      data_json["favorites"] = configuration.sys.favorites;
      data_json["load_at_startup_performance"] = configuration.sys.load_at_startup_performance;
      data_json["load_at_startup_page"] = configuration.sys.load_at_startup_page;
      data_json["display_rotation"] = configuration.sys.display_rotation;
      data_json["touch_rotation"] = configuration.sys.touch_rotation;
      data_json["ui_reverse"] = configuration.sys.ui_reverse;
      data_json["screen_saver_start"] = configuration.sys.screen_saver_start;
      data_json["gp_speed"] = configuration.sys.gamepad_speed;
      if (GAMEPAD_BUTTON_A != GAMEPAD_BUTTON_B) {
        data_json["gp_up_0"] = GAMEPAD_UP_0;
        data_json["gp_up_1"] = GAMEPAD_UP_1;
        data_json["gp_down_0"] = GAMEPAD_DOWN_0;
        data_json["gp_down_1"] = GAMEPAD_DOWN_1;
        data_json["gp_left_0"] = GAMEPAD_LEFT_0;
        data_json["gp_left_1"] = GAMEPAD_LEFT_1;
        data_json["gp_right_0"] = GAMEPAD_RIGHT_0;
        data_json["gp_right_1"] = GAMEPAD_RIGHT_1;
        data_json["gp_select"] = GAMEPAD_SELECT;
        data_json["gp_start"] = GAMEPAD_START;
        data_json["gp_a"] = GAMEPAD_BUTTON_A;
        data_json["gp_b"] = GAMEPAD_BUTTON_B;
      }
#if defined(DEBUG) && defined(DEBUG_SHOW_JSON)
      Serial.println(F("Write JSON data:"));
      serializeJsonPretty(data_json, Serial);
      Serial.println();
#endif
      serializeJsonPretty(data_json, json);
      json.close();
      AudioInterrupts();
      return (true);
    }
    json.close();
  } else {
#ifdef DEBUG
    Serial.print(F("E : Cannot open "));
    Serial.print(filename);
    Serial.println(F(" on SD."));
#endif
  }

  AudioInterrupts();
  return (false);
}


/******************************************************************************
   SD BRAIDS
 ******************************************************************************/
#ifdef USE_BRAIDS
FLASHMEM bool load_sd_braids_json(uint8_t number) {
  if (number < 0)
    return (false);

  if (sd_card > 0) {
    number = constrain(number, PERFORMANCE_NUM_MIN, PERFORMANCE_NUM_MAX);
    snprintf_P(filename, sizeof(filename), PSTR("/%s/%d/%s.json"), PERFORMANCE_CONFIG_PATH, number, BRAIDS_CONFIG_NAME);

    // first check if file exists...
    AudioNoInterrupts();
    if (SD.exists(filename)) {
      // ... and if: load
#ifdef DEBUG
      Serial.print(F("Found braids configuration"));
#endif
      json = SD.open(filename);
      if (json) {
        StaticJsonDocument<JSON_BUFFER_SIZE> data_json;
        deserializeJson(data_json, json);

        json.close();
        AudioInterrupts();

#if defined(DEBUG) && defined(DEBUG_SHOW_JSON)
        Serial.println(F("Read JSON data:"));
        //serializeJsonPretty(data_json, Serial);
        Serial.println();
#endif

        braids_osc.sound_intensity = data_json["vol"];
        braids_osc.algo = data_json["algo"];
        braids_osc.color = data_json["color"];
        braids_osc.timbre = data_json["timbre"];
        braids_osc.coarse = data_json["coarse"];
        braids_osc.env_attack = data_json["attack"];
        braids_osc.env_decay = data_json["decay"];
        braids_osc.env_sustain = data_json["sustain"];
        braids_osc.env_release = data_json["release"];
        braids_osc.filter_mode = data_json["filter_mode"];
        braids_osc.filter_freq_from = data_json["freq_from"];
        braids_osc.filter_freq_to = data_json["freq_to"];
        braids_osc.filter_resonance = data_json["res"];
        braids_osc.filter_speed = data_json["filter_speed"];
        braids_osc.rev_send = data_json["rev"];
        braids_osc.flanger = data_json["flanger"];
        braids_osc.flanger_spread = data_json["flanger_spread"];
        braids_osc.delay_send = data_json["delay"];
        braids_osc.midi_channel = data_json["midi"];
        braids_osc.pan = data_json["pan"];

        braids_update_all_settings();
        return (true);
      }
#ifdef DEBUG
      else {
        Serial.print(F("E : Cannot open "));
        Serial.print(filename);
        Serial.println(F(" on SD."));
      }
    } else {
      Serial.print(F("No "));
      Serial.print(filename);
      Serial.println(F(" available."));
#endif
    }
  }

  AudioInterrupts();
  return (false);
}

FLASHMEM bool save_sd_braids_json(uint8_t number) {
  if (sd_card > 0) {
    snprintf_P(filename, sizeof(filename), PSTR("/%s/%d/%s.json"), PERFORMANCE_CONFIG_PATH, number, BRAIDS_CONFIG_NAME);
#ifdef DEBUG
    Serial.print(F("Saving braids"));
    Serial.print(number);
    Serial.print(F(" to "));
    Serial.println(filename);
#endif

    AudioNoInterrupts();
    SD.remove(filename);
    json = SD.open(filename, FILE_WRITE);
    if (json) {
      StaticJsonDocument<JSON_BUFFER_SIZE> data_json;
      data_json["vol"] = braids_osc.sound_intensity;
      data_json["algo"] = braids_osc.algo;
      data_json["color"] = braids_osc.color;
      data_json["timbre"] = braids_osc.timbre;
      data_json["coarse"] = braids_osc.coarse;
      data_json["attack"] = braids_osc.env_attack;
      data_json["decay"] = braids_osc.env_decay;
      data_json["sustain"] = braids_osc.env_sustain;
      data_json["release"] = braids_osc.env_release;
      data_json["filter_mode"] = braids_osc.filter_mode;
      data_json["freq_from"] = braids_osc.filter_freq_from;
      data_json["freq_to"] = braids_osc.filter_freq_to;
      data_json["res"] = braids_osc.filter_resonance;
      data_json["filter_speed"] = braids_osc.filter_speed;
      data_json["rev"] = braids_osc.rev_send;
      data_json["flanger"] = braids_osc.flanger;
      data_json["flanger_spread"] = braids_osc.flanger_spread;
      data_json["delay"] = braids_osc.delay_send;
      data_json["midi"] = braids_osc.midi_channel;
      data_json["pan"] = braids_osc.pan;
#if defined(DEBUG) && defined(DEBUG_SHOW_JSON)
      Serial.println(F("Write JSON data:"));
      serializeJsonPretty(data_json, Serial);
      Serial.println();
#endif
      serializeJsonPretty(data_json, json);
      json.close();
      AudioInterrupts();
      return (true);
    }
    json.close();
  } else {
#ifdef DEBUG
    Serial.print(F("E : Cannot open "));
    Serial.print(filename);
    Serial.println(F(" on SD."));
#endif
  }

  AudioInterrupts();
  return (false);
}
#endif

/******************************************************************************
   SD MULTIBAND
 ******************************************************************************/
#ifdef USE_MULTIBAND
FLASHMEM bool load_sd_multiband_json(uint8_t number) {
  if (number < 0)
    return (false);

  if (sd_card > 0) {
    number = constrain(number, PERFORMANCE_NUM_MIN, PERFORMANCE_NUM_MAX);
    snprintf_P(filename, sizeof(filename), PSTR("/%s/%d/%s.json"), PERFORMANCE_CONFIG_PATH, number, MULTIBAND_CONFIG_NAME);

    // first check if file exists...
    AudioNoInterrupts();
    if (SD.exists(filename)) {
      // ... and if: load
#ifdef DEBUG
      Serial.print(F("Found multiband configuration"));
#endif
      json = SD.open(filename);
      if (json) {
        StaticJsonDocument<JSON_BUFFER_SIZE> data_json;
        deserializeJson(data_json, json);

        json.close();
        AudioInterrupts();

#if defined(DEBUG) && defined(DEBUG_SHOW_JSON)
        Serial.println(F("Read JSON data:"));
        //serializeJsonPretty(data_json, Serial);
        Serial.println();
#endif

        mb_cross_freq_low = data_json["f_low"];
        mb_cross_freq_mid = data_json["f_mid"];
        mb_cross_freq_upper_mid = data_json["f_upper_mid"];
        mb_cross_freq_high = data_json["f_high"];
        mb_global_gain = data_json["g_gain"];
        mb_gain_low = data_json["g_low"];
        mb_gain_mid = data_json["g_mid"];
        mb_gain_upper_mid = data_json["g_upper_mid"];
        mb_gain_high = data_json["g_high"];
        multiband_active = data_json["active"];
        mb_threshold_low = data_json["t_low"];
        mb_threshold_mid = data_json["t_mid"];
        mb_threshold_upper_mid = data_json["t_upper_mid"];
        mb_threshold_low = data_json["t_low"];
        mb_q_low = data_json["q_low"];
        mb_q_mid = data_json["q_mid"];
        mb_q_upper_mid = data_json["q_upper_mid"];
        mb_q_high = data_json["q_high"];
        mb_solo_low = data_json["s_low"];
        mb_solo_mid = data_json["s_mid"];
        mb_solo_upper_mid = data_json["s_upper_mid"];
        mb_solo_high = data_json["s_high"];
        mb_global_ratio = data_json["g_ratio"];

        return (true);
      }
#ifdef DEBUG
      else {
        Serial.print(F("E : Cannot open "));
        Serial.print(filename);
        Serial.println(F(" on SD."));
      }
    } else {
      Serial.print(F("No "));
      Serial.print(filename);
      Serial.println(F(" available."));
#endif
    }
  }

  AudioInterrupts();
  return (false);
}

FLASHMEM bool save_sd_multiband_json(uint8_t number) {
  if (sd_card > 0) {
    snprintf_P(filename, sizeof(filename), PSTR("/%s/%d/%s.json"), PERFORMANCE_CONFIG_PATH, number, MULTIBAND_CONFIG_NAME);
#ifdef DEBUG
    Serial.print(F("Saving multiband"));
    Serial.print(number);
    Serial.print(F(" to "));
    Serial.println(filename);
#endif

    AudioNoInterrupts();
    SD.remove(filename);
    json = SD.open(filename, FILE_WRITE);
    if (json) {
      StaticJsonDocument<JSON_BUFFER_SIZE> data_json;
      data_json["f_low"] = mb_cross_freq_low;
      data_json["f_mid"] = mb_cross_freq_mid;
      data_json["f_upper_mid"] = mb_cross_freq_upper_mid;
      data_json["f_high"] = mb_cross_freq_high;
      data_json["g_gain"] = mb_global_gain;
      data_json["g_low"] = mb_gain_low;
      data_json["g_mid"] = mb_gain_mid;
      data_json["g_upper_mid"] = mb_gain_upper_mid;
      data_json["g_high"] = mb_gain_high;
      data_json["active"] = multiband_active;
      data_json["t_low"] = mb_threshold_low;
      data_json["t_mid"] = mb_threshold_mid;
      data_json["t_upper_mid"] = mb_threshold_upper_mid;
      data_json["t_low"] = mb_threshold_low;
      data_json["q_low"] = mb_q_low;
      data_json["q_mid"] = mb_q_mid;
      data_json["q_upper_mid"] = mb_q_upper_mid;
      data_json["q_high"] = mb_q_high;
      data_json["s_low"] = mb_solo_low;
      data_json["s_mid"] = mb_solo_mid;
      data_json["s_upper_mid"] = mb_solo_upper_mid;
      data_json["s_high"] = mb_solo_high;
      data_json["g_ratio"] = mb_global_ratio;

#if defined(DEBUG) && defined(DEBUG_SHOW_JSON)
      Serial.println(F("Write JSON data:"));
      serializeJsonPretty(data_json, Serial);
      Serial.println();
#endif
      serializeJsonPretty(data_json, json);
      json.close();
      AudioInterrupts();
      return (true);
    }
    json.close();
  } else {
#ifdef DEBUG
    Serial.print(F("E : Cannot open "));
    Serial.print(filename);
    Serial.println(F(" on SD."));
#endif
  }

  AudioInterrupts();
  return (false);
}
#endif

/******************************************************************************
   SD SEQUENCER
 ******************************************************************************/

FLASHMEM bool load_sd_chain_json(uint8_t number) {
  if (number < 0)
    return (false);

  if (sd_card > 0) {
    number = constrain(number, PERFORMANCE_NUM_MIN, PERFORMANCE_NUM_MAX);
    snprintf_P(filename, sizeof(filename), PSTR("/%s/%d/%s.json"), PERFORMANCE_CONFIG_PATH, number, CHAIN_CONFIG_NAME);
    // first check if file exists...
    AudioNoInterrupts();
    if (SD.exists(filename)) {
      // ... and if: load
      json = SD.open(filename);
      if (json) {
        StaticJsonDocument<JSON_BUFFER_SIZE> data_json;
        deserializeJson(data_json, json);
        json.close();
        AudioInterrupts();
        int total = sizeof(seq.chain);
        int columns = sizeof(seq.chain[0]);
        int rows = total / columns;
        int count = 0;
        for (uint8_t i = 0; i < rows; i++) {
          for (uint8_t j = 0; j < columns; j++) {
            seq.chain[i][j] = data_json["c"][count];
            count++;
          }
        }
        return (true);
      }
#ifdef DEBUG
      else {
        Serial.print(F("E: Cannot open "));
        Serial.print(filename);
        Serial.println(F(" on SD."));
      }
    } else {
      Serial.print(F("No "));
      Serial.print(filename);
      Serial.println(F(" available."));
#endif
    }
  }
  return (false);
}


FLASHMEM bool save_sd_chain_json(uint8_t number) {
  if (sd_card > 0) {
    number = constrain(number, PERFORMANCE_NUM_MIN, PERFORMANCE_NUM_MAX);
    snprintf_P(filename, sizeof(filename), PSTR("/%s/%d/%s.json"), PERFORMANCE_CONFIG_PATH, number, CHAIN_CONFIG_NAME);

    int count = 0;
    int total = sizeof(seq.chain);
    int columns = sizeof(seq.chain[0]);
    int rows = total / columns;
    AudioNoInterrupts();
    SD.remove(filename);
    json = SD.open(filename, FILE_WRITE);
    if (json) {
      StaticJsonDocument<JSON_BUFFER_SIZE> data_json;
      for (uint8_t i = 0; i < rows; i++) {
        for (uint8_t j = 0; j < columns; j++) {
          data_json["c"][count] = seq.chain[i][j];
          count++;
        }
      }
      serializeJsonPretty(data_json, json);
      json.close();
      AudioInterrupts();
      return (true);
    }
    json.close();
  } else {
#ifdef DEBUG
    Serial.print(F("E : Cannot open "));
    Serial.print(filename);
    Serial.println(F(" on SD."));
#endif
  }
  return (false);
}

FLASHMEM bool load_sd_transpose_json(uint8_t number) {
  if (number < 0)
    return (false);

  if (sd_card > 0) {
    number = constrain(number, PERFORMANCE_NUM_MIN, PERFORMANCE_NUM_MAX);
    snprintf_P(filename, sizeof(filename), PSTR("/%s/%d/%s.json"), PERFORMANCE_CONFIG_PATH, number, TRANSPOSE_CONFIG_NAME);
    // first check if file exists...
    AudioNoInterrupts();
    if (SD.exists(filename)) {
      // ... and if: load
      json = SD.open(filename);
      if (json) {
        StaticJsonDocument<JSON_BUFFER_SIZE> data_json;
        deserializeJson(data_json, json);
        json.close();
        AudioInterrupts();
        int total = sizeof(seq.chain_transpose);
        int columns = sizeof(seq.chain_transpose[0]);
        int rows = total / columns;
        int count = 0;
        for (uint8_t i = 0; i < rows; i++) {
          for (uint8_t j = 0; j < columns; j++) {
            seq.chain_transpose[i][j] = data_json["t"][count];
            count++;
          }
        }
        return (true);
      }
#ifdef DEBUG
      else {
        Serial.print(F("E: Cannot open "));
        Serial.print(filename);
        Serial.println(F(" on SD."));
      }
    } else {
      Serial.print(F("No "));
      Serial.print(filename);
      Serial.println(F(" available."));
#endif
    }
  }
  return (false);
}


FLASHMEM bool save_sd_transpose_json(uint8_t number) {
  if (sd_card > 0) {
    number = constrain(number, PERFORMANCE_NUM_MIN, PERFORMANCE_NUM_MAX);
    snprintf_P(filename, sizeof(filename), PSTR("/%s/%d/%s.json"), PERFORMANCE_CONFIG_PATH, number, TRANSPOSE_CONFIG_NAME);

    int count = 0;
    int total = sizeof(seq.chain_transpose);
    int columns = sizeof(seq.chain_transpose[0]);
    int rows = total / columns;
    AudioNoInterrupts();
    SD.remove(filename);
    json = SD.open(filename, FILE_WRITE);
    if (json) {
      StaticJsonDocument<JSON_BUFFER_SIZE> data_json;
      for (uint8_t i = 0; i < rows; i++) {
        for (uint8_t j = 0; j < columns; j++) {
          data_json["t"][count] = seq.chain_transpose[i][j];
          count++;
        }
      }
      serializeJsonPretty(data_json, json);
      json.close();
      AudioInterrupts();
      return (true);
    }
    json.close();
  } else {
#ifdef DEBUG
    Serial.print(F("E : Cannot open "));
    Serial.print(filename);
    Serial.println(F(" on SD."));
#endif
  }
  return (false);
}


FLASHMEM bool load_sd_song_json(uint8_t number) {
  if (number < 0)
    return (false);

  if (sd_card > 0) {
    number = constrain(number, PERFORMANCE_NUM_MIN, PERFORMANCE_NUM_MAX);
    snprintf_P(filename, sizeof(filename), PSTR("/%s/%d/%s.json"), PERFORMANCE_CONFIG_PATH, number, SONG_CONFIG_NAME);
    // first check if file exists...
    AudioNoInterrupts();
    if (SD.exists(filename)) {
      // ... and if: load
      json = SD.open(filename);
      if (json) {
        StaticJsonDocument<JSON_BUFFER_SIZE> data_json;
        deserializeJson(data_json, json);
        json.close();
        AudioInterrupts();
        int total = sizeof(seq.song);
        int columns = sizeof(seq.song[0]);
        int rows = total / columns;
        int count = 0;
        for (uint8_t i = 0; i < rows; i++) {
          for (uint8_t j = 0; j < columns; j++) {
            //  if (i<6)
            seq.song[i][j] = data_json["s"][count];
            //    else
            //    seq.song[i][j] = 99;
            count++;
          }
        }
        return (true);
      }
#ifdef DEBUG
      else {
        Serial.print(F("E: Cannot open "));
        Serial.print(filename);
        Serial.println(F(" on SD."));
      }
    } else {
      Serial.print(F("No "));
      Serial.print(filename);
      Serial.println(F(" available."));
#endif
    }
  }
  return (false);
}


FLASHMEM bool save_sd_song_json(uint8_t number) {
  if (sd_card > 0) {
    number = constrain(number, PERFORMANCE_NUM_MIN, PERFORMANCE_NUM_MAX);
    snprintf_P(filename, sizeof(filename), PSTR("/%s/%d/%s.json"), PERFORMANCE_CONFIG_PATH, number, SONG_CONFIG_NAME);

    int count = 0;
    int total = sizeof(seq.song);
    int columns = sizeof(seq.song[0]);
    int rows = total / columns;
    AudioNoInterrupts();
    SD.remove(filename);
    json = SD.open(filename, FILE_WRITE);
    if (json) {
      StaticJsonDocument<JSON_BUFFER_SIZE> data_json;
      for (uint8_t i = 0; i < rows; i++) {
        for (uint8_t j = 0; j < columns; j++) {
          data_json["s"][count] = seq.song[i][j];
          count++;
        }
      }
      serializeJsonPretty(data_json, json);
      json.close();
      AudioInterrupts();
      return (true);
    }
    json.close();
  } else {
#ifdef DEBUG
    Serial.print(F("E : Cannot open "));
    Serial.print(filename);
    Serial.println(F(" on SD."));
#endif
  }
  return (false);
}

FLASHMEM bool save_sd_seq_sub_vel_json(uint8_t number) {
  if (sd_card > 0) {
    number = constrain(number, PERFORMANCE_NUM_MIN, PERFORMANCE_NUM_MAX);
    snprintf_P(filename, sizeof(filename), PSTR("/%s/%d/%s.json"), PERFORMANCE_CONFIG_PATH, number, VELOCITY_CONFIG_NAME);
#ifdef DEBUG
    Serial.print(F("Saving sequencer velocity "));
    Serial.print(number);
    Serial.print(F(" to "));
    Serial.println(filename);
#endif
    int count = 0;
    int total = sizeof(seq.vel);
    int columns = sizeof(seq.vel[0]);
    int rows = total / columns;
    AudioNoInterrupts();
    SD.remove(filename);
    json = SD.open(filename, FILE_WRITE);
    if (json) {
      StaticJsonDocument<JSON_BUFFER_SIZE> data_json;
      for (uint8_t i = 0; i < rows; i++) {
        for (uint8_t j = 0; j < columns; j++) {
          data_json["seq_velocity"][count] = seq.vel[i][j];
          count++;
        }
      }

#if defined(DEBUG) && defined(DEBUG_SHOW_JSON)
      Serial.println(F("Write JSON data:"));
      serializeJsonPretty(data_json, Serial);
      Serial.println();
#endif
      serializeJsonPretty(data_json, json);
      json.close();
      AudioInterrupts();
      return (true);
    }
    json.close();
  } else {
#ifdef DEBUG
    Serial.print(F("E : Cannot open "));
    Serial.print(filename);
    Serial.println(F(" on SD."));
#endif
  }
  return (false);
}

FLASHMEM bool save_sd_seq_sub_patterns_json(uint8_t number) {
  if (sd_card > 0) {
    number = constrain(number, PERFORMANCE_NUM_MIN, PERFORMANCE_NUM_MAX);
    snprintf_P(filename, sizeof(filename), PSTR("/%s/%d/%s.json"), PERFORMANCE_CONFIG_PATH, number, PATTERN_CONFIG_NAME);
#ifdef DEBUG
    Serial.print(F("Saving sequencer patterns "));
    Serial.print(number);
    Serial.print(F(" to "));
    Serial.println(filename);
#endif
    int count = 0;
    int total = sizeof(seq.note_data);
    int columns = sizeof(seq.note_data[0]);
    int rows = total / columns;
    AudioNoInterrupts();
    SD.remove(filename);
    json = SD.open(filename, FILE_WRITE);
    if (json) {
      StaticJsonDocument<JSON_BUFFER_SIZE> data_json;
      for (uint8_t i = 0; i < rows; i++) {
        for (uint8_t j = 0; j < columns; j++) {
          data_json["seq_data"][count] = seq.note_data[i][j];
          count++;
        }
      }

#if defined(DEBUG) && defined(DEBUG_SHOW_JSON)
      Serial.println(F("Write JSON data:"));
      serializeJsonPretty(data_json, Serial);
      Serial.println();
#endif
      serializeJsonPretty(data_json, json);
      json.close();
      AudioInterrupts();
      return (true);
    }
    json.close();
  } else {
#ifdef DEBUG
    Serial.print(F("E : Cannot open "));
    Serial.print(filename);
    Serial.println(F(" on SD."));
#endif
  }
  return (false);
}

FLASHMEM bool save_sd_performance_json(uint8_t number) {
  bool seq_was_running = false;
  number = constrain(number, PERFORMANCE_NUM_MIN, PERFORMANCE_NUM_MAX);

  if (seq.running == true) {
    seq_was_running = true;
    handleStop();
  }
  dac_mute();

  AudioNoInterrupts();

  check_performance_directory(number);

#ifdef DEBUG
  Serial.print(F("Write performance config "));
  Serial.println(number);
#endif

  save_sd_seq_sub_vel_json(number);
  save_sd_seq_sub_patterns_json(number);
  save_sd_drummappings_json(number);
  save_sd_song_json(number);
  save_sd_transpose_json(number);
  save_sd_chain_json(number);
  save_sd_fx_json(number);
  save_sd_epiano_json(number);
  save_sd_multisample_presets_json(number);
#ifdef USE_BRAIDS
  save_sd_braids_json(number);
#endif
#ifdef USE_MULTIBAND
  save_sd_multiband_json(number);
#endif

  for (uint8_t i = 0; i < MAX_DEXED; i++) {
    snprintf_P(filename, sizeof(filename), PSTR("/%s/%d/%s%d.json"), PERFORMANCE_CONFIG_PATH, number, VOICE_CONFIG_NAME, i);
#ifdef DEBUG
    Serial.print(F("Write Voice-Config for sequencer"));
    Serial.println(filename);
#endif
    save_sd_microsynth_json(number, i);
    save_sd_voiceconfig_json(number, i);
  }
  if (sd_card > 0) {
    snprintf_P(filename, sizeof(filename), PSTR("/%s/%d/%s.json"), PERFORMANCE_CONFIG_PATH, number, SEQUENCER_CONFIG_NAME);
#ifdef DEBUG
    Serial.print(F("Saving sequencer config "));
    Serial.print(number);
    Serial.print(F(" to "));
    Serial.println(filename);
#endif

    Serial.print(F("  "));
    SD.remove(filename);
    json = SD.open(filename, FILE_WRITE);
    if (json) {
      StaticJsonDocument<JSON_BUFFER_SIZE> data_json;
      data_json["seq_tempo_ms"] = seq.tempo_ms;
      data_json["seq_bpm"] = seq.bpm;
      data_json["arp_speed"] = seq.arp_speed;
      data_json["arp_length"] = seq.arp_length;
      data_json["arp_volume_fade"] = seq.arp_volume_fade;
      data_json["arp_style"] = seq.arp_style;
      data_json["seq_chord_vel"] = seq.chord_vel;
      data_json["seq_transpose"] = seq.transpose;
      data_json["chord_key_ammount"] = seq.chord_key_ammount;
      data_json["seq_oct_shift"] = seq.oct_shift;
      data_json["seq_element_shift"] = seq.element_shift;
      data_json["euclidean_active"] = seq.euclidean_active;
      data_json["euclidean_offset"] = seq.euclidean_offset;
      data_json["COLOR_SYSTEXT"] = COLOR_SYSTEXT;
      data_json["COLOR_SYSTEXT_ACCENT"] = COLOR_SYSTEXT_ACCENT;
      data_json["COLOR_BACKGROUND"] = COLOR_BACKGROUND;
      data_json["COLOR_INSTR"] = COLOR_INSTR;
      data_json["COLOR_CHORDS"] = COLOR_CHORDS;
      data_json["COLOR_ARP"] = COLOR_ARP;
      data_json["COLOR_DRUMS"] = COLOR_DRUMS;
      data_json["COLOR_PITCHSMP"] = COLOR_PITCHSMP;
      for (uint8_t i = 0; i < sizeof(seq.track_type); i++) {
        data_json["track_type"][i] = seq.track_type[i];
      }
      for (uint8_t i = 0; i < sizeof(seq.content_type); i++) {
        data_json["content_type"][i] = seq.content_type[i];
      }
      for (uint8_t i = 0; i < sizeof(seq.instrument); i++) {
        data_json["seq_inst_dexed"][i] = seq.instrument[i];
      }
      for (uint8_t i = 0; i < FILENAME_LEN; i++) {
        data_json["seq_name"][i] = seq.name[i];
      }
      for (uint8_t pat = 0; pat < NUM_SEQ_PATTERN; pat++) {
        data_json["chance"][pat] = seq.pat_chance[pat];
        data_json["vel_variation"][pat] = seq.pat_vel_variation[pat];
      }

#if defined(DEBUG) && defined(DEBUG_SHOW_JSON)
      Serial.println(F("Write JSON data:"));
      serializeJsonPretty(data_json, Serial);
      Serial.println();
#endif
      serializeJsonPretty(data_json, json);
      json.close();
      AudioInterrupts();
      dac_unmute();
      if (seq_was_running == true)
        handleStart();
      return (true);
    }
    //json.close();
    //AudioInterrupts();
  }
#ifdef DEBUG
  else {
    Serial.print(F("E : Cannot open "));
    Serial.print(filename);
    Serial.println(F(" on SD."));
    AudioInterrupts();
  }
#endif

  return (false);
}

FLASHMEM bool check_performance_directory(uint8_t number) {
  char dir[CONFIG_FILENAME_LEN];

  if (sd_card > 0) {
    snprintf_P(dir, sizeof(dir), PSTR("/%s/%d"), PERFORMANCE_CONFIG_PATH, number);

    AudioNoInterrupts();
    if (!SD.exists(dir)) {
#ifdef DEBUG
      if (SD.mkdir(dir)) {
        Serial.print(F("Creating directory "));
        Serial.println(dir);
      } else {
        Serial.print(F("E: Cannot create "));
        Serial.println(dir);
        AudioInterrupts();
        return (false);
      }
#else
      SD.mkdir(dir);
#endif
    }
    AudioInterrupts();
    return (true);
  }
#ifdef DEBUG
  else {
    Serial.println(F("E: SD card not available"));
  }
#endif
  return (false);
}

FLASHMEM void get_sd_performance_name_json(uint8_t number) {
  memset(seq.name_temp, 0, FILENAME_LEN);
  if (sd_card > 0) {
    number = constrain(number, PERFORMANCE_NUM_MIN, PERFORMANCE_NUM_MAX);
    snprintf_P(filename, sizeof(filename), PSTR("/%s/%d/%s.json"), PERFORMANCE_CONFIG_PATH, number, SEQUENCER_CONFIG_NAME);

    // first check if file exists...
    //AudioNoInterrupts();
    if (SD.exists(filename)) {
      // ... and if: load

      json = SD.open(filename);
      StaticJsonDocument<JSON_BUFFER_SIZE> data_json;
      if (json) {
        deserializeJson(data_json, json);
        json.close();
        //AudioInterrupts();
      }
      if (data_json["seq_name"][0] != 0) {
        for (uint8_t i = 0; i < FILENAME_LEN; i++) {
          seq.name_temp[i] = data_json["seq_name"][i];
        }
#ifdef DEBUG
        Serial.print(F("Get performance name for "));
        Serial.print(number);
        Serial.print(F(": "));
        Serial.print(seq.name_temp);
        Serial.println();
#endif
      }
#ifdef DEBUG
      else {
        Serial.print(F("Cannot get performance name for "));
        Serial.print(number);
        Serial.println();
      }
#endif
    }
  }
}

FLASHMEM bool load_sd_seq_sub_vel_json(uint8_t number) {
  if (number < 0)
    return (false);

  if (sd_card > 0) {
    number = constrain(number, PERFORMANCE_NUM_MIN, PERFORMANCE_NUM_MAX);
    snprintf_P(filename, sizeof(filename), PSTR("/%s/%d/%s.json"), PERFORMANCE_CONFIG_PATH, number, VELOCITY_CONFIG_NAME);

    // first check if file exists...
    AudioNoInterrupts();
    if (SD.exists(filename)) {
      // ... and if: load
#ifdef DEBUG
      Serial.print(F("Found velocity data ["));
      Serial.print(filename);
      Serial.println(F("]... loading..."));
      Serial.println(F(" "));
#endif
      json = SD.open(filename);
      if (json) {
        StaticJsonDocument<JSON_BUFFER_SIZE> data_json;
        deserializeJson(data_json, json);
        json.close();
        AudioInterrupts();

#if defined(DEBUG) && defined(DEBUG_SHOW_JSON)
        Serial.println(F("Read JSON data:"));
        serializeJsonPretty(data_json, Serial);
        Serial.println();
#endif
        int total = sizeof(seq.vel);
        int columns = sizeof(seq.vel[0]);
        int rows = total / columns;
        int count = 0;
        for (uint8_t i = 0; i < rows; i++) {
          for (uint8_t j = 0; j < columns; j++) {
            seq.vel[i][j] = data_json["seq_velocity"][count];
            count++;
          }
        }
        return (true);
      }
#ifdef DEBUG
      else {
        Serial.print(F("E: Cannot open "));
        Serial.print(filename);
        Serial.println(F(" on SD."));
      }
    } else {
      Serial.print(F("No "));
      Serial.print(filename);
      Serial.println(F(" available."));
#endif
    }
  }
  return (false);
}

FLASHMEM bool load_sd_seq_sub_patterns_json(uint8_t number) {
  if (number < 0)
    return (false);

  if (sd_card > 0) {
    number = constrain(number, PERFORMANCE_NUM_MIN, PERFORMANCE_NUM_MAX);
    snprintf_P(filename, sizeof(filename), PSTR("/%s/%d/%s.json"), PERFORMANCE_CONFIG_PATH, number, PATTERN_CONFIG_NAME);

    // first check if file exists...
    AudioNoInterrupts();
    if (SD.exists(filename)) {
      // ... and if: load
#ifdef DEBUG
      Serial.print(F("Found pattern data ["));
      Serial.print(filename);
      Serial.println(F("]... loading..."));
      Serial.println(F(" "));
#endif
      json = SD.open(filename);
      if (json) {
        StaticJsonDocument<JSON_BUFFER_SIZE> data_json;
        deserializeJson(data_json, json);

        json.close();
        AudioInterrupts();

#if defined(DEBUG) && defined(DEBUG_SHOW_JSON)
        Serial.println(F("Read JSON data:"));
        serializeJsonPretty(data_json, Serial);
        Serial.println();
#endif
        int total = sizeof(seq.note_data);
        int columns = sizeof(seq.note_data[0]);
        int rows = total / columns;
        int count = 0;

        for (uint8_t i = 0; i < rows; i++) {
          for (uint8_t j = 0; j < columns; j++) {
            seq.note_data[i][j] = data_json["seq_data"][count];
            count++;
          }
        }
        return (true);
      }
#ifdef DEBUG
      else {
        Serial.print(F("E : Cannot open "));
        Serial.print(filename);
        Serial.println(F(" on SD."));
      }
    } else {
      Serial.print(F("No "));
      Serial.print(filename);
      Serial.println(F(" available."));
#endif
    }
  }
  return (false);
}

FLASHMEM bool load_sd_performance_json(uint8_t number) {
#if defined(USE_SEQUENCER)
  bool seq_was_running = false;

  if (seq.running) {
    seq_was_running = true;
    handleStop();
  }
#endif

  dac_mute();
  handleStop();
  number = constrain(number, PERFORMANCE_NUM_MIN, PERFORMANCE_NUM_MAX);
  AudioNoInterrupts();
  load_sd_seq_sub_patterns_json(number);
  load_sd_seq_sub_vel_json(number);
  load_sd_fx_json(number);
  load_sd_epiano_json(number);
  load_sd_drummappings_json(number);
  load_sd_song_json(number);
  load_sd_transpose_json(number);
  load_sd_chain_json(number);
  load_sd_multisample_presets_json(number);
#ifdef USE_BRAIDS
  load_sd_braids_json(number);
#endif
#ifdef USE_MULTIBAND
  load_sd_multiband_json(number);
#endif
  configuration.sys.performance_number = number;

  if (sd_card > 0) {
    snprintf_P(filename, sizeof(filename), PSTR("/%s/%d/%s.json"), PERFORMANCE_CONFIG_PATH, number, SEQUENCER_CONFIG_NAME);
    // first check if file exists...
    if (SD.exists(filename)) {
      // ... and if: load
#ifdef DEBUG
      Serial.print(F("Found Performance configuration ["));
      Serial.print(filename);
      Serial.println(F("]... loading..."));
#endif
      json = SD.open(filename);
      if (json) {
        StaticJsonDocument<JSON_BUFFER_SIZE> data_json;
        deserializeJson(data_json, json);
        json.close();
        AudioInterrupts();

#if defined(DEBUG) && defined(DEBUG_SHOW_JSON)
        Serial.println(F("Read JSON data:"));
        serializeJsonPretty(data_json, Serial);
        Serial.println();
#endif

        for (uint8_t i = 0; i < sizeof(seq.track_type); i++) {
          seq.track_type[i] = data_json["track_type"][i];
        }
        for (uint8_t i = 0; i < sizeof(seq.content_type); i++) {
          seq.content_type[i] = data_json["content_type"][i];
        }
        for (uint8_t i = 0; i < sizeof(seq.instrument); i++) {
          seq.instrument[i] = data_json["seq_inst_dexed"][i];
        }

        if (data_json["seq_name"][0] != 0) {
          for (uint8_t i = 0; i < FILENAME_LEN; i++) {
            seq.name[i] = data_json["seq_name"][i];
          }
        }


        for (uint8_t pat = 0; pat < NUM_SEQ_PATTERN; pat++) {
          if (data_json["chance"][pat] > 0) {
            seq.pat_chance[pat] = data_json["chance"][pat];
            seq.pat_vel_variation[pat] = data_json["vel_variation"][pat];
          } else {
            seq.pat_chance[pat] = 100;
            seq.pat_vel_variation[pat] = 0;
          }
        }

        seq.tempo_ms = data_json["seq_tempo_ms"];
        seq.bpm = data_json["seq_bpm"];
        seq.arp_speed = data_json["arp_speed"];
        seq.arp_length = data_json["arp_length"];
        seq.arp_volume_fade = data_json["arp_volume_fade"];
        seq.arp_style = data_json["arp_style"];
        seq.chord_vel = data_json["seq_chord_vel"];
        seq.transpose = data_json["seq_transpose"];
        seq.chord_key_ammount = data_json["chord_key_ammount"];
        seq.oct_shift = data_json["seq_oct_shift"];
        seq.element_shift = data_json["seq_element_shift"];
        seq.euclidean_active = data_json["euclidean_active"];
        seq.euclidean_offset = data_json["euclidean_offset"];
        if (data_json["COLOR_SYSTEXT"] != data_json["COLOR_BACKGROUND"]) {
          COLOR_SYSTEXT = data_json["COLOR_SYSTEXT"];
          COLOR_SYSTEXT_ACCENT = data_json["COLOR_SYSTEXT_ACCENT"];
          COLOR_BACKGROUND = data_json["COLOR_BACKGROUND"];
          COLOR_INSTR = data_json["COLOR_INSTR"];
          COLOR_CHORDS = data_json["COLOR_CHORDS"];
          // COLOR_ARP = data_json["COLOR_ARP"];
          COLOR_DRUMS = data_json["COLOR_DRUMS"];
          COLOR_PITCHSMP = data_json["COLOR_PITCHSMP"];
        }
        AudioNoInterrupts();
        for (uint8_t instance_id = 0; instance_id < NUM_DEXED; instance_id++) {
#ifdef DEBUG
          Serial.print(F("Load Voice-Config "));
          Serial.print(instance_id + 1);
          Serial.print(F(" for sequencer"));
#endif
          load_sd_microsynth_json(number, instance_id);
          load_sd_voiceconfig_json(number, instance_id);
          load_sd_voice(configuration.dexed[instance_id].bank, configuration.dexed[instance_id].voice, instance_id);
          MicroDexed[instance_id]->setGain(midi_volume_transform(map(configuration.dexed[instance_id].sound_intensity, SOUND_INTENSITY_MIN, SOUND_INTENSITY_MAX, 0, 127)));
          MicroDexed[instance_id]->panic();
        }

        AudioInterrupts();
        dac_unmute();

#if defined(USE_SEQUENCER)

        if (seq.euclidean_active)
          update_euclidean();

        for (uint8_t d = 0; d < NUM_SEQ_TRACKS; d++) {
          seq.chain_counter[d] = 0;
        }
        if (seq_was_running) {
          //sequencer_timer.begin(sequencer, seq.tempo_ms / 8);
          //seq.running = true;
          handleStart();
        } else
          sequencer_timer.begin(sequencer, seq.tempo_ms / 8, false);
#endif
        return (true);
      }
#ifdef DEBUG
      else {
        AudioInterrupts();
        Serial.print(F("E : Cannot open "));
        Serial.print(filename);
        Serial.println(F(" on SD."));
      }
    } else {
      AudioInterrupts();
      Serial.print(F("No "));
      Serial.print(filename);
      Serial.println(F(" available."));
#endif
    }
  }
  return (false);
}

FLASHMEM bool check_sd_performance_exists(uint8_t number) {
  if (number < 0)
    return (false);

  number = constrain(number, PERFORMANCE_NUM_MIN, PERFORMANCE_NUM_MAX);
  //AudioNoInterrupts();
  if (sd_card > 0) {
    char filename[CONFIG_FILENAME_LEN];

    snprintf_P(filename, sizeof(filename), PSTR("/%s/%d/%s.json"), PERFORMANCE_CONFIG_PATH, number, SEQUENCER_CONFIG_NAME);

    // check if file exists...
    if (SD.exists(filename)) {
      //AudioInterrupts();
      return (true);
    } else {
      //AudioInterrupts();
      return (false);
    }
  } else {
    //AudioInterrupts();
    return (false);
  }
}

/******************************************************************************
   HELPER FUNCTIONS
 ******************************************************************************/
FLASHMEM bool get_sd_data(File sysex, uint8_t format, uint8_t* conf) {
  uint16_t n;
  int32_t bulk_checksum_calc = 0;
  int8_t bulk_checksum;

#ifdef DEBUG
  Serial.print(F("Reading "));
  Serial.print(sysex.size());
  Serial.println(F(" bytes."));
#endif

  AudioNoInterrupts();
  if (sysex.read() != 0xf0)  // check sysex start-byte
  {
#ifdef DEBUG
    Serial.println(F("E : SysEx start byte not found."));
#endif
    return (false);
  }
  if (sysex.read() != 0x67)  // check sysex vendor is unofficial SYSEX-ID for MicroDexed
  {
#ifdef DEBUG
    Serial.println(F("E : SysEx vendor not unofficial SYSEX-ID for MicroDexed."));
#endif
    return (false);
  }
  if (sysex.read() != format)  // check for sysex type
  {
#ifdef DEBUG
    Serial.println(F("E : SysEx type not found."));
#endif
    return (false);
  }
  sysex.seek(sysex.size() - 1);
  if (sysex.read() != 0xf7)  // check sysex end-byte
  {
#ifdef DEBUG
    Serial.println(F("E : SysEx end byte not found."));
#endif
    return (false);
  }

  sysex.seek(sysex.size() - 2);  // Bulk checksum
  bulk_checksum = sysex.read();

  sysex.seek(3);  // start of bulk data
  for (n = 0; n < sysex.size() - 6; n++) {
    uint8_t d = sysex.read();
    bulk_checksum_calc -= d;
#ifdef DEBUG
    Serial.print(F("SYSEX data read: 0x"));
    Serial.println(d, HEX);
#endif
  }
  bulk_checksum_calc &= 0x7f;

  if (int8_t(bulk_checksum_calc) != bulk_checksum) {
#ifdef DEBUG
    Serial.print(F("E : Bulk checksum mismatch : 0x"));
    Serial.print(int8_t(bulk_checksum_calc), HEX);
    Serial.print(F(" != 0x"));
    Serial.println(bulk_checksum, HEX);
#endif
    return (false);
  }
#ifdef DEBUG
  else {
    Serial.print(F("Bulk checksum : 0x"));
    Serial.print(int8_t(bulk_checksum_calc), HEX);
    Serial.print(F(" [0x"));
    Serial.print(bulk_checksum, HEX);
    Serial.println(F("]"));
  }
#endif

  sysex.seek(3);  // start of bulk data
  for (n = 0; n < sysex.size() - 6; n++) {
    uint8_t d = sysex.read();
    *(conf++) = d;
  }
  AudioInterrupts();

#ifdef DEBUG
  Serial.println(F("SD data loaded."));
#endif

  return (true);
}

FLASHMEM bool write_sd_data(File sysex, uint8_t format, uint8_t* data, uint16_t len) {
#ifdef DEBUG
  Serial.print(F("Storing SYSEX format 0x"));
  Serial.print(format, HEX);
  Serial.print(F(" with length of "));
  Serial.print(len, DEC);
  Serial.println(F(" bytes."));
#endif

  // write sysex start
  AudioNoInterrupts();
  sysex.write(0xf0);
#ifdef DEBUG
  Serial.println(F("Write SYSEX start:    0xf0"));
#endif
  // write sysex vendor is unofficial SYSEX-ID for MicroDexed
  sysex.write(0x67);
#ifdef DEBUG
  Serial.println(F("Write SYSEX vendor:   0x67"));
#endif
  // write sysex format number
  sysex.write(format);
#ifdef DEBUG
  Serial.print(F("Write SYSEX format:   0x"));
  Serial.println(format, HEX);
#endif
  // write data
  sysex.write(data, len);
#ifdef DEBUG
  for (uint16_t i = 0; i < len; i++) {
    Serial.print(F("Write SYSEX data:     0x"));
    Serial.println(data[i], HEX);
  }
#endif
  // write checksum
  sysex.write(calc_checksum(data, len));
#ifdef DEBUG
  uint8_t checksum = calc_checksum(data, len);
  sysex.write(checksum);
  Serial.print(F("Write SYSEX checksum: 0x"));
  Serial.println(checksum, HEX);
#endif
  // write sysex end
  sysex.write(0xf7);
  AudioInterrupts();

#ifdef DEBUG
  Serial.println(F("Write SYSEX end:      0xf7"));
#endif

  return (true);
}

FLASHMEM bool get_bank_name(uint8_t b, char* bank_name) {
#ifdef DEBUG
  Serial.printf_P(PSTR("get bank name for bank [%d]\n"), b);
#endif
  b = constrain(b, 0, MAX_BANKS - 1);

  if (sd_card > 0) {
    File sysex_dir;
    char bankdir[4];

    snprintf_P(bankdir, sizeof(bankdir), PSTR("/%d"), b);

    AudioNoInterrupts();
    sysex_dir = SD.open(bankdir);
    AudioInterrupts();
    if (!sysex_dir) {
      strcpy(bank_name, sError);

#ifdef DEBUG
      Serial.print(F("E : Cannot open "));
      Serial.print(bankdir);
      Serial.println(F(" on SD."));
#endif
      return false;
    }

    File entry;
    do {
      entry = sysex_dir.openNextFile();
    } while (entry.isDirectory());

    if (entry.isDirectory()) {
      strcpy(bank_name, sError);
      AudioNoInterrupts();
      entry.close();
      sysex_dir.close();
      AudioInterrupts();
      return false;
    }

    strip_extension(entry.name(), bank_name, BANK_NAME_LEN);
    string_toupper(bank_name);

    AudioNoInterrupts();
    entry.close();
    sysex_dir.close();
    AudioInterrupts();

    return true;
  }

  strcpy(bank_name, sError);
  return false;
}

FLASHMEM bool get_voice_name(uint8_t b, uint8_t v, char* voice_name) {
#ifdef DEBUG
  Serial.printf_P(PSTR("get voice name for voice [%d]\n"), v + 1);
#endif
  b = constrain(b, 0, MAX_BANKS - 1);

  if (sd_card > 0) {
    File sysex_dir;
    char bankdir[4];

    snprintf_P(bankdir, sizeof(bankdir), PSTR("/%d"), b);

    AudioNoInterrupts();
    sysex_dir = SD.open(bankdir);
    AudioInterrupts();
    if (!sysex_dir) {
      strcpy(voice_name, sError);

#ifdef DEBUG
      Serial.print(F("E : Cannot open "));
      Serial.print(bankdir);
      Serial.println(F(" on SD."));
#endif
      return false;
    }

    File entry;
    do {
      entry = sysex_dir.openNextFile();
    } while (entry.isDirectory());

    if (entry.isDirectory()) {
      strcpy(voice_name, sError);
      AudioNoInterrupts();
      entry.close();
      sysex_dir.close();
      AudioInterrupts();
      return false;
    }

    // load name of voices of the bank
#ifdef DEBUG
    char bank_name[BANK_NAME_LEN];
    strip_extension(entry.name(), bank_name, BANK_NAME_LEN);
    string_toupper(bank_name);
    Serial.printf_P(PSTR("Get voice name from [/%d/%s.syx]\n"), b, bank_name);
#endif
    memset(voice_name, 0, VOICE_NAME_LEN);
    entry.seek(124 + (v * 128));
    entry.read(voice_name, min(VOICE_NAME_LEN, 10));
    string_toupper(voice_name);

#ifdef DEBUG
    Serial.printf_P(PSTR("Found voice-name [%s] for bank [%d] and voice [%d]\n"), voice_name, b, v + 1);
#endif

    AudioNoInterrupts();
    entry.close();
    sysex_dir.close();
    AudioInterrupts();

    return true;
  }

  strcpy(voice_name, sError);
  return false;
}

uint8_t calc_checksum(uint8_t* data, uint16_t len) {
  int32_t bulk_checksum_calc = 0;

  for (uint16_t n = 0; n < len; n++)
    bulk_checksum_calc -= data[n];

  return (bulk_checksum_calc & 0x7f);
}

FLASHMEM void strip_extension(const char* s, char* target, uint8_t len) {
  char tmp[CONFIG_FILENAME_LEN];
  char* token;

  strcpy(tmp, s);
  token = strtok(tmp, ".");
  if (token == NULL)
    strcpy(target, sError);
  else
    strcpy(target, token);

  target[len] = '\0';
}

FLASHMEM void string_toupper(char* s) {
  while (*s) {
    *s = toupper((unsigned char)*s);
    s++;
  }
}

FLASHMEM bool save_sd_multisample_presets_json(uint8_t number) {
  if (number < 0)
    return (false);

  if (sd_card > 0) {
    number = constrain(number, PERFORMANCE_NUM_MIN, PERFORMANCE_NUM_MAX);
    snprintf_P(filename, sizeof(filename), PSTR("/%s/%d/%s.json"), PERFORMANCE_CONFIG_PATH, number, MULTISAMPLE_PRESETS_CONFIG_NAME);
#ifdef DEBUG
    Serial.print(F("Saving multisample slot "));
    Serial.print(number);
    Serial.print(F(" to "));
    Serial.println(filename);
#endif
    AudioNoInterrupts();
    if (SD.exists(filename)) {
#ifdef DEBUG
      Serial.println(F("remove old multisample file"));
#endif
      SD.remove(filename);
    }
    json = SD.open(filename, FILE_WRITE);
    if (json) {
      char zone_filename[CONFIG_FILENAME_LEN];
      StaticJsonDocument<JSON_BUFFER_SIZE> data_json;

      for (uint8_t i = 0; i < NUM_MULTISAMPLES; i++) {
        data_json[i]["name"] = msp[i].name;
        data_json[i]["sound_intensity"] = msp[i].sound_intensity;
        data_json[i]["midi_channel"] = msp[i].midi_channel;

        for (uint8_t j = 0; j < NUM_MULTISAMPLE_ZONES; j++) {
          strcpy(zone_filename, msz[i][j].filename);
          if (strchr(zone_filename, '.')) {
            *(strchr(zone_filename, '.')) = '\0';
          }
          data_json[i]["zones"]["filename"][j] = zone_filename;
          data_json[i]["zones"]["root"][j] = msz[i][j].rootnote;
          data_json[i]["zones"]["low"][j] = msz[i][j].low;
          data_json[i]["zones"]["high"][j] = msz[i][j].high;
          data_json[i]["zones"]["playmode"][j] = msz[i][j].playmode;
          data_json[i]["zones"]["vol"][j] = msz[i][j].vol;
          data_json[i]["zones"]["pan"][j] = msz[i][j].pan;
          data_json[i]["zones"]["rev"][j] = msz[i][j].rev;
        }
      }

#if defined(DEBUG) && defined(DEBUG_SHOW_JSON)
      Serial.println(F("Write JSON data:"));
      serializeJsonPretty(data_json, Serial);
      Serial.println();
#endif
      serializeJsonPretty(data_json, json);
      json.close();
      AudioInterrupts();
      return (true);
    } else {
#ifdef DEBUG
      Serial.print(F("E : Cannot open "));
      Serial.print(filename);
      Serial.println(F(" on SD."));
#endif
    }
    AudioInterrupts();
    json.close();
  }
  return (false);
}

FLASHMEM bool load_sd_multisample_presets_json(uint8_t number) {
  if (number < 0)
    return (false);

  if (sd_card > 0) {
    number = constrain(number, PERFORMANCE_NUM_MIN, PERFORMANCE_NUM_MAX);
    snprintf_P(filename, sizeof(filename), PSTR("/%s/%d/%s.json"), PERFORMANCE_CONFIG_PATH, number, MULTISAMPLE_PRESETS_CONFIG_NAME);

    // first check if file exists...
    AudioNoInterrupts();
    if (SD.exists(filename)) {
#ifdef DEBUG
      Serial.print(F("Found msp presets data ["));
      Serial.print(filename);
      Serial.println(F("]... loading..."));
      Serial.println(F(" "));
#endif

      json = SD.open(filename);
      if (json) {
        // StaticJsonDocument<JSON_BUFFER_SIZE> data_json;
        DynamicJsonDocument data_json(JSON_BUFFER_SIZE);
        deserializeJson(data_json, json);
        json.close();
        AudioInterrupts();

#if defined(DEBUG) && defined(DEBUG_SHOW_JSON)
        Serial.println(F("Read JSON data:"));
        serializeJsonPretty(data_json, Serial);
        Serial.println();
#endif
        for (uint8_t i = 0; i < NUM_MULTISAMPLES; i++) {
          strcpy(msp[i].name, data_json[i]["name"]);
          msp[i].sound_intensity = data_json[i]["sound_intensity"];
          msp[i].midi_channel = data_json[i]["midi_channel"];

          for (uint8_t j = 0; j < NUM_MULTISAMPLE_ZONES; j++) {
            strcpy(msz[i][j].filename, data_json[i]["zones"]["filename"][j]);
            if (strlen(msz[i][j].filename) > 0) strcat(msz[i][j].filename, ".wav");
            msz[i][j].rootnote = data_json[i]["zones"]["root"][j];
            msz[i][j].low = data_json[i]["zones"]["low"][j];
            msz[i][j].high = data_json[i]["zones"]["high"][j];
            msz[i][j].playmode = data_json[i]["zones"]["playmode"][j];
            msz[i][j].vol = data_json[i]["zones"]["vol"][j];
            msz[i][j].pan = data_json[i]["zones"]["pan"][j];
            msz[i][j].rev = data_json[i]["zones"]["rev"][j];
          }
        }
        return (true);
      }
#ifdef DEBUG
      else {
        Serial.print(F("E: Cannot open "));
        Serial.print(filename);
        Serial.println(F(" on SD."));
      }
    } else {
      Serial.print(F("No "));
      Serial.print(filename);
      Serial.println(F(" available."));
#endif
    }
  }
  return (false);
}