/*
   MicroDexed-touch

   MicroDexed is a port of the Dexed sound engine
   Dexed ist heavily based on https://github.com/google/music-synthesizer-for-android

   (c)2018-2021 H. Wirtz <wirtz@parasitstudio.de>
   (c)2021      H. Wirtz <wirtz@parasitstudio.de>, M. Koslowski <positionhigh@gmx.de>

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 3 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program; if not, write to the Free Software Foundation,
   Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301  USA
*/

#include <limits.h>
#include "config.h"
#include "scope.h"
#include <Audio.h>
#include <Wire.h>
#include <MIDI.h>
#include <SD.h>
#include <SPI.h>
#include <flashloader.h>

#include "ILI9341_t3n.h"

#if defined GENERIC_DISPLAY
#include "XPT2046_Touchscreen.h"
#endif

#ifdef CAPACITIVE_TOUCH_DISPLAY
#include "Adafruit_FT6206.h"
#endif

#if defined(COMPILE_FOR_PROGMEM) || defined(COMPILE_FOR_PSRAM)
#include <TeensyVariablePlayback.h>
#endif

#ifdef COMPILE_FOR_SDCARD
#include <TeensyVariablePlayback.h>
#endif

#ifdef COMPILE_FOR_FLASH
#include "TeensyVariablePlaybackFlash.h"
#endif

// #ifdef COMPILE_FOR_FLASH
// SerialFlashChip         myfs;
// #endif

extern "C" uint8_t external_psram_size;
uint32_t psram_free_bytes = 0;

#ifdef PSRAM
#include "effect_delay_ext8.h"
#endif

#include <synth_braids.h>
// Allocate the delay lines for left and right channels
short braids_l_delayline[BRAIDS_FLANGE_DELAY_LENGTH];
short braids_r_delayline[BRAIDS_FLANGE_DELAY_LENGTH];
int braids_flanger_idx = BRAIDS_FLANGE_DELAY_LENGTH / 4;
int braids_flanger_depth = BRAIDS_FLANGE_DELAY_LENGTH / 4;
double braids_flanger_freq = .5;

#include <TeensyTimerTool.h>
using namespace TeensyTimerTool;

#include "microsynth.h"
#include "braids.h"

#include "effect_dynamics.h"

#include "midi_devices.hpp"
#include "synth_dexed.h"
#include "dexed_sd.h"
#include "effect_modulated_delay.h"
#include "effect_stereo_mono.h"
#include "effect_mono_stereo.h"
#include "effect_platervbstereo.h"
#include "template_mixer.hpp"
#include "UI.hpp"
#include "drums.h"
//#include "drumset.h"
#include "synth_mda_epiano.h"
#include "effect_stereo_panorama.h"

#include "livesequencer.h"
#include <vector>

elapsedMillis midi_start_delay;

uint8_t check_sd_cards(void);
void check_and_create_directories(void);
void show_cpu_and_mem_usage(void);
void initial_values(bool init);

void loadSample(newdigate::flashloader& loader, uint8_t i, const char* path);
void printLoadedSample(const uint8_t i, const char* name);

#ifdef DEBUG
void show_configuration(void);
void show_cpu_and_mem_usage(void);
void show_patch(uint8_t);
void SerialPrintFormatInt3(uint8_t num);
int FreeMem(void);
#endif

bool checkMidiChannelDexed(byte inChannel, uint8_t instance_id);
bool bootup_performance_loading = true;
bool remote_active = false;

ILI9341_t3n display = ILI9341_t3n(TFT_CS, TFT_DC, TFT_RST, TFT_MOSI, TFT_SCK, TFT_MISO);

#if defined GENERIC_DISPLAY 
XPT2046_Touchscreen touch(TFT_TOUCH_CS);
#endif

#ifdef CAPACITIVE_TOUCH_DISPLAY
Adafruit_FT6206 touch = Adafruit_FT6206();
#endif

// Editor operations
int menuhelper_previous_val;
bool menuhelper_redraw = false;
int sub(int A, int B, int min, int max)
{
  if (A != constrain(A - B, min, max))
    menuhelper_redraw = true;
  return constrain(A - B, min, max);
};

int sum(int A, int B, int min, int max)
{
  if (A != constrain(A + B, min, max))
    menuhelper_redraw = true;
  return constrain(A + B, min, max);
};

int (*calc_val[2])(int, int, int, int);

Realtime_Scope scope;

// Audio engines
AudioSynthDexed* MicroDexed[NUM_DEXED];
AudioSynthEPiano ep(NUM_EPIANO_VOICES);
AudioAnalyzePeak ep_peak_r;
AudioAnalyzePeak ep_peak_l;

//bitcrusher
// AudioEffectBitcrusher bitcrusher_r;
// AudioEffectBitcrusher bitcrusher_l;

AudioSynthBraids* synthBraids[NUM_BRAIDS];
AudioMixer<NUM_BRAIDS> braids_mixer;
AudioMixer<4>* braids_mixer_filter[NUM_BRAIDS];
AudioMixer<2> braids_mixer_reverb;
AudioEffectEnvelope* braids_envelope[NUM_BRAIDS];
AudioFilterBiquad* braids_filter[NUM_BRAIDS];
AudioEffectFlange braids_flanger_r;
AudioEffectFlange braids_flanger_l;
AudioEffectStereoPanorama braids_stereo_panorama;
AudioAnalyzePeak braids_peak_r;
AudioAnalyzePeak braids_peak_l;
uint8_t braids_slot;
braids_t braids_osc;
uint16_t braids_filter_state[NUM_BRAIDS];
uint16_t braids_filter_state_last_displayed[NUM_BRAIDS];
extern uint16_t braids_filter_lfo_count[NUM_BRAIDS];
extern bool braids_lfo_direction[NUM_BRAIDS];
extern elapsedMillis braids_control_rate;

AudioSynthWaveform microsynth_waveform[NUM_MICROSYNTH];
AudioSynthNoisePink microsynth_noise[NUM_MICROSYNTH];
AudioEffectEnvelope microsynth_envelope_osc[NUM_MICROSYNTH];
AudioEffectEnvelope microsynth_envelope_noise[NUM_MICROSYNTH];
AudioFilterStateVariable microsynth_filter_osc[NUM_MICROSYNTH];
AudioFilterStateVariable microsynth_filter_noise[NUM_MICROSYNTH];
AudioEffectStereoPanorama microsynth_stereo_panorama_osc[NUM_MICROSYNTH];
AudioEffectStereoPanorama microsynth_stereo_panorama_noise[NUM_MICROSYNTH];
AudioMixer<2> microsynth_mixer_r[NUM_MICROSYNTH];
AudioMixer<2> microsynth_mixer_l[NUM_MICROSYNTH];
AudioMixer<2> microsynth_main_mixer_r;
AudioMixer<2> microsynth_main_mixer_l;
AudioMixer<4> microsynth_mixer_filter_osc[NUM_MICROSYNTH];
AudioMixer<4> microsynth_mixer_filter_noise[NUM_MICROSYNTH];
AudioMixer<2> microsynth_mixer_reverb;
AudioAnalyzePeak microsynth_peak_osc_0;
AudioAnalyzePeak microsynth_peak_osc_1;
AudioAnalyzePeak microsynth_peak_noise_0;
AudioAnalyzePeak microsynth_peak_noise_1;
extern elapsedMillis microsynth_delay_timer[2];

AudioSynthWaveform* chorus_modulator[NUM_DEXED];

AudioEffectModulatedDelay* dexed_chorus[NUM_DEXED];

AudioMixer<8 + NUM_DRUMS>* global_delay_in_mixer[NUM_DEXED];
AudioMixer<2>* delay_fb_mixer[NUM_DEXED];

AudioFilterStateVariable global_delay_filter[NUM_DEXED];
AudioMixer<4> global_delay_filter_mixer[NUM_DEXED];

#ifdef PSRAM
AudioEffectDelayExternal8* delay_fx[NUM_DEXED];
#else
AudioEffectDelay* delay_fx[NUM_DEXED];
#endif

AudioMixer<2>* delay_mixer[NUM_DEXED];
AudioEffectMonoStereo* dexed_mono2stereo[NUM_DEXED];
AudioEffectMonoStereo* dexed_dry_mono2stereo[NUM_DEXED];
AudioEffectMonoStereo* delay_mono2stereo[NUM_DEXED];

AudioMixer<2> dexed_chorus_mixer_r[NUM_DEXED];
AudioMixer<2> dexed_chorus_mixer_l[NUM_DEXED];

AudioEffectStereoPanorama ep_stereo_panorama;
AudioSynthWaveform ep_chorus_modulator;
AudioFilterBiquad ep_modchorus_filter;
AudioEffectModulatedDelayStereo ep_modchorus;
AudioMixer<2> ep_chorus_mixer_r;
AudioMixer<2> ep_chorus_mixer_l;

AudioAnalyzePeak microdexed_peak_0;
AudioAnalyzePeak microdexed_peak_1;

AudioMixer<8> reverb_mixer_r;
AudioMixer<8> reverb_mixer_l;

AudioEffectPlateReverb reverb;

AudioMixer<11> master_mixer_r;
AudioMixer<11> master_mixer_l;

AudioAmplifier volume_r;
AudioAmplifier volume_l;
AudioEffectStereoMono stereo2mono;
AudioAnalyzePeak master_peak_r;
AudioAnalyzePeak master_peak_l;
#if defined(TEENSY_AUDIO_BOARD) && defined(SGTL5000_AUDIO_THRU)
AudioMixer<2> audio_thru_mixer_r;
AudioMixer<2> audio_thru_mixer_l;
#endif

AudioMixer<2> finalized_mixer_r;
AudioMixer<2> finalized_mixer_l;

AudioPlaySdWav WAV_preview_SD;

#ifdef COMPILE_FOR_FLASH
AudioPlaySerialflashRaw WAV_preview_FLASH;
#endif

#ifdef COMPILE_FOR_PSRAM
AudioPlayArrayResmp WAV_preview_PSRAM;
#endif

// Drumset
#if NUM_DRUMS > 0

#ifdef COMPILE_FOR_FLASH
AudioPlayFlashResmp* Drum[NUM_DRUMS];
// AudioPlaySerialflashRaw*        Drum[NUM_DRUMS];  // playflash from normal audio library (no pitchshift)
#endif

#ifdef COMPILE_FOR_SDCARD
AudioPlaySdResmp* Drum[NUM_DRUMS];
#endif

#if defined(COMPILE_FOR_PROGMEM) || defined(COMPILE_FOR_PSRAM)
AudioPlayArrayResmp* Drum[NUM_DRUMS];
#include "effect_delay_ext8.h"
#endif

AudioFilterBiquad* Drum_filter[NUM_DRUMS];

// test with envelopes for samples
//AudioEffectEnvelope* sample_envelope[NUM_DRUMS];

AudioMixer<NUM_DRUMS> drum_mixer_r;
AudioMixer<NUM_DRUMS> drum_mixer_l;

AudioAnalyzePeak drum_mixer_peak_r;
AudioAnalyzePeak drum_mixer_peak_l;

AudioMixer<NUM_DRUMS> drum_reverb_send_mixer_r;
AudioMixer<NUM_DRUMS> drum_reverb_send_mixer_l;
#endif

AudioAnalyzePeak reverb_return_peak_r;
AudioAnalyzePeak reverb_return_peak_l;


//SPDIF
//AsyncAudioInputSPDIF3 spdif_in;
//AudioInputSPDIF3 spdif_in;

// Outputs

AudioOutputSPDIF3 spdif3; // enable S/PDIF by default

#if defined(TEENSY_AUDIO_BOARD)
AudioOutputI2S i2s1;
AudioControlSGTL5000 sgtl5000;
#elif defined(I2S_AUDIO_ONLY)
AudioOutputI2S i2s1;
#elif defined(TGA_AUDIO_BOARD)
AudioOutputI2S i2s1;
AudioControlWM8731master wm8731_1;
#elif defined(PT8211_AUDIO)
AudioOutputPT8211 pt8211_1;
#elif defined(TEENSY_DAC_SYMMETRIC)
AudioOutputAnalogStereo dacOut;
AudioMixer<4> invMixer;
#elif defined(TEENSY_DAC)
AudioOutputAnalogStereo dacOut;
#endif

#ifdef AUDIO_DEVICE_USB
AudioOutputUSB usb1;
#endif

#if defined(TEENSY_AUDIO_BOARD) && defined(SGTL5000_AUDIO_THRU)
AudioInputI2S i2s1in;
#endif

AudioFilterBiquad mb_filter_l_0;
AudioFilterBiquad mb_filter_l_1;
AudioFilterBiquad mb_filter_l_2;
AudioFilterBiquad mb_filter_l_3;
AudioFilterBiquad mb_filter_r_0;
AudioFilterBiquad mb_filter_r_1;
AudioFilterBiquad mb_filter_r_2;
AudioFilterBiquad mb_filter_r_3;

AudioEffectDynamics mb_compressor_l_0;
AudioEffectDynamics mb_compressor_l_1;
AudioEffectDynamics mb_compressor_l_2;
AudioEffectDynamics mb_compressor_l_3;

AudioEffectDynamics mb_compressor_r_0;
AudioEffectDynamics mb_compressor_r_1;
AudioEffectDynamics mb_compressor_r_2;
AudioEffectDynamics mb_compressor_r_3;

AudioMixer<4> mb_mixer_l;
AudioMixer<4> mb_mixer_r;

AudioConnection patchCord3(mb_filter_l_0, 0, mb_compressor_l_0, 0);
AudioConnection patchCord4(mb_filter_l_1, 0, mb_compressor_l_1, 0);
AudioConnection patchCord5(mb_filter_l_2, 0, mb_compressor_l_2, 0);
AudioConnection patchCord6(mb_filter_l_3, 0, mb_compressor_l_3, 0);

AudioConnection patchCord7(mb_filter_r_0, 0, mb_compressor_r_0, 0);
AudioConnection patchCord8(mb_filter_r_1, 0, mb_compressor_r_1, 0);
AudioConnection patchCord9(mb_filter_r_2, 0, mb_compressor_r_2, 0);
AudioConnection patchCord10(mb_filter_r_3, 0, mb_compressor_r_3, 0);

AudioConnection patchCord11(mb_compressor_l_0, 0, mb_mixer_l, 0);
AudioConnection patchCord12(mb_compressor_l_1, 0, mb_mixer_l, 1);
AudioConnection patchCord13(mb_compressor_l_2, 0, mb_mixer_l, 2);
AudioConnection patchCord14(mb_compressor_l_3, 0, mb_mixer_l, 3);

AudioConnection patchCord15(mb_compressor_r_0, 0, mb_mixer_r, 0);
AudioConnection patchCord16(mb_compressor_r_1, 0, mb_mixer_r, 1);
AudioConnection patchCord17(mb_compressor_r_2, 0, mb_mixer_r, 2);
AudioConnection patchCord18(mb_compressor_r_3, 0, mb_mixer_r, 3);

AudioAnalyzePeak mb_before_l;
AudioAnalyzePeak mb_before_r;
AudioAnalyzePeak mb_after_l;
AudioAnalyzePeak mb_after_r;

// WAV/AUDIO Recording
AudioRecordQueue record_queue_l;
AudioRecordQueue record_queue_r;
File frec;

//
// Static patching of audio objects
//
AudioConnection patchCord[] = {
  // Audio chain tail
  {reverb_mixer_r, 0, reverb, 0},
  {reverb_mixer_l, 0, reverb, 1},
  {reverb, 0, master_mixer_r, MASTER_MIX_CH_REVERB},
  {reverb, 1, master_mixer_l, MASTER_MIX_CH_REVERB},
  {master_mixer_l, volume_l},
  {master_mixer_r, volume_r},
  {volume_l, 0, stereo2mono, 1},
  {volume_r, 0, stereo2mono, 0},

  {master_mixer_l, 0, master_peak_l, 0},
  {master_mixer_r, 0, master_peak_r, 0},

  {master_mixer_l, 0, mb_before_l, 0},
  {master_mixer_r, 0, mb_before_r, 0},

  {mb_mixer_l, 0, mb_after_l, 0},
  {mb_mixer_r, 0, mb_after_r, 0},

  // Realtime Scope
  {master_mixer_l, 0, scope, 0},

  // Outputs
  #if defined(TEENSY_AUDIO_BOARD)
  #ifndef SGTL5000_AUDIO_THRU

      {stereo2mono, 0, finalized_mixer_l, 0},
      {stereo2mono, 1, finalized_mixer_r, 0},

      {master_mixer_l, 0, mb_filter_l_0, 0},
      {master_mixer_r, 0, mb_filter_r_0, 0},

      {master_mixer_l, 0, mb_filter_l_1, 0},
      {master_mixer_r, 0, mb_filter_r_1, 0},

      {master_mixer_l, 0, mb_filter_l_2, 0},
      {master_mixer_r, 0, mb_filter_r_2, 0},

      {master_mixer_l, 0, mb_filter_l_3, 0},
      {master_mixer_r, 0, mb_filter_r_3, 0},

      {mb_mixer_l, 0, finalized_mixer_l, 1},
      {mb_mixer_r, 0, finalized_mixer_r, 1},

      {finalized_mixer_l, 0, i2s1, 0},
      {finalized_mixer_r, 0, i2s1, 1},

  #endif
  #elif defined(I2S_AUDIO_ONLY)

      {stereo2mono, 0, finalized_mixer_l, 0},
      {stereo2mono, 1, finalized_mixer_r, 0},

      {master_mixer_l, 0, mb_filter_l_0, 0},
      {master_mixer_r, 0, mb_filter_r_0, 0},

      {master_mixer_l, 0, mb_filter_l_1, 0},
      {master_mixer_r, 0, mb_filter_r_1, 0},

      {master_mixer_l, 0, mb_filter_l_2, 0},
      {master_mixer_r, 0, mb_filter_r_2, 0},

      {master_mixer_l, 0, mb_filter_l_3, 0},
      {master_mixer_r, 0, mb_filter_r_3, 0},

      {mb_mixer_l, 0, finalized_mixer_l, 1},
      {mb_mixer_r, 0, finalized_mixer_r, 1},


      {finalized_mixer_l, 0, i2s1, 0},
      {finalized_mixer_r, 0, i2s1, 1},

      //  {finalized_mixer_l, 0, bitcrusher_l, 0},
      //  {finalized_mixer_r, 0, bitcrusher_r, 0},

  //bitcrusher master
    // {bitcrusher_l, 0, i2s1, 0},
    // {bitcrusher_r, 0, i2s1, 1},

        // WAV RECORDER
        {finalized_mixer_l, 0, record_queue_l, 0},
        {finalized_mixer_r, 0, record_queue_r, 0},


    #endif


    #if defined(TEENSY_AUDIO_BOARD) && defined(SGTL5000_AUDIO_THRU)
        {stereo2mono, 0, audio_thru_mixer_r, 0},
        {stereo2mono, 1, audio_thru_mixer_l, 0},
        {audio_thru_mixer_r, 0, i2s1, 0},
        {audio_thru_mixer_l, 0, i2s1, 1},
    #endif

        {drum_reverb_send_mixer_r, 0, reverb_mixer_r, REVERB_MIX_CH_DRUMS},
        {drum_reverb_send_mixer_l, 0, reverb_mixer_l, REVERB_MIX_CH_DRUMS},
        {drum_mixer_r, 0, master_mixer_r, MASTER_MIX_CH_DRUMS},
        {drum_mixer_l, 0, master_mixer_l, MASTER_MIX_CH_DRUMS},
        {drum_mixer_r, 0, drum_mixer_peak_r, 0},
        {drum_mixer_l, 0, drum_mixer_peak_l, 0},

        {reverb, 0, reverb_return_peak_r, 0},
        {reverb, 1, reverb_return_peak_l, 0},

        {ep, 0, ep_stereo_panorama, 0},
        {ep, 1, ep_stereo_panorama, 1},
        {ep_stereo_panorama, 0, ep_peak_r, 0},
        {ep_stereo_panorama, 1, ep_peak_l, 0},
        {ep_stereo_panorama, 0, ep_chorus_mixer_r, 0},
        {ep_stereo_panorama, 1, ep_chorus_mixer_l, 0},
        {ep_stereo_panorama, 0, ep_modchorus, 0},
        {ep_stereo_panorama, 1, ep_modchorus, 1},
        {ep_chorus_modulator, 0, ep_modchorus_filter, 0},
        {ep_modchorus_filter, 0, ep_modchorus, 2},
        {ep_modchorus, 0, ep_chorus_mixer_r, 1},
        {ep_modchorus, 1, ep_chorus_mixer_l, 1},
        {ep_chorus_mixer_r, 0, reverb_mixer_r, REVERB_MIX_CH_EPIANO},
        {ep_chorus_mixer_l, 0, reverb_mixer_l, REVERB_MIX_CH_EPIANO},
        {ep_chorus_mixer_r, 0, master_mixer_r, MASTER_MIX_CH_EPIANO},
        {ep_chorus_mixer_l, 0, master_mixer_l, MASTER_MIX_CH_EPIANO},

        {microsynth_noise[0], 0, microsynth_envelope_noise[0], 0}, // noise generator to envelope
        {microsynth_noise[1], 0, microsynth_envelope_noise[1], 0},

        {microsynth_envelope_noise[0], 0, microsynth_filter_noise[0], 0}, // noise envelope to filter
        {microsynth_envelope_noise[1], 0, microsynth_filter_noise[1], 0},

        {microsynth_filter_noise[0], 0, microsynth_mixer_filter_noise[0], 0}, // noise filter to mixer
        {microsynth_filter_noise[0], 1, microsynth_mixer_filter_noise[0], 1},
        {microsynth_filter_noise[0], 2, microsynth_mixer_filter_noise[0], 2},

        {microsynth_filter_noise[1], 0, microsynth_mixer_filter_noise[1], 0},
        {microsynth_filter_noise[1], 1, microsynth_mixer_filter_noise[1], 1},
        {microsynth_filter_noise[1], 2, microsynth_mixer_filter_noise[1], 2},

        {microsynth_envelope_noise[0], 0, microsynth_mixer_filter_noise[0], 3}, // unfiltered noise to mixer
        {microsynth_envelope_noise[1], 0, microsynth_mixer_filter_noise[1], 3},

        {microsynth_mixer_filter_noise[0], 0, microsynth_stereo_panorama_noise[0], 0}, // noise to panorama 0
        {microsynth_mixer_filter_noise[0], 0, microsynth_stereo_panorama_noise[0], 1},

        {microsynth_mixer_filter_noise[1], 0, microsynth_stereo_panorama_noise[1], 0}, // noise to panorama 1
        {microsynth_mixer_filter_noise[1], 0, microsynth_stereo_panorama_noise[1], 1},

        {microsynth_waveform[0], 0, microsynth_envelope_osc[0], 0}, // osc waveform to envelope
        {microsynth_waveform[1], 0, microsynth_envelope_osc[1], 0},

        {microsynth_envelope_osc[0], 0, microsynth_filter_osc[0], 0}, // osc envelope to filter
        {microsynth_envelope_osc[1], 0, microsynth_filter_osc[1], 0},

        {microsynth_envelope_osc[0], 0, microsynth_mixer_filter_osc[0], 3}, // osc env to mixer for bypass
        {microsynth_envelope_osc[1], 0, microsynth_mixer_filter_osc[1], 3},

        {microsynth_filter_osc[0], 0, microsynth_mixer_filter_osc[0], 0}, // osc filter 0 to mixer 0
        {microsynth_filter_osc[0], 1, microsynth_mixer_filter_osc[0], 1},
        {microsynth_filter_osc[0], 2, microsynth_mixer_filter_osc[0], 2},

        {microsynth_filter_osc[1], 0, microsynth_mixer_filter_osc[1], 0}, // osc filter 1 to mixer 1
        {microsynth_filter_osc[1], 1, microsynth_mixer_filter_osc[1], 1},
        {microsynth_filter_osc[1], 2, microsynth_mixer_filter_osc[1], 2},

        {microsynth_mixer_filter_osc[0], 0, microsynth_stereo_panorama_osc[0], 0}, // mixer 0 to panorama 0
        {microsynth_mixer_filter_osc[0], 0, microsynth_stereo_panorama_osc[0], 1},

        {microsynth_mixer_filter_osc[1], 0, microsynth_stereo_panorama_osc[1], 0}, // mixer 1 to panorama 1
        {microsynth_mixer_filter_osc[1], 0, microsynth_stereo_panorama_osc[1], 1},

        {microsynth_stereo_panorama_osc[0], 0, microsynth_mixer_r[0], 0}, // osc 0 pan to internal mixer
        {microsynth_stereo_panorama_osc[0], 1, microsynth_mixer_l[0], 0},

        {microsynth_stereo_panorama_osc[1], 0, microsynth_mixer_r[1], 0}, // osc 1 pan to internal mixer
        {microsynth_stereo_panorama_osc[1], 1, microsynth_mixer_l[1], 0},

        {microsynth_stereo_panorama_noise[0], 0, microsynth_mixer_r[0], 1}, // noise 0 to mixer
        {microsynth_stereo_panorama_noise[0], 1, microsynth_mixer_l[0], 1},

        {microsynth_stereo_panorama_noise[1], 0, microsynth_mixer_r[1], 1}, // noise 1 to mixer
        {microsynth_stereo_panorama_noise[1], 1, microsynth_mixer_l[1], 1},

        {microsynth_mixer_r[0], 0, microsynth_main_mixer_r, 0},
        {microsynth_mixer_l[0], 0, microsynth_main_mixer_l, 0},

        {microsynth_mixer_r[1], 0, microsynth_main_mixer_r, 1},
        {microsynth_mixer_l[1], 0, microsynth_main_mixer_l, 1},

        {microsynth_main_mixer_r, 0, master_mixer_r, MASTER_MIX_CH_MICROSYNTH},
        {microsynth_main_mixer_l, 0, master_mixer_l, MASTER_MIX_CH_MICROSYNTH},

        {microsynth_mixer_r[0], 0, microsynth_mixer_reverb, 0},
        {microsynth_mixer_r[1], 0, microsynth_mixer_reverb, 1},

        {microsynth_mixer_reverb, 0, reverb_mixer_r, REVERB_MIX_CH_MICROSYNTH},
        {microsynth_mixer_reverb, 1, reverb_mixer_l, REVERB_MIX_CH_MICROSYNTH},

        {microsynth_mixer_filter_osc[0], 0, microsynth_peak_osc_0, 0},     // osc 1 (inst1) to peak analyzer
        {microsynth_mixer_filter_osc[1], 0, microsynth_peak_osc_1, 0},     // osc 2 (inst2) to peak analyzer
        {microsynth_mixer_filter_noise[0], 0, microsynth_peak_noise_0, 0}, // unfiltered noise to mixer
        {microsynth_mixer_filter_noise[1], 0, microsynth_peak_noise_1, 0},
};

AudioConnection patchCordUsbL(finalized_mixer_l, 0, usb1, 0);
AudioConnection patchCordUsbR(finalized_mixer_r, 0, usb1, 1);

AudioConnection patchCord_spdif_L(finalized_mixer_l, 0, spdif3, 0); //enable S/PDIF OUTPUT BY DEFAULT
AudioConnection patchCord_spdif_R(finalized_mixer_r, 0, spdif3, 1);

//
// Dynamic patching of MicroDexed objects
//
uint8_t nDynamic = 0;
AudioConnection* dynamicConnections[NUM_DEXED * 16 + NUM_DRUMS * 4 + NUM_BRAIDS * 11 + 14 + 20];
// test with envelopes for samples
// AudioConnection* dynamicConnections[NUM_DEXED * 16 + NUM_DRUMS * 4 + NUM_BRAIDS * 11 + 14 + 16+16];

FLASHMEM void create_audio_dexed_chain(uint8_t instance_id)
{
  MicroDexed[instance_id] = new AudioSynthDexed(MAX_NOTES / NUM_DEXED, SAMPLE_RATE);
  dexed_mono2stereo[instance_id] = new AudioEffectMonoStereo();
  delay_mono2stereo[instance_id] = new AudioEffectMonoStereo();
  dexed_dry_mono2stereo[instance_id] = new AudioEffectMonoStereo();

  chorus_modulator[instance_id] = new AudioSynthWaveform();

  dexed_chorus[instance_id] = new AudioEffectModulatedDelay();
  global_delay_in_mixer[instance_id] = new AudioMixer<8 + NUM_DRUMS>();
  delay_fb_mixer[instance_id] = new AudioMixer<2>();

#ifdef PSRAM
  delay_fx[instance_id] = new AudioEffectDelayExternal8(AUDIO_MEMORY8_EXTMEM, DELAY_MAX_TIME);
  const uint32_t delay_memory_size = (sizeof(((audio_block_t*)0)->data[0])) * (DELAY_MAX_TIME * (AUDIO_SAMPLE_RATE_EXACT / 1000.0f)) + 0.5f;
  psram_free_bytes -= delay_memory_size;
#else
  delay_fx[instance_id] = new AudioEffectDelay();
#endif

  delay_mixer[instance_id] = new AudioMixer<2>();

  if (instance_id == 0)
  {
    dynamicConnections[nDynamic++] = new AudioConnection(*MicroDexed[instance_id], 0, microdexed_peak_0, 0);
  }
  else
    dynamicConnections[nDynamic++] = new AudioConnection(*MicroDexed[instance_id], 0, microdexed_peak_1, 0);

  dynamicConnections[nDynamic++] = new AudioConnection(*MicroDexed[instance_id], 0, *global_delay_in_mixer[instance_id], 0);

  //chorus
  dynamicConnections[nDynamic++] = new AudioConnection(*MicroDexed[instance_id], 0, *dexed_chorus[instance_id], 0);
  dynamicConnections[nDynamic++] = new AudioConnection(*chorus_modulator[instance_id], 0, *dexed_chorus[instance_id], 1);

  dynamicConnections[nDynamic++] = new AudioConnection(*dexed_chorus[instance_id], 0, dexed_chorus_mixer_r[instance_id], 1);
  dynamicConnections[nDynamic++] = new AudioConnection(*dexed_chorus[instance_id], 0, dexed_chorus_mixer_l[instance_id], 1);

  dynamicConnections[nDynamic++] = new AudioConnection(dexed_chorus_mixer_r[instance_id], 0, master_mixer_r, instance_id);
  dynamicConnections[nDynamic++] = new AudioConnection(dexed_chorus_mixer_l[instance_id], 0, master_mixer_l, instance_id);

  dynamicConnections[nDynamic++] = new AudioConnection(*global_delay_in_mixer[instance_id], 0, global_delay_filter[instance_id], 0);
  dynamicConnections[nDynamic++] = new AudioConnection(*global_delay_in_mixer[instance_id], 0, global_delay_filter_mixer[instance_id], 0); // bypass
  dynamicConnections[nDynamic++] = new AudioConnection(global_delay_filter[instance_id], 0, global_delay_filter_mixer[instance_id], 1);    // low
  dynamicConnections[nDynamic++] = new AudioConnection(global_delay_filter[instance_id], 1, global_delay_filter_mixer[instance_id], 2);    // band
  dynamicConnections[nDynamic++] = new AudioConnection(global_delay_filter[instance_id], 2, global_delay_filter_mixer[instance_id], 3);    // hi

  dynamicConnections[nDynamic++] = new AudioConnection(global_delay_filter_mixer[instance_id], 0, *delay_fb_mixer[instance_id], 0);
  dynamicConnections[nDynamic++] = new AudioConnection(global_delay_filter_mixer[instance_id], 0, *delay_mixer[instance_id], 0);

  dynamicConnections[nDynamic++] = new AudioConnection(*delay_fb_mixer[instance_id], 0, *delay_fx[instance_id], 0);
  dynamicConnections[nDynamic++] = new AudioConnection(*delay_fx[instance_id], 0, *delay_fb_mixer[instance_id], 1);
  dynamicConnections[nDynamic++] = new AudioConnection(*delay_fx[instance_id], 0, *delay_mixer[instance_id], 1);
  dynamicConnections[nDynamic++] = new AudioConnection(*delay_mixer[instance_id], 0, *delay_mono2stereo[instance_id], 0);

  dynamicConnections[nDynamic++] = new AudioConnection(*MicroDexed[instance_id], 0, reverb_mixer_r, instance_id);
  dynamicConnections[nDynamic++] = new AudioConnection(*MicroDexed[instance_id], 0, reverb_mixer_l, instance_id);

  // epiano delay
  dynamicConnections[nDynamic++] = new AudioConnection(ep, 0, *global_delay_in_mixer[instance_id], 5);
  dynamicConnections[nDynamic++] = new AudioConnection(ep, 1, *global_delay_in_mixer[instance_id], 6);

  // microsynth delays
  dynamicConnections[nDynamic++] = new AudioConnection(microsynth_mixer_filter_osc[0], 0, *global_delay_in_mixer[instance_id], 2);
  dynamicConnections[nDynamic++] = new AudioConnection(microsynth_mixer_filter_osc[1], 0, *global_delay_in_mixer[instance_id], 3);

  // dexed delay output
  dynamicConnections[nDynamic++] = new AudioConnection(*MicroDexed[instance_id], 0, *dexed_dry_mono2stereo[instance_id], 0);
  dynamicConnections[nDynamic++] = new AudioConnection(*MicroDexed[instance_id], 1, *dexed_dry_mono2stereo[instance_id], 1);

  // dexed dry output
  dynamicConnections[nDynamic++] = new AudioConnection(*dexed_dry_mono2stereo[instance_id], 0, dexed_chorus_mixer_r[instance_id], 0);
  dynamicConnections[nDynamic++] = new AudioConnection(*dexed_dry_mono2stereo[instance_id], 1, dexed_chorus_mixer_l[instance_id], 0);

  // dynamicConnections[nDynamic++] = new AudioConnection(*dexed_dry_mono2stereo[instance_id], 0, master_mixer_r, instance_id);
  // dynamicConnections[nDynamic++] = new AudioConnection(*dexed_dry_mono2stereo[instance_id], 1, master_mixer_l, instance_id);

  // delay outs as sends to reverb
  dynamicConnections[nDynamic++] = new AudioConnection(*delay_mixer[instance_id], 0, reverb_mixer_r, REVERB_MIX_CH_AUX_DELAY1 + instance_id);
  dynamicConnections[nDynamic++] = new AudioConnection(*delay_mixer[instance_id], 0, reverb_mixer_l, REVERB_MIX_CH_AUX_DELAY1 + instance_id);

  // master out
  dynamicConnections[nDynamic++] = new AudioConnection(*delay_mono2stereo[instance_id], 0, master_mixer_r, MASTER_MIX_CH_DELAY1 + instance_id);
  dynamicConnections[nDynamic++] = new AudioConnection(*delay_mono2stereo[instance_id], 1, master_mixer_l, MASTER_MIX_CH_DELAY1 + instance_id);

  if (instance_id == 1)
  {
    // delay1 to delay2
    dynamicConnections[nDynamic++] = new AudioConnection(*global_delay_in_mixer[0], 0, *global_delay_in_mixer[1], 7);
    // delay2 to delay1
    dynamicConnections[nDynamic++] = new AudioConnection(*global_delay_in_mixer[1], 0, *global_delay_in_mixer[0], 7);

  }

#ifdef DEBUG
  LOG.print(F("Dexed-Instance: "));
  LOG.println(instance_id);
#endif
}

FLASHMEM void create_audio_braids_chain(uint8_t instance_id)
{
  synthBraids[instance_id] = new AudioSynthBraids();
  braids_envelope[instance_id] = new AudioEffectEnvelope();
  braids_filter[instance_id] = new AudioFilterBiquad();
  braids_mixer_filter[instance_id] = new AudioMixer<4>;
  dynamicConnections[nDynamic++] = new AudioConnection(*synthBraids[instance_id], 0, *braids_envelope[instance_id], 0);
  dynamicConnections[nDynamic++] = new AudioConnection(*braids_envelope[instance_id], 0, *braids_filter[instance_id], 0);
  dynamicConnections[nDynamic++] = new AudioConnection(*braids_filter[instance_id], 0, *braids_mixer_filter[instance_id], 0);
  dynamicConnections[nDynamic++] = new AudioConnection(*braids_envelope[instance_id], 0, *braids_mixer_filter[instance_id], 3); // filter is off
  dynamicConnections[nDynamic++] = new AudioConnection(*braids_mixer_filter[instance_id], 0, braids_mixer, instance_id);

  if (instance_id == 0)
  {
    dynamicConnections[nDynamic++] = new AudioConnection(braids_mixer, 0, braids_stereo_panorama, 0);
    dynamicConnections[nDynamic++] = new AudioConnection(braids_mixer, 0, braids_stereo_panorama, 1);

    dynamicConnections[nDynamic++] = new AudioConnection(braids_stereo_panorama, 0, braids_flanger_r, 0);
    dynamicConnections[nDynamic++] = new AudioConnection(braids_stereo_panorama, 1, braids_flanger_l, 0);

    dynamicConnections[nDynamic++] = new AudioConnection(braids_flanger_r, 0, master_mixer_r, MASTER_MIX_CH_BRAIDS);
    dynamicConnections[nDynamic++] = new AudioConnection(braids_flanger_l, 0, master_mixer_l, MASTER_MIX_CH_BRAIDS);

    dynamicConnections[nDynamic++] = new AudioConnection{ braids_mixer, 0, braids_mixer_reverb, 0 };
    dynamicConnections[nDynamic++] = new AudioConnection{ braids_mixer, 0, braids_mixer_reverb, 1 };
    dynamicConnections[nDynamic++] = new AudioConnection{ braids_mixer_reverb, 0, reverb_mixer_r, REVERB_MIX_CH_BRAIDS };
    dynamicConnections[nDynamic++] = new AudioConnection{ braids_mixer_reverb, 1, reverb_mixer_l, REVERB_MIX_CH_BRAIDS };

    dynamicConnections[nDynamic++] = new AudioConnection{ braids_stereo_panorama, 0, braids_peak_r, 0 };
    dynamicConnections[nDynamic++] = new AudioConnection{ braids_stereo_panorama, 1, braids_peak_l, 0 };

    // braids delays
    dynamicConnections[nDynamic++] = new AudioConnection(braids_mixer, 0, *global_delay_in_mixer[0], 4);
    dynamicConnections[nDynamic++] = new AudioConnection(braids_mixer, 0, *global_delay_in_mixer[1], 4);
  }
}

//
// Dynamic patching of Drum objects
//
FLASHMEM void create_audio_drum_chain(uint8_t instance_id)
{
  // Drum[instance_id] = new AudioPlayMemory();
  // Drum[instance_id] = new AudioPlaySdWav();

  // test with envelopes for samples
  //sample_envelope[instance_id] = new AudioEffectEnvelope();  // for all compile-types

#ifdef COMPILE_FOR_FLASH
  Drum[instance_id] = new AudioPlayFlashResmp();
  // Drum[instance_id] = new AudioPlaySerialflashRaw();
#endif

#ifdef COMPILE_FOR_SDCARD
  Drum[instance_id] = new AudioPlaySdResmp();
#endif

#if defined(COMPILE_FOR_PROGMEM) || defined(COMPILE_FOR_PSRAM)
  Drum[instance_id] = new AudioPlayArrayResmp();
#endif

  Drum_filter[instance_id] = new AudioFilterBiquad();
  Drum[instance_id]->enableInterpolation(false);
  Drum[instance_id]->setPlaybackRate(1.0);

  // dynamicConnections[nDynamic++] = new AudioConnection(*Drum[instance_id], 0, drum_mixer_r, instance_id);
  // dynamicConnections[nDynamic++] = new AudioConnection(*Drum[instance_id], 0, drum_mixer_l, instance_id);
  // dynamicConnections[nDynamic++] = new AudioConnection(*Drum[instance_id], 0, drum_reverb_send_mixer_r, instance_id);
  // dynamicConnections[nDynamic++] = new AudioConnection(*Drum[instance_id], 0, drum_reverb_send_mixer_l, instance_id);

//test with filters for samples
  dynamicConnections[nDynamic++] = new AudioConnection(*Drum[instance_id], 0, *Drum_filter[instance_id], 0);
  dynamicConnections[nDynamic++] = new AudioConnection(*Drum_filter[instance_id], 0, drum_mixer_r, instance_id);
  dynamicConnections[nDynamic++] = new AudioConnection(*Drum_filter[instance_id], 0, drum_mixer_l, instance_id);

  dynamicConnections[nDynamic++] = new AudioConnection(*Drum_filter[instance_id], 0, drum_reverb_send_mixer_r, instance_id);
  dynamicConnections[nDynamic++] = new AudioConnection(*Drum_filter[instance_id], 0, drum_reverb_send_mixer_l, instance_id);

  dynamicConnections[nDynamic++] = new AudioConnection(*Drum_filter[instance_id], 0, *global_delay_in_mixer[0], 8 + instance_id);
  dynamicConnections[nDynamic++] = new AudioConnection(*Drum_filter[instance_id], 0, *global_delay_in_mixer[1], 8 + instance_id);
  ////photodo

  Drum_filter[instance_id]->setLowpass(0, 20000, 0.2); //phtodo

  // test with envelopes for samples
  // dynamicConnections[nDynamic++] = new AudioConnection(*Drum[instance_id], 0, *sample_envelope[instance_id], 0);
  // dynamicConnections[nDynamic++] = new AudioConnection(*sample_envelope[instance_id], 0, drum_mixer_r, instance_id);
  // dynamicConnections[nDynamic++] = new AudioConnection(*sample_envelope[instance_id], 0, drum_mixer_l, instance_id);
  // dynamicConnections[nDynamic++] = new AudioConnection(*sample_envelope[instance_id], 0, drum_reverb_send_mixer_r, instance_id);
  // dynamicConnections[nDynamic++] = new AudioConnection(*sample_envelope[instance_id], 0, drum_reverb_send_mixer_l, instance_id);

#ifdef DEBUG
  LOG.print(F("Drum-Instance: "));
  LOG.println(instance_id);
  LOG.print(F("Dynamic-Connection-Counter="));
  LOG.println(nDynamic);
#endif
}

FLASHMEM void create_audio_wav_preview_chain()
{
  dynamicConnections[nDynamic++] = new AudioConnection(WAV_preview_SD, 0, master_mixer_r, MASTER_MIX_CH_WAV_PREVIEW_SD);
  dynamicConnections[nDynamic++] = new AudioConnection(WAV_preview_SD, 0, master_mixer_l, MASTER_MIX_CH_WAV_PREVIEW_SD);
#ifdef COMPILE_FOR_FLASH
  dynamicConnections[nDynamic++] = new AudioConnection(WAV_preview_FLASH, 0, master_mixer_r, MASTER_MIX_CH_WAV_PREVIEW_FLASH_OR_PSRAM);
  dynamicConnections[nDynamic++] = new AudioConnection(WAV_preview_FLASH, 0, master_mixer_l, MASTER_MIX_CH_WAV_PREVIEW_FLASH_OR_PSRAM);
#endif

#ifdef COMPILE_FOR_PSRAM
  dynamicConnections[nDynamic++] = new AudioConnection(WAV_preview_PSRAM, 0, master_mixer_r, MASTER_MIX_CH_WAV_PREVIEW_FLASH_OR_PSRAM);
  dynamicConnections[nDynamic++] = new AudioConnection(WAV_preview_PSRAM, 0, master_mixer_l, MASTER_MIX_CH_WAV_PREVIEW_FLASH_OR_PSRAM);
#endif

}

uint8_t sd_card = 0;
Sd2Card card;
SdVolume volume;
const float midi_ticks_factor[10] = { 0.0, 0.25, 0.375, 0.5, 0.75, 1.0, 1.5, 2.0, 3.0, 4.0 };
uint8_t midi_bpm_counter = 0;
uint8_t midi_bpm = 0;
int16_t _midi_bpm = -1;
elapsedMillis midi_bpm_timer;
elapsedMillis long_button_pressed;
elapsedMillis control_rate;
elapsedMillis save_sys;
elapsedMillis record_timer;
elapsedMillis touchReadTimer = 0;

static constexpr int SCREENSAVER_RESET_RATE_MS = 500;
elapsedMillis screenSaverResetTimer = 0;
bool wakeScreenFlag = true;

bool save_sys_flag = false;
uint8_t active_voices[NUM_DEXED];
uint8_t midi_voices[NUM_DEXED];
#ifdef SHOW_CPU_LOAD_MSEC
elapsedMillis cpu_mem_millis;
#endif
uint32_t cpumax = 0;

bool sidechain_a_active = false;
bool sidechain_b_active = false;

bool sidechain_trigger_a;
bool sidechain_trigger_b;

uint8_t sidechain_a_sample_number = NUM_DRUMSET_CONFIG - 1;
uint8_t sidechain_b_sample_number = NUM_DRUMSET_CONFIG - 1;

uint8_t sidechain_a_speed = 0;
uint8_t sidechain_b_speed = 0;

sdcard_t sdcard_infos;

#ifdef COMPILE_FOR_FLASH
flash_t flash_infos;
#endif

config_t configuration;
char version_string[display_cols + 10 + 1];
char sd_string[display_cols + 1];
char g_bank_name[NUM_DEXED][BANK_NAME_LEN];
char g_voice_name[NUM_DEXED][VOICE_NAME_LEN];
char tmp_bank_name[BANK_NAME_LEN];
char tmp_voice_name[VOICE_NAME_LEN];
char receive_bank_filename[FILENAME_LEN];
uint8_t selected_instance_id = 0;
uint8_t microsynth_selected_instance = 0;
bool ui_save_notification_icon;
char noteNames[12][3] = { "C", "C#", "D", "D#", "E", "F", "F#", "G", "G#", "A", "A#", "B" };

uint8_t remote_MIDI_CC = 0;
uint8_t remote_MIDI_CC_value;

#include "filemanager.h"
uint8_t incomingSerialByte;

int8_t midi_decay_dexed[NUM_DEXED] = { -1, -1 };
int8_t midi_decay_microsynth[NUM_MICROSYNTH];
elapsedMillis midi_decay_timer_microsynth;
extern void update_microsynth_params(void);
elapsedMillis microsynth_control_rate;
elapsedMillis midi_decay_timer_dexed;

int perform_attack_mod[NUM_DEXED] = { 0, 0 };
int perform_release_mod[NUM_DEXED] = { 0, 0 };
// Allocate the delay lines for chorus
int16_t* delayline[NUM_DEXED];
int16_t* ep_delayline_r;
int16_t* ep_delayline_l;

extern drum_config_t drum_config[NUM_DRUMSET_CONFIG];
uint8_t drum_counter;
uint8_t drum_type[NUM_DRUMS];
uint8_t drum_midi_channel = DEFAULT_DRUM_MIDI_CHANNEL;

extern sequencer_t seq;
extern void sequencer(void);
extern uint8_t seq_prev_note[NUM_SEQ_TRACKS];
extern void print_arp_start_stop_button(void);
PeriodicTimer sequencer_timer;

multisample_s msp[NUM_MULTISAMPLES];
multisample_zone_t msz[NUM_MULTISAMPLES][NUM_MULTISAMPLE_ZONES];
uint8_t note_buffer_msp[NUM_DRUMS];
uint8_t msp_sample_slot_trigger_step[NUM_DRUMS];

extern LCDMenuLib2 LCDML;

extern void getNoteName(char* noteName, uint8_t noteNumber);
extern void update_midi_learn_button(void);
custom_midi_map_t custom_midi_map[NUM_CUSTOM_MIDI_MAPPINGS];
extern void print_custom_mappings(void);
extern void handle_page_with_touch_back_button();
extern void sequencer_part2(void);

bool touch_ic_found = false;

// // Hook (https://www.pjrc.com/teensy/td_startup.html)
// extern "C" void startup_late_hook(void);
// extern "C" volatile uint32_t systick_millis_count;

// void startup_late_hook(void) {
//   // force millis() to be 300 to skip startup delays
//   systick_millis_count = 300;

/***********************************************************************
   SETUP
 ***********************************************************************/

FLASHMEM void setup()
{

#ifdef DEBUG
  LOG.println(CrashReport);
  // setup_debug_message();

  generate_version_string(version_string, sizeof(version_string));
  LOG.println(F("MicroDexed based on https://github.com/asb2m10/dexed"));
  LOG.println(F("(c)2018-2023 H. Wirtz <wirtz@parasitstudio.de>"));
  LOG.println(F("https://codeberg.org/dcoredump/MicroDexed"));
  LOG.println(F("MicroDexed touch based on https://codeberg.org/dcoredump/MicroDexed"));
  LOG.println(F("https://codeberg.org/positionhigh/MicroDexed-touch"));
  LOG.print(F("Version: "));
  LOG.println(version_string);
  LOG.print(F("CPU-Speed: "));
  LOG.print(F_CPU / 1000000.0, 1);
  LOG.println(F(" MHz"));
  LOG.println(F("<setup start>"));
  LOG.flush();
#endif

#ifdef CAPACITIVE_TOUCH_DISPLAY
  pinMode(TFT_TOUCH_IRQ, INPUT);
#endif

  pinMode(BUT_R_PIN, INPUT_PULLUP);
  pinMode(BUT_L_PIN, INPUT_PULLUP);

#ifdef ONBOARD_BUTTON_INTERFACE
  pinMode(BI_UP, INPUT_PULLUP);
  pinMode(BI_DOWN, INPUT_PULLUP);
  pinMode(BI_LEFT, INPUT_PULLUP);
  pinMode(BI_RIGHT, INPUT_PULLUP);
  pinMode(BI_SELECT, INPUT_PULLUP);
  pinMode(BI_START, INPUT_PULLUP);
  pinMode(BI_BUTTON_A, INPUT_PULLUP);
  pinMode(BI_BUTTON_B, INPUT_PULLUP);
#endif

  // Init display / SPI Flash
  SPI.begin();
  display.begin();

  // invert display colors
#ifdef CAPACITIVE_TOUCH_DISPLAY
  display.invertDisplay(1);
  //display.invertDisplay(configuration.sys.invert_colors);
#endif

#if defined GENERIC_DISPLAY
  touch.begin();
  touch_ic_found = true;
#endif

#if defined GENERIC_DISPLAY
  delay(10);  // have seen some boot issues with old display without this delay 2024/06/19 
#endif

#if defined(PSRAM)
  delay(10); // FIXME: this somehow workarounds capacitive build with PSRAM not booting reliably
  // 2024/4/14
  // However it is reported that it is not necessary with 16MB psram and even making boot problems
  // after testing with my only mdt with 8MB psram and capacitive touch, the delay makes boot problems after COLD BOOT
  // will disable delay, until any non-working combination is reported
  // 2024/8/16 after being stuck in bootloops for hours, i put back in the delay and it just booted up with psram
#endif

#ifdef CAPACITIVE_TOUCH_DISPLAY
  if (!touch.begin(40))
  {
    ;
#ifdef DEBUG
    LOG.println("Unable to start touchscreen.");
#endif
    touch_ic_found = false;

  }
  else
  {
    ;
#ifdef DEBUG
    LOG.println("Touchscreen started.");
#endif
    touch_ic_found = true;
  }
#endif

#ifdef COMPILE_FOR_PSRAM
  psram_free_bytes = external_psram_size * 1048576;
#endif

  // Setup MIDI devices
  setup_midi_devices();

  // Start audio system
  AudioMemory(AUDIO_MEM);

#if defined(TEENSY_AUDIO_BOARD)
  sgtl5000.enable();
  sgtl5000.lineOutLevel(SGTL5000_LINEOUT_LEVEL);
  sgtl5000.dacVolumeRamp();
  sgtl5000.dacVolume(1.0);
  // sgtl5000.dacVolumeRampLinear();
  // sgtl5000.dacVolumeRampDisable();
  sgtl5000.unmuteHeadphone();
  sgtl5000.unmuteLineout();
  sgtl5000.volume(SGTL5000_HEADPHONE_VOLUME, SGTL5000_HEADPHONE_VOLUME); // Headphone volume

#ifdef SGTL5000_AUDIO_THRU
  // sgtl5000.audioPreProcessorEnable();
  sgtl5000.inputSelect(AUDIO_INPUT_LINEIN);
  sgtl5000.lineInLevel(5);
  // sgtl5000.adcHighPassFilterEnable();
  // sgtl5000.adcHighPassFilterDisable();
#endif
  sgtl5000.audioProcessorDisable();
  sgtl5000.autoVolumeDisable();
  sgtl5000.surroundSoundDisable();
  sgtl5000.enhanceBassDisable();

#ifdef DEBUG
  LOG.println(F("Teensy-Audio-Board enabled."));
#endif

#elif defined(TGA_AUDIO_BOARD)
  wm8731_1.enable();
  wm8731_1.volume(1.0);
#ifdef DEBUG
  LOG.println(F("TGA board enabled."));
#endif
#elif defined(I2S_AUDIO_ONLY)
#ifdef DEBUG
  LOG.println(F("I2S enabled."));
#endif
#elif defined(PT8211_AUDIO)
#ifdef DEBUG
  LOG.println(F("PT8211 enabled."));
#endif
#elif defined(TEENSY_DAC_SYMMETRIC)
  invMixer.gain(0, -1.f);
#ifdef DEBUG
  LOG.println(F("Internal DAC using symmetric outputs enabled."));
#endif
#else

#ifdef DEBUG
  LOG.println(F("Internal DAC enabled."));
#endif

#endif

  // create dynamic Dexed instances
  for (uint8_t instance_id = 0; instance_id < NUM_DEXED; instance_id++)
  {
#ifdef DEBUG
    LOG.print(F("Creating MicroDexed instance "));
    LOG.println(instance_id, DEC);
#endif
    create_audio_dexed_chain(instance_id);
  }
#ifdef DEBUG
  for (uint8_t instance_id = 0; instance_id < NUM_DEXED; instance_id++)
  {
    LOG.print(F("Instance "));
    LOG.print(instance_id);
    LOG.print(F(": "));
    LOG.print(MicroDexed[instance_id]->getMaxNotes());
    LOG.println(F(" voices"));
  }
#endif

  // create dynamic Braids instances
  for (uint8_t instance_id = 0; instance_id < NUM_BRAIDS; instance_id++)
  {
    create_audio_braids_chain(instance_id);
  }

  if (!braids_flanger_r.begin(braids_r_delayline, BRAIDS_FLANGE_DELAY_LENGTH, braids_flanger_idx, braids_flanger_depth, braids_flanger_freq))
  {
#ifdef DEBUG
    LOG.println(F("AudioEffectFlanger - right channel begin failed"));
#endif
    while (1)
      ;
  }

  if (!braids_flanger_l.begin(braids_l_delayline, BRAIDS_FLANGE_DELAY_LENGTH, braids_flanger_idx, braids_flanger_depth, braids_flanger_freq))
  {
#ifdef DEBUG
    LOG.println(F("AudioEffectFlanger - left channel begin failed"));
#endif
    while (1)
      ;
  }
  // Initially the flanger effect is off.
  braids_flanger_r.voices(FLANGE_DELAY_PASSTHRU, 0, 0);
  braids_flanger_l.voices(FLANGE_DELAY_PASSTHRU, 0, 0);

  // Setup (PROGMEM) sample drums
  // create dynamic Drum instances
  for (uint8_t instance_id = 0; instance_id < NUM_DRUMS; instance_id++)
  {
#ifdef DEBUG
    LOG.print(F("Creating Drum instance "));
    LOG.println(instance_id, DEC);
#endif
    create_audio_drum_chain(instance_id);

    drum_mixer_r.gain(instance_id, 1.0);
    drum_mixer_l.gain(instance_id, 1.0);
    drum_reverb_send_mixer_r.gain(instance_id, 0.0);
    drum_reverb_send_mixer_l.gain(instance_id, 0.0);

    //global_delay_in_mixer[0]->gain(instance_id+ALL_STATIC_DELAY_INSTR_OUT,0.5);  //phtodo
    //global_delay_in_mixer[1]->gain(instance_id+ALL_STATIC_DELAY_INSTR_OUT,0.5); 

        // test with envelopes for samples
        // sample_envelope[instance_id]->attack(0);
        // sample_envelope[instance_id]->delay(0);
        // sample_envelope[instance_id]->hold(50);
        // sample_envelope[instance_id]->decay(50);
        // sample_envelope[instance_id]->sustain(1.0);
        // sample_envelope[instance_id]->release(1425);
        // sample_envelope[instance_id]->releaseNoteOn(5);//xxxyyy
  }

  // Setup EPiano
  // EP_CHORUS
  ep_delayline_r = (int16_t*)malloc(MOD_DELAY_SAMPLE_BUFFER * sizeof(int16_t));
  if (ep_delayline_r == NULL)
  {
#ifdef DEBUG
    LOG.println(F("AudioEffectModulatedDelay R - memory allocation failed EP"));
#endif
    while (1)
      ;
  }
  ep_delayline_l = (int16_t*)malloc(MOD_DELAY_SAMPLE_BUFFER * sizeof(int16_t));
  if (ep_delayline_l == NULL)
  {
#ifdef DEBUG
    LOG.println(F("AudioEffectModulatedDelay L - memory allocation failed EP"));
#endif
    while (1)
      ;
  }

  if (!ep_modchorus.begin(ep_delayline_r, ep_delayline_l, MOD_DELAY_SAMPLE_BUFFER))
  {
#ifdef DEBUG
    LOG.println(F("AudioEffectModulatedDelayStereo - begin failed EP"));
#endif
    while (1)
      ;
  }

#if MOD_FILTER_OUTPUT == MOD_BUTTERWORTH_FILTER_OUTPUT
  // Butterworth filter, 12 db/octave
  ep_modchorus_filter.setLowpass(0, MOD_FILTER_CUTOFF_HZ, 0.707);
#elif MOD_FILTER_OUTPUT == MOD_LINKWITZ_RILEY_FILTER_OUTPUT
  // Linkwitz-Riley filter, 48 dB/octave
  ep_modchorus_filter.setLowpass(0, MOD_FILTER_CUTOFF_HZ, 0.54);
  ep_modchorus_filter.setLowpass(1, MOD_FILTER_CUTOFF_HZ, 1.3);
  ep_modchorus_filter.setLowpass(2, MOD_FILTER_CUTOFF_HZ, 0.54);
  ep_modchorus_filter.setLowpass(3, MOD_FILTER_CUTOFF_HZ, 1.3);
#endif
  ep_chorus_mixer_r.gain(0, 1.0);
  ep_chorus_mixer_l.gain(0, 1.0);
  ep_chorus_mixer_r.gain(1, mapfloat(EP_CHORUS_LEVEL_DEFAULT, EP_CHORUS_LEVEL_MIN, EP_CHORUS_LEVEL_MAX, 0.0, 0.5));
  ep_chorus_mixer_l.gain(1, mapfloat(EP_CHORUS_LEVEL_DEFAULT, EP_CHORUS_LEVEL_MIN, EP_CHORUS_LEVEL_MAX, 0.0, 0.5));
  ep_stereo_panorama.panorama(mapfloat(PANORAMA_DEFAULT, PANORAMA_MIN, PANORAMA_MAX, -1.0, 1.0));

  microsynth_update_all_settings(0);
  microsynth_update_all_settings(1);

  braids_update_all_settings();

  liveSeq.initOnce();

  // Setup effects

  /// dexed chorus
  for (uint8_t instance_id = 0; instance_id < NUM_DEXED; instance_id++)
  {
    delayline[instance_id] = (int16_t*)malloc(MOD_DELAY_SAMPLE_BUFFER * sizeof(int16_t));
    if (delayline[instance_id] != NULL)
    {
      memset(delayline[instance_id], 0, MOD_DELAY_SAMPLE_BUFFER * sizeof(int16_t));
      if (!dexed_chorus[instance_id]->begin(delayline[instance_id], MOD_DELAY_SAMPLE_BUFFER))
      {
#ifdef DEBUG
        LOG.print(F("AudioEffectModulatedDelay - begin failed ["));
        LOG.print(instance_id);
        LOG.println(F("]"));
#endif
        while (1)
          ;
      }
    }
    else
    {
#ifdef DEBUG
      LOG.print(F("AudioEffectModulatedDelay - memory allocation failed ["));
      LOG.print(instance_id);
      LOG.println(F("]"));
#endif
      while (1)
        ;
    }
  }
#ifdef DEBUG
  LOG.print(F("MOD_DELAY_SAMPLE_BUFFER="));
  LOG.print(MOD_DELAY_SAMPLE_BUFFER, DEC);
  LOG.println(F(" samples"));
#endif

  // Setup SD WAV play

  create_audio_wav_preview_chain();
  master_mixer_r.gain(MASTER_MIX_CH_WAV_PREVIEW_SD, 0.4);
  master_mixer_l.gain(MASTER_MIX_CH_WAV_PREVIEW_SD, 0.4);

#if defined(COMPILE_FOR_FLASH) || defined(COMPILE_FOR_PSRAM)
  master_mixer_r.gain(MASTER_MIX_CH_WAV_PREVIEW_FLASH_OR_PSRAM, 0.4);
  master_mixer_l.gain(MASTER_MIX_CH_WAV_PREVIEW_FLASH_OR_PSRAM, 0.4);
#endif

  //  Serial Flash Init

#ifdef COMPILE_FOR_FLASH
  if (!SerialFlash.begin(FlashChipSelect))
  {
#ifdef DEBUG
    LOG.print(F("Unable to access SPI Flash chip"));
#endif
  }
#endif

  AudioNoInterrupts();
  // Start SD card
  sd_card = check_sd_cards();
  AudioInterrupts();
  if (sd_card < 1)
  {
#ifdef DEBUG
    LOG.println(F("SD card not accessible."));
#endif
    strcpy(receive_bank_filename, "*ERROR*");
    for (uint8_t instance_id = 0; instance_id < NUM_DEXED; instance_id++)
    {
      strcpy(g_voice_name[instance_id], "*ERROR*");
      strcpy(g_bank_name[instance_id], "*ERROR*");
    }
  }
  else
  {
#ifdef DEBUG
    LOG.println(F("SD card found."));
#endif
    AudioNoInterrupts();
    check_and_create_directories();
    AudioInterrupts();
    memset(receive_bank_filename, 0, FILENAME_LEN);
    for (uint8_t instance_id = 0; instance_id < NUM_DEXED; instance_id++)
    {
      // load default SYSEX data
      // load_sd_voice(configuration.dexed[instance_id].pool, configuration.dexed[instance_id].bank, configuration.dexed[instance_id].voice, instance_id);

      memset(g_voice_name[instance_id], 0, VOICE_NAME_LEN);
      memset(g_bank_name[instance_id], 0, BANK_NAME_LEN);
    }
  }

  // Start timer (to avoid a crash when loading the performance data)
  sequencer_timer.begin(sequencer, seq.tempo_ms / (seq.ticks_max + 1), false);

  if (digitalRead(BUT_R_PIN) == false || digitalRead(BUT_L_PIN) == false) // is pushed
    bootup_performance_loading = false;
#ifdef ONBOARD_BUTTON_INTERFACE
  if (digitalRead(BI_BUTTON_A) == false || digitalRead(BI_BUTTON_B) == false) // is pushed
    bootup_performance_loading = false;
#endif

  // Initialize processor and memory measurements
  AudioProcessorUsageMaxReset();
  AudioMemoryUsageMaxReset();

  // Load voices
#ifdef DEBUG
  for (uint8_t instance_id = 0; instance_id < NUM_DEXED; instance_id++)
  {
    LOG.print(F("Dexed instance "));
    LOG.print(instance_id);
    LOG.println(F(":"));
    LOG.print(F("Bank/Voice ["));
    LOG.print(configuration.dexed[instance_id].bank, DEC);
    LOG.print(F("/"));
    LOG.print(configuration.dexed[instance_id].voice, DEC);
    LOG.println(F("]"));
    LOG.print(F("Polyphony: "));
    LOG.println(configuration.dexed[instance_id].polyphony, DEC);
  }
  LOG.print(F("AUDIO_BLOCK_SAMPLES="));
  LOG.print(AUDIO_BLOCK_SAMPLES);
  LOG.print(F(" (Time per block="));
  LOG.print(1000000 / (SAMPLE_RATE / AUDIO_BLOCK_SAMPLES));
  LOG.println(F("ms)"));
#endif

#ifdef DEBUG
  LOG.println(F("Show CPU Usage"));
#endif

#if defined(DEBUG) && defined(SHOW_CPU_LOAD_MSEC)
  show_cpu_and_mem_usage();
#endif

  // Init master_mixer

  for (uint8_t instance_id = 0; instance_id < NUM_DEXED; instance_id++)
  {
    master_mixer_r.gain(instance_id, VOL_MAX_FLOAT);
    master_mixer_l.gain(instance_id, VOL_MAX_FLOAT);

    dexed_chorus_mixer_r[instance_id].gain(0, VOL_MAX_FLOAT);
    dexed_chorus_mixer_l[instance_id].gain(0, VOL_MAX_FLOAT);

    dexed_chorus_mixer_r[instance_id].gain(1, 0);
    dexed_chorus_mixer_l[instance_id].gain(1, 0);

  }

  master_mixer_r.gain(MASTER_MIX_CH_REVERB, VOL_MAX_FLOAT);
  master_mixer_l.gain(MASTER_MIX_CH_REVERB, VOL_MAX_FLOAT);

  master_mixer_r.gain(MASTER_MIX_CH_DRUMS, VOL_MAX_FLOAT);
  master_mixer_l.gain(MASTER_MIX_CH_DRUMS, VOL_MAX_FLOAT);
  seq.drums_volume = VOL_MAX_FLOAT;

  master_mixer_r.gain(MASTER_MIX_CH_EPIANO, VOL_MAX_FLOAT);
  master_mixer_l.gain(MASTER_MIX_CH_EPIANO, VOL_MAX_FLOAT);

  master_mixer_r.gain(MASTER_MIX_CH_MICROSYNTH, VOL_MAX_FLOAT);
  master_mixer_l.gain(MASTER_MIX_CH_MICROSYNTH, VOL_MAX_FLOAT);

  master_mixer_r.gain(MASTER_MIX_CH_BRAIDS, VOL_MAX_FLOAT);
  master_mixer_l.gain(MASTER_MIX_CH_BRAIDS, VOL_MAX_FLOAT);

  master_mixer_r.gain(MASTER_MIX_CH_DELAY1, VOL_MAX_FLOAT);
  master_mixer_l.gain(MASTER_MIX_CH_DELAY1, VOL_MAX_FLOAT);
  master_mixer_r.gain(MASTER_MIX_CH_DELAY2, VOL_MAX_FLOAT);
  master_mixer_l.gain(MASTER_MIX_CH_DELAY2, VOL_MAX_FLOAT);

#if defined(TEENSY_AUDIO_BOARD) && defined(SGTL5000_AUDIO_THRU)
  audio_thru_mixer_r.gain(0, VOL_MAX_FLOAT); // MD signal sum
  audio_thru_mixer_l.gain(0, VOL_MAX_FLOAT); // MD signal sum
#ifdef TEENSY_AUDIO_BOARD
  audio_thru_mixer_r.gain(1, VOL_MAX_FLOAT); // I2S input
  audio_thru_mixer_l.gain(1, VOL_MAX_FLOAT); // I2S input
#else
  audio_thru_mixer_r.gain(1, 0.0);
  audio_thru_mixer_l.gain(1, 0.0);
#endif
  audio_thru_mixer_r.gain(2, 0.0);
  audio_thru_mixer_l.gain(2, 0.0);
  audio_thru_mixer_r.gain(3, 0.0);
  audio_thru_mixer_l.gain(3, 0.0);
#endif

  finalized_mixer_l.gain(0, VOL_MAX_FLOAT); // normal output, mute multiband
  finalized_mixer_r.gain(0, VOL_MAX_FLOAT);
  finalized_mixer_l.gain(1, 0);
  finalized_mixer_r.gain(1, 0);

  //bitcrusher
  // bitcrusher_l.bits(6);
  //   bitcrusher_r.bits(6);
  //   bitcrusher_l.sampleRate(44100/3);
  //   bitcrusher_r.sampleRate(44100/3);

  global_delay_in_mixer[0]->gain(7, 0.0);
  global_delay_in_mixer[1]->gain(7, 0.0);

#ifdef DEBUG
  LOG.println(F("<setup end>"));
#endif

  // ep_modchorus.set_bypass(true);
  if (seq.name[0] == 0)
    strcpy(seq.name, "INIT Perf");

#ifdef DEBUG
  LOG.println(F("Setup UI."));
#endif
  // Load initial Performance or the last used one
  initial_values(false);

  setup_ui();

  gamepad_buttons_neutral = joysticks[0].getButtons();
  gamepad_0_neutral = joysticks[0].getAxis(0);
  gamepad_1_neutral = joysticks[0].getAxis(1);

  // Setup editor helpers
  calc_val[0] = sum;
  calc_val[1] = sub;

#ifdef COMPILE_FOR_PSRAM
  display.setTextSize(2);
  display.setTextColor(COLOR_SYSTEXT, COLOR_BACKGROUND);
  display.setCursor(1 * CHAR_width, CHAR_height * 1);
  display.print(F("LOADING SAMPLES:"));
  display.setCursor(21 * CHAR_width, CHAR_height * 1);
  display.print(F("/"));
  display.setCursor(23 * CHAR_width, CHAR_height * 1);
  display.print(NUM_DRUMSET_CONFIG);

  newdigate::flashloader loader;
  display.setTextSize(1);
  for (int i = 0; i < 6; i++) {  // load pitched samples
    char temp_name[36];
    strcpy(temp_name, "/DRUMS/");
    strcat(temp_name, drum_config[i].filename);
    strcat(temp_name, ".wav");
    loadSample(loader, i, temp_name);
    printLoadedSample(i, temp_name);
  }

  // load drums
  for (int i = 6 + NUM_CUSTOM_SAMPLES; i < NUM_DRUMSET_CONFIG; i++) {
    char temp_name[36];
    strcpy(temp_name, "/DRUMS/");
    strcat(temp_name, drum_config[i].filename);
    strcat(temp_name, ".wav");
    loadSample(loader, i, temp_name);
    printLoadedSample(i, temp_name);
  }
  display.setTextSize(2);
  AudioInterrupts();
  display.console = true;
  display.fillRect(0, CHAR_height, DISPLAY_WIDTH, CHAR_height * 4, COLOR_BACKGROUND);
  display.console = false;
#endif

  //SD CARD PRESET CONTENT NOT FOUND, PROVIDE POSSIBLE SOLUTIONS TO USER
  if (!SD.exists("/DEXED/0/0") || !SD.exists("/DEXED/0/99") || !SD.exists("/PERFORMANCE") || !SD.exists("/DRUMS"))
  {
    LCDML.OTHER_jumpToFunc(UI_func_sd_content_not_found);
  }
  else {
    if (count_omni() != 0 || count_midi_channel_duplicates(false) != 0) // startup with midi channel setup page
      LCDML.OTHER_jumpToFunc(UI_func_midi_channels);
    else
      if (seq.clock == 1) //MIDI Slave
        LCDML.OTHER_jumpToFunc(UI_func_seq_settings);
      else
      {
        if (configuration.sys.boot_anim_skip == 0)
          boot_animation();
        // Menu Startup
        switch (configuration.sys.load_at_startup_page)
        {
        case 0:
          LCDML.OTHER_jumpToFunc(UI_func_voice_select);
          break;
        case 1:
          LCDML.OTHER_jumpToFunc(UI_func_song);
          break;
        case 2:
          LCDML.OTHER_jumpToFunc(UI_func_seq_pattern_editor);
          break;
        case 3:
          LCDML.OTHER_jumpToFunc(UI_func_microsynth);
          break;
        case 4:
          LCDML.OTHER_jumpToFunc(UI_func_seq_tracker);
          break;
        case 5:
          LCDML.OTHER_jumpToFunc(UI_func_MultiSamplePlay);
          break;
        case 6:
          LCDML.OTHER_jumpToFunc(UI_func_epiano);
          break;
        case 7:
          LCDML.OTHER_jumpToFunc(UI_func_braids);
          break;
        case 8:
          LCDML.OTHER_jumpToFunc(UI_func_mixer);
          break;
        case 9:
          LCDML.OTHER_jumpToFunc(UI_func_livesequencer);
          break;
        case 50:
          LCDML.OTHER_jumpToFunc(UI_func_information);
          break;
        default:
          LCDML.OTHER_jumpToFunc(UI_func_voice_select); // fallback to voice select
        }
      }
  }
}

#ifdef COMPILE_FOR_PSRAM
FLASHMEM void load_custom_samples_to_psram()
{
  // load CUSTOM samples
  newdigate::flashloader loader;
  int i = NUM_STATIC_PITCHED_SAMPLES;
  do {
    char temp_name[36];
    strcpy(temp_name, "/CUSTOM/");
    strcat(temp_name, drum_config[i].filename);
    loadSample(loader, i, temp_name);
    if (drum_config[i].len == 0 || drum_config[i].len > 10000000)
    {
      snprintf_P(temp_name, 12, PSTR("CUSTOM%02d"), i - NUM_STATIC_PITCHED_SAMPLES + 1);
      strcpy(drum_config[i].filename, temp_name);
      strcpy(drum_config[i].name, temp_name);
    }
    i++;
  } while (i < NUM_CUSTOM_SAMPLES + NUM_STATIC_PITCHED_SAMPLES);
}
#endif

FLASHMEM void print_midi_channel_activity(uint8_t x, uint8_t y, float audio_vol)
{
  uint8_t display_val = 0;
  if (audio_vol * 1024 > 253)
    display_val = 254;
  else
    display_val = uint8_t(audio_vol * 1024);

  setCursor_textGrid(x, y);
  display.setTextColor(ColorHSV(100, 250, display_val));
  display.print(">");
}

FLASHMEM void handle_touchscreen_midi_channel_page()
{
  if (seq.running)
  {
    display.setTextSize(2);
    print_midi_channel_activity(19, 3, microdexed_peak_0.read());
    print_midi_channel_activity(19, 4, microdexed_peak_1.read());
    print_midi_channel_activity(19, 5, (ep_peak_l.read() + ep_peak_r.read()) / 2);
    if (microsynth_peak_osc_0.available())
      print_midi_channel_activity(19, 6, microsynth_peak_osc_0.read());
    else
      print_midi_channel_activity(19, 6, 0);
    if (microsynth_peak_osc_1.available())
      print_midi_channel_activity(19, 7, microsynth_peak_osc_1.read());
    else
      print_midi_channel_activity(19, 7, 0);

    if (braids_peak_l.available() && braids_peak_r.available())
      print_midi_channel_activity(19, 8, (braids_peak_l.read() + braids_peak_r.read()) / 2);
    else
      print_midi_channel_activity(19, 8, 0);

    print_midi_channel_activity(19, 9, ts.msp_peak[0]);
    print_midi_channel_activity(19, 10, ts.msp_peak[1]);
    ts.msp_peak[0] = ts.msp_peak[0] / 1.05;
    ts.msp_peak[1] = ts.msp_peak[1] / 1.05;

    if (drum_mixer_peak_r.available() && drum_mixer_peak_l.available())
      print_midi_channel_activity(19, 11, (drum_mixer_peak_l.read() + drum_mixer_peak_r.read()) / 2);
    else
      print_midi_channel_activity(19, 11, 0);

    display.setTextColor(COLOR_SYSTEXT, COLOR_BACKGROUND);
  }
}

FLASHMEM void sub_step_recording(bool touchinput, uint8_t touchparam)
{
  if (seq.running == false && seq.step_recording)
  {
    if (touchinput && touchparam == 1) //rest step
    {
      seq.note_in = 0;
      seq.note_in_velocity = 0;
    }
    else if (touchinput && touchparam == 2 && seq.content_type[seq.active_pattern] != 0) //latch step
    {
      seq.note_in = 130;
      seq.note_in_velocity = 0;
    }
    if ((seq.recording == false && seq.note_in > 0 && seq.note_in_velocity > 0 && touchinput == false) ||
      (touchinput == true && seq.cycle_touch_element == 1 && touchparam == 1) ||
      (touchinput == true && seq.cycle_touch_element == 1 && touchparam == 2 && seq.content_type[seq.active_pattern] != 0))

    {
      uint8_t cur_step = 0;
      if (LCDML.FUNC_getID() == LCDML.OTHER_getIDFromFunction(UI_func_seq_pattern_editor))
        cur_step = seq.menu - 3;
      else if (LCDML.FUNC_getID() == LCDML.OTHER_getIDFromFunction(UI_func_seq_vel_editor))
        cur_step = seq.menu - 1;

      if (cur_step < 16)
      {
        seq.note_data[seq.active_pattern][cur_step] = seq.note_in;

        if (touchparam == 0)
        {
          if (seq.content_type[seq.active_pattern] == 0 && get_sample_note(activesample) > 209 && ts.virtual_keyboard_instrument != 6) // pitched sample
          {
            seq.vel[seq.active_pattern][cur_step] = get_sample_note(activesample);
          }
          else
            seq.vel[seq.active_pattern][cur_step] = seq.note_in_velocity;
        }

        else if (touchinput && touchparam == 1)
        {
          seq.vel[seq.active_pattern][cur_step] = 0;
        }
        else if (touchinput && touchparam == 2)
        {
          seq.vel[seq.active_pattern][cur_step] = 0;
        }

        setCursor_textGrid(cur_step, 1);
        display.setTextSize(2);

        if (seq.auto_advance_step == 0)
          display.setTextColor(COLOR_SYSTEXT, RED);
        else
        {
          if (seq.vel[seq.active_pattern][cur_step] > 209)
            display.setTextColor(COLOR_PITCHSMP, COLOR_BACKGROUND);
          else
            set_pattern_content_type_color(seq.active_pattern);
        }
        display.print(seq_find_shortname(cur_step)[0]);
        seq_printVelGraphBar_single_step(cur_step, GREY1);
        if (seq.cycle_touch_element != 1)
          print_track_steps_detailed_only_current_playing_note(0, CHAR_height * 4 + 3, cur_step);

        display.setTextSize(2);
        display.setTextColor(COLOR_SYSTEXT, RED);
        if (seq.auto_advance_step > 0)
        {
          if (cur_step < 15)
          {
            seq.menu = seq.menu + 1;
            setCursor_textGrid(cur_step + 1, 1);
            display.print(seq_find_shortname(cur_step + 1)[0]);
            if (seq.cycle_touch_element != 1)
              print_track_steps_detailed_only_current_playing_note(0, CHAR_height * 4 + 3, cur_step + 1);
          }
          else
          {
            if (seq.auto_advance_step == 1) // continue auto advance after last step on first step
            {
              seq.menu = seq.menu - 15;
              setCursor_textGrid(cur_step - 15, 1);
              display.print(seq_find_shortname(cur_step - 15)[0]);
              if (seq.cycle_touch_element != 1)
                print_track_steps_detailed_only_current_playing_note(0, CHAR_height * 4 + 3, cur_step - 15);
            }
            else // stop at last step
            {
              // disable step record
              seq.step_recording = false;
              // handleStop
              draw_button_on_grid(36, 1, "STEP", "RECORD", 1); // print step recorder icon
            }
          }
        }
      }
      seq.note_in = 0;
      seq.note_in_velocity = 0;
    }
  }
}

float pseudo_log_curve(float value)
{
  // return (mapfloat(_pseudo_log * arm_sin_f32(value), 0.0, _pseudo_log * arm_sin_f32(1.0), 0.0, 1.0));
  // return (1 - sqrt(1 - value * value));
  // return (pow(2, value) - 1);
  return (pow(value, 2.2));
}

float sc_dexed1_current = 1.0;
float sc_dexed2_current = 1.0;

uint8_t sc_dexed1_target_a = 0;
uint8_t sc_dexed2_target_a = 0;

uint8_t sc_dexed1_target_b = 0;
uint8_t sc_dexed2_target_b = 0;

float sc_braids_current = 1.0;

uint8_t sc_braids_target_a = 0;
uint8_t sc_braids_target_b = 0;

float sc_delay_a_current = 1.0;
float sc_delay_b_current = 1.0;

uint8_t sc_delay_a_target_a = 0;
uint8_t sc_delay_a_target_b = 0;
uint8_t sc_delay_b_target_a = 0;
uint8_t sc_delay_b_target_b = 0;

float sc_reverb_current = 1.0;

uint8_t sc_reverb_target_a = 0;
uint8_t sc_reverb_target_b = 0;

uint8_t sc_seq_step_displayed;

void update_sidechain()
{
  if (LCDML.FUNC_getID() == LCDML.OTHER_getIDFromFunction(UI_func_sidechain))
  {
    display.setTextSize(1);
    display.setTextColor(RED, COLOR_BACKGROUND);
    setCursor_textGrid_small(28, 4);
    if (sidechain_trigger_a)
    {
      display.print(F("T"));
      sc_seq_step_displayed = seq.step;
    }
    else if (sc_seq_step_displayed != seq.step)
    {
      display.print(F(" "));
      sc_seq_step_displayed = seq.step;
    }
    display.setTextColor(COLOR_SYSTEXT, COLOR_BACKGROUND);
  }

  if (sidechain_trigger_a)
  {

    if (sc_dexed1_target_a != 0)
    {
      sc_dexed1_current = VOL_MAX_FLOAT - (float)sc_dexed1_target_a / 100;
      if (sc_dexed1_current < 0.0)
        sc_dexed1_current = 0.0;
      master_mixer_r.gain(MASTER_MIX_CH_DEXED1, sc_dexed1_current);
      master_mixer_l.gain(MASTER_MIX_CH_DEXED1, sc_dexed1_current);
    }

    if (sc_dexed2_target_a != 0)
    {
      sc_dexed2_current = VOL_MAX_FLOAT - (float)sc_dexed2_target_a / 100;
      if (sc_dexed2_current < 0.0)
        sc_dexed2_current = 0.0;
      master_mixer_r.gain(MASTER_MIX_CH_DEXED2, sc_dexed2_current);
      master_mixer_l.gain(MASTER_MIX_CH_DEXED2, sc_dexed2_current);
    }

    if (sc_braids_target_a != 0)
    {
      sc_braids_current = VOL_MAX_FLOAT - (float)sc_braids_target_a / 100;
      if (sc_braids_current < 0.0)
        sc_braids_current = 0.0;
      master_mixer_r.gain(MASTER_MIX_CH_BRAIDS, sc_braids_current);
      master_mixer_l.gain(MASTER_MIX_CH_BRAIDS, sc_braids_current);
    }

    if (sc_delay_a_target_a != 0)
    {

      sc_delay_a_current = VOL_MAX_FLOAT - (float)sc_delay_a_target_a / 100;
      if (sc_delay_a_current < 0.0)
        sc_delay_a_current = 0.0;
      master_mixer_r.gain(MASTER_MIX_CH_DELAY1, sc_delay_a_current);
      master_mixer_l.gain(MASTER_MIX_CH_DELAY1, sc_delay_a_current);
    }

    if (sc_delay_b_target_a != 0)
    {

      sc_delay_b_current = VOL_MAX_FLOAT - (float)sc_delay_b_target_a / 100;
      if (sc_delay_b_current < 0.0)
        sc_delay_b_current = 0.0;
      master_mixer_r.gain(MASTER_MIX_CH_DELAY1, sc_delay_b_current);
      master_mixer_l.gain(MASTER_MIX_CH_DELAY1, sc_delay_b_current);
    }

    if (sc_reverb_target_a != 0)
    {

      sc_reverb_current = VOL_MAX_FLOAT - (float)sc_reverb_target_a / 100;
      if (sc_reverb_current < 0.0)
        sc_reverb_current = 0.0;
      master_mixer_r.gain(MASTER_MIX_CH_REVERB, sc_reverb_current);
      master_mixer_l.gain(MASTER_MIX_CH_REVERB, sc_reverb_current);
    }

    sidechain_trigger_a = false;
  }

  if (seq.running && control_rate > CONTROL_RATE_MS)
  {
    if (sc_dexed1_current < VOL_MAX_FLOAT && sc_dexed1_target_a != 0)
    {
      sc_dexed1_current = sc_dexed1_current + (float)0.002 * sidechain_a_speed;

      if (sc_dexed1_current >= VOL_MAX_FLOAT)
        sc_dexed1_current = VOL_MAX_FLOAT;

      master_mixer_r.gain(MASTER_MIX_CH_DEXED1, sc_dexed1_current);
      master_mixer_l.gain(MASTER_MIX_CH_DEXED1, sc_dexed1_current);
    }

    if (sc_dexed2_current < VOL_MAX_FLOAT && sc_dexed2_target_a != 0)
    {

      sc_dexed2_current = sc_dexed2_current + (float)0.002 * sidechain_a_speed;

      if (sc_dexed2_current >= VOL_MAX_FLOAT)
        sc_dexed2_current = VOL_MAX_FLOAT;

      master_mixer_r.gain(MASTER_MIX_CH_DEXED2, sc_dexed2_current);
      master_mixer_l.gain(MASTER_MIX_CH_DEXED2, sc_dexed2_current);
    }

    if (seq.running && sc_braids_current < VOL_MAX_FLOAT && sc_braids_target_a != 0)
    {

      sc_braids_current = sc_braids_current + (float)0.002 * sidechain_a_speed;

      if (sc_braids_current >= VOL_MAX_FLOAT)
        sc_braids_current = VOL_MAX_FLOAT;

      master_mixer_r.gain(MASTER_MIX_CH_BRAIDS, sc_braids_current);
      master_mixer_l.gain(MASTER_MIX_CH_BRAIDS, sc_braids_current);
    }

    if (sc_delay_a_current < VOL_MAX_FLOAT && sc_delay_a_target_a != 0)
    {

      sc_delay_a_current = sc_delay_a_current + (float)0.002 * sidechain_a_speed;

      if (sc_delay_a_current >= VOL_MAX_FLOAT)
        sc_delay_a_current = VOL_MAX_FLOAT;

      master_mixer_r.gain(MASTER_MIX_CH_DELAY1, sc_delay_a_current);
      master_mixer_l.gain(MASTER_MIX_CH_DELAY1, sc_delay_a_current);
    }

    if (sc_delay_b_current < VOL_MAX_FLOAT && sc_delay_b_target_a != 0)
    {

      sc_delay_b_current = sc_delay_b_current + (float)0.002 * sidechain_b_speed;

      if (sc_delay_b_current >= VOL_MAX_FLOAT)
        sc_delay_b_current = VOL_MAX_FLOAT;

      master_mixer_r.gain(MASTER_MIX_CH_DELAY2, sc_delay_b_current);
      master_mixer_l.gain(MASTER_MIX_CH_DELAY2, sc_delay_b_current);
    }

    if (sc_reverb_current < VOL_MAX_FLOAT && sc_reverb_target_a != 0)
    {

      sc_reverb_current = sc_reverb_current + (float)0.002 * sidechain_a_speed;

      if (sc_reverb_current >= VOL_MAX_FLOAT)
        sc_reverb_current = VOL_MAX_FLOAT;

      master_mixer_r.gain(MASTER_MIX_CH_REVERB, sc_reverb_current);
      master_mixer_l.gain(MASTER_MIX_CH_REVERB, sc_reverb_current);
    }
  }

  if (control_rate > CONTROL_RATE_MS && LCDML.FUNC_getID() == LCDML.OTHER_getIDFromFunction(UI_func_sidechain))
  {
    print_sidechain_level_indicators();
  }
}

void preview_sample()
{
  if (LCDML.FUNC_getID() == LCDML.OTHER_getIDFromFunction(UI_func_file_manager))
  {
    draw_button_on_grid(46, 25, "PLEASE", "WAIT", 2);
    if (fm.active_window == 0 && !fm.sd_is_folder && strstr(fm.sd_temp_name, ".wav") != NULL)
    { // preview sd file
      strcpy(fm.sd_full_name, fm.sd_new_name);
      strcat(fm.sd_full_name, "/");
      strcat(fm.sd_full_name, fm.sd_temp_name);
      playWAVFile(fm.sd_full_name);
    }
    else if (fm.active_window == 1)
    { // preview flash file
#if defined(COMPILE_FOR_FLASH)
      if (strstr(fm.flash_or_psram_temp_name, ".wav") != NULL)
        playWAVFile(fm.flash_or_psram_temp_name);
#endif
#if defined(COMPILE_FOR_PSRAM)
      playWAVFilePSRAM(fm.flash_or_psram_selected_file + 6 + fm.flash_or_psram_skip_files);
#endif
    }
    draw_button_on_grid(46, 25, "PLAY", "SAMPLE", 1);
  }
  else if (LCDML.FUNC_getID() == LCDML.OTHER_getIDFromFunction(UI_func_sample_editor))
  {
    draw_button_on_grid(45, 23, "PLEASE", "WAIT", 2);
    if (fm.sample_source > 1 && fm.sample_source < 4) // MSP
    {
      playWAVFile(msz[fm.sample_source - 2][temp_int].filename);
    }
    else if (fm.sample_source == 1) // FLASH
    {
      uint32_t filesize;
      SerialFlash.opendir();
      if (temp_int + 1 > 0)
      {
        for (int f = 0; f < temp_int + 1; f++)
        {
          if (SerialFlash.readdir(filename, sizeof(filename), filesize))
            ;
          else
            break;
        }
      }
      playWAVFile(filename);
    }
    else if (fm.sample_source == 0) // SD
    {
      if (!WAV_preview_SD.isPlaying())
      {
        draw_button_on_grid(45, 23, "PLEASE", "WAIT", 2);
        char temp_name[26];
        File f;
        File myDir = SD.open("/DRUMS");
        for (uint16_t i = 0; i < temp_int + 1; i++)
        {
          f.close();
          f = myDir.openNextFile();
        }
        strcpy(temp_name, "/DRUMS/");
        strcat(temp_name, f.name());
        playWAVFile(temp_name);

      }
    }
    draw_button_on_grid(45, 23, "PLAY", "SAMPLE", 0);
  }
}


// Gaussian Wave Breathing

// float glow_smoothness_pts = 20;//larger=slower change in brightness
// float glow_gamma = 0.25; // affects the width of peak (more or less darkness)
// float glow_beta = 0.5; // shifts the gaussian to be symmetric
// float glow_bright = 0;
// int glow_count = 0;
// int glow_x;
// int glow_y;
// int glow_value;

// uint8_t glow_type; // 0 = off, 1 = intbar, 2=panbar

// void draw_glow()
// {
//   setCursor_textGrid_small(glow_x, glow_y);
//   display.setTextSize(1);
//   display.setTextColor(COLOR_BACKGROUND, ColorHSV(1, 0, int(glow_bright)));
//   if (glow_type == 1) {
//     print_formatted_number(glow_value, 3);
//   }
//   else
//     if (glow_type == 2) {
//       if (glow_value < 20)
//       {
//         display.print(F("L"));
//         print_formatted_number(20 - glow_value, 2);
//       }
//       else if (glow_value > 20)
//       {
//         display.print(F("R"));
//         print_formatted_number(glow_value - 20, 2);
//       }
//       else
//       {
//         display.print(F("C"));
//         print_formatted_number(glow_value - 20, 2);
//       }
//     }
// }

// void glow()
// {
//   if (seq.edit_state == 0 && glow_type != 0) {

//     if (glow_count < glow_smoothness_pts)
//       glow_count++;
//     else
//       glow_count = 0;
//     glow_bright = 255.0 * (exp(-(pow(((glow_count / glow_smoothness_pts) - glow_beta) / glow_gamma, 2.0)) / 2.0));
//     draw_glow();
//   }
//   else
//     glow_count = glow_smoothness_pts / 2;
// }

bool back_button_touch_page_check_and_init_done = false;
bool current_page_has_touch_back_button = false;

void loop()
{

  // Serial read (commands from web remote)
  incomingSerialByte = 0;
  if (Serial.available() > 0)
  {
    incomingSerialByte = Serial.read();
    if (incomingSerialByte == '%')
    {
      // SD file management from remote console
      sd_filemanager();
    }
  }

  // MIDI input handling
  check_midi_devices();

  // check encoder
  ENCODER[ENC_L].update();
  ENCODER[ENC_R].update();

  LCDML.loop();

  if (touchReadTimer >= TOUCH_MAX_REFRESH_RATE_MS && touch_ic_found) {
    touchReadTimer = 0;
    updateTouchScreen();
    const TouchFn touchHandler = getCurrentTouchHandler();
    if (touchHandler) {
      touchHandler();
    }
    else if (LCDML.FUNC_getID() > _LCDML_DISP_cnt) {
      // menu
      handle_touchscreen_menu();
      scope.draw_scope(230, 18, 87, 64);
    }

    if (back_button_touch_page_check_and_init_done == false) {
      current_page_has_touch_back_button = (touch_button_back_page() || legacy_touch_button_back_page());
      if (current_page_has_touch_back_button) {
        draw_back_touchbutton();
      }
      back_button_touch_page_check_and_init_done = true;
    }

    if (current_page_has_touch_back_button) {
      handle_page_with_touch_back_button();
    }

    const ScopeSettings s = getCurrentScopeSettings();
    const bool inhibitDrawing = s.onlyDrawWhenRunning && !seq.running;
    if (s.enabled && !inhibitDrawing) {
      scope.draw_scope(s.x, s.y, s.w, s.h);
    }
  }

  if (screenSaverResetTimer >= SCREENSAVER_RESET_RATE_MS) {
    screenSaverResetTimer = 0;
    if (wakeScreenFlag) {
      wakeScreenFlag = false;
      resetScreenTimer();
    }
  }

  if (LCDML.FUNC_getID() == LCDML.OTHER_getIDFromFunction(UI_func_recorder))
  {
    if (fm.wav_recorder_mode == 1) {
      continueRecording();
    }
  }
  else if (LCDML.FUNC_getID() == LCDML.OTHER_getIDFromFunction(UI_func_seq_pattern_editor) || LCDML.FUNC_getID() == LCDML.OTHER_getIDFromFunction(UI_func_seq_vel_editor))
  {
    if (seq.running == false) {
      sub_step_recording(false, 0);
    }
  }

  if (microsynth_control_rate > MICROSYNTH_CONTROL_RATE_MS) // update lfos, filters etc. when played live or by seq.
  {
    microsynth_control_rate = 0;
    update_microsynth_params();
    // Microsynth Realtime Screen Updates

    if (LCDML.FUNC_getID() == LCDML.OTHER_getIDFromFunction(UI_func_microsynth)) {
      if (microsynth[microsynth_selected_instance].pwm_last_displayed != microsynth[microsynth_selected_instance].pwm_current && seq.cycle_touch_element != 1)
      {
        display.setTextSize(1);
        setCursor_textGrid_small(15, 18);
        display.setTextColor(GREY2, COLOR_BACKGROUND);
        print_formatted_number(microsynth[microsynth_selected_instance].pwm_current / 10, 3);
        microsynth[microsynth_selected_instance].pwm_last_displayed = microsynth[microsynth_selected_instance].pwm_current;
      }

      if (seq.cycle_touch_element != 1 && microsynth[microsynth_selected_instance].filter_osc_freq_last_displayed != microsynth[microsynth_selected_instance].filter_osc_freq_current)
      {
        display.setTextSize(1);
        setCursor_textGrid_small(15, 16);
        display.setTextColor(GREY2, COLOR_BACKGROUND);
        print_formatted_number(microsynth[microsynth_selected_instance].filter_osc_freq_current / 100, 3);
        microsynth[microsynth_selected_instance].filter_osc_freq_last_displayed = microsynth[microsynth_selected_instance].filter_osc_freq_current;
      }

      // debug
      display.setTextSize(1);
      display.setTextColor(GREY2, COLOR_BACKGROUND);
      setCursor_textGrid_small(42, 9);
      print_formatted_number(microsynth[0].lfo_delay, 4);
      setCursor_textGrid_small(42, 10);
      print_formatted_number(microsynth[0].lfo_fade, 4);
    }
  }

  if (braids_control_rate > BRAIDS_CONTROL_RATE_MS) // update  filters when played live or by seq.
  {
    braids_control_rate = 0;
    update_braids_params();
    if (LCDML.FUNC_getID() == LCDML.OTHER_getIDFromFunction(UI_func_braids))
    {
      display.setTextSize(1);
      display.setTextColor(GREY2, COLOR_BACKGROUND);

      for (uint8_t d = 0; d < NUM_BRAIDS; d++)
      {
        if (braids_filter_state_last_displayed[d] != braids_filter_state[d])
        {
          if (d >= NUM_BRAIDS / 2)
            setCursor_textGrid_small(49, 7 + d - NUM_BRAIDS / 2);
          else
            setCursor_textGrid_small(45, 7 + d);
          print_formatted_number(braids_filter_state[d] / 100, 3);
          braids_filter_state_last_displayed[d] = braids_filter_state[d];
        }
      }
    }
  }

  if (seq.running)
  {
    if (sidechain_a_active || sidechain_b_active)
      ;
    //update_sidechain();  //work in progress
  }

  if (control_rate > CONTROL_RATE_MS)
  {
    control_rate = 0;
    // glow();
    // display.fillRect(10,10,30,30, ColorHSV(1, 0, int(led_bright)));

    if (seq.running && seq.step != seq.UI_last_seq_step)
    {
      update_display_functions_while_seq_running();
    }

    // check for value changes, unused voices and CPU overload
    for (uint8_t instance_id = 0; instance_id < NUM_DEXED; instance_id++)
    {
      active_voices[instance_id] = MicroDexed[instance_id]->getNumNotesPlaying();
      if (active_voices[instance_id] == 0)
        midi_voices[instance_id] = 0;
    }

#ifdef COMPILE_FOR_FLASH
    if (WAV_preview_FLASH.isPlaying())
    {
      fm.sample_screen_position_x = fm.sample_screen_position_x + seq.wave_spacing / 10;
      if (fm.sample_screen_position_x < DISPLAY_WIDTH)
        display.fillRect(fm.sample_screen_position_x, 180, 1, 2, RED);
    }
#endif
    if (LCDML.FUNC_getID() == LCDML.OTHER_getIDFromFunction(UI_func_voice_select)) // draw MIDI in activity bars
    {
      for (uint8_t instance_id = 0; instance_id < NUM_DEXED; instance_id++)
      {
        display.console = true;
        if (midi_decay_timer_dexed > MIDI_DECAY_TIMER && midi_decay_dexed[instance_id] > 0)
        {
          midi_decay_dexed[instance_id]--;
          display.fillRect(187 + (instance_id * 12), 19, 5, 8 - midi_decay_dexed[instance_id], COLOR_BACKGROUND);                           // region above bar
          display.fillRect(187 + (instance_id * 12), 27 - midi_decay_dexed[instance_id], 5, midi_decay_dexed[instance_id], COLOR_PITCHSMP); // bar
        }
        else if (midi_voices[instance_id] == 0 && midi_decay_dexed[instance_id] == 0 && !MicroDexed[instance_id]->getSustain())
        {
          midi_decay_dexed[instance_id]--;
          display.fillRect(187 + (instance_id * 12), 19 + 7, 5, 1, COLOR_BACKGROUND);
        }
      }
      if (midi_decay_timer_dexed > MIDI_DECAY_LEVEL_TIME)
      {
        midi_decay_timer_dexed = 0;
      }
      display.console = false;
    }
    else if (LCDML.FUNC_getID() == LCDML.OTHER_getIDFromFunction(UI_func_microsynth)) // draw MIDI in activity bars on microsynth page
    {
      for (uint8_t instance_id = 0; instance_id < NUM_MICROSYNTH; instance_id++)
      {
        display.console = true;
        if (midi_decay_timer_microsynth > MIDI_DECAY_TIMER && midi_decay_microsynth[instance_id] > 1)
        {
          midi_decay_microsynth[instance_id]--;
          display.fillRect(13 * 6 + (instance_id * 12), 19, 5, 8 - midi_decay_microsynth[instance_id], COLOR_BACKGROUND);                                // region above bar
          display.fillRect(13 * 6 + (instance_id * 12), 27 - midi_decay_microsynth[instance_id], 5, midi_decay_microsynth[instance_id], COLOR_PITCHSMP); // bar
        }
        // if value is 0 then no drawing should happen - so stop at 1
        if (midi_decay_microsynth[instance_id] == 1)
        {
          display.fillRect(13 * 6 + (instance_id * 12), 19 + 7, 5, 1, COLOR_BACKGROUND);
          midi_decay_microsynth[instance_id] = 0;
        }
        if (midi_decay_timer_microsynth > MIDI_DECAY_LEVEL_TIME)
        {
          midi_decay_timer_microsynth = 0;
        }
      }
      display.console = false;
    }
  }

  // SAVE-SYS-EVENT-HANDLING
  if (save_sys > SAVE_SYS_MS && save_sys_flag == true)
  {
#ifdef DEBUG
    LOG.println(F("Check if we can save configuration.sys"));
#endif
    bool instance_is_playing = false;
    for (uint8_t instance_id = 0; instance_id < NUM_DEXED; instance_id++)
    {
      if (active_voices[instance_id] > 0 || seq.running)
      {
        instance_is_playing = true;
        break;
      }
    }
    if (instance_is_playing == false)
    {
      for (uint8_t instance_id = 0; instance_id < NUM_DRUMS; instance_id++)
      {
        if (Drum[instance_id]->isPlaying())
        {
          instance_is_playing = true;
          break;
        }
      }
    }
    if (instance_is_playing == false)
    {
      save_sd_sys_json();
      save_sys = 0;
      save_sys_flag = false;
      if (LCDML.FUNC_getID() == 255 && ts.keyb_in_menu_activated == false)
      {
        draw_button_on_grid(2, 25, "CONFIG", "SAVED", 1);
        ui_save_notification_icon = true;
      }
      else if (LCDML.FUNC_getID() == LCDML.OTHER_getIDFromFunction(UI_func_system_settings) ||
        LCDML.FUNC_getID() == LCDML.OTHER_getIDFromFunction(UI_func_test_touchscreen))
      {
        draw_button_on_grid(2, 25, "CONFIG", "SAVED", 1);
        ui_save_notification_icon = true;
      }
#ifdef DEBUG
      LOG.println(F("Saved."));
#endif
    }
    else
    {
#ifdef DEBUG
      LOG.println(F("System is playing, next try..."));
#endif
      save_sys = 0;
    }
  }

  if (save_sys > SAVE_SYS_MS && ui_save_notification_icon)
  {
    if (LCDML.FUNC_getID() == 255)
    {
      display.console = true;
      display.fillRect(2 * CHAR_width_small, 25 * CHAR_height_small, 42, 32, COLOR_BACKGROUND);
      ui_save_notification_icon = false;
      display.console = false;
    }
    else if (LCDML.FUNC_getID() == LCDML.OTHER_getIDFromFunction(UI_func_system_settings) ||
      LCDML.FUNC_getID() == LCDML.OTHER_getIDFromFunction(UI_func_test_touchscreen))
    {
      display.console = true;
      display.fillRect(2 * CHAR_width_small, 25 * CHAR_height_small, 42, 32, COLOR_BACKGROUND);
      ui_save_notification_icon = false;
      display.console = false;
    }
  }

#if defined(DEBUG) && defined(SHOW_CPU_LOAD_MSEC)
  if (cpu_mem_millis >= SHOW_CPU_LOAD_MSEC)
  {
    cpu_mem_millis -= SHOW_CPU_LOAD_MSEC;
    show_cpu_and_mem_usage();
  }
#endif

  // send all buffered draw commands to WebRemote
  display.flushSysEx();
}

/******************************************************************************
  MIDI MESSAGE HANDLER
******************************************************************************/

#if  defined(COMPILE_FOR_PSRAM)
FLASHMEM void playWAVFilePSRAM(uint8_t d)
{
  if (LCDML.FUNC_getID() == LCDML.OTHER_getIDFromFunction(UI_func_file_manager) && fm.active_window == 1)
  {
    if (drum_config[d].drum_data != NULL && drum_config[d].len > 0 && drum_config[d].len < 10000000) {
      if (WAV_preview_PSRAM.isPlaying())
        WAV_preview_PSRAM.stop();
      WAV_preview_PSRAM.playRaw((int16_t*)drum_config[d].drum_data, drum_config[d].len, 1);
      while (WAV_preview_PSRAM.isPlaying())
      {
        display.fillRect(6, 189, (float)(DISPLAY_WIDTH - 8) / (WAV_preview_PSRAM.lengthMillis()) * WAV_preview_PSRAM.positionMillis() + 1, 7, RED);
        delay(25);
      }
      delay(15);
      display.fillRect(6, 189, DISPLAY_WIDTH - 7, 7, COLOR_BACKGROUND);
    }
  }
}
#endif

FLASHMEM void playWAVFile(const char* filename)
{
  if (LCDML.FUNC_getID() == LCDML.OTHER_getIDFromFunction(UI_func_file_manager))
  {
    if (fm.active_window == 0)
    { // preview from sd card
      if (WAV_preview_SD.isPlaying())
        WAV_preview_SD.stop();
      WAV_preview_SD.play(filename);
      while (WAV_preview_SD.isPlaying())
      {
        display.fillRect(6, 189, (float)(DISPLAY_WIDTH - 8) / (WAV_preview_SD.lengthMillis()) * WAV_preview_SD.positionMillis() + 1, 7, RED);
        delay(25);
      }
      delay(15);
      display.fillRect(6, 189, DISPLAY_WIDTH - 7, 7, COLOR_BACKGROUND);
    }
#ifdef COMPILE_FOR_FLASH
    if (fm.active_window == 1)
    { // preview from flash
      if (WAV_preview_FLASH.isPlaying())
        WAV_preview_FLASH.stop();
      WAV_preview_FLASH.play(filename);
      while (WAV_preview_FLASH.isPlaying())
      {
        display.fillRect(6, 189, (float)(DISPLAY_WIDTH - 8) / (WAV_preview_FLASH.lengthMillis()) * WAV_preview_FLASH.positionMillis() + 1, 7, RED);
        delay(25);
      }
      delay(15);
      display.fillRect(6, 189, DISPLAY_WIDTH - 7, 7, COLOR_BACKGROUND);
    }
#endif
  }

#ifdef COMPILE_FOR_FLASH
  if (LCDML.FUNC_getID() == LCDML.OTHER_getIDFromFunction(UI_func_sample_editor) && fm.sample_source > 0 && fm.sample_source < 4)
  {
    // MSP + FLASH SAMPLES

    if (WAV_preview_FLASH.isPlaying())
      WAV_preview_FLASH.stop();
    WAV_preview_FLASH.play(filename);
    while (WAV_preview_FLASH.isPlaying())
    {
      display.fillRect(6, 222, (float)(DISPLAY_WIDTH - 8) / (WAV_preview_FLASH.lengthMillis()) * WAV_preview_FLASH.positionMillis() + 1, 5, RED);
      delay(25);
    }
    delay(15);
    display.fillRect(6, 222, DISPLAY_WIDTH - 7, 5, COLOR_BACKGROUND);
  }
#endif
  if (LCDML.FUNC_getID() == LCDML.OTHER_getIDFromFunction(UI_func_sample_editor) && fm.sample_source == 0)
  {

    // preview from sd card
    if (WAV_preview_SD.isPlaying())
      WAV_preview_SD.stop();
    WAV_preview_SD.play(filename);
    delay(15);
    while (WAV_preview_SD.isPlaying())
    {
      display.fillRect(6, 222, (float)(DISPLAY_WIDTH - 8) / (WAV_preview_SD.lengthMillis()) * WAV_preview_SD.positionMillis() + 1, 5, RED);
      delay(25);
    }
    delay(15);
    display.fillRect(6, 222, DISPLAY_WIDTH - 7, 5, COLOR_BACKGROUND);
  }
}

uint8_t msp_playmode_sample_slot[NUM_DRUMS];

#if defined(COMPILE_FOR_FLASH) || defined COMPILE_FOR_PSRAM

void Multi_Sample_Player(byte inNumber, byte inVelocity, byte instance_id)
{

  // find the MSP preset slot for this midi channel
  byte presetslot = instance_id;

  if (drum_counter >= NUM_DRUMS)
    drum_counter = 0;
  uint8_t slot = drum_get_slot(DRUM_POLY);

  if (slot != 99)
  {

    drum_type[slot] = DRUM_POLY;
    Drum[slot]->enableInterpolation(true);
    for (uint8_t y = 0; y < NUM_MULTISAMPLE_ZONES; y++)
    {
      if (inNumber >= msz[presetslot][y].low && inNumber <= msz[presetslot][y].high)
      {

        Drum_filter[slot]->setLowpass(0, (inVelocity * 110), 0.3);


        float pan = mapfloat(msz[presetslot][y].pan, PANORAMA_MIN, PANORAMA_MAX, 0.0, 1.0);
        // drum_mixer_r.gain(slot, (1.0 - pan) * volume_transform(mapfloat(inVelocity * msz[presetslot][y].vol * msp[presetslot].sound_intensity, 0, 1270000, 0.0, 0.9)));
        // drum_mixer_l.gain(slot, pan * volume_transform(mapfloat(inVelocity * msz[presetslot][y].vol * msp[presetslot].sound_intensity, 0, 1270000, 0.0, 0.9)));

        drum_mixer_r.gain(slot, (1.0 - pan) * volume_transform(mapfloat(inVelocity * msz[presetslot][y].vol * msp[presetslot].sound_intensity, 0, 1270000, 0.2, 0.98)));
        drum_mixer_l.gain(slot, pan * volume_transform(mapfloat(inVelocity * msz[presetslot][y].vol * msp[presetslot].sound_intensity, 0, 1270000, 0.2, 0.98)));

        ts.msp_peak[presetslot] = (inVelocity * msz[presetslot][y].vol * msp[presetslot].sound_intensity) / 900000;
        drum_reverb_send_mixer_r.gain(slot, volume_transform(mapfloat(msz[presetslot][y].rev, 0, 100, 0.0, 0.8)));
        drum_reverb_send_mixer_l.gain(slot, volume_transform(mapfloat(msz[presetslot][y].rev, 0, 100, 0.0, 0.8)));

        msp_playmode_sample_slot[slot] = msz[presetslot][y].playmode;

        if (msz[presetslot][y].loop_type == 0)
          Drum[slot]->setLoopType(loop_type::looptype_none);
        else   if (msz[presetslot][y].loop_type == 1)
          Drum[slot]->setLoopType(loop_type::looptype_repeat);
        else if (msz[presetslot][y].loop_type == 2)
          Drum[slot]->setLoopType(loop_type::looptype_pingpong);

        note_buffer_msp[slot] = inNumber;
        // Drum[slot]->setPlaybackRate(powf(2.0, (inNumber - msz[presetslot][y].rootnote) / 12.0) );

        Drum[slot]->setPlaybackRate(powf(2.0, (inNumber - msz[presetslot][y].rootnote) / 12.0) * mapfloat(msz[presetslot][y].tune, 0, 200, 0.98, 1.02));
        // test with envelopes for samples
        //sample_envelope[slot]->noteOn();

// uint8_t p=0;
//   //pitched sample
// if (inNumber>209)
// {
//   d=inNumber-210;
// }
// else
//  d=inNumber-18;

#if defined(COMPILE_FOR_FLASH)
        Drum[slot]->playWav(msz[presetslot][y].filename);
#endif
#if  defined COMPILE_FOR_PSRAM
        Drum[slot]->playRaw((int16_t*)drum_config[msz[presetslot][y].psram_entry_number].drum_data, drum_config[msz[presetslot][y].psram_entry_number].len, 1);
#endif

        if (msz[presetslot][y].loop_type != 0)
        {
          Drum[slot]->setLoopStart(msz[presetslot][y].loop_start);
          Drum[slot]->setLoopFinish(msz[presetslot][y].loop_end);
        }

        //   if (LCDML.FUNC_getID() == LCDML.OTHER_getIDFromFunction(UI_func_MultiSamplePlay)) {

        //   display.fillRect(0, 185 + y * 5, 2 * CHAR_width_small + msz[presetslot][y].low * 3.5 - (24 * 3.5) - 1, 5, COLOR_BACKGROUND);

        //   display.fillRect(2 * CHAR_width_small + msz[presetslot][y].low * 3.5 - (24 * 3.5), 185 + y * 5,
        //                    (msz[presetslot][y].high - msz[presetslot][y].low) * 3.5 + 2.5, 5, RED);

        //   display.fillRect(2 * CHAR_width_small + msz[presetslot][y].high * 3.5 - (24 * 3.5) + 3.5, 185 + y * 5,
        //                    DISPLAY_WIDTH - (msz[presetslot][y].high * 3.5) + (18 * 3.5), 5, COLOR_BACKGROUND);

        //   display.fillRect(2 * CHAR_width_small + msz[presetslot][y].rootnote * 3.5 - (24 * 3.5) - 1, 185 + y * 5 + 1,
        //                    3.5 + 1, 5 - 2, COLOR_SYSTEXT);
        // }
      }
      else
      {
        ;
        //   if (LCDML.FUNC_getID() == LCDML.OTHER_getIDFromFunction(UI_func_MultiSamplePlay)) {
        //   display.fillRect(2 * CHAR_width_small + msz[presetslot][y].low * 3.5 - (24 * 3.5), 185 + y * 5,
        //                    (msz[presetslot][y].high - msz[presetslot][y].low) * 3.5 + 2.5, 5, get_multisample_zone_color(y));
        //   display.fillRect(2 * CHAR_width_small + msz[presetslot][y].rootnote * 3.5 - (24 * 3.5) - 1, 185 + y * 5 + 1,
        //                    3.5 + 1, 5 - 2, COLOR_SYSTEXT);
        // }
      }
    }
  }
}
#endif

FLASHMEM void learn_key(byte inChannel, byte inNumber)
{
  uint8_t found = 199;

  if (generic_temp_select_menu == 0)
  { // learn drum mapping
    if (inChannel == drum_midi_channel)
    {
      for (uint8_t c = 0; c < NUM_CUSTOM_MIDI_MAPPINGS; c++)
      {
        if (inNumber == custom_midi_map[c].in && custom_midi_map[c].type == 1)
        {
          found = c;
          break;
        }
      }
      if (found != 199) // remap to new destination if it was already mapped before
      {
        custom_midi_map[found].in = inNumber;
        custom_midi_map[found].out = drum_config[activesample].midinote;
        custom_midi_map[found].type = 1;
        custom_midi_map[found].channel = drum_midi_channel;
      }
      else
      {
        found = 199;
        for (uint8_t c = 0; c < NUM_CUSTOM_MIDI_MAPPINGS; c++)
        {
          if (custom_midi_map[c].in == 0)
          {
            found = c;
            break;
          }
        }
        if (found != 199) // else map to next empty slot if it was not mapped before
        {
          custom_midi_map[found].in = inNumber;
          custom_midi_map[found].out = drum_config[activesample].midinote;
          custom_midi_map[found].type = 1;
          custom_midi_map[found].channel = drum_midi_channel;
        }
        else
          ; // can not be mapped, no empty slot left
      }
    }
  }
  else // UI MIDI KEY MAP
  {
    // if (inChannel != drum_midi_channel) {
    for (uint8_t c = 0; c < NUM_CUSTOM_MIDI_MAPPINGS; c++)
    {
      if (inNumber == custom_midi_map[c].in && custom_midi_map[c].type == 3)
      {
        found = c;
        break;
      }
    }
    if (found != 199) // remap to new destination if it was already mapped before
    {
      custom_midi_map[found].in = inNumber;
      custom_midi_map[found].out = cc_dest_values_UI_mapping[generic_menu];
      custom_midi_map[found].type = 3;
      custom_midi_map[found].channel = 0;
    }
    else
    {
      found = 199;
      for (uint8_t c = 0; c < NUM_CUSTOM_MIDI_MAPPINGS; c++)
      {
        if (custom_midi_map[c].in == 0)
        {
          found = c;
          break;
        }
      }
      if (found != 199) // else map to next empty slot if it was not mapped before
      {
        custom_midi_map[found].in = inNumber;
        custom_midi_map[found].out = cc_dest_values_UI_mapping[generic_menu];
        custom_midi_map[found].type = 3;
        custom_midi_map[found].channel = 0;
      }
      else
        ; // can not be mapped, no empty slot left
    }
    // }
  }
  seq.midi_learn_active = false;
  update_midi_learn_button();
  print_custom_mappings();
}

FLASHMEM void learn_cc(byte inChannel, byte inNumber)
{
  uint8_t found = 199;
  for (uint8_t c = 0; c < NUM_CUSTOM_MIDI_MAPPINGS; c++)
  {
    if (inNumber == custom_midi_map[c].in && custom_midi_map[c].type == 2)
    {
      found = c;
      break;
    }
  }
  if (found != 199) // remap to new destination if it was already mapped before
  {
    custom_midi_map[found].in = inNumber;
    custom_midi_map[found].out = cc_dest_values[generic_menu];
    custom_midi_map[found].type = 2;
    custom_midi_map[found].channel = configuration.dexed[selected_instance_id].midi_channel;
  }
  else
  {
    found = 199;
    for (uint8_t c = 0; c < NUM_CUSTOM_MIDI_MAPPINGS; c++)
    {
      if (custom_midi_map[c].in == 0)
      {
        found = c;
        break;
      }
    }
    if (found != 199) // else map to next empty slot if it was not mapped before
    {
      custom_midi_map[found].in = inNumber;
      custom_midi_map[found].out = cc_dest_values[generic_menu];
      custom_midi_map[found].type = 2;
      custom_midi_map[found].channel = configuration.dexed[selected_instance_id].midi_channel;
    }
    else
      ; // can not be mapped, no empty slot left
  }

  seq.midi_learn_active = false;
  update_midi_learn_button();
  print_custom_mappings();
}

uint8_t midiNoteToDrumNote(uint8_t note) {
  // TODO: eliminate magic numbers
  const uint8_t offset = (note > 209) ? 210 : 18;
  return note - offset;
}

void handleNoteOnInput(byte inChannel, byte inNumber, byte inVelocity, byte device) {
  //drum played from input selects current drum note if drum screen open
  if (inChannel == drum_midi_channel || drum_midi_channel == MIDI_CHANNEL_OMNI) {
    const uint8_t drumNote = midiNoteToDrumNote(inNumber);
    drumChanged(drumNote);
  }
  // play note
  handleNoteOn(inChannel, inNumber, inVelocity, device);
}


void handleNoteOn(byte inChannel, byte inNumber, byte inVelocity, byte device)
{
  wakeScreenFlag = true;

  if ((device == 4 || braids_osc.midi_channel == MIDI_CHANNEL_OMNI || braids_osc.midi_channel == inChannel) && inNumber < 119)
  {
    braids_slot++;
    if (braids_slot > NUM_BRAIDS - 1)
      braids_slot = 0;

    if (braids_osc.filter_mode == 0)
      braids_mixer_filter[braids_slot]->gain(3, volume_transform(mapfloat(inVelocity, REVERB_SEND_MIN, REVERB_SEND_MAX, 0.5, 1.2)));
    else
      braids_mixer_filter[braids_slot]->gain(braids_osc.filter_mode - 1, volume_transform(mapfloat(inVelocity, REVERB_SEND_MIN, REVERB_SEND_MAX, 0.5, 1.2)));

    braids_filter_state[braids_slot] = braids_osc.filter_freq_from;

    braids_lfo_direction[braids_slot] = false;
    braids_filter_lfo_count[braids_slot] = 0;

    braids_filter[braids_slot]->setLowpass(0, braids_osc.filter_freq_from, braids_osc.filter_resonance / 10);

    synthBraids[braids_slot]->set_braids_pitch((inNumber + braids_osc.coarse) << 7);
    braids_envelope[braids_slot]->noteOn();
    braids_osc.note_buffer[braids_slot] = inNumber;
  }

#ifdef MIDI_DEVICE_USB_HOST
  if (device == 1)
    midi_usb.sendNoteOn(inNumber, inVelocity, inChannel);
#endif

#ifdef MIDI_DEVICE_DIN
  if (device == 2)
  {
    midi_serial.sendNoteOn(inNumber, inVelocity, inChannel);
    //    #ifdef DEBUG
    //        LOG.print(F(" DIN OUT Channel:"));
    //        LOG.print(inChannel);
    //        LOG.println(F);
    //        LOG.print(F(" DIN inNumber:"));
    //        LOG.print(inNumber);
    //        LOG.println();
    //        LOG.print(F(" DIN inVelocity:"));
    //        LOG.print(inVelocity);
    //        LOG.println();
    //    #endif
  }
#endif

  if (device == 3) //internal Micro USB
    usbMIDI.sendNoteOn(inNumber, inVelocity, inChannel);

  if (device == 0)
  {

    // check custom midi UI KEY mapping
    if (seq.midi_learn_active == false)
    {
      for (uint8_t c = 0; c < NUM_CUSTOM_MIDI_MAPPINGS; c++)
      {
        if (inNumber == custom_midi_map[c].in && custom_midi_map[c].type == 3)
        {
          remote_MIDI_CC = custom_midi_map[c].out;
          remote_MIDI_CC_value = 127;
          break;
        }
      }
    }

#if defined(COMPILE_FOR_FLASH) || defined COMPILE_FOR_PSRAM
    // Multisamples
    for (uint8_t instance_id = 0; instance_id < NUM_MULTISAMPLES; instance_id++)
    {
      if (msp[instance_id].midi_channel == MIDI_CHANNEL_OMNI || msp[instance_id].midi_channel == inChannel)
      {
        Multi_Sample_Player(inNumber, inVelocity, instance_id);
      }
    }
#endif

    if (seq.midi_learn_active && LCDML.FUNC_getID() == LCDML.OTHER_getIDFromFunction(UI_func_custom_mappings))
      learn_key(inChannel, inNumber);

    // if (activesample < 6 && seq.running == false && LCDML.FUNC_getID() == LCDML.OTHER_getIDFromFunction(UI_func_seq_pattern_editor)) // live play pitched sample
    // {
    //   if (drum_counter >= NUM_DRUMS)
    //     drum_counter = 0;
    //   uint8_t slot = drum_get_slot(drum_config[activesample].drum_class);
    //   float pan = mapfloat(drum_config[activesample].pan, -1.0, 1.0, 0.0, 1.0);
    //   drum_mixer_r.gain(slot, (1.0 - pan) * drum_config[activesample].vol_max);
    //   drum_mixer_l.gain(slot, pan * drum_config[activesample].vol_max);
    //   drum_reverb_send_mixer_r.gain(slot, (1.0 - pan) * volume_transform(drum_config[activesample].reverb_send));
    //   drum_reverb_send_mixer_l.gain(slot, pan * volume_transform(drum_config[activesample].reverb_send));
    //   // if (drum_config[activesample].drum_data != NULL && drum_config[activesample].len > 0)
    //   // {
    //   //        Drum[slot]->enableInterpolation(true);
    //   //        Drum[slot]->setPlaybackRate(  (float)pow (2, (inNumber - 72) / 12.00) * drum_config[activesample].p_offset   );
    //   //        Drum[slot]->playRaw((int16_t*)drum_config[activesample].drum_data, drum_config[activesample].len, 1);
    //   // }
    // }

    // Ignore the note when playing & recording the same note into the sequencer
    if (seq.recording == false || (seq.recording && inNumber != seq.note_in))
    {
      // Check for MicroDexed
      for (uint8_t instance_id = 0; instance_id < NUM_DEXED; instance_id++)
      {
        if (checkMidiChannelDexed(inChannel, instance_id))
        {
          if (inNumber >= configuration.dexed[instance_id].lowest_note && inNumber <= configuration.dexed[instance_id].highest_note)
          {
            if (configuration.dexed[instance_id].polyphony > 0)
              MicroDexed[instance_id]->keydown(inNumber, uint8_t(float(configuration.dexed[instance_id].velocity_level / 127.0) * inVelocity + 0.5));

            midi_voices[instance_id]++;

            if (LCDML.FUNC_getID() == LCDML.OTHER_getIDFromFunction(UI_func_voice_select))
            {
              midi_decay_timer_dexed = 0;
              midi_decay_dexed[instance_id] = min(inVelocity / 5, 8);
            }

            // #ifdef DEBUG
            //                 char note_name[4];
            //                 getNoteName(note_name, inNumber);
            //                 LOG.print(F("Keydown "));
            //                 LOG.print(note_name);
            //                 LOG.print(F(" instance "));
            //                 LOG.print(instance_id, DEC);
            //                 LOG.print(F(" MIDI-channel "));
            //                 LOG.print(inChannel, DEC);
            //                 LOG.println();
            // #endif
            //  return;
          }
        }
      }

      // Check for MicroSynth
      for (uint8_t instance_id = 0; instance_id < NUM_MICROSYNTH; instance_id++)
      {
        // if (microsynth[instance_id].midi_channel == MIDI_CHANNEL_OMNI || microsynth[instance_id].midi_channel == inChannel) {
        // hanging notes when playing notes manually with omni - noteoff issue not fixed for now
        if (microsynth[instance_id].midi_channel == inChannel)
        {
          if (inNumber == MIDI_C8) // is noise only, mute osc
          {
            microsynth_mixer_l[instance_id].gain(1, mapfloat((microsynth[instance_id].noise_vol / 127.0) * inVelocity, SOUND_INTENSITY_MIN, SOUND_INTENSITY_MAX, 0.0f, 0.25f));
            microsynth_mixer_r[instance_id].gain(1, mapfloat((microsynth[instance_id].noise_vol / 127.0) * inVelocity, SOUND_INTENSITY_MIN, SOUND_INTENSITY_MAX, 0.0f, 0.25f));

            microsynth_mixer_l[instance_id].gain(0, 0);
            microsynth_mixer_r[instance_id].gain(0, 0);
            microsynth_noise[instance_id].amplitude(0.9);
            microsynth_envelope_noise[instance_id].noteOn();
          }
          else
          {
            if (microsynth[instance_id].trigger_noise_with_osc)
            {
              microsynth_noise[instance_id].amplitude(0.9);
              microsynth_mixer_l[instance_id].gain(1, mapfloat((microsynth[instance_id].noise_vol / 127.0) * inVelocity, SOUND_INTENSITY_MIN, SOUND_INTENSITY_MAX, 0.0f, 0.25f));
              microsynth_mixer_r[instance_id].gain(1, mapfloat((microsynth[instance_id].noise_vol / 127.0) * inVelocity, SOUND_INTENSITY_MIN, SOUND_INTENSITY_MAX, 0.0f, 0.25f));

              microsynth_envelope_noise[instance_id].noteOn();
              microsynth_mixer_l[instance_id].gain(0, mapfloat((microsynth[instance_id].sound_intensity / 127.0) * inVelocity, SOUND_INTENSITY_MIN, SOUND_INTENSITY_MAX, 0.0f, 0.15f));
              microsynth_mixer_r[instance_id].gain(0, mapfloat((microsynth[instance_id].sound_intensity / 127.0) * inVelocity, SOUND_INTENSITY_MIN, SOUND_INTENSITY_MAX, 0.0f, 0.15f));
            }
            else
            {
              //  without noise
              microsynth_mixer_l[instance_id].gain(1, 0);
              microsynth_mixer_r[instance_id].gain(1, 0);
              microsynth_noise[instance_id].amplitude(0.0);
              microsynth_mixer_l[instance_id].gain(0, mapfloat((microsynth[instance_id].sound_intensity / 127.0) * inVelocity, SOUND_INTENSITY_MIN, SOUND_INTENSITY_MAX, 0.0f, 0.15f));
              microsynth_mixer_r[instance_id].gain(0, mapfloat((microsynth[instance_id].sound_intensity / 127.0) * inVelocity, SOUND_INTENSITY_MIN, SOUND_INTENSITY_MAX, 0.0f, 0.15f));
            }
          }
          if (microsynth[instance_id].wave == 4 || microsynth[instance_id].wave == 7)
          {
            microsynth_waveform[instance_id].pulseWidth(microsynth[instance_id].pwm_from / 2000.1);
            microsynth[instance_id].pwm_current = microsynth[instance_id].pwm_from;
          }
          microsynth_filter_osc[instance_id].frequency(microsynth[instance_id].filter_osc_freq_from + (inVelocity * (microsynth[instance_id].vel_mod_filter_osc / 20.1)));
          microsynth[instance_id].filter_osc_freq_current = microsynth[instance_id].filter_osc_freq_from + (inVelocity * (microsynth[instance_id].vel_mod_filter_osc / 20.1));
          microsynth_filter_noise[instance_id].frequency(microsynth[instance_id].filter_noise_freq_from + (inVelocity * (microsynth[instance_id].vel_mod_filter_noise / 20.1)));
          microsynth[instance_id].filter_noise_freq_current = microsynth[instance_id].filter_noise_freq_from + (inVelocity * (microsynth[instance_id].vel_mod_filter_noise / 20.1));

          float noteFreq = tune_frequencies2_PGM[inNumber + microsynth[instance_id].coarse];
          float tunedFreq = 0;
          if (microsynth[instance_id].detune < 0)
          {
            float prevSemitoneFreq = tune_frequencies2_PGM[inNumber + microsynth[instance_id].coarse - 1];
            tunedFreq = (noteFreq - prevSemitoneFreq) * microsynth[instance_id].detune * 0.01;
          }
          else
          {
            float nextSemitoneFreq = tune_frequencies2_PGM[inNumber + microsynth[instance_id].coarse + 1];
            tunedFreq = (nextSemitoneFreq - noteFreq) * microsynth[instance_id].detune * 0.01;
          }
          microsynth_waveform[instance_id].frequency(noteFreq + tunedFreq);
          microsynth[instance_id].osc_freq_current = noteFreq + tunedFreq;

          microsynth_envelope_osc[instance_id].noteOn();
          microsynth_delay_timer[instance_id] = 0;
          microsynth[instance_id].lfo_fade = 0;
          if (microsynth[instance_id].lfo_mode > 0) // If LFO in 1up or 1down
            microsynth[instance_id].lfo_value = 0;
          if (LCDML.FUNC_getID() == LCDML.OTHER_getIDFromFunction(UI_func_microsynth))
          {
            midi_decay_timer_microsynth = 0;
            midi_decay_microsynth[instance_id] = min(inVelocity / 5, 8);
          }
        }
      }

      // Check for Drum
      if (inChannel == drum_midi_channel || drum_midi_channel == MIDI_CHANNEL_OMNI)
      {
        if (drum_counter >= NUM_DRUMS)
          drum_counter = 0;

        // check custom midi mapping
        for (uint8_t c = 0; c < NUM_CUSTOM_MIDI_MAPPINGS; c++)
        {
          if (inNumber == custom_midi_map[c].in && custom_midi_map[c].type == 1)
          {
            inNumber = custom_midi_map[c].out;
            break;
          }
        }

        const uint8_t d = midiNoteToDrumNote(inNumber);

#ifdef COMPILE_FOR_FLASH
        char temp_name[26];
#endif

#ifdef COMPILE_FOR_SDCARD
        char temp_name[26];
#endif
        uint8_t slot = drum_get_slot(drum_config[d].drum_class);

        if (slot != 99)
        {

          // if (drum_config[d].filter_mode == 0)
          //   Drum_filter[slot]->setLowpass(0, 20000, 0.2);
          // else if (drum_config[d].filter_mode == 1)
          //   Drum_filter[slot]->setLowpass(0, drum_config[d].filter_freq * 1000, 0.5);
          // else if (drum_config[d].filter_mode == 2)
          //   Drum_filter[slot]->setBandpass(0, drum_config[d].filter_freq * 1000, 0.5);
          // else if (drum_config[d].filter_mode == 3)
          //   Drum_filter[slot]->setHighpass(0, drum_config[d].filter_freq * 1000, 0.5);

          global_delay_in_mixer[0]->gain(slot + 8, drum_config[d].delay1 / 5);
          global_delay_in_mixer[1]->gain(slot + 8, drum_config[d].delay2 / 5);

          float pan = mapfloat(drum_config[d].pan, -1.0, 1.0, 0.0, 1.0);

          drum_mixer_r.gain(slot, (1.0 - pan) * volume_transform(mapfloat(inVelocity, 0, 127, drum_config[d].vol_min, drum_config[d].vol_max)));
          drum_mixer_l.gain(slot, pan * volume_transform(mapfloat(inVelocity, 0, 127, drum_config[d].vol_min, drum_config[d].vol_max)));

          drum_reverb_send_mixer_r.gain(slot, (1.0 - pan) * volume_transform(drum_config[d].reverb_send));
          drum_reverb_send_mixer_l.gain(slot, pan * volume_transform(drum_config[d].reverb_send));


          if (sidechain_a_active && d == sidechain_a_sample_number)
            sidechain_trigger_a = true;
          if (sidechain_b_active && d == sidechain_b_sample_number)
            sidechain_trigger_b = true;

#if defined(COMPILE_FOR_PROGMEM) || defined(COMPILE_FOR_PSRAM)
          if (drum_config[d].drum_data != NULL && drum_config[d].len > 0 && drum_config[d].len < 10000000)
          {
#endif

            if (drum_config[d].pitch != 0.0)
            {
              Drum[slot]->enableInterpolation(true);
              Drum[slot]->setPlaybackRate(drum_config[d].pitch);
            }
            Drum[slot]->setLoopType(loop_type::looptype_none);
            // test with envelopes for samples
            //sample_envelope[slot]->noteOn();

            if (sidechain_a_active && d == sidechain_a_sample_number)
              sidechain_trigger_a = true;
            if (sidechain_b_active && d == sidechain_b_sample_number)
              sidechain_trigger_b = true;

#if defined(COMPILE_FOR_PROGMEM) || defined(COMPILE_FOR_PSRAM)
            Drum[slot]->playRaw((int16_t*)drum_config[d].drum_data, drum_config[d].len, 1);
#endif

#if defined(COMPILE_FOR_FLASH)
            snprintf_P(temp_name, sizeof(temp_name), PSTR("%s.wav"), drum_config[d].filename);
            Drum[slot]->playWav(temp_name);
            // Drum[slot]->playWav("DMpop.wav");  //Test
            if (sidechain_a_active && d == sidechain_a_sample_number)
              sidechain_trigger_a = true;
            if (sidechain_b_active && d == sidechain_b_sample_number)
              sidechain_trigger_b = true;
#endif

#ifdef COMPILE_FOR_SDCARD
            strcpy(temp_name, "/DRUMS/");
            strcat(temp_name, drum_config[d].name);
            strcat(temp_name, ".wav");
            // test with envelopes for samples
            // sample_envelope[slot]->noteOn();
            Drum[slot]->playWav(temp_name);
#endif

            // #ifdef DEBUG
            //         char note_name[4];
            //         getNoteName(note_name, inNumber);
            //         LOG.print(F("=> Drum["));
            //         LOG.print(slot, DEC);
            //         LOG.print(F("]: "));
            //         LOG.println(note_name);
            // #endif

#if defined(COMPILE_FOR_PROGMEM) || defined(COMPILE_FOR_PSRAM)
          }
#endif
          // #ifdef DEBUG
          //                 LOG.print(F("Drum "));
          //                 LOG.print(drum_config[d].shortname);
          //                 LOG.print(F(" ["));
          //                 LOG.print(drum_config[d].name);
          //                 LOG.print(F("], Slot "));
          //                 LOG.print(slot);
          //                 LOG.print(F(": V"));
          //                 LOG.print(mapfloat(inVelocity, 0, 127, drum_config[d].vol_min, drum_config[d].vol_max), 2);
          //                 LOG.print(F(" P"));
          //                 LOG.print(drum_config[d].pan, 2);
          //                 LOG.print(F(" PAN"));
          //                 LOG.print(pan, 2);
          //                 LOG.print(F(" RS"));
          //                 LOG.println(drum_config[d].reverb_send, 2);
          // #endif

        }
      }

      //
      // E-Piano
      //
      if (configuration.epiano.midi_channel == MIDI_CHANNEL_OMNI || configuration.epiano.midi_channel == inChannel)
      {
        if (inNumber >= configuration.epiano.lowest_note && inNumber <= configuration.epiano.highest_note)
        {
          ep.noteOn(inNumber + configuration.epiano.transpose - 24, inVelocity);
          // #ifdef DEBUG
          //               char note_name[4];
          //               getNoteName(note_name, inNumber);
          //               LOG.print(F("KeyDown "));
          //               LOG.print(note_name);
          //               LOG.print(F(" EPIANO "));
          //               LOG.print(F(" MIDI - channel "));
          //               LOG.print(inChannel, DEC);
          //               LOG.println();
          // #endif
        }
      }
    }
  }
}

void printLoadedSample(const uint8_t i, const char* name) {
  display.setTextSize(2);
  display.setCursor(18 * CHAR_width, CHAR_height * 1);
  display.print(i);
  display.setTextSize(1);
  display.setCursor(1 * CHAR_width, CHAR_height * 3);
  display.print(name);
  display.print(F("       "));
}

void loadSample(newdigate::flashloader& loader, uint8_t i, const char* path) {
  newdigate::audiosample* sample = loader.loadSample(path);
  if (sample != nullptr) {
    psram_free_bytes -= sample->samplesize;
    // NOTE: removing wav header by incrementing 44 bytes
    static constexpr uint8_t wavHeaderLength = 44;// bytes
    sample->sampledata += wavHeaderLength / 2;    // 16bit words
    sample->samplesize -= wavHeaderLength;        // bytes
    drum_config[i].drum_data = (uint8_t*)sample->sampledata;
    drum_config[i].len = sample->samplesize / 2;  // 16bit words
  }
}

void stop_all_drum_slots()
{
  for (uint8_t i = 0; i < NUM_DRUMS; i++)
  {
    if (Drum[i]->isPlaying())
    {
      // test with envelopes for samples
     //sample_envelope[i]->noteOff();
      drum_type[i] = DRUM_NONE;
      Drum[i]->stop();
      Drum[i]->enableInterpolation(false);
      Drum[i]->setPlaybackRate(1.0);
      Drum[i]->setLoopType(loop_type::looptype_none);
    }
  }
  delay(30);
}

uint8_t drum_get_slot(uint8_t dt)
{
  bool found = false;

  for (uint8_t i = 0; i < NUM_DRUMS; i++)
  {
    if (Drum[i]->isPlaying() == false && found == false)
    {
      drum_type[i] = DRUM_NONE;
      Drum[i]->enableInterpolation(false);
      Drum[i]->setPlaybackRate(1.0);
      // Drum[i]->setLoopType(loop_type::looptype_none);
       // sample_envelope[i]->noteOff();
      found = true;
      return (i);
    }
  }

  if (found == false && drum_counter < NUM_DRUMS)
  {
    Drum[drum_counter]->stop();
    // sample_envelope[drum_counter]->noteOff();
    drum_type[drum_counter] = DRUM_NONE;
    Drum[drum_counter]->enableInterpolation(false);
    Drum[drum_counter]->setPlaybackRate(1.0);
    // Drum[drum_counter]->setLoopType(loop_type::looptype_none);
    drum_type[drum_counter % NUM_DRUMS] = dt;

    // #ifdef DEBUG
    //     LOG.print(F("Stopping sample "));
    //     LOG.print(drum_counter);
    //     LOG.print(F(" type "));
    //     LOG.println(dt);
    // #endif

    drum_counter++;
    return (drum_counter - 1 % NUM_DRUMS);
  }
  //  #ifdef DEBUG
  //    LOG.print(F("Using next free Drum slot "));
  //    LOG.println(drum_counter % NUM_DRUMS);
  //  #endif

  // do not play the sample
  return (99);
}

void handleNoteOff(byte inChannel, byte inNumber, byte inVelocity, byte device)
{

  // check custom midi UI KEY mapping
  if (seq.midi_learn_active == false)
  {
    for (uint8_t c = 0; c < NUM_CUSTOM_MIDI_MAPPINGS; c++)
    {
      if (inNumber == custom_midi_map[c].in && custom_midi_map[c].type == 3)
      {
        if (custom_midi_map[c].out == 24)
        {
          remote_MIDI_CC_value = 0;
          remote_console_keystate_select = 0;
          break;
        }
        else if (custom_midi_map[c].out == 26)
        {
          remote_MIDI_CC_value = 0;
          remote_console_keystate_b = 0;
          break;
        }
        else if (custom_midi_map[c].out == 27)
        {
          remote_MIDI_CC_value = 0;
          remote_console_keystate_a = 0;
          break;
        }
      }
    }
  }

  // following code breaks dexed release when playing live via MIDI and only when in Main Menu 15/04/2023

    // if ((seq.running == false && LCDML.FUNC_getID() == LCDML.OTHER_getIDFromFunction(UI_func_seq_pattern_editor)) || (seq.running == false && LCDML.FUNC_getID() == LCDML.OTHER_getIDFromFunction(UI_func_velocity_level)))
    // {
    //   // is in pattern editor and sequencer is not running, play the actual sound that will be used for the pattern
    //   // dexed instance 0+1,  2 = epiano , 3+4 = MicroSynth, 5-20 = MIDI OUT USB, 21-36 MIDI OUT DIN

    //   if (seq.current_track_type_of_active_pattern == 0) // drums
    //     inChannel = drum_midi_channel;
    //   else
    //   {
    //     uint8_t trk = 0;
    //     trk = seq.instrument[find_track_in_song_where_pattern_is_used(seq.active_pattern)];
    //     if (trk == 0)
    //       inChannel = configuration.dexed[0].midi_channel;
    //     else if (trk == 1)
    //       inChannel = configuration.dexed[1].midi_channel;
    //     else if (trk == 2)
    //       inChannel = configuration.epiano.midi_channel;
    //     else if (trk == 3)
    //       inChannel = microsynth[0].midi_channel;
    //     else if (trk == 4)
    //       inChannel = microsynth[1].midi_channel;
    //     else if (trk == 5)
    //       inChannel = msp[0].midi_channel;
    //     else if (trk == 6)
    //       inChannel = msp[1].midi_channel;
    //   }
    // }

  if (device == 4 || braids_osc.midi_channel == MIDI_CHANNEL_OMNI || braids_osc.midi_channel == inChannel)
  {
    for (uint8_t i = 0; i < NUM_BRAIDS; i++)
    {
      if (inNumber == braids_osc.note_buffer[i])
        braids_envelope[i]->noteOff();
    }
  }

  for (uint8_t j = 0; j < NUM_MULTISAMPLES; j++)
  {
    if (device == 5 || msp[j].midi_channel == MIDI_CHANNEL_OMNI || msp[j].midi_channel == inChannel)
    {
      // if (device == 5 || msp[j].midi_channel == inChannel) {
      for (uint8_t i = 0; i < 8; i++)
      {
        if ((inNumber == note_buffer_msp[i] && note_buffer_msp[i] > 1 && msp_playmode_sample_slot[i] != 0 && seq.ticks == msp_playmode_sample_slot[i]) ||
          (inNumber == note_buffer_msp[i] && note_buffer_msp[i] > 1))
          //  (inNumber == note_buffer_msp[i] && note_buffer_msp[i] > 1 && seq.running == false && msp_playmode_sample_slot[i] != 0))
        {
          note_buffer_msp[i] = 0;
          // test with envelopes for samples
          //sample_envelope[i]->noteOff();

          // Drum[i]->stop();///xxxyy
           // Drum[i]->enableInterpolation(false);
           // Drum[i]->setPlaybackRate(1.0);
          break;
        }
      }
    }
  }

#ifdef MIDI_DEVICE_USB_HOST
  if (device == 1)
    midi_usb.sendNoteOff(inNumber, inVelocity, inChannel);
#endif
#ifdef MIDI_DEVICE_DIN
  if (device == 2)
    midi_serial.sendNoteOff(inNumber, inVelocity, inChannel);
#endif

  if (device == 3) //internal Micro USB
    usbMIDI.sendNoteOff(inNumber, inVelocity, inChannel);

  if (device == 0)
  {
    for (uint8_t instance_id = 0; instance_id < NUM_MICROSYNTH; instance_id++)
    {
      // if (inChannel == microsynth[instance_id].midi_channel || (inChannel == MIDI_CHANNEL_OMNI && microsynth[instance_id].midi_channel==0)) {
      if (inChannel == microsynth[instance_id].midi_channel)
      {
        microsynth_envelope_osc[instance_id].noteOff();
        if (inNumber == MIDI_C8 || microsynth[instance_id].trigger_noise_with_osc) // is noise only or is osc_with_noise
          microsynth_envelope_noise[instance_id].noteOff();
      }
    }

    for (uint8_t instance_id = 0; instance_id < NUM_DEXED; instance_id++)
    {
      if (checkMidiChannelDexed(inChannel, instance_id))
      {
        if (inNumber >= configuration.dexed[instance_id].lowest_note && inNumber <= configuration.dexed[instance_id].highest_note)
        {
          if (configuration.dexed[instance_id].polyphony > 0)
            MicroDexed[instance_id]->keyup(inNumber);
          midi_voices[instance_id]--;
          // #ifdef DEBUG
          //         char note_name[4];
          //         getNoteName(note_name, inNumber);
          //         LOG.print(F("KeyUp "));
          //         LOG.print(note_name);
          //         LOG.print(F(" instance "));
          //         LOG.print(instance_id, DEC);
          //         LOG.print(F(" MIDI - channel "));
          //         LOG.print(inChannel, DEC);
          //         LOG.println();
          // #endif
        }
      }
    }

    if (configuration.epiano.midi_channel == MIDI_CHANNEL_OMNI || configuration.epiano.midi_channel == inChannel)
    {
      if (inNumber >= configuration.epiano.lowest_note && inNumber <= configuration.epiano.highest_note)
      {
        ep.noteOff(inNumber + configuration.epiano.transpose - 24);
#ifdef DEBUG
        char note_name[4];
        getNoteName(note_name, inNumber);
        LOG.print(F("KeyUp "));
        LOG.print(note_name);
        LOG.print(F(" EPIANO "));
        LOG.print(F(" MIDI - channel "));
        LOG.print(inChannel, DEC);
        LOG.println();
#endif
      }
    }
  }
}

void handleControlChange(byte inChannel, byte inCtrl, byte inValue)
{
  inCtrl = constrain(inCtrl, 0, 127);
  inValue = constrain(inValue, 0, 127);

  if (inCtrl == 115 && inValue == 127) // MIDI seq start/stop
  {
    // #ifdef DEBUG
    // LOG.printf_P(PSTR("MIDI START/STOP , ch:%d d1:%d d2:%d"),  inChannel, inCtrl, inValue);
    // #endif
    if (!seq.running)
      handleStart();
    else
      handleStop();
    //handleClock();
  }

  remote_MIDI_CC = 0;

  if (seq.midi_learn_active && LCDML.FUNC_getID() == LCDML.OTHER_getIDFromFunction(UI_func_custom_mappings))
  {
    learn_cc(inChannel, inCtrl);
  }
  else
  {

    // check custom midi mapping
    for (uint8_t c = 0; c < NUM_CUSTOM_MIDI_MAPPINGS; c++)
    {
      if (inCtrl == custom_midi_map[c].in && custom_midi_map[c].type == 2)
      {
        inCtrl = custom_midi_map[c].out;
        inChannel = custom_midi_map[c].channel;
        break;
      }
    }

    // MDT internal CC
    if (inCtrl >= 20 && inCtrl <= 31)
    {
      // case 20:  // RIGHT
      // case 21:  // LEFT
      // case 22:  // UP
      // case 23:  // DOWN
      // case 24:  // SELECT
      // // case 25: // START
      // case 26:  // BUTTON B
      // case 27:  // BUTTON A
      // case 28:  // init display at remote connection
      // case 29:  // remote touch pressed X
      // case 30:  // remote touch pressed Y
      // case 31:  // remote touch released
      remote_MIDI_CC = inCtrl;
      remote_MIDI_CC_value = inValue;
      return;
    }
    // end MDT internal CC

    for (uint8_t instance_id = 0; instance_id < NUM_DEXED; instance_id++)
    {
      if (checkMidiChannelDexed(inChannel, instance_id))
      {
#ifdef DEBUG
        LOG.print(F("INSTANCE "));
        LOG.print(instance_id, DEC);
        LOG.print(F(": CC#"));
        LOG.print(inCtrl, DEC);
        LOG.print(F(": "));
        LOG.println(inValue, DEC);
#endif
        switch (inCtrl)
        {
        case 0: // BankSelect MSB
#ifdef DEBUG
          LOG.println(F("BANK - SELECT MSB CC"));
#endif
          configuration.dexed[instance_id].bank = constrain((inValue << 7) & configuration.dexed[instance_id].bank, 0, MAX_BANKS - 1);
          /* load_sd_voice(configuration.dexed[instance_id].pool, configuration.dexed[instance_id].bank, configuration.dexed[instance_id].voice, instance_id);
            if (LCDML.FUNC_getID() == LCDML.OTHER_getIDFromFunction(UI_func_voice_select))
            {
            LCDML.OTHER_updateFunc();
            LCDML.loop_menu();
            } */
          break;
        case 1:
#ifdef DEBUG
          LOG.println(F("MODWHEEL CC"));
#endif
          MicroDexed[instance_id]->setModWheel(inValue);
          MicroDexed[instance_id]->ControllersRefresh();
          break;
        case 2:
#ifdef DEBUG
          LOG.println(F("BREATH CC"));
#endif
          MicroDexed[instance_id]->setBreathController(inValue);
          MicroDexed[instance_id]->ControllersRefresh();
          break;
        case 4:
#ifdef DEBUG
          LOG.println(F("FOOT CC"));
#endif
          MicroDexed[instance_id]->setFootController(inValue);
          MicroDexed[instance_id]->ControllersRefresh();
          break;
        case 5: // Portamento time
          configuration.dexed[instance_id].portamento_time = inValue;
          MicroDexed[instance_id]->setPortamento(configuration.dexed[instance_id].portamento_mode, configuration.dexed[instance_id].portamento_glissando, configuration.dexed[instance_id].portamento_time);
          break;
        case 7: // Instance Volume
#ifdef DEBUG
          LOG.println(F("VOLUME CC"));
#endif
          configuration.dexed[instance_id].sound_intensity = map(inValue, 0, 127, SOUND_INTENSITY_MIN, SOUND_INTENSITY_MAX);
          MicroDexed[instance_id]->setGain(midi_volume_transform(map(configuration.dexed[instance_id].sound_intensity, SOUND_INTENSITY_MIN, SOUND_INTENSITY_MAX, 0, 127)));
          if (LCDML.FUNC_getID() == LCDML.OTHER_getIDFromFunction(UI_func_sound_intensity))
          {
            LCDML.OTHER_updateFunc();
            LCDML.loop_menu();
          }
          break;
        case 10: // Pan
#ifdef DEBUG
          LOG.println(F("PANORAMA CC"));
#endif
          configuration.dexed[instance_id].pan = map(inValue, 0, 0x7f, PANORAMA_MIN, PANORAMA_MAX);
          dexed_mono2stereo[instance_id]->panorama(mapfloat(configuration.dexed[instance_id].pan, PANORAMA_MIN, PANORAMA_MAX, -1.0, 1.0));
          if (LCDML.FUNC_getID() == LCDML.OTHER_getIDFromFunction(UI_func_dexed_audio))
          {
            LCDML.OTHER_updateFunc();
            LCDML.loop_menu();
          }
          break;

        case 32: // BankSelect LSB
#ifdef DEBUG
          LOG.println(F("BANK - SELECT LSB CC"));
#endif
          configuration.dexed[instance_id].bank = constrain(inValue, 0, MAX_BANKS - 1);
          /*load_sd_voice(configuration.dexed[instance_id].pool, configuration.dexed[instance_id].bank, configuration.dexed[instance_id].voice, instance_id);
            if (LCDML.FUNC_getID() == LCDML.OTHER_getIDFromFunction(UI_func_voice_select))
            {
            LCDML.OTHER_updateFunc();
            LCDML.loop_menu();
            }*/
          break;

        case 64:
          MicroDexed[instance_id]->setSustain(inValue > 63);
          if (!MicroDexed[instance_id]->getSustain())
          {
            for (uint8_t note = 0; note < MicroDexed[instance_id]->getMaxNotes(); note++)
            {
              if (MicroDexed[instance_id]->voices[note].sustained && !MicroDexed[instance_id]->voices[note].keydown)
              {
                MicroDexed[instance_id]->voices[note].dx7_note->keyup();
                MicroDexed[instance_id]->voices[note].sustained = false;
              }
            }
          }
          break;
        case 65:
          MicroDexed[instance_id]->setPortamento(configuration.dexed[instance_id].portamento_mode, configuration.dexed[instance_id].portamento_glissando, configuration.dexed[instance_id].portamento_time);
          if (LCDML.FUNC_getID() == LCDML.OTHER_getIDFromFunction(UI_func_dexed_setup))
          {
            LCDML.OTHER_updateFunc();
            LCDML.loop_menu();
          }
          break;
        case 94: // CC 94: (de)tune
          configuration.dexed[selected_instance_id].tune = map(inValue, 0, 0x7f, TUNE_MIN, TUNE_MAX);
          MicroDexed[selected_instance_id]->setMasterTune((int((configuration.dexed[selected_instance_id].tune - 100) / 100.0 * 0x4000) << 11) * (1.0 / 12));
          MicroDexed[selected_instance_id]->doRefreshVoice();
          if (LCDML.FUNC_getID() == LCDML.OTHER_getIDFromFunction(UI_func_dexed_setup))
          {
            LCDML.OTHER_updateFunc();
            LCDML.loop_menu();
          }
          break;
        case 91: // CC 91: reverb send
          configuration.fx.reverb_send[selected_instance_id] = map(inValue, 0, 0x7f, REVERB_SEND_MIN, REVERB_SEND_MAX);
          reverb_mixer_r.gain(selected_instance_id, volume_transform(mapfloat(configuration.fx.reverb_send[selected_instance_id], REVERB_SEND_MIN, REVERB_SEND_MAX, 0.0, VOL_MAX_FLOAT)));
          reverb_mixer_l.gain(selected_instance_id, volume_transform(mapfloat(configuration.fx.reverb_send[selected_instance_id], REVERB_SEND_MIN, REVERB_SEND_MAX, 0.0, VOL_MAX_FLOAT)));
          // if (LCDML.FUNC_getID() == LCDML.OTHER_getIDFromFunction(UI_func_reverb_send)) {
          //   LCDML.OTHER_updateFunc();
          //   LCDML.loop_menu();
          // }
          break;
        case 93: // CC 93: chorus level
          configuration.fx.chorus_level[selected_instance_id] = map(inValue, 0, 0x7f, CHORUS_LEVEL_MIN, CHORUS_LEVEL_MAX);
          dexed_chorus_mixer_r[selected_instance_id].gain(1, mapfloat(configuration.fx.chorus_level[selected_instance_id], CHORUS_LEVEL_MIN, CHORUS_LEVEL_MAX, 0.0, 0.5));
          dexed_chorus_mixer_l[selected_instance_id].gain(1, mapfloat(configuration.fx.chorus_level[selected_instance_id], CHORUS_LEVEL_MIN, CHORUS_LEVEL_MAX, 0.0, 0.5));
          if (LCDML.FUNC_getID() == LCDML.OTHER_getIDFromFunction(UI_func_chorus_level))
          {
            LCDML.OTHER_updateFunc();
            LCDML.loop_menu();
          }
          break;
        case 103: // CC 103: filter resonance
          configuration.fx.filter_resonance[instance_id] = map(inValue, 0, 0x7f, FILTER_RESONANCE_MIN, FILTER_RESONANCE_MAX);
          MicroDexed[instance_id]->setFilterResonance(mapfloat(configuration.fx.filter_resonance[instance_id], FILTER_RESONANCE_MIN, FILTER_RESONANCE_MAX, 1.0, 0.0));
          if (LCDML.FUNC_getID() == LCDML.OTHER_getIDFromFunction(UI_func_filter_resonance))
          {
            LCDML.OTHER_updateFunc();
            LCDML.loop_menu();
          }
          break;
        case 104: // CC 104: filter cutoff
          configuration.fx.filter_cutoff[instance_id] = map(inValue, 0, 0x7f, FILTER_CUTOFF_MIN, FILTER_CUTOFF_MAX);
          MicroDexed[instance_id]->setFilterCutoff(mapfloat(configuration.fx.filter_cutoff[instance_id], FILTER_CUTOFF_MIN, FILTER_CUTOFF_MAX, 1.0, 0.0));
          ;
          if (LCDML.FUNC_getID() == LCDML.OTHER_getIDFromFunction(UI_func_filter_cutoff))
          {
            LCDML.OTHER_updateFunc();
            LCDML.loop_menu();
          }
          break;
        case 105: // CC 105: delay time
          configuration.fx.delay_time[instance_id] = map(inValue, 0, 0x7f, DELAY_TIME_MIN, DELAY_TIME_MAX);
          delay_fx[instance_id]->delay(0, constrain(configuration.fx.delay_time[instance_id] * 10, DELAY_TIME_MIN * 10, DELAY_TIME_MAX * 10));
          // if (LCDML.FUNC_getID() == LCDML.OTHER_getIDFromFunction(UI_func_delay_time)) {
          //   LCDML.OTHER_updateFunc();
          //   LCDML.loop_menu();
          // }
          break;
        case 106: // CC 106: delay feedback
          configuration.fx.delay_feedback[instance_id] = map(inValue, 0, 0x7f, DELAY_FEEDBACK_MIN, DELAY_FEEDBACK_MAX);
          delay_fb_mixer[instance_id]->gain(1, mapfloat(configuration.fx.delay_feedback[instance_id], DELAY_FEEDBACK_MIN, DELAY_FEEDBACK_MAX, 0, 0.7));
          // delay_fb_mixer[instance_id]->gain(1, midi_volume_transform(map(configuration.fx.delay_feedback[instance_id], DELAY_FEEDBACK_MIN, DELAY_FEEDBACK_MAX, 0, 127))); // amount of feedback

          // if (LCDML.FUNC_getID() == LCDML.OTHER_getIDFromFunction(UI_func_delay_feedback)) {
          //   LCDML.OTHER_updateFunc();
          //   LCDML.loop_menu();
          // }
          break;
        case 107: // CC 107: delay volume
          configuration.fx.delay_level[instance_id] = map(inValue, 0, 0x7f, DELAY_LEVEL_MIN, DELAY_LEVEL_MAX);
          global_delay_in_mixer[instance_id]->gain(0, mapfloat(configuration.fx.delay_level[instance_id], DELAY_LEVEL_MIN, DELAY_LEVEL_MAX, 0, 0.9));
          break;
        case 120:
          MicroDexed[instance_id]->panic();
          break;
        case 121:
          MicroDexed[instance_id]->resetControllers();
          break;
        case 123:
          MicroDexed[instance_id]->notesOff();
          break;
        case 126:
          if (inValue > 0)
            MicroDexed[instance_id]->setMonoMode(true);
          else
            MicroDexed[instance_id]->setMonoMode(false);
          break;
        case 127:
          if (inValue > 0)
            MicroDexed[instance_id]->setMonoMode(true);
          else
            MicroDexed[instance_id]->setMonoMode(false);
          break;
        case 200: // CC 200: seq start/stop
          if (!seq.running)
            handleStart();
          // else
          // handleStop();
          break;
        case 201: // CC 201: seq stop
          if (seq.running)
            handleStop();
          break;
        case 202: // CC 202: seq record
          if (seq.running)
            seq.running = true;
          seq.recording = true;
          seq.note_in = 0;
          break;
        case 203: // CC 203: dexed panic
          MicroDexed[0]->panic();
          MicroDexed[1]->panic();
          break;
        }
      }
    }
  }

  if (configuration.epiano.midi_channel == MIDI_CHANNEL_OMNI || configuration.epiano.midi_channel == inChannel)
    ep.processMidiController(inCtrl, inValue);
}

void handleAfterTouch(byte inChannel, byte inPressure)
{
  for (uint8_t instance_id = 0; instance_id < NUM_DEXED; instance_id++)
  {
    if (checkMidiChannelDexed(inChannel, instance_id))
    {
      MicroDexed[instance_id]->setAftertouch(inPressure);
      MicroDexed[instance_id]->ControllersRefresh();
    }
  }
}

void handlePitchBend(byte inChannel, int16_t inPitch)
{
  for (uint8_t instance_id = 0; instance_id < NUM_DEXED; instance_id++)
  {
    if (checkMidiChannelDexed(inChannel, instance_id))
    {
      MicroDexed[instance_id]->setPitchbend(inPitch);
    }
  }
}

void handleProgramChange(byte inChannel, byte inProgram)
{
  for (uint8_t instance_id = 0; instance_id < NUM_DEXED; instance_id++)
  {
    if (checkMidiChannelDexed(inChannel, instance_id))
    {
      configuration.dexed[instance_id].voice = constrain(inProgram, 0, MAX_VOICES - 1);
      load_sd_voice(configuration.dexed[instance_id].pool, configuration.dexed[instance_id].bank, configuration.dexed[instance_id].voice, instance_id);

      if (LCDML.FUNC_getID() == LCDML.OTHER_getIDFromFunction(UI_func_voice_select))
      {
        LCDML.OTHER_updateFunc();
        LCDML.loop_menu();
      }
    }
  }
}

void handleSystemExclusive(byte* sysex, unsigned len)
{
  for (uint8_t instance_id = 0; instance_id < NUM_DEXED; instance_id++)
  {
    if (!checkMidiChannelDexed((sysex[2] & 0x0f) + 1, instance_id))
    {
#ifdef DEBUG
      LOG.print(F("INSTANCE "));
      LOG.print(instance_id, DEC);
      LOG.println(F(": SYSEX - MIDI - Channel mismatch"));
#endif
      return;
    }

#ifdef DEBUG
    LOG.print(F("SysEx data length: ["));
    LOG.print(len);
    LOG.println(F("]"));

    LOG.println(F("SysEx data: "));
    for (uint16_t i = 0; i < len; i++)
    {
      LOG.print(F("[0x"));
      uint8_t s = sysex[i];
      if (s < 16)
        LOG.print(F("0"));
      LOG.print(s, HEX);
      LOG.print(F(" | "));
      if (s < 100)
        LOG.print(F("0"));
      if (s < 10)
        LOG.print(F("0"));
      LOG.print(s, DEC);
      LOG.print(F("]"));
      if ((i + 1) % 16 == 0)
        LOG.println();
    }
    LOG.println();
#endif

    // Check for SYSEX end byte
    if (sysex[len - 1] != 0xf7)
    {
#ifdef DEBUG
      LOG.println(F("E: SysEx end status byte not detected."));
#endif
      return;
    }

    if (sysex[1] != 0x43) // check for Yamaha sysex
    {
#ifdef DEBUG
      LOG.println(F("E: SysEx vendor not Yamaha."));
#endif
      return;
    }

#ifdef DEBUG
    LOG.print(F("Substatus: ["));
    LOG.print((sysex[2] & 0x70) >> 4);
    LOG.println(F("]"));
#endif

    // parse parameter change
    if (len == 7)
    {
      if (((sysex[3] & 0x7c) >> 2) != 0 && ((sysex[3] & 0x7c) >> 2) != 2)
      {
#ifdef DEBUG
        LOG.println(F("E: Not a SysEx parameter or function parameter change."));
#endif
        return;
      }

      sysex[4] &= 0x7f;
      sysex[5] &= 0x7f;

      if ((sysex[3] & 0x7c) >> 2 == 0)
      {
#ifdef DEBUG
        LOG.println(F("SysEx Voice parameter: "));
        LOG.print(F("Parameter #"));
        LOG.print(sysex[4] + ((sysex[3] & 0x03) * 128), DEC);
        LOG.print(F(" Value: "));
        LOG.println(sysex[5], DEC);
#endif
        MicroDexed[instance_id]->setVoiceDataElement(sysex[4] + ((sysex[3] & 0x03) * 128), sysex[5]);
      }
      else if ((sysex[3] & 0x7c) >> 2 == 2)
      {
#ifdef DEBUG
        LOG.println(F("SysEx Function parameter: "));
        LOG.print(F("Parameter #"));
        LOG.print(sysex[4], DEC);
        LOG.print(F(" Value: "));
        LOG.println(sysex[5], DEC);
#endif
        switch (sysex[4])
        {
        case 65:
          configuration.dexed[instance_id].pb_range = constrain(sysex[5], PB_RANGE_MIN, PB_RANGE_MAX);
          MicroDexed[instance_id]->setPitchbendRange(configuration.dexed[instance_id].pb_range);
          if (LCDML.FUNC_getID() == LCDML.OTHER_getIDFromFunction(UI_func_dexed_controllers))
          {
            LCDML.OTHER_updateFunc();
            LCDML.loop_menu();
          }
          break;
        case 66:
          configuration.dexed[instance_id].pb_step = constrain(sysex[5], PB_STEP_MIN, PB_STEP_MAX);
          MicroDexed[instance_id]->setPitchbendRange(configuration.dexed[instance_id].pb_step);
          if (LCDML.FUNC_getID() == LCDML.OTHER_getIDFromFunction(UI_func_dexed_controllers))
          {
            LCDML.OTHER_updateFunc();
            LCDML.loop_menu();
          }
          break;
        case 67:
          configuration.dexed[instance_id].portamento_mode = constrain(sysex[5], PORTAMENTO_MODE_MIN, PORTAMENTO_MODE_MAX);
          MicroDexed[instance_id]->setPortamento(configuration.dexed[instance_id].portamento_mode, configuration.dexed[instance_id].portamento_glissando, configuration.dexed[instance_id].portamento_time);
          if (LCDML.FUNC_getID() == LCDML.OTHER_getIDFromFunction(UI_func_dexed_setup))
          {
            LCDML.OTHER_updateFunc();
            LCDML.loop_menu();
          }
          break;
        case 68:
          configuration.dexed[instance_id].portamento_glissando = constrain(sysex[5], PORTAMENTO_GLISSANDO_MIN, PORTAMENTO_GLISSANDO_MAX);
          MicroDexed[instance_id]->setPortamento(configuration.dexed[instance_id].portamento_mode, configuration.dexed[instance_id].portamento_glissando, configuration.dexed[instance_id].portamento_time);
          if (LCDML.FUNC_getID() == LCDML.OTHER_getIDFromFunction(UI_func_dexed_setup))
          {
            LCDML.OTHER_updateFunc();
            LCDML.loop_menu();
          }
          break;
        case 69:
          configuration.dexed[instance_id].portamento_time = constrain(sysex[5], PORTAMENTO_TIME_MIN, PORTAMENTO_TIME_MAX);
          MicroDexed[instance_id]->setPortamento(configuration.dexed[instance_id].portamento_mode, configuration.dexed[instance_id].portamento_glissando, configuration.dexed[instance_id].portamento_time);
          if (LCDML.FUNC_getID() == LCDML.OTHER_getIDFromFunction(UI_func_dexed_setup))
          {
            LCDML.OTHER_updateFunc();
            LCDML.loop_menu();
          }
          break;
        case 70:
          configuration.dexed[instance_id].mw_range = constrain(sysex[5], MW_RANGE_MIN, MW_RANGE_MAX);
          MicroDexed[instance_id]->setModWheelRange(configuration.dexed[instance_id].mw_range);
          if (LCDML.FUNC_getID() == LCDML.OTHER_getIDFromFunction(UI_func_dexed_controllers))
          {
            LCDML.OTHER_updateFunc();
            LCDML.loop_menu();
          }
          break;
        case 71:
          configuration.dexed[instance_id].mw_assign = constrain(sysex[5], MW_ASSIGN_MIN, MW_ASSIGN_MAX);
          MicroDexed[instance_id]->setModWheelTarget(configuration.dexed[instance_id].mw_assign);
          if (LCDML.FUNC_getID() == LCDML.OTHER_getIDFromFunction(UI_func_dexed_controllers))
          {
            LCDML.OTHER_updateFunc();
            LCDML.loop_menu();
          }
          break;
        case 72:
          configuration.dexed[instance_id].fc_range = constrain(sysex[5], FC_RANGE_MIN, FC_RANGE_MAX);
          MicroDexed[instance_id]->setFootControllerRange(configuration.dexed[instance_id].fc_range);
          if (LCDML.FUNC_getID() == LCDML.OTHER_getIDFromFunction(UI_func_dexed_controllers))
          {
            LCDML.OTHER_updateFunc();
            LCDML.loop_menu();
          }
          break;
        case 73:
          configuration.dexed[instance_id].fc_assign = constrain(sysex[5], FC_ASSIGN_MIN, FC_ASSIGN_MAX);
          MicroDexed[instance_id]->setFootControllerTarget(configuration.dexed[instance_id].fc_assign);
          if (LCDML.FUNC_getID() == LCDML.OTHER_getIDFromFunction(UI_func_dexed_controllers))
          {
            LCDML.OTHER_updateFunc();
            LCDML.loop_menu();
          }
          break;
        case 74:
          configuration.dexed[instance_id].bc_range = constrain(sysex[5], BC_RANGE_MIN, BC_RANGE_MAX);
          MicroDexed[instance_id]->setBreathControllerRange(configuration.dexed[instance_id].bc_range);
          if (LCDML.FUNC_getID() == LCDML.OTHER_getIDFromFunction(UI_func_dexed_controllers))
          {
            LCDML.OTHER_updateFunc();
            LCDML.loop_menu();
          }
          break;
        case 75:
          configuration.dexed[instance_id].bc_assign = constrain(sysex[5], BC_ASSIGN_MIN, BC_ASSIGN_MAX);
          MicroDexed[instance_id]->setBreathControllerTarget(configuration.dexed[instance_id].bc_assign);
          if (LCDML.FUNC_getID() == LCDML.OTHER_getIDFromFunction(UI_func_dexed_controllers))
          {
            LCDML.OTHER_updateFunc();
            LCDML.loop_menu();
          }
          break;
        case 76:
          configuration.dexed[instance_id].at_range = constrain(sysex[5], AT_RANGE_MIN, AT_RANGE_MAX);
          MicroDexed[instance_id]->setAftertouchRange(configuration.dexed[instance_id].at_range);
          if (LCDML.FUNC_getID() == LCDML.OTHER_getIDFromFunction(UI_func_dexed_controllers))
          {
            LCDML.OTHER_updateFunc();
            LCDML.loop_menu();
          }
          break;
        case 77:
          configuration.dexed[instance_id].at_assign = constrain(sysex[5], AT_ASSIGN_MIN, AT_ASSIGN_MAX);
          MicroDexed[instance_id]->setAftertouchTarget(configuration.dexed[instance_id].at_assign);
          if (LCDML.FUNC_getID() == LCDML.OTHER_getIDFromFunction(UI_func_dexed_controllers))
          {
            LCDML.OTHER_updateFunc();
            LCDML.loop_menu();
          }
          break;
        default:
          MicroDexed[instance_id]->setVoiceDataElement(sysex[4], sysex[5]); // set function parameter
          break;
        }
        MicroDexed[instance_id]->ControllersRefresh();
      }
#ifdef DEBUG
      else
      {
        LOG.println(F("E: Unknown SysEx voice or function."));
      }
#endif
    }
    else if (len == 163)
    {
      int32_t bulk_checksum_calc = 0;
      int8_t bulk_checksum = sysex[161];

      // 1 Voice bulk upload
#ifdef DEBUG
      LOG.println(F("One Voice bulk upload"));
#endif

      if ((sysex[3] & 0x7f) != 0)
      {
#ifdef DEBUG
        LOG.println(F("E: Not a SysEx voice bulk upload."));
#endif
        return;
      }

      if (((sysex[4] << 7) | sysex[5]) != 0x9b)
      {
#ifdef DEBUG
        LOG.println(F("E: Wrong length for SysEx voice bulk upload (not 155)."));
#endif
        return;
      }

      // checksum calculation
      for (uint8_t i = 0; i < 155; i++)
      {
        bulk_checksum_calc -= sysex[i + 6];
      }
      bulk_checksum_calc &= 0x7f;

      if (bulk_checksum_calc != bulk_checksum)
      {
#ifdef DEBUG
        LOG.print(F("E : Checksum error for one voice [0x"));
        LOG.print(bulk_checksum, HEX);
        LOG.print(F(" / 0x"));
        LOG.print(bulk_checksum_calc, HEX);
        LOG.println(F("]"));
#endif
        return;
      }

      // fix voice name
      for (uint8_t i = 0; i < 10; i++)
      {
        if (sysex[151 + i] > 126) // filter characters
          sysex[151 + i] = 32;
      }

      // load sysex-data into voice memory
      MicroDexed[instance_id]->loadVoiceParameters(&sysex[6]);

#ifdef DEBUG
      show_patch(instance_id);
#endif

      // show voice name
      strncpy(g_voice_name[instance_id], (char*)&sysex[151], VOICE_NAME_LEN - 1);

      if (LCDML.FUNC_getID() == LCDML.OTHER_getIDFromFunction(UI_func_voice_select))
      {
        LCDML.OTHER_updateFunc();
        LCDML.loop_menu();
      }
    }
    else if (len == 4104)
    {
      if (strlen(receive_bank_filename) > 0 && LCDML.FUNC_getID() == LCDML.OTHER_getIDFromFunction(UI_func_sysex_receive_bank))
      {
        int32_t bulk_checksum_calc = 0;
        int8_t bulk_checksum = sysex[4102];

        // 1 Bank bulk upload
        if ((sysex[3] & 0x7f) != 9)
        {
#ifdef DEBUG
          LOG.println(F("E : Not a SysEx bank bulk upload."));
#endif
          setCursor_textGrid(1, 2);
          display.print(F("Error (TYPE)      "));
          delay(MESSAGE_WAIT_TIME);
          LCDML.FUNC_goBackToMenu();
          return;
        }

#ifdef DEBUG
        LOG.println(F("Bank bulk upload."));
#endif

        if (((sysex[4] << 7) | sysex[5]) != 0x1000)
        {
#ifdef DEBUG
          LOG.println(F("E : Wrong length for SysEx bank bulk upload (not 4096)."));
#endif
          setCursor_textGrid(1, 2);
          display.print(F("Error (SIZE)     "));
          delay(MESSAGE_WAIT_TIME);
          LCDML.FUNC_goBackToMenu();
          return;
        }

#ifdef DEBUG
        LOG.println(F("Bank type ok"));
#endif

        // checksum calculation
        for (uint16_t i = 0; i < 4096; i++)
        {
          bulk_checksum_calc -= sysex[i + 6];
        }
        bulk_checksum_calc &= 0x7f;

        if (bulk_checksum_calc != bulk_checksum)
        {
#ifdef DEBUG
          LOG.print(F("E : Checksum error for bank [0x"));
          LOG.print(bulk_checksum, HEX);
          LOG.print(F(" / 0x"));
          LOG.print(bulk_checksum_calc, HEX);
          LOG.println(F("]"));
#endif
          setCursor_textGrid(1, 2);
          display.print(F("Error (CHECKSUM)"));
          delay(MESSAGE_WAIT_TIME);
          LCDML.FUNC_goBackToMenu();
          return;
        }

#ifdef DEBUG
        LOG.println(F("Bank checksum ok"));
#endif

        if (save_sd_bank(receive_bank_filename, sysex))
        {
#ifdef DEBUG
          LOG.print(F("Bank saved as ["));
          LOG.print(receive_bank_filename);
          LOG.println(F("]"));
#endif
          setCursor_textGrid(1, 2);
          display.print(F("Done.           "));
          delay(MESSAGE_WAIT_TIME);
          LCDML.FUNC_goBackToMenu();
        }
        else
        {
#ifdef DEBUG
          LOG.println(F("Error during saving bank as ["));
          LOG.print(receive_bank_filename);
          LOG.println(F("]"));
#endif
          setCursor_textGrid(1, 2);
          display.print(F("Error.          "));
          delay(MESSAGE_WAIT_TIME);
          LCDML.FUNC_goBackToMenu();
        }
        memset(receive_bank_filename, 0, FILENAME_LEN);
      }
#ifdef DEBUG
      else
        LOG.println(F("E : Not in MIDI receive bank mode."));
#endif
    }
#ifdef DEBUG
    else
      LOG.println(F("E : SysEx parameter length wrong."));
#endif
  }
}

void handleTimeCodeQuarterFrame(byte data)
{
  ;
}

void handleAfterTouchPoly(byte inChannel, byte inNumber, byte inVelocity)
{
  ;
}

void handleSongSelect(byte inSong)
{
  ;
}

void handleTuneRequest(void)
{
  ;
}

void send_midi_clock()
{
#ifdef MIDI_DEVICE_USB_HOST
  midi_usb.sendRealTime(midi::Clock);
#endif

#ifdef MIDI_DEVICE_DIN
  midi_serial.sendRealTime(midi::Clock);
#endif

#ifdef MIDI_DEVICE_USB
  usbMIDI.sendRealTime(midi::Clock);
#endif


}

void handleClock(void)
{

  if (seq.clock == 1)// MIDI CLOCK TIMING, MIDI SLAVE

  {
    sequencer();

    // if (midi_bpm_counter % 24 == 0)  // too slow to adapt to tempo changes for liveseq
    if (midi_bpm_counter % 12 == 0) // go for every 1/8 instead for every 1/4 note (MIDI CLOCK 24 BEATS PER 1/4 NOTE)
    {
      midi_bpm = (round(60000.0f / float(midi_bpm_timer * 2)));
      // midi_bpm = ( 60000.0f / float(midi_bpm_timer)) + 0.01f);
      // if (_midi_bpm > -1 && _midi_bpm != midi_bpm)

      if ((_midi_bpm > midi_bpm + 2 && midi_bpm > 30 && midi_bpm < 180) ||
        (_midi_bpm < midi_bpm - 2 && midi_bpm >30 && midi_bpm < 180))  //needs some tolerance in bpm - otherwise delay recalculation will come up every step and cause audible clicks
        // tried muting them and fading them back but that makes it worse since constant retriggering mutes them constantly and also makes clicking

      {
        // #ifdef DEBUG
        //       LOG.print(F("---------------------------------------MIDI Clock : "));
        //       LOG.print(midi_bpm);
        //       LOG.print(F(" bpm ("));
        //       LOG.print(midi_bpm_timer, DEC);
        //       LOG.println(F("ms per quarter)"));
        // #endif
        seq.bpm = midi_bpm;
        _midi_bpm = midi_bpm;
        update_seq_speed();
      }

      midi_bpm_timer = 0;
      midi_bpm_counter = 0;
    }
    midi_bpm_counter++;
  }
}

FLASHMEM void dac_mute(void)
{
#ifdef TEENSY_AUDIO_BOARD
  sgtl5000.lineOutLevel(0.0);
  sgtl5000.dacVolume(0.0);
  sgtl5000.volume(0.0, 0.0); // Headphone volume
#endif
#ifdef PCM5102_MUTE_PIN
  digitalWrite(PCM5102_MUTE_PIN, LOW);
#endif
  seq.DAC_mute_state = true;
}

FLASHMEM void dac_unmute(void)
{
#ifdef TEENSY_AUDIO_BOARD
  sgtl5000.lineOutLevel(SGTL5000_LINEOUT_LEVEL);
  sgtl5000.dacVolume(1.0);
  sgtl5000.volume(SGTL5000_HEADPHONE_VOLUME, SGTL5000_HEADPHONE_VOLUME); // Headphone volume
#endif
#ifdef PCM5102_MUTE_PIN
  digitalWrite(PCM5102_MUTE_PIN, HIGH);
#endif
  seq.DAC_mute_state = false;
}

LiveSequencer liveSeq;

void handleStart(void)
{
  if (LCDML.FUNC_getID() != LCDML.OTHER_getIDFromFunction(UI_func_information))
  {
    midi_bpm_timer = 0;
    midi_bpm_counter = 0;

    seq.step = 0;
    seq.arp_note = 0;
    seq.arp_chord = 0;

    mb_set_mutes();
    mb_set_compressor();
    mb_set_master();

    if (seq.loop_start == 99) // no loop start set, start at step 0
      seq.current_song_step = 0;
    else
      seq.current_song_step = seq.loop_start;

    // MIDI SYNC: use timer only when internal or MIDI Master
    if (seq.clock == 0 || seq.clock == 2)
    {
      liveSeq.onStarted();
      liveSeq.handlePatternBegin();
      sequencer_timer.begin(sequencer, seq.tempo_ms / (seq.ticks_max + 1));
    }

    seq.running = true;

#ifdef MIDI_DEVICE_USB_HOST
    midi_usb.sendRealTime(midi::Start);
#endif

#ifdef MIDI_DEVICE_DIN
    midi_serial.sendRealTime(midi::Start);
#endif

#ifdef MIDI_DEVICE_USB
    usbMIDI.sendRealTime(midi::Start);
#endif

    if (LCDML.FUNC_getID() == LCDML.OTHER_getIDFromFunction(UI_func_arpeggio))
    {
      print_arp_start_stop_button();
    }
    else if (LCDML.FUNC_getID() == 255 && ts.keyb_in_menu_activated == false) // when in main menu
      draw_button_on_grid(45, 18, "SEQ.", "STOP", 2);
  }
  if (LCDML.FUNC_getID() == LCDML.OTHER_getIDFromFunction(UI_func_seq_pattern_editor) || LCDML.FUNC_getID() == LCDML.OTHER_getIDFromFunction(UI_func_seq_vel_editor))
  {
    display.fillRect(36 * CHAR_width_small, CHAR_height_small, button_size_x * CHAR_width_small, CHAR_height_small * button_size_y, COLOR_BACKGROUND); // clear scope
  }
  else if (LCDML.FUNC_getID() == LCDML.OTHER_getIDFromFunction(UI_func_drums))
  {
    display.fillRect(174, 14, DISPLAY_WIDTH - 174 - 11, 38, COLOR_BACKGROUND);
  }

}

void handleContinue(void)
{
  ;
}

void handleStop(void)
{
  if (LCDML.FUNC_getID() != LCDML.OTHER_getIDFromFunction(UI_func_information))
  {
    liveSeq.onStopped();
    if (seq.running)
    {
      sequencer_part2();
      if (LCDML.FUNC_getID() == LCDML.OTHER_getIDFromFunction(UI_func_seq_pattern_editor))
      {
        draw_button_on_grid(36, 1, "STEP", "RECORD", 1); // print step recorder icon
      }
      else if (LCDML.FUNC_getID() == LCDML.OTHER_getIDFromFunction(UI_func_seq_vel_editor))
      {
        display.fillRect(36 * CHAR_width_small, CHAR_height_small, button_size_x * CHAR_width_small, CHAR_height_small * button_size_y, COLOR_BACKGROUND); // clear scope
      }

      MicroDexed[0]->panic();
#if NUM_DEXED > 1
      MicroDexed[1]->panic();
#endif

      if (seq.clock == 0 || seq.clock == 2)
        sequencer_timer.stop();

      if (LCDML.FUNC_getID() == LCDML.OTHER_getIDFromFunction(UI_func_drums))
      {
        display.fillRect(174, 14, DISPLAY_WIDTH - 174 - 11, 38, COLOR_BACKGROUND);
      }
      else if (LCDML.FUNC_getID() == LCDML.OTHER_getIDFromFunction(UI_func_song))
      {
        clear_song_playhead();
        sub_song_print_tracknumbers();
      }
    }

    seq.running = false;
    seq.recording = false;
    seq.note_in = 0;
    seq.step = 0;
    midi_bpm_counter = 0;
    midi_bpm_timer = 0;
    seq.arp_step = 0;
    seq.ticks = 0;
    for (uint8_t d = 0; d < NUM_SEQ_TRACKS; d++)
    {
      seq.chain_counter[d] = 0;
      seq.current_pattern[d] = 99;
    }
    if (seq.loop_start == 99) // no loop start set, start at step 0
      seq.current_song_step = 0;
    else
      seq.current_song_step = seq.loop_start;

    microsynth_envelope_osc[0].noteOff();
    microsynth_envelope_osc[1].noteOff();
    microsynth_envelope_noise[0].noteOff();
    microsynth_envelope_noise[1].noteOff();

    for (uint8_t i = 0; i < NUM_BRAIDS; i++)
    {
      if (braids_envelope[i]->isActive())
        braids_envelope[i]->noteOff();
    }

#ifdef MIDI_DEVICE_USB_HOST
    midi_usb.sendRealTime(midi::Stop);
#endif

#ifdef MIDI_DEVICE_DIN
    midi_serial.sendRealTime(midi::Stop);
#endif

#ifdef MIDI_DEVICE_USB
    usbMIDI.sendRealTime(midi::Stop);
#endif

    if (LCDML.FUNC_getID() == LCDML.OTHER_getIDFromFunction(UI_func_arpeggio))
    {
      print_arp_start_stop_button();
    }
    else if (LCDML.FUNC_getID() == 255 && ts.keyb_in_menu_activated == false) // when in main menu
      draw_button_on_grid(45, 18, "SEQ.", "START", 1);
  }
}

void handleActiveSensing(void)
{
  ;
}

void handleSystemReset(void)
{
  for (uint8_t instance_id = 0; instance_id < NUM_DEXED; instance_id++)
  {
#ifdef DEBUG
    LOG.println(F("MIDI SYSEX RESET"));
#endif
    MicroDexed[instance_id]->notesOff();
    MicroDexed[instance_id]->panic();
    MicroDexed[instance_id]->resetControllers();
  }
}

/******************************************************************************
  MIDI HELPER
******************************************************************************/
bool checkMidiChannelDexed(byte inChannel, uint8_t instance_id)
{
  // check for MIDI channel
  if (configuration.dexed[instance_id].midi_channel == MIDI_CHANNEL_OMNI)
  {
    return (true);
  }
  else if (inChannel != configuration.dexed[instance_id].midi_channel)
  {
    // #ifdef DEBUG
    //     LOG.print(F("INSTANCE "));
    //     LOG.print(instance_id, DEC);
    //     LOG.print(F(" : Ignoring MIDI data on channel "));
    //     LOG.print(inChannel);
    //     LOG.print(F("(listening on "));
    //     LOG.print(configuration.dexed[instance_id].midi_channel);
    //     LOG.println(F(")"));
    // #endif

    return (false);
  }
  return (true);
}

FLASHMEM void init_MIDI_send_CC(void)
{
#ifdef DEBUG
  LOG.println(F("init_MIDI_send_CC(): "));
#endif
  MD_sendControlChange(configuration.dexed[selected_instance_id].midi_channel, 7, configuration.dexed[selected_instance_id].sound_intensity);
  MD_sendControlChange(configuration.dexed[selected_instance_id].midi_channel, 10, configuration.dexed[selected_instance_id].pan);
  MD_sendControlChange(configuration.dexed[selected_instance_id].midi_channel, 91, configuration.fx.reverb_send[selected_instance_id]);
  MD_sendControlChange(configuration.dexed[selected_instance_id].midi_channel, 93, configuration.fx.chorus_level[selected_instance_id]);
  MD_sendControlChange(configuration.dexed[selected_instance_id].midi_channel, 94, configuration.dexed[selected_instance_id].tune);
  MD_sendControlChange(configuration.dexed[selected_instance_id].midi_channel, 103, configuration.fx.filter_resonance[selected_instance_id]);
  MD_sendControlChange(configuration.dexed[selected_instance_id].midi_channel, 104, configuration.fx.filter_cutoff[selected_instance_id]);
  MD_sendControlChange(configuration.dexed[selected_instance_id].midi_channel, 105, configuration.fx.delay_time[selected_instance_id]);
  MD_sendControlChange(configuration.dexed[selected_instance_id].midi_channel, 106, configuration.fx.delay_feedback[selected_instance_id]);
  MD_sendControlChange(configuration.dexed[selected_instance_id].midi_channel, 107, configuration.fx.delay_level[selected_instance_id]);
}

/******************************************************************************
  VOLUME HELPER
******************************************************************************/

FLASHMEM void set_drums_volume(float vol)
{
  master_mixer_r.gain(MASTER_MIX_CH_DRUMS, vol);
  master_mixer_l.gain(MASTER_MIX_CH_DRUMS, vol);
}

static float VolumeToAmplification(int volume)
//Volume in the Range 0..100
{
  /*
     https://www.dr-lex.be/info-stuff/volumecontrols.html

     Dynamic range   a           b       Approximation
     50 dB         3.1623e-3     5.757       x^3
     60 dB         1e-3          6.908       x^4
     70 dB         3.1623e-4     8.059       x^5
     80 dB         1e-4          9.210       x^6
     90 dB         3.1623e-5     10.36       x^6
     100 dB        1e-5          11.51       x^7
 */

  float x = volume / 100.0f;

  float a = 0.00397; //2
  float b = 5.53;
  float ampl = a * expf(b * x);
  if (x < 0.001f) ampl = 0.0;

  return ampl;
}

FLASHMEM void set_volume(uint8_t v, uint8_t m)
{

  if (configuration.sys.vol > 100)
    configuration.sys.vol = 100;

  configuration.sys.mono = m;

#ifdef DEBUG
  LOG.print(F("Setting volume : VOL = "));
  LOG.println(v, DEC);
  LOG.print(F(" V = "));
  LOG.println(volume_transform(v / 100.0));
#endif

  // volume_r.gain(volume_transform(tmp_v / 100.0) * VOLUME_MULTIPLIER);
  // volume_l.gain(volume_transform(tmp_v / 100.0) * VOLUME_MULTIPLIER);

  volume_r.gain(VolumeToAmplification(configuration.sys.vol));
  volume_l.gain(VolumeToAmplification(configuration.sys.vol));

  switch (m)
  {
  case 0: // stereo
    stereo2mono.stereo(true);
    for (uint8_t instance_id = 0; instance_id < NUM_DEXED; instance_id++)
      dexed_mono2stereo[instance_id]->panorama(mapfloat(configuration.dexed[instance_id].pan, PANORAMA_MIN, PANORAMA_MAX, -1.0, 1.0));
    break;
  case 1: // mono both
    stereo2mono.stereo(false);
    for (uint8_t instance_id = 0; instance_id < NUM_DEXED; instance_id++)
      dexed_mono2stereo[instance_id]->panorama(mapfloat(PANORAMA_DEFAULT, PANORAMA_MIN, PANORAMA_MAX, -1.0, 1.0));
    break;
  case 2: // mono right
    volume_l.gain(0.0);
    stereo2mono.stereo(false);
    for (uint8_t instance_id = 0; instance_id < NUM_DEXED; instance_id++)
      dexed_mono2stereo[instance_id]->panorama(mapfloat(PANORAMA_MAX, PANORAMA_MIN, PANORAMA_MAX, -1.0, 1.0));
    break;
  case 3: // mono left
    volume_r.gain(0.0);
    stereo2mono.stereo(false);
    for (uint8_t instance_id = 0; instance_id < NUM_DEXED; instance_id++)
      dexed_mono2stereo[instance_id]->panorama(mapfloat(PANORAMA_MIN, PANORAMA_MIN, PANORAMA_MAX, -1.0, 1.0));
    break;
  }
}

/******************************************************************************
  SD HELPER
******************************************************************************/

FLASHMEM void initial_values(bool init)
{
  init_configuration();

  load_sd_sys_json();
  if (bootup_performance_loading)
  {
    uint8_t perfNumber = STARTUP_NUM_DEFAULT;
    if (configuration.sys.load_at_startup_performance == 255)
    {
#ifdef DEBUG
      LOG.print(F("Loading initial system data from performance "));
      LOG.println(configuration.sys.performance_number, DEC);
#endif
      perfNumber = configuration.sys.performance_number;
    }
    else if (configuration.sys.load_at_startup_performance < 100)
    {
#ifdef DEBUG
      LOG.print(F("Loading initial system data from performance "));
      LOG.println(configuration.sys.load_at_startup_performance, DEC);
#endif
      perfNumber = configuration.sys.load_at_startup_performance;
    }
    else
    {
#ifdef DEBUG
      LOG.print(F("Loading initial system data from default performance "));
      LOG.println(STARTUP_NUM_DEFAULT, DEC);
#endif
    }

    load_performance_and_check_midi(perfNumber);

#ifdef DEBUG
    LOG.println(F("OK, loaded!"));
#endif
  }
  else
  { // boot without performance - set some defaults
    configuration.dexed[0].midi_channel = DEFAULT_DEXED_MIDI_CHANNEL_INST0;
    configuration.dexed[1].midi_channel = DEFAULT_DEXED_MIDI_CHANNEL_INST1;
    // configuration.epiano.midi_channel = constrain(configuration.epiano.midi_channel + 1, 0, 16);
    microsynth[0].midi_channel = DEFAULT_MICROSYNTH_MIDI_CHANNEL_INST0;
    microsynth[1].midi_channel = DEFAULT_MICROSYNTH_MIDI_CHANNEL_INST1;
    braids_osc.midi_channel = DEFAULT_BRAIDS_MIDI_CHANNEL;
    msp[0].midi_channel = DEFAULT_MSP_MIDI_CHANNEL_INST0;
    msp[1].midi_channel = DEFAULT_MSP_MIDI_CHANNEL_INST1;
    // drum_midi_channel = constrain(drum_midi_channel + 1, 0, 16);
  }
  check_configuration();
  set_volume(configuration.sys.vol, configuration.sys.mono);
  mb_set_mutes();
  mb_set_compressor();
  mb_set_master();

#ifdef DEBUG
  show_configuration();
#endif
}

FLASHMEM void check_configuration(void)
{
  check_configuration_sys();
  check_configuration_fx();
  for (uint8_t instance_id = 0; instance_id < NUM_DEXED; instance_id++)
    check_configuration_dexed(instance_id);
  check_configuration_epiano();
}

FLASHMEM void check_configuration_sys(void)
{
  configuration.sys.vol = constrain(configuration.sys.vol, VOLUME_MIN, VOLUME_MAX);
  configuration.sys.mono = constrain(configuration.sys.mono, MONO_MIN, MONO_MAX);
  configuration.sys.soft_midi_thru = constrain(configuration.sys.soft_midi_thru, SOFT_MIDI_THRU_MIN, SOFT_MIDI_THRU_MAX);
  configuration.sys.favorites = constrain(configuration.sys.favorites, FAVORITES_NUM_MIN, FAVORITES_NUM_MAX);
  configuration.sys.performance_number = constrain(configuration.sys.performance_number, PERFORMANCE_NUM_MIN, PERFORMANCE_NUM_MAX);
  configuration.sys.load_at_startup_performance = constrain(configuration.sys.load_at_startup_performance, STARTUP_NUM_MIN, STARTUP_NUM_MAX);
  configuration.sys.display_rotation = constrain(configuration.sys.display_rotation, 0, 3);
  configuration.sys.touch_rotation = constrain(configuration.sys.touch_rotation, 0, 3);
  configuration.sys.ui_reverse = constrain(configuration.sys.ui_reverse, false, true);
  configuration.sys.boot_anim_skip = constrain(configuration.sys.boot_anim_skip, false, true);

  setup_screensaver();

  configuration.sys.gamepad_speed = constrain(configuration.sys.gamepad_speed, 60, 500);
}

FLASHMEM void check_configuration_fx(void)
{

  configuration.fx.reverb_lowpass = constrain(configuration.fx.reverb_lowpass, REVERB_LOWPASS_MIN, REVERB_LOWPASS_MAX);
  configuration.fx.reverb_lodamp = constrain(configuration.fx.reverb_lodamp, REVERB_LODAMP_MIN, REVERB_LODAMP_MAX);
  configuration.fx.reverb_hidamp = constrain(configuration.fx.reverb_hidamp, REVERB_HIDAMP_MIN, REVERB_HIDAMP_MAX);
  configuration.fx.reverb_diffusion = constrain(configuration.fx.reverb_diffusion, REVERB_DIFFUSION_MIN, REVERB_DIFFUSION_MAX);

  configuration.fx.reverb_roomsize = constrain(configuration.fx.reverb_roomsize, REVERB_ROOMSIZE_MIN, REVERB_ROOMSIZE_MAX);
  configuration.fx.reverb_level = constrain(configuration.fx.reverb_level, REVERB_LEVEL_MIN, REVERB_LEVEL_MAX);

  for (uint8_t instance_id = 0; instance_id < NUM_DEXED; instance_id++)
  {
    configuration.fx.filter_cutoff[instance_id] = constrain(configuration.fx.filter_cutoff[instance_id], FILTER_CUTOFF_MIN, FILTER_CUTOFF_MAX);
    configuration.fx.filter_resonance[instance_id] = constrain(configuration.fx.filter_resonance[instance_id], FILTER_RESONANCE_MIN, FILTER_RESONANCE_MAX);
    configuration.fx.chorus_frequency[instance_id] = constrain(configuration.fx.chorus_frequency[instance_id], CHORUS_FREQUENCY_MIN, CHORUS_FREQUENCY_MAX);
    configuration.fx.chorus_waveform[instance_id] = constrain(configuration.fx.chorus_waveform[instance_id], CHORUS_WAVEFORM_MIN, CHORUS_WAVEFORM_MAX);
    configuration.fx.chorus_depth[instance_id] = constrain(configuration.fx.chorus_depth[instance_id], CHORUS_DEPTH_MIN, CHORUS_DEPTH_MAX);
    configuration.fx.chorus_level[instance_id] = constrain(configuration.fx.chorus_level[instance_id], CHORUS_LEVEL_MIN, CHORUS_LEVEL_MAX);
    if (configuration.fx.delay_sync[instance_id] == 0)
      configuration.fx.delay_time[instance_id] = constrain(configuration.fx.delay_time[instance_id], DELAY_TIME_MIN, DELAY_TIME_MAX);
    configuration.fx.delay_feedback[instance_id] = constrain(configuration.fx.delay_feedback[instance_id], DELAY_FEEDBACK_MIN, DELAY_FEEDBACK_MAX);
    configuration.fx.delay_level[instance_id] = constrain(configuration.fx.delay_level[instance_id], DELAY_LEVEL_MIN, DELAY_LEVEL_MAX);
    configuration.fx.delay_level_global[instance_id] = constrain(configuration.fx.delay_level_global[instance_id], DELAY_LEVEL_MIN, DELAY_LEVEL_MAX);
    configuration.fx.delay_pan[instance_id] = constrain(configuration.fx.delay_pan[instance_id], PANORAMA_MIN, PANORAMA_MAX);
    configuration.fx.delay_sync[instance_id] = constrain(configuration.fx.delay_sync[instance_id], DELAY_SYNC_MIN, DELAY_SYNC_MAX);
    configuration.fx.reverb_send[instance_id] = constrain(configuration.fx.reverb_send[instance_id], REVERB_SEND_MIN, REVERB_SEND_MAX);
  }

  configuration.fx.ep_chorus_frequency = constrain(configuration.fx.ep_chorus_frequency, EP_CHORUS_FREQUENCY_MIN, EP_CHORUS_FREQUENCY_MAX);
  configuration.fx.ep_chorus_waveform = constrain(configuration.fx.ep_chorus_waveform, EP_CHORUS_WAVEFORM_MIN, EP_CHORUS_WAVEFORM_MAX);
  configuration.fx.ep_chorus_depth = constrain(configuration.fx.ep_chorus_depth, EP_CHORUS_DEPTH_MIN, EP_CHORUS_DEPTH_MAX);
  configuration.fx.ep_chorus_level = constrain(configuration.fx.ep_chorus_level, EP_CHORUS_LEVEL_MIN, EP_CHORUS_LEVEL_MAX);
  configuration.fx.ep_reverb_send = constrain(configuration.fx.ep_reverb_send, REVERB_SEND_MIN, REVERB_SEND_MAX);
}

FLASHMEM void check_configuration_dexed(uint8_t instance_id)
{
  configuration.dexed[instance_id].bank = constrain(configuration.dexed[instance_id].bank, 0, MAX_BANKS - 1);
  configuration.dexed[instance_id].voice = constrain(configuration.dexed[instance_id].voice, 0, MAX_VOICES - 1);
  configuration.dexed[instance_id].midi_channel = constrain(configuration.dexed[instance_id].midi_channel, MIDI_CHANNEL_MIN, MIDI_CHANNEL_MAX);
  configuration.dexed[instance_id].lowest_note = constrain(configuration.dexed[instance_id].lowest_note, INSTANCE_LOWEST_NOTE_MIN, INSTANCE_LOWEST_NOTE_MAX);
  configuration.dexed[instance_id].highest_note = constrain(configuration.dexed[instance_id].highest_note, INSTANCE_HIGHEST_NOTE_MIN, INSTANCE_HIGHEST_NOTE_MAX);
  configuration.dexed[instance_id].sound_intensity = constrain(configuration.dexed[instance_id].sound_intensity, SOUND_INTENSITY_MIN, SOUND_INTENSITY_MAX);
  configuration.dexed[instance_id].pan = constrain(configuration.dexed[instance_id].pan, PANORAMA_MIN, PANORAMA_MAX);
  configuration.dexed[instance_id].transpose = constrain(configuration.dexed[instance_id].transpose, TRANSPOSE_MIN, TRANSPOSE_MAX);
  configuration.dexed[instance_id].tune = constrain(configuration.dexed[instance_id].tune, TUNE_MIN, TUNE_MAX);
  configuration.dexed[instance_id].polyphony = constrain(configuration.dexed[instance_id].polyphony, POLYPHONY_MIN, POLYPHONY_MAX);
  configuration.dexed[instance_id].velocity_level = constrain(configuration.dexed[instance_id].velocity_level, VELOCITY_LEVEL_MIN, VELOCITY_LEVEL_MAX);
  configuration.dexed[instance_id].monopoly = constrain(configuration.dexed[instance_id].monopoly, MONOPOLY_MIN, MONOPOLY_MAX);
  configuration.dexed[instance_id].note_refresh = constrain(configuration.dexed[instance_id].note_refresh, NOTE_REFRESH_MIN, NOTE_REFRESH_MAX);
  configuration.dexed[instance_id].pb_range = constrain(configuration.dexed[instance_id].pb_range, PB_RANGE_MIN, PB_RANGE_MAX);
  configuration.dexed[instance_id].pb_step = constrain(configuration.dexed[instance_id].pb_step, PB_STEP_MIN, PB_STEP_MAX);
  configuration.dexed[instance_id].mw_range = constrain(configuration.dexed[instance_id].mw_range, MW_RANGE_MIN, MW_RANGE_MAX);
  configuration.dexed[instance_id].mw_assign = constrain(configuration.dexed[instance_id].mw_assign, MW_ASSIGN_MIN, MW_ASSIGN_MAX);
  configuration.dexed[instance_id].mw_mode = constrain(configuration.dexed[instance_id].mw_mode, MW_MODE_MIN, MW_MODE_MAX);
  configuration.dexed[instance_id].fc_range = constrain(configuration.dexed[instance_id].fc_range, FC_RANGE_MIN, FC_RANGE_MAX);
  configuration.dexed[instance_id].fc_assign = constrain(configuration.dexed[instance_id].fc_assign, FC_ASSIGN_MIN, FC_ASSIGN_MAX);
  configuration.dexed[instance_id].fc_mode = constrain(configuration.dexed[instance_id].fc_mode, FC_MODE_MIN, FC_MODE_MAX);
  configuration.dexed[instance_id].bc_range = constrain(configuration.dexed[instance_id].bc_range, BC_RANGE_MIN, BC_RANGE_MAX);
  configuration.dexed[instance_id].bc_assign = constrain(configuration.dexed[instance_id].bc_assign, BC_ASSIGN_MIN, BC_ASSIGN_MAX);
  configuration.dexed[instance_id].bc_mode = constrain(configuration.dexed[instance_id].bc_mode, BC_MODE_MIN, BC_MODE_MAX);
  configuration.dexed[instance_id].at_range = constrain(configuration.dexed[instance_id].at_range, AT_RANGE_MIN, AT_RANGE_MAX);
  configuration.dexed[instance_id].at_assign = constrain(configuration.dexed[instance_id].at_assign, AT_ASSIGN_MIN, AT_ASSIGN_MAX);
  configuration.dexed[instance_id].at_mode = constrain(configuration.dexed[instance_id].at_mode, AT_MODE_MIN, AT_MODE_MAX);
  configuration.dexed[instance_id].portamento_mode = constrain(configuration.dexed[instance_id].portamento_mode, PORTAMENTO_MODE_MIN, PORTAMENTO_MODE_MAX);
  configuration.dexed[instance_id].portamento_glissando = constrain(configuration.dexed[instance_id].portamento_glissando, PORTAMENTO_GLISSANDO_MIN, PORTAMENTO_GLISSANDO_MAX);
  configuration.dexed[instance_id].portamento_time = constrain(configuration.dexed[instance_id].portamento_time, PORTAMENTO_TIME_MIN, PORTAMENTO_TIME_MAX);
  configuration.dexed[instance_id].op_enabled = constrain(configuration.dexed[instance_id].op_enabled, OP_ENABLED_MIN, OP_ENABLED_MAX);
}

FLASHMEM void check_configuration_epiano(void)
{
  configuration.epiano.decay = constrain(configuration.epiano.decay, EP_DECAY_MIN, EP_DECAY_MAX);
  configuration.epiano.release = constrain(configuration.epiano.release, EP_RELEASE_MIN, EP_RELEASE_MAX);
  configuration.epiano.hardness = constrain(configuration.epiano.hardness, EP_HARDNESS_MIN, EP_HARDNESS_MAX);
  configuration.epiano.treble = constrain(configuration.epiano.treble, EP_TREBLE_MIN, EP_TREBLE_MAX);
  configuration.epiano.pan_tremolo = constrain(configuration.epiano.pan_tremolo, EP_PAN_TREMOLO_MIN, EP_PAN_TREMOLO_MAX);
  configuration.epiano.pan_lfo = constrain(configuration.epiano.pan_lfo, EP_PAN_LFO_MIN, EP_PAN_LFO_MAX);
  configuration.epiano.velocity_sense = constrain(configuration.epiano.velocity_sense, EP_VELOCITY_SENSE_MIN, EP_VELOCITY_SENSE_MAX);
  configuration.epiano.stereo = constrain(configuration.epiano.stereo, EP_STEREO_MIN, EP_STEREO_MAX);
  configuration.epiano.polyphony = constrain(configuration.epiano.polyphony, EP_POLYPHONY_MIN, EP_POLYPHONY_MAX);
  configuration.epiano.tune = constrain(configuration.epiano.tune, EP_TUNE_MIN, EP_TUNE_MAX);
  configuration.epiano.detune = constrain(configuration.epiano.detune, EP_DETUNE_MIN, EP_DETUNE_MAX);
  configuration.epiano.overdrive = constrain(configuration.epiano.overdrive, EP_OVERDRIVE_MIN, EP_OVERDRIVE_MAX);
  configuration.epiano.lowest_note = constrain(configuration.epiano.lowest_note, EP_LOWEST_NOTE_MIN, EP_LOWEST_NOTE_MAX);
  configuration.epiano.highest_note = constrain(configuration.epiano.highest_note, EP_HIGHEST_NOTE_MIN, EP_HIGHEST_NOTE_MAX);
  configuration.epiano.transpose = constrain(configuration.epiano.transpose, EP_TRANSPOSE_MIN, EP_TRANSPOSE_MAX);
  configuration.epiano.sound_intensity = constrain(configuration.epiano.sound_intensity, SOUND_INTENSITY_MIN, SOUND_INTENSITY_MAX);
  configuration.epiano.pan = constrain(configuration.epiano.pan, PANORAMA_MIN, PANORAMA_MAX);
  configuration.epiano.velocity_sense = constrain(configuration.epiano.velocity_sense, EP_VELOCITY_SENSE_MIN, EP_VELOCITY_SENSE_MAX);
  configuration.epiano.midi_channel = constrain(configuration.epiano.midi_channel, EP_MIDI_CHANNEL_MIN, EP_MIDI_CHANNEL_MAX);
}

FLASHMEM void init_configuration(void)
{
#ifdef DEBUG
  LOG.println(F("INITIALIZING CONFIGURATION"));
#endif

  configuration.sys.vol = VOLUME_DEFAULT;
  configuration.sys.mono = MONO_DEFAULT;
  configuration.sys.soft_midi_thru = SOFT_MIDI_THRU_DEFAULT;
  configuration.sys.performance_number = PERFORMANCE_NUM_DEFAULT;
  configuration.sys.load_at_startup_performance = STARTUP_NUM_DEFAULT;
  configuration.sys.display_rotation = DISPLAY_ROTATION_DEFAULT;
  configuration.sys.touch_rotation = TOUCH_ROTATION_DEFAULT;
  configuration.sys.ui_reverse = false;
  configuration.sys.boot_anim_skip = false;
  configuration.sys.screen_saver_start = SCREEN_SAVER_START_DEFAULT;
  configuration.sys.screen_saver_mode = SCREEN_SAVER_MODE_DEFAULT;
  configuration.sys.gamepad_speed = GAMEPAD_SPEED_DEFAULT;
  configuration.fx.reverb_lowpass = REVERB_LOWPASS_DEFAULT;
  configuration.fx.reverb_lodamp = REVERB_LODAMP_DEFAULT;
  configuration.fx.reverb_hidamp = REVERB_HIDAMP_DEFAULT;
  configuration.fx.reverb_diffusion = REVERB_DIFFUSION_DEFAULT;
  configuration.fx.reverb_roomsize = REVERB_ROOMSIZE_DEFAULT;
  configuration.fx.reverb_level = REVERB_LEVEL_DEFAULT;

  configuration.fx.ep_chorus_frequency = EP_CHORUS_FREQUENCY_DEFAULT;
  configuration.fx.ep_chorus_waveform = EP_CHORUS_WAVEFORM_DEFAULT;
  configuration.fx.ep_chorus_depth = EP_CHORUS_DEPTH_DEFAULT;
  configuration.fx.ep_chorus_level = EP_CHORUS_LEVEL_DEFAULT;
  configuration.fx.ep_reverb_send = REVERB_SEND_DEFAULT;

  for (uint8_t instance_id = 0; instance_id < NUM_DEXED; instance_id++)
  {
    configuration.dexed[instance_id].bank = SYSEXBANK_DEFAULT;
    configuration.dexed[instance_id].voice = SYSEXSOUND_DEFAULT;
    configuration.dexed[instance_id].midi_channel = DEFAULT_MIDI_CHANNEL;
    configuration.dexed[instance_id].lowest_note = INSTANCE_LOWEST_NOTE_MIN;
    configuration.dexed[instance_id].highest_note = INSTANCE_HIGHEST_NOTE_MAX;
    configuration.dexed[instance_id].sound_intensity = SOUND_INTENSITY_DEFAULT;
    configuration.dexed[instance_id].pan = PANORAMA_DEFAULT;
    configuration.dexed[instance_id].transpose = TRANSPOSE_DEFAULT;
    configuration.dexed[instance_id].tune = TUNE_DEFAULT;
    configuration.dexed[instance_id].polyphony = POLYPHONY_DEFAULT;
    configuration.dexed[instance_id].velocity_level = VELOCITY_LEVEL_DEFAULT;
    configuration.dexed[instance_id].monopoly = MONOPOLY_DEFAULT;
    configuration.dexed[instance_id].note_refresh = NOTE_REFRESH_DEFAULT;
    configuration.dexed[instance_id].pb_range = PB_RANGE_DEFAULT;
    configuration.dexed[instance_id].pb_step = PB_STEP_DEFAULT;
    configuration.dexed[instance_id].mw_range = MW_RANGE_DEFAULT;
    configuration.dexed[instance_id].mw_assign = MW_ASSIGN_DEFAULT;
    configuration.dexed[instance_id].mw_mode = MW_MODE_DEFAULT;
    configuration.dexed[instance_id].fc_range = FC_RANGE_DEFAULT;
    configuration.dexed[instance_id].fc_assign = FC_ASSIGN_DEFAULT;
    configuration.dexed[instance_id].fc_mode = FC_MODE_DEFAULT;
    configuration.dexed[instance_id].bc_range = BC_RANGE_DEFAULT;
    configuration.dexed[instance_id].bc_assign = BC_ASSIGN_DEFAULT;
    configuration.dexed[instance_id].bc_mode = BC_MODE_DEFAULT;
    configuration.dexed[instance_id].at_range = AT_RANGE_DEFAULT;
    configuration.dexed[instance_id].at_assign = AT_ASSIGN_DEFAULT;
    configuration.dexed[instance_id].at_mode = AT_MODE_DEFAULT;
    configuration.dexed[instance_id].portamento_mode = PORTAMENTO_MODE_DEFAULT;
    configuration.dexed[instance_id].portamento_glissando = PORTAMENTO_GLISSANDO_DEFAULT;
    configuration.dexed[instance_id].portamento_time = PORTAMENTO_TIME_DEFAULT;
    configuration.dexed[instance_id].op_enabled = OP_ENABLED_DEFAULT;

    configuration.fx.filter_cutoff[instance_id] = FILTER_CUTOFF_DEFAULT;
    configuration.fx.filter_resonance[instance_id] = FILTER_RESONANCE_DEFAULT;
    configuration.fx.chorus_frequency[instance_id] = CHORUS_FREQUENCY_DEFAULT;
    configuration.fx.chorus_waveform[instance_id] = CHORUS_WAVEFORM_DEFAULT;
    configuration.fx.chorus_depth[instance_id] = CHORUS_DEPTH_DEFAULT;
    configuration.fx.chorus_level[instance_id] = CHORUS_LEVEL_DEFAULT;
    configuration.fx.delay_time[instance_id] = DELAY_TIME_DEFAULT / 10;
    configuration.fx.delay_feedback[instance_id] = DELAY_FEEDBACK_DEFAULT;
    configuration.fx.delay_level[instance_id] = DELAY_LEVEL_DEFAULT;
    configuration.fx.delay_level_global[instance_id] = DELAY_LEVEL_GLOBAL_DEFAULT;
    configuration.fx.delay_sync[instance_id] = DELAY_SYNC_DEFAULT;
    configuration.fx.reverb_send[instance_id] = REVERB_SEND_DEFAULT;

    MicroDexed[instance_id]->ControllersRefresh();
  }

  configuration.epiano.decay = EP_DECAY_DEFAULT;
  configuration.epiano.release = EP_RELEASE_DEFAULT;
  configuration.epiano.hardness = EP_HARDNESS_DEFAULT;
  configuration.epiano.treble = EP_TREBLE_DEFAULT;
  configuration.epiano.pan_tremolo = EP_PAN_TREMOLO_DEFAULT;
  configuration.epiano.pan_lfo = EP_PAN_LFO_DEFAULT;
  configuration.epiano.velocity_sense = EP_VELOCITY_SENSE_DEFAULT;
  configuration.epiano.stereo = EP_STEREO_DEFAULT;
  configuration.epiano.polyphony = EP_POLYPHONY_DEFAULT;
  configuration.epiano.tune = EP_TUNE_DEFAULT;
  configuration.epiano.detune = EP_DETUNE_DEFAULT;
  configuration.epiano.overdrive = EP_OVERDRIVE_DEFAULT;
  configuration.epiano.lowest_note = EP_LOWEST_NOTE_DEFAULT;
  configuration.epiano.highest_note = EP_HIGHEST_NOTE_DEFAULT;
  configuration.epiano.transpose = EP_TRANSPOSE_DEFAULT;
  configuration.epiano.sound_intensity = SOUND_INTENSITY_DEFAULT;
  configuration.epiano.pan = PANORAMA_DEFAULT;
  configuration.epiano.velocity_sense = EP_VELOCITY_SENSE_DEFAULT;
  configuration.epiano.midi_channel = EP_MIDI_CHANNEL_DEFAULT;

  set_volume(configuration.sys.vol, configuration.sys.mono);
}

/******************************************************************************
  PARAMETER-HELPERS
******************************************************************************/

void set_sample_pitch(uint8_t sample, float playbackspeed)
{
  drum_config[sample].pitch = playbackspeed;
}

void set_sample_p_offset(uint8_t sample, float s_offset)
{
  drum_config[sample].p_offset = s_offset;
}

void set_sample_pan(uint8_t sample, float s_pan)
{
  drum_config[sample].pan = s_pan;
}

void set_sample_vol_max(uint8_t sample, float s_max)
{
  drum_config[sample].vol_max = s_max;
}

void set_sample_vol_min(uint8_t sample, float s_min)
{
  drum_config[sample].vol_min = s_min;
}

void set_sample_reverb_send(uint8_t sample, float s_reverb)
{
  drum_config[sample].reverb_send = s_reverb;
}

void set_sample_filter_mode(uint8_t sample, uint8_t s_filter_mode)
{
  drum_config[sample].filter_mode = s_filter_mode;
}

void set_sample_filter_freq(uint8_t sample, float s_freq)
{
  if (s_freq>2.195)
  s_freq=2.195;
  drum_config[sample].filter_freq = s_freq;
}

void set_sample_filter_q(uint8_t sample, float s_q)
{
  drum_config[sample].filter_q = s_q;
}

void set_sample_delay1(uint8_t sample, float s_d1)
{
  drum_config[sample].delay1 = s_d1;
}
void set_sample_delay2(uint8_t sample, float s_d2)
{
  drum_config[sample].delay2 = s_d2;
}

uint8_t get_sample_note(uint8_t sample)
{
  return (drum_config[sample].midinote);
}
float get_sample_pitch(uint8_t sample)
{
  return (drum_config[sample].pitch);
}
float get_sample_p_offset(uint8_t sample)
{
  return (drum_config[sample].p_offset);
}
float get_sample_pan(uint8_t sample)
{
  return (drum_config[sample].pan);
}
float get_sample_vol_max(uint8_t sample)
{
  return (drum_config[sample].vol_max);
}
float get_sample_vol_min(uint8_t sample)
{
  return (drum_config[sample].vol_min);
}
float get_sample_reverb_send(uint8_t sample)
{
  return (drum_config[sample].reverb_send);
}
uint8_t get_sample_filter_mode(uint8_t sample)
{
  return (drum_config[sample].filter_mode);
}
float get_sample_filter_freq(uint8_t sample)
{
  return (drum_config[sample].filter_freq);
}
float get_sample_filter_q(uint8_t sample)
{
  return (drum_config[sample].filter_q);
}
float get_sample_delay1(uint8_t sample)
{
  return (drum_config[sample].delay1);
}
float get_sample_delay2(uint8_t sample)
{
  return (drum_config[sample].delay2);
}


FLASHMEM void set_fx_params(void)
{
  for (uint8_t instance_id = 0; instance_id < NUM_DEXED; instance_id++)
  {
    // CHORUS
    switch (configuration.fx.chorus_waveform[instance_id])
    {
    case 0:
      chorus_modulator[instance_id]->begin(WAVEFORM_TRIANGLE);
      break;
    case 1:
      chorus_modulator[instance_id]->begin(WAVEFORM_SINE);
      break;
    default:
      chorus_modulator[instance_id]->begin(WAVEFORM_TRIANGLE);
    }
    chorus_modulator[instance_id]->phase(0);

    chorus_modulator[instance_id]->frequency(configuration.fx.chorus_frequency[instance_id] / 10.0);
    chorus_modulator[instance_id]->amplitude(mapfloat(configuration.fx.chorus_depth[instance_id], CHORUS_DEPTH_MIN, CHORUS_DEPTH_MAX, 0.0, 1.0));

    chorus_modulator[instance_id]->offset(0.0);

    // DELAY

    delay_mixer[instance_id]->gain(1, mapfloat(configuration.fx.delay_level_global[instance_id], DELAY_LEVEL_MIN, DELAY_LEVEL_MAX, 0.0, 0.9));
    delay_mono2stereo[instance_id]->panorama(mapfloat(configuration.fx.delay_pan[instance_id], PANORAMA_MIN, PANORAMA_MAX, -1.0, 1.0));
    delay_fb_mixer[instance_id]->gain(1, mapfloat(configuration.fx.delay_feedback[instance_id], DELAY_FEEDBACK_MIN, DELAY_FEEDBACK_MAX, 0.0, 0.8));

    if (configuration.fx.delay_level[instance_id] <= DELAY_LEVEL_MIN)
      delay_fx[instance_id]->disable(0);
    else if (configuration.fx.delay_sync[instance_id] == 0)
      delay_fx[instance_id]->delay(0, constrain(configuration.fx.delay_time[instance_id], DELAY_TIME_MIN, DELAY_TIME_MAX) * 10);

    if (configuration.fx.delay_multiplier[instance_id] == 0)
      configuration.fx.delay_multiplier[instance_id] = 1;
    if (configuration.fx.delay_sync[instance_id] > 0)
    {
      uint16_t midi_sync_delay_time = uint16_t(60000.0 * midi_ticks_factor[configuration.fx.delay_sync[instance_id]] / seq.bpm);
      // set_delay_sync(configuration.fx.delay_sync[instance_id], instance_id);
      delay_fx[instance_id]->delay(0, constrain(midi_sync_delay_time * configuration.fx.delay_multiplier[instance_id], DELAY_TIME_MIN, DELAY_TIME_MAX * 10));
    }
    global_delay_in_mixer[instance_id]->gain(0, mapfloat(configuration.fx.delay_level[instance_id], DELAY_LEVEL_MIN, DELAY_LEVEL_MAX, 0.0, 0.9));

    // REVERB SEND
    reverb_mixer_r.gain(instance_id, volume_transform(mapfloat(configuration.fx.reverb_send[instance_id], REVERB_SEND_MIN, REVERB_SEND_MAX, 0.0, VOL_MAX_FLOAT)));
    reverb_mixer_l.gain(instance_id, volume_transform(mapfloat(configuration.fx.reverb_send[instance_id], REVERB_SEND_MIN, REVERB_SEND_MAX, 0.0, VOL_MAX_FLOAT)));

    // DEXED FILTER
    MicroDexed[instance_id]->setFilterResonance(mapfloat(configuration.fx.filter_resonance[instance_id], FILTER_RESONANCE_MIN, FILTER_RESONANCE_MAX, 1.0, 0.0));
    MicroDexed[instance_id]->setFilterCutoff(mapfloat(configuration.fx.filter_cutoff[instance_id], FILTER_CUTOFF_MIN, FILTER_CUTOFF_MAX, 1.0, 0.0));
    MicroDexed[instance_id]->doRefreshVoice();
  }

  global_delay_in_mixer[0]->gain(5, mapfloat(configuration.fx.ep_delay_send_1, DELAY_LEVEL_MIN, DELAY_LEVEL_MAX, 0.0, 1.0));
  global_delay_in_mixer[0]->gain(6, mapfloat(configuration.fx.ep_delay_send_1, DELAY_LEVEL_MIN, DELAY_LEVEL_MAX, 0.0, 1.0));
  global_delay_in_mixer[1]->gain(5, mapfloat(configuration.fx.ep_delay_send_2, DELAY_LEVEL_MIN, DELAY_LEVEL_MAX, 0.0, 1.0));
  global_delay_in_mixer[1]->gain(6, mapfloat(configuration.fx.ep_delay_send_2, DELAY_LEVEL_MIN, DELAY_LEVEL_MAX, 0.0, 1.0));

  global_delay_in_mixer[1]->gain(7, mapfloat(configuration.fx.delay1_to_delay2, DELAY_LEVEL_MIN, DELAY_LEVEL_MAX, 0.0, 0.9));
  global_delay_in_mixer[0]->gain(7, mapfloat(configuration.fx.delay2_to_delay1, DELAY_LEVEL_MIN, DELAY_LEVEL_MAX, 0.0, 0.9));

  reverb_mixer_l.gain(REVERB_MIX_CH_AUX_DELAY1, mapfloat(configuration.fx.delay_to_reverb[0], REVERB_LEVEL_MIN, REVERB_LEVEL_MAX, 0.0, 1.0));
  reverb_mixer_r.gain(REVERB_MIX_CH_AUX_DELAY1, mapfloat(configuration.fx.delay_to_reverb[0], REVERB_LEVEL_MIN, REVERB_LEVEL_MAX, 0.0, 1.0));
  reverb_mixer_l.gain(REVERB_MIX_CH_AUX_DELAY2, mapfloat(configuration.fx.delay_to_reverb[1], REVERB_LEVEL_MIN, REVERB_LEVEL_MAX, 0.0, 1.0));
  reverb_mixer_r.gain(REVERB_MIX_CH_AUX_DELAY2, mapfloat(configuration.fx.delay_to_reverb[1], REVERB_LEVEL_MIN, REVERB_LEVEL_MAX, 0.0, 1.0));

  // REVERB
  reverb.size(mapfloat(configuration.fx.reverb_roomsize, REVERB_ROOMSIZE_MIN, REVERB_ROOMSIZE_MAX, 0.0, 1.0));
  reverb.lowpass(mapfloat(configuration.fx.reverb_lowpass, REVERB_LOWPASS_MIN, REVERB_LOWPASS_MAX, 0.0, 1.0));
  reverb.lodamp(mapfloat(configuration.fx.reverb_lodamp, REVERB_LODAMP_MIN, REVERB_LODAMP_MAX, 0.0, 1.0));
  reverb.hidamp(mapfloat(configuration.fx.reverb_hidamp, REVERB_HIDAMP_MIN, REVERB_HIDAMP_MAX, 0.0, 1.0));
  reverb.diffusion(mapfloat(configuration.fx.reverb_diffusion, REVERB_DIFFUSION_MIN, REVERB_DIFFUSION_MAX, 0.0, 1.0));

  reverb_mixer_r.gain(REVERB_MIX_CH_DRUMS, 1.0); // Drums Reverb-Send
  reverb_mixer_l.gain(REVERB_MIX_CH_DRUMS, 1.0); // Drums Reverb-Send

  reverb_mixer_r.gain(REVERB_MIX_CH_EPIANO, mapfloat(configuration.fx.ep_reverb_send, REVERB_SEND_MIN, REVERB_SEND_MAX, 0.0, 1.0)); // EPiano Reverb-Send
  reverb_mixer_l.gain(REVERB_MIX_CH_EPIANO, mapfloat(configuration.fx.ep_reverb_send, REVERB_SEND_MIN, REVERB_SEND_MAX, 0.0, 1.0)); // EPiano Reverb-Send

  // EP_CHORUS
  switch (configuration.fx.ep_chorus_waveform)
  {
  case 0:
    ep_chorus_modulator.begin(WAVEFORM_TRIANGLE);
    break;
  case 1:
    ep_chorus_modulator.begin(WAVEFORM_SINE);
    break;
  default:
    ep_chorus_modulator.begin(WAVEFORM_TRIANGLE);
  }
  ep_chorus_modulator.phase(0);
  ep_chorus_modulator.frequency(configuration.fx.ep_chorus_frequency / 10.0);
  ep_chorus_modulator.amplitude(mapfloat(configuration.fx.ep_chorus_depth, EP_CHORUS_DEPTH_MIN, EP_CHORUS_DEPTH_MAX, 0.0, 1.0));
  ep_chorus_modulator.offset(0.0);

#if MOD_FILTER_OUTPUT == MOD_BUTTERWORTH_FILTER_OUTPUT
  // Butterworth filter, 12 db/octave
  ep_modchorus_filter.setLowpass(0, MOD_FILTER_CUTOFF_HZ, 0.707);
#elif MOD_FILTER_OUTPUT == MOD_LINKWITZ_RILEY_FILTER_OUTPUT
  // Linkwitz-Riley filter, 48 dB/octave
  ep_modchorus_filter.setLowpass(0, MOD_FILTER_CUTOFF_HZ, 0.54);
  ep_modchorus_filter.setLowpass(1, MOD_FILTER_CUTOFF_HZ, 1.3);
  ep_modchorus_filter.setLowpass(2, MOD_FILTER_CUTOFF_HZ, 0.54);
  ep_modchorus_filter.setLowpass(3, MOD_FILTER_CUTOFF_HZ, 1.3);
#endif
  ep_chorus_mixer_r.gain(0, 1.0);
  ep_chorus_mixer_l.gain(0, 1.0);
  ep_chorus_mixer_r.gain(1, mapfloat(configuration.fx.ep_chorus_level, EP_CHORUS_LEVEL_MIN, EP_CHORUS_LEVEL_MAX, 0.0, 0.5));
  ep_chorus_mixer_l.gain(1, mapfloat(configuration.fx.ep_chorus_level, EP_CHORUS_LEVEL_MIN, EP_CHORUS_LEVEL_MAX, 0.0, 0.5));

  master_mixer_r.gain(MASTER_MIX_CH_REVERB, volume_transform(mapfloat(configuration.fx.reverb_level, REVERB_LEVEL_MIN, REVERB_LEVEL_MAX, 0.0, VOL_MAX_FLOAT)));
  master_mixer_l.gain(MASTER_MIX_CH_REVERB, volume_transform(mapfloat(configuration.fx.reverb_level, REVERB_LEVEL_MIN, REVERB_LEVEL_MAX, 0.0, VOL_MAX_FLOAT)));

  init_MIDI_send_CC();
}

FLASHMEM void set_voiceconfig_params(uint8_t instance_id)
{
  // INIT PEAK MIXER
  // microdexed_peak_mixer.gain(instance_id, 1.0);

  // Controller
  MicroDexed[instance_id]->setMaxNotes(configuration.dexed[instance_id].polyphony);
  MicroDexed[instance_id]->setPBController(configuration.dexed[instance_id].pb_range, configuration.dexed[instance_id].pb_step);
  MicroDexed[instance_id]->setMWController(configuration.dexed[instance_id].mw_range, configuration.dexed[instance_id].mw_assign, configuration.dexed[instance_id].mw_mode);
  MicroDexed[instance_id]->setFCController(configuration.dexed[instance_id].fc_range, configuration.dexed[instance_id].fc_assign, configuration.dexed[instance_id].fc_mode);
  MicroDexed[instance_id]->setBCController(configuration.dexed[instance_id].bc_range, configuration.dexed[instance_id].bc_assign, configuration.dexed[instance_id].bc_mode);
  MicroDexed[instance_id]->setATController(configuration.dexed[instance_id].at_range, configuration.dexed[instance_id].at_assign, configuration.dexed[instance_id].at_mode);
  MicroDexed[instance_id]->ControllersRefresh();
  MicroDexed[instance_id]->setOPAll(configuration.dexed[instance_id].op_enabled);
  MicroDexed[instance_id]->doRefreshVoice();
  MicroDexed[instance_id]->setMonoMode(configuration.dexed[instance_id].monopoly);
  MicroDexed[instance_id]->setEngineType(configuration.sys.dexed_engine_type);
  // Dexed output level
  MicroDexed[instance_id]->setGain(midi_volume_transform(map(configuration.dexed[instance_id].sound_intensity, SOUND_INTENSITY_MIN, SOUND_INTENSITY_MAX, 0, 127)));

  // PANORAMA
  dexed_mono2stereo[instance_id]->panorama(mapfloat(configuration.dexed[instance_id].pan, PANORAMA_MIN, PANORAMA_MAX, -1.0, 1.0));
}

FLASHMEM void set_epiano_params(void)
{
#ifdef DEBUG
  LOG.print(F("Setting EPiano parameters... "));
#endif
  ep.setDecay(mapfloat(configuration.epiano.decay, EP_DECAY_MIN, EP_DECAY_MAX, 0.0, 1.0));
  ep.setRelease(mapfloat(configuration.epiano.release, EP_RELEASE_MIN, EP_RELEASE_MAX, 0.0, 1.0));
  ep.setHardness(mapfloat(configuration.epiano.hardness, EP_HARDNESS_MIN, EP_HARDNESS_MAX, 0.0, 1.0));
  ep.setTreble(mapfloat(configuration.epiano.treble, EP_TREBLE_MIN, EP_TREBLE_MAX, 0.0, 1.0));
  ep.setPanTremolo(mapfloat(configuration.epiano.pan_tremolo, EP_PAN_TREMOLO_MIN, EP_PAN_TREMOLO_MAX, 0.0, 1.0));
  ep.setPanLFO(mapfloat(configuration.epiano.pan_lfo, EP_PAN_LFO_MIN, EP_PAN_LFO_MAX, 0.0, 1.0));
  ep.setVelocitySense(mapfloat(configuration.epiano.velocity_sense, EP_VELOCITY_SENSE_MIN, EP_VELOCITY_SENSE_MAX, 0.0, 1.0));
  ep.setStereo(mapfloat(configuration.epiano.stereo, EP_STEREO_MIN, EP_STEREO_MAX, 0.0, 1.0));
  ep.setPolyphony(configuration.epiano.polyphony);
  ep.setTune(mapfloat(configuration.epiano.tune, EP_TUNE_MIN, EP_TUNE_MAX, 0.0, 1.0));
  ep.setDetune(mapfloat(configuration.epiano.detune, EP_DETUNE_MIN, EP_DETUNE_MAX, 0, 1.0));
  ep.setOverdrive(mapfloat(configuration.epiano.overdrive, EP_OVERDRIVE_MIN, EP_OVERDRIVE_MAX, 0, 1.0));
  ep.setVolume(mapfloat(configuration.epiano.sound_intensity, SOUND_INTENSITY_MIN, SOUND_INTENSITY_MAX, 0, 1.0));
#ifdef DEBUG
  LOG.println(F("done."));
#endif
}

FLASHMEM void set_sys_params(void)
{
  // set initial volume
  set_volume(configuration.sys.vol, configuration.sys.mono);
}

/******************************************************************************
HELPERS
******************************************************************************/

FLASHMEM float midi_volume_transform(uint8_t midi_amp)
{
#ifdef DEBUG
  LOG.print(F("midi_amp = "));
  LOG.print(midi_amp, DEC);
  LOG.print(F(" transformed_midi_amp = "));
  LOG.println(powf(midi_amp / 127.0, 4), 3);
#endif
  return powf(midi_amp / 127.0, 4);
}

FLASHMEM float volume_transform(float amp)
{
  return powf(amp, 4);
}

FLASHMEM uint32_t crc32(byte* calc_start, uint16_t calc_bytes) // base code from https://www.arduino.cc/en/Tutorial/EEPROMCrc
{
  const uint32_t crc_table[16] = {
      0x00000000, 0x1db71064, 0x3b6e20c8, 0x26d930ac,
      0x76dc4190, 0x6b6b51f4, 0x4db26158, 0x5005713c,
      0xedb88320, 0xf00f9344, 0xd6d6a3e8, 0xcb61b38c,
      0x9b64c2b0, 0x86d3d2d4, 0xa00ae278, 0xbdbdf21c
  };
  uint32_t crc = ~0L;

  for (byte* index = calc_start; index < (calc_start + calc_bytes); ++index)
  {
    crc = crc_table[(crc ^ *index) & 0x0f] ^ (crc >> 4);
    crc = crc_table[(crc ^ (*index >> 4)) & 0x0f] ^ (crc >> 4);
    crc = ~crc;
  }

  return (crc);
}

FLASHMEM void generate_version_string(char* buffer, uint8_t len)
{
  char tmp[3];

  memset(buffer, 0, len);
  strncat(buffer, VERSION, len);
#if defined(TEENSY4_0)
  strncat(buffer, " - 4.0", 6);
#elif defined(TEENSY4_1)
  strncat(buffer, " - 4.1", 6);
#endif
  strncat(buffer, " - FX", 5);
#if defined(MAX_NOTES)
  strncat(buffer, " - MAX ", 7);
  itoa(MAX_NOTES, tmp, 10);
  strncat(buffer, tmp, 2);
#endif
}

FLASHMEM uint8_t check_sd_cards(void)
{
  uint8_t ret = 0;

  memset(sd_string, 0, sizeof(sd_string));

  if (!card.init(SPI_HALF_SPEED, BUILTIN_SDCARD))
  {
#ifdef DEBUG
    LOG.println("Initialization failed. Things to check:");
    LOG.println("* is a card inserted? Is it formatted FAT32 ? ");
#endif
    ret = 0;
  }
  else
  {
#ifdef DEBUG
    LOG.println("SD card is present.");
#endif
    ret = 1; // card found
  }

  if (ret > 0)
  {
#ifdef DEBUG
    LOG.print(F("Card type : "));
#endif
    switch (card.type())
    {
    case SD_CARD_TYPE_SD1:
      strcpy(sdcard_infos.type, "SD1");
      break;
    case SD_CARD_TYPE_SD2:
      strcpy(sdcard_infos.type, "SD2");
      break;
    case SD_CARD_TYPE_SDHC:
      strcpy(sdcard_infos.type, "SDHC");
      break;
    default:
      strcpy(sdcard_infos.type, "UKNW");
    }
    snprintf_P(sd_string, sizeof(sd_string), PSTR("%-5s"), sdcard_infos.type);
#ifdef DEBUG
    LOG.println(sdcard_infos.type);
#endif

    if (!volume.init(card))
    {
#ifdef DEBUG
      LOG.println(F("Could not find FAT16 / FAT32 partition."));
#endif
      ret = 0;
    }
  }

  if (ret > 0)
  {
    uint32_t volumesize;

    volumesize = volume.blocksPerCluster() * volume.clusterCount() / 2097152;

    if (volumesize == 0)
      ret = 0;

#ifdef DEBUG
    LOG.print(F("Volume type is FAT"));
    LOG.println(volume.fatType(), DEC);
    LOG.print(F("Volume size (GB) : "));
    LOG.println(volumesize);
#endif

    snprintf_P(sd_string + 5, sizeof(sd_string), PSTR("FAT %2d %02dGB"), volume.fatType(), int(volumesize));
  }

#ifdef DEBUG
  LOG.println(sd_string);
#endif

  return ret;
}

FLASHMEM void check_and_create_directories(void)
{
  if (sd_card > 0)
  {
    uint8_t i;
    char tmp[FILENAME_LEN];

#ifdef DEBUG
    LOG.println(F("Directory check... "));
#endif

    // Create directories for Dexed banks
    snprintf_P(tmp, sizeof(tmp), PSTR("/%s"), DEXED_CONFIG_PATH);
    if (SD.exists(tmp))
    {
      SD.remove(tmp);
    }
    else
    {
#ifdef DEBUG
      LOG.print(F("Creating directory "));
      LOG.println(tmp);
#endif
      SD.mkdir(tmp);

      // Create all Dexed bank directories
      for (i = 0; i < MAX_BANKS; i++)
      {
        snprintf_P(tmp, sizeof(tmp), PSTR("/%s/%d"), DEXED_CONFIG_PATH, i);
        if (!SD.exists(tmp))
        {
#ifdef DEBUG
          LOG.print(F("Creating directory "));
          LOG.println(tmp);
#endif
          SD.mkdir(tmp);
        }
      }
    }

    // 2022/11/19 update SD: move banks into /DEXED
    File sysex_dir;
    for (i = 0; i < MAX_BANKS; i++)
    {
      snprintf_P(tmp, sizeof(tmp), PSTR("/%d"), i);
      if (SD.exists(tmp))
      {
        sysex_dir = SD.open(tmp);

        if (!sysex_dir)
        {
          break;
        }

        // move file into new /DEXED folder
        File entry;
        do
        {
          entry = sysex_dir.openNextFile();
        } while (entry.isDirectory());

        char sysexFilename[FILENAME_LEN];
        char sysexPath[FILENAME_LEN];
        strcpy(sysexFilename, entry.name());
        snprintf_P(sysexPath, sizeof(sysexPath), PSTR("/%d/%s"), i, sysexFilename);
#ifdef DEBUG
        LOG.printf("move %s to /%s%s\n", sysexPath, DEXED_CONFIG_PATH, sysexPath);
#endif
        entry.close();
        sysex_dir.close();

        File myFileIn = SD.open(sysexPath, FILE_READ);
        byte buffer[4104];
        myFileIn.read(buffer, 4104);
        myFileIn.close();
        SD.remove(sysexPath);

        snprintf_P(sysexPath, sizeof(sysexPath), PSTR("/%s/%d/%s"), DEXED_CONFIG_PATH, i, sysexFilename);
        if (SD.exists(sysexPath))
        {
          SD.remove(sysexPath);
        }
        File myFileOut = SD.open(sysexPath, FILE_WRITE);
        myFileOut.write(buffer, 4104);
        myFileOut.close();

        SD.rmdir(tmp);
      }
    }

    // Create directory for performances
    snprintf_P(tmp, sizeof(tmp), PSTR("/%s"), PERFORMANCE_CONFIG_PATH);
    if (!SD.exists(tmp))
    {
#ifdef DEBUG
      LOG.print(F("Creating directory "));
      LOG.println(tmp);
#endif
      SD.mkdir(tmp);
    }

    /*
      // create directories for configuration files
      snprintf_P(tmp, sizeof(tmp), PSTR("/%s"), VOICE_CONFIG_PATH);
      if (!SD.exists(tmp))
      {
      #ifdef DEBUG
      LOG.print(F("Creating directory "));
      LOG.println(tmp);
      #endif
      SD.mkdir(tmp);
      }
    snprintf_P(tmp, sizeof(tmp), PSTR("/%s"), PERFORMANCE_CONFIG_PATH);
    if (!SD.exists(tmp))
    {
#ifdef DEBUG
    LOG.print(F("Creating directory "));
    LOG.println(tmp);
#endif
    SD.mkdir(tmp);
}
  snprintf_P(tmp, sizeof(tmp), PSTR("/%s"), FX_CONFIG_PATH);
  if (!SD.exists(tmp))
  {
#ifdef DEBUG
    LOG.print(F("Creating directory "));
    LOG.println(tmp);
#endif
    SD.mkdir(tmp);
  }
  snprintf_P(tmp, sizeof(tmp), PSTR("/%s"), DRUM_CONFIG_PATH);
  if (!SD.exists(tmp))
  {
#ifdef DEBUG
    LOG.print(F("Creating directory "));
    LOG.println(tmp);
#endif
    SD.mkdir(tmp);
  }
  snprintf_P(tmp, sizeof(tmp), PSTR("/%s"), FAV_CONFIG_PATH);
  if (!SD.exists(tmp))
  {
#ifdef DEBUG
    LOG.print(F("Creating directory "));
    LOG.println(tmp);
#endif
    SD.mkdir(tmp);
  }
  */

  // 2022/11/22 move existing favourites into /DEXED
    // if (SD.exists("FAVCFG"))
    // {

    //   SD.mkdir(FAV_CONFIG_PATH);

    //   for (i = 0; i < MAX_BANKS; i++)
    //   {
    //     snprintf_P(tmp, sizeof(tmp), PSTR("/FAVCFG/%d"), i);

    //     if (SD.exists(tmp))
    //     {
    //       char favPath[FILENAME_LEN];
    //       char favFilename[FILENAME_LEN];

    //       strcpy(favPath, tmp);

    //       snprintf_P(tmp, sizeof(tmp), PSTR("/%s/%d"), FAV_CONFIG_PATH, i);
    //       SD.mkdir(tmp);

    //       File fav_bank = SD.open(favPath);
    //       while (true)
    //       {
    //         File fav = fav_bank.openNextFile();
    //         if (!fav)
    //           break;

    //         strcpy(favFilename, fav.name());
    //         snprintf_P(favPath, sizeof(favPath), PSTR("/%s/%d/%s"), "FAVCFG", i, favFilename);
    //         fav.close();

    //         SD.remove(favPath);

    //         snprintf_P(favPath, sizeof(favPath), PSTR("/%s/%d/%s"), FAV_CONFIG_PATH, i, favFilename);
    //         File myFileOut = SD.open(favPath, FILE_WRITE);
    //         myFileOut.close();
    //       }
    //       fav_bank.close();
    //       snprintf_P(tmp, sizeof(tmp), PSTR("FAVCFG/%d"), i);
    //       SD.rmdir(tmp);
    //     }
    //   }

    //   SD.rmdir("/FAVCFG");
    // }
  }
#ifdef DEBUG
  LOG.println(F("SD card check end"));
#endif
}

/******************************************************************************
  DEBUG HELPER
******************************************************************************/
#if defined(DEBUG) && defined(SHOW_CPU_LOAD_MSEC)
FLASHMEM void show_cpu_and_mem_usage(void)
{
  uint32_t sum_xrun = 0;
  uint16_t sum_render_time_max = 0;

  for (uint8_t instance_id = 0; instance_id < NUM_DEXED; instance_id++)
  {
    sum_xrun += MicroDexed[instance_id]->getXRun();
    sum_render_time_max += MicroDexed[instance_id]->getRenderTimeMax();
    MicroDexed[instance_id]->resetRenderTimeMax();
  }
  if (AudioProcessorUsageMax() > 99.9)
  {
    cpumax++;
    LOG.print(F("*"));
  }
  else
    LOG.print(F(" "));
  LOG.print(F("CPU : "));
  LOG.print(AudioProcessorUsage(), 2);
  LOG.print(F(" % | CPUMAX : "));
  LOG.print(AudioProcessorUsageMax(), 2);
  LOG.print(F(" % | CPUMAXCNT : "));
  LOG.print(cpumax, DEC);
#ifdef TEENSY4
  LOG.print(F(" | CPUTEMP : "));
  LOG.print(tempmonGetTemp(), 2);
  LOG.print(F("C | MEM : "));
#else
  LOG.print(F(" | MEM : "));
#endif
  LOG.print(AudioMemoryUsage(), DEC);
  LOG.print(F(" | MEMMAX : "));
  LOG.print(AudioMemoryUsageMax(), DEC);
  LOG.print(F(" | AUDIO_MEM_MAX : "));
  LOG.print(AUDIO_MEM, DEC);
  LOG.print(F(" | RENDERTIMEMAX : "));
  LOG.print(sum_render_time_max, DEC);
  LOG.print(F(" | XRUN : "));
  LOG.print(sum_xrun, DEC);
  LOG.print(F(" | BLOCKSIZE : "));
  LOG.print(AUDIO_BLOCK_SAMPLES, DEC);
  LOG.print(F(" | RAM : "));
  LOG.print(FreeMem(), DEC);

  LOG.print(F(" | ACTVOICES : "));
  for (uint8_t instance_id = 0; instance_id < NUM_DEXED; instance_id++)
  {
    LOG.print(instance_id, DEC);
    LOG.print(F(" = "));
    LOG.print(active_voices[instance_id], DEC);
    LOG.print(F(" / "));
    LOG.print(MAX_NOTES / NUM_DEXED, DEC);
    if (instance_id != NUM_DEXED - 1)
      LOG.print(F(", "));
  }
  LOG.println();
  AudioProcessorUsageMaxReset();
  AudioMemoryUsageMaxReset();
}
#endif

#ifdef DEBUG
FLASHMEM void show_configuration(void)
{
  LOG.println();
  LOG.println(F("CONFIGURATION : "));
  LOG.println(F("System"));
  LOG.print(F("  Volume              "));
  LOG.println(configuration.sys.vol, DEC);
  LOG.print(F("  Mono                "));
  LOG.println(configuration.sys.mono, DEC);
  LOG.print(F("  Soft MIDI Thru      "));
  LOG.println(configuration.sys.soft_midi_thru, DEC);
  LOG.print(F("  Favorites           "));
  LOG.println(configuration.sys.favorites, DEC);
  LOG.print(F("  Performance Number  "));
  LOG.println(configuration.sys.performance_number, DEC);
  LOG.print(F("  Load at startup     "));
  LOG.println(configuration.sys.load_at_startup_performance, DEC);
  LOG.print(F("  Display rotation    "));
  LOG.println(configuration.sys.display_rotation, DEC);
  LOG.print(F("  Touch rotation      "));
  LOG.println(configuration.sys.touch_rotation, DEC);
  LOG.print(F("  UI reverse          "));
  LOG.println(configuration.sys.ui_reverse);
  LOG.print(F("  Screen saver start  "));
  LOG.println(configuration.sys.screen_saver_start, DEC);
  LOG.println(F("FX"));
  LOG.print(F("  Reverb Roomsize     "));
  LOG.println(configuration.fx.reverb_roomsize, DEC);
  LOG.print(F("  Reverb Level        "));
  LOG.println(configuration.fx.reverb_level, DEC);

  LOG.print(F("  Reverb Lowpass      "));
  LOG.println(configuration.fx.reverb_lowpass, DEC);
  LOG.print(F("  Reverb Lodamp       "));
  LOG.println(configuration.fx.reverb_lodamp, DEC);
  LOG.print(F("  Reverb Hidamp       "));
  LOG.println(configuration.fx.reverb_hidamp, DEC);
  LOG.print(F("  Reverb Diffusion     "));
  LOG.println(configuration.fx.reverb_diffusion, DEC);

  for (uint8_t instance_id = 0; instance_id < NUM_DEXED; instance_id++)
  {
    LOG.print(F("Dexed instance "));
    LOG.println(instance_id, DEC);
    LOG.print(F("  Bank                 "));
    LOG.println(configuration.dexed[instance_id].bank, DEC);
    LOG.print(F("  Voice                "));
    LOG.println(configuration.dexed[instance_id].voice, DEC);
    LOG.print(F("  MIDI - Channel         "));
    LOG.println(configuration.dexed[instance_id].midi_channel, DEC);
    LOG.print(F("  Lowest Note          "));
    LOG.println(configuration.dexed[instance_id].lowest_note, DEC);
    LOG.print(F("  Highest Note         "));
    LOG.println(configuration.dexed[instance_id].highest_note, DEC);
    LOG.print(F("  Filter Cutoff        "));
    LOG.println(configuration.fx.filter_cutoff[instance_id], DEC);
    LOG.print(F("  Filter Resonance     "));
    LOG.println(configuration.fx.filter_resonance[instance_id], DEC);
    LOG.print(F("  Chorus Frequency     "));
    LOG.println(configuration.fx.chorus_frequency[instance_id], DEC);
    LOG.print(F("  Chorus Waveform      "));
    LOG.println(configuration.fx.chorus_waveform[instance_id], DEC);
    LOG.print(F("  Chorus Depth         "));
    LOG.println(configuration.fx.chorus_depth[instance_id], DEC);
    LOG.print(F("  Chorus Level         "));
    LOG.println(configuration.fx.chorus_level[instance_id], DEC);
    LOG.print(F("  Delay Time           "));
    LOG.println(configuration.fx.delay_time[instance_id], DEC);
    LOG.print(F("  Delay Feedback       "));
    LOG.println(configuration.fx.delay_feedback[instance_id], DEC);
    LOG.print(F("  Delay Level          "));
    LOG.println(configuration.fx.delay_level[instance_id], DEC);
    LOG.print(F("  Delay Sync           "));
    LOG.println(configuration.fx.delay_sync[instance_id], DEC);
    LOG.print(F("  Reverb Send          "));
    LOG.println(configuration.fx.reverb_send[instance_id], DEC);
    LOG.print(F("  Sound Intensity      "));
    LOG.println(configuration.dexed[instance_id].sound_intensity, DEC);
    LOG.print(F("  Panorama             "));
    LOG.println(configuration.dexed[instance_id].pan, DEC);
    LOG.print(F("  Transpose            "));
    LOG.println(configuration.dexed[instance_id].transpose, DEC);
    LOG.print(F("  Tune                 "));
    LOG.println(configuration.dexed[instance_id].tune, DEC);
    LOG.print(F("  Polyphony            "));
    LOG.println(configuration.dexed[instance_id].polyphony, DEC);
    LOG.print(F("  Mono / Poly            "));
    LOG.println(configuration.dexed[instance_id].monopoly, DEC);
    LOG.print(F("  Note Refresh         "));
    LOG.println(configuration.dexed[instance_id].note_refresh, DEC);
    LOG.print(F("  Pitchbend Range      "));
    LOG.println(configuration.dexed[instance_id].pb_range, DEC);
    LOG.print(F("  Pitchbend Step       "));
    LOG.println(configuration.dexed[instance_id].pb_step, DEC);
    LOG.print(F("  Modwheel Range       "));
    LOG.println(configuration.dexed[instance_id].mw_range, DEC);
    LOG.print(F("  Modwheel Assign      "));
    LOG.println(configuration.dexed[instance_id].mw_assign, DEC);
    LOG.print(F("  Modwheel Mode        "));
    LOG.println(configuration.dexed[instance_id].mw_mode, DEC);
    LOG.print(F("  Footctrl Range       "));
    LOG.println(configuration.dexed[instance_id].fc_range, DEC);
    LOG.print(F("  Footctrl Assign      "));
    LOG.println(configuration.dexed[instance_id].fc_assign, DEC);
    LOG.print(F("  Footctrl Mode        "));
    LOG.println(configuration.dexed[instance_id].fc_mode, DEC);
    LOG.print(F("  BreathCtrl Range     "));
    LOG.println(configuration.dexed[instance_id].bc_range, DEC);
    LOG.print(F("  Breathctrl Assign    "));
    LOG.println(configuration.dexed[instance_id].bc_assign, DEC);
    LOG.print(F("  Breathctrl Mode      "));
    LOG.println(configuration.dexed[instance_id].bc_mode, DEC);
    LOG.print(F("  Aftertouch Range     "));
    LOG.println(configuration.dexed[instance_id].at_range, DEC);
    LOG.print(F("  Aftertouch Assign    "));
    LOG.println(configuration.dexed[instance_id].at_assign, DEC);
    LOG.print(F("  Aftertouch Mode      "));
    LOG.println(configuration.dexed[instance_id].at_mode, DEC);
    LOG.print(F("  Portamento Mode      "));
    LOG.println(configuration.dexed[instance_id].portamento_mode, DEC);
    LOG.print(F("  Portamento Glissando "));
    LOG.println(configuration.dexed[instance_id].portamento_glissando, DEC);
    LOG.print(F("  Portamento Time      "));
    LOG.println(configuration.dexed[instance_id].portamento_time, DEC);
    LOG.print(F("  OP Enabled           "));
    LOG.println(configuration.dexed[instance_id].op_enabled, DEC);
    LOG.flush();
  }

  LOG.println();
  LOG.flush();
}

FLASHMEM void show_patch(uint8_t instance_id)
{
  char vn[VOICE_NAME_LEN];

  LOG.print(F("INSTANCE "));
  LOG.println(instance_id, DEC);

  memset(vn, 0, sizeof(vn));
  LOG.println(F(" += == == == == == == == == == == == == == == == == == == == == == == == == == == == == == == == == == == == == == == == == == == == == == == == == == == == == = +"));
  for (int8_t i = 5; i >= 0; --i)
  {
    LOG.println(F(" += == == == == == == == == == == == == == == == == == == == == == == == == == == == == == == == == == == == == == == == == == == == == == == == == == == == == = +"));
    LOG.print(F(" | OP"));
    LOG.print(6 - i, DEC);
    LOG.println(F("                                                                                                      | "));
    LOG.println(F(" += == == = += == == = += == == = += == == = += == == = += == == = += == == = += == == = += == == == == == == == = += == == == == == == == = += == == == == == == == = +"));
    LOG.println(F(" |  R1  |  R2  |  R3  |  R4  |  L1  |  L2  |  L3  |  L4  | LEV_SCL_BRK_PT | SCL_LEFT_DEPTH | SCL_RGHT_DEPTH | "));
    LOG.println(F(" + ------ +------ +------ +------ +------ +------ +------ +------ +---------------- +---------------- +---------------- +"));
    LOG.print(F(" | "));
    SerialPrintFormatInt3(MicroDexed[instance_id]->getVoiceDataElement((i * 21) + DEXED_OP_EG_R1));
    LOG.print(F("  | "));
    SerialPrintFormatInt3(MicroDexed[instance_id]->getVoiceDataElement((i * 21) + DEXED_OP_EG_R2));
    LOG.print(F("  | "));
    SerialPrintFormatInt3(MicroDexed[instance_id]->getVoiceDataElement((i * 21) + DEXED_OP_EG_R3));
    LOG.print(F("  | "));
    SerialPrintFormatInt3(MicroDexed[instance_id]->getVoiceDataElement((i * 21) + DEXED_OP_EG_R4));
    LOG.print(F("  | "));
    SerialPrintFormatInt3(MicroDexed[instance_id]->getVoiceDataElement((i * 21) + DEXED_OP_EG_L1));
    LOG.print(F("  | "));
    SerialPrintFormatInt3(MicroDexed[instance_id]->getVoiceDataElement((i * 21) + DEXED_OP_EG_L2));
    LOG.print(F("  | "));
    SerialPrintFormatInt3(MicroDexed[instance_id]->getVoiceDataElement((i * 21) + DEXED_OP_EG_L3));
    LOG.print(F("  | "));
    SerialPrintFormatInt3(MicroDexed[instance_id]->getVoiceDataElement((i * 21) + DEXED_OP_EG_L4));
    LOG.print(F("  |           "));
    SerialPrintFormatInt3(MicroDexed[instance_id]->getVoiceDataElement((i * 21) + DEXED_OP_LEV_SCL_BRK_PT));
    LOG.print(F("  |           "));
    SerialPrintFormatInt3(MicroDexed[instance_id]->getVoiceDataElement((i * 21) + DEXED_OP_SCL_LEFT_DEPTH));
    LOG.print(F("  |           "));
    SerialPrintFormatInt3(MicroDexed[instance_id]->getVoiceDataElement((i * 21) + DEXED_OP_SCL_RGHT_DEPTH));
    LOG.println(F("  | "));
    LOG.println(F(" += == == = += == == = += == == = += == == = += == == = += == += = += = += == += == == = += == = += == == == = += = += == = += == == == += == += == == == == == == == = +"));
    LOG.println(F(" | SCL_L_CURVE | SCL_R_CURVE | RT_SCALE | AMS | KVS | OUT_LEV | OP_MOD | FRQ_C | FRQ_F | DETUNE             | "));
    LOG.println(F(" + ------------ - +------------ - +---------- +---- - +---- - +-------- - +-------- +------ - +------ - +-------------------- +"));
    LOG.print(F(" |        "));
    SerialPrintFormatInt3(MicroDexed[instance_id]->getVoiceDataElement((i * 21) + DEXED_OP_SCL_LEFT_CURVE));
    LOG.print(F("  |        "));
    SerialPrintFormatInt3(MicroDexed[instance_id]->getVoiceDataElement((i * 21) + DEXED_OP_SCL_RGHT_CURVE));
    LOG.print(F("  |     "));
    SerialPrintFormatInt3(MicroDexed[instance_id]->getVoiceDataElement((i * 21) + DEXED_OP_OSC_RATE_SCALE));
    LOG.print(F("  | "));
    SerialPrintFormatInt3(MicroDexed[instance_id]->getVoiceDataElement((i * 21) + DEXED_OP_AMP_MOD_SENS));
    LOG.print(F("  | "));
    SerialPrintFormatInt3(MicroDexed[instance_id]->getVoiceDataElement((i * 21) + DEXED_OP_KEY_VEL_SENS));
    LOG.print(F("  |    "));
    SerialPrintFormatInt3(MicroDexed[instance_id]->getVoiceDataElement((i * 21) + DEXED_OP_OUTPUT_LEV));
    LOG.print(F("  |   "));
    SerialPrintFormatInt3(MicroDexed[instance_id]->getVoiceDataElement((i * 21) + DEXED_OP_OSC_MODE));
    LOG.print(F("  |  "));
    SerialPrintFormatInt3(MicroDexed[instance_id]->getVoiceDataElement((i * 21) + DEXED_OP_FREQ_COARSE));
    LOG.print(F("  |  "));
    SerialPrintFormatInt3(MicroDexed[instance_id]->getVoiceDataElement((i * 21) + DEXED_OP_FREQ_FINE));
    LOG.print(F("  |               "));
    SerialPrintFormatInt3(MicroDexed[instance_id]->getVoiceDataElement((i * 21) + DEXED_OP_OSC_DETUNE));
    LOG.println(F("  | "));
  }
  LOG.println(F(" += == == == += == == += += == == == += == += == += == == = ++ == == += = += = += == = += == = += = += == == = += == == = += == == += += == == == == == == == == == = +"));
  LOG.println(F(" |  PR1  |  PR2  |  PR3  |  PR4  |  PL1  |  PL2  |  PL3  |  PL4  | ALG  |  FB  | OKS | TRANSPOSE            | "));
  LOG.println(F(" + ------ - +------ - +------ - +------ - +------ - +------ - +------ - +------ - +------ +------ +---- - +---------------------- +"));
  LOG.print(F(" |  "));
  for (int8_t i = 0; i < 8; i++)
  {
    SerialPrintFormatInt3(MicroDexed[instance_id]->getVoiceDataElement(DEXED_VOICE_OFFSET + i));
    LOG.print(F("  |  "));
  }
  SerialPrintFormatInt3(MicroDexed[instance_id]->getVoiceDataElement(DEXED_VOICE_OFFSET + DEXED_ALGORITHM));
  LOG.print(F(" | "));
  SerialPrintFormatInt3(MicroDexed[instance_id]->getVoiceDataElement(DEXED_VOICE_OFFSET + DEXED_FEEDBACK));
  LOG.print(F("  | "));
  SerialPrintFormatInt3(MicroDexed[instance_id]->getVoiceDataElement(DEXED_VOICE_OFFSET + DEXED_OSC_KEY_SYNC));
  LOG.print(F("  |                 "));
  SerialPrintFormatInt3(MicroDexed[instance_id]->getVoiceDataElement(DEXED_VOICE_OFFSET + DEXED_TRANSPOSE));
  LOG.println(F("  | "));
  LOG.println(F(" += == == == += += == == += == += == += == == += += == == == += == == == += = += == = += == == += += == == = ++ == == = += == == += == == == == == == == == == == = +"));
  LOG.println(F(" | LFO SPD | LFO DLY | LFO PMD | LFO AMD | LFO SYNC | LFO WAVE | LFO PMS | NAME                             | "));
  LOG.println(F(" + -------- - +-------- - +-------- - +-------- - +---------- +---------- +-------- - +---------------------------------- +"));
  LOG.print(F(" |    "));
  SerialPrintFormatInt3(MicroDexed[instance_id]->getVoiceDataElement(DEXED_VOICE_OFFSET + DEXED_LFO_SPEED));
  LOG.print(F("  |    "));
  SerialPrintFormatInt3(MicroDexed[instance_id]->getVoiceDataElement(DEXED_VOICE_OFFSET + DEXED_LFO_DELAY));
  LOG.print(F("  |    "));
  SerialPrintFormatInt3(MicroDexed[instance_id]->getVoiceDataElement(DEXED_VOICE_OFFSET + DEXED_LFO_PITCH_MOD_DEP));
  LOG.print(F("  |    "));
  SerialPrintFormatInt3(MicroDexed[instance_id]->getVoiceDataElement(DEXED_VOICE_OFFSET + DEXED_LFO_AMP_MOD_DEP));
  LOG.print(F("  |     "));
  SerialPrintFormatInt3(MicroDexed[instance_id]->getVoiceDataElement(DEXED_VOICE_OFFSET + DEXED_LFO_SYNC));
  LOG.print(F("  |     "));
  SerialPrintFormatInt3(MicroDexed[instance_id]->getVoiceDataElement(DEXED_VOICE_OFFSET + DEXED_LFO_WAVE));
  LOG.print(F("  |    "));
  SerialPrintFormatInt3(MicroDexed[instance_id]->getVoiceDataElement(DEXED_VOICE_OFFSET + DEXED_LFO_PITCH_MOD_SENS));
  LOG.print(F("  | "));
  MicroDexed[instance_id]->getName(vn);
  LOG.print(vn);
  LOG.println(F("                       | "));
  LOG.println(F(" += == == == == += == == == == += == == == == += == == == == += == == == == = += == == == == = += == == == == += == == == == == == == == == == == == == == == == = +"));
  LOG.println(F(" += == == == == == == == == == == == == == == == == == == == == == == == == == == == == == == == == == == == == == == == == == == == == == == == == == == == == = +"));
}

FLASHMEM void SerialPrintFormatInt3(uint8_t num)
{
  char buf[4];
  memset(buf, 0, 4);
  // snprintf_P(buf, sizeof(buf), PSTR("%3d"), num);
  snprintf_P(buf, sizeof(buf), PSTR("%3d"), num);
  LOG.print(buf);
}

/* From: https://forum.pjrc.com/threads/33443-How-to-display-free-ram */
extern unsigned long _heap_end;
extern char* __brkval;
FLASHMEM int FreeMem(void)
{
  return (char*)&_heap_end - __brkval;
}

#endif
