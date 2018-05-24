
#include "Config.h"
#include "stdio_fl.h"
#include <stdarg.h>

void SYSTEM(const char * format, ...)
{
	char buff[4069];
	va_list list;
	va_start(list, format);
	vsnprintf_fl(buff, 4069,7, format, list);
	va_end(list);
	printf_fl("%s\n", buff);
}

int main()
{
	SYSTEM("%d_%s", 6, "abcdefghi");
	getchar();
	return 0;
}
