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
#include "adxl345.h"

#include "sdcard.h"
#include "iridium9602.h"
#include "nrf24l01.h"

extern rscs_gps_t* gps; // GPS
extern rscs_cdm7160_t* cdm; // CDM
extern rscs_bmp280_descriptor_t* bmp; // BMP
extern rscs_ds18b20_t* ds; // DS
extern rscs_adxl345_t* adxl; // ADXL

extern rscs_sdcard_t* sd; // SD
extern rscs_iridium_t* iridium; // IRIDIUM
extern rscs_nrf24l01_bus_t* nrf; // NRF



#endif /* GLOBALS_H_ */
