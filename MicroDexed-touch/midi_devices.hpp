/*
   MicroDexed

   MicroMDAEPiano is a port of the MDA-EPiano sound engine
   (https://sourceforge.net/projects/mda-vst/) for the Teensy-3.5/3.6/4.x with audio shield.

   (c)2019-2021 H. Wirtz <wirtz@parasitstudio.de>

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

#ifndef MIDI_DEVICES_H
#define MIDI_DEVICES_H

#include "config.h"

extern config_t configuration;

#include "sequencer.h"
extern sequencer_t seq;
extern int temp_int;
extern uint8_t GAMEPAD_UP_0;
extern uint8_t GAMEPAD_UP_1;
extern uint32_t GAMEPAD_UP_BUTTONS;
extern uint8_t GAMEPAD_DOWN_0;
extern uint8_t GAMEPAD_DOWN_1;
extern uint32_t GAMEPAD_DOWN_BUTTONS;
extern uint8_t GAMEPAD_RIGHT_0;
extern uint8_t GAMEPAD_RIGHT_1;
extern uint32_t GAMEPAD_RIGTH_BUTTONS;
extern uint8_t GAMEPAD_LEFT_0;
extern uint8_t GAMEPAD_LEFT_1;
extern uint32_t GAMEPAD_LEFT_BUTTONS;
extern uint32_t GAMEPAD_SELECT;
extern uint32_t GAMEPAD_START;
extern uint32_t GAMEPAD_BUTTON_A;
extern uint32_t GAMEPAD_BUTTON_B;

/* #if defined(MIDI_DEVICE_USB)
  #include <midi_UsbTransport.h>
  #endif */
#ifdef MIDI_DEVICE_USB_HOST
#include <USBHost_t36.h>
#endif

  // override default sysex size settings
struct MicroDexedSettings : public midi::DefaultSettings
{
  static const unsigned SysExMaxSize = 4104; // Accept SysEx messages up to 1024 bytes long.
};

#ifdef MIDI_DEVICE_DIN
MIDI_CREATE_CUSTOM_INSTANCE(HardwareSerial, MIDI_DEVICE_DIN, midi_serial, MicroDexedSettings);
#endif
#ifdef MIDI_DEVICE_USB_HOST
USBHost usb_host;
MIDIDevice midi_usb(usb_host);
#endif

#ifdef USB_KEYPAD
KeyboardController keyboard1(usb_host);
#endif

USBHub hub1(usb_host);
USBHIDParser hid1(usb_host);

#define COUNT_JOYSTICKS 1
JoystickController joysticks[COUNT_JOYSTICKS](usb_host);
uint32_t gamepad_buttons_prev = 0;

USBDriver* drivers[] = { &hub1, &joysticks[0], &joysticks[1], &joysticks[2], &joysticks[3], &hid1 };
#define CNT_DEVICES (sizeof(drivers) / sizeof(drivers[0]))

bool driver_active[CNT_DEVICES] = { false, false, false, false };

// Lets also look at HID Input devices
USBHIDInput* hiddrivers[] = { &joysticks[0], &joysticks[1], &joysticks[2], &joysticks[3] };
#define CNT_HIDDEVICES (sizeof(hiddrivers) / sizeof(hiddrivers[0]))
const char* hid_driver_names[CNT_DEVICES] = { "joystick[0H]", "joystick[1H]", "joystick[2H]", "joystick[3H]" };
bool hid_driver_active[CNT_DEVICES] = { false };
bool show_changed_only = true;
uint64_t joystick_full_notify_mask = (uint64_t)-1;

/* #ifdef MIDI_DEVICE_USB
  static const unsigned sUsbTransportBufferSize = 16;
  typedef midi::UsbTransport<sUsbTransportBufferSize> UsbTransport;
  UsbTransport sUsbTransport;
  MIDI_CREATE_INSTANCE(UsbTransport, sUsbTransport, midi_onboard_usb);
  #endif */

void handleNoteOn(byte inChannel, byte inNumber, byte inVelocity, byte device);
void handleNoteOff(byte inChannel, byte inNumber, byte inVelocity, byte device);
void handleAfterTouch(byte inChannel, byte inPressure);
void handlePitchBend(byte inChannel, int16_t inPitch);
void handleControlChange(byte inChannel, byte inData1, byte inData2);
void handleProgramChange(byte inChannel, byte inProgram);
void handleAfterTouchPoly(byte inChannel, byte inNumber, byte inVelocity);
void handleSystemExclusive(byte* data, unsigned len);
// void handleSystemExclusiveChunk(const byte *data, uint len, bool last);
void handleTimeCodeQuarterFrame(byte data);
void handleSongSelect(byte inSong);
void handleTuneRequest(void);
void handleClock(void);
void handleStart(void);
void handleContinue(void);
void handleStop(void);
void handleActiveSensing(void);
void handleSystemReset(void);
// void handleRealTimeSystem(void);
void MD_sendControlChange(uint8_t channel, uint8_t cc, uint8_t value);

#define MIDI_BY_DIN "MIDI_DIN"
#define MIDI_BY_USB_HOST "MIDI_USB_HOST"
#define MIDI_BY_USB "USB_MIDI"

#include "livesequencer.h"

extern LiveSequencer liveSeq;
extern void handleNoteOnInput(byte inChannel, byte inNumber, byte inVelocity, byte device);

//void handle_pitchbend((byte inChannel, byte inData1, byte inData2, const char *midi_device, midi::MidiType event));

void handle_generic(byte inChannel, byte inData1, byte inData2, const char* midi_device, midi::MidiType event)
{
  liveSeq.handleMidiEvent(inChannel, event, inData1, inData2);
#ifdef DEBUG
  char text[10];
#endif
  bool _internal = false;

  switch (event)
  {
  case midi::NoteOn:
    seq.note_in = inData1;
    seq.note_in_velocity = inData2;
    handleNoteOnInput(inChannel, inData1, inData2, 0);
#ifdef DEBUG
#endif
    break;
  case midi::NoteOff:
    seq.note_in = inData1;
    seq.note_in_velocity = inData2;
    handleNoteOff(inChannel, inData1, inData2, 0);
#ifdef DEBUG
    strcpy(text, "NoteOff");
#endif
    break;
  case midi::ControlChange:
    // Internal CC ?
    if (inData1 >= 20 && inData1 <= 31)
      _internal = true;
    handleControlChange(inChannel, inData1, inData2);
#ifdef DEBUG
    strcpy(text, "CC");
#endif
    break;
  case midi::AfterTouchChannel:
    handleAfterTouch(inChannel, inData1);
#ifdef DEBUG
    strcpy(text, "Mono AT");
#endif
    break;
  case midi::ProgramChange:
    handleProgramChange(inChannel, inData1);
#ifdef DEBUG
    strcpy(text, "PC");
#endif
    break;
  case midi::AfterTouchPoly:
    handleAfterTouchPoly(inChannel, inData1, inData2);
#ifdef DEBUG
    strcpy(text, "Poly AT");
#endif
    break;
  default:
    break;
  }
#ifdef DEBUG
  LOG.printf_P(PSTR("MIDI handle_generic [%s] by [%s], ch:%d d1:%d d2:%d"), text, midi_device, inChannel, inData1, inData2);
#endif

  // MIDI THRU (only for non _internal MDT)
  if (configuration.sys.soft_midi_thru == 1 && !_internal)
  {
#ifdef MIDI_DEVICE_USB
    if (strcmp(MIDI_BY_USB, midi_device))
    {
      switch (event)
      {
      case midi::NoteOn:
        usbMIDI.sendNoteOn(inData1, inData2, inChannel);
        break;
      case midi::NoteOff:
        usbMIDI.sendNoteOff(inData1, inData2, inChannel);
        break;
      case midi::ControlChange:
        usbMIDI.sendControlChange(inData1, inData2, inChannel);
        break;
      case midi::AfterTouchChannel:
        usbMIDI.sendAfterTouch(inData1, inChannel);
        break;
      case midi::PitchBend:
        usbMIDI.sendPitchBend(inData1, inChannel);
        break;
      case midi::ProgramChange:
        usbMIDI.sendProgramChange(inData1, inChannel);
        break;
      case midi::AfterTouchPoly:
        usbMIDI.sendAfterTouch(inData1, inData2, inChannel);
        break;
      default:
        break;
      }
#ifdef DEBUG
      LOG.print(F(" THRU->MIDI_USB"));
#endif
    }
#endif

#ifdef MIDI_DEVICE_DIN
    if (strcmp(MIDI_BY_DIN, midi_device))
    {
      switch (event)
      {
      case midi::NoteOn:
        midi_serial.sendNoteOn(inData1, inData2, inChannel);
        break;
      case midi::NoteOff:
        midi_serial.sendNoteOff(inData1, inData2, inChannel);
        break;
      case midi::ControlChange:
        midi_serial.sendControlChange(inData1, inData2, inChannel);
        break;
      case midi::AfterTouchChannel:
        midi_serial.sendAfterTouch(inData1, inChannel);
        break;
      case midi::PitchBend:
        midi_serial.sendPitchBend(inData1, inChannel);
        break;
      case midi::ProgramChange:
        midi_serial.sendProgramChange(inData1, inChannel);
        break;
      case midi::AfterTouchPoly:
        midi_serial.sendAfterTouch(inData1, inData2, inChannel);
        break;
      default:
        break;
      }
#ifdef DEBUG
      LOG.print(F(" THRU->MIDI_DIN"));
#endif
    }
#endif

#ifdef MIDI_DEVICE_USB_HOST
    if (strcmp(MIDI_BY_USB_HOST, midi_device))
    {
      switch (event)
      {
      case midi::NoteOn:
        midi_usb.sendNoteOn(inData1, inData2, inChannel);
        break;
      case midi::NoteOff:
        midi_usb.sendNoteOff(inData1, inData2, inChannel);
        break;
      case midi::ControlChange:
        midi_usb.sendControlChange(inData1, inData2, inChannel);
        break;
      case midi::AfterTouchChannel:
        midi_usb.sendAfterTouch(inData1, inChannel);
        break;
      case midi::PitchBend:
        midi_usb.sendPitchBend(inData1, inChannel);
        break;
      case midi::ProgramChange:
        midi_usb.sendProgramChange(inData1, inChannel);
        break;
      case midi::AfterTouchPoly:
        midi_usb.sendAfterTouch(inData1, inData2, inChannel);
        break;
      default:
        break;
      }
#ifdef DEBUG
      LOG.print(F(" THRU->MIDI_USB_HOST"));
#endif
    }
#endif
  }

#ifdef DEBUG
  LOG.println();
#endif
}

void handleSystemExclusive_generic(byte* data, uint len, const char* midi_device)
{
  handleSystemExclusive(data, len);
#ifdef DEBUG
  LOG.printf_P(PSTR("[%s] SysEx"), midi_device);
#endif

  // MIDI THRU
  if (configuration.sys.soft_midi_thru == 1)
  {
#ifdef MIDI_DEVICE_USB
    if (strcmp(MIDI_BY_USB, midi_device))
    {
      usbMIDI.sendSysEx(len, data);
#ifdef DEBUG
      LOG.print(F(" THRU->MIDI_USB"));
#endif
    }
#endif

#ifdef MIDI_DEVICE_DIN
    if (strcmp(MIDI_BY_DIN, midi_device))
    {
      midi_serial.sendSysEx(len, data);
#ifdef DEBUG
      LOG.print(F(" THRU->MIDI_DIN"));
#endif
    }
#endif

#ifdef MIDI_DEVICE_USB_HOST
    if (strcmp(MIDI_BY_USB_HOST, midi_device))
    {
      midi_usb.sendSysEx(len, data);
#ifdef DEBUG
      LOG.print(F(" THRU->MIDI_USB_HOST"));
#endif
    }
#endif
  }

#ifdef DEBUG
  LOG.println();
#endif
}

void handleSystemCommon_generic(byte inData1, const char* midi_device, midi::MidiType event)
{
  char text[10];

  switch (event)
  {
  case midi::TimeCodeQuarterFrame:
    handleTimeCodeQuarterFrame(inData1);
    strcpy(text, "TimeCodeQuarterFrame");
    break;
  case midi::SongSelect:
    handleSongSelect(inData1);
    strcpy(text, "SongSelect");
    break;
  case midi::TuneRequest:
    handleTuneRequest();
    strcpy(text, "TuneRequest");
    break;
  default:
    break;
  }
#ifdef DEBUG
  LOG.printf_P(PSTR("[%s] %s"), midi_device, text);
#endif

  // MIDI THRU
  if (configuration.sys.soft_midi_thru == 1)
  {
#ifdef MIDI_DEVICE_USB
    if (strcmp(MIDI_BY_USB, midi_device))
    {
      switch (event)
      {
      case midi::TimeCodeQuarterFrame:
        usbMIDI.sendTimeCodeQuarterFrame(0xF1, inData1);
        break;
      case midi::SongSelect:
        usbMIDI.sendSongSelect(inData1);
        break;
      case midi::TuneRequest:
        usbMIDI.sendTuneRequest();
        break;
      default:
        break;
      }
#ifdef DEBUG
      LOG.print(F(" THRU->MIDI_USB"));
#endif
    }
#endif

#ifdef MIDI_DEVICE_DIN
    if (strcmp(MIDI_BY_DIN, midi_device))
    {
      switch (event)
      {
      case midi::TimeCodeQuarterFrame:
        midi_serial.sendTimeCodeQuarterFrame(inData1);
        break;
      case midi::SongSelect:
        midi_serial.sendSongSelect(inData1);
        break;
      case midi::TuneRequest:
        midi_serial.sendTuneRequest();
        break;
      default:
        break;
      }
#ifdef DEBUG
      LOG.print(F(" THRU->MIDI_DIN"));
#endif
    }
#endif

#ifdef MIDI_DEVICE_USB_HOST
    if (strcmp(MIDI_BY_USB_HOST, midi_device))
    {
      switch (event)
      {
      case midi::TimeCodeQuarterFrame:
        midi_usb.sendTimeCodeQuarterFrame(0xF1, inData1);
        break;
      case midi::SongSelect:
        midi_usb.sendSongSelect(inData1);
        break;
      case midi::TuneRequest:
        midi_usb.sendTuneRequest();
        break;
      default:
        break;
      }
#ifdef DEBUG
      LOG.print(F(" THRU->MIDI_USB_HOST"));
#endif
    }
#endif
  }

#ifdef DEBUG
  LOG.println();
#endif
}

void handleRealtime_generic(const char* midi_device, midi::MidiType event)
{
  char text[10];

  switch (event)
  {
  case midi::Clock:
    handleClock();
    strcpy(text, "Clock");
    break;
  case midi::Start:
    handleStart();
    strcpy(text, "Start");
    break;
  case midi::Continue:
    handleContinue();
    strcpy(text, "Continue");
    break;
  case midi::Stop:
    handleStop();
    strcpy(text, "Stop");
    break;
  case midi::ActiveSensing:
    handleActiveSensing();
    strcpy(text, "ActiveSensing");
    break;
  case midi::SystemReset:
    handleSystemReset();
    strcpy(text, "SystemReset");
    break;
  default:
    break;
  }
// #ifdef DEBUG
//   LOG.printf_P(PSTR("[%s] %s"), midi_device, text);
// #endif

  // MIDI THRU
  if (configuration.sys.soft_midi_thru == 1)
  {
#ifdef MIDI_DEVICE_USB
    if (strcmp(MIDI_BY_USB, midi_device))
    {
      usbMIDI.sendRealTime(event);
// #ifdef DEBUG
//       LOG.print(F(" THRU->MIDI_USB"));
// #endif
    }
#endif

#ifdef MIDI_DEVICE_DIN
    if (strcmp(MIDI_BY_DIN, midi_device))
    {
      midi_serial.sendRealTime(event);
// #ifdef DEBUG
//       LOG.print(F(" THRU->MIDI_DIN"));
// #endif
    }
#endif

#ifdef MIDI_DEVICE_USB_HOST
    if (strcmp(MIDI_BY_USB_HOST, midi_device))
    {
      midi_usb.sendRealTime(event);
#ifdef DEBUG
      LOG.print(F(" THRU->MIDI_USB_HOST"));
#endif
    }
#endif
  }

// #ifdef DEBUG
//   LOG.println();
// #endif
}

///* void handleSystemExclusiveChunk_MIDI_DEVICE_DIN(byte *data, uint len, bool last)

// void handlRealTimeSystem_generic(byte inRealTime, byte midi_device) {
//   handleRealTimeSystem();
// #ifdef DEBUG
//   switch(midi_device) {
//     case MIDI_DIN:
//       LOG.print(F("[MIDI_DIN] RealTimeSystem"));
//       break;
//     case MIDI_USB_HOST:
//       LOG.print(F("[MIDI_USB_HOST] RealTimeSystem"));
//       break;
//     case USB_MIDI:
//       LOG.print(F("[USB_MIDI] RealTimeSystem"));
//       break;
//   }
// #endif
//   if (configuration.sys.soft_midi_thru == 1)
//   {
// #ifdef MIDI_DEVICE_USB
//     if(midi_device != USB_MIDI) {
//         usbMIDI.sendRealTime(inRealTime);
//   #ifdef DEBUG
//         LOG.print(F(" THRU->MIDI_USB"));
//   #endif
//     }
// #endif

// #ifdef MIDI_DEVICE_DIN
//     if(midi_device != MIDI_DIN) {
//       midi_serial.sendRealTime((midi::MidiType)inRealTime);
//   #ifdef DEBUG
//         LOG.print(F(" THRU->MIDI_DIN"));
//   #endif
//     }
// #endif

// #ifdef MIDI_DEVICE_USB_HOST
//     if(midi_device != MIDI_USB_HOST) {
//         midi_usb.sendRealTime(inRealTime);
//   #ifdef DEBUG
//         LOG.print(F(" THRU->MIDI_USB_HOST"));
//   #endif
//     }
// #endif
//   }

// #ifdef DEBUG
//   LOG.println();
// #endif
// }


/*****************************************
   MIDI_DEVICE_DIN
 *****************************************/
#ifdef MIDI_DEVICE_DIN

void handleNoteOn_MIDI_DEVICE_DIN(byte inChannel, byte inNoteNumber, byte inVelocity)
{
  handle_generic(inChannel, inNoteNumber, inVelocity, MIDI_BY_DIN, midi::NoteOn);
}

void handleNoteOff_MIDI_DEVICE_DIN(byte inChannel, byte inNoteNumber, byte inVelocity)
{
  handle_generic(inChannel, inNoteNumber, inVelocity, MIDI_BY_DIN, midi::NoteOff);
}

void handleControlChange_MIDI_DEVICE_DIN(byte inChannel, byte inData1, byte inData2)
{
  handle_generic(inChannel, inData1, inData2, MIDI_BY_DIN, midi::ControlChange);
}

void handleAfterTouch_MIDI_DEVICE_DIN(byte inChannel, byte inPressure)
{
  handle_generic(inChannel, inPressure, '\0', MIDI_BY_DIN, midi::AfterTouchChannel);
}

void handlePitchBend_MIDI_DEVICE_DIN(byte inChannel, int inPitch)
{
  handlePitchBend(inChannel, inPitch);
}

void handleProgramChange_MIDI_DEVICE_DIN(byte inChannel, byte inProgram)
{
  handle_generic(inChannel, inProgram, '\0', MIDI_BY_DIN, midi::ProgramChange);
}

void handleAfterTouchPoly_MIDI_DEVICE_DIN(byte inChannel, byte inNoteNumber, byte inVelocity)
{
  handle_generic(inChannel, inNoteNumber, inVelocity, MIDI_BY_DIN, midi::AfterTouchPoly);
}

void handleSystemExclusive_MIDI_DEVICE_DIN(byte* data, uint len)
{
  handleSystemExclusive_generic(data, len, MIDI_BY_DIN);
}

/* void handleSystemExclusiveChunk_MIDI_DEVICE_DIN(byte *data, uint len, bool last)
  {
  handleSystemExclusiveChunk(data, len, last);
  #ifdef DEBUG
  LOG.print(F("[MIDI_DIN] SysExChunk"));
  #endif
    if (configuration.sys.soft_midi_thru == 1)
  {
  #ifdef MIDI_DEVICE_USB_HOST
  midi_usb.sendSysEx(len, data, last);
  #ifdef DEBUG
  LOG.print(F(" THRU->MIDI_USB_HOST"));
  #endif
  #endif
  #ifdef MIDI_DEVICE_USB
  usbMIDI.sendSysEx(len, data, last);
  #ifdef DEBUG
  LOG.print(F(" THRU->MIDI_USB"));
  #endif
  #endif
  }
  #ifdef DEBUG
  LOG.println();
  #endif
  } */

void handleTimeCodeQuarterFrame_MIDI_DEVICE_DIN(byte data)
{
  handleSystemCommon_generic(data, MIDI_BY_DIN, midi::TimeCodeQuarterFrame);
}

void handleSongSelect_MIDI_DEVICE_DIN(byte inSong)
{
  handleSystemCommon_generic(inSong, MIDI_BY_DIN, midi::SongSelect);
}

void handleTuneRequest_MIDI_DEVICE_DIN(void)
{
  handleSystemCommon_generic('\0', MIDI_BY_DIN, midi::TuneRequest);
}

void handleClock_MIDI_DEVICE_DIN(void)
{
  handleRealtime_generic(MIDI_BY_DIN, midi::Clock);
}

void handleStart_MIDI_DEVICE_DIN(void)
{
  handleRealtime_generic(MIDI_BY_DIN, midi::Start);
}

void handleContinue_MIDI_DEVICE_DIN(void)
{
  handleRealtime_generic(MIDI_BY_DIN, midi::Continue);
}

void handleStop_MIDI_DEVICE_DIN(void)
{
  handleRealtime_generic(MIDI_BY_DIN, midi::Stop);
}

void handleActiveSensing_MIDI_DEVICE_DIN(void)
{
  handleRealtime_generic(MIDI_BY_DIN, midi::ActiveSensing);
}

void handleSystemReset_MIDI_DEVICE_DIN(void)
{
  handleRealtime_generic(MIDI_BY_DIN, midi::SystemReset);
}

/* void handlRealTimeSysteme_MIDI_DEVICE_DIN(byte inRealTime)
  {
  handleRealTimeSystem_generic(MIDI_DIN);
  } */
#endif // MIDI_DEVICE_DIN

  /*****************************************
     MIDI_DEVICE_USB_HOST
   *****************************************/
#ifdef MIDI_DEVICE_USB_HOST
void handleNoteOn_MIDI_DEVICE_USB_HOST(byte inChannel, byte inNoteNumber, byte inVelocity)
{
  handle_generic(inChannel, inNoteNumber, inVelocity, MIDI_BY_USB_HOST, midi::NoteOn);
}

void handleNoteOff_MIDI_DEVICE_USB_HOST(byte inChannel, byte inNoteNumber, byte inVelocity)
{
  handle_generic(inChannel, inNoteNumber, inVelocity, MIDI_BY_USB_HOST, midi::NoteOff);
}

void handleControlChange_MIDI_DEVICE_USB_HOST(byte inChannel, byte inData1, byte inData2)
{
  handle_generic(inChannel, inData1, inData2, MIDI_BY_USB_HOST, midi::ControlChange);
}

void handleAfterTouch_MIDI_DEVICE_USB_HOST(byte inChannel, byte inPressure)
{
  handle_generic(inChannel, inPressure, '\0', MIDI_BY_USB_HOST, midi::AfterTouchChannel);
}

void handlePitchBend_MIDI_DEVICE_USB_HOST(byte inChannel, int inPitch)
{
  handlePitchBend(inChannel, inPitch);
}

void handleProgramChange_MIDI_DEVICE_USB_HOST(byte inChannel, byte inPrg)
{
  handle_generic(inChannel, inPrg, '\0', MIDI_BY_USB_HOST, midi::ProgramChange);
}

void handleAfterTouchPoly_MIDI_DEVICE_USB_HOST(byte inChannel, byte inNoteNumber, byte inVelocity)
{
  handle_generic(inChannel, inNoteNumber, inVelocity, MIDI_BY_USB_HOST, midi::AfterTouchPoly);
}

void handleSystemExclusive_MIDI_DEVICE_USB_HOST(byte* data, uint len)
{
  handleSystemExclusive_generic(data, len, MIDI_BY_USB_HOST);
}

/* void handleSystemExclusiveChunk_MIDI_DEVICE_USB_HOST(byte *data, uint len, bool last)
  {
  handleSystemExclusiveChunk(data, len, last);
  #ifdef DEBUG
  LOG.print(F("[MIDI_USB_HOST] SysExChunk"));
  #endif
    if (configuration.sys.soft_midi_thru == 1)
  {
  #ifdef MIDI_DEVICE_DIN
  midi_serial.sendSysEx(len, data, last);
  #ifdef DEBUG
  LOG.print(F(" THRU->MIDI_DIN"));
  #endif
  #endif
  #ifdef MIDI_DEVICE_USB
  usbMIDI.sendSysEx(len, data, last);
  #ifdef DEBUG
  LOG.print(F(" THRU->MIDI_USB"));
  #endif
  #endif
  }
  #ifdef DEBUG
  LOG.println();
  #endif
  } */

void handleTimeCodeQuarterFrame_MIDI_DEVICE_USB_HOST(midi::DataByte data)
{
  handleSystemCommon_generic(data, MIDI_BY_USB_HOST, midi::TimeCodeQuarterFrame);
}

void handleSongSelect_MIDI_DEVICE_USB_HOST(byte inSong)
{
  handleSystemCommon_generic(inSong, MIDI_BY_USB_HOST, midi::SongSelect);
}

void handleTuneRequest_MIDI_DEVICE_USB_HOST(void)
{
  handleSystemCommon_generic('\0', MIDI_BY_USB_HOST, midi::TuneRequest);
}

void handleClock_MIDI_DEVICE_USB_HOST(void)
{
  handleRealtime_generic(MIDI_BY_USB_HOST, midi::Clock);
}

void handleStart_MIDI_DEVICE_USB_HOST(void)
{
  handleRealtime_generic(MIDI_BY_USB_HOST, midi::Start);
}

void handleContinue_MIDI_DEVICE_USB_HOST(void)
{
  handleRealtime_generic(MIDI_BY_USB_HOST, midi::Continue);
}

void handleStop_MIDI_DEVICE_USB_HOST(void)
{
  handleRealtime_generic(MIDI_BY_USB_HOST, midi::Stop);
}

void handleActiveSensing_MIDI_DEVICE_USB_HOST(void)
{
  handleRealtime_generic(MIDI_BY_USB_HOST, midi::ActiveSensing);
}

void handleSystemReset_MIDI_DEVICE_USB_HOST(void)
{
  handleRealtime_generic(MIDI_BY_USB_HOST, midi::SystemReset);
}

/* void handlRealTimeSystem_MIDI_DEVICE_USB_HOST(midi::MidiType inRealTime)
  {
  handleRealTimeSystem_generic(inRealTime, MIDI_USB_HOST);
  } */
#endif // MIDI_DEVICE_USB_HOST

  /*****************************************
     MIDI_DEVICE_USB
   *****************************************/
#ifdef MIDI_DEVICE_USB
void handleNoteOn_MIDI_DEVICE_USB(byte inChannel, byte inNoteNumber, byte inVelocity)
{
  handle_generic(inChannel, inNoteNumber, inVelocity, MIDI_BY_USB, midi::NoteOn);
}

void handleNoteOff_MIDI_DEVICE_USB(byte inChannel, byte inNoteNumber, byte inVelocity)
{
  handle_generic(inChannel, inNoteNumber, inVelocity, MIDI_BY_USB, midi::NoteOff);
}

void handleControlChange_MIDI_DEVICE_USB(byte inChannel, byte inData1, byte inData2)
{
  handle_generic(inChannel, inData1, inData2, MIDI_BY_USB, midi::ControlChange);
}

void handleAfterTouch_MIDI_DEVICE_USB(byte inChannel, byte inPressure)
{
  handle_generic(inChannel, inPressure, '\0', MIDI_BY_USB, midi::AfterTouchChannel);
}

void handlePitchBend_MIDI_DEVICE_USB(byte inChannel, int inPitch)
{
  handlePitchBend(inChannel, inPitch);
}

void handleProgramChange_MIDI_DEVICE_USB(byte inChannel, byte inProgram)
{
  handle_generic(inChannel, inProgram, '\0', MIDI_BY_USB, midi::ProgramChange);
}

void handleAfterTouchPoly_MIDI_DEVICE_USB(byte inChannel, byte inNoteNumber, byte inVelocity)
{
  handle_generic(inChannel, inNoteNumber, inVelocity, MIDI_BY_USB, midi::AfterTouchPoly);
}

void handleSystemExclusive_MIDI_DEVICE_USB(byte* data, uint len)
{
  handleSystemExclusive_generic(data, len, MIDI_BY_USB);
}

/* FLASHMEM void handleSystemExclusiveChunk_MIDI_DEVICE_USB(byte *data, uint len, bool last)
  {
  handleSystemExclusiveChunk(data, len, last);
  #ifdef DEBUG
  LOG.print(F("[MIDI_USB] SysExChunk"));
  #endif
    if (configuration.sys.soft_midi_thru == 1)
  {
  #ifdef MIDI_DEVICE_DIN
  midi_serial.sendSysEx(len, data, last);
  #ifdef DEBUG
  LOG.print(F(" THRU->MIDI_DIN"));
  #endif
  #endif
  #ifdef MIDI_DEVICE_USB_HOST
  midi_usb.sendSysEx(len, data, last);
  #ifdef DEBUG
  LOG.print(F(" THRU->MIDI_USB_HOST"));
  #endif
  #endif
  }
  #ifdef DEBUG
  LOG.println();
  #endif
  } */

void handleTimeCodeQuarterFrame_MIDI_DEVICE_USB(midi::DataByte data)
{
  handleSystemCommon_generic(data, MIDI_BY_USB, midi::TimeCodeQuarterFrame);
}

void handleSongSelect_MIDI_DEVICE_USB(byte inSong)
{
  handleSystemCommon_generic(inSong, MIDI_BY_USB, midi::SongSelect);
}

void handleTuneRequest_MIDI_DEVICE_USB(void)
{
  handleSystemCommon_generic('\0', MIDI_BY_USB, midi::TuneRequest);
}

void handleClock_MIDI_DEVICE_USB(void)
{
  handleRealtime_generic(MIDI_BY_USB, midi::Clock);
}

void handleStart_MIDI_DEVICE_USB(void)
{
  handleRealtime_generic(MIDI_BY_USB, midi::Start);
}

void handleContinue_MIDI_DEVICE_USB(void)
{
  handleRealtime_generic(MIDI_BY_USB, midi::Continue);
}

void handleStop_MIDI_DEVICE_USB(void)
{
  handleRealtime_generic(MIDI_BY_USB, midi::Stop);
}

void handleActiveSensing_MIDI_DEVICE_USB(void)
{
  handleRealtime_generic(MIDI_BY_USB, midi::ActiveSensing);
}

void handleSystemReset_MIDI_DEVICE_USB(void)
{
  handleRealtime_generic(MIDI_BY_USB, midi::SystemReset);
}

/* FLASHMEM void handleRealTimeSystem_MIDI_DEVICE_USB(byte inRealTime)
  {
  handleRealTimeSystem_generic(inRealTime, USB_MIDI);
  } */
#endif // MIDI_DEVICE_USB

FLASHMEM void MD_sendControlChange(uint8_t channel, uint8_t cc, uint8_t value)
{
#ifdef DEBUG
  LOG.print(F("[MD] SendControlChange CH:"));
  LOG.print(channel, DEC);
  LOG.print(F(" CC:"));
  LOG.print(cc);
  LOG.print(F(" VAL:"));
  LOG.print(value);
#endif
#ifdef MIDI_DEVICE_DIN
  midi_serial.sendControlChange(cc, value, channel);
#ifdef DEBUG
  LOG.print(F(" MIDI-DIN"));
#endif
#endif
#ifdef MIDI_DEVICE_USB_HOST
  midi_usb.sendControlChange(cc, value, channel);
#ifdef DEBUG
  LOG.print(F(" MIDI-USB-HOST"));
#endif
#endif
#ifdef MIDI_DEVICE_USB
  usbMIDI.sendControlChange(cc, value, channel);
#ifdef DEBUG
  LOG.print(F(" MIDI-USB"));
#endif
#endif
#ifdef DEBUG
  LOG.println();
#endif
}

/*****************************************
   HELPER FUNCTIONS
 *****************************************/

#ifdef USB_KEYPAD
uint8_t USB_KEY = 0;

void OnPress(int key)
{
  USB_KEY = key;
}

void OnHIDExtrasRelease(uint32_t top, uint16_t key)
{
#ifdef KEYBOARD_INTERFACE
  if (top == 0xc0000)
  {
    Keyboard.release(0XE400 | key);
  }
#endif
  // #ifdef SHOW_KEYBOARD_DATA
  //   LOG.print(F("HID ("));
  //   LOG.print(top, HEX);
  //   LOG.print(F(") key release:"));
  //   LOG.println(key, HEX);
  // #endif
}

void OnRawPress(uint8_t keycode)
{
#ifdef KEYBOARD_INTERFACE
  if (keyboard_leds != keyboard_last_leds)
  {
    // LOG.printf_P(PSTR("New LEDS: %x\n"), keyboard_leds);
    keyboard_last_leds = keyboard_leds;
    keyboard1.LEDS(keyboard_leds);
  }
  if (keycode >= 103 && keycode < 111)
  {
    // one of the modifier keys was pressed, so lets turn it
    // on global..
    uint8_t keybit = 1 << (keycode - 103);
    keyboard_modifiers |= keybit;
    Keyboard.set_modifier(keyboard_modifiers);
  }
  else
  {
    if (keyboard1.getModifiers() != keyboard_modifiers)
    {
#ifdef SHOW_KEYBOARD_DATA
      LOG.printf_P(PSTR("Mods mismatch: %x != %x\n"), keyboard_modifiers, keyboard1.getModifiers());
#endif
      keyboard_modifiers = keyboard1.getModifiers();
      Keyboard.set_modifier(keyboard_modifiers);
    }
    Keyboard.press(0XF000 | keycode);
  }
#endif
  // #ifdef SHOW_KEYBOARD_DATA
  //   LOG.print(F("OnRawPress keycode: "));
  //   LOG.print(keycode, HEX);
  //   LOG.print(F(" Modifiers: "));
  //   LOG.println(keyboard_modifiers, HEX);
  // #endif
}

void OnRawRelease(uint8_t keycode)
{

  USB_KEY = 0;

#ifdef KEYBOARD_INTERFACE
  if (keycode >= 103 && keycode < 111)
  {
    // one of the modifier keys was pressed, so lets turn it
    // on global..
    uint8_t keybit = 1 << (keycode - 103);
    keyboard_modifiers &= ~keybit;
    Keyboard.set_modifier(keyboard_modifiers);
  }
  else
  {
    Keyboard.release(0XF000 | keycode);
  }
#endif
  // #ifdef SHOW_KEYBOARD_DATA
  //   LOG.print(F("OnRawRelease keycode: "));
  //   LOG.print(keycode, HEX);
  //   LOG.print(F(" Modifiers: "));
  //   LOG.println(keyboard1.getModifiers(), HEX);
  // #endif
}
#endif

FLASHMEM void setup_midi_devices(void)
{
#ifdef MIDI_DEVICE_DIN
  // Start serial MIDI
  midi_serial.begin(DEFAULT_MIDI_CHANNEL);
  midi_serial.setHandleNoteOn(handleNoteOn_MIDI_DEVICE_DIN);
  midi_serial.setHandleNoteOff(handleNoteOff_MIDI_DEVICE_DIN);
  midi_serial.setHandleControlChange(handleControlChange_MIDI_DEVICE_DIN);
  midi_serial.setHandleAfterTouchChannel(handleAfterTouch_MIDI_DEVICE_DIN);
  midi_serial.setHandlePitchBend(handlePitchBend_MIDI_DEVICE_DIN);
  midi_serial.setHandleProgramChange(handleProgramChange_MIDI_DEVICE_DIN);
  midi_serial.setHandleSystemExclusive(handleSystemExclusive_MIDI_DEVICE_DIN);
  // midi_serial.setHandleSystemExclusiveChunk(handleSystemExclusiveChunk_MIDI_DEVICE_DIN);
  midi_serial.setHandleTimeCodeQuarterFrame(handleTimeCodeQuarterFrame_MIDI_DEVICE_DIN);
  midi_serial.setHandleAfterTouchPoly(handleAfterTouchPoly_MIDI_DEVICE_DIN);
  midi_serial.setHandleSongSelect(handleSongSelect_MIDI_DEVICE_DIN);
  midi_serial.setHandleTuneRequest(handleTuneRequest_MIDI_DEVICE_DIN);
  midi_serial.setHandleClock(handleClock_MIDI_DEVICE_DIN);
  midi_serial.setHandleStart(handleStart_MIDI_DEVICE_DIN);
  midi_serial.setHandleContinue(handleContinue_MIDI_DEVICE_DIN);
  midi_serial.setHandleStop(handleStop_MIDI_DEVICE_DIN);
  midi_serial.setHandleActiveSensing(handleActiveSensing_MIDI_DEVICE_DIN);
  midi_serial.setHandleSystemReset(handleSystemReset_MIDI_DEVICE_DIN);
  // midi_serial.setHandleRealTimeSystem(handleRealTimeSystem_MIDI_DEVICE_DIN);
#ifdef DEBUG
  LOG.println(F("MIDI_DEVICE_DIN enabled"));
#endif
#endif

  // start up USB host
#ifdef MIDI_DEVICE_USB_HOST
  usb_host.begin();

#ifdef USB_KEYPAD
  keyboard1.attachPress(OnPress);
  keyboard1.attachRawPress(OnRawPress);
  keyboard1.attachRawRelease(OnRawRelease);
  // keyboard1.attachExtrasPress(OnHIDExtrasPress);
  // keyboard1.attachExtrasRelease(OnHIDExtrasRelease);
#endif

  midi_usb.setHandleNoteOn(handleNoteOn_MIDI_DEVICE_USB_HOST);
  midi_usb.setHandleNoteOff(handleNoteOff_MIDI_DEVICE_USB_HOST);
  midi_usb.setHandleControlChange(handleControlChange_MIDI_DEVICE_USB_HOST);
  midi_usb.setHandleAfterTouchChannel(handleAfterTouch_MIDI_DEVICE_USB_HOST);
  midi_usb.setHandlePitchChange(handlePitchBend_MIDI_DEVICE_USB_HOST);
  midi_usb.setHandleProgramChange(handleProgramChange_MIDI_DEVICE_USB_HOST);
  midi_usb.setHandleSystemExclusive(handleSystemExclusive_MIDI_DEVICE_USB_HOST);
  // midi_usb.setHandleSystemExclusiveChunk(handleSystemExclusiveChunk_MIDI_DEVICE_USB_HOST);
  midi_usb.setHandleTimeCodeQuarterFrame(handleTimeCodeQuarterFrame_MIDI_DEVICE_USB_HOST);
  midi_usb.setHandleAfterTouchPoly(handleAfterTouchPoly_MIDI_DEVICE_USB_HOST);
  midi_usb.setHandleSongSelect(handleSongSelect_MIDI_DEVICE_USB_HOST);
  midi_usb.setHandleTuneRequest(handleTuneRequest_MIDI_DEVICE_USB_HOST);
  midi_usb.setHandleClock(handleClock_MIDI_DEVICE_USB_HOST);
  midi_usb.setHandleStart(handleStart_MIDI_DEVICE_USB_HOST);
  midi_usb.setHandleContinue(handleContinue_MIDI_DEVICE_USB_HOST);
  midi_usb.setHandleStop(handleStop_MIDI_DEVICE_USB_HOST);
  midi_usb.setHandleActiveSensing(handleActiveSensing_MIDI_DEVICE_USB_HOST);
  midi_usb.setHandleSystemReset(handleSystemReset_MIDI_DEVICE_USB_HOST);
  // midi_usb.setHandleRealTimeSystem(handleRealTimeSystem_MIDI_DEVICE_USB_HOST);
#ifdef DEBUG
  LOG.println(F("MIDI_DEVICE_USB_HOST enabled."));
#endif
#endif

  // check for onboard USB-MIDI
#ifdef MIDI_DEVICE_USB
  usbMIDI.begin();
  usbMIDI.setHandleNoteOn(handleNoteOn_MIDI_DEVICE_USB);
  usbMIDI.setHandleNoteOff(handleNoteOff_MIDI_DEVICE_USB);
  usbMIDI.setHandleControlChange(handleControlChange_MIDI_DEVICE_USB);
  usbMIDI.setHandleAfterTouchChannel(handleAfterTouch_MIDI_DEVICE_USB);
  usbMIDI.setHandlePitchChange(handlePitchBend_MIDI_DEVICE_USB);
  usbMIDI.setHandleProgramChange(handleProgramChange_MIDI_DEVICE_USB);
  usbMIDI.setHandleSystemExclusive(handleSystemExclusive_MIDI_DEVICE_USB);
  // usbMIDI.setHandleSystemExclusiveChunk(handleSystemExclusiveChunk_MIDI_DEVICE_USB);
  usbMIDI.setHandleTimeCodeQuarterFrame(handleTimeCodeQuarterFrame_MIDI_DEVICE_USB);
  usbMIDI.setHandleAfterTouchPoly(handleAfterTouchPoly_MIDI_DEVICE_USB);
  usbMIDI.setHandleSongSelect(handleSongSelect_MIDI_DEVICE_USB);
  usbMIDI.setHandleTuneRequest(handleTuneRequest_MIDI_DEVICE_USB);
  usbMIDI.setHandleClock(handleClock_MIDI_DEVICE_USB);
  usbMIDI.setHandleStart(handleStart_MIDI_DEVICE_USB);
  usbMIDI.setHandleContinue(handleContinue_MIDI_DEVICE_USB);
  usbMIDI.setHandleStop(handleStop_MIDI_DEVICE_USB);
  usbMIDI.setHandleActiveSensing(handleActiveSensing_MIDI_DEVICE_USB);
  usbMIDI.setHandleSystemReset(handleSystemReset_MIDI_DEVICE_USB);
  // usbMIDI.setHandleRealTimeSystem(handleRealTimeSystem_MIDI_DEVICE_USB);
#ifdef DEBUG
  LOG.println(F("MIDI_DEVICE_USB enabled."));
#endif
#endif
}

void check_midi_devices(void)
{
#ifdef MIDI_DEVICE_DIN
  midi_serial.read();
#endif
#ifdef MIDI_DEVICE_USB
  usbMIDI.read();
#endif
#ifdef MIDI_DEVICE_USB_HOST
  usb_host.Task();
  midi_usb.read();
#endif
}

FLASHMEM void send_sysex_voice(uint8_t midi_channel, uint8_t* data)
{
  uint8_t checksum = 0;
  uint8_t vd[161];

  // Send SYSEX data also via MIDI
  // vd[0] = 0xF0; // SysEx start
  vd[0] = 0x43;         // ID=Yamaha
  vd[1] = midi_channel; // Sub-status and MIDI channel
  vd[2] = 0x00;         // Format number (0=1 voice)
  vd[3] = 0x01;         // Byte count MSB
  vd[4] = 0x1B;         // Byte count LSB
  for (uint8_t n = 0; n < 155; n++)
  {
    checksum -= data[n];
    vd[5 + n] = data[n];
  }
  vd[160] = checksum & 0x7f; // Checksum
  // vd[162] = 0xF7; // SysEx end

#ifdef MIDI_DEVICE_DIN
  midi_serial.sendSysEx(161, vd); // Send to DIN MIDI
#endif
#ifdef MIDI_DEVICE_USB
  usbMIDI.sendSysEx(161, vd); // Send to USB MIDI
#endif
#ifdef MIDI_DEVICE_USB_HOST
  midi_usb.sendSysEx(161, vd); // Send to USB-HOST MIDI
#endif
}

FLASHMEM void send_sysex_bank(uint8_t midi_channel, uint8_t* bank_data)
{
#ifdef MIDI_DEVICE_DIN
  midi_serial.sendSysEx(4104, bank_data); // Send to DIN MIDI
#endif
#ifdef MIDI_DEVICE_USB
  // Sysex bank dump is splitted due to Windows USB driver limitations
  usbMIDI.sendSysEx(2048, bank_data, true); // Send to USB MIDI
  delay(50);
  usbMIDI.sendSysEx(2048, bank_data + 2048, true);
  delay(50);
  usbMIDI.sendSysEx(8, bank_data + 4096, true);
#endif
#ifdef MIDI_DEVICE_USB_HOST
  midi_usb.sendSysEx(4104, bank_data); // Send to USB-HOST MIDI
#endif
}

FLASHMEM void send_sysex_param(uint8_t midi_channel, uint8_t var, uint8_t val, uint8_t param_group)
{
  uint8_t s[5];

  s[0] = 0x43;                   // ID=Yamaha
  s[1] = midi_channel;           // Sub-status and MIDI channel
  s[2] = (param_group & 5) << 2; // Format number (0=1 voice)
  if (param_group == 0)
  {
    s[2] |= 1;
    s[3] = var & 0x7f;
  }
  else
  {
    s[3] = var & 0x7f;
  }
  s[4] = val & 0x7f;

#ifdef MIDI_DEVICE_DIN
  midi_serial.sendSysEx(5, s); // Send to DIN MIDI
#endif
#ifdef MIDI_DEVICE_USB
  usbMIDI.sendSysEx(5, s); // Send to USB MIDI
#endif
#ifdef MIDI_DEVICE_USB_HOST
  midi_usb.sendSysEx(5, s); // Send to USB-HOST MIDI
#endif
}

#endif // MIDI_DEVICES_H
