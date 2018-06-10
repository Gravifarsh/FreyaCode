#include <stdbool.h>
#include <inttypes.h>

#include "timeservice.h"

#include "../globals.h"
#include "buses.h"

#include "geiger.h"
#include "mq7.h"

void gps_request(float* lon, float* lat, float* h, bool* hasFix){
	rscs_gps_read(gps, lon, lat, h, hasFix);
}

void cdm_request(uint16_t* conc){
	rscs_cdm7160_read(cdm, conc);
}

void bmp_request(int32_t* bpress, int32_t* btemp){
	spi_set(BMP);

	int32_t rbpress, rbtemp;

	rscs_bmp280_read(bmp, &rbpress, &rbtemp);
	rscs_bmp280_calculate(rscs_bmp280_get_calibration_values(bmp), rbpress, rbtemp, bpress, btemp);
}

void ds_request(int16_t* dtemp){
	if(rscs_ds18b20_check_ready()){
		rscs_ds18b20_read_temperature(ds, dtemp);
		rscs_ds18b20_start_conversion(ds);
	}
}

void mq7_request(uint16_t* mconc){
	*mconc = sides_mq7_get();
}

void geiger_request(uint32_t* ticks){
	*ticks = sides_gegiger_get_ticks();
}

void time_request(uint32_t* time){
	*time = rscs_time_get();
}
