
#include "Config.h"
#include <process.h>
#include "stdio_fl.h"
FILE *stream;  
int main( void )  
{  
   int    i = 10;  
   double fp = 1.5;  
   wchar_t  s[] = L"hello world , hello china";  
   wchar_t   c = L'\n';  
  
   fopen_fl( &stream, "fwprintf_s.txt", "w" );  
   fwprintf_fl( stream, L"%s%c", s, c );  
   fwprintf_fl( stream, L"%d\n", i );  
   fwprintf_fl( stream, L"%f\n", fp );  
   fclose( stream );  
   printf_fl("the file is writed  successfully!!\n");
   system( "type fwprintf_s.txt" );  //将文件内容打印到屏幕上
   getchar();
   return 0;
} 