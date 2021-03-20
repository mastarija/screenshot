#include <stdlib.h>

#ifndef ALL_SHOT
#define ALL_SHOT

typedef struct
{
  long x;
  long y;
  unsigned long w;
  unsigned long h;
} Screen ;

typedef struct
{
  size_t size;
  Screen data[];
} ScreenList;

typedef struct
{
  size_t        size;
  unsigned char data[];
} ScreenShot;

ScreenList* ss_ScreenList ();
ScreenShot* ss_ScreenShot ( Screen screen );

#endif
