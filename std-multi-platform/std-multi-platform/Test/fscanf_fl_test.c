#include "Config.h"
#include <stdlib.h>
#include "string.h"
#include "stdio_fl.h"
/*如果是字符串的话，用带_s的函数时，必须指定缓存大小*/

int main()
{
    int num;
    char name[10];
	char name1[10];
    FILE *fp;

    fopen_fl(&fp,"b.txt", "r+");//“r+” 以可读写方式打开文件
    if (fp == NULL)
    {
        printf_fl("Cannot open the file!\n");
        exit(0);
    }
    printf_fl("学号 姓名\n");
	//fscanf_fl(fp, "%d%s", &num, SafeParam(name, 10));
	//printf("%d %s ", num, name);

	fscanf_fl(fp, "%s%s",SafeParam(name, 10),SafeParam(name1, 10));
	printf_fl("%s %s ", name, name1);

    fclose(fp);//要记得关闭文件
	getchar();
    return 0;
}

