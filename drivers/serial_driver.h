#ifndef SERIAL_DRIVER_H_
#define SERIAL_DRIVER_H_

#include "global.h"
#include <libopencm3/stm32/usart.h>
#include <libopencm3/stm32/dma.h>


int _write(int file, char *ptr, int len);
void serial_config(void);
void serial_put_s(char *data);

#endif
