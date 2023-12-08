#ifndef DELAY_DRIVER_
#define DELAY_DRIVER_

#include "global.h"
#include <libopencm3/stm32/timer.h>


void delay_config(void);
void delay_ms(uint16_t delay);

#endif
