#include "Config.h"
#include "stdio_fl.h"
int main()
{
wchar_t a[5];
wchar_t c[5];
float d;
while (1)
{
	wprintf_fl(L"Please input two strings:\n");
	wscanf_fl(L"%s%s", SafeParam(a, 5), SafeParam(c, 5));
	wprintf_fl(L"a=%s\nc=%s\n",a,c);

	//wprintf_fl(L"Please input one string and one float-type :\n");
	//wscanf_fl(L"%s%f", SafeParam(a, 5), &d);
	//wprintf(L"a=%s\n d=%f\n",a,d);
}
getchar();
return 0;
}