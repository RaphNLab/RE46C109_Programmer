#include "drivers/includes/audio_player.h"
#include "drivers/includes/audio_recorder.h"
#include "drivers/includes/button_driver.h"
#include "drivers/includes/flash_driver.h"
#include "drivers/includes/lcd_driver.h"
#include "drivers/includes/led_driver.h"
#include "drivers/includes/serial_driver.h"


static void clock_setup(void)
{
	rcc_periph_clock_enable(RCC_GPIOA);
	rcc_periph_clock_enable(RCC_GPIOB);
	rcc_periph_clock_enable(RCC_I2C1);
	rcc_periph_clock_enable(RCC_USART2);
}


int main(void)
{

	float temperature = 0.0;
	char temp_buffer[10];

	clock_setup();
	gpio_setup();
	uart_setup();
	//systick_ms_setup();
	i2c1_setup();

	//put_s("Starting program\r\n");

	while (1) {

		gpio_toggle(GPIOB, GPIO3);

		printf(" °C\n");

		msleep(200);
	}
	return 0;
}
