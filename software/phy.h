#ifndef PHY_H
#define PHY_H

#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include "global.h"

#define RECHARGE 16000

/**
 * Mutex de prise de la ligne
 */
unsigned char mutex_ligne;


/**
 * Verification du temps pour être sur que nous ayons atteint le imparti
 * @return Retourne un booléen pour dire si le temps est OK (42) ou non (0)
 */
unsigned char verificationTemps(void);

/**
 * Fonction servant à relancer le timer 0 avec les bonnes valeurs.
 * @param valeur Valeur à recharger dans le timer
 */
void relancerTimer(int valeur);

/**
 * initTimer permet d'intialiser le timer 1 pour le traitement de la couche physique
 */
void initTimer(void);

/**
 * Emission d'un octet
 * @param octet est l'octet que l'on veut emettre
 * @return retourne un code annonçant l'état de l'émission (0 = échoué, autre pour un succès)
 */
unsigned char emissionOctet( unsigned char octet);

/**
 * Envoie d'un bit logique haut
 * @return code retour de la fonction pour s'assurer de sont bon fonctionnement
 */
unsigned char envoieHaut( void );

/**
 * Envoie d'un bit logique bas
 * @return code retour de la fonction pour s'assurer de sont bon fonctionnement
 */
unsigned char envoieBas( void );

/**
 * Calcul de la parité
 * @return la parité
 */
unsigned char xor( unsigned char octet );

#endif
