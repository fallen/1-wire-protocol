unsigned char uart_recv_char(void);
void uart_send_char(unsigned char);

#define FREQ 16000000

#define BAUD_RATE 9600

#define UBRR FREQ/16/BAUD_RATE-1
/*
#define UBRR0H (unsigned char *)0xC5
#define UBRR0L (unsigned char *)0xC4

#define UCSR0A	(unsigned char*)0xC0
*/
