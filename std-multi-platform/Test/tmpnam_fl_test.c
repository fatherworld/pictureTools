#include "Config.h"
#include "stdio_fl.h"
#include <process.h> 

int main()
{
	char tmpname[L_tmpnam] ;
    char *filename;
    FILE *tmpfp;
	int err;
    tmpnam_fl(tmpname,L_tmpnam);
    printf_fl("Temporary file name is: %s\n", tmpname);
    tmpfile_fl(&tmpfp);
    if(tmpfp)
	{
        printf("Opened a temporary file OK\n");
	}
    else
	{
        printf("tmpfile");
	}
  
	getchar();
	return 0;
}