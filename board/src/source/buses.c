#include "buses.h"

void i2c_init(){
	rscs_i2c_init();
	rscs_i2c_set_scl_rate(1000);
}

void ow_init(){
	rscs_ow_init_bus();
}

void spi_init(){
	rscs_spi_init();
	rscs_spi_set_clk(1000);
	rscs_spi_set_order(RSCS_SPI_ORDER_MSB_FIRST);
}

void spi_set(spi_mode_t mode){
	switch(mode){
	case BMP:
	case ADXL:
	case SD:
		rscs_spi_set_pol(RSCS_SPI_POL_SETUP_FALL_SAMPLE_RISE);
		break;
	case NRF:
		rscs_spi_set_pol(RSCS_SPI_POL_SAMPLE_RISE_SETUP_FALL);
		break;
	}
}
