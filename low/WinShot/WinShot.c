#include "WinShot.h"

ScreenList* ss_ScreenList ()
{
  size_t      size = 0;
  ScreenList* list = NULL;

  EnumDisplayMonitors( NULL , NULL , ss_ScreenEnum , ( LPARAM ) &size );

  if ( !size )
  {
    return NULL;
  }

  list = malloc( sizeof ( ScreenList ) + ( size * sizeof ( Screen ) ) );

  if ( !list )
  {
    return NULL;
  }

  list->size = 0;

  EnumDisplayMonitors( NULL , NULL , ss_ScreenItem , ( LPARAM ) list );

  return list;
}

ScreenShot* ss_ScreenShot ( Screen screen , Bounds bounds )
{
  return NULL;
}

BOOL ss_ScreenEnum ( HMONITOR Arg1 , HDC Arg2 , LPRECT Arg3 , LPARAM Arg4 )
{
  unsigned long* size = ( unsigned long* ) Arg4;
  (*size)++;
  return TRUE;
}

BOOL ss_ScreenItem ( HMONITOR Arg1 , HDC Arg2 , LPRECT Arg3 , LPARAM Arg4 )
{
  long l = Arg3->left;
  long r = Arg3->right;
  long t = Arg3->top;
  long b = Arg3->bottom;

  ScreenList* list = ( ScreenList* ) Arg4;
  size_t      indx = list->size++;
  Screen*     data = ( Screen* ) list->data;
  Screen*     item = &data[ indx ];

  item->marker   = indx;
  item->bounds.x = l;
  item->bounds.y = t;
  item->bounds.w = r > l ? r - l : l - r;
  item->bounds.h = b > t ? b - t : t - b;

  return TRUE;
}
