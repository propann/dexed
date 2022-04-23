# MicroDexed-touch

## This is a development fork of MicroDexed with TFT Touch Display, "nextgen" UI and advanced sequencer

### This fork requires a Teensy 4.1,  Teensy Audio Shield, 320x240 ILI9341 SPI Display with XPT2046 Touchscreen and a serial Flash Chip.

A example Link to the Screen below 10 Euro:

https://www.amazon.de/ILI9341-Display-Seriell-Arduino-Raspberry/dp/B07YTWRZGR/ 

The Display is smaller than the first "nextgen" version but now is much faster - may it be the screen itself or most probably because of the much more optimized display library.

The Teensy supports several Serial Flash Chips, for development currently in use is a:

WINBOND W25Q128FVSG SERIAL FLASH MEMORY 3V 128M-BIT

It must be soldered to the Teensy AUDIO BOARD, not on the backside of the Teensy, to be able to work with the SerialFlash Library.

Compared to the previous Version, there are a lot of new features/improvments:

* "Large" Graphical Interface, view and edit multiple Parameters in one Screen

* Map MIDI CC to external MIDI Devices to control Input/Parameters

* 2 new "Virtual Analog" Synth Instances -  Monosynths with one OSC, PWM, Filter and Noise. A lot of useful new sound possibilities. This module is called "MicroSynth"

* Much improved Sequencer with 8 Tracks, 16 Step Pattern Chains and up to 16*64 Song Steps, Arpeggiator with 1/8 - 1/64 Steps, Arp Patterns, Pitched Drum- and Instrument Sample Support

* Mixer View with all Channels

* Sample Management from SD-CARD and SPI FLASH

YouTube playlist about the development:
https://www.youtube.com/playlist?list=PLHTypoMU1QoGOXPli8bjR6MknPiQpubHl

Development Status:

| Feature | Status |   
|:-----------|:-----------:| 
| Pattern Editor | 90% |
| Step/Grid Editor | 80% |
| Song Mode| 85% |
| MicroSynth | 80% |
| EPiano GUI | 95% |
| General UI Merge | 75% |
| SysEx/MIDI Controls | 70% |
| Main Mixer View | 60% |
| Mute Matrix | 10% |
| Piano Roll Editor | 80% |
| File Manager (SD/Flash) | 60% |
| MultiSample Player | 10% |
| Virtual Keyboard | 80% |
| Custom/User Samples | 0% |
| Sample Editor | 0% |

## Installation/Compile Instructions

* Your Flash Chip should be soldered on the TEENSY AUDIO BOARD, not on the Teensy 4.1 itself

* Copy Libraries from /third-party to you Arduino / Teensy Library Directory (On Mac: usualy in your Home Directory/Documents/Arduino/libraries), if not already existing there.

* Copy all Files/Directories from /addon/SD/ to the root of your SD Card (FAT32) and insert the card in SD Slot of the Teensy 4.1

* Open MicroDexed-touch.ino with Teensyduino

* Set Tools -> Optimize to "smallest code"

* Set USB Type to "Serial + MIDI + Audio"

* Compile and Upload to your Teensy 4.1

* Power on the Teensy and attach USB MIDI Keyboard ( or use the virtual Touch Keyboard and the Encoders to navigate)


## License

This is a port of the original Dexed/msfa engine (see https://github.com/asb2m10/dexed and https://github.com/google/music-synthesizer-for-android) to be used on a ~~Teensy-3.6 or~~ Teensy-4.1.

MicroDexed is licensed on the GPL v3. The msfa component (acronym for music synthesizer for android, see https://github.com/google/music-synthesizer-for-android) stays on the Apache 2.0 license to able to collaborate between projects.

## Manuals:

Links for building a TMA board and/or MicroDexed, a small installation guide for Arduino-IDE and a user manual are located here: https://codeberg.org/dcoredump/MicroDexed/wiki/_pages


## PIN LAYOUT

<img src="https://codeberg.org/positionhigh/MicroDexed-touch/raw/branch/main/doc/pin_layout_t41.png" >

## CURRENT Screenshots:

<img src="https://codeberg.org/positionhigh/MicroDexed-touch/raw/branch/main/addon/microDexedRemoteConsole/Screenshots/microdexed-004831.png" >
<img src="https://codeberg.org/positionhigh/MicroDexed-touch/raw/branch/main/addon/microDexedRemoteConsole/Screenshots/microdexed-004427.png" >
<img src="https://codeberg.org/positionhigh/MicroDexed-touch/raw/branch/main/addon/microDexedRemoteConsole/Screenshots/microdexed-003657.png" >
<img src="https://codeberg.org/positionhigh/MicroDexed-touch/raw/branch/main/addon/microDexedRemoteConsole/Screenshots/microdexed-000674.png" >
<img src="https://codeberg.org/positionhigh/MicroDexed-touch/raw/branch/main/addon/microDexedRemoteConsole/Screenshots/microdexed-008906.png" >
<img src="https://codeberg.org/positionhigh/MicroDexed-touch/raw/branch/main/addon/microDexedRemoteConsole/Screenshots/microdexed-008295.png" >
<img src="https://codeberg.org/positionhigh/MicroDexed-touch/raw/branch/main/addon/microDexedRemoteConsole/Screenshots/microdexed-007285.png" >
<img src="https://codeberg.org/positionhigh/MicroDexed-touch/raw/branch/main/addon/microDexedRemoteConsole/Screenshots/microdexed-006607.png" >
<img src="https://codeberg.org/positionhigh/MicroDexed-touch/raw/branch/main/addon/microDexedRemoteConsole/Screenshots/microdexed-006217.png" >
<img src="https://codeberg.org/positionhigh/MicroDexed-touch/raw/branch/main/addon/microDexedRemoteConsole/Screenshots/microdexed-005944.png" >
<img src="https://codeberg.org/positionhigh/MicroDexed-touch/raw/branch/main/addon/microDexedRemoteConsole/Screenshots/microdexed-004923.png" >
<img src="https://codeberg.org/positionhigh/MicroDexed-touch/raw/branch/main/addon/microDexedRemoteConsole/Screenshots/microdexed-004020.png" >
<img src="https://codeberg.org/positionhigh/MicroDexed-touch/raw/branch/main/addon/microDexedRemoteConsole/Screenshots/microdexed-003602.png" >
<img src="https://codeberg.org/positionhigh/MicroDexed-touch/raw/branch/main/addon/microDexedRemoteConsole/Screenshots/microdexed-002562.png" >
<img src="https://codeberg.org/positionhigh/MicroDexed-touch/raw/branch/main/addon/microDexedRemoteConsole/Screenshots/microdexed-001897.png" >
<img src="https://codeberg.org/positionhigh/MicroDexed-touch/raw/branch/main/addon/microDexedRemoteConsole/Screenshots/microdexed-000728.png" >
<img src="https://codeberg.org/positionhigh/MicroDexed-touch/raw/branch/main/addon/microDexedRemoteConsole/Screenshots/microdexed-004720.png" >



## OLD Screenshots:

<img src="https://codeberg.org/positionhigh/MicroDexed-touch/raw/branch/main/addon/microDexedRemoteConsole/Screenshots/microdexed-007090.png" >

<img src="https://codeberg.org/positionhigh/MicroDexed-touch/raw/branch/main/addon/microDexedRemoteConsole/Screenshots/microdexed-008759.png" >

<img src="https://codeberg.org/positionhigh/MicroDexed-touch/raw/branch/main/addon/microDexedRemoteConsole/Screenshots/microdexed-009681.png" >

<img src="https://codeberg.org/positionhigh/MicroDexed-touch/raw/branch/main/addon/microDexedRemoteConsole/Screenshots/microdexed-019427.png" >

<img src="https://codeberg.org/positionhigh/MicroDexed-touch/raw/branch/main/addon/microDexedRemoteConsole/Screenshots/microdexed-022644.png" >

