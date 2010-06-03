#ifdef __AVR_ATmega328P__
	#warning using __AVR_ATmega328P__
#endif

#include <avr/io.h>
#include <avr/interrupt.h>

//pour flasher la puce
//avr-objcopy -O ihex -R .eeprom test.out test.hex
//sudo avrdude -v  -P  usb -c usbasp -p m328p  -U flash:w:test.hex
//sudo avrdude  -c usbtiny -p m328p  -U flash:w:test.hex 

int main(void )
{

}
