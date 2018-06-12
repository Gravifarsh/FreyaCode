#ifndef BUSES_H_
#define BUSES_H_

// Режимы шины SPI
typedef enum{
	ADXL,
	BMP,
	SD,
	NRF
}spi_mode_t;

// Инициализация I2C
void i2c_init();
// Инициализация OW
void ow_init();
// Инициализация SPI
void spi_init();

// Установка режима шины SPI
void spi_set(spi_mode_t);

#endif /* BUSES_H_ */
