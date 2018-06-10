#include <stdio.h>

#include <avr/interrupt.h>

#include <stdext/stdio.h>
#include "uart.h"

#include "inits.h"
#include "requests.h"
#include "buses.h"

int main(){
	sei();

	ports_init();
	time_init();

	spi_init();
	i2c_init();
	ow_init();

	cdm_init();
	ds_init();
	mq7_init();
	geiger_init();
	gps_init();

	spi_set(BMP);
	bmp_init();


	int32_t bpress, btemp;
	int16_t dtemp;
	uint16_t conc;
	uint16_t mconc;
	uint32_t ticks;
	float lon, lat, h;
	bool hasFix;

	uint32_t time;

	rscs_uart_bus_t *uart = rscs_uart_init(RSCS_UART_ID_UART0,
												RSCS_UART_FLAG_ENABLE_TX | RSCS_UART_FLAG_BUFFER_TX |
												RSCS_UART_FLAG_ENABLE_RX | RSCS_UART_FLAG_BUFFER_RX);
	rscs_uart_set_character_size(uart, 8);
	rscs_uart_set_baudrate(uart, 19200);
	rscs_uart_set_parity(uart, RSCS_UART_PARITY_NONE);
	rscs_uart_set_stop_bits(uart, RSCS_UART_STOP_BITS_ONE);
	stdin = stdout = rscs_make_uart_stream(uart);

	while(1){
		bmp_request(&bpress, &btemp);
		ds_request(&dtemp);
		cdm_request(&conc);
		mq7_request(&mconc);
		geiger_request(&ticks);
		gps_request(&lon, &lat, &h, &hasFix);

		time_request(&time);

		printf("BMP %ld %ld\n", bpress, btemp);
		printf("DS %d\n", dtemp);
		printf("CDM %d\n", conc);
		printf("MQ7 %d\n", mconc);
		printf("GEIGER %ld\n", ticks);
		printf("GPS %f %f %f %d\n", lon, lat, h, hasFix);

		printf("TIME %ld\n\n\n", time);
	}



	return 0;
}
