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
    PrintScreen( data[ i ] );
    printf( "\n" );
  }

  return 0;
}

