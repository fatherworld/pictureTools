#include"Config.h"
#include"stdio_fl.h"
#include"string_fl.h"


int main()
{
	char ss[50]  ="hello,world!hello,china!";
	char *delim = ",!";
	char *p = NULL;
	char *t = NULL;
	printf_fl("%s ",strtok_fl(ss, delim, &t));
	while ((p = strtok_fl(NULL, delim, &t)))
	{
		 printf("%s ", p);
	}
	
	getchar();
	return 0;
}