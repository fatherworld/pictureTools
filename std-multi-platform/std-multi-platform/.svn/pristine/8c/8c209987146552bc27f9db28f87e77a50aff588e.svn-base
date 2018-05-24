#include "Config.h"
#include "stdio_fl.h"
#include <malloc.h>


int main()
{
	wchar_t *str1= L"world";
	wchar_t *str2= L"china";
	wchar_t *buffer = (wchar_t*)malloc(sizeof(wchar_t) * 100);
	swprintf_fl(buffer,100, L"%s%s", str1, str2);
	wprintf_fl(L"%s", buffer);

	free(buffer);
	getchar();

	return 0;
}