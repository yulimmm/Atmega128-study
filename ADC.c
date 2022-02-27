#define F_CPU 16000000L
#include <avr/io.h>

int read_ADC(void)
{
	while( ! ( ADCSRA & (1 << ADIF) ) ); //ADCSRA에 변환이 안끝난다면 ADC에 저장되는 값을 반환
	return ADC;
}

int main(void)
{
	int read;
	
	DDRB = 0xFF;
	TCCR0 |= (1 << WGM00) | (1 << COM01) | (1 << CS02);  //pwm 설정(위상교정 PWM), (비반전모드), (분주비 64)
	ADMUX  |= (1 << REFS0) | (1 << MUX0 | 1 << MUX1); // AVCC 기준전압 (5V), ADC3을 쓰고 단일입력방식
	ADCSRA |= (1 << ADPS2) | (1 << ADPS1) | (1 << ADPS0); // 분주비 128설정
	ADCSRA |= (1 << ADEN); // ADC 활성화
	ADCSRA |= (1 << ADFR); // 프리러닝 모드
	ADCSRA |= (1 << ADSC); // ADC변환모드
	
	while(1)
	{
		read = read_ADC();
		switch(read/100)
		{
			case 10 : OCR0 = 200; break;
			case 9 : OCR0 = 180; break;
			case 8 : OCR0 = 160; break;
			case 7 : OCR0 = 140; break;
			case 6 : OCR0 = 120; break;
			case 5 : OCR0 = 100; break;
			case 4 : OCR0 = 80; break;
			case 3 : OCR0 = 60; break;
			case 2 : OCR0 = 40; break;
			case 1 : OCR0 = 20; break;
			default : OCR0 = 0;
		}
	}

	return 0;

}
	
