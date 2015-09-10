
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
} Time;

Time global_from, global_to;

void set_time(Time time) {
	ds1307_setdate(1, 1, 1, time.hour, time.minute, 0);
}

Time get_time() {
	uint8_t dummy;
	Time result;
	ds1307_getdate(&dummy, &dummy, &dummy, &result.hour, &result.minute, &dummy);
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
	_delay_ms(100);
}

int get_key() {
	while (true) {
		if (PRESSED_LEFT) {
			debounce();
			return 0;
		}
		if (PRESSED_MIDDLE) {
			debounce();
			return 1;
		}
		if (PRESSED_RIGHT) {
			debounce();
			return 2;
		}
	}
}

Time ask_for_time(char* message) {
	char buf[17];

	Time time = get_time();

	while (true) {
		sprintf(buf, "Hour: %d", time.hour);
		LCD_Clear();
		LCD_WriteText(message);
		LCD_GoTo(0, 1);
		LCD_WriteText(buf);

		int key = get_key();
		switch (key) {
			case 0:
				if (time.hour == 0) {
					time.hour = 23;
				} else {
					time.hour--;
				}
				break;
			case 2:
				if (time.hour == 23) {
					time.hour = 0;
				} else {
					time.hour++;
				}
				break;
			case 1:
				goto label_minutes; // yeah!

		}
		_delay_ms(200);
	}

label_minutes:

	while (true) {
		sprintf(buf, "Minute: %d", time.minute);
		LCD_Clear();
		LCD_WriteText(message);
		LCD_GoTo(0, 1);
		LCD_WriteText(buf);

		int key = get_key();
		switch (key) {
			case 0:
				if (time.minute == 0) {
					time.minute = 59;
				} else {
					time.minute--;
				}
				break;
			case 2:
				if (time.minute == 59) {
					time.minute = 0;
				} else {
					time.minute++;
				}
				break;
			case 1:
				goto label_end;
		}
		_delay_ms(200);
	}

label_end:
	return time;
}

void menu_set_span() {
	LCD_Clear();
	debounce();

	Time user_from = ask_for_time("Turn on from");
	Time user_to = ask_for_time("Turn on until");

	global_from = user_from;
	global_to = user_to;

	store_span();
}

void store_span() {
	eeprom_write_byte(13, global_from.hour);
	eeprom_write_byte(14, global_from.minute);
	eeprom_write_byte(15, global_to.hour);
	eeprom_write_byte(16, global_to.minute);
}

void read_or_clear_span() {
	uint8_t from_hour = eeprom_read_byte(13);
	uint8_t from_minute = eeprom_read_byte(14);
	uint8_t to_hour = eeprom_read_byte(15);
	uint8_t to_minute = eeprom_read_byte(16);

	global_from.hour = from_hour;
	global_from.minute = from_minute;
	global_to.hour = to_hour;
	global_to.minute = to_minute;

	if (from_hour == 255) { // freshly flashed
		global_from.hour = global_from.minute = global_to.hour = global_to.minute = 0;
		store_span();
	}
}

void menu_set_time() {
	LCD_Clear();
	debounce();

	Time now = ask_for_time("Current time");

	set_time(now);
	return;

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

	read_or_clear_span();


	char buf[16];

	while (1) {
		LCD_Clear();

		if (PRESSED_MIDDLE) {
			menu();
		}

		Time now = get_time();
		sprintf(buf, "Now %02d:%02d", now.hour, now.minute);
		LCD_WriteText(buf);

		LCD_GoTo(0, 1);

		sprintf(buf, "%02d:%02d - %02d:%02d", global_from.hour, global_from.minute, global_to.hour, global_to.minute);
		LCD_WriteText(buf);

		_delay_ms(100);
	}
}
