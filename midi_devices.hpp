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

#ifdef USE_SEQUENCER
#include "sequencer.h"
extern sequencer_t seq;
#endif

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
/* #ifdef MIDI_DEVICE_USB
  static const unsigned sUsbTransportBufferSize = 16;
  typedef midi::UsbTransport<sUsbTransportBufferSize> UsbTransport;
  UsbTransport sUsbTransport;
  MIDI_CREATE_INSTANCE(UsbTransport, sUsbTransport, midi_onboard_usb);
  #endif */

void handleNoteOn(byte inChannel, byte inNumber, byte inVelocity, byte device);
void handleNoteOff(byte inChannel, byte inNumber, byte inVelocity, byte device);
void handleControlChange(byte inChannel, byte inData1, byte inData2);
void handleAfterTouch(byte inChannel, byte inPressure);
void handlePitchBend(byte inChannel, int inPitch);
void handleProgramChange(byte inChannel, byte inProgram);
void handleSystemExclusive(byte *data, uint len);
//void handleSystemExclusiveChunk(const byte *data, uint len, bool last);
void handleTimeCodeQuarterFrame(byte data);
void handleAfterTouchPoly(byte inChannel, byte inNumber, byte inVelocity);
void handleSongSelect(byte inSong);
void handleTuneRequest(void);
void handleClock(void);
void handleStart(void);
void handleContinue(void);
void handleStop(void);
void handleActiveSensing(void);
void handleSystemReset(void);
//void handleRealTimeSystem(void);
void MD_sendControlChange(uint8_t channel, uint8_t cc, uint8_t value);

enum midi_devices {
  MIDI_DIN,
  MIDI_USB_HOST,
  USB_MIDI,
};

void handleNoteOn_generic(byte inChannel, byte inNumber, byte inVelocity, byte midi_device)
{
  seq.note_in = inNumber;
  seq.note_in_velocity = inVelocity;
  handleNoteOn(inChannel, inNumber, inVelocity, 0);
#ifdef DEBUG
  switch(midi_device) {
    case MIDI_DIN: 
      Serial.print(F("[MIDI_DIN] NoteOn"));
      break;
    case MIDI_USB_HOST: 
      Serial.print(F("[MIDI_USB_HOST] NoteOn"));
      break;
    case USB_MIDI: 
      Serial.print(F("[USB_MIDI] NoteOn"));
      break;
  }
#endif
  if (configuration.sys.soft_midi_thru == 1)
  {
#ifdef MIDI_DEVICE_USB
    if(midi_device != USB_MIDI) {
        usbMIDI.sendNoteOn(inNumber, inVelocity, inChannel);
  #ifdef DEBUG
        Serial.print(F(" THRU->MIDI_USB"));
  #endif
    }
#endif

#ifdef MIDI_DEVICE_DIN
    if(midi_device != MIDI_DIN) {
        midi_serial.sendNoteOn(inNumber, inVelocity, inChannel);
  #ifdef DEBUG
        Serial.print(F(" THRU->MIDI_DIN"));
  #endif
    }
#endif

#ifdef MIDI_DEVICE_USB_HOST
    if(midi_device != MIDI_USB_HOST) {
        midi_usb.sendNoteOn(inNumber, inVelocity, inChannel);
  #ifdef DEBUG
        Serial.print(F(" THRU->MIDI_USB_HOST"));
  #endif
    }
#endif
  }

#ifdef DEBUG
  Serial.println();
#endif
}

void handleNoteOff_generic(byte inChannel, byte inNumber, byte inVelocity, byte midi_device)
{
  seq.note_in = inNumber;
  seq.note_in_velocity = inVelocity;
  handleNoteOff(inChannel, inNumber, inVelocity, 0);
#ifdef DEBUG
  switch(midi_device) {
    case MIDI_DIN: 
      Serial.print(F("[MIDI_DIN] NoteOff"));
      break;
    case MIDI_USB_HOST: 
      Serial.print(F("[MIDI_USB_HOST] NoteOff"));
      break;
    case USB_MIDI: 
      Serial.print(F("[USB_MIDI] NoteOff"));
      break;
  }
#endif
  if (configuration.sys.soft_midi_thru == 1)
  {
#ifdef MIDI_DEVICE_USB
    if(midi_device != USB_MIDI) {
        usbMIDI.sendNoteOff(inNumber, inVelocity, inChannel);
  #ifdef DEBUG
        Serial.print(F(" THRU->MIDI_USB"));
  #endif
    }
#endif

#ifdef MIDI_DEVICE_DIN
    if(midi_device != MIDI_DIN) {
        midi_serial.sendNoteOff(inNumber, inVelocity, inChannel);
  #ifdef DEBUG
        Serial.print(F(" THRU->MIDI_DIN"));
  #endif
    }
#endif

#ifdef MIDI_DEVICE_USB_HOST
    if(midi_device != MIDI_USB_HOST) {
        midi_usb.sendNoteOff(inNumber, inVelocity, inChannel);
  #ifdef DEBUG
        Serial.print(F(" THRU->MIDI_USB_HOST"));
  #endif
    }
#endif
  }

#ifdef DEBUG
  Serial.println();
#endif
}

void handleControlChange_generic(byte inChannel, byte inData1, byte inData2, byte midi_device)
{
  handleControlChange(inChannel, inData1, inData2);
#ifdef DEBUG
  switch(midi_device) {
    case MIDI_DIN: 
      Serial.print(F("[MIDI_DIN] CC"));
      break;
    case MIDI_USB_HOST: 
      Serial.print(F("[MIDI_USB_HOST] CC"));
      break;
    case USB_MIDI: 
      Serial.print(F("[USB_MIDI] CC"));
      break;
  }
#endif
  if (configuration.sys.soft_midi_thru == 1)
  {
#ifdef MIDI_DEVICE_USB
    if(midi_device != USB_MIDI) {
        usbMIDI.sendControlChange(inData1, inData2, inChannel);
  #ifdef DEBUG
        Serial.print(F(" THRU->MIDI_USB"));
  #endif
    }
#endif

#ifdef MIDI_DEVICE_DIN
    if(midi_device != MIDI_DIN) {
        midi_serial.sendControlChange(inData1, inData2, inChannel);
  #ifdef DEBUG
        Serial.print(F(" THRU->MIDI_DIN"));
  #endif
    }
#endif

#ifdef MIDI_DEVICE_USB_HOST
    if(midi_device != MIDI_USB_HOST) {
        midi_usb.sendControlChange(inData1, inData2, inChannel);
  #ifdef DEBUG
        Serial.print(F(" THRU->MIDI_USB_HOST"));
  #endif
    }
#endif
  }

#ifdef DEBUG
  Serial.println();
#endif
}

void handleAfterTouch_generic(byte inChannel, byte inPressure, byte midi_device)
{
  handleAfterTouch(inChannel, inPressure);
#ifdef DEBUG
  switch(midi_device) {
    case MIDI_DIN: 
      Serial.print(F("[MIDI_DIN] AT"));
      break;
    case MIDI_USB_HOST: 
      Serial.print(F("[MIDI_USB_HOST] AT"));
      break;
    case USB_MIDI: 
      Serial.print(F("[USB_MIDI] AT"));
      break;
  }
#endif
  if (configuration.sys.soft_midi_thru == 1)
  {
#ifdef MIDI_DEVICE_USB
    if(midi_device != USB_MIDI) {
        usbMIDI.sendAfterTouch(inPressure, inChannel);
  #ifdef DEBUG
        Serial.print(F(" THRU->MIDI_USB"));
  #endif
    }
#endif

#ifdef MIDI_DEVICE_DIN
    if(midi_device != MIDI_DIN) {
        midi_serial.sendAfterTouch(inPressure, inChannel);
  #ifdef DEBUG
        Serial.print(F(" THRU->MIDI_DIN"));
  #endif
    }
#endif

#ifdef MIDI_DEVICE_USB_HOST
    if(midi_device != MIDI_USB_HOST) {
        midi_usb.sendAfterTouch(inPressure, inChannel);
  #ifdef DEBUG
        Serial.print(F(" THRU->MIDI_USB_HOST"));
  #endif
    }
#endif
  }

#ifdef DEBUG
  Serial.println();
#endif
}

void handlePitchBend_generic(byte inChannel, byte inPitch, byte midi_device)
{
  handlePitchBend(inChannel, inPitch);
#ifdef DEBUG
  switch(midi_device) {
    case MIDI_DIN: 
      Serial.print(F("[MIDI_DIN] PB"));
      break;
    case MIDI_USB_HOST: 
      Serial.print(F("[MIDI_USB_HOST] PB"));
      break;
    case USB_MIDI: 
      Serial.print(F("[USB_MIDI] PB"));
      break;
  }
#endif
  if (configuration.sys.soft_midi_thru == 1)
  {
#ifdef MIDI_DEVICE_USB
    if(midi_device != USB_MIDI) {
        usbMIDI.sendPitchBend(inPitch, inChannel);
  #ifdef DEBUG
        Serial.print(F(" THRU->MIDI_USB"));
  #endif
    }
#endif

#ifdef MIDI_DEVICE_DIN
    if(midi_device != MIDI_DIN) {
        midi_serial.sendPitchBend(inPitch, inChannel);
  #ifdef DEBUG
        Serial.print(F(" THRU->MIDI_DIN"));
  #endif
    }
#endif

#ifdef MIDI_DEVICE_USB_HOST
    if(midi_device != MIDI_USB_HOST) {
        midi_usb.sendPitchBend(inPitch, inChannel);
  #ifdef DEBUG
        Serial.print(F(" THRU->MIDI_USB_HOST"));
  #endif
    }
#endif
  }

#ifdef DEBUG
  Serial.println();
#endif
}

void handleProgramChange_generic(byte inChannel, byte inProgram, byte midi_device)
{
  handleProgramChange(inChannel, inProgram);
#ifdef DEBUG
  switch(midi_device) {
    case MIDI_DIN: 
      Serial.print(F("[MIDI_DIN] PC"));
      break;
    case MIDI_USB_HOST: 
      Serial.print(F("[MIDI_USB_HOST] PC"));
      break;
    case USB_MIDI: 
      Serial.print(F("[USB_MIDI] PC"));
      break;
  }
#endif
  if (configuration.sys.soft_midi_thru == 1)
  {
#ifdef MIDI_DEVICE_USB
    if(midi_device != USB_MIDI) {
        usbMIDI.sendProgramChange(inProgram, inChannel);
  #ifdef DEBUG
        Serial.print(F(" THRU->MIDI_USB"));
  #endif
    }
#endif

#ifdef MIDI_DEVICE_DIN
    if(midi_device != MIDI_DIN) {
        midi_serial.sendProgramChange(inProgram, inChannel);
  #ifdef DEBUG
        Serial.print(F(" THRU->MIDI_DIN"));
  #endif
    }
#endif

#ifdef MIDI_DEVICE_USB_HOST
    if(midi_device != MIDI_USB_HOST) {
        midi_usb.sendProgramChange(inProgram, inChannel);
  #ifdef DEBUG
        Serial.print(F(" THRU->MIDI_USB_HOST"));
  #endif
    }
#endif
  }

#ifdef DEBUG
  Serial.println();
#endif
}

void handleSystemExclusive_generic(byte * data, uint len, byte midi_device) {
  handleSystemExclusive(data, len);
#ifdef DEBUG
  switch(midi_device) {
    case MIDI_DIN: 
      Serial.print(F("[MIDI_DIN] SysEx"));
      break;
    case MIDI_USB_HOST: 
      Serial.print(F("[MIDI_USB_HOST] SysEx"));
      break;
    case USB_MIDI: 
      Serial.print(F("[USB_MIDI] SysEx"));
      break;
  }
#endif
  if (configuration.sys.soft_midi_thru == 1)
  {
#ifdef MIDI_DEVICE_USB
    if(midi_device != USB_MIDI) {
        usbMIDI.sendSysEx(len, data);
  #ifdef DEBUG
        Serial.print(F(" THRU->MIDI_USB"));
  #endif
    }
#endif

#ifdef MIDI_DEVICE_DIN
    if(midi_device != MIDI_DIN) {
        midi_serial.sendSysEx(len, data);
  #ifdef DEBUG
        Serial.print(F(" THRU->MIDI_DIN"));
  #endif
    }
#endif

#ifdef MIDI_DEVICE_USB_HOST
    if(midi_device != MIDI_USB_HOST) {
        midi_usb.sendSysEx(len, data);
  #ifdef DEBUG
        Serial.print(F(" THRU->MIDI_USB_HOST"));
  #endif
    }
#endif
  }

#ifdef DEBUG
  Serial.println();
#endif
}

///* void handleSystemExclusiveChunk_MIDI_DEVICE_DIN(byte *data, uint len, bool last)

void handleTimeCodeQuarterFrame_generic(byte data, byte midi_device) {
  handleTimeCodeQuarterFrame(data);
#ifdef DEBUG
  switch(midi_device) {
    case MIDI_DIN: 
      Serial.print(F("[MIDI_DIN] TimeCodeQuarterFrame"));
      break;
    case MIDI_USB_HOST: 
      Serial.print(F("[MIDI_USB_HOST] TimeCodeQuarterFrame"));
      break;
    case USB_MIDI: 
      Serial.print(F("[USB_MIDI] TimeCodeQuarterFrame"));
      break;
  }
#endif
  if (configuration.sys.soft_midi_thru == 1)
  {
#ifdef MIDI_DEVICE_USB
    if(midi_device != USB_MIDI) {
        usbMIDI.sendTimeCodeQuarterFrame(0xF1, data);
  #ifdef DEBUG
        Serial.print(F(" THRU->MIDI_USB"));
  #endif
    }
#endif

#ifdef MIDI_DEVICE_DIN
    if(midi_device != MIDI_DIN) {
        midi_serial.sendTimeCodeQuarterFrame(data);
  #ifdef DEBUG
        Serial.print(F(" THRU->MIDI_DIN"));
  #endif
    }
#endif

#ifdef MIDI_DEVICE_USB_HOST
    if(midi_device != MIDI_USB_HOST) {
        midi_usb.sendTimeCodeQuarterFrame(0xF1, data);
  #ifdef DEBUG
        Serial.print(F(" THRU->MIDI_USB_HOST"));
  #endif
    }
#endif
  }

#ifdef DEBUG
  Serial.println();
#endif
}

void handleAfterTouchPoly_generic(byte inChannel, byte inNumber, byte inVelocity, byte midi_device)
{
  handleAfterTouchPoly(inChannel, inNumber, inVelocity);
#ifdef DEBUG
  switch(midi_device) {
    case MIDI_DIN: 
      Serial.print(F("[MIDI_DIN] AT-Poly"));
      break;
    case MIDI_USB_HOST: 
      Serial.print(F("[MIDI_USB_HOST] AT-Poly"));
      break;
    case USB_MIDI: 
      Serial.print(F("[USB_MIDI] AT-Poly"));
      break;
  }
#endif
  if (configuration.sys.soft_midi_thru == 1)
  {
#ifdef MIDI_DEVICE_USB
    if(midi_device != USB_MIDI) {
        usbMIDI.sendAfterTouch(inNumber, inVelocity, inChannel);
  #ifdef DEBUG
        Serial.print(F(" THRU->MIDI_USB"));
  #endif
    }
#endif

#ifdef MIDI_DEVICE_DIN
    if(midi_device != MIDI_DIN) {
        midi_serial.sendAfterTouch(inNumber, inVelocity, inChannel);
  #ifdef DEBUG
        Serial.print(F(" THRU->MIDI_DIN"));
  #endif
    }
#endif

#ifdef MIDI_DEVICE_USB_HOST
    if(midi_device != MIDI_USB_HOST) {
        midi_usb.sendAfterTouch(inNumber, inVelocity, inChannel);
  #ifdef DEBUG
        Serial.print(F(" THRU->MIDI_USB_HOST"));
  #endif
    }
#endif
  }

#ifdef DEBUG
  Serial.println();
#endif
}

void handleSongSelect_generic(byte inSong, byte midi_device)
{
  handleSongSelect(inSong);
#ifdef DEBUG
  switch(midi_device) {
    case MIDI_DIN: 
      Serial.print(F("[MIDI_DIN] SongSelect"));
      break;
    case MIDI_USB_HOST: 
      Serial.print(F("[MIDI_USB_HOST] SongSelect"));
      break;
    case USB_MIDI: 
      Serial.print(F("[USB_MIDI] SongSelect"));
      break;
  }
#endif
  if (configuration.sys.soft_midi_thru == 1)
  {
#ifdef MIDI_DEVICE_USB
    if(midi_device != USB_MIDI) {
        usbMIDI.sendSongSelect(inSong);
  #ifdef DEBUG
        Serial.print(F(" THRU->MIDI_USB"));
  #endif
    }
#endif

#ifdef MIDI_DEVICE_DIN
    if(midi_device != MIDI_DIN) {
        midi_serial.sendSongSelect(inSong);
  #ifdef DEBUG
        Serial.print(F(" THRU->MIDI_DIN"));
  #endif
    }
#endif

#ifdef MIDI_DEVICE_USB_HOST
    if(midi_device != MIDI_USB_HOST) {
        midi_usb.sendSongSelect(inSong);
  #ifdef DEBUG
        Serial.print(F(" THRU->MIDI_USB_HOST"));
  #endif
    }
#endif
  }

#ifdef DEBUG
  Serial.println();
#endif
}

void handleTuneRequest_generic(byte midi_device) {
  handleTuneRequest();
#ifdef DEBUG
  switch(midi_device) {
    case MIDI_DIN: 
      Serial.print(F("[MIDI_DIN] TuneRequest"));
      break;
    case MIDI_USB_HOST: 
      Serial.print(F("[MIDI_USB_HOST] TuneRequest"));
      break;
    case USB_MIDI: 
      Serial.print(F("[USB_MIDI] TuneRequest"));
      break;
  }
#endif
  if (configuration.sys.soft_midi_thru == 1)
  {
#ifdef MIDI_DEVICE_USB
    if(midi_device != USB_MIDI) {
        usbMIDI.sendTuneRequest();
  #ifdef DEBUG
        Serial.print(F(" THRU->MIDI_USB"));
  #endif
    }
#endif

#ifdef MIDI_DEVICE_DIN
    if(midi_device != MIDI_DIN) {
        midi_serial.sendTuneRequest();
  #ifdef DEBUG
        Serial.print(F(" THRU->MIDI_DIN"));
  #endif
    }
#endif

#ifdef MIDI_DEVICE_USB_HOST
    if(midi_device != MIDI_USB_HOST) {
        midi_usb.sendTuneRequest();
  #ifdef DEBUG
        Serial.print(F(" THRU->MIDI_USB_HOST"));
  #endif
    }
#endif
  }

#ifdef DEBUG
  Serial.println();
#endif
}

void handleClock_generic(byte midi_device) {
  handleClock();
#ifdef DEBUG
  switch(midi_device) {
    case MIDI_DIN: 
      Serial.print(F("[MIDI_DIN] Clock"));
      break;
    case MIDI_USB_HOST: 
      Serial.print(F("[MIDI_USB_HOST] Clock"));
      break;
    case USB_MIDI: 
      Serial.print(F("[USB_MIDI] Clock"));
      break;
  }
#endif
  if (configuration.sys.soft_midi_thru == 1)
  {
#ifdef MIDI_DEVICE_USB
    if(midi_device != USB_MIDI) {
        usbMIDI.sendRealTime(midi::Clock);
  #ifdef DEBUG
        Serial.print(F(" THRU->MIDI_USB"));
  #endif
    }
#endif

#ifdef MIDI_DEVICE_DIN
    if(midi_device != MIDI_DIN) {
        midi_serial.sendRealTime(midi::Clock);
  #ifdef DEBUG
        Serial.print(F(" THRU->MIDI_DIN"));
  #endif
    }
#endif

#ifdef MIDI_DEVICE_USB_HOST
    if(midi_device != MIDI_USB_HOST) {
        midi_usb.sendRealTime(midi::Clock);
  #ifdef DEBUG
        Serial.print(F(" THRU->MIDI_USB_HOST"));
  #endif
    }
#endif
  }

#ifdef DEBUG
  Serial.println();
#endif
}

void handleStart_generic(byte midi_device) {
  handleStart();
#ifdef DEBUG
  switch(midi_device) {
    case MIDI_DIN: 
      Serial.print(F("[MIDI_DIN] Start"));
      break;
    case MIDI_USB_HOST: 
      Serial.print(F("[MIDI_USB_HOST] Start"));
      break;
    case USB_MIDI: 
      Serial.print(F("[USB_MIDI] Start"));
      break;
  }
#endif
  if (configuration.sys.soft_midi_thru == 1)
  {
#ifdef MIDI_DEVICE_USB
    if(midi_device != USB_MIDI) {
        usbMIDI.sendRealTime(midi::Start);
  #ifdef DEBUG
        Serial.print(F(" THRU->MIDI_USB"));
  #endif
    }
#endif

#ifdef MIDI_DEVICE_DIN
    if(midi_device != MIDI_DIN) {
        midi_serial.sendRealTime(midi::Start);
  #ifdef DEBUG
        Serial.print(F(" THRU->MIDI_DIN"));
  #endif
    }
#endif

#ifdef MIDI_DEVICE_USB_HOST
    if(midi_device != MIDI_USB_HOST) {
        midi_usb.sendRealTime(midi::Start);
  #ifdef DEBUG
        Serial.print(F(" THRU->MIDI_USB_HOST"));
  #endif
    }
#endif
  }

#ifdef DEBUG
  Serial.println();
#endif
}

void handleContinue_generic(byte midi_device) {
  handleContinue();
#ifdef DEBUG
  switch(midi_device) {
    case MIDI_DIN: 
      Serial.print(F("[MIDI_DIN] Continue"));
      break;
    case MIDI_USB_HOST: 
      Serial.print(F("[MIDI_USB_HOST] Continue"));
      break;
    case USB_MIDI: 
      Serial.print(F("[USB_MIDI] Continue"));
      break;
  }
#endif
  if (configuration.sys.soft_midi_thru == 1)
  {
#ifdef MIDI_DEVICE_USB
    if(midi_device != USB_MIDI) {
        usbMIDI.sendRealTime(midi::Continue);
  #ifdef DEBUG
        Serial.print(F(" THRU->MIDI_USB"));
  #endif
    }
#endif

#ifdef MIDI_DEVICE_DIN
    if(midi_device != MIDI_DIN) {
        midi_serial.sendRealTime(midi::Continue);
  #ifdef DEBUG
        Serial.print(F(" THRU->MIDI_DIN"));
  #endif
    }
#endif

#ifdef MIDI_DEVICE_USB_HOST
    if(midi_device != MIDI_USB_HOST) {
        midi_usb.sendRealTime(midi::Continue);
  #ifdef DEBUG
        Serial.print(F(" THRU->MIDI_USB_HOST"));
  #endif
    }
#endif
  }

#ifdef DEBUG
  Serial.println();
#endif
}

void handleStop_generic(byte midi_device) {
  handleStop();
#ifdef DEBUG
  switch(midi_device) {
    case MIDI_DIN: 
      Serial.print(F("[MIDI_DIN] Stop"));
      break;
    case MIDI_USB_HOST: 
      Serial.print(F("[MIDI_USB_HOST] Stop"));
      break;
    case USB_MIDI: 
      Serial.print(F("[USB_MIDI] Stop"));
      break;
  }
#endif
  if (configuration.sys.soft_midi_thru == 1)
  {
#ifdef MIDI_DEVICE_USB
    if(midi_device != USB_MIDI) {
        usbMIDI.sendRealTime(midi::Stop);
  #ifdef DEBUG
        Serial.print(F(" THRU->MIDI_USB"));
  #endif
    }
#endif

#ifdef MIDI_DEVICE_DIN
    if(midi_device != MIDI_DIN) {
        midi_serial.sendRealTime(midi::Stop);
  #ifdef DEBUG
        Serial.print(F(" THRU->MIDI_DIN"));
  #endif
    }
#endif

#ifdef MIDI_DEVICE_USB_HOST
    if(midi_device != MIDI_USB_HOST) {
        midi_usb.sendRealTime(midi::Stop);
  #ifdef DEBUG
        Serial.print(F(" THRU->MIDI_USB_HOST"));
  #endif
    }
#endif
  }

#ifdef DEBUG
  Serial.println();
#endif
}

void handleActiveSensing_generic(byte midi_device) {
  handleActiveSensing();
#ifdef DEBUG
  switch(midi_device) {
    case MIDI_DIN: 
      Serial.print(F("[MIDI_DIN] ActiveSensing"));
      break;
    case MIDI_USB_HOST: 
      Serial.print(F("[MIDI_USB_HOST] ActiveSensing"));
      break;
    case USB_MIDI: 
      Serial.print(F("[USB_MIDI] ActiveSensing"));
      break;
  }
#endif
  if (configuration.sys.soft_midi_thru == 1)
  {
#ifdef MIDI_DEVICE_USB
    if(midi_device != USB_MIDI) {
        usbMIDI.sendRealTime(midi::ActiveSensing);
  #ifdef DEBUG
        Serial.print(F(" THRU->MIDI_USB"));
  #endif
    }
#endif

#ifdef MIDI_DEVICE_DIN
    if(midi_device != MIDI_DIN) {
        midi_serial.sendRealTime(midi::ActiveSensing);
  #ifdef DEBUG
        Serial.print(F(" THRU->MIDI_DIN"));
  #endif
    }
#endif

#ifdef MIDI_DEVICE_USB_HOST
    if(midi_device != MIDI_USB_HOST) {
        midi_usb.sendRealTime(midi::ActiveSensing);
  #ifdef DEBUG
        Serial.print(F(" THRU->MIDI_USB_HOST"));
  #endif
    }
#endif
  }

#ifdef DEBUG
  Serial.println();
#endif
}

void handleSystemReset_generic(byte midi_device) {
  handleSystemReset();
#ifdef DEBUG
  switch(midi_device) {
    case MIDI_DIN: 
      Serial.print(F("[MIDI_DIN] SystemReset"));
      break;
    case MIDI_USB_HOST: 
      Serial.print(F("[MIDI_USB_HOST] SystemReset"));
      break;
    case USB_MIDI: 
      Serial.print(F("[USB_MIDI] SystemReset"));
      break;
  }
#endif
  if (configuration.sys.soft_midi_thru == 1)
  {
#ifdef MIDI_DEVICE_USB
    if(midi_device != USB_MIDI) {
        usbMIDI.sendRealTime(midi::SystemReset);
  #ifdef DEBUG
        Serial.print(F(" THRU->MIDI_USB"));
  #endif
    }
#endif

#ifdef MIDI_DEVICE_DIN
    if(midi_device != MIDI_DIN) {
        midi_serial.sendRealTime(midi::SystemReset);
  #ifdef DEBUG
        Serial.print(F(" THRU->MIDI_DIN"));
  #endif
    }
#endif

#ifdef MIDI_DEVICE_USB_HOST
    if(midi_device != MIDI_USB_HOST) {
        midi_usb.sendRealTime(midi::SystemReset);
  #ifdef DEBUG
        Serial.print(F(" THRU->MIDI_USB_HOST"));
  #endif
    }
#endif
  }

#ifdef DEBUG
  Serial.println();
#endif
}

// void handlRealTimeSysteme_generic(byte inRealTime, byte midi_device) {
//   handleRealTimeSystem();
// #ifdef DEBUG
//   switch(midi_device) {
//     case MIDI_DIN: 
//       Serial.print(F("[MIDI_DIN] RealTimeSystem"));
//       break;
//     case MIDI_USB_HOST: 
//       Serial.print(F("[MIDI_USB_HOST] RealTimeSystem"));
//       break;
//     case USB_MIDI: 
//       Serial.print(F("[USB_MIDI] RealTimeSystem"));
//       break;
//   }
// #endif
//   if (configuration.sys.soft_midi_thru == 1)
//   {
// #ifdef MIDI_DEVICE_USB
//     if(midi_device != USB_MIDI) {
//         usbMIDI.sendRealTime(inRealTime);
//   #ifdef DEBUG
//         Serial.print(F(" THRU->MIDI_USB"));
//   #endif
//     }
// #endif

// #ifdef MIDI_DEVICE_DIN
//     if(midi_device != MIDI_DIN) {
//       midi_serial.sendRealTime((midi::MidiType)inRealTime);
//   #ifdef DEBUG
//         Serial.print(F(" THRU->MIDI_DIN"));
//   #endif
//     }
// #endif

// #ifdef MIDI_DEVICE_USB_HOST
//     if(midi_device != MIDI_USB_HOST) {
//         midi_usb.sendRealTime(inRealTime);
//   #ifdef DEBUG
//         Serial.print(F(" THRU->MIDI_USB_HOST"));
//   #endif
//     }
// #endif
//   }

// #ifdef DEBUG
//   Serial.println();
// #endif
// }


/*****************************************
   MIDI_DEVICE_DIN
 *****************************************/
#ifdef MIDI_DEVICE_DIN

void handleNoteOn_MIDI_DEVICE_DIN(byte inChannel, byte inNumber, byte inVelocity)
{
  handleNoteOn_generic(inChannel, inNumber, inVelocity, MIDI_DIN);
}

void handleNoteOff_MIDI_DEVICE_DIN(byte inChannel, byte inNumber, byte inVelocity)
{
  handleNoteOff_generic(inChannel, inNumber, inVelocity, MIDI_DIN);
}

void handleControlChange_MIDI_DEVICE_DIN(byte inChannel, byte inData1, byte inData2)
{
  handleControlChange_generic(inChannel, inData1, inData2, MIDI_DIN);
}

void handleAfterTouch_MIDI_DEVICE_DIN(byte inChannel, byte inPressure)
{
  handleAfterTouch_generic(inChannel, inPressure, MIDI_DIN);
}

void handlePitchBend_MIDI_DEVICE_DIN(byte inChannel, int inPitch)
{
  handlePitchBend_generic(inChannel, inPitch, MIDI_DIN);
}

void handleProgramChange_MIDI_DEVICE_DIN(byte inChannel, byte inProgram)
{
  handleProgramChange_generic(inChannel, inProgram, MIDI_DIN);
}

void handleSystemExclusive_MIDI_DEVICE_DIN(byte * data, uint len)
{
  handleSystemExclusive_generic(data, len, MIDI_DIN);
}

/* void handleSystemExclusiveChunk_MIDI_DEVICE_DIN(byte *data, uint len, bool last)
  {
  handleSystemExclusiveChunk(data, len, last);
  #ifdef DEBUG
  Serial.print(F("[MIDI_DIN] SysExChunk"));
  #endif
    if (configuration.sys.soft_midi_thru == 1)
  {
  #ifdef MIDI_DEVICE_USB_HOST
  midi_usb.sendSysEx(len, data, last);
  #ifdef DEBUG
  Serial.print(F(" THRU->MIDI_USB_HOST"));
  #endif
  #endif
  #ifdef MIDI_DEVICE_USB
  usbMIDI.sendSysEx(len, data, last);
  #ifdef DEBUG
  Serial.print(F(" THRU->MIDI_USB"));
  #endif
  #endif
  }
  #ifdef DEBUG
  Serial.println();
  #endif
  } */

void handleTimeCodeQuarterFrame_MIDI_DEVICE_DIN(byte data)
{
  handleTimeCodeQuarterFrame_generic(data, MIDI_DIN);
}

void handleAfterTouchPoly_MIDI_DEVICE_DIN(byte inChannel, byte inNumber, byte inVelocity)
{
  handleAfterTouchPoly_generic(inChannel, inNumber, inVelocity, MIDI_DIN);
}

void handleSongSelect_MIDI_DEVICE_DIN(byte inSong)
{
  handleSongSelect_generic(inSong, MIDI_DIN);
}

void handleTuneRequest_MIDI_DEVICE_DIN(void)
{
  handleTuneRequest_generic(MIDI_DIN);
}

void handleClock_MIDI_DEVICE_DIN(void)
{
  handleClock_generic(MIDI_DIN);
}

void handleStart_MIDI_DEVICE_DIN(void)
{
  handleStart_generic(MIDI_DIN);
}

void handleContinue_MIDI_DEVICE_DIN(void)
{
  handleContinue_generic(MIDI_DIN);
}

void handleStop_MIDI_DEVICE_DIN(void)
{
  handleStop_generic(MIDI_DIN);
}

void handleActiveSensing_MIDI_DEVICE_DIN(void)
{
  handleActiveSensing_generic(MIDI_DIN);
}

void handleSystemReset_MIDI_DEVICE_DIN(void)
{
  handleSystemReset_generic(MIDI_DIN);
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
void handleNoteOn_MIDI_DEVICE_USB_HOST(byte inChannel, byte inNumber, byte inVelocity)
{
  handleNoteOn_generic(inChannel, inNumber, inVelocity, MIDI_USB_HOST);
}

void handleNoteOff_MIDI_DEVICE_USB_HOST(byte inChannel, byte inNumber, byte inVelocity)
{
  handleNoteOff_generic(inChannel, inNumber, inVelocity, MIDI_USB_HOST);
}

void handleControlChange_MIDI_DEVICE_USB_HOST(byte inChannel, byte inData1, byte inData2)
{
  handleControlChange_generic(inChannel, inData1, inData2, MIDI_USB_HOST);
}

void handleAfterTouch_MIDI_DEVICE_USB_HOST(byte inChannel, byte inPressure)
{
  handleAfterTouch_generic(inChannel, inPressure, MIDI_USB_HOST);
}

void handlePitchBend_MIDI_DEVICE_USB_HOST(byte inChannel, int inPitch)
{
  handlePitchBend_generic(inChannel, inPitch, MIDI_USB_HOST);
}

void handleProgramChange_MIDI_DEVICE_USB_HOST(byte inChannel, byte inProgram)
{
  handleProgramChange_generic(inChannel, inProgram, MIDI_USB_HOST);
}

void handleSystemExclusive_MIDI_DEVICE_USB_HOST(byte * data, uint len)
{
  handleSystemExclusive_generic(data, len, MIDI_USB_HOST);
}

/* void handleSystemExclusiveChunk_MIDI_DEVICE_USB_HOST(byte *data, uint len, bool last)
  {
  handleSystemExclusiveChunk(data, len, last);
  #ifdef DEBUG
  Serial.print(F("[MIDI_USB_HOST] SysExChunk"));
  #endif
    if (configuration.sys.soft_midi_thru == 1)
  {
  #ifdef MIDI_DEVICE_DIN
  midi_serial.sendSysEx(len, data, last);
  #ifdef DEBUG
  Serial.print(F(" THRU->MIDI_DIN"));
  #endif
  #endif
  #ifdef MIDI_DEVICE_USB
  usbMIDI.sendSysEx(len, data, last);
  #ifdef DEBUG
  Serial.print(F(" THRU->MIDI_USB"));
  #endif
  #endif
  }
  #ifdef DEBUG
  Serial.println();
  #endif
  } */

void handleTimeCodeQuarterFrame_MIDI_DEVICE_USB_HOST(midi::DataByte data)
{
  handleTimeCodeQuarterFrame_generic(data, MIDI_USB_HOST);
}

void handleAfterTouchPoly_MIDI_DEVICE_USB_HOST(byte inChannel, byte inNumber, byte inVelocity)
{
  handleAfterTouchPoly_generic(inChannel, inNumber, inVelocity, MIDI_USB_HOST);
}

void handleSongSelect_MIDI_DEVICE_USB_HOST(byte inSong)
{
  handleSongSelect_generic(inSong, MIDI_USB_HOST);
}

void handleTuneRequest_MIDI_DEVICE_USB_HOST(void)
{
  handleTuneRequest_generic(MIDI_USB_HOST);
}

void handleClock_MIDI_DEVICE_USB_HOST(void)
{
  handleClock_generic(MIDI_USB_HOST);
}

void handleStart_MIDI_DEVICE_USB_HOST(void)
{
  handleStart_generic(MIDI_USB_HOST);
}

void handleContinue_MIDI_DEVICE_USB_HOST(void)
{
  handleContinue_generic(MIDI_USB_HOST);
}

void handleStop_MIDI_DEVICE_USB_HOST(void)
{
  handleStop_generic(MIDI_USB_HOST);
}

void handleActiveSensing_MIDI_DEVICE_USB_HOST(void)
{
  handleActiveSensing_generic(MIDI_USB_HOST);
}

void handleSystemReset_MIDI_DEVICE_USB_HOST(void)
{
  handleSystemReset_generic(MIDI_USB_HOST);
}

/* void handlRealTimeSysteme_MIDI_DEVICE_USB_HOST(midi::MidiType inRealTime)
  {
  handleRealTimeSystem_generic(inRealTime, MIDI_USB_HOST);
  } */
#endif // MIDI_DEVICE_USB_HOST

/*****************************************
   MIDI_DEVICE_USB
 *****************************************/
#ifdef MIDI_DEVICE_USB
void handleNoteOn_MIDI_DEVICE_USB(byte inChannel, byte inNumber, byte inVelocity)
{
  handleNoteOn_generic(inChannel, inNumber, inVelocity, USB_MIDI);
}

void handleNoteOff_MIDI_DEVICE_USB(byte inChannel, byte inNumber, byte inVelocity)
{
  handleNoteOff_generic(inChannel, inNumber, inVelocity, USB_MIDI);
}

void handleControlChange_MIDI_DEVICE_USB(byte inChannel, byte inData1, byte inData2)
{
  handleControlChange_generic(inChannel, inData1, inData2, USB_MIDI);
}

void handleAfterTouch_MIDI_DEVICE_USB(byte inChannel, byte inPressure)
{
  handleAfterTouch_generic(inChannel, inPressure, USB_MIDI);
}

void handlePitchBend_MIDI_DEVICE_USB(byte inChannel, int inPitch)
{
  handlePitchBend_generic(inChannel, inPitch, USB_MIDI);
}

void handleProgramChange_MIDI_DEVICE_USB(byte inChannel, byte inProgram)
{
  handleProgramChange_generic(inChannel, inProgram, USB_MIDI);
}

void handleSystemExclusive_MIDI_DEVICE_USB(byte * data, uint len)
{
  handleSystemExclusive_generic(data, len, USB_MIDI);
}

/* FLASHMEM void handleSystemExclusiveChunk_MIDI_DEVICE_USB(byte *data, uint len, bool last)
  {
  handleSystemExclusiveChunk(data, len, last);
  #ifdef DEBUG
  Serial.print(F("[MIDI_USB] SysExChunk"));
  #endif
    if (configuration.sys.soft_midi_thru == 1)
  {
  #ifdef MIDI_DEVICE_DIN
  midi_serial.sendSysEx(len, data, last);
  #ifdef DEBUG
  Serial.print(F(" THRU->MIDI_DIN"));
  #endif
  #endif
  #ifdef MIDI_DEVICE_USB_HOST
  midi_usb.sendSysEx(len, data, last);
  #ifdef DEBUG
  Serial.print(F(" THRU->MIDI_USB_HOST"));
  #endif
  #endif
  }
  #ifdef DEBUG
  Serial.println();
  #endif
  } */

void handleTimeCodeQuarterFrame_MIDI_DEVICE_USB(midi::DataByte data)
{
  handleTimeCodeQuarterFrame_generic(data, USB_MIDI);
}

void handleAfterTouchPoly_MIDI_DEVICE_USB(byte inChannel, byte inNumber, byte inVelocity)
{
  handleAfterTouchPoly_generic(inChannel, inNumber, inVelocity, USB_MIDI);
}

void handleSongSelect_MIDI_DEVICE_USB(byte inSong)
{
  handleSongSelect_generic(inSong, USB_MIDI);
}

void handleTuneRequest_MIDI_DEVICE_USB(void)
{
  handleTuneRequest_generic(USB_MIDI);
}

void handleClock_MIDI_DEVICE_USB(void)
{
  handleClock_generic(USB_MIDI);
}

void handleStart_MIDI_DEVICE_USB(void)
{
  handleStart_generic(USB_MIDI);
}

void handleContinue_MIDI_DEVICE_USB(void)
{
  handleContinue_generic(USB_MIDI);
}

void handleStop_MIDI_DEVICE_USB(void)
{
  handleStop_generic(USB_MIDI);
}

void handleActiveSensing_MIDI_DEVICE_USB(void)
{
  handleActiveSensing_generic(USB_MIDI);
}

void handleSystemReset_MIDI_DEVICE_USB(void)
{
  handleSystemReset_generic(USB_MIDI);
}

/* FLASHMEM void handleRealTimeSystem_MIDI_DEVICE_USB(byte inRealTime)
  {
  handleRealTimeSystem_generic(inRealTime, USB_MIDI);
  } */
#endif // MIDI_DEVICE_USB

FLASHMEM void MD_sendControlChange(uint8_t channel, uint8_t cc, uint8_t value)
{
#ifdef DEBUG
  Serial.print(F("[MD] SendControlChange CH:"));
  Serial.print(channel, DEC);
  Serial.print(F(" CC:"));
  Serial.print(cc);
  Serial.print(F(" VAL:"));
  Serial.print(value);
#endif
#ifdef MIDI_DEVICE_DIN
  midi_serial.sendControlChange(cc, value, channel);
#ifdef DEBUG
  Serial.print(F(" MIDI-DIN"));
#endif
#endif
#ifdef MIDI_DEVICE_USB_HOST
  midi_usb.sendControlChange(cc, value, channel);
#ifdef DEBUG
  Serial.print(F(" MIDI-USB-HOST"));
#endif
#endif
#ifdef MIDI_DEVICE_USB
  usbMIDI.sendControlChange(cc, value, channel);
#ifdef DEBUG
  Serial.print(F(" MIDI-USB"));
#endif
#endif
#ifdef DEBUG
  Serial.println();
#endif
}

/*****************************************
   HELPER FUNCTIONS
 *****************************************/
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
  //midi_serial.setHandleSystemExclusiveChunk(handleSystemExclusiveChunk_MIDI_DEVICE_DIN);
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
  //midi_serial.setHandleRealTimeSystem(handleRealTimeSystem_MIDI_DEVICE_DIN);
#ifdef DEBUG
  Serial.println(F("MIDI_DEVICE_DIN enabled"));
#endif
#endif

  // start up USB host
#ifdef MIDI_DEVICE_USB_HOST
  usb_host.begin();
  midi_usb.setHandleNoteOn(handleNoteOn_MIDI_DEVICE_USB_HOST);
  midi_usb.setHandleNoteOff(handleNoteOff_MIDI_DEVICE_USB_HOST);
  midi_usb.setHandleControlChange(handleControlChange_MIDI_DEVICE_USB_HOST);
  midi_usb.setHandleAfterTouchChannel(handleAfterTouch_MIDI_DEVICE_USB_HOST);
  midi_usb.setHandlePitchChange(handlePitchBend_MIDI_DEVICE_USB_HOST);
  midi_usb.setHandleProgramChange(handleProgramChange_MIDI_DEVICE_USB_HOST);
  midi_usb.setHandleSystemExclusive(handleSystemExclusive_MIDI_DEVICE_USB_HOST);
  //midi_usb.setHandleSystemExclusiveChunk(handleSystemExclusiveChunk_MIDI_DEVICE_USB_HOST);
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
  //midi_usb.setHandleRealTimeSystem(handleRealTimeSystem_MIDI_DEVICE_USB_HOST);
#ifdef DEBUG
  Serial.println(F("MIDI_DEVICE_USB_HOST enabled."));
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
  //usbMIDI.setHandleSystemExclusiveChunk(handleSystemExclusiveChunk_MIDI_DEVICE_USB);
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
  //usbMIDI.setHandleRealTimeSystem(handleRealTimeSystem_MIDI_DEVICE_USB);
#ifdef DEBUG
  Serial.println(F("MIDI_DEVICE_USB enabled."));
#endif
#endif
}

FLASHMEM void check_midi_devices(void)
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
  //vd[0] = 0xF0; // SysEx start
  vd[0] = 0x43; // ID=Yamaha
  vd[1] = midi_channel; // Sub-status and MIDI channel
  vd[2] = 0x00; // Format number (0=1 voice)
  vd[3] = 0x01; // Byte count MSB
  vd[4] = 0x1B; // Byte count LSB
  for (uint8_t n = 0; n < 155; n++)
  {
    checksum -= data[n];
    vd[5 + n] = data[n];
  }
  vd[160] = checksum & 0x7f; // Checksum
  //vd[162] = 0xF7; // SysEx end

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
  usbMIDI.sendSysEx(2048, bank_data+2048, true);
  delay(50);
  usbMIDI.sendSysEx(8, bank_data+4096, true);
#endif
#ifdef MIDI_DEVICE_USB_HOST
  midi_usb.sendSysEx(4104, bank_data); // Send to USB-HOST MIDI
#endif
}

FLASHMEM void send_sysex_param(uint8_t midi_channel, uint8_t var, uint8_t val, uint8_t param_group)
{
  uint8_t s[5];

  s[0] = 0x43; // ID=Yamaha
  s[1] = midi_channel; // Sub-status and MIDI channel
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
