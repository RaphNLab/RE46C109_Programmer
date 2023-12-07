#ifndef LCD_DRIVER_H_
#define LCD_DRIVER_H_

#include "global.h"
#include <libopencm3/stm32/i2c.h>

#define SDA 				GPIO10
#define SCL 				GPIO9
#define DEV_ID


void i2c1_setup(void);


#endif
