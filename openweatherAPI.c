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

char UART2_string_receive(char *str)
{
	int i;
	char data = UART1_receive();
	for(i = 0; data != '\n'; i++)     //\n이 들어올 때까지의 문자를 data에 저장
	{
		str[i] = data;
		data = UART1_receive();
	}
	return data;
}

void jsonParsing(char* json){

	char *sArr[100] = {NULL, };
	int i = 0;
	
	char *ptr = strtok(*json, "\"");
	while (ptr != NULL)            // " 문자열이 나오지 않을 때까지 반복
	{
		sArr[i] = ptr;             // 문자열을 자른 뒤 메모리 주소를 문자열 포인터 배열에 저장
		i++;                       // 인덱스 증가

		ptr = strtok(NULL, "\"");   // 다음 문자열을 잘라서 포인터를 반환
	}

	for (int i = 0; i < 10; i++)
	{
		if (sArr[i] != NULL)           // 문자열 포인터 배열의 요소가 NULL이 아닐 때만
		printf("%s\n", sArr[i]);   // 문자열 포인터 배열에 인덱스로 접근하여 각 문자열 출력
	}

}

int main(void)
{
	char temp[800];
//	char weatherdata[800];
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
 	
 	UART1_string_transmit("AT+CWJAP=\"TP-Link_B9D0\",\"06312275\"\r\n");  //wifi 연결
	UART1_string_receive(temp);
	_delay_ms(5000);
 	
 	UART1_string_transmit("AT+CIPSTART=\"TCP\",\"api.openweathermap.org\",80\r\n");   // api 요청, 형식:<type><remoteIP><remote port>
 	UART1_string_receive(temp);
	 _delay_ms(5000);
	 
 	UART1_string_transmit("AT+CIPSEND=138\r\n"); //보내는 문자 길이 133
	UART1_string_receive(temp);
	_delay_ms(5000);
 	
	UART1_string_transmit("GET /data/2.5/weather?id=1835848&units=metric&APPID=b154f4f2fefe7862a10432af6e2b4dbd\r\n"); //133
	UART1_string_transmit("Host: api.openweathermap.org\r\n"); 
	UART1_string_transmit("User-Agent: ArduinoWiFi/1.1\r\n");
	UART1_string_transmit("Connection: close\r\n\r\n"); //138
	UART1_string_receive(temp);
	_delay_ms(5000);
 	
	jsonParsing(UART2_string_receive(temp));
	
	
	while(1)
	{
// 		UART1_string_receive(temp);
// 		_delay_ms(50);
	}
	return 0;
	
}
