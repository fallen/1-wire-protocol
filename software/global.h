#ifndef GLOBAL_H
#define GLOBAL_H

/**
 * Entier pour sauvegarder le sreg (pour le timer)
 */
unsigned char sreg;

unsigned char ring_buffer_overflow;

struct packet {
	unsigned char src;
	unsigned char dest;
	unsigned char verif; // 4 MSB = size of payload ; 4 LSB = checksum
	unsigned char payload[16]; // size is from 0 to 15 inside verif, which makes payload going from size 1 to 16
};

struct light_packet {
	unsigned char src;
	unsigned char verif;
	unsigned char payload[16];
};

#endif
