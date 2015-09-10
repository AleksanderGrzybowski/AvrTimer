all:
	avr-gcc -mmcu=atmega328p -Wall -Os -o target/out.elf *.c -DF_CPU=1200000
	avr-objcopy -j .text -j .data -O ihex target/out.elf target/out.hex
	sudo avrdude  -p m328p -c usbasp -P usb -B 4 -U flash:w:target/out.hex
