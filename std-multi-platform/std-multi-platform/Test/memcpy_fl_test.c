#include "Config.h"
#include "stdio_fl.h"
#include "memory_fl.h"

int main()
{
	char dest[7];
	char *sour = "abcdefg";
	memset(&dest,0,sizeof(dest));
	memcpy_fl(dest, 7, sour, 3);
	printf_fl("des = %s", dest);
	getchar();
	return 0;
}