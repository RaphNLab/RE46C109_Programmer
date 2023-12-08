#include "delay_driver.h"

void tim2_isr(void)
{
	if (timer_get_flag(TIM2, TIM_SR_UIF))
	{
		timer_clear_flag(TIM2, TIM_SR_UIF);

	}
}

void delay_config(void)
{
	// Timer 2 is connected to APB1 that uses half frequency of ABP2
	rcc_clock_setup_pll(&rcc_hse_8mhz_3v3[RCC_CLOCK_3V3_84MHZ]);

	rcc_periph_clock_enable(RCC_TIM2);
	nvic_enable_irq(NVIC_TIM2_IRQ);

	timer_set_period(TIM2, 1000); // Generate timer overflow evet every 1s
	timer_set_prescaler(TIM2, 42000); //Timer counter in incremented every 1ms
	timer_set_mode(TIM2, TIM_CR1_CKD_CK_INT, TIM_CR1_CMS_EDGE, TIM_CR1_DIR_UP);
	timer_enable_irq(TIM2, TIM_DIER_UIE);
	timer_enable_counter(TIM2);
}

void delay_ms(uint16_t delay)
{
	timer_set_counter(TIM2, 0);
	while(timer_get_counter(TIM2) < (delay * 2));
}
