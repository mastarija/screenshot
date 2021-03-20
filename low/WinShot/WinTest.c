#include <stdio.h>
#include "WinShot.c"

void PrintScreen( Screen s )
{
  printf( "marker : %d\n" , s.marker );
  printf( "marker.bounds.x : %d\n" , s.bounds.x );
  printf( "marker.bounds.y : %d\n" , s.bounds.y );
  printf( "marker.bounds.w : %d\n" , s.bounds.w );
  printf( "marker.bounds.h : %d\n" , s.bounds.h );
}

int main ()
{
  ScreenList* list = ss_ScreenList();

  // printf( "Hello world! %d" , list->size );

  Screen* data = ( Screen* ) list->data;

  PrintScreen( data[0] );

  return 0;
}

