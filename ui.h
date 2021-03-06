#ifndef _UI_H
#define _UI_H

#include "HD44780.h"
#include "time.h"
#include "hardware.h"
#include <stdbool.h>

#define DEBOUNCE_DELAY 100
#define LCD_WIDTH 16

#define BUTTON_PORT PORTD
#define BUTTON_PIN  PIND
#define BUTTON_DDR  DDRD

#define BUTTON_LEFT  PD0
#define BUTTON_MIDDLE   PD1
#define BUTTON_RIGHT PD2

#define SWITCH_PORT PORTD
#define SWITCH_PIN  PIND
#define SWITCH_DDR  DDRD
#define SWITCH PD3

#define BACKLIGHT_PORT PORTD
#define BACKLIGHT_PIN  PIND
#define BACKLIGHT_DDR  DDRD
#define BACKLIGHT PD4


#define PRESSED_LEFT   ((BUTTON_PIN & (1 << BUTTON_LEFT)) == 0)
#define PRESSED_MIDDLE ((BUTTON_PIN & (1 << BUTTON_MIDDLE)) == 0)
#define PRESSED_RIGHT  ((BUTTON_PIN & (1 << BUTTON_RIGHT)) == 0)
#define PRESSED_ANY    (PRESSED_LEFT || PRESSED_MIDDLE || PRESSED_RIGHT)


void debounce();
int get_key();
Time ask_for_time(char* message, Time time);
void LCD_WriteTwoRows(char* first, char* second);
bool ask_for_onoff();
void set_backlight(bool state);
#endif
