#include "hardware.h"

void set_time(Time time) {
	ds1307_setdate(1, 1, 1, time.hour, time.minute, 0);
}

Time get_time() {
	uint8_t dummy;
	Time result;
	ds1307_getdate(&dummy, &dummy, &dummy, &result.hour, &result.minute, &dummy);
	return result;
}

void store_span() {
	eeprom_write_byte((uint8_t*)13, from.hour);
	eeprom_write_byte((uint8_t*)14, from.minute);
	eeprom_write_byte((uint8_t*)15, to.hour);
	eeprom_write_byte((uint8_t*)16, to.minute);
	eeprom_write_byte((uint8_t*)17, onoff);
}

void read_span() {
	from.hour = eeprom_read_byte((uint8_t*)13);
	from.minute = eeprom_read_byte((uint8_t*)14);
	to.hour = eeprom_read_byte((uint8_t*)15);
	to.minute = eeprom_read_byte((uint8_t*)16);
	onoff = eeprom_read_byte((uint8_t*)17);
}
