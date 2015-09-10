
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

	Time from = ask_for_time("Turn on from");
	Time to = ask_for_time("Turn on until");

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

	char buf[16];

	while (1) {
		LCD_Clear();

		if (PRESSED_MIDDLE) {
			menu();
		}

		Time now = get_time();
		sprintf(buf, "Now %02d%c%02d", now.hour, (now.second % 2 == 0) ? ':' : ' ', now.minute);

		LCD_WriteText(buf);
		_delay_ms(100);
	}
}
