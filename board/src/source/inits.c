/*
 * inits.c
 *
 *  Created on: 08 июня 2018 г.
 *      Author: developer
 */
#include "globals.h"

void gps_init(){
	if(gps != NULL) rscs_gps_deinit(gps);
	gps = rscs_gps_init(RSCS_UART_ID_UART1);
}

void sd_init(){
	//TODO
}

void cdm_init(){
	if(cdm != NULL) //TODO CDM deinit
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

	rscs_spi_set_pol(RSCS_SPI_POL_SETUP_FALL_SAMPLE_RISE);
	rscs_bmp280_setup(bmp, &params);
	rscs_bmp280_changemode(bmp, RSCS_BMP280_MODE_NORMAL);
}

void ds_init(){
	if(ds != NULL) ds = rscs_ds18b20_deinit(ds);
	ds = rscs_ds18b20_init(0x00);

	rscs_ds18b20_start_conversion(ds);
}

void iridium_init(){
	if(iridium != NULL) rscs_iridium9602_deinit(iridium);
	iridium = rscs_iridium9602_init(RSCS_UART_ID_UART0);
}

