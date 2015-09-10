#define F_CPU 9200000/8

#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include <stdlib.h>
#include <stdio.h>
#include <avr/eeprom.h>

int main() {
	DDRC = 0xff;

	while (1) {
		PORTC = 0;
		_delay_ms(1000);
		PORTC = 1;
		_delay_ms(1000);
	}
}
