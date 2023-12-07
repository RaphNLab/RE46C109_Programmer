#include "drivers/includes/led_driver.h"

void gpio_setup(void)
{
	/* Set GPIO6 (in GPIO port B) to 'output push-pull'. */
	gpio_mode_setup(GPIOB, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, GPIO3);
}
