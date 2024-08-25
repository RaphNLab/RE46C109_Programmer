#ifndef FLASH_DRIVER_H_
#define FLASH_DRIVER_H_

#include "global.h"


#define TEST2_PIN GPIO1 // PA1
#define TEST_PIN GPIO3 	// PB3
#define FEED_PIN GPIO7 	// PC7
#define IO_PIN GPIO4 	// PB5

#define RE46C109_REG_SIZE 38U

struct __attribute__((__packed__)) re46c109_reg
{
	uint8_t ts:1;
	uint8_t eol:1;
	uint8_t lbh:1;
	uint8_t hush:1;
	uint8_t lb:3;
	uint8_t irc:2;
	uint8_t it:2;
	uint8_t pagf:2;
	uint8_t nl:5;
	uint8_t hyl:5;
	uint8_t hul:5;
	uint8_t ctl:5;
	uint8_t ltd:5;
};



void re46c109_config(void);
void send_data(void);
#endif
