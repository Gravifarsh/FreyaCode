/*
 * globals.h
 *
 *  Created on: 08 июня 2018 г.
 *      Author: developer
 */

#ifndef GLOBALS_H_
#define GLOBALS_H_

#include "gps_nmea.h"
#include "cdm7160.h"
#include "bmp280.h"
#include "ds18b20.h"

#include "sdcard.h"
#include "iridium9602.h"
#include "nrf24l01.h"

extern rscs_gps_t* gps;
extern rscs_cdm7160_t* cdm;
extern rscs_bmp280_descriptor_t* bmp;
extern rscs_ds18b20_t* ds;
//exterm rscs_adxl

extern rscs_sdcard_t* sd;
extern rscs_iridium_t* iridium;
extern rscs_nrf24l01_bus_t* nrf;



#endif /* GLOBALS_H_ */
