#include"Config.h"
#include"stdio_fl.h"
#include<stdlib.h>
/*������ַ����� ��_s�ı���ָ���仺���С*/
int main()
{
   wchar_t  string[] = L"69 70 65";
   wchar_t  s[80];
   wchar_t  c;
   float    fp;
   swscanf_fl( string, L"%s", SafeParam(s, 80));
 
   swscanf_fl( string, L"%c", SafeParam(&c,2));
   swscanf_fl( string, L"%f", &fp);

   wprintf_fl(L"String    = %s\n", s );
   wprintf_fl(L"Character = %c\n", c );
   wprintf_fl(L"Real:     = %f\n", fp );


   getchar();
	return 0;
}