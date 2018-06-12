/*
 * packet.h
 *
 *  Created on: 08 июня 2018 г.
 *      Author: developer
 */

#ifndef PACKET_H_
#define PACKET_H_

#include <stddef.h>

// Размер буфера для IRIDIUM, размер посылки - 360 байт
#define IRIDIUM_BUFFER_SIZE 360
// Размер буфера для SD, размер блока - 512 байт
#define SD_BUFFER_SIZE 512

// Максимальное количество блоков, которое будет записано на SD
#define SD_MAX_BLOCKS 14300
// Периодичность отбора пакетов для IRIDIUM в мс
#define IRIDIUM_DELTA 1000

// Сброс телеметрии на SD
size_t sd_telemetry_drop(void* data, size_t datasize);

// Сброс телеметрии в IRIDIUM
size_t iridium_telemetry_drop(void* data, size_t datasize);

// Сброс телеметрии в NRF
size_t nrf_telemetry_drop(void* data, size_t datasize);

#endif /* PACKET_H_ */
