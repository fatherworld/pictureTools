#include "Config.h"
#include "stdio_fl.h"
/*���������ķ���ֵ���Ͳ�һ�����ж��ļ��Ƿ����fopen_s Ҫ���ݷ���ֵ�����ж�*/
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
