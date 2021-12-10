#include <Audio.h>
#include "synth_mda_epiano.h"

AudioSynthEPiano         ep(16);
AudioOutputI2S           i2s1;
AudioControlSGTL5000     sgtl5000_1;
AudioConnection          patchCord1(ep, 0, i2s1, 0);
AudioConnection          patchCord2(ep, 0, i2s1, 1);

void setup()
{
  AudioMemory(32);

  sgtl5000_1.enable();
  sgtl5000_1.lineOutLevel(29);
  sgtl5000_1.dacVolumeRamp();
  sgtl5000_1.dacVolume(1.0);
  sgtl5000_1.unmuteHeadphone();
  sgtl5000_1.unmuteLineout();
  sgtl5000_1.volume(0.8, 0.8); // Headphone volume
}

void loop()
{
  uint8_t x=random(11);
  int8_t d=random(60)-30;
  uint8_t p=random(7);

  ep.setProgram(p);
  
  Serial.println("Key-Down");
  
  ep.noteOn(48+x, 90+d);
  delay(100);
  ep.noteOn(52+x, 90+d);
  delay(100);
  ep.noteOn(55+x, 90+d);
  delay(100);
  ep.noteOn(60+x, 90+d);
  delay(2000);

  Serial.println("Key-Up");
  ep.noteOff(48+x);
  ep.noteOff(52+x);
  ep.noteOff(55+x);
  ep.noteOff(60+x);
  delay(2000);
}
