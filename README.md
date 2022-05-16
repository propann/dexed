# MicroDexed-touch

## This is a development fork of MicroDexed with TFT Touch Display, "nextgen" UI and advanced sequencer

### This fork requires a Teensy 4.1,  Teensy Audio Shield, 320x240 ILI9341 SPI Display with XPT2046 Touchscreen and a serial Flash Chip.

Compared to the previous Version, there are a lot of new features/improvments:

* "Large" Graphical Interface, view and edit multiple Parameters in one Screen

* Map MIDI CC to external MIDI Devices to control Input/Parameters

* 2 new "Virtual Analog" Synth Instances -  Monosynths with one OSC, PWM, Filter and Noise. A lot of useful new sound possibilities. This module is called "MicroSynth"

* Much improved Sequencer with 8 Tracks, 16 Step Pattern Chains and up to 16*64 Song Steps, Arpeggiator with 1/8 - 1/64 Steps, Arp Patterns, Pitched Drum- and Instrument Sample Support

* Mixer View with all Channels

* Sample Management from SD-CARD and SPI FLASH

* Tracker View (Editor) - work in progress

* Track Print/Recording - work in progress

YouTube playlist about the development:
https://www.youtube.com/playlist?list=PLHTypoMU1QoGOXPli8bjR6MknPiQpubHl

## Development Status:

### Hardware:
https://codeberg.org/positionhigh/MicroDexed-touch/wiki/Current-State:-Hardware-%2F-PCB

### Software (feature progress status):
https://codeberg.org/positionhigh/MicroDexed-touch/wiki/Current-State:-Software

### Build Instructions (including 3d case files):
https://codeberg.org/positionhigh/MicroDexed-touch/wiki/Build-Instructions

### Build of Materials (BOM)
https://codeberg.org/positionhigh/MicroDexed-touch/wiki/current-BOM-%28Bill-of-Materials%29-for-MicroDexed-touch

### Changelog:
https://codeberg.org/positionhigh/MicroDexed-touch/wiki/Changelog

## Installation/Compile Instructions

* Your Flash Chip should be soldered on the TEENSY AUDIO BOARD, not on the Teensy 4.1 backside. The SerialFlash Library will not work with a Chip on the Teensy. For development, currently in use is a:
WINBOND W25Q128FVSG SERIAL FLASH MEMORY 3V 128M-BIT

* Copy Libraries from /third-party to you Arduino / Teensy Library Directory (On Mac: usualy in your Home Directory/Documents/Arduino/libraries), if not already existing there.

* Copy all Files/Directories from /addon/SD/ to the root of your SD Card (FAT32) and insert the card in SD Slot of the Teensy 4.1

* Open MicroDexed-touch.ino with Teensyduino

* Set Tools -> Optimize to "smallest code"

* Set USB Type to "Serial + MIDI + Audio"

* Compile and Upload to your Teensy 4.1

* Power on the Teensy and attach USB MIDI Keyboard ( or use the virtual Touch Keyboard and the Encoders to navigate)

## USB MIDI
https://codeberg.org/positionhigh/MicroDexed-touch/wiki/DIY-USB-MIDI-HOST

## License

This is a port of the original Dexed/msfa engine (see https://github.com/asb2m10/dexed and https://github.com/google/music-synthesizer-for-android) to be used on a ~~Teensy-3.6 or~~ Teensy-4.1.

MicroDexed is licensed on the GPL v3. The msfa component (acronym for music synthesizer for android, see https://github.com/google/music-synthesizer-for-android) stays on the Apache 2.0 license to able to collaborate between projects.

## Manuals:

https://codeberg.org/positionhigh/MicroDexed-touch/wiki/?action=_pages

## Buttons / Encoders

A quick guide how the push Encoders are working:

Usualy [SHORT PUSH] Encoder Left brings you back to the main menu or closer to the main menu one step.

Usualy [SHORT PUSH] Encoder Right selects or confirms an input/menu item/goes deeper into this item.

In most menu settings of dexed, [SHORT PUSH] Encoder Right, switches between the 2 dexed Instances.

In microsynth, [LONG PUSH] Encoder Right, switches between the 2 microsynth Instances.

in Voice select (dexed) : [SHORT PUSH] Encoder Right switches Instances, [LONG PUSH] Encoder Right switches betweens Banks and Sounds. [LONG PUSH] Encoder Left selects/unselects Sounds to the Favorites list.

In Menus that need 2 Encoders, Encoder Right controls Y movement and Encoder Left controls X movement
(for example in Song mode)

## PIN LAYOUT & DISPLAY WIRING

https://codeberg.org/positionhigh/MicroDexed-touch/wiki/Wiring-diagram-for-Display

## CONTRIBUTING

This project lives from the contributions of C++ developers, testers, reviewers. Please check https://codeberg.org/positionhigh/MicroDexed-touch/issues to help in open topics or add your own Issue or Feature Request.

## SURVEY / POLL: Let us know what you want to see
https://www.supersurvey.com/poll4335406x48364DD1-136

## CURRENT Screenshots:

<img src="https://codeberg.org/positionhigh/MicroDexed-touch/raw/branch/main/addon/microDexedRemoteConsole/Screenshots/microdexed-002771.png" >
<img src="https://codeberg.org/positionhigh/MicroDexed-touch/raw/branch/main/addon/microDexedRemoteConsole/Screenshots/microdexed-002496.png" >
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


