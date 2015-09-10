all:
	#avr-gcc -mmcu=attiny13a -Wall -Os -o a.elf a.c -L/usr/lib/avr/lib -L/usr/lib/avr/lib/avrtiny/
	avr-gcc -mmcu=atmega328p -Wall -Os -o target/out.elf *.c
	avr-objcopy -j .text -j .data -O ihex target/out.elf target/out.hex
	sudo avrdude  -p m328p -c usbasp -P usb -B 4 -U flash:w:target/out.hex
