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
#include "microsynth.h"
#include "braids.h"
#include "touch.h"

#if NUM_DRUMS > 0
#include "drums.h"
extern void set_drums_volume(float vol);
extern drum_config_t drum_config[];
extern custom_midi_map_t custom_midi_map[NUM_CUSTOM_MIDI_MAPPINGS];
#endif

extern sdcard_t sdcard_infos;
extern fm_t fm;

extern char g_voice_name[NUM_DEXED][VOICE_NAME_LEN];
extern char g_bank_name[NUM_DEXED][BANK_NAME_LEN];
extern void init_MIDI_send_CC(void);
extern void update_euclidean(void);

extern microsynth_t microsynth[NUM_MICROSYNTH];

extern braids_t braids_osc;

extern uint8_t gamepad_0_neutral;
extern uint8_t gamepad_1_neutral;

#include "sequencer.h"
extern PeriodicTimer sequencer_timer;
extern void sequencer();
extern sequencer_t seq;

extern mb_t mb;

extern uint8_t drum_midi_channel;

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
extern uint8_t get_sample_note(uint8_t sample);
extern float get_sample_pitch(uint8_t sample);
extern float get_sample_p_offset(uint8_t sample);
extern float get_sample_pan(uint8_t sample);
extern float get_sample_vol_max(uint8_t sample);
extern float get_sample_vol_min(uint8_t sample);
extern float get_sample_reverb_send(uint8_t sample);
extern uint8_t find_drum_number_from_note(uint8_t note);
extern multisample_t msp[NUM_MULTISAMPLES];
extern multisample_zone_t msz[NUM_MULTISAMPLES][NUM_MULTISAMPLE_ZONES];

extern bool sidechain_a_active;
extern bool sidechain_b_active;
extern uint8_t sidechain_a_sample_number;
extern uint8_t sidechain_b_sample_number;
extern uint8_t sidechain_a_speed;
extern uint8_t sidechain_b_speed;
extern uint8_t sc_dexed1_target_a;
extern uint8_t sc_dexed2_target_a;
extern uint8_t sc_dexed1_target_b;
extern uint8_t sc_dexed2_target_b;
extern uint8_t sc_reverb_target_a;
extern uint8_t sc_reverb_target_b;

File json;
char filename[CONFIG_FILENAME_LEN];
const char* sError = "*ERROR*";

FLASHMEM void strip_extension(const char* s, char* target, uint8_t len)
{
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

FLASHMEM void string_toupper(char* s)
{
  while (*s)
  {
    *s = toupper((unsigned char)*s);
    s++;
  }
}

FLASHMEM File open_file_for_read(const char* filename)
{
  AudioNoInterrupts();
  File file = SD.open(filename);
  if(!file)
  {
#ifdef DEBUG
    LOG.print(F("E : Cannot open for read: ")); LOG.println(filename);
#endif
    AudioInterrupts();
    return file;
  }
#ifdef DEBUG
  LOG.print(F("Reading ")); LOG.println(filename);
#endif
  return file;
}

FLASHMEM File open_file_for_write(const char* filename)
{
  AudioNoInterrupts();
  File file = SD.open(filename, FILE_WRITE);
  if (!file)
  {
#ifdef DEBUG
    LOG.print(F("E : Cannot open for write: ")); LOG.println(filename);
#endif
    file.close();
    AudioInterrupts();
    return file;
  }
#ifdef DEBUG
  LOG.print(F("Writing ")); LOG.println(filename);
#endif
  return file;
}

FLASHMEM File create_file_for_write(const char* filename)
{
  AudioNoInterrupts();
  SD.remove(filename);
  return open_file_for_write(filename);
}

FLASHMEM void close_file (File file)
{
#ifdef DEBUG
  LOG.print(F("Closing ")); LOG.println(file.name());
#endif
  file.close();
  AudioInterrupts();
}

FLASHMEM bool read_file_json(const char* filename, StaticJsonDocument<JSON_BUFFER_SIZE>& document)
{
  File file = open_file_for_read(filename);
  if(!file) return false;
  deserializeJson(document, file);
  close_file(file);

#if defined(DEBUG) && defined(DEBUG_SHOW_JSON)
  LOG.println(F("Read JSON data:"));
  serializeJsonPretty(document, Serial);
  LOG.println();
#endif
  return true;
}

FLASHMEM bool write_file_json(const char* filename, StaticJsonDocument<JSON_BUFFER_SIZE>& document)
{
#if defined(DEBUG) && defined(DEBUG_SHOW_JSON)
  LOG.println(F("Write JSON data:")); serializeJsonPretty(document, Serial); LOG.println();
#endif

  File file = create_file_for_write(filename);
  if(!file) return false;

  serializeJsonPretty(document, file);
  close_file(file);
  return true;
}

FLASHMEM bool load_sd_config_json(const char* filename, Params* params)
{
  StaticJsonDocument<JSON_BUFFER_SIZE> data_json;
  if(!read_file_json(filename, data_json)) return false;

  Param* prm = params->getParams();
  do{
    if(data_json.containsKey(prm->desc->name))
    {
      prm->set(data_json[prm->desc->name]);
      LOG.print("Load param:"); LOG.print(prm->desc->name); LOG.print(" "); LOG.print(prm->get()); LOG.println();
    }
    else
      LOG.print("Missing param:"); LOG.println(prm->desc->name);
    prm = prm->next();
  }while (prm != NULL);

  return true;
}

FLASHMEM bool save_sd_config_json(const char* filename, Params* params)
{
  StaticJsonDocument<JSON_BUFFER_SIZE> data_json;
  Param* prm = params->getParams();
  do{
    data_json[prm->desc->name] = prm->get();
    LOG.print("Save param:"); LOG.print(prm->desc->name); LOG.print(" "); LOG.print(prm->get()); LOG.println();
    prm = prm->next();
  }while (prm != NULL);

  write_file_json(filename, data_json);

  return true;
}

/******************************************************************************
   SD BANK/VOICE LOADING
 ******************************************************************************/


FLASHMEM bool check_magic(File sysex, uint16_t offset, uint8_t magic, const char* message) {
  sysex.seek(offset);
  if (sysex.read() != magic)
  {
#ifdef DEBUG
    LOG.println(message);
#endif
    return false;
  }
  return true;
}

FLASHMEM bool check_syx(File sysex)
{
  if (sysex.size() != 4104) // check sysex size
  {
#ifdef DEBUG
    LOG.println(F("E : SysEx file size wrong."));
#endif
    return false;
  }
  if(!check_magic(sysex, 0, 0xf0, "E : SysEx start byte not found."  )) return false;
  if(!check_magic(sysex, 1, 0x43, "E : SysEx vendor not Yamaha."     )) return false;
  if(!check_magic(sysex, 4103, 0xf7, "E : SysEx end byte not found." )) return false;
  if(!check_magic(sysex, 3, 0x09, "E : SysEx type not 32 voices."    )) return false;
}

FLASHMEM int8_t checksum_syx(File sysex)
{
  sysex.seek(6); // start of bulk data
  uint8_t data[4096];
  sysex.read(data, 4096);
  int32_t checksum = 0;
  for (uint16_t n = 0; n < 4096; n++)
    checksum -= data[n];

  return checksum & 0x7f;
}

FLASHMEM File find_first_file(const char* dirname)
{
  File dir = open_file_for_read(dirname);
  if (!dir) return false;

  File file;
  do
  {
    file = dir.openNextFile();
  } while (file.isDirectory());
  dir.close();
  
  return file;
}

FLASHMEM bool load_sd_voice(uint8_t p, uint8_t b, uint8_t v, uint8_t instance_id)
{
#ifdef DEBUG
  LOG.printf_P(PSTR("load voice, bank [%d] - voice [%d]\n"), b, v + 1);
#endif


  // default on error names until all steps where successful
  strcpy(g_bank_name[instance_id], sError);
  strcpy(g_voice_name[instance_id], sError);

  // find the first file (any name) in bank directory
  char bankdir[FILENAME_LEN];
  snprintf_P(bankdir, sizeof(bankdir), PSTR("/%s/%d/%d"), DEXED_CONFIG_PATH, p, b);
  File sysex = find_first_file(bankdir);
  if (!sysex || !check_syx(sysex))
  {
    close_file(sysex);
    return false;
  }

  strip_extension(sysex.name(), g_bank_name[instance_id], BANK_NAME_LEN);
  string_toupper(g_bank_name[instance_id]);
#ifdef DEBUG
  LOG.printf(F("Loading voice from /%s/%d/%s.syx\n"),DEXED_CONFIG_PATH, b, g_bank_name[instance_id]); 
#endif

  v = constrain(v, 0, MAX_VOICES - 1);
  
  uint8_t data[128];
  sysex.seek(6 + v * 128); // start of selected voice data
  sysex.read(data, 128);
  close_file(sysex);

  memset(g_voice_name[instance_id], '\0', VOICE_NAME_LEN);
  memcpy(g_voice_name[instance_id], &data[118], min(VOICE_NAME_LEN-1, 10));
  string_toupper(g_voice_name[instance_id]);
  
  // MicroDexed[0]->resetRenderTimeMax(); // necessary?

#ifdef DEBUG
  LOG.printf(F("get_sd_voice:[%s]\n"),g_voice_name[instance_id]);
#endif

  uint8_t tmp_data[156];
  if(!MicroDexed[instance_id]->decodeVoice(tmp_data, data)) return false;

  MicroDexed[instance_id]->loadVoiceParameters(tmp_data);
#ifdef DEBUG
  show_patch(instance_id);
#endif

  configuration.dexed[instance_id].transpose = MicroDexed[instance_id]->getTranspose();
  configuration.dexed[instance_id].pool = p;
  configuration.dexed[instance_id].bank = b;
  configuration.dexed[instance_id].voice = v;

  send_sysex_voice(configuration.dexed[instance_id].midi_channel, tmp_data);
  init_MIDI_send_CC();

  return true;
}

FLASHMEM bool save_sd_voice(uint8_t p, uint8_t b, uint8_t v, uint8_t instance_id)
{
#ifdef DEBUG
  LOG.printf_P(PSTR("save_sd_voice, b:%d - d:%d\n"), b, v);
#endif

  snprintf_P(filename, sizeof(filename), PSTR("/%s/%d/%d/%s.syx"), DEXED_CONFIG_PATH, p, b, g_bank_name[instance_id]);

  File sysex = open_file_for_write(filename);
  if (!sysex) return false;
  if(!check_syx(sysex)) {
    close_file(sysex);
    return false;
  }

  uint8_t data[128];
  MicroDexed[instance_id]->encodeVoice(data);
  memcpy(&data[118],g_voice_name[instance_id], 10); // TODO voice appears empty sometimes otherwise, why?
  v = constrain(v, 0, MAX_VOICES - 1);
  sysex.seek(6 + (v * 128));
  sysex.write(data, 128);
  // checksum calculation
  uint8_t checksum=checksum_syx(sysex);
  sysex.seek(4102); // Bulk checksum
  sysex.write(checksum);
#ifdef DEBUG
  LOG.print(F("Bulk checksum : 0x")); LOG.println(checksum, HEX);
#endif

  close_file(sysex);
#ifdef DEBUG
  LOG.printf(F("Saved voice to %s [%s]\n"), filename, g_voice_name[instance_id]);
#endif
  return true;
}

FLASHMEM bool save_sd_bank(const char* bank_filename, uint8_t* data)
{
  char tmp[FILENAME_LEN];
  char tmp2[FILENAME_LEN];
  int bank_number;
  File root, entry;

  if (sd_card > 0)
  {
#ifdef DEBUG
    LOG.print(F("Trying t o store "));
    LOG.print(bank_filename);
    LOG.println(F("."));
#endif

    // first remove old bank => find the bank number
    sscanf(bank_filename, "/%s/%d/%s", tmp, &bank_number, tmp2);
    snprintf_P(tmp, sizeof(tmp), PSTR("/%s/%d"), DEXED_CONFIG_PATH, bank_number);
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
          LOG.print(F("Removing "));
          LOG.print(tmp);
          LOG.print(F("/"));
          LOG.println(entry.name());
#endif
          snprintf_P(tmp2, sizeof(tmp2), PSTR("/%s/%s/%s"), DEXED_CONFIG_PATH, tmp, entry.name());
          entry.close();
#ifdef DEBUG
          LOG.printf("Remove file %s\n", tmp2);
#else
          bool r = SD.remove(tmp2);
          if (r == false)
          {
            LOG.print(F("E: cannot remove "));
            LOG.print(tmp2);
            LOG.println(F("."));
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

    // store new bank at /DEXED/<b>/<bank_name>.syx
#ifdef DEBUG
    LOG.print(F("Storing bank as "));
    LOG.print(bank_filename);
    LOG.print(F("..."));
#endif
    root = SD.open(bank_filename, FILE_WRITE);
    root.write(data, 4104);
    root.close();
    AudioInterrupts();
#ifdef DEBUG
    LOG.println(F(" done."));
#endif
  }
  else
    return (false);

  return (true);
}

/******************************************************************************
   SD DRUM CUSTOM MAPPINGS
 ******************************************************************************/

FLASHMEM bool load_sd_drummappings_json(uint8_t number)
{
#if NUM_DRUMS > 0
  if (number < 0) false;

  number = constrain(number, PERFORMANCE_NUM_MIN, PERFORMANCE_NUM_MAX);
  snprintf_P(filename, sizeof(filename), PSTR("/%s/%d/%s.json"), PERFORMANCE_CONFIG_PATH, number, DRUMS_MAPPING_NAME);
  StaticJsonDocument<JSON_BUFFER_SIZE> data_json;
  if(!read_file_json(filename, data_json)) return false;

  for (uint8_t i = 0; i < NUM_CUSTOM_MIDI_MAPPINGS; i++)
  {
    custom_midi_map[i].type = data_json["type"][i];
    custom_midi_map[i].in = data_json["in"][i];
    custom_midi_map[i].out = data_json["out"][i];
    custom_midi_map[i].channel = data_json["channel"][i];
  }
#endif
  return true;
}

FLASHMEM bool save_sd_drummappings_json(uint8_t number)
{
#if NUM_DRUMS > 0
  number = constrain(number, 0, 99);
  if (!check_performance_directory(number)) return false;
  
  StaticJsonDocument<JSON_BUFFER_SIZE> data_json;
  for (uint8_t i = 0; i < NUM_CUSTOM_MIDI_MAPPINGS; i++)
  {
    data_json["type"][i] = custom_midi_map[i].type;
    data_json["in"][i] = custom_midi_map[i].in;
    data_json["out"][i] = custom_midi_map[i].out;
    data_json["channel"][i] = custom_midi_map[i].channel;
  }
  snprintf_P(filename, sizeof(filename), PSTR("/%s/%d/%s.json"), PERFORMANCE_CONFIG_PATH, number, DRUMS_MAPPING_NAME);
  return write_file_json(filename, data_json);
#endif
}

/******************************************************************************
   SD DRUMSETTINGS
 ******************************************************************************/
FLASHMEM bool load_sd_drumsettings_json(uint8_t number)
{
#if NUM_DRUMS > 0

  if (number < 0) return false;

  number = constrain(number, PERFORMANCE_NUM_MIN, PERFORMANCE_NUM_MAX);
  snprintf_P(filename, sizeof(filename), PSTR("/%s/%d/%s.json"), PERFORMANCE_CONFIG_PATH, number, DRUMS_CONFIG_NAME);
  StaticJsonDocument<JSON_BUFFER_SIZE> data_json;
  if(!read_file_json(filename, data_json)) return false;

  seq.drums_volume = data_json["drums_volume"];
  set_drums_volume(seq.drums_volume);
  for (uint8_t i = 0; i < NUM_DRUMSET_CONFIG - 1; i++)
  {
    uint8_t drumnumber = 0;
    drumnumber = find_drum_number_from_note(data_json["note"][i]);
    if (((int)data_json["note"][i] > 0 && find_drum_number_from_note(data_json["note"][i]) > 0) || (i == 0 && (int)data_json["note"][i] == 210))
    {
      set_sample_pitch(drumnumber, data_json["pitch"][i]);
      set_sample_p_offset(drumnumber, data_json["p_offset"][i]);
      set_sample_pan(drumnumber, data_json["pan"][i]);
      set_sample_vol_max(drumnumber, data_json["vol_max"][i]);
      set_sample_vol_min(drumnumber, data_json["vol_min"][i]);
      set_sample_reverb_send(drumnumber, data_json["reverb_send"][i]);
    }
  }
#endif
  return true;
}

FLASHMEM bool save_sd_drumsettings_json(uint8_t number)
{
#if NUM_DRUMS > 0
  number = constrain(number, PERFORMANCE_NUM_MIN, PERFORMANCE_NUM_MAX);
  if (!check_performance_directory(number)) return false;

  StaticJsonDocument<JSON_BUFFER_SIZE> data_json;
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
  snprintf_P(filename, sizeof(filename), PSTR("/%s/%d/%s.json"), PERFORMANCE_CONFIG_PATH, number, DRUMS_CONFIG_NAME);
  return write_file_json(filename, data_json);
#endif
}

/******************************************************************************
   SD VOICECONFIG
 ******************************************************************************/

FLASHMEM bool load_sd_voiceconfig_json(uint8_t vc, uint8_t instance_id)
{
  vc = constrain(vc, PERFORMANCE_NUM_MIN, PERFORMANCE_NUM_MAX);
  snprintf_P(filename, sizeof(filename), PSTR("/%s/%d/%s%d.json"), PERFORMANCE_CONFIG_PATH, vc, VOICE_CONFIG_NAME, instance_id + 1);
  if(!load_sd_config_json(filename, &configuration.dexed[instance_id])) return false;
  set_voiceconfig_params(instance_id);
  return true;
}

FLASHMEM bool save_sd_voiceconfig_json(uint8_t vc, uint8_t instance_id)
{
  vc = constrain(vc, PERFORMANCE_NUM_MIN, PERFORMANCE_NUM_MAX);
  snprintf_P(filename, sizeof(filename), PSTR("/%s/%d/%s%d.json"), PERFORMANCE_CONFIG_PATH, vc, VOICE_CONFIG_NAME, instance_id + 1);
  return save_sd_config_json(filename, &configuration.dexed[instance_id]);
}

/******************************************************************************
   SD MICROSYNTH
 ******************************************************************************/
FLASHMEM bool load_sd_microsynth_json(uint8_t ms, uint8_t instance_id)
{
  ms = constrain(ms, PERFORMANCE_NUM_MIN, PERFORMANCE_NUM_MAX);
  snprintf_P(filename, sizeof(filename), PSTR("/%s/%d/%s%d.json"), PERFORMANCE_CONFIG_PATH, ms, MICROSYNTH_CONFIG_NAME, instance_id + 1);
  if(!load_sd_config_json(filename, &microsynth[instance_id])) return false;
  microsynth_update_all_settings(instance_id);
  return true;
}

FLASHMEM bool save_sd_microsynth_json(uint8_t ms, uint8_t instance_id)
{
  ms = constrain(ms, PERFORMANCE_NUM_MIN, PERFORMANCE_NUM_MAX);
  snprintf_P(filename, sizeof(filename), PSTR("/%s/%d/%s%d.json"), PERFORMANCE_CONFIG_PATH, ms, MICROSYNTH_CONFIG_NAME, instance_id + 1);
  return save_sd_config_json(filename, &microsynth[instance_id]);
}

/******************************************************************************
   SD FX
 ******************************************************************************/
FLASHMEM bool load_sd_fx_json(uint8_t number)
{
  number = constrain(number, PERFORMANCE_NUM_MIN, PERFORMANCE_NUM_MAX);
  load_sd_drumsettings_json(number);

  snprintf_P(filename, sizeof(filename), PSTR("/%s/%d/%s.json"), PERFORMANCE_CONFIG_PATH, number, FX_CONFIG_NAME);  
  StaticJsonDocument<JSON_BUFFER_SIZE> data_json;
  if(!read_file_json(filename, data_json)) return false;

  Param* prm = configuration.fx.getParams();
  do{
    prm->set(data_json[prm->desc->name]);
    LOG.print("Load param:"); LOG.print(prm->desc->name); LOG.print(" "); LOG.print(prm->get()); LOG.println();
    prm = prm->next();
  }while (prm != NULL);

  for (uint8_t i = 0; i < MAX_DEXED; i++)
  {
    Param* prm = configuration.fx.dexed[i].getParams();
    do{
      prm->set(data_json[prm->desc->name][i]);
      LOG.print("Load param:"); LOG.print(prm->desc->name); LOG.print(" "); LOG.print(prm->get()); LOG.println();
      prm = prm->next();
    }while (prm != NULL);
    if (configuration.fx.dexed[i].delay_sync > 0)
      configuration.fx.dexed[i].delay_time = 0;
  }

  set_fx_params();

  return true;
}

FLASHMEM bool save_sd_fx_json(uint8_t number)
{
  number = constrain(number, PERFORMANCE_NUM_MIN, PERFORMANCE_NUM_MAX);

  save_sd_drumsettings_json(number);

  StaticJsonDocument<JSON_BUFFER_SIZE> data_json;
  
  Param* prm = configuration.fx.getParams();
  do{
    data_json[prm->desc->name] = prm->get();
    LOG.print("Save param:"); LOG.print(prm->desc->name); LOG.print(" "); LOG.print(prm->get()); LOG.println();
    prm = prm->next();
  }while (prm != NULL);

  for (uint8_t i = 0; i < MAX_DEXED; i++)
  {
    Param* prm = configuration.fx.dexed[i].getParams();
    do{
      data_json[prm->desc->name][i] = prm->get();
      LOG.print("Save param:"); LOG.print(prm->desc->name); LOG.print(" "); LOG.print(prm->get()); LOG.println();
      prm = prm->next();
    }while (prm != NULL);
  }

  snprintf_P(filename, sizeof(filename), PSTR("/%s/%d/%s.json"), PERFORMANCE_CONFIG_PATH, number, FX_CONFIG_NAME);
  return write_file_json(filename, data_json);
}

/******************************************************************************
   SD EPIANO
 ******************************************************************************/
FLASHMEM bool load_sd_epiano_json(uint8_t number)
{
  number = constrain(number, PERFORMANCE_NUM_MIN, PERFORMANCE_NUM_MAX);
  snprintf_P(filename, sizeof(filename), PSTR("/%s/%d/%s.json"), PERFORMANCE_CONFIG_PATH, number, EPIANO_CONFIG_NAME);
  if(!load_sd_config_json(filename, &configuration.epiano)) return false;
  set_epiano_params();
  return true;
}

FLASHMEM bool save_sd_epiano_json(uint8_t number)
{
  number = constrain(number, PERFORMANCE_NUM_MIN, PERFORMANCE_NUM_MAX);
  snprintf_P(filename, sizeof(filename), PSTR("/%s/%d/%s.json"), PERFORMANCE_CONFIG_PATH, number, EPIANO_CONFIG_NAME);
  return save_sd_config_json(filename, &configuration.epiano);
}

/******************************************************************************
   SD SYS
 ******************************************************************************/
FLASHMEM bool load_sd_sys_json(void)
{
  snprintf_P(filename, sizeof(filename), PSTR("/%s.json"), SYS_CONFIG_NAME);
  if(!load_sd_config_json(filename, &configuration.sys)) return false;
  set_sys_params();
}

FLASHMEM bool save_sd_sys_json(void)
{
  snprintf_P(filename, sizeof(filename), PSTR("/%s.json"), SYS_CONFIG_NAME);
  return save_sd_config_json(filename, &configuration.sys);
}

/******************************************************************************
   SD BRAIDS
 ******************************************************************************/

FLASHMEM bool load_sd_braids_json(uint8_t number)
{
  if (number < 0)
    return (false);
  number = constrain(number, PERFORMANCE_NUM_MIN, PERFORMANCE_NUM_MAX);
  snprintf_P(filename, sizeof(filename), PSTR("/%s/%d/%s.json"), PERFORMANCE_CONFIG_PATH, number, BRAIDS_CONFIG_NAME);
  if(!load_sd_config_json(filename, &braids_osc)) return false;
  braids_update_all_settings();
  return true;
}

FLASHMEM bool save_sd_braids_json(uint8_t number)
{
  snprintf_P(filename, sizeof(filename), PSTR("/%s/%d/%s.json"), PERFORMANCE_CONFIG_PATH, number, BRAIDS_CONFIG_NAME);
  return save_sd_config_json(filename, &braids_osc);
}

/******************************************************************************
   SD MULTIBAND
 ******************************************************************************/

FLASHMEM bool load_sd_multiband_json(uint8_t number)
{
  if (number < 0)
    return (false);
  number = constrain(number, PERFORMANCE_NUM_MIN, PERFORMANCE_NUM_MAX);
  snprintf_P(filename, sizeof(filename), PSTR("/%s/%d/%s.json"), PERFORMANCE_CONFIG_PATH, number, MULTIBAND_CONFIG_NAME);
  return load_sd_config_json(filename, &mb);
}

FLASHMEM bool save_sd_multiband_json(uint8_t number)
{
  snprintf_P(filename, sizeof(filename), PSTR("/%s/%d/%s.json"), PERFORMANCE_CONFIG_PATH, number, MULTIBAND_CONFIG_NAME);
  return save_sd_config_json(filename, &mb);
}

/******************************************************************************
   SD SIDECHAIN
 ******************************************************************************/

FLASHMEM bool load_sd_sidechain_json(uint8_t number)
{
  if (number < 0) return false;

  number = constrain(number, PERFORMANCE_NUM_MIN, PERFORMANCE_NUM_MAX);
  snprintf_P(filename, sizeof(filename), PSTR("/%s/%d/%s.json"), PERFORMANCE_CONFIG_PATH, number, SIDECHAIN_CONFIG_NAME);
  StaticJsonDocument<JSON_BUFFER_SIZE> data_json;
  if(!read_file_json(filename, data_json)) return false;

  sidechain_a_active = data_json["a_active"];
  sidechain_b_active = data_json["b_active"];
  sidechain_a_sample_number = data_json["a_sample_number"];
  sidechain_b_sample_number = data_json["b_sample_number"];
  sidechain_a_speed = data_json["a_speed"];
  sidechain_b_speed = data_json["b_speed"];
  sc_dexed1_target_a = data_json["dexed1_target_a"];
  sc_dexed2_target_a = data_json["dexed2_target_a"];
  sc_dexed1_target_b = data_json["dexed1_target_b"];
  sc_dexed2_target_b = data_json["dexed2_target_b"];
  sc_reverb_target_a = data_json["reverb_target_a"];
  sc_reverb_target_b = data_json["reverb_target_b"];
  return (true);
}

FLASHMEM bool save_sd_sidechain_json(uint8_t number)
{
  StaticJsonDocument<JSON_BUFFER_SIZE> data_json;

  data_json["a_active"] = sidechain_a_active;
  data_json["b_active"] = sidechain_b_active;
  data_json["a_sample_number"] = sidechain_a_sample_number;
  data_json["b_sample_number"] = sidechain_b_sample_number;
  data_json["a_speed"] = sidechain_a_speed;
  data_json["b_speed"] = sidechain_b_speed;
  data_json["dexed1_target_a"] = sc_dexed1_target_a;
  data_json["dexed2_target_a"] = sc_dexed2_target_a;
  data_json["dexed1_target_b"] = sc_dexed1_target_b;
  data_json["dexed2_target_b"] = sc_dexed2_target_b;
  data_json["reverb_target_a"] = sc_reverb_target_a;
  data_json["reverb_target_b"] = sc_reverb_target_b;

  snprintf_P(filename, sizeof(filename), PSTR("/%s/%d/%s.json"), PERFORMANCE_CONFIG_PATH, number, SIDECHAIN_CONFIG_NAME);
  return write_file_json(filename, data_json);
}

/******************************************************************************
   SD SEQUENCER
 ******************************************************************************/

FLASHMEM bool load_sd_chain_json(uint8_t number)
{
  if (number < 0) return false;

  number = constrain(number, PERFORMANCE_NUM_MIN, PERFORMANCE_NUM_MAX);
  snprintf_P(filename, sizeof(filename), PSTR("/%s/%d/%s.json"), PERFORMANCE_CONFIG_PATH, number, CHAIN_CONFIG_NAME);

  StaticJsonDocument<JSON_BUFFER_SIZE> data_json;
  if(!read_file_json(filename, data_json)) return false;

  int total = sizeof(seq.chain);
  int columns = sizeof(seq.chain[0]);
  int rows = total / columns;
  int count = 0;
  for (uint8_t i = 0; i < rows; i++)
  {
    for (uint8_t j = 0; j < columns; j++)
    {
      seq.chain[i][j] = data_json["c"][count];
      count++;
    }
  }
  return true;
}

FLASHMEM bool save_sd_chain_json(uint8_t number)
{
  int count = 0;
  int total = sizeof(seq.chain);
  int columns = sizeof(seq.chain[0]);
  int rows = total / columns;
  StaticJsonDocument<JSON_BUFFER_SIZE> data_json;
  for (uint8_t i = 0; i < rows; i++)
  {
    for (uint8_t j = 0; j < columns; j++)
    {
      data_json["c"][count] = seq.chain[i][j];
      count++;
    }
  }

  number = constrain(number, PERFORMANCE_NUM_MIN, PERFORMANCE_NUM_MAX);
  snprintf_P(filename, sizeof(filename), PSTR("/%s/%d/%s.json"), PERFORMANCE_CONFIG_PATH, number, CHAIN_CONFIG_NAME);
  return write_file_json(filename, data_json);
}

FLASHMEM bool load_sd_transpose_json(uint8_t number)
{
  if (number < 0) return false;

  number = constrain(number, PERFORMANCE_NUM_MIN, PERFORMANCE_NUM_MAX);
  snprintf_P(filename, sizeof(filename), PSTR("/%s/%d/%s.json"), PERFORMANCE_CONFIG_PATH, number, TRANSPOSE_CONFIG_NAME);
  StaticJsonDocument<JSON_BUFFER_SIZE> data_json;
  if(!read_file_json(filename, data_json)) return false;
  int total = sizeof(seq.chain_transpose);
  int columns = sizeof(seq.chain_transpose[0]);
  int rows = total / columns;
  int count = 0;
  for (uint8_t i = 0; i < rows; i++)
  {
    for (uint8_t j = 0; j < columns; j++)
    {
      seq.chain_transpose[i][j] = data_json["t"][count];
      count++;
    }
  }
  return true;
}

FLASHMEM bool save_sd_transpose_json(uint8_t number)
{
  int count = 0;
  int total = sizeof(seq.chain_transpose);
  int columns = sizeof(seq.chain_transpose[0]);
  int rows = total / columns;

  StaticJsonDocument<JSON_BUFFER_SIZE> data_json;
  for (uint8_t i = 0; i < rows; i++)
  {
    for (uint8_t j = 0; j < columns; j++)
    {
      data_json["t"][count] = seq.chain_transpose[i][j];
      count++;
    }
  }
  number = constrain(number, PERFORMANCE_NUM_MIN, PERFORMANCE_NUM_MAX);
  snprintf_P(filename, sizeof(filename), PSTR("/%s/%d/%s.json"), PERFORMANCE_CONFIG_PATH, number, TRANSPOSE_CONFIG_NAME);
  return write_file_json(filename, data_json);
}

FLASHMEM bool load_sd_song_json(uint8_t number)
{
  if (number < 0) return false;

  number = constrain(number, PERFORMANCE_NUM_MIN, PERFORMANCE_NUM_MAX);
  snprintf_P(filename, sizeof(filename), PSTR("/%s/%d/%s.json"), PERFORMANCE_CONFIG_PATH, number, SONG_CONFIG_NAME);

  StaticJsonDocument<JSON_BUFFER_SIZE> data_json;
  read_file_json(filename, data_json);
  int total = sizeof(seq.song);
  int columns = sizeof(seq.song[0]);
  int rows = total / columns;
  int count = 0;
  for (uint8_t i = 0; i < rows; i++)
  {
    for (uint8_t j = 0; j < columns; j++)
    {
      seq.song[i][j] = data_json["s"][count];
      count++;
    }
  }
  return true;
}

FLASHMEM bool save_sd_song_json(uint8_t number)
{
  int count = 0;
  int total = sizeof(seq.song);
  int columns = sizeof(seq.song[0]);
  int rows = total / columns;

  StaticJsonDocument<JSON_BUFFER_SIZE> data_json;
  for (uint8_t i = 0; i < rows; i++)
  {
    for (uint8_t j = 0; j < columns; j++)
    {
      data_json["s"][count] = seq.song[i][j];
      count++;
    }
  }
  number = constrain(number, PERFORMANCE_NUM_MIN, PERFORMANCE_NUM_MAX);
  snprintf_P(filename, sizeof(filename), PSTR("/%s/%d/%s.json"), PERFORMANCE_CONFIG_PATH, number, SONG_CONFIG_NAME);
  return write_file_json(filename, data_json);
}

FLASHMEM bool save_sd_seq_sub_vel_json(uint8_t number)
{
  number = constrain(number, PERFORMANCE_NUM_MIN, PERFORMANCE_NUM_MAX);
  int count = 0;
  int total = sizeof(seq.vel);
  int columns = sizeof(seq.vel[0]);
  int rows = total / columns;
  StaticJsonDocument<JSON_BUFFER_SIZE> data_json;
  for (uint8_t i = 0; i < rows; i++)
  {
    for (uint8_t j = 0; j < columns; j++)
    {
      data_json["seq_velocity"][count] = seq.vel[i][j];
      count++;
    }
  }
  snprintf_P(filename, sizeof(filename), PSTR("/%s/%d/%s.json"), PERFORMANCE_CONFIG_PATH, number, VELOCITY_CONFIG_NAME);
  write_file_json(filename, data_json);
  return (true);
}

FLASHMEM bool save_sd_seq_sub_patterns_json(uint8_t number)
{
  int count = 0;
  int total = sizeof(seq.note_data);
  int columns = sizeof(seq.note_data[0]);
  int rows = total / columns;

  StaticJsonDocument<JSON_BUFFER_SIZE> data_json;
  for (uint8_t i = 0; i < rows; i++)
  {
    for (uint8_t j = 0; j < columns; j++)
    {
      data_json["seq_data"][count] = seq.note_data[i][j];
      count++;
    }
  }
  snprintf_P(filename, sizeof(filename), PSTR("/%s/%d/%s.json"), PERFORMANCE_CONFIG_PATH, number, PATTERN_CONFIG_NAME);
  return write_file_json(filename, data_json);
}

FLASHMEM bool save_sd_performance_json(uint8_t number)
{
  bool seq_was_running = false;

  if (seq.running == true)
  {
    seq_was_running = true;
    handleStop();
  }
  dac_mute();

  AudioNoInterrupts();

  check_performance_directory(number);

  save_sd_seq_sub_vel_json(number);
  save_sd_seq_sub_patterns_json(number);
  save_sd_drummappings_json(number);
  save_sd_song_json(number);
  save_sd_transpose_json(number);
  save_sd_chain_json(number);
  save_sd_fx_json(number);
  save_sd_epiano_json(number);
  save_sd_multisample_presets_json(number);
  save_sd_braids_json(number);
  save_sd_multiband_json(number);
  save_sd_sidechain_json(number);

  for (uint8_t i = 0; i < MAX_DEXED; i++)
  {
    snprintf_P(filename, sizeof(filename), PSTR("/%s/%d/%s%d.json"), PERFORMANCE_CONFIG_PATH, number, VOICE_CONFIG_NAME, i);
    save_sd_microsynth_json(number, i);
    save_sd_voiceconfig_json(number, i);
  }


  StaticJsonDocument<JSON_BUFFER_SIZE> data_json;
  data_json["seq_tempo_ms"] = seq.tempo_ms;
  data_json["pattern_len_dec"] = seq.pattern_len_dec;
  data_json["swing_steps"] = seq.swing_steps;
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

  for (uint8_t i = 0; i < sizeof(seq.track_type); i++)
  {
    data_json["track_type"][i] = seq.track_type[i];
  }
  for (uint8_t i = 0; i < sizeof(seq.content_type); i++)
  {
    data_json["content_type"][i] = seq.content_type[i];
  }
  for (uint8_t i = 0; i < sizeof(seq.instrument); i++)
  {
    data_json["seq_inst_dexed"][i] = seq.instrument[i];
  }
  for (uint8_t i = 0; i < FILENAME_LEN; i++)
  {
    data_json["seq_name"][i] = seq.name[i];
  }
  for (uint8_t pat = 0; pat < NUM_SEQ_PATTERN; pat++)
  {
    data_json["chance"][pat] = seq.pat_chance[pat];
    data_json["vel_variation"][pat] = seq.pat_vel_variation[pat];
  }
  data_json["drum_midi_channel"] = drum_midi_channel;

  snprintf_P(filename, sizeof(filename), PSTR("/%s/%d/%s.json"), PERFORMANCE_CONFIG_PATH, number, SEQUENCER_CONFIG_NAME);
  bool ret=write_file_json(filename, data_json);
  dac_unmute();
  if (ret && seq_was_running == true)
    handleStart();
  return ret;
}

FLASHMEM bool check_performance_directory(uint8_t number)
{
  char dir[CONFIG_FILENAME_LEN];

  if (!sd_card) 
  {
    LOG.println(F("E: SD card not available"));
    return false;
  }
  snprintf_P(dir, sizeof(dir), PSTR("/%s/%d"), PERFORMANCE_CONFIG_PATH, number);

  AudioNoInterrupts();
  bool ret=true;
  if (!SD.exists(dir))
  {
    ret=SD.mkdir(dir);
#ifdef DEBUG
    if (ret)
    {
      LOG.print(F("Creating directory ")); LOG.println(dir);
    }
    else
    {
      LOG.print(F("E: Cannot create ")); LOG.println(dir);
    }
#endif
  }
  AudioInterrupts();
  return ret;
}

FLASHMEM void get_sd_performance_name_json(uint8_t number)
{
  memset(seq.name_temp, 0, FILENAME_LEN);
  if (!sd_card) return;

  number = constrain(number, PERFORMANCE_NUM_MIN, PERFORMANCE_NUM_MAX);
  snprintf_P(filename, sizeof(filename), PSTR("/%s/%d/%s.json"), PERFORMANCE_CONFIG_PATH, number, SEQUENCER_CONFIG_NAME);


  StaticJsonDocument<JSON_BUFFER_SIZE> data_json;
  if(!read_file_json(filename, data_json)) return;
  if (data_json["seq_name"][0] != 0)
  {
    for (uint8_t i = 0; i < FILENAME_LEN; i++)
    {
      seq.name_temp[i] = data_json["seq_name"][i];
    }
#ifdef DEBUG
    LOG.printf("Get performance name for %d : %s\n", number, seq.name_temp);
#endif
  }
}

FLASHMEM bool load_sd_seq_sub_vel_json(uint8_t number)
{
  if (number < 0) return false;
  number = constrain(number, PERFORMANCE_NUM_MIN, PERFORMANCE_NUM_MAX);

  StaticJsonDocument<JSON_BUFFER_SIZE> data_json;
  snprintf_P(filename, sizeof(filename), PSTR("/%s/%d/%s.json"), PERFORMANCE_CONFIG_PATH, number, VELOCITY_CONFIG_NAME);
  if(!read_file_json(filename,data_json)) return false;

  int total = sizeof(seq.vel);
  int columns = sizeof(seq.vel[0]);
  int rows = total / columns;
  int count = 0;
  for (uint8_t i = 0; i < rows; i++)
  {
    for (uint8_t j = 0; j < columns; j++)
    {
      seq.vel[i][j] = data_json["seq_velocity"][count];
      count++;
    }
  }
  return true;
}

FLASHMEM bool load_sd_seq_sub_patterns_json(uint8_t number)
{
  if (number < 0) return false;
  number = constrain(number, PERFORMANCE_NUM_MIN, PERFORMANCE_NUM_MAX);

  StaticJsonDocument<JSON_BUFFER_SIZE> data_json;
  snprintf_P(filename, sizeof(filename), PSTR("/%s/%d/%s.json"), PERFORMANCE_CONFIG_PATH, number, PATTERN_CONFIG_NAME);
  read_file_json(filename, data_json);

  int total = sizeof(seq.note_data);
  int columns = sizeof(seq.note_data[0]);
  int rows = total / columns;
  int count = 0;

  for (uint8_t i = 0; i < rows; i++)
  {
    for (uint8_t j = 0; j < columns; j++)
    {
      seq.note_data[i][j] = data_json["seq_data"][count];
      count++;
    }
  }
  return true;
}

FLASHMEM bool load_sd_performance_json(uint8_t number)
{
  bool seq_was_running = false;
  if (seq.running)
  {
    seq_was_running = true;
    handleStop();
  }
  dac_mute();
  handleStop();
  number = constrain(number, PERFORMANCE_NUM_MIN, PERFORMANCE_NUM_MAX);
  AudioNoInterrupts();
  load_sd_seq_sub_patterns_json(number);
  load_sd_seq_sub_vel_json(number);
  load_sd_epiano_json(number);
  load_sd_drummappings_json(number);
  load_sd_song_json(number);
  load_sd_transpose_json(number);
  load_sd_chain_json(number);
  load_sd_multisample_presets_json(number);
  load_sd_braids_json(number);
  load_sd_multiband_json(number);
  load_sd_sidechain_json(number);
  configuration.sys.performance_number = number;

  snprintf_P(filename, sizeof(filename), PSTR("/%s/%d/%s.json"), PERFORMANCE_CONFIG_PATH, number, SEQUENCER_CONFIG_NAME);
  StaticJsonDocument<JSON_BUFFER_SIZE> data_json;
  if(!read_file_json(filename, data_json)) return false;
  for (uint8_t i = 0; i < sizeof(seq.track_type); i++)
  {
    seq.track_type[i] = data_json["track_type"][i];
  }
  for (uint8_t i = 0; i < sizeof(seq.content_type); i++)
  {
    seq.content_type[i] = data_json["content_type"][i];
  }
  for (uint8_t i = 0; i < sizeof(seq.instrument); i++)
  {
    seq.instrument[i] = data_json["seq_inst_dexed"][i];
  }

  if (data_json["seq_name"][0] != 0)
  {
    for (uint8_t i = 0; i < FILENAME_LEN; i++)
    {
      seq.name[i] = data_json["seq_name"][i];
    }
  }

  for (uint8_t pat = 0; pat < NUM_SEQ_PATTERN; pat++)
  {
    if (data_json["chance"][pat] > 0)
    {
      seq.pat_chance[pat] = data_json["chance"][pat];
      seq.pat_vel_variation[pat] = data_json["vel_variation"][pat];
    }
    else
    {
      seq.pat_chance[pat] = 100;
      seq.pat_vel_variation[pat] = 0;
    }
  }

  seq.tempo_ms = data_json["seq_tempo_ms"];
  seq.bpm = data_json["seq_bpm"];
  seq.pattern_len_dec = data_json["pattern_len_dec"];
  seq.swing_steps = data_json["swing_steps"];
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

  if (data_json["drum_midi_channel"] > 0) //do not set to onmi when it was never saved before. Better to use the default channel in this case.
    drum_midi_channel = data_json["drum_midi_channel"];

  for (uint8_t instance_id = 0; instance_id < NUM_DEXED; instance_id++)
  {
    load_sd_microsynth_json(number, instance_id);
    load_sd_voiceconfig_json(number, instance_id);
    load_sd_voice(configuration.dexed[instance_id].pool, configuration.dexed[instance_id].bank, configuration.dexed[instance_id].voice, instance_id);
    MicroDexed[instance_id]->setGain(midi_volume_transform(map(configuration.dexed[instance_id].sound_intensity, SOUND_INTENSITY_MIN, SOUND_INTENSITY_MAX, 0, 127)));
    MicroDexed[instance_id]->panic();
  }

  load_sd_fx_json(configuration.sys.performance_number); //loaded here since bpm must be loaded first
  dac_unmute();
  if (seq.euclidean_active)
    update_euclidean();
  for (uint8_t d = 0; d < NUM_SEQ_TRACKS; d++)
    seq.chain_counter[d] = 0;

  if (seq_was_running)
    handleStart();
  else
    sequencer_timer.begin(sequencer, seq.tempo_ms / 8, false);

  return true;
}

FLASHMEM bool check_sd_performance_exists(uint8_t number)
{
  char filename[CONFIG_FILENAME_LEN];
  snprintf_P(filename, sizeof(filename), PSTR("/%s/%d/%s.json"), PERFORMANCE_CONFIG_PATH, number, SEQUENCER_CONFIG_NAME);

  return SD.exists(filename);
}

/******************************************************************************
   HELPER FUNCTIONS
 ******************************************************************************/

// TODO is this still feasable? The bank-directories now reside in a pool-directory..
// so this seems harmful
FLASHMEM bool get_bank_name(uint8_t b, char* bank_name)
{
#ifdef DEBUG
  LOG.printf_P(PSTR("get bank name for bank [%d]\n"), b);
#endif

  strcpy(bank_name, sError);

  char bankdir[FILENAME_LEN];
  snprintf_P(bankdir, sizeof(bankdir), PSTR("/%s/%d"), DEXED_CONFIG_PATH, b);

  File entry = find_first_file(bankdir);
  if(!entry) return false;
  
  strip_extension(entry.name(), bank_name, BANK_NAME_LEN);
  string_toupper(bank_name);
  close_file(entry);
  return true;
}

// TODO is this still feasable? The bank-directories now reside in a pool-directory..
// so this seems harmful
FLASHMEM bool get_voice_name(uint8_t b, uint8_t v, char* voice_name)
{
  char bankdir[FILENAME_LEN];

  strcpy(voice_name, sError);
  snprintf_P(bankdir, sizeof(bankdir), PSTR("/%s/%d"), DEXED_CONFIG_PATH, b);
  File entry = find_first_file(bankdir);
  if(!entry || !check_syx(entry)) return false;

  memset(voice_name, 0, VOICE_NAME_LEN);
  entry.seek(124 + (v * 128));
  entry.read(voice_name, min(VOICE_NAME_LEN, 10));
  string_toupper(voice_name);
  close_file(entry);

  return true;
}

FLASHMEM bool save_sd_multisample_presets_json(uint8_t number)
{
  char zone_filename[CONFIG_FILENAME_LEN];
  StaticJsonDocument<JSON_BUFFER_SIZE> data_json;

  for (uint8_t i = 0; i < NUM_MULTISAMPLES; i++)
  {
    data_json[i]["name"] = msp[i].name;
    data_json[i]["sound_intensity"] = msp[i].sound_intensity;
    data_json[i]["midi_channel"] = msp[i].midi_channel;

    for (uint8_t j = 0; j < NUM_MULTISAMPLE_ZONES; j++)
    {
      strcpy(zone_filename, msz[i][j].filename);
      if (strchr(zone_filename, '.'))
      {
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
      data_json[i]["zones"]["tune"][j] = msz[i][j].tune;
      data_json[i]["zones"]["loop_type"][j] = msz[i][j].loop_type;
      data_json[i]["zones"]["loop_start"][j] = msz[i][j].loop_start;
      data_json[i]["zones"]["loop_end"][j] = msz[i][j].loop_end;
    }
  }

  snprintf_P(filename, sizeof(filename), PSTR("/%s/%d/%s.json"), PERFORMANCE_CONFIG_PATH, number, MULTISAMPLE_PRESETS_CONFIG_NAME);
  return write_file_json(filename, data_json);
}

FLASHMEM bool load_sd_multisample_presets_json(uint8_t number)
{
  if (number < 0) return false;

  number = constrain(number, PERFORMANCE_NUM_MIN, PERFORMANCE_NUM_MAX);
  snprintf_P(filename, sizeof(filename), PSTR("/%s/%d/%s.json"), PERFORMANCE_CONFIG_PATH, number, MULTISAMPLE_PRESETS_CONFIG_NAME);

  json = open_file_for_read(filename);
  if (!json) return false;
  // StaticJsonDocument<JSON_BUFFER_SIZE> data_json;
  DynamicJsonDocument data_json(JSON_BUFFER_SIZE);
  deserializeJson(data_json, json);
  close_file(json);

#if defined(DEBUG) && defined(DEBUG_SHOW_JSON)
  LOG.println(F("Read JSON data:"));
  serializeJsonPretty(data_json, Serial);
  LOG.println();
#endif

  for (uint8_t i = 0; i < NUM_MULTISAMPLES; i++)
  {
    strcpy(msp[i].name, data_json[i]["name"]);
    msp[i].sound_intensity = data_json[i]["sound_intensity"];
    msp[i].midi_channel = data_json[i]["midi_channel"];

    for (uint8_t j = 0; j < NUM_MULTISAMPLE_ZONES; j++)
    {
      strcpy(msz[i][j].filename, data_json[i]["zones"]["filename"][j]);
      if (strlen(msz[i][j].filename) > 0)
        strcat(msz[i][j].filename, ".wav");
      msz[i][j].rootnote = data_json[i]["zones"]["root"][j];
      msz[i][j].low = data_json[i]["zones"]["low"][j];
      msz[i][j].high = data_json[i]["zones"]["high"][j];
      msz[i][j].playmode = data_json[i]["zones"]["playmode"][j];
      msz[i][j].vol = data_json[i]["zones"]["vol"][j];
      msz[i][j].pan = data_json[i]["zones"]["pan"][j];
      msz[i][j].rev = data_json[i]["zones"]["rev"][j];
      msz[i][j].tune = data_json[i]["zones"]["tune"][j];
      msz[i][j].loop_type = data_json[i]["zones"]["loop_type"][j];
      msz[i][j].loop_start = data_json[i]["zones"]["loop_start"][j];
      msz[i][j].loop_end = data_json[i]["zones"]["loop_end"][j];

      if (msz[i][j].tune == 0)
        msz[i][j].tune = 100;
    }
  }
  return true;
}

int compare_files_by_name(const void* a, const void* b)
{
  storage_file_t* fileA = (storage_file_t*)a;
  storage_file_t* fileB = (storage_file_t*)b;

  String strA = ((String)fileA->name).toLowerCase();
  String strB = ((String)fileB->name).toLowerCase();

  if (strA.length() == 1)
  {
    strA = "0" + strA;
  }
  if (strB.length() == 1)
  {
    strB = "0" + strB;
  }

  return strA < strB ? -1 : strA > strB ? 1
    : 0;
}

FLASHMEM void load_sd_directory()
{
  strcpy(fm.sd_prev_dir, fm.sd_new_name);
  File sd_root = SD.open(fm.sd_new_name);
  fm.sd_sum_files = 0;
  while (true)
  {
    File sd_entry = sd_root.openNextFile();
    if (!sd_entry)
      break;

    // skip special files
    if (strcmp(sd_entry.name(), "System Volume Information") && strstr(sd_entry.name(), "._") == NULL)
    {
      strcpy(sdcard_infos.files[fm.sd_sum_files].name, sd_entry.name());
      sdcard_infos.files[fm.sd_sum_files].size = sd_entry.size();
      sdcard_infos.files[fm.sd_sum_files].isDirectory = sd_entry.isDirectory();
#ifdef DEBUG
      LOG.print(fm.sd_sum_files);
      LOG.print(F("  "));
      LOG.print(sdcard_infos.files[fm.sd_sum_files].name);
      LOG.print(F("  "));
      LOG.print(sdcard_infos.files[fm.sd_sum_files].size);
      LOG.print(F(" bytes"));
      LOG.println();
#endif

      fm.sd_sum_files++;
    }
    sd_entry.close();
  }
  sd_root.close();

  // clear all the unused files in array
  for (uint8_t i = fm.sd_sum_files; i < MAX_FILES; i++)
  {
    strcpy(sdcard_infos.files[i].name, "");
    sdcard_infos.files[i].size = 0;
    sdcard_infos.files[i].isDirectory = false;
  }

  qsort(sdcard_infos.files, fm.sd_sum_files, sizeof(storage_file_t), compare_files_by_name);
}
