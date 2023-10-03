#include <Arduino.h>
#include <Audio.h>
#include <Wire.h>
#include <SPI.h>
#include <effect_delay_ext8.h>

// GUItool: begin automatically generated code
AudioSynthWaveformSine sine1;                                     //xy=382,432
AudioAmplifier amp1;                                              //xy=606,441
AudioEffectDelayExternal8 delayExt1(AUDIO_MEMORY8_EXTMEM, 20000); //xy=770,321
AudioMixer4 mixer1;                                               //xy=983,386
AudioOutputI2S i2s1;                                              //xy=1131,343
AudioConnection patchCord1(sine1, amp1);
AudioConnection patchCord2(amp1, delayExt1);
AudioConnection patchCord3(amp1, 0, mixer1, 1);
AudioConnection patchCord4(delayExt1, 0, mixer1, 0);
AudioConnection patchCord5(mixer1, 0, i2s1, 0);
AudioConnection patchCord6(mixer1, 0, i2s1, 1);
AudioControlSGTL5000 sgtl5000;  //xy=943,541
// GUItool: end automatically generated code


void setup() {
  Serial.begin(230400);
  delay(50);
  Serial.println("<setup begin>");

  AudioMemory(100);

  delayExt1.delay(0, 533);
  delayExt1.delay(1, 264);
  delayExt1.delay(2, 1000);
  sine1.amplitude(1.0);
  sine1.frequency(440);
  sine1.phase(0.0);
  mixer1.gain(0, 0.5);
  mixer1.gain(1, 1.0);
  amp1.gain(1.0);

  Serial.println("<setup end>");
}

void loop() {
  Serial.print("<SINE ON>");
  amp1.gain(1.0);
  delay(200);
  amp1.gain(0.0);
  Serial.println("<SINE OFF>");
  delay(2000);
}
