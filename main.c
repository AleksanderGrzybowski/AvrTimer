
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
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

#define BUTTON_PORT PORTD
#define BUTTON_PIN  PIND
#define BUTTON_DDR  DDRD

#define BUTTON_LEFT  PD0
#define BUTTON_MIDDLE   PD1
#define BUTTON_RIGHT PD2

#define PRESSED_LEFT   ((BUTTON_PIN & (1 << BUTTON_LEFT)) == 0)
#define PRESSED_MIDDLE ((BUTTON_PIN & (1 << BUTTON_MIDDLE)) == 0)
#define PRESSED_RIGHT  ((BUTTON_PIN & (1 << BUTTON_RIGHT)) == 0)
#define PRESSED_ANY    (PRESSED_LEFT || PRESSED_MIDDLE || PRESSED_RIGHT)

void debounce() {
	_delay_ms(200);
}

int get_key() {
	while (true) {
		if (PRESSED_LEFT) {
			return 0;
		}
		if (PRESSED_MIDDLE) {
			return 1;
		}
		if (PRESSED_RIGHT) {
			return 2;
		}
	}
}

void menu_set_span() {
	// NYI
}

void menu_set_time() {
	// NYI
}

int option = 0;

void menu() {
	while (true) {
		debounce();

		LCD_Clear();
		LCD_WriteText("Span Time Exit");
		LCD_GoTo(0, 1);

		switch (option) {
			case 0:
				LCD_WriteText("^");
				break;
			case 1:
				LCD_WriteText("     ^");
				break;
			case 2:
				LCD_WriteText("          ^");
				break;
		}

		int key = get_key();

		if (key == 0 && option != 0) {
			option--;
		} else if (key == 2 && option != 2) {
			option++;
		} else if (key == 1) {
			switch (option) {
				case 0:
					menu_set_span();
					break;
				case 1:
					menu_set_time();
					break;
				case 2:
					debounce();
					LCD_Clear();
					return;
			}
		}

	}
}


int main() {
	LCD_Initalize();

	BUTTON_PORT |= (1 << BUTTON_LEFT);
	BUTTON_PORT |= (1 << BUTTON_MIDDLE);
	BUTTON_PORT |= (1 << BUTTON_RIGHT);

	BUTTON_DDR &= ~(1 << BUTTON_LEFT);
	BUTTON_DDR &= ~(1 << BUTTON_MIDDLE);
	BUTTON_DDR &= ~(1 << BUTTON_RIGHT);

	char buf[16];

	while (1) {
		LCD_Clear();

		if (PRESSED_MIDDLE) {
			menu();
		}

		Time now = get_time();
		sprintf(buf, "Now %02d:%02d:%02d", now.hour, now.minute, now.second);

		LCD_WriteText(buf);
		_delay_ms(500);
	}
}
