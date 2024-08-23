#include "RE46C109_drv.h"
#include "timer_drv.h"


static void test_config(void)
{
	rcc_periph_clock_enable(RCC_GPIOA);
	gpio_mode_setup(GPIOA, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE , (TEST2_PIN | TEST_PIN | IO_PIN));

	gpio_set_output_options(GPIOC, GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ, (TEST2_PIN | TEST_PIN | IO_PIN));
}

static void feed_config(void)
{
	rcc_periph_clock_enable(RCC_GPIOC);
	gpio_mode_setup(GPIOC, GPIO_MODE_AF, GPIO_PUPD_NONE, FEED_PIN);

	gpio_set_output_options(GPIOC, GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ, FEED_PIN);
	//Setup PC7 as alternate function for TIM3_CH2. //
	gpio_set_af(GPIOC, GPIO_AF2, FEED_PIN);

}

static void start_pwm(void)
{
	timer3_init();
}

static void stop_pwm(void)
{
	timer_disable_oc_output(TIM3, TIM_OC1);
	timer_disable_counter(TIM3);
}

static void generate_feed(void)
{
	start_pwm();
}

void re46c109_config(void)
{
	test_config();
	feed_config();
}

void send_data(void)
{
	uint16_t i;
	volatile uint64_t mask = (uint64_t)pow((double)2, (double)RE46C109_REG_SIZE);
	uint64_t *data;

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

	data = (uint64_t *)&config_reg;

	generate_feed();
	gpio_set(GPIOA, TEST2_PIN);

	for(i = 0; i < RE46C109_REG_SIZE; i++)
	{
		if((*data) & mask)
		{
			gpio_set(GPIOA, TEST_PIN);
		}
		else
		{
			gpio_clear(GPIOA, TEST_PIN);
		}
		mask >>= 1;
		sleep_us(150);
	}
	gpio_clear(GPIOA, TEST_PIN);

	gpio_set(GPIOA, IO_PIN);
	sleep_ms(20);
	gpio_clear(GPIOA, (IO_PIN | TEST2_PIN));
	stop_pwm();
	sleep_ms(5);
}

