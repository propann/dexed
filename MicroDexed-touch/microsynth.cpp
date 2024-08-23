#include "config.h"
#include <Audio.h>
#include "microsynth.h"
#include "template_mixer.hpp"
#include "effect_stereo_panorama.h"

elapsedMillis microsynth_delay_timer[2];
extern AudioSynthWaveform microsynth_waveform[NUM_MICROSYNTH];
extern AudioSynthNoisePink microsynth_noise[NUM_MICROSYNTH];
extern AudioEffectEnvelope microsynth_envelope_osc[NUM_MICROSYNTH];
extern AudioEffectEnvelope microsynth_envelope_noise[NUM_MICROSYNTH];
extern AudioFilterStateVariable microsynth_filter_osc[NUM_MICROSYNTH];
extern AudioFilterStateVariable microsynth_filter_noise[NUM_MICROSYNTH];
extern AudioEffectStereoPanorama microsynth_stereo_panorama_osc[NUM_MICROSYNTH];
extern AudioEffectStereoPanorama microsynth_stereo_panorama_noise[NUM_MICROSYNTH];
extern AudioMixer<2> microsynth_mixer_r[NUM_MICROSYNTH];
extern AudioMixer<2> microsynth_mixer_l[NUM_MICROSYNTH];
extern AudioMixer<4> microsynth_mixer_filter_osc[NUM_MICROSYNTH];
extern AudioMixer<4> microsynth_mixer_filter_noise[NUM_MICROSYNTH];
extern AudioMixer<2> microsynth_mixer_reverb;
extern AudioAnalyzePeak microsynth_peak_osc_0;
extern AudioAnalyzePeak microsynth_peak_osc_1;
extern AudioAnalyzePeak microsynth_peak_noise_0;
extern AudioAnalyzePeak microsynth_peak_noise_1;
extern microsynth_t microsynth[NUM_MICROSYNTH];
extern uint8_t microsynth_selected_instance;
extern short wave_type[9];
extern AudioMixer<ALL_STATIC_DELAY_INSTR_OUT+NUM_DRUMS>* global_delay_in_mixer[NUM_DEXED];
extern uint8_t generic_temp_select_menu;

extern float volume_transform(float amp);

#include "sequencer.h"
extern sequencer_t seq;

FLASHMEM void microsynth_update_all_settings(uint8_t instance_id)
{
  microsynth_mixer_filter_osc[instance_id].gain(0, 0.0);
  microsynth_mixer_filter_osc[instance_id].gain(1, 0.0);
  microsynth_mixer_filter_osc[instance_id].gain(2, 0.0);
  microsynth_mixer_filter_osc[instance_id].gain(3, 0.0);
  microsynth_mixer_filter_noise[instance_id].gain(0, 0.0);
  microsynth_mixer_filter_noise[instance_id].gain(1, 0.0);
  microsynth_mixer_filter_noise[instance_id].gain(2, 0.0);
  microsynth_mixer_filter_noise[instance_id].gain(3, 0.0);
  if (microsynth[instance_id].filter_osc_mode == 0)
    microsynth_mixer_filter_osc[instance_id].gain(3, 1.0);
  else if (microsynth[instance_id].filter_osc_mode == 1)
    microsynth_mixer_filter_osc[instance_id].gain(0, 1.0);
  else if (microsynth[instance_id].filter_osc_mode == 2)
    microsynth_mixer_filter_osc[instance_id].gain(1, 1.0);
  else if (microsynth[instance_id].filter_osc_mode == 3)
    microsynth_mixer_filter_osc[instance_id].gain(2, 1.0);
  if (microsynth[instance_id].filter_noise_mode == 0)
    microsynth_mixer_filter_noise[instance_id].gain(3, 1.0);
  else if (microsynth[instance_id].filter_noise_mode == 1)
    microsynth_mixer_filter_noise[instance_id].gain(0, 1.0);
  else if (microsynth[instance_id].filter_noise_mode == 2)
    microsynth_mixer_filter_noise[instance_id].gain(1, 1.0);
  else if (microsynth[instance_id].filter_noise_mode == 3)
    microsynth_mixer_filter_noise[instance_id].gain(2, 1.0);
  microsynth_envelope_osc[instance_id].attack(microsynth[instance_id].env_attack * 4);
  microsynth_envelope_osc[instance_id].decay(microsynth[instance_id].env_decay * 4);
  microsynth_envelope_osc[instance_id].sustain(microsynth[instance_id].env_sustain / 50.1);
  microsynth_envelope_osc[instance_id].release(microsynth[microsynth_selected_instance].env_release * microsynth[microsynth_selected_instance].env_release);
  microsynth_mixer_reverb.gain(instance_id, volume_transform(mapfloat(microsynth[instance_id].rev_send, REVERB_SEND_MIN, REVERB_SEND_MAX, 0.0, VOL_MAX_FLOAT)));
  microsynth_filter_noise[instance_id].frequency(microsynth[instance_id].filter_noise_freq_from);
  microsynth_filter_osc[instance_id].frequency(microsynth[instance_id].filter_osc_freq_from);
  microsynth_filter_noise[instance_id].resonance(microsynth[instance_id].filter_noise_resonance / 20);
  microsynth_filter_osc[instance_id].resonance(microsynth[instance_id].filter_osc_resonance / 20);
  microsynth_envelope_noise[instance_id].decay(microsynth[instance_id].noise_decay + 20);
  microsynth_envelope_noise[instance_id].sustain(0);
  microsynth_envelope_noise[instance_id].release(microsynth[instance_id].noise_decay * 20 + 20);
  microsynth_waveform[instance_id].pulseWidth(microsynth[instance_id].pwm_from / 2000.1);
  microsynth[instance_id].pwm_current = microsynth[instance_id].pwm_from;

  microsynth_noise[instance_id].amplitude(0.9f);
  microsynth_waveform[instance_id].amplitude(0.5f);
  microsynth_mixer_l[instance_id].gain(0, mapfloat(microsynth[instance_id].sound_intensity, SOUND_INTENSITY_MIN, SOUND_INTENSITY_MAX, 0.0f, 0.15f));
  microsynth_mixer_r[instance_id].gain(0, mapfloat(microsynth[instance_id].sound_intensity, SOUND_INTENSITY_MIN, SOUND_INTENSITY_MAX, 0.0f, 0.15f));

  microsynth_waveform[instance_id].begin(wave_type[microsynth[instance_id].wave]);
  microsynth_stereo_panorama_osc[instance_id].panorama(mapfloat(microsynth[instance_id].pan, PANORAMA_MIN, PANORAMA_MAX, -1.0, 1.0));
  microsynth_stereo_panorama_noise[instance_id].panorama(mapfloat(microsynth[instance_id].pan, PANORAMA_MIN, PANORAMA_MAX, -1.0, 1.0));

  global_delay_in_mixer[0]->gain(2, mapfloat(microsynth[0].delay_send[0], DELAY_LEVEL_MIN, DELAY_LEVEL_MAX, 0.0, 0.80));
  global_delay_in_mixer[1]->gain(2, mapfloat(microsynth[0].delay_send[1], DELAY_LEVEL_MIN, DELAY_LEVEL_MAX, 0.0, 0.80));

  global_delay_in_mixer[0]->gain(3, mapfloat(microsynth[1].delay_send[0], DELAY_LEVEL_MIN, DELAY_LEVEL_MAX, 0.0, 0.80));
  global_delay_in_mixer[1]->gain(3, mapfloat(microsynth[1].delay_send[1], DELAY_LEVEL_MIN, DELAY_LEVEL_MAX, 0.0, 0.80));
}

FLASHMEM void update_microsynth_params()
{
  for (uint8_t d = 0; d < NUM_MICROSYNTH; d++)
  {
    if (microsynth_envelope_osc[d].isActive()) // pwm down
    {
      if (microsynth[d].pwm_from > microsynth[d].pwm_to)
      {
        if (microsynth[d].pwm_current > microsynth[d].pwm_to)
        {
          if (microsynth[d].pwm_current - microsynth[d].pwm_speed >= 0)
            microsynth[d].pwm_current = microsynth[d].pwm_current - microsynth[d].pwm_speed;
          else
            microsynth[d].pwm_current = 0;
          microsynth_waveform[d].pulseWidth(microsynth[d].pwm_current / 2000.1);
        }
      }
      else
      {
        if (microsynth[d].pwm_current < microsynth[d].pwm_to) // pwm up
        {
          if (microsynth[d].pwm_current + microsynth[d].pwm_speed <= 2000)
            microsynth[d].pwm_current = microsynth[d].pwm_current + microsynth[d].pwm_speed;
          else
            microsynth[d].pwm_current = 2000;
          microsynth_waveform[d].pulseWidth(microsynth[d].pwm_current / 2000.1);
        }
      }
    }
    if (microsynth[d].filter_osc_freq_from > microsynth[d].filter_osc_freq_to && microsynth[d].filter_osc_speed != 0)
    {
      if (microsynth[d].filter_osc_freq_current > microsynth[d].filter_osc_freq_to) // osc filter down
      {
        if (int(microsynth[d].filter_osc_freq_current / float((1.01 + (microsynth[d].filter_osc_speed * 0.001)))) >= 0)
          microsynth[d].filter_osc_freq_current = int(microsynth[d].filter_osc_freq_current / float((1.01 + (microsynth[d].filter_osc_speed * 0.001))));
        else
          microsynth[d].filter_osc_freq_current = 0;

        microsynth_filter_osc[d].frequency(microsynth[d].filter_osc_freq_current);
      }
    }
    else
    {
      if (microsynth[d].filter_osc_freq_current < microsynth[d].filter_osc_freq_to && microsynth[d].filter_osc_speed != 0)
      { // osc filter up
        if (microsynth[d].filter_osc_freq_current + microsynth[d].filter_osc_speed <= 15000)
          microsynth[d].filter_osc_freq_current = microsynth[d].filter_osc_freq_current + microsynth[d].filter_osc_speed;
        else
          microsynth[d].filter_osc_freq_current = 15000;
        microsynth_filter_osc[d].frequency(microsynth[d].filter_osc_freq_current);
      }
    }

    if (microsynth[d].filter_noise_freq_from > microsynth[d].filter_noise_freq_to && microsynth[d].filter_noise_speed != 0)
    {
      if (microsynth[d].filter_noise_freq_current > microsynth[d].filter_noise_freq_to)
      {
        if (int(microsynth[d].filter_noise_freq_current / float((1.01 + (microsynth[d].filter_noise_speed * 0.001)))) >= 0)
          microsynth[d].filter_noise_freq_current = int(microsynth[d].filter_noise_freq_current / float((1.01 + (microsynth[d].filter_noise_speed * 0.001))));
        else
          microsynth[d].filter_noise_freq_current = 0;
        microsynth_filter_noise[d].frequency(microsynth[d].filter_noise_freq_current);
      }
    }
    else
    {
      if (microsynth[d].filter_noise_freq_current < microsynth[d].filter_noise_freq_to && microsynth[d].filter_noise_speed != 0)
      {
        if (microsynth[d].filter_noise_freq_current + microsynth[d].filter_noise_speed <= 15000)
          microsynth[d].filter_noise_freq_current = microsynth[d].filter_noise_freq_current + microsynth[d].filter_noise_speed;
        else
          microsynth[d].filter_noise_freq_current = 15000;
        microsynth_filter_noise[d].frequency(microsynth[d].filter_noise_freq_current);
      }
    } //  --------------------------------------------------------- OSC LFO ----------------------------------------------------------------------

    if (microsynth[d].lfo_speed > 0 && microsynth[d].lfo_mode == 0) // LFO U&D
    {
      if (microsynth[d].lfo_direction == false && microsynth[d].lfo_value > microsynth[d].lfo_intensity * -1)
        microsynth[d].lfo_value = microsynth[d].lfo_value - microsynth[d].lfo_speed;

      else if (microsynth[d].lfo_direction == true && microsynth[d].lfo_value < microsynth[d].lfo_intensity)
        microsynth[d].lfo_value = microsynth[d].lfo_value + microsynth[d].lfo_speed;

      if (microsynth[d].lfo_value <= microsynth[d].lfo_intensity * -1) // switch mode 0 LFO direction
        microsynth[d].lfo_direction = !microsynth[d].lfo_direction;
      else if (microsynth[d].lfo_mode == 0 && microsynth[d].lfo_value >= microsynth[d].lfo_intensity)
        microsynth[d].lfo_direction = !microsynth[d].lfo_direction;
    }
    else if (microsynth[d].lfo_speed > 0 && microsynth[d].lfo_mode == 1) // LFO Up
    {
      if (microsynth[d].lfo_value < microsynth[d].lfo_intensity * 10)
        microsynth[d].lfo_value = microsynth[d].lfo_value + microsynth[d].lfo_speed;
    }
    else if (microsynth[d].lfo_speed > 0 && microsynth[d].lfo_mode == 2) // LFO Down
    {
      if (microsynth[d].lfo_value > microsynth[d].lfo_intensity * -10)
        microsynth[d].lfo_value = microsynth[d].lfo_value - microsynth[d].lfo_speed;
    }

    //--------------------------------------------------------------------------- LFO FADE/DELAY ---------------------------------------------------------

    if (microsynth[d].lfo_delay == 0) // no delay, instant lfo mod
    {
      microsynth_waveform[d].frequency(microsynth[d].osc_freq_current + microsynth[d].lfo_value / 10);
    }
    else if ((int)microsynth_delay_timer[d] / 10 > microsynth[d].lfo_delay && microsynth[d].lfo_fade == 0) // init lfo fade in
    {
      microsynth[d].lfo_fade = microsynth[d].lfo_delay;
    }
    if (microsynth[d].lfo_fade > 0) // fade in to max lfo intensity
    {
      if (microsynth[d].osc_freq_current + ((microsynth[d].lfo_value / 10) / (1 + float(microsynth[d].lfo_fade / 10))) > 20 && microsynth[d].osc_freq_current + ((microsynth[d].lfo_value / 10) / (1 + float(microsynth[d].lfo_fade / 10))) < 10000)
        microsynth_waveform[d].frequency(microsynth[d].osc_freq_current + ((microsynth[d].lfo_value / 10) / (1 + float(microsynth[d].lfo_fade / 10))));
      if (microsynth[d].lfo_fade > 1) // only count down to multiplier of 1, not 0
        microsynth[d].lfo_fade = microsynth[d].lfo_fade - 1;
    }
  }
}

FLASHMEM void microsynth_update_single_setting(uint8_t instance_id)
{
  microsynth_mixer_filter_osc[instance_id].gain(0, 0.0);
  microsynth_mixer_filter_osc[instance_id].gain(1, 0.0);
  microsynth_mixer_filter_osc[instance_id].gain(2, 0.0);
  microsynth_mixer_filter_osc[instance_id].gain(3, 0.0);
  microsynth_mixer_filter_noise[instance_id].gain(0, 0.0);
  microsynth_mixer_filter_noise[instance_id].gain(1, 0.0);
  microsynth_mixer_filter_noise[instance_id].gain(2, 0.0);
  microsynth_mixer_filter_noise[instance_id].gain(3, 0.0);
  if (microsynth[instance_id].filter_osc_mode == 0)
    microsynth_mixer_filter_osc[instance_id].gain(3, 1.0);
  else if (microsynth[instance_id].filter_osc_mode == 1)
    microsynth_mixer_filter_osc[instance_id].gain(0, 1.0);
  else if (microsynth[instance_id].filter_osc_mode == 2)
    microsynth_mixer_filter_osc[instance_id].gain(1, 1.0);
  else if (microsynth[instance_id].filter_osc_mode == 3)
    microsynth_mixer_filter_osc[instance_id].gain(2, 1.0);

  if (microsynth[instance_id].filter_noise_mode == 0)
    microsynth_mixer_filter_noise[instance_id].gain(3, 1.0);
  else if (microsynth[instance_id].filter_noise_mode == 1)
    microsynth_mixer_filter_noise[instance_id].gain(0, 1.0);
  else if (microsynth[instance_id].filter_noise_mode == 2)
    microsynth_mixer_filter_noise[instance_id].gain(1, 1.0);
  else if (microsynth[instance_id].filter_noise_mode == 3)
    microsynth_mixer_filter_noise[instance_id].gain(2, 1.0);

  if (generic_temp_select_menu == 4)
    microsynth_envelope_osc[instance_id].attack(microsynth[instance_id].env_attack * 4);
  if (generic_temp_select_menu == 5)
    microsynth_envelope_osc[instance_id].decay(microsynth[instance_id].env_decay * 4);
  if (generic_temp_select_menu == 6)
    microsynth_envelope_osc[instance_id].sustain(microsynth[instance_id].env_sustain / 50.1);
  if (generic_temp_select_menu == 7)
    microsynth_envelope_osc[instance_id].release(microsynth[microsynth_selected_instance].env_release * microsynth[microsynth_selected_instance].env_release);
  microsynth_mixer_reverb.gain(instance_id, volume_transform(mapfloat(microsynth[instance_id].rev_send, REVERB_SEND_MIN, REVERB_SEND_MAX, 0.0, VOL_MAX_FLOAT)));
  if (generic_temp_select_menu == 20)
    microsynth_filter_noise[instance_id].frequency(microsynth[instance_id].filter_noise_freq_from);
  microsynth_filter_osc[instance_id].frequency(microsynth[instance_id].filter_osc_freq_from);
  if (generic_temp_select_menu == 22)
    microsynth_filter_noise[instance_id].resonance(microsynth[instance_id].filter_noise_resonance / 20);
  microsynth_filter_osc[instance_id].resonance(microsynth[instance_id].filter_osc_resonance / 20);

  if (generic_temp_select_menu == 17)
    microsynth_envelope_noise[instance_id].decay(microsynth[instance_id].noise_decay + 20);
  microsynth_envelope_noise[instance_id].sustain(0);
  microsynth_envelope_noise[instance_id].release(microsynth[instance_id].noise_decay * 20 + 20);
  microsynth_waveform[instance_id].pulseWidth(microsynth[instance_id].pwm_from / 2000.1);
  microsynth[instance_id].pwm_current = microsynth[instance_id].pwm_from;
  if (generic_temp_select_menu == 16)
  {
    microsynth_noise[instance_id].amplitude(0.9);
    microsynth_mixer_l[instance_id].gain(1, mapfloat(microsynth[instance_id].noise_vol, SOUND_INTENSITY_MIN, SOUND_INTENSITY_MAX, 0.0f, 0.25f));
    microsynth_mixer_r[instance_id].gain(1, mapfloat(microsynth[instance_id].noise_vol, SOUND_INTENSITY_MIN, SOUND_INTENSITY_MAX, 0.0f, 0.25f));
  }

  if (generic_temp_select_menu == 0)
  {
    // microsynth_waveform[instance_id].amplitude(mapfloat(microsynth[instance_id].sound_intensity, SOUND_INTENSITY_MIN, SOUND_INTENSITY_MAX, 0.0f, 0.15f));
    microsynth_mixer_l[instance_id].gain(0, mapfloat(microsynth[instance_id].sound_intensity, SOUND_INTENSITY_MIN, SOUND_INTENSITY_MAX, 0.0f, 0.15f));
    microsynth_mixer_r[instance_id].gain(0, mapfloat(microsynth[instance_id].sound_intensity, SOUND_INTENSITY_MIN, SOUND_INTENSITY_MAX, 0.0f, 0.15f));
  }
  if (generic_temp_select_menu == 1)
    microsynth_waveform[instance_id].begin(wave_type[microsynth[instance_id].wave]);
  microsynth_stereo_panorama_osc[instance_id].panorama(mapfloat(microsynth[instance_id].pan, PANORAMA_MIN, PANORAMA_MAX, -1.0, 1.0));
  microsynth_stereo_panorama_noise[instance_id].panorama(mapfloat(microsynth[instance_id].pan, PANORAMA_MIN, PANORAMA_MAX, -1.0, 1.0));

  if (generic_temp_select_menu == 30 || generic_temp_select_menu == 31)
  {
    global_delay_in_mixer[0]->gain(2, mapfloat(microsynth[0].delay_send[0], DELAY_LEVEL_MIN, DELAY_LEVEL_MAX, 0.0, 0.80));
    global_delay_in_mixer[1]->gain(2, mapfloat(microsynth[0].delay_send[1], DELAY_LEVEL_MIN, DELAY_LEVEL_MAX, 0.0, 0.80));
    global_delay_in_mixer[0]->gain(3, mapfloat(microsynth[1].delay_send[0], DELAY_LEVEL_MIN, DELAY_LEVEL_MAX, 0.0, 0.80));
    global_delay_in_mixer[1]->gain(3, mapfloat(microsynth[1].delay_send[1], DELAY_LEVEL_MIN, DELAY_LEVEL_MAX, 0.0, 0.80));
  }
}
