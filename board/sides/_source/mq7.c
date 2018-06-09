#include <avr/io.h>

#include "mq7.h"

void sides_mq7_init(){
	ADMUX = (1 << 6); // VCC AREF
	ADCSRA = (1 << 7) | (1 << 6) | (1 << 5);
}

uint16_t sides_mq7_get(){
	return ((ADCH & 0b11) << 8) | ADCL;
}
