#include "led_driver.h"
#include "serial_driver.h"
#include "audio_player.h"
#include "audio_recorder.h"
#include "button_driver.h"
#include "flash_driver.h"
#include "lcd_driver.h"

//#include "systick.h"

static void clock_setup(void);

int main(void)
{

	clock_setup();
	gpio_setup();
	uart_setup();
	//systick_ms_setup();
	i2c1_setup();

	put_s("Starting program\r\n");

	while (1) {

		gpio_toggle(GPIOB, GPIO3);

		printf(" °C\n");

		//msleep(200);
	}
	return 0;
}

static void clock_setup(void)
{
	rcc_periph_clock_enable(RCC_GPIOA);
	rcc_periph_clock_enable(RCC_GPIOB);
	rcc_periph_clock_enable(RCC_I2C1);
	rcc_periph_clock_enable(RCC_USART2);
}
