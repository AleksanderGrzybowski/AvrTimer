#include "ui.h"

void debounce() {
	_delay_ms(DEBOUNCE_DELAY);
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

void LCD_WriteTwoRows(char* first, char* second) {
	LCD_Clear();
	LCD_WriteText(first);
	LCD_GoTo(0, 1);
	LCD_WriteText(second);
}

Time ask_for_time(char* message) {
	char buf[LCD_WIDTH + 1];

	Time time = get_time();

	while (true) {
		sprintf(buf, "Hour: %d", time.hour);
		LCD_WriteTwoRows(message, buf);

		switch (get_key()) {
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
		LCD_WriteTwoRows(message, buf);

		switch (get_key()) {
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

bool ask_for_onoff() {
	bool selected = onoff;
	char first[] = "On or off?";
	char second[17];

	while (true) {
        sprintf(second, selected ? ">ON  OFF" : " ON >OFF");

		LCD_WriteTwoRows(first, second);
		int key = get_key();

		switch(key) {
		case 0:
			selected = true;
			break;
		case 2:
			selected = false;
			break;
		case 1:
			return selected;
		}
	}
}

