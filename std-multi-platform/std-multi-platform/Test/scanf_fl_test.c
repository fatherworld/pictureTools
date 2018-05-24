#include"Config.h"
#include"stdio_fl.h"
int main()
{
char  a[5];
char  b[5];
float d;

while (1)
{
	//printf_fl("请输入两个字符串数据:\n");
	//scanf_fl("%s %s",SafeParam(a, 5), SafeParam(b, 5));
	//printf_fl("a=%s\nd=%s\n",a,b);

	printf_fl("请输入字符串和浮点数据:\n");
	scanf_fl("%s%f",SafeParam(a, 5), &d);
	printf_fl("a=%s\nd=%f\n",a,d);

}
return 0;
}