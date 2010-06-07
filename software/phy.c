#include "phy.h"
#include "mac.h"

/**
 * Gestion de l'interruption du Timer0
 * @param TIMER0_OVF Vecteur d'interruption
 */
ISR(TIMER1_OVF_vect)
{
	unsigned char compteur = 0;
	unsigned char reception = 0;
	unsigned char parite_recue = 0;
	if( !verificationTemps() || mutex_ligne)
	{
		asm("reti");
	}
	if( compteur == 8 )
	{
		parite_recue = ( (PORTD & (1 << 2) ) >> 2 );
		if( parite_recue == (xor(reception) >> 7) )
		{
			push_byte(reception);
		}
		else
		{
			
		}
		compteur++;
	}
	else if( compteur > 8 )
	{
		compteur = 0;
		reception = 0;
		mutex_ligne = 0;
	}
	else
	{
		mutex_ligne = 1;
		reception |= ( ((PORTD & (1 << 2) ) >> 2 ) << compteur );
		compteur++;
	}
}

/**
 * Gestion de l'interruption sur front changeant de la pin INT0
 * @param PCINT0 Vecteur d'interruption
 */
ISR(PCINT0_vect)
{
	if( mutex_ligne )
		reti();
	relancerTimer(RECHARGE);
}

unsigned char verificationTemps(void)
{
	unsigned char temps = 0;
	if( temps == 5 )
		return 42;
	
	temps++;
	relancerTimer(RECHARGE);
	return 0;
}

void relancerTimer(int valeur)
{
	sreg = SREG;
	cli();
	TCNT1 = 65535-valeur;
	SREG = sreg;
}

void initTimer(void)
{
	//Permet de mettre les bonnes valeurs dans les registres pour activer le timer 1 en mode normal
	TCCR1A &= (~(1 << WGM10 ) & ~(1 << WGM11));
	TCCR1B &= (~(1 << WGM12 ) & ~(1 << WGM13));
	
	//Permet d'activer l'interruption timer 1
	TIMSK1 |= (1 << TOIE1);
}

unsigned char emissionOctet( unsigned char octet)
{
	//compteur2 et parite2 sont des variables locales pour savoir quel est le numéro du bit qu'on envoie et la parite calculée.
	unsigned char compteur2 = 0;
	if( mutex_ligne )
		return 0;
	mutex_ligne = 1;
	DDRD |= ( 1<<DDD2 );
	for( compteur2 = 0; compteur2 < 8; compteur2++)
	{
		if( (octet &= (1 << compteur2)) )
		{
			if( !(envoieHaut()) )
				return 0;
		}
		else
		{
			if( !(envoieBas()) )
				return 0;
		}
	}
	if( (xor(octet)) )
	{
		if( !(envoieHaut()) )
			return 0;
	}
	else
	{
		if( !(envoieBas()) )
			return 0;
	}
	DDRD &= ~( 1<<DDD2 );
	mutex_ligne = 0;
	return 1;
}


unsigned char envoieHaut( void )
{
	if( mutex_ligne )
		return 0;
	PORTD |= (1 << PORTD2);
	_delay_ms(7);
	PORTD &= ~(1 << PORTD2);
	_delay_ms(3);
	return 1;
}


unsigned char envoieBas( void )
{
	if( mutex_ligne )
		return 0;
	PORTD |= (1 << PORTD2);
	_delay_ms(3);
	PORTD &= ~(1 << PORTD2);
	_delay_ms(7);
	return 1;
}

unsigned char xor( unsigned char octet )
{
	unsigned char parite2=0;
	parite2 ^= octet;
	parite2 ^= (octet << 1);
	parite2 ^= (octet << 2);
	parite2 ^= (octet << 3);
	parite2 ^= (octet << 4);
	parite2 ^= (octet << 5);
	parite2 ^= (octet << 6);
	parite2 ^= (octet << 7);
	parite2 &= 0x80;
	return parite2;
}
