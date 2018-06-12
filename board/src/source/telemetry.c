#include <inttypes.h>
#include <stdbool.h>

#include "error.h"

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
	enum{
		STARTING,
		WRITING,
		SENDING
	}state;
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
		printf("SAVING TO SD BUFFER DATA:\n");
		for(int i = 0; i < datasize; i++)
		{
			sd_buffer.buffer[sd_buffer.carret++] = *((uint8_t*)data + i);
			printf("%02x", *((uint8_t*)data + i));
		}
		printf("\n");
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
		printf("WRITED %d BYTES ON SD WITH OFFSET %ld AND ERROR %d\n DATA IS:\n", sd_buffer.carret, sd_buffer.block, error);
		for(int i = 0; i < SD_BUFFER_SIZE; i++) printf("%02X", sd_buffer.buffer[i]);
		printf("\n");

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
	if(iridium_buffer_need_init){
		iridium_buffer.carret = 0;
		iridium_buffer.state = STARTING;

		iridium_buffer_need_init = false;
	}

	size_t writed;

	switch(iridium_buffer.state){
	case STARTING:
		if(IRIDIUM_BUFFER_SIZE - iridium_buffer.carret >= datasize){
			for(int i = 0; i < datasize; i++){
				iridium_buffer.buffer[iridium_buffer.carret++] = *((uint8_t*)data + i);
			}
			writed = datasize;
		}
		else{
			iridium_buffer.state = WRITING;
			writed = 0;
		}
		break;
	case SENDING:
	{
		rscs_e error = rscs_iridium9602_send(iridium);
		if(error == RSCS_E_NONE){
			iridium_buffer.state = WRITING;
			writed = 0;
		}
		else{
			if(true && IRIDIUM_BUFFER_SIZE - iridium_buffer.carret >= datasize){
				for(int i = 0; i < datasize; i++){
					iridium_buffer.buffer[iridium_buffer.carret++] = *((uint8_t*)data + i);
				}
			}
			writed = datasize;
		}

	}
		break;
	case WRITING:
	{
		rscs_e error = rscs_iridium9602_write_bytes(iridium, iridium_buffer.buffer, iridium_buffer.carret);
		if(error == RSCS_E_NONE){
			iridium_buffer.carret = 0;
			iridium_buffer.state = SENDING;
		}
		writed = 0;
	}
		break;
	}
	return writed;
}

size_t nrf_telemetry_drop(void* data, size_t datasize){
	spi_set(NRF);
	return rscs_nrf24l01_write(nrf, data, datasize);
}
