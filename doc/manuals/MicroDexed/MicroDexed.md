---
fontfamily: dejavu
fontsize: 16pt
geometry: a4paper,margin=2cm
---

![](../../images/Logo/MicroDexed_logo_black.svg)

written by Holger Wirtz and Thierry Pottier

Version 1.1 (July 2020)

# What is MicroDexed?

MicroDexed is a FM-Software-Synthesizer with six operators and much additional features.
It is written in C/C++ for the microcontroller Teensy-3.6/4.x. The sound generation (msfa)
from the free VST-plugin Dexed was used and a user interface was created using two encoders
and an LCD display.

For the original Dexed/msfa software take a look at [Dexed on Github](https://github.com/asb2m10/dexed) and
[Music Synthesizer for Android on Github](https://github.com/google/music-synthesizer-for-android).

<center><img src="images/MicroDexed1.jpg" width="250"/><img src="images/MicroDexed2.jpg" width="250"/></center>

## Features

* Compatible to a legendary FM synth with six operators from a famous Japanese manufacturer
* MIDI interface:
	* DIN IN/OUT with software THRU (can be disabled, optional hardware THRU possible)
	* USB-Slave (for connecting to a PC)
	* USB-Master (for connecting keyboards)
* Audio interface:
	* RCA stereo IN/OUT with audio THRU (daisy-chain your sound generators(adds a little bit of noise))
* Onboard effects:
	* Chorus (mono)
	* Delay (mono, up to 500ms, with feedback)
	* Low-pass filter with resonance
	* Reverb (stereo)
	* Resonant low-pass filter 
* Mono sound engine with panorama controller before reverb
* Up to 20 voices of polyphony
* Up to 100 banks of 32 voices can be stored on an SD card
* MIDI SYSEX compatible
	* Sounds can be edited with external editors like...
		* [EdiSyn](https://github.com/eclab/edisyn)
		* [Dexed-VST](https://asb2m10.github.io/dexed/)
		* [DX7 by Vstforx](https://dx7.vstforx.de/)
		* [Synthmata](https://synthmata.com/volca-fm/)
		* [KI generated DX banks](https://www.thisdx7cartdoesnotexist.com/)
	* Sending of Voice/Bank MIDI-SYSEX dumps
	* Receiving of Voice/Bank MIDI-SYSEX dumps
	* Voice-Parameter change via MIDI-SYSEX
	* Flexible MIDI controller settings with additional features
	* Modwheel, Pitchbend, Portamento, Breath-Controller, Aftertouch, Foot-Controller
	* Additional modes for most controllers (linear, inverse, direct)
	* Controller parameter change via MIDI-SYSEX
	* Additional MIDI-CCs
		* Bank select
		* Preset select
		* Volume
		* Panorama
		* Filter resonance
		* Filter cutoff
		* Delay time
		* Delay feedback
		* Delay volume
* Storage of voice presets, effect presets and combinations of both as "performance" on SD card
* Transpose, fine-tune, mono-mode
* Note refresh options: normal or retriggered
* Velocity level adaption 
* Three sound engines:
	* Modern : this is the original 24-bit music-synthesizer-for-android implementation.
	* Mark I : Based on the OPL Series but at a higher resolution (LUT are 10-bits). The target of this engine is to be closest to the real DX7.
	* OPL Series : this is an experimental implementation of the reverse-engineered OPL family chips, 8-bit. Keep in mind that the envelopes still need tuning.
* Open-Source (https://codeberg.org/dcoredump/MicroDexed)

<div style="page-break-after: always"></div>

## Manuals

A manual how you can build your own MicroDexed can be found here: [https://codeberg.org/dcoredump/MicroDexed/src/branch/master/doc/manuals/Build-Manual.pdf](https://codeberg.org/dcoredump/MicroDexed/src/branch/master/doc/manuals/Build-Manual.pdf)

A user manual can be found at: [https://codeberg.org/dcoredump/MicroDexed/src/branch/master/doc/manuals/MicroDexed-User_Manual/MicroDexed-User_Manual.pdf](https://codeberg.org/dcoredump/MicroDexed/src/branch/master/doc/manuals/MicroDexed-User_Manual/MicroDexed-User_Manual.pdf)

## License
MicroDexed is licensed under the GPL v3. The msfa component (acronym for music synthesizer for android, see https://github.com/google/music-synthesizer-for-android) stays under the Apache 2.0 license to be able to collaborate between projects.

## Credits & thanks

* Dexed engine by Pascal Gauthier (asb2m10)
* DX Synth engine (as part of Dexed): Raph Levien and the msfa team
* PPPlay : Great OPL3 implementation, with documented code :D
* Thierry Pottier: for extreme testing, discussing about different options, images and many good suggestions for UI handling
* Lars Pelz: Testing and documentation

![MicroDexed](images/01_MD_Peace.jpg)

