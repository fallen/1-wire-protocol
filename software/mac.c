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

void push_byte(unsigned char b)
{
	if(recv_index >= current_packet_size || recv_index == MAX_PACKET_SIZE)
	{
		reception_buffer.payload[recv_index - 3] = b;
		if (ring_buffer_index == 3)
			ring_buffer_overflow = 1;
		ring_buffer_index++;
		ring_buffer_index %= 4;
		copy_packet_to_rx_ring();
		current_packet_size = MAX_PACKET_SIZE + 1;
		recv_index = 0;
	}
	else
	{
		switch (recv_index)
		{
			case 0:
				if (b >> 5 == 5) // est-ce un début de trame ?
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

void mauvaise_parite(void)
{
	recv_index = 0;
	erreur_sur_paquet();
}

void erreur_sur_paquet(void)
{
	puts("oups, erreur sur un paquet\r\n");
}

void copy_packet_to_rx_ring(void)
{
	unsigned char i;

	if( reception_buffer.dest == ADDRESS_SRC )
	{
		rx_ring_buff[ring_buffer_index].src = reception_buffer.src;
		rx_ring_buff[ring_buffer_index].verif = reception_buffer.verif;

		for (i = 0 ; i < current_packet_size ;  i++)
		{
			rx_ring_buff[ring_buffer_index].payload[i] = reception_buffer.payload[i];
		}
	}
}

unsigned char rx_buffer_overflow(void) {
	unsigned char temp = ring_buffer_overflow;
	clear_ring_buffer_overflow();
	return temp;
}

void send( uint8_t address_dest, uint8_t data[], uint8_t taille)
{
	uint8_t checksum_taille = 0;
	uint8_t i;
	
	checksum_taille = (taille << 4);
	checksum_taille |= calcul_checksum(data,taille);
	
	emissionOctet(ADDRESS_SRC);
	emissionOctet(address_dest);
	emissionOctet(checksum_taille);
	for( i=0; i < taille; i++)
	{
		emissionOctet(data[i]);
	}
}

uint8_t calcul_checksum( uint8_t data[], uint8_t taille)
{
	uint8_t i;
	uint8_t checksum = 0;
	for( i=0; i<taille; i++)
	{
		checksum += data[i];
	}
	checksum += (checksum >> 4);
	return (checksum & 0x0f);
}
