#include <inttypes.h>
#include <avr/interrupt.h>
#include <util/atomic.h>
#include <stdio.h>

#include "../geiger.h"
#include "timeservice.h"

volatile static uint32_t global_ticks = 0;
static uint32_t prev_ticks = 0;
static uint32_t prev_time = 0;

ISR(INT4_vect){
	global_ticks++;
}


void sides_geiger_init(){
	EIMSK = (1 << 4); // INT4 interrupt enabled
	EICRB = (1 << 1); // Falling edge
}


uint32_t sides_gegiger_get_total_ticks(){
	uint32_t retval;
	ATOMIC_BLOCK(ATOMIC_RESTORESTATE){
		retval = global_ticks;
	}
	return retval;
}


 bool sides_geiger_get_uRh(float * value) {
	 uint32_t current_time = rscs_time_get();
	 uint32_t current_ticks = sides_gegiger_get_total_ticks();

	 uint32_t dtime = rscs_time_get() - prev_time;
	 uint32_t dtics = current_ticks - prev_ticks;

	 if (dtime <= 30*1000)
	 	 {
		 	 return false;
	 	 }
	 else
	 	 {
		 	 *value = ( dtics/70 ) / (dtime/3600000.0);

		 	 prev_time = current_time;
		 	 prev_ticks = current_ticks;
		 	 return true;
	 	 }
 }


/*
 float sides_geiger_get_uRh_now() {
	 uint32_t current_time = rscs_time_get();
	 uint32_t current_ticks = sides_gegiger_get_total_ticks();

	 uint32_t dtime = rscs_time_get() - prev_time;
	 uint32_t dticks = current_ticks - prev_ticks;
	 float radiation = 0;

	 if (dtime <= 30000)
	 	 {
		 	 printf("CALCULATING\n");
		 	 printf("INTERVAL: %ld\n",dtime);
		 	 printf("COUNTS: %ld\n", dticks);
	 	 }
	 else
	 	 {
		 	 radiation =  ( dticks / 75.0) / ( dtime / 3600000.0);

		 	 prev_time = current_time;
		 	 prev_ticks = current_ticks;
		 	 printf("RADIATION: %f", radiation);
		 	 return radiation;
	 	 }
	 return 0;
 }
 */

 uint32_t sides_geiger_get_uRh_now() {
	 	 uint32_t current_time = rscs_time_get();
	 	 uint32_t current_ticks = sides_gegiger_get_total_ticks();

	 	 uint32_t dtime = rscs_time_get() - prev_time;
	 	 uint32_t dticks = current_ticks - prev_ticks;
	 	 uint32_t radiation = 0;

	 	 if (dtime <= 46000)
	 	 	 {
	 		 	 printf("CALCULATING\n");
	 		 	 printf("INTERVAL: %ld\n",dtime);
	 		 	 printf("COUNTS: %ld\n", dticks);
	 	 	 }
	 	 else
	 	 	 {
	 		 	 radiation =  dticks;

	 		 	 prev_time = current_time;
	 		 	 prev_ticks = current_ticks;
	 		 	 printf("RADIATION: %ld", radiation);
	 		 	 return radiation;
	 	 	 }
	 	 return 0;
 }

