
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include <stdlib.h>
#include <stdio.h>
#include <avr/eeprom.h>

#include "HD44780.h"

int main() {
	LCD_Initalize();

	char buf[10];
	int i = 0;

	while (1) {
		LCD_Clear();
		sprintf(buf, "Hello %d", i++);
		LCD_WriteText(buf);
		_delay_ms(1000);
	}
}
