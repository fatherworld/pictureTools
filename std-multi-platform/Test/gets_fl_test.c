#include "Config.h"
#include "stdio_fl.h"

int main()
{
    char s[20];
    printf_fl("What's your name?\n");
    gets_fl(s, 20);                     
    printf_fl("My name is %s\n", s);
	getchar();
    return 0;    
}