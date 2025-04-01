#include "uart_drv.h"
#include "memory.h"

/**
 * Data type, Constant and macro definitions
 *
*/

<<<<<<< HEAD

/**
 * Data type, Constant and macro definitions
 *
*/

/**
 * Static data declaration
 *
*/
serial_dev_t serial_debug_dev;
serial_dev_t serial_com_dev;
=======
>>>>>>> develop

uint8_t uartRxBuffer[USART_DATA_LEN];
uint8_t uartTxBuffer[USART_DATA_LEN];

UartDev_T myUartDev;
uint8_t isrCnt = 0;

char *uartCmdList[] =
{
	"AT+RUN_T0",  /* Run T0 for calibration */
	"AT+RUN_T1",  /* Run T1 To set Normal Limit */
	"AT+RUN_T2",  /* Run T2 to set Hysteresis */
	"AT+RUN_T3",  /* Run T3 to set hush limitation */
	"AT+RUN_T4",  /* Run T4 to set ch test limitation*/
	"AT+RUN_T5",  /* Run T5 to run LTD baseline */
	"AT+RUN_T6",  /* Run T6 for serial read/write */
	"AT+RUN_T7",  /* Run T7 to perform Norm limitation check*/
	"AT+RUN_T8",  /* Run T8 to perform Hysteresis limitation check */
	"AT+RUN_T9",  /* Run T9 to perform Hush limitation check*/
	"AT+RUN_T10", /* Run T10 to perform Ch Test limitation check */
	"AT+RUN_T11", /* Run T11 to perform Horn test */
	"AT+SMOKE_CALIBRATE" /* Run mode T1 to T5*/
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

/**
 * Private function prototypes
 *
*/

<<<<<<< HEAD
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
=======
/**
 * Public functions
 *
*/
status_t serial_rcv_pkt(serial_dev_t serial_dev, uint16_t size)
{
	status_t err = GLOBAL_ERROR;

	if(size > SERIAL_MAX_SATA_SIZE)
	{
		// Report size too big
	}
	else
	{
		if(serial_dev.serial_receive_done)
		{
			err = GLOBAL_OK;
		}
	}
	return err;
>>>>>>> refs/remotes/origin/main
}

<<<<<<< HEAD

status_t serial_send_pkt(serial_dev_t serial_dev, uint16_t size)
{
	status_t err = GLOBAL_ERROR;
	if(size > SERIAL_MAX_SATA_SIZE)
	{
		// Report size too big
	}
	else
	{
		if(serial_dev.serial_send_done)
		{
			err = GLOBAL_OK;
		}
	}

	return err;
}


void serial_xfer_config(void)
{
	serial_com_dev.uart_port = USART1;
	serial_com_dev.rx_buffer = serial_rx_buffer;
	serial_com_dev.tx_buffer = serial_tx_buffer;
	serial_com_dev.serial_receive_done = FALSE;
	serial_com_dev.serial_send_done = FALSE;

	rcc_periph_clock_enable(RCC_USART1);
	rcc_periph_clock_enable(RCC_GPIOA);
	nvic_enable_irq(NVIC_USART1_IRQ);

	usart_set_parity(serial_com_dev.uart_port , USART_PARITY_NONE);
	usart_set_baudrate(serial_com_dev.uart_port , 115200);
	usart_set_stopbits(serial_com_dev.uart_port , USART_STOPBITS_1);
	usart_set_databits(serial_com_dev.uart_port , 8);
	usart_set_flow_control(serial_com_dev.uart_port , USART_FLOWCONTROL_NONE);
	usart_enable_rx_interrupt(serial_com_dev.uart_port );
	usart_enable_tx_interrupt(serial_com_dev.uart_port );
	usart_set_mode(serial_com_dev.uart_port , USART_MODE_TX_RX);

	gpio_mode_setup(GPIOA, GPIO_MODE_AF , GPIO_PUPD_NONE, (GPIO9 | GPIO10));
	gpio_set_af(GPIOA, GPIO_AF7, (GPIO9 | GPIO10));

	usart_enable(serial_com_dev.uart_port );
}

#ifdef STM32F4
void serial_debug_config(void)
{
	uint8_t text_len = strlen((char*)serial_tx_buffer);
	serial_debug_dev.uart_port = USART2;

	nvic_enable_irq(NVIC_DMA1_STREAM6_IRQ);
	rcc_periph_clock_enable(RCC_DMA1);

	rcc_periph_clock_enable(RCC_USART2);
	rcc_periph_clock_enable(RCC_GPIOA);

	usart_set_parity(USART2, USART_PARITY_NONE);
	usart_set_baudrate(USART2, 9600);
	usart_set_stopbits(USART2, USART_STOPBITS_1);
	usart_set_databits(USART2, 8);
	usart_set_flow_control(USART2, USART_FLOWCONTROL_NONE);

	usart_set_mode(USART2, USART_MODE_TX);
	gpio_mode_setup(GPIOA, GPIO_MODE_AF , GPIO_PUPD_NONE, (GPIO2 | GPIO3));
	gpio_set_af(GPIOA, GPIO_AF7, (GPIO2 | GPIO3));


	/*Configure DMA for data transmission*/
	dma_channel_select(DMA1, DMA_STREAM6, DMA_SxCR_CHSEL_4);
	dma_set_transfer_mode(DMA1, DMA_STREAM6, DMA_SxCR_DIR_MEM_TO_PERIPHERAL);
	dma_set_priority(DMA1, DMA_STREAM6, DMA_SxCR_PL_LOW);
	dma_set_memory_size(DMA1, DMA_STREAM6, DMA_SxCR_MSIZE_8BIT);
	dma_set_peripheral_size(DMA1, DMA_STREAM6, DMA_SxCR_PSIZE_8BIT);
	dma_set_peripheral_address(DMA1, DMA_STREAM6, (uint32_t)SERIAL_DATA_REG);
	dma_set_memory_address(DMA1, DMA_STREAM6, (uint32_t)serial_tx_buffer);
	dma_set_number_of_data(DMA1, DMA_STREAM6, (uint16_t)text_len);

	dma_enable_transfer_complete_interrupt(DMA1, DMA_STREAM6);
	dma_enable_memory_increment_mode(DMA1, DMA_STREAM6);
	dma_enable_circular_mode(DMA1, DMA_STREAM6);


	usart_enable_tx_dma(USART2);
	usart_enable(USART2);
	/*dma_enable_stream(DMA1, DMA_STREAM6); */
}
#endif

/* Print strings on serial port USART2*/
#ifdef STM32F4
void serial_put_s(char *data)
{
	/*while(*data != '\0'){
		usart_send_blocking(USART2, (uint16_t)*data);
		data++;
	}*/
	dma_enable_stream(DMA1, DMA_STREAM6);
}
#endif


<<<<<<< HEAD
status_t serial_send_pkt(serial_dev_t serial_dev, uint16_t size)
{
	status_t err = GLOBAL_ERROR;
	if(size > SERIAL_MAX_SATA_SIZE)
	{
		/* Report size too big */
	}
	else
	{
		if(serial_dev.serial_send_done)
		{
			err = GLOBAL_OK;
		}
	}

	return (err);
}

status_t serial_rcv_pkt(serial_dev_t serial_dev, uint16_t size)
{
	status_t err = GLOBAL_ERROR;

	if(size > SERIAL_MAX_SATA_SIZE)
	{
		/* Report size too big */
	}
	else
	{
		if(serial_dev.serial_receive_done)
		{
			err = GLOBAL_OK;
		}
	}
	return (err);
}

/*void dma1_stream6_isr(void)
{
	if(dma_get_interrupt_flag(DMA1, DMA_STREAM6, DMA_TCIF))
	{
		//Clear flag and Disable DMA
		dma_clear_interrupt_flags(DMA1, DMA_STREAM6, DMA_TCIF);
		dma_disable_stream(DMA1, DMA_STREAM6);
	}
}*/

=======
>>>>>>> refs/remotes/origin/main

/**
 * Private functions
 *
*/
void usart1_isr(void)
{
	/*	Handle data reception	*/
	if(usart_get_flag(serial_com_dev.uart_port, USART_SR_RXNE))
	{

	}

	/* Handle Data transmission */
	if(usart_get_flag(serial_com_dev.uart_port, USART_SR_TXE))
	{

	}
}


/*void dma1_stream6_isr(void)
{
	if(dma_get_interrupt_flag(DMA1, DMA_STREAM6, DMA_TCIF))
	{
		//Clear flag and Disable DMA
		dma_clear_interrupt_flags(DMA1, DMA_STREAM6, DMA_TCIF);
		dma_disable_stream(DMA1, DMA_STREAM6);
	}
}*/

int _write(int file, char *ptr, int len)
{
	int i;

	if (file == STDOUT_FILENO || file == STDERR_FILENO) {
		for (i = 0; i < len; i++) {
			if (ptr[i] == '\n') {
				usart_send_blocking(USART2, '\r');
			}
			usart_send_blocking(USART2, ptr[i]);
		}
		return i;
	}
	errno = EIO;
	return -1;
}



=======


>>>>>>> develop
