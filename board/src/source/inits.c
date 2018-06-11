#include <util/delay.h>

#include "timeservice.h"
#include "uart.h"

#include "buses.h"
#include "globals.h"

#include "geiger.h"
#include "mq7.h"

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

	spi_set(BMP);

	rscs_bmp280_setup(bmp, &params);
	rscs_bmp280_changemode(bmp, RSCS_BMP280_MODE_NORMAL);
}

void ds_init(){
	if(ds != NULL) rscs_ds18b20_deinit(ds);
	ds = rscs_ds18b20_init(0x00);
}

void adxl_init(){
	if(adxl != NULL) rscs_adxl345_deinit(adxl);
	adxl = rscs_adxl345_initspi(&PORTB, 5);

	spi_set(ADXL);

	rscs_adxl345_startup(adxl);
	rscs_adxl345_set_rate(adxl, RSCS_ADXL345_RATE_200HZ);
	rscs_adxl345_set_range(adxl, RSCS_ADXL345_RANGE_2G);
}

void mq7_init(){
	sides_mq7_init();
}

void geiger_init(){
	sides_geiger_init();
}

void iridium_init(){
	if(iridium != NULL) rscs_iridium9602_deinit(iridium);
	iridium = rscs_iridium9602_init(RSCS_UART_ID_UART0);
}

void sd_init(){
	if(sd != NULL) rscs_sd_deinit(sd);
	sd = rscs_sd_init(&DDRB, &PORTB, (1 << 7));

	rscs_sd_startup(sd);
}

void nrf_init(){
	if(nrf != NULL) rscs_nrf24l01_deinit(nrf);
	nrf = rscs_nrf24l01_init(rscs_spi_do, &PORTB, 0, &PORTB, 4);

	spi_set(NRF);

	rscs_nrf24l01_config_t set;
	rscs_nrf24l01_get_config(&set, nrf);

	set.config.crc0 = 0;
	set.config.en_crc = 1;
	set.config.max_rt = 0;
	set.config.rx_dr = 0;
	set.config.tx_ds = 0;
	set.config.pwr_up = 1;
	set.config.prim_rx = 0;

	set.feature.en_ack_pay = 1;
	set.feature.en_dpl = 1;
	set.feature.en_dyn_ack = 1;

	set.rf_ch.rf_ch = 2;

	set.rf_setup.pll_lock = 0;
	set.rf_setup.rf_dr = 1;
	set.rf_setup.rf_pwr = 3;
	set.rf_setup.lna_hcurr = 1;

	set.setup_aw.aw = 3;

	set.setup_retr.arc = 7;
	set.setup_retr.ard = 7;

	set.tx.addr = 0x1122334455;

	rscs_nrf24l01_set_config(set, nrf);

	rscs_nrf24l01_pipe_config_t pipe;
	pipe.num = 0;
	rscs_nrf24l01_get_pipe_config(&pipe, nrf);

	pipe.en = 1;
	pipe.en_aa = 1;
	pipe.en_dpl = 1;
	pipe.pw = 0;
	pipe.rx_addr = 0x1122334455;

	rscs_nrf24l01_set_pipe_config(pipe, nrf);
}

void time_init(){
	rscs_time_init();
}

void ports_init(){
	// All cs and ce output, then down and up
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
