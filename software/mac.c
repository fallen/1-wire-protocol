#include "mac.h"
#include "phy.h"
#include "global.h"

struct packet reception_buffer;

struct light_packet rx_ring_buff[4];

unsigned char received_byte = 0;

unsigned char byte_has_been_received = 0;

uint8_t pointeur_buffer_lecture = 0;
uint8_t pointeur_buffer_ecriture = 0;

void init_mac(void) {

	recv_index = 0;
	pointeur_buffer_ecriture = 0;
	current_packet_size = MAX_PACKET_SIZE + 1;
	init_phy();
}

inline void clear_ring_buffer_overflow(void) {
	ring_buffer_overflow = 0;
}

void push_byte(unsigned char b)
{
	//uart_send_char(b);
	received_byte = b;
	byte_has_been_received = 1;
	//uart_send_char(received_byte);
	/*if (recv_index >= current_packet_size || recv_index == MAX_PACKET_SIZE)
	{
		reception_buffer.payload[recv_index - 3] = b;
		if (pointeur_buffer_ecriture == 3)
			ring_buffer_overflow = 1;
		pointeur_buffer_ecriture++;
		pointeur_buffer_ecriture %= 4;
		//copy_packet_to_rx_ring();
		byte_has_been_received = 1;
		current_packet_size = MAX_PACKET_SIZE + 1;
		recv_index = 0;
	}
	else
	{
		switch (recv_index)
		{
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
	}*/

}

void copy_packet_to_rx_ring(void) {
	unsigned char i;

	rx_ring_buff[pointeur_buffer_ecriture].src = reception_buffer.src;
	rx_ring_buff[pointeur_buffer_ecriture].verif = reception_buffer.verif;

	for (i = 0 ; i < current_packet_size ;  i++) {
		rx_ring_buff[pointeur_buffer_ecriture].payload[i] = reception_buffer.payload[i];

	}
}

unsigned char rx_buffer_overflow(void) {
	unsigned char temp = ring_buffer_overflow;
	clear_ring_buffer_overflow();
	return temp;
}

void detection_erreur( uint8_t erreur)
{
	puts("erreur lors de la reception\r\n");
	if( erreur == PARITY_ERROR )
		puts("erreur de parite\r\n");
	else if( erreur == FULL_PAYLOAD_ERROR )
		puts("erreur car le ring buffer est plein\r\n");
	else if( erreur == CHECKSUM_ERROR )
		puts("erreur de checksum\r\n");
	else
		puts("autre erreur\r\n");
}
