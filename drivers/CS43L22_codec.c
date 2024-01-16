#include "CS43L22_codec.h"
#include <libopencm3/stm32/i2s.h>
#include <libopencm3/stm32/usart.h>


void i2c_config(void)
{
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




void codec_init(void)
{

}

void codec_configure(void)
{

}


void codec_generate_MCLK(void)
{

}

uint8_t codec_read(void)
{

}

void codec_write(void)
{

}

void codec_set_volume(uint16_t vol)
{

}

uint32_t codec_get_volume(void)
{

}

void codec_decrease_volume(void)
{

}


void codec_increase_volume(void)
{

}

void codec_set_mute(void)
{

}


