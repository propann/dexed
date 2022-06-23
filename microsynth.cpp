
#include "microsynth.h"
#include "config.h"
#include <Audio.h>
#include "template_mixer.hpp"
#include "effect_stereo_panorama.h"

#ifdef USE_MICROSYNTH
extern AudioSynthWaveform              microsynth_waveform[NUM_MICROSYNTH];
extern AudioSynthNoisePink             microsynth_noise[NUM_MICROSYNTH];
extern AudioEffectEnvelope             microsynth_envelope_osc[NUM_MICROSYNTH];
extern AudioEffectEnvelope             microsynth_envelope_noise[NUM_MICROSYNTH];
extern AudioFilterStateVariable        microsynth_filter_osc[NUM_MICROSYNTH];
extern AudioFilterStateVariable        microsynth_filter_noise[NUM_MICROSYNTH];
extern AudioEffectStereoPanorama       microsynth_stereo_panorama_osc[NUM_MICROSYNTH];
extern AudioEffectStereoPanorama       microsynth_stereo_panorama_noise[NUM_MICROSYNTH];
extern AudioMixer<4>                   microsynth_mixer_r;
extern AudioMixer<4>                   microsynth_mixer_l;
extern AudioMixer<4>                   microsynth_mixer_filter_osc[NUM_MICROSYNTH];
extern AudioMixer<4>                   microsynth_mixer_filter_noise[NUM_MICROSYNTH];
extern AudioMixer<2>                   microsynth_mixer_reverb;
extern AudioAnalyzePeak                microsynth_peak_osc_0;
extern AudioAnalyzePeak                microsynth_peak_osc_1;
extern AudioAnalyzePeak                microsynth_peak_noise_0;
extern AudioAnalyzePeak                microsynth_peak_noise_1;
extern microsynth_t  microsynth[NUM_MICROSYNTH];
extern uint8_t microsynth_selected_instance;
extern short wave_type[9];
#endif

#ifdef USE_BRAIDS
#include <synth_braids.h>
extern AudioSynthBraids*               synthBraids[NUM_BRAIDS];
extern braids_t  braids_osc;
//extern braids_filter_state_t* braids_filter_state[NUM_BRAIDS];
extern uint16_t braids_filter_state[NUM_BRAIDS];
extern AudioMixer<NUM_BRAIDS>          braids_mixer;
extern AudioMixer<4>*                   braids_mixer_filter[NUM_BRAIDS];
extern AudioMixer<2>                   braids_mixer_reverb;
extern AudioEffectEnvelope*            braids_envelope[NUM_BRAIDS];
extern AudioFilterStateVariable*        braids_filter[NUM_BRAIDS];
extern AudioEffectStereoPanorama       braids_stereo_panorama;
#endif

extern float volume_transform(float amp);

#ifdef USE_SEQUENCER
#include "sequencer.h"
extern sequencer_t seq;
#endif

void microsynth_update_settings(uint8_t instance_id)
{
#ifdef USE_MICROSYNTH
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
  microsynth_envelope_osc[instance_id].decay(microsynth[instance_id].env_decay * 4 );
  microsynth_envelope_osc[instance_id].sustain(microsynth[instance_id].env_sustain / 50.1);
  microsynth_envelope_osc[instance_id].release( microsynth[microsynth_selected_instance].env_release * microsynth[microsynth_selected_instance].env_release );
  microsynth_mixer_reverb.gain(instance_id, volume_transform(mapfloat(microsynth[instance_id].rev_send, EP_REVERB_SEND_MIN, EP_REVERB_SEND_MAX, 0.0, VOL_MAX_FLOAT)));
  microsynth_filter_noise[instance_id].frequency(microsynth[instance_id].filter_noise_freq_from);
  microsynth_filter_osc[instance_id].frequency(microsynth[instance_id].filter_osc_freq_from);
  microsynth_filter_noise[instance_id].resonance(microsynth[instance_id].filter_noise_resonance / 20);
  microsynth_filter_osc[instance_id].resonance(microsynth[instance_id].filter_osc_resonance / 20);
  microsynth_envelope_noise[instance_id].release(microsynth[instance_id].noise_decay * 4);
  microsynth_waveform[instance_id].pulseWidth(microsynth[instance_id].pwm_from / 2000.1);
  microsynth[instance_id].pwm_current = microsynth[instance_id].pwm_from;
  microsynth_noise[instance_id].amplitude(microsynth[instance_id].noise_vol / 100.1);
  microsynth_waveform[instance_id].amplitude( mapfloat(microsynth[instance_id].sound_intensity, MS_SOUND_INTENSITY_MIN, MS_SOUND_INTENSITY_MAX, 0, 0.15f));
  microsynth_waveform[instance_id].begin(wave_type[microsynth[instance_id].wave]);
  microsynth_stereo_panorama_osc[instance_id].panorama(mapfloat(microsynth[instance_id].pan, PANORAMA_MIN, PANORAMA_MAX, -1.0, 1.0));
  microsynth_stereo_panorama_noise[instance_id].panorama(mapfloat(microsynth[instance_id].pan, PANORAMA_MIN, PANORAMA_MAX, -1.0, 1.0));
#endif
}

void braids_update_settings()
{
#ifdef USE_BRAIDS

  braids_mixer_reverb.gain(0, volume_transform(mapfloat(braids_osc.rev_send, EP_REVERB_SEND_MIN, EP_REVERB_SEND_MAX, 0.0, VOL_MAX_FLOAT)));
  braids_mixer_reverb.gain(1, volume_transform(mapfloat(braids_osc.rev_send, EP_REVERB_SEND_MIN, EP_REVERB_SEND_MAX, 0.0, VOL_MAX_FLOAT)));
  braids_stereo_panorama.panorama(mapfloat(braids_osc.pan, PANORAMA_MIN, PANORAMA_MAX, -1.0, 1.0));

  for (uint8_t instance_id = 0; instance_id < NUM_BRAIDS; instance_id++)
  {
    braids_mixer.gain(instance_id, volume_transform(mapfloat(braids_osc.sound_intensity, EP_REVERB_SEND_MIN, EP_REVERB_SEND_MAX, 0.0, VOL_MAX_FLOAT)));
    synthBraids[instance_id]->set_braids_shape(braids_osc.algo);
    synthBraids[instance_id]->set_braids_color(braids_osc.color << 6 );
    synthBraids[instance_id]->set_braids_timbre( (braids_osc.timbre * 2) << 6 );

    braids_envelope[instance_id]->attack(braids_osc.env_attack * 4);
    braids_envelope[instance_id]->decay(braids_osc.env_decay * 4 );
    braids_envelope[instance_id]->sustain(braids_osc.env_sustain / 50.1);
    braids_envelope[instance_id]->release( braids_osc.env_release * braids_osc.env_release );

    braids_mixer_filter[instance_id]->gain(0, 0.0);
    braids_mixer_filter[instance_id]->gain(1, 0.0);
    braids_mixer_filter[instance_id]->gain(2, 0.0);
    braids_mixer_filter[instance_id]->gain(3, 0.0);
    if (braids_osc.filter_mode == 0)
      braids_mixer_filter[instance_id]->gain(3, 1.0);
    else if (braids_osc.filter_mode == 1)
      braids_mixer_filter[instance_id]->gain(0, 1.0);
    else if (braids_osc.filter_mode == 2)
      braids_mixer_filter[instance_id]->gain(1, 1.0);
    else if (braids_osc.filter_mode == 3)
      braids_mixer_filter[instance_id]->gain(2, 1.0);

    if (seq.running == false)
    {
      braids_filter[instance_id]->frequency(braids_osc.filter_freq_from);
      braids_filter[instance_id]->resonance(braids_osc.filter_resonance / 20);
      braids_filter_state[instance_id] = braids_osc.filter_freq_from;
    }
  }

#endif
}
