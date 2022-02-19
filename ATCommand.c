#define F_CPU 16000000L
#include <avr/io.h>
#include <stdio.h>
#include <util/delay.h>
#include <string.h>

void UART1_init(void)
{
	UCSR1A |= (1 << U2X1);                    // 1번 UART 2배속 모드
	UCSR1B |= (1 << RXEN1) | (1 << TXEN1);      // 송수신 활성화
	UCSR1C |= (1 << UCSZ11) | (1 << UCSZ10);   // 8비트
	UBRR1H = 0;
	UBRR1L = 16;               // 보율 115200
}

void UART1_transmit(char data)
{
	while( !(UCSR1A & (1 << UDRE1)));   //내가 데이터를 보낼 수 있을 때까지 기다리기
	UDR1 = data;
}

void UART1_string_transmit(char *str)
{
	for(int i = 0; str[i]; i++)    //들어오는 문자를 문자열로 저장
	UART1_transmit(str[i]);        
}

unsigned char UART1_receive(void)
{
	while( !(UCSR1A & (1 << RXC1)));  //읽을 게 있을 때 까지 기다리기
	return UDR1;
}

void UART1_string_receive(char *str)
{
	int i;
	char data = UART1_receive();
	for(i = 0; data != '\n'; i++)     //\n이 들어올 때까지의 문자를 data에 저장
	{
		str[i] = data;
		data = UART1_receive();       
	}
}

int main(void)
{
	char temp[800];
 	UART1_init();
	 
// 	UART1_string_transmit("AT+RST\r\n"); //reset
// 	UART1_string_receive(temp);
// 	_delay_ms(100);
	
// 	UART1_string_transmit("ATE0\r\n");  //echo off
// 	_delay_ms(100);
// 	UART1_string_receive(temp);
	
 	UART1_string_transmit("AT+CWMODE=1\r\n");  //wifi mode 설정
	UART1_string_receive(temp);
	_delay_ms(5000);
 	
 	UART1_string_transmit("AT+CWJAP=\"<와이파이 이름>\",\"<비밀번호>\"\r\n");  //wifi 연결
	UART1_string_receive(temp);
	_delay_ms(5000);
 	
 	UART1_string_transmit("AT+CIPSTART=\"TCP\",\"api.openweathermap.org\",80\r\n");   // api 요청, 형식:<type><remoteIP><remote port>
 	UART1_string_receive(temp);
	 _delay_ms(5000);
	 
 	UART1_string_transmit("AT+CIPSEND=138\r\n"); //보내는 문자 길이 133
	UART1_string_receive(temp);
	_delay_ms(5000);
 	
	UART1_string_transmit("GET /data/2.5/weather?id=1835848&units=metric&APPID=<키 넘버>\r\n"); //133
	UART1_string_transmit("Host: api.openweathermap.org\r\n"); 
	UART1_string_transmit("User-Agent: ArduinoWiFi/1.1\r\n");
	UART1_string_transmit("Connection: close\r\n\r\n"); //138
	UART1_string_receive(temp);
	_delay_ms(5000);
	
	while(1)
	{
// 		UART1_string_receive(temp);
// 		_delay_ms(50);
	}
	return 0;
	
}
