/*
   MicroDexed-touch

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
   along with this programf; if not, write to the Free Software Foundation,
   Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301  USA

*/

#pragma once

#include <Arduino.h>
#include "midinotes.h"
#include "teensy_board_detection.h"

// If you want to test the system with Linux and without any keyboard and/or audio equipment, you can do the following:
// 1. In Arduino-IDE enable "Tools->USB-Type->Serial + MIDI + Audio"
// 2. Build the firmware with "MIDI_DEVICE_USB" enabled in config.h.
// 3. Afterconnecting to a Linux system there should be a MIDI an audio device available that is called "MicroMDexed", so you can start the following:
// $ aplaymidi -p 20:0 <MIDI-File> # e.g. test.mid
// $ vkeybd --addr 20:0
// $ arecord -f cd -Dhw:1,0 /tmp/<AUDIO_FILE_NAME>.wav
//
// Tools for testing MIDI:  https://github.com/gbevin/SendMIDI
//                          https://github.com/gbevin/ReceiveMIDI
//
// e.g.:
// * sendmidi dev "MicroDexed MIDI" on 80 127 && sleep 1.0 && sendmidi dev "MicroDexed MIDI" off 80 0
// * sendmidi dev "MicroDexed MIDI" syf addon/SD/90/RitCh1.syx
// * amidi -p hw:2,0,0 -s addon/SD/90/RitCh1.syx
//
// Receiving and storing MIDI SYSEX with Linux:
// amidi -p hw:2,0,0 -d -r /tmp/bkup1.syx
//
// For SYSEX Bank upload via USB:
// sed -i.orig 's/SYSEX_MAX_LEN = 290/SYSEX_MAX_LEN = 4104/' /usr/local/arduino-teensy/hardware/teensy/avr/libraries/USBHost_t36/USBHost_t36.h
// sed -i.orig 's/^#define USB_MIDI_SYSEX_MAX 290/#define USB_MIDI_SYSEX_MAX 4104/' /usr/local/arduino-teensy/hardware/teensy/avr/cores/teensy3/usb_midi.h
// sed -i.orig 's/^#define USB_MIDI_SYSEX_MAX 290/#define USB_MIDI_SYSEX_MAX 4104/' /usr/local/arduino-teensy/hardware/teensy/avr/cores/teensy4/usb_midi.h
//#define USB_MIDI_SYSEX_MAX 4104

#define VERSION "1.4.6.17"

//*************************************************************************************************
//* DEVICE SETTINGS
//*************************************************************************************************
//#define UI_REVERSE

//*************************************************************************************************
//* MIDI HARDWARE SETTINGS
//*************************************************************************************************
#define MIDI_DEVICE_DIN Serial1
#define MIDI_DEVICE_USB 1
#define MIDI_DEVICE_USB_HOST

//*************************************************************************************************
//* USB HOST KEYPAD/KEYBOARD/GAMEPAD SUPPORT
//*************************************************************************************************

// USB_KEYBOARD is for a PC-Numeric Keypad, connected via USB
// USB_GAMEPAD is for a classic 8bit/16bit style of USB Gamepad.
// ONBOARD_BUTTON_Interface is for directly attached buttons to the teensy. USB_GAMEPAD must be also be enabled for it to work.

// IF YOU WANT TO USE REMOTE CONSOLE ON PC WITH PC KEYBOARD + AUDIO ETC. DON'T CHANGE ANYTHING IN THIS SECTION, 
// JUST ENABLE REMOTE_CONSOLE AND IT WILL TAKE CARE FOR EVERYTHING ELSE

//#define USB_KEYPAD 1
//#define USB_GAMEPAD 1
//#define ONBOARD_BUTTON_INTERFACE 1

//*************************************************************************************************
//* AUDIO HARDWARE SETTINGS
//*************************************************************************************************
// If nothing is defined Teensy internal DAC is used as audio output device!
// Left and right channel audio signal is presented on pins A21 and A22.

// IF YOU WANT TO USE REMOTE CONSOLE ON PC WITH PC KEYBOARD + AUDIO ETC. DON'T CHANGE ANYTHING IN THIS SECTION, 
// JUST ENABLE REMOTE_CONSOLE AND IT WILL TAKE CARE FOR EVERYTHING ELSE

#define I2S_AUDIO_ONLY  // for PCM5102 or other I2S DACs

//#define AUDIO_DEVICE_USB

//#define TEENSY_AUDIO_BOARD // for legacy reasons, only
//#define PT8211_AUDIO
//#define TGA_AUDIO_BOARD

//*************************************************************************************************
//* MIDI SOFTWARE SETTINGS
//*************************************************************************************************

#define DEFAULT_MIDI_CHANNEL MIDI_CHANNEL_OMNI
#define SYSEXBANK_DEFAULT 0
#define SYSEXSOUND_DEFAULT 0

//*************************************************************************************************
//* DEBUG OUTPUT SETTINGS
//*************************************************************************************************
//#define DEBUG 1
//#define DEBUG_SHOW_JSON 1
//#define REMOTE_CONSOLE  //enable USB Display + USB AUDIO + GAMEPAD/Key Input - This is NOT for serial monitor from Teensyduino! For that, please use #define DEBUG 1
#define SERIAL_SPEED 230400
#define SHOW_XRUN 1
#define SHOW_CPU_LOAD_MSEC 5000

//*************************************************************************************************
//* PROGMEM AND FLASH COMPILE MODES
//*************************************************************************************************

#define COMPILE_FOR_PROGMEM  // enable this if you do not have a SPI FLASH chip
//#define COMPILE_FOR_FLASH  // this is the intended configuration, with SPI FLASH chip
//#define COMPILE_FOR_SDCARD  // experimental, for testing purposes only
//#define COMPILE_FOR_QSPI    // experimental, for testing purposes only

//*************************************************************************************************
//* DEXED SEQUENCER, EPIANO AND EFFECTS SETTINGS
//*************************************************************************************************
// Number of Dexed instances
#define NUM_DEXED 2  // 1 or 2 - nothing else!
#define DEFAULT_DEXED_MIDI_CHANNEL_INST0 1
#define DEFAULT_DEXED_MIDI_CHANNEL_INST1 2

// FX-CHAIN ENABLE/DISABLE
#define USE_FX 1

// NUMBER OF PARALLEL SAMPLEDRUMS
#define NUM_DRUMS 8  // set 0 to disable Drums

// DEFAULT MIDI CHANNEL FOR DRUMSAMPLER
#define DRUM_MIDI_CHANNEL 10
#define NUM_CUSTOM_MIDI_MAPPINGS 10  //Number of Custom Key, CC and Button Mappings

// NUMBER OF SAMPLES IN DRUMSET
#define NUM_DRUMSET_CONFIG 71

// SEQUENCER
#define USE_SEQUENCER
#define NUM_SEQ_PATTERN 32
#define NUM_SEQ_TRACKS 8
#define SONG_LENGTH 64
#define NUM_CHAINS 32

// SAMPLER
#define USE_MULTISAMPLES
#define NUM_MULTISAMPLES 2
#define NUM_MULTISAMPLE_ZONES 8
#define DEFAULT_MSP_MIDI_CHANNEL_INST0 7
#define DEFAULT_MSP_MIDI_CHANNEL_INST1 8

// EPIANO
#define USE_EPIANO
#ifdef USE_EPIANO
#define NUM_EPIANO_VOICES 16
#define DEFAULT_EP_MIDI_CHANNEL 3
#endif

// MICROSYNTH
#define USE_MICROSYNTH
#ifdef USE_MICROSYNTH
#define NUM_MICROSYNTH 2
#define DEFAULT_MICROSYNTH_MIDI_CHANNEL_INST0 4
#define DEFAULT_MICROSYNTH_MIDI_CHANNEL_INST1 5
#endif

// BRAIDS
#define USE_BRAIDS  //very early state. not ready for usage
#ifdef USE_BRAIDS
#define NUM_BRAIDS 8
#define DEFAULT_BRAIDS_MIDI_CHANNEL 6
#endif

// MULTIBAND COMPRESSOR
#define USE_MULTIBAND

#define VIRT_KEYB_YPOS 166

// CHORUS parameters
#define MOD_DELAY_SAMPLE_BUFFER int32_t(TIME_MS2SAMPLES(15.0))  // 15.0 ms delay buffer.
#define MOD_WAVEFORM WAVEFORM_TRIANGLE                          // WAVEFORM_SINE WAVEFORM_TRIANGLE WAVEFORM_SAWTOOTH WAVEFORM_SAWTOOTH_REVERSE
#define MOD_FILTER_OUTPUT MOD_BUTTERWORTH_FILTER_OUTPUT         // MOD_LINKWITZ_RILEY_FILTER_OUTPUT MOD_BUTTERWORTH_FILTER_OUTPUT MOD_NO_FILTER_OUTPUT
#define MOD_FILTER_CUTOFF_HZ 2000

// SGTL5000
#ifdef TEENSY_AUDIO_BOARD
#define SGTL5000_AUDIO_ENHANCE 1
//#define SGTL5000_AUDIO_THRU 1
#define SGTL5000_HEADPHONE_VOLUME 0.8
#endif
#if defined(TEENSY4)
#define USE_PLATEREVERB 1
#endif

// DELAYTIME
#define DELAY_MAX_TIME 500

//*************************************************************************************************
//* AUDIO SOFTWARE SETTINGS
//*************************************************************************************************
#define SAMPLE_RATE 44100

#ifdef USE_FX
#if defined(USE_EPIANO)
#define AUDIO_MEM SAMPLE_RATE* NUM_DEXED* DELAY_MAX_TIME / 128000 + 36 + 14
#else
#define AUDIO_MEM SAMPLE_RATE* NUM_DEXED* DELAY_MAX_TIME / 128000 + 36
#endif
#else
#if defined(USE_EPIANO)
#define AUDIO_MEM NUM_DEXED * 15 + 14
#else
#define AUDIO_MEM NUM_DEXED * 15
#endif
#endif

#ifdef TEENSY_AUDIO_BOARD
/*
  13: 3.16 Volts p-p
  14: 2.98 Volts p-p
  15: 2.83 Volts p-p
  16: 2.67 Volts p-p
  17: 2.53 Volts p-p
  18: 2.39 Volts p-p
  19: 2.26 Volts p-p
  20: 2.14 Volts p-p
  21: 2.02 Volts p-p
  22: 1.91 Volts p-p
  23: 1.80 Volts p-p
  24: 1.71 Volts p-p
  25: 1.62 Volts p-p
  26: 1.53 Volts p-p
  27: 1.44 Volts p-p
  28: 1.37 Volts p-p
  29: 1.29 Volts p-p  (default)
  30: 1.22 Volts p-p
  31: 1.16 Volts p-p
*/
#define SGTL5000_LINEOUT_LEVEL 29
#endif

//*************************************************************************************************
//* UI
//*************************************************************************************************

#define CLASSIC_UI
//#define MODERN_UI

// Assign human-readable names to some common 16-bit color values:
// Color definitions

#define RED 0xF000
#define PINK 0xF81F
#define YELLOW 0xFFEB

#define GREEN 0x07E0
#define MIDDLEGREEN 0x1500
#define DARKGREEN 0x0AE0

#define GREY1 0xC638
#define GREY2 0x52AA
#define GREY3 0x2104
#define GREY4 0x10A2
//#define GREY4 0xC638 //only for UI test
#define DX_DARKCYAN 0x03EF

// Display

#define TFT_WIDTH 240
#define TFT_HEIGHT 320

#define DISPLAY_WIDTH 320
#define DISPLAY_HEIGHT 240

#define display_cols 20
#define display_rows 6
#define CHAR_width 12
#define CHAR_height 17
#define CHAR_width_small 6
#define CHAR_height_small 8
#define button_size_x 7
#define button_size_y 4

#define VOICE_SELECTION_MS 60000
#define BACK_FROM_VOLUME_MS 2000
#define MESSAGE_WAIT_TIME 1000
#define MIDI_DECAY_TIMER 50

//*************************************************************************************************
//* HARDWARE SETTINGS
//*************************************************************************************************

// Teensy Audio Shield
#define SDCARD_AUDIO_CS_PIN 10
#define SDCARD_AUDIO_MOSI_PIN 7
#define SDCARD_AUDIO_SCK_PIN 14

#define SDCARD_TEENSY_CS_PIN BUILTIN_SDCARD
#define SDCARD_TEENSY_MOSI_PIN 11
#define SDCARD_TEENSY_SCK_PIN 13

const int FlashChipSelect = 6;  // digital pin for flash chip CS pin (on Audio Shield)

// Encoders with push button
//#define ENCODER_USE_INTERRUPTS
#define NUM_ENCODER 2
#define ENC_L_PIN_A 32
#define ENC_L_PIN_B 31
#define BUT_L_PIN 30

#define ENC_R_PIN_A 28
#define ENC_R_PIN_B 29
#define BUT_R_PIN 25

#define BUT_DEBOUNCE_MS 20
#define LONG_BUTTON_PRESS 500

// Internal timer
#define AUTOSTORE_MS 5000

// EEPROM address
#define EEPROM_START_ADDRESS 0xFF

#define MAX_BANKS 100
#define MAX_VOICES 32      // voices per bank
#define BANK_NAME_LEN 11   // 10 (plus '\0')
#define VOICE_NAME_LEN 12  // 11 (plus '\0')
#define FILENAME_LEN BANK_NAME_LEN + VOICE_NAME_LEN
#define CONFIG_FILENAME_LEN 50
#define DRUM_NAME_LEN 21

#define FAV_CONFIG_PATH "FAVCFG"
#define FAV_CONFIG_NAME "FAVCFG"

#define PERFORMANCE_CONFIG_PATH "PERFORMANCE"
#define SEQUENCER_CONFIG_NAME "sequencer"
#define DRUMS_CONFIG_NAME "drums"
#define DRUMS_MAPPING_NAME "drmmap"
#define PATTERN_CONFIG_NAME "patterns"
#define VELOCITY_CONFIG_NAME "velocity"
#define CHAIN_CONFIG_NAME "chain"
#define TRANSPOSE_CONFIG_NAME "transp"
#define SONG_CONFIG_NAME "song"
#define FX_CONFIG_NAME "fx"
#define VOICE_CONFIG_NAME "voice"
#define SYS_CONFIG_NAME "sys"
#define EPIANO_CONFIG_NAME "epiano"
#define MICROSYNTH_CONFIG_NAME "msynth"
#define MULTISAMPLE_PRESETS_CONFIG_NAME "msp_presets"
#define BRAIDS_CONFIG_NAME "braids"
#define MULTIBAND_CONFIG_NAME "multiband"

#define MAX_PERF_MOD 30

// SerialFlash
#define MAX_FLASH_FILES 255
#define MAX_FLASH_FILENAME_LEN 64

//*************************************************************************************************
//* DO NO CHANGE ANYTHING BEYOND IF YOU DON'T KNOW WHAT YOU ARE DOING !!!
//*************************************************************************************************

//do not touch beyond here:

#ifdef I2S_AUDIO_ONLY
#define PCM5102_MUTE_PIN 34  // hardware pin for PCM5102 XSMT soft mute function
#endif

#ifdef REMOTE_CONSOLE
#define USB_GAMEPAD 1
#define ONBOARD_BUTTON_INTERFACE 1
#define AUDIO_DEVICE_USB
#endif

#ifdef ONBOARD_BUTTON_INTERFACE
#define BI_UP 2
#define BI_DOWN 3
#define BI_LEFT 4
#define BI_RIGHT 5
#define BI_SELECT 9
#define BI_START 10
#define BI_BUTTON_A 16
#define BI_BUTTON_B 17
#endif

#define MAX_DEXED 2  // No! - even don't think about increasing this number! IT _WILL_ PRODUCE MASSIVE PROBLEMS!
#define CONTROL_RATE_MS 50
#define MICROSYNTH_LFO_RATE_MS 20
#define BRAIDS_LFO_RATE_MS 20
#define SAVE_SYS_MS 5000
#define VOL_MAX_FLOAT 0.95

#define EEPROM_MARKER 0x4243

#ifndef NUM_DRUMS
#define NUM_DRUMS 0
#endif

// MAX_NOTES SETTINGS
// Teensy-4.x settings
#ifdef TEENSY4
#define MAX_NOTES 32
#define MIDI_DECAY_LEVEL_TIME 500
#endif

// MIDI
//#ifdef MIDI_DEVICE_USB
//#define USBCON 1  ??
//#endif

// Some optimizations
#define USE_TEENSY_DSP 1

// HELPER MACROS
#define TIME_MS2SAMPLES(x) floor(uint32_t(x) * AUDIO_SAMPLE_RATE / 1000) * sizeof(int16_t)
#define SAMPLES2TIME_MS(x) float(uint32_t(x) * 1000 / AUDIO_SAMPLE_RATE * sizeof(int16_t)
#define XSTR(x) STR(x)
#define STR(x) #x

// Modulated delay options
#define MOD_NO_FILTER_OUTPUT 0
#define MOD_BUTTERWORTH_FILTER_OUTPUT 1
#define MOD_LINKWITZ_RILEY_FILTER_OUTPUT 2

#if defined(TEENSY_DAC_SYMMETRIC)
#define MONO_MIN 1
#define MONO_MAX 1
#define MONO_DEFAULT 1
#else
#define MONO_MIN 0
#define MONO_MAX 3
#define MONO_DEFAULT 0
#endif

#define MIDI_CONTROLLER_MODE_MAX 2

#define VOLUME_MIN 0
#define VOLUME_MAX 100
#define VOLUME_DEFAULT 80
//#define VOLUME_ENC_STEPS 5

#define PANORAMA_MIN 0
#define PANORAMA_MAX 40
#define PANORAMA_DEFAULT 20

#define MIDI_CHANNEL_MIN MIDI_CHANNEL_OMNI
#define MIDI_CHANNEL_MAX 16
#define MIDI_CHANNEL_DEFAULT MIDI_CHANNEL_OMNI

#define INSTANCE_LOWEST_NOTE_MIN 21
#define INSTANCE_LOWEST_NOTE_MAX 108
#define INSTANCE_LOWEST_NOTE_DEFAULT 21

#define INSTANCE_HIGHEST_NOTE_MIN 21
#define INSTANCE_HIGHEST_NOTE_MAX 108
#define INSTANCE_HIGHEST_NOTE_DEFAULT 108

#define CHORUS_FREQUENCY_MIN 0
#define CHORUS_FREQUENCY_MAX 100
#define CHORUS_FREQUENCY_DEFAULT 0

#define CHORUS_WAVEFORM_MIN 0
#define CHORUS_WAVEFORM_MAX 1
#define CHORUS_WAVEFORM_DEFAULT 0

#define CHORUS_DEPTH_MIN 0
#define CHORUS_DEPTH_MAX 100
#define CHORUS_DEPTH_DEFAULT 0

#define CHORUS_LEVEL_MIN 0
#define CHORUS_LEVEL_MAX 100
#define CHORUS_LEVEL_DEFAULT 0

#define DELAY_TIME_MIN 0
#define DELAY_TIME_MAX DELAY_MAX_TIME / 10
#define DELAY_TIME_DEFAULT 0

#define DELAY_FEEDBACK_MIN 0
#define DELAY_FEEDBACK_MAX 100
#define DELAY_FEEDBACK_DEFAULT 0

#define DELAY_LEVEL_MIN 0
#define DELAY_LEVEL_MAX 100
#define DELAY_LEVEL_DEFAULT 0

#define DELAY_SYNC_MIN 0
#define DELAY_SYNC_MAX 9
#define DELAY_SYNC_DEFAULT 0

#define REVERB_ROOMSIZE_MIN 0
#define REVERB_ROOMSIZE_MAX 100
#define REVERB_ROOMSIZE_DEFAULT 0

#define REVERB_DAMPING_MIN 0
#define REVERB_DAMPING_MAX 100
#define REVERB_DAMPING_DEFAULT 0

#define REVERB_LOWPASS_MIN 0
#define REVERB_LOWPASS_MAX 100
#define REVERB_LOWPASS_DEFAULT 0

#define REVERB_LODAMP_MIN 0
#define REVERB_LODAMP_MAX 100
#define REVERB_LODAMP_DEFAULT 0

#define REVERB_HIDAMP_MIN 0
#define REVERB_HIDAMP_MAX 100
#define REVERB_HIDAMP_DEFAULT 0

#define REVERB_DIFFUSION_MIN 0
#define REVERB_DIFFUSION_MAX 100
#define REVERB_DIFFUSION_DEFAULT 0

#define REVERB_LEVEL_MIN 0
#define REVERB_LEVEL_MAX 100
#define REVERB_LEVEL_DEFAULT 0

#define REVERB_SEND_MIN 0
#define REVERB_SEND_MAX 100
#define REVERB_SEND_DEFAULT 0

#define FILTER_CUTOFF_MIN 0
#define FILTER_CUTOFF_MAX 100
#define FILTER_CUTOFF_DEFAULT 0

#define FILTER_RESONANCE_MIN 0
#define FILTER_RESONANCE_MAX 100
#define FILTER_RESONANCE_DEFAULT 0

#define TRANSPOSE_MIN 0
#define TRANSPOSE_MAX 48
#define TRANSPOSE_DEFAULT 12

#define TUNE_MIN 1
#define TUNE_MAX 199
#define TUNE_DEFAULT 100

#define SOUND_INTENSITY_MIN 0
#define SOUND_INTENSITY_MAX 100
#define SOUND_INTENSITY_DEFAULT 100

#define POLYPHONY_MIN 0
#define POLYPHONY_MAX MAX_NOTES
#define POLYPHONY_DEFAULT MAX_NOTES / NUM_DEXED

#define MONOPOLY_MIN 0
#define MONOPOLY_MAX 1
#define MONOPOLY_DEFAULT 0

#define NOTE_REFRESH_MIN 0
#define NOTE_REFRESH_MAX 1
#define NOTE_REFRESH_DEFAULT 0

#define PB_RANGE_MIN 0
#define PB_RANGE_MAX 12
#define PB_RANGE_DEFAULT 1

#define PB_STEP_MIN 0
#define PB_STEP_MAX 12
#define PB_STEP_DEFAULT 0

#define MW_RANGE_MIN 0
#define MW_RANGE_MAX 99
#define MW_RANGE_DEFAULT 50

#define MW_ASSIGN_MIN 0
#define MW_ASSIGN_MAX 7
#define MW_ASSIGN_DEFAULT 0  // Bitmapped: 0: Pitch, 1: Amp, 2: Bias

#define MW_MODE_MIN 0
#define MW_MODE_MAX MIDI_CONTROLLER_MODE_MAX
#define MW_MODE_DEFAULT 0

#define FC_RANGE_MIN 0
#define FC_RANGE_MAX 99
#define FC_RANGE_DEFAULT 50

#define FC_ASSIGN_MIN 0
#define FC_ASSIGN_MAX 7
#define FC_ASSIGN_DEFAULT 0  // Bitmapped: 0: Pitch, 1: Amp, 2: Bias

#define FC_MODE_MIN 0
#define FC_MODE_MAX MIDI_CONTROLLER_MODE_MAX
#define FC_MODE_DEFAULT 0

#define BC_RANGE_MIN 0
#define BC_RANGE_MAX 99
#define BC_RANGE_DEFAULT 50

#define BC_ASSIGN_MIN 0
#define BC_ASSIGN_MAX 7
#define BC_ASSIGN_DEFAULT 0  // Bitmapped: 0: Pitch, 1: Amp, 2: Bias

#define BC_MODE_MIN 0
#define BC_MODE_MAX MIDI_CONTROLLER_MODE_MAX
#define BC_MODE_DEFAULT 0

#define AT_RANGE_MIN 0
#define AT_RANGE_MAX 99
#define AT_RANGE_DEFAULT 50

#define AT_ASSIGN_MIN 0
#define AT_ASSIGN_MAX 7
#define AT_ASSIGN_DEFAULT 0  // Bitmapped: 0: Pitch, 1: Amp, 2: Bias

#define AT_MODE_MIN 0
#define AT_MODE_MAX MIDI_CONTROLLER_MODE_MAX
#define AT_MODE_DEFAULT 0

#define OP_ENABLED_MIN 0
#define OP_ENABLED_MAX 0x3f
#define OP_ENABLED_DEFAULT OP_ENABLED_MAX

#define PORTAMENTO_MODE_MIN 0
#define PORTAMENTO_MODE_MAX 1
#define PORTAMENTO_MODE_DEFAULT 0  // 0: Retain, 1: Follow

#define PORTAMENTO_GLISSANDO_MIN 0
#define PORTAMENTO_GLISSANDO_MAX 1
#define PORTAMENTO_GLISSANDO_DEFAULT 0

#define PORTAMENTO_TIME_MIN 0
#define PORTAMENTO_TIME_MAX 99
#define PORTAMENTO_TIME_DEFAULT 0

#define INSTANCES_MIN 1
#define INSTANCES_MAX NUM_DEXED
#define INSTANCES_DEFAULT 1

#define INSTANCE_NOTE_START_MIN MIDI_AIS0
#define INSTANCE_NOTE_START_MAX MIDI_B7
#define INSTANCE_NOTE_START_DEFAULT INSTANCE_NOTE_START_MIN

#define INSTANCE_NOTE_END_MIN MIDI_AIS0
#define INSTANCE_NOTE_END_MAX MIDI_B7
#define INSTANCE_NOTE_END_DEFAULT INSTANCE_NOTE_END_MAX

#define SOFT_MIDI_THRU_MIN 0
#define SOFT_MIDI_THRU_MAX 1
#define SOFT_MIDI_THRU_DEFAULT 1

#define VELOCITY_LEVEL_MIN 50
#define VELOCITY_LEVEL_MAX 127
#define VELOCITY_LEVEL_DEFAULT 100

#define PERFORMANCE_NUM_MIN 0
#define PERFORMANCE_NUM_MAX 99
#define PERFORMANCE_NUM_DEFAULT 0

#define DISPLAY_ROTATION_MIN 0
#define DISPLAY_ROTATION_MAX 3
#define DISPLAY_ROTATION_DEFAULT 3
#define DISPLAY_ROTATION_INVERTED 1

#define TOUCH_ROTATION_MIN 0
#define TOUCH_ROTATION_MAX 3
#define TOUCH_ROTATION_DEFAULT 1
#define TOUCH_ROTATION_INVERTED 3

#define SCREEN_SAVER_START_MIN 1
#define SCREEN_SAVER_START_MAX 59
#define SCREEN_SAVER_START_DEFAULT 4

#define GAMEPAD_SPEED_MIN 60
#define GAMEPAD_SPEED_MAX 300
#define GAMEPAD_SPEED_DEFAULT 180

#define EQ_1_MIN 15
#define EQ_1_MAX 250
#define EQ_1_DEFAULT 50

#define EQ_2_MIN -99
#define EQ_2_MAX 99
#define EQ_2_DEFAULT 0

#define EQ_3_MIN -99
#define EQ_3_MAX 99
#define EQ_3_DEFAULT 0

#define EQ_4_MIN -99
#define EQ_4_MAX 99
#define EQ_4_DEFAULT 0

#define EQ_5_MIN -99
#define EQ_5_MAX 99
#define EQ_5_DEFAULT 0

#define EQ_6_MIN -99
#define EQ_6_MAX 99
#define EQ_6_DEFAULT 0

#define EQ_7_MIN 8
#define EQ_7_MAX 18
#define EQ_7_DEFAULT 15

#define EP_CHORUS_FREQUENCY_MIN 0
#define EP_CHORUS_FREQUENCY_MAX 100
#define EP_CHORUS_FREQUENCY_DEFAULT 0

#define EP_CHORUS_WAVEFORM_MIN 0
#define EP_CHORUS_WAVEFORM_MAX 1
#define EP_CHORUS_WAVEFORM_DEFAULT 0

#define EP_CHORUS_DEPTH_MIN 0
#define EP_CHORUS_DEPTH_MAX 100
#define EP_CHORUS_DEPTH_DEFAULT 0

#define EP_CHORUS_LEVEL_MIN 0
#define EP_CHORUS_LEVEL_MAX 100
#define EP_CHORUS_LEVEL_DEFAULT 0

#define EP_REVERB_SEND_MIN 0
#define EP_REVERB_SEND_MAX 100
#define EP_REVERB_SEND_DEFAULT 0

#define EP_DECAY_MIN 0
#define EP_DECAY_MAX 100
#define EP_DECAY_DEFAULT 50

#define EP_RELEASE_MIN 0
#define EP_RELEASE_MAX 100
#define EP_RELEASE_DEFAULT 50

#define EP_HARDNESS_MIN 0
#define EP_HARDNESS_MAX 100
#define EP_HARDNESS_DEFAULT 50

#define EP_TREBLE_MIN 0
#define EP_TREBLE_MAX 100
#define EP_TREBLE_DEFAULT 50

#define EP_PAN_TREMOLO_MIN 0
#define EP_PAN_TREMOLO_MAX 100
#define EP_PAN_TREMOLO_DEFAULT 20

#define EP_PAN_LFO_MIN 0
#define EP_PAN_LFO_MAX 100
#define EP_PAN_LFO_DEFAULT 65

#define EP_VELOCITY_SENSE_MIN 0
#define EP_VELOCITY_SENSE_MAX 100
#define EP_VELOCITY_SENSE_DEFAULT 25

#define EP_STEREO_MIN 0
#define EP_STEREO_MAX 100
#define EP_STEREO_DEFAULT 50

#define EP_POLYPHONY_MIN 0
#if defined(USE_EPIANO)
#define EP_POLYPHONY_MAX NUM_EPIANO_VOICES
#define EP_POLYPHONY_DEFAULT NUM_EPIANO_VOICES
#else
#define EP_POLYPHONY_MAX 16
#define EP_POLYPHONY_DEFAULT 16
#endif

#define EP_TUNE_MIN 1
#define EP_TUNE_MAX 199
#define EP_TUNE_DEFAULT 100

#define EP_DETUNE_MIN 0
#define EP_DETUNE_MAX 100
#define EP_DETUNE_DEFAULT 15

#define EP_OVERDRIVE_MIN 0
#define EP_OVERDRIVE_MAX 100
#define EP_OVERDRIVE_DEFAULT 0

#define EP_REVERB_SEND_MIN 0
#define EP_REVERB_SEND_MAX 100
#define EP_REVERB_SEND_DEFAULT 0

#define EP_LOWEST_NOTE_MIN 21
#define EP_LOWEST_NOTE_MAX 108
#define EP_LOWEST_NOTE_DEFAULT EP_LOWEST_NOTE_MIN

#define EP_HIGHEST_NOTE_MIN 21
#define EP_HIGHEST_NOTE_MAX 108
#define EP_HIGHEST_NOTE_DEFAULT EP_HIGHEST_NOTE_MAX

#define EP_TRANSPOSE_MIN 0
#define EP_TRANSPOSE_MAX 48
#define EP_TRANSPOSE_DEFAULT 24

#define EP_SOUND_INTENSITY_MIN 0
#define EP_SOUND_INTENSITY_MAX 100
#define EP_SOUND_INTENSITY_DEFAULT 100

#define EP_MONOPOLY_MIN 0
#define EP_MONOPOLY_MAX 1
#define EP_MONOPOLY_DEFAULT 0

#define EP_MIDI_CHANNEL_MIN MIDI_CHANNEL_OMNI
#define EP_MIDI_CHANNEL_MAX 16
#if defined(USE_EPIANO)
#define EP_MIDI_CHANNEL_DEFAULT DEFAULT_EP_MIDI_CHANNEL
#else
#define EP_MIDI_CHANNEL_DEFAULT 3
#endif

// Number of samples in each delay line
#define BRAIDS_FLANGE_DELAY_LENGTH (6 * AUDIO_BLOCK_SAMPLES)

#define FAVORITES_NUM_MIN 0
#define FAVORITES_NUM_MAX 100
#define FAVORITES_NUM_DEFAULT 0

#define STARTUP_NUM_MIN 0
#define STARTUP_NUM_MAX 255
#define STARTUP_NUM_DEFAULT 0

#define VOLUME_MULTIPLIER 1.4

//Microsynth
#define MS_SOUND_INTENSITY_MIN 0
#define MS_SOUND_INTENSITY_MAX 100
#define MS_SOUND_INTENSITY_DEFAULT 50

// Buffer-size define for load/save configuration as JSON
#define JSON_BUFFER_SIZE 12000

// Internal configuration structure
typedef struct dexed_s {
  uint8_t bank;
  uint8_t voice;
  uint8_t lowest_note;
  uint8_t highest_note;
  uint8_t transpose;
  uint8_t tune;
  uint8_t sound_intensity;
  uint8_t pan;
  uint8_t polyphony;
  uint8_t velocity_level;
  uint8_t monopoly;
  uint8_t note_refresh;
  uint8_t pb_range;
  uint8_t pb_step;
  uint8_t mw_range;
  uint8_t mw_assign;
  uint8_t mw_mode;
  uint8_t fc_range;
  uint8_t fc_assign;
  uint8_t fc_mode;
  uint8_t bc_range;
  uint8_t bc_assign;
  uint8_t bc_mode;
  uint8_t at_range;
  uint8_t at_assign;
  uint8_t at_mode;
  uint8_t portamento_mode;
  uint8_t portamento_glissando;
  uint8_t portamento_time;
  uint8_t op_enabled;
  uint8_t midi_channel;
} dexed_t;

typedef struct fx_s {
  uint8_t filter_cutoff[MAX_DEXED];
  uint8_t filter_resonance[MAX_DEXED];
  uint8_t chorus_frequency[MAX_DEXED];
  uint8_t chorus_waveform[MAX_DEXED];
  uint8_t chorus_depth[MAX_DEXED];
  uint8_t chorus_level[MAX_DEXED];
  uint8_t delay_time[MAX_DEXED];
  uint8_t delay_feedback[MAX_DEXED];
  uint8_t delay_level[MAX_DEXED];
  uint8_t delay_sync[MAX_DEXED];
  uint8_t reverb_send[MAX_DEXED];
  uint8_t reverb_roomsize;
  uint8_t reverb_damping;
  uint8_t reverb_lowpass;
  uint8_t reverb_lodamp;
  uint8_t reverb_hidamp;
  uint8_t reverb_diffusion;
  uint8_t reverb_level;
  uint8_t eq_1;
  int8_t eq_2;
  int8_t eq_3;
  int8_t eq_4;
  int8_t eq_5;
  int8_t eq_6;
  uint8_t eq_7;
  uint8_t ep_chorus_frequency;
  uint8_t ep_chorus_waveform;
  uint8_t ep_chorus_depth;
  uint8_t ep_chorus_level;
  uint8_t ep_reverb_send;
} fx_t;

typedef struct epiano_s {
  uint8_t decay;
  uint8_t release;
  uint8_t hardness;
  uint8_t treble;
  uint8_t pan_tremolo;
  uint8_t pan_lfo;
  uint8_t velocity_sense;
  uint8_t stereo;
  uint8_t polyphony;
  uint8_t tune;
  uint8_t detune;
  uint8_t overdrive;
  uint8_t lowest_note;
  uint8_t highest_note;
  uint8_t transpose;
  uint8_t sound_intensity;
  uint8_t pan;
  uint8_t midi_channel;
} epiano_t;

#ifdef USE_MICROSYNTH
typedef struct microsynth_s {
  int coarse;
  int detune;
  int lfo_intensity;
  int lfo_delay;
  uint8_t lfo_mode;
  uint8_t lfo_speed;
  //internal lfo values
  boolean lfo_direction;
  int lfo_value;
  int lfo_fade;
  bool trigger_noise_with_osc;
  uint8_t pan;
  uint8_t wave;
  uint8_t midi_channel;
  uint8_t sound_intensity;
  uint8_t env_attack;
  uint8_t env_decay;
  uint8_t env_sustain;
  uint8_t env_release;
  uint8_t filter_osc_mode;
  uint16_t osc_freq_current;
  uint16_t filter_osc_freq_from;
  uint16_t filter_osc_freq_to;
  uint16_t filter_osc_freq_current;
  uint16_t filter_osc_freq_last_displayed = 99;
  uint16_t filter_osc_speed;
  uint8_t filter_osc_resonance;
  uint8_t noise_vol;
  uint8_t noise_decay;
  uint8_t filter_noise_mode;
  uint16_t filter_noise_freq_from;
  uint16_t filter_noise_freq_to;
  uint16_t filter_noise_freq_current;
  uint16_t filter_noise_speed;
  uint8_t filter_noise_resonance;
  uint16_t pwm_from;
  uint16_t pwm_to;
  uint8_t pwm_speed;
  uint16_t pwm_current;
  uint16_t pwm_last_displayed;
  uint8_t rev_send;
  uint8_t chorus_send;
  uint8_t delay_send;
  uint8_t vel_mod_filter_osc = 0;
  uint8_t vel_mod_filter_noise = 0;
} microsynth_t;
#endif

#ifdef USE_BRAIDS
typedef struct braids_s {
  uint8_t sound_intensity;
  uint8_t algo;
  uint8_t color;
  uint8_t timbre;
  int coarse;
  uint8_t env_attack;
  uint8_t env_decay;
  uint8_t env_sustain;
  uint8_t env_release;
  uint8_t filter_mode;
  uint16_t filter_freq_from;
  uint16_t filter_freq_to;
  uint8_t filter_resonance;
  uint16_t filter_speed;
  uint8_t rev_send;
  uint8_t flanger;
  uint8_t flanger_spread;
  uint8_t delay_send;
  uint8_t midi_channel;
  uint8_t pan;
  uint8_t note_buffer[NUM_BRAIDS];
} braids_t;
#endif

typedef struct multisample_s {
  char name[18];
  uint8_t sound_intensity;
  uint8_t midi_channel;
} multisample_t;

typedef struct multisample_zone_s {
  char filename[MAX_FLASH_FILENAME_LEN];
  uint8_t entry_number;
  uint8_t rootnote;  // sample root note
  uint8_t low;       // lowest note in range
  uint8_t high;      // highest note in range
  bool playmode;     // Trigger/Hold Mode
  uint8_t vol;       // volume
  uint8_t pan;       // panorama
  uint8_t rev;       // reverb send
} multisample_zone_t;

typedef struct sys_s {
  uint8_t vol;
  uint8_t mono;
  uint8_t soft_midi_thru;
  uint8_t performance_number;
  uint8_t favorites;
  uint8_t load_at_startup_performance;
  uint8_t load_at_startup_page;
  uint8_t display_rotation;
  uint8_t touch_rotation;
  uint8_t screen_saver_start; // minutes
#ifdef USB_GAMEPAD
  uint16_t gamepad_speed; // milliseconds
#endif
  bool ui_reverse;
} sys_t;

#ifdef COMPILE_FOR_FLASH
typedef struct flash_s {
  char filenames[MAX_FLASH_FILES][MAX_FLASH_FILENAME_LEN];
  uint32_t sum_used;
  unsigned long chipsize;
} flash_t;
#endif

typedef struct configuration_s {
  sys_t sys;
  dexed_t dexed[MAX_DEXED];
  epiano_t epiano;
  fx_t fx;
} config_t;

typedef struct custom_midi_map_s {
  uint8_t type;     // 0 = empty, 1 = Key/Pad, 2 = CC / Values, 3 = Button push on/off
  uint8_t channel;  // Midi Channel
  uint8_t in;       // Midi Input Key/Pad / Value
  uint8_t out;      // Destination Key / Value
} custom_midi_map_t;

enum master_mixer_ports {
  MASTER_MIX_CH_DEXED2,
  MASTER_MIX_CH_DEXED1,
  MASTER_MIX_CH_REVERB,
  MASTER_MIX_CH_DRUMS,
  MASTER_MIX_CH_EPIANO,
  MASTER_MIX_CH_MICROSYNTH,
  MASTER_MIX_CH_BRAIDS,
  MASTER_MIX_CH_WAV_PREVIEW,
};

enum reverb_mixer_ports {
  REVERB_MIX_CH_DEXED2,
  REVERB_MIX_CH_DEXED1,
  REVERB_MIX_CH_DRUMS,
  REVERB_MIX_CH_EPIANO,
  REVERB_MIX_CH_MICROSYNTH,
};

#ifndef _MAPFLOAT
#define _MAPFLOAT
inline float mapfloat(float val, float in_min, float in_max, float out_min, float out_max) {
  return (val - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}
#endif
