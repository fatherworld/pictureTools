#include "Config.h"
#include <stdlib.h>
#include "string.h"
#include "stdio_fl.h"
/*������ַ����Ļ����ô�_s�ĺ���ʱ������ָ�������С*/

int main()
{
    int num;
    char name[10];
	char name1[10];
    FILE *fp;

    fopen_fl(&fp,"b.txt", "r+");//��r+�� �Կɶ�д��ʽ���ļ�
    if (fp == NULL)
    {
        printf_fl("Cannot open the file!\n");
        exit(0);
    }
    printf_fl("ѧ�� ����\n");
	//fscanf_fl(fp, "%d%s", &num, SafeParam(name, 10));
	//printf("%d %s ", num, name);

	fscanf_fl(fp, "%s%s",SafeParam(name, 10),SafeParam(name1, 10));
	printf_fl("%s %s ", name, name1);

    fclose(fp);//Ҫ�ǵùر��ļ�
	getchar();
    return 0;
}

