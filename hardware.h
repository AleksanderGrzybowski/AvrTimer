#ifndef _HARDWARE_C
#define _HARDWARE_C

#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include <avr/eeprom.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>

#include "HD44780.h"
#include "ds1307.h"
#include "time.h"

void set_time(Time time);
Time get_time();
void store_span();
void read_span();

#endif
