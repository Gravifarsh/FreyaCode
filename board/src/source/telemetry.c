#include <inttypes.h>
#include <stdbool.h>
#include <util/delay.h>

#include "error.h"
#include "timeservice.h"

#include "globals.h"
#include "telemtry.h"
#include "buses.h"
#include "inits.h"

bool sd_buffer_need_init = true;
bool iridium_buffer_need_init = true;

struct{
	uint8_t buffer[SD_BUFFER_SIZE];
	uint16_t carret;
	// Блок записи
	uint32_t block;
} sd_buffer;

struct{
	uint8_t buffer[IRIDIUM_BUFFER_SIZE];
	uint16_t carret;
	// Состояние
	enum{
		STARTING,
		WRITING,
		SENDING
	}state;

	// Время последнего занесения в буфер данных
	uint32_t last;
} iridium_buffer;


#include <stdio.h>

size_t sd_telemetry_drop(void* data, size_t datasize){
	// Инициализируем буфер, если необходимо
	if(sd_buffer_need_init){
		sd_buffer.carret = 0;
		sd_buffer.block = 0;

		sd_buffer_need_init = false;
	}

	// Если превысили лимит записи - ничего не делаем
	if(sd_buffer.block > SD_MAX_BLOCKS) return 0;

	// Если в буфере достаточно места для данных - записываем
	if(SD_BUFFER_SIZE - sd_buffer.carret> datasize)
	{
		for(int i = 0; i < datasize; i++)
		{
			sd_buffer.buffer[sd_buffer.carret++] = *((uint8_t*)data + i);
		}
		return datasize;
	}
	// Если буфер заполнен
	else
	{
		// Записываем сколько можем
		uint16_t last = SD_BUFFER_SIZE - sd_buffer.carret;

		for(int i = 0; i < last; i++)
		{
			sd_buffer.buffer[sd_buffer.carret++] = *((uint8_t*)data + i);
		}

		// Устанавливаем SPI на работу с SD
		spi_set(SD);

		// Пытаемся записать на SD
		if(rscs_sd_block_write(sd, sd_buffer.block, sd_buffer.buffer, 1)){
			// Если ошибка - реинициализируем SD
			sd_init();

			// Буфер не трогаем
			return last;
		}
		else{
			// Если удача - буфер сбрасываем
			sd_buffer.block++;
			sd_buffer.carret = 0;

			//Записывем в него то, что не записали
			for(int i = last; i < datasize; i++)
			{
				sd_buffer.buffer[sd_buffer.carret++] = *((uint8_t*)data + i);
			}

			return datasize;
		}
	}
}

size_t iridium_telemetry_drop(void* data, size_t datasize){
	// Инициализируем буфер, если необходимо
	if(iridium_buffer_need_init){
		iridium_buffer.carret = 0;
		iridium_buffer.state = STARTING;
		iridium_buffer.last = rscs_time_get();

		iridium_buffer_need_init = false;
	}

	size_t writed = 0;

	switch(iridium_buffer.state){
	// Если мы только начали: буфер пуст и нет текущей посылки в иридиум
	case STARTING:
		// Если пакет полностью влезает в буфер - записываем
		if(IRIDIUM_BUFFER_SIZE - iridium_buffer.carret >= datasize){
			for(int i = 0; i < datasize; i++){
				iridium_buffer.buffer[iridium_buffer.carret++] = *((uint8_t*)data + i);
			}
			writed = datasize;
		}
		// Если нет - мы его заполнили, отправляем
		else{
			iridium_buffer.state = WRITING;
			writed = 0;
		}
		break;
	case WRITING:
	{
		// Пытаемся записать иридиуму данные
		rscs_e error = rscs_iridium9602_write_bytes(iridium, iridium_buffer.buffer, iridium_buffer.carret);
		if(error == RSCS_E_NONE){
			// Если удача - сбрасывем буфер и пытаемся послать
			iridium_buffer.carret = 0;
			iridium_buffer.state = SENDING;
		}
		writed = 0;
	}
		break;
	case SENDING:
	{
		// Пытаемся послать
		rscs_e error = rscs_iridium9602_send(iridium);
		if(error == RSCS_E_NONE){
			// Если удача - переходим к записи, нельзя терять время!
			iridium_buffer.state = WRITING;
			writed = 0;
		}
		else{
			// Если неудача - хорошо, аккумулируем сколько можем данных для следующей посылки
			if(((rscs_time_get() - iridium_buffer.last) > IRIDIUM_DELTA) // Записываем с периодичностью IRIDIUM_DELTA
					&& ((IRIDIUM_BUFFER_SIZE - iridium_buffer.carret) >= datasize)){
				PORTG ^= (1 << 3); // Инвертируем светодиод для индицирования записи
				for(int i = 0; i < datasize; i++){
					iridium_buffer.buffer[iridium_buffer.carret++] = *((uint8_t*)data + i);
				}
				iridium_buffer.last = rscs_time_get(); // Обновляем время для последней записи
			}
			writed = datasize;
		}

	}
		break;
	}
	return writed;
}

size_t nrf_telemetry_drop(void* data, size_t datasize){
	// Ставим шину в режим NRF
	spi_set(NRF);
	// И просто пытемся отправить
	return rscs_nrf24l01_write(nrf, data, datasize);
}
