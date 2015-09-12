#ifndef _TIME_H
#define _TIME_H

#include <stdint.h>

typedef struct {
	uint8_t hour;
	uint8_t minute;
} Time;

Time from, to; // global TODO can it be there?
int onoff;

#endif
