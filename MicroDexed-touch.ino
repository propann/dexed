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
#include <EEPROM.h>
#include <SD.h>
#include <SPI.h>

#include "ILI9341_t3n.h"
#include "XPT2046_Touchscreen.h"

#ifdef COMPILE_FOR_PROGMEM
#include <TeensyVariablePlayback.h>
#endif

#ifdef COMPILE_FOR_SDCARD
#include <TeensyVariablePlayback.h>
#endif

#ifdef COMPILE_FOR_FLASH
#include "TeensyVariablePlaybackFlash.h"
//#include <TeensyVariablePlayback.h>
#endif

#include <TeensyTimerTool.h>
using namespace TeensyTimerTool;

#include "microsynth.h"
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
#include "drumset.h"
#ifdef SGTL5000_AUDIO_ENHANCE
#include "control_sgtl5000plus.h"
#endif
#if defined (USE_EPIANO)
#include "synth_mda_epiano.h"
#include "effect_stereo_panorama.h"
#endif

#define TFT_DC 37 
#define TFT_CS 41
#define TFT_RST 24
#define TFT_SCK 27
#define TFT_MISO 39
#define TFT_MOSI 26
#define TFT_TOUCH_CS  38
#define TFT_TOUCH_IRQ 33

ILI9341_t3n display = ILI9341_t3n(TFT_CS, TFT_DC, TFT_RST, TFT_MOSI, TFT_SCK, TFT_MISO);
XPT2046_Touchscreen touch(TFT_TOUCH_CS, TFT_TOUCH_IRQ);  // CS, Touch IRQ Pin - interrupt enabled polling

//uint16_t COLOR_BACKGROUND = 0x0000;
//uint16_t COLOR_SYSTEXT = 0xFFFF;
//uint16_t COLOR_SYSTEXT_ACCENT = 0x159A;
//uint16_t COLOR_INSTR = 0x7BBD;
//uint16_t COLOR_CHORDS = 0xE2FA;
//uint16_t COLOR_ARP = 0x071B;
//uint16_t COLOR_DRUMS = 0xFE4F;
//uint16_t COLOR_PITCHSMP = 0x159A;

// Audio engines
AudioSynthDexed*                MicroDexed[NUM_DEXED];
#if defined(USE_EPIANO)
AudioSynthEPiano                ep(NUM_EPIANO_VOICES);
AudioAnalyzePeak                ep_peak_r;
AudioAnalyzePeak                ep_peak_l;
#endif

#if defined(USE_MICROSYNTH)
AudioSynthWaveform              microsynth_waveform[NUM_MICROSYNTH];
AudioSynthNoisePink             microsynth_noise[NUM_MICROSYNTH];
AudioEffectEnvelope             microsynth_envelope_osc[NUM_MICROSYNTH];
AudioEffectEnvelope             microsynth_envelope_noise[NUM_MICROSYNTH];
AudioFilterStateVariable        microsynth_filter_osc[NUM_MICROSYNTH];
AudioFilterStateVariable        microsynth_filter_noise[NUM_MICROSYNTH];
AudioEffectStereoPanorama       microsynth_stereo_panorama_osc[NUM_MICROSYNTH];
AudioEffectStereoPanorama       microsynth_stereo_panorama_noise[NUM_MICROSYNTH];
AudioMixer<4>                   microsynth_mixer_r;
AudioMixer<4>                   microsynth_mixer_l;
AudioMixer<4>                   microsynth_mixer_filter_osc[NUM_MICROSYNTH];
AudioMixer<4>                   microsynth_mixer_filter_noise[NUM_MICROSYNTH];
AudioMixer<2>                   microsynth_mixer_reverb;
AudioAnalyzePeak                microsynth_peak_osc_0;
AudioAnalyzePeak                microsynth_peak_osc_1;
AudioAnalyzePeak                microsynth_peak_noise_0;
AudioAnalyzePeak                microsynth_peak_noise_1;
#endif

#if defined(USE_FX)
AudioSynthWaveform*             chorus_modulator[NUM_DEXED];
#if MOD_FILTER_OUTPUT != MOD_NO_FILTER_OUTPUT
AudioFilterBiquad*              modchorus_filter[NUM_DEXED];
#endif
AudioEffectModulatedDelay*      modchorus[NUM_DEXED];
AudioMixer<2>*                  chorus_mixer[NUM_DEXED];
AudioMixer<2>*                  delay_fb_mixer[NUM_DEXED];
AudioEffectDelay*               delay_fx[NUM_DEXED];
AudioMixer<2>*                  delay_mixer[NUM_DEXED];
#endif
AudioEffectMonoStereo*          mono2stereo[NUM_DEXED];

#if defined(USE_FX) && defined(USE_EPIANO)
AudioEffectStereoPanorama       ep_stereo_panorama;
AudioSynthWaveform              ep_chorus_modulator;
#if MOD_FILTER_OUTPUT != MOD_NO_FILTER_OUTPUT
AudioFilterBiquad               ep_modchorus_filter;
#endif
AudioEffectModulatedDelayStereo ep_modchorus;
AudioMixer<2>                   ep_chorus_mixer_r;
AudioMixer<2>                   ep_chorus_mixer_l;
#endif

AudioAnalyzePeak                microdexed_peak_0;
AudioAnalyzePeak                microdexed_peak_1;

#if defined(USE_FX)
#if defined(USE_EPIANO) && defined(USE_MICROSYNTH)
AudioMixer<5>                   reverb_mixer_r;
AudioMixer<5>                   reverb_mixer_l;
#elif defined(USE_EPIANO)
AudioMixer<4>                   reverb_mixer_r;
AudioMixer<4>                   reverb_mixer_l;
#else
AudioMixer<3>                   reverb_mixer_r;
AudioMixer<3>                   reverb_mixer_l;
#endif
#endif

AudioEffectPlateReverb          reverb;
Realtime_Scope                  scope;

#if defined(USE_FX) && defined(USE_EPIANO) && defined(USE_MICROSYNTH)
AudioMixer<7>                   master_mixer_r;
AudioMixer<7>                   master_mixer_l;
#elif defined(USE_FX) && defined(USE_EPIANO)
AudioMixer<6>                   master_mixer_r;
AudioMixer<6>                   master_mixer_l;
#else
AudioMixer<5>                   master_mixer_r;
AudioMixer<5>                   master_mixer_l;
#endif
AudioAmplifier                  volume_r;
AudioAmplifier                  volume_l;
AudioEffectStereoMono           stereo2mono;
AudioAnalyzePeak                master_peak_r;
AudioAnalyzePeak                master_peak_l;
#if defined(TEENSY_AUDIO_BOARD) && defined(SGTL5000_AUDIO_THRU)
AudioMixer<2>                   audio_thru_mixer_r;
AudioMixer<2>                   audio_thru_mixer_l;
#endif

// Drumset
#if NUM_DRUMS > 0
AudioPlaySdWav                  sd_WAV;

#ifdef COMPILE_FOR_FLASH
AudioPlayFlashResmp*          Drum[NUM_DRUMS];
//AudioPlaySerialflashRaw*        Drum[NUM_DRUMS];  // playflash from normal audio library (no pitch)
#endif

#ifdef COMPILE_FOR_SDCARD
AudioPlaySdResmp*               Drum[NUM_DRUMS];
#endif

#ifdef COMPILE_FOR_PROGMEM
AudioPlayArrayResmp*            Drum[NUM_DRUMS];
#endif

AudioMixer<NUM_DRUMS>           drum_mixer_r;
AudioMixer<NUM_DRUMS>           drum_mixer_l;

AudioAnalyzePeak                drum_mixer_peak_r;
AudioAnalyzePeak                drum_mixer_peak_l;

AudioMixer<8>                   drum_reverb_send_mixer_r;
AudioMixer<8>                   drum_reverb_send_mixer_l;

AudioAnalyzePeak                reverb_return_peak_r;
AudioAnalyzePeak                reverb_return_peak_l;

#endif

// Outputs
#if defined(TEENSY_AUDIO_BOARD)
AudioOutputI2S                  i2s1;
#ifdef SGTL5000_AUDIO_ENHANCE
AudioControlSGTL5000Plus        sgtl5000;
#else
AudioControlSGTL5000            sgtl5000;
#endif
#elif defined (I2S_AUDIO_ONLY)
AudioOutputI2S                  i2s1;
#elif defined(TGA_AUDIO_BOARD)
AudioOutputI2S                  i2s1;
AudioControlWM8731master        wm8731_1;
#elif defined(PT8211_AUDIO)
AudioOutputPT8211               pt8211_1;
#elif defined(TEENSY_DAC_SYMMETRIC)
AudioOutputAnalogStereo         dacOut;
AudioMixer<4>                   invMixer;
#elif defined(TEENSY_DAC)
AudioOutputAnalogStereo         dacOut;
#endif
#ifdef AUDIO_DEVICE_USB
AudioOutputUSB                  usb1;
#endif

#if defined(TEENSY_AUDIO_BOARD) && defined(SGTL5000_AUDIO_THRU)
AudioInputI2S                   i2s1in;
#endif


//
// Static patching of audio objects
//
AudioConnection patchCord[] = {
  // Audio chain tail
#if defined(USE_FX)
#ifdef USE_PLATEREVERB
  {reverb_mixer_r, 0, reverb, 0},
  {reverb_mixer_l, 0, reverb, 1},
  {reverb, 0, master_mixer_r, MASTER_MIX_CH_REVERB},
  {reverb, 1, master_mixer_l, MASTER_MIX_CH_REVERB},
#endif
#endif
  {master_mixer_r, volume_r},
  {master_mixer_l, volume_l},
  {volume_r, 0, stereo2mono, 0},
  {volume_l, 0, stereo2mono, 1},
  {stereo2mono, 0, master_peak_r, 0},
  {stereo2mono, 0, master_peak_l, 0},

  //Realtime Scope
  {stereo2mono, 0, scope, 0},

  // Outputs
#if defined(TEENSY_AUDIO_BOARD)
#ifndef SGTL5000_AUDIO_THRU
  {stereo2mono, 0, i2s1, 0},
  {stereo2mono, 1, i2s1, 1},
#endif
#elif defined (I2S_AUDIO_ONLY)
  {stereo2mono, 0, i2s1, 0},
  {stereo2mono, 1, i2s1, 1},
#elif defined(TGA_AUDIO_BOARD)
  {stereo2mono, 0, i2s1, 0},
  {stereo2mono, 1, i2s1, 1},
#elif defined(PT8211_AUDIO)
  {stereo2mono, 0, pt8211_1, 0},
  {stereo2mono, 1, pt8211_1, 1},
#elif defined(TEENSY_DAC_SYMMETRIC)
  {stereo2mono, 0, dacOut  , 0},
  {stereo2mono, 1, invMixer, 0},
  {invMixer, 0, dacOut  , 1},
#elif defined(TEENSY_DAC)
  {stereo2mono, 0, dacOut, 0},
  {stereo2mono, 1, dacOut, 1},
#endif
#ifdef AUDIO_DEVICE_USB
  {stereo2mono, 0, usb1, 0},
  {stereo2mono, 1, usb1, 1},
#endif

#if defined(TEENSY_AUDIO_BOARD) && defined(SGTL5000_AUDIO_THRU)
  {stereo2mono, 0, audio_thru_mixer_r, 0},
  {stereo2mono, 1, audio_thru_mixer_l, 0},
  {i2s1in, 0, audio_thru_mixer_r, 1},  //phtodo check
  {i2s1in, 1, audio_thru_mixer_l, 1},
  {audio_thru_mixer_r, 0, i2s1, 0},
  {audio_thru_mixer_l, 0, i2s1, 1},
#endif

  {drum_reverb_send_mixer_r, 0, reverb_mixer_r, REVERB_MIX_CH_DRUMS},
  {drum_reverb_send_mixer_l, 0, reverb_mixer_l, REVERB_MIX_CH_DRUMS},
  {drum_mixer_r, 0, master_mixer_r, MASTER_MIX_CH_DRUMS},
  {drum_mixer_l, 0, master_mixer_l, MASTER_MIX_CH_DRUMS},

  { reverb, 0, reverb_return_peak_r, 0},
  { reverb, 1, reverb_return_peak_l, 0},

  {drum_mixer_r, 0, drum_mixer_peak_r, 0},
  {drum_mixer_l, 0, drum_mixer_peak_l, 0},


#if defined(USE_EPIANO)
  {ep, 0, ep_stereo_panorama , 0},
  {ep, 1, ep_stereo_panorama , 1},
#if defined(USE_FX)
  {ep_stereo_panorama, 0, ep_chorus_mixer_r, 0},
  {ep_stereo_panorama, 1, ep_chorus_mixer_l, 0},
  {ep_stereo_panorama, 0, ep_modchorus, 0},
  {ep_stereo_panorama, 1, ep_modchorus, 1},
#if MOD_FILTER_OUTPUT != MOD_NO_FILTER_OUTPUT
  {ep_chorus_modulator, 0, ep_modchorus_filter, 0},
  {ep_modchorus_filter, 0, ep_modchorus, 2},
#else
  {ep_chorus_modulator, 0, ep_modchorus, 2},
#endif
  {ep_modchorus, 0, ep_chorus_mixer_r, 1},
  {ep_modchorus, 1, ep_chorus_mixer_l, 1},
  {ep_chorus_mixer_r, 0, reverb_mixer_r, REVERB_MIX_CH_EPIANO},
  {ep_chorus_mixer_l, 0, reverb_mixer_l, REVERB_MIX_CH_EPIANO},
  {ep_chorus_mixer_r, 0, master_mixer_r, MASTER_MIX_CH_EPIANO},
  {ep_chorus_mixer_l, 0, master_mixer_l, MASTER_MIX_CH_EPIANO},

  { master_mixer_r, MASTER_MIX_CH_EPIANO, ep_peak_r, 0},
  { master_mixer_l, MASTER_MIX_CH_EPIANO, ep_peak_l, 0},

#else
  {ep_stereo_panorama, 0, master_mixer_r, MASTER_MIX_CH_EPIANO},
  {ep_stereo_panorama, 1, master_mixer_l, MASTER_MIX_CH_EPIANO},

#endif
#endif

#if defined(USE_MICROSYNTH)

  {microsynth_noise[0], 0, microsynth_envelope_noise[0] , 0},  //noise generator to envelope
  {microsynth_noise[1], 0, microsynth_envelope_noise[1] , 0},

  {microsynth_envelope_noise[0], 0, microsynth_filter_noise[0], 0}, //noise envelope to filter
  {microsynth_envelope_noise[1], 0, microsynth_filter_noise[1], 0},

  {microsynth_filter_noise[0], 0, microsynth_mixer_filter_noise[0] , 0}, //noise filter to mixer
  {microsynth_filter_noise[0], 1, microsynth_mixer_filter_noise[0] , 1},
  {microsynth_filter_noise[0], 2, microsynth_mixer_filter_noise[0] , 2},

  {microsynth_filter_noise[1], 0, microsynth_mixer_filter_noise[1] , 0},
  {microsynth_filter_noise[1], 1, microsynth_mixer_filter_noise[1] , 1},
  {microsynth_filter_noise[1], 2, microsynth_mixer_filter_noise[1] , 2},

  {microsynth_envelope_noise[0], 0, microsynth_mixer_filter_noise[0] , 3}, // unfiltered noise to mixer
  {microsynth_envelope_noise[1], 0, microsynth_mixer_filter_noise[1] , 3},

  {microsynth_mixer_filter_noise[0], 0, microsynth_stereo_panorama_noise[0] , 0}, //noise to panorama 0
  {microsynth_mixer_filter_noise[0], 0, microsynth_stereo_panorama_noise[0] , 1},

  {microsynth_mixer_filter_noise[1], 0, microsynth_stereo_panorama_noise[1] , 0}, //noise to panorama 1
  {microsynth_mixer_filter_noise[1], 0, microsynth_stereo_panorama_noise[1] , 1},

  {microsynth_waveform[0], 0, microsynth_envelope_osc[0], 0}, // osc waveform to envelope
  {microsynth_waveform[1], 0, microsynth_envelope_osc[1], 0},

  {microsynth_envelope_osc[0], 0, microsynth_filter_osc[0] , 0}, // osc envelope to filter
  {microsynth_envelope_osc[1], 0, microsynth_filter_osc[1] , 0},

  {microsynth_envelope_osc[0], 0, microsynth_mixer_filter_osc[0] , 3},// osc env to mixer for bypass
  {microsynth_envelope_osc[1], 0, microsynth_mixer_filter_osc[1] , 3},

  {microsynth_filter_osc[0], 0, microsynth_mixer_filter_osc[0] , 0}, //osc filter 0 to mixer 0
  {microsynth_filter_osc[0], 1, microsynth_mixer_filter_osc[0] , 1},
  {microsynth_filter_osc[0], 2, microsynth_mixer_filter_osc[0] , 2},

  {microsynth_filter_osc[1], 0, microsynth_mixer_filter_osc[1] , 0}, //osc filter 1 to mixer 1
  {microsynth_filter_osc[1], 1, microsynth_mixer_filter_osc[1] , 1},
  {microsynth_filter_osc[1], 2, microsynth_mixer_filter_osc[1] , 2},

  {microsynth_mixer_filter_osc[0], 0, microsynth_stereo_panorama_osc[0] , 0}, //mixer 0 to panorama 0
  {microsynth_mixer_filter_osc[0], 0, microsynth_stereo_panorama_osc[0] , 1},

  {microsynth_mixer_filter_osc[1], 0, microsynth_stereo_panorama_osc[1] , 0}, //mixer 1 to panorama 1
  {microsynth_mixer_filter_osc[1], 0, microsynth_stereo_panorama_osc[1] , 1},

  {microsynth_stereo_panorama_osc[0], 0, microsynth_mixer_r, 0},  //osc 0 pan to internal mixer
  {microsynth_stereo_panorama_osc[0], 1, microsynth_mixer_l, 0},

  {microsynth_stereo_panorama_osc[1], 0, microsynth_mixer_r, 1}, //osc 1 pan to internal mixer
  {microsynth_stereo_panorama_osc[1], 1, microsynth_mixer_l, 1},

  {microsynth_stereo_panorama_noise[0], 0, microsynth_mixer_r, 2},  // noise 0 to mixer
  {microsynth_stereo_panorama_noise[0], 1, microsynth_mixer_l, 2},

  {microsynth_stereo_panorama_noise[1], 0, microsynth_mixer_r, 3},  //noise 1 to mixer
  {microsynth_stereo_panorama_noise[1], 1, microsynth_mixer_l, 3},

  {microsynth_mixer_r, 0, master_mixer_r, MASTER_MIX_CH_MICROSYNTH},  //mixer to master mixer
  {microsynth_mixer_l, 0, master_mixer_l, MASTER_MIX_CH_MICROSYNTH},

  {microsynth_envelope_osc[0], 0, microsynth_mixer_reverb , 0},
  {microsynth_envelope_osc[1], 0, microsynth_mixer_reverb , 1},

  {microsynth_mixer_reverb, 0, reverb_mixer_r, REVERB_MIX_CH_MICROSYNTH},
  {microsynth_mixer_reverb, 1, reverb_mixer_l, REVERB_MIX_CH_MICROSYNTH},

  {microsynth_mixer_filter_osc[0], 0, microsynth_peak_osc_0, 0}, //osc 1 (inst1) to peak analyzer
  {microsynth_mixer_filter_osc[1], 0, microsynth_peak_osc_1, 0}, //osc 2 (inst2) to peak analyzer
  {microsynth_mixer_filter_noise[0], 0, microsynth_peak_noise_0, 0}, // unfiltered noise to mixer
  {microsynth_mixer_filter_noise[1], 0, microsynth_peak_noise_1, 0},

#endif

};

//
// Dynamic patching of MicroDexed objects
//
uint8_t nDynamic = 0;
#if defined(USE_FX) && MOD_FILTER_OUTPUT != MOD_NO_FILTER_OUTPUT
AudioConnection* dynamicConnections[NUM_DEXED * 16 + NUM_DRUMS * 4 ];
#elif defined(USE_FX) && MOD_FILTER_OUTPUT == MOD_NO_FILTER_OUTPUT
AudioConnection* dynamicConnections[NUM_DEXED * 15 + NUM_DRUMS * 4];
#else
AudioConnection* dynamicConnections[NUM_DEXED * 4 + NUM_DRUMS * 2];
#endif
void create_audio_dexed_chain(uint8_t instance_id)
{
  MicroDexed[instance_id] = new AudioSynthDexed(MAX_NOTES / NUM_DEXED, SAMPLE_RATE);
  mono2stereo[instance_id] = new AudioEffectMonoStereo();
#if defined(USE_FX)
  chorus_modulator[instance_id] = new AudioSynthWaveform();
#if MOD_FILTER_OUTPUT != MOD_NO_FILTER_OUTPUT
  modchorus_filter[instance_id] = new AudioFilterBiquad();
#endif
  modchorus[instance_id] = new AudioEffectModulatedDelay();
  chorus_mixer[instance_id] = new AudioMixer<2>();
  delay_fb_mixer[instance_id] = new AudioMixer<2>();
  delay_fx[instance_id] = new AudioEffectDelay();
  delay_mixer[instance_id] = new AudioMixer<2>();
#endif

  if (instance_id == 0)
    dynamicConnections[nDynamic++] = new AudioConnection(*MicroDexed[instance_id], 0, microdexed_peak_0, 0);
  else
    dynamicConnections[nDynamic++] = new AudioConnection(*MicroDexed[instance_id], 0, microdexed_peak_1, 0);

#if defined(USE_FX)
  dynamicConnections[nDynamic++] = new AudioConnection(*MicroDexed[instance_id], 0, *chorus_mixer[instance_id], 0);
  dynamicConnections[nDynamic++] = new AudioConnection(*MicroDexed[instance_id], 0, *modchorus[instance_id], 0); //////////////////////
#if MOD_FILTER_OUTPUT != MOD_NO_FILTER_OUTPUT
  dynamicConnections[nDynamic++] = new AudioConnection(*chorus_modulator[instance_id], 0, *modchorus_filter[instance_id], 0);
  dynamicConnections[nDynamic++] = new AudioConnection(*modchorus_filter[instance_id], 0, *modchorus[instance_id], 1);
#else
  dynamicConnections[nDynamic++] = new AudioConnection(*chorus_modulator[instance_id], 0, *modchorus[instance_id], 1);
#endif
  dynamicConnections[nDynamic++] = new AudioConnection(*modchorus[instance_id], 0, *chorus_mixer[instance_id], 1);
  dynamicConnections[nDynamic++] = new AudioConnection(*chorus_mixer[instance_id], 0, *delay_fb_mixer[instance_id], 0);
  dynamicConnections[nDynamic++] = new AudioConnection(*chorus_mixer[instance_id], 0, *delay_mixer[instance_id], 0);
  dynamicConnections[nDynamic++] = new AudioConnection(*delay_fb_mixer[instance_id], 0, *delay_fx[instance_id], 0);
  dynamicConnections[nDynamic++] = new AudioConnection(*delay_fx[instance_id], 0, *delay_fb_mixer[instance_id], 1);
  dynamicConnections[nDynamic++] = new AudioConnection(*delay_fx[instance_id], 0, *delay_mixer[instance_id], 1);
  dynamicConnections[nDynamic++] = new AudioConnection(*delay_mixer[instance_id], 0, *mono2stereo[instance_id], 0);
  dynamicConnections[nDynamic++] = new AudioConnection(*mono2stereo[instance_id], 0, reverb_mixer_r, instance_id);
  dynamicConnections[nDynamic++] = new AudioConnection(*mono2stereo[instance_id], 1, reverb_mixer_l, instance_id);
#else
  dynamicConnections[nDynamic++] = new AudioConnection(*MicroDexed[instance_id], 0, *mono2stereo[instance_id], 0);
#endif
  dynamicConnections[nDynamic++] = new AudioConnection(*mono2stereo[instance_id], 0, master_mixer_r, instance_id);
  dynamicConnections[nDynamic++] = new AudioConnection(*mono2stereo[instance_id], 1, master_mixer_l, instance_id);

#ifdef DEBUG
  Serial.print(F("Dexed-Instance: "));
  Serial.println(instance_id);
#endif
}

//
// Dynamic patching of Drum objects
//
#if NUM_DRUMS > 0
void create_audio_drum_chain(uint8_t instance_id)
{
  //Drum[instance_id] = new AudioPlayMemory();
  //Drum[instance_id] = new AudioPlaySdWav();

#ifdef COMPILE_FOR_FLASH
  Drum[instance_id] = new AudioPlayFlashResmp();
  //Drum[instance_id] = new AudioPlaySerialflashRaw();
#endif

#ifdef COMPILE_FOR_SDCARD
  Drum[instance_id] = new AudioPlaySdResmp();
#endif

#ifdef COMPILE_FOR_PROGMEM
  Drum[instance_id] = new AudioPlayArrayResmp();
#endif

  Drum[instance_id]->enableInterpolation(false);
  Drum[instance_id]->setPlaybackRate(1.0);

  dynamicConnections[nDynamic++] = new AudioConnection(*Drum[instance_id], 0, drum_mixer_r, instance_id);
  dynamicConnections[nDynamic++] = new AudioConnection(*Drum[instance_id], 0, drum_mixer_l, instance_id);
#ifdef USE_FX
  dynamicConnections[nDynamic++] = new AudioConnection(*Drum[instance_id], 0, drum_reverb_send_mixer_r, instance_id);
  dynamicConnections[nDynamic++] = new AudioConnection(*Drum[instance_id], 0, drum_reverb_send_mixer_l, instance_id);
#endif


#ifdef DEBUG
  Serial.print(F("Drum-Instance: "));
  Serial.println(instance_id);
  Serial.print(F("Dynamic-Connection-Counter="));
  Serial.println(nDynamic);
#endif
}
#endif

void create_audio_sd_wav_preview_chain()
{
  //sd_WAV[instance_id] = new AudioPlaySdWav();
  //sd_WAV = new AudioPlaySdWav();

  //phtodo
  //  dynamicConnections[nDynamic++] = new AudioConnection(sd_WAV, 0, master_mixer_r, MASTER_MIX_CH_SD_FILE_PREVIEW);
  //  dynamicConnections[nDynamic++] = new AudioConnection(sd_WAV, 0, master_mixer_l, MASTER_MIX_CH_SD_FILE_PREVIEW);
}

uint8_t sd_card = 0;
Sd2Card card;
SdVolume volume;
const float midi_ticks_factor[10] = {0.0, 0.25, 0.375, 0.5, 0.75, 1.0, 1.5, 2.0, 3.0 , 4.0};
uint8_t midi_bpm_counter = 0;
uint8_t midi_bpm = 0;
int16_t _midi_bpm = -1;
elapsedMillis midi_bpm_timer;
elapsedMillis long_button_pressed;
elapsedMillis control_rate;
elapsedMillis microsynth_lfo_control_rate;
elapsedMillis microsynth_lfo_delay_timer[2];
elapsedMillis save_sys;
bool save_sys_flag = false;
uint8_t active_voices[NUM_DEXED];
uint8_t midi_voices[NUM_DEXED];
#ifdef SHOW_CPU_LOAD_MSEC
elapsedMillis cpu_mem_millis;
#endif
uint32_t cpumax = 0;
uint32_t peak_dexed = 0;
float peak_dexed_value = 0.0;
uint32_t peak_r = 0;
uint32_t peak_l = 0;
config_t configuration;
const uint8_t cs_pins[] = { SDCARD_TEENSY_CS_PIN, SDCARD_AUDIO_CS_PIN };
const uint8_t mosi_pins[] = { SDCARD_TEENSY_MOSI_PIN, SDCARD_AUDIO_MOSI_PIN };
const uint8_t sck_pins[] = { SDCARD_TEENSY_SCK_PIN, SDCARD_AUDIO_SCK_PIN };
char version_string[display_cols + 1];
char sd_string[display_cols + 1];
char g_bank_name[NUM_DEXED][BANK_NAME_LEN];
char g_voice_name[NUM_DEXED][VOICE_NAME_LEN];
char tmp_bank_name[BANK_NAME_LEN];
char tmp_voice_name[VOICE_NAME_LEN];
char receive_bank_filename[FILENAME_LEN];
uint8_t selected_instance_id = 0;
uint8_t microsynth_selected_instance = 0;

char noteNames[12][3] = { "C", "C#", "D", "D#", "E", "F", "F#", "G", "G#", "A", "A#", "B" };

#if NUM_DEXED>1
int8_t midi_decay_dexed[NUM_DEXED] = { -1, -1};
#else
int8_t midi_decay_dexed[NUM_DEXED] = { -1};
#endif
int8_t midi_decay_microsynth[NUM_MICROSYNTH];
elapsedMillis midi_decay_timer_dexed;
elapsedMillis midi_decay_timer_microsynth;

#if NUM_DEXED>1
int perform_attack_mod[NUM_DEXED] = { 0, 0 };
int perform_release_mod[NUM_DEXED] = { 0, 0 };
#else
int perform_attack_mod[NUM_DEXED] = { 0 };
int perform_release_mod[NUM_DEXED] = { 0 };
#endif
#if defined(USE_FX)
// Allocate the delay lines for chorus
int16_t* delayline[NUM_DEXED];
#ifdef USE_EPIANO
int16_t* ep_delayline_r;
int16_t* ep_delayline_l;
#endif
#endif

#if NUM_DRUMS > 0
extern sequencer_t seq;
extern multisample_t ms[NUM_MULTISAMPLES];
extern multisample_zone_t msz[NUM_MULTISAMPLES][NUM_MULTISAMPLE_ZONES];
extern drum_config_t drum_config[NUM_DRUMSET_CONFIG];
uint8_t drum_counter;
uint8_t drum_type[NUM_DRUMS];
extern void sequencer(void);
uint8_t drum_midi_channel = DRUM_MIDI_CHANNEL;
custom_midi_map_t custom_midi_map[NUM_CUSTOM_MIDI_MAPPINGS];
extern void print_custom_mappings(void);
extern uint8_t seq_prev_note[NUM_SEQ_TRACKS];
extern void handle_touchscreen_mute_matrix(void);
extern void handle_touchscreen_voice_select(void);
extern void handle_touchscreen_pattern_editor(void);
extern void handle_touchscreen_microsynth(void);
extern void handle_touchscreen_file_manager(void);
extern void handle_touchscreen_custom_mappings(void);
extern void handle_touchscreen_cc_mappings(void);
extern void handle_touchscreen_color_edit(void);
extern void handle_touchscreen_arpeggio(void);
extern void update_midi_learn_button(void);
extern void print_arp_start_stop_button(void);
#endif

extern void update_microsynth_params(void);

extern LCDMenuLib2 LCDML;

extern void getNoteName(char* noteName, uint8_t noteNumber);

#if defined(USE_SEQUENCER)
PeriodicTimer sequencer_timer;
#endif

/***********************************************************************
   SETUP
 ***********************************************************************/
void setup()
{
#ifdef DEBUG
  Serial.begin(SERIAL_SPEED);

  Serial.println(CrashReport);
  //setup_debug_message();


  // test DIN MIDI out
  // MIDI.begin();

  generate_version_string(version_string, sizeof(version_string));
  Serial.println(F("MicroDexed based on https://github.com/asb2m10/dexed"));
  Serial.println(F("(c)2018-2022 H. Wirtz <wirtz@parasitstudio.de>"));
  Serial.println(F("https://codeberg.org/dcoredump/MicroDexed"));
  Serial.print(F("Version: "));
  Serial.println(version_string);
  Serial.print(F("CPU-Speed: "));
  Serial.print(F_CPU / 1000000.0, 1);
  Serial.println(F(" MHz"));
  Serial.println(F("<setup start>"));
  Serial.flush();
#endif

  display.begin();  //TFT is on SPI1

  touch.begin();
  //touch.setRotation(3); // not necessary for ILI9341

  // Setup MIDI devices
  setup_midi_devices();

  // Start audio system
  AudioMemory(AUDIO_MEM);

#if defined(TEENSY_AUDIO_BOARD)
  sgtl5000.enable();
  sgtl5000.lineOutLevel(SGTL5000_LINEOUT_LEVEL);
  sgtl5000.dacVolumeRamp();
  sgtl5000.dacVolume(1.0);
  //sgtl5000.dacVolumeRampLinear();
  //sgtl5000.dacVolumeRampDisable();
  sgtl5000.unmuteHeadphone();
  sgtl5000.unmuteLineout();
  sgtl5000.volume(SGTL5000_HEADPHONE_VOLUME, SGTL5000_HEADPHONE_VOLUME); // Headphone volume

#ifdef SGTL5000_AUDIO_THRU
  //sgtl5000.audioPreProcessorEnable();
  sgtl5000.inputSelect(AUDIO_INPUT_LINEIN);
  sgtl5000.lineInLevel(5);
  //sgtl5000.adcHighPassFilterEnable();
#endif
#ifdef SGTL5000_AUDIO_ENHANCE
  sgtl5000.audioPostProcessorEnable();
  sgtl5000.init_parametric_eq(7);
  //sgtl5000.enhanceBassEnable();
  //sgtl5000.enhanceBass(1.0, 1.5, 0, 5); // enhanceBass(1.0, 1.0, 1, 2); // Configures the bass enhancement by setting the levels of the original stereo signal and the bass-enhanced mono level which will be mixed together. The high-pass filter may be enabled (0) or bypassed (1).
  //sgtl5000.surroundSoundEnable();
  //sgtl5000.surroundSound(7, 3); // Configures virtual surround width from 0 (mono) to 7 (widest). select may be set to 1 (disable), 2 (mono input) or 3 (stereo input).
  //sgtl5000.autoVolumeEnable();
  //sgtl5000.autoVolumeControl(1, 1, 1, 0.9, 0.01, 0.05);
#else
  sgtl5000.audioProcessorDisable();
  sgtl5000.autoVolumeDisable();
  sgtl5000.surroundSoundDisable();
  sgtl5000.enhanceBassDisable();
#endif

#ifdef DEBUG
  Serial.println(F("Teensy-Audio-Board enabled."));
#endif

#elif defined(TGA_AUDIO_BOARD)
  wm8731_1.enable();
  wm8731_1.volume(1.0);
#ifdef DEBUG
  Serial.println(F("TGA board enabled."));
#endif
#elif defined(I2S_AUDIO_ONLY)
#ifdef DEBUG
  Serial.println(F("I2S enabled."));
#endif
#elif defined(PT8211_AUDIO)
#ifdef DEBUG
  Serial.println(F("PT8211 enabled."));
#endif
#elif defined(TEENSY_DAC_SYMMETRIC)
  invMixer.gain(0, -1.f);
#ifdef DEBUG
  Serial.println(F("Internal DAC using symmetric outputs enabled."));
#endif
#else

#ifdef DEBUG
  Serial.println(F("Internal DAC enabled."));
#endif

#endif

  // create dynamic Dexed instances
  for (uint8_t instance_id = 0; instance_id < NUM_DEXED; instance_id++)
  {
#ifdef DEBUG
    Serial.print(F("Creating MicroDexed instance "));
    Serial.println(instance_id, DEC);
#endif
    create_audio_dexed_chain(instance_id);
  }
#ifdef DEBUG
  for (uint8_t instance_id = 0; instance_id < NUM_DEXED; instance_id++)
  {
    Serial.print(F("Instance "));
    Serial.print(instance_id);
    Serial.print(F(": "));
    Serial.print(MicroDexed[instance_id]->getMaxNotes());
    Serial.println(F(" voices"));
  }
#endif

  // Setup (PROGMEM) sample drums
#if NUM_DRUMS > 0
  // create dynamic Drum instances
  for (uint8_t instance_id = 0; instance_id < NUM_DRUMS; instance_id++)
  {
#ifdef DEBUG
    Serial.print(F("Creating Drum instance "));
    Serial.println(instance_id, DEC);
#endif
    create_audio_drum_chain(instance_id);

    drum_mixer_r.gain(instance_id, 1.0);
    drum_mixer_l.gain(instance_id, 1.0);
#ifdef USE_FX
    drum_reverb_send_mixer_r.gain(instance_id, 0.0);
    drum_reverb_send_mixer_l.gain(instance_id, 0.0);
#endif
  }
#endif

  // Setup EPiano
#if defined(USE_FX)
#if defined(USE_EPIANO)
  // EP_CHORUS
  ep_delayline_r = (int16_t*)malloc(MOD_DELAY_SAMPLE_BUFFER * sizeof(int16_t));
  if (ep_delayline_r == NULL)
  {
#ifdef DEBUG
    Serial.println(F("AudioEffectModulatedDelay R - memory allocation failed EP"));
#endif
    while (1);
  }
  ep_delayline_l = (int16_t*)malloc(MOD_DELAY_SAMPLE_BUFFER * sizeof(int16_t));
  if (ep_delayline_l == NULL)
  {
#ifdef DEBUG
    Serial.println(F("AudioEffectModulatedDelay L - memory allocation failed EP"));
#endif
    while (1);
  }

  if (!ep_modchorus.begin(ep_delayline_r, ep_delayline_l, MOD_DELAY_SAMPLE_BUFFER))
  {
#ifdef DEBUG
    Serial.println(F("AudioEffectModulatedDelayStereo - begin failed EP"));
#endif
    while (1);
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
#endif
#endif

#if defined(USE_MICROSYNTH)
  microsynth_update_settings(0);
  microsynth_update_settings(1);
#endif

  // Setup effects
#if defined(USE_FX)
  for (uint8_t instance_id = 0; instance_id < NUM_DEXED; instance_id++)
  {
    delayline[instance_id] = (int16_t*)malloc(MOD_DELAY_SAMPLE_BUFFER * sizeof(int16_t));
    if (delayline[instance_id] != NULL)
    {
      memset(delayline[instance_id], 0, MOD_DELAY_SAMPLE_BUFFER * sizeof(int16_t));
      if (!modchorus[instance_id]->begin(delayline[instance_id], MOD_DELAY_SAMPLE_BUFFER))
      {
#ifdef DEBUG
        Serial.print(F("AudioEffectModulatedDelay - begin failed ["));
        Serial.print(instance_id);
        Serial.println(F("]"));
#endif
        while (1);
      }
    }
    else
    {
#ifdef DEBUG
      Serial.print(F("AudioEffectModulatedDelay - memory allocation failed ["));
      Serial.print(instance_id);
      Serial.println(F("]"));
#endif
      while (1);
    }
  }
#ifdef DEBUG
  Serial.print(F("MOD_DELAY_SAMPLE_BUFFER="));
  Serial.print(MOD_DELAY_SAMPLE_BUFFER, DEC);
  Serial.println(F(" samples"));
#endif
#endif

#ifdef COMPILE_FOR_FLASH

  //Setup SD WAV play
  // for (uint8_t instance_id = 0; instance_id < 2; instance_id++)
  // {
#ifdef DEBUG
  Serial.print(F("Creating WAV playback instance "));
  //  Serial.println(instance_id, DEC);
#endif
  create_audio_sd_wav_preview_chain();
  master_mixer_r.gain(MASTER_MIX_CH_SD_FILE_PREVIEW, 0.4);
  master_mixer_l.gain(MASTER_MIX_CH_SD_FILE_PREVIEW, 0.4);

#endif
  // }

  //  Serial Flash Init

#ifdef COMPILE_FOR_FLASH
  if (!SerialFlash.begin(FlashChipSelect))
  {
#ifdef DEBUG
    Serial.print(F("Unable to access SPI Flash chip"));
#endif
  }
#endif

  // Start SD card

  sd_card = check_sd_cards();

  if (sd_card < 1)
  {
#ifdef DEBUG
    Serial.println(F("SD card not accessible."));
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
    Serial.println(F("SD card found."));
#endif
    check_and_create_directories();

    memset(receive_bank_filename, 0, FILENAME_LEN);
    for (uint8_t instance_id = 0; instance_id < NUM_DEXED; instance_id++)
    {
      // load default SYSEX data
      //load_sd_voice(configuration.dexed[instance_id].bank, configuration.dexed[instance_id].voice, instance_id);

      memset(g_voice_name[instance_id], 0, VOICE_NAME_LEN);
      memset(g_bank_name[instance_id], 0, BANK_NAME_LEN);
    }
  }

#if defined(USE_SEQUENCER)
  // Start timer (to avoid a crash when loading the performance data)
  sequencer_timer.begin(sequencer, seq.tempo_ms / 8, false);
#endif

  // Load initial Performance or the last used one
  initial_values(false);

  // Initialize processor and memory measurements
  AudioProcessorUsageMaxReset();
  AudioMemoryUsageMaxReset();

  // Load voices
#ifdef DEBUG
  for (uint8_t instance_id = 0; instance_id < NUM_DEXED; instance_id++)
  {
    Serial.print(F("Dexed instance "));
    Serial.print(instance_id);
    Serial.println(F(":"));
    Serial.print(F("Bank/Voice ["));
    Serial.print(configuration.dexed[instance_id].bank, DEC);
    Serial.print(F("/"));
    Serial.print(configuration.dexed[instance_id].voice, DEC);
    Serial.println(F("]"));
    Serial.print(F("Polyphony: "));
    Serial.println(configuration.dexed[instance_id].polyphony, DEC);
  }
  Serial.print(F("AUDIO_BLOCK_SAMPLES="));
  Serial.print(AUDIO_BLOCK_SAMPLES);
  Serial.print(F(" (Time per block="));
  Serial.print(1000000 / (SAMPLE_RATE / AUDIO_BLOCK_SAMPLES));
  Serial.println(F("ms)"));
#endif

#ifdef DEBUG
  Serial.println(F("Show CPU Usage"));
#endif

#if defined (DEBUG) && defined (SHOW_CPU_LOAD_MSEC)
  show_cpu_and_mem_usage();
#endif

  // Init master_mixer
#if NUM_DEXED > 1
  for (uint8_t instance_id = 0; instance_id < NUM_DEXED; instance_id++)
  {
    master_mixer_r.gain(instance_id, VOL_MAX_FLOAT);
    master_mixer_l.gain(instance_id, VOL_MAX_FLOAT);
  }
#else
  master_mixer_r.gain(MASTER_MIX_CH_DEXED1, VOL_MAX_FLOAT);
  master_mixer_l.gain(MASTER_MIX_CH_DEXED1, VOL_MAX_FLOAT);
  master_mixer_r.gain(MASTER_MIX_CH_DEXED2, 0.0);
  master_mixer_l.gain(MASTER_MIX_CH_DEXED2, 0.0);
#endif
  master_mixer_r.gain(MASTER_MIX_CH_REVERB, VOL_MAX_FLOAT);
  master_mixer_l.gain(MASTER_MIX_CH_REVERB, VOL_MAX_FLOAT);
#if NUM_DRUMS > 0
  master_mixer_r.gain(MASTER_MIX_CH_DRUMS, VOL_MAX_FLOAT);
  master_mixer_l.gain(MASTER_MIX_CH_DRUMS, VOL_MAX_FLOAT);
  seq.drums_volume = VOL_MAX_FLOAT;
#else
  master_mixer_r.gain(MASTER_MIX_CH_DRUMS, 0.0);
  master_mixer_l.gain(MASTER_MIX_CH_DRUMS, 0.0);
#endif
#if defined(USE_EPIANO)
  master_mixer_r.gain(MASTER_MIX_CH_EPIANO, VOL_MAX_FLOAT);
  master_mixer_l.gain(MASTER_MIX_CH_EPIANO, VOL_MAX_FLOAT);
#endif
#if defined(USE_MICROSYNTH)
  master_mixer_r.gain(MASTER_MIX_CH_MICROSYNTH, VOL_MAX_FLOAT);
  master_mixer_l.gain(MASTER_MIX_CH_MICROSYNTH, VOL_MAX_FLOAT);
#endif
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

#ifdef DEBUG
  Serial.println(F("<setup end>"));
#endif

  //ep_modchorus.set_bypass(true);
  if ( seq.name[0] == 0 )
    strcpy(seq.name, "INIT Perf");
  LCDML.OTHER_jumpToFunc(UI_func_voice_select);

#ifdef DEBUG
  Serial.println(F("Setup UI."));
#endif

  setup_ui();
  if ( seq.name[0] == 0 )
    strcpy(seq.name, "INIT Perf");

  //Menu Startup
  if (configuration.sys.load_at_startup_page == 0)
    LCDML.OTHER_jumpToFunc(UI_func_voice_select);
  else if (configuration.sys.load_at_startup_page == 1)
    LCDML.OTHER_jumpToFunc(UI_func_song);
  else if (configuration.sys.load_at_startup_page == 2)
    LCDML.OTHER_jumpToFunc(UI_func_seq_pattern_editor);
  else if (configuration.sys.load_at_startup_page == 3)
    LCDML.OTHER_jumpToFunc(UI_func_microsynth);
  else if (configuration.sys.load_at_startup_page == 4)
    LCDML.OTHER_jumpToFunc(UI_func_seq_tracker);
  else if (configuration.sys.load_at_startup_page == 5)
    LCDML.OTHER_jumpToFunc(UI_func_MultiSamplePlay);
  else
    LCDML.OTHER_jumpToFunc(UI_func_voice_select); //fallback to voice select

  scope.clear();

  // temporary set volumes for 1. multisample until load/save is in place
  for (uint8_t zone = 0; zone < NUM_MULTISAMPLE_ZONES; zone++)
  {
    msz[seq.active_multisample][zone].vol = 100;
    msz[seq.active_multisample][zone].pan = 20;
    msz[seq.active_multisample][zone].rev = 50;
  }
}

void draw_volmeter(int x, int y, uint8_t arr, float value)
{
  int height;
  //draw text
  display.setCursor(x, y + 4);
  if (value == 0 || value > 0.16)
  {
    height = 0;
    seq_print_formatted_number( 0, 3 );
  }
  else
  {
    height = mapfloat(value, 0.0, 0.13, 0, 99);
    seq_print_formatted_number( height, 3 );
  }
  //draw bar
  if (height > ts.displayed_peak[arr])
  {
    int z = 0;
    do {
      // display.drawFastHLine ( x, y - height + z, 19, GREEN  );
      display.drawFastHLine ( x, y - height + z, 19, ColorHSV(( 100 - height + z), 200, 200) );
      z++;
    } while (z < height - ts.displayed_peak[arr] );
    ts.displayed_peak[arr] = height;
  }
  else
  {
    if (ts.displayed_peak[arr] > 1)
    {
      display.fillRect(x, y - (ts.displayed_peak[arr]), 20, 2, COLOR_BACKGROUND);
      ts.displayed_peak[arr] = ts.displayed_peak[arr] - 2;
    }
    else if (ts.displayed_peak[arr] > 0)
    {
      display.fillRect(x, y - (ts.displayed_peak[arr]), 20, 1, COLOR_BACKGROUND);
      ts.displayed_peak[arr] = ts.displayed_peak[arr] - 1;
    }
  }
}

void handle_touchscreen_mixer()
{
  if (scope.scope_delay % 60 == 0)
  {
    float dr, dl;
    dr = drum_mixer_peak_l.read() / 6;
    dl = drum_mixer_peak_r.read() / 6;
    display.setTextSize(1);
    display.setTextColor(COLOR_SYSTEXT, COLOR_BACKGROUND);

    draw_volmeter(0, 170, 2, microdexed_peak_0.read() );
    draw_volmeter(CHAR_width_small * 4, 170, 3, microdexed_peak_1.read() );

    draw_volmeter(CHAR_width_small * 8, 170, 8, microsynth_peak_osc_0.read() );
    draw_volmeter(CHAR_width_small * 12, 170, 9, microsynth_peak_osc_1.read() );

    draw_volmeter(CHAR_width_small * 16, 170, 4, dr );
    draw_volmeter(CHAR_width_small * 20, 170, 5, dl );

    draw_volmeter(CHAR_width_small * 27, 170, 6, reverb_return_peak_l.read() );
    draw_volmeter(CHAR_width_small * 32, 170, 7, reverb_return_peak_r.read() );

    draw_volmeter(CHAR_width_small * 40, 170, 0, master_peak_l.read() );
    draw_volmeter(CHAR_width_small * 47, 170, 1, master_peak_r.read() );
  }
}

void sub_step_recording()
{
  if (seq.running == false)
  {
    if (seq.step_recording && seq.recording == false && seq.note_in > 0 )
    {
      uint8_t cur_step = 0;
      if (LCDML.FUNC_getID() == LCDML.OTHER_getIDFromFunction(UI_func_seq_pattern_editor))
        cur_step = seq.menu - 3;
      else if  (LCDML.FUNC_getID() == LCDML.OTHER_getIDFromFunction(UI_func_seq_vel_editor))
        cur_step = seq.menu - 1;

      if (cur_step < 16)
      {
        seq.note_data[seq.active_pattern][cur_step] = seq.note_in;
        if ( seq.content_type[seq.active_pattern] == 0 && get_sample_note(activesample) > 209 ) // pitched sample
        {
          seq.vel[seq.active_pattern][cur_step] = get_sample_note(activesample);
        }
        else
          seq.vel[seq.active_pattern][cur_step] = seq.note_in_velocity;
        setCursor_textGrid(cur_step, 1);
        display.setTextSize(2);
        if ( seq.vel[seq.active_pattern][cur_step] > 209 )
          display.setTextColor(COLOR_PITCHSMP, COLOR_BACKGROUND);
        else
          set_pattern_content_type_color(seq.active_pattern);
        display.print(seq_find_shortname(cur_step)[0]);
        seq_printVelGraphBar_single_step(cur_step, GREY1);
        print_track_steps_detailed_only_current_playing_note(0, CHAR_height * 4 + 3, cur_step);
        seq.note_in = 0;
        seq.note_in_velocity = 0;
        display.setTextSize(2);
        display.setTextColor(GREEN, GREY2);
        if (seq.auto_advance_step > 0)
        {
          if (cur_step < 15)
          {
            seq.menu = seq.menu + 1;
            setCursor_textGrid(cur_step + 1, 1);
            display.print(seq_find_shortname(cur_step + 1)[0]);
            print_track_steps_detailed_only_current_playing_note(0, CHAR_height * 4 + 3, cur_step + 1);
          }
          else
          {
            if (seq.auto_advance_step == 1) // continue auto advance after last step on first step
            {
              seq.menu = seq.menu - 15;
              setCursor_textGrid(cur_step - 15, 1);
              display.print(seq_find_shortname(cur_step - 15)[0]);
              print_track_steps_detailed_only_current_playing_note(0, CHAR_height * 4 + 3, cur_step - 15);
            }
            else //stop at last step
            {
              // disable step record
              seq.step_recording = false;
              //handleStop
              draw_button_on_grid(36, 1, "STEP", "RECORD", 2); //print step recorder icon
            }
          }
        }
      }
    }
  }
}

void loop()
{
  // MIDI input handling
  check_midi_devices();

  // check encoder
  ENCODER[ENC_L].update();
  ENCODER[ENC_R].update();

  LCDML.loop();

  if (LCDML.FUNC_getID() > _LCDML_DISP_cnt && seq.running)
    scope.draw_scope(225, 20, 92);
  else  if (LCDML.FUNC_getID() == LCDML.OTHER_getIDFromFunction(UI_func_voice_select))
  {
    handle_touchscreen_voice_select();
    scope.draw_scope(216, 32, 103);
  }
  else if (LCDML.FUNC_getID() == LCDML.OTHER_getIDFromFunction(UI_func_seq_pattern_editor) ||
           LCDML.FUNC_getID() == LCDML.OTHER_getIDFromFunction(UI_func_seq_vel_editor))
  {
    handle_touchscreen_pattern_editor();
    if (seq.running)
      scope.draw_scope(216, -9,  button_size_x * CHAR_width_small);
    else
      sub_step_recording();
  }
  else if (LCDML.FUNC_getID() == LCDML.OTHER_getIDFromFunction(UI_func_microsynth))
  {
    handle_touchscreen_microsynth();
    scope.draw_scope(253, 34, 58);
  }
  else if (LCDML.FUNC_getID() == LCDML.OTHER_getIDFromFunction(UI_func_file_manager))
    handle_touchscreen_file_manager();
  else if (LCDML.FUNC_getID() == LCDML.OTHER_getIDFromFunction(UI_func_seq_mute_matrix))
    handle_touchscreen_mute_matrix();
  else if (LCDML.FUNC_getID() == LCDML.OTHER_getIDFromFunction(UI_func_colors))
    handle_touchscreen_color_edit();
  else if (LCDML.FUNC_getID() == LCDML.OTHER_getIDFromFunction(UI_func_custom_mappings))
    handle_touchscreen_custom_mappings();
  else if (LCDML.FUNC_getID() == LCDML.OTHER_getIDFromFunction(UI_func_cc_mappings))
    handle_touchscreen_cc_mappings();
  else if (LCDML.FUNC_getID() == LCDML.OTHER_getIDFromFunction(UI_func_mixer))
  {
    handle_touchscreen_mixer();
    scope.draw_scope(225, 0, 80);
  }
  else if (LCDML.FUNC_getID() == LCDML.OTHER_getIDFromFunction(UI_func_arpeggio))
  {
    scope.draw_scope(232, -2, 64);
    handle_touchscreen_arpeggio();
  }
  if (microsynth_lfo_control_rate > MICROSYNTH_LFO_RATE_MS) //update lfos, filters etc. when played live or by seq.
  {
    microsynth_lfo_control_rate = 0;
    update_microsynth_params();
    //Microsynth Realtime Screen Updates
    if (LCDML.FUNC_getID() == LCDML.OTHER_getIDFromFunction(UI_func_microsynth) &&
        microsynth[microsynth_selected_instance].pwm_last_displayed != microsynth[microsynth_selected_instance].pwm_current && seq.cycle_touch_element != 1)
    {
      display.setTextSize(1);
      setCursor_textGrid_small(15, 18);//phtodo
      display.setTextColor(GREY2, COLOR_BACKGROUND);
      seq_print_formatted_number( microsynth[microsynth_selected_instance].pwm_current, 3);
      microsynth[microsynth_selected_instance].pwm_last_displayed = microsynth[microsynth_selected_instance].pwm_current;
    }
    if (LCDML.FUNC_getID() == LCDML.OTHER_getIDFromFunction(UI_func_microsynth) && seq.cycle_touch_element != 1 &&
        microsynth[microsynth_selected_instance].filter_osc_freq_last_displayed != microsynth[microsynth_selected_instance].filter_osc_freq_current)
    {
      display.setTextSize(1);
      setCursor_textGrid_small(15, 15);
      display.setTextColor(GREY2, COLOR_BACKGROUND);
      seq_print_formatted_number( microsynth[microsynth_selected_instance].filter_osc_freq_current / 100, 3);
      microsynth[microsynth_selected_instance].filter_osc_freq_last_displayed = microsynth[microsynth_selected_instance].filter_osc_freq_current;
    }

    if (LCDML.FUNC_getID() == LCDML.OTHER_getIDFromFunction(UI_func_microsynth) ) //debug
    {
      display.setTextSize(1);
      display.setTextColor(GREY2, COLOR_BACKGROUND);
      setCursor_textGrid_small(42, 9);
      seq_print_formatted_number( microsynth[0].lfo_delay , 4);

      setCursor_textGrid_small(42, 10);
      seq_print_formatted_number( microsynth[0].lfo_fade, 4);
    }
  }
  if (control_rate > CONTROL_RATE_MS)
  {
    control_rate = 0;

    if (seq.running && seq.step != seq.UI_last_seq_step )
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

    if (LCDML.FUNC_getID() == LCDML.OTHER_getIDFromFunction(UI_func_voice_select)) // draw MIDI in activity bars
    {
      for (uint8_t instance_id = 0; instance_id < NUM_DEXED; instance_id++)
      {
        if (midi_decay_timer_dexed > MIDI_DECAY_TIMER && midi_decay_dexed[instance_id] > 0)
        {
          midi_decay_dexed[instance_id]--;
          drawBitmap(177 + (instance_id * 12), 16, special_chars[15 - (7 - midi_decay_dexed[instance_id])], 8, 8, COLOR_PITCHSMP, COLOR_BACKGROUND);
        }
        else if (midi_voices[instance_id] == 0 && midi_decay_dexed[instance_id] == 0 && !MicroDexed[instance_id]->getSustain())
        {
          midi_decay_dexed[instance_id]--;
          display.fillRect(180 + (instance_id * 12), 23, 5, 1, COLOR_BACKGROUND); // blank
        }
      }
      if (midi_decay_timer_dexed > MIDI_DECAY_LEVEL_TIME)
      {
        midi_decay_timer_dexed = 0;
      }
    }
    else if (LCDML.FUNC_getID() == LCDML.OTHER_getIDFromFunction(UI_func_microsynth)) // draw MIDI in activity bars on microsynth page
    {
      for (uint8_t instance_id = 0; instance_id < NUM_MICROSYNTH; instance_id++)
      {

        if (midi_decay_timer_microsynth > MIDI_DECAY_TIMER && midi_decay_microsynth[instance_id] > 0)
        {
          midi_decay_microsynth[instance_id]--;
          drawBitmap( 13 * 6 - 3 + (instance_id * 12), 18, special_chars[15 - (7 - midi_decay_microsynth[instance_id])], 8, 8, COLOR_PITCHSMP, COLOR_BACKGROUND);
        }
        else if (midi_decay_microsynth[instance_id] == 0)
          display.fillRect( 13 * 6 + (instance_id * 12), 25, 5, 1, COLOR_BACKGROUND); // blank

        if (midi_decay_timer_microsynth > MIDI_DECAY_LEVEL_TIME)
        {
          midi_decay_timer_microsynth = 0;
        }
      }
    }
  }
  else
    yield();

  // SAVE-SYS-EVENT-HANDLING
  if (save_sys > SAVE_SYS_MS && save_sys_flag == true)
  {
#ifdef DEBUG
    Serial.println(F("Check if we can save configuration.sys"));
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
    //    if (instance_is_playing == false)
    //    {
    //      for (uint8_t instance_id = 0; instance_id < NUM_DRUMS; instance_id++)
    //      {
    //        if (Drum[instance_id]->isPlaying())
    //        {
    //          instance_is_playing = true;
    //          break;
    //        }
    //      }
    //    }
    if (instance_is_playing == false)
      save_sd_sys_json();
    else
    {
#ifdef DEBUG
      Serial.println(F("System is playing, next try..."));
#endif
      save_sys = 0;
    }
  }

#if defined (DEBUG) && defined (SHOW_CPU_LOAD_MSEC)
  if (cpu_mem_millis >= SHOW_CPU_LOAD_MSEC)
  {
    //    if (master_peak_r.available())
    //      if (master_peak_r.read() == 1.0)
    //        peak_r++;
    //    if (master_peak_l.available())
    //      if (master_peak_l.read() == 1.0)
    //        peak_l++;
    //    if (microdexed_peak.available())
    //    {
    //      peak_dexed_value = microdexed_peak.read();
    //      if (peak_dexed_value > 0.99)
    //        peak_dexed++;
    //    }
    cpu_mem_millis -= SHOW_CPU_LOAD_MSEC;
    show_cpu_and_mem_usage();
  }
#endif
}

/******************************************************************************
  MIDI MESSAGE HANDLER
******************************************************************************/

void playWAVFile(const char *filename)
{
  //sd_WAV[fm.sd_preview_slot]->play(filename);
  //sd_WAV.stop();
  sd_WAV.play(filename);

  // A brief delay for the library to read WAV info
  //delay(25);
  // Simply wait for the file to finish playing.
  while (sd_WAV.isPlaying())
  {

  }
}

#ifdef COMPILE_FOR_FLASH
void Multi_Sample_Player(byte inNumber, byte inVelocity, byte presetslot)
{
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

        float pan = mapfloat(msz[presetslot][y].pan, PANORAMA_MIN, PANORAMA_MAX, 0.0, 1.0);
        drum_mixer_r.gain(slot, (1.0 - pan) * volume_transform(mapfloat(inVelocity * msz[presetslot][y].vol, 0, 127 * 100, 0.0, 0.7)));
        drum_mixer_l.gain(slot, pan * volume_transform(mapfloat(inVelocity * msz[presetslot][y].vol, 0, 127 * 100, 0.0, 0.7)));
#ifdef USE_FX
        drum_reverb_send_mixer_r.gain(slot, volume_transform(mapfloat(msz[presetslot][y].rev, 0, 100, 0.0, 1.0)));
        drum_reverb_send_mixer_l.gain(slot, volume_transform(mapfloat(msz[presetslot][y].rev, 0, 100, 0.0, 1.0)));
#endif

        Drum[slot]->setPlaybackRate(  (float)pow (2, (inNumber - msz[presetslot][y].rootnote - 12) / 12.00)   );
        Drum[slot]->playWav( msz[presetslot][y].name );

        if ( LCDML.FUNC_getID() == LCDML.OTHER_getIDFromFunction(UI_func_MultiSamplePlay) )
        {

          display.fillRect (0, 185 + y * 5, 2 * CHAR_width_small + msz[presetslot][y].low * 3.5 - (24 * 3.5) - 1 , 5, COLOR_BACKGROUND);

          display.fillRect (2 * CHAR_width_small + msz[presetslot][y].low * 3.5 - (24 * 3.5), 185 + y * 5,
                            (msz[presetslot][y].high - msz[presetslot][y].low) * 3.5 + 2.5 , 5, RED);

          display.fillRect (2 * CHAR_width_small + msz[presetslot][y].high * 3.5 - (24 * 3.5) + 3.5,  185 + y * 5,
                            DISPLAY_WIDTH - (msz[presetslot][y].high  * 3.5  )  + (18 * 3.5) , 5, COLOR_BACKGROUND);

          display.fillRect (2 * CHAR_width_small + msz[presetslot][y].rootnote * 3.5 - (24 * 3.5) - 1 ,  185 + y * 5 + 1,
                            3.5 + 1 , 5 - 2, COLOR_SYSTEXT);
        }
      }
      //      else
      //      {
      //        if ( LCDML.FUNC_getID() == LCDML.OTHER_getIDFromFunction(UI_func_MultiSamplePlay) )
      //        {
      //          display.fillRect (2 * CHAR_width_small + msz[presetslot][y].low * 3.5 - (24 * 3.5), 185 + y * 5,
      //                            (msz[presetslot][y].high - msz[presetslot][y].low) * 3.5 + 2.5 , 5, get_multisample_zone_color(y));
      //          display.fillRect (2 * CHAR_width_small + msz[presetslot][y].rootnote * 3.5 - (24 * 3.5) - 1 ,  185 + y * 5 + 1,
      //                            3.5 + 1 , 5 - 2, COLOR_SYSTEXT);
      //        }
      //      }
    }
  }
}
#endif

void learn_key(byte inChannel, byte inNumber)
{
  uint8_t found = 199;

  if (inChannel == DRUM_MIDI_CHANNEL)
  {
    for (uint8_t c = 0; c < NUM_CUSTOM_MIDI_MAPPINGS; c++)
    {
      if (inNumber == custom_midi_map[c].in && custom_midi_map[c].type == 1)
      {
        found = c;
        break;
      }
    }
    if (found != 199) //remap to new destination if it was already mapped before
    {
      custom_midi_map[found].in = inNumber;
      custom_midi_map[found].out = drum_config[activesample].midinote;
      custom_midi_map[found].type = 1;
      custom_midi_map[found].channel = DRUM_MIDI_CHANNEL;
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
        custom_midi_map[found].channel = DRUM_MIDI_CHANNEL;
      }
      else
        ; // can not be mapped, no empty slot left
    }
  }
  seq.midi_learn_active = false;
  update_midi_learn_button();
  print_custom_mappings();
}

void learn_cc(byte inChannel, byte inNumber)
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
  if (found != 199) //remap to new destination if it was already mapped before
  {
    custom_midi_map[found].in = inNumber;
    custom_midi_map[found].out = cc_dest_values[seq.temp_select_menu];
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
      custom_midi_map[found].out = cc_dest_values[seq.temp_select_menu];
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

void handleNoteOn(byte inChannel, byte inNumber, byte inVelocity, byte device)
{
  if ( ( seq.running == false && LCDML.FUNC_getID() == LCDML.OTHER_getIDFromFunction(UI_func_seq_pattern_editor)) ||
       ( seq.running == false && LCDML.FUNC_getID() == LCDML.OTHER_getIDFromFunction(UI_func_velocity_level)))
  {
    // is in pattern editor and sequencer is not running, play the actual sound that will be used for the pattern
    //dexed instance 0+1,  2 = epiano , 3+4 = MicroSynth, 5-14 MultiSample 15-30 = MIDI OUT USB, 31-46 MIDI OUT DIN

    if (seq.current_track_type_of_active_pattern == 0) // drums
      inChannel = DRUM_MIDI_CHANNEL;
    else
    {
      uint8_t trk = 0;
      trk = seq.instrument[find_track_in_song_where_pattern_is_used(seq.active_pattern)];
      if (trk == 0)
        inChannel = configuration.dexed[0].midi_channel;
      else  if (trk == 1)
        inChannel = configuration.dexed[1].midi_channel;
      else  if (trk == 2)
        inChannel = configuration.epiano.midi_channel;
      else  if (trk == 3)
        inChannel = microsynth[0].midi_channel;
      else  if (trk == 4)
        inChannel = microsynth[1].midi_channel;
    }
  }

#ifdef MIDI_DEVICE_USB_HOST
  if (device == 1)
    midi_usb.sendNoteOn(inNumber, inVelocity, inChannel);
#endif
#ifdef MIDI_DEVICE_DIN
  if (device == 2)
  {
    midi_serial.sendNoteOn(inNumber, inVelocity, inChannel);
    //#ifdef DEBUG
    //    Serial.print(F(" DIN OUT Channel:"));
    //    Serial.print(inChannel);
    //    Serial.println(" ");
    //#endif
  }
#endif

#ifdef COMPILE_FOR_FLASH
  if (device == 3) //playing Multisample by Sequencer
  {
    Multi_Sample_Player(inNumber, inVelocity, inChannel);
  }
#endif

  if (device == 0)
  {
#ifdef COMPILE_FOR_FLASH
    if ( LCDML.FUNC_getID() == LCDML.OTHER_getIDFromFunction(UI_func_MultiSamplePlay) && seq.running == false)
    {
      Multi_Sample_Player(inNumber, inVelocity, seq.active_multisample);  //play multisample live with manual preset selection
    }
    else
    {
#endif
      if (seq.midi_learn_active && LCDML.FUNC_getID() == LCDML.OTHER_getIDFromFunction(UI_func_custom_mappings) )
        learn_key(inChannel, inNumber);

      if (activesample < 6 && seq.running == false && LCDML.FUNC_getID() == LCDML.OTHER_getIDFromFunction(UI_func_seq_pattern_editor) ) // live play pitched sample
      {
        if (drum_counter >= NUM_DRUMS)
          drum_counter = 0;
        uint8_t slot = drum_get_slot(drum_config[activesample].drum_class);
        float pan = mapfloat(drum_config[activesample].pan, -1.0, 1.0, 0.0, 1.0);
        drum_mixer_r.gain(slot, (1.0 - pan) * drum_config[activesample].vol_max);
        drum_mixer_l.gain(slot, pan * drum_config[activesample].vol_max);
#ifdef USE_FX
        drum_reverb_send_mixer_r.gain(slot, (1.0 - pan) * volume_transform(drum_config[activesample].reverb_send));
        drum_reverb_send_mixer_l.gain(slot, pan * volume_transform(drum_config[activesample].reverb_send));
#endif
        // if (drum_config[activesample].drum_data != NULL && drum_config[activesample].len > 0)
        // {

        //        Drum[slot]->enableInterpolation(true);
        //        Drum[slot]->setPlaybackRate(  (float)pow (2, (inNumber - 72) / 12.00) * drum_config[activesample].p_offset   );
        //
        //        Drum[slot]->playRaw((int16_t*)drum_config[activesample].drum_data, drum_config[activesample].len, 1);

        // }
      }

      //Ignore the note when playing & recording the same note into the sequencer
      if (seq.recording == false || (seq.recording && inNumber != seq.note_in ))
      {
        // Check for MicroDexed
        for (uint8_t instance_id = 0; instance_id < NUM_DEXED; instance_id++)
        {
          if (checkMidiChannel(inChannel, instance_id))
          {
            if (inNumber >= configuration.dexed[instance_id].lowest_note && inNumber <= configuration.dexed[instance_id].highest_note)
            {
              if (configuration.dexed[instance_id].polyphony > 0)
                MicroDexed[instance_id]->keydown(inNumber, uint8_t(float(configuration.dexed[instance_id].velocity_level / 127.0)*inVelocity + 0.5));

              midi_voices[instance_id]++;

              if (LCDML.FUNC_getID() == LCDML.OTHER_getIDFromFunction(UI_func_voice_select))
              {
                midi_decay_timer_dexed = 0;
                midi_decay_dexed[instance_id] = min(inVelocity / 5, 7);
              }

              //#ifdef DEBUG
              //                char note_name[4];
              //                getNoteName(note_name, inNumber);
              //                Serial.print(F("Keydown "));
              //                Serial.print(note_name);
              //                Serial.print(F(" instance "));
              //                Serial.print(instance_id, DEC);
              //                Serial.print(F(" MIDI-channel "));
              //                Serial.print(inChannel, DEC);
              //                Serial.println();
              //#endif
              return;
            }
          }
        }

        // Check for MicroSynth
        for (uint8_t instance_id = 0; instance_id < NUM_MICROSYNTH; instance_id++)
        {
          if (inChannel == microsynth[instance_id].midi_channel)
          {
            if (inNumber == MIDI_C8)  // is noise only, mute osc
            {
              microsynth_noise[instance_id].amplitude(microsynth[instance_id].noise_vol / 100.1);
              microsynth_envelope_noise[instance_id].noteOn();
              microsynth_waveform[instance_id].amplitude(0);
            }
            else
            {
              if (microsynth[instance_id].trigger_noise_with_osc)
              {
                microsynth_noise[instance_id].amplitude(microsynth[instance_id].noise_vol / 100.1);
                microsynth_envelope_noise[instance_id].noteOn();
                microsynth_waveform[instance_id].amplitude(  mapfloat(
                      ((microsynth[instance_id].sound_intensity / 127.0)*inVelocity + 0.5), MS_SOUND_INTENSITY_MIN, MS_SOUND_INTENSITY_MAX, 0, 0.25f));
              }
              else
              {
                microsynth_noise[instance_id].amplitude(0.0f);
                microsynth_waveform[instance_id].amplitude(  mapfloat(
                      ((microsynth[instance_id].sound_intensity / 127.0)*inVelocity + 0.5), MS_SOUND_INTENSITY_MIN, MS_SOUND_INTENSITY_MAX, 0, 0.25f));

              }
            }
            if (microsynth[instance_id].wave == 4 || microsynth[instance_id].wave == 7)
            {
              microsynth_waveform[instance_id].pulseWidth(microsynth[instance_id].pwm_from / 2000.1);
              microsynth[instance_id].pwm_current = microsynth[instance_id].pwm_from;
            }
            microsynth_filter_osc[instance_id].frequency(microsynth[instance_id].filter_osc_freq_from);
            microsynth[instance_id].filter_osc_freq_current = microsynth[instance_id].filter_osc_freq_from;
            microsynth_filter_noise[instance_id].frequency(microsynth[instance_id].filter_noise_freq_from);
            microsynth[instance_id].filter_noise_freq_current = microsynth[instance_id].filter_noise_freq_from;
            microsynth_waveform[instance_id].frequency(  tune_frequencies2_PGM[inNumber + microsynth[instance_id].coarse]  );
            microsynth[instance_id].osc_freq_current = tune_frequencies2_PGM[inNumber + microsynth[instance_id].coarse] ;

            microsynth_envelope_osc[instance_id].noteOn();
            microsynth_lfo_delay_timer[instance_id] = 0;
            microsynth[instance_id].lfo_fade = 0;
            if (microsynth[instance_id].lfo_mode > 0) // If LFO in 1up or 1down
              microsynth[instance_id].lfo_value = 0;
            if (LCDML.FUNC_getID() == LCDML.OTHER_getIDFromFunction(UI_func_microsynth))
            {
              midi_decay_timer_microsynth = 0;
              midi_decay_microsynth[instance_id] = min(inVelocity / 5, 7);
            }
          }
        }

#if NUM_DRUMS > 0
        // Check for Drum
        if (inChannel == drum_midi_channel || drum_midi_channel == MIDI_CHANNEL_OMNI)
        {
          if (drum_counter >= NUM_DRUMS)
            drum_counter = 0;

          //check custom midi mapping
          for (uint8_t c = 0; c < NUM_CUSTOM_MIDI_MAPPINGS; c++)
          {
            if (inNumber == custom_midi_map[c].in && custom_midi_map[c].type == 1)
            {
              inNumber = custom_midi_map[c].out;
              break;
            }
          }

          for (uint8_t d = 0; d < NUM_DRUMSET_CONFIG; d++)
          {
            if (inNumber == drum_config[d].midinote)
            {
#ifdef COMPILE_FOR_FLASH
              char temp_name[16];
#endif

#ifdef COMPILE_FOR_SDCARD
              char temp_name[26];
#endif
              uint8_t slot = drum_get_slot(drum_config[d].drum_class);

              if (slot != 99)
              {
                float pan = mapfloat(drum_config[d].pan, -1.0, 1.0, 0.0, 1.0);

                drum_mixer_r.gain(slot, (1.0 - pan) * volume_transform(mapfloat(inVelocity, 0, 127, drum_config[d].vol_min, drum_config[d].vol_max)));
                drum_mixer_l.gain(slot, pan * volume_transform(mapfloat(inVelocity, 0, 127, drum_config[d].vol_min, drum_config[d].vol_max)));
#ifdef USE_FX
                drum_reverb_send_mixer_r.gain(slot, (1.0 - pan) * volume_transform(drum_config[d].reverb_send));
                drum_reverb_send_mixer_l.gain(slot, pan * volume_transform(drum_config[d].reverb_send));
#endif

#ifdef COMPILE_FOR_PROGMEM
                if (drum_config[d].drum_data != NULL && drum_config[d].len > 0)
                {
#endif
                  if (drum_config[d].pitch != 0.0)
                  {
                    Drum[slot]->enableInterpolation(true);
                    Drum[slot]->setPlaybackRate(drum_config[d].pitch);
                  }

#ifdef COMPILE_FOR_PROGMEM
                  Drum[slot]->playRaw((int16_t*)drum_config[d].drum_data, drum_config[d].len, 1);
#endif
#ifdef COMPILE_FOR_FLASH

                  sprintf(temp_name, "%s.wav", drum_config[d].name);
                  Drum[slot]->playWav(temp_name);
                  //Drum[slot]->playWav("DMpop.wav");  //Test
#endif

#ifdef COMPILE_FOR_SDCARD
                  strcpy(temp_name, "/DRUMS/");
                  strcat(temp_name, drum_config[d].name);
                  strcat(temp_name, ".wav");
                  Drum[slot]->playWav(temp_name);
#endif

                  //#ifdef DEBUG
                  //        char note_name[4];
                  //        getNoteName(note_name, inNumber);
                  //        Serial.print(F("=> Drum["));
                  //        Serial.print(slot, DEC);
                  //        Serial.print(F("]: "));
                  //        Serial.println(note_name);
                  //#endif

#ifdef COMPILE_FOR_PROGMEM
                }
#endif
                //#ifdef DEBUG
                //                Serial.print(F("Drum "));
                //                Serial.print(drum_config[d].shortname);
                //                Serial.print(F(" ["));
                //                Serial.print(drum_config[d].name);
                //                Serial.print(F("], Slot "));
                //                Serial.print(slot);
                //                Serial.print(F(": V"));
                //                Serial.print(mapfloat(inVelocity, 0, 127, drum_config[d].vol_min, drum_config[d].vol_max), 2);
                //                Serial.print(F(" P"));
                //                Serial.print(drum_config[d].pan, 2);
                //                Serial.print(F(" PAN"));
                //                Serial.print(pan, 2);
                //                Serial.print(F(" RS"));
                //                Serial.println(drum_config[d].reverb_send, 2);
                //#endif
                break;
              }
            }
          }
#endif
        }
        //
        // E-Piano
        //
#if defined(USE_EPIANO)
        if (configuration.epiano.midi_channel == MIDI_CHANNEL_OMNI || configuration.epiano.midi_channel == inChannel)
        {
          if (inNumber >= configuration.epiano.lowest_note && inNumber <= configuration.epiano.highest_note)
          {
            ep.noteOn(inNumber + configuration.epiano.transpose - 24, inVelocity);
            //#ifdef DEBUG
            //              char note_name[4];
            //              getNoteName(note_name, inNumber);
            //              Serial.print(F("KeyDown "));
            //              Serial.print(note_name);
            //              Serial.print(F(" EPIANO "));
            //              Serial.print(F(" MIDI - channel "));
            //              Serial.print(inChannel, DEC);
            //              Serial.println();
            //#endif
          }
        }
#endif
      }

#ifdef COMPILE_FOR_FLASH
    }
#endif

  }
}

void stop_all_drum_slots()
{
  for (uint8_t i = 0; i < NUM_DRUMS; i++)
  {
    if (Drum[i]->isPlaying())
    {
      Drum[i]->stop();
      drum_type[i] = DRUM_NONE;
      Drum[i]->enableInterpolation(false);
      Drum[i]->setPlaybackRate(1.0);
    }
  }
}

uint8_t drum_get_slot(uint8_t dt)
{
  bool found = false;

  for (uint8_t i = 0; i < NUM_DRUMS; i++)
  {
    if (!Drum[i]->isPlaying() && found == false)
    {
      drum_type[i] = DRUM_NONE;
      Drum[i]->enableInterpolation(false);
      Drum[i]->setPlaybackRate(1.0);
      found = true;
      return (i);
    }
  }

  if (found == false)
  {
    Drum[drum_counter]->stop();
    drum_type[drum_counter] = DRUM_NONE;
    Drum[drum_counter]->enableInterpolation(false);
    Drum[drum_counter]->setPlaybackRate(1.0);
    drum_type[drum_counter % NUM_DRUMS] = dt;

#ifdef DEBUG
    Serial.print(F("Stopping sample "));
    Serial.print(drum_counter);
    Serial.print(F(" type "));
    Serial.println(dt);
#endif

    drum_counter++;
    return (drum_counter - 1 % NUM_DRUMS);

  }
  //  #ifdef DEBUG
  //    Serial.print(F("Using next free Drum slot "));
  //    Serial.println(drum_counter % NUM_DRUMS);
  //  #endif

  // do not play the sample
  return (99);
}

void handleNoteOff(byte inChannel, byte inNumber, byte inVelocity, byte device)
{
  if ( ( seq.running == false && LCDML.FUNC_getID() == LCDML.OTHER_getIDFromFunction(UI_func_seq_pattern_editor)) ||
       ( seq.running == false && LCDML.FUNC_getID() == LCDML.OTHER_getIDFromFunction(UI_func_velocity_level)))
  {
    // is in pattern editor and sequencer is not running, play the actual sound that will be used for the pattern
    //dexed instance 0+1,  2 = epiano , 3+4 = MicroSynth, 5-20 = MIDI OUT USB, 21-36 MIDI OUT DIN

    if (seq.current_track_type_of_active_pattern == 0) // drums
      inChannel = DRUM_MIDI_CHANNEL;
    else
    {
      uint8_t trk = 0;
      trk = seq.instrument[find_track_in_song_where_pattern_is_used(seq.active_pattern)];
      if (trk == 0)
        inChannel = configuration.dexed[0].midi_channel;
      else  if (trk == 1)
        inChannel = configuration.dexed[1].midi_channel;
      else  if (trk == 2)
        inChannel = configuration.epiano.midi_channel;
      else  if (trk == 3)
        inChannel = microsynth[0].midi_channel;
      else  if (trk == 4)
        inChannel = microsynth[1].midi_channel;
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

  if (device == 0)
  {
#if defined(USE_MICROSYNTH)
    for (uint8_t instance_id = 0; instance_id < NUM_MICROSYNTH; instance_id++)
    {
      if (inChannel == microsynth[instance_id].midi_channel)
      {
        microsynth_envelope_osc[instance_id].noteOff();
        if (inNumber == MIDI_C8 || microsynth[instance_id].trigger_noise_with_osc )  // is noise only or is osc_with_noise
          microsynth_envelope_noise[instance_id].noteOff();
      }
    }
#endif

    for (uint8_t instance_id = 0; instance_id < NUM_DEXED; instance_id++)
    {
      if (checkMidiChannel(inChannel, instance_id))
      {
        if (inNumber >= configuration.dexed[instance_id].lowest_note && inNumber <= configuration.dexed[instance_id].highest_note)
        {
          if (configuration.dexed[instance_id].polyphony > 0)
            MicroDexed[instance_id]->keyup(inNumber);
          midi_voices[instance_id]--;
          //#ifdef DEBUG
          //        char note_name[4];
          //        getNoteName(note_name, inNumber);
          //        Serial.print(F("KeyUp "));
          //        Serial.print(note_name);
          //        Serial.print(F(" instance "));
          //        Serial.print(instance_id, DEC);
          //        Serial.print(F(" MIDI - channel "));
          //        Serial.print(inChannel, DEC);
          //        Serial.println();
          //#endif
        }
      }
    }

#if defined(USE_EPIANO)
    if (configuration.epiano.midi_channel == MIDI_CHANNEL_OMNI || configuration.epiano.midi_channel == inChannel)
    {
      if (inNumber >= configuration.epiano.lowest_note && inNumber <= configuration.epiano.highest_note)
      {
        ep.noteOff(inNumber + configuration.epiano.transpose - 24);
#ifdef DEBUG
        char note_name[4];
        getNoteName(note_name, inNumber);
        Serial.print(F("KeyUp "));
        Serial.print(note_name);
        Serial.print(F(" EPIANO "));
        Serial.print(F(" MIDI - channel "));
        Serial.print(inChannel, DEC);
        Serial.println();
#endif
      }
    }
#endif
  }
}

void handleControlChange(byte inChannel, byte inCtrl, byte inValue)
{
  inCtrl = constrain(inCtrl, 0, 127);
  inValue = constrain(inValue, 0, 127);

  if (seq.midi_learn_active && LCDML.FUNC_getID() == LCDML.OTHER_getIDFromFunction(UI_func_cc_mappings) )
    learn_cc(inChannel, inCtrl);
  else
  {

    //check custom midi mapping
    for (uint8_t c = 0; c < NUM_CUSTOM_MIDI_MAPPINGS; c++)
    {
      if (inCtrl == custom_midi_map[c].in && custom_midi_map[c].type == 2)
      {
        inCtrl = custom_midi_map[c].out;
        inChannel = custom_midi_map[c].channel;
        break;
      }
    }

    for (uint8_t instance_id = 0; instance_id < NUM_DEXED; instance_id++)
    {
      if (checkMidiChannel(inChannel, instance_id))
      {
#ifdef DEBUG
        Serial.print(F("INSTANCE "));
        Serial.print(instance_id, DEC);
        Serial.print(F(": CC#"));
        Serial.print(inCtrl, DEC);
        Serial.print(F(": "));
        Serial.println(inValue, DEC);
#endif

        ts.slowdown_UI_input++;

        switch (inCtrl) {
          case 0: // BankSelect MSB
#ifdef DEBUG
            Serial.println(F("BANK - SELECT MSB CC"));
#endif
            configuration.dexed[instance_id].bank = constrain((inValue << 7)&configuration.dexed[instance_id].bank, 0, MAX_BANKS - 1);
            /* load_sd_voice(configuration.dexed[instance_id].bank, configuration.dexed[instance_id].voice, instance_id);
              if (LCDML.FUNC_getID() == LCDML.OTHER_getIDFromFunction(UI_func_voice_select))
              {
              LCDML.OTHER_updateFunc();
              LCDML.loop_menu();
              } */
            break;
          case 1:
#ifdef DEBUG
            Serial.println(F("MODWHEEL CC"));
#endif
            MicroDexed[instance_id]->setModWheel(inValue);
            MicroDexed[instance_id]->ControllersRefresh();
            break;
          case 2:
#ifdef DEBUG
            Serial.println(F("BREATH CC"));
#endif
            MicroDexed[instance_id]->setBreathController(inValue);
            MicroDexed[instance_id]->ControllersRefresh();
            break;
          case 4:
#ifdef DEBUG
            Serial.println(F("FOOT CC"));
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
            Serial.println(F("VOLUME CC"));
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
            Serial.println(F("PANORAMA CC"));
#endif
            configuration.dexed[instance_id].pan = map(inValue, 0, 0x7f, PANORAMA_MIN, PANORAMA_MAX);
            mono2stereo[instance_id]->panorama(mapfloat(configuration.dexed[instance_id].pan, PANORAMA_MIN, PANORAMA_MAX, -1.0, 1.0));
            if (LCDML.FUNC_getID() == LCDML.OTHER_getIDFromFunction(UI_func_panorama))
            {
              LCDML.OTHER_updateFunc();
              LCDML.loop_menu();
            }
            break;
          case 32: // BankSelect LSB
#ifdef DEBUG
            Serial.println(F("BANK - SELECT LSB CC"));
#endif
            configuration.dexed[instance_id].bank = constrain(inValue, 0, MAX_BANKS - 1);
            /*load_sd_voice(configuration.dexed[instance_id].bank, configuration.dexed[instance_id].voice, instance_id);
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
            if (LCDML.FUNC_getID() == LCDML.OTHER_getIDFromFunction(UI_func_portamento_mode))
            {
              LCDML.OTHER_updateFunc();
              LCDML.loop_menu();
            }
            break;
          case 94:  // CC 94: (de)tune
            configuration.dexed[selected_instance_id].tune = map(inValue, 0, 0x7f, TUNE_MIN, TUNE_MAX);
            MicroDexed[selected_instance_id]->setMasterTune((int((configuration.dexed[selected_instance_id].tune - 100) / 100.0 * 0x4000) << 11) * (1.0 / 12));
            MicroDexed[selected_instance_id]->doRefreshVoice();
            if (LCDML.FUNC_getID() == LCDML.OTHER_getIDFromFunction(UI_func_tune))
            {
              LCDML.OTHER_updateFunc();
              LCDML.loop_menu();
            }
            break;
          case 91:  // CC 91: reverb send
            configuration.fx.reverb_send[selected_instance_id] = map(inValue, 0, 0x7f, REVERB_SEND_MIN, REVERB_SEND_MAX);
            reverb_mixer_r.gain(selected_instance_id, volume_transform(mapfloat(configuration.fx.reverb_send[selected_instance_id], REVERB_SEND_MIN, REVERB_SEND_MAX, 0.0, VOL_MAX_FLOAT)));
            reverb_mixer_l.gain(selected_instance_id, volume_transform(mapfloat(configuration.fx.reverb_send[selected_instance_id], REVERB_SEND_MIN, REVERB_SEND_MAX, 0.0, VOL_MAX_FLOAT)));
            if (LCDML.FUNC_getID() == LCDML.OTHER_getIDFromFunction(UI_func_reverb_send))
            {
              LCDML.OTHER_updateFunc();
              LCDML.loop_menu();
            }
            break;
          case 93:  // CC 93: chorus level
            configuration.fx.chorus_level[selected_instance_id] = map(inValue, 0, 0x7f, CHORUS_LEVEL_MIN, CHORUS_LEVEL_MAX);
            chorus_mixer[selected_instance_id]->gain(1, volume_transform(mapfloat(configuration.fx.chorus_level[selected_instance_id], CHORUS_LEVEL_MIN, CHORUS_LEVEL_MAX, 0.0, 0.5)));
            if (LCDML.FUNC_getID() == LCDML.OTHER_getIDFromFunction(UI_func_chorus_level))
            {
              LCDML.OTHER_updateFunc();
              LCDML.loop_menu();
            }
            break;
          case 103:  // CC 103: filter resonance
            configuration.fx.filter_resonance[instance_id] = map(inValue, 0, 0x7f, FILTER_RESONANCE_MIN, FILTER_RESONANCE_MAX);
            MicroDexed[instance_id]->setFilterResonance(mapfloat(configuration.fx.filter_resonance[instance_id], FILTER_RESONANCE_MIN, FILTER_RESONANCE_MAX, 1.0, 0.0));
            if (LCDML.FUNC_getID() == LCDML.OTHER_getIDFromFunction(UI_func_filter_resonance))
            {
              LCDML.OTHER_updateFunc();
              LCDML.loop_menu();
            }
            break;
          case 104:  // CC 104: filter cutoff
            configuration.fx.filter_cutoff[instance_id] = map(inValue, 0, 0x7f, FILTER_CUTOFF_MIN, FILTER_CUTOFF_MAX);
            MicroDexed[instance_id]->setFilterCutoff(mapfloat(configuration.fx.filter_cutoff[instance_id], FILTER_CUTOFF_MIN, FILTER_CUTOFF_MAX, 1.0, 0.0));;
            if (LCDML.FUNC_getID() == LCDML.OTHER_getIDFromFunction(UI_func_filter_cutoff))
            {
              LCDML.OTHER_updateFunc();
              LCDML.loop_menu();
            }
            break;
          case 105:  // CC 105: delay time
            configuration.fx.delay_time[instance_id] = map(inValue, 0, 0x7f, DELAY_TIME_MIN, DELAY_TIME_MAX);
            delay_fx[instance_id]->delay(0, constrain(configuration.fx.delay_time[instance_id] * 10, DELAY_TIME_MIN * 10, DELAY_TIME_MAX * 10));
            if (LCDML.FUNC_getID() == LCDML.OTHER_getIDFromFunction(UI_func_delay_time))
            {
              LCDML.OTHER_updateFunc();
              LCDML.loop_menu();
            }
            break;
          case 106:  // CC 106: delay feedback
            configuration.fx.delay_feedback[instance_id] = map(inValue, 0, 0x7f, DELAY_FEEDBACK_MIN , DELAY_FEEDBACK_MAX);
            delay_fb_mixer[instance_id]->gain(1, midi_volume_transform(map(configuration.fx.delay_feedback[instance_id], DELAY_FEEDBACK_MIN, DELAY_FEEDBACK_MAX, 0, 127))); // amount of feedback
            if (LCDML.FUNC_getID() == LCDML.OTHER_getIDFromFunction(UI_func_delay_feedback))
            {
              LCDML.OTHER_updateFunc();
              LCDML.loop_menu();
            }
            break;
          case 107:  // CC 107: delay volume
            configuration.fx.delay_level[instance_id] = map(inValue, 0, 0x7f, DELAY_LEVEL_MIN, DELAY_LEVEL_MAX);
            delay_mixer[instance_id]->gain(1, midi_volume_transform(map(configuration.fx.delay_level[instance_id], DELAY_LEVEL_MIN, DELAY_LEVEL_MAX, 0, 127)));
            if (LCDML.FUNC_getID() == LCDML.OTHER_getIDFromFunction(UI_func_delay_level))
            {
              LCDML.OTHER_updateFunc();
              LCDML.loop_menu();
            }
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
          case 200:  // CC 200: seq start/stop
            if (ts.slowdown_UI_input > 8000)
            {
              ts.slowdown_UI_input = 0;
              if (!seq.running)
                handleStart();
              else
                handleStop();
            }
            break;
          case 201:  // CC 201: seq stop
            if (seq.running)
              handleStop();
            break;
          case 202:  // CC 202: seq record
            if (seq.running)
              seq.running = true;
            seq.recording = true;
            seq.note_in = 0;
            break;
          case 203:  // CC 203: dexed panic
            MicroDexed[0]->panic();
#if NUM_DEXED > 1
            MicroDexed[1]->panic();
#endif
            break;
        }
      }
    }
  }

#if defined(USE_EPIANO)
  if (configuration.epiano.midi_channel == MIDI_CHANNEL_OMNI || configuration.epiano.midi_channel == inChannel)
    ep.processMidiController(inCtrl, inValue);
#endif
}

void handleAfterTouch(byte inChannel, byte inPressure)
{
  for (uint8_t instance_id = 0; instance_id < NUM_DEXED; instance_id++)
  {
    if (checkMidiChannel(inChannel, instance_id))
    {
      MicroDexed[instance_id]->setAftertouch(inPressure);
      MicroDexed[instance_id]->ControllersRefresh();
    }
  }
}

void handlePitchBend(byte inChannel, int inPitch)
{
  for (uint8_t instance_id = 0; instance_id < NUM_DEXED; instance_id++)
  {
    if (checkMidiChannel(inChannel, instance_id))
    {
      MicroDexed[instance_id]->setPitchbend(inPitch);
    }
  }
}

void handleProgramChange(byte inChannel, byte inProgram)
{
  for (uint8_t instance_id = 0; instance_id < NUM_DEXED; instance_id++)
  {
    if (checkMidiChannel(inChannel, instance_id))
    {
      configuration.dexed[instance_id].voice = constrain(inProgram, 0, MAX_VOICES - 1);
      load_sd_voice(configuration.dexed[instance_id].bank, configuration.dexed[instance_id].voice, instance_id);

      if (LCDML.FUNC_getID() == LCDML.OTHER_getIDFromFunction(UI_func_voice_select))
      {
        LCDML.OTHER_updateFunc();
        LCDML.loop_menu();
      }
    }
  }
}

void handleSystemExclusive(byte * sysex, uint len)
{
  for (uint8_t instance_id = 0; instance_id < NUM_DEXED; instance_id++)
  {
    if (!checkMidiChannel((sysex[2] & 0x0f) + 1 , instance_id))
    {
#ifdef DEBUG
      Serial.print(F("INSTANCE "));
      Serial.print(instance_id, DEC);
      Serial.println(F(": SYSEX - MIDI - Channel mismatch"));
#endif
      return;
    }

#ifdef DEBUG
    Serial.print(F("SysEx data length: ["));
    Serial.print(len);
    Serial.println(F("]"));

    Serial.println(F("SysEx data: "));
    for (uint16_t i = 0; i < len; i++)
    {
      Serial.print(F("[0x"));
      uint8_t s = sysex[i];
      if (s < 16)
        Serial.print(F("0"));
      Serial.print(s, HEX);
      Serial.print(F(" | "));
      if (s < 100)
        Serial.print(F("0"));
      if (s < 10)
        Serial.print(F("0"));
      Serial.print(s, DEC);
      Serial.print(F("]"));
      if ((i + 1) % 16 == 0)
        Serial.println();
    }
    Serial.println();
#endif

    // Check for SYSEX end byte
    if (sysex[len - 1] != 0xf7)
    {
#ifdef DEBUG
      Serial.println(F("E: SysEx end status byte not detected."));
#endif
      return;
    }

    if (sysex[1] != 0x43) // check for Yamaha sysex
    {
#ifdef DEBUG
      Serial.println(F("E: SysEx vendor not Yamaha."));
#endif
      return;
    }

#ifdef DEBUG
    Serial.print(F("Substatus: ["));
    Serial.print((sysex[2] & 0x70) >> 4);
    Serial.println(F("]"));
#endif

    // parse parameter change
    if (len == 7)
    {
      if (((sysex[3] & 0x7c) >> 2) != 0 && ((sysex[3] & 0x7c) >> 2) != 2)
      {
#ifdef DEBUG
        Serial.println(F("E: Not a SysEx parameter or function parameter change."));
#endif
        return;
      }

      sysex[4] &= 0x7f;
      sysex[5] &= 0x7f;

      if ((sysex[3] & 0x7c) >> 2 == 0)
      {
#ifdef DEBUG
        Serial.println(F("SysEx Voice parameter: "));
        Serial.print("Parameter #");
        Serial.print(sysex[4] + ((sysex[3] & 0x03) * 128), DEC);
        Serial.print(" Value: ");
        Serial.println(sysex[5], DEC);
#endif
        MicroDexed[instance_id]->setVoiceDataElement(sysex[4] + ((sysex[3] & 0x03) * 128), sysex[5]);
      }
      else if ((sysex[3] & 0x7c) >> 2 == 2)
      {
#ifdef DEBUG
        Serial.println(F("SysEx Function parameter: "));
        Serial.print("Parameter #");
        Serial.print(sysex[4], DEC);
        Serial.print(" Value: ");
        Serial.println(sysex[5], DEC);
#endif
        switch (sysex[4])
        {
          case 65:
            configuration.dexed[instance_id].pb_range = constrain(sysex[5], PB_RANGE_MIN, PB_RANGE_MAX);
            MicroDexed[instance_id]->setPitchbendRange(configuration.dexed[instance_id].pb_range);
            if (LCDML.FUNC_getID() == LCDML.OTHER_getIDFromFunction(UI_func_pb_range))
            {
              LCDML.OTHER_updateFunc();
              LCDML.loop_menu();
            }
            break;
          case 66:
            configuration.dexed[instance_id].pb_step = constrain(sysex[5], PB_STEP_MIN, PB_STEP_MAX);
            MicroDexed[instance_id]->setPitchbendRange(configuration.dexed[instance_id].pb_step);
            if (LCDML.FUNC_getID() == LCDML.OTHER_getIDFromFunction(UI_func_pb_step))
            {
              LCDML.OTHER_updateFunc();
              LCDML.loop_menu();
            }
            break;
          case 67:
            configuration.dexed[instance_id].portamento_mode = constrain(sysex[5], PORTAMENTO_MODE_MIN, PORTAMENTO_MODE_MAX);
            MicroDexed[instance_id]->setPortamento(configuration.dexed[instance_id].portamento_mode, configuration.dexed[instance_id].portamento_glissando, configuration.dexed[instance_id].portamento_time);
            if (LCDML.FUNC_getID() == LCDML.OTHER_getIDFromFunction(UI_func_portamento_mode))
            {
              LCDML.OTHER_updateFunc();
              LCDML.loop_menu();
            }
            break;
          case 68:
            configuration.dexed[instance_id].portamento_glissando = constrain(sysex[5], PORTAMENTO_GLISSANDO_MIN, PORTAMENTO_GLISSANDO_MAX);
            MicroDexed[instance_id]->setPortamento(configuration.dexed[instance_id].portamento_mode, configuration.dexed[instance_id].portamento_glissando, configuration.dexed[instance_id].portamento_time);
            if (LCDML.FUNC_getID() == LCDML.OTHER_getIDFromFunction(UI_func_portamento_glissando))
            {
              LCDML.OTHER_updateFunc();
              LCDML.loop_menu();
            }
            break;
          case 69:
            configuration.dexed[instance_id].portamento_time = constrain(sysex[5], PORTAMENTO_TIME_MIN, PORTAMENTO_TIME_MAX);
            MicroDexed[instance_id]->setPortamento(configuration.dexed[instance_id].portamento_mode, configuration.dexed[instance_id].portamento_glissando, configuration.dexed[instance_id].portamento_time);
            if (LCDML.FUNC_getID() == LCDML.OTHER_getIDFromFunction(UI_func_portamento_time))
            {
              LCDML.OTHER_updateFunc();
              LCDML.loop_menu();
            }
            break;
          case 70:
            configuration.dexed[instance_id].mw_range = constrain(sysex[5], MW_RANGE_MIN, MW_RANGE_MAX);
            MicroDexed[instance_id]->setModWheelRange(configuration.dexed[instance_id].mw_range);
            if (LCDML.FUNC_getID() == LCDML.OTHER_getIDFromFunction(UI_func_mw_range))
            {
              LCDML.OTHER_updateFunc();
              LCDML.loop_menu();
            }
            break;
          case 71:
            configuration.dexed[instance_id].mw_assign = constrain(sysex[5], MW_ASSIGN_MIN, MW_ASSIGN_MAX);
            MicroDexed[instance_id]->setModWheelTarget(configuration.dexed[instance_id].mw_assign);
            if (LCDML.FUNC_getID() == LCDML.OTHER_getIDFromFunction(UI_func_mw_assign))
            {
              LCDML.OTHER_updateFunc();
              LCDML.loop_menu();
            }
            break;
          case 72:
            configuration.dexed[instance_id].fc_range = constrain(sysex[5], FC_RANGE_MIN, FC_RANGE_MAX);
            MicroDexed[instance_id]->setFootControllerRange(configuration.dexed[instance_id].fc_range);
            if (LCDML.FUNC_getID() == LCDML.OTHER_getIDFromFunction(UI_func_fc_range))
            {
              LCDML.OTHER_updateFunc();
              LCDML.loop_menu();
            }
            break;
          case 73:
            configuration.dexed[instance_id].fc_assign = constrain(sysex[5], FC_ASSIGN_MIN, FC_ASSIGN_MAX);
            MicroDexed[instance_id]->setFootControllerTarget(configuration.dexed[instance_id].fc_assign);
            if (LCDML.FUNC_getID() == LCDML.OTHER_getIDFromFunction(UI_func_fc_assign))
            {
              LCDML.OTHER_updateFunc();
              LCDML.loop_menu();
            }
            break;
          case 74:
            configuration.dexed[instance_id].bc_range = constrain(sysex[5], BC_RANGE_MIN, BC_RANGE_MAX);
            MicroDexed[instance_id]->setBreathControllerRange(configuration.dexed[instance_id].bc_range);
            if (LCDML.FUNC_getID() == LCDML.OTHER_getIDFromFunction(UI_func_bc_range))
            {
              LCDML.OTHER_updateFunc();
              LCDML.loop_menu();
            }
            break;
          case 75:
            configuration.dexed[instance_id].bc_assign = constrain(sysex[5], BC_ASSIGN_MIN, BC_ASSIGN_MAX);
            MicroDexed[instance_id]->setBreathControllerTarget(configuration.dexed[instance_id].bc_assign);
            if (LCDML.FUNC_getID() == LCDML.OTHER_getIDFromFunction(UI_func_bc_assign))
            {
              LCDML.OTHER_updateFunc();
              LCDML.loop_menu();
            }
            break;
          case 76:
            configuration.dexed[instance_id].at_range = constrain(sysex[5], AT_RANGE_MIN, AT_RANGE_MAX);
            MicroDexed[instance_id]->setAftertouchRange(configuration.dexed[instance_id].at_range);
            if (LCDML.FUNC_getID() == LCDML.OTHER_getIDFromFunction(UI_func_at_range))
            {
              LCDML.OTHER_updateFunc();
              LCDML.loop_menu();
            }
            break;
          case 77:
            configuration.dexed[instance_id].at_assign = constrain(sysex[5], AT_ASSIGN_MIN, AT_ASSIGN_MAX);
            MicroDexed[instance_id]->setAftertouchTarget(configuration.dexed[instance_id].at_assign);
            if (LCDML.FUNC_getID() == LCDML.OTHER_getIDFromFunction(UI_func_at_assign))
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
        Serial.println(F("E: Unknown SysEx voice or function."));
      }
#endif
    }
    else if (len == 163)
    {
      int32_t bulk_checksum_calc = 0;
      int8_t bulk_checksum = sysex[161];

      // 1 Voice bulk upload
#ifdef DEBUG
      Serial.println(F("One Voice bulk upload"));
#endif

      if ((sysex[3] & 0x7f) != 0)
      {
#ifdef DEBUG
        Serial.println(F("E: Not a SysEx voice bulk upload."));
#endif
        return;
      }

      if (((sysex[4] << 7) | sysex[5]) != 0x9b)
      {
#ifdef DEBUG
        Serial.println(F("E: Wrong length for SysEx voice bulk upload (not 155)."));
#endif
        return;
      }

      // checksum calculation
      for (uint8_t i = 0; i < 155 ; i++)
      {
        bulk_checksum_calc -= sysex[i + 6];
      }
      bulk_checksum_calc &= 0x7f;

      if (bulk_checksum_calc != bulk_checksum)
      {
#ifdef DEBUG
        Serial.print(F("E : Checksum error for one voice [0x"));
        Serial.print(bulk_checksum, HEX);
        Serial.print(F(" / 0x"));
        Serial.print(bulk_checksum_calc, HEX);
        Serial.println(F("]"));
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
          Serial.println(F("E : Not a SysEx bank bulk upload."));
#endif
          setCursor_textGrid(1, 2);
          display.print(F("Error (TYPE)      "));
          delay(MESSAGE_WAIT_TIME);
          LCDML.FUNC_goBackToMenu();
          return;
        }

#ifdef DEBUG
        Serial.println(F("Bank bulk upload."));
#endif

        if (((sysex[4] << 7) | sysex[5]) != 0x1000)
        {
#ifdef DEBUG
          Serial.println(F("E : Wrong length for SysEx bank bulk upload (not 4096)."));
#endif
          setCursor_textGrid(1, 2);
          display.print(F("Error (SIZE)     "));
          delay(MESSAGE_WAIT_TIME);
          LCDML.FUNC_goBackToMenu();
          return;
        }

#ifdef DEBUG
        Serial.println(F("Bank type ok"));
#endif

        // checksum calculation
        for (uint16_t i = 0; i < 4096 ; i++)
        {
          bulk_checksum_calc -= sysex[i + 6];
        }
        bulk_checksum_calc &= 0x7f;

        if (bulk_checksum_calc != bulk_checksum)
        {
#ifdef DEBUG
          Serial.print(F("E : Checksum error for bank [0x"));
          Serial.print(bulk_checksum, HEX);
          Serial.print(F(" / 0x"));
          Serial.print(bulk_checksum_calc, HEX);
          Serial.println(F("]"));
#endif
          setCursor_textGrid(1, 2);
          display.print(F("Error (CHECKSUM)"));
          delay(MESSAGE_WAIT_TIME);
          LCDML.FUNC_goBackToMenu();
          return;
        }

#ifdef DEBUG
        Serial.println(F("Bank checksum ok"));
#endif

        if (save_sd_bank(receive_bank_filename, sysex))
        {
#ifdef DEBUG
          Serial.print(F("Bank saved as ["));
          Serial.print(receive_bank_filename);
          Serial.println(F("]"));
#endif
          setCursor_textGrid(1, 2);
          display.print(F("Done.           "));
          delay(MESSAGE_WAIT_TIME);
          LCDML.FUNC_goBackToMenu();
        }
        else
        {
#ifdef DEBUG
          Serial.println(F("Error during saving bank as ["));
          Serial.print(receive_bank_filename);
          Serial.println(F("]"));
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
        Serial.println(F("E : Not in MIDI receive bank mode."));
#endif
    }
#ifdef DEBUG
    else
      Serial.println(F("E : SysEx parameter length wrong."));
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

void handleClock(void)
{
  if (midi_bpm_counter % 24 == 0)
  {
    midi_bpm = (60000.0f / float(midi_bpm_timer) + 0.5);

    if (_midi_bpm > -1 && _midi_bpm != midi_bpm)
    {
#ifdef DEBUG
      Serial.print(F("MIDI Clock : "));
      Serial.print(midi_bpm);
      Serial.print(F(" bpm ("));
      Serial.print(midi_bpm_timer, DEC);
      Serial.println(F("ms per quarter)"));
#endif

#ifdef USE_FX
      /*
        1   1/16  =  6 ticks / 0.0625
        2   1/16T =  9 ticks / 0.09375
        3   1/8   = 12 ticks / 0.125
        4   1/8T  = 18 ticks / 0.1875
        5   1/4   = 24 ticks / 0.25
        6   1/4T  = 36 ticks / 0.375
        7   1/2   = 48 ticks / 0.5
        8   1/2T  = 72 ticks / 0.75
        9   1/1   = 96 ticks / 1.0
      */
      for (uint8_t instance_id = 0; instance_id < NUM_DEXED; instance_id++)
      {
        if (configuration.fx.delay_sync[instance_id] > 0)
        {
          uint16_t midi_sync_delay_time = uint16_t(60000.0 * midi_ticks_factor[configuration.fx.delay_sync[instance_id]] / float(midi_bpm) + 0.5);
          delay_fx[instance_id]->delay(0, constrain(midi_sync_delay_time, DELAY_TIME_MIN * 10, DELAY_TIME_MAX * 10));
#ifdef DEBUG
          Serial.print(F("Setting Delay - Sync of instance "));
          Serial.print(instance_id);
          Serial.print(F(" to "));
          Serial.print(constrain(midi_sync_delay_time, DELAY_TIME_MIN * 10, DELAY_TIME_MAX * 10), DEC);
          Serial.println(F(" ms"));
#endif
        }
      }
#endif
    }

    _midi_bpm = midi_bpm;
    midi_bpm_counter = 0;
    midi_bpm_timer = 0;
  }

  midi_bpm_counter++;
}

void dac_mute(void)
{
  sgtl5000.lineOutLevel(0.0);
  sgtl5000.dacVolume(0.0);
  sgtl5000.volume(0.0, 0.0); // Headphone volume
}

void dac_unmute(void)
{
  sgtl5000.lineOutLevel(SGTL5000_LINEOUT_LEVEL);
  sgtl5000.dacVolume(1.0);
  sgtl5000.volume(SGTL5000_HEADPHONE_VOLUME, SGTL5000_HEADPHONE_VOLUME); // Headphone volume
}

void handleStart(void)
{
  if (LCDML.FUNC_getID() == LCDML.OTHER_getIDFromFunction(UI_func_seq_pattern_editor) ||
      LCDML.FUNC_getID() == LCDML.OTHER_getIDFromFunction(UI_func_seq_vel_editor))
  {
    display.fillRect(36 * CHAR_width_small ,  CHAR_height_small, button_size_x * CHAR_width_small, CHAR_height_small * button_size_y, COLOR_BACKGROUND);//clear scope
  }
  midi_bpm_timer = 0;
  midi_bpm_counter = 0;
  _midi_bpm = -1;

  seq.step = 0;
  seq.current_song_step = 0;
  seq.arp_note = 0;
  seq.arp_chord = 0;

  if (seq.loop_start == 99)  // no loop start set, start at step 0
    seq.current_song_step = 0;
  else
    seq.current_song_step = seq.loop_start;

  sequencer_timer.begin(sequencer, seq.tempo_ms / 8);
  seq.running = true;

  if (LCDML.FUNC_getID() == LCDML.OTHER_getIDFromFunction(UI_func_arpeggio))
  {
    print_arp_start_stop_button();
  }
}

void handleContinue(void)
{
  ;
}

void handleStop(void)
{
#if defined(USE_SEQUENCER)
  if (seq.running)
  {
    if (LCDML.FUNC_getID() == LCDML.OTHER_getIDFromFunction(UI_func_seq_pattern_editor))
    {
      //      scope.clear();
      //       if (LCDML.FUNC_getID() == LCDML.OTHER_getIDFromFunction(UI_func_seq_pattern_editor))  // clear screen space on right side from scope in pattern editor
      //      display.fillRect(36 * CHAR_width_small + button_size_x * CHAR_width_small , 1 * CHAR_height_small, 10, CHAR_height_small * button_size_y, COLOR_BACKGROUND);
      draw_button_on_grid(36, 1, "STEP", "RECORD", 2); //print step recorder icon
    }
    MicroDexed[0]->panic();
#if NUM_DEXED > 1
    MicroDexed[1]->panic();
#endif

    sequencer_timer.stop();

    if (LCDML.FUNC_getID() == LCDML.OTHER_getIDFromFunction(UI_func_song))
    {
      display.setTextColor( GREEN, COLOR_BACKGROUND);  //play indicator song view

      if (CHAR_height_small * 8  + 10 *  ( seq.current_song_step - 1 - seq.scrollpos) > CHAR_height_small * 7)
      {
        display.setCursor(CHAR_width_small * 4 , CHAR_height_small * 8  + 10  *  (seq.current_song_step - seq.scrollpos)  );
        display.print(" ");
      }
      sub_song_print_tracknumbers();
    }

  }
  seq.running = false;
  seq.recording = false;
  seq.note_in = 0;
  seq.step = 0;
  for (uint8_t d = 0; d < NUM_SEQ_TRACKS; d++)
  {
    seq.chain_counter[d] = 0;
    seq.current_pattern[d] = 99;
  }
  if (seq.loop_start == 99)  // no loop start set, start at step 0
    seq.current_song_step = 0;
  else
    seq.current_song_step = seq.loop_start;
#endif

#if defined(USE_MICROSYNTH)
  microsynth_envelope_osc[0].noteOff();
  microsynth_envelope_osc[1].noteOff();
  microsynth_envelope_noise[0].noteOff();
  microsynth_envelope_noise[1].noteOff();
#endif

  if (LCDML.FUNC_getID() == LCDML.OTHER_getIDFromFunction(UI_func_arpeggio))
  {
    print_arp_start_stop_button();
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
    Serial.println(F("MIDI SYSEX RESET"));
#endif
    MicroDexed[instance_id]->notesOff();
    MicroDexed[instance_id]->panic();
    MicroDexed[instance_id]->resetControllers();
  }
}

/******************************************************************************
  MIDI HELPER
******************************************************************************/
bool checkMidiChannel(byte inChannel, uint8_t instance_id)
{
  // check for MIDI channel
  if (configuration.dexed[instance_id].midi_channel == MIDI_CHANNEL_OMNI)
  {
    return (true);
  }
  else if (inChannel != configuration.dexed[instance_id].midi_channel)
  {
    //#ifdef DEBUG
    //    Serial.print(F("INSTANCE "));
    //    Serial.print(instance_id, DEC);
    //    Serial.print(F(" : Ignoring MIDI data on channel "));
    //    Serial.print(inChannel);
    //    Serial.print(F("(listening on "));
    //    Serial.print(configuration.dexed[instance_id].midi_channel);
    //    Serial.println(F(")"));
    //#endif

    return (false);
  }
  return (true);
}

void init_MIDI_send_CC(void)
{
#ifdef DEBUG
  Serial.println("init_MIDI_send_CC() : ");
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

void set_drums_volume(float vol)
{
  master_mixer_r.gain(MASTER_MIX_CH_DRUMS, vol);
  master_mixer_l.gain(MASTER_MIX_CH_DRUMS, vol);
}
void set_volume(uint8_t v, uint8_t m)
{
  float tmp_v;

  configuration.sys.vol = v;

  if (configuration.sys.vol > 100)
    configuration.sys.vol = 100;
  tmp_v = float(v);

  configuration.sys.mono = m;

#ifdef DEBUG
  Serial.print(F("Setting volume : VOL = "));
  Serial.println(v, DEC);
  Serial.print(F(" V = "));
  Serial.println(volume_transform(tmp_v / 100.0));
#endif

  volume_r.gain(volume_transform(tmp_v / 100.0)*VOLUME_MULTIPLIER);
  volume_l.gain(volume_transform(tmp_v / 100.0)*VOLUME_MULTIPLIER);

  switch (m)
  {
    case 0: // stereo
      stereo2mono.stereo(true);
      for (uint8_t instance_id = 0; instance_id < NUM_DEXED; instance_id++)
        mono2stereo[instance_id]->panorama(mapfloat(configuration.dexed[instance_id].pan, PANORAMA_MIN, PANORAMA_MAX, -1.0, 1.0));
      break;
    case 1: // mono both
      stereo2mono.stereo(false);
      for (uint8_t instance_id = 0; instance_id < NUM_DEXED; instance_id++)
        mono2stereo[instance_id]->panorama(mapfloat(PANORAMA_DEFAULT, PANORAMA_MIN, PANORAMA_MAX, -1.0, 1.0));
      break;
    case 2: // mono right
      volume_l.gain(0.0);
      stereo2mono.stereo(false);
      for (uint8_t instance_id = 0; instance_id < NUM_DEXED; instance_id++)
        mono2stereo[instance_id]->panorama(mapfloat(PANORAMA_MAX, PANORAMA_MIN, PANORAMA_MAX, -1.0, 1.0));
      break;
    case 3: // mono left
      volume_r.gain(0.0);
      stereo2mono.stereo(false);
      for (uint8_t instance_id = 0; instance_id < NUM_DEXED; instance_id++)
        mono2stereo[instance_id]->panorama(mapfloat(PANORAMA_MIN, PANORAMA_MIN, PANORAMA_MAX, -1.0, 1.0));
      break;
  }
}

/******************************************************************************
  EEPROM HELPER
******************************************************************************/

void initial_values(bool init)
{
  uint16_t _m_;

  if (init == true)
    init_configuration();
  else
  {
    _m_ = (EEPROM[EEPROM_START_ADDRESS] << 8) | EEPROM[EEPROM_START_ADDRESS + 1];
    if (_m_ != EEPROM_MARKER)
    {
#ifdef DEBUG
      Serial.println(F("Found wrong EEPROM marker, initializing EEPROM..."));
#endif
      init_configuration();
      //load_sd_performance_json(PERFORMANCE_NUM_MIN);
    }
    else
    {
      load_sd_sys_json();
      if (configuration.sys.load_at_startup_performance == 255)
      {
#ifdef DEBUG
        Serial.print(F("Loading initial system data from performance "));
        Serial.println(configuration.sys.performance_number, DEC);
#endif
        load_sd_performance_json(configuration.sys.performance_number);
      }
      else if (configuration.sys.load_at_startup_performance < 100)
      {
#ifdef DEBUG
        Serial.print(F("Loading initial system data from performance "));
        Serial.println(configuration.sys.load_at_startup_performance, DEC);
#endif
        load_sd_performance_json(configuration.sys.load_at_startup_performance);
      }
      else
      {
#ifdef DEBUG
        Serial.print(F("Loading initial system data from default performance "));
        Serial.println(STARTUP_NUM_DEFAULT, DEC);
#endif
        load_sd_performance_json(STARTUP_NUM_DEFAULT);
      }
    }
#ifdef DEBUG
    Serial.println(F("OK, loaded!"));
#endif


    check_configuration();
  }
  configuration.sys.vol = EEPROM[EEPROM_START_ADDRESS + 2];
  set_volume(configuration.sys.vol, configuration.sys.mono);

#ifdef DEBUG
  show_configuration();
#endif
}

void check_configuration(void)
{
  check_configuration_sys();
  check_configuration_fx();
  for (uint8_t instance_id = 0; instance_id < NUM_DEXED; instance_id++)
    check_configuration_dexed(instance_id);
  check_configuration_epiano();
}

void check_configuration_sys(void)
{
  configuration.sys.vol = constrain(configuration.sys.vol, VOLUME_MIN, VOLUME_MAX);
  configuration.sys.mono = constrain(configuration.sys.mono, MONO_MIN, MONO_MAX);
  configuration.sys.soft_midi_thru = constrain(configuration.sys.soft_midi_thru, SOFT_MIDI_THRU_MIN, SOFT_MIDI_THRU_MAX);
  configuration.sys.favorites = constrain(configuration.sys.favorites, FAVORITES_NUM_MIN, FAVORITES_NUM_MAX);
  configuration.sys.performance_number = constrain(configuration.sys.performance_number, PERFORMANCE_NUM_MIN, PERFORMANCE_NUM_MAX);
  configuration.sys.load_at_startup_performance = constrain(configuration.sys.load_at_startup_performance, STARTUP_NUM_MIN, STARTUP_NUM_MAX);
}

void check_configuration_fx(void)
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
    configuration.fx.delay_sync[instance_id] = constrain(configuration.fx.delay_sync[instance_id], DELAY_SYNC_MIN, DELAY_SYNC_MAX);
    configuration.fx.reverb_send[instance_id] = constrain(configuration.fx.reverb_send[instance_id], REVERB_SEND_MIN, REVERB_SEND_MAX);
  }

  configuration.fx.eq_1 = constrain(configuration.fx.eq_1, EQ_1_MIN, EQ_1_MAX);
  configuration.fx.eq_2 = constrain(configuration.fx.eq_2, EQ_2_MIN, EQ_2_MAX);
  configuration.fx.eq_3 = constrain(configuration.fx.eq_3, EQ_3_MIN, EQ_3_MAX);
  configuration.fx.eq_4 = constrain(configuration.fx.eq_4, EQ_4_MIN, EQ_4_MAX);
  configuration.fx.eq_5 = constrain(configuration.fx.eq_5, EQ_5_MIN, EQ_5_MAX);
  configuration.fx.eq_6 = constrain(configuration.fx.eq_6, EQ_6_MIN, EQ_6_MAX);
  configuration.fx.eq_7 = constrain(configuration.fx.eq_7, EQ_7_MIN, EQ_7_MAX);

#if defined(USE_EPIANO)
  configuration.fx.ep_chorus_frequency = constrain(configuration.fx.ep_chorus_frequency, EP_CHORUS_FREQUENCY_MIN, EP_CHORUS_FREQUENCY_MAX);
  configuration.fx.ep_chorus_waveform = constrain(configuration.fx.ep_chorus_waveform, EP_CHORUS_WAVEFORM_MIN, EP_CHORUS_WAVEFORM_MAX);
  configuration.fx.ep_chorus_depth = constrain(configuration.fx.ep_chorus_depth, EP_CHORUS_DEPTH_MIN, EP_CHORUS_DEPTH_MAX);
  configuration.fx.ep_chorus_level = constrain(configuration.fx.ep_chorus_level, EP_CHORUS_LEVEL_MIN, EP_CHORUS_LEVEL_MAX);
  configuration.fx.ep_reverb_send = constrain(configuration.fx.ep_reverb_send, EP_REVERB_SEND_MIN, EP_REVERB_SEND_MAX);
#endif
}

void check_configuration_dexed(uint8_t instance_id)
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

void check_configuration_epiano(void)
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
  configuration.epiano.sound_intensity = constrain(configuration.epiano.sound_intensity, EP_SOUND_INTENSITY_MIN, EP_SOUND_INTENSITY_MAX);
  configuration.epiano.pan = constrain(configuration.epiano.pan, PANORAMA_MIN, PANORAMA_MAX);
  configuration.epiano.velocity_sense = constrain(configuration.epiano.velocity_sense, EP_VELOCITY_SENSE_MIN, EP_VELOCITY_SENSE_MAX);
  configuration.epiano.midi_channel = constrain(configuration.epiano.midi_channel, EP_MIDI_CHANNEL_MIN, EP_MIDI_CHANNEL_MAX);
}

void init_configuration(void)
{
#ifdef DEBUG
  Serial.println(F("INITIALIZING CONFIGURATION"));
#endif

  configuration.sys.vol = VOLUME_DEFAULT;
  configuration.sys.mono = MONO_DEFAULT;
  configuration.sys.soft_midi_thru = SOFT_MIDI_THRU_DEFAULT;
  configuration.sys.performance_number = PERFORMANCE_NUM_DEFAULT;
  configuration.sys.load_at_startup_performance = STARTUP_NUM_DEFAULT;


  configuration.fx.reverb_lowpass = REVERB_LOWPASS_DEFAULT;
  configuration.fx.reverb_lodamp = REVERB_LODAMP_DEFAULT;
  configuration.fx.reverb_hidamp = REVERB_HIDAMP_DEFAULT;
  configuration.fx.reverb_diffusion = REVERB_DIFFUSION_DEFAULT;
  configuration.fx.reverb_roomsize = REVERB_ROOMSIZE_DEFAULT;
  configuration.fx.reverb_level = REVERB_LEVEL_DEFAULT;

#if defined(USE_EPIANO)
  configuration.fx.ep_chorus_frequency = EP_CHORUS_FREQUENCY_DEFAULT;
  configuration.fx.ep_chorus_waveform = EP_CHORUS_WAVEFORM_DEFAULT;
  configuration.fx.ep_chorus_depth = EP_CHORUS_DEPTH_DEFAULT;
  configuration.fx.ep_chorus_level = EP_CHORUS_LEVEL_DEFAULT;
  configuration.fx.ep_reverb_send = EP_REVERB_SEND_DEFAULT;
#endif

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
    configuration.fx.delay_sync[instance_id] = DELAY_SYNC_DEFAULT;
    configuration.fx.reverb_send[instance_id] = REVERB_SEND_DEFAULT;

    MicroDexed[instance_id]->ControllersRefresh();
  }

#if defined(USE_EPIANO)
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
  configuration.epiano.sound_intensity = EP_SOUND_INTENSITY_DEFAULT;
  configuration.epiano.pan = PANORAMA_DEFAULT;
  configuration.epiano.velocity_sense = EP_VELOCITY_SENSE_DEFAULT;
  configuration.epiano.midi_channel = EP_MIDI_CHANNEL_DEFAULT;
#endif

  //set_volume(configuration.sys.vol, configuration.sys.mono);

  eeprom_update();
}

void eeprom_update(void)
{
  EEPROM.update(EEPROM_START_ADDRESS, (EEPROM_MARKER & 0xff00) >> 8);
  EEPROM.update(EEPROM_START_ADDRESS + 1, EEPROM_MARKER & 0xff);
  EEPROM.update(EEPROM_START_ADDRESS + 2, configuration.sys.vol);
}

/******************************************************************************
  PARAMETER-HELPERS
******************************************************************************/

void set_sample_note(uint8_t sample, uint8_t note)
{
  drum_config[sample].midinote = note;
}

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

uint8_t find_drum_number_from_note(uint8_t note)
{
  uint8_t number = 0;
  for (uint8_t d = 0; d < NUM_DRUMSET_CONFIG - 1; d++)
  {
    if (note == drum_config[d].midinote)
    {
      number = d;
      break;
    }
  }
  return number;
}

void set_fx_params(void)
{
#if defined(USE_FX)
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

    // phtodo test if working now:
    // chorus_modulator[instance_id]->frequency
    // chorus_modulator[instance_id]->amplitude
    // 8.1.2022 it is not, check with HW

    //  chorus_modulator[instance_id]->frequency(configuration.fx.chorus_frequency[instance_id] / 10.0);
    //  chorus_modulator[instance_id]->amplitude(mapfloat(configuration.fx.chorus_depth[instance_id], CHORUS_DEPTH_MIN, CHORUS_DEPTH_MAX, 0.0, 1.0));
    chorus_modulator[instance_id]->offset(0.0);
#if MOD_FILTER_OUTPUT == MOD_BUTTERWORTH_FILTER_OUTPUT
    // Butterworth filter, 12 db/octave
    modchorus_filter[instance_id]->setLowpass(0, MOD_FILTER_CUTOFF_HZ, 0.707);
#elif MOD_FILTER_OUTPUT == MOD_LINKWITZ_RILEY_FILTER_OUTPUT
    // Linkwitz-Riley filter, 48 dB/octave
    modchorus_filter[instance_id]->setLowpass(0, MOD_FILTER_CUTOFF_HZ, 0.54);
    modchorus_filter[instance_id]->setLowpass(1, MOD_FILTER_CUTOFF_HZ, 1.3);
    modchorus_filter[instance_id]->setLowpass(2, MOD_FILTER_CUTOFF_HZ, 0.54);
    modchorus_filter[instance_id]->setLowpass(3, MOD_FILTER_CUTOFF_HZ, 1.3);
#endif
    chorus_mixer[instance_id]->gain(0, 1.0);
    chorus_mixer[instance_id]->gain(1, mapfloat(configuration.fx.chorus_level[instance_id], CHORUS_LEVEL_MIN, CHORUS_LEVEL_MAX, 0.0, 0.5));

    // DELAY
    delay_mixer[instance_id]->gain(0, 1.0);
    delay_mixer[instance_id]->gain(1, midi_volume_transform(map(configuration.fx.delay_level[instance_id], DELAY_LEVEL_MIN, DELAY_LEVEL_MAX, 0, 127)));
    delay_fb_mixer[instance_id]->gain(0, 1.0);
    delay_fb_mixer[instance_id]->gain(1, midi_volume_transform(map(configuration.fx.delay_feedback[instance_id], DELAY_FEEDBACK_MIN, DELAY_FEEDBACK_MAX, 0, 127)));
    if (configuration.fx.delay_level[selected_instance_id] <= DELAY_LEVEL_MIN)
      delay_fx[instance_id]->disable(0);
    else if (configuration.fx.delay_sync[instance_id] == 0)
      delay_fx[instance_id]->delay(0, constrain(configuration.fx.delay_time[instance_id], DELAY_TIME_MIN, DELAY_TIME_MAX) * 10);
    if (configuration.fx.delay_sync[instance_id] > 0)
    {
      uint16_t midi_sync_delay_time = uint16_t(60000.0 *  midi_ticks_factor[configuration.fx.delay_sync[instance_id]] / seq.bpm);
      delay_fx[instance_id]->delay(0, constrain(midi_sync_delay_time, DELAY_TIME_MIN, DELAY_TIME_MAX * 10));
    }

    // REVERB SEND
    reverb_mixer_r.gain(instance_id, volume_transform(mapfloat(configuration.fx.reverb_send[instance_id], REVERB_SEND_MIN, REVERB_SEND_MAX, 0.0, VOL_MAX_FLOAT)));
    reverb_mixer_l.gain(instance_id, volume_transform(mapfloat(configuration.fx.reverb_send[instance_id], REVERB_SEND_MIN, REVERB_SEND_MAX, 0.0, VOL_MAX_FLOAT)));

    // DEXED FILTER
    MicroDexed[instance_id]->setFilterResonance(mapfloat(configuration.fx.filter_resonance[instance_id], FILTER_RESONANCE_MIN, FILTER_RESONANCE_MAX, 1.0, 0.0));
    MicroDexed[instance_id]->setFilterCutoff(mapfloat(configuration.fx.filter_cutoff[instance_id], FILTER_CUTOFF_MIN, FILTER_CUTOFF_MAX, 1.0, 0.0));
    MicroDexed[instance_id]->doRefreshVoice();
  }

  // REVERB
  reverb.size(mapfloat(configuration.fx.reverb_roomsize, REVERB_ROOMSIZE_MIN, REVERB_ROOMSIZE_MAX, 0.0, 1.0));
  reverb.lowpass(mapfloat(configuration.fx.reverb_lowpass, REVERB_LOWPASS_MIN, REVERB_LOWPASS_MAX, 0.0, 1.0));
  reverb.lodamp(mapfloat(configuration.fx.reverb_lodamp, REVERB_LODAMP_MIN, REVERB_LODAMP_MAX, 0.0, 1.0));
  reverb.hidamp(mapfloat(configuration.fx.reverb_hidamp, REVERB_HIDAMP_MIN, REVERB_HIDAMP_MAX, 0.0, 1.0));
  reverb.diffusion(mapfloat(configuration.fx.reverb_diffusion, REVERB_DIFFUSION_MIN, REVERB_DIFFUSION_MAX, 0.0, 1.0));

#if NUM_DRUMS > 0
#ifdef USE_FX
  reverb_mixer_r.gain(REVERB_MIX_CH_DRUMS, 1.0); // Drums Reverb-Send
  reverb_mixer_l.gain(REVERB_MIX_CH_DRUMS, 1.0); // Drums Reverb-Send
#endif
#endif

#if defined(USE_EPIANO)
  reverb_mixer_r.gain(REVERB_MIX_CH_EPIANO, mapfloat(configuration.fx.ep_reverb_send, EP_REVERB_SEND_MIN, EP_REVERB_SEND_MAX, 0.0, 1.0)); // EPiano Reverb-Send
  reverb_mixer_l.gain(REVERB_MIX_CH_EPIANO, mapfloat(configuration.fx.ep_reverb_send, EP_REVERB_SEND_MIN, EP_REVERB_SEND_MAX, 0.0, 1.0)); // EPiano Reverb-Send

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
#endif

  master_mixer_r.gain(MASTER_MIX_CH_REVERB, volume_transform(mapfloat(configuration.fx.reverb_level, REVERB_LEVEL_MIN, REVERB_LEVEL_MAX, 0.0, VOL_MAX_FLOAT)));
  master_mixer_l.gain(MASTER_MIX_CH_REVERB, volume_transform(mapfloat(configuration.fx.reverb_level, REVERB_LEVEL_MIN, REVERB_LEVEL_MAX, 0.0, VOL_MAX_FLOAT)));

#endif
#ifdef SGTL5000_AUDIO_ENHANCE
  sgtl5000.setEQFc(1, float(configuration.fx.eq_1));
  sgtl5000.setEQGain(2, mapfloat(configuration.fx.eq_2, EQ_2_MIN, EQ_2_MAX, -9.9, 9.9));
  sgtl5000.setEQGain(3, mapfloat(configuration.fx.eq_3, EQ_3_MIN, EQ_3_MAX, -9.9, 9.9));
  sgtl5000.setEQGain(4, mapfloat(configuration.fx.eq_4, EQ_4_MIN, EQ_4_MAX, -9.9, 9.9));
  sgtl5000.setEQGain(5, mapfloat(configuration.fx.eq_5, EQ_5_MIN, EQ_5_MAX, -9.9, 9.9));
  sgtl5000.setEQGain(6, mapfloat(configuration.fx.eq_6, EQ_6_MIN, EQ_6_MAX, -9.9, 9.9));
  sgtl5000.setEQFc(7, float(configuration.fx.eq_7));
  for (uint8_t band = 1; band <= 7; band++)
  {
    sgtl5000.commitFilter(band);
#ifdef DEBUG
    sgtl5000.show_params(band);
#endif
  }
#endif

  init_MIDI_send_CC();
}

void set_voiceconfig_params(uint8_t instance_id)
{
  // INIT PEAK MIXER
  //microdexed_peak_mixer.gain(instance_id, 1.0);

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

  // Dexed output level
  MicroDexed[instance_id]->setGain(midi_volume_transform(map(configuration.dexed[instance_id].sound_intensity, SOUND_INTENSITY_MIN, SOUND_INTENSITY_MAX, 0, 127)));

  // PANORAMA
  mono2stereo[instance_id]->panorama(mapfloat(configuration.dexed[instance_id].pan, PANORAMA_MIN, PANORAMA_MAX, -1.0, 1.0));
}

void set_epiano_params(void)
{
#if defined(USE_EPIANO)
#ifdef DEBUG
  Serial.print(F("Setting EPiano parameters... "));
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
  ep.setVolume(mapfloat(configuration.epiano.sound_intensity, EP_SOUND_INTENSITY_MIN, EP_SOUND_INTENSITY_MAX, 0, 1.0));
#ifdef DEBUG
  Serial.println(F("done."));
#endif
#else
  ;
#endif
}

void set_sys_params(void)
{
  // set initial volume
  set_volume(configuration.sys.vol, configuration.sys.mono);
}

/******************************************************************************
  HELPERS
******************************************************************************/

// https://www.reddit.com/r/Teensy/comments/7r19uk/reset_and_reboot_teensy_lc_via_code/
#define SCB_AIRCR (*(volatile uint32_t *)0xE000ED0C) // Application Interrupt and Reset Control location
void _softRestart(void)
{
  Serial.end();  //clears the serial monitor  if used
  SCB_AIRCR = 0x05FA0004;  //write value for restart
}

/*float pseudo_log_curve(float value)
  {
  //return (mapfloat(_pseudo_log * arm_sin_f32(value), 0.0, _pseudo_log * arm_sin_f32(1.0), 0.0, 1.0));
  //return (1 - sqrt(1 - value * value));
  //return (pow(2, value) - 1);
  return (pow(value, 2.2));
  }*/

float midi_volume_transform(uint8_t midi_amp)
{
#ifdef DEBUG
  Serial.print(F("midi_amp = "));
  Serial.print(midi_amp, DEC);
  Serial.print(F(" transformed_midi_amp = "));
  Serial.println(powf(midi_amp / 127.0, 4), 3);
#endif
  return powf(midi_amp / 127.0, 4);
}

float volume_transform(float amp)
{
  return powf(amp, 4);
}

uint32_t crc32(byte * calc_start, uint16_t calc_bytes) // base code from https://www.arduino.cc/en/Tutorial/EEPROMCrc
{
  const uint32_t crc_table[16] =
  {
    0x00000000, 0x1db71064, 0x3b6e20c8, 0x26d930ac,
    0x76dc4190, 0x6b6b51f4, 0x4db26158, 0x5005713c,
    0xedb88320, 0xf00f9344, 0xd6d6a3e8, 0xcb61b38c,
    0x9b64c2b0, 0x86d3d2d4, 0xa00ae278, 0xbdbdf21c
  };
  uint32_t crc = ~0L;

  for (byte* index = calc_start ; index < (calc_start + calc_bytes) ; ++index)
  {
    crc = crc_table[(crc ^ *index) & 0x0f] ^ (crc >> 4);
    crc = crc_table[(crc ^ (*index >> 4)) & 0x0f] ^ (crc >> 4);
    crc = ~crc;
  }

  return (crc);
}

void generate_version_string(char* buffer, uint8_t len)
{
  char tmp[3];

  memset(buffer, 0, len);
  strncat(buffer, VERSION, len);
#if defined(TEENSY4_0)
  strncat(buffer, " - 4.0", 6);
#elif defined(TEENSY4_1)
  strncat(buffer, " - 4.1", 6);
#endif
#if defined(USE_FX)
  strncat(buffer, " - FX", 5);
#endif
#if defined(MAX_NOTES)
  strncat(buffer, " - MAX ", 7);
  itoa (MAX_NOTES, tmp, 10);
  strncat(buffer, tmp, 2);
#endif
}

uint8_t check_sd_cards(void)
{
  uint8_t ret = 0;

  memset(sd_string, 0, sizeof(sd_string));

  for (uint8_t i = 0; i < sizeof(cs_pins); i++)
  {
#ifdef DEBUG
    Serial.print(F("Checking CS pin "));
    Serial.print(cs_pins[i], DEC);
    Serial.println(F(" for SD card"));
#endif
    SPI.setMOSI(mosi_pins[i]);
    SPI.setSCK(sck_pins[i]);

    if (SD.begin(cs_pins[i]) == true)
    {
#ifdef DEBUG
      Serial.print(F("Found. Using pin "));
      Serial.println(cs_pins[i], DEC);
#endif
      ret = cs_pins[i];
      break;
    }
  }

  if (ret > 0)
  {
    if (!card.init(SPI_HALF_SPEED, ret))
    {
#ifdef DEBUG
      Serial.println(F("SD card initialization failed."));
#endif
      ret = 0;
    }
  }

  if (ret > 0)
  {
#ifdef DEBUG
    Serial.print(F("Card type : "));
#endif
    switch (card.type()) {
      case SD_CARD_TYPE_SD1:
        sprintf(sd_string, "%-5s", "SD1");
#ifdef DEBUG
        Serial.println(F("SD1"));
#endif
        break;
      case SD_CARD_TYPE_SD2:
        sprintf(sd_string, "%-5s", "SD2");
#ifdef DEBUG
        Serial.println(F("SD2"));
#endif
        break;
      case SD_CARD_TYPE_SDHC:
        sprintf(sd_string, "%-5s", "SD2");
#ifdef DEBUG
        Serial.println(F("SDHC"));
#endif
        break;
      default:
        sprintf(sd_string, "%-5s", "UKNW");
#ifdef DEBUG
        Serial.println(F("Unknown"));
#endif
    }

    if (!volume.init(card))
    {
#ifdef DEBUG
      Serial.println(F("Could not find FAT16 / FAT32 partition."));
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
    Serial.print(F("Volume type is FAT"));
    Serial.println(volume.fatType(), DEC);
    Serial.print(F("Volume size (GB) : "));
    Serial.println(volumesize);
#endif

    sprintf(sd_string + 5, "FAT %2d %02dGB", volume.fatType(), int(volumesize));
  }

#ifdef DEBUG
  Serial.println(sd_string);
#endif

  return ret;
}

void check_and_create_directories(void)
{
  if (sd_card > 0)
  {
    uint8_t i;
    char tmp[FILENAME_LEN];

#ifdef DEBUG
    Serial.println(F("Directory check... "));
#endif
    // create directories for banks
    for (i = 0; i < MAX_BANKS; i++)
    {
      sprintf(tmp, "/%d", i);
      if (!SD.exists(tmp))
      {
#ifdef DEBUG
        Serial.print(F("Creating directory "));
        Serial.println(tmp);
#endif
        SD.mkdir(tmp);
      }
    }

    sprintf(tmp, "/%s", PERFORMANCE_CONFIG_PATH);
    if (!SD.exists(tmp))
    {
#ifdef DEBUG
      Serial.print(F("Creating directory "));
      Serial.println(tmp);
#endif
      SD.mkdir(tmp);
    }

    /*
      // create directories for configuration files
      sprintf(tmp, "/%s", VOICE_CONFIG_PATH);
      if (!SD.exists(tmp))
      {
      #ifdef DEBUG
      Serial.print(F("Creating directory "));
      Serial.println(tmp);
      #endif
      SD.mkdir(tmp);
      }
      sprintf(tmp, "/%s", PERFORMANCE_CONFIG_PATH);
      if (!SD.exists(tmp))
      {
      #ifdef DEBUG
      Serial.print(F("Creating directory "));
      Serial.println(tmp);
      #endif
      SD.mkdir(tmp);
      }
      sprintf(tmp, "/%s", FX_CONFIG_PATH);
      if (!SD.exists(tmp))
      {
      #ifdef DEBUG
      Serial.print(F("Creating directory "));
      Serial.println(tmp);
      #endif
      SD.mkdir(tmp);
      }
      sprintf(tmp, "/%s", DRUM_CONFIG_PATH);
      if (!SD.exists(tmp))
      {
      #ifdef DEBUG
      Serial.print(F("Creating directory "));
      Serial.println(tmp);
      #endif
      SD.mkdir(tmp);
      }
      sprintf(tmp, "/%s", FAV_CONFIG_PATH);
      if (!SD.exists(tmp))
      {
      #ifdef DEBUG
      Serial.print(F("Creating directory "));
      Serial.println(tmp);
      #endif
      SD.mkdir(tmp);
      }
    */



  }
#ifdef DEBUG
  Serial.println(F("SD card check end"));
#endif
}

/******************************************************************************
  DEBUG HELPER
******************************************************************************/
#if defined (DEBUG) && defined (SHOW_CPU_LOAD_MSEC)
void show_cpu_and_mem_usage(void)
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
#ifdef DEBUG
    Serial.print(F("*"));
#endif
  }
#ifdef DEBUG
  else
    Serial.print(F(" "));
  Serial.print(F("CPU : "));
  Serial.print(AudioProcessorUsage(), 2);
  Serial.print(F(" % | CPUMAX : "));
  Serial.print(AudioProcessorUsageMax(), 2);
  Serial.print(F(" % | CPUMAXCNT : "));
  Serial.print(cpumax, DEC);
#ifdef TEENSY4
  Serial.print(F(" | CPUTEMP : "));
  Serial.print(tempmonGetTemp(), 2);
  Serial.print(F("C | MEM : "));
#else
  Serial.print(F(" | MEM : "));
#endif
  Serial.print(AudioMemoryUsage(), DEC);
  Serial.print(F(" | MEMMAX : "));
  Serial.print(AudioMemoryUsageMax(), DEC);
  Serial.print(F(" | AUDIO_MEM_MAX : "));
  Serial.print(AUDIO_MEM, DEC);
  Serial.print(F(" | RENDERTIMEMAX : "));
  Serial.print(sum_render_time_max, DEC);
  Serial.print(F(" | XRUN : "));
  Serial.print(sum_xrun, DEC);
  //  Serial.print(F(" | PEAKR : "));
  //  Serial.print(peak_r, DEC);
  //  Serial.print(F(" | PEAKL : "));
  //  Serial.print(peak_l, DEC);
  //  Serial.print(F(" | PEAKMD : "));
  //  Serial.print(peak_dexed, DEC);
  //  Serial.print(F(" | ACTPEAKMD : "));
  //  Serial.print(peak_dexed_value, 1);
  Serial.print(F(" | BLOCKSIZE : "));
  Serial.print(AUDIO_BLOCK_SAMPLES, DEC);
  Serial.print(F(" | RAM : "));
  Serial.print(FreeMem(), DEC);

  Serial.print(F(" | ACTVOICES : "));
  for (uint8_t instance_id = 0; instance_id < NUM_DEXED; instance_id++)
  {
    Serial.print(instance_id, DEC);
    Serial.print(F(" = "));
    Serial.print(active_voices[instance_id], DEC);
    Serial.print(F(" / "));
    Serial.print(MAX_NOTES / NUM_DEXED, DEC);
    if (instance_id != NUM_DEXED - 1)
      Serial.print(F(", "));
  }
  Serial.println();
#endif
  AudioProcessorUsageMaxReset();
  AudioMemoryUsageMaxReset();
}
#endif

#ifdef DEBUG
void show_configuration(void)
{
  Serial.println();
  Serial.println(F("CONFIGURATION : "));
  Serial.println(F("System"));
  Serial.print(F("  Volume              ")); Serial.println(configuration.sys.vol, DEC);
  Serial.print(F("  Mono                ")); Serial.println(configuration.sys.mono, DEC);
  Serial.print(F("  Soft MIDI Thru      ")); Serial.println(configuration.sys.soft_midi_thru, DEC);
  Serial.print(F("  Favorites           ")); Serial.println(configuration.sys.favorites, DEC);
  Serial.print(F("  Performance Number  ")); Serial.println(configuration.sys.performance_number, DEC);
  Serial.print(F("  Load at startup     ")); Serial.println(configuration.sys.load_at_startup_performance, DEC);
  Serial.println(F("FX"));
  Serial.print(F("  Reverb Roomsize     ")); Serial.println(configuration.fx.reverb_roomsize, DEC);
  Serial.print(F("  Reverb Level        ")); Serial.println(configuration.fx.reverb_level, DEC);

  Serial.print(F("  Reverb Lowpass      ")); Serial.println(configuration.fx.reverb_lowpass, DEC);
  Serial.print(F("  Reverb Lodamp       ")); Serial.println(configuration.fx.reverb_lodamp, DEC);
  Serial.print(F("  Reverb Hidamp       ")); Serial.println(configuration.fx.reverb_hidamp, DEC);
  Serial.print(F("  Reverb Diffusion     ")); Serial.println(configuration.fx.reverb_diffusion, DEC);

  for (uint8_t instance_id = 0; instance_id < NUM_DEXED; instance_id++)
  {
    Serial.print(F("Dexed instance "));
    Serial.println(instance_id, DEC);
    Serial.print(F("  Bank                 ")); Serial.println(configuration.dexed[instance_id].bank, DEC);
    Serial.print(F("  Voice                ")); Serial.println(configuration.dexed[instance_id].voice, DEC);
    Serial.print(F("  MIDI - Channel         ")); Serial.println(configuration.dexed[instance_id].midi_channel, DEC);
    Serial.print(F("  Lowest Note          ")); Serial.println(configuration.dexed[instance_id].lowest_note, DEC);
    Serial.print(F("  Highest Note         ")); Serial.println(configuration.dexed[instance_id].highest_note, DEC);
    Serial.print(F("  Filter Cutoff        ")); Serial.println(configuration.fx.filter_cutoff[instance_id], DEC);
    Serial.print(F("  Filter Resonance     ")); Serial.println(configuration.fx.filter_resonance[instance_id], DEC);
    Serial.print(F("  Chorus Frequency     ")); Serial.println(configuration.fx.chorus_frequency[instance_id], DEC);
    Serial.print(F("  Chorus Waveform      ")); Serial.println(configuration.fx.chorus_waveform[instance_id], DEC);
    Serial.print(F("  Chorus Depth         ")); Serial.println(configuration.fx.chorus_depth[instance_id], DEC);
    Serial.print(F("  Chorus Level         ")); Serial.println(configuration.fx.chorus_level[instance_id], DEC);
    Serial.print(F("  Delay Time           ")); Serial.println(configuration.fx.delay_time[instance_id], DEC);
    Serial.print(F("  Delay Feedback       ")); Serial.println(configuration.fx.delay_feedback[instance_id], DEC);
    Serial.print(F("  Delay Level          ")); Serial.println(configuration.fx.delay_level[instance_id], DEC);
    Serial.print(F("  Delay Sync           ")); Serial.println(configuration.fx.delay_sync[instance_id], DEC);
    Serial.print(F("  Reverb Send          ")); Serial.println(configuration.fx.reverb_send[instance_id], DEC);
    Serial.print(F("  Sound Intensity      ")); Serial.println(configuration.dexed[instance_id].sound_intensity, DEC);
    Serial.print(F("  Panorama             ")); Serial.println(configuration.dexed[instance_id].pan, DEC);
    Serial.print(F("  Transpose            ")); Serial.println(configuration.dexed[instance_id].transpose, DEC);
    Serial.print(F("  Tune                 ")); Serial.println(configuration.dexed[instance_id].tune, DEC);
    Serial.print(F("  Polyphony            ")); Serial.println(configuration.dexed[instance_id].polyphony, DEC);
    Serial.print(F("  Mono / Poly            ")); Serial.println(configuration.dexed[instance_id].monopoly, DEC);
    Serial.print(F("  Note Refresh         ")); Serial.println(configuration.dexed[instance_id].note_refresh, DEC);
    Serial.print(F("  Pitchbend Range      ")); Serial.println(configuration.dexed[instance_id].pb_range, DEC);
    Serial.print(F("  Pitchbend Step       ")); Serial.println(configuration.dexed[instance_id].pb_step, DEC);
    Serial.print(F("  Modwheel Range       ")); Serial.println(configuration.dexed[instance_id].mw_range, DEC);
    Serial.print(F("  Modwheel Assign      ")); Serial.println(configuration.dexed[instance_id].mw_assign, DEC);
    Serial.print(F("  Modwheel Mode        ")); Serial.println(configuration.dexed[instance_id].mw_mode, DEC);
    Serial.print(F("  Footctrl Range       ")); Serial.println(configuration.dexed[instance_id].fc_range, DEC);
    Serial.print(F("  Footctrl Assign      ")); Serial.println(configuration.dexed[instance_id].fc_assign, DEC);
    Serial.print(F("  Footctrl Mode        ")); Serial.println(configuration.dexed[instance_id].fc_mode, DEC);
    Serial.print(F("  BreathCtrl Range     ")); Serial.println(configuration.dexed[instance_id].bc_range, DEC);
    Serial.print(F("  Breathctrl Assign    ")); Serial.println(configuration.dexed[instance_id].bc_assign, DEC);
    Serial.print(F("  Breathctrl Mode      ")); Serial.println(configuration.dexed[instance_id].bc_mode, DEC);
    Serial.print(F("  Aftertouch Range     ")); Serial.println(configuration.dexed[instance_id].at_range, DEC);
    Serial.print(F("  Aftertouch Assign    ")); Serial.println(configuration.dexed[instance_id].at_assign, DEC);
    Serial.print(F("  Aftertouch Mode      ")); Serial.println(configuration.dexed[instance_id].at_mode, DEC);
    Serial.print(F("  Portamento Mode      ")); Serial.println(configuration.dexed[instance_id].portamento_mode, DEC);
    Serial.print(F("  Portamento Glissando ")); Serial.println(configuration.dexed[instance_id].portamento_glissando, DEC);
    Serial.print(F("  Portamento Time      ")); Serial.println(configuration.dexed[instance_id].portamento_time, DEC);
    Serial.print(F("  OP Enabled           ")); Serial.println(configuration.dexed[instance_id].op_enabled, DEC);
    Serial.flush();
  }

  Serial.println();
  Serial.flush();
}

void show_patch(uint8_t instance_id)
{
  char vn[VOICE_NAME_LEN];

  Serial.print(F("INSTANCE "));
  Serial.println(instance_id, DEC);

  memset(vn, 0, sizeof(vn));
  Serial.println(F(" += == == == == == == == == == == == == == == == == == == == == == == == == == == == == == == == == == == == == == == == == == == == == == == == == == == == == = +"));
  for (int8_t i = 5; i >= 0; --i)
  {
    Serial.println(F(" += == == == == == == == == == == == == == == == == == == == == == == == == == == == == == == == == == == == == == == == == == == == == == == == == == == == == = +"));
    Serial.print(F(" | OP"));
    Serial.print(6 - i, DEC);
    Serial.println(F("                                                                                                      | "));
    Serial.println(F(" += == == = += == == = += == == = += == == = += == == = += == == = += == == = += == == = += == == == == == == == = += == == == == == == == = += == == == == == == == = +"));
    Serial.println(F(" |  R1  |  R2  |  R3  |  R4  |  L1  |  L2  |  L3  |  L4  | LEV_SCL_BRK_PT | SCL_LEFT_DEPTH | SCL_RGHT_DEPTH | "));
    Serial.println(F(" + ------ +------ +------ +------ +------ +------ +------ +------ +---------------- +---------------- +---------------- +"));
    Serial.print(" | ");
    SerialPrintFormatInt3(MicroDexed[instance_id]->getVoiceDataElement((i * 21) + DEXED_OP_EG_R1));
    Serial.print(F("  | "));
    SerialPrintFormatInt3(MicroDexed[instance_id]->getVoiceDataElement((i * 21) + DEXED_OP_EG_R2));
    Serial.print(F("  | "));
    SerialPrintFormatInt3(MicroDexed[instance_id]->getVoiceDataElement((i * 21) + DEXED_OP_EG_R3));
    Serial.print(F("  | "));
    SerialPrintFormatInt3(MicroDexed[instance_id]->getVoiceDataElement((i * 21) + DEXED_OP_EG_R4));
    Serial.print(F("  | "));
    SerialPrintFormatInt3(MicroDexed[instance_id]->getVoiceDataElement((i * 21) + DEXED_OP_EG_L1));
    Serial.print(F("  | "));
    SerialPrintFormatInt3(MicroDexed[instance_id]->getVoiceDataElement((i * 21) + DEXED_OP_EG_L2));
    Serial.print(F("  | "));
    SerialPrintFormatInt3(MicroDexed[instance_id]->getVoiceDataElement((i * 21) + DEXED_OP_EG_L3));
    Serial.print(F("  | "));
    SerialPrintFormatInt3(MicroDexed[instance_id]->getVoiceDataElement((i * 21) + DEXED_OP_EG_L4));
    Serial.print(F("  |           "));
    SerialPrintFormatInt3(MicroDexed[instance_id]->getVoiceDataElement((i * 21) + DEXED_OP_LEV_SCL_BRK_PT));
    Serial.print(F("  |           "));
    SerialPrintFormatInt3(MicroDexed[instance_id]->getVoiceDataElement((i * 21) + DEXED_OP_SCL_LEFT_DEPTH));
    Serial.print(F("  |           "));
    SerialPrintFormatInt3(MicroDexed[instance_id]->getVoiceDataElement((i * 21) + DEXED_OP_SCL_RGHT_DEPTH));
    Serial.println(F("  | "));
    Serial.println(F(" += == == = += == == = += == == = += == == = += == == = += == += = += = += == += == == = += == = += == == == = += = += == = += == == == += == += == == == == == == == = +"));
    Serial.println(F(" | SCL_L_CURVE | SCL_R_CURVE | RT_SCALE | AMS | KVS | OUT_LEV | OP_MOD | FRQ_C | FRQ_F | DETUNE             | "));
    Serial.println(F(" + ------------ - +------------ - +---------- +---- - +---- - +-------- - +-------- +------ - +------ - +-------------------- +"));
    Serial.print(F(" |        "));
    SerialPrintFormatInt3(MicroDexed[instance_id]->getVoiceDataElement((i * 21) + DEXED_OP_SCL_LEFT_CURVE));
    Serial.print(F("  |        "));
    SerialPrintFormatInt3(MicroDexed[instance_id]->getVoiceDataElement((i * 21) + DEXED_OP_SCL_RGHT_CURVE));
    Serial.print(F("  |     "));
    SerialPrintFormatInt3(MicroDexed[instance_id]->getVoiceDataElement((i * 21) + DEXED_OP_OSC_RATE_SCALE));
    Serial.print(F("  | "));
    SerialPrintFormatInt3(MicroDexed[instance_id]->getVoiceDataElement((i * 21) + DEXED_OP_AMP_MOD_SENS));
    Serial.print(F("  | "));
    SerialPrintFormatInt3(MicroDexed[instance_id]->getVoiceDataElement((i * 21) + DEXED_OP_KEY_VEL_SENS));
    Serial.print(F("  |    "));
    SerialPrintFormatInt3(MicroDexed[instance_id]->getVoiceDataElement((i * 21) + DEXED_OP_OUTPUT_LEV));
    Serial.print(F("  |   "));
    SerialPrintFormatInt3(MicroDexed[instance_id]->getVoiceDataElement((i * 21) + DEXED_OP_OSC_MODE));
    Serial.print(F("  |  "));
    SerialPrintFormatInt3(MicroDexed[instance_id]->getVoiceDataElement((i * 21) + DEXED_OP_FREQ_COARSE));
    Serial.print(F("  |  "));
    SerialPrintFormatInt3(MicroDexed[instance_id]->getVoiceDataElement((i * 21) + DEXED_OP_FREQ_FINE));
    Serial.print(F("  |               "));
    SerialPrintFormatInt3(MicroDexed[instance_id]->getVoiceDataElement((i * 21) + DEXED_OP_OSC_DETUNE));
    Serial.println(F("  | "));
  }
  Serial.println(F(" += == == == += == == += += == == == += == += == += == == = ++ == == += = += = += == = += == = += = += == == = += == == = += == == += += == == == == == == == == == = +"));
  Serial.println(F(" |  PR1  |  PR2  |  PR3  |  PR4  |  PL1  |  PL2  |  PL3  |  PL4  | ALG  |  FB  | OKS | TRANSPOSE            | "));
  Serial.println(F(" + ------ - +------ - +------ - +------ - +------ - +------ - +------ - +------ - +------ +------ +---- - +---------------------- +"));
  Serial.print(F(" |  "));
  for (int8_t i = 0; i < 8; i++)
  {
    SerialPrintFormatInt3(MicroDexed[instance_id]->getVoiceDataElement(DEXED_VOICE_OFFSET + i));
    Serial.print(F("  |  "));
  }
  SerialPrintFormatInt3(MicroDexed[instance_id]->getVoiceDataElement(DEXED_VOICE_OFFSET + DEXED_ALGORITHM));
  Serial.print(F(" | "));
  SerialPrintFormatInt3(MicroDexed[instance_id]->getVoiceDataElement(DEXED_VOICE_OFFSET + DEXED_FEEDBACK));
  Serial.print(F("  | "));
  SerialPrintFormatInt3(MicroDexed[instance_id]->getVoiceDataElement(DEXED_VOICE_OFFSET + DEXED_OSC_KEY_SYNC));
  Serial.print(F("  |                 "));
  SerialPrintFormatInt3(MicroDexed[instance_id]->getVoiceDataElement(DEXED_VOICE_OFFSET + DEXED_TRANSPOSE));
  Serial.println(F("  | "));
  Serial.println(F(" += == == == += += == == += == += == += == == += += == == == += == == == += = += == = += == == += += == == = ++ == == = += == == += == == == == == == == == == == = +"));
  Serial.println(F(" | LFO SPD | LFO DLY | LFO PMD | LFO AMD | LFO SYNC | LFO WAVE | LFO PMS | NAME                             | "));
  Serial.println(F(" + -------- - +-------- - +-------- - +-------- - +---------- +---------- +-------- - +---------------------------------- +"));
  Serial.print(F(" |    "));
  SerialPrintFormatInt3(MicroDexed[instance_id]->getVoiceDataElement(DEXED_VOICE_OFFSET + DEXED_LFO_SPEED));
  Serial.print(F("  |    "));
  SerialPrintFormatInt3(MicroDexed[instance_id]->getVoiceDataElement(DEXED_VOICE_OFFSET + DEXED_LFO_DELAY));
  Serial.print(F("  |    "));
  SerialPrintFormatInt3(MicroDexed[instance_id]->getVoiceDataElement(DEXED_VOICE_OFFSET + DEXED_LFO_PITCH_MOD_DEP));
  Serial.print(F("  |    "));
  SerialPrintFormatInt3(MicroDexed[instance_id]->getVoiceDataElement(DEXED_VOICE_OFFSET + DEXED_LFO_AMP_MOD_DEP));
  Serial.print(F("  |     "));
  SerialPrintFormatInt3(MicroDexed[instance_id]->getVoiceDataElement(DEXED_VOICE_OFFSET + DEXED_LFO_SYNC));
  Serial.print(F("  |     "));
  SerialPrintFormatInt3(MicroDexed[instance_id]->getVoiceDataElement(DEXED_VOICE_OFFSET + DEXED_LFO_WAVE));
  Serial.print(F("  |    "));
  SerialPrintFormatInt3(MicroDexed[instance_id]->getVoiceDataElement(DEXED_VOICE_OFFSET + DEXED_LFO_PITCH_MOD_SENS));
  Serial.print(F("  | "));
  MicroDexed[instance_id]->getName(vn);
  Serial.print(vn);
  Serial.println(F("                       | "));
  Serial.println(F(" += == == == == += == == == == += == == == == += == == == == += == == == == = += == == == == = += == == == == += == == == == == == == == == == == == == == == == = +"));
  Serial.println(F(" += == == == == == == == == == == == == == == == == == == == == == == == == == == == == == == == == == == == == == == == == == == == == == == == == == == == == = +"));
}

void SerialPrintFormatInt3(uint8_t num)
{
  char buf[4];
  memset(buf, 0, 4);
  sprintf(buf, " % 3d", num);
  Serial.print(buf);
}

#ifdef TEENSY3_6
/* From: https://forum.pjrc.com/threads/33443-How-to-display-free-ram */
extern "C" char* sbrk(int incr);
uint32_t FreeMem(void)
{
  char top;
  return &top - reinterpret_cast<char*>(sbrk(0));
}
#else
/* From: https://forum.pjrc.com/threads/33443-How-to-display-free-ram */
extern unsigned long _heap_end;
extern char *__brkval;
int FreeMem(void)
{
  return (char *)&_heap_end - __brkval;
}
#endif
#endif
