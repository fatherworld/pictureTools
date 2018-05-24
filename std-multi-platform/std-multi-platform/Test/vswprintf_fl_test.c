#include "Config.h"
#include "stdio_fl.h"
#include <stdarg.h>
#include <wchar.h>



void PrintWide ( const wchar_t * format, ... )
{
  wchar_t buffer[80];
  va_list args;
  va_start ( args, format );
  vswprintf_fl ( buffer, 80, format, args );
  fputws ( buffer, stdout );               // Êä³öµ½ÆÁÄ»
  va_end ( args );
}

int main ()
{
   wchar_t str[] = L"test string has %d wide characters.\n";
   PrintWide ( str, wcslen(str) );
   getchar();
   return 0;
} 