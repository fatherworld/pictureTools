#include"Config.h"
#include"stdio_fl.h"
#include<stdlib.h>
/*如果是字符串， 带_s的必须指定其缓存大小*/
int main()
{
      char input[ ]="aaa ddd ccc";
	  char input1[ ]="aaa 123 ddd";
	  char i[10];
      char s[10]; 
	  char ss[10];
	  int b ;
	  sscanf_fl(input,"%s%s%s",SafeParam(i,10),SafeParam(s,10),SafeParam(ss,10));//每个字符字符串大小是缓存区大小
	  printf_fl("%s %s %s\n",i,s,ss);

	  sscanf_fl(input1,"%s%d%s",SafeParam(i,10), &b , SafeParam(ss,10));
      printf_fl("%s %d %s",i,b,ss);
	  getchar();

	return 0;
}
