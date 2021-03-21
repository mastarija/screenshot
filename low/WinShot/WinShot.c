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
    goto EXIT01;
  }

  hBitmap = CreateCompatibleBitmap( hScreen , s.w , s.h );

  if ( !hBitmap )
  {
    goto EXIT02;
  }

  hBitmapOld = ( HBITMAP ) SelectObject( hScreenMemory , hBitmap );

  if ( !hBitmapOld || hBitmapOld == HGDI_ERROR )
  {
    goto EXIT03;
  }

  //

  if ( !BitBlt( hScreenMemory , 0 , 0 , s.w , s.h , hScreen , s.x , s.y , SRCCOPY ) )
  {
    goto EXIT04;
  }

  //

  pBitmapInfo = malloc( sBitmapInfo );

  if ( !pBitmapInfo )
  {
    goto EXIT04;
  }

  pBitmapFileHeader = malloc( sBitmapFileHeader );

  if ( !pBitmapFileHeader )
  {
    goto EXIT05;
  }

  pBitmapInfo->bmiHeader.biSize = sBitmapInfo;

  if ( !GetDIBits( hScreenMemory , hBitmap , 0 , 0 , NULL , pBitmapInfo , DIB_RGB_COLORS ) )
  {
    goto EXIT06;
  }

  nColors = ss_ColourSize( &pBitmapInfo->bmiHeader );
  sPixelBuffer = pBitmapInfo->bmiHeader.biSizeImage;

  if ( nColors > 0 )
  {
    BITMAPINFO* temp = NULL;

    sBitmapInfo = sBitmapInfo + sizeof ( RGBQUAD ) * nColors;

    temp = realloc( pBitmapInfo , sBitmapInfo );

    if ( !temp )
    {
      goto EXIT06;
    }

    pBitmapInfo = temp;
  }

  pPixelBuffer = malloc( sPixelBuffer );

  if ( !pPixelBuffer )
  {
    goto EXIT06;
  }

  if ( !GetDIBits( hScreenMemory , hBitmap , 0 , pBitmapInfo->bmiHeader.biHeight , pPixelBuffer , pBitmapInfo , DIB_RGB_COLORS ) )
  {
    goto EXIT07;
  }

  pBitmapFileHeader->bfType = 0x4D42;
  pBitmapFileHeader->bfSize = sBitmapFileHeader + sBitmapInfo + sPixelBuffer;
  pBitmapFileHeader->bfReserved1 = 0;
  pBitmapFileHeader->bfReserved2 = 0;
  pBitmapFileHeader->bfOffBits   = sBitmapFileHeader + sBitmapInfo;

  pBitmapBuffer = malloc( pBitmapFileHeader->bfSize );

  if ( !pBitmapBuffer )
  {
    goto EXIT07;
  }

  memcpy( pBitmapBuffer , pBitmapFileHeader , sBitmapFileHeader );

  memcpy( pBitmapBuffer + sBitmapFileHeader , pBitmapInfo , sBitmapInfo );

  memcpy( pBitmapBuffer + sBitmapFileHeader + sBitmapInfo , pPixelBuffer , sPixelBuffer );

  result = malloc( sizeof ( *result ) + pBitmapFileHeader->bfSize );

  if ( !result )
  {
    goto EXIT08;
  }

  result->size = pBitmapFileHeader->bfSize;

  memcpy( result->data , pBitmapBuffer , pBitmapFileHeader->bfSize );

  //

  EXIT08:
    free( pBitmapBuffer );
  EXIT07:
    free( pPixelBuffer );
  EXIT06:
    free( pBitmapFileHeader );
  EXIT05:
    free( pBitmapInfo );
  EXIT04:
    SelectObject( hScreenMemory , hBitmapOld );
  EXIT03:
    DeleteObject( hBitmap );
  EXIT02:
    DeleteDC( hScreenMemory );
  EXIT01:
    ReleaseDC( NULL , hScreen );
  EXIT00:
    return result;
}

DWORD ss_ColourSize( BITMAPINFOHEADER* h ) {
  DWORD result        = 0;

  DWORD biClrUsed     = h->biClrUsed;
  WORD  biBitCount    = h->biBitCount;
  DWORD biCompression = h->biCompression;

  switch ( biBitCount ) {
    case 24:
      result = biClrUsed;
      break;
    case 16:
    case 32:
      if ( biCompression == BI_RGB )
        result = biClrUsed;
      else if ( biCompression == BI_BITFIELDS )
        result = 3;
      break;
    default: // for 0, 1, 2, 4, and 8
      if ( biClrUsed == 0 )
        result = ( 1 << biBitCount ); // 2^biBitCount
      else
        result = biClrUsed;
      break;
  }

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
