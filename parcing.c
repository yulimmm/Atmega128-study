//비주얼스튜디오로 잘 돌아가는지 확인용 코드 

#include <stdio.h>
#include <string.h>


void readJSON(char* tmp) {
	int count = 0;
	char json[] = "0, \"main\" : \"rain\", \"description\" : \"clear sky\", \"icon\" : \"01d\"}]";
	int i = 0;
	char data = json[i];

	while (i < 1000) {
		if (i >= 4) {
			tmp[0] = tmp[1];
			tmp[1] = tmp[2];
			tmp[2] = tmp[3];
			tmp[3] = data;
			data = json[i];
		}
		else {
			tmp[i] = data;
			data = json[i];
		}
		if (tmp[0] == 'm' && tmp[1] == 'a' && tmp[2] == 'i' && tmp[3] == 'n') {
			while (1)
			{
				count++;
				tmp[0] = tmp[1];
				tmp[1] = tmp[2];
				tmp[2] = tmp[3];
				tmp[3] = data;
				if (count == 10)	return;
				data = json[i];
				i++;
			}
		}
		i++;
	}
}

int main() {
	while (1)
	{
		char weather[5];
		readJSON(weather);
		if ((weather[0] == 'r' && weather[1] == 'a' && weather[2] == 'i' && weather[3] == 'n') || (weather[0] == 'S' && weather[1] == 'n' && weather[2] == 'o' && weather[3] == 'w')) {
			printf("rain or snow");
			break;
		}
		else {
			printf("good");
			break;
		}
	}
	return 0;
}
