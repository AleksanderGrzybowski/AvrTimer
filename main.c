
#include "time.h"
#include "ui.h"
#include "hardware.h" // TODO fix this dep?

void menu_set_span() {
	LCD_Clear();
	debounce();

	Time user_from = ask_for_time("From:", from);
	Time user_to = ask_for_time("To:", to);
	bool _onoff = ask_for_onoff();
	from = user_from;
	to = user_to;
	onoff = _onoff;

	store_span();
}

void menu_set_time() {
	LCD_Clear();
	debounce();

	Time now = ask_for_time("Current time", get_time());

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

	if (_now > _from && _now < _to) { // includes
		if (onoff) {
			SWITCH_PORT |= (1 << SWITCH);
		} else {
            SWITCH_PORT &= ~(1 << SWITCH);
		}
	} else { // excludes
		if (!onoff) {
			SWITCH_PORT |= (1 << SWITCH);
		} else {
            SWITCH_PORT &= ~(1 << SWITCH);
		}
	}
}

char buf1[LCD_WIDTH + 1];
char buf2[LCD_WIDTH + 1];

int main() {
	LCD_Initalize();
	LCD_WriteText("Wait...");

	BUTTON_PORT |= (1 << BUTTON_LEFT);
	BUTTON_PORT |= (1 << BUTTON_MIDDLE);
	BUTTON_PORT |= (1 << BUTTON_RIGHT);

	BUTTON_DDR &= ~(1 << BUTTON_LEFT);
	BUTTON_DDR &= ~(1 << BUTTON_MIDDLE);
	BUTTON_DDR &= ~(1 << BUTTON_RIGHT);

	SWITCH_DDR |= (1 << SWITCH);
	SWITCH_PORT &= ~(1 << SWITCH); // turn off at the beginning

	BACKLIGHT_DDR |= (1 << BACKLIGHT);
	set_backlight(true);

	_delay_ms(1000); // let caps charge

	read_span();

	int update_lcd_cnt = 10;
	int lcd_backlight_cnt = 0;
	int colon_blink_flag = true;

	while (1) {
		if (lcd_backlight_cnt++ > 100) {
			set_backlight(false);
		} else {
			set_backlight(true);
		}

		if (PRESSED_LEFT || PRESSED_RIGHT) {
			lcd_backlight_cnt = 0;
		}

		if (PRESSED_MIDDLE) {
			lcd_backlight_cnt = 0;
			set_backlight(true);
			menu();
		}

		if (update_lcd_cnt++ > 10) {
			update_lcd_cnt = 0;
			colon_blink_flag = !colon_blink_flag;
			LCD_GoTo(0, 0); // LCD_Clear() was here before TODO can it work?
			Time now = get_time();
			sprintf(buf1, "Now %02d%c%02d      ", now.hour,
					colon_blink_flag ? ':' : ' ', now.minute);
			sprintf(buf2, "%s%02d:%02d-%02d:%02d", onoff ? "ON  " : "OFF ", from.hour, from.minute,
					to.hour, to.minute);
			LCD_WriteTwoRows(buf1, buf2);
			handle_switch(now);
		}

		_delay_ms(100);
	}
}
