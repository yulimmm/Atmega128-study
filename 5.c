#define  F_CPU 16000000L
#include <avr/io.h>

int main(void)
{
	DDRB = 0xFF;
	PORTB = 0x01;
	
	TCCR0 |= (1<<CS02) | (1<<CS01) | (1<<CS00);
	TCCR0 |= (1<<WGM00) | (1<<WGM01);
	TCCR0 |= (1<<COM01);

	OCR0 = 20;
	while(1) { };
	return 0;
}
