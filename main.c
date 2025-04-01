<<<<<<< HEAD

#include "drivers/RE46C109_drv.h"
=======
>>>>>>> refs/remotes/origin/main
#include "drivers/timer_drv.h"
#include "drivers/uart_drv.h"
#include "drivers/led_driver.h"
<<<<<<< HEAD
#include "drivers/button.h"

int main(void)
{
	timer_sleep_config();
	timer_debounce_config();

	button_config();
	led_config();

	while (1)
	{
<<<<<<< HEAD
=======
#include "drivers/uart_drv.h"


int main(void)
{
	sleep_config();
	re46c109_config();
	led_config();

	uartDevConfig(&myUartDev, MY_USART_DEVICE, uartRxBuffer, uartTxBuffer, USART_DATA_LEN);


	while (1)
	{
		UartHandleCmd_Task(&myUartDev);

>>>>>>> develop
		re46c109_setPrameter(config_reg);
=======
		if(button_debounce(GREEN_BUTTON))
		{
			//gpio_set(GPIOA, GPIO5);
			gpio_clear(GPIOA, GPIO5);
			//gpio_toggle(GPIOA, GPIO5);
		}
		else if (button_debounce(GREEN_BUTTON))
		{
			//gpio_clear(GPIOA, GPIO5);
			gpio_set(GPIOA, GPIO5);
		}
>>>>>>> refs/remotes/origin/main
	}
	return (0);
}
