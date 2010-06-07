#ifdef __AVR_ATmega328P__
	#warning using __AVR_ATmega328P__
#endif

#include <avr/io.h>
#include <avr/interrupt.h>
#include "usart.h"
#include "global.h"
#include "mac.h"


//pour flasher la puce
//avr-objcopy -O ihex -R .eeprom test.out test.hex
//sudo avrdude -P /dev/ttyUSB0 -c stk500v1 -p m328p -b 57600 -U flash:w:main.hex


int main(void)
{


	uart_init();
	puts("initialisation...\n\r");
	init_mac();
	puts("MAC layer initialized !\n\r");

	while (1) {
		puts("Received : ");
		uart_send_char(received_buffer);
		puts("\n\r");
	}
	
	puts("=FIN=\n\r");
	return 0;
}
