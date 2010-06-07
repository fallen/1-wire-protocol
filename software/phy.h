#include <avr/io.h>
#include <avr/interrupt.h>
#include "global.h"

#ifdef PHY_H
#define PHY_H

#define RECHARGE 16000

/**
 * Compteur permettant de savoir quel sera le prochain bit reçu.
 */
unsigned char compteur = 0;

/**
 * Variable recevant l'octet à réceptionner
 */
unsigned char reception = 0;

/**
 * Bit de parité envoyé par l'émetteur
 */
unsigned char parite_recue = 0;

/**
 * Bit de parité calculé par le récepteur
 */
unsigned char parite = 0;

/**
 * Compteur de temps pour faire 5 ms
 */
unsigned char temps = 0;



/**
 * Verification du temps pour être sur que nous ayons atteint le imparti
 * @return Retourne un booléen pour dire si le temps est OK (42) ou non (0)
 */
unsigned char verificationTemps();

/**
 * Fonction servant à relancer le timer 0 avec les bonnes valeurs.
 * @param valeur Valeur à recharger dans le timer
 */
void relancerTimer(int valeur);

/**
 * initTimer permet d'intialiser le timer 1 pour le traitement de la couche physique
 */
void initTimer();

#ENDIF
