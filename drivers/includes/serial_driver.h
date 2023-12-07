#ifndef _SERIAL_DRIVER_H_
#define _SERIAL_DRIVER_H_

#include "global.h"
#include <libopencm3/stm32/usart.h>
#include <libopencm3/stm32/dma.h>



void uart_setup(void);
void put_s(char *data);

#endif
