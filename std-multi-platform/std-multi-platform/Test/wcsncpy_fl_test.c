#include"Config.h"
#include"stdio_fl.h"
#include"string_fl.h"

int main()
{
	wchar_t dest[20];
	wchar_t *sour = L"hello world !";
	memset(&dest,0,sizeof(dest));
	wcsncpy_fl(dest, 12, sour, 11);;
	wprintf_fl(L"des = %s", dest);
	getchar();
	return 0;
}