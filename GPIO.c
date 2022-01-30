#define F_CPU 16000000L
#include <avr/io.h>
#include <util/delay.h>

int main(void)
{
	DDRB = 0xFF; // 0b11111111
	DDRD = 0x00; // 0b00000000
	while (1)
	{
		PORTB = 0b01010101;
		_delay_ms(200);
		PORTB = 0b10101010;
		_delay_ms(200);
		if(PIND & 0x01) { // (PIND & 0x01) == 1
			PORTB = 0xFF;
			_delay_ms(1000);
		}
	}
}
