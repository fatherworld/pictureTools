#include "Config.h"
#include "stdio_fl.h"


/*wprintf和wprintf_s的区别就在于wprintf只会检查格式字符串是否为空，而wprintf_s还会检查用户自定义的格式字符串是否合法*/
int main()
{
    wchar_t* test = L"Hello world,hello china";
    wchar_t* formatStr = L"%s\n";

	wprintf_fl(formatStr, test);
    wprintf_fl(formatStr, test);
	getchar();
}