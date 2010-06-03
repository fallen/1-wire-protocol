unsigned char uart_recv_char(void);
void uart_send_char(unsigned char);

#define FREQ 16000000

#define BAUD_RATE 9600

#define UBRR (FREQ/8/BAUD_RATE - 1)/2
