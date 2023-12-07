#include "drivers/includes/lcd_driver.h"


void i2c1_setup(void)
{
	// Setup SDA and SLC for I2C communication/
	gpio_mode_setup(GPIOA, GPIO_MODE_AF, GPIO_PUPD_NONE, SCL);
	gpio_mode_setup(GPIOA, GPIO_MODE_AF, GPIO_PUPD_NONE, SDA);

	//Setup SDA and SCL pin as alternate function. //
	gpio_set_af(GPIOA, GPIO_AF4, SCL);
	gpio_set_af(GPIOA, GPIO_AF4, SDA);

	//i2c_enable_stretching(I2C1);
	//i2c_enable_analog_filter(I2C1);
	//i2c_set_7bit_addr_mode(I2C1);
	i2c_set_speed(I2C1,i2c_speed_sm_100k, 8);
	i2c_peripheral_enable(I2C1);
}
