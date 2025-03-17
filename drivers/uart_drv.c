#include "uart_drv.h"
#include "memory.h"

/**
 * Data type, Constant and macro definitions
 *
*/


uint8_t uartRxBuffer[USART_DATA_LEN];
uint8_t uartTxBuffer[USART_DATA_LEN];

UartDev_T myUartDev;
uint8_t isrCnt = 0;

char *uartCmdList[] =
{
	"AT+RUN_T0",  /* Run T0 for calibration */
	"AT+RUN_T1",  /* Run T1 To set Norm Limit */
	"AT+RUN_T2",  /* Run T2 to set Hysteresis */
	"AT+RUN_T3",  /* Run T3 to set hush limitation */
	"AT+RUN_T4",  /* Run T4 to set ch test limitation*/
	"AT+RUN_T5",  /* Run T5 to run LTD baseline */
	"AT+RUN_T6",  /* Run T6 for serial read/write */
	"AT+RUN_T7",  /* Run T7 to perform Norm limitation check*/
	"AT+RUN_T8",  /* Run T8 to perform Hysteresis limitation check */
	"AT+RUN_T9",  /* Run T9 to perform Hush limitation check*/
	"AT+RUN_T10", /* Run T10 to perform Ch Test limitation check */
	"AT+RUN_T11"  /* Run T11 to perform Horn test */
};


/**
 * Static data declaration
 *
*/

/**
 * Private function prototypes
 *
*/
static void uartGpioSetup(void);
static void serial_debug_setup(void);




/**
 * @brief Enable and configure GPIO pins used as alternate function for RX and TX pin
 * @param None
 * */
static void uartGpioSetup(void)
{
	rcc_periph_clock_enable(RCC_GPIOA);
	gpio_mode_setup(GPIOA, GPIO_MODE_AF, GPIO_PUPD_NONE, (GPIO2 | GPIO3));
	gpio_set_af(GPIOA, GPIO_AF7, (GPIO2 | GPIO3));
}


/**
 * @brief Configure the serial console by setting the baudrate, databits, RX/TX mode
 * 		  Enable interupt and activate the USART device
 * @param None
 * */
static void serial_debug_setup(void)
{
	rcc_periph_clock_enable(RCC_USART2);

	usart_set_parity(MY_USART_DEVICE, USART_PARITY_NONE);
	usart_set_baudrate(MY_USART_DEVICE, CONSOLE_BAUDRATE);
	usart_set_stopbits(MY_USART_DEVICE, USART_STOPBITS_1);
	usart_set_databits(MY_USART_DEVICE, CONSOLE_DATABIT);
	usart_set_flow_control(MY_USART_DEVICE, USART_FLOWCONTROL_NONE);
	usart_set_mode(MY_USART_DEVICE, USART_MODE_TX_RX);

	/*Enable Interrupt on RX pin*/
	usart_enable_rx_interrupt(MY_USART_DEVICE);
	nvic_enable_irq(NVIC_USART2_IRQ);

	uartGpioSetup();
	usart_enable(MY_USART_DEVICE);
}


/**
 * @brief This function configures the UART device as well as its transmit and receive buffers
 * @param uartDev uartDev UartDev_T* pointer to USART device
 * @param uartBase uint32_t USART base address
 * @param rxBuffer uint8_t* pointer to the receive buffer
 * @param txBuffer uint8_t* pointer to the transmit buffer
 * @param size uint8_t data size
 */

void uartDevConfig(UartDev_T *uartDev, uint32_t uartBase, uint8_t *rxBuffer, uint8_t *txBuffer, uint8_t size)
{
	if(uartDev == NULL)
	{
		/* Report invalid device pointer*/
	}
	else if (txBuffer == NULL)
	{
		/* Report invalid buffer pointer*/
	}
	else if (rxBuffer == NULL)
	{
		/* Report invalid buffer pointer*/
	}
	else
	{
		uartDev->uartBase = uartBase;
		uartDev->uartRxBuffer = rxBuffer;
		uartDev->uartTxBuffer = txBuffer;
		uartDev->uartRxFlag = UART_NO_RX;
		uartDev->uartTxFlag = UART_NO_TX;
		uartDev->size = size;

		serial_debug_setup();
	}
}


/**
 * @brief Task to handle every incomming commands from the console
 * @param uartDev UartDev_T* pointer to uart device
 * */
void UartHandleCmd_Task(UartDev_T *uartDev)
{
	if(uartDev->uartRxFlag == UART_RX_CMP)
	{
		char *args[16];
		char *token;
		uint8_t i;
		for(i = 0; i < 16; i++)
		{
			args[i] = "";
		}
		token = strtok((char*)uartDev->uartRxBuffer, " ");
		i = 0;
		while(token != NULL)
		{
			args[i] = token;
			i++;
			token = strtok(NULL, " ");
		}

		if((strcasecmp(args[0], (const char*)uartCmdList[AT_RUN_T0])) == 0)
		{
			printf("Calibration starts\n");
		}
		else if((strcasecmp(args[0], (const char*)uartCmdList[AT_RUN_T1])) == 0)
		{
			printf("Setting Norm Limitation Start\n");
		}
		else if((strcasecmp(args[0], (const char*)uartCmdList[AT_RUN_T2])) == 0)
		{
			/* Suspend all Task not the uart task to monitor incoming command*/
			printf("Setting Hysteresis Limitation Start\n");
		}
		else if((strcasecmp(args[0], (const char*)uartCmdList[AT_RUN_T3])) == 0)
		{
			printf("Setting Hush Limitation Start\n");
		}
		else if((strcasecmp(args[0], (const char*)uartCmdList[AT_RUN_T4])) == 0)
		{
			printf("Setting CH test Limitation Start\n");
		}
		else if((strcasecmp(args[0], (const char*)uartCmdList[AT_RUN_T5])) == 0)
		{
			printf("LTD BAseline\n");
		}
		else if((strcasecmp(args[0], (const char*)uartCmdList[AT_RUN_T6])) == 0)
		{
			printf("Serial Read/Write\n");
		}
		else if((strcasecmp(args[0], (const char*)uartCmdList[AT_RUN_T7])) == 0)
		{
			printf("Norm Limitation check Start\n");
		}
		else if((strcasecmp(args[0], (const char*)uartCmdList[AT_RUN_T8])) == 0)
		{
			printf("Hysteresis Limitation check Start\n");
		}
		else if((strcasecmp(args[0], (const char*)uartCmdList[AT_RUN_T9])) == 0)
		{
			printf("Hush Limitation check Start\n");
		}
		else if((strcasecmp(args[0], (const char*)uartCmdList[AT_RUN_T11])) == 0)
		{
			printf("Ch Test limitation check Start\n");
		}
		else if((strcasecmp(args[0], (const char*)uartCmdList[AT_RUN_T11])) == 0)
		{
			printf("Horn test start\n");
		}
		else
		{
			/* Report invalid command*/
			printf("Invalid CMD %s\n", uartDev->uartRxBuffer);
		}
		/* Reset the flag*/
		uartDev->uartRxFlag = UART_NO_RX;
		memErase(uartDev->uartRxBuffer, uartDev->size);
	}
}


/**
 * @brief USART2 interrupt service routine handling every incoming byte from the console
 * 	      Once a command is fully received a flag is set to notify.
 * */
void usart2_isr(void)
{
	volatile uint8_t rcv_char = '\0';
	if(usart_get_flag(myUartDev.uartBase, USART_SR_RXNE))
	{
		rcv_char = usart_recv(myUartDev.uartBase);

		if(rcv_char != '\r')
		{
			myUartDev.uartRxBuffer[isrCnt] = rcv_char;
			isrCnt++;
		}
		else /* If the enter character is received*/
		{
			myUartDev.uartRxFlag = UART_RX_CMP;
			isrCnt = 0;
		}
	}
}


/**
 * Private functions
 *
*/

int _write(int file, char *ptr, int len)
{
	int i;

	if (file == STDOUT_FILENO || file == STDERR_FILENO) {
		for (i = 0; i < len; i++) {
			if (ptr[i] == '\n') {
				usart_send_blocking(MY_USART_DEVICE, '\r');
			}
			usart_send_blocking(MY_USART_DEVICE, ptr[i]);
		}
		return (i);
	}
	errno = EIO;
	return (-1);
}



