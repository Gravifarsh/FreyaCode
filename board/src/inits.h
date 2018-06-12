/*
 * inits.h
 *
 *  Created on: 08 июня 2018 г.
 *      Author: developer
 */

#ifndef INITS_H_
#define INITS_H_

// Инициализация GPS
void gps_init();

// Инициализация CDM
void cdm_init();

// Инициализация BMP
void bmp_init();

// Инициализация DS
void ds_init();

// Инициализация ADXL
void adxl_init();

// Инициализация MQ7
void mq7_init();

// Инициализация GEIGER
void geiger_init();


// Инициализация IRIDIUM
void iridium_init();

// Инициализация SD
void sd_init();

// Инициализация NRF
void nrf_init();


// Инициализация портов
void ports_init();
// Инициализация TIMESERVICE
void time_init();


#endif /* INITS_H_ */
