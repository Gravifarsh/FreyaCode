/*
 * requests.h
 *
 *  Created on: 09 июня 2018 г.
 *      Author: developer
 */

#ifndef REQUESTS_H_
#define REQUESTS_H_

// Опрос GPS
void gps_request(float* lon, float* lat, float* h, bool* hasFix);

// Опрос CDM
void cdm_request(uint16_t* conc);

// Опрос BMP
void bmp_request(int32_t* bpress, int32_t* btemp);

// Опрос DS
void ds_request(int16_t* dtemp);

// Опрос ADXL
void adxl_request(float* x, float* y, float* z);

// Опрос MQ7
void mq7_request(uint16_t* mconc);

// Опрос GEIGER
void geiger_request(uint32_t* ticks);

// Опрос TIMESERVICE
void time_request(uint32_t* time);

#endif /* REQUESTS_H_ */
