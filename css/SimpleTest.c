#include <stdlib.h>
#include <stdio.h>

struct Bitmap
  { void*         image;
    unsigned long bytes;
  };

int main()
{
  struct Bitmap* test = malloc( sizeof(struct Bitmap) );

  if( test == NULL )
    printf( "failure" );
  else
    printf( "success" );
  return ( test == NULL ) ? 1 : 0;
}
