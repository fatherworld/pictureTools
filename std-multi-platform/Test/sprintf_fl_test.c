#include "Config.h"
#include <malloc.h>
#include "stdio_fl.h"

int main()
{
	char *str1= "hello ";
	char *str2= "world ";
	char *str3= "hello china";
	char* buffer = (char*)malloc(sizeof(char) * 50);
	sprintf_fl(buffer,50,"%s%s%s", str1, str2, str3);
	printf_fl("%s", buffer);

	free(buffer);
	getchar();

	return 0;
}
