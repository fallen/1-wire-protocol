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


extern uint8_t pointeur_buffer_lecture;
extern uint8_t pointeur_buffer_ecriture;

int main(void)
{
	uint8_t i;
	struct light_packet lp;
	uint8_t src;
	uint8_t taille;
	unsigned char datas[16];
	uart_init();
	init_mac();
	_delay_ms(10000);
	while (1)
	{
		if( recv(&src, &taille, datas) > 0)
		{
			for (i = 0 ; i < taille ; i++)
			{
				uart_send_char(datas[i]);
			}
		}
		print("\r\n");
	}
	return 0;
}

