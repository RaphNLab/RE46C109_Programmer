#include "global.h"



#include <libopencm3/stm32/i2c.h>
#include "systick.h"




#define DEVICE_ID_REG 		0x01
#define T_H_LIMIT_REG 		0x02
#define T_L_LIMIT_REG 		0x03
#define CTRL_REG 			0x04
#define STSTUS_REG 			0x05
#define DATA_T_L_REG  		0x06
#define DATA_T_H_REG 		0x07
#define SOFT_RESET_REG 		0x0C

#define DEVICE_ID_MAN 		0xA0

#define TEMP_SENSOR_ADDR0 	0x38
#define TEMP_SENSOR_ADDR1 	0x3F
#define SW_RESET 			2

/* Available Output data rate */
#define ODR_25_HZ  			0x0F
#define	ODR_50_HZ  			0x1F
#define	ODR_100_HZ 			0x2F
#define ODR_200_HZ 			0x3F

//#define FREERUN (1 << 2)
//#define BDU (1 << 6)

typedef struct {
	uint8_t data_t_h;
	uint8_t data_t_l;
}TEMP_RAW_DATA_t;


/* Status register */
typedef enum {
	BUSY= 0,
	UNDER_TLL,
	OVER_THL,
	MAX_STATUS
}STATUS_t;

/* Control register bits
 * For ODR, see macro on the top
 * */
typedef enum {
	ONE_SHOT = 0,
	FREERUN = 2,
	IF_ADD_INC = 3,
	BDU = 6,
	MAX_CTRL = 7
}CTRL_t;

typedef enum {
	TIDS_ERROR = 0,
	TIDS_OK
}RESULT_t;

static void clock_setup(void)
{
	rcc_periph_clock_enable(RCC_GPIOA);
	rcc_periph_clock_enable(RCC_GPIOB);
	rcc_periph_clock_enable(RCC_I2C1);
	rcc_periph_clock_enable(RCC_USART2);
}

static void uart_setup(void)
{
	usart_enable(USART2);
	// Set USART2 even parity
	usart_set_parity(USART2, USART_PARITY_EVEN);
	//SEt baudrate to 115200
	usart_set_baudrate(USART2, 115200);
	//Set stopbit t0 1
	usart_set_stopbits(USART2, USART_STOPBITS_1);
	// Set databit to 8
	usart_set_databits(USART2, 8);

	// Set mode TX
	usart_set_mode(USART2, USART_MODE_TX);
	gpio_mode_setup(GPIOA, GPIO_MODE_AF , GPIO_PUPD_NONE, GPIO2);
	gpio_set_af(GPIOA, GPIO_AF7, GPIO2);

}



static void gpio_setup(void)
{
	/* Set GPIO6 (in GPIO port B) to 'output push-pull'. */
	gpio_mode_setup(GPIOB, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, GPIO3);
}





/* Print strings on serial port USART2*/
static void put_s(char *data)
{
	while(*data != '\0'){
		usart_send_blocking(USART2, *data);
		data++;
	}
}

uint8_t data_buffer[4];


static uint8_t read_device_ID(void)
{
	RESULT_t status = TIDS_ERROR;

	msleep(15);
	data_buffer[0] = DEVICE_ID_REG;

	i2c_transfer7(I2C1, TEMP_SENSOR_ADDR0, data_buffer, 1, (data_buffer+1), 1);

	if (data_buffer[1] == DEVICE_ID_MAN){
		put_s("Device ID OK\r\n");
		put_s("Communication successful\r\n");
		status = TIDS_OK;
	}else{
		put_s("Wrong device ID\r\n");
		put_s("No communication possible\r\n");
		status = TIDS_ERROR;
	}
	return status;
}

/* Set temperature maximum and minimum threshold value
 * Max Threshold Temperature = (T_H_LIMIT - 63) ×0.64 °C
 * Min Threshold Temperature = (T_L_LIMIT - 63) ×0.64 °C
 *
 * 1 		<= threshold <= 255
 * -39.68°C <= threshold <= 122.88°C
 *
 *  */
static void set_temp_limit(uint8_t h_limit, uint8_t l_limit)
{

	if (read_device_ID()) {
		/* Set maximum Temperature threshold*/
		data_buffer[0] = T_H_LIMIT_REG;
		data_buffer[1] = h_limit;
		i2c_transfer7(I2C1, TEMP_SENSOR_ADDR0, data_buffer, 2, NULL, 0);


		/* Set minimum Temperature threshold*/
		data_buffer[0] = T_L_LIMIT_REG;
		data_buffer[1] = l_limit;
		i2c_transfer7(I2C1, TEMP_SENSOR_ADDR0, data_buffer, 2, NULL, 0);
	}

}

/* Set sensor into power down mode */
static void power_down_mode(void)
{
	/* This set the FREERUN bit of the CTRL register to 0*/
	data_buffer[0] =  CTRL_REG;
	i2c_transfer7(I2C1, TEMP_SENSOR_ADDR0, data_buffer, 1, (data_buffer+1), 1);
	data_buffer[1] &= ~(1 << FREERUN);
	i2c_transfer7(I2C1, TEMP_SENSOR_ADDR0, data_buffer, 2, NULL, 0);
}

/* Set sensor into single conversion mode */
static uint8_t single_mode(void)
{

	STATUS_t status = BUSY;
	RESULT_t rt = TIDS_ERROR;

	if(read_device_ID()){
		/* Enable software reset */
		data_buffer[0] = SOFT_RESET_REG;
		data_buffer[1] = (1 << SW_RESET);
		i2c_transfer7(I2C1, TEMP_SENSOR_ADDR0, data_buffer, 2, NULL, 0);

		/* This disabels the SW reset */
		data_buffer[0] =  SOFT_RESET_REG;
		i2c_transfer7(I2C1, TEMP_SENSOR_ADDR0, data_buffer, 1, (data_buffer+1), 1);
		data_buffer[1] &= ~(1 << SW_RESET);
		i2c_transfer7(I2C1, TEMP_SENSOR_ADDR0, data_buffer, 2, NULL, 0);


		/* Enable continuous mode  set FREERUN and BDU to 1*/
		data_buffer[0] = CTRL_REG;
		data_buffer[1] = (1 << ONE_SHOT);
		i2c_transfer7(I2C1, TEMP_SENSOR_ADDR0, data_buffer, 2, NULL, 0);

		/* Read sensor status */
		data_buffer[0] =  STSTUS_REG;
		i2c_transfer7(I2C1, TEMP_SENSOR_ADDR0, data_buffer, 1, (data_buffer+1), 1);

		if(data_buffer[1] == status)
		{
			put_s("Single mode activate \r\n");
			rt = TIDS_OK;
			return rt;
		}
		else{
			put_s("Device busy \r\n");
			rt = TIDS_ERROR;
			return rt;
		}
	}

}

/* Set sensor into continuous mode*/
static void continuous_mode(uint8_t odr)
{
	uint8_t tmp = 0;

	if(read_device_ID()){
		/* Enable software reset */
		data_buffer[0] = SOFT_RESET_REG;
		data_buffer[1] = (1 << SW_RESET);
		i2c_transfer7(I2C1, TEMP_SENSOR_ADDR0, data_buffer, 2, NULL, 0);

		/* This disabels the SW reset */
		data_buffer[0] =  SOFT_RESET_REG;
		i2c_transfer7(I2C1, TEMP_SENSOR_ADDR0, data_buffer, 1, (data_buffer+1), 1);
		data_buffer[1] &= ~(1 << SW_RESET);
		i2c_transfer7(I2C1, TEMP_SENSOR_ADDR0, data_buffer, 2, NULL, 0);


		/* Enable continuous mode  set FREERUN and BDU to 1*/
		data_buffer[0] = CTRL_REG;
		data_buffer[1] = ((1 << FREERUN) | (1 << BDU));
		i2c_transfer7(I2C1, TEMP_SENSOR_ADDR0, data_buffer, 2, NULL, 0);


		/* SET output data rate */
		data_buffer[0] = CTRL_REG;
		i2c_transfer7(I2C1, TEMP_SENSOR_ADDR0, data_buffer, 1, (data_buffer+1), 1);
		tmp = data_buffer[1];
		data_buffer[1] &= odr;
		data_buffer[1] |= tmp;
		i2c_transfer7(I2C1, TEMP_SENSOR_ADDR0, data_buffer, 2, NULL, 0);
	}
}

static void read_temperature(float *temperature)
{
	uint8_t temperature_l_data = 0;
	uint8_t temperature_h_data = 0;
	uint16_t raw_temperature = 0;

	/* Read Temperature L data */
	data_buffer[0] = DATA_T_L_REG;
	i2c_transfer7(I2C1, TEMP_SENSOR_ADDR0, data_buffer, 1, (data_buffer+1), 1);
	temperature_l_data = data_buffer[1];

	/* Read Temperature H data */
	data_buffer[0] =  DATA_T_H_REG;
	i2c_transfer7(I2C1, TEMP_SENSOR_ADDR0, data_buffer, 1, (data_buffer+1), 1);
	temperature_h_data = data_buffer[1];

	raw_temperature = (uint16_t)((temperature_h_data << 8) | temperature_l_data);

	*temperature = raw_temperature / 100.0;
}

int main(void)
{

	float temperature = 0.0;
	char temp_buffer[10];

	clock_setup();
	gpio_setup();
	uart_setup();
	systick_ms_setup();
	i2c1_setup();

	put_s("Starting program\r\n");

	continuous_mode(ODR_50_HZ);
	while (1) {

		gpio_toggle(GPIOB, GPIO3);

		read_temperature(&temperature);
		printf("Temperature = %.2lf °C\n", temperature);

		msleep(200);
	}

	return 0;
}
