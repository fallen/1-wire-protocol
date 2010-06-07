#include "phy.h"

/**
 * Gestion de l'interruption du Timer0
 * @param TIMER0_OVF Vecteur d'interruption
 */
ISR(TIMER1_OVF_vect)
{
	if( !verificationTemps())
	{
		asm("iret");
	}
	if( compteur == 8 )
	{
		parite_recue = ( (PORTD & (1 << 2) ) >> 2 );
		if( parite_recue == parite )
		{
			
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
	}
	else
	{
		reception |= ((PORTD & (1 << 2) ) >> 2 ) << compteur );
		compteur++;
	}
}

/**
 * Gestion de l'interruption sur front changeant de la pin INT0
 * @param PCINT0 Vecteur d'interruption
 */
ISR(PCINT0_vect)
{
	relancerTimer(RECHARGE);
}

unsigned char verificationTemps()
{
	if( temps == 5 )
		return 42;
	
	temps++;
	relancerTimer(recharge);
	return 0;
}

void relancerTimer(int valeur)
{
	sreg = SREG;
	_CLI();
	TCNT1 = 65535-valeur;
	SREG = sreg;
}

void initTimer()
{
	//Permet de mettre les bonnes valeurs dans les registres pour activer le timer 1 en mode normal
	TCCR1A &= (~(1 << WGM10 ) & ~(1 << WGM11));
	TCCR1B &= (~(1 << WGM12 ) & ~(1 << WGM13));
	
	//Permet d'activer l'interruption timer 1
	TIMSK1 |= (1 << TOIE1);
}
