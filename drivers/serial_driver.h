#ifndef SERIAL_DRIVER_H_
#define SERIAL_DRIVER_H_

#include "global.h"
#include <libopencm3/stm32/usart.h>
#include <libopencm3/stm32/dma.h>


int _write(int file, char *ptr, int len);
void uart_setup(void);
void put_s(char *data);

#endif
