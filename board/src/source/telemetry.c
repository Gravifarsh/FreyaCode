#include <inttypes.h>
#include <stdbool.h>

#include "globals.h"
#include "telemtry.h"
#include "buses.h"
#include "inits.h"

bool sd_buffer_need_init = true;
bool iridium_buffer_need_init = true;

struct{
	uint8_t buffer[SD_BUFFER_SIZE];
	uint16_t carret;
	uint32_t block;
} sd_buffer;

struct{
	uint8_t buffer[IRIDIUM_BUFFER_SIZE];
	uint16_t carret;
} iridium_buffer;


#include <stdio.h>

size_t sd_telemetry_drop(void* data, size_t datasize){
	if(sd_buffer_need_init){
		sd_buffer.carret = 0;
		sd_buffer.block = 0;

		sd_buffer_need_init = false;
	}

	if(sd_buffer.block > SD_MAX_BLOCKS) return 0;

	if(SD_BUFFER_SIZE - sd_buffer.carret> datasize)
	{
		for(int i = 0; i < datasize; i++)
		{
			sd_buffer.buffer[sd_buffer.carret++] = *((uint8_t*)data + i);
		}
		return datasize;
	}
	else
	{
		uint16_t last = SD_BUFFER_SIZE - sd_buffer.carret;

		for(int i = 0; i < last; i++)
		{
			sd_buffer.buffer[sd_buffer.carret++] = *((uint8_t*)data + i);
		}

		spi_set(SD);

		int error = rscs_sd_block_write(sd, sd_buffer.block, sd_buffer.buffer, 1);
		printf("%d\n", error);

		if(error){
			sd_init();

			return last;
		}
		else{
			sd_buffer.block++;
			sd_buffer.carret = 0;

			for(int i = last; i < datasize; i++)
			{
				sd_buffer.buffer[sd_buffer.carret++] = *((uint8_t*)data + i);
			}

			return datasize;
		}
	}
}

size_t iridium_telemetry_drop(void* data, size_t datasize){
	return 0;
}

size_t nrf_telemetry_drop(void* data, size_t datasize){
	spi_set(NRF);
	return rscs_nrf24l01_write(nrf, data, datasize);
}
