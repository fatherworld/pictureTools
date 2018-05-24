#include "Config.h"
#include"stdio_fl.h"
#include"string_fl.h"

int main()
{
	char dest[100]="hello";
	char *sour = "world";
	memmove_fl(dest, 100, sour,2);
	printf_fl("des = %s", dest);
	getchar();
	return 0;
}