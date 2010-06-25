#include "mac.h"
#include "phy.h"
#include "global.h"

#include <avr/io.h>

struct packet reception_buffer;

struct light_packet rx_ring_buff[4];

uint8_t pointeur_buffer_lecture = 0;
uint8_t pointeur_buffer_ecriture = 0;

uint8_t pointeur_overflow = 0;

uint8_t recv_index = 0;

uint8_t current_packet_size = MAX_PACKET_SIZE + 1;

uint8_t can_write(void) {

	return  ( ( !pointeur_overflow && (pointeur_buffer_lecture <= pointeur_buffer_ecriture) ) || (pointeur_overflow && (pointeur_buffer_ecriture < pointeur_buffer_lecture) ) );

}

uint8_t can_read(void) {

	return ( ( (!pointeur_overflow) && (pointeur_buffer_ecriture > pointeur_buffer_lecture)) || pointeur_overflow);

}

void inc_write_pointer(void) {
	pointeur_buffer_ecriture++;
	pointeur_buffer_ecriture %= 4;
	if (pointeur_buffer_ecriture == 0)
		pointeur_overflow = ! pointeur_overflow;
}

void inc_read_pointer(void) {

	print("r");

	pointeur_buffer_lecture++;
	pointeur_buffer_lecture %= 4;
	if (pointeur_buffer_lecture == 0)
		pointeur_overflow = ! pointeur_overflow;
}

//void recv(struct light_packet *lp) {
void recv (uint8_t *src, uint8_t *taille, unsigned char *datas) {
	uint8_t i;
	while (!can_read());
	*src = rx_ring_buff[pointeur_buffer_lecture].src;
	*taille = rx_ring_buff[pointeur_buffer_lecture].size;
	for (i = 0 ; i < *taille ; i++)
		datas[i] = rx_ring_buff[pointeur_buffer_lecture].payload[i];
	inc_read_pointer();
}

void init_mac(void) {

	recv_index = 0;
	pointeur_buffer_ecriture = 0;
	pointeur_buffer_lecture = 0;
	current_packet_size = MAX_PACKET_SIZE + 1;
	init_phy();
}

inline void clear_ring_buffer_overflow(void) {
	ring_buffer_overflow = 0;
}

void push_byte(unsigned char b)
{
	if ( recv_index >= (current_packet_size + 2) || recv_index == MAX_PACKET_SIZE)
	{
		reception_buffer.payload[recv_index - 3] = b;
		copy_packet_to_rx_ring();
		current_packet_size = MAX_PACKET_SIZE + 1;
		recv_index = 0;
	}
	else
	{
		switch (recv_index)
		{
			case 0:
				if ((b >> 5) == 5)
					reception_buffer.src = b;
				else
					return;
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

}

void detection_erreur( uint8_t erreur)
{
	print("erreur lors de la reception\r\n");
	if( erreur == PARITY_ERROR )
		print("erreur de parite\r\n");
	else if( erreur == FULL_PAYLOAD_ERROR )
		print("erreur car le ring buffer est plein\r\n");
	else if( erreur == CHECKSUM_ERROR )
		print("erreur de checksum\r\n");
	else
		print("autre erreur\r\n");
}

void copy_packet_to_rx_ring(void) {
	uint8_t i;
	if (can_write())
	{
		rx_ring_buff[pointeur_buffer_ecriture].src = reception_buffer.src;
		rx_ring_buff[pointeur_buffer_ecriture].size = current_packet_size;
		for (i = 0 ; i < current_packet_size ;  i++) {
			rx_ring_buff[pointeur_buffer_ecriture].payload[i] = reception_buffer.payload[i];
		}
		inc_write_pointer();
	}
	else
	{
		detection_erreur(FULL_PAYLOAD_ERROR);
		ring_buffer_overflow = 1;
	}
}

unsigned char rx_buffer_overflow(void) {
	unsigned char temp = ring_buffer_overflow;
	clear_ring_buffer_overflow();
	return temp;
}

