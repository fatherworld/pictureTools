
#include "Config.h"
#include <stdlib.h>
#include "string.h"
#include "stdio_fl.h"
int main()
{
    int num;
    wchar_t name[10];
	wchar_t name1[10];
    FILE *fp;


    fopen_fl( &fp,"b.txt", "r+");//“r+” 以可读写方式打开文件
    if (fp == NULL)
    {
        printf_fl("Cannot open the file!\n");
        exit(0);
    }
    printf_fl("学号 姓名\n");		
	//fwscanf_fl(fp, L"%d%s", &num, SafeParam(name,10));
	//wprintf(L"%d %s ", num, name);

	fwscanf_fl(fp, L"%s%s",SafeParam(name,10),SafeParam(name1, 10));
	wprintf_fl(L"%s %s ", name, name1);

    fclose(fp);//要记得关闭文件
	getchar();
    return 0;
}