#include"Config.h"
#include"stdio_fl.h"
#include"string_fl.h"

int main()
{
	char dest[8];
	char *sour = "abcdefg";
	memset(&dest, 0, sizeof(dest));
	strncpy_fl(dest, 8, sour, 3);
	printf_fl("des = %s", dest);
	getchar();
	return 0;
}