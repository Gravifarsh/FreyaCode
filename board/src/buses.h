/*
 * buses.h
 *
 *  Created on: 09 июня 2018 г.
 *      Author: developer
 */

#ifndef BUSES_H_
#define BUSES_H_

#include "i2c.h"
#include "spi.h"
#include "onewire.h"
#include "uart.h"

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
