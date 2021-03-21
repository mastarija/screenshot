#include <stdio.h>
#include "WinShot.c"

void PrintScreen( Screen s )
{
  printf( "screen.x : %d\n" , s.x );
  printf( "screen.y : %d\n" , s.y );
  printf( "screen.w : %d\n" , s.w );
  printf( "screen.h : %d\n" , s.h );
}

int main ()
{
  ScreenList* list = ss_ScreenList();

  if ( !list )
  {
    return 1;
  }

  Screen* data = ( Screen* ) list->data;

  for ( int i = 0; i < list->size; i++ )
  {
    char name[] = "Test0.bmp";
         name[ 4 ] = i + '0';

    PrintScreen( data[ i ] );
    printf( "\n" );

    ScreenShot* screenShot = ss_ScreenShot( data[ i ] );

    FILE* handle = fopen( name , "wb" );

    fwrite( screenShot->data , screenShot->size , 1 , handle );

    fclose( handle );
  }

  return 0;
}

