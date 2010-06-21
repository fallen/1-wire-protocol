#include "mac.h"
#include "phy.h"
#include "global.h"

struct packet reception_buffer;

struct light_packet rx_ring_buff[4];

unsigned char received_byte = 0;

unsigned char byte_has_been_received = 0;

void init_mac(void) {

	recv_index = 0;
	ring_buffer_index = 0;
	current_packet_size = MAX_PACKET_SIZE + 1;
	init_phy();
}

inline void clear_ring_buffer_overflow(void) {
	ring_buffer_overflow = 0;
}

void push_byte(unsigned char b) {

/*	int i;

	for (i = 0 ; i < 8 ; i++)
		uart_send_char( ((b & (1 << i)) >> i)  + 0x30);

	puts("\r\n");*/

	byte_has_been_received = 1;
	received_byte = b;

/*	uart_send_char('R');
	uart_send_char(':');
	uart_send_char(b);
	puts("\r\n");*/
/*
	if (recv_index >= current_packet_size || recv_index == MAX_PACKET_SIZE) {
		reception_buffer.payload[recv_index - 3] = b;
		if (ring_buffer_index == 3)
			ring_buffer_overflow = 1;
		ring_buffer_index++;
		ring_buffer_index %= 4;
		copy_packet_to_rx_ring();
		current_packet_size = MAX_PACKET_SIZE + 1;
		recv_index = 0;
	} else {
		switch (recv_index) {
			case 0:
				reception_buffer.src = b;
				break;
			case 1:
				reception_buffer.dest = b;
				break;
			case 2:
				reception_buffer.verif = b;
				current_packet_size = b >> 4;
				break;
			default :
				reception_buffer.payload[recv_index - 3] = b;
		}
		recv_index++;
	}
*/
}

void copy_packet_to_rx_ring(void) {
	unsigned char i;

	rx_ring_buff[ring_buffer_index].src = reception_buffer.src;
	rx_ring_buff[ring_buffer_index].verif = reception_buffer.verif;

	for (i = 0 ; i < current_packet_size ;  i++) {
		rx_ring_buff[ring_buffer_index].payload[i] = reception_buffer.payload[i];

	}
}

unsigned char rx_buffer_overflow(void) {
	unsigned char temp = ring_buffer_overflow;
	clear_ring_buffer_overflow();
	return temp;
}
