#include "microsynth.h"
#include "config.h"
#include <Audio.h>
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
extern AudioMixer<4> microsynth_mixer_r;
extern AudioMixer<4> microsynth_mixer_l;
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
extern AudioMixer<8>* global_delay_in_mixer[NUM_DEXED];

#include <synth_braids.h>
elapsedMillis braids_control_rate;
uint16_t braids_filter_lfo_count[NUM_BRAIDS];
boolean braids_lfo_direction[NUM_BRAIDS];
extern uint8_t generic_active_function;
extern AudioSynthBraids* synthBraids[NUM_BRAIDS];
extern braids_t braids_osc;
extern uint16_t braids_filter_state[NUM_BRAIDS];
extern AudioMixer<NUM_BRAIDS> braids_mixer;
extern AudioMixer<4>* braids_mixer_filter[NUM_BRAIDS];
extern AudioMixer<2> braids_mixer_reverb;
extern AudioEffectEnvelope* braids_envelope[NUM_BRAIDS];
//extern AudioFilterStateVariable* braids_filter[NUM_BRAIDS];
extern AudioFilterBiquad* braids_filter[NUM_BRAIDS];
extern AudioEffectStereoPanorama braids_stereo_panorama;
extern AudioEffectFlange braids_flanger_r;
extern AudioEffectFlange braids_flanger_l;
extern int braids_flanger_idx;
extern int braids_flanger_depth;
extern double braids_flanger_freq;
extern uint8_t generic_temp_select_menu;

extern float volume_transform(float amp);

#include "sequencer.h"
extern sequencer_t seq;

FLASHMEM void microsynth_update_all_settings(uint8_t instance_id) {
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
  microsynth_envelope_noise[instance_id].release(microsynth[instance_id].noise_decay * 8 + 20);
  microsynth_waveform[instance_id].pulseWidth(microsynth[instance_id].pwm_from / 2000.1);
  microsynth[instance_id].pwm_current = microsynth[instance_id].pwm_from;
  microsynth_noise[instance_id].amplitude(microsynth[instance_id].noise_vol / 100.1);
  microsynth_waveform[instance_id].amplitude(mapfloat(microsynth[instance_id].sound_intensity, SOUND_INTENSITY_MIN, SOUND_INTENSITY_MAX, 0, 0.15f));
  microsynth_waveform[instance_id].begin(wave_type[microsynth[instance_id].wave]);
  microsynth_stereo_panorama_osc[instance_id].panorama(mapfloat(microsynth[instance_id].pan, PANORAMA_MIN, PANORAMA_MAX, -1.0, 1.0));
  microsynth_stereo_panorama_noise[instance_id].panorama(mapfloat(microsynth[instance_id].pan, PANORAMA_MIN, PANORAMA_MAX, -1.0, 1.0));

  global_delay_in_mixer[0]->gain(2, mapfloat(microsynth[0].delay_send[0], DELAY_LEVEL_MIN, DELAY_LEVEL_MAX, 0.0, 0.80));
  global_delay_in_mixer[1]->gain(2, mapfloat(microsynth[0].delay_send[1], DELAY_LEVEL_MIN, DELAY_LEVEL_MAX, 0.0, 0.80));

  global_delay_in_mixer[0]->gain(3, mapfloat(microsynth[1].delay_send[0], DELAY_LEVEL_MIN, DELAY_LEVEL_MAX, 0.0, 0.80));
  global_delay_in_mixer[1]->gain(3, mapfloat(microsynth[1].delay_send[1], DELAY_LEVEL_MIN, DELAY_LEVEL_MAX, 0.0, 0.80));

}

void update_microsynth_params() {
  for (uint8_t d = 0; d < NUM_MICROSYNTH; d++) {
    if (microsynth_envelope_osc[d].isActive())  //pwm down
    {
      if (microsynth[d].pwm_from > microsynth[d].pwm_to) {
        if (microsynth[d].pwm_current > microsynth[d].pwm_to) {
          if (microsynth[d].pwm_current - microsynth[d].pwm_speed >= 0)
            microsynth[d].pwm_current = microsynth[d].pwm_current - microsynth[d].pwm_speed;
          else
            microsynth[d].pwm_current = 0;
          microsynth_waveform[d].pulseWidth(microsynth[d].pwm_current / 2000.1);
        }
      } else {
        if (microsynth[d].pwm_current < microsynth[d].pwm_to)  //pwm up
        {
          if (microsynth[d].pwm_current + microsynth[d].pwm_speed <= 2000)
            microsynth[d].pwm_current = microsynth[d].pwm_current + microsynth[d].pwm_speed;
          else
            microsynth[d].pwm_current = 2000;
          microsynth_waveform[d].pulseWidth(microsynth[d].pwm_current / 2000.1);
        }
      }
    }
    if (microsynth[d].filter_osc_freq_from > microsynth[d].filter_osc_freq_to && microsynth[d].filter_osc_speed != 0) {
      if (microsynth[d].filter_osc_freq_current > microsynth[d].filter_osc_freq_to)  //osc filter down
      {
        if (int(microsynth[d].filter_osc_freq_current / float((1.01 + (microsynth[d].filter_osc_speed * 0.001)))) >= 0)
          microsynth[d].filter_osc_freq_current = int(microsynth[d].filter_osc_freq_current / float((1.01 + (microsynth[d].filter_osc_speed * 0.001))));
        else
          microsynth[d].filter_osc_freq_current = 0;

        microsynth_filter_osc[d].frequency(microsynth[d].filter_osc_freq_current);
      }
    } else {
      if (microsynth[d].filter_osc_freq_current < microsynth[d].filter_osc_freq_to && microsynth[d].filter_osc_speed != 0) {  //osc filter up
        if (microsynth[d].filter_osc_freq_current + microsynth[d].filter_osc_speed <= 15000)
          microsynth[d].filter_osc_freq_current = microsynth[d].filter_osc_freq_current + microsynth[d].filter_osc_speed;
        else
          microsynth[d].filter_osc_freq_current = 15000;
        microsynth_filter_osc[d].frequency(microsynth[d].filter_osc_freq_current);
      }
    }

    if (microsynth[d].filter_noise_freq_from > microsynth[d].filter_noise_freq_to && microsynth[d].filter_noise_speed != 0) {
      if (microsynth[d].filter_noise_freq_current > microsynth[d].filter_noise_freq_to) {
        if (int(microsynth[d].filter_noise_freq_current / float((1.01 + (microsynth[d].filter_noise_speed * 0.001)))) >= 0)
          microsynth[d].filter_noise_freq_current = int(microsynth[d].filter_noise_freq_current / float((1.01 + (microsynth[d].filter_noise_speed * 0.001))));
        else
          microsynth[d].filter_noise_freq_current = 0;
        microsynth_filter_noise[d].frequency(microsynth[d].filter_noise_freq_current);
      }
    } else {
      if (microsynth[d].filter_noise_freq_current < microsynth[d].filter_noise_freq_to && microsynth[d].filter_noise_speed != 0) {
        if (microsynth[d].filter_noise_freq_current + microsynth[d].filter_noise_speed <= 15000)
          microsynth[d].filter_noise_freq_current = microsynth[d].filter_noise_freq_current + microsynth[d].filter_noise_speed;
        else
          microsynth[d].filter_noise_freq_current = 15000;
        microsynth_filter_noise[d].frequency(microsynth[d].filter_noise_freq_current);
      }
    }  //  --------------------------------------------------------- OSC LFO ----------------------------------------------------------------------

    if (microsynth[d].lfo_speed > 0 && microsynth[d].lfo_mode == 0)  // LFO U&D
    {
      if (microsynth[d].lfo_direction == false && microsynth[d].lfo_value > microsynth[d].lfo_intensity * -1)
        microsynth[d].lfo_value = microsynth[d].lfo_value - microsynth[d].lfo_speed;

      else if (microsynth[d].lfo_direction == true && microsynth[d].lfo_value < microsynth[d].lfo_intensity)
        microsynth[d].lfo_value = microsynth[d].lfo_value + microsynth[d].lfo_speed;

      if (microsynth[d].lfo_value <= microsynth[d].lfo_intensity * -1)  //switch mode 0 LFO direction
        microsynth[d].lfo_direction = !microsynth[d].lfo_direction;
      else if (microsynth[d].lfo_mode == 0 && microsynth[d].lfo_value >= microsynth[d].lfo_intensity)
        microsynth[d].lfo_direction = !microsynth[d].lfo_direction;
    } else if (microsynth[d].lfo_speed > 0 && microsynth[d].lfo_mode == 1)  // LFO Up
    {
      if (microsynth[d].lfo_value < microsynth[d].lfo_intensity * 10)
        microsynth[d].lfo_value = microsynth[d].lfo_value + microsynth[d].lfo_speed;
    } else if (microsynth[d].lfo_speed > 0 && microsynth[d].lfo_mode == 2)  // LFO Down
    {
      if (microsynth[d].lfo_value > microsynth[d].lfo_intensity * -10)
        microsynth[d].lfo_value = microsynth[d].lfo_value - microsynth[d].lfo_speed;
    }

    //--------------------------------------------------------------------------- LFO FADE/DELAY ---------------------------------------------------------

    if (microsynth[d].lfo_delay == 0)  // no delay, instant lfo mod
    {
      microsynth_waveform[d].frequency(microsynth[d].osc_freq_current + microsynth[d].lfo_value / 10);
    } else if ((int)microsynth_delay_timer[d] / 10 > microsynth[d].lfo_delay && microsynth[d].lfo_fade == 0)  //init lfo fade in
    {
      microsynth[d].lfo_fade = microsynth[d].lfo_delay;
    }
    if (microsynth[d].lfo_fade > 0)  //fade in to max lfo intensity
    {
      if (microsynth[d].osc_freq_current + ((microsynth[d].lfo_value / 10) / (1 + float(microsynth[d].lfo_fade / 10))) > 20 && microsynth[d].osc_freq_current + ((microsynth[d].lfo_value / 10) / (1 + float(microsynth[d].lfo_fade / 10))) < 10000)
        microsynth_waveform[d].frequency(microsynth[d].osc_freq_current + ((microsynth[d].lfo_value / 10) / (1 + float(microsynth[d].lfo_fade / 10))));
      if (microsynth[d].lfo_fade > 1)  //only count down to multiplier of 1, not 0
        microsynth[d].lfo_fade = microsynth[d].lfo_fade - 1;
    }
  }
}

void update_braids_filter(uint8_t d) {
  if (braids_filter_state[d] >= 0 && braids_filter_state[d] <= 15000) {
    if (braids_osc.filter_mode == 1)
      braids_filter[d]->setLowpass(0, braids_filter_state[d], 0.1 + braids_osc.filter_resonance / 10);
    if (braids_osc.filter_mode == 2)
      braids_filter[d]->setBandpass(0, braids_filter_state[d], 0.1 + braids_osc.filter_resonance / 10);
    if (braids_osc.filter_mode == 3)
      braids_filter[d]->setHighpass(0, braids_filter_state[d], 0.1 + braids_osc.filter_resonance / 10);
  }
}

void update_braids_params() {

  for (uint8_t d = 0; d < NUM_BRAIDS; d++) {

    if (braids_envelope[d]->isActive()) {

      if (braids_osc.filter_freq_from > braids_osc.filter_freq_to && braids_osc.filter_speed != 0) {
        if (braids_filter_state[d] > braids_osc.filter_freq_to)  //osc filter down
        {
          if (int(braids_filter_state[d] / float((1.01 + (braids_osc.filter_speed * 0.001)))) >= 0) {
            braids_filter_state[d] = int(braids_filter_state[d] / float((1.01 + (braids_osc.filter_speed * 0.001))));
          } else
            braids_filter_state[d] = 0;
        }

      } else {
        if (braids_filter_state[d] < braids_osc.filter_freq_to && braids_osc.filter_speed != 0) {  //osc filter up
          if (braids_filter_state[d] + braids_osc.filter_speed <= 15000)
            braids_filter_state[d] = braids_filter_state[d] + braids_osc.filter_speed;
          else
            braids_filter_state[d] = 15000;
        }
      }


      if (braids_osc.filter_lfo_speed > 0 && braids_osc.filter_lfo_intensity > 0)  // LFO
      {
        if (braids_lfo_direction[d] == true && braids_filter_state[d] - (braids_osc.filter_lfo_intensity / 100) > 0)
          braids_filter_state[d] = braids_filter_state[d] - (braids_osc.filter_lfo_intensity / 100);
        if (braids_lfo_direction[d] == false && braids_filter_state[d] + (braids_osc.filter_lfo_intensity / 100) < 15000)
          braids_filter_state[d] = braids_filter_state[d] + (braids_osc.filter_lfo_intensity / 100);

        braids_filter_lfo_count[d]++;
        if (braids_filter_lfo_count[d] > 512 / braids_osc.filter_lfo_speed) {
          braids_filter_lfo_count[d] = 0;
          braids_lfo_direction[d] = !braids_lfo_direction[d];
        }
      }
    }
    update_braids_filter(d);
  }
}

FLASHMEM void microsynth_update_single_setting(uint8_t instance_id) {
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
  microsynth_envelope_noise[instance_id].release(microsynth[instance_id].noise_decay * 8 + 20);
  microsynth_waveform[instance_id].pulseWidth(microsynth[instance_id].pwm_from / 2000.1);
  microsynth[instance_id].pwm_current = microsynth[instance_id].pwm_from;
  if (generic_temp_select_menu == 16)
    microsynth_noise[instance_id].amplitude(microsynth[instance_id].noise_vol / 100.1);
  if (generic_temp_select_menu == 0)
    microsynth_waveform[instance_id].amplitude(mapfloat(microsynth[instance_id].sound_intensity, SOUND_INTENSITY_MIN, SOUND_INTENSITY_MAX, 0, 0.15f));
  if (generic_temp_select_menu == 1)
    microsynth_waveform[instance_id].begin(wave_type[microsynth[instance_id].wave]);
  microsynth_stereo_panorama_osc[instance_id].panorama(mapfloat(microsynth[instance_id].pan, PANORAMA_MIN, PANORAMA_MAX, -1.0, 1.0));
  microsynth_stereo_panorama_noise[instance_id].panorama(mapfloat(microsynth[instance_id].pan, PANORAMA_MIN, PANORAMA_MAX, -1.0, 1.0));

  if (generic_temp_select_menu == 30 || generic_temp_select_menu == 31) {
    global_delay_in_mixer[0]->gain(2, mapfloat(microsynth[0].delay_send[0], DELAY_LEVEL_MIN, DELAY_LEVEL_MAX, 0.0, 0.80));
    global_delay_in_mixer[1]->gain(2, mapfloat(microsynth[0].delay_send[1], DELAY_LEVEL_MIN, DELAY_LEVEL_MAX, 0.0, 0.80));
    global_delay_in_mixer[0]->gain(3, mapfloat(microsynth[1].delay_send[0], DELAY_LEVEL_MIN, DELAY_LEVEL_MAX, 0.0, 0.80));
    global_delay_in_mixer[1]->gain(3, mapfloat(microsynth[1].delay_send[1], DELAY_LEVEL_MIN, DELAY_LEVEL_MAX, 0.0, 0.80));
  }
}

FLASHMEM void braids_update_all_settings() {

  if (braids_osc.flanger > 0) {
    braids_flanger_r.voices(braids_flanger_idx, braids_flanger_depth, (float)braids_osc.flanger * 0.003);
    braids_flanger_l.voices(braids_flanger_idx, braids_flanger_depth, (float)braids_osc.flanger * 0.003 + (braids_osc.flanger_spread * 0.001));
  } else {
    braids_flanger_r.voices(FLANGE_DELAY_PASSTHRU, 0, 0);
    braids_flanger_l.voices(FLANGE_DELAY_PASSTHRU, 0, 0);
  }

  braids_mixer_reverb.gain(0, volume_transform(mapfloat(braids_osc.rev_send, REVERB_SEND_MIN, REVERB_SEND_MAX, 0.0, VOL_MAX_FLOAT)));
  braids_mixer_reverb.gain(1, volume_transform(mapfloat(braids_osc.rev_send, REVERB_SEND_MIN, REVERB_SEND_MAX, 0.0, VOL_MAX_FLOAT)));
  braids_stereo_panorama.panorama(mapfloat(braids_osc.pan, PANORAMA_MIN, PANORAMA_MAX, -1.0, 1.0));

  global_delay_in_mixer[0]->gain(4, mapfloat(braids_osc.delay_send_1, DELAY_LEVEL_MIN, DELAY_LEVEL_MAX, 0.0, 0.80));
  global_delay_in_mixer[1]->gain(4, mapfloat(braids_osc.delay_send_2, DELAY_LEVEL_MIN, DELAY_LEVEL_MAX, 0.0, 0.80));

  for (uint8_t instance_id = 0; instance_id < NUM_BRAIDS; instance_id++) {
    braids_mixer.gain(instance_id, volume_transform(mapfloat(braids_osc.sound_intensity, REVERB_SEND_MIN, REVERB_SEND_MAX, 0.0, 0.7)));
    synthBraids[instance_id]->set_braids_shape(braids_osc.algo);
    synthBraids[instance_id]->set_braids_color(braids_osc.color << 6);
    synthBraids[instance_id]->set_braids_timbre((braids_osc.timbre * 2) << 6);

    braids_envelope[instance_id]->attack(braids_osc.env_attack * 4);
    braids_envelope[instance_id]->decay(braids_osc.env_decay * 4);
    braids_envelope[instance_id]->sustain(braids_osc.env_sustain / 50.1);
    braids_envelope[instance_id]->release(braids_osc.env_release * braids_osc.env_release);

    if (braids_osc.filter_mode == 0) {
      braids_mixer_filter[instance_id]->gain(0, 0.0);
      braids_mixer_filter[instance_id]->gain(3, 1.0);
    } else {
      braids_mixer_filter[instance_id]->gain(0, 1.0);
      braids_mixer_filter[instance_id]->gain(3, 0.0);
    }

    if (seq.running == false) {
      braids_filter[instance_id]->setLowpass(0, braids_osc.filter_freq_from, braids_osc.filter_resonance / 10);
      braids_filter_state[instance_id] = braids_osc.filter_freq_from;
    }
  }
}

FLASHMEM void braids_update_single_setting() {

  if ((generic_temp_select_menu == 19 && generic_active_function) || (generic_temp_select_menu == 20 && generic_active_function)) {
    global_delay_in_mixer[0]->gain(4, mapfloat(braids_osc.delay_send_1, DELAY_LEVEL_MIN, DELAY_LEVEL_MAX, 0.0, 0.80));
    global_delay_in_mixer[1]->gain(4, mapfloat(braids_osc.delay_send_2, DELAY_LEVEL_MIN, DELAY_LEVEL_MAX, 0.0, 0.80));
  }
  if ((generic_temp_select_menu == 17 && generic_active_function) || (generic_temp_select_menu == 18 && generic_active_function)) {
    if (braids_osc.flanger > 0) {
      braids_flanger_r.voices(braids_flanger_idx, braids_flanger_depth, (float)braids_osc.flanger * 0.003);
      braids_flanger_l.voices(braids_flanger_idx, braids_flanger_depth, (float)braids_osc.flanger * 0.003 + (braids_osc.flanger_spread * 0.001));
    } else {
      braids_flanger_r.voices(FLANGE_DELAY_PASSTHRU, 0, 0);
      braids_flanger_l.voices(FLANGE_DELAY_PASSTHRU, 0, 0);
    }
  }
  if (generic_temp_select_menu == 16 && generic_active_function) {
    braids_mixer_reverb.gain(0, volume_transform(mapfloat(braids_osc.rev_send, REVERB_SEND_MIN, REVERB_SEND_MAX, 0.0, VOL_MAX_FLOAT)));
    braids_mixer_reverb.gain(1, volume_transform(mapfloat(braids_osc.rev_send, REVERB_SEND_MIN, REVERB_SEND_MAX, 0.0, VOL_MAX_FLOAT)));
  }
  if (generic_temp_select_menu == 21 && generic_active_function) {
    braids_stereo_panorama.panorama(mapfloat(braids_osc.pan, PANORAMA_MIN, PANORAMA_MAX, -1.0, 1.0));
  }
  for (uint8_t instance_id = 0; instance_id < NUM_BRAIDS; instance_id++) {
    if (generic_temp_select_menu == 0 && generic_active_function)
      braids_mixer.gain(instance_id, volume_transform(mapfloat(braids_osc.sound_intensity, REVERB_SEND_MIN, REVERB_SEND_MAX, 0.0, 0.7)));
    if (generic_temp_select_menu == 1 && generic_active_function)
      synthBraids[instance_id]->set_braids_shape(braids_osc.algo);
    if (generic_temp_select_menu == 2 && generic_active_function)
      synthBraids[instance_id]->set_braids_color(braids_osc.color << 6);
    if (generic_temp_select_menu == 3 && generic_active_function)
      synthBraids[instance_id]->set_braids_timbre((braids_osc.timbre * 2) << 6);

    if (generic_temp_select_menu == 5 && generic_active_function)
      braids_envelope[instance_id]->attack(braids_osc.env_attack * 4);
    if (generic_temp_select_menu == 6 && generic_active_function)
      braids_envelope[instance_id]->decay(braids_osc.env_decay * 4);
    if (generic_temp_select_menu == 7 && generic_active_function)
      braids_envelope[instance_id]->sustain(braids_osc.env_sustain / 50.1);
    if (generic_temp_select_menu == 8 && generic_active_function)
      braids_envelope[instance_id]->release(braids_osc.env_release * braids_osc.env_release);

    if (generic_temp_select_menu == 9 && generic_active_function) {
      if (braids_osc.filter_mode == 0) {
        braids_mixer_filter[instance_id]->gain(0, 0.0);
        braids_mixer_filter[instance_id]->gain(3, 1.0);
      } else {
        braids_mixer_filter[instance_id]->gain(0, 1.0);
        braids_mixer_filter[instance_id]->gain(3, 0.0);
      }
    }

    if (generic_temp_select_menu > 9 && generic_temp_select_menu < 16 && generic_active_function) {
      if (seq.running == false) {
        braids_filter[instance_id]->setLowpass(0, braids_osc.filter_freq_from, braids_osc.filter_resonance / 10);
        braids_filter_state[instance_id] = braids_osc.filter_freq_from;
      }
    }
  }
}
