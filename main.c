
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include <stdlib.h>
#include <stdio.h>
#include <avr/eeprom.h>

#include "HD44780.h"
#include "ds1307.h"

typedef struct {
	uint8_t hour;
	uint8_t minute;
	uint8_t second;
} Time;

void set_time(Time time) {
	ds1307_setdate(1, 1, 1, time.hour, time.minute, time.second);
}

Time get_time() {
	uint8_t dummy;
	Time result;
	ds1307_getdate(&dummy, &dummy, &dummy, &result.hour, &result.minute, &result.second);
	return result;
}

int main() {
	LCD_Initalize();

	char buf[16];

	Time new_time = {12, 34, 56};
	set_time(new_time);

	while (1) {
		LCD_Clear();

		Time now = get_time();
		sprintf(buf, "Now %02d:%02d:%02d", now.hour, now.minute, now.second);

		LCD_WriteText(buf);
		_delay_ms(1000);
	}
}
