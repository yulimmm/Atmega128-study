#define F_CPU 16000000L
#include <avr/io.h>

void ADC_init()
{
	ADMUX  |= (1 << REFS0);					// AVCC 핀 입력을 기준전압(5V)
	ADMUX  |= (1 << MUX0 | 1 << MUX1);			// PF3을 ADC입력핀으로 설정, 단일 입력 방식
	ADCSRA |= (1 << ADPS2) | (1 << ADPS1) | (1 << ADPS0);	// 분주비 128로 설정
	ADCSRA |= (1 << ADEN);					// ADC 활성화 (전원 ON)
	ADCSRA |= (1 << ADFR);					// 프리러닝 모드로 설정
	ADCSRA |= (1 << ADSC);					// ADC 변환 시작
}
int read_ADC(void)
{
	while( ! ( ADCSRA & (1 << ADIF) ) );
	return ADC;
}
int main(void)
{
	int read;
	DDRB = 0xFF;
	ADC_init();

	while(1)
	{
		read = read_ADC();
		switch(read/100)
		{
			case 10	: 	PORTB = 0b11111111;	break;
			case 9	: 	PORTB = 0b01111111;	break;
			case 8	: 	PORTB = 0b00111111;	break;
			case 7	: 	PORTB = 0b00011111;	break;
			case 5	: 	PORTB = 0b00001111;	break;
			case 4	: 	PORTB = 0b00000111;	break;
			case 3	: 	PORTB = 0b00000011;	break;
			case 2	: 	PORTB = 0b00000001;	break;
			case 1	: 	PORTB = 0b00000000;	break;
			default	: 	PORTB = 0b00000000;
		}
	}
	return 0;
}
