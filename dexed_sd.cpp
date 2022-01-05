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
#include "ILI9486_Teensy.h"
#include "dexed_sd.h"
#include "synth_dexed.h"
#if NUM_DRUMS > 0
#include "sequencer.h"
#include "drums.h"
extern void set_drums_volume(float vol);
extern drum_config_t drum_config[];
extern custom_midi_map_t custom_midi_map[NUM_CUSTOM_MIDI_MAPPINGS];
#endif

extern void init_MIDI_send_CC(void);
extern void check_configuration_dexed(uint8_t instance_id);
extern void check_configuration_performance(void);
extern void check_configuration_fx(void);
extern void check_configuration_epiano(void);
extern void sequencer();
extern sequencer_t seq;
#ifdef USE_SEQUENCER
extern PeriodicTimer sequencer_timer;
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
extern bool save_sys_flag;

/******************************************************************************
   SD BANK/VOICE LOADING
 ******************************************************************************/
bool load_sd_voice(uint8_t b, uint8_t v, uint8_t instance_id)
{
  v = constrain(v, 0, MAX_VOICES - 1);
  b = constrain(b, 0, MAX_BANKS - 1);

  if (sd_card > 0)
  {
    File sysex;
    char filename[FILENAME_LEN];
    char bank_name[BANK_NAME_LEN];
    uint8_t data[128];

    get_bank_name(b, bank_name, sizeof(bank_name));
    sprintf(filename, "/%d/%s.syx", b, bank_name);

    AudioNoInterrupts();
    sysex = SD.open(filename);
    AudioInterrupts();
    if (!sysex)
    {
#ifdef DEBUG
      Serial.print(F("E : Cannot open "));
      Serial.print(filename);
      Serial.println(F(" on SD."));
#endif
      return (false);
    }

    if (get_sd_voice(sysex, v, data))
    {
#ifdef DEBUG
      char voice_name[VOICE_NAME_LEN];
      get_voice_name(b, v, voice_name, sizeof(voice_name));
      Serial.print(F("Loading voice from "));
      Serial.print(filename);
      Serial.print(F(" ["));
      Serial.print(voice_name);
      Serial.println(F("]"));
#endif
      uint8_t tmp_data[156];
      bool ret = MicroDexed[instance_id]->decodeVoice(tmp_data, data);
      MicroDexed[instance_id]->loadVoiceParameters(tmp_data);
#ifdef DEBUG
      show_patch(instance_id);
#endif

      AudioNoInterrupts();
      sysex.close();
      AudioInterrupts();
      configuration.dexed[instance_id].transpose = MicroDexed[instance_id]->getTranspose();
      configuration.dexed[instance_id].bank = b;
      configuration.dexed[instance_id].voice = v;

      uint8_t data_copy[155];
      MicroDexed[instance_id]->getVoiceData(data_copy);
      send_sysex_voice(configuration.dexed[instance_id].midi_channel, data_copy);
      init_MIDI_send_CC();
      return (ret);
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

bool save_sd_voice(uint8_t b, uint8_t v, uint8_t instance_id)
{
  v = constrain(v, 0, MAX_VOICES - 1);
  b = constrain(b, 0, MAX_BANKS - 1);

  if (sd_card > 0)
  {
    File sysex;
    char filename[FILENAME_LEN];
    char bank_name[BANK_NAME_LEN];
    uint8_t data[128];

    get_bank_name(b, bank_name, sizeof(bank_name));
    sprintf(filename, "/%d/%s.syx", b, bank_name);

    AudioNoInterrupts();
    sysex = SD.open(filename, FILE_WRITE);
    AudioInterrupts();
    if (!sysex)
    {
#ifdef DEBUG
      Serial.print(F("E : Cannot open "));
      Serial.print(filename);
      Serial.println(F(" on SD."));
#endif
      return (false);
    }

    MicroDexed[instance_id]->encodeVoice(data);

    if (put_sd_voice(sysex, v, data))
    {
#ifdef DEBUG
      char voice_name[VOICE_NAME_LEN];

      MicroDexed[instance_id]->getName(voice_name);
      Serial.print(F("Saving voice to "));
      Serial.print(filename);
      Serial.print(F(" ["));
      Serial.print(voice_name);
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

bool get_sd_voice(File sysex, uint8_t voice_number, uint8_t* data)
{
  uint16_t n;
  int32_t bulk_checksum_calc = 0;
  int8_t bulk_checksum;

  AudioNoInterrupts();
  if (sysex.size() != 4104) // check sysex size
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
  sysex.seek(4102); // Bulk checksum
  bulk_checksum = sysex.read();

  sysex.seek(6); // start of bulk data
  for (n = 0; n < 4096; n++)
  {
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

  if (bulk_checksum_calc != bulk_checksum)
  {
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

bool put_sd_voice(File sysex, uint8_t voice_number, uint8_t* data)
{
  uint16_t n;
  int32_t bulk_checksum_calc = 0;

  AudioNoInterrupts();
  sysex.seek(0);

  if (sysex.size() != 4104) // check sysex size
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
  sysex.seek(6); // start of bulk data
  for (n = 0; n < 4096; n++)
  {
    uint8_t d = sysex.read();
    bulk_checksum_calc -= d;
  }
  sysex.seek(4102); // Bulk checksum
  sysex.write(bulk_checksum_calc & 0x7f);
  AudioInterrupts();

#ifdef DEBUG
  Serial.print(F("Bulk checksum : 0x"));
  Serial.println(bulk_checksum_calc & 0x7f, HEX);
#endif

  return (true);
}

bool save_sd_bank(const char* bank_filename, uint8_t* data)
{
  char tmp[FILENAME_LEN];
  char tmp2[FILENAME_LEN];
  int bank_number;
  File root, entry;

  if (sd_card > 0)
  {
#ifdef DEBUG
    Serial.print(F("Trying so store "));
    Serial.print(bank_filename);
    Serial.println(F("."));
#endif

    // first remove old bank
    sscanf(bank_filename, "/%d/%s", &bank_number, tmp);
    sprintf(tmp, "/%d", bank_number);
    AudioNoInterrupts();
    root = SD.open(tmp);
    while (42 == 42)
    {
      entry = root.openNextFile();
      if (entry)
      {
        if (!entry.isDirectory())
        {
#ifdef DEBUG
          Serial.print(F("Removing "));
          Serial.print(tmp);
          Serial.print(F("/"));
          Serial.println(entry.name());
#endif
          sprintf(tmp2, "%s/%s", tmp, entry.name());
          entry.close();
#ifndef DEBUG
          SD.remove(tmp2);
#else
          bool r = SD.remove(tmp2);
          if (r == false)
          {
            Serial.print(F("E: cannot remove "));
            Serial.print(tmp2);
            Serial.println(F("."));
          }
#endif
          break;
        }
      }
      else
      {
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
  }
  else
    return (false);

  return (true);
}

/******************************************************************************
   SD DRUM CUSTOM MAPPINGS
 ******************************************************************************/

bool load_sd_drummappings_json(uint8_t number)
{
  if (number < 0)
    return (false);

  number = constrain(number, PERFORMANCE_NUM_MIN, PERFORMANCE_NUM_MAX);

  if (sd_card > 0)
  {
    File json;
    StaticJsonDocument<JSON_BUFFER_SIZE> data_json;
    char filename[CONFIG_FILENAME_LEN];

    sprintf(filename, "/%s/%d/%s.json", PERFORMANCE_CONFIG_PATH, number, DRUMS_MAPPING_NAME);

    // first check if file exists...
    AudioNoInterrupts();
    if (SD.exists(filename))
    {
      // ... and if: load
#ifdef DEBUG
      Serial.print(F("Found drum mapping ["));
      Serial.print(filename);
      Serial.println(F("]... loading..."));
#endif
      json = SD.open(filename);
      if (json)
      {
        deserializeJson(data_json, json);
        json.close();
        AudioInterrupts();
#ifdef DEBUG
        Serial.println(F("Read JSON data:"));
        serializeJsonPretty(data_json, Serial);
        Serial.println();
#endif

        for (uint8_t i = 0; i < NUM_CUSTOM_MIDI_MAPPINGS - 1; i++)
        {
          custom_midi_map[i].type = data_json["type"][i];
          custom_midi_map[i].in = data_json["in"][i];
          custom_midi_map[i].out = data_json["out"][i];
          custom_midi_map[i].channel = data_json["channel"][i];
        }
        return (true);
      }
#ifdef DEBUG
      else
      {
        Serial.print(F("E : Cannot open "));
        Serial.print(filename);
        Serial.println(F(" on SD."));
      }
    }
    else
    {
      Serial.print(F("No "));
      Serial.print(filename);
      Serial.println(F(" available."));
#endif
    }
  }
  return (false);
}

bool save_sd_drummappings_json(uint8_t number)
{
  char filename[CONFIG_FILENAME_LEN];
  number = constrain(number, 0, 99);

  if (sd_card > 0)
  {
    File json;
    StaticJsonDocument<JSON_BUFFER_SIZE> data_json;

    if (check_performance_directory(number))
    {
      sprintf(filename, "/%s/%d/%s.json", PERFORMANCE_CONFIG_PATH, number, DRUMS_MAPPING_NAME);

#ifdef DEBUG
      Serial.print(F("Saving drum mapping "));
      Serial.print(number);
      Serial.print(F(" to "));
      Serial.println(filename);
#endif
      AudioNoInterrupts();
      if (SD.exists(filename)) {
        Serial.println("remove old drum mapping file");
        SD.begin();
        SD.remove(filename);
      }
      json = SD.open(filename, FILE_WRITE);
      if (json)
      {
        for (uint8_t i = 0; i < NUM_CUSTOM_MIDI_MAPPINGS - 1; i++)
        {
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
      }
      else
      {
#ifdef DEBUG
        Serial.print(F("E : Cannot open "));
        Serial.print(filename);
        Serial.println(F(" on SD."));
#endif
        AudioInterrupts();
        return (false);
      }
    }
    else
    {
      AudioInterrupts();
      return (false);
    }
  }
#ifdef DEBUG
  else
  {
    Serial.println(F("E: SD card not available"));
  }
#endif
  return (false);
}


/******************************************************************************
   SD DRUMSETTINGS
 ******************************************************************************/
bool load_sd_drumsettings_json(uint8_t number)
{
  if (number < 0)
    return (false);

  number = constrain(number, PERFORMANCE_NUM_MIN, PERFORMANCE_NUM_MAX);

  if (sd_card > 0)
  {
    File json;
    StaticJsonDocument<JSON_BUFFER_SIZE> data_json;
    char filename[CONFIG_FILENAME_LEN];

    sprintf(filename, "/%s/%d/%s.json", PERFORMANCE_CONFIG_PATH, number, DRUMS_CONFIG_NAME);

    // first check if file exists...
    AudioNoInterrupts();
    if (SD.exists(filename))
    {
      // ... and if: load
#ifdef DEBUG
      Serial.print(F("Found drums configuration ["));
      Serial.print(filename);
      Serial.println(F("]... loading..."));
#endif
      json = SD.open(filename);
      if (json)
      {
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
        for (uint8_t i = 0; i < NUM_DRUMSET_CONFIG - 1; i++)
        {
          uint8_t drumnumber = 0;
          drumnumber = find_drum_number_from_note( data_json["note"][i] );
          if (((int)data_json["note"][i] > 0 && find_drum_number_from_note(data_json["note"][i]) > 0) || (i == 0 && (int)data_json["note"][i] == 210))
          {
            set_sample_pitch( drumnumber, data_json["pitch"][i] );
            set_sample_p_offset ( drumnumber, data_json["p_offset"][i] );
            set_sample_pan( drumnumber, data_json["pan"][i]) ;
            set_sample_vol_max( drumnumber, data_json["vol_max"][i]) ;
            set_sample_vol_min( drumnumber, data_json["vol_min"][i] );
            set_sample_reverb_send( drumnumber, data_json["reverb_send"][i]);
          }
        }
        return (true);
      }
#ifdef DEBUG
      else
      {
        Serial.print(F("E : Cannot open "));
        Serial.print(filename);
        Serial.println(F(" on SD."));
      }
    }
    else
    {
      Serial.print(F("No "));
      Serial.print(filename);
      Serial.println(F(" available."));
#endif
    }
  }
  return (false);
}

bool save_sd_drumsettings_json(uint8_t number)
{
  char filename[CONFIG_FILENAME_LEN];
  number = constrain(number, PERFORMANCE_NUM_MIN, PERFORMANCE_NUM_MAX);

  if (sd_card > 0)
  {
    File json;
    StaticJsonDocument<JSON_BUFFER_SIZE> data_json;

    if (check_performance_directory(number))
    {
      sprintf(filename, "/%s/%d/%s.json", PERFORMANCE_CONFIG_PATH, number, DRUMS_CONFIG_NAME);

#ifdef DEBUG
      Serial.print(F("Saving drums config "));
      Serial.print(number);
      Serial.print(F(" to "));
      Serial.println(filename);
#endif
      AudioNoInterrupts();
      if (SD.exists(filename)) {
#ifdef DEBUG
        Serial.println("remove old drumsettings file");
#endif
        SD.begin();
        SD.remove(filename);
      }
      json = SD.open(filename, FILE_WRITE);
      if (json)
      {
        data_json["drums_volume"] = seq.drums_volume;
        for (uint8_t i = 0; i < NUM_DRUMSET_CONFIG - 1; i++)
        {
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
      }
      else
      {
#ifdef DEBUG
        Serial.print(F("E : Cannot open "));
        Serial.print(filename);
        Serial.println(F(" on SD."));
#endif
        AudioInterrupts();
        return (false);
      }
    }
    else
    {
      AudioInterrupts();
      return (false);
    }
  }
#ifdef DEBUG
  else
  {
    Serial.println(F("E: SD card not available"));
  }
#endif
  return (false);
}

/******************************************************************************
   SD VOICECONFIG
 ******************************************************************************/
bool load_sd_voiceconfig_json(uint8_t vc, uint8_t instance_id)
{
  char filename[CONFIG_FILENAME_LEN];

  vc = constrain(vc, PERFORMANCE_NUM_MIN, PERFORMANCE_NUM_MAX);

  if (sd_card > 0)
  {
    File json;
    StaticJsonDocument<JSON_BUFFER_SIZE> data_json;

    sprintf(filename, "/%s/%d/%s%d.json", PERFORMANCE_CONFIG_PATH, vc, VOICE_CONFIG_NAME, instance_id + 1);

    // first check if file exists...
    AudioNoInterrupts();
    if (SD.exists(filename))
    {
      // ... and if: load
#ifdef DEBUG
      Serial.print(F("Found voice configuration ["));
      Serial.print(filename);
      Serial.println(F("]... loading..."));
#endif
      json = SD.open(filename);
      if (json)
      {
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
      else
      {
        Serial.print(F("E : Cannot open "));
        Serial.print(filename);
        Serial.println(F(" on SD."));
      }
    }
    else
    {
      Serial.print(F("No "));
      Serial.print(filename);
      Serial.println(F(" available."));
#endif
    }
  }

  AudioInterrupts();
  return (false);
}

bool save_sd_voiceconfig_json(uint8_t vc, uint8_t instance_id)
{
  char filename[CONFIG_FILENAME_LEN];

  vc = constrain(vc, PERFORMANCE_NUM_MIN, PERFORMANCE_NUM_MAX);

  if (sd_card > 0)
  {
    File json;
    StaticJsonDocument<JSON_BUFFER_SIZE> data_json;
    sprintf(filename, "/%s/%d/%s%d.json", PERFORMANCE_CONFIG_PATH, vc, VOICE_CONFIG_NAME, instance_id + 1);

#ifdef DEBUG
    Serial.print(F("Saving voice config "));
    Serial.print(vc);
    Serial.print(F("["));
    Serial.print(instance_id);
    Serial.print(F("]"));
    Serial.print(F(" to "));
    Serial.println(filename);
#endif

    AudioNoInterrupts();
    SD.begin();
    SD.remove(filename);
    json = SD.open(filename, FILE_WRITE);
    if (json)
    {
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
  }
  else
  {
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
   SD FX
 ******************************************************************************/
bool load_sd_fx_json(uint8_t number)
{
  number = constrain(number, PERFORMANCE_NUM_MIN, PERFORMANCE_NUM_MAX);

  load_sd_drumsettings_json(number);

  if (sd_card > 0)
  {
    File json;
    StaticJsonDocument<JSON_BUFFER_SIZE> data_json;
    char filename[CONFIG_FILENAME_LEN];
    sprintf(filename, "/%s/%d/%s.json", PERFORMANCE_CONFIG_PATH, number, FX_CONFIG_NAME);

    // first check if file exists...
    AudioNoInterrupts();
    if (SD.exists(filename))
    {
      // ... and if: load
#ifdef DEBUG
      Serial.print(F("Found fx configuration ["));
      Serial.print(filename);
      Serial.println(F("]... loading..."));
#endif
      json = SD.open(filename);
      if (json)
      {
        deserializeJson(data_json, json);

        json.close();
        AudioInterrupts();

#if defined(DEBUG) && defined(DEBUG_SHOW_JSON)
        Serial.println(F("Read JSON data:"));
        serializeJsonPretty(data_json, Serial);
        Serial.println();
#endif
        for (uint8_t i = 0; i < MAX_DEXED; i++)
        {
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
          if (configuration.fx.delay_sync[i] > 0)
          {
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
      else
      {
        Serial.print(F("E : Cannot open "));
        Serial.print(filename);
        Serial.println(F(" on SD."));
      }
    }
    else
    {
      Serial.print(F("No "));
      Serial.print(filename);
      Serial.println(F(" available."));
#endif
    }
  }

  AudioInterrupts();
  return (false);
}

bool save_sd_fx_json(uint8_t number)
{
  char filename[CONFIG_FILENAME_LEN];

  number = constrain(number, PERFORMANCE_NUM_MIN, PERFORMANCE_NUM_MAX);

  save_sd_drumsettings_json(number);
  if (sd_card > 0)
  {
    File json;
    StaticJsonDocument<JSON_BUFFER_SIZE> data_json;
    sprintf(filename, "/%s/%d/%s.json", PERFORMANCE_CONFIG_PATH, number, FX_CONFIG_NAME);

#ifdef DEBUG
    Serial.print(F("Saving fx config "));
    Serial.print(number);
    Serial.print(F(" to "));
    Serial.println(filename);
#endif

    AudioNoInterrupts();
    SD.begin();
    SD.remove(filename);
    json = SD.open(filename, FILE_WRITE);
    if (json)
    {
      for (uint8_t i = 0; i < MAX_DEXED; i++)
      {
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
  }
  else
  {
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
   SD EPIANO
 ******************************************************************************/
bool load_sd_epiano_json(uint8_t number)
{
  number = constrain(number, PERFORMANCE_NUM_MIN, PERFORMANCE_NUM_MAX);

  if (sd_card > 0)
  {
    File json;
    StaticJsonDocument<JSON_BUFFER_SIZE> data_json;
    char filename[CONFIG_FILENAME_LEN];
    sprintf(filename, "/%s/%d/%s.json", PERFORMANCE_CONFIG_PATH, number, EPIANO_CONFIG_NAME);

    // first check if file exists...
    AudioNoInterrupts();
    if (SD.exists(filename))
    {
      // ... and if: load
#ifdef DEBUG
      Serial.print(F("Found epiano configuration ["));
      Serial.print(filename);
      Serial.println(F("]... loading..."));
#endif
      json = SD.open(filename);
      if (json)
      {
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
      else
      {
        Serial.print(F("E : Cannot open "));
        Serial.print(filename);
        Serial.println(F(" on SD."));
      }
    }
    else
    {
      Serial.print(F("No "));
      Serial.print(filename);
      Serial.println(F(" available."));
#endif
    }
  }

  AudioInterrupts();
  return (false);
}

bool save_sd_epiano_json(uint8_t number)
{
  char filename[CONFIG_FILENAME_LEN];

  number = constrain(number, PERFORMANCE_NUM_MIN, PERFORMANCE_NUM_MAX);

  if (sd_card > 0)
  {
    File json;
    StaticJsonDocument<JSON_BUFFER_SIZE> data_json;
    sprintf(filename, "/%s/%d/%s.json", PERFORMANCE_CONFIG_PATH, number, EPIANO_CONFIG_NAME);

#ifdef DEBUG
    Serial.print(F("Saving epiano config "));
    Serial.print(number);
    Serial.print(F(" to "));
    Serial.println(filename);
#endif

    AudioNoInterrupts();
    SD.begin();
    SD.remove(filename);
    json = SD.open(filename, FILE_WRITE);
    if (json)
    {
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
  }
  else
  {
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
bool load_sd_sys_json(void)
{
  if (sd_card > 0)
  {
    File json;
    StaticJsonDocument<JSON_BUFFER_SIZE> data_json;
    char filename[CONFIG_FILENAME_LEN];
    sprintf(filename, "/%s.json", SYS_CONFIG_NAME);

    // first check if file exists...
    AudioNoInterrupts();
    if (SD.exists(filename))
    {
      // ... and if: load
#ifdef DEBUG
      Serial.print(F("Found sys configuration"));
#endif
      json = SD.open(filename);
      if (json)
      {
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
        configuration.sys.load_at_startup = data_json["load_at_startup"];

        check_configuration_sys();
        set_sys_params();

        return (true);
      }
#ifdef DEBUG
      else
      {
        Serial.print(F("E : Cannot open "));
        Serial.print(filename);
        Serial.println(F(" on SD."));
      }
    }
    else
    {
      Serial.print(F("No "));
      Serial.print(filename);
      Serial.println(F(" available."));
#endif
    }
  }

  AudioInterrupts();
  return (false);
}

bool save_sd_sys_json(void)
{
  char filename[CONFIG_FILENAME_LEN];

  if (sd_card > 0)
  {
    File json;
    StaticJsonDocument<JSON_BUFFER_SIZE> data_json;
    sprintf(filename, "/%s.json", SYS_CONFIG_NAME);

#ifdef DEBUG
    Serial.print(F("Saving sys config to "));
    Serial.println(filename);
#endif

    AudioNoInterrupts();
    SD.begin();
    SD.remove(filename);
    json = SD.open(filename, FILE_WRITE);
    if (json)
    {
      data_json["vol"] = configuration.sys.vol;
      data_json["mono"] = configuration.sys.mono;
      data_json["soft_midi_thru"] = configuration.sys.soft_midi_thru;
      data_json["performance_number"] = configuration.sys.performance_number;
      data_json["favorites"] = configuration.sys.favorites;
      data_json["load_at_startup"] = configuration.sys.load_at_startup;

#if defined(DEBUG) && defined(DEBUG_SHOW_JSON)
      Serial.println(F("Write JSON data:"));
      serializeJsonPretty(data_json, Serial);
      Serial.println();
#endif
      serializeJsonPretty(data_json, json);
      json.close();
      AudioInterrupts();
      save_sys_flag = false;
      return (true);
    }
    json.close();
  }
  else
  {
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
   SD SEQUENCER
 ******************************************************************************/

bool load_sd_chain_json(uint8_t number)
{
  if (number < 0)
    return (false);
  number = constrain(number, PERFORMANCE_NUM_MIN, PERFORMANCE_NUM_MAX);
  if (sd_card > 0)
  {
    File json;
    StaticJsonDocument<JSON_BUFFER_SIZE> data_json;
    char filename[CONFIG_FILENAME_LEN];
    sprintf(filename, "/%s/%d/%s.json", PERFORMANCE_CONFIG_PATH, number, CHAIN_CONFIG_NAME);
    // first check if file exists...
    AudioNoInterrupts();
    if (SD.exists(filename))
    {
      // ... and if: load
      json = SD.open(filename);
      if (json)
      {
        deserializeJson(data_json, json);
        json.close();
        AudioInterrupts();
        int total = sizeof(seq.chain);
        int columns = sizeof(seq.chain[0]);
        int rows = total / columns;
        int count = 0;
        for (uint8_t i = 0; i < rows; i++)
        {
          for (uint8_t j = 0; j < columns; j++) {
            seq.chain[i][j] = data_json["c"][count];
            count++;
          }
        }
        return (true);
      }
#ifdef DEBUG
      else
      {
        Serial.print(F("E: Cannot open "));
        Serial.print(filename);
        Serial.println(F(" on SD."));
      }
    }
    else
    {
      Serial.print(F("No "));
      Serial.print(filename);
      Serial.println(F(" available."));
#endif
    }
  }
  return (false);
}


bool save_sd_chain_json(uint8_t number)
{
  char filename[CONFIG_FILENAME_LEN];
  int count = 0;
  number = constrain(number, PERFORMANCE_NUM_MIN, PERFORMANCE_NUM_MAX);

  if (sd_card > 0)
  {
    File json;
    StaticJsonDocument<JSON_BUFFER_SIZE> data_json;
    sprintf(filename, "/%s/%d/%s.json", PERFORMANCE_CONFIG_PATH, number, CHAIN_CONFIG_NAME);

    int total = sizeof(seq.chain);
    int columns = sizeof(seq.chain[0]);
    int rows = total / columns;
    AudioNoInterrupts();
    SD.begin();
    SD.remove(filename);
    json = SD.open(filename, FILE_WRITE);
    if (json)
    {
      for (uint8_t i = 0; i < rows; i++)
      {
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
  }
  else
  {
#ifdef DEBUG
    Serial.print(F("E : Cannot open "));
    Serial.print(filename);
    Serial.println(F(" on SD."));
#endif
  }
  return (false);
}

bool load_sd_transpose_json(uint8_t number)
{
  if (number < 0)
    return (false);
  number = constrain(number, PERFORMANCE_NUM_MIN, PERFORMANCE_NUM_MAX);
  if (sd_card > 0)
  {
    File json;
    StaticJsonDocument<JSON_BUFFER_SIZE> data_json;
    char filename[CONFIG_FILENAME_LEN];
    sprintf(filename, "/%s/%d/%s.json", PERFORMANCE_CONFIG_PATH, number, TRANSPOSE_CONFIG_NAME);
    // first check if file exists...
    AudioNoInterrupts();
    if (SD.exists(filename))
    {
      // ... and if: load
      json = SD.open(filename);
      if (json)
      {
        deserializeJson(data_json, json);
        json.close();
        AudioInterrupts();
        int total = sizeof(seq.chain_transpose);
        int columns = sizeof(seq.chain_transpose[0]);
        int rows = total / columns;
        int count = 0;
        for (uint8_t i = 0; i < rows; i++)
        {
          for (uint8_t j = 0; j < columns; j++) {
            seq.chain_transpose[i][j] = data_json["t"][count];
            count++;
          }
        }
        return (true);
      }
#ifdef DEBUG
      else
      {
        Serial.print(F("E: Cannot open "));
        Serial.print(filename);
        Serial.println(F(" on SD."));
      }
    }
    else
    {
      Serial.print(F("No "));
      Serial.print(filename);
      Serial.println(F(" available."));
#endif
    }
  }
  return (false);
}


bool save_sd_transpose_json(uint8_t number)
{
  char filename[CONFIG_FILENAME_LEN];
  int count = 0;
  number = constrain(number, PERFORMANCE_NUM_MIN, PERFORMANCE_NUM_MAX);

  if (sd_card > 0)
  {
    File json;
    StaticJsonDocument<JSON_BUFFER_SIZE> data_json;
    sprintf(filename, "/%s/%d/%s.json", PERFORMANCE_CONFIG_PATH, number, TRANSPOSE_CONFIG_NAME);

    int total = sizeof(seq.chain_transpose);
    int columns = sizeof(seq.chain_transpose[0]);
    int rows = total / columns;
    AudioNoInterrupts();
    SD.begin();
    SD.remove(filename);
    json = SD.open(filename, FILE_WRITE);
    if (json)
    {
      for (uint8_t i = 0; i < rows; i++)
      {
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
  }
  else
  {
#ifdef DEBUG
    Serial.print(F("E : Cannot open "));
    Serial.print(filename);
    Serial.println(F(" on SD."));
#endif
  }
  return (false);
}


bool load_sd_song_json(uint8_t number)
{
  if (number < 0)
    return (false);
  number = constrain(number, PERFORMANCE_NUM_MIN, PERFORMANCE_NUM_MAX);
  if (sd_card > 0)
  {
    File json;
    StaticJsonDocument<JSON_BUFFER_SIZE> data_json;
    char filename[CONFIG_FILENAME_LEN];
    sprintf(filename, "/%s/%d/%s.json", PERFORMANCE_CONFIG_PATH, number, SONG_CONFIG_NAME);
    // first check if file exists...
    AudioNoInterrupts();
    if (SD.exists(filename))
    {
      // ... and if: load
      json = SD.open(filename);
      if (json)
      {
        deserializeJson(data_json, json);
        json.close();
        AudioInterrupts();
        int total = sizeof(seq.song);
        int columns = sizeof(seq.song[0]);
        int rows = total / columns;
        int count = 0;
        for (uint8_t i = 0; i < rows; i++)
        {
          for (uint8_t j = 0; j < columns; j++) {
            seq.song[i][j] = data_json["s"][count];
            count++;
          }
        }
        return (true);
      }
#ifdef DEBUG
      else
      {
        Serial.print(F("E: Cannot open "));
        Serial.print(filename);
        Serial.println(F(" on SD."));
      }
    }
    else
    {
      Serial.print(F("No "));
      Serial.print(filename);
      Serial.println(F(" available."));
#endif
    }
  }
  return (false);
}


bool save_sd_song_json(uint8_t number)
{
  char filename[CONFIG_FILENAME_LEN];
  int count = 0;
  number = constrain(number, PERFORMANCE_NUM_MIN, PERFORMANCE_NUM_MAX);

  if (sd_card > 0)
  {
    File json;
    StaticJsonDocument<JSON_BUFFER_SIZE> data_json;
    sprintf(filename, "/%s/%d/%s.json", PERFORMANCE_CONFIG_PATH, number, SONG_CONFIG_NAME);

    int total = sizeof(seq.song);
    int columns = sizeof(seq.song[0]);
    int rows = total / columns;
    AudioNoInterrupts();
    SD.begin();
    SD.remove(filename);
    json = SD.open(filename, FILE_WRITE);
    if (json)
    {
      for (uint8_t i = 0; i < rows; i++)
      {
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
  }
  else
  {
#ifdef DEBUG
    Serial.print(F("E : Cannot open "));
    Serial.print(filename);
    Serial.println(F(" on SD."));
#endif
  }
  return (false);
}

bool save_sd_seq_sub_vel_json(uint8_t number)
{
  char filename[CONFIG_FILENAME_LEN];
  int count = 0;
  number = constrain(number, PERFORMANCE_NUM_MIN, PERFORMANCE_NUM_MAX);

  if (sd_card > 0)
  {
    File json;
    StaticJsonDocument<JSON_BUFFER_SIZE> data_json;
    sprintf(filename, "/%s/%d/%s.json", PERFORMANCE_CONFIG_PATH, number, VELOCITY_CONFIG_NAME);
#ifdef DEBUG
    Serial.print(F("Saving sequencer velocity "));
    Serial.print(number);
    Serial.print(F(" to "));
    Serial.println(filename);
#endif
    int total = sizeof(seq.vel);
    int columns = sizeof(seq.vel[0]);
    int rows = total / columns;
    AudioNoInterrupts();
    SD.begin();
    SD.remove(filename);
    json = SD.open(filename, FILE_WRITE);
    if (json)
    {
      for (uint8_t i = 0; i < rows; i++)
      {
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
  }
  else
  {
#ifdef DEBUG
    Serial.print(F("E : Cannot open "));
    Serial.print(filename);
    Serial.println(F(" on SD."));
#endif
  }
  return (false);
}

bool save_sd_seq_sub_patterns_json(uint8_t number)
{
  char filename[CONFIG_FILENAME_LEN];
  int count = 0;
  number = constrain(number, PERFORMANCE_NUM_MIN, PERFORMANCE_NUM_MAX);

  if (sd_card > 0)
  {
    File json;
    StaticJsonDocument<JSON_BUFFER_SIZE> data_json;
    sprintf(filename, "/%s/%d/%s.json", PERFORMANCE_CONFIG_PATH, number, PATTERN_CONFIG_NAME);
#ifdef DEBUG
    Serial.print(F("Saving sequencer patterns "));
    Serial.print(number);
    Serial.print(F(" to "));
    Serial.println(filename);
#endif
    int total = sizeof(seq.note_data);
    int columns = sizeof(seq.note_data[0]);
    int rows = total / columns;
    AudioNoInterrupts();
    SD.begin();
    SD.remove(filename);
    json = SD.open(filename, FILE_WRITE);
    if (json)
    {
      for (uint8_t i = 0; i < rows; i++)
      {
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
  }
  else
  {
#ifdef DEBUG
    Serial.print(F("E : Cannot open "));
    Serial.print(filename);
    Serial.println(F(" on SD."));
#endif
  }
  return (false);
}

bool save_sd_performance_json(uint8_t number)
{
  char filename[CONFIG_FILENAME_LEN];
  bool seq_was_running = false;
  number = constrain(number, PERFORMANCE_NUM_MIN, PERFORMANCE_NUM_MAX);

  if (seq.running == true ) {
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

  for (uint8_t i = 0; i < MAX_DEXED; i++)
  {
    sprintf(filename, "/%s/%d/%s%d.json", PERFORMANCE_CONFIG_PATH, number, VOICE_CONFIG_NAME, i);
#ifdef DEBUG
    Serial.print(F("Write Voice-Config for sequencer"));
    Serial.println(filename);
#endif
    save_sd_voiceconfig_json(number, i);
  }
  if (sd_card > 0)
  {
    File json;
    StaticJsonDocument<JSON_BUFFER_SIZE> data_json;
    sprintf(filename, "/%s/%d/%s.json", PERFORMANCE_CONFIG_PATH, number, SEQUENCER_CONFIG_NAME);
#ifdef DEBUG
    Serial.print(F("Saving sequencer config "));
    Serial.print(number);
    Serial.print(F(" to "));
    Serial.println(filename);
#endif

    Serial.print(F("  "));
    SD.remove(filename);
    json = SD.open(filename, FILE_WRITE);
    if (json)
    { 
      data_json["seq_tempo_ms"] = seq.tempo_ms ;
      data_json["seq_bpm"] = seq.bpm;
      data_json["arp_speed"] = seq.arp_speed;
      data_json["arp_lenght"] = seq.arp_lenght;
      data_json["arp_volume_fade"] = seq.arp_volume_fade;
      data_json["arp_style"] = seq.arp_style;
      data_json["seq_chord_velocity"] = seq.chord_velocity;
      data_json["seq_chord_dexed_inst"] = seq.chord_dexed_inst;
      data_json["seq_transpose"] = seq.transpose;
      data_json["chord_key_ammount"] = seq.chord_key_ammount;
      data_json["seq_oct_shift"] = seq.oct_shift;
      data_json["seq_element_shift"] = seq.element_shift;
      for (uint8_t i = 0; i < sizeof(seq.track_type); i++) {
        data_json["track_type"][i] = seq.track_type[i];
      }
      for (uint8_t i = 0; i < sizeof(seq.content_type); i++) {
        data_json["content_type"][i] = seq.content_type[i];
      }
      for (uint8_t i = 0; i < sizeof(seq.inst_dexed); i++) {
        data_json["seq_inst_dexed"][i] = seq.inst_dexed[i];
      }
      for (uint8_t i = 0; i < FILENAME_LEN; i++) {
        data_json["seq_name"][i] = seq.name[i];
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
      if (seq_was_running == true )
        handleStart();
      return (true);
    }
    json.close();
    AudioInterrupts();
  }
#ifdef DEBUG
  else
  {
    Serial.print(F("E : Cannot open "));
    Serial.print(filename);
    Serial.println(F(" on SD."));
    AudioInterrupts();
  }
#endif

  return (false);
}

bool check_performance_directory(uint8_t number)
{
  char dir[CONFIG_FILENAME_LEN];

  if (sd_card > 0)
  {
    sprintf(dir, "/%s/%d", PERFORMANCE_CONFIG_PATH, number);

    AudioNoInterrupts();
    SD.begin();
    if (!SD.exists(dir))
    {
#ifdef DEBUG
      if (SD.mkdir(dir))
      {
        Serial.print(F("Creating directory "));
        Serial.println(dir);
      }
      else
      {
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
  else
  {
    Serial.println(F("E: SD card not available"));
  }
#endif
  return (false);
}

void get_sd_performance_name_json(uint8_t number)
{
  number = constrain(number, PERFORMANCE_NUM_MIN, PERFORMANCE_NUM_MAX);
  memset(seq.name_temp, 0, FILENAME_LEN);
  if (sd_card > 0)
  {
    File json;
    StaticJsonDocument<JSON_BUFFER_SIZE> data_json;
    char filename[CONFIG_FILENAME_LEN];

    sprintf(filename, "/%s/%d/%s.json", PERFORMANCE_CONFIG_PATH, number, SEQUENCER_CONFIG_NAME);

    // first check if file exists...
    AudioNoInterrupts();
    if (SD.exists(filename))
    {
      // ... and if: load

      json = SD.open(filename);
      if (json)
      {
        deserializeJson(data_json, json);

        json.close();
        AudioInterrupts();
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
      else
      {
        Serial.print(F("Cannot get performance name for "));
        Serial.print(number);
        Serial.println();
      }
#endif
    }
  }
}

bool load_sd_seq_sub_vel_json(uint8_t number)
{
  if (number < 0)
    return (false);
  number = constrain(number, PERFORMANCE_NUM_MIN, PERFORMANCE_NUM_MAX);
  if (sd_card > 0)
  {
    File json;
    StaticJsonDocument<JSON_BUFFER_SIZE> data_json;
    char filename[CONFIG_FILENAME_LEN];

    sprintf(filename, "/%s/%d/%s.json", PERFORMANCE_CONFIG_PATH, number, VELOCITY_CONFIG_NAME);

    // first check if file exists...
    AudioNoInterrupts();
    if (SD.exists(filename))
    {
      // ... and if: load
#ifdef DEBUG
      Serial.print(F("Found velocity data ["));
      Serial.print(filename);
      Serial.println(F("]... loading..."));
      Serial.println(F(" "));
#endif
      json = SD.open(filename);
      if (json)
      {
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
        for (uint8_t i = 0; i < rows; i++)
        {
          for (uint8_t j = 0; j < columns; j++) {
            seq.vel[i][j] = data_json["seq_velocity"][count];
            count++;
          }
        }
        return (true);
      }
#ifdef DEBUG
      else
      {
        Serial.print(F("E: Cannot open "));
        Serial.print(filename);
        Serial.println(F(" on SD."));
      }
    }
    else
    {
      Serial.print(F("No "));
      Serial.print(filename);
      Serial.println(F(" available."));
#endif
    }
  }
  return (false);
}

bool load_sd_seq_sub_patterns_json(uint8_t number)
{
  if (number < 0)
    return (false);
  number = constrain(number, PERFORMANCE_NUM_MIN, PERFORMANCE_NUM_MAX);
  if (sd_card > 0)
  {
    File json;
    StaticJsonDocument<JSON_BUFFER_SIZE> data_json;
    char filename[CONFIG_FILENAME_LEN];

    sprintf(filename, "/%s/%d/%s.json", PERFORMANCE_CONFIG_PATH, number, PATTERN_CONFIG_NAME);

    // first check if file exists...
    AudioNoInterrupts();
    if (SD.exists(filename))
    {
      // ... and if: load
#ifdef DEBUG
      Serial.print(F("Found pattern data ["));
      Serial.print(filename);
      Serial.println(F("]... loading..."));
      Serial.println(F(" "));
#endif
      json = SD.open(filename);
      if (json)
      {
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

        for (uint8_t i = 0; i < rows; i++)
        {
          for (uint8_t j = 0; j < columns; j++) {
            seq.note_data[i][j] = data_json["seq_data"][count];
            count++;
          }
        }
        return (true);
      }
#ifdef DEBUG
      else
      {
        Serial.print(F("E : Cannot open "));
        Serial.print(filename);
        Serial.println(F(" on SD."));
      }
    }
    else
    {
      Serial.print(F("No "));
      Serial.print(filename);
      Serial.println(F(" available."));
#endif
    }
  }
  return (false);
}

bool load_sd_performance_json(uint8_t number)
{
#ifdef USE_SEQUENCER
  bool seq_was_running = false;

  if (seq.running)
  {
    seq_was_running = true;
    seq.running = false;
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

  configuration.sys.performance_number = number;

  if (sd_card > 0)
  {
    File json;
    StaticJsonDocument<JSON_BUFFER_SIZE> data_json;
    char filename[CONFIG_FILENAME_LEN];
    sprintf(filename, "/%s/%d/%s.json", PERFORMANCE_CONFIG_PATH, number, SEQUENCER_CONFIG_NAME);
    // first check if file exists...
    if (SD.exists(filename))
    {
      // ... and if: load
#ifdef DEBUG
      Serial.print(F("Found Performance configuration ["));
      Serial.print(filename);
      Serial.println(F("]... loading..."));
#endif
      AudioNoInterrupts();
      json = SD.open(filename);
      if (json)
      {
        deserializeJson(data_json, json);
        json.close();
        AudioInterrupts();

#if defined(DEBUG) && defined(DEBUG_SHOW_JSON)
        Serial.println(F("Read JSON data:"));
        serializeJsonPretty(data_json, Serial);
        Serial.println();
#endif

        for (uint8_t i = 0; i < sizeof(seq.track_type); i++)
        {
          seq.track_type[i] = data_json["track_type"][i];
        }
        for (uint8_t i = 0; i < sizeof(seq.content_type); i++)
        {
          seq.content_type[i] = data_json["content_type"][i];
        }
        for (uint8_t i = 0; i < sizeof(seq.inst_dexed); i++)
        {
          seq.inst_dexed[i] = data_json["seq_inst_dexed"][i];
        }

        if (data_json["seq_name"][0] != 0)
        {
          for (uint8_t i = 0; i < FILENAME_LEN; i++)
          {
            seq.name[i] = data_json["seq_name"][i];
          }
        }

        seq.tempo_ms = data_json["seq_tempo_ms"] ;
        seq.bpm = data_json["seq_bpm"];
        seq.arp_speed = data_json["arp_speed"] ;
        seq.arp_lenght = data_json["arp_lenght"];
        seq.arp_volume_fade = data_json["arp_volume_fade"];
        seq.arp_style = data_json["arp_style"];
        seq.chord_velocity = data_json["seq_chord_velocity"];
        seq.chord_dexed_inst = data_json["seq_chord_dexed_inst"] ;
        seq.transpose = data_json["seq_transpose"];
        seq.chord_key_ammount = data_json["chord_key_ammount"];
        seq.oct_shift = data_json["seq_oct_shift"];
        seq.element_shift = data_json["seq_element_shift"];
        for (uint8_t instance_id = 0;  instance_id < NUM_DEXED; instance_id++)
        {
#ifdef DEBUG
          Serial.print(F("Load Voice-Config "));
          Serial.print(instance_id + 1);
          Serial.print(F(" for sequencer"));
#endif
          load_sd_voiceconfig_json(number, instance_id);
          load_sd_voice(configuration.dexed[instance_id].bank, configuration.dexed[instance_id].voice, instance_id);
          MicroDexed[instance_id]->setGain(midi_volume_transform(map(configuration.dexed[instance_id].sound_intensity, SOUND_INTENSITY_MIN, SOUND_INTENSITY_MAX, 0, 127)));
          MicroDexed[instance_id]->panic();
        }
        /* for (uint8_t instance_id = 0; instance_id < NUM_DEXED; instance_id++)
          set_voiceconfig_params(instance_id);

          set_fx_params();*/

        dac_unmute();
        seq.step = 0;

#ifdef USE_SEQUENCER
        if (seq_was_running)
        {
          sequencer_timer.begin(sequencer, seq.tempo_ms / 8);
          seq.running = true;
        }
        else
          sequencer_timer.begin(sequencer, seq.tempo_ms / 8, false);
#else
        seq.running = false;
#endif
        return (true);
      }
#ifdef DEBUG
      else
      {
        AudioInterrupts();
        Serial.print(F("E : Cannot open "));
        Serial.print(filename);
        Serial.println(F(" on SD."));
      }
    }
    else
    {
      Serial.print(F("No "));
      Serial.print(filename);
      Serial.println(F(" available."));
#endif
    }
  }
  return (false);
}

bool check_sd_performance_exists(uint8_t number)
{
  if (number < 0)
    return (false);

  number = constrain(number, PERFORMANCE_NUM_MIN, PERFORMANCE_NUM_MAX);
  AudioNoInterrupts();
  if (sd_card > 0)
  {
    char filename[CONFIG_FILENAME_LEN];

    sprintf(filename, "/%s/%d/%s.json", PERFORMANCE_CONFIG_PATH, number, SEQUENCER_CONFIG_NAME);

    // check if file exists...
    if (SD.exists(filename))
    {
      AudioInterrupts();
      return (true);
    } else
    {
      AudioInterrupts();
      return (false);
    }
  }
  else
  {
    AudioInterrupts();
    return (false);
  }
}

/******************************************************************************
   HELPER FUNCTIONS
 ******************************************************************************/
bool get_sd_data(File sysex, uint8_t format, uint8_t* conf)
{
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
  if (sysex.read() != format) // check for sysex type
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

  sysex.seek(sysex.size() - 2); // Bulk checksum
  bulk_checksum = sysex.read();

  sysex.seek(3); // start of bulk data
  for (n = 0; n < sysex.size() - 6; n++)
  {
    uint8_t d = sysex.read();
    bulk_checksum_calc -= d;
#ifdef DEBUG
    Serial.print(F("SYSEX data read: 0x"));
    Serial.println(d, HEX);
#endif
  }
  bulk_checksum_calc &= 0x7f;

  if (int8_t(bulk_checksum_calc) != bulk_checksum)
  {
#ifdef DEBUG
    Serial.print(F("E : Bulk checksum mismatch : 0x"));
    Serial.print(int8_t(bulk_checksum_calc), HEX);
    Serial.print(F(" != 0x"));
    Serial.println(bulk_checksum, HEX);
#endif
    return (false);
  }
#ifdef DEBUG
  else
  {
    Serial.print(F("Bulk checksum : 0x"));
    Serial.print(int8_t(bulk_checksum_calc), HEX);
    Serial.print(F(" [0x"));
    Serial.print(bulk_checksum, HEX);
    Serial.println(F("]"));
  }
#endif

  sysex.seek(3); // start of bulk data
  for (n = 0; n < sysex.size() - 6; n++)
  {
    uint8_t d = sysex.read();
    *(conf++) = d;
  }
  AudioInterrupts();

#ifdef DEBUG
  Serial.println(F("SD data loaded."));
#endif

  return (true);
}

bool write_sd_data(File sysex, uint8_t format, uint8_t* data, uint16_t len)
{
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
  for (uint16_t i = 0; i < len; i++)
  {
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

uint8_t calc_checksum(uint8_t* data, uint16_t len)
{
  int32_t bulk_checksum_calc = 0;

  for (uint16_t n = 0; n < len; n++)
    bulk_checksum_calc -= data[n];

  return (bulk_checksum_calc & 0x7f);
}

void strip_extension(const char* s, char* target, uint8_t len)
{
  char tmp[CONFIG_FILENAME_LEN];
  char* token;

  strcpy(tmp, s);
  token = strtok(tmp, ".");
  if (token == NULL)
    strcpy(target, "*ERROR*");
  else
    strcpy(target, token);

  target[len] = '\0';
}

bool get_bank_name(uint8_t b, char* name, uint8_t len)
{
  File sysex;

  if (sd_card > 0)
  {
    char bankdir[4];
    File entry;

    memset(name, 0, len);

    sprintf(bankdir, "/%d", b);

    // try to open directory
    sysex = SD.open(bankdir);
    if (!sysex)
      return (false);

    do
    {
      entry = sysex.openNextFile();
    } while (entry.isDirectory());

    if (entry.isDirectory())
    {
      entry.close();
      sysex.close();
      return (false);
    }

    strip_extension(entry.name(), name, len);

#ifdef DEBUG
    Serial.print(F("Found bank-name ["));
    Serial.print(name);
    Serial.print(F("] for bank ["));
    Serial.print(b);
    Serial.println(F("]"));
#endif

    entry.close();
    sysex.close();

    return (true);
  }

  return (false);
}

bool get_voice_name(uint8_t b, uint8_t v, char* name, uint8_t len)
{
  File sysex;

  if (sd_card > 0)
  {
    char bank_name[BANK_NAME_LEN];
    char filename[FILENAME_LEN];

    b = constrain(b, 0, MAX_BANKS - 1);
    v = constrain(v, 0, MAX_VOICES - 1);

    get_bank_name(b, bank_name, sizeof(bank_name));
    sprintf(filename, "/%d/%s.syx", b, bank_name);
#ifdef DEBUG
    Serial.print(F("Reading voice-name from ["));
    Serial.print(filename);
    Serial.println(F("]"));
#endif

    // try to open directory
    AudioNoInterrupts();
    sysex = SD.open(filename);
    if (!sysex)
      return (false);

    memset(name, 0, len);
    sysex.seek(124 + (v * 128));
    sysex.read(name, min(len, 10));

#ifdef DEBUG
    Serial.print(F("Found voice-name ["));
    Serial.print(name);
    Serial.print(F("] for bank ["));
    Serial.print(b);
    Serial.print(F("] and voice ["));
    Serial.print(v);
    Serial.println(F("]"));
#endif

    sysex.close();
    AudioInterrupts();

    return (true);
  }

  return (false);
}

bool get_voice_by_bank_name(uint8_t b, const char* bank_name, uint8_t v, char* voice_name, uint8_t len)
{
  File sysex;

  if (sd_card > 0)
  {
    char filename[FILENAME_LEN];

    sprintf(filename, "/%d/%s.syx", b, bank_name);
#ifdef DEBUG
    Serial.print(F("Reading voice-name from ["));
    Serial.print(filename);
    Serial.println(F("]"));
#endif

    // try to open directory
    AudioNoInterrupts();
    sysex = SD.open(filename);
    if (!sysex)
      return (false);

    memset(voice_name, 0, len);
    sysex.seek(124 + (v * 128));
    sysex.read(voice_name, min(len, 10));

#ifdef DEBUG
    Serial.print(F("Found voice-name ["));
    Serial.print(voice_name);
    Serial.print(F("] for bank ["));
    Serial.print(b);
    Serial.print(F("|"));
    Serial.print(bank_name);
    Serial.print(F("] and voice ["));
    Serial.print(v);
    Serial.println(F("]"));
#endif

    sysex.close();
    AudioInterrupts();

    return (true);
  }

  return (false);
}

void string_toupper(char* s)
{
  while (*s)
  {
    *s = toupper((unsigned char) * s);
    s++;
  }
}
