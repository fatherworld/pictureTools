#include "Config.h"
#include "stdio_fl.h"
#include <process.h> 

int main(void) 
{ 
   FILE *tempfp; 
  // //
  tmpfile_fl(&tempfp);
  if (tempfp!=NULL) 
      printf_fl("Temporary file created\n"); 
   else 
   { 
      printf_fl("Unable to create temporary file\n"); 
      exit(1); 
   } 

   getchar();
   return 0; 
} 
