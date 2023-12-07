#ifndef _GLOBAL_H_
#define _GLOBAL_H_

#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <libopencm3/cm3/nvic.h>
#include <libopencm3/stm32/rcc.h>
#include <libopencm3/stm32/gpio.h>


typedef enum
{
	FALSE = 0,
	TRUE
}bool_t;



#endif
