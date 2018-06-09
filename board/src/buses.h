#ifndef BUSES_H_
#define BUSES_H_

typedef enum{
	ADXL,
	BMP,
	SD,
	NRF
}spi_mode_t;

void i2c_init();
void ow_init();
void spi_init();

void spi_set(spi_mode_t);

#endif /* BUSES_H_ */
