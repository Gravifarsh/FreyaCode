#ifndef GEIGER_H_
#define GEIGER_H_

#include <stdint.h>
#include <stdbool.h>

void sides_geiger_init();
uint32_t sides_gegiger_get_total_ticks();
bool sides_geiger_get_uRh(float * value);
uint32_t sides_geiger_get_uRh_now();

#endif /* GEIGER_H_ */
