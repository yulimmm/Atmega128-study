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

void str_clean(char *str)
{
	for(int i = 0; str[i] != '\0'; i++)  //str[i] = 0이 아닌 동안
	str[i] = '\0';       //str[i] = 0
}

void readJSON(char* tmp){
	int count = 0;
	UART1_string_transmit("GET /data/2.5/weather?id=1835848&units=metric&APPID=b154f4f2fefe7862a10432af6e2b4dbd\r\n"); 
	UART1_string_transmit("Host: api.openweathermap.org\r\n");
	UART1_string_transmit("User-Agent: ArduinoWiFi/1.1\r\n");
	UART1_string_transmit("Connection: close\r\n\r\n"); //138
	
	char data = UART1_receive();
	int i = 0;
	while(i < 1000){
		if(i >= 4){
			tmp[0] = tmp[1];
			tmp[1] = tmp[2];
			tmp[2] = tmp[3];
			tmp[3] = data;
			data = UART1_receive();
		}
		else{
			tmp[i] = data;
			data = UART1_receive();
		}
		if(tmp[0] == 'm' && tmp[1] == 'a' && tmp[2] == 'i' && tmp[3] == 'n'){
			while(1)
			{
				count++;
				tmp[0] = tmp[1];
				tmp[1] = tmp[2];
				tmp[2] = tmp[3];
				tmp[3] = data;
				if(count == 10)	return;
				data = UART1_receive();
				i++;
				
			}
		}
		i++;
	}
}

int main(void)
{
	DDRB = 0xFF; // 0b11111111
	char temp[800];
	char weather[5];
	
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
	
	UART1_string_transmit("AT+CWJAP=\"와이파이 이름\",\"비밀번호\"\r\n");  //wifi 연결
	UART1_string_receive(temp);
	_delay_ms(5000);
	
	str_clean(weather);
	
	UART1_string_transmit("AT+CIPSTART=\"TCP\",\"api.openweathermap.org\",80\r\n");   // api 요청, 형식:<type><remoteIP><remote port>
	UART1_string_receive(temp);
	_delay_ms(5000);
	
	UART1_string_transmit("AT+CIPSEND=138\r\n"); //보내는 문자 길이 138
	UART1_string_receive(temp);
	_delay_ms(5000);
	
	while(1)
	{
		readJSON(weather);
		if((weather[0] == 'R' && weather[1] == 'a' && weather[2] == 'i' && weather[3] == 'n') || (weather[0] == 'S' && weather[1] == 'n' && weather[2] == 'o' && weather[3] == 'w') ){
			PORTB = 0b11111111;
			_delay_ms(200);
			break;
		}
		else{
			PORTB = 0b10101010;
			_delay_ms(500);
			break;

		}
	}
	return 0;
	
}
