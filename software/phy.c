#include "phy.h"
#include "mac.h"

/**
 * Mutex de prise de la ligne
 */
unsigned char mutex_ligne;

void initPin(void) {

	DDRD &= ~( 1 << PORTD2 );
	PORTD |= ( 1 << PORTD2 );
	EICRA |= (1 << ISC01) | (1 << ISC00);
}

void init_phy(void) {
	initPin();
	cli();
	initTimer();
	EIMSK |= (1 << INT0); // disables INT1 and enables INT0
	EIMSK &= ~(1 << INT1);
//	PCICR = 0; // disables all the other PIN CHANGE interrupts
//	PCMSK0 = 0; // mask the pin change interrupts
//	PCMSK1 = 0; // idem
	sei();
}
/*
ISR(TIMER1_OVF_vect) {

	static unsigned int timer = 0;

	if (timer < 1000)
		timer++;
	else {
		puts("t");
		timer = 0;
	}

	relancerTimer(RECHARGE);

}
*/


/**
 * Gestion de l'interruption du Timer0
 * @param TIMER0_OVF Vecteur d'interruption
 */

ISR(TIMER1_OVF_vect)
{
	static unsigned char compteur = 100;
	static uint8_t tempo = 0;
	static unsigned char reception = 0;
	unsigned char parite_recue = 0;
	

//	TCCR1B &= ~(1 << CS10 );
//	TIMSK1 &= ~(1 << TOIE1);
	if( compteur == 100)
	{
		if( tempo != 7 )
		{
			tempo++;
			relancerTimer(RECHARGE);
			return;
		}
		tempo = 0;
		compteur = 0;
	}
	if( !verificationTemps() ) //|| mutex_ligne)
	{
		//asm("reti");
		//reti();
		return;
		//puts("LOLILOL\r\n");
	}
	if( compteur == 8 )
	{
		//puts("apres push byte\r\n");
/*		parite_recue = ( (PORTD & (1 << 2) ) >> 2 );
		if( parite_recue == (xor(reception) >> 7) )
		{
			push_byte(reception);
		}
		else
		{
			
		}*/
		compteur++;
	}
	else if( compteur == 9 )
	{
		push_byte(reception);
		compteur = 100;
		reception = 0;
	//	mutex_ligne = 0;
		TIMSK1 &= ~(1 << TOIE1);

	}
	else
	{
	//	mutex_ligne = 1;
		if ( PORTD & ( 1 << PORTD5) )
			PORTD &= ~( 1 << PORTD5);
		else
			PORTD |= (1 << PORTD5);

/*		if ( PIND & (1 << PIND2) )
			puts("r: 1\r\n");
		else
			puts("r: 0\r\n");*/
		reception |= ( ((PIND & (1 << PIND2) ) >> PIND2 ) << compteur );
		compteur++;
	}
}


/**
 * Gestion de l'interruption sur front changeant de la pin INT0
 * @param PCINT0 Vecteur d'interruption
 */
ISR(INT0_vect)
{
//	EIFR |= (1 << INTF0); // Clears the External Interrupt Flag 0
//	if( mutex_ligne )
//		reti();
	relancerTimer(RECHARGE);
}

unsigned char verificationTemps(void)
{
	static unsigned char temps = 0;
	if( temps == 4 )
	{
		temps = 0;
		TCCR1B &= ~(1 << CS10 );
		return 42;
	}
	
	temps++;
	relancerTimer(RECHARGE);
	return 0;
}

void relancerTimer(int valeur)
{
//	unsigned int i = 65535 - valeur;
	sreg = SREG;
	cli();
	TCCR1B |= (1 << CS10 );
	TIMSK1 |= (1 << TOIE1);
	TCNT1 = 65535 - valeur;
//	TCNT1L = (unsigned char)(65535 - valeur);
//	TCNT1H = (unsigned char)((65535 - valeur) >> 8);
	SREG = sreg;
}

void initTimer(void)
{
	//Permet de mettre les bonnes valeurs dans les registres pour activer le timer 1 en mode normal
	TCCR1A &= (~(1 << WGM10 ) & ~(1 << WGM11));
	TCCR1B &= (~(1 << WGM12 ) & ~(1 << WGM13));
	
	//Permet d'activer l'interruption timer 1
	TIMSK1 |= (1 << TOIE1);
	
	//Permet de lancer le timer
//	TCCR1B |= (1 << CS10 );

}

unsigned char emissionOctet( unsigned char octet)
{
	//compteur2 et parite2 sont des variables locales pour savoir quel est le numéro du bit qu'on envoie et la parite calculée.
	unsigned char compteur2 = 0;
//	if( mutex_ligne )
//		return 0;
//	mutex_ligne = 1;
	DDRD |= ( 1<<DDD2 );
	depart();
	for( compteur2 = 0; compteur2 < 8; compteur2++)
	{
		if( (octet & (1 << compteur2)) )
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
	pause();
	DDRD &= ~( 1<<DDD2 );
//	mutex_ligne = 0;
	return 1;
}


unsigned char envoieHaut( void )
{
//	if( mutex_ligne )
//		return 0;
	PORTD |= (1 << PORTD2);
	_delay_ms(7);
	PORTD &= ~(1 << PORTD2);
	_delay_ms(3);
	return 1;
}


unsigned char envoieBas( void )
{
//	if( mutex_ligne )
//		return 0;
	PORTD |= (1 << PORTD2);
	_delay_ms(3);
	PORTD &= ~(1 << PORTD2);
	_delay_ms(7);
	return 1;
}

void pause(void)
{
	PORTD |= (1 << PORTD2);
	_delay_ms(2);
}

void depart(void)
{
	PORTD &= ~(1 << PORTD2);
	_delay_ms(2);
	PORTD |= (1 << PORTD2);
	_delay_ms(9);
	PORTD &= ~(1 << PORTD2);
	_delay_ms(1);
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
