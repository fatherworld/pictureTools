#include "Config.h"
#include "stdio_fl.h"
#include <stdlib.h>
#include "stdarg.h"




void PrintFormatted (FILE *stream, wchar_t *format, ...)
{ 
va_list args;
va_start (args, format);
vfwprintf_fl(stream, format, args);
va_end (args);
}
int main()
{
	FILE * fp;
	fp=fopen ("myfile.txt","w");
	if(!fp)
	{
		printf_fl("cannot open the file.\n");
		exit(-1); 
	}
	PrintFormatted (fp,L"name:%s.\n",L"Zhangxiaofeng");
	PrintFormatted (fp,L"chinese:%d.\n",99);
	PrintFormatted (fp,L"math:%f.\n",85.0); 
	fclose (fp);  
	printf_fl("文件写入成功.\n");
	getchar();
	return 0;
}