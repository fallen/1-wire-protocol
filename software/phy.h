#ifndef PHY_H
#define PHY_H

#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include "global.h"
#include "mac.h"

#define RECHARGE 16000


/**
 * Verification du temps pour être sur que nous ayons atteint le imparti
 * @return Retourne un booléen pour dire si le temps est OK (42) ou non (0)
 */
unsigned char verificationTemps(void);

/**
 * Fonction servant à relancer le timer 0 avec les bonnes valeurs.
 * @param valeur Valeur à recharger dans le timer
 */
void relancerTimer(uint16_t valeur);

/**
 * initTimer permet d'intialiser le timer 1 pour le traitement de la couche physique
 */
void initTimer(void);

/**
 * Emission d'un octet
 * @param octet est l'octet que l'on veut emettre
 * @return retourne un code annonçant l'état de l'émission (0 = échoué, autre pour un succès)
 */
void emissionOctet( uint8_t octet);

/**
 * Envoie d'un bit logique haut
 * @return code retour de la fonction pour s'assurer de sont bon fonctionnement
 */
void envoieHaut( void );

/**
 * Envoie d'un bit logique bas
 * @return code retour de la fonction pour s'assurer de sont bon fonctionnement
 */
void envoieBas( void );

/**
 * Mise à l'état par defaut de la ligne
 */
void pause(void);

/**
 * Impulsion de départ (mise à l'état 0 de la ligne)
 */
void depart(void);

/**
 * Calcul de la parité
 * @return la parité
 */
uint8_t xor( uint8_t octet );

/**
 * Sample le port D 2
 * @return il retourne 7 zéro suivi de la valeur sur le port D 2
 */
uint8_t sample(void);

/**
 * Stop le timer
 */
void stop_timer();

#endif
