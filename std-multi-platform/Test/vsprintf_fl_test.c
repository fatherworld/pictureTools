#include "Config.h"
#include "stdio_fl.h"
#include <conio.h> 
#include <stdarg.h>


char buffer[80]; 
int vspf(char *fmt, ...) 
{
	va_list argptr; 
    int cnt;
	va_start(argptr, fmt); 
    cnt = vsprintf_fl(buffer,80, fmt, argptr);  
    va_end(argptr);
	return(cnt); 
}
int main()
{
   int inumber = 30; 
   float fnumber = 90.0; 
   char string[4] = "abc"; 
   vspf("%d %f %s", inumber, fnumber, string); 
   printf_fl("%s/n", buffer); 
   getchar();
   return 0; 

}
