
#include "drivers/RE46C109_drv.h"
#include "drivers/timer_drv.h"
#include "drivers/uart_drv.h"
#include "drivers/led_driver.h"

int main(void)
{
	sleep_config();

	re46c109_config();
	led_config();
	while (1)
	{
		re46c109_setPrameter(config_reg);
	}
	return (0);
}
