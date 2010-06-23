#include "phy.h"
#include "mac.h"

/**
 * Mutex de prise de la ligne
 */
uint8_t mutex_ligne;

void initPin(void) {

	DDRD &= ~( 1 << PORTD2 );
	PORTD |= ( 1 << PORTD2 );
	EICRA |= (1 << ISC01) | (1 << ISC00);
}

void init_phy(void)
{
	mutex_ligne = 0;
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

/**
 * Gestion de l'interruption du Timer0
 * @param TIMER0_OVF Vecteur d'interruption
 */

ISR(TIMER1_OVF_vect)
{
	static uint8_t compteur = 0;							//permet de savoir quel est le bit qu'on attend de recevoir
	static uint8_t timer_is_ok =0;							//permet de mesure le temps
	static uint8_t reception = 0; 							//Octet de réception
	static uint8_t parite = 0;							//Bit de parite recu

	
	/*
	 * Est ce que nous attendons le bit de start ?
	 * on sample donc à 2 millisecondes.
	 * Si ce n'est pas le cas, on passe à la suite.
	 */
	if( compteur == 0 )
	{
		if( timer_is_ok == 1 )
		{
			if( !(sample()) )
			{
				mutex_ligne = 1;
				compteur++;
			}
			timer_is_ok = 0;
			stop_timer();
		}
		else
		{
			timer_is_ok++;
			relancerTimer(RECHARGE);
		}
	}
	else if( compteur == 10 )
	{
		compteur = 0;
		reception = 0;
		timer_is_ok = 0;
		mutex_ligne = 0;
		stop_timer();
	}
	else
	{
		/*
		* On vérifie qu'il s'est bien écoulé 5ms
		* C'est à dire que l'on rappel 4 fois cette fonction
		*/
		if( !(timer_is_ok == 4) )
		{
			timer_is_ok++;
			relancerTimer(RECHARGE);
		}
		else
		{
			if( compteur < 9 )						//On reçoit les bits de data de l'octet
			{
				reception |= ( sample() << (8 - compteur) );		//On shift de manière à recevoir le MSB en premier, et le LSB en dernier.
				compteur++;
				stop_timer();
			}
			else								//On reçoit le bit de parité
			{
				if( sample() == xor( reception ))
				{
					push_byte(reception);
				}
				else
				{
					puts("gogole\r\n");
				}
				stop_timer();
				compteur++;
			}
			timer_is_ok = 0;						//On reset la variable timer_is_ok
		}
	}
	return;
}

uint8_t sample(void)
{
	return (( PIND & (1 << PIND2) ) >> PIND2 );
}

void stop_timer()
{
	TCCR1B &= ~(1 << CS10 );
}

/**
 * Gestion de l'interruption du Timer0
 * @param TIMER0_OVF Vecteur d'interruption
 */


/**
 * Gestion de l'interruption sur front changeant de la pin INT0
 * @param PCINT0 Vecteur d'interruption
 */
ISR(INT0_vect)
{
//	EIFR |= (1 << INTF0); // Clears the External Interrupt Flag 0
//	if( mutex_ligne )
//		reti();
	TCNT1 = 0;
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

void relancerTimer(uint16_t valeur)
{
//	unsigned int i = 65535 - valeur;
	uint16_t timer;
	sreg = SREG;
	cli();
	stop_timer();
	timer = TCNT1;
	TCNT1 = 65535 - valeur + timer;
	TCCR1B |= (1 << CS10 );
	TIMSK1 |= (1 << TOIE1);
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

void emissionOctet( uint8_t octet)
{
	int8_t compteur = 0;
	if( mutex_ligne == 0 )
	{
		EIMSK &= ~(1 << INT0);
		mutex_ligne = 1;
		DDRD |= ( 1<<DDD2 );
		depart();
		for( compteur = 7; compteur >= 0; compteur--)
		{
			if( (octet & (1 << compteur)) )
				envoieHaut();
			else
			envoieBas();
		}
		if( (xor(octet)) )
			envoieHaut();
		else
			envoieBas();
		pause();
		DDRD &= ~( 1<<DDD2 );
		mutex_ligne = 0;
		EIMSK |= (1 << INT0);
	}
}


void envoieHaut( void )
{
	PORTD |= (1 << PORTD2);
	_delay_ms(7);
	PORTD &= ~(1 << PORTD2);
	_delay_ms(3);
}


void envoieBas( void )
{
	PORTD |= (1 << PORTD2);
	_delay_ms(3);
	PORTD &= ~(1 << PORTD2);
	_delay_ms(7);
}

void pause(void)
{
	PORTD |= (1 << PORTD2);
	_delay_ms(20);
}

void depart(void)
{
	PORTD &= ~(1 << PORTD2);
	_delay_ms(10);
	PORTD |= (1 << PORTD2);
	_delay_ms(1);
	PORTD &= ~(1 << PORTD2);
	_delay_ms(9);
}

uint8_t xor( uint8_t octet )
{
	uint8_t parite = octet;
	parite ^= (octet << 1);
	parite ^= (octet << 2);
	parite ^= (octet << 3);
	parite ^= (octet << 4);
	parite ^= (octet << 5);
	parite ^= (octet << 6);
	parite ^= (octet << 7);
	parite &= 0x80;
	return ( parite >> 7 );
}
