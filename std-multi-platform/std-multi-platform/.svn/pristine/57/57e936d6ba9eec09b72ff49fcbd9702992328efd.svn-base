#include "Config.h"
#include "stdio_fl.h"
#include "string.h"
int main(void)
{
    FILE*stream;
	int error;
    char msg[]="this is a test";
    char buf[20];
	fopen_fl(&stream,"a.txt","w+");
    if(stream == NULL)
    {
        fprintf_fl(stderr,"Cannot open outputfile.\n");
        return 0;
    }
    fwrite(msg,1,strlen(msg)+1,stream);
    fseek(stream,0,SEEK_SET);
    fread_fl(buf,strlen(msg)+1,20,1,stream);
    printf_fl("%s\n",buf);
    fclose(stream);
	getchar();
    return 0;
}