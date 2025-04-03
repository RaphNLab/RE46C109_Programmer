/* Copyright (C) 1883 Thomas Edison - All Rights Reserved
 * You may use, distribute and modify this code under the
 * terms of the XYZ license, which unfortunately won't be
 * written for another century.
 *
 * You should have received a copy of the XYZ license with
 * this file. If not, please write to: , or visit :
 */

#include "RE46C109_drv.h"
#include "timer_drv.h"


/**
 * Private function definition
 **/
static void re46c109_testConfig(void);
static void re46c109_feedConfig(void);
static void re46c109_TESTClock(uint8_t clockAmount);
static void re46c109_sendDataT0(struct re46c109_reg_t configReg);
static void re46c109_sendDataT6(struct re46c109_reg_t configReg);
static void re46c109_initIo(void);

/**
 * Global variable declaration / definition
 * */

state_t next_state = START;
calibration_mode_t calibrationMode = CAL_T1_MODE;
verification_mode_t verificationMode = VERIF_T7_MODE;
bool_t parameterIsrFlag = FALSE;
bool_t smokeCalibrationIsrFalg = FALSE;
sequence_t next_sequence = PARAMETRIC_SELECTION;


/**
 * @brief Initialize configuration register
 * */
struct re46c109_reg_t config_reg =
{
	.ts = 0b1,
	.eol = 0b0,
	.lbh = 0b0,
	.hush = 0b0,
	.ltde = 0b0,
	.lb = 0b010,
	.irc = 0b00,
	.it = 0b11,
	.pagf = 0b00,
	.nl = 0b00000,
	.hyl = 0b00000,
	.hul = 0b00000,
	.ctl = 0b00000,
	.ltd = 0b00000
};


/**
 * @brief Configure FEED, TEST2, IO and HB pins
 * @param NOne
 * @returns None
 * */
static void re46c109_testConfig(void)
{
	rcc_periph_clock_enable(RCC_GPIOA);
	gpio_mode_setup(GPIOA, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, TEST2_PIN);
	gpio_mode_setup(GPIOA, GPIO_MODE_INPUT, GPIO_PUPD_PULLUP, HB_PIN);

	rcc_periph_clock_enable(RCC_GPIOB);
	gpio_mode_setup(GPIOB, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, (TEST_PIN | IO_PIN));
}

/**
 * @brief Configure FEED pin
 * @param NOne
 * @returns None
 * */
static void re46c109_feedConfig(void)
{
	rcc_periph_clock_enable(RCC_GPIOC);
	gpio_mode_setup(GPIOC, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, FEED_PIN);
}


/**
 * @brief Configure the timer, TEST, TEST2, IO, HB, and FEED pins
 * @param None
 * @returns None
 * */
void re46c109_config(void)
{
	timer3_init();
	re46c109_testConfig();
	re46c109_feedConfig();
}


/**
 * @brief Configure the RE46C190 by setting calibration parameters
 * @param configReg struct re46c109_reg_t configuration register for parameter to set
 * @returns None
 */
static void re46c109_sendDataT0(struct re46c109_reg_t configReg)
{
	uint16_t i;
	static volatile uint64_t mask = (uint64_t)pow((double)2, (double)RE46C109_REG_SIZE);
	uint64_t *data;

	data = (uint64_t *)&configReg;
	gpio_set(GPIOA, TEST2_PIN);
	for(i = 0; i <= RE46C109_REG_SIZE && parameterIsrFlag; i++)
	{
		switch(next_state)
		{
		case START:
			/* Do nothing*/
			gpio_clear(GPIOB, TEST_PIN);
			gpio_clear(GPIOC, FEED_PIN);
			next_state = SET_TEST;
			break;
		case SET_TEST:
			if((*data) & mask)
			{
				gpio_set(GPIOB, TEST_PIN);
			}
			else
			{
				gpio_clear(GPIOB, TEST_PIN);
			}
			mask >>= 1;
			next_state = SET_FEED;
			break;
		case SET_FEED:
			gpio_set(GPIOC, FEED_PIN);
			next_state = RESET_FEED;
			break;
		case RESET_FEED:
			gpio_clear(GPIOC, FEED_PIN);
			next_state = RESET_TEST;
			break;
		case RESET_TEST:
			gpio_clear(GPIOB, TEST_PIN);
			next_state = START;
			break;
		default:
			gpio_clear(GPIOB, TEST_PIN);
			gpio_clear(GPIOC, FEED_PIN);
			break;
		}
		parameterIsrFlag = FALSE;
	}

	/*Register content completely transmitted*/
	if((mask == 0) && parameterIsrFlag)
	{
		gpio_set(GPIOB, IO_PIN);
		sleep_ms(20);
		gpio_clear(GPIOB, IO_PIN);
		gpio_clear(GPIOA, TEST2_PIN);
		sleep_ms(5);
		mask = (uint64_t)pow((double)2, (double)RE46C109_REG_SIZE);
		i = 0;
		parameterIsrFlag = FALSE;
	}
}


static void re46c109_sendDataT6(struct re46c109_reg_t configReg)
{
	uint16_t i;
	static volatile uint64_t mask = (uint64_t)pow((double)2, (double)RE46C109_REG_SIZE);
	uint64_t *data;

	data = (uint64_t *)&configReg;
	gpio_set(GPIOA, TEST2_PIN);
	for(i = 0; i <= RE46C109_REG_SIZE && parameterIsrFlag; i++)
	{
		switch(next_state)
		{
		case START:
			/* Do nothing*/
			gpio_clear(GPIOB, TEST_PIN);
			gpio_clear(GPIOC, FEED_PIN);
			next_state = SET_TEST;
			break;
		case SET_TEST:
			if((*data) & mask)
			{
				gpio_set(GPIOB, TEST_PIN);
			}
			else
			{
				gpio_clear(GPIOB, TEST_PIN);
			}
			mask >>= 1;
			next_state = SET_FEED;
			break;
		case SET_FEED:
			gpio_set(GPIOC, FEED_PIN);
			next_state = RESET_FEED;
			break;
		case RESET_FEED:
			gpio_clear(GPIOC, FEED_PIN);
			next_state = RESET_TEST;
			break;
		case RESET_TEST:
			gpio_clear(GPIOB, TEST_PIN);
			next_state = START;
			break;
		default:
			gpio_clear(GPIOB, TEST_PIN);
			gpio_clear(GPIOC, FEED_PIN);
			break;
		}
		parameterIsrFlag = FALSE;
	}

	/*Register content completely transmitted*/
	if((mask == 0) && parameterIsrFlag)
	{
		gpio_set(GPIOB, IO_PIN);
		sleep_ms(20);
		gpio_clear(GPIOB, IO_PIN);
		gpio_clear(GPIOA, TEST2_PIN);
		sleep_ms(5);
		mask = (uint64_t)pow((double)2, (double)RE46C109_REG_SIZE);
		i = 0;
		parameterIsrFlag = FALSE;
	}
}


/**
 * @brief Set and reset TEST pin to jump into a specific calibration mode
 * @param clockAmount uint8_t how many time the clock shall be set. Corresponds
 * to the calibration mode to apply
 * @returns none
**/
static void re46c109_TESTClock(uint8_t clockAmount)
{
	uint8_t i;
	for(i = 0; i < clockAmount; i++)
	{
		gpio_set(GPIOB, TEST_PIN);
		sleep_us(150);
		gpio_clear(GPIOB, TEST_PIN);
		sleep_us(50);
	}
}


static bool_t re46c109_FEEDAdjust(calibration_mode_t mode)
{
	bool_t retVal = FALSE;

	if(mode != CAL_T5_MODE)
	{
		while(!retVal)
		{
			gpio_set(GPIOC, FEED_PIN);
			sleep_us(50);

			if(!gpio_get(GPIOA, HB_PIN))
			{
				/* Store calibration */
				gpio_set(GPIOB, IO_PIN);
				sleep_ms(20);
				gpio_clear(GPIOB, IO_PIN);
				retVal = TRUE;
			}
		}
	}
	else
	{
		while(!retVal)
		{
			gpio_set(GPIOC, FEED_PIN);
			sleep_ms(5);

			if(!gpio_get(GPIOA, HB_PIN))
			{
				/* Store calibration */
				gpio_set(GPIOB, IO_PIN);
				sleep_ms(20);
				gpio_clear(GPIOB, IO_PIN);
				retVal = TRUE;
			}
		}
	}
	return (retVal);
}

void re46c109_smokeCalibrate(void)
{
	/* Set all pins to Vss */
	gpio_clear(GPIOB, (IO_PIN | TEST_PIN));
	gpio_clear(GPIOA, TEST2_PIN);
	gpio_clear(GPIOC, FEED_PIN);


	/* Set TEST2_PIN to Vdd till the end of the calibration*/
	gpio_set(GPIOA, TEST2_PIN);
	/* Setup time*/
	sleep_ms(5);

	while(calibrationMode != CAL_MODE_END)
	{
		switch(calibrationMode)
		{
		case CAL_T1_MODE:
			re46c109_TESTClock(CAL_T1_MODE);
			if(re46c109_FEEDAdjust(calibrationMode))
			{
				calibrationMode = CAL_T2_MODE;
			}
			break;
		case CAL_T2_MODE:
			re46c109_TESTClock(CAL_T2_MODE);
			if(re46c109_FEEDAdjust(calibrationMode))
			{
				calibrationMode = CAL_T4_MODE;
			}
			break;
		case CAL_T3_MODE:
			re46c109_TESTClock(CAL_T3_MODE);
			if(re46c109_FEEDAdjust(calibrationMode))
			{
				calibrationMode = CAL_T4_MODE;
			}
			break;
		case CAL_T4_MODE:
			re46c109_TESTClock(CAL_T4_MODE);
			if(re46c109_FEEDAdjust(calibrationMode))
			{
				calibrationMode = CAL_T5_MODE;
			}
			break;
		case CAL_T5_MODE:
			re46c109_TESTClock(CAL_T5_MODE);

			calibrationMode = CAL_MODE_END;
			break;
		case CAL_MODE_END:
				/* Do nothing */
				/* The Loop shall break */
				break;
		default:
			/* Error Handling */
			gpio_clear(GPIOB, (IO_PIN | TEST_PIN));
			gpio_clear(GPIOA, TEST2_PIN);
			gpio_clear(GPIOC, FEED_PIN);
		}
	}
}


/**
 * @brief Configure the RE46C190 by setting calibration parameters
 * @param configReg struct re46c109_reg_t configuration register for parameter to set
 * @returns None
 */
void re46c109_runModeT0(struct re46c109_reg_t dataT0)
{
	re46c109_sendDataT0(dataT0);
}


void re46c109_runModeT6(struct re46c109_reg_t dataT6)
{
	/*Enter T6 mode by clocking Test 6 times*/
	re46c109_TESTClock(CAL_T6_MODE);
	
	sleep_us(50);
	/* Send and store serial data to EEPROM */
	re46c109_sendDataT6(dataT6);
}




bool_t re46c109_verify(void)
{
	bool_t retVal = FALSE;

	return (retVal);
}





void tim3_isr(void)
{
	if (timer_get_flag(TIM3, TIM_SR_UIF))
	{
		timer_clear_flag(TIM3, TIM_SR_UIF);
		parameterIsrFlag = TRUE;
	}
}