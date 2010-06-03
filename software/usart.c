#include <avr/io.h>
#include <avr/interrupt.h>
#include "usart.h"

unsigned char uart_recv_char(void) {
	while ( !UCSR0A & (1 << RXC0) );
	
	return UDR0;
}

void uart_send_char(unsigned char byte) {
	while ( !( UCSR0A & (1<<UDRE0)) );

	UDR0 = byte;
}

uart_init(void) {
	UBRR0H = (unsigned char)(UBRR >> 8);
	UBRR0L = (unsigned char)UBRR;
	
	UCSR0B = (1<<RXEN0)|(1<<TXEN0);

	UCSR0C = (1<<USBS0)|(3<<UCSZ00);
}
