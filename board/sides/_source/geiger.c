#include <inttypes.h>
#include <avr/interrupt.h>
#include <util/atomic.h>

#include "geiger.h"

uint32_t ticks = 0;

ISR(INT4_vect){
	ticks++;
}

void sides_geiger_init(){
	EIMSK = (1 << 4); // INT4 interrupt enabled
	EICRB = (1 << 1); // Falling edge
}

uint32_t sides_gegiger_get_ticks(){
	uint32_t retval;
	ATOMIC_BLOCK(ATOMIC_RESTORESTATE){
		retval = ticks;
	}
	return retval;
}
