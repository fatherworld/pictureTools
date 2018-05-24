#include "Config.h"
#include "stdio_fl.h"
/*两个函数的返回值类型不一样，判断文件是否存在fopen_s 要根据返回值类型判断*/
int main()
{

	FILE  *fp = NULL;
	fopen_fl(&fp, "a.txt", "r");
	if(fp == NULL)
	{
		printf_fl("the file is not opened");
	}
	else{
		printf_fl("the file is  opened");
	}
	getchar();
	fclose(fp);
	return 0;
}
