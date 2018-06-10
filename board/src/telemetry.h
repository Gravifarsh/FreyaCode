/*
 * packet.h
 *
 *  Created on: 08 июня 2018 г.
 *      Author: developer
 */

#ifndef PACKET_H_
#define PACKET_H_

#include <stddef.h>

#define IRIDIUM_BUFFER_SIZE 350
#define SD_BUFFER_SIZE 512

size_t sd_telemetry_drop(void* data, size_t datasize);
size_t iridium_telemetry_drop(void* data, size_t datasize);
size_t nrf_telemetry_drop(void* data, size_t datasize);

#endif /* PACKET_H_ */
