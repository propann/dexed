#ifndef _MICROSYNTH_H_
#define _MICROSYNTH_h_

#include "Arduino.h"

void update_microsynth_params();
void microsynth_update_all_settings(uint8_t instance_id);
void microsynth_update_single_setting(uint8_t instance_id);

#endif