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
		send_data();
		//led_heart_beat(100);
		//led_ring(100, LED_RIGTH);
	}
	return 0;
}
