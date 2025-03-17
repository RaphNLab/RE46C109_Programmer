
#include "drivers/RE46C109_drv.h"
#include "drivers/timer_drv.h"
#include "drivers/uart_drv.h"
#include "drivers/led_driver.h"
#include "drivers/uart_drv.h"


int main(void)
{
	sleep_config();
	re46c109_config();
	led_config();

	uartDevConfig(&myUartDev, MY_USART_DEVICE, uartRxBuffer, uartTxBuffer, USART_DATA_LEN);

	printf("Program Starts\n");

	while (1)
	{
		UartHandleCmd_Task(&myUartDev);

		re46c109_setPrameter(config_reg);
	}
	return (0);
}
