#ifdef __AVR_ATmega328P__
	#warning using __AVR_ATmega328P__
#endif

#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include "usart.h"
#include "global.h"
#include "mac.h"


//pour flasher la puce
//avr-objcopy -O ihex -R .eeprom test.out test.hex
//sudo avrdude -P /dev/ttyUSB0 -c stk500v1 -p m328p -b 57600 -U flash:w:main.hex


int main(void)
{
	DDRD &= (1 << PORTD2);
	while (1) {
		//emissionOctet('@');
		PORTD |= (1 << PORTD2);
		_delay_ms(2000);
		PORTD &= ~(1 << PORTD2);
		_delay_ms(2000);
/*		depart();
		envoieBas();
		envoieHaut();
		envoieBas();
		envoieBas();
		envoieBas();
		envoieBas();
		envoieBas();
		envoieBas();
		envoieHaut();
		pause();*/

	}
	return 0;
}
