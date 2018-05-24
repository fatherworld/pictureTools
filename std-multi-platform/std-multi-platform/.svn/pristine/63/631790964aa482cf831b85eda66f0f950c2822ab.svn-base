#include"Config.h"
#include"stdio_fl.h"
#include"stdarg.h"


int my_printf( const wchar_t *format,...)
{
	va_list ap;
	int retval;
	va_start(ap,format);
	printf_fl("my_printf():");
	retval = vwprintf_fl(format,ap);
	va_end(ap);
	return retval;
}
main()
{
	int i = 150,j = -100;
	double k = 3.14159;
	my_printf(L"%d %f %x\n",j,k,i);
	my_printf(L"%2d %*d\n",i,2,i);
	getchar();
	return 0;
}