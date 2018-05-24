#include  "Config.h"
#include <process.h>
#include "stdio_fl.h"
/*fprintf和printf_s的区别就在于printf_s还会检查用户自定义的格式字符串是否合法*/
FILE *stream;  
int main( void )  
{  
   int    i   = 10;  
   double fp  = 1.5;  
   char   s[] = "hello world，hello china";  
   char   ss[]= "My world";
   char   c   = '\n';
   fopen_fl( &stream, "fprintf_s.txt", "w" );  
   fprintf_fl( stream, "%s%c%s\n",ss, c,s);
   fprintf_fl( stream, "%d\n", i );  
   fprintf_fl( stream, "%f\n", fp );  
   fclose( stream );  
   printf_fl("the file is writed  successfully!!\n");
   system( "type fprintf_s.txt" );  //将文件内容打印到屏幕上
   getchar();
   return 0;
}  