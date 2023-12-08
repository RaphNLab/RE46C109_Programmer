#include "serial_driver.h"


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


void serial_config(void)
{
	rcc_periph_clock_enable(RCC_USART2);
	rcc_periph_clock_enable(RCC_GPIOA);
	nvic_enable_irq(NVIC_USART2_IRQ );


	// Set USART2 even parity
	usart_set_parity(USART2, USART_PARITY_EVEN);
	//SEt baudrate to 115200
	usart_set_baudrate(USART2, 115200);
	//Set stopbit t0 1
	usart_set_stopbits(USART2, USART_STOPBITS_1);
	// Set databit to 8
	usart_set_databits(USART2, 8);

	usart_enable_rx_interrupt(USART2);

	// Set mode TX
	usart_set_mode(USART2, USART_MODE_TX);
	gpio_mode_setup(GPIOA, GPIO_MODE_AF , GPIO_PUPD_NONE, GPIO2);
	gpio_set_af(GPIOA, GPIO_AF7, GPIO2);

	usart_enable(USART2);
}


/* Print strings on serial port USART2*/
void serial_put_s(char *data)
{
	while(*data != '\0'){
		usart_send_blocking(USART2, *data);
		data++;
	}
}


