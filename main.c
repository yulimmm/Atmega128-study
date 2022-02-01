#define F_CPU 16000000L
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>

volatile int mode = 0;

ISR(INT0_vect)
{
	mode = 0;
}

ISR(INT1_vect)
{
	mode = 1;
}

ISR(INT2_vect)
{
	mode = 2;
}

ISR(INT3_vect)
{
	mode = 3;
}

int main(){
	DDRD  = 0x00;
	DDRB  = 0xFF;
	
	EICRA = 0xFF; // 0b11111111
	EIMSK = 0x0F; // 0b00001111

	sei();

	while(1) {
		switch(mode) {
			case 0 : 
				PORTB = 0xFF;
				_delay_ms(500);
				PORTB = 0x00;
				_delay_ms(500);
				break;
			
			case 1 :
				PORTB = 0xFF;
				_delay_ms(250);
				PORTB = 0x00;
				_delay_ms(250);
				break;
			
			case 2 :
				PORTB = 0xFF;
				_delay_ms(100);
				PORTB = 0x00;
				_delay_ms(100);
				break;
				
			case 3 :
				PORTB = 0xFF;
				_delay_ms(50);
				PORTB = 0x00;
				_delay_ms(50);
				break;
				
			default:
				PORTB = 0xFF;
				break;
		}
	}
	return 0;
}
