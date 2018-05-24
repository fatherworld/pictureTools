#include"Config.h"
#include"string_fl.h"
#include"stdio_fl.h"
int main()
{
	char dest[100]="hello";
	char *sour = "world";
	strcat_fl(dest, 100, sour);
	printf_fl("des = %s", dest);
	getchar();
	return 0;
}