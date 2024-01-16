#include "audio_player.h"
#include <libopencm3/stm32/dac.h>
#include <libopencm3/stm32/dma.h>
#include <libopencm3/stm32/timer.h>

const uint16_t sine_wave[32] =
		{
			2047, 2447, 2831, 3185, 3498, 3750, 3939, 4056, 4095, 4056,
			3939, 3750, 3495, 3185, 2831, 2447, 2047, 1647, 1263, 909,
			599, 344, 155, 38, 0, 38, 155, 344, 599, 909, 1263, 1647
		};

const uint8_t escalator[8] =
		{
			0x0, 0x33, 0x66, 0x99, 0xCC, 0xFF
		};




/*Configure the DAC for convertion*/
void dac_config(void)
{
	/* Port A and C are on AHB1 */
	rcc_periph_clock_enable(RCC_GPIOA);
	rcc_periph_clock_enable(RCC_GPIOC);
	/* Set the digital test output on PC1 */
	gpio_mode_setup(GPIOC, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, GPIO1);
	gpio_set_output_options(GPIOC, GPIO_OTYPE_PP, GPIO_OSPEED_2MHZ, GPIO1);
	/* Set PA4 for DAC channel 1 to analogue, ignoring drive mode. */
	gpio_mode_setup(GPIOA, GPIO_MODE_ANALOG, GPIO_PUPD_NONE, GPIO4);

	/* Enable the DAC clock on APB1 */
	rcc_periph_clock_enable(RCC_DAC);
	/* Setup the DAC channel 1, with timer 2 as trigger source.
	 * Assume the DAC has woken up by the time the first transfer occurs */
	dac_trigger_enable(DAC1, DAC_CHANNEL1);
	dac_set_trigger_source(DAC1, DAC_CR_TSEL1_T2);

	rcc_periph_clock_enable(RCC_DMA1);
	nvic_enable_irq(NVIC_DMA1_STREAM5_IRQ);
	dma_stream_reset(DMA1, DMA_STREAM5);
	dma_set_priority(DMA1, DMA_STREAM5, DMA_SxCR_PL_LOW);
	dma_set_memory_size(DMA1, DMA_STREAM5, DMA_SxCR_MSIZE_8BIT);
	dma_set_peripheral_size(DMA1, DMA_STREAM5, DMA_SxCR_PSIZE_8BIT);
	dma_enable_memory_increment_mode(DMA1, DMA_STREAM5);
	dma_enable_circular_mode(DMA1, DMA_STREAM5);
	dma_set_transfer_mode(DMA1, DMA_STREAM5, DMA_SxCR_DIR_MEM_TO_PERIPHERAL);
	/* The register to target is the DAC1 8-bit right justified data
	   register */
	dma_set_peripheral_address(DMA1, DMA_STREAM5, (uint32_t) &DAC_DHR8R1(DAC1));
	/* The array v[] is filled with the waveform data to be output */
	dma_set_memory_address(DMA1, DMA_STREAM5, (uint32_t)sine_wave);
	dma_set_number_of_data(DMA1, DMA_STREAM5, 32);
	dma_enable_transfer_complete_interrupt(DMA1, DMA_STREAM5);
	dma_channel_select(DMA1, DMA_STREAM5, DMA_SxCR_CHSEL_7);
	dma_enable_stream(DMA1, DMA_STREAM5);

	dac_dma_enable(DAC1, DAC_CHANNEL1);
	dac_enable(DAC1, DAC_CHANNEL1);
}

/*Configure the timer to trigger the DAC to start convertion each second*/
void timer_config(void)
{
	rcc_clock_setup_pll(&rcc_hse_8mhz_3v3[RCC_CLOCK_3V3_84MHZ]);

	rcc_periph_clock_enable(RCC_TIM2);
	rcc_periph_reset_pulse(RST_TIM2);
	/* Timer global mode: - No divider, Alignment edge, Direction up */
	timer_set_mode(TIM2, TIM_CR1_CKD_CK_INT,
			   TIM_CR1_CMS_EDGE, TIM_CR1_DIR_UP);
	timer_continuous_mode(TIM2);
	timer_set_period(TIM2, 1152);
	timer_disable_oc_output(TIM2, TIM_OC2 | TIM_OC3 | TIM_OC4);
	timer_enable_oc_output(TIM2, TIM_OC1);
	timer_disable_oc_clear(TIM2, TIM_OC1);
	timer_disable_oc_preload(TIM2, TIM_OC1);
	timer_set_oc_slow_mode(TIM2, TIM_OC1);
	timer_set_oc_mode(TIM2, TIM_OC1, TIM_OCM_TOGGLE);
	timer_set_oc_value(TIM2, TIM_OC1, 500);
	timer_disable_preload(TIM2);
	/* Set the timer trigger output (for the DAC) to the channel 1 output
	   compare */
	timer_set_master_mode(TIM2, TIM_CR2_MMS_COMPARE_OC1REF);
	timer_enable_counter(TIM2);
}

/*Sine wave Output */
void dac_sine_wave(uint8_t channel)
{

}

/*Escalator wave Output */
void dac_escalator(uint8_t channel)
{

}


void dma1_stream5_isr(void)
{
	if (dma_get_interrupt_flag(DMA1, DMA_STREAM5, DMA_TCIF)) {
		dma_clear_interrupt_flags(DMA1, DMA_STREAM5, DMA_TCIF);
		/* Toggle PC1 just to keep aware of activity and frequency. */
		gpio_toggle(GPIOC, GPIO1);
	}
}

