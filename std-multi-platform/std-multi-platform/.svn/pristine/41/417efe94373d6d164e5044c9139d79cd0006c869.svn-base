#include "Config.h"
#include "stdio_fl.h" 
#include <stdlib.h>  
  
FILE *stream;  
  
int main( void )  
{  
   // Reassign "stderr" to "freopen.out":   
  freopen_fl( &stream, "freopen.txt", "w", stderr );  
  if( stream == NULL )  
      fprintf_fl( stdout, "error on freopen\n" );  
   else  
   {  
      fprintf_fl( stdout, "successfully reassigned\n" ); fflush( stdout );  
      fprintf_fl( stream, "This will go to the file 'freopen.txt'\n" );  
      fclose( stream );  
   }  
   system( "type freopen.txt" );  
   getchar();
}