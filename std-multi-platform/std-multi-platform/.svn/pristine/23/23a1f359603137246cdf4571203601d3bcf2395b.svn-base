#include "Config.h"
#include "stdio_fl.h"
#include "stdarg.h"



void PrintFormatted (FILE *stream, char *format, ...)
{ 
va_list args;
va_start (args, format);
vfprintf_fl (stream, format, args);
va_end (args);
}
int main()
{
	FILE * fp;
	fp=fopen ("myfile.txt","w");
	if(!fp)
	{
		printf_fl("cannot open the file.\n");
		return(-1); 
	}
	PrintFormatted (fp,"name:%s.\n","张晓峰");
	PrintFormatted (fp,"chinese:%d.\n",87);
	PrintFormatted (fp,"math:%f.\n",91.5); 
	fclose (fp);  
	printf_fl("文件写入成功.\n");
	getchar();
	return 0;
}
