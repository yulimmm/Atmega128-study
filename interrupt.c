#define  F_CPU 16000000L
#include <avr/io.h>
#include <avr/interrupt.h>

volatile int count = 0;
volatile int state = 0;

ISR(TIMER0_COMP_vect)
{
	count++;
	TCNT0 = 0;

	if(count == 64)
	{
		count = 0;
		state = !state;
		if(state) PORTB = 0xFF;
		else PORTB = 0x00;
	}
}

int main(void)
{
	DDRD = 0x00;
	DDRB = 0xFF;

	TCCR0 |= (1 << CS02) | (1 << CS01) | (1 << CS00);
	TIMSK |= (1 << OCIE0);
	OCR0 = 255;
	sei();

	while(1)
	{
		switch(PIND)
		{
			case 1: OCR0 = 32; break;
			case 2: OCR0 = 64; break;
			case 4: OCR0 = 96; break;
			case 8: OCR0 = 128; break;
			case 16: OCR0 = 160; break;
			case 32: OCR0 = 192; break;
			case 64: OCR0 = 224; break;
			case 128: OCR0 = 255; break;
		}
	};
	return 0;
}
