#include "Config.h"
#include <stdio.h>
#include"stdio_fl.h"
/*printf和printf_s的区别就在于printf只会检查格式字符串是否为空，而printf_s还会检查用户自定义的格式字符串是否合法*/
void func(int i) {
    printf_fl("%s\n", __FUNCTION__);//__FUNCTION__是获取函数名的
}
int main()
{
    char* test = "Hello world,hello china";
    char* formatStr = "%s\n";
	int i = 8;
	func(i);
	printf_fl(formatStr, test);
    printf_fl(formatStr, test);
	getchar();
}