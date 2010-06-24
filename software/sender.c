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


extern struct packet reception_buffer;

extern unsigned char byte_has_been_received;
extern unsigned char received_byte;

int main(void)
{
	uart_init();
	init_mac();
//	EIMSK &= ~(1 << INT0);
	PORTD |= (1 << PORTD2);
	DDRD |= (1 << PORTD2);
	while (1)
	{
		if (byte_has_been_received)
		{
			byte_has_been_received = 0;
			uart_send_char(received_byte);
		}
		emissionOctet('@');
	}
	return 0;
}
