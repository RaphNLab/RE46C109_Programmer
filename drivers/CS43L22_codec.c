#include "CS43L22_codec.h"
//#include <libopencm3/stm32/i2s.h>
#include <libopencm3/stm32/usart.h>
#include <libopencm3/stm32/timer.h>
#include <libopencm3/stm32/i2c.h>
#include "delay_driver.h"

/*
void codec_config(void);
void codec_generate_MCLK(void);
uint8_t codec_read(uint8_t reg);
void codec_write(uint8_t reg, uint8_t val);
void codec_set_volume(int32_t vol);
int32_t codec_get_volume(void);
static void timer3_init(void);*/



void i2c_config(void)
{
	rcc_periph_clock_enable(RCC_GPIOB);
	rcc_periph_clock_enable(RCC_I2C1);
	// Setup SDA and SLC for I2C communication/
	gpio_mode_setup(GPIOB, GPIO_MODE_AF, GPIO_PUPD_NONE, I2C_SCL);
	gpio_mode_setup(GPIOB, GPIO_MODE_AF, GPIO_PUPD_NONE, I2C_SDA);

	//Setup SDA and SCL pin as alternate function. //
	gpio_set_af(GPIOB, GPIO_AF4, I2C_SCL);
	gpio_set_af(GPIOB, GPIO_AF4, I2C_SDA);

	i2c_set_dutycycle(I2C1, I2C_CCR_DUTY);
	i2c_set_speed(I2C1, i2c_speed_sm_100k, 8); //Make better

;	i2c_peripheral_enable(I2C1);
}


void i2s_config(void)
{

}


/*Configure timer 3 to generate PWM*/
static void timer3_init(void)
{
	const uint16_t arr_val = 7;
	rcc_periph_clock_enable(RCC_TIM3);

	timer_set_period(TIM2, arr_val - 1); // Generate timer overflow evet every 1s
	timer_set_prescaler(TIM2, 0); //Timer counter in incremented every 1ms
	timer_set_mode(TIM3, TIM_CR1_CKD_CK_INT, TIM_CR1_CMS_CENTER_1, TIM_CR1_DIR_UP);
	timer_set_oc_mode(TIM3, TIM_OC2, TIM_OCM_PWM2);

	/*Enable capture/compare*/
	timer_enable_oc_output(TIM3, TIM_OC2);
	/*Set duty cycle*/
	timer_set_oc_value(TIM3, TIM_OC2, 200);
	timer_enable_counter(TIM3);
}

static void codec_generate_MCLK(void)
{
	timer3_init();
	rcc_periph_clock_enable(RCC_GPIOC);
	gpio_mode_setup(GPIOC, GPIO_MODE_AF, GPIO_PUPD_NONE, I2S_MCLK);

	gpio_set_output_options(GPIOC, GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ, I2S_MCLK);
	//Setup PC7 as alternate function for TIM3_CH2. //
	gpio_set_af(GPIOC, GPIO_AF2, I2S_MCLK);
}

/*Read a value to the selected register*/
static uint8_t codec_read(uint8_t reg)
{
	uint8_t buffer[2];
	buffer[0] = reg;

	i2c_transfer7(I2C1, CS43L22_CODEC_ID, buffer, 1, (buffer+1), 1);
	return buffer[1];
}

/*Write a value to the selected register*/
static void codec_write(uint8_t reg, uint8_t val)
{
	uint8_t buffer[2];
	buffer[0] = reg;
	buffer[1] = val;
	i2c_transfer7(I2C1, CS43L22_CODEC_ID, buffer, 2, NULL, 0);
}

static void codec_set_volume(int32_t vol)
{
	if(vol < -128)
	{
		codec_write(CS43L22_MASTER_VOL_A, (uint8_t)(vol+256));
	}
	else
	{
		codec_write(CS43L22_MASTER_VOL_A, (uint8_t)vol);
	}
}

static int32_t codec_get_volume(void)
{
	int32_t vol, out_vol;
	uint8_t val;

	val = codec_read(CS43L22_MASTER_VOL_A);
	vol = (int32_t)val;

	if(vol > 24)
	{
		out_vol = -204; // -102 db (Saturated)
		if(vol > 52)
		{
			out_vol = vol -256;
		}
	}
	else
	{
		out_vol = vol;
	}
	return out_vol;
}

void codec_decrease_volume(uint32_t step)
{
	int32_t cur_vol = codec_get_volume();
	const int32_t min_vol = CS43L22_MIN_VOL_DB * 2;// -102dB

	uint32_t n = 0;
	while((cur_vol > min_vol) && (n < step))
	{
		cur_vol--; //0.5dB decrement
		codec_set_volume(cur_vol);
		n++;
	}
}


void codec_increase_volume(uint32_t step)
{
	int32_t cur_vol = codec_get_volume();
	const int32_t max_vol = CS43L22_MAX_VOL_DB * 2;// -102dB

	uint32_t n = 0;
	while((cur_vol < max_vol) && (n < step))
	{
		cur_vol++; //0.5dB decrement
		codec_set_volume(cur_vol);
		n++;
	}
}

void codec_set_mute(bool_t state)
{
	uint8_t val;
	if(state)
	{
		val = 0x01;
	}
	else
	{
		val = 0x00;
	}
	codec_write(CS43L22_PLAYBACK_CTL_1, val);
	//codec_write(CS43L22_PLAYBACK_CTL_2, 0xFA);
}


/*Configure Audio Codec*/
static void codec_config(void)
{
	codec_write(CS43L22_PWR_CTL_1, 0x01); // Power down the codec
	delay_ms(10);
	for(uint8_t i = 0; i < ARR_SIZE(codec_init_arr); i++)
	{
		codec_write(codec_init_arr[i].reg, codec_init_arr[i].val);
	}

	for(uint8_t i = 0; i < ARR_SIZE(codec_i2s_audio); i++)
	{
		codec_write(codec_i2s_audio[i].reg, codec_i2s_audio[i].val);
	}
}



void codec_init(void)
{
	i2c_config();
	codec_config();
	timer3_init();
	codec_generate_MCLK();

	/*Power up the CODEC*/
	codec_write(CS43L22_PWR_CTL_1, 0x9E);
	//codec_write(CS43L22_PWR_CTL_1, 0x00);
	delay_ms(10);
}

void codec_read_chip_id(uint8_t *chip_id)
{
	*chip_id = codec_read(CS43L22_CHIP_ID);
}

