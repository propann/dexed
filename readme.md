# MicroDexed-touch

<br><p>
<img src="https://codeberg.org/positionhigh/MicroDexed-touch/raw/branch/main/doc/Build_InstructionsV2/final1.png" >
<br>

## This is the development fork of MicroDexed with TFT Touch Display, "nextgen" UI and advanced sequencer

### This build requires a Teensy 4.1,  PCM5102 Audio Board, 320x240 ILI9341 SPI Display with XPT2046 Touchscreen and a serial Flash Chip.

* The Flash Chip is presoldered on a small PCB attachment for <2$. You do not require to solder any SMD part by yourself anymore.
<p>

MicroDexed started as a Teensy based, 6-operator-FM-synthesizer. The first generation is still maintained at: https://codeberg.org/dcoredump/MicroDexed

Compared to the first generation, MicroDexed-touch offers a lot of new features/improvements:

* "Large" Graphical Interface, view and edit multiple Parameters in one Screen

* Map MIDI CC to external MIDI Devices to control Input/Parameters

* Loading, playing and editing multi-sampled instruments or layered instruments in a "live" mode or played by the sequencer. Auto detection sample zones but also can be edited manually

* 2 new "Virtual Analog" Synth Instances -  Monosynths with one OSC, PWM, Filter and Noise. A lot of useful new sound possibilities. This module is called "MicroSynth"

* Much improved Sequencer with 8 Tracks, 16 Step Patterns, 16 Step Pattern Chains, up to 16384 Song Steps, Arpeggiator with 1/8 - 1/64 Steps, Arp Patterns, Pitched Drum- and Instrument Sample Support

* The sequencer can also play chords stacked up to 7 notes, using only a single track

* Mixer View with all Channels

* Multiband Master Compressor with 4 bands

* Sample Management from SD-CARD and SPI FLASH, samples can be loaded/modified from SD-CARD to FLASH during runtime.

* Touch Mute Matrix for live / realtime performance

* Tracker View (Editor) - work in progress

* Track Print/Recording - work in progress

* Mutable Instruments Braids Oscillator - with up to 8 voice polyphony, added envelopes and filters, with everything of the core features, except the wavetables


### YouTube playlist about the development:
[https://www.youtube.com/playlist?list=PLHTypoMU1QoGOXPli8bjR6MknPiQpubHl
](https://www.youtube.com/playlist?list=PLHTypoMU1QoGOXPli8bjR6MknPiQpubHl)



### User Chat group

[https://signal.group/#CjQKID8_6_bax5W1WS2rcq7NokgSckC0kxh8uPdo20X_S4hKEhBnZ-9ZbTvMT2vacLZqn92N](https://signal.group/#CjQKID8_6_bax5W1WS2rcq7NokgSckC0kxh8uPdo20X_S4hKEhBnZ-9ZbTvMT2vacLZqn92N)

### PCB is available from PCBWAY

If you register and pay as a new user at PCBWAY with this Invite link, you should get $5 "New User Free Credit" - so you can order 5 PCB pieces for "free", except shipping and customs cost etc.
    
[https://www.pcbway.com/setinvite.aspx?inviteid=565384](https://www.pcbway.com/setinvite.aspx?inviteid=565384)

<a href="https://www.pcbway.com/project/shareproject/MicroDexed_Touch_current_version_with_PCM5102_d643a695.html"><img src="https://www.pcbway.com/project/img/images/frompcbway-1220.png" alt="PCB from PCBWay" /></a>

### upcoming features/work in progess:

<img src="https://codeberg.org/positionhigh/MicroDexed-touch/raw/branch/main/addon/microDexedRemoteConsole/Screenshots/multiband.png" >
<br>
Multiband Compressor/Waveshaper
<p>
<img src="https://codeberg.org/positionhigh/MicroDexed-touch/raw/branch/main/addon/microDexedRemoteConsole/Screenshots/multi_sampler.png" >
<br>
Enhancements to Multisample Playback
<p>
<img src="https://codeberg.org/positionhigh/MicroDexed-touch/raw/branch/main/addon/microDexedRemoteConsole/Screenshots/sample_editor.png" >
<br>
Sample Editor
<p>
<img src="https://codeberg.org/positionhigh/MicroDexed-touch/raw/branch/main/addon/microDexedRemoteConsole/Screenshots/braids.png" >
<br>
(parts of) Mutable Instruments Braids Oscillator(s)
<p>

## Development Status:

### Hardware:
[https://codeberg.org/positionhigh/MicroDexed-touch/wiki/Current-State:-Hardware-%2F-PCB](https://codeberg.org/positionhigh/MicroDexed-touch/wiki/Current-State:-Hardware-%2F-PCB)

### Software (feature progress status):
[https://codeberg.org/positionhigh/MicroDexed-touch/wiki/Current-State:-Software](https://codeberg.org/positionhigh/MicroDexed-touch/wiki/Current-State:-Software)

### Build Instructions (including 3d case files):
[https://codeberg.org/positionhigh/MicroDexed-touch/wiki/Build-Instructions](https://codeberg.org/positionhigh/MicroDexed-touch/wiki/Build-Instructions)

### Build of Materials (BOM)
[https://codeberg.org/positionhigh/MicroDexed-touch/wiki/current-BOM-%28Bill-of-Materials%29-for-MicroDexed-touch](https://codeberg.org/positionhigh/MicroDexed-touch/wiki/current-BOM-%28Bill-of-Materials%29-for-MicroDexed-touch)

### Changelog / News:
[https://codeberg.org/positionhigh/MicroDexed-touch/wiki/Changelog](https://codeberg.org/positionhigh/MicroDexed-touch/wiki/Changelog)

### Troubleshooting Guide & FAQ
[https://codeberg.org/positionhigh/MicroDexed-touch/wiki/Troubleshooting-Guide-and-FAQ](https://codeberg.org/positionhigh/MicroDexed-touch/wiki/Troubleshooting-Guide-and-FAQ)

## Installation/Compile Instructions

* Copy Libraries from /third-party to you Arduino / Teensy Library Directory (On Mac: usualy in your Home Directory/Documents/Arduino/libraries), if not already existing there.

* Copy all Files/Directories from /addon/SD/ to the root of your SD Card (FAT32) and insert the card in SD Slot of the Teensy 4.1

* Open MicroDexed-touch.ino with Teensyduino

* Set Tools -> Optimize to "smallest code"

* Set USB Type to "Serial + MIDI + Audio"

* Compile and Upload to your Teensy 4.1

* Power on the Teensy and attach USB MIDI Keyboard ( or use the virtual Touch Keyboard and the Encoders to navigate)

* If you have a FLASH chip, go to FileManager, Touch the icon "Copy Presets" and push Encoder R. This will copy all Samples from sdcard/DRUMS to FLASH.

## USB MIDI
https://codeberg.org/positionhigh/MicroDexed-touch/wiki/DIY-USB-MIDI-HOST

## License

This is a port of the original Dexed/msfa engine (see https://github.com/asb2m10/dexed and https://github.com/google/music-synthesizer-for-android) to be used on a ~~Teensy-3.6 or~~ Teensy-4.1.

MicroDexed is licensed on the GPL v3. The msfa component (acronym for music synthesizer for android, see https://github.com/google/music-synthesizer-for-android) stays on the Apache 2.0 license to able to collaborate between projects.

## Manuals:

[https://codeberg.org/positionhigh/MicroDexed-touch/wiki/?action=_pages](https://codeberg.org/positionhigh/MicroDexed-touch/wiki/?action=_pages)

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
[https://www.supersurvey.com/poll4335406x48364DD1-136](https://www.supersurvey.com/poll4335406x48364DD1-136)

## If you want to donate to this project, please check out this link:

<a href="https://liberapay.com/positionhigh/donate"><img alt="Donate using Liberapay" src="https://liberapay.com/assets/widgets/donate.svg"></a>

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
<img src="https://codeberg.org/positionhigh/MicroDexed-touch/raw/branch/main/addon/microDexedRemoteConsole/Screenshots/microdexed-001230.png" >
<img src="https://codeberg.org/positionhigh/MicroDexed-touch/raw/branch/main/addon/microDexedRemoteConsole/Screenshots/microdexed-000759.png" >
