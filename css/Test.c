#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include "Test.h"

int main()
{
  struct ScreenList* list = ScreenList();

  printf("size : %d\n", list->count );

  for ( int i = 0 ; i < list->count ; i++ )
  {
    struct Screen s = list->items[ i ];
    printf( "screen %d : <x , y> ( %d , %d ) - <w , h> ( %d , %d )\n" , i , s.x , s.y , s.w , s.h );
  }

  struct Bitmap* bitmap = ScreenShot( 0 , 0 , 1366 , 768 );

  if( bitmap == NULL )
    return 1;

  FILE* fh = fopen( "screenshot.bmp" , "wb" );

  fwrite( bitmap->image , 1 , bitmap->bytes , fh );

  fclose( fh );

  free( bitmap->image );
  free( bitmap );

  return 0;
}

////////////////////////////////////////////////////////////////////////////////

// get the list of detected screens
struct ScreenList* ScreenList()
{
  unsigned char       count = 0;    // size of the list
  struct ScreenList*  items = NULL; // list of detected screens

  // run through detected screens increasing the 'count' of the list
  EnumDisplayMonitors( NULL , NULL , ScreenCounter , (LPARAM) &count );

  // allocate enough memory to store all of the detected screens' data
  items = malloc( sizeof(*items) + count * sizeof(struct Screen) );

  // return null if memory allocation fails for some reason
  if ( items == NULL ) return NULL;

  // prepare the screen list size value for the 'ScreenFetcher' function
  items->count = 0;

  // load position and size of each monitor into the list
  EnumDisplayMonitors( NULL , NULL , ScreenFetcher , (LPARAM) items );

  // return the pointer to the 'ScreenList'
  return items;
}

//

struct Bitmap* ScreenShot( int x , int y , int w , int h )
{
  struct Bitmap*    result            = NULL;

  HDC               hScreen           = GetDC( NULL );
  HDC               hScreenMemory     = CreateCompatibleDC( hScreen );

  HBITMAP           hBitmap           = CreateCompatibleBitmap( hScreen , w , h );
  HBITMAP           hBitmapOld        = (HBITMAP) SelectObject( hScreenMemory , hBitmap );

  DWORD             nColors           = 0;

  DWORD             sBitmapInfo       = sizeof( BITMAPINFOHEADER );
  DWORD             sPixelBuffer      = 0;    // byte size of PIXEL buffer
  DWORD             sBitmapFileHeader = sizeof( BITMAPFILEHEADER );

  BYTE*             pPixelBuffer      = NULL;
  BYTE*             pBitmapBuffer     = NULL;

  BITMAPINFO*       pBitmapInfo       = malloc( sBitmapInfo );
  BITMAPFILEHEADER* pBitmapFileHeader = malloc( sBitmapFileHeader );

  //--------------------------------------------------------------------------//

  BitBlt( hScreenMemory , 0 , 0 , w , h , hScreen , x , y , SRCCOPY );

  //--------------------------------------------------------------------------//

  // return null if the allocation for pBitmapInfo and pBitmapFileHeader failed
  if( ( pBitmapInfo == NULL ) || ( pBitmapFileHeader == NULL ) )
    goto EXIT01;

  // store the BITMAPINFOHEADER data into the BITMAPINFO structure
  pBitmapInfo->bmiHeader.biSize = sBitmapInfo;
  if( !GetDIBits( hScreenMemory, hBitmap, 0, 0, NULL, pBitmapInfo, DIB_RGB_COLORS ) )
    goto EXIT01;

  // calculate the number of additional bytes to allocate for the Color Table
  nColors       = ColorTableSize( &pBitmapInfo->bmiHeader );
  sPixelBuffer  = pBitmapInfo->bmiHeader.biSizeImage;

  // calculate the size of BITMAPINFO data structure
  if ( nColors > 0 )
    sBitmapInfo = sBitmapInfo + sizeof( RGBQUAD ) * nColors;

  // reallocate the BITMAPINFO data structure in case there is a Color Table
  if ( nColors > 0 )
    pBitmapInfo = realloc( pBitmapInfo, sBitmapInfo );

  // return null if reallocation failed for some reason
  if ( pBitmapInfo == NULL )
    goto EXIT01;

  // allocate enough bytes for storing pixels
  pPixelBuffer = malloc( sPixelBuffer );
  if ( pPixelBuffer == NULL )
    goto EXIT01;

  // get full BITMAPINFO structure this time and return NULL if it fails
  if( !GetDIBits( hScreenMemory , hBitmap , 0 , pBitmapInfo->bmiHeader.biHeight , pPixelBuffer , pBitmapInfo , DIB_RGB_COLORS ) )
    goto EXIT02;

  // initialize the BITMAPFILEHEADER
  pBitmapFileHeader->bfType      = 0x4D42; // 0x4D B, 0x42 M : BM ( BitMap )
  pBitmapFileHeader->bfSize      = sBitmapFileHeader + sBitmapInfo + sPixelBuffer;
  pBitmapFileHeader->bfReserved1 = 0;
  pBitmapFileHeader->bfReserved2 = 0;
  pBitmapFileHeader->bfOffBits   = sBitmapFileHeader + sBitmapInfo;

  // allocate enough space to keep the complete BMP file in memory
  pBitmapBuffer = malloc( sBitmapFileHeader + sBitmapInfo + sPixelBuffer );
  if ( pBitmapBuffer == NULL )
    goto EXIT02;

  // copy BITMAPFILEHEADER into the bitmap buffer
  memcpy( pBitmapBuffer , pBitmapFileHeader , sBitmapFileHeader );
  // copy BITMAPINFO into the bitmap buffer
  memcpy( pBitmapBuffer + sBitmapFileHeader , pBitmapInfo , sBitmapInfo );
  // copy PIXELS into the bitmap buffer
  memcpy( pBitmapBuffer + sBitmapFileHeader + sBitmapInfo , pPixelBuffer , sPixelBuffer );

  // allocate enough memory for BMP structure
  result = malloc( sizeof( *result ) );
  if ( result == NULL )
    goto EXIT03;

  // insert data into resulting Bitmap structure
  result->image = pBitmapBuffer;
  result->bytes = sBitmapFileHeader + sBitmapInfo + sPixelBuffer;

  goto DONE;

  // memory cleanup
  EXIT03:
    free( pBitmapBuffer );
  DONE:
  EXIT02:
    free( pPixelBuffer );
  EXIT01:
    free( pBitmapInfo );
    free( pBitmapFileHeader );
    DeleteDC( hScreenMemory );
    ReleaseDC( NULL, hScreen );
    DeleteObject( hBitmapOld );

  // return the pointer to the resulting Bitmap
  return result;
}

////////////////////////////////////////////////////////////////////////////////

// increase the value on 'aSize' address
BOOL ScreenCounter( HMONITOR hMonitor, HDC hDC, LPRECT pRect, LPARAM aSize )
{
  int* pSize = (int*) aSize;  // cast the address into a pointer
  (*pSize)++;                 // increase size through a pointer
  return TRUE;                // continue iterating over screens
}

// fetch width , height and position of a virtual screen
BOOL ScreenFetcher( HMONITOR hMonitor , HDC hDC , LPRECT pRect , LPARAM aScreenList )
{
  struct ScreenList* list = (struct ScreenList*) aScreenList; // cast the address into a pointer
  int                indx = list->count;                      // get the current index / size

  MONITORINFO info;                              // monitor info temp var
              info.cbSize = sizeof(MONITORINFO); // needed for G...M...InfoA

  // get the screen info
  GetMonitorInfoA( hMonitor , &info );

  // store relevant screen info into the list at index 'indx'
  list->items[indx].x = (int) info.rcMonitor.left;
  list->items[indx].y = (int) info.rcMonitor.top;
  list->items[indx].w = (int) abs( info.rcMonitor.left - info.rcMonitor.right  );
  list->items[indx].h = (int) abs( info.rcMonitor.top  - info.rcMonitor.bottom );

  list->count++; // increase the list index / count
  return TRUE;   // continue iterating over screens
}

DWORD ColorTableSize( BITMAPINFOHEADER* h ) {
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
