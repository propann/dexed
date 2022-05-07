
#include "microsynth.h"
#include "config.h"
#include <Audio.h>
#include "template_mixer.hpp"
#include "effect_stereo_panorama.h"

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
extern float volume_transform(float amp);
extern short wave_type[9];

void microsynth_update_settings(uint8_t instance_id)
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
}
