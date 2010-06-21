#ifdef __AVR_ATmega328P__
	#warning using __AVR_ATmega328P__
#endif

#include <avr/io.h>
#include <avr/interrupt.h>
#include "usart.h"
#include "global.h"
#include "mac.h"
#include "phy.h"

//pour flasher la puce
//avr-objcopy -O ihex -R .eeprom test.out test.hex
//sudo avrdude -P /dev/ttyUSB0 -c stk500v1 -p m328p -b 57600 -U flash:w:main.hex

extern struct packet reception_buffer;

extern unsigned char byte_has_been_received;
extern unsigned char received_byte;

int main(void)
{

	DDRD |= (1 << 5);
	PORTD &= ~(1 << 5);
	uart_init();
//	puts("initialisation...\n\r");
	init_mac();
//	puts("MAC layer initialized !\n\r");
	puts(".\r\n");	
	while (1) {
//		PORTB |= (1 << 5);
//		PORTB &= ~(1 << 5);
//		puts("\r\nTEST\r\n");
/*		puts("timer = ");
		uart_send_char(TCNT1 >> 8);
		uart_send_char( (unsigned char)TCNT1);
		puts("\r\n");*/
/*		puts("R : ");
		uart_send_char(reception_buffer.src);
		puts("\n\r");*/

		if (byte_has_been_received) {
			byte_has_been_received = 0;
			uart_send_char(received_byte);
			uart_send_char('\r');
			uart_send_char('\n');
		}
	}
	
	puts("=FIN=\n\r");
	return 0;
}
