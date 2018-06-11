#include <stdio.h>
#include <util/delay.h>

#include <avr/interrupt.h>

#include <stdext/stdio.h>
#include "uart.h"

#include "inits.h"
#include "requests.h"
#include "telemtry.h"
#include "buses.h"

#pragma pack(push, 1)
typedef struct{
	int32_t bpress, btemp;
	int16_t dtemp;
	uint16_t conc;
	uint16_t mconc;
	uint32_t ticks;
	float x, y, z;
	float lon, lat, h;
	bool hasFix;

	uint32_t time;
} packet_t;
#pragma pack(pop)

int main(){
	sei();

	ports_init();
	time_init();

	spi_init();
	i2c_init();
	ow_init();

	gps_init();
	cdm_init();
	ds_init();
	mq7_init();
	geiger_init();
	bmp_init();
	adxl_init();

	nrf_init();

	packet_t pack;

	rscs_uart_bus_t *uart = rscs_uart_init(RSCS_UART_ID_UART0,
												RSCS_UART_FLAG_ENABLE_TX | RSCS_UART_FLAG_BUFFER_TX);
	rscs_uart_set_character_size(uart, 8);
	rscs_uart_set_baudrate(uart, 19200);
	rscs_uart_set_parity(uart, RSCS_UART_PARITY_NONE);
	rscs_uart_set_stop_bits(uart, RSCS_UART_STOP_BITS_ONE);
	stdin = stdout = rscs_make_uart_stream(uart);

	while(1){
		bmp_request(&pack.bpress, &pack.btemp);
		ds_request(&pack.dtemp);
		cdm_request(&pack.conc);
		mq7_request(&pack.mconc);
		geiger_request(&pack.ticks);
		gps_request(&pack.lon, &pack.lat, &pack.h, &pack.hasFix);
		adxl_request(&pack.x, &pack.y, &pack.z);

		time_request(&pack.time);

		printf("BMP %ld %ld\n", pack.bpress, pack.btemp);
		printf("DS %d\n", pack.dtemp);
		printf("CDM %d\n", pack.conc);
		printf("MQ7 %d\n", pack.mconc);
		printf("GEIGER %ld\n", pack.ticks);
		printf("GPS %f %f %f %d\n", pack.lon, pack.lat, pack.h, pack.hasFix);
		printf("ADXL %f %f %f\n", pack.x, pack.y, pack.z);

		printf("TIME %ld\n", pack.time);

		char str[] = "OMG IM STRING THAT WILL BE ON SD! ^^";
		printf("WRITED %d\n\n\n", sd_telemetry_drop(str, sizeof(str)));
		//printf("WRITED NRF %d\n\n\n", nrf_telemetry_drop(&pack, sizeof(pack)));

		_delay_ms(1000);
	}
	return 0;
}
