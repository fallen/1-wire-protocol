#ifndef _MAC_H_
#define _MAC_H_


#include <avr/io.h>


#define MAX_PACKET_SIZE 15
#define ADDRESS_SRC 172

#define PARITY_ERROR 1
#define FULL_PAYLOAD_ERROR 2
#define CHECKSUM_ERROR 3

void init_mac(void);

void push_byte(unsigned char);

void clear_ring_buffer_overflow(void);

void copy_packet_to_rx_ring(void);

/**
 * En cas d'erreur de réception d'un octet (erreur sur la parité) on appel cette fonction
 */
void mauvaise_parite(void);

/**
 * En cas d'erreur sur un paquet (parité mauvaise sur un octet ou chekcsum faux), on appel cette fonction qui le signale à la couche supérieur
 */
void erreur_sur_paquet(void);

/**
 * Cette fonction est appelée lorsque la couche applicative veut envoyer un un message
 * @param address le premier paramètre est l'adresse de destination
 * @param data est tout simplement les datas à envoyer, sotckées sous forme de tableau
 * @param taille est le nombre d'octet à envoyer (puisque la taille du tableau data est static)
 */
void send( uint8_t address_dest, uint8_t data[16], uint8_t taille);

/**
 * Fonction qui calcul le checksum
 * @param data les datas dont on calcul le checksum
 * @param taille taille des datas
 * @return retourne le checksum
 */
uint8_t calcul_checksum( uint8_t data[16], uint8_t taille);

unsigned char rx_buffer_overflow(void);


#endif
