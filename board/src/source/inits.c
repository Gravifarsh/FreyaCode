#include <util/delay.h>

#include "buses.h"
#include "globals.h"

void gps_init(){
	if(gps != NULL) rscs_gps_deinit(gps);
	gps = rscs_gps_init(RSCS_UART_ID_UART1);
}

void cdm_init(){
	if(cdm != NULL) rscs_cdm7160_deinit(cdm);
	cdm = rscs_cdm7160_init(RSCS_CDM7160_ADDR_HIGH);

	rscs_cdm7160_reset(cdm);
	rscs_cdm7160_mode_set(cdm, RSCS_CDM7160_MODE_CONTINUOUS);
}

void bmp_init(){
	if(bmp != NULL) rscs_bmp280_deinit(bmp);
	bmp = rscs_bmp280_initspi(&PORTB, &DDRB, 6);

	rscs_bmp280_parameters_t params;
	params.filter = RSCS_BMP280_FILTER_X16;
	params.pressure_oversampling = RSCS_BMP280_OVERSAMPLING_X4;
	params.temperature_oversampling = RSCS_BMP280_OVERSAMPLING_X4;
	params.standbytyme = RSCS_BMP280_STANDBYTIME_250MS;

	//rscs_spi_set_pol(RSCS_SPI_POL_SETUP_FALL_SAMPLE_RISE);
	rscs_bmp280_setup(bmp, &params);
	rscs_bmp280_changemode(bmp, RSCS_BMP280_MODE_NORMAL);
}

void ds_init(){
	if(ds != NULL) rscs_ds18b20_deinit(ds);
	ds = rscs_ds18b20_init(0x00);
}

void mq7_init(){
	ADMUX = (1 << 6); // VCC AREF
	ADCSRA = (1 << 7) | (1 << 6) | (1 << 5);
}

void geiger_init(){
	EIMSK = (1 << 4); // INT4 interrupt enabled
	EICRB = (1 << 1); // Falling edge
}

void iridium_init(){
	if(iridium != NULL) rscs_iridium9602_deinit(iridium);
	iridium = rscs_iridium9602_init(RSCS_UART_ID_UART0);
}

void sd_init(){
	if(sd != NULL) rscs_sd_deinit(sd);
	//sd = rscs_sd_init(); TODO
}

void ports_init(){
	// All cs output, then down and up
	DDRB = (1 << 0) | // NRF CS
		   (1 << 4) | // NRF CE
		   (1 << 5) | // ADXL CS
		   (1 << 6) | // BMP CS
		   (1 << 7);  // SD CS
	PORTB &= ~((1 << 0) |
			   (1 << 4) |
			   (1 << 5) |
			   (1 << 6) |
			   (1 << 7));
	_delay_us(100);
	PORTB |= (1 << 0) |
			 (1 << 4) |
			 (1 << 5) |
			 (1 << 6) |
			 (1 << 7);

	// CDM MSEL - Output and down to i2c
	DDRD |= (1 << 5);
	PORTD &= ~(1 << 5);

	// IRIDIUM ON/OFF - Output and down to off
	DDRE |= (1 << 3);
	PORTE &= ~(1 << 3);

	// GEIGER - Input
	DDRE &= ~(1 << 4);

	// LED - Output
	DDRG |= (1 << 3);
}
