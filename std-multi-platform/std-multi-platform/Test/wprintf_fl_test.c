#include "Config.h"
#include "stdio_fl.h"


/*wprintf��wprintf_s�����������wprintfֻ�����ʽ�ַ����Ƿ�Ϊ�գ���wprintf_s�������û��Զ���ĸ�ʽ�ַ����Ƿ�Ϸ�*/
int main()
{
    wchar_t* test = L"Hello world,hello china";
    wchar_t* formatStr = L"%s\n";

	wprintf_fl(formatStr, test);
    wprintf_fl(formatStr, test);
	getchar();
}