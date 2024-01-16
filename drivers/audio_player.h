#ifndef AUDIO_PLAYER_H_
#define AUDIO_PLAYER_H_


#include "global.h"


void dac_config(void);
void timer_config(void);
void dac_sine_wave(uint8_t channel);
void dac_escalator(uint8_t channel);



#endif
