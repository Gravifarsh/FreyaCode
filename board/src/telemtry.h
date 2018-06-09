/*
 * packet.h
 *
 *  Created on: 08 июня 2018 г.
 *      Author: developer
 */

#ifndef PACKET_H_
#define PACKET_H_

#pragma pack(push, 1)
typedef struct{
	int32_t time;
	int32_t bpress, btemp;
	int16_t x, y, z;
	int16_t dtemp;
	uint16_t conc;
	uint16_t mconc;
	float lon, lat;
}packet_t;
#pragma pack(pop)

void sd_telemetry_drop();
void iridium_telemetry_drop();
void nrf_telemetry_drop();

#endif /* PACKET_H_ */
