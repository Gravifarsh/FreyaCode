#include <stdio.h>
#include <util/delay.h>

#include <avr/interrupt.h>

#include "stdext/stdio.h"
#include "uart.h"

#include "inits.h"
#include "requests.h"
#include "telemtry.h"
#include "buses.h"

#pragma pack(push, 1)
typedef struct{
	uint8_t flag; // Флаг
	uint32_t time; // Время

	// А дальше - один из трёх наборов данных
	union{
		struct{
			int32_t bpress, btemp;
			int16_t dtemp;
			float x, y, z;
		} standard;

		struct{
			uint16_t conc;
			uint16_t mconc;
			uint32_t ticks;
			float lon, lat, h;
			bool fix;
		} advanced;

		struct{
			int32_t bpress, btemp;
			int16_t dtemp;
			float x, y, z;
			uint16_t conc;
			uint16_t mconc;
			uint32_t ticks;
			float lon, lat, h;
			bool fix;
		} full;
	};
} packet_t;
#pragma pack(pop)

int main(){
/* ИНИЦИАЛИЗАЦИЯ */
	sei(); // Включение прерываний

	ports_init(); // Настройка портов
	time_init(); // Инициализация TIMESERVICE

	spi_init(); // Иициализация SPI
	i2c_init(); // Иициализация I2C
	ow_init(); // Иициализация OW

	gps_init(); // Иициализация GPS
	cdm_init(); // Иициализация CDM
	ds_init(); // Иициализация DS
	mq7_init(); // Иициализация MQ7
	geiger_init(); // Иициализация GEIGER
	bmp_init(); // Иициализация BMP
	adxl_init(); // Иициализация ADXL

	sd_init(); // Иициализация SD
	nrf_init(); // Иициализация NRF
#ifndef DEBUG_PRINT
	iridium_init(); // Иициализация IRIDIUM
#endif

	packet_t pack; // Пакет телеметрии

#ifdef DEBUG_PRINT
	PORTE &= ~(1 << 3);

	rscs_uart_bus_t *uart = rscs_uart_init(RSCS_UART_ID_UART0,
												RSCS_UART_FLAG_ENABLE_TX | RSCS_UART_FLAG_BUFFER_TX |
												RSCS_UART_FLAG_ENABLE_RX | RSCS_UART_FLAG_BUFFER_RX);
	rscs_uart_set_character_size(uart, 8);
	rscs_uart_set_baudrate(uart, 19200);
	rscs_uart_set_parity(uart, RSCS_UART_PARITY_NONE);
	rscs_uart_set_stop_bits(uart, RSCS_UART_STOP_BITS_ONE);
	stdin = stdout = rscs_make_uart_stream(uart);
#endif

	while(1){
		/* Генерируем пакет STANDARD */
		pack.flag = 0x57;

		bmp_request(&pack.standard.bpress, &pack.standard.btemp);
		ds_request(&pack.standard.dtemp);
		adxl_request(&pack.standard.x, &pack.standard.y, &pack.standard.z);

		time_request(&pack.time);

#ifdef DEBUG_PRINT
		printf("STANDARD %d\n", pack.flag);
		printf("\tTIME %ld\n", pack.time);
		printf("\tBMP %ld %ld\n", pack.standard.bpress, pack.standard.btemp);
		printf("\tDS %d\n", pack.standard.dtemp);
		printf("\tADXL %f %f %f\n", pack.standard.x, pack.standard.y, pack.standard.z);
		printf("\tSIZE %d\n", sizeof(pack.flag) + sizeof(pack.time) + sizeof(pack.standard));
#endif
		/* Срасываем в NRF */
		nrf_telemetry_drop(&pack, sizeof(pack.flag) + sizeof(pack.time) + sizeof(pack.standard));

		/* Генерируем пакет ADVANCED */
		pack.flag = 0xad;

		cdm_request(&pack.advanced.conc);
		mq7_request(&pack.advanced.mconc);
		geiger_request(&pack.advanced.ticks);
		gps_request(&pack.advanced.lon, &pack.advanced.lat, &pack.advanced.h, &pack.advanced.fix);

		time_request(&pack.time);

#ifdef DEBUG_PRINT
		printf("ADVANCED %d\n", pack.flag);
		printf("\tTIME %ld\n", pack.time);
		printf("\tCDM %u\n", pack.advanced.conc);
		printf("\tMQ7 %d\n", pack.advanced.mconc);
		printf("\tGEIGER %ld\n", pack.advanced.ticks);
		printf("\tGPS %f %f %f %d\n", pack.advanced.lon, pack.advanced.lat, pack.advanced.h, pack.advanced.fix);
		printf("\tSIZE %d\n", sizeof(pack.flag) + sizeof(pack.time) + sizeof(pack.advanced));
#endif
		/* Срасываем в NRF */
		nrf_telemetry_drop(&pack, sizeof(pack.flag) + sizeof(pack.time) + sizeof(pack.advanced));

		/* Генерируем пакет FULL */
		pack.flag = 0xf4;

		bmp_request(&pack.full.bpress, &pack.full.btemp);
		ds_request(&pack.full.dtemp);
		adxl_request(&pack.full.x, &pack.full.y, &pack.full.z);
		cdm_request(&pack.full.conc);
		mq7_request(&pack.full.mconc);
		geiger_request(&pack.full.ticks);
		gps_request(&pack.full.lon, &pack.full.lat, &pack.full.h, &pack.full.fix);

		time_request(&pack.time);

#ifdef DEBUG_PRINT
		printf("FULL %d\n", pack.flag);
		printf("\tTIME %ld\n", pack.time);
		printf("\tBMP %ld %ld\n", pack.full.bpress, pack.full.btemp);
		printf("\tDS %d\n", pack.full.dtemp);
		printf("\tADXL %f %f %f\n", pack.full.x, pack.full.y, pack.full.z);
		printf("\tCDM %u\n", pack.full.conc);
		printf("\tMQ7 %d\n", pack.full.mconc);
		printf("\tGEIGER %ld\n", pack.full.ticks);
		printf("\tGPS %f %f %f %d\n", pack.full.lon, pack.full.lat, pack.full.h, pack.full.fix);
		printf("\tSIZE %d\n", sizeof(pack.flag) + sizeof(pack.time) + sizeof(pack.full));
#endif
		/* Срасываем на SD */
		sd_telemetry_drop(&pack, sizeof(pack.flag) + sizeof(pack.time) + sizeof(pack.full));

		/* Генерируем пакет FULL */
		pack.flag = 0xf4;

		bmp_request(&pack.full.bpress, &pack.full.btemp);
		ds_request(&pack.full.dtemp);
		adxl_request(&pack.full.x, &pack.full.y, &pack.full.z);
		cdm_request(&pack.full.conc);
		mq7_request(&pack.full.mconc);
		geiger_request(&pack.full.ticks);
		gps_request(&pack.full.lon, &pack.full.lat, &pack.full.h, &pack.full.fix);

		time_request(&pack.time);

#ifdef DEBUG_PRINT
		printf("FULL %d\n", pack.flag);
		printf("\tTIME %ld\n", pack.time);
		printf("\tBMP %ld %ld\n", pack.full.bpress, pack.full.btemp);
		printf("\tDS %d\n", pack.full.dtemp);
		printf("\tADXL %f %f %f\n", pack.full.x, pack.full.y, pack.full.z);
		printf("\tCDM %u\n", pack.full.conc);
		printf("\tMQ7 %d\n", pack.full.mconc);
		printf("\tGEIGER %ld\n", pack.full.ticks);
		printf("\tGPS %f %f %f %d\n", pack.full.lon, pack.full.lat, pack.full.h, pack.full.fix);
		printf("\tSIZE %d\n", sizeof(pack.flag) + sizeof(pack.time) + sizeof(pack.full));
#endif
		/* Срасываем в IRIDIUM */
		iridium_telemetry_drop(&pack, sizeof(pack.flag) + sizeof(pack.time) + sizeof(pack.full));
	}
	return 0;
}
