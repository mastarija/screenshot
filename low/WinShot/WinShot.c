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

ScreenShot* ss_ScreenShot ( Screen s )
{
  ScreenShot*       result            = NULL;

  HDC               hScreen           = NULL;
  HDC               hScreenMemory     = NULL;

  HBITMAP           hBitmap           = NULL;
  HBITMAP           hBitmapOld        = NULL;

  DWORD             nColors           = 0;

  DWORD             sBitmapInfo       = sizeof ( BITMAPINFOHEADER );
  DWORD             sPixelBuffer      = 0;
  DWORD             sBitmapFileHeader = sizeof ( BITMAPFILEHEADER );

  BYTE*             pPixelBuffer      = NULL;
  BYTE*             pBitmapBuffer     = NULL;

  BITMAPINFO*       pBitmapInfo       = NULL;
  BITMAPFILEHEADER* pBitmapFileHeader = NULL;

  //

  hScreen = GetDC( NULL );

  if ( !hScreen )
  {
    goto EXIT00;
  }

  hScreenMemory = CreateCompatibleDC( hScreen );

  if ( !hScreenMemory )
  {
    goto EXIT00;
  }

  hBitmap = CreateCompatibleBitmap( hScreen , s.w , s.h );

  if ( !hBitmap )
  {
    goto EXIT01;
  }

  hBitmapOld = ( HBITMAP ) SelectObject( hScreenMemory , hBitmap );

  if ( !hBitmapOld || hBitmapOld == HGDI_ERROR )
  {
    goto EXIT02;
  }

  //

  if ( !BitBlt( hScreenMemory , 0 , 0 , s.w , s.h , hScreen , s.x , s.y , SRCCOPY ) )
  {
    goto EXIT03;
  }

  //



  //

  EXIT03:
    DeleteObject( hBitmapOld );
  EXIT02:
    DeleteObject( hBitmap ); // TODO : check if I should be doing this
  EXIT01:
    DeleteDC( hScreenMemory );
  EXIT00:
    return result;
}

BOOL ss_ScreenEnum ( HMONITOR Arg1 , HDC Arg2 , LPRECT Arg3 , LPARAM Arg4 )
{
  unsigned long* size = ( unsigned long* ) Arg4;
  (*size)++;
  return TRUE;
}

BOOL ss_ScreenItem ( HMONITOR Arg1 , HDC Arg2 , LPRECT Arg3 , LPARAM Arg4 )
{
  long
    l = Arg3->left  ,
    r = Arg3->right ,
    t = Arg3->top   ,
    b = Arg3->bottom;

  ScreenList* list = ( ScreenList* ) Arg4;
  Screen*     item = ( Screen* ) ( list->data + ( list->size++ ) );

  item->x = l;
  item->y = t;
  item->w = r > l ? r - l : l - r;
  item->h = b > t ? b - t : t - b;

  return TRUE;
}
