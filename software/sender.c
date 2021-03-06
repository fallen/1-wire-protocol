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


int main(void)
{
	uart_init();
	init_mac();
	while (1)
	{
		uint8_t data[16];
		data[0] = 'A';
		data[1] = '@';
		send( ADDRESS_SRC, data, 2);
		
		data[0] = 'D';
		data[1] = 'C';
		send( 0x82, data, 2);

		
		
		
		data[0] = 'H';
		data[1] = 'e';
		data[2] = 'l';
		data[3] = 'l';
		data[4] = 'o';
		send( ADDRESS_SRC, data, 5);
	}
	return 0;
}
