#include "config.h"
#include <Audio.h>
#include "template_mixer.hpp"
#include "effect_stereo_panorama.h"
#include <synth_braids.h>
#include "braids.h"

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
extern AudioMixer<8>* global_delay_in_mixer[NUM_DEXED];

extern float volume_transform(float amp);

#include "sequencer.h"
extern sequencer_t seq;


FLASHMEM void _update_braids_filter(uint8_t d) {
  if (braids_filter_state[d] >= 0 && braids_filter_state[d] <= 15000) {
    if (braids_osc.filter_mode == 1)
      braids_filter[d]->setLowpass(0, braids_filter_state[d], 0.1 + braids_osc.filter_resonance / 10);
    if (braids_osc.filter_mode == 2)
      braids_filter[d]->setBandpass(0, braids_filter_state[d], 0.1 + braids_osc.filter_resonance / 10);
    if (braids_osc.filter_mode == 3)
      braids_filter[d]->setHighpass(0, braids_filter_state[d], 0.1 + braids_osc.filter_resonance / 10);
  }
}

FLASHMEM void update_braids_params() {

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
    _update_braids_filter(d);
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
