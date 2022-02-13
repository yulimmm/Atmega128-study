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
	char temp[20];
	UART1_init();
	UART1_string_transmit("AT+RST\r\n"); //reset
	_delay_ms(100);
	UART1_string_receive(temp);
	
	if(strcmp(temp, "OK\r\n")==0){
		UART1_string_transmit("AT+CWMODE=3\r\n");  //wifi mode 설정
		_delay_ms(1000);
		UART1_string_receive(temp);
		
		if(strcmp(temp, "OK\r\n")==0){
			UART1_string_transmit("AT+CWJAP=\"TP-Link_B9D0\",\"06312275\"\r\n");  //wifi 연결
			_delay_ms(1000);
			UART1_string_receive(temp);
			
			if(strcmp(temp, "OK\r\n")==0){
				UART1_string_transmit("AT+CIPSTART=\"TCP\",\"api.openweathermap.org\",80\r\n");  //api연결
				_delay_ms(1000);
				UART1_string_receive(temp);
				
				if(strcmp(temp, "OK\r\n")==0){
					UART1_string_transmit("AT+CIPSEND=98\r\n");  //call 길이 전송
					_delay_ms(1000);
					UART1_string_receive(temp);
					
					if(strcmp(temp, "OK\r\n")==0){
						UART1_string_transmit("api.openweathermap.org/data/2.5/weather?lat=38&lon=127&appid=b154f4f2fefe7862a10432af6e2b4dbd\r\n");  //call 길이 전송
						_delay_ms(1000);
						UART1_string_receive(temp);
					}
				}
			}
		}
	}
	
// 	UART1_string_transmit("AT+CWMODE=3\r\n");  //wifi mode 설정
// 	_delay_ms(1000);
// 	
// 	UART1_string_transmit("AT+CWJAP=\"TP-Link_B9D0\",\"06312275\"\r\n");  //wifi 연결
// 	_delay_ms(1000);
// 	
// 	UART1_string_transmit("AT+CIPSTART=\"TCP\",\"api.openweathermap.org\",80\r\n");   // api 요청, 형식:<type><remoteIP><remote port>
// 	_delay_ms(1000);
// 	
// 	UART1_string_transmit("AT+CIPSEND=98\r\n"); //보내는 문자 길이 98
// 	_delay_ms(1000);
// 	
// 	UART1_string_transmit("api.openweathermap.org/data/2.5/weather?lat=38&lon=127&appid=b154f4f2fefe7862a10432af6e2b4dbd\r\n"); //key제공, 형식:api.openweathermap.org/data/2.5/weather?lat={lat}&lon={lon}&appid={API key}
// 	_delay_ms(1000);
	
	while(1)
	{
// 		UART1_string_receive(temp);
// 		_delay_ms(50);
	}
	return 0;
	
}