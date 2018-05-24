#include"Config.h"
#include"stdio_fl.h"
#include"string_fl.h"

int main()
{
	wchar_t ss[50]  =L"hello,world!hello,china!";
	wchar_t *delim = L",!";
	wchar_t *p = NULL;
	wchar_t *t = NULL;
	wprintf_fl(L"%s ",wcstok_fl(ss, delim, &t));
	while ((p = wcstok_fl(NULL, delim, &t)))
	{
		 wprintf_fl(L"%s ", p);
	}
	getchar();
	return 0;
}