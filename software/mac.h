#ifndef _MAC_H_
#define _MAC_H_

#define MAX_PACKET_SIZE 15

void push_byte(unsigned char);

void clear_ring_buffer_overflow(void);

void copy_packet_to_rx_ring(void);

unsigned char rx_buffer_overflow(void);

unsigned char recv_index;

unsigned char ring_buffer_index;

unsigned char current_packet_size;

#endif
