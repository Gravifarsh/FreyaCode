#include <inttypes.h>

#include "globals.h"
#include "telemtry.h"

struct{
	uint8_t buffer[SD_BUFFER_SIZE];
	uint16_t carret;
	uint32_t block;
} sd_buffer;

struct{
	uint8_t buffer[IRIDIUM_BUFFER_SIZE];
	uint16_t carret;
} iridium_buffer;


size_t sd_telemetry_drop(void* data, size_t datasize){
	return 0;
}

size_t iridium_telemetry_drop(void* data, size_t datasize){
	return 0;
}

size_t nrf_telemetry_drop(void* data, size_t datasize){
	return 0;
}
