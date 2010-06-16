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

int main(void)
{

//	DDRB |= (1 << 5);
	uart_init();
	puts("initialisation...\n\r");
	init_mac();
	puts("MAC layer initialized !\n\r");
	
	while (1) {
//		PORTB |= (1 << 5);
//		_delay_ms(500);
//		PORTB &= ~(1 << 5);
		puts("\r\n\r\nTEST TEST TEST TEST TEST\r\n\r\n");
/*		puts("timer = ");
		uart_send_char(TCNT1 >> 8);
		uart_send_char( (unsigned char)TCNT1);
		puts("\r\n");*/
//		puts("Received : ");
//		uart_send_char(reception_buffer.src + 0x30);
//		puts("\n\r");
	}
	
	puts("=FIN=\n\r");
	return 0;
}
