# MicroDexed-touch

<br><p>
<img src="https://codeberg.org/positionhigh/MicroDexed-touch/raw/branch/main/doc/Build_InstructionsV3_Capacitive_Touch/mdt_capacitive_touch.png" >
<br>

## This is the MicroDexed with TFT Touch Display, graphic UI and sequencer

### This build requires a Teensy 4.1,  PCM5102 Audio Board, 320x240 ILI9341 SPI Display with Capacitive Touchscreen and a serial Flash Chip.

* The Flash Chip is presoldered on a small PCB attachment for <2$. You do not have to solder small SMD parts to add this to your build.
<p>

MicroDexed started as a Teensy based, 6-operator-FM-synthesizer. The first generation is still maintained at: [https://codeberg.org/dcoredump/MicroDexed](https://codeberg.org/dcoredump/MicroDexed)

Compared to the first generation, MicroDexed-touch offers a lot of new features/improvements:

* "Large" Graphical Interface, view and edit multiple Parameters in one Screen

* Use external MIDI gear over USB or Mini TRS Jacks (DIN MIDI with adaptor) from 2 x 16 MIDI Channels in the Sequencer

* Map MIDI CC to external MIDI Devices to control Input/Parameters

* Loading, playing and editing multi-sampled instruments or layered instruments in a "live" mode or played by the sequencer. Auto detection sample zones but also can be edited manually. This is early work in progress and currently not stable when using pitch shifting and polyphony

* 2 instances of "Virtual Analog" Monosynths with one OSC, PWM, Multimode Filter and Noise. A lot of useful new sound possibilities.

* 8 voice polyphonic Oscillator, based on the code of Mutable Instruments - Braids. No wavetables but added individual envelopes and multimode filters for all voices
 
* Much improved Sequencer with 8 Tracks, 16 Step Patterns, 16 Step Pattern Chains, up to 16384 Song Steps, Arpeggiator with 1/8 - 1/64 Steps, Arp Patterns including Euclidean settings, Pitched Drum- and Instrument Sample playback

* The sequencer can also play chords of various types, stacked up to 7 notes, using only a single sequencer track

* Global Mixer View with all Channels

* Multiband Master Compressor with 4 bands

* Sample Management from SD-CARD and SPI FLASH, samples can be loaded/modified from SD-CARD to FLASH during runtime.

* Touch Mute Matrix for live / realtime performance

* Tracker View (Editor) - work in progress

* Track Print/Recording - work in progress

* Remote control in web-based 1:1 UI with added features (filemanager,filetransfer, screenshots etc.) Work in progress - connected to PC via Teensy MicroUSB Connector

### YouTube playlist about the development:
[https://www.youtube.com/playlist?list=PLHTypoMU1QoGOXPli8bjR6MknPiQpubHl
](https://www.youtube.com/playlist?list=PLHTypoMU1QoGOXPli8bjR6MknPiQpubHl)

## If you want to donate to this project, please check out this link:

<a href="https://liberapay.com/positionhigh/donate"><img alt="Donate using Liberapay" src="https://liberapay.com/assets/widgets/donate.svg"></a>

### User Chat groups

[https://discord.gg/XCYk5P8GzF](https://discord.gg/XCYk5P8GzF)

[https://matrix.to/#/#microdexed:matrix.org](https://matrix.to/#/#microdexed:matrix.org)

### PCB is available from PCBWAY

If you register and pay as a new user at PCBWAY with this Invite link, you should get $5 "New User Free Credit" - so you can order 5 PCB pieces for "free", except shipping and customs cost etc.
    
[https://www.pcbway.com/setinvite.aspx?inviteid=565384](https://www.pcbway.com/setinvite.aspx?inviteid=565384)

<a href="https://www.pcbway.com/project/shareproject/MicroDexed_Touch_current_version_with_PCM5102_d643a695.html"><img src="https://www.pcbway.com/project/img/images/frompcbway-1220.png" alt="PCB from PCBWay" /></a>

<br>
<p>

## WIKI

Installation / BOM / Compile Instructions / 3D printer files / FAQ / Changelog / News / Troubleshooting etc.
[https://codeberg.org/positionhigh/MicroDexed-touch/wiki/?action=_pages](https://codeberg.org/positionhigh/MicroDexed-touch/wiki/?action=_pages)

## License

This is a port of the original Dexed/msfa engine (see https://github.com/asb2m10/dexed and https://github.com/google/music-synthesizer-for-android) to be used on a ~~Teensy-3.6 or~~ Teensy-4.1.

MicroDexed is licensed on the GPL v3. The msfa component (acronym for music synthesizer for android, see https://github.com/google/music-synthesizer-for-android) stays on the Apache 2.0 license to able to collaborate between projects.

## Buttons / Encoders

A quick guide how the push Encoders are working:

Usualy [SHORT PUSH] Encoder Left brings you back to the main menu or closer to the main menu one step.

Usualy [SHORT PUSH] Encoder Right selects or confirms an input/menu item/goes deeper into this item.

In most menu settings of dexed, [SHORT PUSH] Encoder Right, switches between the 2 dexed Instances.

In microsynth, [LONG PUSH] Encoder Right, switches between the 2 microsynth Instances.

in Voice select (dexed) : [SHORT PUSH] Encoder Right switches Instances, [LONG PUSH] Encoder Right switches betweens Banks and Sounds. [LONG PUSH] Encoder Left selects/unselects Sounds to the Favorites list.

In Menus that need 2 Encoders, Encoder Right controls Y movement and Encoder Left controls X movement
(for example in Song mode)


## CONTRIBUTING

This project lives from the contributions of C++ developers, testers, reviewers. Please check https://codeberg.org/positionhigh/MicroDexed-touch/issues to help in open topics or add your own Issue or Feature Request.

## SURVEY / POLL: Let us know what you want to see
[https://www.supersurvey.com/poll4335406x48364DD1-136](https://www.supersurvey.com/poll4335406x48364DD1-136)

## CURRENT Screenshots:

<img src="https://codeberg.org/positionhigh/MicroDexed-touch/raw/branch/main/addon/microDexedRemoteConsole/Screenshots/euc1.png" >
<br>
Euclidean Arps
<p>
<br>
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
<br>
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
