/*
 * globals.h
 *
 *  Created on: 08 июня 2018 г.
 *      Author: developer
 */

#ifndef GLOBALS_H_
#define GLOBALS_H_

#include "gps_nmea.h"
#include "sdcard.h"
#include "cdm7160.h"
#include "bmp280.h"
#include "dht22.h"

extern rscs_gps_t* gps;
extern rscs_sdcard_t* sd;
extern rscs_cdm7160_t* cdm;
extern rscs_bmp280_descriptor_t* bmp;
extern rscs_dht22_t* dht;


#endif /* GLOBALS_H_ */
