#include "RE46C109_drv.h"
#include "timer_drv.h"

state_t next_state = START;
uint8_t isr_flag = 0;

struct re46c109_reg config_reg =
{
	.ts = 0b1,
	.eol = 0b0,
	.lbh = 0b0,
	.hush = 0b0,
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


static void test_config(void)
{
	rcc_periph_clock_enable(RCC_GPIOA);
	gpio_mode_setup(GPIOA, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE , TEST2_PIN);

	rcc_periph_clock_enable(RCC_GPIOB);
	gpio_mode_setup(GPIOB, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE , (TEST_PIN | IO_PIN));
}

static void feed_config(void)
{
	rcc_periph_clock_enable(RCC_GPIOC);
	gpio_mode_setup(GPIOC, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, FEED_PIN);

	gpio_set_output_options(GPIOC, GPIO_OTYPE_PP, GPIO_OSPEED_40MHZ, FEED_PIN);
}


void re46c109_config(void)
{
	timer3_init();
	test_config();
	feed_config();
}

uint16_t i;
void send_data(void)
{
	static volatile uint64_t mask = (uint64_t)pow((double)2, (double)RE46C109_REG_SIZE);
	uint64_t *data;

	data = (uint64_t *)&config_reg;
	gpio_set(GPIOA, TEST2_PIN);
	for(i = 0; i <= RE46C109_REG_SIZE && isr_flag; i++)
	{
		switch(next_state)
		{
		case START:
			// Do nothing
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
		isr_flag = 0;
	}

	// Register content completely transmitted
	if((mask == 0) && isr_flag)
	{
		gpio_set(GPIOB, IO_PIN);
		sleep_ms(20);
		gpio_clear(GPIOB, IO_PIN);
		gpio_clear(GPIOA, TEST2_PIN);
		sleep_ms(5);
		mask = (uint64_t)pow((double)2, (double)RE46C109_REG_SIZE);
		i = 0;
		isr_flag = 0;
	}
}


void tim3_isr(void)
{
	if (timer_get_flag(TIM3, TIM_SR_UIF))
	{
		timer_clear_flag(TIM3, TIM_SR_UIF);
		isr_flag = 1;
	}
}






