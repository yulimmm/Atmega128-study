#define F_CPU 16000000L
#include <avr/io.h>

void UART1_init(void)
{
	UCSR1A |= (1 << U2X1);				        // 1번 UART 2배속 모드
	UCSR1B |= (1 << RXEN1) | (1 << TXEN1);		// 송수신 활성화
	UCSR1C |= (1 << UCSZ11) | (1 << UCSZ10);	// 8비트
	UBRR1H = 0;
	UBRR1L = 207;					// 보율 9600
}
void UART1_transmit(char data)
{
	while( !(UCSR1A & (1 << UDRE1)));   //내가 데이터를 보낼 수 있을 때까지 기다리기
	UDR1 = data;
}
unsigned char UART1_receive(void)
{
	while( !(UCSR1A & (1 << RXC1)));  //읽을 게 있을 때 까지 기다리기
	return UDR1;
}
int main(void)
{
	char temp;
	UART1_init();
	while(1)
	{
		temp = UART1_receive();
		UART1_transmit(temp);
	}
	return 0;
}
