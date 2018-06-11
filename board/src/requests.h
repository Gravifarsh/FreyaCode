/*
 * requests.h
 *
 *  Created on: 09 июня 2018 г.
 *      Author: developer
 */

#ifndef REQUESTS_H_
#define REQUESTS_H_

void gps_request(float* lon, float* lat, float* h);

void cdm_request(uint16_t* conc);

void bmp_request(int32_t* bpress, int32_t* btemp);

void ds_request(int16_t* dtemp);

void adxl_request(float* x, float* y, float* z);

void mq7_request(uint16_t* mconc);

void geiger_request(uint32_t* ticks);

void time_request(uint32_t* time);

#endif /* REQUESTS_H_ */
