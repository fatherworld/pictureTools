#include"Config.h"
#include"stdio_fl.h"
#include<stdlib.h>
/*������ַ����� ��_s�ı���ָ���仺���С*/
int main()
{
      char input[ ]="aaa ddd ccc";
	  char input1[ ]="aaa 123 ddd";
	  char i[10];
      char s[10]; 
	  char ss[10];
	  int b ;
	  sscanf_fl(input,"%s%s%s",SafeParam(i,10),SafeParam(s,10),SafeParam(ss,10));//ÿ���ַ��ַ�����С�ǻ�������С
	  printf_fl("%s %s %s\n",i,s,ss);

	  sscanf_fl(input1,"%s%d%s",SafeParam(i,10), &b , SafeParam(ss,10));
      printf_fl("%s %d %s",i,b,ss);
	  getchar();

	return 0;
}
