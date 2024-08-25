#include "timer_drv.h"

#include <libopencm3/stm32/timer.h>

void tim2_isr(void)
{
	if (timer_get_flag(TIM2, TIM_SR_UIF))
	{
		timer_clear_flag(TIM2, TIM_SR_UIF);

	}
}

void sleep_config(void)
{
	//rcc_clock_setup_pll(&rcc_clock_config[RCC_CLOCK_VRANGE1_HSI_RAW_4MHZ]);
	rcc_clock_setup_pll(&rcc_clock_config[RCC_CLOCK_VRANGE1_HSI_PLL_24MHZ]);
	rcc_periph_clock_enable(RCC_TIM2);
	nvic_enable_irq(NVIC_TIM2_IRQ);

	timer_set_period(TIM2, 5000); // Generate timer overflow evet every 2ms
	timer_set_prescaler(TIM2, 24); // Counter increment every 1us
	timer_set_mode(TIM2, TIM_CR1_CKD_CK_INT, TIM_CR1_CMS_EDGE, TIM_CR1_DIR_UP);
	timer_enable_irq(TIM2, TIM_DIER_UIE);
	timer_enable_counter(TIM2);
}

/*Configure timer 3 to generate PWM*/
void timer3_init(void)
{
	rcc_periph_clock_enable(RCC_TIM3);
	timer_set_prescaler(TIM3, 24);
	timer_set_period(TIM3, 120);
	timer_set_mode(TIM3, TIM_CR1_CKD_CK_INT, TIM_CR1_CMS_EDGE, TIM_CR1_DIR_UP);
	timer_set_oc_mode(TIM3, TIM_OC2, TIM_OCM_PWM2);

	/*Set duty cycle*/
	timer_set_oc_value(TIM3, TIM_OC2, 75); // ~25% duty cycle
}

void sleep_us(uint16_t delay_us)
{
	uint16_t start = timer_get_counter(TIM2);
	while((timer_get_counter(TIM2) - start) < delay_us);
}

void sleep_ms(uint16_t delay_ms)
{
	uint16_t i;
	for(i = 0; i < delay_ms; i++)
	{
		sleep_us(1000);
	}
}


