


#include "time.h"
#include "ui.h"
#include "hardware.h" // TODO fix this dep?

void menu_set_span() {
	LCD_Clear();
	debounce();

	Time user_from = ask_for_time("Turn on from");
	Time user_to = ask_for_time("Turn on until");

	from = user_from;
	to = user_to;

	store_span();
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
					option = 0;
					return;
			}
		}

	}
}

void handle_switch(Time now) {
	int _from = from.hour * 60 + from.minute;
	int _to = to.hour * 60 + to.minute;
	int _now = now.hour * 60 + now.minute;

	if (_now > _from && _now < _to) {
		SWITCH_PORT |= (1 << SWITCH);
	} else {
		SWITCH_PORT &= ~(1 << SWITCH);
	}
}

char buf1[LCD_WIDTH + 1];
char buf2[LCD_WIDTH + 1];

int main() {
	LCD_Initalize();

	BUTTON_PORT |= (1 << BUTTON_LEFT);
	BUTTON_PORT |= (1 << BUTTON_MIDDLE);
	BUTTON_PORT |= (1 << BUTTON_RIGHT);

	BUTTON_DDR &= ~(1 << BUTTON_LEFT);
	BUTTON_DDR &= ~(1 << BUTTON_MIDDLE);
	BUTTON_DDR &= ~(1 << BUTTON_RIGHT);

	SWITCH_DDR |= (1 << SWITCH);
	SWITCH_PORT &= ~(1 << SWITCH); // turn off at the beginning

	read_span();

	while (1) {
		LCD_Clear();

		if (PRESSED_MIDDLE) {
			menu();
		}

		Time now = get_time();
		sprintf(buf1, "Now %02d:%02d", now.hour, now.minute);
		sprintf(buf2, "%02d:%02d - %02d:%02d", from.hour, from.minute, to.hour, to.minute);
		LCD_WriteTwoRows(buf1, buf2);

		handle_switch(now);

		_delay_ms(100);
	}
}
